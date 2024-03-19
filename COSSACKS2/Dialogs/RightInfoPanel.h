int Renew1Time=0;
int NatForces[8]={0,0,0,0,0,0,0,0};
int NatFin[8]   ={0,0,0,0,0,0,0,0};
int NatFlot[8]  ={0,0,0,0,0,0,0,0};
int GetFinPower(int* Fin);
char* GetPName(int i);
void GetSquare(){
	/*
	memset(NatSquare,0,sizeof NatSquare);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->Ready&&(!OB->Sdoxlo)&&OB->NInside>=5&&OB->newMons->Usage==MineID){
			NatSquare[OB->NNUM]++;
			if(OB->NInside>=10)NatSquare[OB->NNUM]++;
		};
	};
	*/
};
#define HDRDY -24
#define BarInfColor 132
int ShowNationalForces(int x,int y,bool Header){
	int xx=x+5;
	if(Header){
		ShowString(x-7,y+HDRDY,NationalForces,&WhiteFont);
		return 0;
	};
	int tt=GetRealTime();
	if(tt-Renew1Time>10000){
		memset(NatForces,0,4*8);
		memset(NatFin,0,4*8);
		memset(NatFlot,0,4*8);
		//finances
		for(int i=0;i<8;i++){
			int RRR[8];
			for(int p=0;p>8;p++)RRR[p]=XRESRC(i,p);
			NatFin[i]=GetFinPower(RRR);
		};
	};
	int y0=y+14;
	//int xx=x+(210-GetRLCStrWidth(NationalForces,&BigYellowFont))/2;
	//ShowString(xx,y,NationalForces,&BigYellowFont);
	if(tmtmt>20&&GSets.CGame.PL_INFO[0].VictCond){
		if(LastTimeStage==-1){
			LastTimeStage=GetRealTime();
		}else{
			int dd=GetRealTime()-LastTimeStage;
			if(dd>1000){
				dd/=1000;
				if(GSets.CGame.PL_INFO[0].GameTime>dd)GSets.CGame.PL_INFO[0].GameTime-=dd;
				else GSets.CGame.PL_INFO[0].GameTime=0;
				GameTime+=dd;
				if(PeaceTimeLeft>1){
					PeaceTimeLeft-=dd;
					if(PeaceTimeLeft<1)PeaceTimeLeft=1;
				}else if(PeaceTimeLeft==1){
					CmdEndPT();
				};
				LastTimeStage+=dd*1000;
			};
		};
		char cc1[100];
		int t=GSets.CGame.PL_INFO[0].GameTime;
		int s=t%60;
		int m=(t/60)%60;
		int h=t/3600;
		if(s<10&&m<10)sprintf(cc1,"%d:0%d:0%d",h,m,s);
		else if(s<10&&m>=10)sprintf(cc1,"%d:%d:0%d",h,m,s);
		else if(s>10&&m<10)sprintf(cc1,"%d:0%d:%d",h,m,s);
		else sprintf(cc1,"%d:%d:%d",h,m,s);
		//ShowString(x+2,y0+2,cc1,&BigBlackFont);
		ShowString(x,y0,cc1,&BigWhiteFont);
		y0+=30;
		int v=GetBestPlayer();
		if(v!=-1){
			sprintf(cc1,BESTPL,GetPName(v));
			ShowString(x+2,y0+2,cc1,&BlackFont);
			ShowString(x,y0,cc1,&WhiteFont);
		};
		y0+=18;
	};
	byte res[8];
	int par[8];
	int np=SortPlayers(res,par);
	for(int q=0;q<np;q++){
		int i=res[q];
		//CBar(x+2,y0,16,16,0xD0+i*4);
		int nat=GSets.CGame.cgi_NatRefTBL[i];
		char* nname=GetPName(i);
		if(strcmp(nname,"???")){
			GPS.ShowGP(x-6,y0,BordGP,95+nat,nat);
			ShowString(x+60,y0+6,nname,&WhiteFont);
			int nid=GSets.CGame.PL_INFO[i].NationID;
			if(nid>=0&&nid<GlobalAI.NAi){
				if(!SCENINF.hLib)ShowString(x+60,y0+26,GlobalAI.Ai[nid].Message,&SpecialYellowFont);
			};
					
			y0+=50;
			GPS.ShowGP(x-11+8,y0,BordGP,91,0);
			//if(NatSquare[i]<3)NatSquare[i]=3;
			char ccr[24];
			ShowVictInFormat(ccr,nat,2);
			ShowString(x+4,y0+2,INFO_G2,&SpecialYellowFont);
			ISM->Flush();
			ShowString(x+165-GetRLCStrWidth(ccr,&SpecialWhiteFont)/2,y0+2,ccr,&SpecialWhiteFont);
			ISM->Flush();
			y0+=24;
			ShowVictInFormat(ccr,nat,1);
			ShowString(x+4,y0+1,INFO_G1,&SpecialYellowFont);
			ISM->Flush();
			ShowString(x+165-GetRLCStrWidth(ccr,&SpecialWhiteFont)/2,y0+1,ccr,&SpecialWhiteFont);
			ISM->Flush();
			y0+=24;
		};
	};
	return y0-y;
};
struct HashItem{
	char* Message;
	DWORD HashKey;
	int Param1;
	int Param2;
};
class NamesHash{
public:
	int NHash;
	int MaxHash;
	HashItem* HASH;
	void AddString(char* Str,int Parm1,int Parm2);
	NamesHash();
	~NamesHash();
};
NamesHash::NamesHash(){
	NHash=0;
	MaxHash=0;
	HASH=NULL;
};
NamesHash::~NamesHash(){
	if(HASH){
		free(HASH);
		NHash=0;
		MaxHash=0;
	};
};
void NamesHash::AddString(char* Str,int Parm1,int Parm2){
	for(int i=0;i<NHash;i++){
		if(!strcmp(Str,HASH[i].Message)){
			HASH[i].Param1+=Parm1;
			HASH[i].Param2+=Parm2;
			return;
		};
	};
	if(NHash>=MaxHash){
		MaxHash+=32;
		HASH=(HashItem*)realloc(HASH,MaxHash*sizeof HashItem);
	};
	HASH[NHash].Param1=Parm1;
	HASH[NHash].Param2=Parm2;
	HASH[NHash].Message=Str;
	NHash++;
};
int ShowUnitsList(int x,int y,int Lx,word* Value){
	Nation* NTM=NATIONS+MyNation;
	//word* KILL=NTM->NKilled;
	int DY=15+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	int N2=NTM->NMon;
	int y0=y+14;
	char cc[100];
	int xx=x+5;
	NamesHash UNITS;
	NamesHash BUILDINGS;
	GeneralObject** GOS=NTM->Mon;
	for(int i=0;i<N2;i++){
		if(Value[i]){
			if(GOS[i]->newMons->Building)
				BUILDINGS.AddString(GOS[i]->Message,Value[i],0);
			else UNITS.AddString(GOS[i]->Message,Value[i],0);
		};
	};

	int x0=x+Lx;//Lx=205
	if(UNITS.NHash){
		ShowString(x+5,y0,GI_UNITS,&SmallWhiteFont);
		y0+=17;
		Hline(x-2,y0,x0+5,BarInfColor);
		int N=UNITS.NHash;
		for(i=0;i<N;i++){
			sprintf(cc,"%d",UNITS.HASH[i].Param1);
			ShowString(x+5,y0+1+DDY,UNITS.HASH[i].Message,&SmallYellowFont);
			ShowString(x0-GetRLCStrWidth(cc,&SmallYellowFont),y0+1+DDY,cc,&SmallYellowFont);
			Hline(x-2,y0+DY,x0+5,BarInfColor);
			Vline(x0-45,y0,y0+DY,BarInfColor);
			Vline(x-2,y0,y0+DY,BarInfColor);
			Vline(x0+5,y0,y0+DY,BarInfColor);
			y0+=DY-1;
		};
		y0+=4;
	};
	if(BUILDINGS.NHash){
		ShowString(x+5,y0,GI_BLD,&SmallWhiteFont);
		y0+=17;
		Hline(x-2,y0,x0+5,BarInfColor);
		int N=BUILDINGS.NHash;
		for(i=0;i<N;i++){
			sprintf(cc,"%d",BUILDINGS.HASH[i].Param1);
			ShowString(x+5,y0+1+DDY,BUILDINGS.HASH[i].Message,&SmallYellowFont);
			ShowString(x0-GetRLCStrWidth(cc,&SmallYellowFont),y0+1+DDY,cc,&SmallYellowFont);
			Hline(x-2,y0+DY,x0+5,BarInfColor);
			Vline(x0-45,y0,y0+DY,BarInfColor);
			Vline(x-2,y0,y0+DY,BarInfColor);
			Vline(x0+5,y0,y0+DY,BarInfColor);
			y0+=DY-1;
		};
		y0+=4;
	};
	return y0-y;
};
int ShowDeathList(int x,int y,bool Header){
	if(Header){
		ShowString(x-7,y+HDRDY,INFO_G3,&WhiteFont);
		return y;
	};
	return ShowUnitsList(x-5,y,200,NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NKilled);
};

int GetCurrentCost(byte Buy,byte Sell,int Amount);
int GetNominalCost(byte Buy,byte Sell,int Amount);
byte DECO[6]={0,3,2,1,4,5};
byte EECO[6]={2,0,4,5,1,3};
int ShowEconomy(int x,int y,bool Header){
	int xx=x+5;
	if(Header){
		ShowString(x-7,y+HDRDY,Economica,&WhiteFont);
		return 0;
	};
	char cc[128];
	int y0=y+33-3;
	x-=3;
	for(int ix=0;ix<6;ix++){
		for(int iy=0;iy<6;iy++){
			int xx=x+44+ix*32;
			int yy=y0+20+iy*27;
			if(ix!=iy){
				int CCost=GetCurrentCost(EECO[ix],EECO[iy],1000);
				sprintf(cc,"%d",CCost);
				ShowString(xx-(GetRLCStrWidth(cc,&SmallWhiteFont))/2,yy-5,cc,&SmallWhiteFont);
				int NCost=GetNominalCost(EECO[ix],EECO[iy],1000);
				sprintf(cc,"%d",NCost);
				ShowString(xx-(GetRLCStrWidth(cc,&SmallYellowFont))/2,yy+5,cc,&SmallYellowFont);
				if(NCost<CCost){
					Xbar(xx-15,yy-8,31,26,0x96);
				};
			};
			if(ix==0){
				//ShowString(xx-30-GetRLCStrWidth(RDS[DECO[iy]].Name,&SmallWhiteFont)/2,yy,RDS[DECO[iy]].Name,&SmallYellowFont);
				GPS.ShowGP(xx-35-13,yy-13+5,BordGP,37+DECO[iy],0);
			};
			if(iy==0){
				//ShowString(xx-GetRLCStrWidth(RDS[DECO[ix]].Name,&SmallWhiteFont)/2,yy-20,RDS[DECO[ix]].Name,&SmallYellowFont);
				GPS.ShowGP(xx-13,yy-25-13,BordGP,37+DECO[ix],0);
			};
		};
	};
	y0+=175+5;
	char* ccx=E_Comment;
	do{
		int pos=0;
		while(ccx[0]!='\\'&&ccx[0]!=0){
			cc[pos]=ccx[0];
			pos++;
			ccx++;
		};
		cc[pos]=0;
		ShowString(x,y0,cc,&SmallYellowFont);
		y0+=15;
		if(ccx[0]=='\\')ccx++;
	}while(ccx[0]);
	return 0;
};
int ShowDoxod(int x,int y,bool Header1){
	int DY=15+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	int xx;
	if(Header1){
		ShowString(x-7,y+HDRDY,INFO_G4,&WhiteFont);
		return 0;
	};
	int y0=y+14;
	char cc[128];
	ShowString(x,y0,INFO_R1,&SmallWhiteFont);
	y0+=DY+1;
	int x0=x+208;
	Hline(x-5,y0,x0+5,BarInfColor);
	ShowString(x,y0+1+DDY,RDS[CoalID].Name,&SmallYellowFont);
	sprintf(cc,"%d",NInCoal[GSets.CGame.cgi_NatRefTBL[MyNation]]);
	ShowString(x+196-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
	y0+=DY;
	Vline(x-5,y0,y0-DY,BarInfColor);
	Vline(x0+5,y0,y0-DY,BarInfColor);
	Vline(x0-30,y0,y0-DY,BarInfColor);
	Hline(x-5,y0,x0+5,BarInfColor);

	ShowString(x,y0+1+DDY,RDS[IronID].Name,&SmallYellowFont);
	sprintf(cc,"%d",NInIron[GSets.CGame.cgi_NatRefTBL[MyNation]]);
	ShowString(x+196-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
	y0+=DY;
	Vline(x-5,y0,y0-DY,BarInfColor);
	Vline(x0+5,y0,y0-DY,BarInfColor);
	Vline(x0-30,y0,y0-DY,BarInfColor);
	Hline(x-5,y0,x0+5,BarInfColor);

	ShowString(x,y0+1+DDY,RDS[GoldID].Name,&SmallYellowFont);
	sprintf(cc,"%d",NInGold[GSets.CGame.cgi_NatRefTBL[MyNation]]);
	ShowString(x+196-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
	y0+=DY;
	Vline(x-5,y0,y0-DY,BarInfColor);
	Vline(x0+5,y0,y0-DY,BarInfColor);
	Vline(x0-30,y0,y0-DY,BarInfColor);
	Hline(x-5,y0,x0+5,BarInfColor);

	ShowString(x,y0+1+DDY,RDS[StoneID].Name,&SmallYellowFont);
	sprintf(cc,"%d",NInStone[GSets.CGame.cgi_NatRefTBL[MyNation]]);
	ShowString(x+196-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
	y0+=DY;
	Vline(x-5,y0,y0-DY,BarInfColor);
	Vline(x0+5,y0,y0-DY,BarInfColor);
	Vline(x0-30,y0,y0-DY,BarInfColor);
	Hline(x-5,y0,x0+5,BarInfColor);
	//----------------------------------------------//
	y0+=2;
	ShowString(x,y0,INFO_R2,&SmallWhiteFont);
	y0+=DY+1;
	Hline(x-5,y0,x0+5,BarInfColor);
	ShowString(x,y0+1+DDY,RDS[TreeID].Name,&SmallYellowFont);
	sprintf(cc,"%d",CITY[GSets.CGame.cgi_NatRefTBL[MyNation]].WoodSpeed);
	ShowString(x+196-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
	y0+=DY;
	Vline(x-5,y0,y0-DY,BarInfColor);
	Vline(x0+5,y0,y0-DY,BarInfColor);
	Vline(x0-30,y0,y0-DY,BarInfColor);
	Hline(x-5,y0,x0+5,BarInfColor);

	//ShowString(x,y0+1+DDY,RDS[StoneID].Name,&SmallYellowFont);
	//sprintf(cc,"%d",CITY[GSets.CGame.cgi_NatRefTBL[MyNation]].StoneSpeed);
	//ShowString(x+196-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
	//y0+=DY;
	//Vline(x-5,y0,y0-DY,BarInfColor);
	//Vline(x0+5,y0,y0-DY,BarInfColor);
	//Vline(x0-30,y0,y0-DY,BarInfColor);
	//Hline(x-5,y0,x0+5,BarInfColor);

	ShowString(x,y0+1+DDY,RDS[FoodID].Name,&SmallYellowFont);
	sprintf(cc,"%d",CITY[GSets.CGame.cgi_NatRefTBL[MyNation]].FoodSpeed);
	ShowString(x+196-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
	y0+=DY;
	Vline(x-5,y0,y0-DY,BarInfColor);
	Vline(x0+5,y0,y0-DY,BarInfColor);
	Vline(x0-30,y0,y0-DY,BarInfColor);
	Hline(x-5,y0,x0+5,BarInfColor);
	DY=14;
	y0+=10;
	bool Header=false;
	int N=NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NMon;
	word* RAM=CITY[GSets.CGame.cgi_NatRefTBL[MyNation]].ReadyAmount;
	GeneralObject** GO=NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Mon;
	int Itog=0;
	NamesHash RASXODB;
	NamesHash RASXODU;
	for(int i=0;i<N;i++){
		NewMonster* NM=GO[i]->newMons;
		if(NM->ResConsumer&&NM->ResConsID==GoldID){
			int Np=(int(NM->ResConsumer)*int(RAM[i]))/80;
			if(Np){
				if(NM->Building)RASXODB.AddString(NM->Message,Np,RAM[i]);
				else RASXODU.AddString(NM->Message,Np,RAM[i]);
			};
		};
	};
	if(RASXODB.NHash+RASXODU.NHash){

		xx=x+(210-GetRLCStrWidth(INFO_R4,&YellowFont))/2;
		ShowString(xx,y0,INFO_R4,&WhiteFont);
		y0+=20;
		char* ccx=INFO_R7;
		do{
			int pos=0;
			while(ccx[0]!='\\'&&ccx[0]!=0){
				cc[pos]=ccx[0];
				pos++;
				ccx++;
			};
			cc[pos]=0;
			ShowString(x,y0,cc,&SmallYellowFont);
			y0+=DY;
			if(ccx[0]=='\\')ccx++;
		}while(ccx[0]);
		y0+=5;
		ShowString(x+160-GetRLCStrWidth(INFO_R5,&SmallYellowFont)/2,y0,INFO_R5,&SmallWhiteFont);
		ShowString(x+200-4-GetRLCStrWidth(INFO_R6,&SmallYellowFont)/2,y0,INFO_R6,&SmallWhiteFont);
		//y0+=20;

		N=RASXODU.NHash;
		if(N){
			ShowString(x,y0,GI_UNITS,&SmallWhiteFont);
			y0+=DY+1;
			Hline(x-5,y0,x0+5,BarInfColor);
			for(i=0;i<N;i++){
				ShowString(x,y0+1+DDY,RASXODU.HASH[i].Message,&SmallYellowFont);
				sprintf(cc,"%d",RASXODU.HASH[i].Param2);
				ShowString(x+160+1-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
				int Np=RASXODU.HASH[i].Param1;
				if(Np%10)sprintf(cc,"%d.%d",Np/10,Np%10);
				else sprintf(cc,"%d",Np/10,Np%10);
				ShowString(x+200-4-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
				Hline(x-5,y0+15,x0+5,BarInfColor);
				Vline(x0-45+15,y0,y0+DY,BarInfColor);
				Vline(x0-85+20,y0,y0+DY,BarInfColor);
				Vline(x-5,y0,y0+DY,BarInfColor);
				Vline(x0+5,y0,y0+DY,BarInfColor);
				y0+=DY;
				Itog+=Np;
			};
		};
		N=RASXODB.NHash;
		if(N){
			if(RASXODU.NHash){
				y0+=3;
			};
			ShowString(x,y0,GI_BLD,&SmallWhiteFont);
			y0+=16;
			Hline(x-5,y0,x0+5,BarInfColor);
			for(i=0;i<N;i++){
				ShowString(x,y0+1+DDY,RASXODB.HASH[i].Message,&SmallYellowFont);
				sprintf(cc,"%d",RASXODB.HASH[i].Param2);
				ShowString(x+160+1-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
				int Np=RASXODB.HASH[i].Param1;
				if(Np%10){
					if(Np<0)sprintf(cc,"-%d.%d",abs(Np)/10,abs(Np)%10);
					else sprintf(cc,"%d.%d",Np/10,Np%10);
				}else sprintf(cc,"%d",Np/10,Np%10);
				ShowString(x+200-4-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+1+DDY,cc,&SmallYellowFont);
				Hline(x-5,y0+15,x0+5,BarInfColor);
				Vline(x0-45+15,y0,y0+DY,BarInfColor);
				Vline(x0-85+20,y0,y0+DY,BarInfColor);
				Vline(x-5,y0,y0+DY,BarInfColor);
				Vline(x0+5,y0,y0+DY,BarInfColor);
				y0+=DY;
				Itog+=Np;
			};
		};
	};
	if(Itog){
		y0+=5;
		char ccx[16];
		if(Itog%10){
			if(Itog<0)sprintf(ccx,"-%d.%d",abs(Itog)/10,abs(Itog)%10);
			else sprintf(ccx,"%d.%d",Itog/10,Itog%10);
		}else sprintf(ccx,"%d",Itog/10);
		//sprintf(cc,INFO_R8,ccx);
		ShowString(x+200-4-GetRLCStrWidth(ccx,&SmallYellowFont)/2,y0,ccx,&SmallWhiteFont);
		ShowString(x,y0,INFO_R8,&SmallWhiteFont);
		y0+=16;
	};
	return y0-y;
};
word LastSelID=0xFFFF;
void PopupDipDialog();
int DipDlgDX=0;

CEXPORT
void SetDipDlgDX(int dx){
	DipDlgDX=dx;
};
byte InfoStyle=0;
int InfoPanelLY=0;
void CreateINFORM();
CEXPORT
void SetInfoStyle(byte Style,int LY){
	if(InfoStyle!=Style){
		InfoStyle=Style;
		InfoPanelLY=LY;
		CreateINFORM();
	}else{
		InfoStyle=Style;
		InfoPanelLY=LY;
	};		
};

int InfDX0=10+243+6;
void ProcessInformation(bool Allow){
	if(InfDX0==-1)InfDX0=InfDX;
	ProcessMiniMenu();
	if(NoPress&&mouseX>RealLx-InfDX-32)return;
	//bool mInZone=Inform&&mouseX>RealLx-InfDX-8&&mouseY<455;
	if(Inform==2){
		ShowUnitInfo();
		if(ImNSL[MyNation]){
			word MID=ImSelm[MyNation][0];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==ImSerN[MyNation][0]&&LastSelID!=OB->Index&&!OB->Sdoxlo){
					if(OB->NIndex!=UINF.UnitID){
						CreateUNITINF_UNIT(OB->NIndex,0);
						LastSelID=OB->Index;
					};
				};
			};
		}else LastSelID=0xFFFF;
		return;
	};
	if(Inform==1){
		if(InformMode==4)InfDX=InfDX0+DipDlgDX;
		else InfDX=InfDX0;
		//DrawPaperPanel(100,100,100+64*8,100+64*6);
		void DrawScrollTable1(int x0,int y0,int x1,int y1,int ym);
		if(InfoStyle)DrawScrollTable1(RealLx-InfDX,32+InfAddY,RealLx-16,InfY1+3,InfY1-25);
		else DrawScrollDoubleTable1(RealLx-InfDX,32+InfAddY,RealLx-16,InfY1+3,InfY1-25);
		bool closeit=0;
		if(Lpressed){
			if(mouseX>RealLx-32-10&&mouseY>InfAddY+32+8-13&&mouseX<RealLx-32-10+26&&mouseY<InfAddY+32+8-13+26){
				closeit=1;
			};
		};
		TempWindow TMW;
		PushWindow(&TMW);
		int x0=RealLx-243;
		int y0=InfDY+25;
		int x1=RealLx-24;
		int y1=InfY2-15;
		int ys=y0;
		int yfin;
		switch(InformMode){
		case 0:
			ShowNationalForces(x0,y0,1);
			IntersectWindows(x0-10,y0+7,x1+20,y1+6);
			yfin=ShowNationalForces(x0,y0-GlobalInfDY,0);
			break;
		case 1:
			ShowDeathList(x0,y0,1);
			IntersectWindows(x0-10,y0+7,x1+20,y1+6);
			yfin=ShowDeathList(x0,y0-GlobalInfDY,0);
			break;
		case 2:
			ShowEconomy(x0,y0,1);
			IntersectWindows(x0-10,y0+7,x1+20,y1+6);
			yfin=ShowEconomy(x0,y0-GlobalInfDY,0);
			break;
		case 3:
			ShowDoxod(x0,y0,1);
			IntersectWindows(x0-10,y0+7,x1+20,y1+6);
			yfin=ShowDoxod(x0,y0-GlobalInfDY,0);
			break;
		case 4://diplomacy
			yfin=y1-y0;
			PopupDipDialog();
			break;
		};
		MaxGlobalInfDY=yfin-y1+y0;
		if(MaxGlobalInfDY>0){
			VScrollBar* VS=(VScrollBar*)INFORM.DSS[0];
			VS->SMaxPos=MaxGlobalInfDY;
			GlobalInfDY=VS->SPos;
			VS->Enabled=1;
			VS->Visible=1;
		}else{
			VScrollBar* VS=(VScrollBar*)INFORM.DSS[0];
			VS->Enabled=0;
			VS->Visible=0;
		};
		PopWindow(&TMW);
		bool L=Lpressed;
		//if(!Allow)Lpressed=false;
		INFORM.ProcessDialogs();
		if(InfDX0!=InfDX&&!InfoStyle){
			TempWindow TW;
			PushWindow(&TW);
			int L=InfDX-InfDX0;
			for(int p=0;p<L;p+=16){
				PopWindow(&TW);
				IntersectWindows(RealLx-InfDX-1+p,InfY2,RealLx-InfDX0-1,InfY2+32);
				GPS.ShowGP(RealLx-InfDX-3+p,InfY2,BordGP,85,0);
			};
			PopWindow(&TW);
		};
		Lpressed=L;
		if(closeit){
			if(InfoStyle){
			}else{
				Inform=0;
				InfDX=InfDX0;
			};
		};
		//INFORM.RefreshView();
	};
	//if(mInZone)Lpressed=false;
};
void CreateINFORM(){
	if(InfDX0==-1)InfDX0=InfDX;
	CreateMiniMenu();
	CreateUNITINF_UNIT(UINF.UnitID,0);
	INFORM.CloseDialogs();
	INFORM.addNewGP_VScrollBar(NULL,RealLx-22-20,63+InfAddY,364+InfY1-InfAddY-450-6,1,0,3,0);
	if(InfoStyle)return;
	int DD1=75;
	int D=18;
	INFORM.HintFont=&SmallBlackFont1;
	INFORM.HintX=-1;
	INFORM.HintY=350;
	INFORM.addGPPicture(NULL,RealLx-InfDX0-1,InfY2,BordGP,85);
	SimpleDialog* TB=INFORM.addViewPort(RealLx-InfDX-1+33,InfY2+2,66-33,21);
	TB->OnClick=&InfClick;
	TB->UserParam=0;
	TB->Hint=GetTextByID("NationalForces");
	TB=INFORM.addViewPort(RealLx-InfDX0-1+70,InfY2+2,102-70,21);
	TB->OnClick=&InfClick;
	TB->UserParam=2;
	TB->Hint=GetTextByID("Economica");
	TB=INFORM.addViewPort(RealLx-InfDX0-1+106,InfY2+2,138-106,21);
	TB->OnClick=&InfClick;
	TB->UserParam=1;
	TB->Hint=GetTextByID("DeathList");
	TB=INFORM.addViewPort(RealLx-InfDX0-1+142,InfY2+2,174-142,21);
	TB->OnClick=&InfClick;
	TB->UserParam=3;
	TB->Hint=GetTextByID("Doxod");
	TB=INFORM.addViewPort(RealLx-InfDX0-1+178,InfY2+2,212-178,21);
	TB->OnClick=&InfClick;
	TB->UserParam=4;
	TB->Hint=GetTextByID("Diplomacy");
};