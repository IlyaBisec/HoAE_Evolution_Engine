#pragma once
#include "MapTemplates.h"
#include "vui_Action.h"
//////////////////////////////////////////////////////////////////////////
class cvi_Point: public BaseClass{
public:
	int x;
	int y;
	SAVE(cvi_Point){
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
	}ENDSAVE;
};
//
class cvi_Missions: public ReferableBaseClass{
public:
	_str FileM3D;
	char* Name;
	char* Description;
	short DescrGPFile;
	int DescrGPSprite;
	bool Tutorial;
	_str MapPreview;
	int MapCenterX;
	int MapCenterY;
	ClonesArray<cvi_Point> ScreenSaver;
	SAVE(cvi_Missions){
		REG_FILEPATH(FileM3D,".m3d");
		REG_MEMBER(_textid,Name);
		REG_MEMBER(_textid,Description);
		REG_MEMBER(_gpfile,DescrGPFile);
		REG_SPRITE(DescrGPSprite,DescrGPFile);
		REG_MEMBER(_bool,Tutorial);		
		REG_FILEPATH(MapPreview,".jpg");
		REG_MEMBER(_int,MapCenterX);
		REG_MEMBER(_int,MapCenterY);
		REG_AUTO(ScreenSaver);
	}ENDSAVE;
};
extern ClonesArray<cvi_Missions> EW2_Missions;
extern const char* EW2_MissionsXML;
//
regAc(va_EW2_Mis_List,vfI vfS);
regAc(va_EW2_Mis_Tap,vfS);
regAc(va_EW2_Mis_Text,vfS);
regAc(va_EW2_Mis_Picture,vfS);
regAc(va_EW2_Mis_Start,vfS vfL);
regAc(va_EW2_Mis_CheckTutorial, vfS; bool Tutorial, REG_MEMBER(_bool,Tutorial); );