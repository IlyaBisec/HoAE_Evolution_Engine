#ifndef __gFile__
#define __gFile__

#ifndef GFILE_API
#ifndef GFILE_USER
#define GFILE_API __declspec(dllexport)
#else
#define GFILE_API __declspec(dllimport)
#endif
#endif
class GFILE_API GFILE{
public:
	byte Buf[16384];
	int NBytesRead;
	int BufPos;
	int GlobalPos;
	int Size;
	ResFile F;
	FILE* rf;
	bool RealText;
	GFILE();
	~GFILE();
	bool Open(char* Name);
	void Close();
	int ReadByte();
	int CheckByte();
	//standart functions
	int Gscanf(char* Mask,va_list args);
	int Ggetch();
};
GFILE_API GFILE* Gopen(char* Name,char* Mode);
GFILE_API int Gscanf(GFILE* F,char* mask,...);
GFILE_API int Ggetch(GFILE* F);
GFILE_API void Gprintf(GFILE* F,const char *format,...);
GFILE_API void Gclose(GFILE* F);

#endif __gFile__