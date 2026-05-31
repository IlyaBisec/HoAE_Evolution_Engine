#include "CommonDip.h"
//#include "..\COSSACKS2\Fastdraw.h"
#include "..\COSSACKS2\Fonts.h"
#include "UnitsGroup.h"
#include "Abilki.h"
#include "..\cossacks2\smart_assert.h"
#include "..\Cossacks2\Polk.h"

extern DiplomacySystem DIPS;

CIMPORT void CopyBrigadeUnitsToGroup(byte BrigNI, word BrigID, UnitsGroup* UG);
CIMPORT void RemoveNotBrigadesUnit(UnitsGroup* Source, UnitsGroup* Dest);

CIMPORT int GetGameSpeed();
CIMPORT void ProcessDeffenders(GAMEOBJ* Grp, byte Mask, int xc, int yc, int R,int BigR);
bool FillFormationFromGroup(byte BrigNI, word BrigID, GAMEOBJ* Source, GAMEOBJ* Dest);
DLLIMPORT bool OBJ_GoToMineAlways(word Index,word DestMine,byte prio,byte OrdType);

void DIP_SimpleBuilding::InitThisTribe2(char* FileID, char* Dip, char* GDance, char* ZDance){
	InitThisTribe("",Dip,FileID);	
	RegisterUnits(&Actors,GDance);
	//UseLikeSkarb=(Actors.Type==0);	
	UseLikeSkarb=0;
	NStartActors=GetNUnits(&Actors);
	if(NStartActors){
		OneUnit OU;
		if(GetUnitInfo(&Actors,0,&OU)){;
			ActorsType=OU.NIndex;
		};
	};
	RegisterZone(&DanceZone,ZDance);
#ifdef AC_ADDON
	RegisterDynGroup(&Friends);
#endif AC_ADDON
}


word DIP_SimpleBuilding::CurSetlSelected=0xFFFF;

DLLIMPORT void RegisterZoneEx(GAMEOBJ* GOBJ,char* Name, bool ErrRaport);
//CIMPORT void VitalError(char*);
CIMPORT void SetStuff(GAMEOBJ* Grp, word stuff);

void DIP_SimpleBuilding::InitThisTribe(char* Preffix,char* FirstGroupName,char* CommonID){
	if(RegisterUnits(&CentralGroup,FirstGroupName)){
		RegisterDynGroup(&ProduceGroup);
		CopyUnits(&CentralGroup,&ProduceGroup,0,100000,true,false);
		//
		SetUp();
		//
		if(strlen(FirstGroupName)+1<sizeof(CentralName)){
			strcpy(CentralName,FirstGroupName);
		}else{
			char s[256];
			sprintf(s,"FirstGroupName '%s' too longin",FirstGroupName);
			VitalError(s);
		}

		char cc[256];
		char CCC[1024];

		// LoadingConfig
		sprintf(cc,"Dip\\%s",CommonID);
		strcpy(cc+strlen(cc)-2,".dat");
		GFILE* F=Gopen(cc,"r");
		if(!F){
			strcpy(cc+strlen(cc)-4,".dip");
			F=Gopen(cc,"r");
		}
		
		if(F){
			bool good=1;
			do{
				char str[1024];
				good=ReadWinString(F,str,1023);
				if(good){
					char com[128];
					com[0]=0;
					sscanf(str,"%s",com);

					if(Settlement){
						if(!strcmp(com,"$COP")){
							char CopType[64];
							int z=sscanf(str,"%s%d%s%d%d",com,&CopBuild,CopType,&NCops);
							if(z!=4){
								sprintf(CCC,"%s : Invalid string : %s",cc,str);	
								MissErrorMessage("Dip error!",CCC);
							}else{
								GAMEOBJ T;
								if(RegisterUnitType(&T,CopType)){
									CopNIndex=T.Index;
									
									if(!GetPeaceMode()){
										int n=NCops;//>>1;
										for(int j=0;j<n;j++){
											ProduceUnitTicks(&ProduceGroup,&T,&Defenders,20);
										}
									}

								}else{
									sprintf(CCC,"%s : Invalid cop type : %s",cc,str);	
									MissErrorMessage("Dip error!",CCC);
								}
							}
						}else
						if(!strcmp(com,"$PEASANT")){
							char Type[64];
							int z=sscanf(str,"%s%d%s%d%d%d%d%d%d",com,&PeasantBuild,Type,NPeasants,NPeasants+3,NPeasants+2,NPeasants+1,NPeasants+4,NPeasants+5);
							if(z!=9){
								sprintf(CCC,"%s : Invalid string : %s",cc,str);	
								MissErrorMessage("Dip error!",CCC);
							}else{
								GAMEOBJ T;
								if(RegisterUnitType(&T,Type)){
									PeasantNIndex=T.Index;
									for(int i=0;i<6;i++)if(NPeasants[i]>0){
										GAMEOBJ* grp=PeasGrp+i;
										RegisterDynGroup(grp);
										
										if(!GetPeaceMode()){
											int np=NPeasants[i]-2;//>>1;
											if(np>0){
												for(int j=0;j<np;j++){
													ProduceUnitTicks(&ProduceGroup,&T,grp,20);
												}
											}
										}

										//SGP_TakeResources(grp,0,7,i);
									}
								}else{
									sprintf(CCC,"%s : Invalid peasant type : %s",cc,str);	
									MissErrorMessage("Dip error!",CCC);
								}
							};
						}else
						if(!strcmp(com,"$PRODUCE")){
							int z=sscanf(str,"%s%d%d%d%d%d%d",com,Produce,Produce+3,Produce+2,Produce+1,Produce+4,Produce+5);
							if(z!=7){
								sprintf(CCC,"%s : Invalid string : %s",cc,str);	
								MissErrorMessage("Dip error!",CCC);
							};
						}else
						if(!strcmp(com,"$RESOURCEMAX")){
							int z=sscanf(str,"%s%d%d%d%d%d%d",com,ResourceMax,ResourceMax+3,ResourceMax+2,ResourceMax+1,ResourceMax+4,ResourceMax+5);
							if(z!=7){
								sprintf(CCC,"%s : Invalid string : %s",cc,str);	
								MissErrorMessage("Dip error!",CCC);
							};
						}else							
						if(!strcmp(com,"$CARAVAN")){
							char Type[64];
							int z=sscanf(str,"%s%s%d%d%d%d%d%d",com,Type,CaravanCapacity,CaravanCapacity+3,CaravanCapacity+2,CaravanCapacity+1,CaravanCapacity+4,CaravanCapacity+5);
							if(z!=8){
								sprintf(CCC,"%s : Invalid string : %s",cc,str);	
								MissErrorMessage("Dip error!",CCC);
							}else{
								GAMEOBJ T;
								if(RegisterUnitType(&T,Type)){
									CaravanNIndex=T.Index;
								}else{
									sprintf(CCC,"%s : Invalid caravan type : %s",cc,str);	
									MissErrorMessage("Dip error!",CCC);
								}
							}
						}else
						if(!strcmp(com,"$SETLUPG")){
							DString dstr(str);							
							SetlUpgrade* SU=new SetlUpgrade;
							SU->Init(SetlUpg.GetAmount(),Index,&dstr);
							SetlUpg.Add(SU);
						}else
						if(!strcmp(com,"$ABILKI")){
							int Slot;		
							int AM,AS;
							int Res;
							char Name[256];
							char HintName[256];
							int z=sscanf(str,"%s%d%s%d%d%d%s",com,&Slot,Name,&AM,&AS,&Res,HintName);
							if(z!=7&&Slot>8){
								sprintf(CCC,"%s : Invalid string : %s",cc,str);	
								MissErrorMessage("Dip error!",CCC);
							}else{
								SetlOneBrigAbilka* BA=BrigAbilka+Slot;
								BA->Slot=(byte)Slot;
								BA->ID=GetAbilityByName(Name);
								BA->AmountMax=(byte)AM;
								BA->AmountStart=(byte)AS;
								BA->Restore=Res;
								BA->Hint=GetTextByID(HintName);
								if(BA->Hint==HintName) BA->Hint=NULL;
							}
						}//else						
					};//else

					if(!strcmp(com,"$SETTLEMENT")){
						Settlement=1;
						char z[256];
						sprintf(z,"Z0-%s",FirstGroupName);
						RegisterZoneEx(&BigZone,z,false);
						if(BigZone.Type!='ZONE') UnitsCenter(&BigZone,&CentralGroup,900);
						sprintf(z,"Z1-%s",FirstGroupName);
						RegisterZoneEx(&VeryBigZone,z,false);						
						if(VeryBigZone.Type!='ZONE') UnitsCenter(&VeryBigZone,&CentralGroup,1300);
						SetlUpg.Clear();
						//for(i=0;i<ReinfBrig;i++) SReinfBrig[i].BrigID=0xFFFF;
						
						// Owner init
						int N=GetAmountOfZones();
						for(int i=0;i<N;i++){
							char* ZN=GetZoneName(i);
							char com[64]="";
							sscanf(ZN,"%s",com);
							if(!strcmp(com,"$SETOWNER")){
								int own=0xFF;
								int np=sscanf(ZN,"%s%d",com,&own);
								if(np==2){
									if(own!=0xFF&&own<7){
										GAMEOBJ Z;
										RegisterZone(&Z,ZN);
										if(GetUnitsAmount1(&Z,&CentralGroup)){
											Owner=own;
										}
									}
								}
							}
						}
						SetlOneBrigAbilka* BA=BrigAbilka;
						for(int i=0;i<9;i++,BA++) BA->Init(Index);
					}else
					if(!strcmp(com,"$PSB")){
						DipComParams DPP;
						int z=sscanf(str,"%s%s%s%s%d%d%d%d%d%d%s",com,DPP.MessageID,DPP.HintID,DPP.ProcessID,DPP.Cost,DPP.Cost+1,DPP.Cost+2,DPP.Cost+3,DPP.Cost+4,DPP.Cost+5,DPP.ComID);
						if(z==11){
							DIPPAR=(DipComParams*)realloc(DIPPAR,(NDipPar+1)*sizeof DipComParams);
							char* s=strstr(str,DPP.ComID);
							if(s)strcpy(DPP.Params,s+strlen(DPP.ComID)+1);
							memcpy(DIPPAR+NDipPar,&DPP,sizeof DipComParams);
							NDipPar++;							
							if(!Settlement){
								BigZone.Type=0;
								UnitsCenter(&BigZone,&CentralGroup,800);
								UnitsCenter(&VeryBigZone,&CentralGroup,750);
							}
						}else{
							sprintf(CCC,"%s : Invalid string : %s",cc,str);	
							MissErrorMessage("Dip error!",CCC);
						};
					}else
					if(!strcmp(com,"$IFTELLNO$")){
						sscanf(str,"%s%s",com,TellNo);
					}else
					if(!strcmp(com,"$CONGRAT")){
						sscanf(str,"%s%s",com,CongratID);
					}else
					if(!strcmp(com,"$FIRSTMEET")){
						sscanf(str,"%s%s",com,FirstMeetID);
					}else
					if(!strcmp(com,"$RESELL")){
						sscanf(str,"%s%s",com,ReSellID);
					}else
					if(!strcmp(com,"$STARTPRICE")){
						sscanf(str,"%s%d%d%d%d%d%d",com,ReSellCost,ReSellCost+1,ReSellCost+2,ReSellCost+3,ReSellCost+4,ReSellCost+5);
					}else
					if(!strcmp(com,"$NAME")){
						char NID[32];
						char CCC[128];
						sscanf(str,"%s%s",com,NID);
						int idx=0;
						bool doit=1;
						do{
							sprintf(CCC,NID,idx);
							char* id=GetTextByID(CCC);
							if(id[0]!='$')idx++;
							else doit=0;
						}while(doit);
						if(idx){
							sprintf(NameOfTribe,NID,CurIndex%idx);
							CurIndex++;
						}else{
							sprintf(NameOfTribe,NID,0);
						};
					}else
					if(!strcmp(com,"$PIX")){
						int st=0;
						int npix=0;
						sscanf(str,"%s%s%d%d",com,gpPix,&st,&npix);
						if(npix){
							gpidx=st+(CurIndex%npix);
						}else gpidx=-1;
					}else
					if(!strcmp(com,"$REPAIR")){
						int st=0;
						char cc[128];
						int z=sscanf(str,"%s%s%d",com,cc,&st);
						if(z==3){
							GAMEOBJ UTP;
							RegisterUnitType(&UTP,cc);
							if(UTP.Type=='UTYP'){
								RepType=short(UTP.Index);
								MaxRep=st;
							};
						};
					}else
					if(!(strcmp(com,"$DEFEND")&&strcmp(com,"$DEFENCE"))){
						int max,ads,ada;
						char cc[128];
						int z=sscanf(str,"%s%s%d%d%d",com,cc,&max,&ada,&ads);
						if(z==5){
							GAMEOBJ UTP;
							RegisterUnitType(&UTP,cc);
							if(UTP.Type=='UTYP'){
								DefType=short(UTP.Index);
								MaxDef=max;
								AddDefAttack=ada;
								AddDefShield=ads;
							};
						};
					};
				};
			}while(good);
			Gclose(F);
		}else{
			sprintf(CCC,"Unable to open : %s (Group %s)",cc,FirstGroupName);
			MissErrorMessage("Dip error!",CCC);
		};

		// PostLoadConfig initialization

		if(Settlement){
			// ExGroupInfo
			SettlementInfo* si=new SettlementInfo;			
			Save(si);			
			AddExGroupInfo(FirstGroupName,si);			
			si->Save(cc);
			return;
		}

		sprintf(cc,"$ACTORS-%s%s",CommonID,Preffix);
		RegisterUnits(&Actors,cc);
		UseLikeSkarb=Actors.Type==0;

		sprintf(cc,"$DZONE-%s%s",CommonID,Preffix);
		RegisterZone(&DanceZone,cc);
		DanceStage=0;
		
		NStartActors=GetNUnits(&Actors);
		if(NStartActors){
			OneUnit OU;
			if(GetUnitInfo(&Actors,0,&OU)){;
				ActorsType=OU.NIndex;
			};
		};
	};
};
int DIP_SimpleBuilding::GetAmountOfPropositions(){
	return NDipPar;
};
DWORD DIP_SimpleBuilding::GetPossibility(int Index,char* Message,char* Hint){
	strcpy(Message,GetTextByID(DIPPAR[Index].MessageID));
	strcpy(Hint,GetTextByID(DIPPAR[Index].HintID));
	return Index;
};
void DIP_SimpleBuilding::PerformPossibility(DWORD Code){
	bool success=0;
	char* cod=DIPPAR[Code].ComID;
	if(!strcmp(cod,"@@@PROD")){
		//produce units
		ProcessID=1;
		//sending command
		word BUF[32];
		BUF[0]=0;//standart possibility
		BUF[1]=Index;//tribe index
		BUF[2]=word(Code);//possibility code
		SendDipCommand((char*)BUF,6);
	}else
	if(!strcmp(cod,"@@@RAZVEDKA")){
		ProcessID=2;
		//sending command
		word BUF[32];
		BUF[0]=0;//standart possibility
		BUF[1]=Index;//tribe index
		BUF[2]=word(Code);//possibility code
		SendDipCommand((char*)BUF,6);
	}else
	if(!strcmp(cod,"@@@FIRERS")){
		ProcessID=3;
		//sending command
		word BUF[32];
		BUF[0]=0;//standart possibility
		BUF[1]=Index;//tribe index
		BUF[2]=word(Code);//possibility code
		SendDipCommand((char*)BUF,6);
	}else
	if(!strcmp(cod,"@@@STORMS")){
		ProcessID=4;
		//sending command
		word BUF[32];
		BUF[0]=0;//standart possibility
		BUF[1]=Index;//tribe index
		BUF[2]=word(Code);//possibility code
		SendDipCommand((char*)BUF,6);
	}else
	if(!strcmp(cod,"@@@KILLERS")){
		ProcessID=5;
		//sending command
		word BUF[32];
		BUF[0]=0;//standart possibility
		BUF[1]=Index;//tribe index
		BUF[2]=word(Code);//possibility code
		SendDipCommand((char*)BUF,6);
	}else
	if(!strcmp(cod,"@@@TOMAHAWKS")){
		ProcessID=6;
		//sending command
		word BUF[32];
		BUF[0]=0;//standart possibility
		BUF[1]=Index;//tribe index
		BUF[2]=word(Code);//possibility code
		SendDipCommand((char*)BUF,6);
	};
	if(success){
		Busy=1;
		strcpy(CurrentStateMessage,DIPPAR[Code].ProcessID);
	};
};
void DIP_SimpleBuilding::PerformCommand(char* Data,int size){
	if(Busy){
		if(Owner==GetMyNation()){
			CreateAbilityPage();
			ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
		};
		return;
	};
	word* wData=(word*)Data;
	switch(wData[0]){
	case 0://perform possibility (produce units...)
		{
			int Code=wData[2];
			char* cod=DIPPAR[Code].ComID;
			if(!strcmp(cod,"@@@PROD")){
				//produce units
				int N;
				char ID[64];
				int z=sscanf(DIPPAR[Code].Params,"%d%s",&N,ID);
				if(z==2){
					//checking for cost
					for(int i=0;i<6;i++)if(GetResource(Owner,i)<DIPPAR[Code].Cost[i]){
						if(Owner==GetMyNation())ShowNoMoney();
						return;
					};
					for(int i=0;i<6;i++)SetResource(Owner,i,GetResource(Owner,i)-DIPPAR[Code].Cost[i]);
					RegisterUnitType(&info.Prod.UnitType,ID);
					info.Prod.Amount=N;
					info.Prod.Stage=0;
					info.Prod.StartTime=GetGlobalTime();
#ifdef AC_ADDON
					byte speed;
					z=sscanf(DIPPAR[Code].Params,"%d%s%d",&N,ID,&speed);
					info.Prod.Speed=3;
					if(z>2&&speed<8) info.Prod.Speed=speed;
#endif AC_ADDON
					ProcessID=1+1024;
					Busy=1;
					strcpy(CurrentStateMessage,DIPPAR[Code].ProcessID);
				}else{
					ProcessID=-1;//error
					char Temp[128];
					sprintf(Temp,"error: bad string: %s",DIPPAR[Code].Params);
					strcpy(CurrentStateMessage,Temp);
				};
			}else
			if(!strcmp(cod,"@@@RAZVEDKA")){
				int N;
				char ID[64];
				int z=sscanf(DIPPAR[Code].Params,"%d%s",&N,ID);
				if(z==2){
					//checking for cost
					for(int i=0;i<6;i++)if(GetResource(Owner,i)<DIPPAR[Code].Cost[i]){
						if(Owner==GetMyNation())ShowNoMoney();
						return;
					};
					for(int i=0;i<6;i++)SetResource(Owner,i,GetResource(Owner,i)-DIPPAR[Code].Cost[i]);
					RegisterUnitType(&info.Prod.UnitType,ID);
					info.Prod.Amount=N;
					info.Prod.Stage=0;
					info.Prod.StartTime=GetGlobalTime();
#ifdef AC_ADDON
					byte speed;
					z=sscanf(DIPPAR[Code].Params,"%d%s%d",&N,ID,&speed);
					info.Prod.Speed=3;
					if(z>2&&speed<8) info.Prod.Speed=speed;
#endif AC_ADDON
					ProcessID=2+1024;
					Busy=1;
					strcpy(CurrentStateMessage,DIPPAR[Code].ProcessID);
				}else{
					ProcessID=-1;//error
					char Temp[128];
					sprintf(Temp,"error: bad string: %s",DIPPAR[Code].Params);
					strcpy(CurrentStateMessage,Temp);
				};
			}else
			if(!strcmp(cod,"@@@FIRERS")){
				//produce units
				int N;
				char ID[64];
				int z=sscanf(DIPPAR[Code].Params,"%d%s",&N,ID);
				if(z==2){
					//checking for cost
					for(int i=0;i<6;i++)if(GetResource(Owner,i)<DIPPAR[Code].Cost[i]){
						if(Owner==GetMyNation())ShowNoMoney();
						return;
					};
					for(int i=0;i<6;i++)SetResource(Owner,i,GetResource(Owner,i)-DIPPAR[Code].Cost[i]);
					RegisterUnitType(&info.Prod.UnitType,ID);
					info.Prod.Amount=N;
					info.Prod.Stage=0;
					info.Prod.StartTime=GetGlobalTime();
#ifdef AC_ADDON
					byte speed;
					z=sscanf(DIPPAR[Code].Params,"%d%s%d",&N,ID,&speed);
					info.Prod.Speed=3;
					if(z>2&&speed<8) info.Prod.Speed=speed;
#endif AC_ADDON
					ProcessID=3+1024;
					Busy=1;
					strcpy(CurrentStateMessage,DIPPAR[Code].ProcessID);
				}else{
					ProcessID=-1;//error
					char Temp[128];
					sprintf(Temp,"error: bad string: %s",DIPPAR[Code].Params);
					strcpy(CurrentStateMessage,Temp);
				};
			}else
			if(!strcmp(cod,"@@@STORMS")){
				//produce units
				int N;
				char ID[64];
				int z=sscanf(DIPPAR[Code].Params,"%d%s",&N,ID);
				if(z==2){
					//checking for cost
					for(int i=0;i<6;i++)if(GetResource(Owner,i)<DIPPAR[Code].Cost[i]){
						if(Owner==GetMyNation())ShowNoMoney();
						return;
					};
					for(int i=0;i<6;i++)SetResource(Owner,i,GetResource(Owner,i)-DIPPAR[Code].Cost[i]);
					RegisterUnitType(&info.Prod.UnitType,ID);
					info.Prod.Amount=N;
					info.Prod.Stage=0;
					info.Prod.StartTime=GetGlobalTime();
#ifdef AC_ADDON
					byte speed;
					z=sscanf(DIPPAR[Code].Params,"%d%s%d",&N,ID,&speed);
					info.Prod.Speed=3;
					if(z>2&&speed<8) info.Prod.Speed=speed;
#endif AC_ADDON
					ProcessID=4+1024;
					Busy=1;
					strcpy(CurrentStateMessage,DIPPAR[Code].ProcessID);
				}else{
					ProcessID=-1;//error
					char Temp[128];
					sprintf(Temp,"error: bad string: %s",DIPPAR[Code].Params);
					strcpy(CurrentStateMessage,Temp);
				};
			}else
			if(!strcmp(cod,"@@@KILLERS")){
				//produce units
				int N;
				char ID[64];
				int z=sscanf(DIPPAR[Code].Params,"%d%s",&N,ID);
				if(z==2){
					//checking for cost
					for(int i=0;i<6;i++)if(GetResource(Owner,i)<DIPPAR[Code].Cost[i]){
						if(Owner==GetMyNation())ShowNoMoney();
						return;
					};
					for(int i=0;i<6;i++)SetResource(Owner,i,GetResource(Owner,i)-DIPPAR[Code].Cost[i]);
					RegisterUnitType(&info.Prod.UnitType,ID);
					info.Prod.Amount=N;
					info.Prod.Stage=0;
					info.Prod.StartTime=GetGlobalTime();
#ifdef AC_ADDON
					byte speed;
					z=sscanf(DIPPAR[Code].Params,"%d%s%d",&N,ID,&speed);
					info.Prod.Speed=3;
					if(z>2&&speed<8) info.Prod.Speed=speed;
#endif AC_ADDON
					ProcessID=5+1024;
					Busy=1;
					strcpy(CurrentStateMessage,DIPPAR[Code].ProcessID);
				}else{
					ProcessID=-1;//error
					char Temp[128];
					sprintf(Temp,"error: bad string: %s",DIPPAR[Code].Params);
					strcpy(CurrentStateMessage,Temp);
				};
			}else
			if(!strcmp(cod,"@@@TOMAHAWKS")){
				//produce units
				int N;
				char ID[64];
				int z=sscanf(DIPPAR[Code].Params,"%d%s",&N,ID);
				if(z==2){
					//checking for cost
					for(int i=0;i<6;i++)if(GetResource(Owner,i)<DIPPAR[Code].Cost[i]){
						if(Owner==GetMyNation())ShowNoMoney();
						return;
					};
					for(int i=0;i<6;i++)SetResource(Owner,i,GetResource(Owner,i)-DIPPAR[Code].Cost[i]);
					RegisterUnitType(&info.Prod.UnitType,ID);
					info.Prod.Amount=N;
					info.Prod.Stage=0;
					info.Prod.StartTime=GetGlobalTime();
#ifdef AC_ADDON
					byte speed;
					z=sscanf(DIPPAR[Code].Params,"%d%s%d",&N,ID,&speed);
					info.Prod.Speed=3;
					if(z>2&&speed<8) info.Prod.Speed=speed;
#endif AC_ADDON
					ProcessID=6+1024;
					Busy=1;
					strcpy(CurrentStateMessage,DIPPAR[Code].ProcessID);
				}else{
					ProcessID=-1;//error
					char Temp[128];
					sprintf(Temp,"error: bad string: %s",DIPPAR[Code].Params);
					strcpy(CurrentStateMessage,Temp);
				};
			};
		};
		break;
	case 1://agreement to cowork (first time), if button "yes" pressed
		{
			Owner=byte(wData[2]);
			if(Owner==GetMyNation()){
				CreateAbilityPage();
				ActivateDipDialog("LW_new&|LW_file&Dip//tmp.cml");
			};
			CleanGroup(&CentralGroup);
			CleanGroup(&Actors);
			SetGroupNMASK(&CentralGroup,128|GetNatNMASK(Owner)); // 255
			SetGroupNMASK(&Actors,128|GetNatNMASK(Owner));
			word Shaman=wData[3];
			if(Shaman!=0xFFFF){
				word BID=0xFFFF;
				int N=GetNUnits(&CentralGroup);
				OneUnit OU;
				for(int i=0;i<N;i++){
					GetUnitInfo(&CentralGroup,i,&OU);
					if(OU.Index!=0xFFFF)BID=OU.Index;
				};
				ChangeUnitColor(Shaman,7);
				if(BID!=0xFFFF)InviteUnitIntoBuilding(BID,Shaman,0);

			};
		};
		break;
	case 2://agreement to resell itself
		{
			//checking for price
			byte NI=byte(wData[2]);
			bool good=1;
			for(int i=0;i<6;i++)if(GetResource(NI,i)<ReSellCost[i])good=0;
			if(!good){
				if(NI==GetMyNation()){
					if(Owner==GetMyNation())ShowNoMoney();
				};
			}else{
				for(int i=0;i<6;i++){
					SetResource(NI,i,GetResource(NI,i)-ReSellCost[i]);
					ReSellCost[i]<<=1;
				};
				Owner=NI;
				if(Owner==GetMyNation()){
					CreateAbilityPage();
					ActivateDipDialog("LW_new&|LW_file&Dip//tmp.cml");
				};
				CleanGroup(&CentralGroup);
				CleanGroup(&Actors);
				SetGroupNMASK(&CentralGroup,128|GetNatNMASK(Owner)); // 255
				SetGroupNMASK(&Actors,128|GetNatNMASK(Owner));
				word Shaman=wData[3];
				if(Shaman!=0xFFFF){
					word BID=0xFFFF;
					int N=GetNUnits(&CentralGroup);
					OneUnit OU;
					for(int i=0;i<N;i++){
						GetUnitInfo(&CentralGroup,i,&OU);
						if(OU.Index!=0xFFFF)BID=OU.Index;
					};
					ChangeUnitColor(Shaman,7);
					if(BID!=0xFFFF)InviteUnitIntoBuilding(BID,Shaman,0);
				};
			};
		};
		break;
	};
	if(Busy){
		if(Owner==GetMyNation()){
			//CreateAbilityPage();
			DIPS.CreateContactsList(1);
			ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
		};
		return;
	};
};
void DIP_SimpleBuilding::PerformProduceUnitsCommend(){
	int Stage=info.Prod.Stage;
	if(Stage==0){
		//need to order units;
		EraseGroup(&DestGroup);
		byte speed=3;
#ifdef AC_ADDON
		speed=info.Prod.Speed;
#endif AC_ADDON
		for(int i=0;i<info.Prod.Amount;i++)
			ProduceUnitFast(&ProduceGroup,&info.Prod.UnitType,&DestGroup,speed);
		info.Prod.Stage=1;
	}else
	if(Stage>0){
		//units under construction
		int NU=GetTotalAmount0(&DestGroup);
		SetGroupStartNMASK(&DestGroup,128|GetNatNMASK(Owner));
		if(NU>=info.Prod.Amount||GetGlobalTime()-info.Prod.StartTime>4000){
			//repainting units
			CleanGroup(&DestGroup);
			SelectUnits(&DestGroup,0);
			if(Owner<7){
				SelChangeNation(7,Owner);//units are ready for you
				SetGroupNMASK(&DestGroup,GetNatNMASK(Owner));
			};
			Busy=0;
			ProcessID=0;
			if(DIP->BusyPageIndex==Index){
				CreateAbilityPage();
				ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
			};
		};
		ProcessDance();
	};
};
void DIP_SimpleBuilding::PerformStorms(){
	int Stage=info.Prod.Stage;
	if(Stage==0){
		//need to order units;
		EraseGroup(&DestGroup);
		byte speed=3;
#ifdef AC_ADDON
		speed=info.Prod.Speed;
#endif AC_ADDON
		for(int i=0;i<info.Prod.Amount;i++)
			ProduceUnitFast(&ProduceGroup,&info.Prod.UnitType,&DestGroup,speed);
		info.Prod.Stage=1;
	}else
	if(Stage>0){
		//units under construction
		int NU=GetTotalAmount0(&DestGroup);
		SetGroupStartNMASK(&DestGroup,128|GetNatNMASK(Owner));
		if(NU>=info.Prod.Amount||GetGlobalTime()-info.Prod.StartTime>4000){
			CleanGroup(&DestGroup);
			int N=GetNUnits(&DestGroup);
			SetGroupNMASK(&DestGroup,GetNatNMASK(Owner)|128);
			DIP->AddStorm(&DestGroup,Owner);
			CleanGroup(&DestGroup);
			Busy=0;
			ProcessID=0;
			if(DIP->BusyPageIndex==Index){
				CreateAbilityPage();
				ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
			};
		};
		ProcessDance();
	};
};

void DIP_SimpleBuilding::PerformKillers(){
	int Stage=info.Prod.Stage;
	if(Stage==0){
		//need to order units;
		EraseGroup(&DestGroup);
		byte speed=3;
#ifdef AC_ADDON
		speed=info.Prod.Speed;
#endif AC_ADDON
		for(int i=0;i<info.Prod.Amount;i++)
			ProduceUnitFast(&ProduceGroup,&info.Prod.UnitType,&DestGroup,speed);
		info.Prod.Stage=1;
	}else
	if(Stage>0){
		//units under construction
		int NU=GetTotalAmount0(&DestGroup);
		SetGroupStartNMASK(&DestGroup,128|GetNatNMASK(Owner));
		if(NU>=info.Prod.Amount||GetGlobalTime()-info.Prod.StartTime>4000){
			CleanGroup(&DestGroup);
			int N=GetNUnits(&DestGroup);
			SetGroupNMASK(&DestGroup,GetNatNMASK(Owner)|128);
			DIP->AddKillers(&DestGroup,Owner);
			CleanGroup(&DestGroup);
			Busy=0;
			ProcessID=0;
			if(DIP->BusyPageIndex==Index){
				CreateAbilityPage();
				ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
			};
		};
		ProcessDance();
	};
};

void DIP_SimpleBuilding::PerformTomahawks(){
	int Stage=info.Prod.Stage;
	if(Stage==0){
		//need to order units;
		EraseGroup(&DestGroup);
		byte speed=3;
#ifdef AC_ADDON
		speed=info.Prod.Speed;
#endif AC_ADDON
		for(int i=0;i<info.Prod.Amount;i++)
			ProduceUnitFast(&ProduceGroup,&info.Prod.UnitType,&DestGroup,speed);
		info.Prod.Stage=1;
	}else
	if(Stage>0){
		//units under construction
		int NU=GetTotalAmount0(&DestGroup);
		SetGroupStartNMASK(&DestGroup,128|GetNatNMASK(Owner));
		if(NU>=info.Prod.Amount||GetGlobalTime()-info.Prod.StartTime>4000){
			CleanGroup(&DestGroup);
			int N=GetNUnits(&DestGroup);
			SetGroupNMASK(&DestGroup,GetNatNMASK(Owner)|128);
			DIP->AddTomahawks(&DestGroup,Owner,0,0,0);
			CleanGroup(&DestGroup);
			Busy=0;
			ProcessID=0;
			if(DIP->BusyPageIndex==Index){
				CreateAbilityPage();
				ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
			};
		};
		ProcessDance();
	};
};

void DIP_SimpleBuilding::PerformFiring(){
	int Stage=info.Prod.Stage;
	if(Stage==0){
		//need to order units;
		EraseGroup(&DestGroup);
		byte speed=3;
#ifdef AC_ADDON
		speed=info.Prod.Speed;
#endif AC_ADDON
		for(int i=0;i<info.Prod.Amount;i++)
			ProduceUnitFast(&ProduceGroup,&info.Prod.UnitType,&DestGroup,speed);
		info.Prod.Stage=1;
	}else
	if(Stage>0){
		//units under construction
		int NU=GetTotalAmount0(&DestGroup);
		SetGroupStartNMASK(&DestGroup,128|GetNatNMASK(Owner));
		if(NU>=info.Prod.Amount||GetGlobalTime()-info.Prod.StartTime>4000){
			CleanGroup(&DestGroup);
			int N=GetNUnits(&DestGroup);
			SetGroupNMASK(&DestGroup,GetNatNMASK(Owner)|128);
			DIP->AddFirers(&DestGroup,Owner);
			CleanGroup(&DestGroup);
			Busy=0;
			ProcessID=0;
			if(DIP->BusyPageIndex==Index){
				CreateAbilityPage();
				ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
			};
		};
		ProcessDance();
	};
};
void DIP_SimpleBuilding::PerformRazvedka(){
	int Stage=info.Prod.Stage;
	if(Stage==0){
		//need to order units;
		EraseGroup(&DestGroup);
		byte speed=3;
#ifdef AC_ADDON
		speed=info.Prod.Speed;
#endif AC_ADDON
		for(int i=0;i<info.Prod.Amount;i++)
			ProduceUnitFast(&ProduceGroup,&info.Prod.UnitType,&DestGroup,speed);
		info.Prod.Stage=1;
	}else
	if(Stage>0){
		//units under construction
		int NU=GetTotalAmount0(&DestGroup);
		SetGroupStartNMASK(&DestGroup,128|GetNatNMASK(Owner));
		if(NU>=info.Prod.Amount||GetGlobalTime()-info.Prod.StartTime>4000){
			CleanGroup(&DestGroup);
			int N=GetNUnits(&DestGroup);
			OneUnit OU;
			SetGroupNMASK(&DestGroup,GetNatNMASK(Owner));
			for(int i=0;i<N;i++){
				if(GetUnitInfo(&DestGroup,i,&OU)){
					DIP->AddWalker(OU.Index);
				};
			};
			CleanGroup(&DestGroup);
			Busy=0;
			ProcessID=0;
			if(DIP->BusyPageIndex==Index){
				CreateAbilityPage();
				ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
			};
		};
		ProcessDance();
	};
};
//
int Norma(int,int);
CIMPORT bool BrigDisbandOne(byte NI, int BrIndex);
CIMPORT word GetPortBranch(OneObject* OB);
CIMPORT void UnPress();
CIMPORT void fes_neSettlementCapture(byte AgressorNI);
CIMPORT void AddPulseSquare(int x, int y);
CIMPORT int GetAmountBrigsInZone(byte NI,GAMEOBJ* Zone);
CIMPORT int AddUnlimitedUnitsInZone(byte NI,word NIndex,int x,int y,int r,GAMEOBJ* Dest);
//
void DIP_SimpleBuilding::ProcessTribe(){
	if(Index==0) RunPF(262,"Tribe 0");

	//checking if main building is damaged
	word MainID=0xFFFF;
	bool MainBusy=0;
	bool NeedActors=0;
	bool NeedRepair=0;
	CleanGroup(&CentralGroup);
	OneUnit OU;
	if(GetUnitInfo(&CentralGroup,0,&OU)){
		MainID=OU.Index;
		MainBusy=OU.Busy!=0;
	};
	OneObject* MainOB=GetOBJ(MainID);
	if(!MainOB) return;		

	word ProducerID=0xFFFF;
	OneUnit Producer;
	if(!GetUnitInfo(&ProduceGroup,0,&Producer)){
		Producer=OU;
	}

	//if(CounsilNI<7) NMask=(1<<CounsilNI);
	word NMask=GetNatNMASK(Owner)|128;//|128
	//if(Owner==7)NMask=128;
	SetGroupNMASK(&CentralGroup,NMask);
	SetGroupNMASK(&SecondGroup,NMask);	
	SetGroupNMASK(&Actors,NMask);
	SetGroupNMASK(&Defenders,NMask);
	SetGroupNMASK(&Alexanders,NMask);
	SetGroupNMASK(&ReinfGrp,NMask);
	SetGroupNMASK(&Repairers,NMask);
#ifdef AC_ADDON
	SetGroupNMASK(&Friends,NMask);
#endif AC_ADDON

	if(Settlement){
		// select only main building
		OBJ_SetNotSelectable(MainID,false);
		UnitsGroup* UG=GetUnitsGroup(&SecondGroup);
		if(UG){			
			byte Select=0;
			for(int i=0;i<UG->N;i++){
				OneObject* OB=GetOBJ(UG->IDS[i]);
				if(OB&&OB->Serial==UG->SNS[i]){
					//OBJ_SetNotSelectable(OB->Index,MainOB->Selected);
					OBJ_SetNotSelectable(OB->Index,false);
					Select|=OB->Selected;					
				}
			}
			for(int i=0;i<8;i++){
				if((1<<i)&Select){
					//ClearSelection(i);
					SelectUnits1(i,&CentralGroup,false);
				}
			}
			if(Select){
				UnPress();
			}
		}
		//		
		if(GetPeaceMode()) return;

		// find sklad
		if(GetRND(100)>90){
			setlFindNearStorages();
		}
		// апгрейды
		int n=SetlUpg.GetAmount();
		for(int i=0;i<n;i++){
			SetlUpg[i]->Process();			
		}
		CleanGroup(&NewUnits);
		SetGroupNMASK(&NewUnits,NMask);

		// Reinforsment variables
		int bn=0;
		const int bmax=40;
		word bid[bmax];
		word bNIndex[bmax];
		int bfree[bmax];				// free places
		memset(bfree,0,sizeof(bfree));
		int dfposx[bmax][ReinfBrig];	// positions x
		int dfposy[bmax][ReinfBrig];
		int bpos[bmax][ReinfBrig];
		int bnpos[bmax];

		//
		byte NI=GetMyRefNation();
		if(Owner==NI){
			SetLightSpot(&BigZone,3,Index);
		}else{
			ClearLightSpot(Index);
		}		
		//
		
		if(Index==0) RunPF(265,"not netral");
		// not netral
		int NOwner=0;
		if(Owner!=7){
			EraseGroup(&DestGroup);
			AddUnitsInZoneToGroup(&BigZone,&DestGroup,Owner,-1);			
			// Process owner units in settlement
			UnitsGroup* UG=GetUnitsGroup(&DestGroup);
			NOwner=UG?UG->N:0;			
			if(NOwner){
				// find nearest brigade
				ClientOfID=0xFFFF;
				int ClientR;
				
				// Process reinforsment
				memset(bnpos,0,sizeof(bnpos));

				int zx,zy,zr;
				GetZoneCoor(&BigZone,&zx,&zy);
				GetZoneR(&BigZone,&zr);

				word* id=UG->IDS;
				word* sn=UG->SNS;
				for(int i=0; i<NOwner && bn<bmax; i++)if(*id!=0xFFFF){
					OneObject* OB=GetOBJ(*id);
					if(OB&&!OB->Sdoxlo&&OB->BrigadeID!=0xFFFF){
						if(OB->BrIndex==0){
                            int r=Norma((OB->RealX>>4)-OU.x,(OB->RealY>>4)-OU.y);
							if(ClientOfID==0xFFFF||r<ClientR){
								ClientOfID=*id;
								ClientOfSN=OB->Serial;
								ClientR=r;
							}
						}
						if(OB->BrIndex>2){
							word nbid=OB->BrigadeID;
							for(int j=0;j<bn;j++)if(bid[j]==nbid){
								nbid=0xFFFF;
							}
							if(nbid!=0xFFFF&&GetPortBranch(OB)==0){
								bid[bn]=nbid;

								int cx,cy;
								bfree[bn]=GetFreePlacesInBrig(Owner,nbid,ReinfBrig,dfposx[bn],dfposy[bn],bpos[bn],cx,cy);
								int r=Norma(cx-zx,cy-zy)-100;
								if(r>zr||cx<0){
									bfree[bn]=0;
								}
								
								if(bfree[bn]>0){
									bNIndex[bn]=OB->NIndex;
									bn++;
								}
							}
						}
					}
					id++;
					sn++;
				}
				// пополнение				
				if(bn>0){
					for(int j=0;j<bn;j++){
						if(GetRND(12)==4){
							//FillFormationFromGroup(Owner,bid[j],&ReinfGrp);
							FillFormationFromGroup(Owner,bid[j],&Defenders);
						}
					}
				}else{
					//RemoveGroup(&ReinfGrp,&Defenders);
				}
				/*
				UnitsGroup* RG=GetUnitsGroup(&ReinfGrp);
				int nrg=RG->N;
				for(i=0;i<nrg;i++){
					word uid=RG->IDS[i];
					OneObject* OB=GetOBJ(uid);
					if(OB&&OB->Serial==RG->SNS[i]&&OB->BrigadeID==0xFFFF&&OB->InArmy==0&&!OB->Sdoxlo){
						for(int j=0;j<bn;j++){
							int N;
							word *SN,*ID;
							if(bfree[j]>0&&bNIndex[j]==OB->NIndex&&bnpos[j]<ReinfBrig&&GetBrGroup(Owner,bid[j],&N,&ID,&SN)){
								int bridx=bpos[j][bnpos[j]];
								if(ID[bridx]==0xFFFF){
									OBJ_ChangeNation(OB,Owner);
									OneObject* OB=GetOBJ(uid);
									if(OB){
										ID[bridx]=uid;
										SN[bridx]=OB->Serial;
										OB->BrigadeID=bid[j];
										OB->BrIndex=bridx;
										OB->InArmy=1;
										//OBJ_SmartSendTo(uid,dfposx[j][bnpos[j]],dfposy[j][bnpos[j]],0,0,128+1,0);
										OBJ_PreciseSendTo(uid,dfposx[j][bnpos[j]],dfposy[j][bnpos[j]],128+1,0);
										byte dir;
										if(GetBrDir(Owner,bid[j],dir)){	
											OBJ_Rotate(uid,dir,2);
										}										
										GetBrAddDS(Owner,bid[j],OB->AddDamage,OB->AddShield);
										SetOBRifleAttack(OB,0);

										bnpos[j]++;

										RG->IDS[i]=0xFFFF;
										bfree[j]--;							
										break;
									}
								}
							}
						}
					}
				}
				*/
			}
		}
		if(Index==0) StopPF(265);
		
		// process caravan
		if(Index==0) RunPF(263,"process caravan");		
		for(int j=0;j<MaxCaravan;j++) Caravan[j].Process();
		if(Index==0) StopPF(263);

		// process cops
		if(Index==0) RunPF(264,"process cops");
		//setlProcessCops();
		OneUnit OU;
		GetUnitInfo(&CentralGroup,0,&OU);

		GAMEOBJ Zone;
		UnitsCenter(&Zone,&CentralGroup,512);

		int NCop=CleanGroup(&Defenders);
		int NAlex=CleanGroup(&Alexanders);

		Polk* DP=GetPolkByIndex(DefPolk);
		bool bornDP=false;
		if(!DP){
			DefPolk=CreatePolk();
			DP=GetPolkByIndex(DefPolk);
			bornDP=true;
		}		
		if(DP){
			if(bornDP){
				int x=OU.x-256+GetRND(512);
				int y=OU.y-256+GetRND(512);
				DP->MovePolkTo(x,y,GetRND(255));
			}	
			UnitsGroup* UG=GetUnitsGroup(&Alexanders);
			if(UG&&GetRND(12)==7){				
				int nf=DP->GetNFormations();
				for(int i=0;i<nf;i++){
					byte NI;
					int BrigID;
					if(DP->GetFormation(i,NI,BrigID)){						
						CopyBrigadeUnitsToGroup(NI,BrigID,UG);
					}
				}
			}
		}
		if(DP&&GetRND(4)==2){
			int nf=DP->GetNFormations();
			/*if(nf==0)*/
			if(bn==0){
				if(NCop>=20){
					UnitsGroup* UG=GetUnitsGroup(&Defenders);
					if(UG){
						InsertUnitToGroup(&Defenders,UG->IDS[0]);
						UG->IDS[0]=0xFFFF;
						for(int j=1;j<3;j++){
							OneObject* OBj=GetOBJ(UG->IDS[j]);
							if(!OBj||OBj->NIndex!=CopNIndex){
								for(int i=3;i<UG->N;i++){
									OneObject* OB=GetOBJ(UG->IDS[i]);
									if(OB&&OB->NIndex==CopNIndex){										
										swap(UG->IDS[j],UG->IDS[i]);
										swap(UG->SNS[j],UG->SNS[i]);
										break;
									}
								}
							}
						}
						word BrID=CreateFormationFromGroup(7,&Defenders,OU.x,OU.y,GetRND(256));
						if(BrID!=0xFFFF){
							DP->AddBrigadeToPolk(7,BrID);							
							//
							int x;
							int y;							
							//x=OU.x-256+GetRND(512);
							//y=OU.y-256+GetRND(512);
							//SGP_MoveToPoint(Owner,&Alexanders,x,y,512,0,0,32+1);
							//
							UnitsGroup* UG=GetUnitsGroup(&Alexanders);
							CopyBrigadeUnitsToGroup(7,BrID,UG);
							//
							/*
							x=OU.x-256+GetRND(512);
							y=OU.y-256+GetRND(512);
							SGP_MoveToPoint(Owner,&Alexanders,x,y,512,0,0,32+1);
							*/
							//Brigade
							//BR->HumanGlobalSendTo(OBP->xc,OBP->yc,OBP->Dir,128,OrdType);
						}
					}
				}
			/*}else{
				//CopyUnits(&Defenders,&Alexanders,0,1000,1,0);
				//CollectUnits(&Alexanders,&Defenders,10000,CopNIndex)
				//DP->RemanPolkFrom(&Defenders);				
				if(GetRND(16)==13){
					FillFormationFromGroup(Owner,DP->,&ReinfGrp);
				}*/
			}
			for(int i=0;i<nf;i++){
				byte brNI;
				int brID;
				if(DP->GetFormation(i,brNI,brID)){
					if(bn){
						BrigDisbandOne(brNI, brID);
					}else{
						if(GetRND(3)==1){
							FillFormationFromGroup(brNI,brID,&Defenders,&Alexanders);
						}
					}
				}
			}
		}
		if(GetRND(6)==1){
			UnitsGroup* sUG=GetUnitsGroup(&Alexanders);
			UnitsGroup* dUG=GetUnitsGroup(&Defenders);
			RemoveNotBrigadesUnit(sUG,dUG);
		}

		NCop=setlGetNDeffenders();
		int NCaptCop=setlGetNDeffenders(false);

		NCopInfo=NCop;
		NOwnerInfo=NOwner;
		//
		int NA=0;
		int NPretenders=0;
		byte Agressor=0xFF;
		//if(GetRND(5)==3){
			byte OwMask=GetNatNMASK(Owner);
			for(int i=0;i<7;i++)if(i!=Owner&&!(OwMask&GetNatNMASK(i))){
				//EraseGroup(&DestGroup);
				//AddUnitsInZoneToGroup(&BigZone,&DestGroup,i,-1);
				//int Nagr=GetNUnits(&DestGroup);
				int Nagr=GetAmountBrigsInZone(i,&BigZone);
				if(Nagr>NA){
					NA=Nagr;
					Agressor=i;
				}
				if(Nagr){
					NPretenders++;
				}
			}
		//}
		//
		int SetlDeff=NCaptCop+NOwner;		
		if(SetlDeff<10&&NAlex==0&&NPretenders==1){			
 			if(Agressor!=0xFF){
				if(NA){ //NA>3*SetlDeff
					if(GetMyRefNation()==Agressor||GetMyRefNation()==Owner){
						fes_neSettlementCapture(Agressor);
						AddPulseSquare(OU.x,OU.y);
					}
					/*
						{
						PlaySound("VILAGECAPTURED");
						static char* txt=GetTextByID("#VILAGECAPTURED");
						AssignHint1(txt,100);
					}
					if()
					{
						PlaySound("VILAGECAPTUREDBYENEMY");
						static char* txt=GetTextByID("#VILAGECAPTUREDBYENEMY");
						AssignHint1(txt,100);
					}
					*/
					Owner=Agressor;					
					setlFindNearStorages();
					GAMEOBJ T={CopNIndex,0,'UTYP'};
					SetGroupNMASK(&CentralGroup,GetNatNMASK(Owner)|128); // 255
					DeleteOrders(&CentralGroup);
				}
			}
		}else{
			if(NOwner==0){
				int R0,R1;
				if(GetRND(4)==1){
					if(GetZoneR(&BigZone,&R0)&&GetZoneR(&VeryBigZone,&R1)){
						ProcessDeffenders(&Defenders,NMask,OU.x,OU.y,R0,R1);
						//ProcessDeffenders(&Alexanders,NMask,OU.x,OU.y,R0,R1);				
					}else{
						ProcessDeffenders(&Defenders,NMask,OU.x,OU.y,400,400);
						//ProcessDeffenders(&Alexanders,NMask,OU.x,OU.y,400,400);
					}			
				}
			}
		}
		// process peasants
		if(Index==0) RunPF(266,"process peasants");
		int time=GetAnimTime();
		if(NA>=0)for(int i=0;i<6;i++)if(NPeasants[i]){
			GAMEOBJ* grp=PeasGrp+i;

			AddUnlimitedUnitsInZone(7,PeasantNIndex,Producer.x,Producer.y,300,grp);
			int np=CleanGroup(grp);			
			int need=NPeasants[i]-np;
			int fastneed=NPeasants[i]/2-np;

			SetGroupNMASK(grp,NMask);			
			if(np){				
				if(NA){
					// прячемся
					//CopyUnits(grp,&ComeInside,0,np,1,0);
					SetToInternalResourcesZero(grp);
					SGP_ComeIntoBuilding(grp,0,Producer.Index);					
					LastProduce[i]=time;
				}else{
					// выбегаем
					word* List=NULL;
					int NList=GetInsideList(Producer.Index,&List);
					int in=0;
					if(List){
						for(int ii=0;ii<NList;ii++){
							word InID=List[ii];
							OneObject* ob=GetOBJ(InID);
							if(ob){								
								//UnitsGroup* UG=GetUnitsGroup(grp);
								//word* id=UG->IDS;
								for(int j=0;j<np;j++){
									OneUnit ou;
									GetUnitInfo(grp,j,&ou);
									if(InID==ou.Index){
										List[ii]=List[0];
										List[0]=InID;																				
										OBJ_LeaveMine(Producer.Index,ob->NIndex);
										in++;
									}
								}
							}
						}
					}
					//if(np==in){
						//SetToInternalResourcesZero(grp);
					//}
					// пашем					
					if(Produce[i]){	// 64*256
						SGP_TakeResourcesZone(grp,0,7,i,&VeryBigZone);
						int p0=time-LastProduce[i];
						int t=Produce[i]*256;
						if(p0>=t){							
							int rest=p0%t;
							Resource[i]+=p0/t*np;
							LastProduce[i]=time-rest;
							if(Resource[i]>=CaravanCapacity[i]&&StorageID[i]!=0xFFFF){
								//
								int res=CaravanCapacity[i]*(Resource[i]/CaravanCapacity[i]);
								// make caravan
								DipCaravan* empty=NULL;
								for(int j=0;j<MaxCaravan;j++){
									DipCaravan* car=Caravan+j;
									if(car->isFree()&&car->Run(i,res)){
										Resource[i]-=res;
										empty=NULL;
										break;
									}else
									if(!empty&&car->isEmpty()){
										empty=car;
									}
								}
								if(empty&&empty->Run(i,res)){
									Resource[i]-=res;
								}
							}
							if(Resource[i]>ResourceMax[i]){
								Resource[i]=ResourceMax[i];
							}
						}						
					}				
				}
			}else{
				LastProduce[i]=time;
			}

			// Peasant build			
			if(NA==0&&/*!Producer.Busy&&*/need>0){
				GAMEOBJ T={PeasantNIndex,0,'UTYP'};				
				ProduceUnitAtLeast(&ProduceGroup,&T,grp,fastneed,20);
				ProduceUnitAtLeast(&ProduceGroup,&T,grp,need,PeasantBuild/2);
			}

		}
		if(Index==0) StopPF(266);
		// Cop managment
		if(CopNIndex!=0xFFFF){
			GAMEOBJ T={CopNIndex,0,'UTYP'};		
			if(Owner!=7){
				UnitsGroup* DG=GetUnitsGroup(&Defenders);
				int nd=DG->N;
				if(bn==0){
					word* id=DG->IDS;
					for(int i=0;i<nd;i++,id++){
						OneObject* OB=GetOBJ(*id);
						if(OB){
							if(OB->Hidden){
								ProduceUnitTicks(&ProduceGroup,&T,&Defenders,10);
								DeleteUnit(*id);
								*id=0xFFFF;
							}else
							if(OB->NIndex!=CopNIndex){
								if(GetOrderType(OB)==0){
									OBJ_GoToMineAlways(OB->Index,Producer.Index,128+16,0);
								}								
							}
						}
					}					
				}else{
					int i=0;
					//for(int i=0;i<bn;i++)
					{
						//for(int j=0;j<bfree[i];j++){
							word nind=bNIndex[i];
							bool find=false;
							word copid=0xFFFF;
							word* id=DG->IDS;
							for(int d=0;d<nd;d++,id++){
								OneObject* OB=GetOBJ(*id);
								if(OB&&OB->Serial==DG->SNS[d]&&(!OB->Sdoxlo||OB->Hidden)&&!OB->InArmy){
									if(OB->NIndex!=nind){ //OB->NIndex==CopNIndex
										if(OB->Hidden){
											find=true;										
											GAMEOBJ t={nind,0,'UTYP'};
											ProduceUnitTicks(&ProduceGroup,&t,&Defenders,10);
											DeleteUnit(*id);
											*id=0xFFFF;
											//id--;
											//nd--;										
										}else
										if(copid==0xFFFF&&GetOrderType(OB)==0/*&&OB->NIndex==CopNIndex&&!OB->Attack&&GetOrderType(OB)!=242*/){
											copid=*id;
											OBJ_GoToMineAlways(copid,Producer.Index,128+16,0);
											T.Index=nind;
										}
									}else
									if(OB->NIndex==nind&&bn>0){	// bn - amount reinforce brigades
										//find=true;
										//InsertUnitToGroup(&ReinfGrp,*id);
										//*id=0xFFFF;								
									}
								}						
							}	
							T.Index=nind;
							/*
							if(!find){
								if(copid!=0xFFFF){
									//OBJ_GoToMine(copid,Producer.Index,128+16,0);
								}else{
									T.Index=nind;
									break;
								}
							}
							*/
						//}
					}
				}
			}						
			if(!Producer.Busy&&(NCops-NCop)>0){
				ProduceUnitAtLeast(&ProduceGroup,&T,&Defenders,NCops-NCop,CopBuild/2);
			}
		}
		if(Index==0) StopPF(264);

		if(Index==0) StopPF(262);
		return;
	}

	// old simple building
	if(UseLikeSkarb){
		if(MainID!=0xFFFF&&!AlreadyUsedLikeSkarb){
			int MAXR=0;
			if(OU.NI!=7){
				for(int i=0;i<6;i++){
					int R=GetResource(OU.NI,GOLD);
					SetResource(OU.NI,GOLD,R+DiploCost[i]);
					if(DiploCost[i]!=0)MAXR=DiploCost[i];
				};
				if(OU.NI==GetMyNation()){
					char cc[512];
					sprintf(cc,GetTextByID(FirstMeetID),MAXR);
					AssignHint(cc,300);
				};
				AlreadyUsedLikeSkarb=1;
			};
		};
		return;
	};
	SetAI_Guard(&Actors,1);
	SetAI_Guard(&CentralGroup,1);
	CleanGroup(&Actors);
	int DAC=NStartActors-GetNUnits(&Actors);
	if(DAC>0&&ActorsType!=-1){
		GAMEOBJ RTP;
		RTP.Type='UTYP';
		RTP.Serial=0;
		RTP.Index=ActorsType;
		if(!Producer.Busy){
			if(DAC>6)ProduceUnitFast(&ProduceGroup,&RTP,&Actors,0);
			else ProduceUnitFast(&ProduceGroup,&RTP,&Actors,7);
		};
		NeedActors=1;
	};
	int N=GetNUnits(&CentralGroup);
	if(N){
		if(RepType!=-1){
			if(MainID!=0xFFFF){
				GAMEOBJ RTP;
				if(OU.Life<OU.MaxLife)NeedRepair=1;
				if(NeedRepair){
					//need to repair
					RTP.Type='UTYP';
					RTP.Serial=0;
					RTP.Index=RepType;
					CleanGroup(&Repairers);
					if(GetNUnits(&Repairers)<MaxRep){
//						OneUnit OU;
						if(!Producer.Busy)ProduceUnitFast(&ProduceGroup,&RTP,&Repairers,0);
					};
					if(GetRND(100)<30)SGP_RepairBuilding(&Repairers,0,MainID);
				};
				if(NeedActors||NeedRepair||GetNUnits(&Defenders))
					if(!AttackEnemyInZone2(&Actors,&VeryBigZone,7)){
						ProcessDance();
						ProcessDance();
					};
				if(NeedActors||NeedRepair){
					if(DefType!=-1){
						RTP.Type='UTYP';
						RTP.Serial=0;
						RTP.Index=DefType;
						CleanGroup(&Defenders);
						if(GetNUnits(&Defenders)<MaxDef){
							if(!Producer.Busy)ProduceUnitFast(&ProduceGroup,&RTP,&Defenders,0);
						};
						if(!AttackEnemyInZone2(&Defenders,&VeryBigZone,7)){
							CopyUnits(&Defenders,&ComeInside,0,1000,1,1);
							ProcessDance();
						};
					};
					int ND=GetNUnits(&Defenders);
					for(int j=0;j<ND;j++){
						OneUnit OU;
						if(GetUnitInfo(&Defenders,j,&OU)){
							OU.AddDamage=AddDefAttack;
							OU.AddShield=AddDefShield;
							SetUnitInfo(&OU);
						};
					};
				}else{
					if(GetRND(100)<1){
						CopyUnits(&Repairers,&ComeInside,0,1000,1,1);
						CopyUnits(&Defenders,&ComeInside,0,1000,1,1);
						ProcessDance();
					};
				};
			}else{
				Owner=7;
				return;
			};
		};
	}else{
		Owner=7;
		return;
	};
	if(MainID!=0xFFFF){
		word* IDS;
		int N=GetInsideList(MainID,&IDS);
		if(N){
			word MID=IDS[0];
			if(MID!=0xFFFF){
				DieUnit(MID);
			};
		};
	};
	
	if(Busy){
		switch(ProcessID){
		case 1024+1://produce units
			PerformProduceUnitsCommend();
			break;
		case 1024+2:
			PerformRazvedka();
			break;
		case 1024+3:
			PerformFiring();
			break;
		case 1024+4:
			PerformStorms();
			break;
		case 1024+5:
			PerformKillers();
			break;
		case 1024+6:
			PerformTomahawks();
			break;
		};
	}else{
		//seeking for diplomat
		//AddUnitsInZoneToGroup(&VeryBigZone,&DestGroup,0xFF,DiplomatID);
		//int N=GetNUnits(&DestGroup);
//		OneUnit OU;
		//for(int p=0;p<N;p++){
		//	if(GetUnitInfo(&DestGroup,p,&OU)){
		//		OU.NMask|=(1<<Owner);
		//		SetUnitInfoNew(&OU);
		//	};
		//};
		if(DipProposition){
			//if(GetGlobalTime()-StartRelTime>500){
			//	DipProposition=0;
			//	CounsilNI=7;
			//};
			EraseGroup(&DestGroup);
			if(BigZone.Type){
				int Usage=DiplomatID;
#ifdef AC_ADDON
				if(EasyTalk) Usage=0;
#endif AC_ADDON
				AddUnitsInZoneToGroup(&BigZone,&DestGroup,0xFF,Usage);
				int N=GetNUnits(&DestGroup);
				if(!N)DipProposition=0;
			};
		}else{
			EraseGroup(&DestGroup);
			if(BigZone.Type){
				int Usage=DiplomatID;
#ifdef AC_ADDON
				if(EasyTalk) Usage=0;
#endif AC_ADDON
				AddUnitsInZoneToGroup(&BigZone,&DestGroup,0xFF,Usage);
				int N=GetNUnits(&DestGroup);
				OneUnit OU;
				if(!N)DipProposition=0;
				for(int p=0;p<N;p++){
					if(GetUnitInfo(&DestGroup,p,&OU)){
						if(OU.NI!=7&&OU.NI!=Owner){
							if(Owner==7){
								DipProposition=1;
								//CounsilNI=OU.NI;
								StartRelTime=GetGlobalTime();
								if(OU.NI==GetMyNation()){
									GAMEOBJ Zone;
									UnitsCenter(&Zone,&CentralGroup,512);
									SetStartPoint(&Zone);
									//now nobody owns this tribe
									DString DST;
									DST.ReadFromFile("Dip\\InviteMask.cml");
									DST.Replace("$CONTXT$",GetTextByID("$CONTXT$"));
									DST.Replace("$GPVOG$",gpPix);
									DST.Replace("$FRAME$",gpidx);
									DST.Replace("$PROPOSE$",GetTextByID(FirstMeetID));
									DST.Replace("$YESTXT$",GetTextByID("$YESTXT$"));
									DST.Replace("$NOTXT$",GetTextByID("$NOTXT$"));
									DST.Replace("$YESCODE$","%d&%d&%d",Index,OU.NI,OU.Index);
									DST.Replace("$NOCODE$","%d&%d&%d",Index,OU.NI,OU.Index);
									DST.WriteToFile("Dip\\tmp.cml");
									ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
									DIP->BusyPageIndex=-1;
								};
							}else{
								DipProposition=1;
								//CounsilNI=OU.NI;
								StartRelTime=GetGlobalTime();
								if(OU.NI==GetMyNation()){
									char tmp[128];
									GAMEOBJ Zone;
									UnitsCenter(&Zone,&CentralGroup,512);
									SetStartPoint(&Zone);
									//now nobody owns this tribe
									DString DST;
									DST.ReadFromFile("Dip\\ResellMask.cml");
									DST.Replace("$CONTXT$",GetTextByID("$CONTXT$"));
									DST.Replace("$GPVOG$",gpPix);
									DST.Replace("$FRAME$",gpidx);
									DST.Replace("$PROPOSE$",GetTextByID(ReSellID),GetMoneyString(ReSellCost,tmp));
									DST.Replace("$YESTXT$",GetTextByID("$YESTXT$"));
									DST.Replace("$NOTXT$",GetTextByID("$NOTXT$"));
									DST.Replace("$YESCODE$","%d&%d&%d",Index,OU.NI,OU.Index);
									DST.Replace("$NOCODE$","%d&%d&%d",Index,OU.NI,OU.Index);
									DST.WriteToFile("Dip\\tmp.cml");
									ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
									DIP->BusyPageIndex=-1;
								};
							};
						};
					};
				};
			};
		};
	};
};
void DIP_SimpleBuilding::CreateAbilityPage(){
	DString DST;
	DString One;
	if(Settlement){
		return;
		DST.ReadFromFile("Dip\\Settlment.cml");

		DST.Replace("$GPVOG$",gpPix);
		DST.Replace("$FRAME$",gpidx);

		char str[128];
		sprintf(str,"Owner: %d\nPeas: %d, %d, %d, %d, %d, %d\nRes: %d, %d, %d, %d, %d, %d",Owner,
			GetNUnits(PeasGrp+0),GetNUnits(PeasGrp+3),GetNUnits(PeasGrp+2),GetNUnits(PeasGrp+1),GetNUnits(PeasGrp+4),GetNUnits(PeasGrp+5),
			Resource[0],Resource[3],Resource[2],Resource[1],Resource[4],Resource[5]);
		DST.Replace("$PROPOSE$",str);

		// Brigade ability
		
		DST.WriteToFile("Dip\\tmp.cml");
		DIP->BusyPageIndex=-1;
		return;
	}
	if(Owner!=GetMyNation()){
		DST.ReadFromFile("Dip\\WaitMask.cml");
		DST.Replace("$CONTXT$",GetTextByID("$CONTXT$"));
		DST.Replace("$GPVOG$",gpPix);
		DST.Replace("$FRAME$",gpidx);
		DST.Replace("$PROPOSE$",GetTextByID("$DIPREBEL"));
		DST.WriteToFile("Dip\\tmp.cml");
		DIP->BusyPageIndex=-1;
		return;
	};
	if(Busy){
		DST.ReadFromFile("Dip\\WaitMask.cml");
		DST.Replace("$CONTXT$",GetTextByID("$CONTXT$"));
		DST.Replace("$GPVOG$",gpPix);
		DST.Replace("$FRAME$",gpidx);
		DST.Replace("$PROPOSE$",GetTextByID(CurrentStateMessage));
		DST.WriteToFile("Dip\\tmp.cml");
		DIP->BusyPageIndex=Index;
		return;
	};
	DST.ReadFromFile("Dip\\Propositions.cml");
	DST.Replace("$CONTXT$",GetTextByID("$CONTXT$"));
	DST.Replace("$GPVOG$",gpPix);
	DST.Replace("$FRAME$",gpidx);
	DST.Replace("$PROPOSE$",GetTextByID(CongratID));
	for(int i=0;i<NDipPar;i++){
		One.ReadFromFile("Dip\\PropString.cml");
		One.Replace("$IDX$",i+1);
		One.Replace("$IDXX$",i);
		char MSS[512];
		char STR[512];
		strcpy(MSS,GetTextByID(DIPPAR[i].MessageID));
		char* RESS[6]={"$W","$G","$S","$F","$I","$C"};
		for(int p=0;p<6;p++){
			char* s=strstr(MSS,RESS[p]);
			if(s){
				s[0]='%';
				s[1]='d';
				sprintf(STR,MSS,DIPPAR[i].Cost[p]);
				strcpy(MSS,STR);
			};
		};
		One.Replace("$PROPOSE$",MSS);
		One.Replace("$PERFORM$","%d&%d&%d",Index,i,GetMyNation());
		DST+=One;
	};
	DST.WriteToFile("Dip\\tmp.cml");
	DIP->BusyPageIndex=-1;
	ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
};
void CreateRadialPosition(int* x,int* y,int N,int R1,int R2,int Phase1,int Phase2){
	if(!N)return;
	for(int i=0;i<N;i++){
		int R,ph;
		if(i&1){
			R=R1;
			ph=Phase1;
		}else{
			R=R2;
			ph=Phase2;
		};
		x[i]=(GetTCos(ph+(i<<8)/N)*R)>>8;
		y[i]=(GetTSin(ph+(i<<8)/N)*R)>>8;
	};
};
void DIP_SimpleBuilding::ProcessDance(){
	int N=GetNUnits(&Actors);
	if(N){
		int x,y,R;
		if(GetZoneCoor(&DanceZone,&x,&y)){
			GetZoneR(&DanceZone,&R);
			int ang=256/N;
			bool busy=0;
			OneUnit OU;
			for(int i=0;i<N;i++){
				GetUnitInfo(&Actors,i,&OU);
				if(OU.Busy&&!OU.Attack)busy=1;
			};
			if(!busy){
				DanceStage++;
				int xi[512];
				int yi[512];
				int r1,r2,p1,p2;
				if(DanceStage==1){
					r1=R;r2=R;p1=0;p2=0;
				}else{
					int st=(DanceStage-1)%4;
					switch(st){
					case 0:
						r1=R-64;
						r2=R+64;
						p1=0;
						p2=0;
						break;
					case 1:
						r1=R-64;
						r2=R+64;
						p1=64;
						p2=-64;
						break;
					case 2:
						r1=R+64;
						r2=R-64;
						p1=0;
						p2=0;
						break;
					case 3:
						r1=R+64;
						r2=R-64;
						p1=-64;
						p2=64;
						break;
					};
				};
				CreateRadialPosition(xi,yi,N,r1,r2,p1,p2);
				for(int i=0;i<N;i++){
					GetUnitInfo(&Actors,i,&OU);
					if(OU.Index!=0xFFFF){
						if(!OU.Busy)OBJ_SendTo(OU.Index,xi[i]+x,yi[i]+y,128+16,0);
					};
				};
			};
		};
	};
};
void DIP_SimpleBuilding::ShowNoMoney(){
	DString DST;
	DST.ReadFromFile("Dip\\NoMoney.cml");
	DST.Replace("$CONTXT$",GetTextByID("$CONTXT$"));
	DST.Replace("$GPVOG$",gpPix);
	DST.Replace("$FRAME$",gpidx);
	DST.Replace("$PROPOSE$",GetTextByID("$NOMONEY$"));
	DST.Replace("$GOBACK$",GetTextByID("$GOBACK$"));
	DST.Replace("$TRIBEINFO$",Index);
	DST.WriteToFile("Dip\\tmp.cml");
	ActivateDipDialog("LW_new&|LW_file&Dip/tmp.cml");
	DIP->BusyPageIndex=-1;
};

int DIP_SimpleBuilding::GetSaveData(byte* buf,bool write){
	int sz=5;
	if(write){
		buf[0]=1;
	};
	if(write)memcpy(buf+sz,&Owner,((byte*)&DIP)-((byte*)&Owner));
	sz+=((byte*)&DIP)-((byte*)&Owner);
	if(write)memcpy(buf+sz,&NDipPar,((byte*)&StartRelTime)-((byte*)&NDipPar)+4);
	sz+=((byte*)&StartRelTime)-((byte*)&NDipPar)+4;
	if(NDipPar&&write)memcpy(buf+sz,DIPPAR,NDipPar*sizeof DipComParams);
	sz+=NDipPar*sizeof DipComParams;
	int s=((byte*)&BrigAbilka)-((byte*)&Settlement)+sizeof(BrigAbilka);
	if(write)memcpy(buf+sz,&Settlement,s);
	sz+=s;
	//
	xmlQuote xml;
	SetlUpg.Save(xml,&SetlUpg);
	DString txt;
	xml.GetXMLSource(&txt);
	s=strlen(txt.pchar())+1;
	if(write)memcpy(buf+sz,txt.pchar(),s);
	sz+=s;
	/*
	int n=SetlUpg.GetAmount();
	if(write)memcpy(buf+sz,&n,4);
	sz+=4;
	for(int i=0;i<n;i++){
	sz+=SetlUpg[i].GetSaveData(buf+sz,write);
	}
	*/
	// final
	if(write)((DWORD*)(buf+1))[0]=sz;
	return sz;
};
void DIP_SimpleBuilding::SetSaveData(byte* buf,int size){
	int sz=0;
	memcpy(&Owner,buf+sz,((byte*)&DIP)-((byte*)&Owner));
	sz+=((byte*)&DIP)-((byte*)&Owner);
	memcpy(&NDipPar,buf+sz,((byte*)&StartRelTime)-((byte*)&NDipPar)+4);
	sz+=((byte*)&StartRelTime)-((byte*)&NDipPar)+4;
	if(NDipPar){
		DIPPAR=(DipComParams*)malloc(NDipPar*sizeof DipComParams);
		memcpy(DIPPAR,buf+sz,NDipPar*sizeof DipComParams);
	};
	sz+=NDipPar*sizeof DipComParams;
	int s=((byte*)&BrigAbilka)-((byte*)&Settlement)+sizeof(BrigAbilka);
	memcpy(&Settlement,buf+sz,s);
	sz+=s;
	//
	xmlQuote xml;
	xml.Load((char*)(buf+sz));
	ErrorPager Err;
	SetlUpg.Load(xml,&SetlUpg,&Err);
	sz+=strlen((char*)(buf+sz))+1;
	/*
	int n=(int)*(buf+sz);
	sz+=4;
	for(int i=0;i<n;i++){
		if(i>=SetlUpg.GetAmount()){
			SetlUpgrade* SU=new SetlUpgrade;
			SetlUpg.Add(SU);
		}		
		sz+=SetlUpg[i].SetSaveData(buf+sz,size);
	}
	SetlUpg.Del(n,SetlUpg.GetAmount()-n);	
	*/
};
/*
int SetlUpgrade::GetSaveData(byte* buf,bool write){
}
int SetlUpgrade::SetSaveData(byte* buf){
	int sz=0;
	int s=((byte*)&RunTime)-((byte*)&Index)+4;
	memcpy(&Owner,buf+sz,s);
	sz+=s;
	return sz;
}
*/
void DIP_SimpleBuilding::Init(){
	memset(this,0,((byte*)&DIP)-((byte*)&Owner));
	DIP=NULL;
	memset(&DIPPAR,0,((byte*)&StartRelTime)-((byte*)&DIPPAR)+4);
};
void DIP_SimpleBuilding::Free(){
	if(DIPPAR)free(DIPPAR);
	Init();
};
CEXPORT void CheckDipBuilding(byte NI,int Index){
	for(int i=0;i<DIPS.NDIPS;i++)if(DIPS.DIPS[i]->Owner==NI){
		OneUnit OU;
		if(GetUnitInfo(&DIPS.DIPS[i]->CentralGroup,0,&OU)&&OU.Index==Index){
			((DIP_SimpleBuilding*)DIPS.DIPS[i])->CreateAbilityPage();			
		};
	};
};

void DIP_SimpleBuilding::DrawSettlementInterface(int x0,int y0,int Lx0,int Ly0,int x1,int y1){
	char txt[256];
	OneUnit OU;
	GetUnitInfo(&CentralGroup,0,&OU);
	sprintf(txt,"Owner: %d (ID: %d)",Owner,OU.Index);
	ShowString(x0+15,y0+5,txt,&fon13y2);

	if(Owner==GetMyRefNation()){		
		GAMEOBJ* pg=PeasGrp;
		int* Np=NPeasants;
		int y=0;
		for(int i=0;i<6;i++){
			int np=min(GetNUnits(pg),*Np);
			if(np){
				sprintf(txt,"%s: %d (%d)",GetResourceName(i),np,*Np);
				//ShowProgressBar(txt,np,*Np);
				ShowString(x0+25,y0+20*(y+1),txt,&fon13y2);
				y++;
			}
			pg++;
			Np++;
		}
	
		// upgrades
		int n=SetlUpg.GetAmount();
		for(int i=0;i<n;i++){
			SetlUpg[i]->Draw();			
		}

		// brigade abilki
		SetlOneBrigAbilka* BA=BrigAbilka;
		for(int i=0;i<9;i++,BA++) BA->Draw();
	}
};


// ------  DipCaravan.begin  ------------------
void DipCaravan::Init(word dsbid){
	ID=0xFFFF;
	DSBID=dsbid;
	State=0;
};
bool DipCaravan::isEmpty(){
	if(State==0&&ResAmount==0||State!=1&&ID==0xFFFF){
		State=0;
		ResAmount=0;
		return true;
	}
	return false;
}
bool DipCaravan::isFree(){
	if(State==0||ID==0xFFFF){
		//State=0;
		//return true;
	}else
	if(State==1||State==3){
		return false;
	}else
	if(State==2){
		OneObject* OB=GetOBJ(ID);
		if(OB&&OB->Serial==SN&&!OB->Sdoxlo){
			DIP_SimpleBuilding* SB=(DIP_SimpleBuilding*) DIPS.DIPS[DSBID];
			OneUnit OU;
			//GetUnitInfo(&SB->CentralGroup,0,&OU);
			GetUnitInfo(&SB->ProduceGroup,0,&OU);			
			int Norma(int,int);
			//if(Norma(OU.x-(OB->RealX>>4),OU.y-(OB->RealY>>4))<300&&!OU.Busy){
			//	return true;
			//}
			int x=-1,y;
			GetBuildingEntryPoint(&x,&y,OU.Index,0);//32);			
			if(x!=-1&&Norma(x-(OB->RealX>>4),y-(OB->RealY>>4))<300){
				return true;
			}
		}
		return false;
	}
	return false;
};
bool DipCaravan::Run(byte res, int amount){
	DIP_SimpleBuilding* SB=(DIP_SimpleBuilding*)DIPS.DIPS[DSBID];
	if(SB->Owner==7) return false;
	Owner=SB->Owner;
	ResID=res;
	ResAmount=amount;
	return true;
};
//
CIMPORT void SetStuff(word Index, word stuff);
int Norma(int,int);
CIMPORT void EraseObjIfOutOfScreen(int Index);
CIMPORT void CmdEraseObjIfOutOfScreen(int Index);
CIMPORT bool isInCampaign();
//
void DipCaravan::Process(){
	DIP_SimpleBuilding* SB=(DIP_SimpleBuilding*)DIPS.DIPS[DSBID];
	OneObject* OB=GetOBJ(ID);
	if(!OB){
		ID=0xFFFF;
	}
	if(State!=3){
		DestStorageID=0xFFFF;
	}
	// загруженный караван
	if(ResAmount>0){
		// need build
		if(State==0){			
			word DID=SB->StorageID[ResID];
			OneObject* SOB=GetOBJ(DID);			
			if(SOB&&SOB->Serial==SB->StorageSN[ResID]&&!SOB->Sdoxlo){
				GAMEOBJ CT={SB->CaravanNIndex,0,'UTYP'};
				ProduceUnitTicks(&SB->ProduceGroup,&CT,&SB->NewUnits,20); //60
				State=1;
				Owner=SB->Owner;
			}
		}else
		// wait when builded
		if(State==1){			
			OneUnit cu;
			GAMEOBJ* grp=&SB->NewUnits;
			int N=GetNUnits(grp);
			//SelectUnits(grp,0);
			for(int i=0;i<N;i++){
				GetUnitInfo(grp,i,&cu);
				if(cu.Index!=0xFFFF&&cu.NIndex==SB->CaravanNIndex){
					ID=cu.Index;
					SN=cu.Serial;
					DeleteUnitFromGroup(grp,i);
					State=3;
					// set owner
					OneObject* OB=GetOBJ(ID);
					Owner=SB->Owner;
					OB->NMask=128|GetNatNMASK(Owner);
					OB->Ready=1;
					SetStuff(ID,SB->Index);
					break;
				}
			}
		}//else
	}

	if(ID==0xFFFF||!OB) return;	
	if(OB->Serial==SN&&(!OB->Sdoxlo||OB->Hidden)){
		OneUnit MU;
		GetUnitInfo(&SB->ProduceGroup,0,&MU);
		word MainID=MU.Index;
		if(MainID==0xFFFF) return;		
		// пустой караван
		if(ResAmount<=0){			
			State=2;
		}
		// wait caravan near main building for loading and run
		if(State==2){
			if(OB->Hidden){
				DeleteUnit(ID);
				ID=0xFFFF;
				return;
			}else
			if(GetOrderType(OB)==0||OB->Attack||GetRND(100)==1){
				// go for loading near main building
				int x=-1,y;
				GetBuildingEntryPoint(&x,&y,MainID,0);//32);
				if(x==-1){
					char txt[128];
					//sprintf(txt,"Vital's error - Building: %d - do not have entry point (GetBuildingEntryPoint)",MainID);
					//MessageBox(NULL,txt,"Caravan x==-1 (stage=2)",MB_TOPMOST);
					//assert(false);
				}else{
					OB->NMask=128|GetNatNMASK(Owner);
					if(Norma(x-(OB->RealX>>4),y-(OB->RealY>>4))<300){
						if(ResAmount>0){
							State=3;
						}else
						if(GetRND(10)==7){
							//OBJ_GoToMineAlways(ID,MainID,128+16,0);
							CmdEraseObjIfOutOfScreen(ID);
						}						
						Owner=SB->Owner;						
						OB->Ready=true;
					}else{
						OB->Ready=true;
						OBJ_SmartSendTo(OB->Index,x+100-GetRND(200),y+100-GetRND(200),0,0,128+1,0);
						OB->Ready=false;
					}
				}				
			}
		}else
		// загруженный караван
		if(State==3&&(GetOrderType(OB)==0||OB->Attack||GetRND(200)==1)){
			// run order
			int ox=OB->RealX>>4,oy=OB->RealY>>4;			
			OneObject* SOB=GetOBJ(DestStorageID);
			if(!(SOB&&SOB->Serial==DestStorageSN&&!SOB->Sdoxlo)){
				Owner=SB->Owner;
				DestStorageID=SB->StorageID[ResID];
				DestStorageSN=SB->StorageSN[ResID];				
				SOB=GetOBJ(DestStorageID);
			}			
			if(SOB&&SOB->Serial==DestStorageSN&&!SOB->Sdoxlo){
				int x=-1,y;
				GetBuildingEntryPoint(&x,&y,DestStorageID,0);//32);
				if(x==-1){
					//MessageBox(NULL,"Vital's error - DipCaravan::Process()","Caravan x==-1 (stage=3)",MB_TOPMOST);
					//assert(false);
				}else{
					if(Norma(ox-x,oy-y)<100 || SOB->StandTime>4){
						AddResource(Owner,ResID,ResAmount);
						PlayerStatistic_AddTotalRes(Owner,ResID,ResAmount);
						ResAmount=0;
					}else{
						OBJ_SmartSendTo(OB->Index,x,y,0,0,128+1,0);
					}
				}
			}
		}//else
	}else{
		if(!OB->UnlimitedMotion){
			ID=0xFFFF;
		}
		/*
		if(State!=1){
			State=0;
		}
		*/
	}
	if(isInCampaign()){
		OB->NMask=255;
		//OBJ_SetNotSelectable(OB->Index,true);
	}	
};

// ------  DipCaravan.end  ------------------

// ------  SetlUpgrade.begin -------
void SetlUpgrade::Init(int id, int DSB_id, DString* str){

	Index=id;
	DSB_ID=DSB_id;
	DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[DSB_id];

	DString word;	
	str->ExtractWord(word);
	char* s=word.pchar();

	str->ExtractWord(word);
	IconFile=GPS.PreLoadGPImage(s);
	if(!GPS.LoadGP(IconFile)){
		char txt[128];
		sprintf(txt,"Wrong file name '%s' in upgrade id=%d, in group '%s'",s,id,DSB->CentralName);
		VitalError(txt);
	}
	str->ExtractWord(word);
	Icon=atoi(s); //GetIconByNameExt(s);
	str->ExtractWord(word);
	X=atoi(s);
	str->ExtractWord(word);
	Y=atoi(s);
	str->ExtractWord(word);
	strcpy(HintID,s);
	Hint=GetTextByID(HintID);
	str->ExtractWord(word);
	Duration=atoi(s);
	str->ExtractWord(word);
	if(!strcmp(s,"E_PEASANT+")){
		EffType=0;
		for(int i=0;i<6;i++){
			int ii;
			if(i==1) ii=3; else
			if(i==3) ii=1; else
				ii=i;
			str->ExtractWord(word);
			Effect[ii]=atoi(s);
		}
	}
	str->ExtractWord(word);
	if(!strcmp(s,"P_PRICE")){
		PriType=0;
		for(int i=0;i<6;i++){
			int ii;
			if(i==1) ii=3; else
			if(i==3) ii=1; else
				ii=i;
			str->ExtractWord(word);
			Price[ii]=atoi(s);
		}
	}

}

void SetlUpgrade::Process(){
	if(Icon==0xFFFF||isComplete) return;

	AddRandNew(__FILE__,__LINE__,Index);

	if(isRun){		
		int time=GetGlobalTime()-RunTime;
		if(time>=Duration){
			isComplete=true;			
			DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[DSB_ID];
			int* npeas=DSB->NPeasants;
			for(int i=0;i<6;i++) npeas[i]+=Effect[i];			
		}
	}
	return;
}
CIMPORT bool vf_SettlementUpgrade_EnableState();
void SI_RClick(int param);
CIMPORT void GetCostString(byte NI,int* Cost,char* str);
void SetlUpgrade::Draw(){
	if(!vf_SettlementUpgrade_EnableState()) return;
	//Process();
	if(Icon==0xFFFF||isComplete) return;
	//AddRandNew(__FILE__,__LINE__,Index);
	void* ico=AddIconToPanel(X,Y,0,Icon);
	if(isRun){		
		int time=GetGlobalTime()-RunTime;
		if(time<Duration){			
			AddStageToIcon(ico,time,Duration);
		}
	}
	DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[DSB_ID];
	byte Owner=DSB->Owner;
	char cost[256];
	GetCostString(Owner,(int*)(&Price),cost);
	char hint[256];
	sprintf(hint,"%s\\ (%s)",Hint,cost);
	AddHintToIcon(ico,hint);
	if(!isRun)
		AddProcToIcon(ico,SI_RClick,(DSB_ID<<16)+Index,NULL,0);	
	else
		AddProcToIcon(ico,NULL,0,NULL,0);	

}
int SUBorderFileID=-1;
CEXPORT int GetSUBorderFileID(){
	if(SUBorderFileID==-1) SUBorderFileID=GPS.PreLoadGPImage("interf3\\FormInterface");
	return SUBorderFileID;
}
CIMPORT void UnPress();
bool RunSetlUpgrade(SimpleDialog* SD){
	if(SD->UserParam!=-1) SI_RClick(SD->UserParam);
	UnPress();
	return 0;
}
bool NullClick(SimpleDialog* SD){
	UnPress();
	return 0;
}

void SetlUpgrade::Draw(DialogsSystem* DS, int x, int y){
	if(!vf_SettlementUpgrade_EnableState()) return;
	if(Icon==0xFFFF||isComplete||Duration==0) return;
	int box_spr=18;
	int FID=GetSUBorderFileID();
	int dx=65;//GPS.GetGPWidth
	int dy=65;
	x+=X*dx;
	y+=Y*dy;	
	if(isRun){
		box_spr=19;
		int x0=59;
		int y0=7;
		int y1=52;
		int H=y1-y0+1;
		int Ly=H*(GetGlobalTime()-RunTime)/Duration;
		if(Ly>H) Ly=H;
		Canvas* can=DS->AddCanvas(x+x0,y+RealLy+y0,3,H);
		can->AddLine(0,H-Ly,0,H,0xFF20FF20);
		can->AddLine(1,H-Ly,1,H,0xFF20FF20);
		can->AddLine(2,H-Ly,2,H,0xFF000000);
	}
	DS->addGPPicture(NULL,x,y,FID,box_spr);
	GPPicture* pic=DS->addGPPicture(NULL,x+3,y+4,IconFile,Icon);
	//
	//DString htxt(Hint);
	//Price.GetCostString(htxt);
	//DYNHINT(pic,htxt.pchar());
	//
	DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[DSB_ID];
	byte Owner=DSB->Owner;
	char cost[256];
	GetCostString(Owner,((int*)&Price.Wood),cost);
	char hint[256];
	sprintf(hint,"%s \\%s ",Hint,cost);
	DYNHINT(pic,hint);
	//
	pic->OnUserClick=RunSetlUpgrade;
	pic->OnUserRightClick=NullClick;
	if(Price.PlayerHasMore(GetMyRefNation())){
		pic->UserParam=(DSB_ID<<16)+Index;
		pic->Diffuse=0xFFFFFFFF;
	}else{
		pic->UserParam=-1;
		pic->Diffuse=0x88FFFFFF;		
	}
}
//
CIMPORT void PlayerStatistic_AddOnUpgradeRes(byte NI, byte ResType, int Amount);
bool SetlUpgrade::Run(){
	if(Icon==0xFFFF||isRun) return false;
	if(GetPeaceMode()) return false;
	//
	DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[DSB_ID];
	byte Owner=DSB->Owner;	
	for(int i=0;i<6;i++) if(GetResource(Owner,i)<Price[i]){		
		return false;
	}	
	isRun=true;
	RunTime=GetGlobalTime();
	Duration*=8;
	for(int i=0;i<6;i++){
		int res=GetResource(Owner,i)-Price[i];
		SetResource(Owner,i,res);
		PlayerStatistic_AddOnUpgradeRes(Owner,i,Price[i]);
	}
	return true;
};

CEXPORT void DrawSetlUpgrade(word Index, DialogsSystem* DS, int x, int y){
	DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding *)DIPS.DIPS[Index];
	DSB->DrawSetlUpg(DS,x,y);
}

void DIP_SimpleBuilding::DrawSetlUpg(DialogsSystem* DS, int x, int y){
	int n=SetlUpg.GetAmount();
	for(int i=0;i<n;i++){
		SetlUpg[i]->Draw(DS,x,y);
	}
};

//
// SetlOneBrigAbilka
void SetlOneBrigAbilka::Init(int DSB_id){
	ID=0xFFFF;
	DSB_ID=DSB_id;
}

void SI_Abilka_RClick(int param);
void SetlOneBrigAbilka::Draw(){	
	//return;
	if(ID==0xFFFF) return;
	//AddRandNew(__FILE__,__LINE__,Index);
	int file,sprite;
	GetAbilitySprite(ID,file,sprite);
	void* ico=AddIconToPanel(Slot,2,file,sprite);
	/*
	GetCostStringExp(Price,cost);
	char hint[256];
	sprintf(hint,"%s (%s)",Hint,cost);
	*/
	//if(Hint) AddHintToIcon(ico,Hint);
	char* hint=GetAbilityMessage(ID);
	if(hint) AddHintToIcon(ico,hint);

	AddProcToIcon(ico,SI_Abilka_RClick,(DSB_ID<<16)+ID,NULL,0);	
}

CEXPORT bool RegisterTribe(GAMEOBJ* Tribe, char* MainGroup){	
	DIP_SimpleBuilding** BDR=(DIP_SimpleBuilding**) DIPS.DIPS;
	for(int i=0;i<DIPS.NDIPS;i++){
		if(!strcmp(BDR[i]->CentralName,MainGroup)){
			Tribe->Index=i;
			Tribe->Type='TRIB';
			return true;
		}
	}
	Tribe->Type=0;
	return false;
}

CEXPORT byte GetTribeOwner(GAMEOBJ* Tribe){
	if(Tribe->Type=='TRIB'&&Tribe->Index<DIPS.NDIPS){
		return DIPS.DIPS[Tribe->Index]->Owner;
	}
	return 0xFF;
}

CEXPORT word GetNTribes(){
	return DIPS.NDIPS;
}
CEXPORT word GetNTribes(byte Owner){
	int n=0;
	for(int i=0;i<DIPS.NDIPS;i++){
		if(DIPS.DIPS[i]->Owner==Owner){
			n++;
		}
	}
	return n;
}
CEXPORT byte GetTribeOwner(word Index){
	if(Index<DIPS.NDIPS){
		return DIPS.DIPS[Index]->Owner;
	}
	return 0xFF;
}
CEXPORT int GetTribeBigZoneRadius(word Index){
	int R=0;
	if(Index<DIPS.NDIPS){
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[Index];
		GetZoneR(&DSB->BigZone,&R);
	}
	return R;
}
CEXPORT byte GetTribeOwner(char* MainGroup){
	DIP_SimpleBuilding** BDR=(DIP_SimpleBuilding**) DIPS.DIPS;
	for(int i=0;i<DIPS.NDIPS;i++){
		if(!strcmp(BDR[i]->CentralName,MainGroup)){
			return DIPS.DIPS[i]->Owner;
		}
	}
	return 0xFF;
}
CEXPORT bool GetTribeCenter(word Index, int &x, int &y){
	if(Index<DIPS.NDIPS){
		((DIP_SimpleBuilding*)DIPS.DIPS[Index])->GetCenter(x,y);
		return true;
	}
	return false;
}

CEXPORT word GetTribeID(OneObject* OB){
	if(OB){
		DIP_SimpleBuilding** BDR=(DIP_SimpleBuilding**) DIPS.DIPS;
		for(int i=0;i<DIPS.NDIPS;i++){
			DIP_SimpleBuilding* DS=BDR[i];
			OneUnit U;
			GetUnitInfo(&DS->CentralGroup,0,&U);
			if(U.Index==OB->Index&&U.Serial==OB->Serial){
				return i;
			}
		}
	}
	return 0xFFFF;
}
CEXPORT bool SetTribeOwner(word TribeID, byte Owner){
	if(TribeID<DIPS.NDIPS){
		DIPS.DIPS[TribeID]->Owner=Owner;
		return true;
	}
	return false;
}

CEXPORT void GetTribeGoods(word Index, byte& Resource, byte& NPeasants, byte& NPMax){
	if(Index<DIPS.NDIPS){
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[Index];
		for(byte i=0;i<6;i++){
			if(DSB->Produce[i]>0){
				Resource=i;
				NPeasants=GetTotalAmount(&DSB->PeasGrp[i]);				
				NPMax=DSB->NPeasants[i];
				if(NPeasants>NPMax) NPeasants=NPMax;
				return;
			}
		}		
	}
	Resource=0xFF;
	NPeasants=0xFF;
}
CEXPORT void GetTribeDebugInfo(word Index, int& NCop, int& NOwner){
	if(Index<DIPS.NDIPS){
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[Index];		
		NCop=DSB->NCopInfo;
		NOwner=DSB->NOwnerInfo;
		return;
	}
	NCop=-1;
	NOwner=-1;
}

CEXPORT int GetSetlFlagFileID(){
	return DIP_SimpleBuilding::FlagFileID;
}

OneObject* DIP_SimpleBuilding::GetStorage(byte resID){
	if(resID>5) return NULL;
	OneObject* OB=GetOBJ(StorageID[resID]);
	if(OB&&OB->Serial==StorageSN[resID]&&!OB->Sdoxlo) return OB;
	else return NULL;
};
OneObject* DIP_SimpleBuilding::GetMainObj(){
	UnitsGroup* UG=GetUnitsGroup(&CentralGroup);
	if(UG){
		OneObject* OB=GetOBJ(UG->IDS[0]);
		if(OB&&OB->Serial==UG->SNS[0]&&!OB->Sdoxlo){
			return OB;
		}
	}
	return NULL;
};
int DIP_SimpleBuilding::setlGetWorkers(byte resID){
	//if()
	return min(GetTotalAmount(PeasGrp+resID),NPeasants[resID]);
};

int DIP_SimpleBuilding::setlGetWorkersIntrf(byte resID){
	//if()
	return min(GetTotalAmount(PeasGrp+resID)+GetTotalAmount(PeasReinfGrp+resID),NPeasants[resID]);
};

int DIP_SimpleBuilding::setlGetWorkersFreePlacesIntrf(byte resID){
	if(NPeasants[resID]){
		return NPeasants[resID]-setlGetWorkersIntrf(resID);
	}
	return 0;
};

bool DIP_SimpleBuilding::setlAddWorker(byte resID, OneObject* OB){	
	OBJ_ChangeNation(OB,7);
	AddOneUnitToGroup(PeasGrp+resID,OB);
	return true;
}

bool DIP_SimpleBuilding::setlAddWorkerIntrf(byte resID, OneObject* OB){
	// need detect in PeasGrp and PeasReinfGrp (not dublicate)
	if(PeasReinfGrp[resID].FindUnits(OB->Index,OB->Serial)==0xFFFF){
		AddOneUnitToGroup(PeasReinfGrp+resID,OB);
		const int N=5;
		word Data[N];
		Data[0]=3005;
		Data[1]=Index;
		Data[2]=OB->Index;
		Data[3]=OB->Serial;
		Data[4]=resID;
		SendDipCommand((char*)Data,N<<1);
		return true;
	}
	return false;
};


void DIP_SimpleBuilding::setlProcessCops(){
};

void DIP_SimpleBuilding::setlFindNearStorages(){	
	//
	memset(StorageID,0xFF,sizeof(StorageID));
	if(Owner==7) return;
	//
	OneUnit OU;
	GetUnitInfo(&CentralGroup,0,&OU);
	word* stID=StorageID;
	word* stSN=StorageSN;
	for(int i=0;i<6;i++,stID++,stSN++)if(NPeasants[i]>0){
		OneObject* OB;//=GetOBJ(*stID);
		//if(!OB||OB->Serial!=*stSN||OB->Sdoxlo){
			*stID=FindNearestSklad(Owner,i,OU.x,OU.y);
			if(*stID!=0xFFFF){
				OB=GetOBJ(*stID);
				*stSN=OB->Serial;
			}
		//}
	}
}

void DIP_SimpleBuilding::Save(SettlementInfo* si){
	si->StartOwner=7;
	for(int i=0;i<6;i++) si->MaxResAmount[i]=ResourceMax[i];
	//if(CopNIndex!=0xFFFF) 
	si->CopID=GetMonsterID(CopNIndex);
	si->CopBuidTime=CopBuild;
	si->CopMaxAmount=NCops;
	//if(PeasantNIndex!=0xFFFF) 
	si->PeonID=GetMonsterID(PeasantNIndex);
	si->PeonBuidTime=PeasantBuild;
	for(int i=0;i<6;i++) si->PeonMaxAmount[i]=NPeasants[i];
	for(int i=0;i<6;i++) si->PeonTimeOnProduce[i]=Produce[i];
	//if(CaravanNIndex!=0xFFFF) 
	si->ObozID=GetMonsterID(CaravanNIndex);
	for(int i=0;i<6;i++) si->ObozResAmount[i]=CaravanCapacity[i];
	int n=SetlUpg.GetAmount();
	for(int i=0;i<n;i++){
		SetlUpgrade* NewUpg=new SetlUpgrade;
		*NewUpg=*(SetlUpg[i]);
		si->Upgrades.Add(NewUpg);
	}
};
CIMPORT bool isMine(OneObject* OB);
CIMPORT bool isStorage(OneObject* OB);
void DIP_SimpleBuilding::Load(char* GroupName, SettlementInfo* si){	
	if(RegisterUnits(&CentralGroup,GroupName)){		
		RegisterDynGroup(&ProduceGroup);
		CopyUnits(&CentralGroup,&ProduceGroup,0,100000,true,false);
		//
		if(!si->SecondGroupName.isClear()){
			if(RegisterUnits(&SecondGroup,si->SecondGroupName.pchar())){
				RemoveGroup(&CentralGroup,&SecondGroup);
				UnitsGroup* UG=GetUnitsGroup(&SecondGroup);
				if(UG){
					int Mine=-1, Storage=-1;
					for(int i=0;i<UG->N;i++){
						OneObject* OB=GetOBJ(UG->IDS[i]);
						if(OB&&OB->Serial==UG->SNS[i]){
							if(isMine(OB)){
								Mine=i;
							}else
							if(isStorage(OB)){
								Storage=i;
							}
						}
					}
					if(Mine==-1) Mine=Storage;
					if(Mine!=-1){
						InsertUnitToGroup(&SecondGroup,&CentralGroup,Mine);
						UG->IDS[Mine]=0xFFFF;
					}
				}
				CleanGroup(&CentralGroup);
				CleanGroup(&SecondGroup);
			};
		}
		SetUp();
		Settlement=true;
		Owner=si->StartOwner;		
		// group name
		if(strlen(GroupName)<127) strcpy(CentralName,GroupName);
		// zone
		char z[256];
		sprintf(z,"Z0-%s",GroupName);
		RegisterZoneEx(&BigZone,z,false);
		if(BigZone.Type!='ZONE') UnitsCenter(&BigZone,&CentralGroup,900);
		sprintf(z,"Z1-%s",GroupName);
		RegisterZoneEx(&VeryBigZone,z,false);						
		if(VeryBigZone.Type!='ZONE') UnitsCenter(&VeryBigZone,&CentralGroup,1300);
		//
		// settlement info		
		//
		int i;
		NCops=si->CopMaxAmount;
		GAMEOBJ type;
		if(RegisterUnitType(&type,si->CopID.pchar())){
			CopNIndex=type.Index;
			if(!GetPeaceMode()){
				int n=NCops;
				for(int j=0;j<n;j++){
					ProduceUnitTicks(&ProduceGroup,&type,&Defenders,20);
				}
			}
		}else CopNIndex=0xFFFF;
		CopBuild=si->CopBuidTime;
		
		if(RegisterUnitType(&type,si->PeonID.pchar())){
			PeasantNIndex=type.Index;			//
			for(i=0;i<6;i++){
				int np=si->PeonMaxAmount[i];
				NPeasants[i]=np;
				if(np>0){
					GAMEOBJ* grp=PeasGrp+i;
					RegisterDynGroup(grp);
					if(!GetPeaceMode()){
						if(np>2){
							for(int j=0;j<np;j++){
								ProduceUnitTicks(&ProduceGroup,&type,grp,20);
							}
						}
					}
				}
			}
		}else PeasantNIndex=0xFFFF;
		PeasantBuild=si->PeonBuidTime;
		
		for(i=0;i<6;i++) Produce[i]=si->PeonTimeOnProduce[i];
		if(RegisterUnitType(&type,si->ObozID.pchar())){
			CaravanNIndex=type.Index;
		}else CaravanNIndex=0xFFFF;
		for(i=0;i<6;i++){
			CaravanCapacity[i]=si->ObozResAmount[i];
			Resource[i]=CaravanCapacity[i];
			ResourceMax[i]=si->MaxResAmount[i];
		}	
		
		int n=si->Upgrades.GetAmount();
		for(i=0;i<n;i++){
			SetlUpgrade* su=new SetlUpgrade;
			*su=*(si->Upgrades[i]);
			su->Index=i;
			su->DSB_ID=Index;
			SetlUpg.Add(su);
		}

		// abilky
		SetlOneBrigAbilka* BA=BrigAbilka;
		for(i=0;i<9;i++,BA++) BA->Init(Index);

		setlFindNearStorages();
	}
};

void SettlementInfo::Load(){
};
void SettlementInfo::Save(char* FileName){
	xmlQuote xml("Template_for_settlement");
	BaseClass::Save(xml,this);
	DString fn=FileName;
	fn.Replace0(".dip",".dip.xml");
	xml.WriteToFile(fn.pchar());
};

int DIP_SimpleBuilding::setlGetNDeffenders(bool Reman){	
	int n=0;
	if(Reman){
		n=CleanGroup(&Defenders);
	}
	Polk* DP=GetPolkByIndex(DefPolk);
	if(DP){
		n+=DP->GetUnitsAmountInPolk(Reman);
	}
	return n;
};

void DIP_SimpleBuilding::SetUp(){
	//
	ClientOfID=0xFFFF;
	CopNIndex=0xFFFF;
	PeasantNIndex=0xFFFF;
	CaravanNIndex=0xFFFF;

	for(int j=0;j<MaxCaravan;j++) Caravan[j].Init(Index);
	int time=GetGlobalTime();
	for(int j=0;j<6;j++) LastProduce[j]=time;
	RegisterDynGroup(&NewUnits);
	
#ifdef AC_ADDON
	EasyTalk=0;
#endif AC_ADDON
	Busy=0;
	Owner=7;
	DipProposition=0;
	CounsilNI=7;
	DIPPAR=NULL;
	RepType=-1;
	DefType=-1;
	MaxRep=0;
	MaxDef=0;
	AddDefAttack=0;
	AddDefShield=0;
	DanceStage=0;
	ActorsType=-1;
	NDipPar=0;
	strcpy(TellNo,"$TELLNO");
	strcpy(CongratID,"$WECAN$");
	memset(ReSellCost,0,sizeof ReSellCost);
	CurrentStateMessage[0]=0;

	SetStuff(&CentralGroup,Index);
	SetStuff(&SecondGroup,Index);

	// PreLoadingConfig initialization
	RegisterDynGroup(&Repairers);
	RegisterDynGroup(&Defenders);
	RegisterDynGroup(&ComeInside);
	RegisterDynGroup(&DestGroup);
	RegisterDynGroup(&ReinfGrp);
	RegisterDynGroup(&Alexanders);	

	OneUnit OU;
	if(GetUnitInfo(&CentralGroup,0,&OU)){
		SGP_RepairBuilding(&Repairers,0,OU.Index);
		SGP_ComeIntoBuilding(&ComeInside,0,OU.Index);
	};

	DefPolk=0xFFFF;

};

void DIP_SimpleBuilding::GetCenter(int &x, int &y){
	OneUnit OU;
	GetUnitInfo(&CentralGroup,0,&OU);
	if(OU.Index!=0xFFFF){
		x=OU.x;
		y=OU.y;
	}else{
		x=-1;
	}
};

CEXPORT int vdf_GetAmountOfEnemySettlements(byte Owner,	int ResType, bool CheckUpgrades, int Level){
	int n=DIPS.NDIPS;
	int a=0;
	byte NMask=GetPlayerNMask(Owner);
	for(int i=0;i<n;i++){
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[i];
		if(DSB&&DSB->Owner!=Owner&&DSB->Owner!=7&&!(GetPlayerNMask(DSB->Owner)&NMask)&&DSB->Settlement&&DSB->Produce[ResType]>0&&DSB->NPeasants[ResType]>0){
			if(!CheckUpgrades||DSB->setlUpgIsRun(ResType,Level)){
				a++;
			}
		}
	}
	return a;
}

CEXPORT int vdf_GetAmountOfSettlements(byte Owner,	int ResType, bool CheckUpgrades, int Level){
	int n=DIPS.NDIPS;
	int a=0;
	byte NMask=GetPlayerNMask(Owner);
	for(int i=0;i<n;i++){
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[i];
		if(DSB&&DSB->Owner==Owner&&DSB->Settlement&&DSB->Produce[ResType]>0&&DSB->NPeasants[ResType]>0){
			if(!CheckUpgrades||DSB->setlUpgIsRun(ResType,Level)){
				a++;
			}
		}
	}
	return a;
}

CEXPORT int vdf_GetAmountOfPeasant(byte Owner, int ResType){
	int n=DIPS.NDIPS;
	int a=0;
	//byte NMask=GetPlayerNMask(Owner);
	for(int i=0;i<n;i++){
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[i];
		if(DSB&&DSB->Owner==Owner&&DSB->Settlement&&DSB->Produce[ResType]>0){
			a+=DSB->setlGetWorkers(ResType);
		}
	}
	return a;
}

CIMPORT int GetDistToCenterOfCity(byte Nat, int x, int y); // in pixels

CEXPORT int vdf_RunUpgInSettlement(byte Owner, int ResType, int UpgLevel, int CostPercent, int MaxDistance){
	int n=DIPS.NDIPS;
	int a=0;
	for(int i=0;i<n;i++){
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS.DIPS[i];
		if(DSB&&DSB->Owner==Owner&&DSB->Settlement){
			int m=DSB->SetlUpg.GetAmount();
			for(int j=0;j<=UpgLevel&&j<m;j++){
				SetlUpgrade* Upg=DSB->SetlUpg[j];
				if(Upg->Effect[ResType]>0&&Upg->Price.PlayerHasMorePerc(Owner,CostPercent)){
					OneUnit u;
					GetUnitInfo(&DSB->CentralGroup,0,&u);
					if(GetDistToCenterOfCity(Owner,u.x,u.y)<=MaxDistance&&Upg->Run()){
						a++;
						break;						
					}					
				}
			}
		}
	}
	return a;
}