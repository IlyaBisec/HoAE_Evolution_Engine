struct TrainInfo{
	word Cost[6];
	int CostTo[6];
	char* Name;
	short AddValue[8];
	short AttValue[8];
};
struct SD_Strip{
	SimpleDialog* SD;
	int Page;
};
class UnitInfo{
public:
	int UnitType;//0-unit,1-artillery,2-ship,3-building
	char* Name;
	short CurATT[NAttTypes];
	short CurSHL[16];
	TrainInfo ATTACK[16];
	int NATT;
	TrainInfo SHIELD[16];
	int NSHL;
	byte ATTMask;
	byte SHLMask;
	int NMoreUpg;
	word UPID[15];
	int Cost[6];
	short Shield;
	short GoldUpkeep;
	short IronUpkeep;
	short CoalUpkeep;
	short StoneUpkeep;
	short WoodUpkeep;
	//short FoodUpkeep;
	short BuildStages;
	byte* Picture;
	short Lx,Ly;
	word  UnitID;
	SD_Strip* SDS;
	int N_SD;
	int MaxSD;
	int Y1Line;
	char** Hints;
	int NHints;
	int MaxHints;
	char* AddHint(char* Hint);
	UnitInfo();
	~UnitInfo();
	void Close();
	void Create(int ID);
	void AddSD(SimpleDialog* SD,int Page);
};
UnitInfo::UnitInfo(){
	memset(this,0,sizeof UnitInfo);
};
void UnitInfo::Close(){
	if(Picture)free(Picture);
	if(NHints){
		for(int i=0;i<NHints;i++)free(Hints[i]);
		free(Hints);
	};
	if(SDS)free(SDS);
	memset(this,0,sizeof UnitInfo);
};
char* UnitInfo::AddHint(char* Hint){
	if(NHints>=MaxHints){
		MaxHints+=64;
		Hints=(char**)realloc(Hints,MaxHints*4);
	};
	Hints[NHints]=znew(char,strlen(Hint)+1);
	strcpy(Hints[NHints],Hint);
	NHints++;
	return Hints[NHints-1];
};
UnitInfo::~UnitInfo(){
	Close();
};
void UnitInfo::AddSD(SimpleDialog* SD,int Page){
	if(N_SD>=MaxSD){
		MaxSD+=32;
		SDS=(SD_Strip*)realloc(SDS,MaxSD*sizeof SD_Strip);
	};
	SDS[N_SD].SD=SD;
	SDS[N_SD].Page=Page;
	N_SD++;
};
extern char** NatsIDS;
int UI_Current=0;
void UnitInfo::Create(int ID){
	if(ID>=NATIONS->NMon)return;
	UI_Current=ID;
	Close();
	//1.Picture
	char* File=NULL;
	char ccc[80];
	//int Lx,Ly;
	GeneralObject* GO=NATIONS[0].Mon[ID];
	NewMonster* NM=GO->newMons;
	int pid=NM->PictureID;
	UnitType=NM->InfType;
	Name=NM->Message;
	switch(NM->InfType){
	case 0://unit
		File="Units.bmp";
		Lx=104;
		Ly=132;
		break;
	case 1://artillery
		File="Artillery.bmp";
		Lx=132;
		Ly=114;
		break;
	case 2://Ship
		File="Ships.bmp";
		Lx=132;
		Ly=151;
		break;
	case 3://Buildings
		sprintf(ccc,"%s_BLD.bmp",NatsIDS[GO->NatID]);
		File=ccc;
		Lx=240;
		Ly=151;
		break;
	};
	if(!File)return;
	ResFile F=RReset(File);
	if(F!=INVALID_HANDLE_VALUE){
		int Max=(RFileSize(F)-0x436)/Lx/Ly;
		if(pid<Max){
			Picture=znew(byte,Lx*Ly);
			RSeek(F,(Max-pid-1)*Lx*Ly+0x436);
			for(int i=0;i<Ly;i++)RBlockRead(F,Picture+Lx*(Ly-i-1),Lx);
		};
		RClose(F);
	};
	//CurATT
	for(int i=0;i<NAttTypes;i++)if(NM->MaxDamage[i]&&CurATT[NM->WeaponKind[i]]<NM->MaxDamage[i])CurATT[NM->WeaponKind[i]]=NM->MaxDamage[i];
	//CurSHL
	for(int i=0;i<16;i++)if(NM->Protection[i])CurSHL[i+1]=NM->Protection[i];
	Shield=NM->Shield;
	//ATTACK - search for upgrades
	int NUP=0;
	Nation* NT=NATIONS;
	for(int i=0;i<NT->NUpgrades&&NUP<6;i++){
		NewUpgrade* NU=NT->UPGRADE[i];
		if(NU->CtgUpgrade==12&&NU->UnitGroup==NULL&&NU->UnitType==0&&NU->UnitValue==ID&&NU->CtgGroup==NULL&&NU->CtgType==1){//Upgrade on attack
			if(NU->Level==NUP+2){
				if(NU->CtgGroup)for(int j=0;j<NU->NCtg;j++)ATTACK[NUP].AddValue[NU->CtgGroup[j]]=NU->Value;
				else ATTACK[NUP].AddValue[NU->CtgValue]=NU->Value;
				memcpy(ATTACK[NUP].Cost,NU->Cost,2*6);
				if(NUP){
					for(int j=0;j<8;j++)ATTACK[NUP].AttValue[j]=ATTACK[NUP-1].AttValue[j]+ATTACK[NUP].AddValue[j];
					for(int j=0;j<6;j++)ATTACK[NUP].CostTo[j]=ATTACK[NUP-1].CostTo[j]+NU->Cost[j];
				}else{
					for(int j=0;j<8;j++)ATTACK[NUP].AttValue[j]=CurATT[j]+ATTACK[NUP].AddValue[j];
					for(int j=0;j<6;j++)ATTACK[NUP].CostTo[j]=NU->Cost[j];
				};
				NU->NatID+=32;
				ATTACK[NUP].Name=NU->Message;
				NUP++;
			};
		};
	};
	NATT=NUP;
	for(int j=0;j<NATT;j++)
		for(int k=0;k<8;k++)if(ATTACK[j].AddValue[k])ATTMask|=1<<k;
	//for(j=0;j<8;j++)if(CurATT[j])ATTMask|=1<<j;
	//SHIELD - search for upgrades
	NUP=0;
	for(int i=0;i<NT->NUpgrades&&NUP<6;i++){
		NewUpgrade* NU=NT->UPGRADE[i];
		if(NU->CtgUpgrade==2&&NU->UnitGroup==NULL&&NU->UnitType==0&&NU->UnitValue==ID){//Upgrade on protection
			if(NU->Level==NUP+2){
				if(NU->CtgGroup)for(int j=0;j<NU->NCtg;j++)SHIELD[NUP].AddValue[NU->CtgGroup[j]]=NU->Value;
				else SHIELD[NUP].AddValue[NU->CtgValue]=NU->Value;
				memcpy(SHIELD[NUP].Cost,NU->Cost,2*6);
				if(NUP){
					for(int j=0;j<8;j++)SHIELD[NUP].AttValue[j]=SHIELD[NUP-1].AttValue[j]+SHIELD[NUP].AddValue[j];
					for(int j=0;j<6;j++)SHIELD[NUP].CostTo[j]=SHIELD[NUP-1].CostTo[j]+NU->Cost[j];
				}else{
					for(int j=0;j<8;j++)SHIELD[NUP].AttValue[j]=CurSHL[j+1]+SHIELD[NUP].AddValue[j];
					for(int j=0;j<6;j++)SHIELD[NUP].CostTo[j]=NU->Cost[j];
				};
				NU->NatID+=32;
				SHIELD[NUP].Name=NU->Message;
				NUP++;
			};
		};
	};
	NSHL=NUP;
	for(int j=0;j<NSHL;j++)
		for(int k=0;k<8;k++)if(SHIELD[j].AddValue[k])SHLMask|=1<<k;
	//for(j=0;j<8;j++)if(CurSHL[j])SHLMask|=1<<j;
	//Other upgrades
	for(int i=0;i<NT->NUpgrades;i++){
		NewUpgrade* NU=NT->UPGRADE[i];
		bool add=false;
		if(NU->NatID<32){
			if(GO->NatID==NU->NatID&&strcmp(NU->Name,NU->Message)){
				int UCTG=NU->CtgUpgrade;
				if((UCTG<18)&&UCTG!=11&&UCTG!=14&&UCTG!=15){
					if(NU->UnitGroup){
						for(int p=0;p<NU->NUnits&&!add;p++)if(NU->UnitGroup[p]==ID)add=true;
					}else if(NU->UnitValue==ID)add=true;
				};
				if(NM->Artilery&&(UCTG==5||UCTG==10))add=false;
				if(NM->Peasant&&UCTG>=19&&UCTG<=21)add=true;
			};
		}else{
			NU->NatID-=32;
		};
		if(add&&NMoreUpg<25){
			UPID[NMoreUpg]=i;
			NMoreUpg++;
		};
	};
	//cost
	memcpy(Cost,NM->NeedRes,6*4);
	//Upkeep
	if(NM->ResConsID==GoldID)GoldUpkeep=(int(NM->ResConsumer)*100)/80;
	if(NM->ResConsID==StoneID)StoneUpkeep=(int(NM->ResConsumer)*100)/800;
	if(NM->ResConsID==TreeID)WoodUpkeep=(int(NM->ResConsumer)*100)/800;
	if(NM->NShotRes){
		for(int i=0;i<NM->NShotRes;i++){
			int r=NM->ShotRes[i+i];
			if(r==IronID)IronUpkeep=NM->ShotRes[i+i+1];
			if(r==CoalID)CoalUpkeep=NM->ShotRes[i+i+1];
		};
	};
	BuildStages=NM->ProduceStages;
	UnitID=ID;
};
int UI_CurPage=0;
int UI_PrmPage[4]={0,0,0,3};
int UI_Style=0;
UnitInfo UINF;
bool UI_PageSelect(SimpleDialog* SD){
	UI_PrmPage[UINF.UnitType]=SD->UserParam;
	Lpressed=false;
	return true;
};
int CreateUNITINF_UNIT(int ID,bool refresh);
extern int NNations;
bool UI_NextBTN(SimpleDialog* SD){
	bool FOUND=0;
	for(int i=0;i<NNations;i++){
		int N=NATIONS->NUnits[i];
		for(int j=0;j<N;j++){
			if(NATIONS->UnitsIDS[i][j]==UI_Current){
				FOUND=1;
			}else if(FOUND){
				int ID=NATIONS->UnitsIDS[i][j];
				if(ID!=0xFFFF){
					GeneralObject* GO=NATIONS[0].Mon[ID];
					if(GO->newMons->IconID!=0xFFFF){
						UI_Current=ID;
						return true;
					};
				};
			};
		};
	};
	Lpressed=false;
	return true;
};
bool UI_PrevBTN(SimpleDialog* SD){
	bool FOUND=0;
	int ID=-1;
	for(int i=0;i<NNations;i++){
		int N=NATIONS->NUnits[i];
		for(int j=0;j<N;j++){
			if(NATIONS->UnitsIDS[i][j]==UI_Current){
				if(ID==-1)return false;
				else{
					UI_Current=ID;
					return true;
				};
			};
			int ID2=NATIONS->UnitsIDS[i][j];
			if(ID2!=0xFFFF){
				GeneralObject* GO=NATIONS[0].Mon[ID2];
				if(GO->newMons->IconID!=0xFFFF){
					ID=ID2;
				};
			};
		};
	};
	Lpressed=false;
	return true;
};
void ClearUINF(){
	UINF.Close();
};
extern short WeaponIcn [32];
extern short ProtectIcn[32];
void GetCostString(byte NI,word* Cost,char* str){	
	if(!EngSettings.Resource.EnableSprHint){
		int zp=0;
		for(int i=0;i<6;i++){
			if(Cost[i]){
				if(Cost[i]>XRESRC(NI,i)){
					if(zp)sprintf(str,", {CR}%s: %d{C}",RDS[i].Name,Cost[i]);
					else sprintf(str,"{CR}%s: %d{C}",RDS[i].Name,Cost[i]);
				}else{
					if(zp)sprintf(str,", %s: %d",RDS[i].Name,Cost[i]);
					else sprintf(str,"%s: %d",RDS[i].Name,Cost[i]);
				}
				str+=strlen(str);
				zp++;
			};
		};
	}else{
		int cost[8];		
		for(int i=0;i<8;i++){
			cost[i]=Cost[i];
		}
		_str txt;
		EngSettings.Resource.SetHint(NI,cost,&txt);
		str[0]=0;
		if(txt.pchar())strcat(str,txt.pchar());
	}
};
CEXPORT void GetCostString(byte NI,int* Cost,char* str){	
	if(!EngSettings.Resource.EnableSprHint){
		int zp=0;
		for(int i=0;i<6;i++){
			if(Cost[i]){
				if(Cost[i]>XRESRC(NI,i)){
					if(zp)sprintf(str,", {CR}%s: %d{C}",RDS[i].Name,Cost[i]);
					else sprintf(str,"{CR}%s: %d{C}",RDS[i].Name,Cost[i]);
				}else{
					if(zp)sprintf(str,", %s: %d",RDS[i].Name,Cost[i]);
					else sprintf(str,"%s: %d",RDS[i].Name,Cost[i]);
				}
				str+=strlen(str);
				zp++;
			};
		};
	}else{
		int cost[8];		
		for(int i=0;i<8;i++){
			cost[i]=Cost[i];
		}
		_str txt;
		EngSettings.Resource.SetHint(NI,cost,&txt);
		str[0]=0;
		if(txt.pchar())strcat(str,txt.pchar());
	}
};
void GetCostString(word* Cost,char* str){
	int zp=0;
	for(int i=0;i<6;i++){
		if(Cost[i]){			
			if(zp)sprintf(str,", %s: %d",RDS[i].Name,Cost[i]);
			else sprintf(str,"%s: %d",RDS[i].Name,Cost[i]);
			str+=strlen(str);
			zp++;
		};
	};
};
void GetCostString(int* Cost,char* str){
	int zp=0;
	for(int i=0;i<6;i++){
		if(Cost[i]){
			if(zp)sprintf(str,", %s: %d",RDS[i].Name,Cost[i]);
			else sprintf(str,"%s: %d",RDS[i].Name,Cost[i]);
			str+=strlen(str);
			zp++;
		};
	};
};
char* THIS_UNIT=NULL;
int CreateMoraleInterface(DialogsSystem* DSS,int LX,int y0,OneObject* OB);
int LastUINF_Time=0;

int UIscrDX=-15;
int UIscrDY=2;
int UIscrDY1=-8;

int CreateUNITINF_UNIT(int ID,bool refresh){
#ifdef COSSACKS2
	if(refresh&&GetTickCount()-LastUINF_Time<700)return 0;
#else
	if(refresh&&GetTickCount()-LastUINF_Time<8000)return 0;
#endif
	if(refresh&&UI_PrmPage[0]!=7)return 0;
	if(!NATIONS->NMon)return 0;
	word ThisMID=0xFFFF;
	OneObject* ThisOB=NULL;
#ifndef NEWMORALE
	if(ImNSL[MyNation]&&ImSelm[MyNation]){
		word MID=ImSelm[MyNation][0];

		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo&&OB->NIndex==ID){
				ThisMID=MID;
				ThisOB=OB;
			};
		};
	};
#endif //NEWMORALE
	if(refresh&&!ThisOB)return 0;
	LastUINF_Time=GetTickCount();
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	UNITINF.CloseDialogs();
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	UINF.Create(ID);
	UNITINF.BaseX=x0-1;
	UNITINF.BaseY=y0+31;
	int dx=0;
	int dy=0;
	if(UINF.UnitType==3)dx=3;
	//if(UINF.UnitType==1)dy=-5;
	if(UINF.Picture)UNITINF.addBPXView(NULL,dx+1,dy-3,UINF.Lx,UINF.Ly,1,1,1,UINF.Picture,NULL);
	GPPicture* Panel;
	GP_TextButton* ThisUnit=NULL;
	GP_TextButton* General=NULL;
	GP_TextButton* Description=NULL;
	GP_TextButton* UnitUpgrades=NULL;
	GP_TextButton* Next=NULL;
	GP_TextButton* Previous=NULL;
	GP_TextButton* PriceUpg=NULL;
	RLCFont* FN1=&SmallWhiteFont;
	RLCFont* FN2=&SmallYellowFont;
	RLCFont* FN3=&SmallWhiteFont;
	RLCFont* FN4=&SmallYellowFont;
	RLCFont* FN5=&fn10;
	UNITINF.addTextButton(NULL,8,-29,UINF.Name,&fon16y1,&fon16y1,&fon16y1,0);
	UNITINF.HintX=-1;
	UNITINF.HintY=280;
	UNITINF.HintFont=&SmallBlackFont1;
	int dxx=0;
	int dyy=0;
	int onedy;
	switch(UINF.UnitType){
	case 0://Unit
		dxx=-1;
		dyy=1+UNI_LINEDY2;
		Panel=UNITINF.addGPPicture(NULL,0,-3,BordGP,19);
		onedy=22;

		if(ThisOB){
#ifndef NEWMORALE
			dyy+=2;
			onedy=20;
			//UNITINF.addGPPicture(NULL,115+dxx,dyy,BordGP,54);
			if(!THIS_UNIT)THIS_UNIT=GetTextByID("THIS_UNIT");
			ThisUnit=UNITINF.addGP_TextButton(NULL,115+dxx,dyy+2,THIS_UNIT,BordGP,55,FN1,FN2);
			dyy+=10;
#endif //NEWMORALE

		};

		//UNITINF.addGPPicture(NULL,112+dxx,11+dyy,BordGP,54);
		General=UNITINF.addGP_TextButton(NULL,115+dxx,11+dyy,BTN_General,BordGP,55,FN1,FN2);

		//UNITINF.addGPPicture(NULL,112+dxx,33+dyy,BordGP,54);
		Description=UNITINF.addGP_TextButton(NULL,115+dxx,11+onedy+dyy,BTN_Descr,BordGP,55,FN1,FN2);

		//UNITINF.addGPPicture(NULL,112+dxx,55+dyy,BordGP,54);
		UnitUpgrades=UNITINF.addGP_TextButton(NULL,115+dxx,11+onedy*2+dyy,BTN_Upgr,BordGP,55,FN1,FN2);

		//UNITINF.addGPPicture(NULL,112+dxx,77+dyy,BordGP,54);
		Next=UNITINF.addGP_TextButton(NULL,115+dxx,11+onedy*3+dyy,BTN_Next,BordGP,55,FN1,FN2);

		//UNITINF.addGPPicture(NULL,112+dxx,99+dyy,BordGP,54);
		Previous=UNITINF.addGP_TextButton(NULL,115+dxx,11+onedy*4+dyy,BTN_Prev,BordGP,55,FN1,FN2);
		break;
	case 1://Artillery
		dxx=3;
		dyy=-2+UNI_LINEDY2;
		Panel=UNITINF.addGPPicture(NULL,0,-3,BordGP,21);

		UNITINF.addGPPicture(NULL,137+dxx,5+dyy,BordGP,56);
		General=UNITINF.addGP_TextButton(NULL,137+dxx,5+dyy,BTN_General,BordGP,57,FN1,FN2);

		UNITINF.addGPPicture(NULL,137+dxx,27+dyy,BordGP,56);
		Description=UNITINF.addGP_TextButton(NULL,137+dxx,27+dyy,BTN_Descr,BordGP,57,FN1,FN2);

		UNITINF.addGPPicture(NULL,137+dxx,49+dyy,BordGP,56);
		UnitUpgrades=UNITINF.addGP_TextButton(NULL,137+dxx,49+dyy,BTN_Upgr,BordGP,57,FN1,FN2);

		UNITINF.addGPPicture(NULL,137+dxx,71+dyy,BordGP,56);
		Next=UNITINF.addGP_TextButton(NULL,137+dxx,71+dyy,BTN_Next,BordGP,57,FN1,FN2);

		UNITINF.addGPPicture(NULL,137+dxx,93+dyy,BordGP,56);
		Previous=UNITINF.addGP_TextButton(NULL,137+dxx,93+dyy,BTN_Prev,BordGP,57,FN1,FN2);
		break;
	case 2://Ship
		dxx=6;
		dyy=16+UNI_LINEDY2;
		Panel=UNITINF.addGPPicture(NULL,0,-3,BordGP,20);

		//UNITINF.addGPPicture(NULL,137+dxx,4+dyy,BordGP,56);
		General=UNITINF.addGP_TextButton(NULL,137+dxx,4+dyy,BTN_General,BordGP,94,FN1,FN2);

		//UNITINF.addGPPicture(NULL,137+dxx,26+dyy,BordGP,56);
		Description=UNITINF.addGP_TextButton(NULL,137+dxx,26+dyy,BTN_Descr,BordGP,94,FN1,FN2);

		//UNITINF.addGPPicture(NULL,137+dxx,48+dyy,BordGP,56);
		UnitUpgrades=UNITINF.addGP_TextButton(NULL,137+dxx,48+dyy,BTN_Upgr,BordGP,94,FN1,FN2);

		//UNITINF.addGPPicture(NULL,137+dxx,70+dyy,BordGP,56);
		Next=UNITINF.addGP_TextButton(NULL,137+dxx,70+dyy,BTN_Next,BordGP,94,FN1,FN2);

		//UNITINF.addGPPicture(NULL,137+dxx,93+dyy,BordGP,56);
		Previous=UNITINF.addGP_TextButton(NULL,137+dxx,93+dyy,BTN_Prev,BordGP,94,FN1,FN2);
		break;
	case 3://building
		Panel=UNITINF.addGPPicture(NULL,0,-4,BordGP,4);

		//UNITINF.addGPPicture(NULL,5,159,BordGP,54);
		Description=UNITINF.addGP_TextButton(NULL,5+7,158+UNI_LINEDY2,BTN_Descr,BordGP,57,FN1,FN2);

		//UNITINF.addGPPicture(NULL,124,159,BordGP,54);
		PriceUpg=UNITINF.addGP_TextButton(NULL,124+8,158+UNI_LINEDY2,BTN_PriceUpg,BordGP,57,FN1,FN2);

		//UNITINF.addGPPicture(NULL,124,182,BordGP,54);
		Next=UNITINF.addGP_TextButton(NULL,124+8,176+UNI_LINEDY2,BTN_Next,BordGP,57,FN1,FN2);

		//UNITINF.addGPPicture(NULL,5,182,BordGP,54);
		Previous=UNITINF.addGP_TextButton(NULL,5+7,176+UNI_LINEDY2,BTN_Prev,BordGP,57,FN1,FN2);
		break;
	};
	if(ThisUnit){
		ThisUnit->OnClick=&UI_PageSelect;
		ThisUnit->UserParam=7;
	};
	if(General){
		General->OnClick=&UI_PageSelect;
		General->UserParam=0;
	};
	if(Description){
		Description->OnClick=&UI_PageSelect;
		Description->UserParam=1;
	};
	if(UnitUpgrades){
		UnitUpgrades->OnClick=&UI_PageSelect;
		UnitUpgrades->UserParam=2;
	};
	if(PriceUpg){
		PriceUpg->OnClick=&UI_PageSelect;
		PriceUpg->UserParam=3;
	};
	if(Next){
		Next->OnClick=&UI_NextBTN;
	};
	if(Previous){
		Previous->OnClick=&UI_PrevBTN;
	};
	UINF.Y1Line=y0+GPS.GetGPHeight(BordGP,Panel->SpriteID)+29;
	int yy=GPS.GetGPHeight(BordGP,Panel->SpriteID)+16;
	int xx=13;
	SimpleDialog* SD;
	char cc[1000];
	int yy0=yy;
	UNITINF.addClipper(0,UINF.Y1Line+2,RealLx-1,InfY1);
	int YMAX=InfY1-UNITINF.BaseY;
	if(ThisUnit){
#ifndef NEWMORALE
		int minSD=0;
		for(minSD=0;minSD<UNITINF.DSS.GetAmount()&&UNITINF.DSS[minSD];minSD++);
		yy=CreateMoraleInterface(&UNITINF,230,yy,ThisOB);
		for(int i=minSD;i<UNITINF.DSS.GetAmount()&&UNITINF.DSS[i];i++)UINF.AddSD(UNITINF.DSS[i],7);
		int dd=yy-YMAX;
		if(dd>0){
			UNITINF.addClipper(0,0,RealLx-1,RealLy-1);
			VScrollBar* GLSB=UNITINF.addNewGP_VScrollBar(NULL,xx+220+UIscrDX,yy0-11+UIscrDY,InfY1-UINF.Y1Line+UIscrDY1-UIscrDY,dd,0,3,0);
			GLSB->OnesDy=10;
			UNITINF.addClipper(0,UINF.Y1Line+2,RealLx-1,InfY1);
			GLSB->ScrDy=100;
			for(int i=0;i<UINF.N_SD;i++)if(UINF.SDS[i].Page==7)UINF.SDS[i].SD->ParentSB=GLSB;
			UINF.AddSD(GLSB,7);
		};
		UI_PrmPage[0]=7;
#endif //NEWMORALE
	}else if(UI_PrmPage[0]==7)UI_PrmPage[0]=0;
	yy=yy0;
	if(General){
		//creating "General" page, price and upkeep information 
		SD=UNITINF.addTextButton(NULL,xx,yy,GetTextByID("UI_Price"),FN3,FN3,FN3,0);
		UINF.AddSD(SD,0);
		yy+=DY;
		for(int i=0;i<6;i++){
			SD=UNITINF.addGPPicture(NULL,xx+3+35*i,yy,BordGP,37+i);
			UINF.AddSD(SD,0);
		};
		yy+=DY+10;
		SD=UNITINF.addGPPicture(NULL,xx,yy,BordGP,35);
		UINF.AddSD(SD,0);
		for(int i=0;i<6;i++)if(UINF.Cost[i]){
			sprintf(cc,"%d",UINF.Cost[i]);
			SD=UNITINF.addTextButton(NULL,xx+17+34*i,yy+3,cc,FN5,FN5,FN5,1);
			UINF.AddSD(SD,0);
		};
		yy+=22;
		if(UINF.GoldUpkeep||UINF.IronUpkeep||UINF.CoalUpkeep||UINF.StoneUpkeep||UINF.WoodUpkeep){
			SD=UNITINF.addTextButton(NULL,xx,yy,GetTextByID("UI_Upkeep"),FN3,FN3,FN3,0);
			UINF.AddSD(SD,0);
			yy+=DY;
			if(UINF.GoldUpkeep){
				sprintf(cc,GetTextByID("UI_UGold"),UINF.GoldUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,0);
				yy+=DY;
			};
			if(UINF.WoodUpkeep){
				sprintf(cc,GetTextByID("UI_UWood"),UINF.WoodUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,0);
				yy+=DY;
			};
			if(UINF.StoneUpkeep){
				sprintf(cc,GetTextByID("UI_UStone"),UINF.StoneUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,0);
				yy+=DY;
			};
			if(UINF.IronUpkeep){
				sprintf(cc,GetTextByID("UI_UIron"),UINF.IronUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,0);
				yy+=DY;
			};
			if(UINF.CoalUpkeep){
				sprintf(cc,GetTextByID("UI_UCoal"),UINF.CoalUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,0);
				yy+=DY;
			};
		};
		sprintf(cc,GetTextByID("UI_STAGE"),UINF.BuildStages);
		SD=UNITINF.addTextButton(NULL,xx,yy,cc,FN3,FN3,FN3,0);
		UINF.AddSD(SD,0);
		yy+=DY;
		NewMonster* NM=NATIONS->Mon[UINF.UnitID]->newMons;
		if(NM->Capture){
			if(NM->Building){
				SD=UNITINF.addTextButton(NULL,xx,yy,GetTextByID("UI_BCAP"),FN3,FN3,FN3,0);
			}else{
				SD=UNITINF.addTextButton(NULL,xx,yy,GetTextByID("UI_CAPT"),FN3,FN3,FN3,0);
			};
			yy+=DY;
			UINF.AddSD(SD,0);
		};
		sprintf(cc,GetTextByID("UI_ACCNT"),float(NM->Ves)/100);
		SD=UNITINF.addTextButton(NULL,xx,yy,cc,FN3,FN3,FN3,0);
		UINF.AddSD(SD,0);
		yy+=DY;
		if(NM->Capture){
			sprintf(cc,"%s %s",GetTextByID("UI_ACHINT"),GetTextByID("UI_ACHCAP"));
			SD->Hint=UINF.AddHint(cc);
		}else SD->Hint=GetTextByID("UI_ACHINT");
		sprintf(cc,GetTextByID("STORM_I"),NM->StormForce);
		SD=UNITINF.addTextButton(NULL,xx,yy,cc,FN3,FN3,FN3,0);
		SD->Hint=GetTextByID("STORM_I_HINT");
		UINF.AddSD(SD,0);
		yy+=DY;
	};
	if(PriceUpg){
		SD=UNITINF.addTextButton(NULL,xx,yy,GetTextByID("UI_Price"),FN3,FN3,FN3,0);
		UINF.AddSD(SD,3);
		yy+=DY;
		for(int i=0;i<6;i++){
			SD=UNITINF.addGPPicture(NULL,xx+3+35*i,yy,BordGP,37+i);
			UINF.AddSD(SD,3);
		};
		yy+=24;
		SD=UNITINF.addGPPicture(NULL,xx,yy,BordGP,35);
		UINF.AddSD(SD,3);
		for(int i=0;i<6;i++)if(UINF.Cost[i]){
			sprintf(cc,"%d",UINF.Cost[i]);
			SD=UNITINF.addTextButton(NULL,xx+17+34*i,yy+3,cc,FN5,FN5,FN5,1);
			UINF.AddSD(SD,3);
		};
		yy+=22;
		if(UINF.GoldUpkeep||UINF.IronUpkeep||UINF.CoalUpkeep||UINF.StoneUpkeep||UINF.WoodUpkeep){
			SD=UNITINF.addTextButton(NULL,xx,yy,GetTextByID("UI_Upkeep"),FN3,FN3,FN3,0);
			UINF.AddSD(SD,3);
			yy+=DY;
			if(UINF.GoldUpkeep){
				sprintf(cc,GetTextByID("UI_UGold"),UINF.GoldUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,3);
				yy+=DY;
			};
			if(UINF.WoodUpkeep){
				sprintf(cc,GetTextByID("UI_UWood"),UINF.WoodUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,3);
				yy+=DY;
			};
			if(UINF.StoneUpkeep){
				sprintf(cc,GetTextByID("UI_UStone"),UINF.StoneUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,3);
				yy+=DY;
			};
			if(UINF.IronUpkeep){
				sprintf(cc,GetTextByID("UI_UIron"),UINF.IronUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,3);
				yy+=DY;
			};
			if(UINF.CoalUpkeep){
				sprintf(cc,GetTextByID("UI_UCoal"),UINF.CoalUpkeep);
				SD=UNITINF.addTextButton(NULL,xx+3,yy,cc,FN4,FN4,FN4,0);
				UINF.AddSD(SD,3);
				yy+=DY;
			};
		};
		sprintf(cc,GetTextByID("UI_STAGE"),UINF.BuildStages);
		SD=UNITINF.addTextButton(NULL,xx,yy,cc,FN3,FN3,FN3,0);
		UINF.AddSD(SD,3);
		yy+=DY;
		NewMonster* NM=NATIONS->Mon[UINF.UnitID]->newMons;
		if(NM->Capture){
			if(NM->Building){
				SD=UNITINF.addTextButton(NULL,xx,yy,GetTextByID("UI_BCAP"),FN3,FN3,FN3,0);
			}else{
				SD=UNITINF.addTextButton(NULL,xx,yy,GetTextByID("UI_CAPT"),FN3,FN3,FN3,0);
			};
			yy+=DY;
			UINF.AddSD(SD,3);
		};
		sprintf(cc,GetTextByID("UI_ACCNT"),float(NM->Ves)/100);
		SD=UNITINF.addTextButton(NULL,xx,yy,cc,FN3,FN3,FN3,0);
		UINF.AddSD(SD,3);
		yy+=DY;
		if(NM->Capture){
			sprintf(cc,"%s %s",GetTextByID("UI_ACHINT"),GetTextByID("UI_ACHCAP"));
			SD->Hint=UINF.AddHint(cc);
		}else SD->Hint=GetTextByID("UI_ACHINT");
		if(UINF.NMoreUpg){
			for(int i=0;i<UINF.NMoreUpg;i++){
				int xs=xx+(i%5)*42-5;
				int ys=yy+(i/5)*42;
				NewUpgrade* NU=NATIONS->UPGRADE[UINF.UPID[i]];
				if(NU->IconFileID!=0xFFFF){
					SD=UNITINF.addGPPicture(NULL,xs-1,ys-1,0,NU->IconSpriteID);
					UINF.AddSD(SD,3);
				};
				//SD=UNITINF.addGPPicture(NULL,xs,ys,BordGP,34);
				char* hnt=cc;
				sprintf(hnt,"%s/",NU->Message);
				hnt+=strlen(hnt);
				strcpy(hnt,GetTextByID("UI_Cost"));
				hnt+=strlen(hnt);
				GetCostString(NU->Cost,hnt);
				SD->Hint=UINF.AddHint(cc);
				//SD->Hint=NU->Message;
				UINF.AddSD(SD,3);
			};
			yy+=(UINF.NMoreUpg/5)*42;
			if(UINF.NMoreUpg%5)yy+=42;
		};
		//yy+=4;
		int dd=yy-YMAX;
		if(dd>0){
			UNITINF.addClipper(0,0,RealLx-1,RealLy-1);
			VScrollBar* GLSB=UNITINF.addNewGP_VScrollBar(NULL,xx+220+UIscrDX,yy0-11+UIscrDY,InfY1-UINF.Y1Line+UIscrDY1-UIscrDY,dd,0,3,0);
			GLSB->OnesDy=10;
			UNITINF.addClipper(0,UINF.Y1Line+2,RealLx-1,InfY1);
			GLSB->ScrDy=100;
			for(int i=0;i<UINF.N_SD;i++)if(UINF.SDS[i].Page==3)UINF.SDS[i].SD->ParentSB=GLSB;
			UINF.AddSD(GLSB,3);
		};
		sprintf(cc,GetTextByID("OPPOSIT_I"),NM->MinOposit,NM->MaxOposit);
		SD=UNITINF.addTextButton(NULL,xx,yy,cc,FN3,FN3,FN3,0);
		SD->Hint=GetTextByID("OPPOSIT_I_HINT");
		UINF.AddSD(SD,3);
		yy+=DY;
	};
	yy=yy0;
	if(UnitUpgrades){
		if(UINF.NMoreUpg){
			for(int i=0;i<UINF.NMoreUpg;i++){
				int xs=xx+(i%5)*42-5;
				int ys=yy+(i/5)*42;
				NewUpgrade* NU=NATIONS->UPGRADE[UINF.UPID[i]];
				if(NU->IconFileID!=0xFFFF){
					SD=UNITINF.addGPPicture(NULL,xs-1,ys-1,0,NU->IconSpriteID);
					UINF.AddSD(SD,2);
				};
				//SD=UNITINF.addGPPicture(NULL,xs,ys,BordGP,34);
				char* hnt=cc;
				sprintf(hnt,"%s/",NU->Message);
				hnt+=strlen(hnt);
				strcpy(hnt,GetTextByID("UI_Cost"));
				hnt+=strlen(hnt);
				GetCostString(NU->Cost,hnt);
				SD->Hint=UINF.AddHint(cc);
				UINF.AddSD(SD,2);
			};
			yy+=(UINF.NMoreUpg/5)*42;
			if(UINF.NMoreUpg%5)yy+=42;
		};
		int ady=2;
		if(UINF.NATT){
			int NAL=0;
			for(int j=0;j<8;j++)if(UINF.ATTMask&(1<<j))NAL++;
			if(NAL){
				//yy+=3;
				SD=UNITINF.addTextButton(NULL,xx,yy,GetTextByID("UI_AttUpg"),FN3,FN3,FN3,0);
				UINF.AddSD(SD,2);
				yy+=DY;
				SD=UNITINF.addGPPicture(NULL,xx,yy,BordGP,33);
				UINF.AddSD(SD,2);
				yy+=DY;
				int CAL=0;
				for(int j=0;j<8;j++)if(UINF.ATTMask&(1<<j)){
					if(CAL<NAL-1)SD=UNITINF.addGPPicture(NULL,xx,yy,BordGP,32);
					else SD=UNITINF.addGPPicture(NULL,xx,yy,BordGP,31);
					UINF.AddSD(SD,2);
					SD=UNITINF.addGPPicture(NULL,xx+1,yy-1,4,WeaponIcn[j]);
					UINF.AddSD(SD,2);
					sprintf(cc,"UI_SATT%d",j);
					SD=UNITINF.addViewPort(xx+14,yy,35,19);
					UINF.AddSD(SD,2);
					SD->Hint=GetTextByID(cc);
					sprintf(cc,"UI_ATT%d",j);
					SD=UNITINF.addViewPort(xx,yy,19,19);
					UINF.AddSD(SD,2);
					SD->Hint=GetTextByID(cc);
					for(int k=0;k<UINF.NATT;k++){
						if(UINF.ATTACK[k].AddValue[j]){
							sprintf(cc,"+%d",UINF.ATTACK[k].AddValue[j]);
							SD=UNITINF.addTextButton(NULL,xx+62+26*k,yy+ady,cc,FN5,FN5,FN5,1);
							UINF.AddSD(SD,2);
							SD=UNITINF.addViewPort(xx+49+26*k,yy,26,19);
							UINF.AddSD(SD,2);
							char* hnt=cc;
							char cc1[32];
							sprintf(cc1,"UI_ATT%d",j);
							sprintf(cc,"%s/%s%d/%s",GetTextByID(cc1),GetTextByID("UI_AttTo"),UINF.ATTACK[k].AttValue[j],GetTextByID("UI_Cost"));
							hnt+=strlen(hnt);
							GetCostString(UINF.ATTACK[k].Cost,hnt);
							hnt+=strlen(hnt);
							sprintf(hnt,"/%s",GetTextByID("UI_CostTo"));
							hnt+=strlen(hnt);
							GetCostString(UINF.ATTACK[k].CostTo,hnt);
							SD->Hint=UINF.AddHint(cc);
						};
					};
					sprintf(cc,"%d",UINF.CurATT[j]);
					SD=UNITINF.addTextButton(NULL,xx+31,yy+ady,cc,FN5,FN5,FN5,1);
					UINF.AddSD(SD,2);
					yy+=19;
					CAL++;
				};
				yy+=3;
			};
		};
		if(UINF.NSHL){
			int NAL=0;
			for(int j=0;j<8;j++)if(UINF.SHLMask&(1<<j))NAL++;
			if(NAL){
				SD=UNITINF.addTextButton(NULL,xx,yy+ady,GetTextByID("UI_ShlUpg"),FN3,FN3,FN3,0);
				UINF.AddSD(SD,2);
				yy+=DY;
				SD=UNITINF.addGPPicture(NULL,xx,yy,BordGP,33);
				UINF.AddSD(SD,2);
				yy+=DY-1;
				int CAL=0;
				for(int j=0;j<8;j++)if(UINF.SHLMask&(1<<j)){
					if(CAL<NAL-1)SD=UNITINF.addGPPicture(NULL,xx,yy,BordGP,32);
					else SD=UNITINF.addGPPicture(NULL,xx,yy,BordGP,31);
					UINF.AddSD(SD,2);
					SD=UNITINF.addGPPicture(NULL,xx+1,yy-1,4,ProtectIcn[j]);
					UINF.AddSD(SD,2);
					sprintf(cc,"UI_SSHL%d",j);
					SD=UNITINF.addViewPort(xx+14,yy,35,19);
					UINF.AddSD(SD,2);
					SD->Hint=GetTextByID(cc);
					sprintf(cc,"UI_SHL%d",j);
					SD=UNITINF.addViewPort(xx,yy,19,19);
					UINF.AddSD(SD,2);
					SD->Hint=GetTextByID(cc);
					for(int k=0;k<UINF.NSHL;k++){
						if(UINF.SHIELD[k].AddValue[j]){
							sprintf(cc,"+%d",UINF.SHIELD[k].AddValue[j]);
							SD=UNITINF.addTextButton(NULL,xx+62+26*k,yy+ady,cc,FN5,FN5,FN5,1);
							UINF.AddSD(SD,2);
							SD=UNITINF.addViewPort(xx+49+26*k,yy,26,19);
							UINF.AddSD(SD,2);
							char* hnt=cc;
							char cc1[32];
							sprintf(cc1,"UI_SHL%d",j);
							sprintf(cc,"%s/%s%d/%s",GetTextByID(cc1),GetTextByID("UI_ShlTo"),UINF.SHIELD[k].AttValue[j],GetTextByID("UI_Cost"));
							hnt+=strlen(hnt);
							GetCostString(UINF.SHIELD[k].Cost,hnt);
							hnt+=strlen(hnt);
							sprintf(hnt,"/%s",GetTextByID("UI_CostTo"));
							hnt+=strlen(hnt);
							GetCostString(UINF.SHIELD[k].CostTo,hnt);
							SD->Hint=UINF.AddHint(cc);
						};
					};
					sprintf(cc,"%d",UINF.CurSHL[j+1]);
					SD=UNITINF.addTextButton(NULL,xx+31,yy+ady,cc,FN5,FN5,FN5,1);
					UINF.AddSD(SD,2);
					yy+=19;
					CAL++;
				};
				yy+=3;
			};
		};
		yy+=4;
		int dd=yy-YMAX;
		if(dd>0){
			UNITINF.addClipper(0,0,RealLx-1,RealLy-1);
			VScrollBar* GLSB=UNITINF.addNewGP_VScrollBar(NULL,xx+220+UIscrDX,yy0-11+UIscrDY,InfY1-UINF.Y1Line+UIscrDY1-UIscrDY,dd,0,3,0);
			GLSB->OnesDy=10;
			UNITINF.addClipper(0,UINF.Y1Line+2,RealLx-1,InfY1);
			GLSB->ScrDy=100;
			for(int i=0;i<UINF.N_SD;i++)if(UINF.SDS[i].Page==2)UINF.SDS[i].SD->ParentSB=GLSB;
			UINF.AddSD(GLSB,2);
		};
	};
	yy=yy0;
	if(Description){
		SD=UNITINF.addTextButton(NULL,xx-4,yy-3,GetTextByID("BTN_Descr"),&SmallWhiteFont,&SmallWhiteFont,&SmallWhiteFont,0);
		UINF.AddSD(SD,1);
		sprintf(cc,"Description\\%s.md",NATIONS->Mon[UINF.UnitID]->newMons->MD_File);
		TextViewer* TV=UNITINF.addTextViewer(NULL,xx-4,yy-3+16,205,InfY1-UINF.Y1Line-16-16,cc,&SmallYellowFont);
		TV->SymSize+=2;
		TV->PageSize=(TV->y1-TV->y)/TV->SymSize;
		UINF.AddSD(TV,1);
		VScrollBar* SB=UNITINF.addNewGP_VScrollBar(NULL,xx+220+UIscrDX,yy0-11+UIscrDY,InfY1-UINF.Y1Line+UIscrDY1-UIscrDY,1,0,3,0);
		UINF.AddSD(SB,1);
		TV->AssignScroll(SB);

	};
	//UNITINF.addClipper(0,0,RealLx-1,RealLy-1);
	int cpg=UI_PrmPage[UINF.UnitType];
	for(int i=0;i<UINF.N_SD;i++)UINF.SDS[i].SD->Visible=UINF.SDS[i].Page==cpg;
	return true;
};
void ShowUnitInfo(){
	if(UINF.N_SD){
		int cpg=UI_PrmPage[UINF.UnitType];
		for(int i=0;i<UINF.N_SD;i++)UINF.SDS[i].SD->Visible=UINF.SDS[i].Page==cpg;
		if(UI_Current!=UINF.UnitID)CreateUNITINF_UNIT(UI_Current,0);
		else CreateUNITINF_UNIT(UI_Current,1);
		DrawScrollDoubleTable(RealLx-InfDX,InfDY,RealLx-10-5,InfY1,UINF.Y1Line);
		//bool L=Lpressed;
		//if(!Allow)Lpressed=false;
		TempWindow TW;
		PushWindow(&TW);
		UNITINF.ProcessDialogs();
		PopWindow(&TW);
		bool closeit=0;
		if(Lpressed){
			if(mouseX>RealLx-32-10&&mouseY>InfAddY+32+8-13&&mouseX<RealLx-32-10+26&&mouseY<InfAddY+32+8-13+26){
				closeit=1;
			};
		};
		if(closeit){
			Inform=0;
			InfDX=InfDX0;
		};
		//Lpressed=L;
	}else{
		CreateUNITINF_UNIT(0,0);
	};
};