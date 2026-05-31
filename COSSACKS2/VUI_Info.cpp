#include "stdheader.h"
#include "UnitsInterface.h"
#include "unitability.h"
#include ".\cvi_brigcreate.h"
#include ".\cvi_market.h"
#include ".\cvi_academy.h"
//////////////////////////////////////////////////////////////////////////
extern int VitCursor;
bool GetPushkaChargeState(OneObject*,int&,int&);
void CmdAttackGround(byte NI,int x,int y);
int CreateStandartUnitsInterface();
//////////////////////////////////////////////////////////////////////////

void ACT(int x);

xmlQuote vxSelPoint;
xmlQuote vxWeapons;
xmlQuote vxMiniCom;
xmlQuote vxProduceDsk;
xmlQuote vxProduceGP;
xmlQuote vxUpgradeGP;

DialogsSystem ds;
DialogsSystem vdsWeap;
DialogsSystem vdsMini;
DialogsSystem vdsProduce;
DialogsSystem vdsUpgrade;

DialogsSystem* vdsSel=NULL;			// selection
DialogsDesk* vddAbility=NULL;
SimpleDialog* vsdAbilityOne=NULL;
DialogsDesk* vddActAbility=NULL;		// active ability from OneObject
SimpleDialog* vsdActAbilityOne=NULL;
DialogsDesk* vddInside=NULL;			// buildings inside list of units
SimpleDialog* vsdInsideOne=NULL;

void vgf_UI_Clear(){
	vxSelPoint.ClearAll();	
	vxWeapons.ClearAll();	
	vxMiniCom.ClearAll();	
	vxProduceDsk.ClearAll();	
	vxProduceGP.ClearAll();
	vxUpgradeGP.ClearAll();
	ds.DSS.Clear();
	vdsWeap.DSS.Clear();
	vdsMini.DSS.Clear();
	vdsProduce.DSS.Clear();
	vdsUpgrade.DSS.Clear();
	if(vdsSel){ delete(vdsSel); vdsSel=NULL; }
	if(vddAbility){ delete(vddAbility); vddAbility=NULL; }
	if(vsdAbilityOne){ delete(vsdAbilityOne); vsdAbilityOne=NULL; }
	if(vddActAbility){ delete(vddActAbility); vddActAbility=NULL; }
	if(vsdActAbilityOne){ delete(vsdActAbilityOne); vsdActAbilityOne=NULL; }
}
//
bool vGoToMine=false;
bool vTakeRes=false;
bool vBuildObj=false;
//
void SPSounds(){
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	//
	static word NIndex=0xFFFF;
	static word Index=0xFFFF;
	if(SP&&SP->NI==MyNation){		
		//if(vTakeRes||vBuildObj){
		if(Rpressed){
			if(SP->OB->newMons->Peasant){
				ACT(82);
			}else ACT(0);
		}else
		if(NIndex!=SP->OB->NIndex||Index!=SP->OB->BrigadeID){//){
			void ACT(int x);
			if(SP->OB->newMons->Peasant){
				if(NIndex!=SP->OB->NIndex){
					ACT(82);
				}else ACT(0);
			}else{
				ACT(81);
			}
			NIndex=SP->OB->NIndex;
			Index=SP->OB->BrigadeID;
		}else{
			ACT(0);
		}
	}else{
		NIndex=0xFFFF;
		Index=0xFFFF;
		ACT(0);
	}
	vTakeRes=false;
	vBuildObj=false;
}
//
int vgf_UI_Process(){

	vui_SelPoint* SP=OIS.GetLastSelPoint();

	if(SP){
		SP->SetActiveUnitAbility(SP->ActAbl);
	}

	return 4;

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	static int pX;
	static int pY;

	if(!vxSelPoint.GetNSubQuotes()){
		vxSelPoint.ReadFromFile("Dialogs\\v\\SelPoint.DialogsDesk.Dialogs.xml");
		vxWeapons.ReadFromFile("Dialogs\\v\\Weapons.DialogsDesk.Dialogs.xml");
		vxMiniCom.ReadFromFile("Dialogs\\v\\MiniCom.DialogsDesk.Dialogs.xml");
		vxProduceGP.ReadFromFile("Dialogs\\v\\UnitProduce.GPPicture.Dialogs.xml");
		vxUpgradeGP.ReadFromFile("Dialogs\\v\\Upgrade.VitButton.xml");
		//
		DialogsSystem* D=(DialogsSystem*)&vdsProduce;
		for(int i=0;i<16;i++){
			ErrorPager ep(0);
			GPPicture* P;
			P=new GPPicture;
			int lsp=OIS.LastSP;
			OIS.LastSP=-1;
			P->Load(vxProduceGP,P,&ep);
			OIS.LastSP=lsp;
			D->DSS.Add(P);
			P->ID=i;
			pX=P->x;
			pY=P->y;
		}
	}

	if(vdsWeap.DSS.GetAmount()==0){		
		if(!vdsSel){ 
			vdsSel=new DialogsSystem; 
			if(vdsSel->ReadFromFile("Dialogs\\v\\Sel.DialogsSystem.xml")){				
				vdsSel->Copy(&ds);
				vdsSel->Copy(&vdsWeap);
				vdsSel->Copy(&vdsMini);
				vdsSel->Copy(&vdsProduce);
				vdsSel->Copy(&vdsUpgrade);
				if(!vddAbility){ 
					vddAbility=new DialogsDesk; 
					if(vddAbility->SafeReadFromFile("Dialogs\\v\\Ability.DialogsDesk.dialogs.xml")){
						vsdAbilityOne=(SimpleDialog*)vddAbility->DSS[0]->new_element();
						vddAbility->DSS[0]->Copy(vsdAbilityOne);
						//vddAbility->DSS.Clear();
						vdsSel->DSS.Add(vddAbility);
					}else{
						delete vddAbility;
						vddAbility=NULL;
					};
				}
				if(!vddActAbility){
					vddActAbility=new DialogsDesk; 
					if(vddActAbility->SafeReadFromFile("Dialogs\\v\\ActAbility.DialogsDesk.dialogs.xml")){
						vsdActAbilityOne=(SimpleDialog*)vddActAbility->DSS[0]->new_element();
						vddActAbility->DSS[0]->Copy(vsdActAbilityOne);
						//vddActAbility->DSS.Clear();
						vdsSel->DSS.Add(vddActAbility);
					}else{
						delete vddActAbility;
						vddActAbility=NULL;
					};
				}
				if(!vddInside){
					vddInside=new DialogsDesk; 
					if(vddInside->SafeReadFromFile("Dialogs\\v\\Inside.DialogsDesk.dialogs.xml")){
						int i=vddInside->DSS.GetAmount()-1;
						vsdInsideOne=(SimpleDialog*)vddInside->DSS[i]->new_element();
						vddInside->DSS[i]->Copy(vsdInsideOne);
						//vddInside->DSS.Clear();
						vdsSel->DSS.Add(vddInside);
					}else{
						delete vddInside;
						vddInside=NULL;
					};
				}				
			}else{
				delete vdsSel;
				vdsSel=NULL;
			}
		}

		ErrorPager ep(0);
		DialogsDesk* DD;
		DD=new DialogsDesk;
		DD->Load(vxWeapons,DD,&ep);
		vdsWeap.DSS.Add(DD);
		DD=new DialogsDesk;
		DD->Load(vxMiniCom,DD,&ep);
		vdsMini.DSS.Add(DD);
	}

	int i;
	int n=OIS.SelPoint.GetAmount();
	//vui_SelPoint* SP=OIS.GetLastSelPoint();

	// sound script, sorry wrong place for enter
	void SPSounds();
	SPSounds();
	//
	static bool _3row=false;
	static int _3r_Lx=-1;
	static int _3r_Ly=-1;
	static bool _new=false;
	//
	if(vdsSel){
		// Brig Create
		SimpleDialog* SD=vBrigCreate.Init();
		if(SD){
			vdsSel->DSS.Add(SD);
		}
		if(SP){
			byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
			vBrigCreate.Process(SP->OB, n==1&&SP->NI==NI);
		}
		// Market
		vMarket=&vMarketInGame;	
		SD=vMarket->Init();
		if(SD){
			vdsSel->DSS.Add(SD);
		}
		vMarket->Process();
		// Academy
		vAcademy.Process(vdsSel);
	}	
	//
	if(!_new){
		// width of SelPoint sides	
		OIS.SPSideLx=35;	
		if(SP&&SP->Type==ve_UT_building) OIS.SPSideLx=80;
		if(OIS.SelPoint.GetAmount()>1){
			int PosibleLx=RealLx-744;
			if(SP->Type==ve_UT_building) PosibleLx=RealLx-400;
			//
			int nLx=PosibleLx/(OIS.SelPoint.GetAmount()-1);
			if(nLx<OIS.SPSideLx){
				if(nLx>20) OIS.SPSideLx=nLx;
					else OIS.SPSideLx=20;
			}
			int nPosible=PosibleLx/OIS.SPSideLx;
			if(nPosible<OIS.SelPoint.GetAmount()){
				OIS.SelPoint.Del(nPosible,1000);
			}
		}
	}
	n=OIS.SelPoint.GetAmount();
	for(i=ds.DSS.GetAmount();(i<n||i<20)&&!_new;i++){
		//vui_SelPoint* SP=OIS.SelPoint+i;
		DialogsDesk* DD=new DialogsDesk;
		ErrorPager ep(0);
		int lsp=OIS.LastSP;
		OIS.LastSP=-1;
		DD->Load(vxSelPoint,DD,&ep);
		OIS.LastSP=lsp;
		ds.DSS.Add(DD);
		if(DD->Name=="3ROW") _3row=true;
		if(DD->v_Actions.GetAmount()) _new=true;
	}
	//
	if(n==0) return CreateStandartUnitsInterface();
	//
	if(!_new){
		n+=OIS.LastSP-1;
		//
		int nsp=ds.DSS.GetAmount();
		for(i=0;i<nsp;i++){
			DialogsDesk* DD=(DialogsDesk*)ds.DSS[i];
			if(i<OIS.SelPoint.GetAmount()){
				DD->Visible=1;
				if(i<OIS.LastSP){
					DD->ID=i;
					{
						DD->DSS[0]->Visible=0;
						DD->DSS[1]->Visible=1;
						DD->DSS[2]->Visible=0;
					}
				}else{
					DD->ID=n-i;
					if(DD->ID==OIS.LastSP){
						DD->DSS[0]->Visible=0;
						DD->DSS[1]->Visible=0;
						DD->DSS[2]->Visible=1;
					}else{
						DD->DSS[0]->Visible=1;
						DD->DSS[1]->Visible=0;
						DD->DSS[2]->Visible=0;
					}
				}
				if(_3row){
					if(OIS.SelPoint.GetAmount()==1){
						DD->DSS[0]->Visible=0;
						DD->DSS[1]->Visible=0;
						DD->DSS[2]->Visible=1;
						DD->Setx(0);
						DD->Sety(1);
					}else{
						DD->DSS[0]->Visible=0;
						DD->DSS[1]->Visible=1;
						DD->DSS[2]->Visible=0;
						if(_3r_Lx==-1){
							GPPicture* GP=(GPPicture*)DD->DSS[1]->DSS[0];
							_3r_Lx=GP->GetWidth();
							_3r_Ly=GP->GetHeight();
						}					
						DD->Setx((DD->ID/3)*_3r_Lx);
						DD->Sety((DD->ID%3)*_3r_Ly);
					}
				}else{
					DD->Setx(DD->ID*OIS.SPSideLx);
				}			
			}else{
				DD->Visible=0;
			}
		}
	}
	// Produce
	DialogsSystem* D=(DialogsSystem*)&vdsProduce;
	n=OIS.Produce.GetAmount();
	for(i=D->DSS.GetAmount();i<n;i++){
		ErrorPager ep(0);
		GPPicture* P;
		P=new GPPicture;
		int lsp=OIS.LastSP;
		OIS.LastSP=-1;
		P->Load(vxProduceGP,P,&ep);
		OIS.LastSP=lsp;
		D->DSS.Add(P);
		P->ID=i;
		//pX=P->x;
		//pY=P->y;
	}

	int Dx=EngSettings.BuildIconLx+6;
	int Dy=EngSettings.BuildIconLy+6;
	//if(P->GetHeight()<70) Dy=67;

	for(i=0;i<n;i++){
		GPPicture* P=(GPPicture*)D->DSS[i];
		P->Visible=true;
		vui_ProduceInfo* I=OIS.Produce+i;
		P->Setx(pX+I->x*Dx);
		P->Sety(pY+I->y*Dy); //(I->y-1)
	}
	for(i=n;i<D->DSS.GetAmount();i++){
		GPPicture* P=(GPPicture*)D->DSS[i];
		P->Visible=false;
	}
	// Upgrade
	D=(DialogsSystem*)&vdsUpgrade;
	n=OIS.Upgrade.GetAmount();
	static int uX;
	static int uY;
	for(i=D->DSS.GetAmount();i<n;i++){
		ErrorPager ep(0);
		VitButton* P;
		P=new VitButton;
		int lsp=OIS.LastSP;
		OIS.LastSP=-1;
		P->Load(vxUpgradeGP,P,&ep);
		OIS.LastSP=lsp;
		D->DSS.Add(P);
		P->ID=i;
		uX=P->x;
		uY=P->y;
	}
	for(i=0;i<n;i++){
		VitButton* P=(VitButton*)D->DSS[i];
		P->Visible=true;
		vui_UpgradeInfo* I=OIS.Upgrade+i;
		P->Setx(uX+I->x*67);
		P->Sety(uY+I->y*67);
	}
	for(i=n;i<D->DSS.GetAmount();i++){
		VitButton* P=(VitButton*)D->DSS[i];
		P->Visible=false;
	}
	int NSP=OIS.SelPoint.GetAmount();
	// Ability	
	if(vddAbility){
		/*
		if(SP&&SP->Abl&&vdsSel&&vsdAbilityOne&&NSP==1){
			vddAbility->Visible=true;
			int lsp=OIS.LastSP;
			OIS.LastSP=-1;
			for(int n=SP->Abl->AbilitiesList.GetAmount()-vddAbility->DSS.GetAmount();n>0;n--){
				SimpleDialog* SD=(SimpleDialog*)vsdAbilityOne->new_element();
				vsdAbilityOne->Copy(SD);
				vddAbility->DSS.Add(SD);
			}
			OIS.LastSP=lsp;
			int x=0;
			for(n=SP->Abl->AbilitiesList.GetAmount()-1;n>=0;n--){
				UnitAbility* UA=SP->Abl->AbilitiesList[n];
				GPPicture* GP=dynamic_cast<GPPicture*>(vddAbility->DSS[n]);
				if(UA&&GP&&UA->Visible){
					GP->ID=n;
					GP->SetHint(GP,&UA->Name);
					GP->SetFileID(&UA->FileID);
					GP->SetSpriteID(UA->SpriteID);
					GP->Setx(x);
					x+=GP->GetWidth();
					GP->Visible=true;
				}else{
					GP->Visible=false;
				}
			}
			for(n=SP->Abl->AbilitiesList.GetAmount();n<vddAbility->DSS.GetAmount();n++){
				GPPicture* GP=dynamic_cast<GPPicture*>(vddAbility->DSS[n]);
				if(GP){
					GP->Visible=false;
				}
			}			
		}else*/{
			vddAbility->Visible=false;
		}
	}	
	// Active Ability
	// fantasy
	DialogsDesk* vActAblDD=NULL;
	if(SP&&SP->ActAbl){
		vActAblDD=dynamic_cast<DialogsDesk*>(vdsWeap.Find("ActAbl"));
		if(vActAblDD){
			SP->SetActiveUnitAbility(SP->ActAbl);
		}
	}
	// alexander, cossacks2
	if(vddActAbility){
		if(SP&&SP->ActAbl&&vdsSel&&vsdActAbilityOne/*&&NSP==1*//*&&SP->Type==ve_UT_cannon*/){
			vddActAbility->Visible=true;
			//
			for(int n=0;n<vddActAbility->DSS.GetAmount();n++){
				GPPicture* GP=dynamic_cast<GPPicture*>(vddActAbility->DSS[n]);
				if(GP){					
					GP->UserParam=0;
				}
			}		
			//
			int lsp=OIS.LastSP;
			OIS.LastSP=-1;
			int nAdd=SP->ActAbl->ActiveAbilities.GetAmount()-vddActAbility->DSS.GetAmount();			
			for(int n=nAdd;n>0;n--){
				SimpleDialog* SD=(SimpleDialog*)vsdActAbilityOne->new_element();
				vsdActAbilityOne->Copy(SD);
				vddActAbility->DSS.Add(SD);
			}
			OIS.LastSP=lsp;
			int x=0;
			int a=SP->ActAbl->ActiveAbilities.GetAmount();
			for(n=0;n<a;n++){
				ActiveUnitAbility* UA=SP->ActAbl->ActiveAbilities[n];
				GPPicture* GP=dynamic_cast<GPPicture*>(vddActAbility->DSS[n]);
				if(GP){					
					//GP->Visible=false;
					if(UA&&UA->Visible){
						//CannonAutoShotActive* ac=dynamic_cast<CannonAutoShotActive*>(UA);						
						//if(ac){ //!(ac&&ac->On)
							GP->ID=n;
							GP->UserParam=(int)UA;
							//GP->SetHint(UA->GetHint());
							//GP->SetFileID(&UA->FileID);
							//GP->SetSpriteID(UA->SpriteID);
							GP->Setx(x);
							x+=GP->GetWidth();
							//GP->Visible=true;
							/*
							GPPicture* gp=NULL;
							if(GP->DSS.GetAmount()>0) gp=dynamic_cast<GPPicture*>(GP->DSS[0]);
							if(gp){
							}
							*/
						//}
					}
				}
			}
		}else{
			vddActAbility->Visible=false;
		}
	}
	// Inside
	if(vddInside){
		if(vdsSel&&vsdInsideOne&&NSP==1&&SP&&
			(SP->Type==ve_UT_building&&SP->Inf.Buildings.Amount==1||
			SP->OB->NInside&&SP->Type==ve_UT_units&&SP->Inf.Units.Amount==1)){
			vddInside->Visible=true;
			//
			//OneObject* OB=SP->Inf.Buildings.OB[0];
			OneObject* OB=SP->OB;
			//int N=OB->NInside;			
			word* ID=OB->Inside;
			DynArray<word> InLstN;
			DynArray<word> InMaxN;
			DynArray<word> NIndex;
			for(int i=0;i<OB->NInside;i++){
				OneObject* ob=Group[ID[i]];
				if(ob){
					AdvCharacter* adc=ob->MoreCharacter;
					int idx=-1;
					for(int j=0;j<NIndex.GetAmount();j++){
						if(ob->NIndex==NIndex[j]){
							InLstN[j]++;
							idx=j;
							break;
						}else				
						if(ob->NIndex>NIndex[j]){
							InLstN.Insert(j,1);
							InMaxN.Insert(j,adc->MaxInside+ob->AddInside);
							NIndex.Insert(j,ob->NIndex);
							idx=j;
							break;
						}
					}
					if(idx==-1){
						InLstN.Add(1);
						InMaxN.Add(adc->MaxInside+ob->AddInside);
						NIndex.Add(ob->NIndex);
					}
				}
			}
			// add new
			int lsp=OIS.LastSP;
			OIS.LastSP=-1;
			for(int n=NIndex.GetAmount()-vddInside->DSS.GetAmount()+1;n>0;n--){
				SimpleDialog* SD=(SimpleDialog*)vsdInsideOne->new_element();
				vsdInsideOne->Copy(SD);
				vddInside->DSS.Add(SD);
			}
			OIS.LastSP=lsp;
			// parameters
			int x=vsdInsideOne->Getx();
			//if(SP->Type==ve_UT_units) x=120;
			for(i=0;i<NIndex.GetAmount();i++){
				//ActiveUnitAbility* UA=SP->ActAbl->ActiveAbilities[i];
				GPPicture* GP;
				if(i+1<vddInside->DSS.GetAmount()){
					GP=dynamic_cast<GPPicture*>(vddInside->DSS[i+1]);
				}else{
					GP=dynamic_cast<GPPicture*>(vddInside->DSS[i]);
				}				
				if(GP){
					GP->ID=NIndex[i];
					GP->UserParam=InLstN[i]+(InMaxN[i]<<16);
					int w=GP->GetWidth();
					int h=GP->GetHeight();
					GP->Setx(x+i/3*w);
					GP->Sety((i%3)*h);
					GP->Visible=true;
				}
			}
			// hide lishnie
			for(i=NIndex.GetAmount();i<vddInside->DSS.GetAmount()-1;i++){
				GPPicture* GP=dynamic_cast<GPPicture*>(vddInside->DSS[i+1]);
				if(GP){
					GP->Visible=false;
				}
			}		
			//
			SimpleDialog* sdLeaveAll=vddInside->DSS[0];
			sdLeaveAll->Visible=SP->OB->NInside;
			//
		}else{
			vddInside->Visible=false;
		}
	}
	//
	int y=RealLy-768;
	if(vdsSel) vdsSel->Sety(y);
	ds.Sety(y);
	vdsWeap.Sety(y);
	vdsMini.Sety(y);
	vdsProduce.Sety(y);
	vdsUpgrade.Sety(y);
	//////////////////////////////////////////////////////////////////////////
	ds.ProcessDialogs();
	//////////////////////////////////////////////////////////////////////////
	//bool miniShow=true;
	static int vWeapX=vdsWeap.Getx();
	if(_new){
		vdsWeap.Setx(vWeapX+OIS.SPSideLx);
	}else
	if(_3row){
		//static int vx=vdsWeap.Getx();
		int n=OIS.SelPoint.GetAmount();
		if(n==1){
			vdsWeap.Setx(120);
			//vdsMini.Setx(120);			
		}else{
			vdsWeap.Setx(((n-1)/3)*_3r_Lx);
			//miniShow=false;
		}
	}else vdsWeap.Setx((OIS.SelPoint.GetAmount()-1)*OIS.SPSideLx);
	if(vddActAbility){
		static int aaX=vddActAbility->Getx();
		vddActAbility->Setx(aaX-vWeapX+vdsWeap.Getx());
	}
	//
	if(!(SP->OB->NInside/*||SP->OB->newMons->Priest*/)){ // from The Greate Alexander
		vdsWeap.ProcessDialogs();
	}	
	//////////////////////////////////////////////////////////////////////////
	static int vProduceX=vdsProduce.Getx();
	if(SP){
		int dx=0;
		if(SP->Type==ve_UT_units||SP->Type==ve_UT_brigade){
			vui_UnitInfo* I=&SP->Inf.Units;
			if(I->Damage[0]>0) dx+=62;
			if(I->Damage[1]>0) dx+=72;
			if(SP->Type==ve_UT_brigade){
				dx+=90;
			}
		}
		vdsProduce.Setx(vProduceX+dx);
		vdsUpgrade.Setx(vProduceX+dx);
	}
	//	
	vdsProduce.ProcessDialogs();
	vdsUpgrade.ProcessDialogs();
	//////////////////////////////////////////////////////////////////////////
	static int vMiniX=vdsMini.Getx();
	if(_new){
		vdsMini.Setx(vMiniX+OIS.SPSideLx);
	}
	if(SP&&OIS.SelPoint.GetAmount()){ //==1
		vdsMini.ProcessDialogs();
	}
	//
	if(vdsSel){
		vdsSel->ProcessDialogs();
	}
	return 4;
}

void vdf_BGI_Draw(){
	if(!(GSets.CGame.ViewMask&4))return;
	static DialogsSystem* DS=NULL;
	static bool init=false;
	// init
	if(!DS&&!init){
		init=true;
		xmlQuote xml;
		if(xml.ReadFromFile("dialogs\\v\\GlBuildSel.GPPicture.Dialogs.xml")){
			DS=new DialogsSystem;		
			ErrorPager Error(0);
			GPPicture* P=new GPPicture;
			P->Load(xml,P,&Error);
			DS->DSS.Add(P);
		}
	}
	if(DS){
		DS->ProcessDialogs();
	}
}