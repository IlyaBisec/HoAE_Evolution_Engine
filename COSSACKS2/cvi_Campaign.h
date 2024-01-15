#pragma once
#include "MapTemplates.h"
#include "vui_Action.h"
//////////////////////////////////////////////////////////////////////////
class SimpleDialog;
class DialogsSystem;
//////////////////////////////////////////////////////////////////////////

class cvi_MissionFrame: public ReferableBaseClass
{
public:
	//cvi_MissionFrame(void);
	//~cvi_MissionFrame(void);
	virtual void Play(){};
	virtual bool Process(){ return false; };
	//bool Done;
	//
	AUTONEW(cvi_MissionFrame);
};
//
class cvi_ChangeDifficulty: public cvi_MissionFrame
{
public:
	virtual void Play();
	int Value;
	SAVE(cvi_ChangeDifficulty){
		REG_PARENT(cvi_MissionFrame);
		REG_MEMBER(_int,Value);		
	}ENDSAVE;
};
//
typedef ClonesArray< ClassRef<BitPicture> > raBitPicture;
//
class cvi_Fresco: public BaseClass
{
private:
	int ScrollLx;
	double ScrollTime;	
	double ScrollSpeed;
public:
	//cvi_Fresco(void);
	//~cvi_Fresco(void);
	void Play(raBitPicture& aBP, TextButton* TB);
    void Process(raBitPicture& aBP, TextButton* TB);
	bool isFinished();
	//
	bool Scroll;
	ClonesArray<_picfile> Pics;
    _picfile Picture;
	char* Text;
	_str Sound;
	SAVE(cvi_Fresco){
		SAVE_SECTION(0xFFFFFFFF);
		REG_MEMBER(_bool,Scroll);
		SAVE_SECTION(1);
		REG_AUTO(Picture);
		SAVE_SECTION(2);
		REG_AUTO(Pics);
		SAVE_SECTION(0xFFFFFFFF);
		REG_MEMBER(_textid,Text);
		REG_FILEPATH(Sound,".ogg");
	}ENDSAVE;	
	//
	virtual DWORD GetClassMask(){
		if(Scroll) return 2;
			else return 1;
	}
};
class cvi_mfFrescos: public cvi_MissionFrame
{
public:
	//cvi_mfFrescos(void);
	//~cvi_mfFrescos(void);
	virtual void Play();
	virtual bool Process();
	//
	int curFresco;
	//
	ClonesArray<cvi_Fresco> Frescos;
	SAVE(cvi_mfFrescos){
		REG_PARENT(cvi_MissionFrame);
		REG_AUTO(Frescos);
	}ENDSAVE;	
};
class cvi_mfMovie: public cvi_MissionFrame
{
public:
	cvi_mfMovie(void){
		px=0.0f; py=0.17f;
	};
	//~cvi_mfMovie(void);
	virtual void Play();
	//virtual bool Process();
	virtual bool CheckIfObjectIsGlobal(){ return true; }
	//
	_str Movie;	
	float px,py;
	bool PassMission;
	SAVE(cvi_mfMovie){
		REG_PARENT(cvi_MissionFrame);
		REG_MEMBER(_str,Name);
		REG_FILEPATH(Movie,".ogg,.bik");
		REG_MEMBER(_float,px);
		REG_MEMBER(_float,py);
		REG_MEMBER(_bool,PassMission);
	}ENDSAVE;	
};
/*
class cvi_mfStat
{
public:
	int 
};
class cvi_mfStatistic: public cvi_MissionFrame
{
public:
	//cvi_Fresco(void);
	//~cvi_Fresco(void);
	//_str Movie;
	virtual void Play();
	SAVE(cvi_mfStatistic){
		REG_PARENT(cvi_MissionFrame);
		//REG_FILEPATH(Movie,".bik");
	}ENDSAVE;	
};
*/
class cvi_mfGame: public cvi_MissionFrame
{
public:
	//cvi_Fresco(void);
	//~cvi_Fresco(void);
	virtual void Play();
	//virtual bool Process();
	//
	_str MissFileName;
	bool PostStatistic;
	SAVE(cvi_mfGame){
		REG_PARENT(cvi_MissionFrame);
		REG_FILEPATH(MissFileName,".m3d");
		REG_MEMBER(_bool,PostStatistic);
	}ENDSAVE;	
};
//
class cvi_Mission: public BaseClass {
public:

	int curFrame;
	
	char* Name;
	char* GameDescription;
	ClassArray<cvi_MissionFrame> Scene;
	int MapX;
	int MapY;

	short PreviewFile;
	int PreviewSprite;
	
	SAVE(cvi_Mission){
		REG_MEMBER(_textid,Name);
		REG_MEMBER(_textid,GameDescription);
		REG_AUTO(Scene);
		REG_MEMBER(_int,MapX);
		REG_MEMBER(_int,MapY);
		REG_MEMBER(_gpfile,PreviewFile);
		REG_SPRITE(PreviewSprite,PreviewFile);
	}ENDSAVE;

	cvi_Mission();
	void StartScene();
	bool Process();

};
//////////////////////////////////////////////////////////////////////////
class cvi_Campaign: public BaseClass
{
public:
	cvi_Campaign(void);
	~cvi_Campaign(void);
	bool Init;
	int curMission;
	bool Process();
	//
	char* Name;
	_str DeskName;
	ClonesArray<cvi_Mission> Missions;
	DWORD Over;
	DWORD Finished;
	DWORD Next;
	DWORD Selected;
	DWORD Frontier;
	short heroGP;
	short heroGP_Left;
	short heroGP_Right;
	int heroSPR;
	int heroDx;
	int heroDy;
	short PreviewFile;
	int PreviewSprite;
	SAVE(cvi_Campaign){
		REG_LOADSAVE("dialogs\\","*.cvi_Campaign.xml");
		REG_MEMBER(_textid,Name);
		REG_AUTO(Missions);
		//REG_FILEPATH(DeskFileName,".DialogsDesk.Dialogs.xml");
		REG_AUTO(DeskName);
		REG_MEMBER(_color,Over);
		REG_MEMBER(_color,Finished);
		REG_MEMBER(_color,Next);
		REG_MEMBER(_color,Selected);
		REG_MEMBER(_color,Frontier);		
		REG_MEMBER(_gpfile,heroGP);
		REG_SPRITE(heroSPR,heroGP);
		REG_MEMBER(_gpfile,heroGP_Left);
		REG_MEMBER(_gpfile,heroGP_Right);
		REG_MEMBER(_int,heroDx);
		REG_MEMBER(_int,heroDy);
		REG_MEMBER(_gpfile,PreviewFile);
		REG_SPRITE(PreviewSprite,PreviewFile);
	}ENDSAVE;	
};
extern ClonesArray<cvi_Campaign> vmCampaigns;
extern const char* vmCampaignXML;
extern int vmCampID;
extern cvi_Campaign* vmCamp;
//////////////////////////////////////////////////////////////////////////
regAc(cva_Camp_Select, vfS vfL
	ClassRef<ListDesk> List;
	,
	REG_AUTO(List);
);
regAc(cva_CCamp_Name, vfS	
	,	
);
regAc(cva_CCamp_Sectors, vfS	
	,	
);
regAc(cva_CMiss_Name, vfS	
	,
);
regAc(cva_Campaign, vfS
	,
);
regAc(cva_CampDiff, vfS vfI
	ClassRef<TextButton> tbMissDescr;
	,
	REG_AUTO(tbMissDescr);
);
regAc(cva_MissList, vfS);
regAc(cva_Mission, vfS vfL	
	cva_Mission::cva_Mission(){
		Colored=true;
	}
	int Index;
	bool Colored;
	ClassRef<VitButton> Main;
	,
	REG_MEMBER(_int,Index);
	REG_MEMBER(_bool,Colored);
	REG_AUTO(Main);
);
regAc(cva_Missions_XY, vfS );
regAc(cva_Camp_StartMission, vfI vfS vfL
	static int MissionID;
	,	
);
regAc(cva_Fresco_Desk, vfS
	raBitPicture Picture;
	ClassRef<TextButton> Text;
	ClassRef<VitButton> MissTitle;
	//ClassRef<VitButton> TextTitle;
	,
	REG_AUTO(Picture);
	REG_AUTO(Text);
	REG_AUTO(MissTitle);
);
regAc(cva_Fresco_Next, vfS vfL
	static cvi_mfFrescos* FR;
	static bool Clicked;
	//static int CUR;
);
regAc(cva_Fresco_Close, vfS vfL
	,
);
//
regAc(cva_RC_Statistic, vfS
	ClassRef<TextButton> CurExp;
	ClassRef<DialogsDesk> InfoTable;
	, 
	REG_AUTO(CurExp);
	REG_AUTO(InfoTable);
);

regAc(cva_HeroesUpgrades, vfS
	ClassRef<GPPicture>		UpgradeAA;
	ClassRef<GPPicture>		UpgradeAB;
	ClassRef<GPPicture>		UpgradeAC;
	ClassRef<GPPicture>		UpgradeBA;
	ClassRef<GPPicture>		UpgradeBB;
	ClassRef<GPPicture>		UpgradeBC;
	ClassRef<GPPicture>		UpgradeCA;
	ClassRef<GPPicture>		UpgradeCB;
	ClassRef<GPPicture>		UpgradeCC;
	ClassRef<GPPicture>		CardA;
	ClassRef<GPPicture>		CardB;
	ClassRef<GPPicture>		CardC;
	ClassRef<TextButton>	HATitle;
	ClassRef<TextButton>	HBTitle;
	ClassRef<TextButton>	HCTitle;
	ClassRef<TextButton>	LPriceAA;
	ClassRef<TextButton>	LPriceAB;
	ClassRef<TextButton>	LPriceAC;
	ClassRef<TextButton>	LPriceBA;
	ClassRef<TextButton>	LPriceBB;
	ClassRef<TextButton>	LPriceBC;
	ClassRef<TextButton>	LPriceCA;
	ClassRef<TextButton>	LPriceCB;
	ClassRef<TextButton>	LPriceCC;
	ClassRef<GPPicture>		HeroeA;
	ClassRef<GPPicture>		HeroeB;
	ClassRef<GPPicture>		HeroeC;
	ClassRef<TextButton>	AllExp;
	,
	REG_AUTO(UpgradeAA);
	REG_AUTO(UpgradeAB);
	REG_AUTO(UpgradeAC);
	REG_AUTO(UpgradeBA);
	REG_AUTO(UpgradeBB);
	REG_AUTO(UpgradeBC);
	REG_AUTO(UpgradeCA);
	REG_AUTO(UpgradeCB);
	REG_AUTO(UpgradeCC);
	REG_AUTO(CardA);
	REG_AUTO(CardB);
	REG_AUTO(CardC);
	REG_AUTO(HATitle);
	REG_AUTO(HBTitle);
	REG_AUTO(HCTitle);
	REG_AUTO(LPriceAA);
	REG_AUTO(LPriceAB);
	REG_AUTO(LPriceAC);
	REG_AUTO(LPriceBA);
	REG_AUTO(LPriceBB);
	REG_AUTO(LPriceBC);
	REG_AUTO(LPriceCA);
	REG_AUTO(LPriceCB);
	REG_AUTO(LPriceCC);
	REG_AUTO(HeroeA);
	REG_AUTO(HeroeB);
	REG_AUTO(HeroeC);
	REG_AUTO(AllExp);
);

regAc(cva_HeroeXUpgradeX, vfS vfL
	int idHeroe;
	int idUpgrade;
	ClassRef<GPPicture>		Ability;
	ClassRef<GPPicture>		Plus;
	ClassRef<GPPicture>		Level;
	,
	REG_MEMBER(_int, idHeroe);
	REG_MEMBER(_int, idUpgrade);
	REG_AUTO(Ability);
	REG_AUTO(Plus);
	REG_AUTO(Level);
	);

regAc(cva_RC_Hero, vfS	
	int HeroID;
	, 
	REG_MEMBER(_int, HeroID);
);
regAc(cva_RC_Ability, vfS vfL
	int dExp;
	int HeroID;
	int AbilityID;
	, 
	REG_MEMBER(_int, HeroID);
	REG_MEMBER(_int, AbilityID);
);
regAc(cva_SPD_FrescoList, vfS vfI vfA , );