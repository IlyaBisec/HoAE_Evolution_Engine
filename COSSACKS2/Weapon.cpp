#include "stdheader.h"
#include "unitability.h"
//
#include "kContext.h"
#define MaxExpl 8192
#define ExMask (MaxExpl-1)
#define WEPSH 14
void PlayAnimation(NewAnimation* NA,int Frame,int x,int y);
extern byte AlphaR[65536];
extern byte AlphaW[65536];
void ShowRLCItemGrad(int x,int y,lpRLCTable lprt,int n,byte* Pal);
extern word FlyMops[256][256];
int NUCLUSE[4];
bool EUsage[MaxExpl];
word LastAnmIndex;
AnmObject* GAnm[MaxExpl];
short TSin[257]={0,6,12,18,25,31,37,43,49,56,62,68,74,80,86,92,97,103,109,115,120,126,131,136,142,147,152,157,162,167,171,176,181,185,189,193,197,201,205,209,212,216,219,222,225,228,231,234,236,238,241,243,244,246,248,249,251,252,253,254,254,255,255,255,256,255,255,255,254,254,253,252,251,249,248,246,244,243,241,238,236,234,231,228,225,222,219,216,212,209,205,201,197,193,189,185,181,176,171,167,162,157,152,147,142,136,131,126,120,115,109,103,97,92,86,80,74,68,62,56,49,43,37,31,25,18,12,6,0,-6,-12,-18,-25,-31,-37,-43,-49,-56,-62,-68,-74,-80,-86,-92,-97,-103,-109,-115,-120,-126,-131,-136,-142,-147,-152,-157,-162,-167,-171,-176,-181,-185,-189,-193,-197,-201,-205,-209,-212,-216,-219,-222,-225,-228,-231,-234,-236,-238,-241,-243,-244,-246,-248,-249,-251,-252,-253,-254,-254,-255,-255,-255,-256,-255,-255,-255,-254,-254,-253,-252,-251,-249,-248,-246,-244,-243,-241,-238,-236,-234,-231,-228,-225,-222,-219,-216,-212,-209,-205,-201,-197,-193,-189,-185,-181,-176,-171,-167,-162,-157,-152,-147,-142,-136,-131,-126,-120,-115,-109,-103,-97,-92,-86,-80,-74,-68,-62,-56,-49,-43,-37,-31,-25,-18,-12,-6,0};
short TCos[257]={256,255,255,255,254,254,253,252,251,249,248,246,244,243,241,238,236,234,231,228,225,222,219,216,212,209,205,201,197,193,189,185,181,176,171,167,162,157,152,147,142,136,131,126,120,115,109,103,97,92,86,80,74,68,62,56,49,43,37,31,25,18,12,6,0,-6,-12,-18,-25,-31,-37,-43,-49,-56,-62,-68,-74,-80,-86,-92,-97,-103,-109,-115,-120,-126,-131,-136,-142,-147,-152,-157,-162,-167,-171,-176,-181,-185,-189,-193,-197,-201,-205,-209,-212,-216,-219,-222,-225,-228,-231,-234,-236,-238,-241,-243,-244,-246,-248,-249,-251,-252,-253,-254,-254,-255,-255,-255,-256,-255,-255,-255,-254,-254,-253,-252,-251,-249,-248,-246,-244,-243,-241,-238,-236,-234,-231,-228,-225,-222,-219,-216,-212,-209,-205,-201,-197,-193,-189,-185,-181,-176,-171,-167,-162,-157,-152,-147,-142,-136,-131,-126,-120,-115,-109,-103,-97,-92,-86,-80,-74,-68,-62,-56,-49,-43,-37,-31,-25,-18,-12,-6,0,6,12,18,25,31,37,43,49,56,62,68,74,80,86,92,97,103,109,115,120,126,131,136,142,147,152,157,162,167,171,176,181,185,189,193,197,201,205,209,212,216,219,222,225,228,231,234,236,238,241,243,244,246,248,249,251,252,253,254,254,255,255,255,256};
short TAtg[257]={0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,8,8,9,9,9,9,9,9,9,10,10,10,10,10,10,11,11,11,11,11,11,11,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,20,20,20,20,20,20,20,20,20,21,21,21,21,21,21,21,21,22,22,22,22,22,22,22,22,22,23,23,23,23,23,23,23,23,23,24,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,25,26,26,26,26,26,26,26,26,26,26,27,27,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,31,31,31,32};
word ID_FIRE;
word ID_FIREX;
word ID_FIRE1;
word ID_MAGW;
word ID_EXPL1;
word ID_EXPL2;
word ID_GLASSFLY;
word ID_GLASSBROKEN;
word ID_FLYFIRE;
word ID_MAGEXP;
word ID_FOG;
word ID_FOG1;
NewAnimation** FiresAnm[2]={NULL,NULL};
NewAnimation** PreFires[2]={NULL,NULL};
NewAnimation** PostFires[2]={NULL,NULL};
int            NFiresAnm[2];

void FreeFiresAnm(){
	for(int i=0;i<2;i++){
		if(FiresAnm[i])free(FiresAnm[i]);
		if(PreFires[i])free(PreFires[i]);
		if(PostFires[i])free(PostFires[i]);
	};
};
int  NWeaponIDS;
char* WeaponIDS[32];
int GetWeaponType(char* Name){
	for(int i=0;i<NWeaponIDS;i++){
		if(!strcmp(WeaponIDS[i],Name))return i;
	};
	return -1;
};
int mul3(int);
int nEused;
word LastReq;
short randoma[8192];
word rpos;
bool DebugWeapMode=false;
extern int tmtmt;
void LogWeap(char* str,...){
	FILE* logfile=fopen("weapon.log","a");
	if(logfile){
		fprintf(logfile,"%6d : ",tmtmt);
		va_list args;
		va_start(args, str);	
		vfprintf(logfile, str, args);
		vprintf(str, args);
		fflush(logfile);
		va_end(args);
		fclose(logfile);
	}
}

void InitExplosions(){
	for(int i=0;i<MaxExpl;i++)if(!GAnm[i])GAnm[i]=new AnmObject;
	/*
	for(i=0;i<257;i++){
		TSin[i]=short(256*sin(2*3.1415927*i/256));
		TCos[i]=short(256*cos(2*3.1415927*i/256));
		TAtg[i]=short(128*atan2(i,256)/3.1415927);
	};
	FILE* FF=fopen("c:\\tab.txt","w");
	for(i=0;i<257;i++){
		fprintf(FF,"%d,",TSin[i]);
	}
	fprintf(FF,"\n\n");
	for(i=0;i<257;i++){
		fprintf(FF,"%d,",TCos[i]);
	}
	fprintf(FF,"\n\n");
	for(i=0;i<257;i++){
		fprintf(FF,"%d,",TAtg[i]);
	}
	fprintf(FF,"\n\n");
	fclose(FF);
	*/

	memset(&EUsage,0,MaxExpl);
	LastReq=0;
	nEused=0;
	ResFile rf=RReset("random.lst");
	RBlockRead(rf,randoma,16384);
	RClose(rf);
//	for(i=0;i<8192;i++){
//		randoma[i]=rand();
//	};
	rpos=0;
	//ResFile rf=RRewrite("random.lst");
	//RBlockWrite(rf,randoma,16384);
	//RClose(rf);

};
//short rando(){
//	rpos++;
//	rpos&=8191;
//	return randoma[rpos];
//};
//void addrand(int i){;
//	rpos+=i;
//	rpos&=8191;
//};
void CloseExu(int i){
	if(EUsage[i]){
		if(DebugWeapMode)LogWeap("erased %s_%d\n",GAnm[i]->Weap->Name.pchar(),GAnm[i]->ASerial);
		EUsage[i]=false;
		nEused--;
	};
};
void CloseExplosions(){
	for(int i=0;i<MaxExpl;i++)if(GAnm[i]){
		memset(GAnm[i],0,sizeof AnmObject);
	}
};
bool Create3DAnmObject(Weapon* Weap,int xs,int ys,int zs,
					                int xd,int yd,int zd,
									OneObject* OB);
extern int ExplMedia;
extern byte TexMedia[256];
extern byte* WaterDeep;
char WpnChar[228];
char* AOChar(AnmObject* AO){
	return WpnChar;
	int Index=div(int(AO)-int(GAnm),sizeof AnmObject).quot;
	if(AO->Sender){
		sprintf(WpnChar,"ANM:%d,Weap=%d,Sender=%d,x=%d,y=%d,z=%d,Frame=%d",Index,AO->Weap->MyIndex,
			AO->Sender->Index,AO->x,AO->y,AO->z,AO->Frame);
	}else{
		sprintf(WpnChar,"ANM:%d,Weap=%d,Sender=??,x=%d,y=%d,z=%d,Frame=%d",Index,AO->Weap->MyIndex,
			AO->x,AO->y,AO->z,AO->Frame);
	};
	return WpnChar;
};
bool Create3DAnmObjectEX(Weapon* Weap,int xs,int ys,int zs1,
					                int xd,int yd,int zd,
									OneObject* OB,byte AttType,word DestObj,
									int vx,int vy,int vz,int GRDZ);
int DetonationX=-1;
int DetonationY=-1;
int DetonationForce=-1;
void ExplodeAnmObject(AnmObject* AO,bool Landing){
	addrand(AO->ASerial);
	addrand(AO->x);
	addrand(AO->y);
	Weapon* Weap=AO->Weap;
	if(Weap->DetonationForce){
		DetonationForce=Weap->DetonationForce;
		DetonationX=AO->x>>WEPSH;
		DetonationY=AO->y>>WEPSH;
	};
	if(AO->Damage){
		int Damage=AO->Damage;
		if(Weap->FullParent)Damage=Weap->Damage;
		if(AO->Weap->Radius){
			int Damage=AO->Damage;
			if(AO->NTimes>0){
				Damage-=int(Weap->Times-AO->NTimes)*Weap->DamageDec/100;
				if(Damage<=0)Damage=1;
			};
			if(DebugWeapMode)LogWeap("try to damage in radius %s_%d R=%d Dam=%d\n",AO->Weap->Name.pchar(),AO->ASerial,AO->Weap->Radius,Damage);
			MakeRoundDamage(AO->x>>(WEPSH-4),AO->y>>(WEPSH-4),
			AO->Weap->Radius,Damage,AO->Sender,AO->AttType,AO->Weap->Splash);
		};
		if(AO->DestObj!=0xFFFF){
			OneObject* OB=Group[AO->DestObj];
			if(OB&&OB->Serial==AO->DestSN){
				ExplMedia=OB->newMons->ExplosionMedia;
				addrand(7);
				addrand(ExplMedia);
				addrand(AO->Damage);
				addrand(OB->Life);
				addrand(AO->AttType);
				OB->MakeDamage(0,AO->Damage,AO->Sender,AO->AttType);
			};
		};
	};
	if(ExplMedia==-1){
		//water analysing
		int wx=AO->x>>WEPSH;
		int wy=AO->y>>WEPSH;
		if(GetHeight(wx,wy)<=0)ExplMedia=1;			
		if(ExplMedia==-1){
			//texture analysing
			ExplMedia=TexMedia[0];
		};
	};
	ChildWeapon* CWP=NULL;
	int cwtp=-1;
	if(ExplMedia!=-1){
		for(int j=0;j<Weap->NCustomEx;j++){
			ChildWeapon* CW=Weap->CustomEx+j;
			if(CW->Type==ExplMedia){
				CWP=CW;
				cwtp=j;
			};
		};
		if(!CWP)CWP=&Weap->Default;
	};

	if(CWP){
		int rnd=rando();
		int NChild=CWP->MinChild+(((CWP->MaxChild-CWP->MinChild)*rnd)>>15);
		//if(Weap->FullParent){
		for(int i=0;i<NChild;i++){
			bool ok=Landing;
			Weapon* BWpn=NULL;
			int natm=20;
			do{
				BWpn=CWP->Child[(rando()*CWP->NChildWeapon)>>15];
				if(BWpn->Propagation!=7)ok=1;
				natm--;
			}while(natm&&!ok);
			addrand(79);
			if(BWpn){
				if(DebugWeapMode)LogWeap("%s_%d->%s\n",AO->Weap->Name.pchar(),AO->ASerial,BWpn->Name.pchar());
				Create3DAnmObjectEX(BWpn,AO->x>>WEPSH,AO->y>>WEPSH,AO->z>>WEPSH,
						AO->xd>>WEPSH,AO->yd>>WEPSH,AO->zd>>WEPSH,AO->Sender,AO->AttType,AO->DestObj,AO->vx,AO->vy,AO->vz,AO->GraphDZ);
			}
		};
	};
	/*
	}else{
		for(int i=0;i<NChild;i++){
			Weapon* BWpn=CWP->Child[(rando()*CWP->NChildWeapon)>>15];
			Create3DAnmObject(BWpn,AO->x>>WEPSH,AO->y>>WEPSH,AO->z>>WEPSH,
				AO->xd>>WEPSH,AO->yd>>WEPSH,AO->zd>>WEPSH,NULL);
		};
	};
	*/
	if(Weap->NSyncWeapon&&!(ExplMedia!=-1&&CWP!=&Weap->Default)){
		for(int i=0;i<Weap->NSyncWeapon;i++){
			Weapon* BWpn=Weap->SyncWeapon[i];
			addrand(78);
			if(DebugWeapMode)LogWeap("%s_%d->%s\n",AO->Weap->Name.pchar(),AO->ASerial,BWpn->Name.pchar());
			Create3DAnmObjectEX(BWpn,AO->x>>WEPSH,AO->y>>WEPSH,AO->z>>WEPSH,
				AO->xd>>WEPSH,AO->yd>>WEPSH,AO->zd>>WEPSH,AO->Sender,AO->AttType,AO->DestObj,AO->vx,AO->vy,AO->vz,AO->GraphDZ);
		};
	};
	DetonationForce=-1;
	DetonationX=-1;
	DetonationY=-1;
};
void ExplodeAnmObject(AnmObject* AO){
	ExplodeAnmObject(AO,0);
};
int ConvScrY(int x,int y);
void ShowRLCItemDark(int x,int y,lpRLCTable lprt,int n);
extern bool NOPAUSE;
void MiniShowExpl();
void ShowExpl(){
#ifdef _USE3D
	return;
#endif
	if(LMode){
		MiniShowExpl();
		return;
	};
	int ScrX=mapx<<5;
	int ScrY=mul3(mapy<<5)>>2;
	int ScrX1=(mapx+smaplx)<<5;
	int ScrY1=mul3((mapy+smaply)<<5)>>2;
	for(int i=0;i<MaxExpl;i++)if(EUsage[i]){
		if(EUsage[i]){
			AnmObject* AO=GAnm[i];
			Weapon* Weap=AO->Weap;
			int xs=(AO->x>>WEPSH);
			int ys0=mul3(AO->y>>4)>>(WEPSH-2);
			int ys=ys0-(AO->z>>WEPSH);
			int DZ=AO->GraphDZ;
			int xs1=xs;
			xs=xs1;
			NewAnimation* NA=Weap->NewAnm;
			//Visualization
			if(xs>=ScrX&&ys>=ScrY&&xs<=ScrX1&&ys<=ScrY1){
				//determining the direction
				if(AO->Frame>NA->NFrames-1)AO->Frame=NA->NFrames-1;
				NewFrame* NF=NA->Frames[AO->Frame];
				PlayAnimation(NA,int(AO->Frame)<<8,xs,ys);
				int NDir=((NA->Rotations-1)<<1);
				int spr;
				xs-=ScrX-smapx;
				ys-=ScrY-smapy;
				ys0-=ScrY-smapy;
				int oxs=xs;
				//int oys=ys;
				if(NDir){
					double angl;
					bool bdir;
					if(!(AO->vx||AO->vy)){
						angl=atan2(float(AO->xd-AO->x),float(AO->yd-AO->y));
						bdir=AO->xd-AO->x>0;
					}else{
						angl=atan2(float(-AO->vx),float(AO->vy-Prop43(AO->vz)));
						bdir=AO->vx>0;
					};
					//angl+=(3.14152927/NDir);
					if(angl>=3.1415297)angl-=3.14152927*2;
					if(angl<0)angl=-angl;
					spr=angl*double(NDir)/3.14152927;
					if(spr>=NDir)spr=NDir-1;
					spr=(spr+1)>>1;
					if(bdir){
						spr+=4096;
						xs-=NF->dx;
					}else{
						xs+=NF->dx;
					};
				}else{
					spr=0;
					xs+=NF->dx;
				};
				int zz=GetHeight(AO->x>>WEPSH,AO->y>>WEPSH);
				int zz1=(AO->z>>WEPSH)-zz;
				ys+=NF->dy;
				spr+=NF->SpriteID*NA->Rotations;
				if(Weap->HiLayer)ys0+=300;
				//it is visible!
				switch(Weap->Transparency){
				case 1://DARK
					//GPS.ShowGPDark(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0);
					AddOptPoint(ZBF_NORMAL,oxs,ys0+DZ,xs,ys,NULL,NF->FileID,spr,AV_DARK|AV_GRADIENT);
					break;
				case 2://WHITE
					//GPS.ShowGPMutno(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0);
					AddOptPoint(ZBF_NORMAL,oxs,ys0+DZ,xs,ys,NULL,NF->FileID,spr,AV_WHITE|AV_GRADIENT);
					break;
				case 3://RED
					//GPS.ShowGPFired(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0);
					//break;
				case 4://BRIGHT
				case 5://YELLOW
				case 6://ALPHAR
					//GPS.ShowGPGrad(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0,AlphaR);
					//break;
				case 7://ALPHAW
					//GPS.ShowGPGrad(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0,AlphaW);
					//break;
				default:
					//GPS.ShowGP(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0);
					AddOptPoint(ZBF_NORMAL,oxs,ys0+DZ,xs,ys,NULL,NF->FileID,spr,0);
					break;
				};
			};
		};
	};
};
void ProcessFlyBodies();
int TreeHeight=128;
void ProcessExpl(){
	if(!NOPAUSE)return;
	ProcessFlyBodies();
	//int ScrX=mapx<<5;
	//int ScrY=mul3(mapy<<5)>>2;
	//int ScrX1=(mapx+smaplx)<<5;
	//int ScrY1=mul3((mapy+smaply)<<5)>>2;
	//if(!NOPAUSE)return;
	for(int i=0;i<MaxExpl;i++)if(EUsage[i]){
		if(EUsage[i]){

			AnmObject* AO=GAnm[i];
			Weapon* Weap=AO->Weap;
#ifdef _USE3D
			addrand(AO->x);
			addrand(AO->y);
			addrand(AO->z);
			addrand(AO->vx);
			addrand(AO->vy);
			addrand(AO->vz);
			addrand(AO->az);
			if(Weap->FogWeight){
				void AddOneDust(int x, int y);
				int x=(AO->x)>>WEPSH;
				int y=((AO->y)>>WEPSH)-(AO->z>>(WEPSH-1));
				int xd=(AO->xd)>>WEPSH;
				int yd=((AO->yd)>>WEPSH)-(AO->zd>>(WEPSH-1));
				int dx=xd-x;
				int dy=yd-y;
				int N=Norma(dx,dy);
				if(N){
					dx=dx*int(Weap->FogR)/N;
					dy=dy*int(Weap->FogR)/N;
				};
				x+=dx;
				y+=dy;
				int wd=Weap->FogWeight;
				int w16=(wd-1)*32;
				int w10=(wd-1)*24;
				switch(wd){
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					AddOneDust(x-w16,y);
					AddOneDust(x+w16,y);
					AddOneDust(x,y-w16);
					AddOneDust(x,y+w16);
					break;
				case 9:
					AddOneDust(x-w16,y);
					AddOneDust(x+w16,y);
					AddOneDust(x,y-w16);
					AddOneDust(x,y+w16);

					AddOneDust(x-w10,y-w10);
					AddOneDust(x+w10,y-w10);
					AddOneDust(x-w10,y+w10);
					AddOneDust(x+w10,y+w10);
					break;
				case 255:
					AddOneDust(x,y-32);	
					AddOneDust(x,y-64);	
					AddOneDust(x,y-128);	
					AddOneDust(x-64,y-148);	
					AddOneDust(x+64,y-148);	
					AddOneDust(x-64,y-120);	
					AddOneDust(x+64,y-120);	
					break;
				default:
					AddOneDust(x,y);
				};
			};
#endif
			NewAnimation* NA=Weap->NewAnm;
			if(Weap->NTileWeapon){
				int tpp=Weap->TileProbability;
				int vdx=div(AO->vx,tpp).quot;
				int vdy=div(AO->vy,tpp).quot;
				int vdz=div(AO->vz,tpp).quot;
				int xxx=AO->x;
				int yyy=AO->y;
				int zzz=AO->z;
				for(int j=0;j<tpp;j++){
					Weapon* WP=Weap->TileWeapon[(rando()*Weap->NTileWeapon)>>15];
					addrand(77);
					Create3DAnmObject(WP,xxx>>WEPSH,yyy>>WEPSH,zzz>>WEPSH,
						AO->xd>>WEPSH,AO->yd>>WEPSH,AO->zd>>WEPSH,NULL);
					xxx-=vdx;
					yyy-=vdy;
					zzz-=vdz;
				};
			};
			AO->vz+=AO->az;
			AO->x+=AO->vx;
			AO->y+=AO->vy;
			AO->z+=AO->vz;
			addrand(AO->x);
			addrand(AO->y);
			addrand(AO->z);
			addrand(AO->vx);
			addrand(AO->vy);
			addrand(AO->vz);
			addrand(AO->az);
			addrand(AO->x+AO->y+AO->z+AO->vx+AO->vy+AO->vz);			
			int dis=abs(AO->x-AO->xd)+abs(AO->y-AO->yd)+abs(AO->z-AO->zd);
			int wprp=Weap->Propagation;
			if((wprp==3||wprp==5)&&dis<65536*4){
				ExplodeAnmObject(AO,1);
				CloseExu(i);
				goto UUU2;
			}else{
				int ssx=AO->x>>WEPSH;
				int ssy=AO->y>>WEPSH;
				int zz=GetHeight(ssx,ssy);
				int zz0=zz;
				int wpt=Weap->Propagation;
				int BHi=0;
				if(wpt>=2&&wpt<=5){
					BHi=GetBar3DHeightEx(ssx,ssy,AO->vx>>16,AO->vy>>16);
					zz+=BHi;
				};
				int DH=Weap->DamageHeight;
				int AOZ=(AO->z>>WEPSH);
				if(DH&&zz>AOZ-DH-TreeHeight){
					//damage under spot
					if(AO->Damage){
						int Damage=AO->Damage;
						//if(Weap->FullParent)Damage=Weap->Damage;
						if(AO->NTimes>0){
							Damage-=int(Weap->Times-AO->NTimes)*Weap->DamageDec/100;
							if(Damage<=0)Damage=1;
						};
						bool DetectSpriteInPoint(int x,int y,int Damage,OneObject* Sender,int AttType);
						bool EXPL=0;
						
						if((zz>AOZ-DH&&MakeRoundDamage(AO->x>>(WEPSH-4),AO->y>>(WEPSH-4),
							AO->Weap->Radius,Damage,AO->Sender,AO->AttType,AO->Weap->Splash))||
							(EXPL=DetectSpriteInPoint(AO->x>>(WEPSH),AO->y>>(WEPSH),Damage,AO->Sender,AO->AttType))){
							if(Weap->Propagation==8||EXPL){
								ExplodeAnmObject(AO);
								CloseExu(i);
								goto UUU2;
							};
						};
						if(AO->DestObj!=0xFFFF){
							OneObject* OB=Group[AO->DestObj];
							if(OB&&OB->Serial==AO->DestSN){
								ExplMedia=OB->newMons->ExplosionMedia;
								if(Weap->DetonationForce){
									DetonationForce=Weap->DetonationForce;
									DetonationX=AO->x>>WEPSH;
									DetonationY=AO->y>>WEPSH;
								};
								OB->MakeDamage(0,AO->Damage,AO->Sender,AO->AttType);
								if(Weap->Propagation==8){
									ExplodeAnmObject(AO);
									CloseExu(i);
								};
								DetonationForce=-1;
								DetonationX=-1;
								DetonationY=-1;
							};
						};
					};
				};
				if(zz>(AO->z>>WEPSH)){
					//Collision with surface
					if(BHi){
						int IDI=GetBar3DOwner(ssx,ssy);
						if(AO->Sender&&IDI!=-1&&IDI==AO->Sender->Index){
							zz=zz0;
							BHi=0;
							goto UUU1;
						}else{
							OneObject* OB=Group[IDI];
							if(OB){
								AO->DestObj=IDI;
								AO->DestSN=OB->Serial;
							};
							ExplodeAnmObject(AO);
							CloseExu(i);
						};
					}else{
						ExplodeAnmObject(AO,1);
						CloseExu(i);
					};
				}else{
UUU1:/*
				if(FrmDec==2){
					if(AO->NTimes==1&&(AO->Frame>>SpeedSh)==(Weap->HotFrame>>SpeedSh)){
						ExplodeAnmObject(AO);
					};
				}else{
				*/
					int Hf=int(Weap->HotFrame)<<8;
					//if(Hf==0&&AO->PrevLongFrame==0)Hf=1;
					if(AO->NTimes==1&&AO->LongFrame>=Hf&&AO->PrevLongFrame<Hf&&Weap->HotFrame!=254){
						ExplodeAnmObject(AO);
					};					
					if(NA&&AO->Frame>=NA->NFrames-1){
						if(AO->NTimes==1){
							if(Weap->HotFrame==254)ExplodeAnmObject(AO);
							CloseExu(i);
						}else{
							if(AO->NTimes>0)AO->NTimes--;
							AO->Frame=-1;
							AO->LongFrame=0;
							AO->PrevLongFrame=-1;
						};
					};
				};
			};
			addrand(AO->x);
			addrand(AO->y);
			addrand(AO->z);
			addrand(AO->vx);
			addrand(AO->vy);
			addrand(AO->vz);
			addrand(AO->az);
			//AO->Frame++;//=FrmDec;
			AO->PrevLongFrame=AO->LongFrame;
			AO->LongFrame+=GameSpeed;
			AO->Frame=AO->LongFrame>>8;
UUU2:;
		};
	};
};
void MoveAway(int x,int y);
bool TraceObjectsInLine(int xs,int ys,int zs,int* xD,int* yD,int* zD,int damage,OneObject* Sender,byte AttType);
void AddOneDust(int x, int y);
void CreateNewActiveWeapon(Weapon* Weap,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, int Damage, int AttType);
bool Create3DAnmObjectEX(Weapon* Weap,int xs,int ys,int zs1,
					                int xd,int yd,int zd,
									OneObject* OB,byte AttType,word DestObj,
									int vx,int vy,int vz,int GRDZ){
	
	if(!Weap)return false;
	//
	if(Weap->Modificator&&Weap->Modificator->Get()){
		int Inn=0xFFFF;
		if(OB)Inn=OB->Index;
		CreateNewActiveWeapon(Weap,Inn,xs,ys,zs1,DestObj,xd,yd,zd,OB->MoreCharacter->MaxDamage[AttType],AttType);
		return true;
	}
	//
#ifdef _USE3D
	void SetTexRadiusWeighted(int x,int y,int r0,int dr,int Tex,int TexWeight,int Facture,int FactWeight);
	if(Weap->TexRadius){
		SetTexRadiusWeighted(xs,ys,0,Weap->TexRadius,Weap->TextureID,Weap->CenterTexWeight,Weap->FactureID,Weap->CenterTexWeight);
	};
	if(Weap->Projection==1)zs1=GetHeight(xs,ys);
#endif

	if(AttType>=NAttTypes)AttType=0;
	if(Weap->SoundID!=-1){
		AddEffect(xs,ys-zs1*2,Weap->SoundID);
	};
	int zs;
	int hig=GetHeight(xs,ys);
	if(zs1<hig)zs=hig+1;
	else zs=zs1;
	short i=LastReq;
	while(EUsage[i]&&i<MaxExpl)i++;	
	if(i>=MaxExpl){
		i=0;
		while(EUsage[i]&&i<LastReq)i++;
	};
	LastAnmIndex=-1;
	if(EUsage[i])return 0;
	LastAnmIndex=i;
	LastReq=(i+1)&ExMask;
	EUsage[i]=true;
	nEused++;
	AnmObject* AO=GAnm[i];
	AO->ASerial=rando();
	if(DebugWeapMode)LogWeap("created %s_%d\n",Weap->Name.pchar(),AO->ASerial);
	AO->AttType=AttType;
	AO->GraphDZ=GRDZ;
	addrand(AttType);
	if(OB&&Weap->FullParent){
		int nn=OB->newMons->MaxInside;
		if(OB->newMons->ShotPtZ&&nn){
			int N=OB->NInside;
			int ns=0;
			for(int i=0;i<N;i++){
				word MID=OB->Inside[i];
				if(MID!=0xFFFF){
					OneObject* OBX=Group[MID];
					if(OBX&&!OBX->Sdoxlo){
						byte use=OBX->newMons->Usage;
						if(use==StrelokID)ns++;
					};
				};
			};
			int min=OB->newMons->MinDamage[AttType];
			int max=OB->MoreCharacter->MaxDamage[AttType];
			AO->Damage=min+(((max-min)*ns)/nn);
		}else AO->Damage=OB->MoreCharacter->MaxDamage[AttType];
	}else AO->Damage=Weap->Damage;
	AO->Weap=Weap;
	AO->Sender=OB;
	AO->DestObj=0xFFFF;
	AO->DestSN=0xFFFF;
	if(DestObj!=0xFFFF&&Weap->FullParent){
		OneObject* OB=Group[DestObj];
		if(OB){
			AO->DestObj=DestObj;
			AO->DestSN=OB->Serial;
		};
	};
	AO->az=-Weap->Gravity*2000*FrmDec*FrmDec;
	int SPEED=Weap->Speed<<SpeedSh;
	int time,dist;
	switch(Weap->Propagation){
	case 0://STAND
		AO->x=xs<<WEPSH;
		AO->y=ys<<WEPSH;
		AO->z=zs<<WEPSH;
		AO->vx=0;
		AO->vy=0;
		AO->vz=0;
		AO->NTimes=Weap->Times;
		if(AO->Weap->HotFrame==0xFF){
			AO->xd=xd<<WEPSH;
			AO->yd=yd<<WEPSH;
			AO->zd=zd<<WEPSH;
			AO->Frame=0;
			AO->LongFrame=0;
			AO->PrevLongFrame=-1;
			ExplodeAnmObject(AO);
		};
		break;
	case 1://SLIGHTUP
		AO->x=xs<<WEPSH;
		AO->y=ys<<WEPSH;
		AO->z=zs<<WEPSH;
		AO->vx=0;
		AO->vy=0;
		AO->vz=SPEED<<WEPSH;
		AO->NTimes=Weap->Times;
		if(AO->Weap->HotFrame==0xFF){
			AO->xd=xd<<WEPSH;
			AO->yd=yd<<WEPSH;
			AO->zd=zd<<WEPSH;
			AO->Frame=0;
			AO->LongFrame=0;
			AO->PrevLongFrame=-1;
			ExplodeAnmObject(AO);
		};
		break;
	case 7://REFLECT
		{
			vx>>=16;
			vy>>=16;
			vz>>=16;
			int angl=0;
			if(vx||vy||vz){
				int zL=GetHeight(xs-32,ys);
				int zR=GetHeight(xs+32,ys);
				int zU=GetHeight(xs,ys-32);
				int zD=GetHeight(xs,ys+32);
				int nx=zL-zR;
				int ny=zD-zU;
				int nz=32;
				int norma=sqrt(float(nx*nx+ny*ny+nz*nz));
				int vn=2*(vx*nx+vy*ny+vz*nz)/norma;
				int vv=sqrt(float(vx*vx+vy*vy+vz*vz));
				vx-=(vn*nx)/norma;
				vy-=(vn*ny)/norma;
				vz-=(vn*nz)/norma;
				angl=(vn<<5)/vv;
			};
			vx=(vx*Weap->Speed)<<10;
			vy=(vy*Weap->Speed)<<10;
			vz=(vz*Weap->Speed)<<10;
			AO->z=zs<<WEPSH;
			AO->x=xs<<WEPSH;
			AO->y=ys<<WEPSH;
			AO->vx=vx;
			AO->vy=vy;
			AO->vz=vz;
			AO->NTimes=Weap->Times;
			AO->xd=0;
			AO->yd=0;
			AO->zd=0;
			if(abs(angl)>Weap->RandomAngle){
				AO->xd=xd<<WEPSH;
				AO->yd=yd<<WEPSH;
				AO->zd=zd<<WEPSH;
				AO->Frame=0;
				AO->LongFrame=0;
				AO->PrevLongFrame=-1;
				ExplodeAnmObject(AO);
				CloseExu(i);
			};
		};
		break;
	case 6:
	case 2://RANDOM
		AO->x=xs<<WEPSH;
		AO->y=ys<<WEPSH;
		AO->z=zs<<WEPSH;
		AO->vx=((rando()-16384)*SPEED);
		AO->vy=((rando()-16384)*SPEED);
		AO->vz=((rando())*SPEED)>>1;
		AO->NTimes=Weap->Times;
		break;
	case 8:
		{
			AO->x=xs<<WEPSH;
			AO->y=ys<<WEPSH;
			AO->z=zs<<WEPSH;
			dist=sqrt((xs-xd)*(xs-xd)+(ys-yd)*(ys-yd));
			time=div(dist,SPEED).quot;
			AO->NTimes=Weap->Times;
			if(!time)time=1;
			AO->vx=div((xd-xs)<<WEPSH,time+1).quot;
			AO->vy=div((yd-ys)<<WEPSH,time+1).quot;
			AO->vz=div((zd-zs)<<WEPSH,time+1).quot-((AO->az*(time+2))>>1);
			int vx=AO->vx>>8;
			int vy=AO->vy>>8;
			int vz=AO->vz>>8;
			int v=sqrt(vx*vx+vy*vy+vz*vz);
			int d=((int(rando()>>7)-128)*v)>>7;
			d=(d*Weap->RandomAngle)/100;
			AO->vx+=d<<8;
			d=((int(rando()>>7)-128)*v)>>7;
			d=(d*Weap->RandomAngle)/100;
			AO->vy+=d<<8;
			d=((int(rando()>>7)-128)*v)>>7;
			d=(d*Weap->RandomAngle)/100;
			AO->vz+=(d<<8)*Weap->RandomAngleZX/256;
		};
		break;
	case 3://FLY
		AO->x=xs<<WEPSH;
		AO->y=ys<<WEPSH;
		AO->z=zs<<WEPSH;
		dist=sqrt((xs-xd)*(xs-xd)+(ys-yd)*(ys-yd));
		time=div(dist,SPEED).quot;
		AO->NTimes=Weap->Times;
		if(time){
			AO->vx=div((xd-xs)<<WEPSH,time+1).quot;
			AO->vy=div((yd-ys)<<WEPSH,time+1).quot;
			AO->vz=div((zd-zs)<<WEPSH,time+1).quot-((AO->az*(time+2))>>1);
		}else{
#ifdef CONQUEST
			if(!TraceObjectsInLine(xs,ys,zs,&xd,&yd,&zd,AO->Damage,AO->Sender,AO->AttType)){
				if(DestObj!=0xFFFF&&AO->Sender){
					OneObject* OOO=Group[DestObj];
					if(OOO&&OOO->NewBuilding&&!OOO->Sdoxlo){
						int RR=Norma(xd-xs,yd-ys)-OOO->newMons->AddShotRadius;
						if(RR<0)RR=0;
#ifdef COSSACKS2
						int GetDamFall(int x,int x0,int Dam);
						int damage=GetDamFall(RR,AO->Sender->newMons->DamageDecr[AO->AttType],AO->Damage);
#else
						int damage=int(float(AO->Damage)*exp(-0.693147*float(RR)/float(AO->Sender->newMons->DamageDecr[AO->AttType])));
#endif
						if(Weap->DetonationForce){
							DetonationForce=Weap->DetonationForce;
							DetonationX=AO->x>>WEPSH;
							DetonationY=AO->y>>WEPSH;
						};
						OOO->MakeDamage(damage,damage,AO->Sender,AO->AttType);
						DetonationForce=-1;
						DetonationX=-1;
						DetonationY=-1;
					};
				};
			};
			//ExplodeAnmObject(AO);
			AO->vx=0;
			AO->vy=0;
			AO->vz=0;
			AO->x=xd<<WEPSH;
			AO->y=yd<<WEPSH;
			AO->z=zd<<WEPSH;
			AO->xd=xd<<WEPSH;
			AO->yd=yd<<WEPSH;
			AO->zd=zd<<WEPSH;
			AO->Frame=0;
			AO->LongFrame=0;
			AO->PrevLongFrame=-1;
			CloseExu(i);
#else
			AO->vx=0;
			AO->vy=0;
			AO->vz=0;
			AO->x=xd<<WEPSH;
			AO->y=yd<<WEPSH;
			AO->z=zd<<WEPSH;
			AO->xd=xd<<WEPSH;
			AO->yd=yd<<WEPSH;
			AO->zd=zd<<WEPSH;
			AO->Frame=0;
			AO->LongFrame=0;
			AO->PrevLongFrame=-1;
			ExplodeAnmObject(AO);
			CloseExu(i);
#endif
			return true;
		};
		break;
	case 4://IMMEDIATE
		AO->x=xd<<WEPSH;
		AO->y=yd<<WEPSH;
		AO->z=zd<<WEPSH;
		AO->vx=0;
		AO->vy=0;
		AO->vz=0;
		AO->NTimes=Weap->Times;
		break;
	case 5://ANGLE
		{
			int rxy=int(sqrt((xd-xs)*(xd-xs)+(yd-ys)*(yd-ys)));
			int t=zd-zs-int(double(rxy)*tan(Weap->Speed*3.1415/180));
			if(t>=0||!AO->az){
				EUsage[i]=false;
				return false;
			};
			t=4*sqrt(double(t<<(WEPSH-3))/AO->az);
			AO->vx=div((xd-xs)<<WEPSH,t).quot;
			AO->vy=div((yd-ys)<<WEPSH,t).quot;
			int vxy=sqrt(double(AO->vx)*double(AO->vx)+double(AO->vy)*double(AO->vy));
			AO->vz=int(double(vxy)*tan(Weap->Speed*3.1415/180));
			AO->x=xs<<WEPSH;
			AO->y=ys<<WEPSH;
			AO->z=zs<<WEPSH;
			AO->NTimes=Weap->Times;
			//if(AO->Damage>200){
			//MoveAway(xd<<4,yd<<4);
			//};
		};
		break;
	default:
		assert(0);
	};
	AO->xd=xd<<WEPSH;
	AO->yd=yd<<WEPSH;
	AO->zd=zd<<WEPSH;
	AO->Frame=0;
	AO->LongFrame=0;
	AO->PrevLongFrame=-1;
	return true;
};
bool Create3DAnmObject(Weapon* Weap,int xs,int ys,int zs1,
					                int xd,int yd,int zd,
									OneObject* OB,byte AttType,word DestObj){
	return Create3DAnmObjectEX(Weap,xs,ys,zs1,xd,yd,zd,OB,AttType,DestObj,0,0,0,0);
};
bool Create3DAnmObject(Weapon* Weap,int xs,int ys,int zs1,
					                int xd,int yd,int zd,
									OneObject* OB){
	return Create3DAnmObjectEX(Weap,xs,ys,zs1,xd,yd,zd,OB,0,0xFFFF,0,0,0,0);
};
void ChildWeapon::InitChild(){
	MinChild=0;
	MaxChild=0;
	NChildWeapon=0;
	for(int i=0;i<MaxChildWeap;i++){
		Child[i]=NULL;
	};
};
bool CheckWpn(Weapon* WP){
	int wpt=WP->Propagation;
	if(wpt>=3&&wpt<=5)return true;
	return false;
};
Weapon* ReturnFlyChild(Weapon* Weap){
	if(CheckWpn(Weap))return Weap;
	ChildWeapon* CWP=&Weap->Default;
	for(int i=0;i<CWP->NChildWeapon;i++){
		Weapon* CW=CWP->Child[i];
		if(CheckWpn(CW))return CW;
	};
	for(int i=0;i<Weap->NSyncWeapon;i++){
		Weapon* SW=Weap->SyncWeapon[i];
		if(CheckWpn(SW))return SW;
	};
	return NULL;
};
int CheckWpPoint(int x,int y,int z,word Index){
	int zg=GetBar3DHeight(x,y);
	int zL;
	if(zg<20){
		zL=-45;
	}else zL=GetHeight(x,y);
	if(zg+zL<=z)return -1;
	if(zg){
		word Own=GetBar3DOwner(x,y);
		if(Own!=Index)return Own;
		else if(zL<z)return -1;

	}else ;
	return 0xFFFF;
};
int CheckWpLine(int xs,int ys,int zs,int xd,int yd,int zd,word Index){
	int v=Norma(xd-xs,yd-ys);
	if(!v)return -1;
	int N=(v>>6)+1;
	int vx=((xd-xs)<<WEPSH)/N;
	int vy=((yd-ys)<<WEPSH)/N;
	int vz=((zd-zs)<<WEPSH)/N;
	xs<<=WEPSH;
	ys<<=WEPSH;
	zs<<=WEPSH;
	for(int i=0;i<N;i++){
		int res=CheckWpPoint(xs>>WEPSH,ys>>WEPSH,zs>>WEPSH,Index);
		if(res!=-1)return res;
		xs+=vx;
		ys+=vy;
		zs+=vz;
	};
	return -1;
};
bool ShotRecommended;
int PredictShot(Weapon* Weap,int xs,int ys,int zs,int xd,int yd,int zd,word Index){
	if(Weap->Modificator!=NULL)return -1;
	Weapon* WP=ReturnFlyChild(Weap);
	if(WP){
		int x,y,z,xf,yf,zf,vx,vy,vz,g;
		g=-WP->Gravity*2000*FrmDec*FrmDec;
		int SPEED=WP->Speed<<SpeedSh;
		x=xs<<WEPSH;
		y=ys<<WEPSH;
		z=zs<<WEPSH;
		xf=xd<<WEPSH;
		yf=yd<<WEPSH;
		zf=zd<<WEPSH;
		int N=0;
		int dist,time;
		bool NoCheck=1;
		switch(WP->Propagation){
		case 3://FLY
			dist=sqrt((xs-xd)*(xs-xd)+(ys-yd)*(ys-yd));
			time=div(dist,SPEED).quot;
			vx=div((xd-xs)<<WEPSH,time+1).quot;
			vy=div((yd-ys)<<WEPSH,time+1).quot;
			vz=div((zd-zs)<<WEPSH,time+1).quot-((g*(time+2))>>1);
			if(!time)NoCheck=0;
			break;
		case 4://IMMEDIATE
			vx=xf-x;
			vy=yf-y;
			vz=zf-z;
			NoCheck=0;
			break;
		case 5://ANGLE
			{
				int rxy=int(sqrt((xd-xs)*(xd-xs)+(yd-ys)*(yd-ys)));
				int t1=zd-zs-int(double(rxy)*tan(WP->Speed*3.1415/180));
				if(t1>=0)return 0xFFFE;
				int t=4*sqrt(double(t1<<(WEPSH-3))/g);
				vx=div((xd-xs)<<WEPSH,t).quot;
				vy=div((yd-ys)<<WEPSH,t).quot;
				int vxy=sqrt(double(vx)*double(vx)+double(vy)*double(vy));
				vz=int(double(vxy)*tan(WP->Speed*3.1415/180));
			};
			break;
		};
		if(NoCheck)return -1;
		//process checking
		if(abs(vx>>WEPSH)+abs(vy>>WEPSH)+abs(vz>>WEPSH)<200)ShotRecommended=true;
		else ShotRecommended=false;
		int x0=x;
		int y0=y;
		int z0=z;
		int dst=0;
		do{
			vz+=g;
			int x1=x0+vx;
			int y1=y0+vy;
			int z1=z0+vz;
			int LRes=CheckWpLine(x0>>WEPSH,y0>>WEPSH,z0>>WEPSH,x1>>WEPSH,y1>>WEPSH,z1>>WEPSH,Index);
			if(LRes!=-1)return LRes;
			x0=x1;y0=y1;z0=z1;
			dst=Norma(x0-xf,y0-yf);
		}while(dst>(4<<WEPSH));
	};
	return -1;
};
bool TraceObjectsInLine(int xs,int ys,int zs,int* xD,int* yD,int* zD,int damage,OneObject* Sender,byte AttType){
	int MINR=Sender->newMons->FreeShotDist+Sender->newMons->AddShotRadius;
	int MINR2=MINR+100;
	bool Friend=0;
	if(Sender&&Sender->newMons->FriendlyFire&&!Sender->FriendlyFire)Friend=1;
	int cx=-1;
	int cy=-1;

	int dx=*xD-xs;
	int dy=*yD-ys;
	int dz=*zD-zs;

	int Len=sqrt(dx*dx+dy*dy+dz*dz);
	if(!Len)return false;
	int N=(Len>>4)+1;
	int N2=N+N;
	int MinR=10000;
	word BestID=0xFFFF;
	int SMID=0xFFFF;
	byte MASK=0;
	if(Sender){
		SMID=Sender->Index;
		MASK=Sender->NMask;
	};
	int NOID=0xFFFF;
	if(Sender)NOID=Sender->Index;
	int LastX=0;
	int LastY=0;
	for(int i=0;i<N2&&MinR;i++){
		int xx=(dx*i)/N;
		int yy=(dy*i)/N;
		LastX=xs+xx;
		LastY=ys+yy;
		int zz=zs+(dz*i)/N;
		int H=GetBar3DHeightEx(xs+xx,ys+yy,dx,dy,xs*13+ys*19);
		int H0=GetHeight(xs+xx,ys+yy);
#ifdef COSSACKS2
		bool DetectSpriteInPoint(int x,int y,int Damage,OneObject* Sender,int AttType);
		if(i*Len/N>180 && BestID==0xFFFF && DetectSpriteInPoint(xs+xx,ys+yy,damage,Sender,AttType))return false;
#endif //COSSACKS2
		if(H+H0>zz&&H){
			int ID=GetBar3DOwner(xs+xx,ys+yy);
			if(ID>=0&&ID<0xFFFF&&ID!=NOID){
				//BestID=ID;
				//MinR=0;
				if(BestID==0xFFFF){
					void ViewDamage(int x,int y,int value);
					ViewDamage(LastX,LastY,0);//D);
				}
				break;
			};
		};
		int ccx=(xs+xx)>>7;
		int ccy=(ys+yy)>>7;
		if(cx!=ccx||cy!=ccy){
			cx=ccx;
			cy=ccy;
			if(cx>=0&&cy>=0&&cx<VAL_MAXCX-1&&cy<VAL_MAXCX-1){
				int cell=1+VAL_MAXCX+(cy<<VAL_SHFCX)+cx;
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF&&MID!=SMID){
							OneObject* OB=Group[MID];
							if(OB&&!OB->Sdoxlo){
								int ux=OB->GetAttX()>>4;
								int uy=OB->GetAttY()>>4;
								int R=Norma(ux-xs,uy-ys);
								int dz=zz-OB->RZ;
								int minR=1;//60;
								bool MyUnit=0;
								if(OB->NMask&MASK){
									minR=MINR;
									MyUnit=1;
								};
								if(dz>0&&dz<90&&R>minR&&R<MinR){
									int r=abs((ux-xs)*yy-(uy-ys)*xx)/Len;
									r=r-(r>>2);
									int s=(ux-xs)*dx+(uy-ys)*dy;
									if(s>0){
										if(MyUnit&&R<MINR2)r<<=1;
										if(r<OB->newMons->UnitRadius){
											BestID=MID;
											MinR=R;
											//i=N2;//breaking the cycle
										};
									}
								}
							};
						};
					};
				};
			};
		};
	};
	if(BestID!=0xFFFF){
		OneObject* OB=Group[BestID];
		if(OB){
			if(((OB->NewBuilding&&OB->NInside)||!OB->NewBuilding)){
				*xD=(OB->GetAttX()>>4);
				*yD=(OB->GetAttY()>>4);
				if(Sender&&AttType<NAttTypes){
					int RR=Norma(*xD-xs,*yD-ys);
	#ifdef COSSACKS2
					int GetDamFall(int x,int x0,int Dam);
					damage=GetDamFall(RR,Sender->newMons->DamageDecr[AttType],damage);
	#else
					damage=int(float(damage)*exp(-0.693147*float(RR)/float(Sender->newMons->DamageDecr[AttType])));
	#endif
				}
				if(Friend){
					if(!(Sender->NMask&OB->NMask))OB->MakeDamage(damage,damage,Sender,AttType);
				}else{
					OB->MakeDamage(damage,damage,Sender,AttType);
				}
			}else{
				int D=damage;
				if(Sender&&AttType>=0&&AttType<16){
					int GetDamFall(int x,int x0,int Dam);
					int RR=Norma(LastX-Sender->RealX/16,LastY-Sender->RealY/16);
					D=GetDamFall(RR,Sender->newMons->DamageDecr[AttType],D);
				}
				if (!(Sender->IsInFog)){
				void ViewDamage(int x,int y,int value);
				ViewDamage(LastX,LastY,0);//D);
				}
			}
		};
		return true;
	};
	return false;
};
int CheckFriendlyUnitsInLine(int xs,int ys,int zs,int* xD,int* yD,int* zD,byte MASK,OneObject* Sender){
	int MINR=Sender->newMons->FreeShotDist;
	int MINR2=MINR+100;
	int cx=-1;
	int cy=-1;

	int dx=*xD-xs;
	int dy=*yD-ys;
	int dz=*zD-zs;

	int Len=sqrt(dx*dx+dy*dy+dz*dz);
	if(!Len)return -1;
	int N=(Len>>5)+1;
	int N2=N+N;
	int MinR=10000;
	word BestID=0xFFFF;
	int SMID=0xFFFF;
	int prevcell=-1;
	for(int i=0;i<N2;i++){
		int xx=(dx*i)/N;
		int yy=(dy*i)/N;
		int zz=zs+(dz*i)/N;
		int ccx=(xs+xx)>>7;
		int ccy=(ys+yy)>>7;
		if(cx!=ccx||cy!=ccy){
			cx=ccx;
			cy=ccy;
			if(cx>=0&&cy>=0&&cx<VAL_MAXCX-1&&cy<VAL_MAXCX-1){
				int cell=1+VAL_MAXCX+(cy<<VAL_SHFCX)+cx;
				if(cell!=prevcell){
					prevcell=cell;
					int NMon=MCount[cell];
					if(NMon){
						int ofs1=cell<<SHFCELL;
						word MID;
						for(int i=0;i<NMon;i++){
							MID=GetNMSL(ofs1+i);
							if(MID!=0xFFFF&&MID!=SMID){
								OneObject* OB=Group[MID];
								if(OB&&!OB->Sdoxlo){
									int ux=OB->GetAttX()>>4;
									int uy=OB->GetAttY()>>4;
									int R=Norma(ux-xs,uy-ys);
									int dz=zz-OB->RZ;
									int minR=1;//60;
									bool MyUnit=0;
									if(OB->NMask&MASK){
										minR=MINR;
										MyUnit=1;
									};
									if(dz>0&&dz<90&&R>minR&&R<MinR){
										int r=abs((ux-xs)*yy-(uy-ys)*xx)/Len;
										r=r-(r>>2);
										int s=(ux-xs)*xx+(uy-ys)*yy;
										if(s>0){
											if(MyUnit&&R<MINR2)r<<=1;
											if(r<OB->newMons->UnitRadius){
												BestID=MID;
												MinR=R;
											};
										}
									}
								}
							}
						}
					}
				}
			}
		}
	};
	if(BestID!=0xFFFF){
		OneObject* OB=Group[BestID];
		if(OB->NMask&MASK)return BestID;
	};
	return -1;
};
int PredictShotEx(Weapon* Weap,int xs,int ys,int zs,int xd,int yd,int zd,word Index){
	OneObject* OBJ=Group[Index];
	if(OBJ&&OBJ->newMons->FriendlyFire&&!OBJ->FriendlyFire){
		int r=CheckFriendlyUnitsInLine(xs,ys,zs,&xd,&yd,&zd,OBJ->NMask,Group[Index]);
		if(r==-1){
			return PredictShot(Weap,xs,ys,zs,xd,yd,zd,Index);
		}else return r;
	}else return PredictShot(Weap,xs,ys,zs,xd,yd,zd,Index);
};
void MiniShowExpl(){
	int ScrX=mapx<<5;
	int ScrY=mapy<<4;
	int ScrX1=(mapx+smaplx)<<5;
	int ScrY1=(mapy+smaply)<<4;
	for(int i=0;i<MaxExpl;i++)if(EUsage[i]){
		if(EUsage[i]){
			AnmObject* AO=GAnm[i];
			Weapon* Weap=AO->Weap;
			int xs=(AO->x>>WEPSH);
			int ys0=mul3(AO->y>>4)>>(WEPSH-2);
			int ys=ys0-(AO->z>>WEPSH);
			int DZ=AO->GraphDZ>>2;
			int xs1=xs;
			xs=xs1;
			NewAnimation* NA=Weap->NewAnm;
			//Visualization
			if(xs>=ScrX&&ys>=ScrY&&xs<=ScrX1&&ys<=ScrY1){
				//determining the direction
				if(AO->Frame>NA->NFrames-1)AO->Frame=NA->NFrames-1;
				NewFrame* NF=NA->Frames[AO->Frame];
				PlayAnimation(NA,int(AO->Frame)<<8,xs,ys);
				int NDir=((NA->Rotations-1)<<1);
				int spr;
				xs-=ScrX-smapx;
				ys-=ScrY-smapy;
				ys0-=ScrY-smapy;
				int oxs=xs;
				//int oys=ys;
				if(NDir){
					double angl;
					bool bdir;
					if(!(AO->vx||AO->vy)){
						angl=atan2(AO->xd-AO->x,AO->yd-AO->y);
						bdir=AO->xd-AO->x>0;
					}else{
						angl=atan2(-AO->vx,AO->vy-Prop43(AO->vz));
						bdir=AO->vx>0;
					};
					//angl+=(3.14152927/NDir);
					if(angl>=3.1415297)angl-=3.14152927*2;
					if(angl<0)angl=-angl;
					spr=angl*double(NDir)/3.14152927;
					if(spr>=NDir)spr=NDir-1;
					spr=(spr+1)>>1;
					if(bdir){
						spr+=4096;
						xs-=NF->dx;
					}else{
						xs+=NF->dx;
					};
				}else{
					spr=0;
					xs+=NF->dx;
				};
				xs>>=2;
				ys>>=2;
				oxs>>=2;
				ys0>>=2;
				DZ>>=2;
				int zz=GetHeight(AO->x>>WEPSH,AO->y>>WEPSH);
				int zz1=((AO->z>>WEPSH)-zz)>>2;
				ys+=NF->dy>>2;
				spr+=NF->SpriteID*NA->Rotations;
				if(Weap->HiLayer)ys0+=300;
				//it is visible!
				switch(Weap->Transparency){
				case 1://DARK
					//GPS.ShowGPDark(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0);
					AddOptPoint(ZBF_NORMAL,oxs,ys0+DZ,xs,ys,NULL,NF->FileID,spr,AV_DARK|AV_GRADIENT);
					break;
				case 2://WHITE
					if(NF->FileID){
						//GPS.SetWhiteFont(NF->FileID);
					};
					//GPS.ShowGPMutno(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0);
					AddOptPoint(ZBF_NORMAL,oxs,ys0+DZ,xs,ys,NULL,NF->FileID,spr,AV_WHITE|AV_GRADIENT);
					break;
				case 3://RED
					//GPS.ShowGPFired(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0);
					//break;
				case 4://BRIGHT
				case 5://YELLOW
				case 6://ALPHAR
					//GPS.ShowGPGrad(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0,AlphaR);
					//break;
				case 7://ALPHAW
					//GPS.ShowGPGrad(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0,AlphaW);
					//break;
					if(NF->FileID)
					{
						//GPS.SetWhiteFont(NF->FileID);
					};
				default:
					//GPS.ShowGP(smapx+xs-ScrX,smapy+ys-ScrY,NF->FileID,spr,0);
					AddOptPoint(ZBF_NORMAL,oxs,ys0+DZ,xs,ys,NULL,NF->FileID,spr,0);
					break;
				};
			};
		};
	};
};
//-----------------------FLYING BODY PHYSICS------------------//

struct OneFlyBody{
	word Index;
	word Serial;
	byte Stage;
	int X,Y,Z,Vx,Vy,Vz;
};
#define MaxFlyBod 128
OneFlyBody FBODS[MaxFlyBod];
int NFlyBodies=0;
void AddFlyBody(word Index,int vx,int vy,int vz){
	if(NFlyBodies<MaxFlyBod){
		for(int i=0;i<NFlyBodies;i++)if(FBODS[i].Index==Index)return;
		OneObject* OB=Group[Index];
		if(OB){
			FBODS[NFlyBodies].Index=Index;
			FBODS[NFlyBodies].Serial=OB->Serial;
			FBODS[NFlyBodies].X=OB->RealX<<4;
			FBODS[NFlyBodies].Y=OB->RealY<<4;
			FBODS[NFlyBodies].Z=(GetHeight(OB->RealX>>4,OB->RealY>>4)+1)<<8;
			FBODS[NFlyBodies].Vx=vx;
			FBODS[NFlyBodies].Vy=vy;
			FBODS[NFlyBodies].Vz=vz;
			FBODS[NFlyBodies].Stage=0;
			NFlyBodies++;
		};
	};
};
bool IsFlyBody(word index)
{
	for(int i=0;i<NFlyBodies;i++)
		if(FBODS[i].Index==index)
			return true;
	return false;
}
void ProcessFlyBodies(){
	OneFlyBody* FB=FBODS;
	for(int i=0;i<NFlyBodies;i++){
		bool valid=0;
		OneObject* OB=Group[FB->Index];
		if(OB&&OB->Serial==FB->Serial){
			FB->X+=FB->Vx;
			FB->Y+=FB->Vy;
			FB->Z+=FB->Vz;
			FB->Vz-=200;
			int z=GetHeight(FB->X>>8,FB->Y>>8);
			if(z<(FB->Z>>8)){
				valid=1;
			};
			OB->RealX=FB->X>>4;
			OB->RealY=FB->Y>>4;
			OB->RZ=FB->Z>>8;
			addrand(OB->RZ);
		};
		if(valid){
			FB++;
		}else{
			if(i<NFlyBodies-1)memcpy(FBODS+i,FBODS+i+1,(NFlyBodies-i-1)*sizeof OneFlyBody);
			i--;
			NFlyBodies--;
		};
	};
};
void PushUnitBack(OneObject* OB,byte OrdType,int Force);
void DetonateUnit(OneObject* OB,int CenterX,int CenterY,int Force){
	int DX=(OB->RealX>>4)-CenterX;
	int DY=(OB->RealY>>4)-CenterY;
	int N=int(sqrt(DX*DX+DY*DY));
	if(N){
        int cr=OB->CollisionRadius+1;
        Force=Force*16*16/cr/cr;
		DX=(DX*Force*20)/N/(20+N);
		DY=(DY*Force*20)/N/(20+N);
		int DZ=Force*20/(20+N);		
        //PushUnitBack(OB,0,Force/100);
        NewAnimation* NA=OB->newMons->GetAnimation(anm_FallDown);
        if(NA&&NA->Enabled&&NA->Code!=OB->NewAnm->Code){
            OB->NewAnm=NA;
            OB->SetZeroFrame();
            OB->ClearOrders();
            OB->DestX=-1;
        }
        AddFlyBody(OB->Index,DX,DY,DZ);
	};
};
void InitBodies(){
	NFlyBodies=0; 
	int DetonationX=-1;
	DetonationY=-1;
	DetonationForce=-1;
};
bool DetectSpriteInPointInCell(int cell,int x,int y){
    if(cell<0||cell>=VAL_SPRSIZE)return false;
	int* CEL=SpRefs[cell];
	int   NCEL=NSpri[cell];
	if(!(CEL&&NCEL))return false;
	for(int i=0;i<NCEL;i++){
		OneSprite* OS=&Sprites[CEL[i]];
		//assert(OS->Enabled);
		ObjCharacter* OC=OS->OC;
		if(OC->ResType!=0xFE){
			int dr=OC->ShieldRadius;
			//if(OC->IntResType>8)dr=0;
			int shpr=OC->ShieldProbability;
			if(Norma(OS->x-x,OS->y-y)<dr&&shpr>(rando()%100))
				return true;
        };
    };
    return false;
};
bool DetectSpriteInPoint(int x,int y,int Damage,OneObject* Sender,int AttType){
    int nr=1;
    int nr1=nr+nr+1;
    int cell=(x>>7)-nr+(((y>>7)-nr)<<SprShf);
    for(int ix=0;ix<nr1;ix++)
        for(int iy=0;iy<nr1;iy++)
			if(DetectSpriteInPointInCell(cell+ix+(iy<<SprShf),x,y)){
				int D=Damage;
				if(Sender&&AttType>=0&&AttType<16){
					int GetDamFall(int x,int x0,int Dam);
					int RR=Norma(x-Sender->RealX/16,y-Sender->RealY/16);
					D=GetDamFall(RR,Sender->newMons->DamageDecr[AttType],D);
				}
				if (!(Sender->IsInFog)){
				void ViewDamage(int x,int y,int value);
				ViewDamage(x,y,0);//D);
				}
				return true;
			}

    return false;
};
#ifdef _USE3D
void M4_srtpp(Matrix4D& M,
			  float dx,float dy,float dz,
			  float pivX,float pivY,float Angle,
			  float SX,float SY){
	float COS=cos(Angle);
	float SIN=sin(Angle);
	M.setIdentity();
	M.e00 = SX*COS;
	M.e01 = SY*SIN;
	M.e10 =-SX*SIN;
	M.e11 = SY*COS;
	M.e30 = dx-M.e00*pivX-M.e10*pivY;
	M.e31 = dy-M.e01*pivX-M.e11*pivY;
	M.e32 = dz;
};
struct OneM4_Batch{
	Matrix4D M4;
	int      FileID;
	int      SpriteID;
	int      x0,y0,x1,y1;
	float    zp;
	bool     Drawn;
	DWORD Diffuse;

};
extern IRenderSystem* IRS;
//drawing sprites,using hardware z-buffer
void DrawWeaponUsing3D(byte Pass)
{
	OneM4_Batch MBatches[1024];
	int NMBatches=0;

	GPS.FlushBatches();
	GPS.EnableZBuffer(1);
	static int wsh=IRS->GetShaderID("Weapon");
	int shd=GPS.SetShader(wsh);
    
    Frustum frustum = ICam->GetFrustum();

	float scale=1.0f/float(1<<(5-Shifter));
	for(int i=0;i<MaxExpl;i++)
    {
        if(!EUsage[i]) continue;			
		AnmObject* AO=GAnm[i];
		Weapon* Weap=AO->Weap;
		if (Weap->LayerID!=Pass) continue;
		int xs=(AO->x>>WEPSH);
		int ys0=mul3(AO->y>>4)>>(WEPSH-2);
		int ys=ys0-(AO->z>>WEPSH);
		int DZ=AO->GraphDZ>>2;
		int xs1=xs;
		xs=xs1;
        int xr=AO->x>>WEPSH;
        int yr=AO->y>>WEPSH;
        int zr=AO->z>>WEPSH;
        Vector3D wPos = SkewPt( xr, yr, zr );

		NewAnimation* NA=Weap->NewAnm;
		if(!NA) continue;

        const float c_EffectBoundRadius = 256.0f;
		if (!frustum.Overlap( Sphere( wPos, c_EffectBoundRadius ) )) continue;            
        PushEntityContext( i+i*(AO->xd+AO->yd+AO->zd) );

		//determining the direction
		if(AO->Frame>NA->NFrames-1)AO->Frame=NA->NFrames-1;
		NewFrame* NF=AO->Frame<NA->Frames.GetAmount()?NA->Frames[AO->Frame]:NULL;
		PlayAnimation(NA,int(AO->Frame)<<8,xs,ys);
		if(NA->Rotations==255&&NA->AnimationType==0)
        {
			double angl;
			bool inv=0;
			float sy=scale;
			int dx=-NF->dx;
			int dy=-NF->dy;

			if(NA->Rotations==1){
				if(Weap->Projection==1){
					float da=GetHeight(xr+16,yr)-GetHeight(xr-16,yr);
					angl=atan2(-da,float(32));
					float dz=float(16-GetHeight(xr,yr+16)+GetHeight(xr,yr-16))/16;
					if(dz<0.1)dz=0.1;
					if(dz>3.0)dz=3.0;
					sy*=dz;
				}else{
					angl=0;
				};
			}else{
				if(!(AO->vx||AO->vy)){
					float dy=float((AO->yd-AO->y)>>WEPSH);
					float dx=float((AO->xd-AO->x)>>WEPSH)/0.8;
					angl=atan2(dy,dx);
					//inv=dx>0;
				}else{
					float dy=float(AO->vy-Prop43(AO->vz));
					float dx=float(AO->vx)/0.8;
					angl=atan2(dy,dx);
					//inv=dx>0;
				};
				sy=0.8*scale;
				if(inv)sy=-sy;
				angl+=3.1415;
			};
			
			Vector3D pivot( dx, dy, 0.0f );
			Matrix4D M4 = GetOrientedBillboardTransform( pivot, angl, 2.0f*sy );
			M4.translate( wPos );

			DWORD Diff=0xA0FFFFFF;
			if(Weap->Times!=-1&&(Weap->Fade0!=Weap->Fade1||Weap->Fade0!=Weap->Fade2)){
				int DT;
				if(AO->NTimes==1){
					DT=AO->Frame;
				}else{
					DT=Weap->Times-AO->NTimes;
				}
				DWORD D0,D1;
				int T0,T1;
				if(DT<Weap->FadeFirstStep){
					D0=Weap->Fade0;
					D1=Weap->Fade1;
					T0=DT;
					T1=Weap->FadeFirstStep;
				}else{
					D0=Weap->Fade1;
					D1=Weap->Fade2;
					T0=DT-Weap->FadeFirstStep;
					if(AO->NTimes==1){
                        T1=Weap->FadeFirstStep;
						if(T0>T1)T1=T0;
					}else{
						T1=Weap->Times-Weap->FadeFirstStep;
					}
				}
				int A1=(D0>>24);
				int A2=(D1>>24);
				int R1=(D0>>16)&255;
				int R2=(D1>>16)&255;
				int G1=(D0>>8)&255;
				int G2=(D1>>8)&255;
				int B1=(D0)&255;
				int B2=(D1)&255;
				int A,R,G,B;
				if(T1==0){
					A=A2;
					R=R2;
					G=G2;
					B=B2;
				}else{
					A=A1+(A2-A1)*T0/T1;
					R=R1+(R2-R1)*T0/T1;
					G=G1+(G2-G1)*T0/T1;
					B=B1+(B2-B1)*T0/T1;
				};
				Diff=(A<<24)+(R<<16)+(G<<8)+B;
			};
			if(Weap->SizeFactor==1){
				OneM4_Batch* MB=MBatches+NMBatches;
				MB->M4=M4;
				MB->FileID=NF->FileID;
				MB->SpriteID=NF->SpriteID;
				MB->Drawn=0;
				MB->x0=xs;
				MB->y0=ys;
				// --!!!
				MB->zp=0.0f;
				// --
				MB->Diffuse=Diff;
				NMBatches++;
			}else{
				if(Weap->SizeFactor==2)GPS.FlushBatches();
				GPS.SetCurrentDiffuse(Diff);

				GPS.DrawWSprite(NF->FileID,NF->SpriteID,M4,0);						

				if(Weap->SizeFactor==2)GPS.FlushBatches();
			};
		}
        else
        {
			int dy=((AO->yd-AO->y)>>WEPSH);
			int dx=((AO->xd-AO->x)>>WEPSH);
			word GetDir(int dx,int dy);
			byte angl=GetDir(dx,dy);
			float yB=0;
			float zB=0;
            NA->DrawAt(AO->Frame<<8,0,AO->x>>WEPSH,(AO->y>>WEPSH),(AO->z>>WEPSH),angl,1.0f,0xFFFFFFFF,0,0,NULL);
		}
		PopEntityContext();
	};
	GPS.FlushBatches();
	bool changed=0;
	do{
		changed=0;
		OneM4_Batch* MB=MBatches;
		int xdrawn[64];
		int ydrawn[64];
		int ndrawn=0;

		for(int i=0;i<NMBatches;i++){
			if(!MB->Drawn){
				int x0=MB->x0;
				int y0=MB->y0;
				int j;
				for(j=0;j<ndrawn;j++){
					if(Norma(x0-xdrawn[j],y0-ydrawn[j])<80)break;
				};
				if(j==ndrawn&&ndrawn<64){
					MB->Drawn=1;
					GPS.SetCurrentDiffuse(MB->Diffuse);
					GPS.DrawWSprite(MB->FileID,MB->SpriteID,MB->M4,0);
					xdrawn[ndrawn]=x0;
					ydrawn[ndrawn]=y0;
					ndrawn++;
					changed=1;
				};
			};
			MB++;
		};
		if(changed)GPS.FlushBatches();
	}while(changed);
	
	GPS.SetShader(shd);
	GPS.SetCurrentDiffuse(0xFFFFFFFF);
};
int CheckBreaksBetweenPoints(int x,int y,int z,int xd,int yd,int zd,int Index);
//returns -1 if not possible, otherwice attack type
//reason - the reason of impoosibility, 
//          0-need to move forward,
//          1-need to move back, 
//          2-friendly units does not allows to shot/strike
//          3-absolutely impossibe to shot/strike
//          4-ground does not allows 
int CheckImmediateAttackAbility(OneObject* Killer,OneObject* Victim,byte& Reason,bool Fast){
    int R=Norma(Killer->RealX-Victim->RealX,Killer->RealY-Victim->RealY)>>4;
	int dz=Killer->RZ-Victim->RZ;	
	if(dz<0)dz=0;
	int d=dz*2;
	if(d>300)d=300;
	AdvCharacter* ADC=Killer->MoreCharacter;
	int MinR=ADC->MinR_Attack;
	int BestAttType=-1;
	int NowCanHaveAttState=-1;
	bool ArmOnly=0;
	bool StandGr=Killer->StandGround;
	if(!Killer->RifleAttack)ArmOnly=1;
	for(int i=0;i<2&&ADC->MaxDamage[i];i++){
		if(!(i&&ArmOnly)){
			int RC=R;
            int dd=0;
			if(i){
				dd=d;
				RC+=dd;//height dependence
				if(RC<1)RC=1;
			}
			if(R>=ADC->DetRadius1[i]&&RC<ADC->DetRadius2[i]){
                BestAttType=i;
			}
			if(R>=ADC->AttackRadius1[i]&&R<ADC->AttackRadius2[i]+dd){
				NowCanHaveAttState=i;
			}
		}
	}
	if(BestAttType==-1)return 3;//impossible to attack
    if(R<ADC->MinR_Attack)return 1;//too near
	if(R>ADC->MaxR_Attack+d||NowCanHaveAttState==-1)return 0;//too far	
    //now we need to check for breaks - hills,friendly units,...
	if(Killer->newMons->FreeShotDist>=400)return NowCanHaveAttState;
	if(NowCanHaveAttState>0&&!Fast){
		Coor3D C3D;		
		Victim->GetDamagePoint(Killer->x,Killer->y,&C3D,0);
		int r=CheckBreaksBetweenPoints(Killer->RealX>>4,Killer->RealY>>4,Killer->RZ+32,C3D.x,C3D.y,C3D.z,Killer->Index);
		if(r==-1)return NowCanHaveAttState;
		else if(r==0xFFFF)return 4;
		else return 2;
	}else return NowCanHaveAttState;
}
#endif //_USE3D

extern char* mbm[1024];
extern int	nmbm;
int GetNNewWeap();
char* GetNewWeapName(int idx);
int GetNewWeapIdx(char* name);
int GetWeaponIndex(char* str);
Weapon* GetNewWeaponPtr(int idx);
Weapon* GetWeaponWithModificator(char* Name);
Weapon* GetWeaponByName(char* Name);
Weapon* GetWeaponByIdx(int idx);

class WeaponEnumerator:public ProcEnumerator{
public:
	virtual DWORD GetValue(const char* ID){
		if(ID[0]==0)return 0;
		Weapon* W=GetWeaponByName((char*)ID);
		if(!W){
            W=GetWeaponWithModificator((char*)ID);
		}
		return DWORD(W);
	}	
	virtual char* GetValue(DWORD ID){
		if(ID==0)return "";
		char* name="";
		for(int i=0;i<nmbm;i++)if(ID==(DWORD)GetWeaponByIdx(i))return mbm[i];
		for(int i=0;i<GetNNewWeap();i++)if(ID==(DWORD)GetNewWeaponPtr(i))return GetNewWeapName(i);
		return "";
	}	
	virtual int   GetAmount(){
		return nmbm+GetNNewWeap()+1;
	}
	virtual char* GetIndexedString(int idx){
		if(idx>0){
			if(idx<nmbm)return mbm[idx];
			else if(idx<nmbm+GetNNewWeap())return GetNewWeapName(idx-nmbm);
		}
		return "";
	}
	virtual DWORD GetIndexedValue (int idx){
		if(idx==0)return 0;
		if(idx<nmbm+1)return (DWORD)GetWeaponByIdx(idx-1);
		else return (DWORD)GetNewWeaponPtr(idx-nmbm-1);
	}

	virtual char* GetCategory(int idx){
		if(nmbm>0){
			if(idx<nmbm)return "OldWeaponSystem";
			else return "NewWeaponSystem";
		}else return NULL;
	}	
};
void CreateWeaponEnumerator(){
	WeaponEnumerator* WE=new WeaponEnumerator;
	Enumerator* E=ENUM.Get("WEAPONS");
	E->SetProcEnum(WE);

	E=ENUM.Get("WeaponIDS");
	for(int i=0;i<NWeaponIDS;i++){
		E->Add(WeaponIDS[i]);
	};

}