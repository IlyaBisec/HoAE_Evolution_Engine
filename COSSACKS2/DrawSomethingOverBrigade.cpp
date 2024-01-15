#include "stdheader.h"
#include "unitability.h"
#include "GameExtension.h"
#include "UnitsInterface.h"
#include "vui_Actions.h"
#include ".\cvi_MainMenu.h"
//
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);
extern OISelection OIS;
//
class DrawBrigsAbility:public GameExtension{
public:
	DialogsSystem BrigAbl;
	DialogsDesk* tBrigLAbl;
	DialogsSystem BrigLAbl;
	//
	DialogsSystem BrigMarkers;
	DialogsSystem BrigMarkersMini;
	//
	DrawBrigsAbility(){
		tBrigLAbl=NULL;
	}
	virtual void OnClassRegistration(){
		if(!tBrigLAbl){
			tBrigLAbl=new DialogsDesk;
			if(!tBrigLAbl->SafeReadFromFile("Dialogs\\v\\BRIG_BONUS_L.DIALOGSDESK.DIALOGS.XML")){
				Log.Warning("DrawBrigsAbility can not find file: Dialogs\\v\\BRIG_BONUS_L.DIALOGSDESK.DIALOGS.XML");
			};
			char* BrMarkXML="Dialogs\\v\\BrigMarkers.DialogsSystem.xml";
			if(!BrigMarkers.ReadFromFile(BrMarkXML)){
				Log.Warning("DrawBrigsAbility can not find file: %s",BrMarkXML);
			}
			char* BrMarkXMLm="Dialogs\\v\\BrigMarkersMini.DialogsSystem.xml";
			if(!BrigMarkersMini.ReadFromFile(BrMarkXMLm)){
				Log.Warning("DrawBrigsAbility can not find file: %s",BrMarkXMLm);
			}			
		}
	}
	virtual void OnDrawOnMapAfterTransparentEffects(){
		if(true){ //GetKeyState(VK_CONTROL)&0x8000
			bool Adv=GetKeyState(VK_SHIFT)&0x8000;
			BrigAbl.DSS.Clear();
			int aL=0;	// index of L ability
			for(int i=0;i<8;i++){
				int nb=CITY[i].NBrigs;
				for(int j=0;j<nb;j++){
					Brigade* BR=CITY[i].Brigs+j;
					if(BR->Enabled&&BR->WarType){
						int x,y;
						if(BR->GetCenter(&x,&y)){
							int H=GetHeight(x,y)+80;
                            Vector4D V=SkewPt(x,y,H);
							WorldToScreenSpace(V);							
							if(V.x>-100&&V.y>-100&&V.x<RealLx+100&&V.y<RealLy+100&&GetObjectVisibilityInFog(x,y,H,NULL)){
								if(BR->Ability&&false){
									int NX=0;
									int LX=0;
									int N=BR->Ability->GetAmount();
									for(int q=0;q<N;q++){
										BrigadeIconInfo* BI=(*BR->Ability)[q]->GetAbilityIcon();
										if(BI){
											NX++;
											LX+=BI->IconWidth;
										}
									}
									int x0=V.x-LX/2;
									int y0=V.y;
									LX=0;
									for(int q=0;q<N;q++){
										BrigadeIconInfo* BI=(*BR->Ability)[q]->GetAbilityIcon();
										if(BI){
											GPPicture* GPP=BrigAbl.addGPPicture(NULL,x0+LX-BI->LeftMargin,y0-BI->CenterY,BI->FileID,BI->SpriteID);
											if(BI->HintID.pchar())DYNHINT(GPP,BI->HintID.pchar());
											LX+=BI->IconWidth;
										}
									}
								}
								// set L ability
								DialogsDesk* DD;
								if(aL>=BrigLAbl.DSS.GetAmount()){
									DD=new DialogsDesk;
									tBrigLAbl->Copy(DD,false);
									BrigLAbl.DSS.Add(DD);
								}else{
									DD=(DialogsDesk*)BrigLAbl.DSS[aL];
								}
								DD->Visible=true;
								DD->UserParam=(BR->CT->NI<<8)+BR->ID;
								DD->Setx(V.x-DD->GetWidth()/2);
								DD->Sety(V.y-DD->GetHeight()/2);
								aL++;
							}else{
								void bbMoraleProcess(Brigade* BR);
								bbMoraleProcess(BR);								
							}
						}
					}
				}
			}			
			BrigAbl.ProcessDialogs();
			//
			for(int i=aL;i<BrigLAbl.DSS.GetAmount();i++){
				if(BrigLAbl.DSS[i]){
					BrigLAbl.DSS[i]->Visible=false;
				}
			}
			BrigLAbl.ProcessDialogs();	
		}
		BrigMarkers.ProcessDialogs();
	}
	virtual void OnDrawOnMiniMap(int x,int y,int Lx,int Ly){
		//BrigMarkersMini.Setx(x);
		//BrigMarkersMini.Sety(y);
		//BrigMarkersMini.SetWidth(Lx);
		//BrigMarkersMini.SetHeight(Ly);
		//BrigMarkersMini.ProcessDialogs();
		//		
		if(OIS.SelPoint.GetAmount()==1){
			vui_SelPoint* SP=OIS.SelPoint+0;
			byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
			if(SP->Type==ve_UT_brigade&&SP->NI==NI){
				Brigade* BR=CITY[NI].Brigs+SP->Inf.Brigade.BrigID;
				int xx,yy;
				if(BR->WarType&&BR->GetBrigadeDestination(xx,yy)){
					static int gp=GPS.PreLoadGPImage("flag1");
					x=GetXOnMiniMap(xx,yy);
					y=GetYOnMiniMap(xx,yy);					
					float scale=0.5;
					//int w=GPS.GetGPWidth(gp,0)*scale;
					//int h=GPS.GetGPHeight(gp,0)*scale;
					int w=63*scale;
					int h=88*scale;
					//
					GPS.SetScale(scale);
					GPS.ShowGP(x-w,y-h,gp,(GetTickCount()/40)%GPS.GPNFrames(gp),NI);
					GPS.SetScale(1.0);
				}
			}		
		}		
	}
};
//
extern int TrueTime;
//
void bbMoraleProcess(Brigade* BR){
	byte NI=BR->CT->NI;
	byte ID=BR->ID;
	//
	int dM=v_ISys.bbMoraleDelta;
	int Speed=v_ISys.bbMoraleSpeed;
	//int t=v_ISys.bbHideTime;
	//
	int Morale=BR->Morale;
	int M=BR->M;
	int HideTime=BR->HideTime;
	//
	int T=TrueTime;
	//
	bool Active=false;
	extern word Att;
	extern word Friend;			
	static byte ni=0xFF;
	static byte id=0xFF;
	static int time=0;
	int delay=250;
	int over=Att;
	if(over==0xFFFF){
		over=Friend;
		delay=250;
	}
	if(over!=0xFFFF){
		OneObject* OB=Group[over];
		if(OB&&OB->NNUM==NI&&OB->BrigadeID==ID){
			if(ni!=NI||id!=ID){						
				time=T+delay;
				ni=NI;
				id=ID;
			}
			if(T>time){
				//BR->HideTime=GetTickCount()+v_ISys.bbHideTime;
				Active=true;
			}
		}
	}else{
		ni=0xFF;
	}
	//
	if(M&&abs(M-Morale)>dM||Active){
		// activization
		if(!BR->mmActive){
			BR->mmActive=true;
			int t=HideTime+v_ISys.bbHideTime+v_ISys.bbFade-T;
			if(t>v_ISys.bbFade) t=v_ISys.bbFade;
			if(t<0) t=0;
			HideTime=T-t;
		}
        // process morale
		int t=T-HideTime;
		if(t>300){
			if(t>900)t=900;
			Speed=Speed*t/600*t/600;
			if(M>Morale){
				M-=Speed;
				if(M<Morale)M=Morale;
			}else{
				M+=Speed;
				if(M>Morale)M=Morale;
			}
		}
	}else{
		M=Morale;
		if(BR->mmActive&&T>HideTime+v_ISys.bbFade){
			BR->mmActive=false;
			HideTime=T;
		}
	}
	//
	BR->M=M;
	BR->HideTime=HideTime;
}
//
bool SetMorale(Canvas* C, int Morale, int MoraleMax);
void cva_BB_Morale::SetFrameState(SimpleDialog* SD){
	if(!SD->Visible) return;
	SD->Visible=false;
	byte NI=SD->UserParam>>8;
	byte ID=SD->UserParam&0xFF;
	if(ID<CITY[NI].NBrigs && NI!=7){
		Brigade* BR=CITY[NI].Brigs+ID;
		if(BR->Enabled&&BR->WarType){
			int fade=v_ISys.bbFade;
			int T=TrueTime;
			//
			bbMoraleProcess(BR);
			//
			int M=BR->M;
			int HideTime=BR->HideTime;
			//			
			if(true){ //HideTime+fade>T
				DWORD A=0; //(HideTime+fade-T)*255/fade;
				if(BR->mmActive){
					int t=T-HideTime;
					if(t>fade)t=fade;
					A=0xFF*t/fade;
				}else{
					int t=HideTime+v_ISys.bbHideTime+fade-T;
					if(t>fade)t=fade;
					if(t<0)t=0;
					A=0xFF*t/fade;
				}				
				if(A>0){
					SD->DeepColor=true;
					SD->Diffuse=(A<<24)+0xFFFFFF;
					//
					Canvas* C=dynamic_cast<Canvas*>(SD->Find("Canvas"));
					SD->Visible=SetMorale(C,M/10000,BR->MaxMorale/10000);
					SimpleDialog* sdB=SD->Find("Back");
					GPPicture* GP=dynamic_cast<GPPicture*>(sdB);
					if(GP){
						GP->Nation=NI;
					}
					VitButton* VB=dynamic_cast<VitButton*>(sdB);
					if(VB){
						VB->Nation=NI;
					}
				}
			}
		}
	}
}
void cva_DrawBrig_LAbility::SetFrameState(SimpleDialog* SD){
	if(!SD->Visible) return;
	byte NI=SD->UserParam>>8;
	byte ID=SD->UserParam&0xFF;
	if(ID<CITY[NI].NBrigs){
		Brigade* BR=CITY[NI].Brigs+ID;
		if(BR->Enabled&&BR->WarType){
			bool Adv=GetKeyState(VK_SHIFT)&0x8000;
			int NLive=0;
			int NLiveMax=BR->NMemb-NBPERSONAL;
			for(int i=NBPERSONAL;i<BR->NMemb;i++){
				OneObject* OB=Group[BR->Memb[i]];
				if(OB&&!OB->Sdoxlo&&OB->Serial==BR->MembSN[i]){
					NLive++;
				}
			}
			// Main circle
			ProgressBar* main=Main.Get();
			if(main){
				main->Visible=Adv;
				main->Nation=BR->CT->NI;
				main->MaxValue=NLiveMax;
				main->Value=NLive;									
			}
			// Ability
			DialogsDesk* ability=Ability.Get();
			if(ability){
				int a=0;
				if(BR->Ability){
					int N=BR->Ability->GetAmount();
					int aN=ability->DSS.GetAmount();
					for(int q=0;q<N&&a<aN;q++){
						BrigadeIconInfo* BI=(*BR->Ability)[q]->GetAbilityIcon();
						if(BI){
							GPPicture* aGP=(GPPicture*)ability->DSS[a];
							if(aGP){
								aGP->Visible=true;
								aGP->SetFileID(&BI->FileID);
								aGP->SetSpriteID(BI->SpriteID);
								if(BI->HintID.pchar())DYNHINT(aGP,BI->HintID.pchar());
								a++;
							}
						}
					}
				}
				for(;a<ability->DSS.GetAmount();a++){
					if(ability->DSS[a]){
						ability->DSS[a]->Visible=false;
					}
				}
			}
			// Morale
			DialogsDesk* morale=Morale.Get();
			if(morale){
				morale->Visible=Adv;
				float avkills;
				int exp=BR->GetSkillStatus(avkills);
				for(int i=0;i<morale->DSS.GetAmount();i++){
					//ProgressBar* aGP=ability->DSS[a];
					//aPB->BackColor=0x00000000;
					//ProgressGP_File;
					//ProgressSprite;
					int m=BR->Morale/10000/NLiveMax;
					GPPicture* mGP=(GPPicture*)morale->DSS[i];
					if(mGP){
						mGP->Visible=m>=i;
					}
				}
			}
			// Experience
			DialogsDesk* experience=Experience.Get();
			if(experience){
				experience->Visible=Adv;
				float avkills;
				int exp=BR->GetSkillStatus(avkills);
				for(int i=0;i<experience->DSS.GetAmount();i++){
					//ProgressBar* aGP=ability->DSS[a];
					//aPB->BackColor=0x00000000;
					//ProgressGP_File;
					//ProgressSprite;
					GPPicture* eGP=(GPPicture*)experience->DSS[i];
					if(eGP){
						eGP->Visible=exp>=i;
					}
				}
			}
			// Branch
			GPPicture* branch=Branch.Get();
			if(branch){
				branch->Visible=Adv;
				branch->SetFileID(&MainGP);
				switch(NATIONS->Mon[BR->MembID]->newMons->PortBranch){
					case 0: branch->SetSpriteID(BranchSpr_0); break;
					case 1: branch->SetSpriteID(BranchSpr_1); break;
					case 2: branch->SetSpriteID(BranchSpr_2); break;
				}									
			}
		}
	}
}
bool cva_DrawBrig_LAbility::LeftClick(SimpleDialog* SD){
	//GPPicture* GP;
	byte NI=SD->UserParam>>8;
	byte ID=SD->UserParam&0xFF;
	if(GetKeyState(VK_SHIFT)&0x8000) CmdChooseUnSelBrig(NI,ID);
		else CmdChooseSelBrig(NI,ID);
	return true;
}
//
struct OneUCircle{
	int x,y;
	int R;
	DWORD Color;
};
class DrawCirclesOverUnits:public GameExtension{
public:
	DynArray<OneUCircle> Circles;
	void OnDrawOnMapAfterTransparentEffects(){
		for(int i=0;i<Circles.GetAmount();i++){
			void ShowPen(int x,int y1,int R,DWORD ptc);
			ShowPen(Circles[i].x,Circles[i].y,Circles[i].R,Circles[i].Color);			
		}
		Circles.FastClear();
	}
};
void SetBrigadeFlagbearers(Brigade* BR);
void ClearBrigadeFlagbearers(Brigade* BR);
class BrigadeFlagbearers:public GameExtension{
public:
	void OnBrigadeCreated(Brigade* BR){
		SetBrigadeFlagbearers(BR);
	}
	void OnBrigadeFreedManually(Brigade* BR){
		ClearBrigadeFlagbearers(BR);
	}
	void OnBrigadeKilled(Brigade* BR,byte KillerNation){
		ClearBrigadeFlagbearers(BR);
	}
};

DrawCirclesOverUnits* CIRC;
void AddGroundCircle(int x,int y,int R,DWORD Color){
	OneUCircle UC;
	UC.x=x;
	UC.y=y;
	UC.R=R;
	UC.Color=Color;
	CIRC->Circles.Add(UC);
}
void RegisterBrigAbl(){
	InstallExtension(new DrawBrigsAbility,"DrawBrigAbility");
	CIRC=new DrawCirclesOverUnits;
	InstallExtension(CIRC,"DrawCircles");
	InstallExtension(new BrigadeFlagbearers,"Flagbearers");
}