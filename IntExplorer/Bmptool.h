#define BMPTOOL
typedef unsigned char byte;
#pragma pack(1)
struct BMFH { // bmfh 
        WORD    bfType; 
        DWORD   bfSize; 
        WORD    bfReserved1; 
        WORD    bfReserved2; 
        DWORD   bfOffBits; 
		DWORD   biSize; 
		LONG    biWidth; 
		LONG    biHeight; 
		WORD    biPlanes; 
		WORD    biBitCount;
		DWORD   biCompression; 
		DWORD   biSizeImage; 
		LONG    biXPelsPerMeter; 
		LONG    biYPelsPerMeter; 
		DWORD   biClrUsed; 
		DWORD   biClrImportant; 
};
struct PAL3{
	byte r,g,b;
};
struct PAL4{
	byte b,g,r,reserved;
};
void SaveToBMP24(char* Name,int Lx,int Ly,byte* data);
bool ReadBMP24(char* Name, BMFH* BM,byte** data);
bool ReadBMP8(char* Name, BMFH* BM,byte** data);
bool ReadBMP8TOBPX(char* Name,byte** data);
void SaveToBMP8(char* Name,int Lx,int Ly,byte* data,PAL3* pal);
bool ReadBMP8PAL(char* Name, BMFH* BM,byte** data,byte* PAL);