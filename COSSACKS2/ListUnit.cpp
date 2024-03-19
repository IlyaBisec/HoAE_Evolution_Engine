#include "stdheader.h"
#include "Extensions.h"

/*class MDCommand:public BaseClass{
public:
};
class MDCommandsList:public ClassArray<MDCommand>{
public:
};
class MDMESSAGE:public MDCommand{
public:
	_str message;
	SAVE(MDMESSAGE);
	REG_PARENT(MDCommand);
	ENDSAVE;
};
class MDNAME:public MDCommand{
public:
	_str name;
	SAVE(MDNAME);
	REG_PARENT(MDCommand);
	ENDSAVE;
};
class MDICON:public MDCommand{
public:
	int IconFileID;
	int IconID;
	SAVE(MDICON);
	REG_PARENT(MDCommand);
	ENDSAVE;
};
class MDRECTANGLE:public MDCommand{
public:
	int RectDx;
	int RectDy;
	int RectLx;
	int RectLy;
	SAVE(MDRECTANGLE);
	REG_PARENT(MDCommand);
	ENDSAVE;
};
int NextLine(const char* str){
	int ch=0;
	char word[128];
	int L=strlen(str);
	while(str[ch]!=10){
		ch++;
		if(ch>=L) return 0;
	}
	ch+=2;
	return ch;
}
MDCommandsList UNITLIST;
extern ClonesArray<mdCmdParser> mdCmdList;
void ConvertMDtoXML(char* name){
	if(!name) return;
	xmlQuote xml;
	if(xml.ReadFromFile("mdCmd.xml")){
		ErrorPager Error;
		mdCmdList.Load(xml,&mdCmdList,&Error);
	}
	char Fn[128];
	sprintf(Fn,"%s.md",name);	
	ResFile f1=RReset(Fn);
	if(f1==INVALID_HANDLE_VALUE) return;
	int Lf=RFileSize(f1);
	int L=0;
	int L1=0;
	char* buf=new char[Lf+1];
	RBlockRead(f1,buf,Lf);
	buf[L]=0;
	char unitName[128];
	while(L<Lf){
		int N=mdCmdList.GetAmount();
		for(int i=0;i<N;i++){
            L1=mdCmdList[i]->Parse(buf+L);
			if(L1){
				strcpy(unitName,mdCmdList[i]->ClassName.pchar());
				L+=L1;
				break;
			}
		}
		if(!L1){ //not found
			L1=NextLine(buf+L);
			if(!L1) break;
			L+=L1;
		}
	}
	delete[](buf);
}
void RegisterUnitlistEditor(){
	REG_CLASS(MDCommand);
	REG_CLASS(MDCommandsList);
	REG_CLASS(MDICON);
	REG_CLASS(MDNAME);
	REG_CLASS(MDMESSAGE);
	REG_CLASS(MDRECTANGLE);
	ConvertMDtoXML("asrdo1");
	AddStdEditor("UnitsList",&UNITLIST,"",RCE_DEFAULT);//,rce_MonstersCallback);
}

/*class mdCmdParser;
class mdCmdElm:public BaseClass{
public:
	virtual void GetPreview(_str& Dest){
	}
	bool Parse(const char* src,xmlQuote& xml){
		return false;
	};
};
class mdCmdStr:public mdCmdElm{
public:
	_str Str;
	//bool Parse(const char* src,xmlQuote& xml);
	SAVE(mdCmdStr);
		REG_PARENT(mdCmdElm);
		REG_AUTO(Str);
	ENDSAVE;
	void GetPreview(_str& Dest){
		Dest+=Str;
	}
};
class mdCmdStrAndSpace:public mdCmdElm{
public:
	_str Str;
	//bool Parse(const char* src,xmlQuote& xml);
	SAVE(mdCmdStrAndSpace);
		REG_PARENT(mdCmdElm);
		REG_AUTO(Str);
	ENDSAVE;
	void GetPreview(_str& Dest){
		Dest+=Str;
		Dest+=" ";
	}
};
class mdCmdIntField:public mdCmdElm{
public:
	_str FieldName;
	//bool Parse(const char* src,xmlQuote& xml);
	SAVE(mdCmdIntField);
		REG_PARENT(mdCmdElm);
		REG_AUTO(FieldName);
	ENDSAVE;
	void GetPreview(_str& Dest){
		Dest+=FieldName;
		Dest+=" ";
	}
};
class mdCmdStrFileld:public mdCmdElm{
public:
	_str FieldName;
	//bool Parse(const char* src,xmlQuote& xml);
	SAVE(mdCmdStrFileld);
		REG_PARENT(mdCmdElm);
		REG_AUTO(FieldName);
	ENDSAVE;
	void GetPreview(_str& Dest){
		Dest+=FieldName;
		Dest+=" ";
	}
};
class mdCmdCmdArray:public mdCmdElm{
public:
	ClassArray<mdCmdElm> Elements;
	//bool Parse(const char* src,xmlQuote& xml);
	SAVE(mdCmdCmdArray);
		REG_PARENT(mdCmdElm);
		REG_AUTO(Elements);
	ENDSAVE;
	void GetPreview(_str& Dest){
		Dest+=" N{CG}[{CW}";
		for(int i=0;i<Elements.GetAmount();i++)Elements[i]->GetPreview(Dest);
		Dest+="...{CG}]{CW} ";
	}
};
class mdCmdParser:public BaseClass{
public:
	_str ClassName;
    ClassArray<mdCmdElm> Elements;    
	SAVE(mdCmdParser);
		REG_AUTO(ClassName);
		REG_AUTO(Elements);
	ENDSAVE;
	const char* GetThisElementView(const char* Local){
		static _str Dest;
		Dest="";
		Dest+="{CG}";
		Dest+=ClassName;
		Dest+="{CW}";
		Dest+=" {CW}";
		for(int i=0;i<Elements.GetAmount();i++)Elements[i]->GetPreview(Dest);
		Dest+=" {C}";
		return Dest.pchar();
	}
};
ClonesArray<mdCmdParser> mdCmdList;
void reg_md_editor(){
	REG_CLASS(mdCmdElm);
	REG_CLASS(mdCmdStr);
	REG_CLASS(mdCmdStrAndSpace);
	REG_CLASS(mdCmdIntField);
	REG_CLASS(mdCmdStrFileld);
	REG_CLASS(mdCmdCmdArray);
	REG_CLASS(mdCmdParser);
	AddStdEditor("mdCmdEditor",&mdCmdList,"mdCmd.xml",RCE_DEFAULT);	
}
class mdCmdNAME:public mdCmdParser{
public:
	const char* GetThisElementView(const char* Local){
		static _str Dest;
		Dest="";
		Dest+="{CG}";
		Dest+=ClassName;
		Dest+="{CW}";
		Dest+=" {CW}";
		for(int i=0;i<Elements.GetAmount();i++)Elements[i]->GetPreview(Dest);
		Dest+=" {C}";
		return Dest.pchar();
	}
};
class mdCmdICON:public mdCmdParser{
public:
	int IconFileID;
	int IconID;
};
class mdCmdRECTANGLE:public mdCmdParser{
public:
	int RectDx;
	int RectDy;
	int RectLx;
	int RectLy;
};*/
////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
/*bool ProcessSavingFile(char* Mask,char* DestName,int Header,bool clear);
class mdCommand:public BaseClass{
public:
	virtual bool Perform(){return true;}
};
///////////////////////////////// A ///////////////////////////////////////////////////////
class mdCmdANMEXT:public mdCommand{
public:
	SAVE(mdCmdANMEXT)
	REG_PARENT(mdCommand);
	ENDSAVE;
};
class mdCmdAUTOROTATE:public mdCommand{
public:
	SAVE(mdCmdAUTOROTATE)
	REG_PARENT(mdCommand);
	ENDSAVE;
};
class mdCmdABILITY:public mdCommand{
public:
	SAVE(mdCmdABILITY)
	REG_PARENT(mdCommand);
	ENDSAVE;
};
class mdCmdADDHDIR:public mdCommand{
public:
	SAVE(mdCmdADDHDIR)
	REG_PARENT(mdCommand);
	ENDSAVE;
};
class mdCmdARMRADIUS:public mdCommand{
public:
	int ArmRadius;
	SAVE(mdCmdARMRADIUS)
	REG_PARENT(mdCommand);
	REG_MEMBER(_int,ArmRadius);
	ENDSAVE;
};
class mdCmdAISHIP:public mdCommand{
public:
	int AI_PreferredAttR_Min;
	int AI_PreferredAttR_Max;
	SAVE(mdCmdAISHIP)
	REG_PARENT(mdCommand);
	REG_MEMBER(_int,AI_PreferredAttR_Min);
	REG_MEMBER(_int,AI_PreferredAttR_Max);
	ENDSAVE;
};
class mdCmdATTACK_PAUSE:public mdCommand{
public:
	SAVE(mdCmdATTACK_PAUSE)
	REG_PARENT(mdCommand);
	ENDSAVE;
};
class mdCmdATTACK_ANGLES:public mdCommand{
public:
	SAVE(mdCmdATTACK_ANGLES)
	REG_PARENT(mdCommand);
	ENDSAVE;
};
class mdCmdADD_ATTACK_RADIUS:public mdCommand{
public:
	SAVE(mdCmdADD_ATTACK_RADIUS)
	REG_PARENT(mdCommand);
	ENDSAVE;
};
class mdCmdATTACK_RADIUS:public mdCommand{
public:
	SAVE(mdCmdATTACK_RADIUS)
	REG_PARENT(mdCommand);
	ENDSAVE;
};
class mdCmdATTMASK:public mdCommand{
public:
	bool AttBuild;
	SAVE(mdCmdATTMASK)
	REG_PARENT(mdCommand);
	REG_MEMBER(_bool,AttBuild);
	ENDSAVE;
};
class mdCmdARCHER:public mdCommand{
public:
	int Archer;
	SAVE(mdCmdARCHER)
	REG_PARENT(mdCommand);
	REG_MEMBER(_int,Archer);
	ENDSAVE;
};
class mdCmdADDSHOTRADIUS:public mdCommand{
public:
	int AddShotRadius;
	SAVE(mdCmdADDSHOTRADIUS)
	REG_PARENT(mdCommand);
	REG_MEMBER(_int,AddShotRadius);
	ENDSAVE;
};
/////////////////////////////////////////// N //////////////////////////////////////////
class mdCmdName:public mdCommand{
public:
	_str Name;
	SAVE(mdCmdName)
	REG_PARENT(mdCommand);
	REG_AUTO(Name);
	ENDSAVE;
};
class mdCmdMessage:public mdCommand{
public:
	_str Message;
	SAVE(mdCmdMessage)
	REG_PARENT(mdCommand);
	REG_AUTO(Message);
	ENDSAVE;
};
class mdCmdIcon:public mdCommand{
public:
	int IconFileID;
	int IconID;
	SAVE(mdCmdIcon)
	REG_PARENT(mdCommand);
	REG_MEMBER(_int,IconFileID);
	REG_MEMBER(_int,IconID);
	ENDSAVE;
};
class OneUnitsDescription:public BaseClass{
public:
	_str Description;
	ClassArray<mdCommand> Property;
	virtual const char* GetThisElementView(const char* Local){
		static char cc[128];
		if(Description.pchar())
			sprintf(cc,"%s: %s",Local,Description.pchar());
		else
			sprintf(cc,"%s",Local);
		return cc;
	};
	SAVE(OneUnitsDescription)
	REG_AUTO(Description);
	REG_CLASS(mdCommand);
	REG_CLASS(mdCmdName);
	REG_CLASS(mdCmdMessage);
	REG_CLASS(mdCmdIcon);
	REG_AUTO(Property);
	ENDSAVE;
};

class UnitsList:public BaseClass{
public:
	ClonesArray<OneUnitsDescription> Units;
	SAVE(UnitsList)
	REG_CLASS(OneUnitsDescription);
	REG_AUTO(Units);
	ENDSAVE;
};
extern bool ProtectionMode;
int GetIconByName(char* Name);
void NLine(GFILE* f);
void IncPar(char* name,int line,char* Sect);
void ConvertMDtoXML(UnitsList* UL,char* name,int NNewMon){
	if(!name) return;
	char Fn[128];
	sprintf(Fn,"%s.md",name);
	ProtectionMode=1;
	GFILE* f1=Gopen(Fn,"r");
	ProtectionMode=0;
	AFile(Fn);
	int z,p1,p2,p3,p4,p5,p6,p7;
	char gx[128], gy[128], gz[128];
	bool finish=false;
	int Line=0;
	if(f1){
		do{
			z=Gscanf(f1,"%s",gx);
			if(z!=1)IncPar(name,Line,"COMMAND");
			if(gx[0]!='/'&&gx[0]!='['){
				switch(gx[0]){
					case 'A':
						/*if(!strcmp(gx,"ANMEXT")){//ANMEXT #SRCANM #SUBANM dx dy dz scale Period
							int x,y,z,p;
							float sc;
							int zz=Gscanf(f1,"%s%s%d%d%d%f%d",gy,gz,&x,&y,&z,&sc,&p);
							if(zz==7){
								{
									NewAnimation* NANM=LoadNewAnimationByName(gy,0);
									if(NANM){
                                        NewAnimation* SUB=LoadNewAnimationByName(gz,0);													
										if(SUB){
                                            AnimationExtension* AEX=new AnimationExtension;
											AEX->NA=SUB;
											AEX->dx=x;
											AEX->dy=y;
											AEX->dz=z;
											AEX->Scale=sc;
											AEX->dFi=0;
											AEX->dDir=0;
											AEX->Period=p;
											NANM->AnmExt.Add(AEX);
										}
									}
								}
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"AUTOROTATE")){
							int na=0;
							char fL[32];
							char fR[32];
							int dx,dy;
							int z=Gscanf(f1,"%d%d%s%s",&dx,&dy,fL,fR);
							mdCmdAUTOROTATE* MCA=new mdCmdAUTOROTATE;
							UL->Units[NNewMon]->Property.Add(MCA);
							if(z==4){
								NewAnimation* L=CreateAnimation("#ROTATEL");
								NewAnimation* R=CreateAnimation("#ROTATER");
								int LF=GPS.PreLoadGPImage(fL);
								int RF=GPS.PreLoadGPImage(fR);
								for(int i=0;i<128;i++){
									NewFrame* NFL=new NewFrame;
									NFL->FileID=LF;
									NFL->dx=dx;
									NFL->dy=dy;
									NFL->SpriteID=((16-(i/16)*2)%16)+(i%16)*16;
									L->Frames.Add(NFL);
									NewFrame* NFR=new NewFrame;
									NFR->FileID=RF;
									NFR->dx=dx;
									NFR->dy=dy;
									NFR->SpriteID=((i/16)*2)+(i%16)*16;
									R->Frames.Add(NFR);
								}
								L->NFrames=128;
								R->NFrames=128;
								L->Rotations=1;
								R->Rotations=1;
								L->Enabled=1;
								R->Enabled=1;
							}
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ABILITY")){							
							int z=Gscanf(f1,"%s",gy);
							mdCmdABILITY* MCA=new mdCmdABILITY;
							UL->Units[NNewMon]->Property.Add(MCA);
							if(z==1){
								//AbilityFileName=gy;
								bool AddMonsterAbility(MonsterAbility** MA,char* Name);
								AddMonsterAbility(&Ability,gy);
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;							
						}else
						if(!strcmp(gx,"ADDHDIR")){
							int A,H;
							int z=Gscanf(f1,"%s%d%d",gy,&H,&A);
							mdCmdADDHDIR* MCA=new mdCmdADDHDIR;
							UL->Units[NNewMon]->Property.Add(MCA);
							if(z==3){
								{
									NewAnimation* NANM=LoadNewAnimationByName(gy,0);
									if(NANM){
										NANM->AddDirection=A;
										NANM->AddHeight=H;
									}
								}
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ARMRADIUS")){
							int R2;
							int z=Gscanf(f1,"%d",&R2);
							mdCmdARMRADIUS* MCA=new mdCmdARMRADIUS;
							UL->Units[NNewMon]->Property.Add(MCA);
							if(z==1){
								MCA->ArmRadius=R2;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"AISHIP")){
							char cc[32];
							int R1,R2;
							int z=Gscanf(f1,"%s%d%d",cc,&R1,&R2);
							mdCmdAISHIP* MCA=new mdCmdAISHIP;
							UL->Units[NNewMon]->Property.Add(MCA);
							if(z==3){
								if(!strcmp(cc,"B"))AI_use_against_buildings=1;
								MCA->AI_PreferredAttR_Min=R1;
								MCA->AI_PreferredAttR_Max=R2;
							}else IncPar(name,Line,gx);
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTACK_PAUSE")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,"ATTACK_PAUSE");
							if(p1>=NAttTypes){
								Log.Error( "%s,line %d : Attack type index in ATTACK_PAUSE is too big.",name,Line);
							};
							mdCmdATTACK_PAUSE* MCA=new mdCmdATTACK_PAUSE;
							UL->Units[NNewMon]->Property.Add(MCA);
							AttackPause[p1]=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTACK_ANGLES")){
							z=Gscanf(f1,"%d%d%d",&p1,&p2,&p3);
							if(z!=3)IncPar(name,Line,"ATTACK_ANGLES");
							if(p1>=NAttTypes){
								Log.Error( "%s,line %d : Attack type index in SHOT_ANGLE is too big.",name,Line);
							};
							mdCmdATTACK_ANGLES* MCA=new mdCmdATTACK_ANGLES;
							UL->Units[NNewMon]->Property.Add(MCA);
							AngleUp[p1]=p3;
							AngleDn[p1]=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ADD_ATTACK_RADIUS")){
							z=Gscanf(f1,"%d%d",&p1,&p2);
							if(z!=2)IncPar(name,Line,"ADD_ATTACK_RADIUS");
							if(p1>=NAttTypes){
								Log.Error( "%s,line %d : Attack type index in ADD_ATTACK_RADIUS is too big.",name,Line);
							};
							mdCmdADD_ATTACK_RADIUS* MCA=new mdCmdADD_ATTACK_RADIUS;
							UL->Units[NNewMon]->Property.Add(MCA);
							AttackRadiusAdd[p1]=p2;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTACK_RADIUS")){
							z=Gscanf(f1,"%d%d%d",&p1,&p2,&p3);
							if(z!=3)IncPar(name,Line,gx);
							if(p1>=NAttTypes){
								Log.Error( "%s,line %d : Attack type index in ATTACK_RADIUS is too big.",name,Line);
							};
							mdCmdATTACK_RADIUS* MCA=new mdCmdATTACK_RADIUS;
							UL->Units[NNewMon]->Property.Add(MCA);
							AttackRadius1[p1]=p2;
							AttackRadius2[p1]=p3;
							DetRadius1[p1]=p2;
							DetRadius2[p1]=p3;
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ATTMASK")){
							z=Gscanf(f1,"%d%d",&p1,&p4);
							if(z!=2)IncPar(name,Line,gx);
							mdCmdATTMASK* MCA=new mdCmdATTMASK;
							UL->Units[NNewMon]->Property.Add(MCA);
							AttackMask[p1]=0;
							for(int k=0;k<p4;k++){
								z=Gscanf(f1,"%s",gy);
								p2=GetMatherialType(gy);
								if(p2!=-1){
									AttackMask[p1]|=p2;
									if(!strcmp(gy,"BUILDING")){
										MCA->AttBuild=true;
									};
								}else{
									Log.Error( "%s, Line: %d :ATTMASK: Unknown matherial:%s",name,Line,gy);
								};
							};
							NLine(f1);
							Line++;
						}else
						if(!strcmp(gx,"ARCHER")){
							mdCmdARCHER* MCA=new mdCmdARCHER;
							UL->Units[NNewMon]->Property.Add(MCA);
							MCA->Archer=1;
							Line++;
							NLine(f1);
						}else
						if(!strcmp(gx,"ADDSHOTRADIUS")){
							z=Gscanf(f1,"%d",&p1);
							if(z!=1)IncPar(name,Line,gx);
							mdCmdADDSHOTRADIUS* MCA=new mdCmdADDSHOTRADIUS;
							UL->Units[NNewMon]->Property.Add(MCA);
							MCA->AddShotRadius=p1;
							NLine(f1);
							Line++;
						}else{
							Log.Error( "%s,line %d : Unknown command : %s",name,Line,gx);
						};
						break;*/
					/*case 73: // I
						if(!strcmp(gx,"ICON")){
							z=Gscanf(f1,"%s",&gy);
							if(z!=1)IncPar(name,Line,"ICON");
							p2=GetIconByName(gy);
							if(p2==-1){
								sprintf(gy,"%s,%d : Unknown Icon ID: %s",name,Line,gy);
								ErrM(gy);
							};
							mdCmdIcon* MCI=new mdCmdIcon;
							UL->Units[NNewMon]->Property.Add(MCI);
							MCI->IconFileID=0;
							MCI->IconID=p2;
							NLine(f1);
							Line++;
						}
					case 78: // M
						if(!strcmp(gx,"MESSAGE")){
							z=Gscanf(f1,"%s",&gy);
							if(z!=1)IncPar(name,Line,"MESSAGE");
							mdCmdMessage* MCM=new mdCmdMessage;
							UL->Units[NNewMon]->Property.Add(MCM);
							MCM->Message.Assign(gy);
							NLine(f1);
							Line++;
						}
					case 77: // N
						if(!strcmp(gx,"NAME")){
							z=Gscanf(f1,"%s",&gy);
							if(z!=1)IncPar(name,Line,"NAME");
							UL->Units[NNewMon]->Description.Assign(gy);
							mdCmdName* MCN=new mdCmdName;
							UL->Units[NNewMon]->Property.Add(MCN);
							MCN->Name.Assign(gy);
							NLine(f1);
							Line++;
						}
					default:
						NLine(f1);
						Line++;
				}
			}else{
				if(!strcmp(gx,"[END]"))finish=true;
				NLine(f1);
				Line++;
			}
		}while((!finish)&&z!=-1);
	}
}
void LoadAllNewMonsters(UnitsList* UL){
	word NNewMon=0;
	ProtectionMode=1;
	GFILE* f1=Gopen("nmlist.txt","r");
	ProtectionMode=0;
	AFile("nmlist.txt");
	if(f1){
		char mname[128];
		int z;
		do{
			mname[0]=0;
			z=Gscanf(f1,"%s",mname);
			if(mname[0]){
				UL->Units.Add(new OneUnitsDescription);
				ConvertMDtoXML(UL,mname,NNewMon);
				NNewMon++;
			}
		}while(z!=-1&&mname[0]);
	}else
		return;
}
UnitsList UNITLIST;
void RegisterUnitlistEditor(){	
	LoadAllNewMonsters(&UNITLIST);
	AddStdEditor("UnitsList",&UNITLIST,"",RCE_DEFAULT);//,rce_MonstersCallback);	
}*/