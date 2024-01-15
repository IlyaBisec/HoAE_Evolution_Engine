#include "stdheader.h"
#include "CurrentMapOptions.h"
#include "GameInterface.h"

#define IGNOREMORALE 0
#define M_return if(IGNOREMORALE||EngSettings.MoraleType)return 

struct OneIllustration{
	DWORD Color;
	int x,y,z;
	int LX;
	int LY;
	byte TTL;
	byte Type;
	char Text[32];
	int Value;
	RLCFont* Font;
};
class Illustrator{
public:
	int MinX;
	int MinY;
	int MaxX;
	int MaxY;
	OneIllustration* ILS;
	int NILS;
	int MaxILS;
	Illustrator();
	~Illustrator();
	void Add(int x,int y,char* text,RLCFont* Font,byte Type,DWORD Color);
	void Add(int x,int y,int val,RLCFont* Font,byte Type,DWORD Color);
	void Clear();
	void Draw();
	bool CheckIntersection(int x,int y,int Lx,int Ly);
	bool CheckIntersection(int x,int y,int Lx,int Ly,char* s,byte Type);
};
Illustrator ILSTR;
void AddIllustrator(int x,int y,char* text,RLCFont* Font,byte Type,DWORD Color){
	ILSTR.Add(x,y,text,Font,Type,Color);
}
void AddIllustrator(int x,int y,int val,RLCFont* Font,byte Type,DWORD Color){
	ILSTR.Add(x,y,val,Font,Type,Color);
}
Illustrator::Illustrator(){
	memset(this,0,sizeof *this);
};
Illustrator::~Illustrator(){
	if(ILS)free(ILS);
	memset(this,0,sizeof *this);
};
void Illustrator::Clear(){
	NILS=0;
};
bool Illustrator::CheckIntersection(int x,int y,int Lx,int Ly){
	int xc=x+(Lx>>1);
	int yc=y+(Ly>>1);
	int zc=GetTotalHeight(xc,yc);
	for(int i=0;i<NILS;i++){
		int x=ILS[i].x;
		int y=ILS[i].y;
		int LX=ILS[i].LX;
		int LY=ILS[i].LY;
		x+=LX>>1;
		y+=LY>>1;
		int dx=abs(x-xc);
		int dy=abs(y-yc);
		int dz=abs(ILS[i].z-zc);
		if( dx<((LX+Lx)>>1) && dy<((LY+Ly)>>1) && dz<(LY<<1) )
			return true;
	};
	return false;
};
bool Illustrator::CheckIntersection(int x,int y,int Lx,int Ly,char* S,byte Type){
	int xc=x+(Lx>>1);
	int yc=y+(Ly>>1);
	//int zc=GetTotalHeight(xc,yc);
	for(int i=0;i<NILS;i++){
		int x=ILS[i].x;
		int y=ILS[i].y;
		int LX=ILS[i].LX;
		int LY=ILS[i].LY;
		x+=LX>>1;
		y+=LY>>1;
		int dx=abs(x-xc);
		int dy=abs(y-yc);
		//int dz=abs(ILS[i].z-zc);
		if( dx<((LX+Lx)>>1) && dy<((LY+Ly)>>1)/* && dz<(LY<<1) */){
			if(Type==255){
				if(ILS[i].Type==Type){
					char* s=ILS[i].Text;
					char* s1=strstr(s,"(");
					char* S1=strstr(S,"(");
					if(s1&&S1){
						int v1,v2,V1,V2;
						if(s[0]=='+'){
							v1=atoi(s+1);
						}else v1=atoi(s);
						v2=atoi(s1);
						if(S[0]=='+'){
							V1=atoi(S+1);
						}else V1=atoi(S);
						V2=atoi(S1);
						V1+=v1;
						if(V1>0)sprintf(s,"+%d(%d)",V1,V2);						
					};
				};
			};
			return true;
		};
	};
	return false;
};
void Illustrator::Add(int x,int y,char* text,RLCFont* Font,byte Type,DWORD Color){
	//if(x<MinX||y<MinY||x>MaxX||y>MaxY)return;
	if(NILS>=MaxILS){
		MaxILS+=32;
		ILS=(OneIllustration*)realloc(ILS,MaxILS*sizeof OneIllustration);
	};
	strcpy(ILS[NILS].Text,text);
	ILS[NILS].Value=0;
	ILS[NILS].x=x;
	ILS[NILS].y=y;
	ILS[NILS].z=GetTotalHeight(x,y);
	ILS[NILS].Font=Font;
	ILS[NILS].Type=Type;
	int LX=GetRLCStrWidth(ILS[NILS].Text,Font);
	ILS[NILS].x-=LX>>1;
	ILS[NILS].LX=LX;
	ILS[NILS].LY=GetRLCHeight(Font->RLC,'0')+3;
	ILS[NILS].TTL=0;
	ILS[NILS].Color=Color;
	int na=0;
	while(CheckIntersection(ILS[NILS].x,ILS[NILS].y,ILS[NILS].LX,ILS[NILS].LY,text,Type)&&na<5){
		if(Type==254)return;
		ILS[NILS].x+=(rand()&32)-16;
		ILS[NILS].y+=(rand()&32)-22;
		na++;
	}
	NILS++;
};
void Illustrator::Add(int x,int y,int val,RLCFont* Font,byte Type,DWORD Color){
	//if(x<MinX||y<MinY||x>MaxX||y>MaxY)return;
	if(NILS>=MaxILS){
		MaxILS+=32;
		ILS=(OneIllustration*)realloc(ILS,MaxILS*sizeof OneIllustration);
	};
	ILS[NILS].x=x;
	ILS[NILS].y=y;
	ILS[NILS].z=GetTotalHeight(x,y);
	ILS[NILS].Font=Font;
	ILS[NILS].Type=Type;
	ILS[NILS].TTL=0;
	ILS[NILS].Value=val;
	sprintf(ILS[NILS].Text,"%d",val);
	int LX=GetRLCStrWidth(ILS[NILS].Text,Font);
	ILS[NILS].x-=LX>>1;
	ILS[NILS].LX=LX;
	ILS[NILS].LY=GetRLCHeight(Font->RLC,'0')+1;
	ILS[NILS].Color=Color;
	if(CheckIntersection(ILS[NILS].x,ILS[NILS].y,ILS[NILS].LX,ILS[NILS].LY))
		return;
	NILS++;
};
extern int RealLx;
extern int RealLy;

const int IllustratorBeatAmount=128;
const int IllustratorZSpeed=1;

void Illustrator::Draw(){
	if(LMode)return;
	if(GSets.CGame.ViewMask==0) 
		return;
	int dx=mapx<<5;
	int dy=mapy<<4;
	MinX=dx-64;
	MinY=dy-64;
	MaxX=dx+RealLx+64;
	MaxY=dy+RealLy+64;
	for(int i=0;i<NILS;i++){
		Vector3D V=SkewPt(ILS[i].x,ILS[i].y,ILS[i].z+90);
		WorldToScreenSpace(V);		
		int xx=V.x;
		int yy=V.y;
		int TTL=ILS[i].TTL;
		DWORD C=ILS[i].Color;
		//DWORD A=TTL<16?TTL*16:TTL>100-16?(100-TTL)*16:255;
		DWORD A=TTL<16?TTL*16:TTL>IllustratorBeatAmount-16?(IllustratorBeatAmount-TTL)*16:255;
		if( V.z>0 && V.z<1 && xx>-64&&xx<RealLx+64&&yy<RealLy+64&&yy>-64 ){
			DWORD FC=ILS[i].Font->GetColor();
			ILS[i].Font->SetColor((C&0xFFFFFF)+(A<<24));
			ShowString(xx,yy,ILS[i].Text,ILS[i].Font);
			ILS[i].Font->SetColor(FC);
		};
		//if(TTL&1)
			ILS[i].z+=IllustratorZSpeed;
		ILS[i].TTL++;
		if(ILS[i].TTL>IllustratorBeatAmount){
			if(i<NILS-1)memmove(ILS+i,ILS+i+1,(NILS-i-1)*sizeof OneIllustration);
			NILS--;
		};
	};
};

extern bool HealthMode;
bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);
bool GetObjectVisibilityInFog(OneObject* OB);

void ViewDamage(OneObject* OBJ,int value){
	//if( !HealthMode ) return;
	if (OBJ->IsInFog) return;
	if (!GetObjectVisibilityInFog(OBJ)) return;
	if(!GetObjectVisibilityInFog(OBJ->RealX>>4,OBJ->RealY>>4,OBJ->RZ,OBJ)) return;
	//if(GameInterface::CheckIfCameraIsMoving()) return;
	if(GSets.CGame.ViewMask==0) 
		return;
	
	// by vital
	const int MinNILS=5;
	const int MaxNILS=21;
	int N=0;
	for(int i=0;i<ILSTR.NILS;i++){
		if(ILSTR.ILS[i].Value==value)
			N++;
	}
	if( N>MinNILS && N<MaxNILS ){
		int r=rand()%(N-MinNILS);
		if(r>0)
			return;
	}else if( N>MaxNILS )
		return;
	
	int xx=OBJ->RealX>>4;
	int yy=OBJ->RealY>>4;
	byte ms=1<<GSets.CGame.cgi_NatRefTBL[MyNation];
	static char* c=GetTextByID("#Miss");
	if(OBJ->NMask&ms){
		if(value)ILSTR.Add(xx,yy,value,&SmallWhiteFont,0,0xFFFF0000);
		else ILSTR.Add(xx,yy,c,&SmallWhiteFont,0,0xFFFF0000);
	}else{
		if(value)ILSTR.Add(xx,yy,value,&SmallWhiteFont,0,0xFFFFFF00);	
		else ILSTR.Add(xx,yy,c,&SmallWhiteFont,0,0xFFFFFF00);
	}
}
void ViewDamage(int x,int y,int value){
	//int GetInterpFOW(int x,int y);
	//int dp=GetInterpFOW(x,y);		
	//if(dp<750) return;
	if(GSets.CGame.ViewMask==0) return;
	//ILSTR.Add(x,y/2-GetHeight(x,y)-64,value,&SmallWhiteFont,0,0xFF00FF00);
	if(GetObjectVisibilityInFog(x,y,GetHeight(x,y),NULL)){
		static char* c=GetTextByID("#Miss");
		if(value)ILSTR.Add(x,y,value,&SmallWhiteFont,0,0xFF00FF00);
		else ILSTR.Add(x,y,c,&SmallWhiteFont,0,0xFF00FF00);
	}
}
void ShowIllustrations(){
	ILSTR.Draw();
};
void ClearIllustrations(){
	ILSTR.Clear();
}
void LoadErr(char* sz,...)
{
        char ach[512];
        va_list va;
        va_start( va, sz );
        vsprintf ( ach, sz, va );   
        va_end( va );
		if(MessageBox(NULL,ach,"Loading error!",MB_RETRYCANCEL|MB_ICONSTOP)==IDCANCEL){
			exit(3);
		};
};
char* RANDPMESS[8]={NULL};
word GetDir(int,int);
#ifndef NEWMORALE
void MakePanicInRadius(int x,int y,byte NI,int r,int FearID,int sx,int sy,bool Strong=0);
int PANIC_STEPS=3;
#else 
int PANIC_STEPS=0;
#endif //NEWMORALE
int MAX_PANIC_STEPS=3;
_inline int GetEnemyDensityInCell(int x,int y,int cell,byte Mask){
    cell+=VAL_MAXCX+1;
	if(NPresence[cell]&(~Mask))return -1;
    int NMon=MCount[cell];
	int s=0;
	if(NMon){
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!(OB->NMask&Mask)){
					int r=Norma(x-(OB->RealX>>8),y-(OB->RealY>>8));
					s+=1000/(r+50);
				};
			};
		};
	};	
	word GetOneBld(int cell,int pos=0);	
	word BMID=GetOneBld(cell);
	if(BMID!=0xFFFF){
		OneObject* OB=Group[BMID];
		if(OB&&OB->NInside&&!(OB->NMask&Mask)){
			int r=Norma(x-(OB->RealX>>8),y-(OB->RealY>>8));
			s+=20000/(r+50);
		};
	};
	return s;
};
int GetEnemyDensity(int x,int y,byte mask){
	int cx0=x>>3;
	int cy0=y>>3;
	int s=0;
	for(int r=0;r<5;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int i=0;i<N;i++){
			int xx=cx0+xi[i];
			int yy=cy0+yi[i];
			if(xx>=0&&yy>=0&&xx<VAL_MAXCX-1&&yy<VAL_MAXCX-1){
				int cell=xx+(yy<<VAL_SHFCX);
				int ss=GetEnemyDensityInCell(x,y,cell,mask);
				if(ss==-1){
					int r=Norma(xx-x,yy-y);
					ss=1000*20/(r+50);
				}
				s+=ss;
			};
		};
	};
	return s;
};
extern int TrueTime;
OneObject* Nation::GetNearestCenter(int UType,int x,int y){
	if(TrueTime-LastCheckTime>20*1000){
		CentIDS.Clear();
		CentSNS.Clear();
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* COM=Group[i];
			if(COM&&(!COM->Sdoxlo)&&COM->newMons->CommandCenter&&COM->NNUM==NNUM){
				CentIDS.Add(COM->Index);
				CentSNS.Add(COM->Serial);
			};
		};
		LastCheckTime=TrueTime;		
	}
	int RR=100000000;
	int bx=-1;
	int by;
	OneObject* CBO=NULL;
	for(int i=0;i<CentIDS.GetAmount();i++){
        word ID=CentIDS[i];
		OneObject* OB=Group[ID];
		if(OB&&OB->Serial==CentSNS[i]){
			int R=Norma(OB->RealX-x,OB->RealY-y);
            bool canP=OB->newMons->GlobalCommandCenter;
			if(UType!=-1 && canP==false){
				for(int i=0;i<NATIONS->PACount[OB->NIndex];i++){
					if(NATIONS->PAble[OB->NIndex][i]==UType){
						canP=true;
						break;
					}
				}
			}
			if(!canP)R+=30000*16;
			if(R<RR){
				RR=R;
				CBO=OB;
			}
		}
	}
	return CBO;
}
void AutoSendFreeUnitsToHome(){
	
	return;

	//if((tmtmt&127)==100){
	if(rando()<128){
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OBJ=Group[i];
			if(OBJ && OBJ->Sdoxlo==0 && (OBJ->LocalOrder==NULL||OBJ->Attack) && OBJ->BrigadeID==0xFFFF && OBJ->UnlimitedMotion==false && OBJ->NotSelectable==true){
				if(MOptions.Players.Player[OBJ->NNUM].DontSelectPanicers){
					OneObject* COB=NATIONS[OBJ->NNUM].GetNearestCenter(-1,OBJ->RealX,OBJ->RealY);
					if(COB && Norma(COB->RealX-OBJ->RealX,COB->RealY-OBJ->RealY)>1000*16){						
						OBJ->Morale=33*10000;						
						OBJ->NewMonsterSmartSendTo(((COB->RealX>>4)+(rando()&127)-64),((COB->RealY>>4)+(rando()&127)-64+180),(rando()&63)-32,(rando()&63)-32,128+16,0);						
						OBJ->Morale=33*10000;			
					}
				}
			}
		}
	}
}
void PanicUnitLink(OneObject* OBJ){
	OBJ->UnlimitedMotion=0;
	OBJ->DestX=-1;
	OBJ->DestY=-1;
	int ps=(OBJ->newMons->Psixoz)<<5;
	OBJ->LocalOrder->info.Patrol.x++;
	extern int Morale_FearDec;
	void AddMoraleInRadius(int x,int y,byte Mask,int R,int D,bool);
	if(OBJ->LocalOrder->info.Patrol.x>1)AddMoraleInRadius(OBJ->RealX>>4,OBJ->RealY>>4,OBJ->NMask,256,-Morale_FearDec,0);
	if((rando()<ps&&OBJ->LocalOrder->info.Patrol.x>2)||OBJ->LocalOrder->info.Patrol.x>MAX_PANIC_STEPS){
		if(((!OBJ->newMons->Peasant)&&OBJ->LocalOrder->info.Patrol.x>PANIC_STEPS)||OBJ->LocalOrder->info.Patrol.x>MAX_PANIC_STEPS){
			OneObject* COB=NATIONS[OBJ->NNUM].GetNearestCenter(OBJ->NIndex,OBJ->RealX,OBJ->RealY);
			if(COB||OBJ->LocalOrder->info.Patrol.x>MAX_PANIC_STEPS){
				OBJ->DeleteLastOrder();
				OBJ->Morale=33*10000;
				if(COB){
					OBJ->NewMonsterSmartSendTo(((COB->RealX>>4)+(rando()&127)-64),((COB->RealY>>4)+(rando()&127)-64+180),(rando()&63)-32,(rando()&63)-32,128+16,0);
				}			
				OBJ->Morale=33*10000;			
			}
		}else{ 
			OBJ->DeleteLastOrder();
			OBJ->Morale=33*10000;
		};
		return;
	};
	int x0=OBJ->x;
	int y0=OBJ->y;
	int mins=1000000;
	int maxsR=0;
	int bx=-1;
	int by=-1;
	
	char* xi=Rarr[3].xi;
	char* yi=Rarr[3].yi;
	int N=Rarr[3].N;
	for(int i=0;i<N;i+=2){
		int x=x0+(xi[i]<<2);
		int y=y0+(yi[i]<<2);
		if(!(CheckBar(x-1,y-1,3,3)||CheckBar((x+x0)/2-1,(y+y0)/2-1,3,3))){
			int sr=GetEnemyDensity(x,y,OBJ->NMask);
			int s=sr+(rando()&7);
			if(s<mins){
				mins=s;
				bx=x;
				by=y;
			};
			if(sr>maxsR)maxsR=sr;
		};
	};
	
	if(!maxsR){
		byte dir=OBJ->LocalOrder->info.Patrol.dir;
		int bx1=x0+(TCos[dir]>>5)+(rando()&31)-15;
		int by1=y0+(TSin[dir]>>5)+(rando()&31)-15;
		if(!(CheckBar(bx1-1,by1-1,3,3)||CheckBar((bx1+x0)/2-1,(by1+y0)/2-1,3,3))){
			bx=bx1;
			by=by1;
		}else{
			OBJ->LocalOrder->info.Patrol.dir=GetDir(bx-x0,by-y0);
		};
	}else{
		OBJ->LocalOrder->info.Patrol.dir=GetDir(bx-x0,by-y0);
	};
	if(bx!=-1){
		if(!RANDPMESS[0]){
			RANDPMESS[0]=GetTextByID("FEAR_RNDM0");
			RANDPMESS[1]=GetTextByID("FEAR_RNDM1");
			RANDPMESS[2]=GetTextByID("FEAR_RNDM2");
			RANDPMESS[3]=GetTextByID("FEAR_RNDM3");
			RANDPMESS[4]=GetTextByID("FEAR_RNDM4");
			RANDPMESS[5]=GetTextByID("FEAR_RNDM5");
			RANDPMESS[6]=GetTextByID("FEAR_RNDM6");
			RANDPMESS[7]=GetTextByID("FEAR_RNDM7");
		};
		bx+=(rando()&7)-3;
		by+=(rando()&7)-3;
		OBJ->PrioryLevel=0;
		OBJ->GroundState=1;
		OBJ->DestX=-1;
		OBJ->DeletePath();
		OBJ->NewMonsterSendTo(bx<<8,by<<8,128,1);
		OBJ->UnlimitedMotion=1;
		byte ms=1<<GSets.CGame.cgi_NatRefTBL[MyNation];
		char* s=RANDPMESS[rand()&7];
		if(HealthMode&&rand()<4500){
			if(!OBJ->newMons->NikakixMam){
				//if(OBJ->NMask&ms)ILSTR.Add(OBJ->RealX>>4,(OBJ->RealY>>5)-OBJ->RZ-90,s,&SmallRedFont,0,0xFFFF8000);
				//ILSTR.Add(OBJ->RealX>>4,(OBJ->RealY>>5)-OBJ->RZ-90,s,&SmallYellowFont,0,0xFF80FF00);
				if(OBJ->NMask&ms)ILSTR.Add(OBJ->RealX>>4,OBJ->RealY>>4,s,&SmallWhiteFont,254,0xFFFF0000);
				else ILSTR.Add(OBJ->RealX>>4,OBJ->RealY>>4,s,&SmallWhiteFont,254,0xFFFFFF00);
			};
		};
	}else{
		OBJ->DeleteLastOrder();
#ifdef NEWMORALE
		OBJ->Morale=33*10000;
#endif
	};
};
void DeleteFromSelection2(OneObject* OB){
    if(!OB)return;
	for(int i=0;i<8;i++){
		if(OB->ImSelected&(1<<i)){
			int Nsel=ImNSL[i];
			word* SMon=ImSelm[i];
			word ID=OB->Index;
			for(int f=0;f<Nsel;f++)if(SMon[f]==ID)SMon[f]=0xFFFF;
			OB->ImSelected&=~GM(i);
		};
		if(OB->Selected&(1<<i)){
			int Nsel=NSL[i];
			word* SMon=Selm[i];
			word ID=OB->Index;
			for(int f=0;f<Nsel;f++)if(SMon[f]==ID)SMon[f]=0xFFFF;
			OB->Selected&=~GM(i);
		};
	};
};
void SetAttState(Brigade* BR,bool Val);
void EraseBrigade(Brigade* BR);
void RemoveFromFormation(OneObject* OB){
	if(OB->BrigadeID!=0xFFFF){
		//removing from formation
		Brigade* BR=OB->Nat->CITY->Brigs+OB->BrigadeID;
		if(BR->WarType){
			int NF=0;
			for(int i=0;i<BR->NMemb;i++){
				word ID=BR->Memb[i];
				if(ID==OB->Index){
					BR->Memb[i]=0xFFFF;
					BR->MembSN[i]=0xFFFF;
					ID=0xFFFF;
					if(i>=NBPERSONAL){
						OneObject* TransformUnitType(OneObject* OB,int DestType);
						if(OB->Ref.General->UsualFormID!=0xFFFF)OB=TransformUnitType(OB,OB->Ref.General->UsualFormID);
					}
				};
				if(ID!=0xFFFF)NF++;
			};
			OB->BrigadeID=0xFFFF;
			OB->BrIndex=0xFFFF;
			OB->InArmy=0;
			OB->AddDamage=0;
			OB->AddShield=0;			
			if(NF<5){
				SetAttState(BR,0);
				EraseBrigade(BR);
			};
		};
	};
};
void PanicUnit(OneObject* OB){
	if(OB->newMons->NoMorale)return;
	if(!ext_OnUnitPanic(OB))return;
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&PanicUnitLink)return;
	addrand(1000);
	Order1* OR=OB->CreateOrder(0);
	if(!OR)return;
	DeleteFromSelection2(OB);
	RemoveFromFormation(OB);
	OR->DoLink=&PanicUnitLink;
	OR->PrioryLevel=255;
	OB->PrioryLevel=255;
	OR->OrderType=12;
	OR->info.Patrol.dir=OB->RealDir;
	OR->info.Patrol.x=0;
	OB->UnitSpeed=64;
	OB->CurUnitSpeed=64;
	OB->GroupSpeed=OB->newMons->MotionDist;
	OB->UnitSpeed=64;
	if(MOptions.Players.Player[OB->NNUM].DontSelectPanicers)OB->NotSelectable=1;
};
extern int tmtmt;
int CommCX[8][16];
int CommCY[8][16];
int NCommC[8]={0,0,0,0,0,0,0,0};
void InitCommC(){
	for(int i=0;i<8;i++)NCommC[i]=0;
};
void EnumCommC(){
	InitCommC();
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->newMons->CommandCenter&&!OB->Sdoxlo){
			byte NI=OB->NNUM;
			if(NI<8&&NCommC[NI]<16){
				CommCX[NI][NCommC[NI]]=OB->RealX;
				CommCY[NI][NCommC[NI]]=OB->RealY;
				NCommC[NI]++;
			};
		};
	};
};
#ifndef NEWMORALE
//----------------------MORALE---------------------
//skill factor p=p0*A1/(A1+NKilled)
int PANIC_A1=1;
//hungry factor p=p0*(1+HF1/(HF2+(t-t_hungry)^2))
int PANIC_HF1=5000;
int PANIC_HF2=1000;
//command center lost factor p=p0*(1+CF1/(CF2+(t-t_lost)^2))
int PANIC_CF1=5000;
int PANIC_CF2=1000;
//officer killed p=p0*OFN1/100
int PANIC_OFN1=200;
//drummer lost p=p0*DRF1/100
int PANIC_DRF1=200;
//znamenonocez lost p=p0*FLF1/100
int PANIC_FLF1=300;
//FP1% of formation killed p=p0*FKF1/100
int PANIC_FP1=30;
int PANIC_FKF1=200;
//formation bonus p=p0*100/(100+NMemb*100/FB1)
int PANIC_FB1=15;
//formation skill bonus p=p0*FA1/(FA1+NKilled)
int PANIC_FA1=5;
//officer skill bonus p=p0*OFA1/(OFA1+NKilled*OFA2)
int PANIC_OFA1=1;
int PANIC_OFA2=5;
//distance from command center p=p0*exp(-(R/CCR0)^2)/CCA;
int PANIC_CCA =5;
int PANIC_CCR0=1000;
int ICON_FEAR0=0;
int ICON_FEAR1=1;
int ICON_FEAR2=2;
int ICON_FEAR3=3;
int ICON_BRAVE=4;
int TBL_MOR_R=160;
int TBL_MOR_G=140;
int TBL_MOR_B=0;
int DAMPERCENT[8]={16384,16384,16384,16384,16384,16384,16384,16384};
int DAMPERCENTBACK[8]={32768,32768,32768,32768,32768,32768,32768,32768};
int PANIC_MINVERA=5;
int PANIC_VERAFACTOR=256;

bool ReadWinString(GFILE* F,char* STR,int Max);
void ReadMoraleData(){
	GFILE* F=Gopen("morale.dat","r");
	if(!F)MessageBox(NULL,"Unable to open MORALE.DAT","ERROR!!!",0);
	else{
		char s[512];
		char var[128];
		int val;
		bool neof=1;
		int line=0;
		do{
			neof=ReadWinString(F,s,512);
			line++;
			if(s[0]!='/'&&neof){
				int z=sscanf(s,"%s%d",var,&val);
				if(z==2){
					if(!strcmp(var,"A1"))PANIC_A1=val;else 
					if(!strcmp(var,"HF1"))PANIC_HF1=val;else 
					if(!strcmp(var,"HF2"))PANIC_HF2=val;else 
					if(!strcmp(var,"CF1"))PANIC_CF1=val;else 
					if(!strcmp(var,"CF2"))PANIC_CF2=val;else 
					if(!strcmp(var,"OFN1"))PANIC_OFN1=val;else 
					if(!strcmp(var,"DRF1"))PANIC_DRF1=val;else 
					if(!strcmp(var,"FLF1"))PANIC_FLF1=val;else 
					if(!strcmp(var,"FP1"))PANIC_FP1=val;else 
					if(!strcmp(var,"FKF1"))PANIC_FKF1=val;else 
					if(!strcmp(var,"FB1"))PANIC_FB1=val;else 
					if(!strcmp(var,"FA1"))PANIC_FA1=val;else 
					if(!strcmp(var,"OFA1"))PANIC_OFA1=val;else 
					if(!strcmp(var,"OFA2"))PANIC_OFA2=val;else
					if(!strcmp(var,"CCR0"))PANIC_CCR0=val;else 
					if(!strcmp(var,"CCA"))PANIC_CCA=val;else
					if(!strcmp(var,"STEPS"))PANIC_STEPS=val;else
					if(!strcmp(var,"MAX_PANIC_STEPS"))MAX_PANIC_STEPS=val;else
					if(!strcmp(var,"ICON_FEAR0"))ICON_FEAR0=val;else
					if(!strcmp(var,"ICON_FEAR1"))ICON_FEAR1=val;else
					if(!strcmp(var,"ICON_FEAR2"))ICON_FEAR2=val;else
					if(!strcmp(var,"ICON_FEAR3"))ICON_FEAR3=val;else
					if(!strcmp(var,"ICON_BRAVE"))ICON_BRAVE=val;else
					if(!strcmp(var,"TBL_MOR_R"))TBL_MOR_R=val;else
					if(!strcmp(var,"TBL_MOR_G"))TBL_MOR_G=val;else
					if(!strcmp(var,"MINVERA"))PANIC_MINVERA=val;else
					if(!strcmp(var,"VERAFACTOR"))PANIC_VERAFACTOR=val;else
					if(!strcmp(var,"DAMPERCENT")){
						for(int j=0;j<8;j++)DAMPERCENT[j]=val*32768/1000;
					}else
					if(!strcmp(var,"DAMPERCENTBACK")){
						for(int j=0;j<8;j++)DAMPERCENTBACK[j]=val*32768/1000;
					}else
					if(!strcmp(var,"DAMPERCENT0"))DAMPERCENT[0]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENT1"))DAMPERCENT[1]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENT2"))DAMPERCENT[2]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENT3"))DAMPERCENT[3]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENT4"))DAMPERCENT[4]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENT5"))DAMPERCENT[5]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENTBACK0"))DAMPERCENTBACK[0]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENTBACK1"))DAMPERCENTBACK[1]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENTBACK2"))DAMPERCENTBACK[2]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENTBACK3"))DAMPERCENTBACK[3]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENTBACK4"))DAMPERCENTBACK[4]=val*32768/1000;else
					if(!strcmp(var,"DAMPERCENTBACK5"))DAMPERCENTBACK[5]=val*32768/1000;else
					if(!strcmp(var,"TBL_MOR_B"))TBL_MOR_B=val;else{
						sprintf(s,"Unknown variable <%s> in MORALE.DAT, line %d",var,line);
						MessageBox(NULL,s,"ERROR!!!",0);
					};
				}else{
					sprintf(s,"Error in MORALE.DAT, line %d",line);
					MessageBox(NULL,s,"ERROR!!!",0);
				};
			};
		}while(neof);
		Gclose(F);
	};
};

int GetPanicProbability0(OneObject* OB,int FearID){
	float pan=float(int(OB->MoreCharacter->FearFactor[FearID]))*32.769;
	return int(pan);
};
int GetPanicProbability(OneObject* OB,int FearID){
	if(FearID<0||FearID>=NFEARSUBJ)return 0;
	float pan=float(int(OB->MoreCharacter->FearFactor[FearID]))*32.769;
	City* CT=OB->Nat->CITY;	
	int HT=CT->LastGolodTime;
	if(HT){
		int dt=tmtmt-HT;
		if(dt>0){
			dt=dt*dt;
			pan=(pan+pan*PANIC_HF1/(PANIC_HF2+dt));
		};
	};
	//p=p0*exp(-(R/CCR0)^2)/CCA
	if(NCommC[OB->NNUM]){
		int MINR=10000000;
		int* XX=CommCX[OB->NNUM];
		int* YY=CommCY[OB->NNUM];
		int NC=NCommC[OB->NNUM];
		for(int j=0;j<NC;j++){
			int R=Norma(XX[j]-OB->RealX,YY[j]-OB->RealY)>>4;
			if(R<MINR){
				MINR=R;
			};
		};
		if(MINR<PANIC_CCR0*4){
			float RR=float(MINR)/float(PANIC_CCR0);
			pan=pan/(1+float(PANIC_CCA-1)*exp(-RR*RR));
		};
	};
	if(OB->VeraVPobedu>PANIC_MINVERA){
#ifdef COSSACKS2
		if(OB->BrigadeID==0xFFFF)pan=pan/(1+((int(OB->VeraVPobedu)*PANIC_VERAFACTOR)>>8));
#else
		pan=pan/(1+OB->VeraVPobedu);
#endif
	};
	if(OB->VeraVPobedu<-PANIC_MINVERA){
#ifdef COSSACKS2
		if(OB->BrigadeID==0xFFFF)pan=pan*(1-((int(OB->VeraVPobedu)*PANIC_VERAFACTOR)>>8));
#else
		pan=pan*(1-OB->VeraVPobedu);
#endif
	};
	HT=CT->LastBarrackDestroyTime;
	if(HT){
		int dt=tmtmt-HT;
		if(dt>0){
			dt=dt*dt;
			pan=(pan+pan*PANIC_CF1/(PANIC_CF2+dt));
		};
	};
	bool InBrig=0;
	if(OB->BrigadeID!=0xFFFF){
		Brigade* BR=CT->Brigs+OB->BrigadeID;
		if(BR->WarType){
			InBrig=1;
			if(BR->Memb[0]==0xFFFF){
				//officer lost
				pan=pan*PANIC_OFN1/100;
			};
			if(BR->Memb[1]==0xFFFF){
				//drummer lost
				pan=pan*PANIC_DRF1/100;
			};
			if(BR->Memb[2]==0xFFFF){
				//drummer lost
				pan=pan*PANIC_FLF1/100;
			};
#ifdef COSSACKS2
			if(BR->VeraVPobedu>PANIC_MINVERA){
				pan=pan/(1+((int(BR->VeraVPobedu)*PANIC_VERAFACTOR)>>8));
			};
			if(BR->VeraVPobedu<-PANIC_MINVERA){
				pan=pan*(1-((int(BR->VeraVPobedu)*PANIC_VERAFACTOR)>>8));
			};
#endif //COSSACKS2
			int NM=BR->NMemb;
			if(NM>2){
				word* Mem=BR->Memb;
				word* MSN=BR->MembSN;
				int N=0;
				for(int i=NCOMM;i<NM;i++)if(Mem[i]!=0xFFFF)N++;
				if(NM>NCOMM){
					int per=(NM-N-NCOMM)*100/(NM-NCOMM);
					if(per>PANIC_FP1){
						//x% of formation if lost
						pan=pan*PANIC_FKF1/100;
					};
				};
				//formation bonus
#ifdef COSSACKS2
				int NK=BR->GetBrigExp();
#else
				int NK=0;
				for(i=0;i<NM;i++){
					word MID=Mem[i];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB&&OB->Serial==MSN[i]){
							NK+=OB->Kills;
						};
					};
				};
#endif
				if(NK)pan=pan*PANIC_FA1/(PANIC_FA1+NK);
				pan=pan*100/(100+((N-NCOMM)*100/PANIC_FB1));
				//officer skill bonus
				word OFID=Mem[0];
				if(OFID!=0xFFFF){
					OneObject* OB=Group[OFID];
					if(OB&&OB->Serial==MSN[0]){
						pan=pan*PANIC_OFA1/(PANIC_OFA1+OB->Kills*PANIC_OFA2);
					};
				};
			};
		};
	};
	if(!InBrig)pan=(pan*PANIC_A1)/(PANIC_A1+OB->Kills);//опыт
	if(pan>10000000)pan=10000000;
	return int(pan);
};
int GetPanicProbability(OneObject* OB,int FearID,bool Active);
void TryToPanicUnit(OneObject* OB,int FearID,int v){
	if(OB->UnlimitedMotion)return;
	if(OB->NewAnm&&!(OB->NewAnm->Code>=anm_Attack&&OB->NewAnm->Code>=anm_Attack<anm_Attack+100))return;
	int p=GetPanicProbability(OB,FearID,1);
#ifdef COSSACKS2
	if(p<327*3)return;
#else
	if(p<327)return;
#endif
	int r=(rando()+(OB->RealX>>4)+(OB->RealY>>4)+OB->Index+OB->StandTime+OB->RealDir+v)&32767;
	if(r<p)PanicUnit(OB);
};
#ifdef COSSACKS2
extern City CITY[8];
#define DECVP(x,OB){if(OB->BrigadeID==0xFFFF){if(x>-126)x--;}else if(FearID==0){short* v=&CITY[OB->NNUM].Brigs[OB->BrigadeID].VeraVPobedu;if((*v)>-126)(*v)--;};}
#define INCVP(x,OB){if((OB->BrigadeID==0xFFFF)&&x< 126)x++;}
#else
#define DECVP(x,y) {x--;}
#define INCVP(x,y) {x++;}
#endif
void TryToPanicInCell(int x,int y,int cell,byte NI,int R,int FearID,int sx,int sy,bool Strong){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int r=Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
				if(r<R){
#ifdef COSSACKS2
					if(NI==OB->NNUM){
						if(FearID==0&&OB->BrigadeID!=0xFFFF){
							r++;
							r--;
						};
						if(Strong)DECVP(OB->VeraVPobedu,OB)
						else{
							char d=GetDir((OB->RealX>>4)-sx,(OB->RealY>>4)-sy)-OB->RealDir;
							if(abs(d)>54){
								if(rando()<DAMPERCENT[FearID])DECVP(OB->VeraVPobedu,OB);
							}else{
								if(rando()<DAMPERCENTBACK[FearID])DECVP(OB->VeraVPobedu,OB);
							};
						};
					};
#else
					if(FearID){
						if(NI==OB->NNUM)DECVP(OB->VeraVPobedu,OB)
						else INCVP(OB->VeraVPobedu,OB);
					};
#endif
					if(OB->NNUM==NI){
						if(r<OB->newMons->FearRadius[FearID]){
							TryToPanicUnit(OB,FearID,x+y+cell+NMon+r);
						};
					};
				};
			};
		};
	};
};
void MakePanicInRadius(int x,int y,byte NI,int r,int FearID,int sx,int sy,bool Strong){
	M_return;
	int cx0=x>>7;
	int cy0=y>>7;
	int s=0;
	int rr=(r>>7)+1;
	for(int v=0;v<=rr;v++){
		char* xi=Rarr[v].xi;
		char* yi=Rarr[v].yi;
		int N=Rarr[v].N;
		for(int i=0;i<N;i++){
			int xx=cx0+xi[i];
			int yy=cy0+yi[i];
			if(xx>=0&&yy>=0&&xx<VAL_MAXCX-1&&yy<VAL_MAXCX-1){
				int cell=xx+(yy<<VAL_SHFCX);
				TryToPanicInCell(x,y,cell,NI,r,FearID,sx,sy,Strong);
			};
		};
	};
};
#ifdef COSSACKS2
void IncFaithInCell(int x,int y,int cell,byte NI,int R){
#ifdef COSSACKS2
	int FearID=1;
#endif
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int r=Norma(x-(OB->RealX>>4),y-(OB->RealY>>4));
				if(r<R){
					if(OB->NNUM==NI)INCVP(OB->VeraVPobedu,OB);
				};
			};
		};
	};
};
void IncreaseFaith(int x,int y,byte NI,int r){
	M_return;
	int cx0=x>>7;
	int cy0=y>>7;
	int s=0;
	int rr=(r>>7)+1;
	for(int v=0;v<=rr;v++){
		char* xi=Rarr[v].xi;
		char* yi=Rarr[v].yi;
		int N=Rarr[v].N;
		for(int i=0;i<N;i++){
			int xx=cx0+xi[i];
			int yy=cy0+yi[i];
			if(xx>=0&&yy>=0&&xx<VAL_MAXCX-1&&yy<VAL_MAXCX-1){
				int cell=xx+(yy<<VAL_SHFCX);
				IncFaithInCell(x,y,cell,NI,r);
			};
		};
	};
};
#endif //COSSACKS2
void EnumCommC();
void DecreaseVeruVPobedu(){
#ifdef COSSACKS2
	int FearID=0;
#endif
	if((tmtmt&255)==61){
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->VeraVPobedu){
				if(OB->VeraVPobedu<0)INCVP(OB->VeraVPobedu,OB)
				else DECVP(OB->VeraVPobedu,OB);
			};
		};
		EnumCommC();
	};
#ifdef COSSACKS2
	if((tmtmt&255)==73){
		for(int i=0;i<7;i++){
			extern City CITY[8];
			Brigade* BR=CITY[i].Brigs;
			for(int j=0;j<MaxBrig;j++){
				if(BR->VeraVPobedu<0)BR->VeraVPobedu++;
				else BR->VeraVPobedu--;
				BR++;
			};
		};
	};
#endif
};
//-------------------MORALE INTERFACE----------------
bool AddTextToDS(DialogsSystem* DSS,int* x,int* y,int* x1,int* y1,
				 RLCFont* PFONT,char* Text,char* Hint){
	int align=0;
	//if(!strcmp(param,"c"))align=1;
	//if(!strcmp(param,"r"))align=2;
	int fdy=GetRLCHeight(PFONT->RLC,'y')+1;
	{
		int y0=*y;
		int x0=*x;
		int Lx=(*x1)-(*x);
		char* s=Text;
		
		int nclw=0;
		int lxlw=0;
		char c=0;
		int pos=0;
		int nsym=0;
		int UsedActives=0;
		bool curactive=0;
		bool wasactive=0;

		char curstr[512];
		int xcs0=0;
		int cslx=0;
		curstr[0]=0;
		int csmaxdy=fdy;
		bool eof=0;
		SimpleDialog* SDS[64];
		int NSDS=0;
		int maxdy=fdy;
		do{
			//reading one word/picture/...
			int nsym=0;
			int ntotsym=0;
			int WLen=0;
			//phrase info
			int gpdx=0;
			int gpdy=0;
			char OneWord[512];
			int GP_Idx=-1;
			int a_sp=0;
			int p_sp=0;
			int reallx=0;
			bool doit=1;
			bool eoln=0;
			wasactive=curactive;
			do{
				c=s[pos];
				int L;
				int lx=GetRLCWidthUNICODE(PFONT->RLC,(byte*)(s+pos),&L);
				if((c==' '&&nsym)||((c=='{'||c=='}'||c=='|'||c==13||c==10||c==0)&&ntotsym)||reallx+lx>Lx){
					//end of phrase
					doit=0;
				}else
				if(!c){
					doit=0;
					eof=1;
				}else
				if(c==13||c==10){
					eoln=1;
					if(s[pos+1]==10||s[pos+1]==13)pos++;
					pos++;
				}else
				if(c=='{'&&!ntotsym){
					curactive=1;
					UsedActives++;
					pos++;
				}else
				if(c=='}'&&!ntotsym){
					curactive=0;
					pos++;
				}else{
					//simply the text
					if(ntotsym+L<500){
						memcpy(OneWord+ntotsym,s+pos,L);
						OneWord[ntotsym+L]=0;
						ntotsym+=L;
						pos+=L;
						reallx+=lx;
						if(c!=' ')nsym++;
					};
				};
			}while(doit);
			if(wasactive!=curactive||GP_Idx!=-1||xcs0+cslx+reallx>Lx||eof||eoln){
				TextButton* TB=DSS->addTextButton(NULL,x0+xcs0,y0,curstr,PFONT,PFONT,PFONT,0);
				if(Hint)TB->Hint=Hint;
				SDS[NSDS]=TB;
				NSDS++;
				//new string?
				if(xcs0+cslx+reallx>Lx||eof||eoln){
					y0+=maxdy;
					xcs0=0;
					curstr[0]=0;
					maxdy=fdy;
					cslx=0;
					if(align){
						int xmin=10000;
						int xmax=-10000;
						for(int i=0;i<NSDS;i++){
							if(SDS[i]->x<xmin)xmin=SDS[i]->x;
							if(SDS[i]->x1>xmax)xmax=SDS[i]->x1;
						};
						if(xmax>xmin){
							if(align==1){
								int dx=(Lx-xmax+xmin)>>1;
								for(int i=0;i<NSDS;i++){
									SDS[i]->x+=dx;
									SDS[i]->x1+=dx;
								};
							};
							if(align==2){
								int dx=(Lx-xmax+xmin);
								for(int i=0;i<NSDS;i++){
									SDS[i]->x+=dx;
									SDS[i]->x1+=dx;
								};
							};
						};
					};
					NSDS=0;
				}else{
					xcs0+=cslx;
					cslx=0;
					curstr[0]=0;
				};
			};
			if(OneWord[0]){
				if(maxdy>csmaxdy)csmaxdy=maxdy;
				int nsp=0;
				if(!(xcs0+cslx||eoln)){
					while(OneWord[nsp]==' ')nsp++;
					int L;
					if(nsp)reallx-=GetRLCWidthUNICODE(PFONT->RLC,(byte*)OneWord,&L)*nsp;
				};
				strcat(curstr,OneWord+nsp);
				cslx+=reallx;
			};
		}while(!eof);
		*y1=y0;
	};
	return true;
};
char* THIS_GEN_INF=NULL;
char* FEAR_INF_BORN=NULL;
char* FEAR_INF_BORNH=NULL;
char* FEAR_TXWIDE[NFEARSUBJ]={NULL};
char* FEAR_TXWIDEH[NFEARSUBJ]={NULL};
char* FEAR_ACTS=NULL;// События, повлиявшие на страх:
char* FEAR_MUL1=NULL;// Личный боевой опыт (убил %d врагов): x%d%%
char* FEAR_MUL1H=NULL;
char* FEAR_MUL2=NULL;// Недавно был голод: x%d%%
char* FEAR_MUL2H=NULL;
char* FEAR_MUL3=NULL;// Недавно был уничтожен командный центр (казарма): x%d%%
char* FEAR_MUL3H=NULL;
char* FEAR_MUL4=NULL;// Убит командир отряда: x%d%%
char* FEAR_MUL4H=NULL;
char* FEAR_MUL5=NULL;// Убит барабанщик отряда: x%d%%
char* FEAR_MUL5H=NULL;
char* FEAR_MUL6=NULL;// Убит флагоносец отряда: x%d%%
char* FEAR_MUL6H=NULL;
char* FEAR_MUL7=NULL;// Потеряно более %d%% отряда: x%d%%
char* FEAR_MUL7H=NULL;
char* FEAR_MUL8=NULL;// Опытность командира (убил %d врагов): x%d%%
char* FEAR_MUL8H=NULL;
char* FEAR_MUL9=NULL;// Опытность отряда (убито %d врагов): x%d%%
char* FEAR_MUL9H=NULL;
char* FEAR_MULA=NULL;
char* FEAR_MULAH=NULL;
char* FEAR_MULB=NULL;
char* FEAR_MULBH=NULL;
char* FEAR_MULC=NULL;
char* FEAR_MULCH=NULL;
char* FEAR_ITOG=NULL;// В итоге:
char* START_MVAL=NULL;

char* CURR_MVAL =NULL;
char* CURR_MVAL0=NULL;
char* CURR_MVAL1=NULL;
char* CURR_MVAL2=NULL;
char* CURR_MVAL3=NULL;
char* CURR_MVAL4=NULL;
char* CURR_MVAL5=NULL;
char* CURR_MVAL6=NULL;

char* FEAR_INC=NULL;
char* FEAR_DEC=NULL;


#define CHKTX(c) if(!c)c=GetTextByID(#c);
byte GetPaletteColor(int r,int g,int b);
int FCOLOR=0;
char* NONSTR="----";
int CreateMoraleInterface(DialogsSystem* DSS,int LX,int y0,OneObject* OB){
	if(!FCOLOR)FCOLOR=132;//GetPaletteColor(TBL_MOR_R,TBL_MOR_G,TBL_MOR_B);
	if(!FEAR_TXWIDE[0]){
		for(int i=0;i<NFEARSUBJ;i++){
			char cc[128];
			sprintf(cc,"FEAR_TXWIDE%d",i);
			FEAR_TXWIDE[i]=GetTextByID(cc);
			if(!strcmp(FEAR_TXWIDE[i],cc))FEAR_TXWIDE[i]=NONSTR;
			sprintf(cc,"FEAR_TXWIDE%dH",i);
			FEAR_TXWIDEH[i]=GetTextByID(cc);
			if(!strcmp(FEAR_TXWIDEH[i],cc))FEAR_TXWIDEH[i]=NONSTR;
		};
	};
	CHKTX(THIS_GEN_INF);
	CHKTX(FEAR_INF_BORN);
	CHKTX(FEAR_INF_BORNH);
	CHKTX(FEAR_ACTS);
	CHKTX(FEAR_INC);
	CHKTX(FEAR_DEC);
	CHKTX(FEAR_MUL1);
	CHKTX(FEAR_MUL1H);
	CHKTX(FEAR_MUL2);
	CHKTX(FEAR_MUL2H);
	CHKTX(FEAR_MUL3);
	CHKTX(FEAR_MUL3H);
	CHKTX(FEAR_MUL4);
	CHKTX(FEAR_MUL4H);
	CHKTX(FEAR_MUL5);
	CHKTX(FEAR_MUL5H);
	CHKTX(FEAR_MUL6);
	CHKTX(FEAR_MUL6H);
	CHKTX(FEAR_MUL7);
	CHKTX(FEAR_MUL7H);
	CHKTX(FEAR_MUL8);
	CHKTX(FEAR_MUL8H);
	CHKTX(FEAR_MUL9);
	CHKTX(FEAR_MUL9H);
	CHKTX(FEAR_MULA);
	CHKTX(FEAR_MULAH);
	CHKTX(FEAR_MULB);
	CHKTX(FEAR_MULBH);
	CHKTX(FEAR_MULC);
	CHKTX(FEAR_MULCH);
	CHKTX(FEAR_ITOG);
	CHKTX(START_MVAL);

	CHKTX(CURR_MVAL );
	CHKTX(CURR_MVAL0);
	CHKTX(CURR_MVAL1);
	CHKTX(CURR_MVAL2);
	CHKTX(CURR_MVAL3);
	CHKTX(CURR_MVAL4);
	CHKTX(CURR_MVAL5);
	CHKTX(CURR_MVAL6);
	int x=6;
	int y=y0;
	int x1=LX;
	int y1=y0;
	AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallWhiteFont,THIS_GEN_INF,NULL);
	y=y1+4;
	y1=y0;
	AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,FEAR_INF_BORN,FEAR_INF_BORNH);
	y=y1+4;
	y1=y;
	int NLX=40;
	x1=LX-NLX-NLX;
	int BLX=x1-x+1;
	char str[64];
	char str0[512];
	ColoredBar* CB;
	int L=0;
	bool CHANGE=0;
	for(int i=0;i<NFEARSUBJ;i++)if(OB->MoreCharacter->FearFactor[i]){
		AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,FEAR_TXWIDE[i],FEAR_TXWIDEH[i]);
		CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
		CB->Style=1;
		CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
		CB->Style=1;
		CB=DSS->addColoredBar(x-4+BLX+NLX,y-2,NLX,y1-y+3,FCOLOR);
		CB->Style=1;
		int sv=GetPanicProbability0(OB,i)*1000/32767;
		sprintf(str,"%d.%d%%",sv/10,sv%10);
		L=GetRLCStrWidth(str,&SmallYellowFont);
		TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
		TB->Hint=START_MVAL;
		int cv=GetPanicProbability(OB,i)*1000/32767;
		sprintf(str,"%d.%d%%",cv/10,cv%10);
		L=GetRLCStrWidth(str,&SmallYellowFont);
		TB=DSS->addTextButton(NULL,x-4+BLX+NLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
		if(cv==sv){
			if(cv<=30){
				TB->Hint=CURR_MVAL0;
			}else if(cv<100){
				TB->Hint=CURR_MVAL2;
			}else if(cv<300){
				TB->Hint=CURR_MVAL4;
			}else{
				TB->Hint=CURR_MVAL6;
			};
		}else{ 
			if(cv<=30){
				TB->Hint=CURR_MVAL;
			}else if(cv<100){
				TB->Hint=CURR_MVAL1;
			}else if(cv<300){
				TB->Hint=CURR_MVAL3;
			}else{
				TB->Hint=CURR_MVAL5;
			};
			CHANGE=1;
		};
		y=y1+3;
		y1=y;
	};
	y+=2;
	y1=y;
	x1=x+LX-1;
	if(CHANGE){
		AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,FEAR_ACTS,NULL);
		NLX=60;
		y=y1+4;
		y1=y;
		x1=LX-NLX;
		BLX=x1-x+1;

		City* CT=OB->Nat->CITY;
		if(NCommC[OB->NNUM]){
			int MINR=10000000;
			int* XX=CommCX[OB->NNUM];
			int* YY=CommCY[OB->NNUM];
			int NC=NCommC[OB->NNUM];
			for(int j=0;j<NC;j++){
				int R=Norma(XX[j]-OB->RealX,YY[j]-OB->RealY)>>4;
				if(R<MINR){
					MINR=R;
				};
			};
			if(MINR<PANIC_CCR0*4){
				float RR=float(MINR)/float(PANIC_CCR0);
				int v=1000.0/(1+float(PANIC_CCA-1)*exp(-RR*RR));
				sprintf(str0,FEAR_MULC,int(MINR));
				AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,str0,FEAR_MULCH);
				CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
				CB->Style=1;
				CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
				CB->Style=1;
				sprintf(str,"x%d.%d%%",v/10,v%10);
				L=GetRLCStrWidth(str,&SmallYellowFont);
				TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
				TB->Hint=FEAR_DEC;
				y=y1+3;
				y1=y;
			};
		};
		if(abs(OB->VeraVPobedu)>PANIC_MINVERA){
			int v;
#ifdef COSSACKS2
			if(OB->BrigadeID==0xFFFF){
				if(OB->VeraVPobedu>0){
					v=1000/(1+((int(OB->VeraVPobedu)*PANIC_VERAFACTOR)>>8));
				}else v=1000*(1-((int(OB->VeraVPobedu)*PANIC_VERAFACTOR)>>8));
				sprintf(str0,FEAR_MULB,OB->VeraVPobedu);
				AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,str0,FEAR_MULBH);
				CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
				CB->Style=1;
				CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
				CB->Style=1;
				sprintf(str,"x%d.%d%%",v/10,v%10);
				L=GetRLCStrWidth(str,&SmallYellowFont);
				TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
				if(OB->VeraVPobedu>0)TB->Hint=FEAR_DEC;
				else TB->Hint=FEAR_INC;
				y=y1+3;
				y1=y;
			};
#else //COSSACKS2
			if(OB->VeraVPobedu>0){
				v=1000/(1+OB->VeraVPobedu);
			}else v=1000*(1-OB->VeraVPobedu);
			sprintf(str0,FEAR_MULB,OB->VeraVPobedu);
			AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,str0,FEAR_MULBH);
			CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
			CB->Style=1;
			CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
			CB->Style=1;
			sprintf(str,"x%d.%d%%",v/10,v%10);
			L=GetRLCStrWidth(str,&SmallYellowFont);
			TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
			if(OB->VeraVPobedu>0)TB->Hint=FEAR_DEC;
			else TB->Hint=FEAR_INC;
			y=y1+3;
			y1=y;
#endif //COSSACKS2
			
		};
		int HT=CT->LastGolodTime;
		if(HT){
			int dt=tmtmt-HT;
			if(dt>0){
				dt=dt*dt;
				int v=(1000+1000*PANIC_HF1/(PANIC_HF2+dt));
				AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,FEAR_MUL2,FEAR_MUL2H);
				CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
				CB->Style=1;
				CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
				CB->Style=1;
				sprintf(str,"x%d.%d%%",v/10,v%10);
				L=GetRLCStrWidth(str,&SmallYellowFont);
				TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
				TB->Hint=FEAR_INC;
				y=y1+3;
				y1=y;
			};
		};
		HT=CT->LastBarrackDestroyTime;
		if(HT){
			int dt=tmtmt-HT;
			if(dt>0){
				dt=dt*dt;
				int v=(1000+1000*PANIC_CF1/(PANIC_CF2+dt));
				AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,FEAR_MUL3,FEAR_MUL3H);
				CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
				CB->Style=1;
				CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
				CB->Style=1;
				sprintf(str,"x%d.%d%%",v/10,v%10);
				L=GetRLCStrWidth(str,&SmallYellowFont);
				TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
				TB->Hint=FEAR_INC;
				y=y1+3;
				y1=y;
			};
		};
		bool InBrig=0;
		if(OB->BrigadeID!=0xFFFF){
			Brigade* BR=CT->Brigs+OB->BrigadeID;
			if(BR->WarType){
				InBrig=1;
				if(BR->Memb[0]==0xFFFF){
					//officer lost
					int v=10*PANIC_OFN1;
					AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,FEAR_MUL4,FEAR_MUL4H);
					CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
					CB->Style=1;
					CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
					CB->Style=1;
					sprintf(str,"x%d.%d%%",v/10,v%10);
					L=GetRLCStrWidth(str,&SmallYellowFont);
					TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
					TB->Hint=FEAR_INC;
					y=y1+3;
					y1=y;
				};
				if(BR->Memb[1]==0xFFFF){
					//drummer lost
					int v=10*PANIC_DRF1;
					AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,FEAR_MUL5,FEAR_MUL5H);
					CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
					CB->Style=1;
					CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
					CB->Style=1;
					sprintf(str,"x%d.%d%%",v/10,v%10);
					L=GetRLCStrWidth(str,&SmallYellowFont);
					TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
					TB->Hint=FEAR_INC;
					y=y1+3;
					y1=y;
				};
				if(BR->Memb[2]==0xFFFF){
					//drummer lost
					int v=10*PANIC_FLF1;
					AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,FEAR_MUL6,FEAR_MUL6H);
					CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
					CB->Style=1;
					CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
					CB->Style=1;
					sprintf(str,"x%d.%d%%",v/10,v%10);
					L=GetRLCStrWidth(str,&SmallYellowFont);
					TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
					TB->Hint=FEAR_INC;
					y=y1+3;
					y1=y;
				};
#ifdef COSSACKS2
				if(abs(BR->VeraVPobedu)>PANIC_MINVERA){
					int v;
					if(BR->VeraVPobedu>0){
						v=1000/(1+((int(BR->VeraVPobedu)*PANIC_VERAFACTOR)>>8));
					}else v=1000*(1-((int(BR->VeraVPobedu)*PANIC_VERAFACTOR)>>8));

					sprintf(str0,FEAR_MULB,BR->VeraVPobedu);
					AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,str0,FEAR_MULBH);
					CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
					CB->Style=1;
					CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
					CB->Style=1;
					sprintf(str,"x%d.%d%%",v/10,v%10);
					L=GetRLCStrWidth(str,&SmallYellowFont);
					TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
					if(OB->VeraVPobedu>0)TB->Hint=FEAR_DEC;
					else TB->Hint=FEAR_INC;
					y=y1+3;
					y1=y;
				};
#endif //COSSACKS2
				int NM=BR->NMemb;
				if(NM>2){
					word* Mem=BR->Memb;
					word* MSN=BR->MembSN;
					int N=0;
					for(int i=NCOMM;i<NM;i++)if(Mem[i]!=0xFFFF)N++;
					if(NM>NCOMM){
						int per=(NM-N-NCOMM)*100/(NM-NCOMM);
						if(per>PANIC_FP1){
							//x% of formation if lost
							int v=10*PANIC_FKF1;
							sprintf(str0,FEAR_MUL7,PANIC_FP1);
							AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,str0,FEAR_MUL7H);
							CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
							CB->Style=1;
							CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
							CB->Style=1;
							sprintf(str,"x%d.%d%%",v/10,v%10);
							L=GetRLCStrWidth(str,&SmallYellowFont);
							TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
							TB->Hint=FEAR_INC;
							y=y1+3;
							y1=y;
						};
					};
#ifdef COSSACKS2
					int NK=BR->GetBrigExp();
#else
					int NK=0;
					for(i=0;i<NM;i++){
						word MID=Mem[i];
						if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB&&OB->Serial==MSN[i]){
								NK+=OB->Kills;
							};
						};
					};
#endif
					if(NK){
						int v=1000*PANIC_FA1/(PANIC_FA1+NK);
						sprintf(str0,FEAR_MUL9,NK);
						AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,str0,FEAR_MUL9H);
						CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
						CB->Style=1;
						CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
						CB->Style=1;
						sprintf(str,"x%d.%d%%",v/10,v%10);
						L=GetRLCStrWidth(str,&SmallYellowFont);
						TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
						TB->Hint=FEAR_DEC;
						y=y1+3;
						y1=y;
					};
					//formation bonus
					int v=100000/(100+((N-NCOMM)*100/PANIC_FB1));
					AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,FEAR_MULA,FEAR_MULAH);
					CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
					CB->Style=1;
					CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
					CB->Style=1;
					sprintf(str,"x%d.%d%%",v/10,v%10);
					L=GetRLCStrWidth(str,&SmallYellowFont);
					TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
					TB->Hint=FEAR_DEC;
					y=y1+3;
					y1=y;

					//officer skill bonus
					word OFID=Mem[0];
					if(OFID!=0xFFFF){
						OneObject* OB=Group[OFID];
						if(OB&&OB->Serial==MSN[0]){
							int v=1000*PANIC_OFA1/(PANIC_OFA1+OB->Kills*PANIC_OFA2);
							sprintf(str0,FEAR_MUL8,OB->Kills);
							AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,str0,FEAR_MUL8H);
							CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
							CB->Style=1;
							CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
							CB->Style=1;
							sprintf(str,"x%d.%d%%",v/10,v%10);
							L=GetRLCStrWidth(str,&SmallYellowFont);
							TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
							TB->Hint=FEAR_DEC;
							y=y1+3;
							y1=y;
						};
					};
				};
			};
		};
		if(!InBrig){
			if(OB->Kills){
				int v=1000*PANIC_A1/(PANIC_A1+OB->Kills);//опыт
				sprintf(str0,FEAR_MUL1,OB->Kills);
				AddTextToDS(DSS,&x,&y,&x1,&y1,&SmallYellowFont,str0,FEAR_MUL1H);
				CB=DSS->addColoredBar(x-4,y-2,BLX,y1-y+3,FCOLOR);
				CB->Style=1;
				CB=DSS->addColoredBar(x-4+BLX,y-2,NLX,y1-y+3,FCOLOR);
				CB->Style=1;
				sprintf(str,"x%d.%d%%",v/10,v%10);
				L=GetRLCStrWidth(str,&SmallYellowFont);
				TextButton* TB=DSS->addTextButton(NULL,x-4+BLX+(NLX-L)/2,y,str,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
				TB->Hint=FEAR_DEC;
				y=y1+3;
				y1=y;
			};
		};
	};
	return y1;
};
//-----------------MORALE CASHING-----------------//
#define MR_Shift 10
#define MR_CashSize (1<<MR_Shift)
#define MR_Mask (MR_CashSize-1)

byte MR_Index[MR_CashSize][NFEARSUBJ];
word FearCValue[MR_CashSize][NFEARSUBJ];
int  FearCTime[MR_CashSize][NFEARSUBJ];

void ClearMoraleCash(){
	memset(MR_Index,0,sizeof MR_Index);
	memset(FearCValue,0,sizeof FearCValue);
	memset(FearCTime,0,sizeof FearCTime);
};
int N1=0;
int N2=0;
int GetPanicProbability(OneObject* OB,int FearID,bool Active){
	if(Active){
		int idx=OB->Index>>MR_Shift;
		int idx1=OB->Index&MR_Mask;
		bool found=0;
		int t0;
		if((t0=FearCTime[idx1][FearID])&&idx==MR_Index[idx1][FearID]&&tmtmt-t0<64){
			N1++;
			return FearCValue[idx1][FearID];
		}else{
			int v=GetPanicProbability(OB,FearID);
			FearCValue[idx1][FearID]=v;
			MR_Index[idx1][FearID]=idx;
			FearCTime[idx1][FearID]=tmtmt;
			N2++;
			return v;
		};
	}else return GetPanicProbability(OB,FearID);
};
#else //NEWMORALE

#define MR_ONE 10000
#define NFTYPES 16
#define SETM(x) int(x*MR_ONE)
int Morale_DamageDec				=SETM(0.3  );
int Morale_DamageDec1               =SETM(0.3  );
int Morale_DamageDec2				=SETM(0.3  );
int Morale_DamageDec3				=SETM(0.3  );
int Morale_DamageDec4				=SETM(0.3  );
int Morale_DamageDec5				=SETM(0.3  );
int Morale_DamageDec6				=SETM(0.3  );
int Morale_DamageDec7				=SETM(0.3  );
int Morale_DamageDec8				=SETM(0.3  );
int Morale_DamageDec9				=SETM(0.3  );
int Morale_DamageDec10				=SETM(0.3  );
int Morale_DamageDec11				=SETM(0.3  );
int Morale_DamageDec12				=SETM(0.3  );
int Morale_DamageDec13				=SETM(0.3  );
int Morale_DamageDec14				=SETM(0.3  );
int Morale_DamageDec15				=SETM(0.3  );

int Morale_BackDamageDec			=SETM(1.0  );
int Morale_BackDamageDec1			=SETM(1.0  );
int Morale_BackDamageDec2			=SETM(1.0  );
int Morale_BackDamageDec3			=SETM(1.0  );
int Morale_BackDamageDec4			=SETM(1.0  );
int Morale_BackDamageDec5			=SETM(1.0  );
int Morale_BackDamageDec6			=SETM(1.0  );
int Morale_BackDamageDec7			=SETM(1.0  );
int Morale_BackDamageDec8			=SETM(1.0  );
int Morale_BackDamageDec9			=SETM(1.0  );
int Morale_BackDamageDec10			=SETM(1.0  );
int Morale_BackDamageDec11			=SETM(1.0  );
int Morale_BackDamageDec12			=SETM(1.0  );
int Morale_BackDamageDec13			=SETM(1.0  );
int Morale_BackDamageDec14			=SETM(1.0  );
int Morale_BackDamageDec15			=SETM(1.0  );

int Morale_FormationShield15		=50;
int Morale_FormationShield196		=30;
int Morale_IncDecCoefficient		=100;

int Morale_KillDec					=SETM(1.0  );
int Morale_FearDec					=SETM(0.25 );
int Morale_IncTime					=SETM(0.001);
int MaxMorale_CenterInc				=SETM(100.0);
int MaxMorale_FormIncPerUnit		=SETM(1.0  );
int MaxMorale_CenterRadius			=2000;
int MaxMorale_FormIncOfficer		=SETM(50.0 );
int MaxMorale_FormIncBaraban		=SETM(50.0 );
int MaxMorale_FormIncFlag			=SETM(50.0 );
int MaxMorale_FormIncPerFrag		=SETM(5.0  );
int MaxMorale_DecWhen30Lost			=SETM(20.0 );
int MaxMorale_LostCriticalPercent	=30;
int Morale_DecWhen30Lost				=SETM(20.0 );

int Morale_MinDueToTired			=SETM(36.0 );
int Morale_DecWhenTired				=SETM(0.05 );

#define GETOB(OB,x) if(x!=0xFFFF){OB=Group[x];if(OB&&OB->Sdoxlo)OB=NULL;}else OB=NULL;
#define READCONSTF(x) if(!strcmp(S,#x)){x=SETM(v);FOUND=1;};
#define READCONSTI(x) if(!strcmp(S,#x)){x=int(v); FOUND=1;};
void ReadMoraleData(){
	GFILE* F;
	if(GSets.CGame.ArcadeMode)F=Gopen("NewMoraleArcade.dat","r");
	else F=Gopen("NewMorale.dat","r");
	if(F){
		int z;
		char S[257];
		float v;
		do{
			z=Gscanf(F,"%s%f",S,&v);
			if(z==2){
				bool FOUND=0;
				READCONSTF(Morale_DamageDec);
				READCONSTF(Morale_DamageDec1);
				READCONSTF(Morale_DamageDec2);
				READCONSTF(Morale_DamageDec3);
				READCONSTF(Morale_DamageDec4);
				READCONSTF(Morale_DamageDec5);
				READCONSTF(Morale_DamageDec6);
				READCONSTF(Morale_DamageDec7);
				READCONSTF(Morale_DamageDec8);
				READCONSTF(Morale_DamageDec9);
				READCONSTF(Morale_DamageDec10);
				READCONSTF(Morale_DamageDec11);
				READCONSTF(Morale_DamageDec12);
				READCONSTF(Morale_DamageDec13);
				READCONSTF(Morale_DamageDec14);
				READCONSTF(Morale_DamageDec15);
				READCONSTF(Morale_BackDamageDec);
				READCONSTF(Morale_BackDamageDec1);
				READCONSTF(Morale_BackDamageDec2);
				READCONSTF(Morale_BackDamageDec3);
				READCONSTF(Morale_BackDamageDec4);
				READCONSTF(Morale_BackDamageDec5);
				READCONSTF(Morale_BackDamageDec6);
				READCONSTF(Morale_BackDamageDec7);
				READCONSTF(Morale_BackDamageDec8);
				READCONSTF(Morale_BackDamageDec9);
				READCONSTF(Morale_BackDamageDec10);
				READCONSTF(Morale_BackDamageDec11);
				READCONSTF(Morale_BackDamageDec12);
				READCONSTF(Morale_BackDamageDec13);
				READCONSTF(Morale_BackDamageDec14);
				READCONSTF(Morale_BackDamageDec15);
				READCONSTF(Morale_KillDec);
				READCONSTF(Morale_FearDec);
				READCONSTF(Morale_IncTime);
				READCONSTF(MaxMorale_CenterInc);
				READCONSTF(MaxMorale_FormIncPerUnit);
				READCONSTI(MaxMorale_CenterRadius);
				READCONSTF(MaxMorale_FormIncOfficer);
				READCONSTF(MaxMorale_FormIncBaraban);
				READCONSTF(MaxMorale_FormIncFlag);
				READCONSTF(MaxMorale_FormIncPerFrag);
				READCONSTF(MaxMorale_DecWhen30Lost);
				READCONSTF(Morale_DecWhen30Lost);
				READCONSTF(Morale_MinDueToTired);
				READCONSTF(Morale_DecWhenTired);
				READCONSTI(Morale_FormationShield15);
				READCONSTI(Morale_FormationShield196);
				READCONSTI(MaxMorale_LostCriticalPercent);
				READCONSTI(Morale_IncDecCoefficient);
				READCONSTI(MAX_PANIC_STEPS);
				if(!FOUND){
					LoadErr("NewMorale.dat : Unknown constant : %s",S);
				};
			};
		}while(z==2);
		Gclose(F);
	};
};
extern City CITY[8];
void TestUnitToEscape(OneObject* OB){
	int M=10000000;
	if(OB->BrigadeID!=0xFFFF){
		Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
		if(BR->WarType){
			M=BR->Morale;
		};
	}else{
		M=OB->Morale;
	};
	if(M<32*10000){
		M/=10000;
		if((M-(OB->Index&15))<=16)PanicUnit(OB);
	};
};
int NBRIG;
int BRIGID[512];
int BRIGSN[512];
int BRIGNT[512];

void AddMoraleInRadiusInCell(int x,int y,int cell,byte Mask,int R,int D,bool ThroughMin){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int r=Norma(x-(OB->RealX>>4),y-(OB->RealY>>4));
				if(r<R){
					if(OB->NMask&Mask){
						if(OB->BrigadeID!=0xFFFF){
							Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
							int N=BR->NMemb;
							if(N>3&&BR->SN!=0xFFFF&&NBRIG<512){
								//if(N>30)N=30;
								N-=3;
								D=D*(Morale_FormationShield15+((N-15)*(Morale_FormationShield196-Morale_FormationShield15))/(196-15))/100;
								if(D<0&&!ThroughMin){
									if(BR->Morale>34*10000)BR->Morale+=D;
								}else BR->Morale+=D;
								if(BR->Morale>BR->MaxMorale)BR->Morale=BR->MaxMorale;
								BRIGID[NBRIG]=OB->BrigadeID;
								BRIGNT[NBRIG]=OB->NNUM;
								BRIGSN[NBRIG]=OB->Serial;
								NBRIG++;
								BR->SN=0xFFFF;
							};
						}else{
							if(D<0&&!ThroughMin){
								if(OB->Morale>34*10000)OB->Morale+=D;
							}else OB->Morale+=D;
						};
					};
				};
			};
		};
	};
};
void AddMoraleInRadius(int x,int y,byte Mask,int R,int D,bool ThroughMin){
	M_return;
	if(D>0)D=D*Morale_IncDecCoefficient/100;
	int cx0=x>>7;
	int cy0=y>>7;
	int s=0;
	int rr=(R>>7)+1;
	NBRIG=0;
	for(int v=0;v<rr;v++){
		char* xi=Rarr[v].xi;
		char* yi=Rarr[v].yi;
		int N=Rarr[v].N;
		for(int i=0;i<N;i++){
			int xx=cx0+xi[i];
			int yy=cy0+yi[i];
			if(xx>=0&&yy>=0&&xx<VAL_MAXCX-1&&yy<VAL_MAXCX-1){
				int cell=xx+(yy<<VAL_SHFCX);
				AddMoraleInRadiusInCell(x,y,cell,Mask,R,D,ThroughMin);
			};
		};
	};
	for(int i=0;i<NBRIG;i++){
		CITY[BRIGNT[i]].Brigs[BRIGID[i]].SN=BRIGSN[i];
	};
};
word GetDir(int,int);
void OnUnitDamage(word MID,word Sender,int FType){
	M_return;
	if(FType==0xFFFF)return;
	OneObject* OB;
	OneObject* SOB;
	GETOB(OB,MID);
	GETOB(SOB,Sender);
	if(OB&&SOB){
		int D=GetDir(SOB->RealX-OB->RealX,SOB->RealY-OB->RealY);
		int DD=char(D-OB->RealDir);
		int DM=0;
		if(abs(DD)<74)DM=(&Morale_DamageDec)[FType];
		else DM=(&Morale_BackDamageDec)[FType];
		AddMoraleInRadius(OB->RealX>>4,OB->RealY>>4,OB->NMask,256,-DM,1);
		AddMoraleInRadius(SOB->RealX>>4,SOB->RealY>>4,SOB->NMask,256,DM,1);
	};
};
void OnUnitDeath(word MID,word Sender){
	M_return;
	OneObject* OB;
	OneObject* SOB;
	GETOB(OB,MID);
	GETOB(SOB,Sender);
	if(OB){
		AddMoraleInRadius(OB->RealX>>4,OB->RealY>>4,OB->NMask,256,-Morale_KillDec,1);
		if(SOB)AddMoraleInRadius(SOB->RealX>>4,SOB->RealY>>4,SOB->NMask,256,Morale_KillDec,1);
	};
};
int GetMaxMorale(OneObject* OB){
	return int(OB->newMons->StartMorale)*MR_ONE;
};
int GetMaxMorale(Brigade* BR,bool Act){
	switch(EngSettings.MoraleType){
		case 0:
			if(BR->WarType){
				int STM=int(BR->CT->Nat->Mon[BR->MembID]->MoreCharacter->StartMorale)*MR_ONE;
				STM+=(BR->NMemb-3)*MaxMorale_FormIncPerUnit;
				STM+=int(BR->GetBrigExp())*MaxMorale_FormIncPerFrag;
				if(BR->Memb[0]!=0xFFFF)STM+=MaxMorale_FormIncOfficer;
				if(BR->Memb[1]!=0xFFFF)STM+=MaxMorale_FormIncBaraban;
				if(BR->Memb[2]!=0xFFFF)STM+=MaxMorale_FormIncFlag;
				int NM=BR->NMemb;
				if(NM>3){
					word* Mem=BR->Memb;
					word* MSN=BR->MembSN;
					int N=0;
					for(int i=NCOMM;i<NM;i++)if(Mem[i]!=0xFFFF)N++;
					int per=(NM-N-NCOMM)*100/(NM-NCOMM);
					if(per>MaxMorale_LostCriticalPercent){
						STM-=MaxMorale_DecWhen30Lost;
						if(Act){
							if(BR->WasLess30Lost)BR->Morale-=Morale_DecWhen30Lost;
							BR->WasLess30Lost=0;
						};
					}else{
						if(Act){
							if(!BR->WasLess30Lost)BR->Morale+=Morale_DecWhen30Lost;
							BR->WasLess30Lost=1;
						};
					};
				};
				STM+=BR->AddMaxMorale;
				return STM;
			}else return 0;
		case 1:
			{
				int NM=0;
				for(int i=0;i<BR->NMemb;i++){
                    if(BR->Memb[NM]!=0xFFFF)NM++;
				}
				return NM*10000;
			}
	}
	return 0;
};
int GetMaxMorale(Brigade* BR){
	return GetMaxMorale(BR,0);
};
extern int tmtmt;
extern byte PauseMode;
extern bool NOPAUSE;
void RemakeMaxMorale(){
	if(EngSettings.MoraleType)return;
	if(!NOPAUSE)return;
	int tc=GetTickCount();
	int ms=32;
	if(MAXOBJECT>16000)ms=128;
	if(MAXOBJECT>32000)ms=256;
	int v=tmtmt&(ms-1);
	word BMIDS[512];
	word BMSN[512];
	byte NATS [512];
	int  NB=0;
	for(int i=v;i<MAXOBJECT;i+=ms){
		OneObject* OB=Group[i];
		if(OB){
			if(OB->BrigadeID==0xFFFF){
				OB->MaxMorale=GetMaxMorale(OB);
				OB->Morale+=Morale_IncTime*GameSpeed/8;
				if(OB->Morale>OB->MaxMorale)OB->Morale=OB->MaxMorale;
			}else{
				Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
				if(BR->SN!=0xFFFF&&NB<512){
					int M0=BR->PrevMorale;
					BMIDS[NB]=OB->BrigadeID;
					NATS[NB]=OB->NNUM;
					BMSN[NB]=BR->SN;
					NB++;
					BR->MaxMorale=GetMaxMorale(BR,1);
					int DM=Morale_IncTime*GameSpeed/256;
#ifdef COSSACKS2
					int mi=100+BR->GetAbilityValue(80);//morale increment speed bonus;
					if(mi<0)mi=0;
					if(mi!=100)DM=(DM*mi)/100;
#endif
					BR->Morale+=DM;
					if(BR->Morale>BR->MaxMorale)BR->Morale=BR->MaxMorale;
					M0/=MR_ONE;
					int BM1=BR->Morale/MR_ONE;					
					static int ptime[256];
					static bool init=0;
					if(!init){
                        for(int i=0;i<256;i++)ptime[i]=tc-20000;
						init=true;
					}
					int cv=(BR->ID+BR->CT->NI*32)&255;
					if(abs(M0-BM1)>1&&tc-ptime[cv]>1500){
						ptime[cv]=tc;
						BR->PrevMorale=BR->Morale;
						if(M0!=BM1&&BR->Morale/MR_ONE<500){
							int xc;
							int yc;
							bool GetBrCenter(Brigade* BR,int* x,int* y);
							BR->GetCenter(&xc,&yc);
							char cc[32];
							int D=-(M0-BM1);
							if(D<0)sprintf(cc,"-%d (%d)",-D,BM1);
							else sprintf(cc,"+%d (%d)",D,BM1);
							ILSTR.Add(xc,yc,cc,&SmallYellowFont,255,0xFFFFFF00);
						};
					}
				};
			};
			TestUnitToEscape(OB);
		};
	};
	for(int i=0;i<NB;i++){
		Brigade* BR=CITY[NATS[i]].Brigs+BMIDS[i];
		BR->SN=BMSN[i];
	};
};
void ShowUnitOpinion(OneObject* OB, char* Text, DWORD Color=0xFFFFFF00){
	if(OB){
		int x=OB->RealX>>4;
		int y=OB->RealY>>4;
		ILSTR.Add(x,y,Text,&SmallYellowFont,255,Color);
	}
}
void CBar(int x,int y,int Lx,int Ly,byte c);
void DrawNewMoraleInterface(int x,int y,OneObject* OB){
	int M,MaxM;
	if(!OB->NewBuilding){
		if(OB->BrigadeID!=0xFFFF){
			M=CITY[OB->NNUM].Brigs[OB->BrigadeID].Morale;
			MaxM=CITY[OB->NNUM].Brigs[OB->BrigadeID].MaxMorale;
		}else{
			M=OB->Morale;
			MaxM=OB->MaxMorale;
		};
		if(MaxM){
			M/=MR_ONE;
			MaxM/=MR_ONE;
			int NP=M/100;
			int MNP=MaxM/100;
			int R=M%100;
			if(NP<0)NP=0;
			if(MNP<0)MNP=0;
			char cc[64]="++++++++++++++++";
			cc[NP]=0;
			int bx0=x+20;
			int by0=y+20;
			int blx=120;
			int bly=16;
			int L=R*blx/100;
			Xbar(bx0-1,by0-1,blx+2,bly+2,clrYello);
			if(NP==0){
				if(R>32){
					int L3=32*blx/100;
					CBar(bx0,by0,L3,bly,clrRed);
					CBar(bx0+L3,by0,L-L3,bly,clrGreen);
				}else{
					
					CBar(bx0,by0,L,bly,clrRed);
				};
			}else{
				CBar(bx0,by0,L,bly,clrGreen);
			};
			if(NP==MNP){
				R=MaxM%100;
				L=R*blx/100;
				Vline(bx0+L,by0+1,by0+bly-1,clrYello);
			};
			ShowString(bx0+4,by0-13,cc,&BigWhiteFont);

			sprintf(cc,"%d/%d",M,MaxM);
			ShowString(bx0+(blx-GetRLCStrWidth(cc,&SmallWhiteFont))/2,by0-14,cc,&SmallWhiteFont);
		};
	};
};
#endif //NEWMORALE