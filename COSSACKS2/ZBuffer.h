#pragma once
void AddPoint(short XL,short YL,short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2,int Rotation=0);
void AddLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2);
void AddHiPoint(short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2);
void AddLoPoint(short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2);
void AddSuperLoPoint(short x,short y,OneObject* OB,word FileID,word SpriteID,int Param1,int Param2);
void AddOptPoint(byte Method,short XL,short YL,short x,short y,OneObject* OB,word FileID,word Sprite,int Options);
void AddOptLine(short X1,short Y1,short X2,short Y2,short x,short y,OneObject* OB,word FileID,word SpriteID,int Options);
void AddAnimation(float x,float y,float z,NewAnimation* ANM,int Frame,float Dir,DWORD Diffuse=0xFF808080,OneObject* OB=NULL,float Scale=1.0f,float fiDir=0.0f,float fiOrt=0.0f,DWORD Handle=0);
void AddWorldPoint(int xc,int yc,int zc,int dx,int dy,OneObject* OB,word FileID,word SpriteID,DWORD Diffuse=0xFF808080,bool AllowRotation=false,float ScaleX=1.0f,float ScaleY=1.0f,float RotateAngle=0.0f);
void ShowZBuffer();
void ClearZBuffer();
#define ZBF_NORMAL	0
#define ZBF_LO		1
#define ZBF_HI		2
extern byte CurDrawNation;