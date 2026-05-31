#include "stdheader.h"
#include "UnitsInterface.h"
//#include "vui_info.h"
CEXPORT void VitalError(char*);
//================================================================================================================//


xmlQuote SuperTest;
//================================================================================================================//
// -
extern int FI_File;
void InitGlobalFIVar();
extern int RealLx;
extern int RealLy;
extern byte BaloonState;
extern byte CannonState;
extern byte XVIIIState;
CIMPORT int GetSUBorderFileID();
void PerformUpgradeLink(OneObject*);
void GetUpgCost(byte NI, word UIndex, word* &Cost);
void GetCostString(word*,char*);
extern City CITY[8];
extern bool EditMapMode;
extern byte PlayGameMode;
int GetAmount(word);
int GetProgress(word,int*);
void UnPress();
bool CheckCostHint(byte,word);
void GetMonCost(byte,word,char*);
void DoUPG1(int i);
void PerformUpgradeLink(OneObject*);
void CmdChangeNPID(byte,word);
void CmdUnProduceObj(byte,word);
extern bool BuildMode;
extern word BuildingID;
extern Nation* BNat;
extern "C" CEXPORT int GetMaxUnits(byte NI);
extern "C" CEXPORT int GetCurrentUnits(byte NI);
CIMPORT byte GetTribeOwner(word Index);
CIMPORT void GetTribeGoods(word Index, byte& Resource, byte& NPeasants, byte& NPMax);
extern int FI_ResFile;
extern word NPlayers;
CIMPORT int GetSetlFlagFileID();
extern char* FI_NetralName;
DWORD GetNatColor(int);
// -
//================================================================================================================//
ADSStruct::ADSStruct()
{
	SelectedElement=-1;
	IsMenuInit=false;
	Active=false;
}
void ADSStruct::InitMenu(){
	TextButton* loa=(TextButton*)Menu.Find(5);
	TextButton* sav=(TextButton*)Menu.Find(6);
	TextButton* add=(TextButton*)Menu.Find(1);
	TextButton* sel=(TextButton*)Menu.Find(2);
	TextButton* pro=(TextButton*)Menu.Find(3);
	TextButton* del=(TextButton*)Menu.Find(4);
	if(!loa){
		RLCFont* af=&BlackFont;
		RLCFont* pf=&YellowFont;
		int x=100, y=60, dy=20;
		loa=Menu.addTextButton(NULL,x,y,"Load",af,pf,pf,1);				loa->AssignID(5);	loa->OnUserClick=&ADSStruct::OnClick;
		sav=Menu.addTextButton(NULL,x,y+dy,"Save",af,pf,pf,1);			sav->AssignID(6);	sav->OnUserClick=&ADSStruct::OnClick;
		add=Menu.addTextButton(NULL,x,y+dy*3,"Add",af,pf,pf,1);			add->AssignID(1);	add->OnUserClick=&ADSStruct::OnClick;
		sel=Menu.addTextButton(NULL,x,y+dy*4,"Select",af,pf,pf,1);		sel->AssignID(2);	sel->OnUserClick=&ADSStruct::OnClick;
		pro=Menu.addTextButton(NULL,x,y+dy*5,"Property",af,pf,pf,1);	pro->AssignID(3);	pro->OnUserClick=&ADSStruct::OnClick;
		del=Menu.addTextButton(NULL,x,y+dy*6,"Delete",af,pf,pf,1);		del->AssignID(4);	del->OnUserClick=&ADSStruct::OnClick;

		// select sprite
		//LocalGP CMB("Interf2\\Combo");
		//ComboBox* ss=Menu.addGP_ComboBoxDLX(NULL,200,60,100,CMB.GPID,0,8,0,af,pf,NULL);
	}
}
// on DS click functions
bool SelectASD(SimpleDialog* SD){	
	ADSStruct::SelectedElement=SD->ID;
	Lpressed=false;
	return 0;
}
bool ADSStruct::OnClick(SimpleDialog* SD){
	if(SD->ID==1){
		// add
		GP_Button* but=DS.addGP_Button(NULL,100,100,0,0,0);
		but->UserParam=0;
		but->AssignID(DS_NID);
		SelectedElement=DS_NID;
		DS_NID++;

		Command=1;
		LocalGP gp("interf3\\units_fra");
		SelPict.Init(gp.GPID);		
	}else
	if(SD->ID==2){
		// select
		for(int i=0;i<DS.DSS.GetAmount();i++){
			SimpleDialog* SD=DS.DSS[i];
			if(SD&&SD->Enabled){
				SD->OnUserClick=SelectASD;
			}
		}
		Command=-1;
	}else
	if(SD->ID==3){
		// property
		Command=1;
		LocalGP gp("interf3\\units_fra");
		SelPict.Init(gp.GPID);		
	}else
	if(SD->ID==5){
		LoadScreenFromFile("Interface\\testscreen.xml", &DS);

		
		SuperTest.ReadFromFile("Interface\\testscreen.xml");

		// Load
		//LoadScreenFromFile("Interf3\\test.xml", &DS);
		Command=-1;
	}
	else
	if(SD->ID==6){
		// Save
		SaveScreenToFile("Interf3\\test.xml", &DS);
		SuperTest.WriteToFile("Interf3\\test.xml");
		Command=-1;
	}
	Lpressed=false;
	return 0;
};
void ADSStruct::ProcessCommands(){
	if(Command==1){
		// property
		SelPict.Process();
		if(SelPict.sid!=-1){
			SimpleDialog* SD=DS.Find(SelectedElement);
			if(SD){
				if(SD->UserParam==0){ // GP_Button					
					GP_Button* but=(GP_Button*)SD;
					but->GP_File=SelPict.fid;
					but->ActiveFrame=SelPict.sid;
					but->PassiveFrame=but->ActiveFrame;
				}
				Command=-1;
			}else{
				VitalError("ADSStruct::ProcessCommands, add");
			}
		}
	}else{
		// move
		SimpleDialog* SD=DS.Find(SelectedElement);
		if(SD){
			if(GetKeyState(VK_RIGHT)&0x8000){
				SD->x+=10;
				SD->x1+=10;
			}else
			if(GetKeyState(VK_LEFT)&0x8000){
				SD->x-=10;
				SD->x1-=10;
			}else
			if(GetKeyState(VK_DOWN)&0x8000){
				SD->y+=10;
				SD->y1+=10;
			}else
			if(GetKeyState(VK_UP)&0x8000){
				SD->y-=10;
				SD->y1-=10;
			}else
			if(GetKeyState(VK_HOME)&0x8000){
				int lx=SD->x1-SD->x;
				int ly=SD->y1-SD->y;
				SD->x=100;
				SD->x1=100+lx;
				SD->y=100;
				SD->y1=100+ly;
			}
		}
		//SelPict.Enable=0;
	}
	// Show transparent not selected
	for(int i=0;i<DS.DSS.GetAmount();i++){
		SimpleDialog* SD=DS.DSS[i];
		if(SD&&SD->Enabled){
			if(SD->ID==ADSStruct::SelectedElement) SD->Diffuse=0xFFFFFFFF;
				else SD->Diffuse&=0x7FFFFFFF;
		}
	}
}
void ADSStruct::DrawProcess()
{	
	if(Active)
	{
		InitMenu();
		DS.ProcessDialogs();
		if(!(GetKeyState(VK_MENU)&0x8000)){
			ProcessCommands();
			DrawPaperPanel(50,50,150,240);
			Menu.ProcessDialogs();
		}
	}
}
void ADSStruct::LoadScreenFromFile(char *Fn, DialogsSystem* S)
{
	DString Fl;
	Fl.ReadFromFile(Fn);
	if(Fl.L)
	{
		DString ElmList;
		if(GetMiddleFrame(&Fl,"screen",&ElmList))
		{	
			DString Elm;
			int Pos=0;
			int nsz=GetMiddleFrame(&ElmList,"elm",&Elm);
			while(nsz)
			{
				DString Type;
				if(GetMiddleFrame(&Elm,"type",&Type))
				{
					if(strstr(Type.pchar(),"GPButton"))
					{
						DString gpfile;
						int id=0;
						int activeframe=0;
						int x=100;
						int y=100;
						DString tmp;

						if(GetMiddleFrame(&Elm,"id",&tmp))
							id=atoi(tmp.pchar());
						if(GetMiddleFrame(&Elm,"activeframe",&tmp))
							activeframe=atoi(tmp.pchar());
						if(GetMiddleFrame(&Elm,"x",&tmp))
							x=atoi(tmp.pchar());
						if(GetMiddleFrame(&Elm,"y",&tmp))
							y=atoi(tmp.pchar());

						if(GetMiddleFrame(&Elm,"gpfile",&gpfile))
						{
							LocalGP GP0(gpfile.pchar());
							GP_Button* but=S->addGP_Button(NULL,x,y,GP0.GPID,activeframe,activeframe);
							if(but)
							{
								if(id)
									but->AssignID(id);
								but->UserParam=0;//GP_Button Type
								
								if(GetMiddleFrame(&Elm,"onclick",&tmp))
									but->OnUserClick=GetFunctionByName(tmp.pchar(),&OnUserClick);
							}

						}
						
					}
				}
				ElmList.DeleteChars(0,nsz);
				nsz=GetMiddleFrame(&ElmList,"elm",&Elm);
			}
		}
	}
}
int ADSStruct::GetMiddleFrame(DString *From, char *Frame, DString *Rez)
{
	int nb=0;
	Rez->Clear();
	DString St;
	DString En;
	St.Add("<");
	St.Add(Frame);
	St.Add(">");
	En.Add("</");
	En.Add(Frame);
	En.Add(">");
	char* f=strstr(From->pchar(),St.pchar()); 
	char* e=NULL;
	if(f)
		e=strstr(f,En.pchar()); 
	if(f&&e)
	{
		int len=e-f-St.L;
		Rez->Allocate(len);
		if(len>0)
		{
			memmove(Rez->pchar(),f+St.L,len);
			Rez->pchar()[len]='\0';
			Rez->L=len;
			nb=e-From->pchar()+En.L;
		}
	}

	return nb;
}
void ADSStruct::SaveScreenToFile(char *f, DialogsSystem *S)
{
	DString XML;
	if(S&&f)
	{
		int tab=0;
		XML.Add("<screen>\n");
		tab++;
		int j;
		for(int i=0;i<DS.DSS.GetAmount();i++)
		{
			if(DS.DSS[i]&&DS.DSS[i]->Enabled)
			{
				if(DS.DSS[i]->UserParam==0)////GP_Button Type
				{
					GP_Button* but=(GP_Button*)(DS.DSS[i]);
					for(j=0;j<tab;j++,XML.Add(" "));XML.Add("<elm>\n");tab++;
					 for(j=0;j<tab;j++,XML.Add(" "));XML.Add("<type>GPButton</type>\n");
					 for(j=0;j<tab;j++,XML.Add(" "));XML.Add("<id>");XML.Add((int)but->ID);XML.Add("</id>\n");
					 for(j=0;j<tab;j++,XML.Add(" "));XML.Add("<gpfile>");XML.Add((char*)GPS.GetGPName(but->GP_File));XML.Add("</gpfile>\n");
					 for(j=0;j<tab;j++,XML.Add(" "));XML.Add("<activeframe>");XML.Add(but->ActiveFrame);XML.Add("</activeframe>\n");
					 for(j=0;j<tab;j++,XML.Add(" "));XML.Add("<x>");XML.Add(but->x);XML.Add("</x>\n");
					 for(j=0;j<tab;j++,XML.Add(" "));XML.Add("<y>");XML.Add(but->y);XML.Add("</y>\n");
					 if(but->OnUserClick)
						 for(j=0;j<tab;j++,XML.Add(" "));XML.Add("<onclick>");XML.Add(GetNameByFunction(but->OnUserClick,&OnUserClick));XML.Add("</onclick>\n");
					tab--;for(j=0;j<tab;j++,XML.Add(" "));XML.Add("</elm>\n");
				}
			}
		}
		XML.Add("</screen>\n");
		tab--;//if( tab==0 ) OK
		XML.WriteToFile(f);
	}
}
VCall* ADSStruct::GetFunctionByName(char* fname, DynArray<FunctionsList> *FL)
{
	for(int i=0;i<FL->GetAmount();i++)
		if(strstr((*FL)[i].FName,fname))
			return (*FL)[i].Function;
	return NULL;
}
char* ADSStruct::GetNameByFunction(VCall* ff, DynArray<FunctionsList> *FL)
{
	for(int i=0;i<FL->GetAmount();i++)
		if((*FL)[i].Function==ff)
			return (*FL)[i].FName;
	return "";
}

DialogsSystem ADSStruct::DS;
DialogsSystem ADSStruct::Menu;
int ADSStruct::SelectedElement=-1;
int ADSStruct::DS_NID=1;
int ADSStruct::Command=-1;
CSelPicture ADSStruct::SelPict;
int CSelPicture::fid=-1;
int CSelPicture::sid=-1;
DynArray<FunctionsList> ADSStruct::OnUserClick;
//================================================================================================================//
void CSelPicture::Init(int FileID){
	DS.CloseDialogs();
	fid=FileID;
	sid=-1;

	x=200;
	y=80;
	Lx=200;
	Ly=500;

	int y1=y+Ly;
	int yy=y;

	//LocalGP gp;
	int n=GPS.GPNFrames(fid);
	for(int i=0;i<n&&yy<y1;i++){
		GP_Button* but=DS.addGP_Button(NULL,x,yy,fid,i,i);
		but->UserParam=i;
		but->OnUserClick=&CSelPicture::OnClick;
		yy+=GPS.GetGPHeight(fid,i);
	}
	//gp.
}
void CSelPicture::Process(){
	//if(Enable) 
		DS.ProcessDialogs();
}

ADSStruct DVS;
void ShowAlexVisualSystem(){	
	DVS.DrawProcess();
	/*
	if(DVS.Active)
	{
		// init
		GP_Button* but=(GP_Button*)DVS.DS.Find(1);
		if(!but){
			LocalGP GP0("interf3\\units_fra");
			but=DVS.DS.addGP_Button(NULL,200,300,GP0.GPID,0,0);
			but->AssignID(1);
		}	
		// show
		DVS.DS.ProcessDialogs();
	}
	*/
};
//================================================================================================================//
CIMPORT void GetTribeDebugInfo(word Index, int& NCop, int& NOwner);
void LoadScreen(char* f, DialogsSystem* S)
{
	DVS.LoadScreenFromFile( f, S);
}
CEXPORT
bool RegisterOnClickFunction(char* Identf, VCall*  Fun)
{
	bool rez=true;
	for(int i=0;i<DVS.OnUserClick.GetAmount();i++)
	{
		if(strstr(Identf,DVS.OnUserClick[i].FName))
			rez=false;
	}
	if(rez)
	{
		FunctionsList tm;
		strcpy(tm.FName,Identf);
		tm.Function=Fun;
		DVS.OnUserClick.Add(tm);
	}
	return rez;
}
// --- AlexResource
int AlexResource::GetPropValueSize(void* value, PropType type) const {
	switch(type){
		case Integer: return sizeof(int);
		case String: return strlen((char*)value)+1;
	};
}

AlexResource::~AlexResource(){
	int n=Name.GetAmount();
	for(int i=0;i<n;i++){
		free(Name[i]);
		free(Value[i]);
	}

};

bool AlexResource::RegisterProp(void* value, char* name,PropType type){
	char* _name;
	void* _value;
	
	_name=(char*)malloc( strlen(name)+1 );
	strcpy(_name,name);
	_value=malloc( GetPropValueSize(value,type) );

	Name.Add(_name);
	Type.Add(type);
	Value.Add(_value);
	
	return true;
}

int AlexResource::GetNProp() const {
	return Name.GetAmount();
}

//virtual 
char* AlexResource::GetPropName(int id) const {
	char* x=Name[id];
	return x;
};
PropType AlexResource::GetPropType(int id) const {
	return Type[id];
};
void* AlexResource::GetPropValue(int id) const {
	return Value[id];
};
bool AlexResource::SetPropValue(int id, void* value) {
	switch(Type[id]){
	case Integer: 
		*((int*)Value[id])=*((int*)value);
		return true;
	case String:
		char* src=(char*)value;
		char* dst=(char*)Value[id];
		src=(char*)realloc(dst,min(strlen(src),strlen(dst))+1);
		strcpy(dst,src);
		Value[id]=dst;
		return true;
	};
	return false;
};

int AlexResource::GetPropID(char* name) const {
	int n=GetNProp();
	for(int i=0;i<n;i++){
		if(!strcmp(Name[i],name)){
			return i;
		}
	}
	return -1;
};
PropType AlexResource::GetPropType(char* name) const {
	return GetPropType(GetPropID(name));
};
void* AlexResource::GetPropValue(char* name) const {
	return GetPropValue(GetPropID(name));
};
bool AlexResource::SetPropValue(char* name, void* value) {
	return SetPropValue(GetPropID(name),value);
};

// --- GP_Button


// Show units info panels
OneObject* OB=NULL;
GeneralObject* GO=NULL;
NewMonster* NM=NULL;
int NOB=0;
int bs;	// border sprite
int uf; // unit icon file
int us;
int udx;
int udy;
int bLx;
int bLy;
int style;
void AddBaseInfoPanel(DialogsSystem* DS){

	DS->SetHintStyle(0,1,0,0,0,0,bLx+4,RealLy-bLy+17,400,200,&SmallWhiteFont,0xFFFFFFFF,0);

	GPPicture* GPP=(GPPicture*)DS->Find(1024);
	GPPicture* GBI=(GPPicture*)DS->Find(1025);

	GPP=DS->addGPPicture(NULL,0,-bLy,FI_File,bs);
	GPP->AssignID(1024);
	GPP->ShapeFileID=FI_File;
	GPP->ShapeSpriteID=bs;

	udx=25;
	udy=47;
	GBI=DS->addGPPicture(NULL,udx,udy-bLy,uf,us);
	GBI->AssignID(1025);
	//GBI->Visible=false;

	RLCFont* Font=&SmallYellowFont;
	if(NOB>1){
		char txt[32];
		itoa(NOB,txt,10);
		DS->addTextButton(NULL,bLx/2,3-bLy,txt,Font,Font,Font,1);
	}
}

void AddKriInfoPanel(DialogsSystem* DS){

	if(NOB>1) bs=13;	// border sprite	
		else bs=29;

	if(NM->BigIconFile!=0xFFFF){
		uf=NM->BigIconFile;
		us=NM->BigIconIndex;
	}else{
		uf=NM->IconFileID;
		us=NM->IconID;
	}

	udx=21;
	udy=43;

	bLx=GPS.GetGPWidth(FI_File,bs);
	bLy=GPS.GetGPHeight(FI_File,bs);

	AddBaseInfoPanel(DS);

	//ShowAString(CenterX+dx,RealLy-FI_PortretLY+20,Font,1,"%s",NM->Message);
	RLCFont* Font=&SmallYellowFont;
	TextButton* TB=DS->addTextButton(NULL,bLx/2,21-bLy,NM->Message,Font,Font,Font,1);
    TB->Hint=NM->GetLongMessage();
}

void AddBldInfoPanel(DialogsSystem* DS){
	
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];

	bs=26;	// border sprite	
	
	if(NM->BigIconFile!=0xFFFF){
		uf=NM->BigIconFile;
		us=NM->BigIconIndex;
		udx=25;//21;
		udy=47;//43;
	}else{
		uf=FI_File;
		us=33;
		udx=25;
		udy=47;
		/*
		uf=NM->IconFileID;
		us=NM->IconID;
		*/
	}	

	bLx=GPS.GetGPWidth(FI_File,bs);
	bLy=GPS.GetGPHeight(FI_File,bs);

	AddBaseInfoPanel(DS);

	//ShowAString(CenterX+dx,RealLy-FI_PortretLY+20,Font,1,"%s",NM->Message);
	RLCFont* Font=&SmallYellowFont;
	DS->addTextButton(NULL,bLx/2,21-bLy,NM->Message,Font,Font,Font,1);

	char txt[128];

 	Font=&SmallBlackFont;
	sprintf(txt,"Life: %d/%d",OB->Life,OB->MaxLife);
	DS->addTextButton(NULL,bLx/2,216-bLy,txt,Font,Font,Font,1);

	if(NM->NInFarm){
		sprintf(txt,"Living places: %d",NM->NInFarm);
 		DS->addTextButton(NULL,bLx/2,235-bLy,txt,Font,Font,Font,1);
	}

	sprintf(txt,"Population: %d/%d",GetCurrentUnits(NI),GetMaxUnits(NI));
	DS->addTextButton(NULL,bLx/2,266-bLy,txt,Font,Font,Font,1);

}
//
CEXPORT char* GetPlayerName(byte NI);
//
void AddStlInfoPanel(DialogsSystem* DS){

	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];

	bs=26;	// border sprite	
	
	if(NM->BigIconFile!=0xFFFF){
		uf=NM->BigIconFile;
		us=NM->BigIconIndex;
		udx=21;
		udy=43;
	}else{
		uf=FI_File;
		us=33;
		udx=25;
		udy=47;
	}	

	bLx=GPS.GetGPWidth(FI_File,bs);
	bLy=GPS.GetGPHeight(FI_File,bs);
	//
	byte res,npeas,npmax;
	GetTribeGoods(OB->Stuff,res,npeas,npmax);
	//
	static int BldIc=GPS.PreLoadGPImage("interf3\\build");
	uf=BldIc;
	us=81;
	switch(res){
		case 1: us=81; break;
		case 3: us=83; break;
		case 4: us=82; break;
		case 5: us=84; break;
	};
	//
	AddBaseInfoPanel(DS);
	//
	uf=FI_File;
	us=33;
	//
	RLCFont* Font=&SpecialYellowFont;	
	RLCFont* RFont=&SpecialBlackFont;

	static char* tSettlement=GetTextByID("Setl_Settlement");
	DS->addTextButton(NULL,bLx/2,21-bLy,tSettlement,Font,Font,Font,1);

	Font=&SpecialBlackFont;
	
	int yy=216-bLy;
	static char* tGoods=GetTextByID("Setl_Goods");
 	DS->addTextButton(NULL,50,yy,tGoods,Font,Font,Font,0);	
 	DS->addGPPicture(NULL,97,yy-13,FI_ResFile,10+res);

	char txt[128];
	yy=236-bLy;
	static char* tWorkers=GetTextByID("Setl_Workers");
	DS->addTextButton(NULL,40,yy,tWorkers,Font,Font,Font,0);
	sprintf(txt,"%d/%d",npeas,npmax);	
	DS->addTextButton(NULL,120,yy+2,txt,RFont,RFont,RFont,1); //115
	
	yy=266-bLy;	
	static char* tOwner=GetTextByID("Setl_Owner");
  	DS->addTextButton(NULL,37,yy,tOwner,Font,Font,Font,0);	
	byte nat=GetTribeOwner(OB->Stuff);
	_str stName;
	if(nat==7){
		stName=FI_NetralName;
	}else{	
		/*
		for(int i=0;i<7;i++){ //NPlayers
			if(GSets.CGame.PL_INFO[i].ColorID==nat){
				stName=GSets.CGame.PL_INFO[i].name;				
				break;
			}
		}
		*/
		stName=GetPlayerName(nat);
	}
	if(!stName.isClear()){
		int l=strlen(stName.pchar());
		if(l>8){
			stName.Allocate(9);
			//stName.pchar()[6]='.';
			//stName.pchar()[7]='.';
			//stName.pchar()[8]='.';
			stName.pchar()[6]=0;
		}
		DS->addTextButton(NULL,98-5,yy+2,stName.pchar(),&SmallBlackFont,&SmallBlackFont,&SmallBlackFont,0);
	}	
	// flag
 	//GPPicture* pic=DS->addGPPicture(NULL,78,yy-2,GetSetlFlagFileID(),0);
	//pic->Diffuse=GetNatColor(nat);

	/*
	// Debug
	Font=&SmallYellowFont;
	int cop,owner;
	GetTribeDebugInfo(OB->Stuff,cop,owner);
	sprintf(txt,"Cops amount: %d",cop);
	DS->addTextButton(NULL,10,-60-bLy,txt,Font,Font,Font,0);
	sprintf(txt,"Owner units amount: %d",owner);
	DS->addTextButton(NULL,10,-44-bLy,txt,Font,Font,Font,0);
	*/
}

void AddObzInfoPanel(DialogsSystem* DS){
}

// Show units command panels
bool PrepareToConstruct(SimpleDialog* SD){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(CheckCostHint(NI,SD->UserParam)){
		BuildMode=true;
		BuildingID=SD->UserParam;
		BNat=NATIONS+MyNation;
	};
	UnPress();
	return 0;
}

bool ProduceUnit(SimpleDialog* SD){
	//GetKeyState()
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(CheckCostHint(NI,SD->UserParam)){
		CmdProduceObj(NI,SD->UserParam);
	}
	UnPress();
	return 0;
}

bool UnProduceUnit(SimpleDialog* SD){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	CmdUnProduceObj(NI,SD->UserParam);
	UnPress();
	return 0;
}

bool RunUpgrade(SimpleDialog* SD){
	//DoUPG1(SD->UserParam);
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	CmdPerformUpgrade(NI,SD->UserParam);
	UnPress();
	return 0;
}
void ShowKriComPanel(DialogsSystem* DS){

	int spr=22;

	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];	
	Nation* Nat=NATIONS+NI;
	word NIndex=OB->NIndex;

	int NBuild=Nat->PACount[NIndex];
	word* Build=Nat->PAble[NIndex];
	char* AIndex=Nat->AIndex[NIndex];

	int dx=GPS.GetGPWidth(FI_File,spr)+2;
	int dy=GPS.GetGPHeight(FI_File,spr)+2;
	
	OneObject* OBJ=OB;

	if(NM->Peasant){
		char NATID[5];
		int NPID=CITY[NI].NationalPeasantID;
		if(NPID!=0xFFFF&&!PlayGameMode){
			char* S=NATIONS->Mon[NPID]->MonsterID;
			int L=strlen(S);
			strcpy(NATID,S+L-4);
		}else{
			NATID[0]=0;
		};
		bool CANDO1=1;
		if(NATID[0]){
			CANDO1=strstr(OBJ->Ref.General->MonsterID,NATID);
		};

		bool OKK=1;
		if(OBJ&&OBJ->newMons->Peasant&&!(EditMapMode||PlayGameMode)){
			if(NPID==0xFFFF){
				CITY[NI].NationalPeasantID=OBJ->NIndex;
				CmdChangeNPID(NI,OBJ->NIndex);
			}else{
				if(NPID!=OBJ->NIndex)OKK=0;
			};
		};
		bool CANDO=1;
		if(NATID[0]){
			CANDO=CANDO1;
		};
		if(!(OKK&&CANDO&&OBJ->Ready)) return;
	}

	for(int i=0;i<NBuild;i++,Build++,AIndex++){
		GeneralObject* go=Nat->Mon[*Build];
		
		if(!go->Enabled) continue;
		
		bool okk=1;
		if(go->StageMask){
			byte m=go->StageMask;
			word s=OBJ->StageState;
			for(int i=0;i<5;i++){
				if(m&1){
					byte s1=s&7;
					if(s1!=2)okk=0;
				};
				m>>=1;
				s>>=3;
			};
		};
		if(!okk) continue;	
		
		NewMonster* bm=go->newMons;
		int x=((*AIndex)%12)*dx;
		int y=-(3-(*AIndex)/12)*dy+dy;
		
		// progress
		int GA=0;
		int Health=0;
		int MaxHealth=0;

		if(!go->newMons->Building){
			GA=GetAmount(*Build);
			Health=GetProgress(*Build,&MaxHealth);
		};

		//Draw
		if(GA>0){ //MaxHealth>0&&Health>0

			if(MaxHealth==0) MaxHealth=1;			

			int IconLx=67;//GPS.GetGPWidth(FI_File,spr);
			int IconLy=GPS.GetGPHeight(FI_File,spr);

			DS->addGPPicture(NULL,x,y,FI_File,spr);

			x+=DS->BaseX;
			y+=DS->BaseY;

			int x1=60, y1=7, y2=115;			

			int L=div((y2-y1+1)*Health,MaxHealth).quot;
			Canvas* Can=DS->AddCanvas(x,y,IconLx,IconLy);
			
			y2=y1+L;

			Can->AddLine(x1,y1,x1,y2,0xFF);
			Can->AddLine(x1+1,y1,x1+1,y2,0xFF);
			Can->Diffuse=0xFF00FF00;

			Can=DS->AddCanvas(x,y,IconLx,IconLy);
			Can->AddLine(x1+2,y1,x1+2,y2,0);

			x-=DS->BaseX;
			y-=DS->BaseY;

		}else{
			DS->addGPPicture(NULL,x,y,FI_File,21);
		}

		int f=bm->MinIconFile;
		int s=bm->MinIconIndex;
		if(f==0xFFFF){
			f=bm->IconFileID;
			s=bm->IconID;
		}

		GP_Button* But=DS->addGP_Button(NULL,x,y,f,s,s);

		char txt[128];
		if(GA>0){
			DS->addGPPicture(NULL,x,y,FI_File,23);			
			if(GA<1200){
				sprintf(txt,"%d",GA);
				int xx=x+30, yy=y+106;
				DS->addTextButton(NULL,xx+1,yy+1,txt,&SmallBlackFont,&SmallBlackFont,&SmallBlackFont,1);
				DS->addTextButton(NULL,xx,yy,txt,&SmallWhiteFont,&SmallWhiteFont,&SmallWhiteFont,1);
			}else{
				DS->addGP_Button(NULL,x+19,y+108,FI_File,32,32);
			}
		}

		But->UserParam=*Build;
		if(go->newMons->Building){
			But->OnUserClick=PrepareToConstruct;
		}else{
			But->OnUserClick=ProduceUnit;
			But->OnUserRightClick=UnProduceUnit;
		}
		But->HotKey=bm->BuildHotKey;
		
		txt[0]=0;		
		GetChar(go,txt);
		GetMonCost(NI,*Build,txt);
		DYNHINT(But,txt);

		if(!CheckCostHint(NI,*Build)) But->Diffuse=0x7FFFFFFF;

	}

}

void ShowBuildComPanel(OneObject* OBJ){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	Nation* Nat=NATIONS+NI;
	int NBuild=Nat->PACount[OIS.SelObj[0]];
	if(NBuild){
		OB=OBJ;
		GO=OB->Ref.General;
		NM=GO->newMons;
		DialogsSystem DS;
		DS.BaseX=300;
		DS.BaseY=RealLy-1;
		ShowKriComPanel(&DS);
		DS.ProcessDialogs();
	}
};
void ShowBldComPanel(DialogsSystem* DS){

	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	
	Nation* Nat=NATIONS+NI;
	
	//word NIndex=OB->NIndex;

	int NUpg=GO->NUpgrades;
	word* Upg=GO->Upg;

	int dx=GPS.GetGPWidth(FI_File,19)+2;
	int dy=GPS.GetGPHeight(FI_File,19)+2;
	
	if(!(OB->Ready||OB->LocalOrder&&OB->LocalOrder->DoLink==PerformUpgradeLink)) return;

	ShowKriComPanel(DS);

	for(int i=0;i<NUpg;i++,Upg++){
		NewUpgrade* nu=Nat->UPGRADE[*Upg];
		
		/*
		if((UPG->ManualEnable)||((!UPG->ManualDisable)&&(UPG->Enabled||(UPG->Gray&&!UPG->Done)))){
			NewUpgrade* NUPG=NT->UPGRADE[s];
			bool OKK=false;
			if(NUPG->Individual||NUPG->StageUp){
				if(NUPG->Individual&&OBJ->SingleUpgLevel==NUPG->Level)OKK=true;
//#ifdef CONQUEST
				if(NUPG->StageUp){
					byte st=(OBJ->StageState>>((NUPG->CtgUpgrade-24)*3))&7;
					if(st!=2)OKK=true;
				};
//#endif
			}else OKK=true;
			if(NUPG->StageMask){
				byte m=NUPG->StageMask;
				word st=OBJ->StageState;
				for(int i=0;i<5;i++){
					if(m&1){
						byte cs=st&7;
						if(cs!=2)OKK=false;
					};
					m >>=1;
					st>>=3;
				};
			};
			*/

		bool ok=nu->ManualEnable||(!(nu->ManualDisable||(BaloonState&&nu->Options&2)||
					 (CannonState==1&&nu->Options&1)||
					 (XVIIIState&&nu->Options&4)));
		
		if(nu->Individual&&OB->SingleUpgLevel!=nu->Level) ok=0;

		if(ok){
			int box_spr=18;
			int FID=GetSUBorderFileID();
			
			int x=nu->IconPosition%12;
			int y=nu->IconPosition/12;

			x*=dx;
			y=-(3-y)*dy;

			GP_Button* Box;
			
			if(OB->LocalOrder&&OB->LocalOrder->DoLink==PerformUpgradeLink){
				word Stage=OB->LocalOrder->info.PUpgrade.Stage;
				word NStages=OB->LocalOrder->info.PUpgrade.NStages;
			
				box_spr=19;
				int x0=59;
				int y0=6;
				int Ly0=52-y0+1;
 				int y1=Ly0-Ly0*Stage/NStages;
				Canvas* can=DS->AddCanvas(DS->BaseX+x+x0,DS->BaseY+y+y0,3,Ly0);
				can->AddLine(0,y1,0,Ly0,0xFF);
				can->AddLine(1,y1,1,Ly0,0xFF);
				can->AddLine(2,y1,2,Ly0,0x00);
			}

			Box=DS->addGP_Button(NULL,x,y,FID,box_spr,box_spr);
			Box->UserParam=*Upg;
			Box->OnUserClick=RunUpgrade;
			
			char txt[512]="";
			GetCostString(nu->Cost,txt);
			char hint[256];
			sprintf(hint,"%s/%s",nu->Message,txt);
			DYNHINT(Box,hint);

			Box=DS->addGP_Button(NULL,x,y,nu->IconFileID,nu->IconSpriteID,nu->IconSpriteID);

		}

	}

}

CIMPORT void DrawSetlUpgrade(word Index, DialogsSystem* DS, int x, int y);
void ShowStlComPanel(DialogsSystem* DS){

	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	
	Nation* Nat=NATIONS+NI;
	
	//word NIndex=OB->NIndex;	
	
	int x=55; //GPS.GetGPWidth(FI_File,19)+2;
 	int y=-(65*2+5); //GPS.GetGPHeight(FI_File,19)+2;

	//int dx=GPS.GetGPWidth(FI_File,19)+2;
	//int dy=GPS.GetGPHeight(FI_File,19)+2;
	
 	if(!OB->Ready) return;

	//ShowKriComPanel(DS);

	DrawSetlUpgrade(OB->Stuff,DS,x,y);

	/*
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
	}*/


	/*
	// Upgrade is run
	if((OB->NewBuilding||OB->LockType)&&OB->NUstages&&(!OB->Ready)&&OB->LocalOrder&&OB->LocalOrder->DoLink==&PerformUpgradeLink){
	}
	*/
}

// Standart units: kri, bld, settlement, oboz
CIMPORT byte GetTribeOwner(word Index);
extern int NSelWay;
void CreateSelWay(word t0, word t1, byte LockType,byte NI);
int GetTopology(int,int,byte LockType=0);
void NewMonsterSmartSendToLink(OneObject*);

void PrepareDrawObjWay(OneObject* OB){	
	if(OB){
		Order1* Ord=OB->LocalOrder;
		while(Ord){
			if(Ord->DoLink==NewMonsterSmartSendToLink){
				word t0;
				word t1;

				t0=GetTopology(OB->RealX>>4,OB->RealY>>4,OB->LockType);
				int x=Ord->info.SmartSend.x;
				int y=Ord->info.SmartSend.y;
				t1=GetTopology(x,y,OB->LockType);		

				CreateSelWay(t0,t1,OB->LockType,OB->NNUM);
				break;
			}
			Ord=Ord->NextOrder;
		}
	}	
}

void B_HumanGlobalSendToLink(Brigade*);
void B_KeepPositionsLink(Brigade*);
bool GetBrigDest(CBrigadeOrder* BO,int &x, int &y);
word GetBrigLockType(Brigade*);

extern int NSelWay;
extern word SelWay[2048];
extern byte SelLockType;
word GetBrigLockTypeIntf(Brigade* BR);

void PrepareDrawBrigWay(Brigade* Br){	
	if(Br&&Br->Enabled){
		byte LT;
		word t0=0xFFFF;
		word fint=0xFFFF;
		CBrigadeOrder* BO=Br->NewBOrder;
		while(BO){
			if(t0==0xFFFF){
				LT=GetBrigLockTypeIntf(Br);
				int sx,sy;
				byte dir;
				Br->GetCenter(&sx,&sy,&dir);
				t0=GetTopology(sx,sy,LT);
			}
			if(BO->GetBrigadeOrderID()==BRIGADEORDER_KEEPPOSITION)//B_KeepPositionsLink
			{
				int finx,finy;
				word* ID=Br->Memb;
				word* SN=Br->MembSN;
				int N=Br->NMemb;
				for(int i=0;i<N;i++,ID++,SN++){
					OneObject* OB=Group[*ID];
					if(OB&&OB->Serial==*SN&&!OB->Sdoxlo){
						finx=Br->posX[i];
						finy=Br->posY[i];
						fint=GetTopology(finx,finy,LT);
						break;
					}
				}
			}
			if(BO->GetBrigadeOrderID()==BRIGADEORDER_HUMANGLOBALSENDTO)//B_HumanGlobalSendToLink
			{
				int x,y;				
				word t1;

				if(!GetBrigDest(BO,x,y)){
					break;
				}
				t1=GetTopology(x,y,LT);

				if(t0!=t1){
					CreateSelWay(t0,t1,LT,Br->CT->NI);
				}
				break;
			}
			BO=BO->Next.Get();
		}
		if(t0!=0xFFFF&&fint!=0xFFFF){
			if(NSelWay==0){
				SelLockType=LT;
				NSelWay=2;
				SelWay[0]=t0;
				SelWay[1]=fint;
			}/*else{
				if(SelWay[NSelWay-1]!=fint){
					SelWay[NSelWay]=fint;
					NSelWay++;
				}
			}*/			
		}
	}
}

int CreateStandartUnitsInterface(){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];

	InitGlobalFIVar();

	OB=NULL;

	/*
	if(OIS.NSelObj>0){
		OB=NULL;
		NOB=0;
		int Nsel=ImNSL[MyNation];
		word* sel=ImSelm[MyNation];
		word* ser=ImSerN[MyNation];
		for(int i=0;i<Nsel;i++,sel++,ser++){
			OneObject* ob=Group[*sel];
			if(ob&&ob->Serial==*ser&&!ob->Sdoxlo){
				if(!OB){
					OB=ob;
				}
				if(OB->NIndex==ob->NIndex) NOB++;
			}
		}		
	}else
	if(OIS.Bld.GetAmount()>0){
		OB=Group[OIS.Bld[0]->ID[0]];
		NOB=OIS.Bld[0]->ID.GetAmount();
	}else
	*/
	if(OIS.Settlement!=0xFFFF){
		OB=Group[OIS.Settlement];
		NOB=1;
	}else
	if(OIS.Oboz!=0xFFFF){
		OB=Group[OIS.Oboz];
		NOB=1;
	}
	if(!OB) return 4;
	GO=OB->Ref.General;
	NM=GO->newMons;

	//return 0;
	if(OB->Usage==WeakID){
		if(OB->NNUM==NI) return 0;
			else return 4;
	}

	// prepare for draw selected unit way on mini map;	
	PrepareDrawObjWay(OB);

	int result=0;	
	DialogsSystem DS;

	if(OB->Stuff!=0xFFFF){
		if(OB->NewBuilding){
			style=0;
			DS.BaseX=0;
			DS.BaseY=RealLy-0-1;

			AddStlInfoPanel(&DS);
			if(GetTribeOwner(OB->Stuff)==NI) ShowStlComPanel(&DS);
		}
		result=4;
	}else{
		if(NM->Building){

			style=0;
			DS.BaseX=0;
			DS.BaseY=RealLy-0-1;

			AddBldInfoPanel(&DS);
			
			DS.BaseX+=bLx+4;

			if(OB->NNUM==NI) ShowBldComPanel(&DS);
			result=4;
		
		}else{				
			style=0;
			DS.BaseX=0;
			DS.BaseY=RealLy-0-1;

			AddKriInfoPanel(&DS);
			
			DS.BaseX+=bLx+4;

			if(OB->NNUM==NI) ShowKriComPanel(&DS);
			result=4;
			
			if(!NM->Peasant){
				// кнопки для военных
			}
		}

	}

	DS.ProcessDialogs();	

	return result;
}


CEXPORT void SetStuff(GAMEOBJ* Grp, word stuff){
	if(Grp->Type!='UNIT') return;
	UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];
	int n=UG->N;
	word* id=UG->IDS;
	word* sn=UG->SNS;
	for(int i=0;i<n;i++,id++,sn++){
		OneObject* OB=Group[*id];
		if(OB) OB->Stuff=stuff;
	}
}
CEXPORT void SetStuff(word Index, word stuff){
	if(Index>MAXOBJECT) return;
	OneObject* OB=Group[Index];
	if(OB) OB->Stuff=stuff;
}