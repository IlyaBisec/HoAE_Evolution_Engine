#include "stdheader.h"
#include "WeaponSystem.h"
#include "unitability.h"
#include "HeroAbility.h"
#include "UF_NumericalReturner.h"
#include "Extensions.h"
#include "..\ClassEngine\ClassSolution.h"
#include ".\cvi_HeroButtons.h"
#include "ua_Ability.h"
void SetActiveProducer();
void SetActiveProducer(OneObject* OB);
//==================================================================================================================//
#define ToRealCoord 0
#define ToPixelCoord (ToRealCoord+4)
//#define TRUETIME (AnimTime*10/64)
#define TRUETIME TrueTime
extern int TrueTime;
extern int AnimTime;
word GetDir(int,int);
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
typedef bool cbCheckSprite(OneSprite* OS, void* Param);
int GetSpritesInRadius(int x, int y, int Radius, cbCheckSprite* cbF, void* Param);
extern HeroVariableStorage* CurrentHeroAbility;
HeroVariableStorage* GetHeroVariableStorage(OneObject* OB);
DLLEXPORT void OBJ_ChangeNation(OneObject* OB, byte DstNat);
extern int GetHeight(int x, int y);
void EraseSprite(int Index);
int GetBar3DHeight(int x,int y);
int GetBar3DOwner(int x,int y);
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);
void PushUnitBack(OneObject* OB,byte OrdType,int Force, int EpicenterX, int EpicenterY);
int GetPointToLineDist(int x,int y,int x1,int y1,int x2,int y2);
void DetonateUnit(OneObject* OB,int CenterX,int CenterY,int Force);
bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
DLLEXPORT int GetRND(int Max);
void MarkObjectLikeModified(OneObject* OB);
//==================================================================================================================//
TargetDesignation::TargetDesignation()
{
	UnitIndex=0xFFFF;
	x=0;
	y=0;
	z=0;
}
//==================================================================================================================//
AdditionalWeaponParams::AdditionalWeaponParams()
{
	Damage=0;
	Radius=0;
	NI=0;
	N=0;
}
//==================================================================================================================//
WeaponParams::WeaponParams()
{
	WeaponModificatorP=NULL;
	OwnerWeaponIndex=-1;
	//Damage=0;
	BirthTime=TRUETIME;
	LastMoveTime=TRUETIME;
	TraveledDistance=0;
	x=0;
	y=0;
	z=0;
	NeedDelete=false;
	Dir=0;
	CheckHero=false;
	HVS=NULL;
	OnceProcesed=false;
	Scale=1;
	Color=0xFF808080;
	ExtraFiDir=0;
	ExtraFiOrt=0;
	ExtraDir=0;
	fiDir=0;
	fiOrt=0;

	TargetIsReached=false;
	ReachedUnitID=0xFFFF;

}
extern bool NOPAUSE;
bool WeaponParams::Process()
{
	if(NOPAUSE){
		CurrentHeroAbility=GetHeroStorage();
		if(WeaponModificatorP==NULL){
			Enumerator* En = ENUM.Get("WeaponModificatorEnum");
			WeaponModificatorP = (WeaponModificator*)(En->Get(WeaponModificatorName.pchar()));
		}
		if((int)WeaponModificatorP!=-1){
			OnceProcesed=true;
			return WeaponModificatorP->Process(this);
		}
	}
	return false;
}
bool WeaponParams::Draw()
{
	if(WeaponModificatorP==NULL&&!WeaponModificatorName.isClear()){
		Enumerator* En = ENUM.Get("WeaponModificatorEnum");
		WeaponModificatorP = (WeaponModificator*)(En->Get(WeaponModificatorName.pchar()));
	}
	if(WeaponModificatorP&&(int)WeaponModificatorP!=-1&&IsOnScreen()){
		return WeaponModificatorP->Draw(this);
	}
	return false;
}
bool WeaponParams::IsOnScreen() {
	bool CheckObjectVisibility(int x,int y,int z,int R);

	int xx=x>>ToPixelCoord;
	int yy=y>>ToPixelCoord;
	int zz=z>>ToPixelCoord;

	if( OnceDrawed[MyNation] || CheckObjectVisibility(xx,yy,zz,300) ){
		// если Weapon хоть раз побывал за пределами экрана, он уже не отрисовывается если DisableIfOutOfScreen
		//OutOfScreen[MyNation]=false;	
		//
		OneObject* MyOB=NULL;
		if( From.UnitIndex!=0xFFFF ){
			MyOB=Group[From.UnitIndex];
			if( MyOB ){
				byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
				if(MyOB->NNUM!=NI){
					MyOB=NULL;
				}
			}
		}
		if( MyOB || GetObjectVisibilityInFog(xx,yy,zz,NULL) ){
			OnceDrawed[MyNation]=true;
			return true;
		}
	}else{
		if(OnceProcesed)
			OutOfScreen[MyNation]=true;
	}
	return false;
}
HeroVariableStorage* WeaponParams::GetHeroStorage()
{
	HeroVariableStorage* rez=HVS;
	if(!CheckHero)
	{
		if (From.UnitIndex>=0 && From.UnitIndex!=0xFFFF) {
			HVS=GetHeroVariableStorage(Group[From.UnitIndex]);
		}
		CheckHero=true;
	}
	return rez;
}
//==================================================================================================================//
bool PointModificator::MakeOneStep(WeaponParams* WP)
{
	return false;
}
bool PointModificator::CanDraw(WeaponParams* WP)
{
	return false;
}
bool PointModificator::Draw(WeaponParams* WP)
{
	return false;
}
//==================================================================================================================//
bool WeaponEvent::Check(WeaponParams* WP)
{
	return false;
}
//==================================================================================================================//
int WeaponModificatorList::GetExpansionRules()
{
	return 2; 
}
bool WeaponProcess::Check(WeaponParams* WP)
{
	if(Event.Get())
	{
		return Event.Get()->Check(WP);
	}
	return false;
}
bool WeaponProcess::Process(WeaponParams* WP)
{
	bool rez=false;
	int n = WeaponModificators.GetAmount();
	for(int i=0;i<n;i++)
	{
		bool prr = WeaponModificators[i]->MakeOneStep(WP);
		if(prr&&(!rez))rez=true;
	}
	return rez;
}
bool WeaponProcess::CanDraw(WeaponParams* WP)
{
	bool rez=false;
	int n = WeaponModificators.GetAmount();
	for(int i=0;i<n;i++)
	{
		bool prr = WeaponModificators[i]->CanDraw(WP);
		if(prr&&(!rez))rez=true;
	}
	return rez;
}
bool WeaponProcess::Draw(WeaponParams* WP)
{
	bool rez=false;
	int n = WeaponModificators.GetAmount();
	for(int i=0;i<n;i++)
	{
		bool prr = WeaponModificators[i]->Draw(WP);
		if(prr&&(!rez))rez=true;
	}
	return rez;
}
//==================================================================================================================//
WeaponModificator::WeaponModificator()
{
	WOwner=NULL;
	g_beOBJ_CONTROLER.SetState("WeaponModificators");
}
WeaponModificator::~WeaponModificator(){
	g_beOBJ_CONTROLER.SetState("WeaponModificators");
};
bool WeaponModificator::Process(WeaponParams* WP)
{
	
	word CasterIndex=0xFFFF;
	word CasterSerial;
	
	if( WP->From.UnitIndex!=0xFFFF ){
		OneObject* OB=Group[WP->From.UnitIndex];
		if(OB){
			CasterIndex=OB->Index;
			CasterSerial=OB->Serial;
		}
	}else
	if( WP->To.UnitIndex>=0 && WP->To.UnitIndex!=0xFFFF ){
		OneObject* OB=Group[WP->To.UnitIndex];
		if(OB){
			CasterIndex=OB->Index;
			CasterSerial=OB->Serial;
		}
	}

	if( CasterIndex!=0xFFFF ){
		GlobalUnitsContext::setCaster(CasterIndex,CasterSerial);
	}else{
		GlobalUnitsContext::Reset();
	}

	bool rez=false;
	int n = WPL.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(WPL[i]->Check(WP))
		{
			bool prr = WPL[i]->Process(WP);
			if(prr&&(!rez))rez=true;
		}
	}
	return rez;
}
bool WeaponModificator::Draw(WeaponParams* WP)
{
	bool rez=false;
	if(WP->OnceProcesed){
		int n = WPL.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(WPL[i]&&WPL[i]->CanDraw(WP)&&WPL[i]->Check(WP))
			{
				bool prr = WPL[i]->Draw(WP);
				if(prr&&(!rez))rez=true;
			}
		}
	}
	return rez;
}
//bool WeaponEventPtr::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
//	dest="Event: %$this$%";
//	return true;
//};

const char* WeaponModificator::GetThisElementView(const char* LocalName){
	if(Name.pchar()){
		static char cc[256];
		sprintf(cc,"%s: {CW}%s{C}",LocalName,Name.pchar());
		return cc;
	}else return LocalName;
}

//bool WeaponModificator::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
//	//if(Return) 
//	//	dest="Plymorph: Return";
//	//else 
//		dest="WeaponModificator: %$this$%";
//	return true;
//}

//bool WeaponSystem::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
//	if(!ClassEditor::CheckIfOpen(this)){
//	}else{
//		dest="";
//	}
//	dest="WeaponModificator: %$this$%";
//};
bool WeaponProcess::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	if(!ClassEditor::CheckIfOpen(this)){
		dest="if( %$Event$% )/   %$WeaponModificators,/$%";
	}else{
		dest="if";
	}
	return true;
}
bool TerritoryLockedForUnits::Check(WeaponParams* WP){
    int rr=RadiusForCheck/8+1;
	if(rr>20)rr=20;
	return MFIELDS[0].CheckRound( (WP->x>>(ToPixelCoord+4))-rr/2,(WP->y>>(ToPixelCoord+4))-rr/2,rr);
}
bool TerritoryLockedForShips::Check(WeaponParams* WP){
	int rr=RadiusForCheck/8+1;
	if(rr>20)rr=20;
	return MFIELDS[1].CheckRound( (WP->x>>(ToPixelCoord+4))-rr/2,(WP->y>>(ToPixelCoord+4))-rr/2,rr);
}
bool WaterIsThere::Check(WeaponParams* WP){
	int H=GetTotalHeight(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord);
	return (H<3);
}
//==================================================================================================================//
WeaponSystem::WeaponSystem()
{
	LastSerial=0;
}
void WeaponSystem::RefreshEnumerator()
{
	int n=AllWeaponModificators.GetAmount();
	Enumerator* En = ENUM.Get("WeaponModificatorEnum");
	En->Clear();
	//Weapons.Clear();
	for(int i=0;i<n;i++)
	{
		if(AllWeaponModificators[i]->Name.L)
		{
			En->Add(AllWeaponModificators[i]->Name.pchar(),(DWORD)(AllWeaponModificators[i]));
			/*
			if(i<Weapons.GetAmount())
			{
				Weapons[i].Modificator=AllWeaponModificators[i];
			}
			else
			{
				Weapon W;
				W.Modificator=AllWeaponModificators[i];
				Weapons.Add(W);
			}
			*/
		}
	}
	n = ActiveWeapons.GetAmount();
	for(int i=0;i<n;i++)
	{
		ActiveWeapons[i]->WeaponModificatorP=NULL;
	}
}
void WeaponSystem::Process()
{
	int n=ActiveWeapons.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(ActiveWeapons[i]&&(!ActiveWeapons[i]->NeedDelete))
		{
			if(ActiveWeapons[i]->BirthTime<TRUETIME)
				ActiveWeapons[i]->Process();
		}
		else
		{
			if(ActiveWeapons[i])
			{
				delete (ActiveWeapons[i]);
				ActiveWeapons[i]=NULL;
			}
			ActiveWeapons.Del(i,1);
			i--;
			n--;
		}
	}
}
void WeaponSystem::Draw()
{
	int n=ActiveWeapons.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(ActiveWeapons[i]&&(!ActiveWeapons[i]->NeedDelete))
			ActiveWeapons[i]->Draw();
	}
}
bool WeaponSystem::LoadAllWeaponModificators(char* FileName)
{
	if(FileName)
	{
		AllWeaponModificators.Clear();
		AllWeaponModificators.SafeReadFromFile(FileName);
		RefreshEnumerator();
		return true;
	}
	return false;
}
bool _util_LoadClassLikeFolder(BaseClass* BC,const char* xmlName,const char* FieldName);
void _util_SaveClassLikeFolder(BaseClass* BC,const char* xmlName,const char* FieldName);

bool WeaponArray::SafeReadFromFile(const char* Name){
    _util_LoadClassLikeFolder(this,Name,"Name");
	return true;
}
bool WeaponArray::WriteToFile(const char* Name){
	_util_SaveClassLikeFolder(this,Name,"Name");
	return true;    
}
bool WeaponArray::ReadFromFile(const char* Name){
	_util_LoadClassLikeFolder(this,Name,"Name");
	return true;
}
void WeaponSystem::ClearAllActiveWeapons()
{
	ActiveWeapons.Clear();
}
void WeaponSystem::AddActiveWeapon(WeaponParams* W)
{
	int z=W->z>>ToPixelCoord;
    int x=W->x>>ToPixelCoord;
	int y=W->y>>ToPixelCoord;
	int H=GetTotalHeight(x,y);
	if(H<0)H=0;
	if(z<H)W->z=H<<ToPixelCoord;
	W->Serial=++LastSerial;
	ActiveWeapons.Add(W);
}
//==================================================================================================================//
//==================================================================================================================//
WeaponSystem GameWeaponSystem;
BaseClass* GetWeaponClass(){
	return &GameWeaponSystem.AllWeaponModificators;
}
bool ProcessWeaponClass(ClassEditor* CE,BaseClass* BC,int Options){
	if(Options==2)GameWeaponSystem.RefreshEnumerator();
	return false;
}
ClassSolution ABLWEAP;
extern AbilityList Abilities;
bool ProcessAbilityClass(ClassEditor* CE,BaseClass* BC,int Options);
void RegAW_Solution(){
	ABLWEAP.AddClonesArray(&GameWeaponSystem.AllWeaponModificators,"WeaponSystem\\base.ws.xml");
	ABLWEAP.AddClonesArray(&Abilities.Abilities,"UnitsAbility\\base.ua.xml");
	AddStdEditor("Solution",&ABLWEAP,"UnitsAbility\\Solution.uaws.xml",RCE_DEFAULT,ProcessAbilityClass,NULL,'L');
}
//==================================================================================================================//
void DrawWeaponSystem()
{
	GameWeaponSystem.Draw();
} 
void ProcessWeaponSystem()
{
	GameWeaponSystem.Process();
}
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams)
{
	WeaponParams* P = new WeaponParams();
	//P->WeaponModificatorP=Weap->Modificator;
	P->WeaponModificatorName.Clear();
	P->WeaponModificatorName.Add(WMName);
	P->From.UnitIndex=Index;
	if(Index==0xFFFF)P->From.UnitIndex=0xFFFF;
	P->From.x=sx<<ToPixelCoord;
	P->From.y=sy<<ToPixelCoord;
	P->From.z=sz<<ToPixelCoord;

	P->x=P->From.x;
	P->y=P->From.y;
	P->z=P->From.z;

	P->AdditionalParams.Set(AddParams);
	//P->Damage=Damage;
	//P->AttType=AttType;

	OneObject* OB = Group[P->From.UnitIndex];

	P->To.UnitIndex=DestIndex;
	if(DestIndex==0xFFFF)
	{
		P->To.UnitIndex=0xFFFF;
		if( P->From.UnitIndex!=0xFFFF ){
			if( OB && OB->EnemyID!=0xFFFF ){
				P->To.UnitIndex=OB->EnemyID;
			}
		}
	}
	P->To.x=dx<<ToPixelCoord;
	P->To.y=dy<<ToPixelCoord;
	P->To.z=dz<<ToPixelCoord;

	if( dx-sx==0 && dy-sy==0 && OB ){
		P->VDir=OB->RealDir;
	}else{
		P->VDir=GetDir(dx-sx,dy-sy);
	}
	P->Dir=P->VDir;

	GameWeaponSystem.AddActiveWeapon(P);
}
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, int Damage, int AttType)
{
	AdditionalWeaponParams* AddParams = new  AdditionalWeaponParams();
	AddParams->Damage=Damage;
	AddParams->AttType=AttType;
	CreateNewActiveWeapon(WMName,Index,sx,sy,sz,DestIndex,dx,dy,dz,AddParams);
}
Weapon* GetWeaponWithModificator(char* Name);
void CalculateDestXYZ(BalisticMotion* bm,int time,int& x,int& y,int& z,int spx,int spy,int spz){
    int n=sqrt(spx*spx+spy*spy+spz*spz);
    int nxy=sqrt(spx*spx+spy*spy);
    if(!nxy)nxy=1;
    int vx=bm->ConstSpeed*spx/nxy;
    int vy=bm->ConstSpeed*spy/nxy;
    int vz=bm->ConstSpeed*spz/nxy;
    x+=vx*time/100;
    y+=vy*time/100;
    z+=vz*time/100-bm->g*time*time/20000;
}
Vector3D ShowWeaponDestination(char* WMName,int sx, int sy, int sz, int speedX, int speedY, int speedZ){
    Weapon* W=GetWeaponWithModificator(WMName);
    if(W && W->Modificator){
        WeaponModificator* WM=W->Modificator->Get();
        if(WM){
            for(int i=0;i<WM->WPL.GetAmount();i++){
                WeaponProcess* WP=WM->WPL[i];
                if(WP){
                    for(int j=0;j<WP->WeaponModificators.GetAmount();j++){
                        PointModificator* PM=WP->WeaponModificators[j];
                        if(PM){
                            BalisticMotion* bm=dynamic_cast<BalisticMotion*>(PM);
                            if(bm){
                                int t=10;
                                int xx,yy,zz;
                                do{
                                    xx=sx<<4;
                                    yy=sy<<4;
                                    zz=sz<<4;                                    
                                    CalculateDestXYZ(bm,t,xx,yy,zz,speedX,speedY,speedZ);
                                    int h=GetHeight(xx>>4,yy>>4)<<4;
                                    if(zz<h){
                                        do{
                                            xx=sx<<4;
                                            yy=sy<<4;
                                            zz=sz<<4;                                    
                                            CalculateDestXYZ(bm,t,xx,yy,zz,speedX,speedY,speedZ);
                                            int h=GetHeight(xx>>4,yy>>4)<<4;
                                            if(zz<=h){
                                                xx>>=4;
                                                yy>>=4;
                                                zz>>=4;                                        
                                                return Vector3D(xx,yy,zz);
                                            }
                                            t--;
                                        }while(t>0);
                                        return Vector3D(xx,yy,zz);
                                    }
                                    t+=20;
                                }while(t<2000);                                                                
                            }
                        }
                    }
                }
            }
        }
    }
    return Vector3D(0,0,0);
}
//trajectory and final point will be calculated by WMName, but WpToCreate will be created
void CreateNewActiveWeaponByDirection(char* WMName,char* WpToCreate,int Index,int sx, int sy, int sz, int DestIndex, int speedX, int speedY, int speedZ, int Damage, int AttType){
    Weapon* W=GetWeaponWithModificator(WMName);
	Weapon* W2=GetWeaponWithModificator(WpToCreate);
    if(W && W2 && W->Modificator){
        WeaponModificator* WM=W->Modificator->Get();
        if(WM){
            for(int i=0;i<WM->WPL.GetAmount();i++){
                WeaponProcess* WP=WM->WPL[i];
                if(WP){
                    for(int j=0;j<WP->WeaponModificators.GetAmount();j++){
                        PointModificator* PM=WP->WeaponModificators[j];
                        if(PM){
                            BalisticMotion* bm=dynamic_cast<BalisticMotion*>(PM);
                            if(bm){
                                int t=10;
                                int xx,yy,zz;
                                do{
                                    xx=sx<<4;
                                    yy=sy<<4;
                                    zz=sz<<4;
                                    CalculateDestXYZ(bm,t,xx,yy,zz,speedX,speedY,speedZ);
                                    int h=GetHeight(xx>>4,yy>>4)<<4;
                                    if(zz<h){
                                        do{
                                            xx=sx<<4;
                                            yy=sy<<4;
                                            zz=sz<<4;
                                            CalculateDestXYZ(bm,t,xx,yy,zz,speedX,speedY,speedZ);
                                            int h=GetHeight(xx>>4,yy>>4)<<4;
                                            if(zz>=h){
                                                xx>>=4;
                                                yy>>=4;
                                                zz>>=4;
                                                CreateNewActiveWeapon(WpToCreate,Index,sx,sy,sz,DestIndex,xx,yy,h>>4,Damage,AttType);
                                                return;
                                            }
                                            t--;
                                        }while(t>0);
                                        return;
                                    }
                                    t+=20;
                                }while(t<2000 && Norma(sx-xx/16,sy-yy/16)<6000);                                
                                CreateNewActiveWeapon(WpToCreate,Index,sx,sy,sz,DestIndex,xx>>4,yy>>4,zz>>4,Damage,AttType);
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}
void CreateNewActiveWeapon(Weapon* Weap,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, int Damage, int AttType)
{
	if(Weap->Modificator&&Weap->Modificator->Get())
		CreateNewActiveWeapon(Weap->Modificator->Get()->Name.pchar(),Index,sx,sy,sz,DestIndex,dx,dy,dz,Damage,AttType);
}

void ClearWeapons(){
	GameWeaponSystem.AllWeaponModificators.Clear();
}
bool LoadAllWeaponModificators(char* FileName)
{
	return GameWeaponSystem.LoadAllWeaponModificators(FileName);
}
Weapon* GetWeaponWithModificator(char* Name)
{
	/*
	Enumerator* En = ENUM.Get("WeaponModificatorEnum");
	if(En)
	{
		WeaponModificator* WM = (WeaponModificator*)(En->Get(Name));
		if((int)WM!=-1)
		{
			int n=GameWeaponSystem.Weapons.GetAmount();
			for(int i=0;i<n;i++)
			{
				if(GameWeaponSystem.Weapons[i].Modificator==WM)
					return &GameWeaponSystem.Weapons[i];
			}
		}
	}
	return NULL;
	*/
	Weapon* rez=NULL;
	int n=GameWeaponSystem.Weapons.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(!strcmp(GameWeaponSystem.Weapons[i]->Modificator->GetObjectName(),Name))
		{
			rez=GameWeaponSystem.Weapons[i];
			break;
		}
	}
	if(!rez)
	{
		Weapon* W = new Weapon();
		GameWeaponSystem.Weapons.Add(W);
		rez=GameWeaponSystem.Weapons[n];
		rez->Modificator = new ClassRef<WeaponModificator>;
		rez->Modificator->SetObjectName(Name);
		WeaponModificator* WM=rez->Modificator->Get();
		if(WM)WM->WOwner=W;

	}
	return rez;
}
Weapon* GetWeaponFromModificator(WeaponModificator* WM){
	if(WM->WOwner)return WM->WOwner;
	Weapon* W = new Weapon();	
	int n=GameWeaponSystem.Weapons.GetAmount();
	GameWeaponSystem.Weapons.Add(W);
	W=GameWeaponSystem.Weapons[n];
	W->Modificator = new ClassRef<WeaponModificator>;
	W->Modificator->Set(WM);    		
	WM->WOwner=W;
	return W;
}
int GetNNewWeap(){
	return GameWeaponSystem.Weapons.GetAmount();
}
char* GetNewWeapName(int idx){
	return (char*)GameWeaponSystem.Weapons[idx]->Modificator->GetObjectName();
}
int GetNewWeapIdx(char* name){
	int n=GameWeaponSystem.Weapons.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(!strcmp(GameWeaponSystem.Weapons[i]->Modificator->GetObjectName(),name))return i;		
	}
	return -1;
}
Weapon* GetNewWeaponPtr(int idx){
	return GameWeaponSystem.Weapons[idx];
}
//==================================================================================================================//
//extern int ItemChoose;
bool MMItemChoose(SimpleDialog* SD);

void ProcessWeaponSystemEditor(){
	return;
	xmlQuote xml;
	ItemChoose=-1;
	if(xml.ReadFromFile("Dialogs\\WeaponSystem.DialogsSystem.xml")){
		DialogsSystem DSS;
		ErrorPager EP;
		DSS.Load(xml,&DSS,&EP);
		SimpleDialog* Desk=DSS.Find("OptionsDesk");
		SimpleDialog* OK=DSS.Find("OK");
		SimpleDialog* CANCEL=DSS.Find("CANCEL");
		if(Desk&&OK&&CANCEL){
			int x0,y0,x1,y1;
			DSS.GetDialogsFrame(x0,y0,x1,y1);
			if(x1>x0){
				DSS.x=(RealLx-x1+x0)/2;
				DSS.y=(RealLy-y1+y0)/2;
				OK->OnUserClick=&MMItemChoose;
				OK->UserParam=1;
				CANCEL->OnUserClick=&MMItemChoose;
				CANCEL->UserParam=1;
				ClassEditor CE;
				CE.CreateFromClass(Desk,0,0,Desk->x1-Desk->x,Desk->y1-Desk->y,&GameWeaponSystem,3,"EmptyBorder");
				do{
					GameWeaponSystem.RefreshEnumerator();
					ProcessMessages();					
					DSS.ProcessDialogs();
					CE.Process();
					DSS.RefreshView();
				}while(ItemChoose==-1);
			}
		}
	}	
}
//==================================================================================================================//
//==================================================================================================================//
bool DrawOne::CanDraw(WeaponParams* WP)
{
	//return WP->IsOnScreen();
	return true;
}
void AddAnimation(float x,float y,float z,NewAnimation* ANM,int Frame,float Dir,DWORD Diffuse,OneObject* OB,float Scale,float fiDir,float fiOrt,DWORD Handle);
void AddAnmTarget(Vector3D Dest);
bool DrawOne::Draw(WeaponParams* WP)
{
	if( DisableIfOutOfScreen && WP->OutOfScreen[MyNation] || !Anim.Enabled )
		return false;

	float Scale=1.0;
	if(ScaleToRadius!=0&&WP->AdditionalParams.Get()&&WP->AdditionalParams.Get()->Radius!=0){
		Scale=((float)WP->AdditionalParams.Get()->Radius)/((float)ScaleToRadius);
	}

	DWORD handle=WP->From.UnitIndex<<7;
	handle+=0x12340000+WP->Serial*7123+Anim.ModelID*7791+DrawingPosition*761+TargetOfEffect*19751;
    handle+=WP->BirthTime*724161;
	Vector3D CP;

	DynArray<Line3D>* GetActiveBones(int UIndex,char* BoneMask);
	if(AttachStartPointToBone){
		DynArray<Line3D>* DA=GetActiveBones(WP->From.UnitIndex,BoneNameMask.pchar());
		int N;
		if(N=DA->GetAmount()>0){
			for(int i=0;i<N;i++){
				Line3D L=(*DA)[i];                
				CP=L.getOrig();
				if(DrawingPosition==0){//current position
                    Vector3D st(float(WP->From.x)/16.0f,float(WP->From.y)/16.0f,float(WP->From.z)/16.0f);
					Vector3D fin(float(WP->To.x)/16.0f,float(WP->To.y)/16.0f,float(WP->To.z)/16.0f);
					Vector3D cur(float(WP->x)/16.0f,float(WP->y)/16.0f,float(WP->z)/16.0f);
					Vector3D s0=cur;
                    s0-=st;
					Vector3D s1=fin;
					s1-=st;
					float n0=s0.norm();
					float n1=s1.norm();
					if(n0>1 && n1>1){
						float coef=s1.dot(s0)/n1/n1;
						fin*=coef;
						st*=1-coef;
						st+=fin;
						st-=cur;
						CP*=1-coef;
						CP+=fin;
						CP+=st;
					}
				}else if(DrawingPosition==3){//destination pos
					CP=Vector3D(Vector3D(float(WP->To.x)/16.0f,float(WP->To.y)/16.0f,float(WP->To.z)/16.0f));                    
				}
				if(UseDirectionFromBone){
					Vector3D E=L.getDir();
					E*=256;
					byte D=GetDir(E.x,E.y);
					float sx=sqrtf(E.x*E.x+E.y*E.y);
					float a=-atan2(E.z,sx);
					AddAnimation(CP.x,CP.y,CP.z,&Anim,Frame,
						D,WP->Color,NULL,Scale*WP->Scale,a,0, 
						handle+77751*(i+1));
				}else{
					AddAnimation(CP.x,CP.y,CP.z,&Anim,Frame,
						int(WP->Dir+WP->ExtraDir)%255,WP->Color,NULL,Scale*WP->Scale,
						WP->fiDir+WP->ExtraFiDir, WP->fiOrt+WP->ExtraFiOrt, 
						handle+77751*(i+1));
				}
				if(TargetOfEffect){		
					if(TargetOfEffect==1)AddAnmTarget(Vector3D(float(WP->To.x)/16.0f,float(WP->To.y)/16.0f,float(WP->To.z)/16.0f));
					else AddAnmTarget(Vector3D(float(WP->From.x)/16.0f,float(WP->From.y)/16.0f,float(WP->From.z)/16.0f));
				}
				Anim.Code = 0xBAADF00D;
			}
		}
		return true;
	}
	switch(DrawingPosition){		
		case 1://SourcePosition
			CP=Vector3D(float(WP->From.x)/16.0f,float(WP->From.y)/16.0f,float(WP->From.z)/16.0f);
			break;
		case 2://SourceUnit
			CP=Vector3D(Vector3D(float(WP->From.x)/16.0f,float(WP->From.y)/16.0f,float(WP->From.z)/16.0f));
			if(WP->From.UnitIndex!=0xFFFF){
				OneObject* OB=Group[WP->From.UnitIndex];
				if(OB && !OB->Sdoxlo){
					AddAnmTarget(Vector3D(float(OB->RealX)/16.0f,float(OB->RealY)/16.0f,float(OB->RZ+OB->OverEarth+OB->newMons->SrcZPoint)));
				}
			}
			break;
		case 3://DestinationPosition
			CP=Vector3D(Vector3D(float(WP->To.x)/16.0f,float(WP->To.y)/16.0f,float(WP->To.z)/16.0f));
			break;
		default://CurrentPosition
			CP=Vector3D(float(WP->x)/16.0f,float(WP->y)/16.0f,float(WP->z)/16.0f);
			break;
	}

	int frame=Frame;
	if(Anim.NFrames){
		int dT=TRUETIME-WP->BirthTime;
		frame=(dT/40)%Anim.NFrames;
		frame*=256;
	}

	AddAnimation(CP.x,CP.y,CP.z,&Anim,frame,
					int(WP->Dir+WP->ExtraDir)%255,WP->Color,NULL,Scale*WP->Scale,
                    WP->fiDir+WP->ExtraFiDir, WP->fiOrt+WP->ExtraFiOrt, 
                    handle);
	if(TargetOfEffect){		
		if(TargetOfEffect==1)AddAnmTarget(Vector3D(float(WP->To.x)/16.0f,float(WP->To.y)/16.0f,float(WP->To.z)/16.0f));
		else AddAnmTarget(Vector3D(float(WP->From.x)/16.0f,float(WP->From.y)/16.0f,float(WP->From.z)/16.0f));
	}
    Anim.Code = 0xBAADF00D;
	return true;
}
//==================================================================================================================//
bool SelfMurder::MakeOneStep(WeaponParams* WP)
{
	if (WP) 
	{
		WP->NeedDelete=true;
		return true;
	}
	return false;
}
//==================================================================================================================//
bool ChangeAttachedEffectMask::MakeOneStep(WeaponParams* WP)
{
    if (WP) 
    {
        word ID=WP->From.UnitIndex;
        if(ID!=0xFFFF){
            OneObject* OB=Group[ID];
            if(OB)OB->AttachedEffectMask = Mask;
        }
        return true;
    }
    return false;
}
//==================================================================================================================//
StaticMotion::StaticMotion()
{
	Vx=0;
	Ax=0;
	Vy=0;
	Ay=0;
	Vz=0;
	Az=0;
	FirstStep=true;
}
bool StaticMotion::MakeOneStep(WeaponParams* WP)
{
	Dir=GetDir(WP->To.x-WP->From.x,WP->To.y-WP->From.y); 
	int n=Norma(WP->To.x-WP->From.x,WP->To.y-WP->From.y);
	int DirZ=GetDir(n,WP->To.z-WP->From.z);

	Vmx=(Vx*TCos[Dir]+Vy*TSin[Dir])>>8;
	Vmy=(Vx*TSin[Dir]-Vy*TCos[Dir])>>8;
	Amx=(Ax*TCos[Dir]+Ay*TSin[Dir])>>8;
	Amy=(Ax*TSin[Dir]-Ay*TCos[Dir])>>8;
	
	int ti=TRUETIME-WP->BirthTime;
	
	int Vxx=Vmx+Amx*ti/100;
	int Vyy=Vmy+Amy*ti/100;
	int nV=Norma(Vxx,Vyy);
	int Vzz=Vz+Az*ti/100;
	WP->V=Norma(Vzz,nV);
	Vzz+=((nV*TCos[DirZ])>>8);
	WP->Dir=GetDir(Vxx,Vyy);
	WP->DirZ=DirZ;

	WP->x=WP->From.x+Vmx*ti/100+Amx*ti*ti/20000;
	WP->y=WP->From.y+Vmy*ti/100+Amy*ti*ti/20000;
	WP->z=WP->From.z+Vzz*ti/100+Az*ti*ti/20000;
	WP->LastMoveTime=TRUETIME;
	return true;
}
//==================================================================================================================//
BalisticMotion::BalisticMotion()
{
	ConstSpeed=300;
	ConstHieght=10;
	g=5;
	SetTargetHieghtOnGround=false;
	StopInDestPoint=false;
	StopOnGround=false;
	HorizontalFly=false;
}
bool BalisticMotion::MakeOneStep(WeaponParams* WP)
{
	byte Dir=GetDir(WP->To.x-WP->From.x,WP->To.y-WP->From.y); 
	byte Dir2=GetDir(WP->To.x-WP->x,WP->To.y-WP->y);

    int DX=WP->To.x-WP->From.x;
    int DY=WP->To.y-WP->From.y;
    int N=sq_norma(DX,DY);
    if(N>0){
        DX=DX*1024/N;
        DY=DY*1024/N;
    }	
	int D=abs(char(Dir-Dir2));

	float sq_norma(float x,float y);
	int S=sq_norma(WP->To.x-WP->From.x,WP->To.y-WP->From.y);
	
	if( StopOnGround ){
		int H=GetHeight(WP->x>>4,WP->y>>4)<<4;
		if( WP->z <= H ){
			WP->z=H;
			return true;
		}
	}

	if(S==0&&StopInDestPoint)
		return true;

	int Vmx=0; //(Vx*TCos[Dir]+Vy*TSin[Dir])>>8;
	int Vmy=0; //(Vx*TSin[Dir]-Vy*TCos[Dir])>>8;
	int Vmz=0;
	int Amz=-g;

    int ti=TRUETIME-WP->BirthTime;
	int MaxFlyTime=0;

    int V = 0;
    float Vz = 0.0f;
	if (SetTargetHieghtOnGround) 
	{
		WP->To.z=GetHeight(WP->To.x>>4,WP->To.y>>4)<<4;
		if (WP->To.z < 0) WP->To.z = 0;
	}

	if( ConstSpeed )
	{
		int mt=S*100/ConstSpeed;
		if(mt==0)
			mt=1;
		Vmz=g*mt/200+100*(WP->To.z-WP->From.z)/mt;
		Vmx=(ConstSpeed*DX)>>2;//>>8;
		Vmy=(ConstSpeed*DY)>>2;//>>8;
        V  = ConstSpeed;
        //  current z velocity component
        Vz = Vmz + Amz*ti/100;
		
		MaxFlyTime=mt;
	} 
    else 
	if( ConstHieght )
	{
		int t=sqrt(2*ConstHieght/g);
		if(!t)
			t=1;
		Vmz=ConstHieght/t+(WP->To.z-WP->From.z)/t;
		V=S/t+(WP->To.z-WP->From.z)/t;
		Vmx=(V*TCos[Dir]);//>>8;
		Vmy=(V*TSin[Dir]);//>>8;

		MaxFlyTime=t*2;
	}
	
	WP->x = WP->From.x + ((Vmx*ti/100)>>8);
	WP->y = WP->From.y + ((Vmy*ti/100)>>8);
	if(HorizontalFly){
		WP->z = WP->From.z;
	}else{
		WP->z = WP->From.z + Vmz*ti/100 + Amz*ti*ti/20000;
	}
	

	// perelet
	if( ti>MaxFlyTime || D>64 ){
		if(StopInDestPoint){
			WP->x = WP->To.x;
			WP->y = WP->To.y;
			WP->z = WP->To.z;
		}
		WP->TargetIsReached=true;
	}
    
    //  4.08.2004, by Silver 
    WP->fiDir = c_DoublePI - atan2f( Vz, static_cast<float>( V ) );
    WP->fiOrt = 0.0f;  // assume missile is not rotating around its axis 

    WP->LastMoveTime=TRUETIME;
	WP->Dir=GetDir(Vmx,Vmy);
	//WP->DirZ=GetDir(Norma(Vmx*ti/100,Vmy*ti/100),Vmz*ti/100+Amz*ti*ti/20000);
	return true;
}
pm_Roam::pm_Roam(){
	MaxSpeed=200;
}
bool pm_Roam::MakeOneStep(WeaponParams* WP){
	
	//word Dir=GetDir(WP->To.x-WP->From.x,WP->To.y-WP->From.y);

	int rnd=rando();
	int com=rnd%4;
	
	int turn=rnd%15;
	int acc=rnd%30;

	switch(com){
		case 0: // turn left
			WP->VDir+=turn;
			if(WP->VDir>255){
				WP->VDir-=256;
			}
			WP->V-=turn/2;
			if(WP->V<0){
				WP->V=2;
			}
			break;
		case 1: // turn right
			WP->VDir-=turn;
			if(WP->VDir<0){
				WP->VDir+=256;
			}
			WP->V-=turn/2;
			if(WP->V<0){
				WP->V=2;
			}
			break;
		case 2: // accelerate
			WP->V+=acc;
			if(WP->V>MaxSpeed){
				WP->V=MaxSpeed;
			}
			break;
	}
	
	int Vmx=(WP->V*TCos[WP->VDir])>>8;
	int Vmy=(WP->V*TSin[WP->VDir])>>8;

	WP->x+=Vmx;
	WP->y+=Vmy;

	return true;
}
//==================================================================================================================//
Jump::Jump()
{
	JumpDist=0;
	JumpToEnd=true;
}
bool Jump::MakeOneStep(WeaponParams* WP)
{
	bool rez=false;
	if(WP)
	{
		if(JumpToEnd){
			word tID=WP->To.UnitIndex;
			OneObject* tOB=Group[tID];
			if( DestinationIsObject && tID!=0xFFFF && tOB ){
				WP->x=tOB->RealX;
				WP->y=tOB->RealY;
				WP->z=GetHeight(WP->x>>4,WP->y>>4)<<4;
			}else{
				WP->x=WP->To.x;
				WP->y=WP->To.y;
				if(SetTargetHeightOnGround){
					WP->z=GetHeight(WP->x>>4,WP->y>>4)<<4;
					if (WP->z < 0) WP->z = 0;
				}else{
					WP->z=WP->To.z;
				}
			}
		}else if(JumpDist){
			int dx=WP->To.x-WP->x;
			int dy=WP->To.y-WP->y;
			int dz=WP->To.z-WP->z;
			int sd=abs(dx)+abs(dy)+abs(dz);
			int ReD=JumpDist<<ToPixelCoord;
			dx=(dx*ReD)/sd;
			dy=(dy*ReD)/sd;
			dz=(dz*ReD)/sd;
			WP->x=WP->x+dx;
			WP->y=WP->y+dy;
			if(SetTargetHeightOnGround){
				WP->z=GetHeight(WP->x>>4,WP->y>>4)<<4;
				if (WP->z < 0) WP->z = 0;
			}else{
				WP->z=WP->z+dz;
			}
		}
		rez=true;
	}
	return rez;
}
//==================================================================================================================//
bool Motion::MakeOneStep(WeaponParams* WP)
{
	int ti=TRUETIME-WP->LastMoveTime;
	WP->x=WP->x+((WP->V*ti*TCos[WP->Dir])>>8)/100;
	WP->y=WP->y+((WP->V*ti*TSin[WP->Dir])>>8)/100;
	WP->z=WP->z+((WP->V*ti*TSin[WP->DirZ])>>8)/100;
	WP->LastMoveTime=TRUETIME;
	return true;
}
//==================================================================================================================//
HarmonicMotion::HarmonicMotion()
{
	Hx=0;
	Tx=0;
	Dx=0;
	Hy=0;
	Ty=0;
	Dy=0;
	Hz=0;
	Tz=0;
	Dz=0;
}
bool HarmonicMotion::MakeOneStep(WeaponParams* WP)
{
	int ti=TRUETIME-WP->BirthTime;
	int dx=0;
	if(Tx)
	{
		byte d=(byte)(((ti%Tx)*256)/Tx+Dx);
		dx=(TSin[d]*Hx)>>8;
	}
	int dy=0;
	if(Ty)
	{
		byte d=(byte)(((ti%Ty)*256)/Ty+Dy);
		dy=(TSin[d]*Hy)>>8;
	}
	int dz=0;
	if(Tz)
	{
		byte d=(byte)(((ti%Tz)*256)/Tz+Dz);
		dz=(TSin[d]*Hz)>>8;
	}

	int dxx=(dx*TCos[WP->Dir]+dy*TSin[WP->Dir])>>8;
	int dyy=(dx*TSin[WP->Dir]-dy*TCos[WP->Dir])>>8;

	WP->x+=dxx;
	WP->y+=dyy;
	WP->z+=dz;
	return true;
}
//==================================================================================================================//
FollowUnit::FollowUnit()
{
	F=0;
}
bool FollowUnit::MakeOneStep(WeaponParams* WP)
{
	if(WP->To.UnitIndex!=-1&&WP->To.UnitIndex!=0xFFFF)
	{
		OneObject* OB = Group[WP->To.UnitIndex];
		if(OB)
		{
			WP->To.x=OB->RealX<<ToRealCoord;
			WP->To.y=OB->RealY<<ToRealCoord;
			WP->To.z=(OB->RZ+OB->OverEarth+OB->newMons->DstZPoint)<<ToPixelCoord;
			int dx=WP->To.x-WP->x;
			int dy=WP->To.y-WP->y;
			int dz=WP->To.z-WP->z;
			if(F==0)
			{
				WP->Dir=GetDir(dx,dy);
				WP->DirZ=GetDir(Norma(dx,dy),dz);
			}
			/*
			WP->To.x=OB->RealX<<ToRealCoord;
			WP->To.y=OB->RealY<<ToRealCoord;
			//WP->To.z=OB->RZ<<ToPixelCoord;
			int N1=sqrt(WP->Vx*WP->Vx+WP->Vy*WP->Vy+WP->Vz*WP->Vz);
			int nVx = 0;
			int nVy = 0;
			int nVz = 0;

			if(N1)
			{
				nVx = (WP->Vx*10000)/N1;
				nVy = (WP->Vy*10000)/N1;
				nVz = (WP->Vz*10000)/N1;
			}

			int dx=WP->To.x-WP->x;
			int dy=WP->To.y-WP->y;
			int dz=WP->To.z-WP->z;
			int N2=sqrt(dx*dx+dy*dy+dz*dz);
			
			int nDx = 0;
			int nDy = 0;
			int nDz = 0;

			if(N2)
			{
				nDx = (dx*10000)/N2;
				nDy = (dy*10000)/N2;
				nDz = (dz*10000)/N2;
			}

			if(F==0)
			{
				WP->Vx=(nDx*N1)/10000;
				WP->Vy=(nDy*N1)/10000;
				WP->Vz=(nDz*N1)/10000;
			}
			else
			{
				int dX=nVx-nDx;
				int dY=nVy-nDy;
				int dZ=nVz-nDz;

				WP->Vx+=(dX*F)/200;
				WP->Vy+=(dY*F)/200;
				WP->Vz+=(dZ*F)/200;
			}
			WP->Dir=GetDir(WP->Vx,WP->Vy);
			*/
			return true;
		}
	}
	return false;
}
//==================================================================================================================//
BirthNew::BirthNew()
{
	DamageChange=100;
	BirthPause=0;
	//LeaveFromPoint=false;
	Fr_AddX=0;
	Fr_AddY=0;
	Fr_AddZ=0;
	To_RandomUnitInRadius=0;
	To_RandomPosInRadius=0;
	To_AddX=0;
	To_AddY=0;
	To_AddZ=0;
}
bool BirthNew::MakeOneStep(WeaponParams* WP)
{
	if(WP)
	{
		WeaponParams* NWP = new WeaponParams();
		Enumerator* En = ENUM.Get("WeaponModificatorEnum");
		if(En)
		{
			WeaponModificator* WM = (WeaponModificator*)(En->Get(NewWeaponModificator.pchar()));
			if((int)WM==-1)return false;
			NWP->WeaponModificatorP=WM;
			NWP->WeaponModificatorName.Clear();
			NWP->WeaponModificatorName.Add(NewWeaponModificator.pchar());
		}
		if(WP->AdditionalParams.Get())
		{
			NWP->AdditionalParams.Set(new AdditionalWeaponParams());
			WP->AdditionalParams.Get()->Copy(NWP->AdditionalParams.Get());
			//NWP->AdditionalParams.Get()->Damage=(WP->AdditionalParams.Get()->Damage*DamageChange)/100;
		}
		NWP->OwnerWeaponIndex=WP->OwnerWeaponIndex;
		NWP->BirthTime+=BirthPause;
		NWP->From.UnitIndex=WP->From.UnitIndex;
		switch(WhereDoesItBorns){
		case 0: //Born_in_current_weapon_position
			NWP->From.x=WP->x;
			NWP->From.y=WP->y;
			NWP->From.z=WP->z;
			break;
		case 1: //Born_in_destination_points
			NWP->From.x=WP->To.x;
			NWP->From.y=WP->To.y;
			NWP->From.z=WP->To.z;
			break;
		case 2: //Born_in_start_point_of_this_weapon
			NWP->From.x=WP->From.x;
			NWP->From.y=WP->From.y;
			NWP->From.z=WP->From.z;
			break;
		case 3: //Born_in_finish_point_of_this_weapon
			NWP->From.x=WP->To.x;
			NWP->From.y=WP->To.y;
			NWP->From.z=WP->To.z;
			break;
		}
		NWP->From.x+=(Fr_AddX<<ToPixelCoord);
		NWP->From.y+=(Fr_AddY<<ToPixelCoord);
		NWP->From.z+=(Fr_AddZ<<ToPixelCoord);
		
		
		NWP->To.x=WP->To.x;
		NWP->To.y=WP->To.y;
		NWP->To.z=WP->To.z;
		if(To_RandomPosInRadius)
		{
			int dx=rando()%To_RandomPosInRadius;
			int dy=rando()%To_RandomPosInRadius;
			NWP->To.x+=dx-To_RandomPosInRadius/2;
			NWP->To.y+=dy-To_RandomPosInRadius/2;
		}
		NWP->x=NWP->From.x;
		NWP->y=NWP->From.y;
		NWP->z=NWP->From.z;

		NWP->To.UnitIndex=WP->To.UnitIndex;

		NWP->VDir=GetDir(NWP->To.x-NWP->From.x,NWP->To.y-NWP->From.y);
		NWP->Dir=NWP->VDir;

		GameWeaponSystem.AddActiveWeapon(NWP);
		return true;
	}
	return false;
}
//==================================================================================================================//
bool ParseAndFillExpression(Operand* OP,DString* DS);
int UserFriendlyNumericalReturner_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1, BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam)
{
	IB=Base->addInputBox(NULL,x,y,str,120,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
	Owner=CParam->CE;
	NR=(UserFriendlyNumericalReturner*)DataPtr;
	strcpy(str2,str);
	return y1;
}
bool UserFriendlyNumericalReturner_editor::Assign(xmlQuote* xml)
{
	if(NR)
	{
		DString DS;
		NR->Value.GetAssembledView(DS,false);
		if(DS.pchar())
		{
			strcpy(str,DS.pchar());
			strcpy(str2,str);
		}
	}
	return true;
}
bool ParseAndFillExpression(Operand* OP,DString* DS);
int UserFriendlyNumericalReturner_editor::Get(xmlQuote* xml)
{	
	if(strcmp(str,str2))
	{
		if(IB && IB->Active){
			if(Owner->CheckIfOpen(NR)){
				Owner->DelExpInf(NR);
				CE->ExpInfChanged=true;
				return true;
			}
		}
		CQParser Parser;

		strcpy(str2,str);
		DString DS=str;
		if(NR->Value.Op.Get())
		{
			delete (NR->Value.Op.Get());			
		}
		NR->Value.Op.Set(NULL);
		
		if(Parser.ParseAndFillExpression(&NR->Value,&DS))
		{
			IB->AFont=&YellowFont;
		}
		else
		{
			IB->AFont=&RedFont;
		}
		/*
		if(ParseAndFillExpression((Operand*) &NR->Value,&DS))
		{
			int g=0;
		}
		*/
	}
	return true;
}

//==================================================================================================================//

WeaponInRadius::WeaponInRadius(){
};
void WeaponInRadius::Init(WeaponParams* WP){
	Finded.Clear();
	WeaponModificator* WM=Weapon.Get();
	if(WM){
		int r=Radius.Get();
		for(int i=0;i<GameWeaponSystem.ActiveWeapons.GetAmount();i++){
			WeaponParams* W=GameWeaponSystem.ActiveWeapons[i];
			if (W && W->WeaponModificatorName==WM->Name) {
				//int norm=Norma(WP->x-W->x,WP->y-W->y);
				int norm=Norma(WP->x-W->x>>ToPixelCoord,WP->y-W->y>>ToPixelCoord);
				if(norm<r){
					Finded.Add(W);
				}
			}
		}
	}	
};
bool WeaponInRadius::GetTargetDesignation(WeaponParams* WP, int N,TargetDesignation* TD){
	addrand(222);
	if (TD && N<Finded.GetAmount()){
		TD->WP=Finded[N];
		return true;
	}
	return false;
};

SpriteInRadius::SpriteInRadius()
{
	//Radius=0;
	//MaxUnits=0;
//	Frendly=0;
//	Enemy=0;
	FillList=false;
	Owner=0xFFFF;
}
bool SpriteInRadius::GetTargetDesignation(WeaponParams* WP, int N,TargetDesignation* TD)
{
	addrand(134);
	bool rez=false;
	if(WP&&TD){
		if(N==0){
			FindedUnits.Clear();
			itr_GetSpritesInRadius.Create(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord,Radius.Get());
			
			while(OneSprite* obb=itr_GetSpritesInRadius.Next()){
				bool rez=false;
				if( obb && ( UseSpriteType&&obb->SGIndex==SpriteType || 
					!UseSpriteType&&( SprGroupTrees&&obb->SG==&TREES || SprGroupStones&&obb->SG==&STONES )) ){
					int ddx=(WP->x>>ToPixelCoord)-obb->x;
					int ddy=(WP->y>>ToPixelCoord)-obb->x;
					int r2r=MinRadius.Get()*MinRadius.Get();
					if(((ddx*ddx+ddy*ddy)>=r2r))	rez=true;
					if(rez)FindedUnits.Add(obb->Index);
				}
			}

			FillList=true;
		}
		int n=FindedUnits.GetAmount();
		if(N<n&&N<MaxUnits.Get()){
			OneSprite* OS = &Sprites[FindedUnits[N]];
			if(OS){
				if(DeleteSprite){
					OS->Enabled=false;
				}
				TD->UnitIndex=OS->Index;
				TD->x=int(OS->x)<<ToPixelCoord;
				TD->y=int(OS->y)<<ToPixelCoord;
				TD->z=int(OS->z)<<ToPixelCoord;
				rez=true;
			}
		}
	}
	if(!rez)FillList=false;
	return rez;
}
//==================================================================================================================//
UnitsInRadius::UnitsInRadius()
{
	MaxUnitsDependRadius=true;

	//Radius=0;
	//MaxUnits=0;
	Frendly=0;
	Enemy=0;
	FillList=false;
	Owner=0xFFFF;

	Units=true;	
	LiveBody=true;
}

bool isNotChest(OneObject* OB);

void UnitsInRadius::Init(WeaponParams* WP){
	word CasterID=WP->From.UnitIndex;
	FindedUnits.Clear();

	if(TargetUnit){
		FindedUnits.Add(WP->To.UnitIndex);
		return;
	}

	int R=Radius.Get();
	if (Caster && R==0) {
		FindedUnits.Add(CasterID);
		return;
	}

	if (Units) itr_UnitsInRadius.Create(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord,R);
	if (Buildings) itr_GetBuildingsInRadius.Create(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord,R);

	int MainDir=GetDir(WP->To.x-WP->x,WP->To.y-WP->y);
	
	while(true){
		OneObject* obb=NULL;
		if(Units)obb=itr_UnitsInRadius.Next();
		if(Buildings&&!obb) obb=itr_GetBuildingsInRadius.Next();
		if(obb){
			addrand(obb->Index);
			bool rez=false;
			if( (Caster || obb->Index!=CasterID) && isNotChest(obb) ){
				eLiveState LS=obb->GetLiveState();
				if( LiveBody&&LS==ls_LiveBody || DeadBody&&LS==ls_DeadBody || Bones&&LS==ls_Bones || LiveWounded&&LS==ls_LiveBody&&obb->Life<obb->MaxLife ){
					OneObject* OW=NULL;
					if(WP->From.UnitIndex!=0xFFFF)OW=Group[WP->From.UnitIndex];
					if(OW){
						int ddx=(WP->x>>ToPixelCoord)-(obb->RealX>>ToPixelCoord);
						int ddy=(WP->y>>ToPixelCoord)-(obb->RealY>>ToPixelCoord);
						int r2r=MinRadius.Get()*MinRadius.Get();
						//if(Frendly&&(OW->NMask&obb->NMask)&&((ddx*ddx+ddy*ddy)>=r2r))
						//	rez=true;
						//if(Enemy&&(!(OW->NMask&obb->NMask))&&((ddx*ddx+ddy*ddy)>=r2r))
						//	rez=true;
						if(Frendly&&(OW->NMask&obb->NMask)&&(Norma(ddx,ddy)>=r2r))
							rez=true;
						if(Enemy&&(!(OW->NMask&obb->NMask))&&(Norma(ddx,ddy)>=r2r))
							rez=true;
					}
				}
				if(rez){
					int dSector=Sector.Get();
					if(dSector>0){
						
						int ObDir=GetDir((obb->RealX>>4)-(WP->x>>ToPixelCoord),(obb->RealY>>4)-(WP->y>>ToPixelCoord));

						int d=abs(char(MainDir-ObDir));
						if(d>dSector/2){
							rez=false;
						}
					}
				}
				if(rez){
					FindedUnits.Add(obb->Index);
					addrand(obb->Index);
				}
			}
		}else{
			break;
		}
	}
	addrand(472);
	int nmax=0;
	int indmax=0;
	while(N<FindedUnits.GetAmount()){
		if(UnitsSelectionRule==1){
			indmax=-1;
			for(int obi=0;obi<FindedUnits.GetAmount()-1;obi++){
				OneObject* OB = Group[FindedUnits[obi]];
				if(OB){
					if(Norma(((WP->x>>ToPixelCoord)-(OB->RealX>>ToPixelCoord)),((WP->y>>ToPixelCoord)-(OB->RealY>>ToPixelCoord)))>nmax){
						nmax=Norma(((WP->x>>ToPixelCoord)-(OB->RealX>>ToPixelCoord)),((WP->y>>ToPixelCoord)-(OB->RealY>>ToPixelCoord)));
						indmax=obi;
					}
				}
			}
			if(indmax==-1)break;
			addrand(indmax);
			FindedUnits.Del(indmax,1);
		}else
		if(UnitsSelectionRule==2){
			indmax=FindedUnits.GetAmount()-1;
			addrand(indmax);
			FindedUnits.Del(indmax,1);
		}else
		if(UnitsSelectionRule==0){
			indmax=(int(rando())*(FindedUnits.GetAmount()-1))>>15;
			addrand(indmax);
			FindedUnits.Del(indmax,1);
		}		
	}
	Owner=WP->From.UnitIndex;
	FillList=true;

}
void UnitsInRadius::SetMaxUnits(){
	N=MaxUnits.Get();
	if(MaxUnitsDependRadius){
		int R=Radius.Get();
		if(R>0){
			N=R/20;
			N=N*N;
		}
	}
};
bool UnitsInRadius::GetTargetDesignation(WeaponParams* WP, int i,TargetDesignation* TD){
	addrand(123);
	bool rez=false;
	if(WP&&TD){
		if(N==0){
			return false;
		}
		if(i==0){
			Init(WP);
		}
		int n=FindedUnits.GetAmount();
		if(i<n&&i<N){
			OneObject* OB = Group[FindedUnits[i]];
			if(OB){
				TD->UnitIndex=OB->Index;
				Coor3D DP;
				
				TD->x=OB->RealX<<ToRealCoord;
				TD->y=OB->RealY<<ToRealCoord;
				TD->z=(OB->RZ+OB->OverEarth+OB->newMons->DstZPoint)<<ToPixelCoord;
				//OB->GetDamagePoint(0,0,&DP,0);
				//TD->x=DP.x<<ToRealCoord;
				//TD->y=DP.y<<ToRealCoord;
				//TD->z=DP.z<<ToPixelCoord;

				rez=true;
			}
		}
	}
	if(!rez)FillList=false;
	return rez;
}
bool UnitsInRadius::CheckUnitsInRadius(OneObject* OB,void* param)
{
	bool rez=false;
	if(OB&&param)
	{
		UnitsInRadius* Ui = (UnitsInRadius*)param;
		if(!Ui){
			rez=true;
		}else
		if(!OB->Sdoxlo)
		{
			OneObject* OW=NULL;
			if(Ui->Owner!=0xFFFF)OW=Group[Ui->Owner];
			if(OW){
				int ddx=(OW->RealX-OB->RealX)>>4;
				int ddy=(OW->RealY-OB->RealY)>>4;
				int r2r=Ui->MinRadius.Get();
				r2r*=r2r;
				if(Ui->Frendly&&(OW->NMask&OB->NMask)&&((ddx*ddx+ddy*ddy)>r2r))
					rez=true;
				if(Ui->Enemy&&(!(OW->NMask&OB->NMask))&&((ddx*ddx+ddy*ddy)>r2r))
					rez=true;
			}
		}
		if(rez)Ui->FindedUnits.Add(OB->Index);
	}
	return rez;
}
//==================================================================================================================//
bool RandomPosInRadius::GetTargetDesignation(WeaponParams* WP, int Nn,TargetDesignation* TD)
{
	addrand(Nn);
	bool rez=false;
	int NN=0;
	int Rad=0;
	int mRad=0;
	//if(WP->AdditionalParams.Get())
	//{
	//	NN=WP->AdditionalParams.Get()->N;
	//	Rad=WP->AdditionalParams.Get()->Radius;
	//}
	//else
	{
		NN=N.Get();
		Rad=Radius.Get();
		mRad=MinRadius.Get();
	}
	if(Nn<NN&&Rad&&mRad<Rad)
	{
		addrand(Rad);
		int dx,dy,r;
		do {
			dx=Rad-rando()%((Rad)*2);
			dy=Rad-rando()%((Rad)*2);
			r=dx*dx+dy*dy;
		} while (r>Rad*Rad || r<mRad*mRad);

		TD->x=WP->x+(dx<<ToPixelCoord);
		TD->y=WP->y+(dy<<ToPixelCoord);
		TD->z=WP->z;

		rez=true;
	}
	return rez;
}
bool UserDefinedPoints::GetTargetDesignation(WeaponParams* WP, int N,TargetDesignation* TD){
	addrand(456);
	if(N<Points.GetAmount()){
        int R=Radius.Get();

        //TD->x=WP->x+(int(Points[N]->x*R)<<ToPixelCoord);
		//TD->y=WP->y+(int(Points[N]->y*R)<<ToPixelCoord);
		int Vx=int(Points[N]->x*R)<<ToPixelCoord;
		int Vy=int(Points[N]->y*R)<<ToPixelCoord;
		TD->x=WP->x+((Vx*TCos[WP->Dir]+Vy*TSin[WP->Dir])>>8);
		TD->y=WP->y+((Vx*TSin[WP->Dir]-Vy*TCos[WP->Dir])>>8);
		
		TD->z=WP->z+(int(Points[N]->z*R)<<ToPixelCoord);
		return true;
	}else return false;
}
//==================================================================================================================//
void GetUnitCost(byte NI,word NIndex,int* Cost,word Power);
int mmc_UnitsPrice::Collect(OneObject* From){
	if(From){
		int Cost[8];
		GetUnitCost(From->NNUM,From->NIndex,Cost,0);
		int rez=0;
		if(GiveToOwner){
			for(int i=0;i<8;i++){
				rez+=Cost[i];
				AddXRESRC(From->NNUM,i,Cost[i]);
			}
		}else{
			for(int i=0;i<8;i++) rez+=Cost[i];
		}
		return rez;
	}
	return 0;
};
int mmc_UnitsAmount::Collect(OneObject* From){
	if(From){
		return 1;
	}
	return 0;
};
int mmc_UnitsLife::Collect(OneObject* From){
	if(From){
		if(From->Sdoxlo) return From->MaxLife;
			else return From->Life;
	}
	return 0;
};

DLLEXPORT void ChangeUnitColor(int Index,byte DstNI);

int mmc_Resurrect::Collect(OneObject* From){
	if(From&&!From->NewBuilding){
		OneObject* Caster=GlobalUnitsContext::getCaster();
		if( !Caster || 
				(!OnlySameNatID || Caster->Ref.General->NatID==From->Ref.General->NatID) && 
				(!OnlySameColorID || Caster->NNUM==From->NNUM) ){
			eLiveState LS=From->GetLiveState();
			if( (LS==ls_DeadBody||LS==ls_Bones) && !From->UnlimitedMotion ){

				From->Sdoxlo=0;
				From->Life=From->MaxLife;
				From->ClearOrders();

				if(Caster){
					ChangeUnitColor(From->Index,Caster->NNUM);
					From->NMask=Caster->NMask;
				}

				// animation summon
				NewAnimation* naSum=From->newMons->GetAnimation(anm_Summon);
				if(naSum){
					From->NewAnm=naSum;
					From->SetZeroFrame();
				}

				bool DeleteItemDrops(OneObject* OB);
				DeleteItemDrops(From);

				return 1;
			}		
		}
	}
	return 0;
};

void mmd_TakeResource::Distribute(WeaponParams* WP, OneObject* To, int Amount){
	if(To){
		Operand.Calculate(Amount,Amount);
		AddXRESRC(To->NNUM,GoldID,Amount);
	}
};
void mmd_TakeLife::Distribute(WeaponParams* WP, OneObject* To, int Amount){
	if(To){
		Operand.Calculate(Amount,Amount);
		if(Amount>0){
			To->Life+=Amount;
			if (To->Life > To->MaxLife) To->Life=To->MaxLife;
			//if (To->Life <= 0) {
			//	To->Life=0;
			//	To->Die();
			//}
		}
	}
};
void mmd_TakeMana::Distribute(WeaponParams* WP, OneObject* To, int Amount){
	if(To){
		Operand.Calculate(Amount,Amount);
		if (To->Mana > To->MaxMana) To->Mana=To->MaxMana;
		if (To->Mana < 0) To->Mana=0;
	}
};
void mmd_Summon::Distribute(WeaponParams* WP, OneObject* To, int Amount){
	if(To){
		int x,y;
		int p=ToPixelCoord-4;
		if(p>=0) {x=WP->x>>p; y=WP->y>>p;}
			else {x=WP->x<<p; y=WP->y<<p;}

		int life=Life+Amount*CollectorMultiplicator;

        SetActiveProducer();
		int id=NATIONS[To->NNUM].CreateNewMonsterAt(x,y,UT.UnitType,true);
        SetActiveProducer(NULL);

		if(id!=-1){
			OneObject* NOB=Group[id];

			if(life){
				MarkObjectLikeModified(NOB);
				NOB->MoreCharacter->Life=life;
				NOB->Life=life;
				NOB->MaxLife=life;
			}

			// add ability
			int n=AdditionalAbilites.GetAmount();
			for(int i=0;i<n;i++){
				UnitAbility* UA=AdditionalAbilites[i]->Get();
				AddOneEffect(WP,UA,NOB,To);
			}

			// animation summon
			NewAnimation* naSum=NOB->newMons->GetAnimation(anm_Summon);
			if(naSum){
				NOB->NewAnm=naSum;
				NOB->SetZeroFrame();
			}

		}
	}
}
//
bool MassBirthNew::MakeOneStep(WeaponParams* WP){
	if (WP) {

		OneObject* Caster=Group[WP->From.UnitIndex];
		TargetModify.Result=0;

		WeaponModificator* WM = NewWeaponModificator.Get();
		UnitAbility* UA=NewUnitAbility.Get();
		
		TargetDesignation TD;
		int i=0;

		TargetFinder* TF=NewTargetList.Get();
		if(TF){
			TF->SetMaxUnits();

			while(TF->GetTargetDesignation(WP,i,&TD)) {
				i++;

				if(Probability<100){
					if( Probability<=GetRND(100) ){
						continue;
					}
				}

				if(TD.UnitIndex!=0xFFFF){
					addrand(TD.UnitIndex);
					OneObject* OB=Group[TD.UnitIndex];
					if(OB){
						MassModifyCollect* C=TargetModify.Collector.Get();
						if(C){
							TargetModify.Result+=C->Collect(OB);
						}
						if(UA){
							ActiveUnitAbility* AA=AddOneEffect(WP,UA,OB,Caster);
							if(AA){
								AA->Tx=WP->To.x>>ToPixelCoord;
								AA->Ty=WP->To.y>>ToPixelCoord;
								int TID=WP->To.UnitIndex;
								if(TID!=0xFFFF){
									OneObject* TOB=Group[TID];
									if(TOB){
										AA->tIndex=TOB->Index;
										AA->tSerial=TOB->Serial;
									}
								}
							}
						}
					}
				}
				
				int GetTopZone(int x, int y, byte LockType);
				int TZ=GetTopZone(TD.x>>ToPixelCoord,TD.y>>ToPixelCoord,LockType);
				if(TZ==0xFFFF)
					continue;

				if(WM){
					WeaponParams* NWP = new WeaponParams();
					TD.Duplicate(&NWP->To);

					NWP->WeaponModificatorP=WM; 

					NWP->WeaponModificatorName=WM->Name.pchar();
					if(WP->AdditionalParams.Get())
					{
						NWP->AdditionalParams.Set(new AdditionalWeaponParams());
						WP->AdditionalParams.Get()->Copy(NWP->AdditionalParams.Get());
						//NWP->AdditionalParams.Get()->Damage=(WP->AdditionalParams.Get()->Damage*DamageChange)/100;
					}
					NWP->OwnerWeaponIndex=WP->OwnerWeaponIndex; 
					NWP->BirthTime+=BirthPause;

					int x,y,z;
					NWP->From.UnitIndex=WP->From.UnitIndex;				

					x=WP->From.x; y=WP->From.y; z=WP->From.z;
					switch(WhereDoesItBorns){
						case 0: //Born_in_current_weapon_position
							x=WP->x; y=WP->y; z=WP->z; break;
						case 1://Born_in_destination_points
							x=TD.x; y=TD.y; z=TD.z; break;
						case 2: //Born_in_start_point_of_this_weapon
							break;
						case 3: //Born_in_finish_point_of_this_weapon
							x=WP->To.x; y=WP->To.y; z=WP->To.z;
							//NWP->From.UnitIndex=WP->To.UnitIndex;
							break;
					}
					NWP->From.x=x; NWP->From.y=y; NWP->From.z=z;

					//WhereDoesTheyGo
					NWP->To.UnitIndex=WP->To.UnitIndex;

					x=WP->To.x; y=WP->To.y; z=WP->To.z;
					switch(WhereDoesTheyGo){
						case 0: //To_destination_points
							x=TD.x; y=TD.y; z=TD.z; break;
						case 1: //To_current_weapon_position
							x=WP->x; y=WP->y; z=WP->z; break;
						case 2: //To_start_of_this_weapon
							x=WP->From.x; y=WP->From.y; z=WP->From.z;
							//NWP->To.UnitIndex=WP->From.UnitIndex;
							break;
						case 3: //To_finish_of_this_weapon
							break;
					}
					NWP->To.x=x; NWP->To.y=y; NWP->To.z=z;

					NWP->x=NWP->From.x;
					NWP->y=NWP->From.y;
					NWP->z=NWP->From.z;

					NWP->V=WP->V;
					
					int dx=NWP->To.x-NWP->From.x;
					int dy=NWP->To.y-NWP->From.y;

					if(dx==0&&dy==0){
						NWP->VDir=WP->VDir;
						NWP->Dir=WP->Dir;
					}else{
						NWP->VDir=GetDir(dx,dy);
						NWP->Dir=NWP->VDir;
					}

					NWP->fiDir=WP->fiDir;
					NWP->fiOrt=WP->fiOrt;

					NWP->ExtraFiDir=WP->ExtraFiDir;
					NWP->ExtraFiOrt=WP->ExtraFiOrt;
					NWP->ExtraDir=WP->ExtraDir;

					GameWeaponSystem.AddActiveWeapon(NWP);
				}
			}

			if(Caster){
				MassModifyDistribute* D=TargetModify.Distributer.Get();
				if(D){
					D->Distribute(WP,Caster,TargetModify.Result);
				}
			}
		}

		return true;
	}
	return false;
}
/*bool Teleport::MakeOneStep(WeaponParams* WP){
	if (WP) {

		OneObject* Caster=Group[WP->From.UnitIndex];
		TargetModify.Result=0;

		WeaponModificator* WM = NewWeaponModificator.Get();
		UnitAbility* UA=NewUnitAbility.Get();
		
		TargetDesignation TD;
		int i=0;

		TargetFinder* TF=NewTargetList.Get();
		if(TF){
			TF->SetMaxUnits();

			while(TF->GetTargetDesignation(WP,i,&TD)) {
				i++;

				if(Probability<100){
					if( Probability<=GetRND(100) ){
						continue;
					}
				}

				if(TD.UnitIndex!=0xFFFF){
					addrand(TD.UnitIndex);
					OneObject* OB=Group[TD.UnitIndex];
					if(OB){
						if(UA){
							ActiveUnitAbility* AA=AddOneEffect(WP,UA,OB,Caster);
							if(AA){
								AA->Tx=WP->To.x>>ToPixelCoord;
								AA->Ty=WP->To.y>>ToPixelCoord;
								int TID=WP->To.UnitIndex;
								if(TID!=0xFFFF){
									OneObject* TOB=Group[TID];
									if(TOB){
										AA->tIndex=TOB->Index;
										AA->tSerial=TOB->Serial;
									}
								}
							}
						}
					}
				}
				
				int GetTopZone(int x, int y, byte LockType);
				int TZ=GetTopZone(TD.x>>ToPixelCoord,TD.y>>ToPixelCoord,LockType);
				if(TZ==0xFFFF)
					continue;

				if(WM){
					WeaponParams* NWP = new WeaponParams();
					TD.Duplicate(&NWP->To);

					NWP->WeaponModificatorP=WM; 

					NWP->WeaponModificatorName=WM->Name.pchar();
					if(WP->AdditionalParams.Get())
					{
						NWP->AdditionalParams.Set(new AdditionalWeaponParams());
						WP->AdditionalParams.Get()->Copy(NWP->AdditionalParams.Get());
					}
					NWP->OwnerWeaponIndex=WP->OwnerWeaponIndex; 
					NWP->BirthTime+=BirthPause;

					int x,y,z;
					NWP->From.UnitIndex=WP->From.UnitIndex;				

					x=WP->From.x; y=WP->From.y; z=WP->From.z;
					switch(WhereDoesItBorns){
						case 0: //Born_in_current_weapon_position
							x=WP->x; y=WP->y; z=WP->z; break;
						case 1://Born_in_destination_points
							x=TD.x; y=TD.y; z=TD.z; break;
						case 2: //Born_in_start_point_of_this_weapon
							break;
						case 3: //Born_in_finish_point_of_this_weapon
							x=WP->To.x; y=WP->To.y; z=WP->To.z;
							//NWP->From.UnitIndex=WP->To.UnitIndex;
							break;
					}
					NWP->From.x=x; NWP->From.y=y; NWP->From.z=z;

					//WhereDoesTheyGo
					NWP->To.UnitIndex=WP->To.UnitIndex;

					x=WP->To.x; y=WP->To.y; z=WP->To.z;
					switch(WhereDoesTheyGo){
						case 0: //To_destination_points
							x=TD.x; y=TD.y; z=TD.z; break;
						case 1: //To_current_weapon_position
							x=WP->x; y=WP->y; z=WP->z; break;
						case 2: //To_start_of_this_weapon
							x=WP->From.x; y=WP->From.y; z=WP->From.z;
							//NWP->To.UnitIndex=WP->From.UnitIndex;
							break;
						case 3: //To_finish_of_this_weapon
							break;
					}
					ChangeUnitCoor(OB,x,y);
				}
			}
		}

		return true;
	}
	return false;
}*/
bool MassDelete::MakeOneStep(WeaponParams* WP){
	if(WP)
	{
		//WeaponModificator* WM = NewWeaponModificator.Get();
		//if(WM){
			TargetDesignation TD;
			int i=0;
			WeaponList.Init(WP);
			while (WeaponList.GetTargetDesignation(WP,i,&TD)) {
				// delete weapon
				TD.WP->NeedDelete=true;
				i++;
			}
		//}
		return true;
	}
	return false;
};
//==================================================================================================================//
ChangeModificator::ChangeModificator()
{
//	LeaveFromPoint=false;
	Wm=NULL;
	NewWeaponModificator="";
	CheckedName="";
}
bool ChangeModificator::MakeOneStep(WeaponParams* WP)
{
	if(strcmp(CheckedName.pchar(),NewWeaponModificator.pchar()))
	{
		Enumerator* En = ENUM.Get("WeaponModificatorEnum");
		if(En)
		{
			WeaponModificator* WM = (WeaponModificator*)(En->Get(NewWeaponModificator.pchar()));
			if((int)WM==-1)return false;
			CheckedName.Clear();
			CheckedName.Add(NewWeaponModificator.pchar());
			Wm=WM;
		}
	}
	if(Wm)
	{
		WP->WeaponModificatorP=Wm;
		WP->WeaponModificatorName.Clear();
		WP->WeaponModificatorName.Add(NewWeaponModificator.pchar());
		//if(!LeaveFromPoint)
		//{
			WP->From.x=WP->x;
			WP->From.y=WP->y;
			WP->From.z=WP->z;
			WP->BirthTime=TRUETIME;
			WP->LastMoveTime=TRUETIME;
		//}
	}
	return true;
}
bool ChangeTreesLandTypeInRadius::MakeOneStep(WeaponParams* WP){
	void ChangeTreesInRadius(int x,int y,int R,int LandType);
	ChangeTreesInRadius(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord,R,LandType);
    return true;
}
//==================================================================================================================//
void SetFogSpot(int xx, int yy, int r);
//
bool pm_FogSpot::MakeOneStep(WeaponParams* WP){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	byte NMask=NATIONS[NI].NMask;
	//
	OneObject* OB=Group[WP->From.UnitIndex];
	if(OB&&(OB->NMask&NMask)){
		int x=WP->x>>ToPixelCoord;
		int y=WP->y>>ToPixelCoord;
		int r=Radius;
		//SetFogSpot(x,y,r);

		LightSpot* LS=SCENINF.LSpot+OB->NNUM;
		if(Clear){
			if( LS->x==x && LS->y==y ){
				LS->x=0;
				LS->y=0;
				LS->Type=0;
			}
		}else{
			LS->x=x;
			LS->y=y;
			LS->Type=r;
		}

	}
	return true;
};
//

//==================================================================================================================//
void ApplyLandTypeSlow(char* From,char* To,int xc,int yc,int R1,int R2
					   ,int RiseTime,int GrowTime,int RemainTime,int EliminateTime,bool Forever
					   ,char* BirthWeapon,int BirthProbability
					   ,char* UndoWeapon ,int UndoProbability);
pm_ApplyLandTypeSlow::pm_ApplyLandTypeSlow(){
	GrowTime=5000;
	RiseTime=200;
	RemainTime=20000;
	EliminateTime=5000;
}
bool pm_ApplyLandTypeSlow::MakeOneStep(WeaponParams* WP){
	//if(!(From.isClear()||To.isClear())){
	if (!To.isClear()) {
		int x=WP->x>>ToPixelCoord;
		int y=WP->y>>ToPixelCoord;
		ApplyLandTypeSlow(From.pchar(),To.pchar(),x,y,R1,R2,RiseTime,GrowTime,RemainTime,EliminateTime,Forever
			,BirthWeapon.pchar(),BirthProbability
			,UndoWeapon.pchar(),UndoProbability);
	}
	return true;
}
//==================================================================================================================//
void ApplyStandartChasm(int x,int y,int R,int Time);

pm_ApplyStandartChasm::pm_ApplyStandartChasm(){
	R=300;
	Duration=10000;
}
bool pm_ApplyStandartChasm::MakeOneStep(WeaponParams* WP){
	int x=WP->x>>ToPixelCoord;
	int y=WP->y>>ToPixelCoord;
	ApplyStandartChasm(x,y,R,Duration);
	return true;
}
pm_StealResourceFromBuildings::pm_StealResourceFromBuildings(){
	R=200;
	AmountFromEachBuilding=150;
}
bool pm_StealResourceFromBuildings::MakeOneStep(WeaponParams* WP){
	OneObject* Caster=Group[WP->From.UnitIndex];
	if (WP->From.UnitIndex==0xFFFF) {
		return false;
	}
	
	int Amount=0;
	int A=AmountFromEachBuilding;

	int x=WP->x>>ToPixelCoord;
	int y=WP->y>>ToPixelCoord;
	int r=R;
	itr_GetBuildingsInRadius.Create(x,y,r);
	while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
		if(!OB->Sdoxlo){
			Amount+=A;
			AddXRESRC(OB->NNUM,MoneyID,-A);
			if(XRESRC(OB->NNUM,MoneyID)<0)SetXRESRC(OB->NNUM,MoneyID,0);
		}
	}

	AddXRESRC(Caster->NNUM,MoneyID,Amount);
}
//==================================================================================================================//
void ApplyLandType(char* From,char* To,int xc,int yc,int R1,int R2);
//
bool pm_ApplyLandType::MakeOneStep(WeaponParams* WP){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	byte NMask=NATIONS[NI].NMask;
	//
	//OneObject* OB=Group[WP->From.UnitIndex];
	//if(OB&&(OB->NMask&NMask)){
	if(!(From.isClear()||To.isClear())){
		int x=WP->x>>ToPixelCoord;
		int y=WP->y>>ToPixelCoord;
		ApplyLandType(From.pchar(),To.pchar(),x,y,R1,R2);
	}
	return true;
};
//
//==================================================================================================================//
void ClearLockPointsOnWater(int x,int y,int R);
//
bool pm_OnWaterClearLockPoints::MakeOneStep(WeaponParams* WP){
	//byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//byte NMask=NATIONS[NI].NMask;
	//
	//OneObject* OB=Group[WP->From.UnitIndex];
	//if(OB&&(OB->NMask&NMask)){
	if(Radius>0){
		int x=WP->x>>ToPixelCoord;
		int y=WP->y>>ToPixelCoord;
		ClearLockPointsOnWater(x,y,Radius);
	}
	return true;
};
//
//==================================================================================================================//
void RestoreLockPointsOnWater(int x,int y,int R);
//
bool pm_OnWaterRestoreLockPoints::MakeOneStep(WeaponParams* WP){
	//byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//byte NMask=NATIONS[NI].NMask;
	//
	//OneObject* OB=Group[WP->From.UnitIndex];
	//if(OB&&(OB->NMask&NMask)){
	if(Radius>0){
		int x=WP->x>>ToPixelCoord;
		int y=WP->y>>ToPixelCoord;
		RestoreLockPointsOnWater(x,y,Radius);
	}
	return true;
};
//
//==================================================================================================================//
void SetLockPointsOnLand(int x,int y,int R);
//
bool pm_OnLandSetLockPoints::MakeOneStep(WeaponParams* WP){
	//byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//byte NMask=NATIONS[NI].NMask;
	//
	//OneObject* OB=Group[WP->From.UnitIndex];
	//if(OB&&(OB->NMask&NMask)){
	if(Radius>0){
		int x=WP->x>>ToPixelCoord;
		int y=WP->y>>ToPixelCoord;
		SetLockPointsOnLand(x,y,Radius);
	}
	return true;
};
//
//==================================================================================================================//
void RestoreLockPointsOnLand(int x,int y,int R);
//
bool pm_OnLandRestoreLockPoints::MakeOneStep(WeaponParams* WP){
	//byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//byte NMask=NATIONS[NI].NMask;
	//
	//OneObject* OB=Group[WP->From.UnitIndex];
	//if(OB&&(OB->NMask&NMask)){
	if(Radius>0){
		int x=WP->x>>ToPixelCoord;
		int y=WP->y>>ToPixelCoord;
		RestoreLockPointsOnLand(x,y,Radius);
	}
	return true;
};
//
//==================================================================================================================//
//
void EraseObject(OneObject* OB);
//
Polymorph::Polymorph(){
	//Radius=300;
	SourceAmount=20;
	DestAmount=1;
	//Life=100;
	AddSourceLifeToDest=true;	
}
bool Polymorph::MakeOneStep(WeaponParams* WP){
	if(Radius==0||SourceAmount==0||DestAmount==0||!(TargetFriendly||TargetEnemy)){
		return false;
	}
	OneObject* OB=Group[WP->From.UnitIndex];
	if(OB){
		int x=WP->x>>ToPixelCoord;
		int y=WP->y>>ToPixelCoord;
		itr_UnitsInRadius.Create(x,y,Radius);
		// erase objects	
		int life=0;
		DynArray<int> Ox;
		DynArray<int> Oy;
		DynArray<byte> NI;
		while(OneObject* O=itr_UnitsInRadius.Next()){
			if(!O->Sdoxlo&&O->NIndex!=UT.UnitType)
			if(TargetFriendly&&(O->NMask&OB->NMask)||TargetEnemy&&!(O->NMask&OB->NMask)){
				life+=O->Life;
				Ox.Add(O->RealX);
				Oy.Add(O->RealY);
				NI.Add(O->NNUM);
				EraseObject(O);
				if(Ox.GetAmount()>=SourceAmount){
					break;
				}
			}
		}
		if(Ox.GetAmount()==0) return false;
		int N=DestAmount;
		if(N>Ox.GetAmount())N=Ox.GetAmount();

		if(AddSourceLifeToDest){
			life/=N;
			life+=Life;
		}else{
			life=Life;
		}

		for(int i=0;i<N;i++){
            SetActiveProducer();
			int id=NATIONS[NI[i]].CreateNewMonsterAt(Ox[i],Oy[i],UT.UnitType,true);
            SetActiveProducer(NULL);
			if(id!=-1){
				OneObject* NOB=Group[id];

				if(life){
					MarkObjectLikeModified(NOB);
					NOB->MoreCharacter->Life=life;
					NOB->Life=life;
					NOB->MaxLife=life;
				}

				// add ability
				int n=AdditionalAbilites.GetAmount();
				for(int i=0;i<n;i++){
					UnitAbility* UA=AdditionalAbilites[i]->Get();
					AddOneEffect(WP,UA,NOB,OB);
				}

			}
		}
		return true;

	}
	return false;
}
//==================================================================================================================//
MakeDamage::MakeDamage()
{
	TargetedUnit=false;
	SplashDamageChange=100;
	InRadius=1;
	OnlyEnemyUnits=false;
	MaxUnits=1000;
	PushUnitsForce=0;
}
bool MakeDamage::MakeOneStep(WeaponParams* WP){
	AdditionalWeaponParams* AWP=WP->AdditionalParams.Get();
	if(AWP){
		int Rad=InRadius;
		if(AWP->Radius!=0)
			Rad=AWP->Radius;
        
		OneObject* TO = WP->To.UnitIndex!=0xFFFF ? Group[WP->To.UnitIndex] : NULL;

		int x=WP->x>>ToPixelCoord;
		int y=WP->y>>ToPixelCoord;

		int N=MaxUnits;

		OneObject* OW = Group[WP->From.UnitIndex];
		if( WP->From.UnitIndex==0xFFFF ) 
			OW=NULL;

		if(TargetedUnit&&TO){

			bool reached=false;
			if( WP->ReachedUnitID==TO->Index && WP->ReachedUnitSN==TO->Serial ){
				reached=true;
			}else{
				int wX=WP->x>>ToPixelCoord;
				int wY=WP->y>>ToPixelCoord;
				int oX=TO->RealX>>4;
				int oY=TO->RealY>>4;
				int dist=Norma(wX-oX,wY-oY);
				int R,H,H0;
				TO->GetCylinder(R,H,H0);
				if( dist < /*TO->newMons->UnitRadius*/R ){
					reached=true;
				}
			}

			if(reached){
				if( OW && TO && !TO->Sdoxlo ){
					N--;
					TO->MakeDamage(AWP->Damage,AWP->Damage,OW,AWP->AttType);
					if(PushUnitsForce){
						PushUnitBack(TO,1,PushUnitsForce,x,y);
						DetonateUnit(TO,x,y,PushUnitsForce*100);
					}
				}
			}

		}else{
			//if(DamageBuilding){
			//	int Bd = GetBar3DOwner(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord);
			//	if(Bd!=0xFFFF){
			//		OneObject* OBd = Group[Bd];
			//		if (!OnlyEnemyUnits || OnlyEnemyUnits&&OW&&OBd&&!(OW->NMask&OBd->NMask)){
			//			if(OW&&OBd&&(!OBd->Sdoxlo)&&WP->AdditionalParams.Get()){
			//				OBd->MakeDamage(AWP->Damage,AWP->Damage,OW,AWP->AttType);
			//			}
			//		}
			//	}
			//}
		}
		if(Rad>=10&&SplashDamageChange>0||!TargetedUnit){
			if(TargetedUnit&&TO){
				//x=TO->RealX>>4;
				//y=TO->RealY>>4;
			}
			int Damage=(AWP->Damage*SplashDamageChange)/100;

			//int Par[10];
			//Par[0]=OnlyEnemyUnits;
			//Par[1]=0;
			//Par[2]=MaxUnits;
			//Par[3]=WP->From.UnitIndex;
			//Par[4]=Damage;
			//Par[5]=AWP->AttType;
			//Par[6]=Rad;
			//Par[7]=x;
			//Par[8]=y;
			//Par[9]=PushUnitsForce;

			//PerformActionOverUnitsInRadius(x,y,Rad+300,&MakeDamage::MakeDam,(void*)Par);
			itr_UnitsInRadius.Create(x,y,Rad/*+300*/);
			while(OneObject* OB=itr_UnitsInRadius.Next()){
				if( OB!=OW && !OB->Sdoxlo && (!TargetedUnit||OB!=TO) && !(OnlyEnemyUnits&&(!OW||(OB->NMask&OW->NMask))) && isNotChest(OB) ){
					if(N>0){
						N--;
						OB->MakeDamage(Damage,Damage,OW,AWP->AttType);
						if(PushUnitsForce){
							PushUnitBack(OB,1,PushUnitsForce,x,y);
							DetonateUnit(OB,x,y,PushUnitsForce*100);
						}
					}else 
						break;
				}
			}
			if(DamageBuilding){
				itr_GetBuildingsInRadius.Create(x,y,Rad);
				while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
					if( OB!=OW && !OB->Sdoxlo && (!TargetedUnit||OB!=TO) && !(OnlyEnemyUnits&&(!OW||(OB->NMask&OW->NMask))) &&  isNotChest(OB) ){
						if(N>0){
							N--;
							OB->MakeDamage(Damage,Damage,OW,AWP->AttType);
							if(PushUnitsForce){
								PushUnitBack(OB,1,PushUnitsForce,x,y);
								DetonateUnit(OB,x,y,PushUnitsForce*100);
							}
						}else 
							break;
					}
				}
			}
		}
	}
	return true;
}
bool MakeDamage::MakeDam(OneObject* OB,void* param)
{
	bool rez=false;
	int* Pr = (int*)param;
	int OnlyEnemyUnits=Pr[0];
	int NUnits=Pr[1];
	int MaxU=Pr[2];
	int Owner=Pr[3];
	int Damage=Pr[4];
	int AttType=Pr[5];
	if((!OB->Sdoxlo)&&(MaxU==0||NUnits<MaxU))
	{
		OneObject* OW = Group[Owner];
		if(OW){
			if(OnlyEnemyUnits)
			{
				if(OW->NMask&OB->NMask)
				{
					return false;
				}
			}
			int RX=OB->GetAttX();
			int RY=OB->GetAttY();
			int ds = Norma(Pr[7]-RX/16,Pr[8]-RY/16);
			if(ds<(Pr[6]+OB->newMons->EMediaRadius))
			{
				OB->MakeDamage(Damage,Damage,OW,AttType);
				if(Pr[9]){
					PushUnitBack(OB,1,Pr[9],Pr[7],Pr[8]);
					DetonateUnit(OB,Pr[7],Pr[8],Pr[9]*100);
				}
				Pr[1]++;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
Wave::Wave()
{
	H=0;
	MaxR=0;
	MinR=0;
	PushUnitsForce=0;
	LinearWidth=0;
	OnlyEnemyUnits=false;
	Damage=false;
}
bool Wave::MakeOneStep(WeaponParams* WP)
{
	int pr[2];
	pr[0]=(int)WP;
	pr[1]=(int)this;
	int ds=Norma((WP->x>>ToPixelCoord)-(WP->From.x>>ToPixelCoord),(WP->y>>ToPixelCoord)-(WP->From.y>>ToPixelCoord));
	if(ds>MinR&&ds<MaxR)
	{
		if(LinearWidth==0)
		{
			
			PerformActionOverUnitsInRadius(WP->From.x>>ToPixelCoord,WP->From.y>>ToPixelCoord,ds+100,&Wave::MakeWave,(void*)pr);
		}
		else
		{
			PerformActionOverUnitsInRadius(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord,LinearWidth/2,&Wave::MakeWave,(void*)pr);
		}
	}
	return true;
}
bool Wave::MakeWave(OneObject* OB,void* param)
{
	if(OB&&!OB->Sdoxlo)
	{
		int* p=(int*)param;
		WeaponParams* wp=(WeaponParams*)p[0];
		Wave* w=(Wave*)p[1];
		bool fr=true;
		OneObject* ovn=NULL;Group[wp->From.UnitIndex];
		if(wp->From.UnitIndex!=0xFFFF)
		{
			ovn=Group[wp->From.UnitIndex];
		}
		if(w->OnlyEnemyUnits)
		{
			fr=false;
			if(ovn&&!(ovn->NMask&OB->NMask))
			{
				fr=true;
			}
		}
		if(fr)
		{
			int ds=Norma((wp->x>>ToPixelCoord)-(wp->From.x>>ToPixelCoord),(wp->y>>ToPixelCoord)-(wp->From.y>>ToPixelCoord));
			int uds=Norma(OB->RealX/16-(wp->From.x>>ToPixelCoord),OB->RealY/16-(wp->From.y>>ToPixelCoord));
			if(w->LinearWidth)
			{
				if(ds>w->MinR&&ds<w->MaxR)
				{
					byte drr=GetDir((wp->x>>ToPixelCoord)-(wp->From.x>>ToPixelCoord),(wp->y>>ToPixelCoord)-(wp->From.y>>ToPixelCoord));
					int wht=w->LinearWidth/2;
					int x1=(wp->x>>ToPixelCoord)-((TSin[drr]*wht)>>8);
					int y1=(wp->y>>ToPixelCoord)+((TCos[drr]*wht)>>8);
					int x2=(wp->x>>ToPixelCoord)+((TSin[drr]*wht)>>8);
					int y2=(wp->y>>ToPixelCoord)-((TCos[drr]*wht)>>8);
					int lds=GetPointToLineDist(OB->RealX/16,OB->RealY/16,x1,y1,x2,y2);
					if(lds<5&&OB->OverEarth==0)
					{
						OB->OverEarth=w->H;
						if(w->Damage&&ovn)
						{
							if(wp->AdditionalParams.Get())
							{
								int damage=wp->AdditionalParams.Get()->Damage;
								int attType=wp->AdditionalParams.Get()->AttType;
								OB->MakeDamage(damage,damage,ovn,attType);
							}
						}
						if(w->PushUnitsForce)
						{
							PushUnitBack(OB,1,w->PushUnitsForce, wp->From.x>>ToPixelCoord, wp->From.y>>ToPixelCoord);
							DetonateUnit(OB,wp->From.x>>ToPixelCoord,wp->From.y>>ToPixelCoord,w->PushUnitsForce*100);
						}
					}
				}
			}
			else
			{
				if(uds>w->MinR&&uds<(w->MaxR+100))
				{
					int dw=abs(ds-uds)+1;
					if(dw<100)
					{
						OB->OverEarth=w->H/dw;
						if(w->Damage&&ovn)
						{
							if(wp->AdditionalParams.Get())
							{
								int damage=wp->AdditionalParams.Get()->Damage;
								int attType=wp->AdditionalParams.Get()->AttType;
								OB->MakeDamage(damage,damage,ovn,attType);
							}
						}
						if(dw<5&&w->PushUnitsForce)
						{
							PushUnitBack(OB,1,w->PushUnitsForce, wp->From.x>>ToPixelCoord, wp->From.y>>ToPixelCoord);
							DetonateUnit(OB,wp->From.x>>ToPixelCoord,wp->From.y>>ToPixelCoord,w->PushUnitsForce*100);
						}
					}
				}
			}
		}
	}
	return false;
}
//==================================================================================================================//
bool BirthNewAbility::MakeOneStep(WeaponParams* WP)
{
	if(WP){
		if(WP->From.UnitIndex!=0xFFFF){
			OneObject* fOB=Group[WP->From.UnitIndex];
			OneObject* tOB=Group[WP->To.UnitIndex];

			int n=AdditionalAbilites.GetAmount();
			for(int i=0;i<n;i++){
				UnitAbility* UA=AdditionalAbilites[i]->Get();
				AddOneEffect(WP,UA,tOB,fOB);
			}
			return true;
		}
	}
	return false;
}
//==================================================================================================================//
void MarkObjectLikeModified(OneObject* OB);

bool BirthNewUnit::MakeOneStep(WeaponParams* WP)
{
	if(WP)
	{
		if(WP->From.UnitIndex!=0xFFFF)
		{
			OneObject* OB=Group[WP->From.UnitIndex];
			if(OB){
				OneObject* TOB=NULL;
				word NIndex=UT.UnitType;
				
				int x=WP->x;
				int y=WP->y;
				
				if(Illusion){
					TOB=Group[WP->To.UnitIndex];
					if (WP->To.UnitIndex!=-1 && WP->To.UnitIndex!=0xFFFF && TOB) {
						NIndex=TOB->NIndex;
					} else {
						return true;
					}
					x+=(128-(rando()%256))<<4;
					y+=(128-(rando()%256))<<4;
				}

                SetActiveProducer();
				OneObject* NOB=Group[NATIONS[OB->NNUM].CreateNewMonsterAt(x,y,NIndex,true)];
                SetActiveProducer(NULL);

				if(NOB){
					
					if(NOB->NewBuilding&&NOB->NStages){
						//bool NotSelectable=NOB->NotSelectable;
						//NOB->NotSelectable=true;
						for(int i=0;i<NOB->NStages;i++){
							NOB->NextStage();
						}
						//NOB->NotSelectable=NotSelectable;
					}
					if(Illusion){
						if(NOB->ActiveAbility){
							NOB->ActiveAbility->ActiveAbilities.Clear();
						}
						vHeroButtons.Remove(NOB);
					}

					int n=AdditionalAbilites.GetAmount();
					for(int i=0;i<n;i++)
					{
						UnitAbility* UA=AdditionalAbilites[i]->Get();
						if(UA){
							if(UA->Aura){
								ActiveUnitAbility* AA=UA->GetNewActiveAbility();
								AddActiveUnitAbility(NOB->Index,AA);
							}else{
								AddOneEffect(WP,UA,NOB,OB);
							}
						}
					}

					// animation summon
					NewAnimation* naSum=NOB->newMons->GetAnimation(anm_Summon);
					if(naSum){
						NOB->NewAnm=naSum;
						NOB->SetZeroFrame();
					}

					if(SendTo){
						NOB->NewMonsterSmartSendTo(WP->To.x>>ToPixelCoord,WP->To.y>>ToPixelCoord,0,0,128+16,0);
					}
				}

				return true;
			}
		}
	}
	return false;
}
//==================================================================================================================//
BirthNewUnitsFromSprites::BirthNewUnitsFromSprites()
{
	DeleteSprites=false;
}
bool BirthNewUnitsFromSprites::MakeOneStep(WeaponParams* WP)
{
	bool rez=false;
	if(WP)
	{
		int R = Radius.Get();
		int MaxU = MaxUnits.Get();
		if(WP->AdditionalParams.Get())
		{
			if(WP->AdditionalParams.Get()->Radius!=0)
				R=WP->AdditionalParams.Get()->Radius;
			if(WP->AdditionalParams.Get()->N!=0)
				MaxU=WP->AdditionalParams.Get()->N;
		}
		//typedef bool cbCheckSprite(OneSprite* OS, void* Param);
		if(WP->From.UnitIndex!=0xFFFF)
		{
			OneObject* OB=Group[WP->From.UnitIndex];
			if(OB)
			{
				int Param[4];
				Param[0]=UT.UnitType;
				Param[1]=MaxU;
				Param[2]=(int)this;
				Param[3]=OB->NNUM;
				int n = GetSpritesInRadius(WP->x>>ToPixelCoord, WP->y>>ToPixelCoord, R, &BirthNewUnitsFromSprites::CheckSprite, (void*) Param);
			}
		}
	}
	return rez;
}
bool BirthNewUnitsFromSprites::CheckSprite(OneSprite* OS,void* Param)
{
	bool rez=false;
	int* P=(int*)Param;
	if(P[1]>0)
	{
		BirthNewUnitsFromSprites* Th = (BirthNewUnitsFromSprites*)P[2];
        SetActiveProducer();
		OneObject* NOB=Group[NATIONS[P[3]].CreateNewMonsterAt(OS->x<<4,OS->y<<4,P[0],true)];
        SetActiveProducer(NULL);
		if(Th->DeleteSprites)
		{
			EraseSprite(OS->Index);
		}
		int NLife=Th->UnitLife.Get();
		if(NLife)
		{
			NOB->Life=NLife;
			NOB->MaxLife=NLife;
		}
		int n=Th->AdditionalAbilites.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(Th->AdditionalAbilites[i]->Get())
			{
				Th->AdditionalAbilites[i]->Get()->OnUnitBirth(NOB);
			}
		}
		P[1]--;
		rez=true;
	}
	return rez;
}
bool RandomizePosition::MakeOneStep(WeaponParams* WP){
	int*x,*y;
	if(PointToModify==0){//ModifySourcePoint
        x=&WP->From.x;
		y=&WP->From.y;
	}else
	if(PointToModify==1){//ModifyCurrentPoint
		x=&WP->x;
		y=&WP->y;
	}else{//ModifyDestinationPoint
		x=&WP->To.x;
		y=&WP->To.y;
	}
	int D=RandomAmplitude.Get();
	if(AmplitudeUsage==0){
		D<<=ToPixelCoord;
	}else
	if(AmplitudeUsage==1){
		int R=Norma(WP->x-WP->From.x,WP->y-WP->From.y);
        D=R*D/100;
	}else if(AmplitudeUsage==2){
		int R=Norma(WP->To.x-WP->From.x,WP->To.y-WP->From.y);
		D=R*D/100;
	}
	if(D>0){	
		*x+=(rando()%D)-(D/2);
		*y+=(rando()%D)-(D/2);
	}
	return true;
}
bool DeleteSpritesInRadius::MakeOneStep(WeaponParams* WP){
	void delTrees(int x,int y1,int r);
	delTrees(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord,Radius.Get());
	return true;
}
//==================================================================================================================//
ChangeNation::ChangeNation()
{
	FromNI=-1;
	AnyEnemyNation=false;
	AnyFriendlyNation=false;
	ToNI=-1;
	Radius=0;
	NUnits=1000;
	OnlyTargetUnit=false;
}
bool ChangeNation::MakeOneStep(WeaponParams* WP)
{
	bool rez=false;
	int Rad=Radius;
	int NEWNI=ToNI;
	OneObject* OW = Group[WP->From.UnitIndex];
	if(OW&&ToNI==-1)
		NEWNI=OW->NNUM;
	int MaxUnits=NUnits;
	if(WP->AdditionalParams.Get())
	{
		if(WP->AdditionalParams.Get()->Radius!=-1)
			Rad=WP->AdditionalParams.Get()->Radius;
		if(WP->AdditionalParams.Get()->NI!=-1)
			NEWNI=WP->AdditionalParams.Get()->NI;
		if(WP->AdditionalParams.Get()->N!=-1)
			MaxUnits=WP->AdditionalParams.Get()->N;
	}
	int Par[8];
	Par[0]=FromNI;
	Par[1]=AnyEnemyNation;
	Par[2]=AnyFriendlyNation;
	Par[3]=NEWNI;
	Par[4]=MaxUnits;
	Par[5]=(int)this;
	Par[6]=(int)OW->NMask;
	Par[7]=OW->NNUM;
	if(OnlyTargetUnit)
	{
		OneObject* TO = Group[WP->To.UnitIndex];
		rez=bool(0<ChangeObjectNation(TO,(void*) Par));
	}
	else
	{
		rez=0<PerformActionOverUnitsInRadius(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord,Rad,&ChangeNation::ChangeObjectNation,(void*)Par);
	}
	return rez;
}
bool ChangeNation::ChangeObjectNation(OneObject* OB,void* param)
{
	bool rez=false;
	if(OB&&!OB->Sdoxlo)
	{
		int* Par=(int*)param;
		if(Par[4]>0)
		{
			bool NOK=false;
			if(Par[0]==OB->NNUM)
				NOK=true;
			else
			if(Par[1]==1&&!(OB->NMask&((byte)Par[6])))
				NOK=true;
			else
			if(Par[2]==1&&(OB->NMask&((byte)Par[6]))&&Par[7]!=OB->NNUM)
				NOK=true;
			if(NOK)
			{
				ChangeNation* CN = (ChangeNation*)Par[5];
				int n=CN->TypeList.GetAmount();
				bool TypeOK=true;
				if(n)
				{
					TypeOK=false;
					for(int i=0;i<n&&!TypeOK;i++)
					{
						if(CN->TypeList[i]->UnitType==OB->NIndex)
							TypeOK=true;
					}
				}
				if(TypeOK)
				{
					OBJ_ChangeNation(OB, Par[3]);
					Par[4]--;
				}
			}
		}
	}
	return rez;
}
//==================================================================================================================//
bool PlaySomeSound::MakeOneStep(WeaponParams* WP){
	if(SoundID>0){
		extern CDirSound* CDS;
		CDS->HitSound(SoundID);
		AddEffect(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord,SoundID);		
	}
	return true;
}
//==================================================================================================================//
bool True::Check(WeaponParams* WP)
{
	return true;
}
//==================================================================================================================//
DWORD CurTrigSerial=0;
Trigger::Trigger(){
	Serial=CurTrigSerial++;
	N=1;
};
bool Trigger::Check(WeaponParams* WP){
	if(WP){
		int i=WP->TrigSerial.find(Serial);
		if(i==-1){
			i=WP->TrigSerial.Add(Serial);
			WP->TrigN.Add(N);
		}
		if(WP->TrigN[i]>0){
			WP->TrigN[i]--;
			return true;
		}
	}
	return false;
}
DWORD CurTimeSerial=0;
we_Timer::we_Timer(){
	Serial=CurTimeSerial++;
	StartTime=0;
	Delay=1000;
}
bool we_Timer::Check(WeaponParams* WP){
	if(WP){
		int i=WP->TimeSerial.find(Serial);
		if(i==-1){
			i=WP->TimeSerial.Add(Serial);
			WP->TimeDelay.Add(0);
		}
		int time=WP->TimeDelay[i];
		if(TRUETIME>time){
			if (Delay==0) Delay=1000;
			time=WP->BirthTime+StartTime;
			int n=(TRUETIME-time)/Delay;
			time+=Delay*(n+1);
			WP->TimeDelay[i]=time;
			return true;
		}
	}
	return false;
}
we_Random::we_Random(){
	Percentage=50;
}
bool we_Random::Check(WeaponParams* WP){
	if(WP){
		int R=GetRND(100);
		if(Percentage<R){
			return true;
		}
	}
	return false;
}

//==================================================================================================================//
bool IsTargetDie::Check(WeaponParams* WP)
{
	bool rez=true;
	if(WP)
	{
		if(WP->To.UnitIndex!=0xFFFF)
		{
			OneObject* OB = Group[WP->To.UnitIndex];
			if(OB&&!OB->Sdoxlo)
			{
				rez=false;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
IsTargetInvisible::IsTargetInvisible()
{
	Not=true;
}
bool IsTargetInvisible::Check(WeaponParams* WP)
{
	bool rez=false;
	if(WP)
	{
		if(WP->To.UnitIndex!=0xFFFF)
		{
			OneObject* OB = Group[WP->To.UnitIndex];
			if(OB&&OB->Invisible)
			{
				rez=true;
			}
		}
	}
	if(Not)
		rez=!rez;
	return rez;
}
//==================================================================================================================//
Conditions::Conditions()
{
	LifeTimeMore=-1;
	LifeTimeLess=-1;
	TraveledDistanceMore=-1;
	TraveledDistanceLess=-1;
	RemainderDistanceMore=-1;
	RemainderDistanceLess=-1;
	HeightMore=-1;
	HeightLess=-1;
}
bool Conditions::Check(WeaponParams* WP)
{
	bool rez=false;
	bool add=false;
	int LT=TRUETIME-WP->BirthTime;
	int TD=Norma((WP->From.x>>ToPixelCoord)-(WP->x>>ToPixelCoord),(WP->From.y>>ToPixelCoord)-(WP->y>>ToPixelCoord));
	TD=Norma(TD,(WP->From.z>>ToPixelCoord)-(WP->z>>ToPixelCoord));
	int RD=Norma((WP->To.x>>ToPixelCoord)-(WP->x>>ToPixelCoord),(WP->To.y>>ToPixelCoord)-(WP->y>>ToPixelCoord));
	RD=Norma(RD,(WP->To.z>>ToPixelCoord)-(WP->z>>ToPixelCoord));
	if(LifeTimeMore!=-1){ add=true;	rez=LifeTimeMore<LT; } if(add&&!(rez)) return false;
	if(LifeTimeLess!=-1){ add=true;	rez=LifeTimeLess>LT; } if(add&&!(rez)) return false;
	if(TraveledDistanceMore!=-1){ add=true;	rez=TraveledDistanceMore<TD; } if(add&&!(rez)) return false;
	if(TraveledDistanceLess!=-1){ add=true;	rez=TraveledDistanceLess>TD; } if(add&&!(rez)) return false;
	if(RemainderDistanceMore!=-1){ add=true; rez=RemainderDistanceMore<RD; } if(add&&!(rez)) return false;
	if(RemainderDistanceLess!=-1){ add=true; rez=RemainderDistanceLess>RD; } if(add&&!(rez)) return false;
	int H=0;
	bool GetH=false;
	if(IsInBuilding)
	{
		if(!GetH)
			H=GetHeight(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord);
		int BH=GetBar3DHeight(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord);
		extern word OWNER;
		if(OWNER==0xFFFF||OWNER!=WP->From.UnitIndex){
			if(BH&&(WP->z>>ToPixelCoord)>H&&(WP->z>>ToPixelCoord)<(H+BH))
				return true;		
		}
	}
	if(HeightMore!=-1||HeightLess!=-1||AbsHeightMore!=-1||AbsHeightLess!=-1)
	{
		H=GetHeight(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord);
		if(H<0)
			H=0;
		GetH=true;
		if(HeightMore!=-1){ add=true;	rez=HeightMore<((WP->z>>ToPixelCoord)-H); } if(add&&!(rez)) return false;
		if(HeightLess!=-1){ add=true;	rez=HeightLess>((WP->z>>ToPixelCoord)-H); } if(add&&!(rez)) return false;
		if(AbsHeightMore!=-1){ add=true;	rez=AbsHeightMore<((WP->z>>ToPixelCoord)); } if(add&&!(rez)) return false;
		if(AbsHeightLess!=-1){ add=true;	rez=AbsHeightLess>((WP->z>>ToPixelCoord)); } if(add&&!(rez)) return false;
	}	
	return rez;
}
TimePeriod::TimePeriod(){
	//StartTime=0;
	//FinalTime=1000;
}
bool TimePeriod::Check(WeaponParams* WP){
	int LT=TRUETIME-WP->BirthTime;
	int s=StartTime;
	int f=FinalTime;
	return (LT>=s && LT<f);
}
//==================================================================================================================//
bool TargetReached::Check(WeaponParams* WP){

	bool reach=false;

	if( OneEnemyReached ){
		word ID=WP->From.UnitIndex;
		OneObject* OB=Group[ID];
		if( ID!=0xFFFF && OB ){
			int x=WP->x>>ToPixelCoord;
			int y=WP->y>>ToPixelCoord;
			int z=WP->z>>ToPixelCoord;
			itr_UnitsInRadius.Create(x,y,TargetPointDistance+100);
			itr_GetBuildingsInRadius.Create(x,y,TargetPointDistance+200);
			while( true ){
				OneObject* obEnemy=itr_UnitsInRadius.Next();
				if(!obEnemy)
					obEnemy=itr_GetBuildingsInRadius.Next();
				if(obEnemy){
					if( !(obEnemy->NMask&OB->NMask) && obEnemy->GetLiveState()==ls_LiveBody && isNotChest(OB) ){
						int R,H,H0;
						obEnemy->GetCylinder(R,H,H0);
						if( z>H0-TargetPointDistance && z<H0+H+TargetPointDistance ){
							int eX=obEnemy->RealX>>4;
							int eY=obEnemy->RealY>>4;
							int dist=Norma(eX-x,eY-y);
							if( dist<TargetPointDistance+R ){
								WP->To.UnitIndex=obEnemy->Index;
								WP->ReachedUnitID=obEnemy->Index;
								WP->ReachedUnitSN=obEnemy->Serial;
								return true;
							}
						}
					}
				}else{
					break;
				}
			}
		}
	}

	if(IsInsideBuilding){
		word ID=WP->From.UnitIndex;
		OneObject* OB=Group[ID];
		if( ID!=0xFFFF && OB ){
			int x=WP->x>>ToPixelCoord;
			int y=WP->y>>ToPixelCoord;
			int z=WP->z>>ToPixelCoord;
			itr_GetBuildingsInRadius.Create(x,y,TargetPointDistance+200);
			while( true ){
				OneObject* obEnemy=itr_GetBuildingsInRadius.Next();
				if(obEnemy){
					if( !(obEnemy->NMask&OB->NMask) && obEnemy->GetLiveState()==ls_LiveBody ){
						int R,H,H0;
						obEnemy->GetCylinder(R,H,H0);
						if( z>H0-TargetPointDistance && z<H0+H+TargetPointDistance ){
							int eX=obEnemy->RealX>>4;
							int eY=obEnemy->RealY>>4;
							int dist=Norma(eX-x,eY-y);
							if( dist<TargetPointDistance+R ){
								WP->To.UnitIndex=obEnemy->Index;
								WP->ReachedUnitID=obEnemy->Index;
								WP->ReachedUnitSN=obEnemy->Serial;
								return true;
							}
						}
					}
				}else{
					break;
				}
			}
		}
	}

	if( WP->TargetIsReached ){
		if(TargetObjectReached){
			word ID=WP->To.UnitIndex;
			if(ID!=0xFFFF){
				OneObject* TO=Group[ID];
				if(TO){
					int wX=WP->To.x>>ToPixelCoord;
					int wY=WP->To.y>>ToPixelCoord;
					int oX=TO->RealX>>4;
					int oY=TO->RealY>>4;
					int dist=Norma(wX-oX,wY-oY);
					if( dist < TargetPointDistance+TO->newMons->UnitRadius ){
						WP->ReachedUnitID=TO->Index;
						WP->ReachedUnitSN=TO->Serial;
						return true;
					}
				}
			}
		}
		if(TargetPointReached)
			reach=true;
	}else{
		if(TargetPointReached){
			int RD=Norma(WP->To.x-WP->x,WP->To.y-WP->y)>>ToPixelCoord;
			if(RD<=TargetPointDistance)
				reach=true;
		}
	}

	int H=-10000;
	if(EarthOrWaterReached){
		H=GetTotalHeight(WP->x>>ToPixelCoord,WP->y>>ToPixelCoord);
		if(H<0)H=0;		
		if( (WP->z>>ToPixelCoord) <= H )
			return true;
	}

	return reach;
}
IsFirstStep::IsFirstStep()
{
	Not=false;
}
bool IsFirstStep::Check(WeaponParams* WP)
{
	bool rez=false;
	if(WP&&!WP->OnceProcesed)
		rez=true;
	if(Not)
		rez=!rez;
	return rez;
}
//==================================================================================================================//
int DurationPoint::GetDuration(WeaponParams* WP){
	switch(type) {
	case 0: return (TRUETIME-WP->BirthTime);
		break;
	case 1: return Norma((WP->x>>ToPixelCoord)-(WP->From.x>>ToPixelCoord),(WP->y>>ToPixelCoord)-(WP->From.y>>ToPixelCoord));
		break;
	case 2: return Norma((WP->To.x>>ToPixelCoord)-(WP->x>>ToPixelCoord),(WP->To.y>>ToPixelCoord)-(WP->y>>ToPixelCoord));
		break;
	case 3: 
		if ((WP->To.x==WP->From.x)&&(WP->To.y==WP->From.y)) {
			return 1000;
		}else return (1000*Norma((WP->x>>ToPixelCoord)-(WP->From.x>>ToPixelCoord),(WP->y>>ToPixelCoord)-(WP->From.y>>ToPixelCoord))/Norma((WP->To.x>>ToPixelCoord)-(WP->From.x>>ToPixelCoord),(WP->To.y>>ToPixelCoord)-(WP->From.y>>ToPixelCoord)));
		break;
	case 4: 		
		if ((WP->To.x==WP->To.x)&&(WP->To.y==WP->To.y)) {
			return 1000;
		}else return (1000*Norma((WP->To.x>>ToPixelCoord)-(WP->x>>ToPixelCoord),(WP->To.y>>ToPixelCoord)-(WP->y>>ToPixelCoord))/Norma((WP->To.x>>ToPixelCoord)-(WP->From.x>>ToPixelCoord),(WP->To.y>>ToPixelCoord)-(WP->From.y>>ToPixelCoord)));
		break;
	default: return 0;
	}
}
//==================================================================================================================//
bool graph_ChangeScale::CanDraw(WeaponParams* WP)
{
	return WP->IsOnScreen();
}
bool graph_ChangeScale::Draw(WeaponParams* WP)
{
	int i;
	int koef=1000;
	if (typeD.type!=3&&typeD.type!=4&&Use_Zoom){
		koef=Zoom;
	}
	int CT=typeD.GetDuration(WP);
	WP->Scale=StartScale;
	for(i=0;i<ScalePoints.GetAmount();i++){
		if(ScalePoints[i]->Duration*koef>CT){
			if(i==0){
				WP->Scale=StartScale+CT*(ScalePoints[i]->FinalScale-StartScale)/(ScalePoints[i]->Duration*koef);
			}else{
				WP->Scale=ScalePoints[i-1]->FinalScale+(CT-ScalePoints[i-1]->Duration*koef)*(ScalePoints[i]->FinalScale-ScalePoints[i-1]->FinalScale)/(ScalePoints[i]->Duration*koef-ScalePoints[i-1]->Duration*koef);
			}
			return true;
		}
	}
	if(i==ScalePoints.GetAmount()){
		WP->Scale=ScalePoints[i-1]->FinalScale;
	}
	return true;
}
DWORD			graph_ChangeScale::GetClassMask(){
	if (typeD.type==3||typeD.type==4)
					return	0x00000002;
	if (Use_Zoom)	return	0x00000004;
					return	0x00000001;
};
//==================================================================================================================//
bool graph_ChangeColor::CanDraw(WeaponParams* WP)
{
	return WP->IsOnScreen();
}
DWORD graph_ChangeColor::GetCurrentColor(int step, int CurPos, int koef){
	int bc_k,bc_x,bc_y,bc_z,bc_d,bc_a,bc_b,bc_c,bc_h,bc_e,bc_f,bc_g;
	DWORD tt;
	if(step==0){
		int w=CurPos*255/(ColorPoints[step]->Duration*koef);
		tt=MixDWORD(StartColor,ColorPoints[step]->FinalColor,255-w,w);
	}else{
		int w=(CurPos-ColorPoints[step-1]->Duration*koef)*255/(ColorPoints[step]->Duration*koef-ColorPoints[step-1]->Duration*koef);
		tt=MixDWORD(ColorPoints[step-1]->FinalColor,ColorPoints[step]->FinalColor,255-w,w);
	}
	return tt;
}
bool graph_ChangeColor::Draw(WeaponParams* WP)
{
	int i;
	int koef=1000;
	if (typeD.type!=3&&typeD.type!=4&&Use_Zoom){
		koef=Zoom;
	}
	int CT=typeD.GetDuration(WP);
	WP->Color=StartColor;
	for(i=0;i<ColorPoints.GetAmount();i++){
		if(ColorPoints[i]->Duration*koef>CT){
			WP->Color=GetCurrentColor(i, CT, koef);
			return true;
		}
	}
	//	WP->Color=;
	if(i==ColorPoints.GetAmount() && i>0){
		WP->Color=ColorPoints[i-1]->FinalColor;
	}
	return true;
}
DWORD			graph_ChangeColor::GetClassMask(){
	if (typeD.type==3||typeD.type==4)
		return	0x00000002;
	if (Use_Zoom)	return	0x00000004;
	return	0x00000001;
};
//==================================================================================================================//
bool graph_Rotate::CanDraw(WeaponParams* WP)
{
	return WP->IsOnScreen();
}
#define PI 3.14159265358979
bool graph_Rotate::Draw(WeaponParams* WP){
	int i;
	int koef=1000;
	if (typeD.type!=3&&typeD.type!=4&&Use_Zoom){
		koef=Zoom;
	}
	int CT=typeD.GetDuration(WP);
	WP->ExtraFiDir=StartFiDir*2*PI;
	WP->ExtraFiOrt=StartFiOrt*2*PI;
	WP->ExtraDir=StartDir*255;
	for(i=0;i<DirPoints.GetAmount();i++){
		if(DirPoints[i]->Duration*koef>CT){
			if(i==0){
				if(DirPoints[i]->cycle1){
					WP->ExtraFiDir=(StartFiDir+CT*(DirPoints[i]->FinalFiDir-StartFiDir+1)/(DirPoints[i]->Duration*koef))*2*PI;
				}else{
					WP->ExtraFiDir=(StartFiDir+CT*(DirPoints[i]->FinalFiDir-StartFiDir)/(DirPoints[i]->Duration*koef))*2*PI;
				}
				if(DirPoints[i]->cycle2){
					WP->ExtraFiOrt=(StartFiOrt+CT*(DirPoints[i]->FinalFiOrt-StartFiOrt+1)/(DirPoints[i]->Duration*koef))*2*PI;
				}else{
					WP->ExtraFiOrt=(StartFiOrt+CT*(DirPoints[i]->FinalFiOrt-StartFiOrt)/(DirPoints[i]->Duration*koef))*2*PI;
				}
				if(DirPoints[i]->cycle3){
					WP->ExtraDir=(StartDir+CT*(DirPoints[i]->FinalDir-StartDir+1)/(DirPoints[i]->Duration*koef))*255;
				}else{
					WP->ExtraDir=(StartDir+CT*(DirPoints[i]->FinalDir-StartDir)/(DirPoints[i]->Duration*koef))*255;
				}
			}else{
				if(DirPoints[i]->cycle1){
					WP->ExtraFiDir=(DirPoints[i-1]->FinalFiDir+(CT-DirPoints[i-1]->Duration*koef)*(DirPoints[i]->FinalFiDir-DirPoints[i-1]->FinalFiDir+1)/(DirPoints[i]->Duration*koef-DirPoints[i-1]->Duration*koef))*2*PI;
				}else{
					WP->ExtraFiDir=(DirPoints[i-1]->FinalFiDir+(CT-DirPoints[i-1]->Duration*koef)*(DirPoints[i]->FinalFiDir-DirPoints[i-1]->FinalFiDir)/(DirPoints[i]->Duration*koef-DirPoints[i-1]->Duration*koef))*2*PI;
				}
				if(DirPoints[i]->cycle2){
					WP->ExtraFiOrt=(DirPoints[i-1]->FinalFiOrt+(CT-DirPoints[i-1]->Duration*koef)*(DirPoints[i]->FinalFiOrt-DirPoints[i-1]->FinalFiOrt+1)/(DirPoints[i]->Duration*koef-DirPoints[i-1]->Duration*koef))*2*PI;
				}else{
					WP->ExtraFiOrt=(DirPoints[i-1]->FinalFiOrt+(CT-DirPoints[i-1]->Duration*koef)*(DirPoints[i]->FinalFiOrt-DirPoints[i-1]->FinalFiOrt)/(DirPoints[i]->Duration*koef-DirPoints[i-1]->Duration*koef))*2*PI;
				}
				if(DirPoints[i]->cycle3){
					WP->ExtraDir=(DirPoints[i-1]->FinalDir+(CT-DirPoints[i-1]->Duration*koef)*(DirPoints[i]->FinalDir-DirPoints[i-1]->FinalDir+1)/(DirPoints[i]->Duration*koef-DirPoints[i-1]->Duration*koef))*255;
				}else{
					WP->ExtraDir=(DirPoints[i-1]->FinalDir+(CT-DirPoints[i-1]->Duration*koef)*(DirPoints[i]->FinalDir-DirPoints[i-1]->FinalDir)/(DirPoints[i]->Duration*koef-DirPoints[i-1]->Duration*koef))*255;
				}
			}
			return true;
		}
	}
	if(i==DirPoints.GetAmount()){
				WP->ExtraFiDir=DirPoints[i-1]->FinalFiDir*2*PI;
				WP->ExtraFiOrt=DirPoints[i-1]->FinalFiOrt*2*PI;
				WP->ExtraDir=DirPoints[i-1]->FinalDir*255;
	}
	return true;
}
DWORD			graph_Rotate::GetClassMask(){
	if (typeD.type==3||typeD.type==4)
					return	0x00000002;
	if (Use_Zoom)	return	0x00000004;
					return	0x00000001;
};

bool RandomEvent::Check(WeaponParams* WP){
    return rando()<Probability*32768.0f;
}
