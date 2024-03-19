#pragma once
//#define sprNx  (64<<ADDSH)
#include "MapTemplates.h"
#include <gmDefines.h>
#include <mMath2D.h>
#include <mMath3D.h>
#pragma pack(1)
#define SprShf (6+ADDSH)
#define SprInCell 16
extern int MaxSprt;
#include "OneSprite.h"
class TimeReq{
public:
    int NMembers;
    int MaxMembers;
    int* IDS;
    byte* Kinds;
    TimeReq();
    ~TimeReq();
    void Handle();
    void Add(int ID,byte Kind);
    void Del(int ID,byte Kind);
};
extern TimeReq ObjTimer;
//extern byte NSpri[sprNx*sprNx];
extern byte* NSpri;
//extern word* SpRefs[sprNx*sprNx];
extern int** SpRefs
;
extern OneSprite* Sprites;
void InitSprites();
void addSprite(int x,int y,SprGroup* SG,word id);
int GetHeight(int x,int y);
void addScrSprite(int x,int y,SprGroup* SG,word id);
void addTree(int x,int y);
extern OneSprite* Sprites;
extern SprGroup TREES;
extern SprGroup STONES;
extern SprGroup HOLES;
extern SprGroup COMPLEX;
extern SprGroup ANMSPR;
extern SprGroup SPECIAL;
void ProcessSprites();
byte DetermineResource(int x,int y);
byte FindAnyResInCell(int x,int y,int cell,int* Dist,byte Res,float* Scale);
bool CheckDist(int x,int y,word r);
void HideFlags();
//-----------------------L3--------------------------
//#define L3DX (MAPSX>>4)
//#define L3DY (MAPSY>>4)
//#define L3MAX (MAPSX<<2)
//#define L3SH (ADDSH+5)
//extern byte* L3HIMap[L3DX*L3DY];
//void InitL3();
//void ClearL3();
//void SetL3Point(int x,int y,byte H);
//int GetL3Height(int x,int y);
int GetUnitHeight(int x,int y);