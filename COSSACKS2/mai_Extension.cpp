#include "stdheader.h"
#include ".\mai_extension.h"

void mai_mInit(){
	InstallExtension(new mai_Extension, "Micro AI");
	REG_CLASS(mai_VanGuard);
	REG_CLASS(mai_Script);
	REG_CLASS(mai_VanTroop);
	REG_CLASS(mai_Monitor);	
}

mai_Extension::mai_Extension(void)
{
}

mai_Extension::~mai_Extension(void)
{
}

//
void mai_Extension::ProcessingGame(){
};
void mai_Extension::OnBrigadeCreated(Brigade* BR){
};
void mai_Extension::OnBrigadeFreedManually(Brigade* BR){
};
void mai_Extension::OnBrigadeKilled(Brigade* BR,byte KillerNation){
};

//

void mai_Extension::OnDrawOnMapAfterTransparentEffects(){
	vaiMonitor.DrawOnMap();
	/*
	if(!tBrigLAbl){
		tBrigLAbl=new DialogsDesk;
		if(!tBrigLAbl->SafeReadFromFile("Dialogs\\v\\BRIG_BONUS_L.DIALOGSDESK.DIALOGS.XML")){
			Log.Warning("DrawBrigsAbility can not find file: Dialogs\\v\\BRIG_BONUS_L.DIALOGSDESK.DIALOGS.XML");
			//delete tBrigLAbl;
			//tBrigLAbl=NULL;
		};
	}
	if(GetKeyState(VK_CONTROL)&0x8000){
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
						if(V.x>-100&&V.y>-100&&V.x<RealLx+100&&V.y<RealLy+100){
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
						}
					}
				}
			}
		}			
		BrigAbl.ProcessDialogs();
		//
		for(i=aL;i<BrigLAbl.DSS.GetAmount();i++){
			if(BrigLAbl.DSS[i]){
				BrigLAbl.DSS[i]->Visible=false;
			}
		}
		BrigLAbl.ProcessDialogs();			
	}
	*/
}
void mai_Extension::OnDrawOnMiniMap(int x,int y,int Lx,int Ly){
	vaiMonitor.DrawOnMiniMap(x,y,Lx,Ly);
	/*
	DialogsSystem* DS=&cext_VisualInterface::PulseSquare;
	int n=DS->DSS.GetAmount();
	for(int i=0;i<n;i++){
		GPPicture* GP=dynamic_cast<GPPicture*>(DS->DSS[i]);
		if(GP){ //!GP->Visible
			int px=GP->ID;
			int py=GP->UserParam;
			int w=GP->GetWidth();
			int h=GP->GetHeight();
			GP->Setx((px>>6)-w/2+x);
			GP->Sety((py>>6)-h/2+y);
		}
	}
	DS->ProcessDialogs();
	//
	DS=&cext_VisualInterface::VI_Zone;
	n=DS->DSS.GetAmount();
	for(i=0;i<n;i++){
		GPPicture* GP=dynamic_cast<GPPicture*>(DS->DSS[i]);
		if(GP){ //!GP->Visible
			int px=GP->ID;
			int py=GP->UserParam;
			int w=GP->GetWidth();
			int h=GP->GetHeight();
			GP->Setx((px>>6)-w/2+x);
			GP->Sety((py>>6)-h/2+y);
		}
	}
	DS->ProcessDialogs();
	*/
}
int vMx,vMy;
int vMapx,vMapy;
bool mai_Extension::OnMouseHandling(int mx,int my,bool& LeftPressed,bool& RightPressed,int MapCoordX,int MapCoordY,bool OverMiniMap){
	vMx=mx;
	vMy=my;
	vMapx=MapCoordX;
	vMapy=MapCoordY;
	return false;
};