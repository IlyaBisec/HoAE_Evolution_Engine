/*****************************************************************
/*  File:   kResourceManager.cpp                                      
/*  Desc:   Resource manager implementation
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Nov 2004                                             
/*****************************************************************/
#include "stdafx.h"
#include "IResourceManager.h"
#include "kFilePath.h"
#include "kStaticArray.hpp"
#define  BOOST_WINDOWS
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"


/*****************************************************************/
/*  Struct:  MountEntry
/*  Desc:   Single mounting point for the resource storage
/*****************************************************************/
struct MountEntry
{
    std::string                 m_SourcePath;
    std::string                 m_MountPath;
    DataSourceType              m_SourceType;
}; // struct MountEntry

/*****************************************************************/
/*  Struct:  ResourceStream
/*  Desc:    
/*****************************************************************/
class ResourceStream : public MemInStream
{
    int                 m_ResID;
public:
    ResourceStream() : m_ResID(-1) {} 
    ResourceStream( int resID, BYTE* pData, int size ) : m_ResID( resID ), MemInStream( pData, size ) {} 
    virtual bool Close() { IRM->UnlockData( m_ResID ); return false; }
}; // class ResourceStream

/*****************************************************************/
/*  Struct:  ResourceInstance
/*  Desc:    Resource placeholder
/*****************************************************************/
struct ResourceInstance
{
    IResource*      m_pResource;
    int             m_MountEntry;
    
    std::string     m_Name;
    std::string     m_SourcePath;
    std::string     m_FullPath;

    int             m_ID;
    FILETIME        m_WriteTime;

    BYTE*           m_pData;
    int             m_DataSize;

    ResourceStream  m_ResStream;

    ResourceInstance() : m_pResource(0), m_MountEntry(-1), m_ID(-1), m_pData(NULL), m_DataSize(0) {}
}; // struct ResourceInstance

const int c_MaxResourceInstances = 16384;
/*****************************************************************/
/*  Class:  ResourceManager
/*  Desc:   Implementation of the resource manager interface
/*****************************************************************/
class ResourceManager : public IResourceManager
{
public:
                            ResourceManager ();
    virtual bool            MountDataSource (   const char* sourcePath,
                                                const char* mountName = NULL,  
                                                DataSourceType type = dstDirectory );
    
    virtual int             FindResource    ( const char* resName );
    virtual bool            BindResource    ( int resID, IResource* pRes );
    virtual void            UnbindResource  ( IResource* pRes );
	
	void					RegCallbackOnLockData(IResourceManager::OnLockData *);
	
	virtual BYTE*           LockData        ( int resID, int& size );
    virtual void            UnlockData      ( int resID );    
    virtual InStream&       LockResource    ( const char* resName, int size = 0 );

    virtual void            UpdateResources ();
    
    virtual const char*     GetPath         ( IResource* pRes );
    virtual const char*     GetPath         ( int resID );
    
    virtual const char*     GetName         ( IResource* pRes );
    virtual const char*     GetName         ( int resID );

    virtual const char*     GetFullPath     ( IResource* pRes );
    virtual const char*     GetFullPath     ( int resID );

    virtual const char*     GetHomeDirectory() const { return m_HomeDir.c_str(); }
    virtual void            CheckFileAccess (const char* name);
    virtual void            ConvertPathToRelativeForm( std::string& str );
	virtual const char*     ConvertPathToRelativeForm( const char* s);

private:
    std::vector<MountEntry>                                 m_MountEntries;
    static_array<ResourceInstance, c_MaxResourceInstances>  m_Resources;

    int                                                     m_LockedResource;
    ResourceStream                                          m_ResStream;

    std::string                        m_HomeDir;       //  project root directory
	
	std::vector<IResourceManager::OnLockData *> m_OnLockDataCallbacks;
}; // class ResourceManager

IResourceManager* GetResourceManager()
{
    static ResourceManager s_ResourceManager;
    return &s_ResourceManager;
}
DIALOGS_API IResourceManager*   IRM = GetResourceManager();

/*****************************************************************/
/*  ResourceManager implementation
/*****************************************************************/
ResourceManager::ResourceManager()
{
    char cwd[_MAX_PATH];
    m_HomeDir = _getcwd( cwd, _MAX_PATH );

} // ResourceManager::ResourceManager

bool ResourceManager::MountDataSource(  const char* sourcePath,
                                        const char* mountName,  
                                        DataSourceType type )
{
    if (type = dstDirectory)
    {
        MountEntry entry;
        entry.m_MountPath   = mountName ? mountName : sourcePath;
        entry.m_SourceType  = type;
        entry.m_SourcePath  = sourcePath;
        m_MountEntries.push_back( entry );
        return true;
    }

    if (type == dstResource)
    {
        return false;
    }

    if (type == dstPackage)
    {
        return false;
    }

    return false;
} // ResourceManager::MountDataSource

typedef boost::filesystem::path bpath;
typedef boost::filesystem::directory_iterator BDirIter;

bool find_file( const bpath& dir_path, const std::string& file_name, bpath& path_found )        
{
    if (!boost::filesystem::exists( dir_path )) return false;
    BDirIter end_itr; 
    for (BDirIter itr( dir_path ); itr != end_itr; ++itr)
    {
        if (boost::filesystem::is_directory( *itr ))
        {
            if (find_file( *itr, file_name, path_found )) return true;
        }
        else if (!stricmp( itr->leaf().c_str(), file_name.c_str() ))
        {
            path_found = *itr;
            return true;
        }
    }
    return false;
} // find_file

int ResourceManager::FindResource( const char* resName )
{
    char cwd[_MAX_PATH];
    _getcwd( cwd, _MAX_PATH );

    //  search whether there already is such resource
    for (int i = 0; i < m_Resources.size(); i++)
    {
        if (!strcmp( resName, m_Resources[i].m_Name.c_str() )) return i;
    }
    //  search through mount entries
    for (int i = 0; i < m_MountEntries.size(); i++)
    {
        const MountEntry& me = m_MountEntries[i];
        if (me.m_SourceType == dstDirectory)
        {
            bool bExists = false;            
            try{
                bpath p_cur ( cwd,               boost::filesystem::native );
                bpath p_res ( resName,           boost::filesystem::native );
                bpath p_home( m_HomeDir.c_str(), boost::filesystem::native );

                bpath p_root = boost::filesystem::complete( p_res, p_cur );

                //  look in current directory
                if (boost::filesystem::exists( p_root ))
                {
                    bExists = true;
                }
                //  look at full path from home directory
                else 
                {
                    p_root = boost::filesystem::complete( p_res, p_home );
                    if (boost::filesystem::exists( p_root )) 
                    {
                        bExists = true;
                    }
                    else
                    {
                        bpath p_mdir = boost::filesystem::complete( me.m_SourcePath, p_home );
                        p_root = boost::filesystem::complete( p_res, p_mdir );
                        if (boost::filesystem::exists( p_root )) 
                        {
                            bExists = true;
                        }
                    }
                }
            
                if (bExists)
                {
                    ResourceInstance res;
                    FilePath srcPath( p_root.native_file_string().c_str() );
                    res.m_FullPath      = srcPath.GetFullPath();
                    srcPath.SetFileName ( "" );
                    srcPath.SetExt      ( "" );
                    res.m_ID            = m_Resources.size();
                    res.m_Name          = resName;
                    res.m_MountEntry    = i;
                    res.m_SourcePath    = srcPath.GetFullPath();
                    m_Resources.push_back( res );
					_chdir( GetHomeDirectory() );
                    return res.m_ID;
                }
            }
            catch (...) 
            {
                _chdir( GetHomeDirectory() );
                return -1;
            }
        }
    }
    _chdir( GetHomeDirectory() );
    return -1;
} // ResourceManager::FindResource

bool ResourceManager::BindResource( int resID, IResource* pRes )
{
    if (resID < 0 || resID >= m_Resources.size()) return false;
    m_Resources[resID].m_pResource = pRes;
    return true;
} // ResourceManager::BindResource

const char* ResourceManager::GetPath( int resID )
{
    if (resID < 0 || resID >= m_Resources.size()) return "";
    return m_Resources[resID].m_SourcePath.c_str();
} // ResourceManager::GetPath

const char* ResourceManager::GetPath( IResource* pRes )
{
    int nRes = m_Resources.size();
    for (int i = 0; i < nRes; i++)
    {
        if (m_Resources[i].m_pResource == pRes)
        {
            return m_Resources[i].m_SourcePath.c_str();
        }
    }
    return "";
} // ResourceManager::GetPath

const char* ResourceManager::GetFullPath( int resID )
{
    if (resID < 0 || resID >= m_Resources.size()) return "";
    return m_Resources[resID].m_FullPath.c_str();
} // ResourceManager::GetPath

const char* ResourceManager::GetFullPath( IResource* pRes )
{
    int nRes = m_Resources.size();
    for (int i = 0; i < nRes; i++)
    {
        if (m_Resources[i].m_pResource == pRes)
        {
            return m_Resources[i].m_FullPath.c_str();
        }
    }
    return "";
} // ResourceManager::GetPath

const char* ResourceManager::GetName( IResource* pRes )
{
    int nRes = m_Resources.size();
    for (int i = 0; i < nRes; i++)
    {
        if (m_Resources[i].m_pResource == pRes)
        {
            return m_Resources[i].m_Name.c_str();
        }
    }
    return "";
} // ResourceManager::GetName

const char* ResourceManager::GetName( int resID )
{
    if (resID < 0 || resID >= m_Resources.size()) return "";
    return m_Resources[resID].m_Name.c_str();
} // ResourceManager::GetName

void ResourceManager::UnbindResource( IResource* pRes )
{
    int nRes = m_Resources.size();
    for (int i = 0; i < nRes; i++)
    {
        if (m_Resources[i].m_pResource == pRes)
        {
            m_Resources[i].m_pResource = NULL;
            return;
        }
    }
} // ResourceManager::UnbindResource

// ResourceManager::RegCallbackOnLockData
void ResourceManager::RegCallbackOnLockData(IResourceManager::OnLockData *Fn) {
	int c;
	for(c = 0; c < m_OnLockDataCallbacks.size(); c++) {
		if(m_OnLockDataCallbacks[c] == Fn) {
			break;
		}
	}
	if(c == m_OnLockDataCallbacks.size()) {
		m_OnLockDataCallbacks.push_back(Fn);
	}
} // ResourceManager::RegCallbackOnLockData


BYTE* ResourceManager::LockData( int resID, int& size )
{
    if (resID < 0 || resID >= m_Resources.size()) return NULL;
    ResourceInstance& res = m_Resources[resID];
    if (res.m_MountEntry < 0 || res.m_MountEntry >= m_MountEntries.size()) return NULL;
    MountEntry& me = m_MountEntries[res.m_MountEntry];
    if (me.m_SourceType == dstDirectory)
    {
        FilePath path( res.m_Name.c_str() );
        path.SetDir( res.m_SourcePath.c_str() );

		// Is there any OnLockData callbacks we should call?
		for(int c = 0; c < m_OnLockDataCallbacks.size(); c++) {
			m_OnLockDataCallbacks[c](res.m_Name.c_str());
		}
		
		HANDLE hFile = ::CreateFile( path,  GENERIC_READ,
											FILE_SHARE_READ, 
                                            NULL,
                                            OPEN_EXISTING, 
                                            FILE_FLAG_SEQUENTIAL_SCAN, 
                                            NULL );
        if (hFile == INVALID_HANDLE_VALUE) return NULL;
        if (size == 0) size = ::GetFileSize( hFile, NULL );
        delete []res.m_pData;
        res.m_pData = new BYTE[size + 1];
        res.m_DataSize = size;
        DWORD readBytes = 0;
        BOOL hr = ReadFile( hFile, res.m_pData, res.m_DataSize, &readBytes, NULL );        
        
        //  write trailing 0, just in case read resource is in text format
        res.m_pData[size] = 0;

        m_LockedResource = resID;
        if (hr == 0 || readBytes == 0)
        {
            delete []res.m_pData;
            res.m_pData = NULL;
            m_LockedResource = -1;
        }

        GetFileTime( hFile, NULL, NULL, &res.m_WriteTime );
        CloseHandle( hFile );
        _chdir( GetHomeDirectory() );
        return res.m_pData;
    }
    return NULL;
} // ResourceManager::LockData

void ResourceManager::UnlockData( int resID )
{
    if (resID < 0 || resID >= m_Resources.size()) return;
    ResourceInstance& res = m_Resources[resID];
    m_LockedResource = -1;
    delete []res.m_pData;
    res.m_pData = NULL;
    res.m_DataSize = 0;
} // ResourceManager::UnlockData

void ResourceManager::UpdateResources()
{
    FILETIME fTime;
    for (int i = 0; i < m_Resources.size(); i++)
    {
        ResourceInstance& res = m_Resources[i];
        FilePath path( res.m_Name.c_str() );
        path.SetDir( res.m_SourcePath.c_str() );
        HANDLE hFile = ::CreateFile( path,  GENERIC_READ, 
                                            FILE_SHARE_READ, 
                                            NULL,
                                            OPEN_EXISTING, 
                                            FILE_FLAG_SEQUENTIAL_SCAN, 
                                            NULL );
        if (hFile == INVALID_HANDLE_VALUE) continue;

        GetFileTime( hFile, NULL, NULL, &fTime );
        CloseHandle( hFile );
		try{
			if ( !dynamic_cast<IShader*>(res.m_pResource) || res.m_WriteTime.dwHighDateTime != fTime.dwHighDateTime ||
				res.m_WriteTime.dwLowDateTime != fTime.dwLowDateTime)
			{
				if (res.m_pResource) res.m_pResource->Reload();
			}
		}catch(...){
		}
    }
} // ResourceManager::UpdateResources

InStream& ResourceManager::LockResource( const char* resName, int size )
{
    int resID = IRM->FindResource( resName );
    if (resID == -1) return m_ResStream;
    ResourceInstance& res = m_Resources[resID];
    int realSize = size;
    BYTE* pData = IRM->LockData( resID, realSize );
    if (!pData) return m_ResStream;
    res.m_ResStream = ResourceStream( resID, pData, realSize );
    return res.m_ResStream;
} // ResourceManager::LockResource

bool kRS_LaunchProgram(const char* appName, const char* cmdLine, bool isWait)
{
	STARTUPINFO				StartupInfo;
	PROCESS_INFORMATION	ProcInfo;
	HANDLE					hProcess;
	DWORD						dwExitCode;

	memset(&StartupInfo, 0x00, sizeof(STARTUPINFO));
	StartupInfo.cb	= sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_MINIMIZE;

	char buf[1024];
	sprintf(buf, "\"%s\" %s", appName, cmdLine);
	if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcInfo))
	{
		hProcess = ProcInfo.hProcess;
		if(isWait)
		{
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			do	{
				if(!GetExitCodeProcess(hProcess, &dwExitCode)) break;
				Sleep(100);
				//  update window 
				if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}				
			} while(dwExitCode == STILL_ACTIVE);
			while(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return true;
	}
	else
	{
		DWORD err = GetLastError();
		return false;
	}
}

void ResourceManager::CheckFileAccess (const char* lpcsFileName){    
    DWORD attr = GetFileAttributes( lpcsFileName );

    if( attr!=INVALID_FILE_ATTRIBUTES && attr&FILE_ATTRIBUTE_READONLY ){
        WIN32_FIND_DATA	FD;
        HANDLE hFind;

        // directory
		int pos=0;
		int len=strlen(lpcsFileName)-1;

		if( lpcsFileName[1]==':' )
			pos=2;
		if( lpcsFileName[2]=='\\' || lpcsFileName[2]=='/' )
			pos++;

		char FullName[256];
		if( pos>0 && lpcsFileName[0]!='Q' && lpcsFileName[0]!='q' ){

			sprintf(FullName,"edit \"%s\"",lpcsFileName);

		}else{			
			
			FILE* F=fopen("clientroot.xml","r");
			if(F){
				char cr[256];
				fscanf(F,"%s",cr);
				sprintf(FullName,"edit \"%s",cr);
			}else{
				strcpy(FullName,"edit \"//depot/Data/");
			}

			while( pos<len ){
				if( lpcsFileName[pos]=='\\' || lpcsFileName[pos]=='/' ){

					char PartName[256];
					strcpy(PartName,lpcsFileName);

					PartName[pos]=0;

					hFind = FindFirstFile(PartName,&FD);
					if( hFind != INVALID_HANDLE_VALUE ){
						while( !(FD.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) ){
							if( !FindNextFile(hFind,&FD) ){
								Log.Error("Can not find directory: %s",PartName);
								break;
							}
						}
						strcat(FullName,FD.cFileName);
						strcat(FullName,"/");
					}

				}
				pos++;
			}

			// file name
			hFind = FindFirstFile(lpcsFileName,&FD);
			if( hFind != INVALID_HANDLE_VALUE ){
				strcat(FullName,FD.cFileName);
				strcat(FullName,"\"");
			}

		}		

        FindClose(hFind);

        //ShellExecute(hwnd,"open","cmd",FullName,NULL,SW_SHOW);

        bool kRS_LaunchProgram(const char* appName, const char* cmdLine, bool isWait);
        kRS_LaunchProgram("p4",FullName,true);

        DWORD attr = GetFileAttributes( lpcsFileName );
        if( attr!=INVALID_FILE_ATTRIBUTES && attr&FILE_ATTRIBUTE_READONLY ){
			Log.Error("Can't check out: %s",FullName);
        }
    }
}
void ResourceManager::ConvertPathToRelativeForm( std::string& str ){
    std::string str_temp=str;
    const char* s=str_temp.c_str();
    const char* fp=GetHomeDirectory();
    if(s && fp){
        int L=strlen(fp);
        if(!strnicmp(fp,s,L)){
            s+=L;
            if( L>0 && fp[L-1]!='\\' && fp[L-1]!='/' ){
                s++;
            }
            str=s;
        }
    }
}
const char* ResourceManager::ConvertPathToRelativeForm( const char* s){
	const char* fp=GetHomeDirectory();
	int L=strlen(fp);
	if(!strnicmp(fp,s,L)){
        s+=L;
		if( L>0 && fp[L-1]!='\\' && fp[L-1]!='/'){
			s++;
		}
	}
	return s;
}




