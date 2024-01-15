// Plugin.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "Abilities.h"
#include "Actions.h"

//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

DLL_Interface GI;

__declspec(dllexport) BaseClass* GetInterface(){
	return &GI;
}

//////////////////////////////////////////////////////////////////////////

class AboutString: public BaseClass {
public:
	_str Str;
	SAVE(AboutString){
		REG_AUTO(Str);
	}ENDSAVE;
};
class AboutArray: public ReferableBaseClass {
public:
	ClonesArray<AboutString> Strings;
	SAVE(AboutArray){
		REG_PARENT(ReferableBaseClass);
		REG_AUTO(Strings);
	}ENDSAVE;
	virtual bool CheckIfObjectIsGlobal() { return true; }

	void Load() {
		Strings.Clear();
		int i=0;
		char pp[100];
		while(true){
			sprintf(pp,"ABLINE%d",i++);
			char* tx=GetTextByID(pp);
			if( tx!=pp && tx[0]!=0 ){
				AboutString* AS=new AboutString;
				AS->Str=tx;
				Strings.Add(AS);
			}else{
				break;
			}
		}
	}

};

AboutArray* AA=NULL;

class MyOrder:public IGroupOrder{
public:
	virtual bool Process(){
		return true;
	}
};

class ObjectInterface: public ReferableBaseClass {
public:
	bool HoldPosition;

	class HoldPositionLeftClick: public BaseFunction {
	public:
		SAVE(HoldPositionLeftClick){
			REG_PARENT(BaseFunction);
		}ENDSAVE;
		void EvaluateFunction(){
			ObjectInterface* P=get_parent<ObjectInterface>();
			if(P){
				if(P->HoldPosition){
				}else{
				}
			}
		};
	};

	// отображение над героем вещи, которую только что подобрал
	int mcTimeActivation;
	word mcIndex;
	word mcSerial;

	bool mcVisible;
	DWORD mcColor;
	short mcFileID;
	int mcSpriteID;
	int mcX;
	int mcY;
	float mcScaleX;
	float mcScaleY;

	SAVE(ObjectInterface){
		REG_PARENT(ReferableBaseClass);
		REG_MEMBER(_bool,HoldPosition);

		REG_MEMBER(_bool,mcVisible);
		REG_MEMBER(_color,mcColor);
		REG_MEMBER(_gpfile,mcFileID);
		REG_SPRITE(mcSpriteID,mcFileID);
		REG_MEMBER(_int,mcX);
		REG_MEMBER(_int,mcY);
		REG_MEMBER(_float,mcScaleX);
		REG_MEMBER(_float,mcScaleY);

	}ENDSAVE;

	virtual bool CheckIfObjectIsGlobal() { return true; }
	void Process();

};
ObjectInterface OI;

DIALOGS_API void SetAbilityInfoActivation(int& mcTimeActivation, word& mcIndex, word& mcSerial, short& mcFileID, int& mcSpriteID, int& mcX, int& mcY);

void ObjectInterface::Process(){

	static int x,y;

	mcVisible=false;

	const Period=3000;
	const Fade=Period/5;

	int t=GetTickCount();
	int dt=t-mcTimeActivation;

		if( /*dt>=0 &&*/ dt<Period ){

		pUnit pHero(mcIndex,mcSerial);
		OneObject* OB=pHero.object();
		if(OB){
			
			mcVisible=true;

			byte a=0xFF;
			if( dt<0 ){
				a=0;
			}else
			if( dt>0 && dt<Fade ){
				a=dt*0xFF/Fade;
			}else
			if( dt>Period-Fade ){
				a=(Period-dt)*0xFF/Fade;
			}
			mcColor=(a<<24)+0xFFFFFF;

			//Log.Warning("%d",(int)a);

			Vector3D v(x,y,200);
			//v.x=OB->RealX>>4;
			//v.y=OB->RealY>>4;
			//v.z=OB->RZ+OB->OverEarth+150;

			GameInterface::WorldToScreenSpace(v);

			mcX=v.x-20;
			mcY=v.y;

			float W=GPS.GetGPWidth(mcFileID,mcSpriteID);
			float H=GPS.GetGPHeight(mcFileID,mcSpriteID);

			mcScaleX=37.0/W;
			mcScaleY=60.0/H;

		}

	}else{
		SetAbilityInfoActivation( mcTimeActivation, mcIndex, mcSerial, mcFileID, mcSpriteID, x, y);
	}

}

//////////////////////////////////////////////////////////////////////////
void DLL_Interface::ProcessingGame(){	
    //if(rand()<256)CreateSpecialEffect("Druid_shot",rand()%1023,rand()%1023,70,rand()%1023,rand()%1023,70);	
	//pUnitType T("DruUPik(DR)");
	//if(rand()<1){
	//	pUnit p=CreateUnit(rand()&1023,rand()&1023,rand()&7,T);
	//	ApplyAbility("UA_ClayGolem_Regeneration",p,p);
	//}
	//if(rand()<1){
	//	pUnitsList* L=new pUnitsList;
    //  uList.Add(L);
	//	CreateRandomUnits(10,rand()&1023,rand()&1023,80,rand()&7,T,L);
	//	L->AddNewOrder(new MyOrder);
	//}
	//if(TG.DoOnce()){
	//	pDialogInfo D=GetDialog("DLG1");
	//	D.Show();
	//	pUnit u1=CreateUnit(100,100,0,pUnitType("DruUPik(DR)"),true);
	//	pUnit u2=CreateUnit(200,120,0,pUnitType("DruUPik(DR)"),true);
	//	D.ShowQuestionNearUnit(u1);
	//	D.ShowAnswerNearUnit(u2);
	//}
	
	/*vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP&&SP->Type==ve_UT_units){
		OI.HoldPosition=SP->Inf.Units.HoldPosition;
	}else{
		OI.HoldPosition=false;
	}*/

    OI.Process();

}

regAc(cva_DrawCredits, vfS
	int Top;
	int Speed;
	ClassRef<SimpleDialog> OkButton;
	,
	REG_MEMBER(_int,Top);
	REG_MEMBER(_int,Speed);
	REG_AUTO(OkButton);
);

void cva_DrawCredits::SetFrameState(SimpleDialog* SD){
//void cva_DrawCredits::OnDraw(SimpleDialog* SD){

	int t=GetTickCount();
	static int StartDrawTime=t;
	if(t-StartDrawTime>2000){
		StartDrawTime=t;
	}

	int yStop=-(SD->GetHeight()-200);
	int y0=Top-(GetTickCount()-StartDrawTime)/Speed;

	SD->Sety(y0);

	if(y0<yStop){
		SimpleDialog* Ok=OkButton.Get();
		if(Ok){
			Ok->v_Actions.LeftClick(Ok);
		}
	}

};

//////////////////////////////////////////////////////////////////////////

const int OnUnitInfoDrawTime=2000;
const int OnUnitInfoFadeTime=500;

class OnUnitInfo: public BaseClass {
public:
	pUnit Unit;
	int TimeFirstActivation;
	int TimeLastActivation;

	SAVE(OnUnitInfo){
		REG_AUTO(Unit);
		REG_MEMBER(_int,TimeFirstActivation);
		REG_MEMBER(_int,TimeLastActivation);
	}ENDSAVE;

	virtual void Draw();
};

ClassArray<OnUnitInfo> OnUnitInfoArray;

void AddOnUnitInfoArray(OneObject* OB){
	if(OB){
		pUnit uVictim(OB);
		OnUnitInfo* UI=false;

		int n=OnUnitInfoArray.GetAmount();
		for(int i=0;i<n;i++){
			OnUnitInfo* ui=OnUnitInfoArray[i];
			if( ui && ui->Unit==uVictim ){
				UI=ui;
				break;
			}
		}

		int time=GameInterface::GetGlobalTime();
		if(UI){
			UI->TimeLastActivation=time;
		}else{
			UI = new OnUnitInfo;
			UI->Unit=uVictim;
			UI->TimeFirstActivation=time;
			UI->TimeLastActivation=time;
			OnUnitInfoArray.Add(UI);
		}
	}
}


CIMPORT void DrawHealthEx(OneObject* OB);

void VUI_IExchangeInterface::AddOnUnitInfoArray(OneObject* OB){
	::AddOnUnitInfoArray(OB);
};

void OnUnitInfo::Draw(){
	DrawHealthEx(Unit.object());
};

//////////////////////////////////////////////////////////////////////////

bool DLL_Interface::OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType){
	AddOnUnitInfoArray(DamagedUnit);
	return true;
};

void DLL_Interface::OnDrawOnMapAfterTransparentEffects(){

	int time=GetGlobalTime();

	int n=OnUnitInfoArray.GetAmount();
	for(int i=0;i<n;i++){
		OnUnitInfo* ui=OnUnitInfoArray[i];
		if( ui ){
			if( time > ui->TimeLastActivation+OnUnitInfoDrawTime+2*OnUnitInfoFadeTime ){
				// delete
				OnUnitInfoArray.DelElement(i);
				i--;
				n--;
			}else{
				ui->Draw();
			}
		}
	}

};

//////////////////////////////////////////////////////////////////////////

void DLL_Interface::OnUnloading(){

	OnUnitInfoArray.Clear();

}

void DLL_Interface::OnClassRegistration(){
	REG_CLASS(AboutArray);
	if(!AA){
		AA=new AboutArray;
        AA->Name="AboutArray";
		AA->GetObjectGlobalID();
		AA->Load();
	}

	REG_CLASS(ObjectInterface);
	OI.Name="ObjectInterface";
	OI.GetObjectGlobalID();

	//REG_CLASS_EX(cva_DrawCredits,		"MainMenu");

	REG_CLASS(OnUnitInfo);

	REG_CLASS(suAI_Ability);
	REG_CLASS(suAI_Abilities);
	REG_CLASS(ua_SingleUnitAI);
	REG_CLASS(aa_SingleUnitAI);

	// Actions
};

void DLL_Interface::OnClassUnRegister(){
	UNREG_CLASS("AboutArray");

	UNREG_CLASS("ObjectInterface");

	//UNREG_CLASS("cva_DrawCredits");

	UNREG_CLASS("OnUnitInfo");
}

//////////////////////////////////////////////////////////////////////////
