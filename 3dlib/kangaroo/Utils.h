typedef ClassArray<_str> FileList;
void CreateSortedFileList(const char* Directory,const char* Mask,FileList& result,bool subdir);
DWORD _ModDWORDRGB(DWORD C1,DWORD C2);

__inline int lrintf (float flt){
	int intgr;
	_asm{
		fld flt
		fistp intgr
	};
	return intgr ;
}
__inline int ffloorf(float flt){
    int intgr;
    flt-=0.49999999999;
	_asm{
		fld flt        
		fistp intgr
	};
	return intgr ;
}
void FillEdges(DWORD* buf,int nx,int ny,int pitch,DWORD DefValue,int N,DWORD ValueToFill=0);
void SaveBufferTo(const char* Name,DWORD* buf,int nx,int ny);
void SaveGreyFloatsTo(const char* Name,float* buf,int nx,int ny);
UINT64 GETFREE(char* lpszPath);
bool GetCursorPos2(POINT* p);
int DrawBottomAlignedText(int xb,int yb,char* Text,int MaxLx,int FontSize,DWORD Color,int Align,bool Draw);
int DrawMultilineText(int x,int y,char* s,int DefaultFont,DWORD DefColor,int& MaxLx,int MaxLy=100000,byte CenterX=0,bool Draw=true);