#include "BE_HEADERS.h"
//
CEXPORT int GetGlobalCreateBrigList(byte NI, byte MaxCen, word* CID, byte MaxOf, word* NIndex, word* Amount, byte MaxType, byte* UnlimitedOfficer, byte* UD, byte* UF);
DLLEXPORT int GetUnitIcon(int);
CEXPORT void VitalError(char*);

int BGShow2Pan=0;
word BGParam;

void CmdCreateBrigadeGlobal(word CenID, word SoldierNIndex){
	OneObject* COB=Group[CenID];
	if(COB&&!COB->Sdoxlo&&COB->NewBuilding){
		word Data[6];
		Data[0]=3003;
		Data[1]=CenID;
		Data[2]=COB->Serial;
		Data[3]=SoldierNIndex;
		Data[4]=BGParam;
		Data[5]=MyNation;
		SendDipCommand((char*)Data,sizeof Data);
	}
}

void CmdGBD_ProduceUnit(word CenID, word UnitNIndex){
	OneObject* COB=Group[CenID];
	if(COB&&!COB->Sdoxlo&&COB->NewBuilding){
		word Data[4];
		Data[0]=3004;
		Data[1]=CenID;
		Data[2]=COB->Serial;
		Data[3]=UnitNIndex;
		SendDipCommand((char*)Data,sizeof Data);
	}
}

bool BGInterfR(SimpleDialog* SD){
	UnPress();
	return true;
}
bool BGInterf(SimpleDialog* SD){
	realLpressed=false;
	UnPress();
	if(SD->ID==10){
		if(SD->UserParam!=0xFFFF){			
			CmdCreateBrigadeGlobal(SD->UserParam&0xFFFF,SD->UserParam>>16);
		}
	}else
	if(SD->ID>10){
		if(SD->UserParam!=0xFFFF){
			CmdGBD_ProduceUnit(SD->UserParam&0xFFFF,SD->UserParam>>16);
		}
	}
	return true;
}

void InitGlobalFIVar();
DLLEXPORT int GetResource(byte Nat,byte ID);
extern int FI_File;
extern bool GlBarracksVisible;
bool GetGlobalBrigProp(byte NI);
bool CheckCostHint(byte NI,word NIndex);
//
word GPPrio[2048];
DialogsSystem GBrigDS(0,0);
//
void ShowBrigGlobalInterface(){
	
	if(EngSettings.GameName!=1)
		return;

	RunPF((5<<8),"ShowBrigGlobalInterface");
	byte MC=16;
	byte MO=64;
	byte MT=512;
	static word* CID=(word*)malloc(MC<<1);
	static word* NIndex=(word*)malloc((MO*MT)<<1);
	static word* Amount=(word*)malloc((MC*MO*MT)<<1);
	static byte* UO=(byte*)malloc(MC*MO);
	static byte* UD=(byte*)malloc(MC*MO);
	static byte* UF=(byte*)malloc(MC*MO);
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//
	extern bool EnableGlobalCreateBrig;
	EnableGlobalCreateBrig=false;
	//
	GetGlobalCreateBrigList(NI,MC,CID,MO,NIndex,Amount,MT,UO,UD,UF);	
	// Connect to dialogs	
	GPPicture* Ic_=(GPPicture*)GBrigDS.Find(10);
	GPPicture* Of_=(GPPicture*)GBrigDS.Find(11);
	GPPicture* Dr_=(GPPicture*)GBrigDS.Find(12);
	GPPicture* Fl_=(GPPicture*)GBrigDS.Find(13);
	
	GPPicture* Ico=NULL;
	GPPicture* IOf=NULL;
	GPPicture* IDr=NULL;
	GPPicture* IFl=NULL;

	TextButton* Num=NULL;
	TextButton* NOf=NULL;
	TextButton* NDr=NULL;
	TextButton* NFl=NULL;

	if(Ic_&&Of_&&Dr_&&Fl_){
		Ico=(GPPicture*)Ic_->Find((DWORD)1);	
		Num=(TextButton*)Ic_->Find((DWORD)2);
		IOf=(GPPicture*)Of_->Find((DWORD)3);
		NOf=(TextButton*)Of_->Find((DWORD)6);
		IDr=(GPPicture*)Dr_->Find((DWORD)4);		
		NDr=(TextButton*)Dr_->Find((DWORD)7);
		IFl=(GPPicture*)Fl_->Find((DWORD)5);
		NFl=(TextButton*)Fl_->Find((DWORD)8);
	}	

	GeneralObject** GO=NATIONS[NI].Mon;

	// Init dialog system
	int x0=20,y0=50,dx=42,dy=42;
	int Lx=67;
	if(!Ic_){
		InitGlobalFIVar();
		Lx=GPS.GetGPWidth(FI_File,21)+2;
		//
		Ic_=GBrigDS.addGPPicture(NULL,x0,y0,FI_File,21);
		Of_=GBrigDS.addGPPicture(NULL,x0+Lx*1,y0,FI_File,21);
		Dr_=GBrigDS.addGPPicture(NULL,x0+Lx*2,y0,FI_File,21);
		Fl_=GBrigDS.addGPPicture(NULL,x0+Lx*3,y0,FI_File,21);
		Ic_->AssignID(10);
		Of_->AssignID(11);
		Dr_->AssignID(12);
		Fl_->AssignID(13);

		static char* c="777";
		RLCFont* f=&SpecialYellowFont; //SpecialWhiteFont

		int by=0;
		int ax=31, ay=by+107;

		int icof=GPS.PreLoadGPImage("Interf3\\Units_fra_mini");		
		Ico=Ic_->addGPPicture(NULL,0,0,icof,0);	// x0,y0
		Ic_->addGPPicture(NULL,0,by,FI_File,23);
		Num=Ic_->addTextButton(NULL,ax,ay,c,f,f,f,1);	//x0+5,y0+3 

		IOf=Of_->addGPPicture(NULL,0,0,0,0);
		Of_->addGPPicture(NULL,0,by,FI_File,23);
		NOf=Of_->addTextButton(NULL,ax,ay,c,f,f,f,1);
		Of_->SetHint(GetTextByID("#GlobalBrigIOf"));

		IDr=Dr_->addGPPicture(NULL,0,0,0,0);
		Dr_->addGPPicture(NULL,0,by,FI_File,23);
		NDr=Dr_->addTextButton(NULL,ax,ay,c,f,f,f,1);
		Dr_->SetHint(GetTextByID("#GlobalBrigIDr"));

		IFl=Fl_->addGPPicture(NULL,0,0,0,0);
		Fl_->addGPPicture(NULL,0,by,FI_File,23);
		NFl=Fl_->addTextButton(NULL,ax,ay,c,f,f,f,1);
		Fl_->SetHint(GetTextByID("#GlobalBrigIFl"));
        //
		Ic_->ShapeFileID=0xFFFF;
		Of_->ShapeFileID=0xFFFF;
		Dr_->ShapeFileID=0xFFFF;
		Fl_->ShapeFileID=0xFFFF;
		//		
		Ico->AssignID(1);
		Num->AssignID(2);
		IOf->AssignID(3);
		IDr->AssignID(4);
		IFl->AssignID(5);
		NOf->AssignID(6);
		NDr->AssignID(7);
		NFl->AssignID(8);
		Ic_->OnUserClick=&BGInterf;
		Of_->OnUserClick=&BGInterf;
		Dr_->OnUserClick=&BGInterf;
		Fl_->OnUserClick=&BGInterf;
		Ic_->OnUserRightClick=&BGInterfR;
		Of_->OnUserRightClick=&BGInterfR;
		Dr_->OnUserRightClick=&BGInterfR;
		Fl_->OnUserRightClick=&BGInterfR;
		//
		BGShow2Pan=0;
		// Load priority of unit type
		memset(GPPrio,0xFF,sizeof(GPPrio));
		GFILE* F=Gopen("Dip//GBrigPrio.txt","r");		
		if(F){			
			char line[256];
			int i=0;
			while(ReadWinString(F,line,256)){
				int ln=strlen(line);
				for(int l=0;l<ln;l++) if(line[l]==' ') line[l]=0;
				GAMEOBJ go;
				if(RegisterUnitType(&go,line)){
					GPPrio[go.Index]=i;
					i++;
				};
			}
			Gclose(F);
		}else{
			Log.Warning("file 'Dip//GBrigPrio.txt' not found");
			//VitalError("file 'Dip//GBrigPrio.txt' not open");
		}
	}
	//
	x0=v_ISys.GlobalBrigX;
	y0=v_ISys.GlobalBrigY;
	Lx=v_ISys.GlobalBrigDx;
	//
	Ic_->Setx(x0); Ic_->Sety(y0);
	Of_->Setx(x0+Lx*1); Of_->Sety(y0);
	Dr_->Setx(x0+Lx*2); Dr_->Sety(y0);
	Fl_->Setx(x0+Lx*3); Fl_->Sety(y0);
	//
	extern int NGlobalCenters;
	Ic_->Visible=NGlobalCenters;
	//
	/*
	static bool vTaskCII=false;
	bool vTCII=!Ic_->MouseOver;
	if(vTCII!=vTaskCII){
		vTaskCII=vTCII;
		vValuesMap()->VIEW_OBJS(_vvTASKS_CII_,vTCII);
	}
	*/
	//
	// Find best proposition
	int C,O,T,A=0;
	word prio=0xFFFF;
	for(int c=0;c<MC;c++){
		//x=x0;
		if(CID[c]>=MAXOBJECT) break;
		OneObject* CO=Group[CID[c]];		
		for(int o=0;o<MO;o++){
			word* type=NIndex+MT*o;
			GeneralObject* OfGO=GO[type[0]];			
			for(int t=3;t<MT;t++){				
				int a=Amount[MT*(MO*c+o)+t];
				if(a>0&&OfGO->OFCR){
					OfficerRecord* OfRec=OfGO->OFCR;
					if (OfRec->SDES[0].Amount[0]!=0){
						a/=OfRec->SDES[0].Amount[0];
					}else{
						a=0;
					}
					if(a==0) continue;
					word typ=type[t];
					if(typ==0xFFFF) break;
					if(GPPrio[typ]==0xFFFF) continue;
					if(a>=A&&t>2&&GPPrio[typ]<prio){ 
						A=a; C=c; O=o; T=t; prio=GPPrio[typ];
					}
				}
			}			
		}
		//y+=dy;
	}
	// Refresh dialogs	
	if(A>0){
		word* type=NIndex+MT*O;		
		Ico->SpriteID=GetUnitIcon(type[T]);
		NewMonster* NM=GO[type[T]]->newMons;
		if(NM->MinIconFile!=0xFFFF){
			Ico->FileID=NM->MinIconFile;
			Ico->SetSpriteID(NM->MinIconIndex);
		}
		// find comand center
		Nation* Nat=NATIONS+NI;
		bool ccEnable=false;		
		//
		for(int j=0;j<MC;j++){
			OneObject* OB=Group[CID[j]];
			if(OB){
				word BNIndex=OB->NIndex;
				int NBuild=Nat->PACount[BNIndex];
				word* Build=Nat->PAble[BNIndex];
				for(int i=0;i<NBuild;i++){
					if(Build[i]==type[0]){
						C=j;
						ccEnable=true;
						break;
					}
				}
			}
		}		
		//
		Ic_->UserParam=(type[T]<<16)+CID[C];
		BGParam=type[0];
		itoa(A,Num->Message,10);
		GeneralObject* go=NULL;		
		if(Ic_->MouseOver||Of_->MouseOver||Dr_->MouseOver||Fl_->MouseOver){
			Of_->Visible=(type[0]!=type[T]&&type[0]!=type[T]);
			if(Of_->Visible){
				IOf->SpriteID=GetUnitIcon(type[0]);
				go=GO[type[0]];				
				Of_->UserParam=0xFFFF;
				//
				if(ccEnable&&go->Enabled){
					Of_->Diffuse=0xFFFFFFFF;
					if(!CheckCostHint(NI,type[0])){						
						IOf->Diffuse=0xFF8F8F8F;						
					}else{
						Of_->UserParam=(type[0]<<16)+CID[C];
						IOf->Diffuse=0xFFFFFFFF;
					}
				}else{
					IOf->Diffuse=0x7FFFFFFF;
					Of_->Diffuse=0x7FFFFFFF;
				}
				NM=go->newMons;
				if(NM->MinIconFile!=0xFFFF){
					int f=NM->MinIconFile;
					int s=NM->MinIconIndex;
					IOf->FileID=f;
					IOf->SpriteID=s;
					IOf->x1=IOf->x+GPS.GetGPWidth(f,s);
					IOf->y1=IOf->y+GPS.GetGPHeight(f,s);
				}
			}
			
			Dr_->Visible=(type[1]!=0xFFFF&&type[1]!=type[T]);
			if(Dr_->Visible){
				IDr->SpriteID=GetUnitIcon(type[1]);
				go=GO[type[1]];
				Dr_->UserParam=0xFFFF;
				if(ccEnable&&go->Enabled){
					Dr_->Diffuse=0xFFFFFFFF;					
					if(!CheckCostHint(NI,type[1])){						
						IDr->Diffuse=0xFF8F8F8F;						
					}else{
						Dr_->UserParam=(type[1]<<16)+CID[C];
						IDr->Diffuse=0xFFFFFFFF;
					}
				}else{					
					IDr->Diffuse=0x7FFFFFFF;
					Dr_->Diffuse=0x7FFFFFFF;
				}
				NM=go->newMons;
				if(NM->MinIconFile!=0xFFFF){
					int f=NM->MinIconFile;
					int s=NM->MinIconIndex;
					IDr->FileID=f;
					IDr->SpriteID=s;
					IDr->x1=IDr->x+GPS.GetGPWidth(f,s);
					IDr->y1=IDr->y+GPS.GetGPHeight(f,s);
				}
			}

			Fl_->Visible=(type[2]!=0xFFFF&&type[2]!=type[T]);
			if(Fl_->Visible){
				IFl->SpriteID=GetUnitIcon(type[2]);
 				GeneralObject* go=GO[type[2]];
				Fl_->UserParam=0xFFFF;
				if(ccEnable&&go->Enabled){
					Fl_->Diffuse=0xFFFFFFFF;
					if(!CheckCostHint(NI,type[2])){						
						IFl->Diffuse=0xFF8F8F8F;						
					}else{
						Fl_->UserParam=(type[2]<<16)+CID[C];
						IFl->Diffuse=0xFFFFFFFF;
					}
				}else{					
					IFl->Diffuse=0x7FFFFFFF;
					Fl_->Diffuse=0x7FFFFFFF;
				}						
				NM=go->newMons;
				if(NM->MinIconFile!=0xFFFF){
					int f=NM->MinIconFile;
					int s=NM->MinIconIndex;
					IFl->FileID=f;
					IFl->SpriteID=s;
					IFl->x1=IFl->x+GPS.GetGPWidth(f,s);
					IFl->y1=IFl->y+GPS.GetGPHeight(f,s);
				}
			}
			//
			OneObject* OB=Group[CID[C]];
			C=0;
			word* am=Amount+MT*(MO*C+O);
			//
			int add=OB->CheckAbility(type[0]);
			if(add==-1)add=UO[MO*C+O];
			if(add>0) sprintf(NOf->Message,"%d+%d",am[0],add);
				else sprintf(NOf->Message,"%d",am[0]);
			//
			add=OB->CheckAbility(type[1]);
			if(add==-1)add=UD[MO*C+O];
			if(add>0) sprintf(NDr->Message,"%d+%d",am[1],add);
				else sprintf(NDr->Message,"%d",am[1]);
			//
			add=OB->CheckAbility(type[2]);
			if(add==-1)add=UF[MO*C+O];
			if(add>0) sprintf(NFl->Message,"%d+%d",am[2],add);
				else sprintf(NFl->Message,"%d",am[2]);
			BGShow2Pan=REALTIME;
		}else{
			if(REALTIME-BGShow2Pan>15){
				Of_->Visible=false;
				Dr_->Visible=false;
				Fl_->Visible=false;
			}
		}
		//Ic_->Enabled=true;
		Ic_->Diffuse=0xFFFFFFFF;
		static char* ich=GetTextByID("#GlobalBrigIcoEnable");
		Ic_->SetHint(ich);
		Num->Visible=true;
		if(Ic_->DSS.GetAmount()>1){
			Ic_->DSS[1]->Visible=true;
		}
	}else{
		Ico->FileID=v_ISys.GlobalUnitFileID;
		Ico->SetSpriteID(v_ISys.GlobalUnitSpriteID);
		//
		Ic_->UserParam=0xFFFF;
		//Ic_->Enabled=false;
		Ic_->Diffuse=0x7FFFFFFF;
		static char* ich=GetTextByID("#GlobalBrigIcoDisable");
		Ic_->SetHint(ich);		
		Num->Visible=false;
		if(Ic_->DSS.GetAmount()>1){
			Ic_->DSS[1]->Visible=false;
		}
		//
		Of_->Visible=false;
		Dr_->Visible=false;
		Fl_->Visible=false;
	}
	Ico->Diffuse=Ic_->Diffuse;	
	//Ico->Enabled=Ico->Enabled;
	//IOf->Enabled=IOf->Visible;
	//IDr->Enabled=IDr->Visible;
	//IFl->Enabled=IFl->Visible;
	//Num->Visible=Ic_->Enabled;
	//NOf->Visible=IOf->Visible;	
	//NDr->Visible=IDr->Visible;
	//NFl->Visible=IFl->Visible;
	//Ic_->Visible=Ico->Enabled;
	//Of_->Visible=IOf->Visible;
	//Dr_->Visible=IDr->Visible;
 	//Fl_->Visible=IFl->Visible;
	//
	GBrigDS.ProcessDialogs();
	//
	GlBarracksVisible=Ico->MouseOver;
	StopPF((5<<8));
};

CEXPORT
bool isBrigGlobalInterfaceCanMakeBrigade(){
	GPPicture* Ico=(GPPicture*)GBrigDS.Find(1);
	if(Ico) return Ico->Enabled;
	else return false;
}