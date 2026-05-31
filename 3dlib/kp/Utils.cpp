#include "common.h"

int fl_compare( const void *arg1, const void *arg2 ){
	_str* s1=(_str*)(*(DWORD*)arg1);
	_str* s2=(_str*)(*(DWORD*)arg2);
	return stricmp(s1->pchar(),s2->pchar());
}

void CreateSortedFileList(const char* Directory,const char* Mask,FileList& result,bool subdir){
	WIN32_FIND_DATA FD;
	char x[512];
	sprintf(x,"%s%s",Directory,Mask);
	HANDLE H=FindFirstFile(x,&FD);
	if(H!=INVALID_HANDLE_VALUE){
		do{	
			if( FD.cFileName[0]!='%' && FD.cFileName[0]!='.' ){
				_str* s=new _str;
				*s=Directory;				
				*s+=FD.cFileName;
				result.Add(s);
			}
			if(!FindNextFile(H,&FD))break;
		}while(true);
		FindClose(H);
		if(subdir){
			sprintf(x,"%s*.*",Directory);
			HANDLE H=FindFirstFile(x,&FD);
			if(H!=INVALID_HANDLE_VALUE){
				do{		
					if(FD.cFileName[0]!='%' && FD.cFileName[0]!='.' && (FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
						sprintf(x,"%s%s\\",Directory,FD.cFileName);
						FileList tmp;
						CreateSortedFileList(x,Mask,tmp,subdir);
						for(int j=0;j<tmp.GetAmount();j++){
							sprintf(x,"%s\\%s",FD.cFileName,tmp[j]->pchar());
							_str* s=new _str;
							*s=x;
							result.Add(s);
						}						
					}
					if(!FindNextFile(H,&FD))break;
				}while(true);
				FindClose(H);
			}
		}
		//sorting
		qsort(result.GetValues(),result.GetAmount(),4,fl_compare);
	}
}
struct pt2{
    short x;
    short y;
    pt2(){};
    pt2(int _x,int _y){
        x=_x;
        y=_y;
    }
};

void FillEdges(DWORD* buf,int nx,int ny,int pitch,DWORD DefValue,int N,DWORD VF){
    pitch>>=2;
    DWORD* temp=new DWORD[ny*pitch];
    DynArray<pt2> EdgeList;
    DynArray<pt2> EdgeList2;    
    int ny1=ny-1;
    int nx1=nx-1;
    memcpy(temp,buf,pitch*ny*4);
    for(int y=1;y<ny1;y++){
        int ofs=y*pitch+1;
        for(int x=1;x<nx1;x++){
            if(buf[ofs]==DefValue){
                if(temp[ofs-1]!=DefValue){
                    EdgeList.Add(pt2(x,y));
                    buf[ofs]=temp[ofs-1];
                }else
                if(temp[ofs+1]!=DefValue){
                    EdgeList.Add(pt2(x,y));
                    buf[ofs]=temp[ofs+1];
                }else
                if(temp[ofs-pitch]!=DefValue){
                    EdgeList.Add(pt2(x,y));
                    buf[ofs]=temp[ofs-ny];
                }else
                if(temp[ofs+pitch]!=DefValue){
                    EdgeList.Add(pt2(x,y));
                    buf[ofs]=temp[ofs+pitch];
                }
            }
            ofs++;                
        }
    }
    for(int i=0;i<N;i++){
        int nn=EdgeList.GetAmount();
        for(int q=0;q<nn;q++){
            pt2& p=EdgeList[q];
            int ofs=p.x+p.y*pitch;
            if(buf[ofs]!=DefValue){
                if(p.x>0 && buf[ofs-1]==DefValue){
                    buf[ofs-1]=buf[ofs];
                    EdgeList2.Add(pt2(p.x-1,p.y));
                }
                if(p.x<nx-1 && buf[ofs+1]==DefValue){
                    buf[ofs+1]=buf[ofs];
                    EdgeList2.Add(pt2(p.x+1,p.y));
                }
                if(p.y>0 && buf[ofs-pitch]==DefValue){
                    buf[ofs-pitch]=buf[ofs];
                    EdgeList2.Add(pt2(p.x,p.y-1));
                }
                if(p.y<ny-1 && buf[ofs+pitch]==DefValue){
                    buf[ofs+pitch]=buf[ofs];
                    EdgeList2.Add(pt2(p.x,p.y+1));
                }
            }
        }
        EdgeList2.CopyArrayTo(EdgeList);
        EdgeList2.FastClear();
    }
    for(int y=0;y<ny;y++){
        int ofs=y*pitch;
        for(int x=0;x<nx;x++){
            if(buf[ofs]==DefValue){
                buf[ofs]=VF;                    
            }
            ofs++;                
        }
    }
    delete[]temp;
}
void SaveBufferTo(const char* Name,DWORD* buf,int nx,int ny){
    int T=IRS->CreateTexture(Name,nx,ny,cfARGB8888,0,tmpSystem);
    int pitch;
    BYTE* data=IRS->LockTexBits(T,pitch);
    if(data){
        for(int i=0;i<ny;i++){
            memcpy(data+(ny-i-1)*pitch,buf+i*nx,nx<<2);
        }
        IRS->UnlockTexBits(T);
    }
    IRS->SaveTexture(T,Name);
    IRS->DeleteTexture(T);
}
void SaveGreyFloatsTo(const char* Name,float* buf,int nx,int ny){
    int sz=nx*ny;
    DWORD* D=new DWORD[sz];    
    for(int i=0;i<sz;i++){
        int v=128+buf[i]*127;
        DWORD V=v;//clamp(v,0,255);
        D[i]=V+(V<<8)+(V<<16)+0xFF000000;
    }
    SaveBufferTo(Name,D,nx,ny);
    delete[]D;
}
UINT64 GETFREE(char* lpszPath){
	void* pGetDiskFreeSpaceEx;
	pGetDiskFreeSpaceEx=GetProcAddress(
	GetModuleHandle("kernel32.dll"),
		"GetDiskFreeSpaceExA");
	if (pGetDiskFreeSpaceEx){
		CHAR szTemp[4];
		ULARGE_INTEGER ForCaller,Total;
		ZeroMemory(szTemp,4);
		strncpy(szTemp,lpszPath,3);
  		GetDiskFreeSpaceEx(szTemp,&ForCaller,&Total,NULL);
		return (UINT64)ForCaller.QuadPart;
	}else{
		DWORD
			dwSectPerClust, 
            dwBytesPerSect,
            dwFreeClusters, 
            dwTotalClusters;

		CHAR szTemp[4];
		ZeroMemory(szTemp,4);
		strncpy(szTemp,lpszPath,2);
		GetDiskFreeSpace (szTemp, 
            &dwSectPerClust, 
            &dwBytesPerSect,
            &dwFreeClusters, 
            &dwTotalClusters);
		return (UINT64)(dwSectPerClust*dwBytesPerSect*dwFreeClusters);
	};
};