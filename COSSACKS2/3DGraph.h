//----------------------NEW MAP WITH CASHING--------------------
//
//
//    /|\ /|\ /|\ /|\
//   |/|\|/|\|/|\|/|\|
//   |/|\|/|\|/|\|/|\|
//   
//
//
//
//

//extern int CurTIRef;
//extern int CurTIBufPos;
//extern int MaxExistingTIRef;
#define TriUnit		16 //32 //24
#define TriUnit34	12 //24 //18
#pragma once
void ShowRelief();
void ShowSector(int i,bool Mode3D);
int GetLighting(int i);
void CreateRandomHMap();
void CreateTriBlob(int x,int y,int h,int r);
void SetHi(int i,int h);
int GetHi(int i);
int GetTriX(int i);
int GetTriY(int i);
extern bool Mode3D;
extern byte TexColors[256];
extern word RoadTex[256];
typedef int HiCallback(int x,int y,int Hig);
void CreateEffect(int x,int y,int r,HiCallback* HCB);
void DirectRenderTriangle64(int xs1,int ys1,
						   int xs2,int ys2,
						   int xs3,int ys3,
						   int xb1,int yb1,
						   int xb2,int yb2,
						   int xb3,int yb3,
						   int f1,int f2,int f3,
						   byte * Dest,byte* Bitm,
						   int StartLine,int EndLine,int ScanSize);
int GetVTex(int i);
//-----------------------------------------------------------------------------------//
inline void SetTHMap(int v,int Val){

};
inline int GetTHMap(int v){

};
inline int GetTexMap(int v){	
};
inline int GetSectMap(int v){	
};
inline void SetSectMap(int v,byte s){	
};