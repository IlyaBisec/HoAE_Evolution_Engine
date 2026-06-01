/*****************************************************************
/*  File:   IResourceManager.h                                      
/*  Desc:   Interface to the resource managing system
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Jun 2004                                             
/*****************************************************************/
#ifndef __IRESOURCEMANAGER_H__ 
#define __IRESOURCEMANAGER_H__
#include <string>

/*****************************************************************************/
/*  Struct: DataChunk
/*  Desc:   Represents chunk of data in the memory
/*****************************************************************************/
struct DataChunk
{
    BYTE*       m_Buffer;
    int         m_Size;

    DataChunk() : m_Buffer(NULL), m_Size(0){}
}; // struct DataChunk

enum DataSourceType
{
    dstUnknown          = 0,
    dstDirectory        = 1,
    dstResource         = 2,
    dstPackage          = 3
}; // enum DataSourceType

class IResource;
class InStream;
/*****************************************************************************/
/*    Class:    IResourceManager
/*    Desc:    Interface for managing resources
/*****************************************************************************/
class IResourceManager
{
public:
    /// mounts source for loading data files
    virtual bool            MountDataSource (   const char* sourcePath,
                                                const char* mountName = NULL, 
                                                DataSourceType type = dstDirectory ) = 0;
    
    virtual int             FindResource    ( const char* resName ) = 0;
    virtual bool            BindResource    ( int resID, IResource* pRes ) = 0;
    virtual void            UnbindResource  ( IResource* pRes ) = 0;

    
    virtual BYTE*           LockData        ( int resID, int& size ) = 0;
	
	typedef void			OnLockData(const char *FileName);
	virtual void			RegCallbackOnLockData(OnLockData *Fn) = 0;
	
	virtual void            UnlockData      ( int resID ) = 0;
    virtual InStream&       LockResource    ( const char* resName, int size = 0 ) = 0;

    virtual void            UpdateResources () = 0;
    
    virtual const char*     GetPath         ( IResource* pRes ) = 0;
    virtual const char*     GetPath         ( int resID ) = 0;

    virtual const char*     GetFullPath     ( IResource* pRes ) = 0;
    virtual const char*     GetFullPath     ( int resID ) = 0;

    virtual const char*     GetName         ( IResource* pRes ) = 0;
    virtual const char*     GetName         ( int resID ) = 0;

    virtual const char*     GetHomeDirectory() const = 0;
    virtual void            CheckFileAccess (const char* name) = 0;
    virtual void            ConvertPathToRelativeForm( std::string& str ) = 0;
	virtual const char*     ConvertPathToRelativeForm( const char* s) = 0;

}; // class IResourceManager

extern DIALOGS_API IResourceManager* IRM;
IResourceManager* GetResourceManager();

/*****************************************************************************/
/*    Class:    IResource
/*    Desc:    Interface to the resource instance
/*****************************************************************************/
class IResource
{
public:
    virtual             ~IResource()
    {
        IRM->UnbindResource( this );
    }
    virtual bool        Reload  () = 0;
    virtual bool        Dispose () { return false; }

}; // class IResource

#endif // __IRESOURCEMANAGER_H__ 