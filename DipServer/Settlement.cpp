#include "CommonDip.h"
#include "UnitsGroup.h"
#include "Abilki.h"

extern DiplomacySystem DIPS;

int GetSelectedList(byte NI, word*& List);

CIMPORT int GetGlobalCreateBrigList(byte NI, byte MaxCen, word* CID, byte MaxOf, word* NIndex, word* Amount, byte MaxType);
CIMPORT int CreateBrigInZone(byte NI, int x, int y, word OfType, word SlType);
DLLIMPORT void OBJ_Produce(word Index,word NIndex,word GroupID);

int SettlementInterface(int x0,int y0,int Lx0,int Ly0,int x1,int y1,int Index){
	OneObject* OB=GetOBJ(Index);
	if(OB&&OB->NNUM==7){		
		int NDSB=DIPS.NDIPS;
		for(int i=0;i<NDSB;i++){
			DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[i];
			OneUnit OU;
			GetUnitInfo(&DSB->CentralGroup,0,&OU);
			if(Index==OU.Index){
				DSB->DrawSettlementInterface(x0,y0,Lx0,Ly0,x1,y1);
				DSB->CurSetlSelected=i;
				break;
			}			
		}
		return 3;
	}
	return 0;
}

void InitSettlementInterface(){
	SetUnitInterfaceCallback(SettlementInterface);
}

void SI_RClick(int param){
	int DSB_ID=param>>16;
	int Upg_id=param&0xFFFF;
	if(DSB_ID<0||DSB_ID>DIPS.NDIPS||Upg_id<0) return;
	word Data[3];
	Data[0]=3001;
	Data[1]=DSB_ID;
	Data[2]=Upg_id;
	SendDipCommand((char*)Data,sizeof Data);
}

CIMPORT void SelBrigadeExp(byte NI, byte Type, int ID);
CIMPORT void SetOrderedStateForComplexObject(OneObject* OB,int State);
CIMPORT int GetRefNation(byte NI);
CIMPORT bool GetAIEnable(byte NI);

void SI_PerformCommands(char* Data,int size){
	byte NI=GetMyPlayerColor();
	byte ID=GetMyPlayerID();
	word* wData=(word*)Data;
	word com=wData[0];
	word co1=wData[1];
	word co2=wData[2];	
	addrand(com);
	addrand(co1);
	addrand(co2);
	if(3000<com&&com<3072){
		if(com==3001){
			DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[co1];
			if(GetNUnits(&DSB->CentralGroup)){
				DSB->SetlUpg[co2]->Run();
				AddRandNew(__FILE__,__LINE__,(co1<<16)+co2);
			}
		}else		
		if(com==3002){
			DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[co1];
			if(GetNUnits(&DSB->CentralGroup)){
				OneObject* OB=GetOBJ(wData[3]);
				if(OB&&OB->Serial==wData[4]){
					AddAbilityToBrigade(OB->NNUM,OB->BrigadeID,co2);
				}
			}
		}else		
		if(com==3003){ // Make brig from units near kazarma			
			OneObject* COB=GetOBJ(wData[1]);
			if(COB&&COB->Serial==wData[2]&&!COB->Sdoxlo){
				addrand(COB->NNUM);
				addrand(COB->RealX>>4);
				addrand(COB->RealY>>4);				
				word type=wData[3];		// NIndex of main unit in brigade
				word OfType=wData[4];	// type of officer
				addrand(OfType);
				addrand(type);
				int BID=CreateBrigInZone(COB->NNUM,COB->RealX>>4,COB->RealY>>4,OfType,type);				
				if(BID>=0){
					addrand(BID);					
					byte SelNI=wData[5];
					byte NI=GetRefNation(SelNI);
					if(NI==COB->NNUM){ //!GetAIEnable(NI)
						addrand(SelNI);
						SelBrigadeExp(SelNI,0,BID);
					}
				}
			}
		}else		
		if(com==3004){
			OneObject* COB=GetOBJ(co1);
			if(COB&&COB->Serial==wData[2]&&!COB->Sdoxlo){
				word type=wData[3];		// NIndex of unit for produce
				OBJ_Produce(co1,type,0xFFFF);				
			}			
		}else				
		if(com==3005){ // add workers to settlement
			DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[co1];
			if(GetNUnits(&DSB->CentralGroup)){
				OneObject* OB=GetOBJ(wData[2]);
				if(OB&&OB->Serial==wData[3]&&!OB->Sdoxlo){
					//DSB->setlAddWorker(wData[4],OB);					
				}
			}
		}else
		if(com==3006){ // SetOrderedStateForComplexObject
			OneObject* OB=GetOBJ(wData[1]);
			if(OB&&OB->Serial==wData[2]){
				SetOrderedStateForComplexObject(OB,wData[3]);
			}
		}
	}
}

CIMPORT bool GetUnitVisibility(word Index);
CIMPORT bool GetSpotOnMapVisibility(int x,int y);

word setlSelID=0xFFFF;

CEXPORT bool setlIsReinforce(word ObjID, bool Add){
	if(setlSelID!=0xFFFF){
		OneObject* OB=GetOBJ(ObjID);
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[setlSelID];
		for(int i=0;i<6;i++){
			if(DSB->setlGetWorkersFreePlacesIntrf(i)){
				if(Add){
					DSB->setlAddWorkerIntrf(i,OB);
				}
				return true;
			}
		}
	}
	return false;
}

word TopSelBrigID=0xFFFF;
CEXPORT void SetTopSelBrigID(word BID){
	//TopSelBrigID=BID;
}
CEXPORT word GetTopSelBrigID(){
	return TopSelBrigID;
}
CIMPORT void DrawBrigSign(byte NI, word BrID, int x, int y, int GPFile);
int Norma(int,int);
//
struct PulseBox{
	int x,y,r;
	int StartTime,Length;
	bool Draw(int dx, int dy);
};
CIMPORT int GetXOnMiniMap(int x,int y);
CIMPORT int GetYOnMiniMap(int x,int y);
bool PulseBox::Draw(int dx, int dy){
	//dx+=(x>>6)-11;
	//dy+=(y>>6)-11;
	static int file=GPS.PreLoadGPImage("interf3\\minimap");
	static int spr=28;
	//
	dx=GetXOnMiniMap(x,y)-GPS.GetGPWidth(file,31)/2;
	dy=GetYOnMiniMap(x,y)-GPS.GetGPHeight(file,31)/2;
	//
	int time=GetTickCount()-StartTime;
	if(time<Length){
		if(time<1500&&(time<800||time>1000)){
			int s=spr;
			if(time>200) s++;
			if(time>400) s++;
			if(time>600) s++;
			if(time>1000) s=spr;
			if(time>1200) s=spr+3;			
			DrawPicture(dx,dy,file,s,0);
		}
		return true;
	}
	return false;
};
DynArray<PulseBox> PulseBoxes;
CEXPORT void AddPulseBox(int x, int y){				// graphical coordinates
	for(int i=PulseBoxes.GetAmount()-1;i>=0;i--){
		PulseBox* p=PulseBoxes+i;
		if(Norma(p->x-x,p->y-y)<128) return;
	}
	PulseBox p;
	p.x=x;
	p.y=y;
	p.StartTime=GetTickCount();
	p.Length=2000;
	PulseBoxes.Add(p);
}
/*
struct PulseSquare{
	int x,y;
	float StartScale,FinalScale;
	float RotSpeed;
	int StartTime,Length;
	bool Draw(int dx, int dy);
};
bool PulseSquare::Draw(int dx, int dy){
	dx+=(x>>6)-11;
	dy+=(y>>6)-11;
	static int file=GPS.PreLoadGPImage("interf3\\minimap");
	static int spr=33;
	int time=GetTickCount()-StartTime;
	if(time<Length){
		int dt=Length/10;

		float Scale=StartScale+(FinalScale-StartScale)*time/Length;
		GPS.SetScale(Scale);
		DWORD ld=GPS.GetCurrentDiffuse();
		GPS.SetCurrentDiffuse(0x88FF0000);
		GPS.ShowGP(dx,dy,file,spr,0);
		GPS.SetCurrentDiffuse(ld);
		GPS.SetScale(1.0f);

		return true;
	}
	return false;
};
/*
DynArray<PulseSquare> PulseSquares;
CEXPORT void AddPulseSquare(int x, int y){				// graphical coordinates
	for(int i=PulseBoxes.GetAmount()-1;i>=0;i--){
		PulseSquare* p=PulseBoxes+i;
		if(Norma(p->x-x,p->y-y)<128) return;
	}
	PulseSquare p;
	p.x=x;
	p.y=y;
	p.StartTime=GetTickCount();
	p.Length=2000;
	PulseSquares.Add(p);
}
*/
//
word SelObozID=0xFFFF;
word SelObozSN;
CEXPORT void dipf_SetSelOboz(OneObject* OB, byte& ResID, int& ResAmount, byte& Owner){
	if(OB&&!OB->Sdoxlo&&OB->Stuff<DIPS.NDIPS){
		SelObozID=OB->Index;
		SelObozSN=OB->Serial;
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[OB->Stuff];
		if(DSB){
			DipCaravan* DC=DSB->Caravan;
			for(int i=0;i<MaxCaravan;i++,DC++){							
				if(OB==GetOBJ(DC->ID)){																
					ResID=DC->ResID;
					ResAmount=DC->ResAmount;
					Owner=DC->Owner;
					break;
				}
			}
		}
	}else{
		SelObozID=0xFFFF;
	}	
}
//
CIMPORT int GetXOnMiniMapEx(int x,int y);
CIMPORT int GetYOnMiniMapEx(int x,int y);
CIMPORT DWORD GetNatColor( int natIdx, int Multiplicator );
//
int SI_DrawMiniMap(int x,int y,int Lx,int Ly,int mx0,int my0){
	if(DIP_SimpleBuilding::fDrawOnMiniMap) DIP_SimpleBuilding::fDrawOnMiniMap(x,y,Lx,Ly,mx0,my0);
	RunPF((5<<8)+2,"SI_DrawMiniMap");
	//return 0;
	setlSelID=0xFFFF;
	byte MN=GetMyRefNation();
	byte Nat=GetMyNation();
	int NDSB=DIPS.NDIPS;
	// is selected oboz	
	OneObject* OBz=NULL;
	DIP_SimpleBuilding* DSBz=NULL;	
	if(SelObozID!=0xFFFF){
		OneObject* OBz=GetOBJ(SelObozID);
		SelObozID=0xFFFF;
		if(OBz&&OBz->Serial==SelObozSN&&!OBz->Sdoxlo&&(OBz->ImSelected&(1<<Nat))&&OBz->Stuff<NDSB){			
			DSBz=(DIP_SimpleBuilding*)DIPS.DIPS[OBz->Stuff];
			DipCaravan* DC=DSBz->Caravan;
			for(int i=0;i<MaxCaravan;i++,DC++){							
				if(DC->ResAmount>0&&OBz==GetOBJ(DC->ID)){																
					//int sx=x+(OBz->RealX>>10)-1;
					//int sy=y+(OBz->RealY>>10)-1;
					int sx=GetXOnMiniMapEx((OBz->RealX)>>4,(OBz->RealY)>>4)-GPS.GetGPWidth(DSBz->FlagFileID,0)/2;
					int sy=GetYOnMiniMapEx((OBz->RealX)>>4,(OBz->RealY)>>4)-GPS.GetGPHeight(DSBz->FlagFileID,0)/2;
					if(DC->Owner==MN) SetCurrentDiffuse(0xFFFFFFFF);
						else SetCurrentDiffuse(GetNatColor(DC->Owner));
					DrawPicture(sx,sy,DSBz->FlagFileID,0,0);
					break;
				}
			}
		}
	}
	// settlement picture
	for(int i=0;i<NDSB;i++){
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[i];
		if(DSB->Owner<8){
			UnitsGroup* UG=GetUnitsGroup(&DSB->CentralGroup);
			if(UG&&UG->N){
				OneObject* OB=GetOBJ(UG->IDS[0]);
				if(OB&&!OB->Sdoxlo&&OB->Serial==UG->SNS[0]){
					int MainSpr=22;
                    int dx=GPS.GetGPWidth(DSB->FlagFileID,26)/2;
					int dy=GPS.GetGPHeight(DSB->FlagFileID,26)/2;
					//int xx=x+(OB->RealX>>10)-6;
					//int yy=y+(OB->RealY>>10)-10;
					int xx=GetXOnMiniMapEx((OB->RealX)>>4,(OB->RealY)>>4)-dx;
					int yy=GetYOnMiniMapEx((OB->RealX)>>4,(OB->RealY)>>4)-dy;
					byte nat=7;
					if(MN==DSB->Owner||GetUnitVisibility(OB->Index)) nat=DSB->Owner;
					DWORD D=GetNatColor(nat,300);
					SetCurrentDiffuse(D);
					//SetDrawScale(0.5);
					//bool draw=true;					
					for(int j=0;j<6;j++){
						if(DSB->Produce[j]>0){
							switch(j){
								case 1: MainSpr=46; break; //gold
								case 3: MainSpr=45; break; //food
								case 4: MainSpr=47; break; //iron
								case 5: MainSpr=44; break; //coal
							};
							break;
						}
					}
					bool sel=false;
					// is selected center of settlement
					if((OB->ImSelected&(1<<Nat))||DSB==DSBz){
						sel=true;
						//MainSpr=23;
						setlSelID=DSB->Index;
						SetCurrentDiffuse(0xFFFFFFFF);
						if(MN==DSB->Owner){
							int i;
							// caravany
							if(OB->ImSelected&(1<<Nat)){
								for(i=0;i<MaxCaravan;i++){
									if(DSB->Caravan[i].ResAmount>0){
										OneObject* OB=GetOBJ(DSB->Caravan[i].ID);
										if(OB&&OB->Serial==DSB->Caravan[i].SN&&!OB->Sdoxlo){
											//int sx=x+(OB->RealX>>10)-1;
											//int sy=y+(OB->RealY>>10)-1;
											int sx=GetXOnMiniMapEx((OB->RealX)>>4,(OB->RealY)>>4)-GPS.GetGPWidth(DSB->FlagFileID,0)/2;
											int sy=GetYOnMiniMapEx((OB->RealX)>>4,(OB->RealY)>>4)-GPS.GetGPHeight(DSB->FlagFileID,0)/2;
											DrawPicture(sx,sy,DSB->FlagFileID,0,0);
										}
									}
								}
							}
							// 
							for(i=0;i<6;i++){
								OneObject* SOB=DSB->GetStorage(i);
								if(SOB){
									//int sx=x+(SOB->RealX>>10)-6;
									//int sy=y+(SOB->RealY>>10)-10;
									int spr=20;
									int sx=GetXOnMiniMapEx((SOB->RealX)>>4,(SOB->RealY)>>4)-GPS.GetGPWidth(DSB->FlagFileID,spr+1)/2;
									int sy=GetYOnMiniMapEx((SOB->RealX)>>4,(SOB->RealY)>>4)-GPS.GetGPHeight(DSB->FlagFileID,spr+1)/2;
									DrawPicture(sx,sy,DSB->FlagFileID,spr,0);
								}
							}
						}
					}else{
						//SetCurrentDiffuse(D);//&&(GetGlobalTime()&0xF)>7) draw=false;					
					}
					//if(draw) 
					SetCurrentDiffuse(D);
					DrawPicture(xx,yy,DSB->FlagFileID,MainSpr,0);
					SetCurrentDiffuse(0xFFDFDFDF);
					//SetCurrentDiffuse(0xFFFFFFFF);
					DrawPicture(xx,yy,DSB->FlagFileID,MainSpr+5,0);					
					if(sel){
						SetCurrentDiffuse(0x9FFFFFFF);
						DrawPicture(xx,yy,DSB->FlagFileID,26-0,0);
					}
				}
			}
		}
	}
	SetCurrentDiffuse(0xFFFFFFFF);
	// TopSelBrigID
	if(TopSelBrigID!=0xFFFF){
		DrawBrigSign(MN,TopSelBrigID,x,y,DIP_SimpleBuilding::FlagFileID);
	}	
	for(int i=PulseBoxes.GetAmount()-1;i>=0;i--){
		PulseBox* p=PulseBoxes+i;
		if(!p->Draw(x,y)){
			PulseBoxes.Del(i,1);
		}
	}
	StopPF((5<<8)+2);
	return 0;
}

//
CIMPORT void DrawColoredCircle(int xc,int yc,int R0,int R1,DWORD LineColor1,DWORD LineColor2,DWORD FillColor1,DWORD FillColor2);
//
int SI_DrawOnMap(){	
	if(DIP_SimpleBuilding::fDrawOnMap) DIP_SimpleBuilding::fDrawOnMap();
	//
	
	DIP_SimpleBuilding** DSB=(DIP_SimpleBuilding **)DIPS.DIPS;
	int NDSB=DIPS.NDIPS;
	/*
	for(int i=0;i<NDSB;i++){
		DIP_SimpleBuilding* CDSB=DSB[i];//*cursel];

		UnitsGroup* UG=GetUnitsGroup(&CDSB->CentralGroup);
		OneObject* OB=GetOBJ(UG->IDS[0]);
		
		if(OB&&(OB->Selected||(GetKeyState(VK_MENU)&0x8000))){
			int r0,r1;
			GetZoneR(&CDSB->BigZone,&r0);
			GetZoneR(&CDSB->VeryBigZone,&r1);
			DrawColoredCircle(OB->RealX>>4,OB->RealY>>4,r0,0,0xFFFF2020,0xFFFFFFFF,0x2FFF2020,0x2FFFFFFF);
		}
	}
	*/
	//
	return 0;
	//
	RunPF((5<<8)+1,"SI_DrawOnMap");	
	// magazine manage: show arrow
	byte MN=GetMyNation();
	word* Slist=NULL;
	int NSl=GetSelectedList(MN,Slist);		
	word* cursel=&(DIP_SimpleBuilding::CurSetlSelected);
	//DIP_SimpleBuilding** DSB=(DIP_SimpleBuilding **)DIPS.DIPS;
	//int NDSB=DIPS.NDIPS;
	if(NSl==1){		
		word DID=Slist[0];
		if(*cursel!=0xFFFF){
			UnitsGroup* UG=GetUnitsGroup(&DSB[*cursel]->CentralGroup);
			if(!(UG&&UG->IDS[0]==DID)) *cursel=0xFFFF;
		}
		if(*cursel==0xFFFF){
			OneObject* OB=GetOBJ(DID);
			for(int i=0;i<NDSB;i++){
				UnitsGroup* UG=GetUnitsGroup(&DSB[i]->CentralGroup);
				if(UG&&UG->IDS[0]==DID&&UG->SNS[0]==OB->Serial){
					*cursel=i;
				}
			}
		}
	}else{
		*cursel=0xFFFF;
	}
	//if(*cursel!=0xFFFF)
	for(int i=0;i<NDSB;i++){
		DIP_SimpleBuilding* CDSB=DSB[i];//*cursel];

		UnitsGroup* UG=GetUnitsGroup(&CDSB->CentralGroup);
		OneObject* OB=GetOBJ(UG->IDS[0]);
		
		if(GetKeyState(VK_MENU)&0x8000){
			int r0,r1;
			GetZoneR(&CDSB->BigZone,&r0);
			GetZoneR(&CDSB->VeryBigZone,&r1);
			DrawColoredCircle(OB->RealX>>4,OB->RealY>>4,r0,r1,0xFFFF2020,0xFF20FF20,0x3FFF2020,0x3F20FF20);
		}

		float x,y,scale;
		TransformToScreen(OB->RealX>>4,OB->RealY>>4,OB->RZ,x,y,scale);
		int fi=0;
		if(*cursel==i){
			fi=((GetGlobalTime()>>1)%16)-8;
			if(fi>0) fi=-fi;
		}
		DrawPicture(int(x)-23,int(y)-180+fi,CDSB->ArrowFileID,0,MN);

		if(CDSB->ClientOfID!=0xFFFF){
			OB=GetOBJ(CDSB->ClientOfID);
			if(OB&&!OB->Sdoxlo&&OB->Serial==CDSB->ClientOfSN){
				TransformToScreen(OB->RealX>>4,OB->RealY>>4,OB->RZ,x,y,scale);				
				DrawPicture(int(x)-25,int(y)-94,CDSB->ArrowFileID,0,MN);
			}
		}
	}
	
	StopPF((5<<8)+1);
	return 0;
}

void SI_Abilka_RClick(int param){
	int DSB_ID=param>>16;
	int Upg_id=param&0xFFFF;
	DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[DSB_ID];
	if(DSB_ID<0||DSB_ID>DIPS.NDIPS||Upg_id<0||DSB->ClientOfID==0xFFFF) return;
	OneObject* OB=GetOBJ(DSB->ClientOfID);
	if(OB&&OB->Serial==DSB->ClientOfSN){
		word Data[5];
		Data[0]=3002;
		Data[1]=DSB_ID;
		Data[2]=Upg_id;
		Data[3]=DSB->ClientOfID;
		Data[4]=DSB->ClientOfSN;
		SendDipCommand((char*)Data,sizeof Data);
	}
}

