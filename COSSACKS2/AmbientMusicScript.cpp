#include "stdheader.h"
#include "ClassEditorsRegistration.h"

class am_Condition:public BaseClass{
public:
    SAVE(am_Condition);
	ENDSAVE;
	virtual bool check(){return true;}
};
class am_AND:public am_Condition{
public:
	ClassArray<am_Condition> AND;
	SAVE(am_AND);
		REG_PARENT(am_Condition);
		REG_AUTO(AND);
	ENDSAVE;
	virtual bool check(){
		for(int i=0;i<AND.GetAmount();i++)if(!AND[i]->check())return false;
		return true;
	}
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$AND, and $%";
		return true;
	}
};
class am_NOT:public am_Condition{
public:
	ClassPtr<am_Condition> NOT;
	SAVE(am_NOT);
	REG_PARENT(am_Condition);
	REG_AUTO(NOT);
	ENDSAVE;
	virtual bool check(){
		if(NOT.Get()){
			return !NOT.Get()->check();
		}		
		return true;
	}
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="NOT(%$NOT$%)";
		return true;
	}
};
class am_OR:public am_Condition{
public:
	ClassArray<am_Condition> OR;
	SAVE(am_OR);
		REG_PARENT(am_Condition);
		REG_AUTO(OR);
	ENDSAVE;
	virtual bool check(){
		for(int i=0;i<OR.GetAmount();i++)if(OR[i]->check())return true;
		return false;
	}
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$OR, or $%";
		return true;
	}
};
extern int LastMx;
extern int LastMy;
class am_CheckLandType:public am_Condition{
public:
	int LandType;
	SAVE(am_CheckLandType);
		REG_PARENT(am_Condition);
		REG_ENUM(_index,LandType,LANDLAYERS);
	ENDSAVE;
	virtual bool check(){
		int GetLandTypeInPoint(int x,int y);
		return LandType==GetLandTypeInPoint(LastMx,LastMy);
	}
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
        dest="If_Land_Type(%$LandType$%)";
		return true;
	}
};
class am_CheckMapName:public am_Condition{
	_str PartOfMapName;
	SAVE(am_CheckMapName)
		REG_PARENT(am_Condition);
		REG_AUTO(PartOfMapName);
	ENDSAVE;
	virtual bool check(){
		return strstr(GSets.CGame.cgi_CurrentMap,PartOfMapName.pchar())!=NULL;		
	}
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="If_MapNameContains(%$PartOfMapName$%)";
		return true;
	}
};
class am_CheckRect:public am_Condition{
	int x,y,x1,y1;
	SAVE(am_CheckRect)
		REG_PARENT(am_Condition);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_int,x1);
		REG_MEMBER(_int,y1);
	ENDSAVE;
	virtual bool check(){
		return LastMx>=x && LastMy>=y && LastMx<=x1 && LastMy<y1;
	}
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="If_InRect(%$x$%,%$y$%,%$x1$%,%$y1$%)";
		return true;
	}
};
class am_CheckIfBattle:public am_Condition{
public:
	int AmountOfAttackingUnits;
	SAVE(am_CheckIfBattle);
		REG_PARENT(am_Condition);
		REG_MEMBER(_int,AmountOfAttackingUnits);
	ENDSAVE;
	virtual bool check(){
		itr_UnitsInRadius.Create(LastMx,LastMy,RealLx/2);
		int N=0;
		while(OneObject* OB=itr_UnitsInRadius.Next()){
			if(OB->Attack)N++;
		}
		return (AmountOfAttackingUnits<N);
	}
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="If_Battle(N>%$AmountOfAttackingUnits$%)";
		return true;
	}
};
extern bool GameInProgress;
class am_CheckIfInMenu:public am_Condition{
public:
	SAVE(am_CheckIfInMenu);
		REG_PARENT(am_Condition);
	ENDSAVE;
	virtual bool check(){
		return GameInProgress;
	}
};
class am_CheckIfInGame:public am_Condition{
public:
	SAVE(am_CheckIfInMenu);
	REG_PARENT(am_Condition);
	ENDSAVE;
	virtual bool check(){
		return !GameInProgress;
	}
};
class am_CheckIfWater:public am_Condition{
public:
    float Degree;	
	SAVE(am_CheckIfWater);
		REG_PARENT(am_Condition);
		REG_MEMBER(_float01,Degree);
	ENDSAVE;
	virtual bool check(){
		int WaterPoints=0;
		int MaxWaterPoints=0;
		for(int i=0;i<4;i++){
			char* xi=Rarr[i].xi;
			char* yi=Rarr[i].yi;
			int N=Rarr[i].N;
			for(int j=0;j<N;j++){
				int x=LastMx+int(xi[j])*80;
				int y=LastMy+int(yi[j])*80;
				DWORD GetWaterColor( float x, float y );
				if((GetWaterColor(x,y)>>24)>20)WaterPoints++;
				MaxWaterPoints++;
			}
		}
		return WaterPoints>MaxWaterPoints*Degree;
	}
};
class am_CheckIfForest:public am_Condition{
public:
	float Degree;	
	SAVE(am_CheckIfWater);
		REG_PARENT(am_CheckIfForest);
		REG_MEMBER(_float01,Degree);
	ENDSAVE;    
	virtual bool check(){
        itr_GetSpritesInRadius.Create(LastMx,LastMy,300);
		int N=0;
		while(OneSprite* OS=itr_GetSpritesInRadius.Next()){
            if(OS->SG==&TREES)N++;
		}
		return N>Degree*50;
	}
};
class OneMusicFile:public BaseClass{
public:
	_str FileName;
	SAVE(OneMusicFile);
		REG_FILEPATH(FileName,".ogg");
	ENDSAVE;
	virtual bool ForceSimplification(){
		return true;
	}
};
class OneAmbientSound:public BaseClass{
public:
	int SoundID;
	SAVE(OneAmbientSound);
		REG_ENUM(_index,SoundID,ALL_SOUNDS);
	ENDSAVE;
};
class am_Trigger:public BaseClass{
public:
	ClassPtr<am_Condition> Condition;
	ClonesArray<OneMusicFile> MusicToPlay;
	SAVE(am_Trigger);
		REG_AUTO(Condition);
		REG_AUTO(MusicToPlay);
	ENDSAVE;
};
class am_SoundTrigger:public BaseClass{
public:
	ClassPtr<am_Condition> Condition;
	ClonesArray<OneAmbientSound> AmbientSounds;
	SAVE(am_SoundTrigger);
	REG_AUTO(Condition);
	REG_AUTO(AmbientSounds);
	ENDSAVE;
};
ClonesArray<am_Trigger> MScript;
ClonesArray<am_SoundTrigger> AmSScript;
bool isFinishedMP3();
void ProcessAmbientMusic(){
	if(isFinishedMP3()){
		ClassArray<_str> Names;
		for(int i=0;i<MScript.GetAmount();i++){
			am_Condition* AC=MScript[i]->Condition.Get();
			if( (AC && AC->check()) || !AC){
				for(int j=0;j<MScript[i]->MusicToPlay.GetAmount();j++){
					_str* Name=new _str;
					*Name=MScript[i]->MusicToPlay[j]->FileName;
					Names.Add(Name);
				}			
			}
		}
		if(Names.GetAmount()){
			char* s=Names[rand()%Names.GetAmount()]->pchar();
            void PlayMP3File(char* FileName);
			PlayMP3File(s);
		}
	}
}
void ProcessAmbientSound(){
	for(int i=0;i<AmSScript.GetAmount();i++){
		am_Condition* AC=AmSScript[i]->Condition.Get();
		if( (AC && AC->check()) || !AC){
			int ns=AmSScript[i]->AmbientSounds.GetAmount();
			for(int j=0;j<ns;j++){
				void AddEffectV(int x,int y,int vx,int id);
				AddEffectV(LastMx,LastMy,0,AmSScript[i]->AmbientSounds[j]->SoundID);
			}
		}
	}
}
void AddMScriptEditor(){

	REG_CLASS(am_Condition);
	REG_CLASS(am_AND);
	REG_CLASS(am_OR);
	REG_CLASS(am_NOT);
	REG_CLASS(am_CheckLandType);	
	REG_CLASS(am_CheckIfBattle);
	REG_CLASS(am_CheckIfWater);
	REG_CLASS(am_CheckIfForest);
	REG_CLASS(am_CheckIfInMenu);
	REG_CLASS(am_CheckIfInGame);
	REG_CLASS(am_CheckRect);
	REG_CLASS(am_CheckMapName);

	REG_CLASS(am_Trigger);
	REG_CLASS(am_SoundTrigger);
	REG_CLASS(OneMusicFile);
	REG_CLASS(OneAmbientSound);

	MScript.reset_class(&MScript);
	MScript.SafeReadFromFile("Music\\ambientmusic.xml");
    AddStdEditor("MusicScriptEditor",&MScript,"Music\\ambientmusic.xml",RCE_DEFAULT);
	AmSScript.reset_class(&AmSScript);
	AmSScript.SafeReadFromFile("Sound\\ambientsound.xml");
	AddStdEditor("Ambient sound script editor",&AmSScript,"Sound\\ambientsound.xml",RCE_DEFAULT);
}
