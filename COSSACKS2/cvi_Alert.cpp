#include "stdheader.h"
#include ".\cvi_alert.h"
//////////////////////////////////////////////////////////////////////////
//
void ACT(int x);
CEXPORT void AddPulseSquare(int x, int y);
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);
bool CheckScrollLimitations(int x, int y);
extern int TrueTime;
//
//////////////////////////////////////////////////////////////////////////
class cvi_AlertBase: public BaseClass{
public:
	cvi_Alert* Alert;
	virtual bool GetCenter(int& x, int& y){ return false; }
	virtual int DetectNewLevel(){ return 0; }
	virtual void ActEvents(){
		int px,py;
		if(GetCenter(px,py)){
			AddPulseSquare(px,py);
		}		
	}
};
class cvi_AlertCannon: public cvi_AlertBase{
public:
	OneObject* OB;
	virtual bool GetCenter(int& x, int& y){
		if(OB){
			x=OB->RealX>>4;
			y=OB->RealY>>4;
			return true;
		}
		return false;
	};
	virtual int DetectNewLevel(){
		return 1; 
	}
	virtual void ActEvents(){
		ACT(92); //ACT(42+Alert->Level);
		cvi_AlertBase::ActEvents();
	}
};
class cvi_AlertSettlement: public cvi_AlertBase{	
public:
	cvi_Alert dAlert;
	//DIP_SimpleBuilding* DSB;
	int dx;
	int dy;
	int r;
	virtual bool GetCenter(int& x, int& y){
		//if(DSB){
			x=dx;
			y=dy;
			return true;
		//}
		//return false;
	};
	virtual int DetectNewLevel(){
		return 1; 
	}
	virtual void ActEvents(){
		ACT(112);
		cvi_AlertBase::ActEvents();
	}
};
//
ClassArray<cvi_AlertBase> Comm;
ClassArray<cvi_AlertBase> Pret;
//
bool AddArrayAlert(cvi_AlertBase* AB){
	int x,y;
	cvi_Alert* Alert=AB->Alert;
	if(AB->GetCenter(x,y)){
		int H=GetHeight(x,y);//+80;
		Vector4D V=SkewPt(x,y,H);
		WorldToScreenSpace(V);
		//
		const int border=200;
		bool screen=V.x>-border&&V.y>-border&&V.x<RealLx+border&&V.y<RealLy+border;
		bool fow=CheckScrollLimitations(x,y);
		if(!fow){
			fow=!GetObjectVisibilityInFog(x,y,H,NULL);
		}
		// calculate new level
		int newLevel=!fow;
		if(newLevel>0) newLevel=AB->DetectNewLevel();
		// set new level
		int T=TrueTime;
		if(!Alert->Init){
			// init
			Alert->Init=true;
			if(screen&&!fow){
				Alert->Status=2;
			}else{
				Alert->Status=0;
			}
			Alert->Level=newLevel;
			Alert->LevelTime=T;
			Alert->CommanderTime=0;
			return false;
		}
		if(screen&&!fow){
			// players see this brigade
			Alert->Status=2;
			Alert->Level=newLevel;
			Alert->LevelTime=T;
			return false;
		}
		//
		if(Alert->Level!=newLevel){
			// level changed
			if(newLevel<Alert->Level){
				// loose level
				if(T>Alert->LevelTime+15000){
					if(Alert->Status==1){
						// commanders who loose level
						//Degrades.Add(BR);									
					}
					// do not change status if seen
					if(Alert->Status!=2){
						Alert->Status=0;
					}								
					Alert->Level=newLevel;
					Alert->LevelTime=T;
				}
			}else{
				// rise level							
				Alert->Status=0;							
				Alert->Level=newLevel;
				Alert->LevelTime=T;
				// add pretender
				Pret.Add(AB);
				return true;
			}
		}else{
			// level not changed
			Alert->LevelTime=T;
			if(Alert->Status==0){
				// netral may be commanders if near degredes
				if(Alert->Level){
					//Netrals.Add(BR);
				}							
			}else
			if(Alert->Status==1){
				// check commander
				bool add=true;
				for(int i=0;i<Comm.GetAmount();i++){
					int cx,cy;
					cvi_AlertBase* B2=Comm[i];
					if(B2->Alert->Status==1){
						Comm[i]->GetCenter(cx,cy);
						if( abs(cx-x)<RealLx && abs(cy-y)<RealLy*2 ){
							add=false;
							break;
						}
					}
				}
				if(add){
					Comm.Add(AB);
					return true;
				}else{
					// delete comander
					Alert->Status=0;
				}
			}else
			if(Alert->Status==2){
				// this enemy allready was seen
				Comm.Add(AB);
				return true;
			}
		}
	}
	return false;
}
void CheckArrayAlert(){
	// find in pretenders new commanders
	for(int j=0;j<Pret.GetAmount();j++){
		int px,py;
		cvi_AlertBase* BR=Pret[j];
		BR->GetCenter(px,py);
		bool com=true;
		for(int i=0;i<Comm.GetAmount();i++){
			int cx,cy;
			cvi_AlertBase* B2=Comm[i];
			if(BR->Alert->Level<=B2->Alert->Level){
				B2->GetCenter(cx,cy);
				if( abs(cx-px)<RealLx && abs(cy-py)<RealLy*2 ){
					com=false;
					break;
				}
			}
		}
		if(com){			
			//
			if(BR->Alert->Status==0){
				// add new comannder
				BR->Alert->Status=1;			
				BR->Alert->CommanderTime=TrueTime;
				//
				BR->ActEvents();
			}
			//
			Comm.Add(BR);
			Pret.Del(j,1);
			j--;
		}else{
			BR->Alert->Status=0;
		}
	}
	//Pret.reset_class(&Pret);
	Pret.Clear();
}
//
void AddArrayAlertCann(){	
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	byte NMask=NATIONS[NI].NMask;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(NMask&OB->NMask)&&OB->newMons->PortBranch==2){
			cvi_AlertCannon* AB=new cvi_AlertCannon;
			AB->OB=OB;
			AB->Alert=&OB->Alert;
			//
			if(!AddArrayAlert(AB)){
				delete AB;
			};
		}
	}	
}
//
CIMPORT word GetNTribes();
CIMPORT byte GetTribeOwner(word Index);
CIMPORT bool GetTribeCenter(word Index, int &x, int &y);
CIMPORT int GetTribeBigZoneRadius(word Index);
//
ClassArray<cvi_AlertSettlement> aSettl;
void AddArrayAlertSettl(){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	byte NMask=NATIONS[NI].NMask;
	for(int i=0;i<aSettl.GetAmount();i++){
		cvi_AlertSettlement* AS=aSettl[i];
		if(AS){
			byte sNI=GetTribeOwner(i);
			byte sNMask=NATIONS[sNI].NMask;
			if(!(NMask&sNMask)){
				cvi_AlertSettlement* nAS=new cvi_AlertSettlement;
				nAS->Alert=AS->Alert;
				nAS->dx=AS->dx;
				nAS->dy=AS->dy;
				nAS->r=AS->r;
				if(!AddArrayAlert(nAS)){
					delete nAS;
				};
			}
		}
	}
}
bool CheckLastDeffenderEvent(Brigade* BR){
	if(BR){
		int bx,by;
		if(BR->GetCenter(&bx,&by)){	
			// find tribe
			int sDist=1000000;
			byte tNI=0xFF;
			int tx,ty,r;
			for(int i=0;i<aSettl.GetAmount();i++){
				cvi_AlertSettlement* AS=aSettl[i];
				if(AS){
					int d=Norma(bx-AS->dx,by-AS->dy);
					if(sDist>d){
						sDist=d;
						tNI=GetTribeOwner(i);
						tx=AS->dx;
						ty=AS->dy;
						r=AS->r;
					}				
				}
			}
			byte NI=BR->CT->NI;
			// it is deffender
			if(NI==tNI){
				int nDef=0;
				int nb=CITY[NI].NBrigs;
				for(int j=0;j<nb;j++){
					Brigade* B2=CITY[NI].Brigs+j;
					if(BR!=B2&&B2->Enabled&&B2->WarType){
						int xc,yc;
						if(B2->GetCenter(&xc,&yc)){
							int d=Norma(tx-xc,ty-yc);
							if(d<r){
								nDef++;
							}
						}
					}
				}
				if(nDef==1){
					return true;
				}
			}
		}
	}
	return false;
}
//
bool aInit=false;
void AlertOnUnloading(){
	aSettl.reset_class(&aSettl);
	aInit=false;
}
void AlertInit(){
	if(!aInit){
		aInit=true;
		int n=GetNTribes();
		for(int i=0;i<n;i++){
			cvi_AlertSettlement* AS=new cvi_AlertSettlement;
			if(GetTribeCenter(i,AS->dx,AS->dy)){
				AS->Alert=&AS->dAlert;
				AS->r=GetTribeBigZoneRadius(i);
				aSettl.Add(AS);
			}else{
				delete AS;
				aSettl.Add(NULL);
			}
		}
	}
}
//
void AlertProcess(){
	//return;
	AlertInit();
	//Comm.reset_class(&Comm);
	//Pret.reset_class(&Pret);
	Comm.Clear();
	Pret.Clear();
	AddArrayAlertSettl();
	CheckArrayAlert();
	AddArrayAlertCann();
	CheckArrayAlert();
}

//////////////////////////////////////////////////////////////////////////
int DetectNewLevel(int x, int y){
	int dist=1000000;
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	int nb=CITY[NI].NBrigs;
	for(int j=0;j<nb;j++){
		Brigade* BR=CITY[NI].Brigs+j;
		if(BR->Enabled&&BR->WarType&&BR->NMemb>40){
			int xc,yc;
			if(BR->GetCenter(&xc,&yc)){
				int d=Norma(x-xc,y-yc);
				if(dist>d){
					dist=d;
				}
			}
		}
	}
	// settlement
	int sDist=1000000;
	//byte NMask=NATIONS[NI].NMask;
	for(int i=0;i<aSettl.GetAmount();i++){
		cvi_AlertSettlement* AS=aSettl[i];
		if(AS){
			byte sNI=GetTribeOwner(i);
			//byte sNMask=NATIONS[sNI].NMask;
			if(NI==sNI){
				int d=Norma(x-AS->dx,y-AS->dy);
				if(sDist>d){
					sDist=d;
				}				
			}
		}
	}
	//
	if(sDist<900&&dist>500){
		return 3;
	}
	if(dist>1000){
		return 1;
	}else
	if(dist>600){
		return 2;
	}else{
		return 4;
	}	
}
void AlertBrigProcess(){
	static int T=0;
	extern int TrueTime;
	if(TrueTime<10000){
		T=TrueTime;
	}
	if(T<TrueTime){
		T=TrueTime+777;		
	}else{
		return;
	}
	//
	AlertProcess();
	//
	DynArray<Brigade*> Commanders;
	DynArray<Brigade*> Pretenders;
	DynArray<Brigade*> Degrades;
	DynArray<Brigade*> Netrals;
	//
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	byte NMask=NATIONS[NI].NMask;
	for(int i=0;i<7;i++)if(!(NMask&NATIONS[i].NMask)){
		int nb=CITY[i].NBrigs;
		for(int j=0;j<nb;j++){
			Brigade* BR=CITY[i].Brigs+j;
			if(BR->Enabled&&BR->WarType&&BR->NMemb>40){
				int x,y;
				if(BR->GetCenter(&x,&y)){
					int H=GetHeight(x,y);//+80;
					Vector4D V=SkewPt(x,y,H);
					WorldToScreenSpace(V);
					//
					const int border=200;
					bool screen=V.x>-border&&V.y>-border&&V.x<RealLx+border&&V.y<RealLy+border;
					bool fow=CheckScrollLimitations(x,y);
					if(!fow){
						fow=!GetObjectVisibilityInFog(x,y,H,NULL);
					}
					// calculate new level
					int newLevel=!fow;
					if(newLevel>0) newLevel=DetectNewLevel(x,y);
					// set new level
					int T=GetTickCount();
					if(!BR->Alert.Init){
						// init
						BR->Alert.Init=true;
						if(screen&&!fow){
							BR->Alert.Status=2;
						}else{
							BR->Alert.Status=0;
						}
						BR->Alert.Level=newLevel;
						BR->Alert.LevelTime=T;
						BR->Alert.CommanderTime=0;
						continue;
					}
					if(screen&&!fow){
						// players see this brigade
						BR->Alert.Status=2;
						BR->Alert.Level=newLevel;
						BR->Alert.LevelTime=T;
						continue;
					}
					//
					if(BR->Alert.Level!=newLevel){
						// level changed
						if(newLevel<BR->Alert.Level){
							// loose level
							if(T>BR->Alert.LevelTime+15000){
								if(BR->Alert.Status==1){
									// commanders who loose level
									Degrades.Add(BR);									
								}
								// do not change status if seen
								if(BR->Alert.Status!=2){
									BR->Alert.Status=0;
								}								
								BR->Alert.Level=newLevel;
								BR->Alert.LevelTime=T;
							}
						}else{
							// rise level							
							BR->Alert.Status=0;							
							BR->Alert.Level=newLevel;
							BR->Alert.LevelTime=T;
							// add pretender
							Pretenders.Add(BR);
						}
					}else{
						// level not changed
						BR->Alert.LevelTime=T;
						if(BR->Alert.Status==0){
							// netral may be commanders if near degredes
							if(BR->Alert.Level){
								Netrals.Add(BR);
							}							
						}else
						if(BR->Alert.Status==1){
							// check commander
							bool add=true;
							for(int i=0;i<Commanders.GetAmount();i++){
								int cx,cy;
								Brigade* B2=Commanders[i];
								if(B2->Alert.Status==1){
									Commanders[i]->GetCenter(&cx,&cy);
									if( abs(cx-x)<RealLx && abs(cy-y)<RealLy*2 ){
										add=false;
										break;
									}
								}
							}
							if(add){
								Commanders.Add(BR);
							}else{
								// delete comander
								BR->Alert.Status=0;
							}
						}else
						if(BR->Alert.Status==2){
							// this enemy allready was seen
							Commanders.Add(BR);
						}
					}
				}
			}
		}
	}
	// add to pretenders netrals who close to degredes
	for(int i=0;i<Netrals.GetAmount();i++){
		int nx,ny;
		Brigade* BN=Netrals[i];
		for(int j=0;j<Degrades.GetAmount();j++){		
			int dx,dy;
			Brigade* BD=Degrades[j];
			BD->GetCenter(&dx,&dy);		
			if(BN->Alert.Level>BD->Alert.Level){
				BN->GetCenter(&nx,&ny);
				if( abs(dx-nx)<RealLx/2 && abs(dy-ny)<RealLy ){
					BN->Alert.Status=1;
					BN->Alert.CommanderTime=BD->Alert.CommanderTime;
					Pretenders.Add(BN);
					break;
				}
			}
		}
	}
	// find in pretenders new commanders
	for(int j=0;j<Pretenders.GetAmount();j++){
		int px,py;
		Brigade* BR=Pretenders[j];
		BR->GetCenter(&px,&py);
		bool com=true;
		for(int i=0;i<Commanders.GetAmount();i++){
			int cx,cy;
			Brigade* B2=Commanders[i];
			if(BR->Alert.Level<=B2->Alert.Level){
				B2->GetCenter(&cx,&cy);
				if( abs(cx-px)<RealLx && abs(cy-py)<RealLy*2 ){
					com=false;
					break;
				}
			}
		}
		if(com){			
			//
			if(BR->Alert.Status==0){
				// add new comannder
				BR->Alert.Status=1;			
				BR->Alert.CommanderTime=GetTickCount();
				//
				if(BR->Alert.Level<3){
					ACT(42+BR->Alert.Level);
				}else
				if(BR->Alert.Level==3){
					ACT(111);
				}else{
					ACT(45);
				}				
				AddPulseSquare(px,py);
			}
			//
			Commanders.Add(BR);
		}else{
			BR->Alert.Status=0;
		}
	}
}
Brigade* GetLastBrigAlert(){
	int T=0;
	Brigade* BR=NULL;
	//
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	byte NMask=NATIONS[NI].NMask;
	for(int i=0;i<8;i++)if(!(NMask&NATIONS[i].NMask)){
		int nb=CITY[i].NBrigs;
		for(int j=0;j<nb;j++){
			Brigade* B2=CITY[i].Brigs+j;
			if(B2->Enabled&&B2->WarType){
				if(T<B2->Alert.CommanderTime&&B2->Alert.Status==1){
					T=B2->Alert.CommanderTime;
					BR=B2;
				}
			}
		}
	}
	return BR;
}
//////////////////////////////////////////////////////////////////////////