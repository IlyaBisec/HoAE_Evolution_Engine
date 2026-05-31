#include "stdheader.h"
#include "LoadSave.h"
#include "DrawFeatures.h"
#include "unitability.h"
#pragma pack (push)
#pragma pack (8)
#pragma pack (pop)

//#include "fintClass.h"
//#define float fint<8>
//        
//  =F===X===F=
//        
class MechanicsQuant{
public:
	float xc0,yc0,zc0;
	float Fi0;
	float xc,yc,zc;
	float Fi;

	//int fpx[2];
	//int fpy[2];
	//int fpz[2];

	int ForcePointR[2];
	MechanicsQuant(){
		memset(&xc0,0,int(&ForcePointR)-int(&xc0)+8);
	};
	/*virtual */void MoveQuant(int fpIndex,float dx,float dy);
};
//     =====
//       |
//  =F===X===F=
//       |
//     =====
//
class SingleAxeQuant: public MechanicsQuant{
public:
	DWORD TEMP;
	int AxeL;
	int LeftAngle;
	int RightAngle;
	SingleAxeQuant(){
		AxeL=0;
		LeftAngle=0;
		int RightAngle=0;
	};
	~SingleAxeQuant(){

	};
	/*virtual*/ void MoveQuant(int fpIndex,float dx,float dy);
	/*virtual */float GetShiftOfPoint(float x,float y,float& dx,float& dy);
};
//        /
//      /|
//  =F===X===F=
//       |/
//      /
//
class ComplexAxeQuant: public SingleAxeQuant{
public:
	int AdditionalWheelAngle;
	int MaxAdditionalWheelAngle;
	ComplexAxeQuant(){
		AdditionalWheelAngle=0;
		MaxAdditionalWheelAngle=0;
	};
	~ComplexAxeQuant();
	virtual void MoveQuant(int fpIndex,float dx,float dy);
};
void MechanicsQuant::MoveQuant(int fpIndex,float dx,float dy){
	
};
#define AngScale (3.1415926535f/128.0f)
void SingleAxeQuant::MoveQuant(int fpIndex,float dx,float dy){
	if(fabs(dx)>0.00001||fabs(dy)>0.00001){
		float fi=Fi*AngScale;
		float DX=cos(fi);
		float DY=sin(fi);
		float DL=DX*dx+DY*dy;
		float DT=-DY*dx+DX*dy;
		xc0=xc;
		yc0=yc;
		zc0=zc;
		Fi0=Fi;
		xc+=float(DX*DL);
		yc+=float(DY*DL);
		Fi+=float(DT*407.0)/ForcePointR[fpIndex]/10.0/16.0;
		int DA=DT*AxeL/ForcePointR[fpIndex];
		LeftAngle+=DL+DA;
		RightAngle+=DL-DA;
	}
};
_inline void getxy(float dx,float dy,float fi,float& rdx,float& rdy){
	float FI=AngScale*fi;
	float sin_=sin(FI);
	float cos_=cos(FI);
    rdx=dx*cos_-dy*sin_;
	rdy=dy*cos_+dx*sin_;
}
float SingleAxeQuant::GetShiftOfPoint(float x,float y,float& dx,float& dy){
    float x0,y0,x1,y1;
	getxy(x,y,Fi,x1,y1);
	getxy(x,y,Fi0,x0,y0);
	float rx1=x1*16.0f+xc;
	float ry1=y1*16.0f+yc;
	float rx0=x0*16.0f+xc0;
	float ry0=y0*16.0f+yc0;
	//float rx1=x1+xc;
	//float ry1=y1+yc;
	//float rx0=x0+xc0;
	//float ry0=y0+yc0;
	float rdx=rx1-rx0;
	float rdy=ry1-ry0;
	//dfi=atan2f(rdy,rdx)/AngScale;
	dx=rdx;
	dy=rdy;
	return sqrt(rdx*rdx+rdy*rdy);
}
void ComplexAxeQuant::MoveQuant(int fpIndex,float dx,float dy){

}
int GetTotalHeight0(int x,int y);
float GetFHeight(int x,int y);
void RegisterVisibleGP(OneObject* OB,int gpID,int sprID,int x,int y, float scale );

class ObjectPartAnimation{
public:
	NewAnimation NA;
	char ID[32];
	/*word GPID;
	int GP_dx;
	int GP_dy;
	int NOcts;*/
	int StartFrame;
	int NFrames;
	int DistPerFrame;
	int SortX1;
	int SortX2;
	bool Inversed;
	DWORD ObjectMask;
	__forceinline void DrawAt(int x,int y,byte Dir,int time,OneObject* OB,byte NI,bool DrawOnlyModels){
		/*int dx,dy;
		int SP=GetSprite(Dir,time,dx,dy);
		int SIN=TSin[Dir];
		int COS=TCos[Dir];
		int XL1=(SortX1*COS)>>4;
		int YL1=(SortX1*SIN)>>5;
		int XL2=(SortX2*COS)>>4;
		int YL2=(SortX2*SIN)>>5;*/
		x+=(mapx<<9);
		y+=(mapy<<8);


		bool INV=0;
		int FID;		
		//if(NA.Inverse){
			//Dir=128-Dir;
		//}		
		int csp;
		int V=DistPerFrame;
		if(!V){
			csp=NFrames?(DWORD(time)%NFrames):0;
		}else{
			if(V<0)csp=NFrames-1-(DWORD(time)/(-V))%NFrames;
			else csp=(DWORD(time)/V)%NFrames;
		}		
		if(NA.NFrames){			
			OB->OctantInfo=0xFF;
			if(DrawOnlyModels==(NA.AnimationType==1)){
				int xx=x/16;
				int yy=y/8+OB->RZ*2;
				NA.DrawAt(csp<<8,NI,xx,yy,GetTotalHeight0(xx,yy),Dir,1.0,0xFF808080,0,0,OB);
			}
		}
	};
	int ReadFromString(char* s){  //0 - OK, 1 - Not enough params, 2- unknown GP-name
		char cc[256];
		int gpdx,gpdy,nocts;
		int h=0;
		char snd[128]="";
		int z=sscanf(s,"%s%d%d%d%d%d%d%d%d%d%s",cc,&gpdx,&gpdy,&nocts,&NFrames,&StartFrame,&DistPerFrame,&SortX1,&SortX2,&h);
		if(z<9)return 1;
		int GPID=GPS.PreLoadGPImage(cc);
		if(GPID==0xFFFF)return 2;
		//NA.Frames=new NewFrame[NFrames];
		NA.Frames.Clear();for(int ll=0;ll<NFrames;ll++)NA.Frames.Add(new NewFrame());
		for(int i=0;i<NFrames;i++){
			NA.Frames[i]->FileID=GPID;
			NA.Frames[i]->SpriteID=StartFrame+i;
			NA.Frames[i]->dx=-gpdx;
			NA.Frames[i]->dy=-gpdy;
		}
		NA.NFrames=NFrames;
		NA.Rotations=abs(nocts);
		NA.Inverse=nocts<0;
		NA.AddHeight=h;
		extern char* SoundID[MaxSnd];
		extern word NSounds;
		if((NA.SoundID=SearchStr(SoundID,snd,NSounds))!=-1){
			NA.ActiveFrame=0xFF;
		}									
		return 0;
	};
	__forceinline int GetSprite(byte Dir,int Time,int& dx,int& dy){
		/*bool INV=0;
		int FID;
		if(NOcts&1){
			if(Inversed){
				Dir=128-Dir;
			}
			int NO=(NOcts-1)*2;
			int ANG=256/NO;
			int A2=ANG>>1;
			FID=((Dir+256-64+A2)/ANG)%NO;
			if(FID>=NOcts){
				INV=1;
				FID=NO-FID;
			};
			if(Inversed)INV=~INV;
		}else{
			if(Inversed){
				Dir=128-Dir;
				INV=1;
			}
			int NO=NOcts;
			int ANG=256/NO;
			int A2=ANG>>1;
			FID=((Dir+256-64+A2)/ANG)%NO;
		};
		int csp;
		int V=DistPerFrame;
		if(!V){
			csp=StartFrame+(Time%NFrames);
		}else{
			if(V<0)csp=StartFrame+NFrames-1-(DWORD(Time)/(-V))%NFrames;
			else csp=StartFrame+(DWORD(Time)/V)%NFrames;
		}
		if(INV){
			dx=GP_dx;
			dy=-GP_dy;
			return csp*NOcts+FID+4096;
		}else{
			dx=-GP_dx;
			dy=-GP_dy;
			return csp*NOcts+FID;
		};*/
		return 0;
	};
};
struct QuantSingleElement{
	ObjectPartAnimation* ANM;
	int dx,dy,dfi;
	byte AnmDir:4;
	byte RotDir:4;
};
class QuantPartState{
public:
	int NElm;
	QuantSingleElement* Elm;
	QuantPartState(){
		memset(this,0,sizeof *this);
	};
	~QuantPartState(){
		if(Elm)delete[](Elm);
		memset(this,0,sizeof *this);
	};
};

class QuantPartTransformElement{
public:
	byte OpType;
	int StartTime;
	int TimeAmount;
	int StartFrame;
	int EndFrame;
	ObjectPartAnimation* ANM;
	int x0,y0,Fi0,x1,y1,Fi1;
};
class QuantPartTransform{
public:
	int MaxTransfTime;
	byte QP_Type;
	int  NParts;
	QuantPartTransformElement** QPTE;
	int* NInQPTE;
	QuantPartTransform(){
		memset(this,NULL,sizeof *this);
	};
	~QuantPartTransform(){
		for(int i=0;i<NParts;i++)if(QPTE[i])delete[](QPTE[i]);
		if(NInQPTE)delete[](NInQPTE);
		if(QPTE)delete[](QPTE);
		memset(this,NULL,sizeof *this);
	};

};
void PanicUnit(OneObject* OB);
#define NStates 24
#define MaxHelperUnits 8
#define MaxExpInf 4
struct ExplodeInfo{
	Weapon* Particles;
	int  x,y;
    byte Stage;
};
class OneMQuantDesc{
public:
	char ID[32];
	QuantPartState* States[NStates];
	word Helpers[MaxHelperUnits];
	word HelpersPos[MaxHelperUnits];
	word HelpersOpt[MaxHelperUnits];//1-can attack any time
	word NHelpers;
	ExplodeInfo ExpInf[MaxExpInf];
	word NExpl;

	int X1;
	int X2;
	int R;
	int AttackX;
	int AttackZ;
	int AttackY;
	//          ALONE  HEAD   TALE HEAD&TALE
	//STAND       0      1      2      3
	//MOTION      4      5      6      7
	//ATTACK      8      9     10     11
	//RATTACK    12     13     14     15
	//RATTACK1   16     17     18     19
	//RATTACK2   20     21     22     23
	QuantPartTransform* TRANS[NStates*NStates];
	OneMQuantDesc(){
		memset(this,0,sizeof *this);
	};	
};
#define MaxChains 16
class OneComplexUnitDesc{
public:
	char ID[32];
	int NQuants;	
	OneMQuantDesc* Chain[MaxChains];
	bool CanBeTale :1;
	bool CanBeHead :1;
	bool CanBeAlone:1;
	bool InverseTaleMotion:1;
	OneComplexUnitDesc(){
		memset(this,0,sizeof *this);
	};
};

#define MaxOPA 512
#define MaxQD 256
#define MaxU 256
class ComplexObjDesc{
public:
	ObjectPartAnimation* OPANM[MaxOPA];
	int N_OPANM;
	OneMQuantDesc* OMQD[MaxQD];
	int N_OMQD;
	OneComplexUnitDesc* UnitDesc[MaxU];
	int NUnits;

	//--------------------------
	word GetUnitID(char* Name){
		for(int i=0;i<NUnits;i++)if(!strcmp(Name,UnitDesc[i]->ID))return i;
		return 0xFFFF;
	};
	void ReadChainObjectsDesc();
	int GetAnmIndex(char* ID){
		for(int i=0;i<N_OPANM;i++)if(!strcmp(ID,OPANM[i]->ID))return i;
		return -1;
	};
	int GetStageIndex(char* S1){
		int IDX=-1;
		if(strstr(S1,"$STAND_"))IDX=0;
		if(strstr(S1,"$MOTION_"))IDX=4;
		if(strstr(S1,"$ATTACK_"))IDX=8;
		if(strstr(S1,"$RATTACK_"))IDX=12;
		if(strstr(S1,"$RATTACK1_"))IDX=16;
		if(strstr(S1,"$RATTACK2_"))IDX=20;
		if(IDX==-1)return -1;
		if(strstr(S1,"_HEAD"))IDX++;
		else if(strstr(S1,"_TALE"))IDX+=2;
		else if(strstr(S1,"_HEAD&TALE"))IDX+=3;
		else if(!strstr(S1,"_ALONE"))return -1;
		return IDX;
	};
	ComplexObjDesc(){
		memset(this,0,sizeof *this);
	};
	~ComplexObjDesc(){
		for(int i=0;i<N_OPANM;i++)delete(OPANM[i]);
		for(int i=0;i<N_OMQD;i++)delete(OMQD[i]);
		for(int i=0;i<NUnits;i++)delete(UnitDesc[i]);
		memset(this,0,sizeof *this);
	};
};
bool ReadWinString(GFILE* F,char* STR,int Max);
void LoadErr(char* sz,...);
void ComplexObjDesc::ReadChainObjectsDesc(){
	memset(this,0,sizeof *this);
	GFILE* F=Gopen("ComplexObjects\\Objects.dat","r");
	if(F){
		int Line=0;
		char LINE[256];
		char S1[64];
		char S2[64];
		char S3[64];
		int a,b,c,z;
		int v;
		do{
			v=ReadWinString(F,LINE,255);
			Line++;
			if(v){
				char C0=LINE[0];
				char C1=LINE[1];
				if(LINE[0]!='/'){
					if(C0=='#'){
						if(C1=='3'&&LINE[2]=='D'){//#3DANIM Name Scale ModelID AnimID NFrames DistPerFrame AddH AddFi
							z=sscanf(LINE,"%s%s",S1,S2);
							if(z==2){
								if(N_OPANM<MaxOPA){
									OPANM[N_OPANM]=new ObjectPartAnimation;
									ObjectPartAnimation* OP=OPANM[N_OPANM];									
									N_OPANM++;
									int v=ReadWinString(F,LINE,255);
									if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
									Line++;
									char modid[64];
									char anmid[64];
									int adf,adh;
									float sc;
									char snd[128]="";
                                    z=sscanf(LINE,"%f%s%s%d%d%d%d%s",&sc,modid,anmid,&OP->NFrames,&OP->DistPerFrame,&adh,&adf,snd);
									if(z<7)LoadErr("Objects.dat, line %d : Not enough params for #3DANIM: %s",Line,LINE);
									int mid=IMM->GetModelID(modid);
									if(mid==-1){
										LoadErr("Objects.dat, line %d : Model not found: %s",Line,modid);
									}
									OP->NA.NFrames=OP->NFrames;
									OP->NA.AnimationType=true;
									OP->NA.ModelID=mid;
									OP->NA.AddDirection=adf;
									OP->NA.AddHeight=adh;
									OP->NA.AnimationID=IMM->GetModelID(anmid);
									OP->NA.Scale=sc;
									extern char* SoundID[MaxSnd];
									extern word NSounds;
									if((OP->NA.SoundID=SearchStr(SoundID,snd,NSounds))!=-1){
										OP->NA.ActiveFrame=0xFF;
									}									
									strcpy(OP->ID,S2);
								}else LoadErr("Objects.dat, line %d : Too many animations.",Line);
							}else LoadErr("Objects.dat, line %d : incorrect #ANIM directive params.",Line);
						}else
						if(C1=='A'&&LINE[2]=='N'){//#ANIM
							z=sscanf(LINE,"%s%s",S1,S2);
							if(z==2){
								if(N_OPANM<MaxOPA){
									OPANM[N_OPANM]=new ObjectPartAnimation;
									ObjectPartAnimation* OP=OPANM[N_OPANM];
									N_OPANM++;
									int v=ReadWinString(F,LINE,255);
									if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
									Line++;
									OP->ReadFromString(LINE);
									strcpy(OP->ID,S2);
								}else LoadErr("Objects.dat, line %d : Too many animations.",Line);
							}else LoadErr("Objects.dat, line %d : incorrect #ANIM directive params.",Line);
						}else
						if(C1=='O'){//#OBJECT
							z=sscanf(LINE,"%s%s",S1,S2);
							if(z==2){
								if(N_OPANM<MaxOPA){
									OPANM[N_OPANM]=new ObjectPartAnimation;
									ObjectPartAnimation* OP=OPANM[N_OPANM];
									N_OPANM++;
									int v=ReadWinString(F,LINE,255);
									if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
									Line++;
									OP->ReadFromString(LINE);
									strcpy(OP->ID,S2);
									OP->ObjectMask=1;
								}else LoadErr("Objects.dat, line %d : Too many animations.",Line);
							}else LoadErr("Objects.dat, line %d : incorrect #ANIM directive params.",Line);
						}else
						if(C1=='M'){//#MQUANT
							z=sscanf(LINE,"%s%s%d%d%d",S1,S2,&a,&b,&c);
							if(z==5){
								if(N_OMQD<MaxQD){
									OMQD[N_OMQD]=new OneMQuantDesc;
									OneMQuantDesc* OM=OMQD[N_OMQD];
									N_OMQD++;
									strcpy(OM->ID,S2);
									OM->X1=a;
									OM->X2=b;
									//OM->R=r;
									for(int i=0;i<c;i++){
										int v=ReadWinString(F,LINE,255);
										if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
										Line++;
										z=sscanf(LINE,"%s%d",S1,&a);
										if(z&&S1[0]=='@'){
											if(S1[1]=='T'){//@TRANSFORM
												z=sscanf(LINE,"%s%s%s",S1,S2,S3);
												if(z==3){
													int IDX1=GetStageIndex(S2);
													int IDX2=GetStageIndex(S3);
													if(IDX1==-1)LoadErr("Objects.dat, line %d : Unknown stage: %s.",Line,S2);
													if(IDX2==-1)LoadErr("Objects.dat, line %d : Unknown stage: %s.",Line,S3);
													if(!OM->States[IDX1]){
                                                        LoadErr("Objects.dat, line %d : Missing start state of transformation (%s).",Line,LINE);
														return;
													}
													if(!OM->States[IDX2]){
                                                        LoadErr("Objects.dat, line %d : Missing final state of transformation (%s).",Line,LINE);
														return;
													}
													int N0=OM->States[IDX1]->NElm;
													int IDX=IDX1+IDX2*NStates;
													QuantPartTransform* QPT=OM->TRANS[IDX]=new QuantPartTransform;
													QPT->QPTE=new QuantPartTransformElement* [N0];
													QPT->NInQPTE=new int[N0];
													QPT->NParts=N0;
													QPT->QP_Type=0;
													int TMax=0;
													for(int q=0;q<N0;q++){
														int TM=0;
														QuantPartTransformElement** pQC=QPT->QPTE;
														int v=ReadWinString(F,LINE,255);
														if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
														Line++;
														int n1;
														z=sscanf(LINE,"%s%d",S1,&n1);
														if(z){
															if(z==1){
																pQC[q]=NULL;
																QPT->NInQPTE[q]=0;
															}else{
																pQC[q]=new QuantPartTransformElement[n1];
																QPT->NInQPTE[q]=n1;
																for(int p=0;p<n1;p++){
																	QuantPartTransformElement* QC=pQC[q]+p;
																	int v=ReadWinString(F,LINE,255);
																	if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
																	Line++;
																	z=sscanf(LINE,"%d%s%d%d%d%d%d%d%d%d",&QC->TimeAmount,S1,
																		&QC->StartFrame,&QC->EndFrame,&QC->x0,&QC->y0,&QC->Fi0,&QC->x1,&QC->y1,&QC->Fi1);
																	if(z==10){
																		int w=GetAnmIndex(S1);
																		if(w==-1)LoadErr("Objects.dat, line %d : Unknown animation: %s.",Line,S2);
																		QC->ANM=OPANM[w];
																		QC->StartTime=TM;
																		TM+=QC->TimeAmount;
																	}else LoadErr("Objects.dat, line %d : Unrecognized string.",Line);
																};
																if(TM>TMax)TMax=TM;
															};
														}else LoadErr("Objects.dat, line %d : Unrecognized string.",Line);
													};
													QPT->MaxTransfTime=TMax;
												}else LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
											}else
											if(S1[1]=='D'){//@DIRECT
												z=sscanf(LINE,"%s%s%s",S1,S2,S3);
												if(z==3){
													int IDX1=GetStageIndex(S2);
													int IDX2=GetStageIndex(S3);
													if(IDX1==-1)LoadErr("Objects.dat, line %d : Unknown stage: %s.",Line,S2);
													if(IDX2==-1)LoadErr("Objects.dat, line %d : Unknown stage: %s.",Line,S3);
													int IDX=IDX1+IDX2*NStates;
													QuantPartTransform* QPT=OM->TRANS[IDX]=new QuantPartTransform;
													QPT->QPTE=NULL;
													QPT->NParts=0;
													QPT->QP_Type=255;											
												}else LoadErr("Objects.dat, line %d : Incorrect params for @DIRECT directive.",Line);
											}else LoadErr("Objects.dat, line %d : Unknown transformation directive.",Line);
										}else{
											if(z==2){
												int IDX=GetStageIndex(S1);
												if(IDX==-1)LoadErr("Objects.dat, line %d : Unknown animation type %s.",Line,S1);
												OM->States[IDX]=new QuantPartState;
												QuantPartState* QPS=OM->States[IDX];
												QPS->NElm=a;
												QPS->Elm=new QuantSingleElement[a];
												for(int j=0;j<a;j++){
													int v=ReadWinString(F,LINE,255);
													if(!v)LoadErr("Objects.dat, line %d : Unexpected end of file.",Line);
													Line++;
													int q,w,e;
													e=0;
													z=sscanf(LINE,"%s%d%d%s%d",S1,&q,&w,&S2,&e);
													if(z>=4){
														int aid=GetAnmIndex(S1);
														if(aid==-1)LoadErr("Objects.dat, line %d : Unknown animation: %s.",Line,S1);
														QPS->Elm[j].ANM=OPANM[aid];
														QPS->Elm[j].dx=q;
														QPS->Elm[j].dy=w;
														QPS->Elm[j].dfi=e;
														switch(S2[0]){
														case 'L':
															QPS->Elm[j].AnmDir=0;
															break;
														case 'R':
															QPS->Elm[j].AnmDir=2;
															break;
														case 'F':
															QPS->Elm[j].AnmDir=3;
															break;
														default:
															QPS->Elm[j].AnmDir=1;
															break;
														};
														switch(S2[1]){
														case 'F':
															QPS->Elm[j].RotDir=0;
															break;
														case 'B':
															QPS->Elm[j].RotDir=1;
															break;
														default:
															QPS->Elm[j].RotDir=0;
															break;
														};
													};
												};
											}else{
												z=sscanf(LINE,"%s%s",S1,S2);
												if(z==2){
													int IDX1=GetStageIndex(S1);
													int IDX2=GetStageIndex(S2);
													if(IDX1==-1)LoadErr("Objects.dat, line %d : Unknown animation type %s.",Line,S1);
													if(IDX2==-1)LoadErr("Objects.dat, line %d : Unknown animation type %s.",Line,S2);
													OM->States[IDX1]=new QuantPartState;
													QuantPartState* QPS1=OM->States[IDX1];
													QuantPartState* QPS2=OM->States[IDX2];
													QPS1->NElm=QPS2->NElm;
													QPS1->Elm=new QuantSingleElement[QPS2->NElm];
													memcpy(QPS1->Elm,QPS2->Elm,QPS2->NElm*sizeof QuantSingleElement);
												}else LoadErr("Objects.dat, line %d : incorrect #MQUANT directive params.",Line);
											};
										};
									};
								}else LoadErr("Objects.dat, line %d : Too many quants.",Line);
							}else LoadErr("Objects.dat, line %d : incorrect #MQUANT directive params.",Line);
						}else
						if(C1=='U'){//UNIT
							int N;
							z=sscanf(LINE,"%s%s%s",S1,S2,S2);
							if(z==3){
								if(NUnits<MaxU){
									OneComplexUnitDesc* OCUD=UnitDesc[NUnits]=new OneComplexUnitDesc;
									char CC[MaxChains][32];
									z=sscanf(LINE,"%s %s %s%s%s%s%s%s5s%s%s%s%s%s%s%s%s",S1,S2,
										CC[0],CC[1],CC[2],CC[3],CC[4],CC[5],CC[6],CC[7],
										CC[8],CC[9],CC[10],CC[11],CC[12],CC[13],CC[14],CC[15]);
									strcpy(OCUD->ID,S2);
									N=z-2;
									OCUD->NQuants=N;
									for(int p=0;p<N;p++){
										OCUD->Chain[p]=NULL;
										for(int w=0;w<N_OMQD;w++)if(!strcmp(CC[p],OMQD[w]->ID)){
											OCUD->Chain[p]=OMQD[w];
										};
										if(!OCUD->Chain[p])LoadErr("Objects.dat, line %d : Unknown MQUANT: %s.",Line,CC[p]);
										else{
											if(p==0&&OCUD->Chain[p]->States[2])OCUD->CanBeHead=1;
											if(p==N-1&&OCUD->Chain[p]->States[1])OCUD->CanBeTale=1;
											OCUD->CanBeAlone=1;
										};
									};
									NUnits++;
								}else LoadErr("Objects.dat, line %d : Too many units.",Line);
							}else LoadErr("Objects.dat, line %d : Invalid params for #UNIT directive.",Line);
						}else
						if(C1=='A'&&LINE[2]=='T'){//ATTACK
							int N;
							int x,z;
							int y=0;
							int zz=sscanf(LINE,"%s%s%d%d%d",S1,S2,&x,&z,&y);
							if(zz>=4){
								for(int i=0;i<NUnits;i++)if(!strcmp(UnitDesc[i]->ID,S2)){
									UnitDesc[i]->Chain[0]->AttackX=x;
									UnitDesc[i]->Chain[0]->AttackZ=z;
									UnitDesc[i]->Chain[0]->AttackY=y;
								}
							}else LoadErr("Objects.dat, line %d : Invalid params for #ATTACK directive.",Line);
						}else LoadErr("Objects.dat, line %d : incorrect #MQUANT directive params.",Line);
					};
				};
			};
			}while(v);
		Gclose(F);
	};	
};
#define MaxForw 4
//
class OneComplexObject;
void XorBlockComplexUnit(OneComplexObject* COB);
extern bool fes_bAmunition[8192];
void fes_neAmmunition(word NIndex, char* Message=NULL);
//
class OneComplexObject{
public:
	SingleAxeQuant* MQ[8];
    float ForwDist[MaxForw];
	float ForwDx[MaxForw];
	float ForwDy[MaxForw];
	byte  NQuants;
	byte  NI;
	word  CDIndex;
	word  ObjIndex;
	byte Lx;
	float RealX;
	float RealY;
	int StartState;
	int FinalState;
	int TransTime;
	int GroundMotionState;
	int GroundStandState;
	short DestDir;
	bool Charged;
	bool ResSubtracted;
	bool Lockpoints;

	bool TestResSubtract(){
		if(ResSubtracted)return true;
		if(ObjIndex!=0xFFFF){
			OneObject* OBJ=Group[ObjIndex];
			if(OBJ){
				NewMonster* NM=OBJ->newMons;
				for(int i=0;i<NM->NShotRes;i++){
					int rid=NM->ShotRes[i+i];
					int ram=NM->ShotRes[i+i+1];
					if(XRESRC(OBJ->NNUM,rid)<ram){
						if(OBJ->NNUM==GSets.CGame.cgi_NatRefTBL[MyNation] && (OBJ->ImSelected&(1<<MyNation))){
							char ccc[150];
							sprintf(ccc,GetTextByID("CSNORS"),OBJ->Ref.General->Message);
							fes_neAmmunition(OBJ->NIndex,ccc);
						}
						return false;
					}
				}
				//subtracting resources
				for(int i=0;i<NM->NShotRes;i++){
					int rid=NM->ShotRes[i+i];
					int ram=NM->ShotRes[i+i+1];
					AddXRESRC(OBJ->NNUM,rid,-ram)						
				}
				ResSubtracted=true;
				//				
				fes_bAmunition[OBJ->NIndex]=false;
                return true;
			}
		}
		return false;        
	}


	word LeaderID;
	word LeaderSN;
	word TaleID;
	word TaleSN;
	word HelpersIDS[MaxHelperUnits];
	word HelpersSNS[MaxHelperUnits];
	word HelpersPos[MaxHelperUnits];
	word NHelpers;

	bool NoAttack;
	bool NoMove;
	bool Capture;

	bool GetPositionCoor(int QuantIdx,int QuapntPos,int&x,int&y);
	void SetBackState(bool Back){
		XorBlockComplexUnit(this);
		OneObject* OB=Group[ObjIndex];
		if(OB&&OB->BackMotion!=Back){
			OB->BackMotion=Back;
			int x,y;
			if(Back)GetTalePoint(x,y);
			else GetHeadPoint(x,y);
			OB->RealY=(OB->RZ<<5)+y;
			OB->RealX=x;
			RealX=OB->RealX;
			RealY=OB->RealY;
			OB->RZ=GetHeight(int(RealX)/16,int(RealY)/16);
			addrand(OB->RZ);
			if(Back)OB->RealDir=byte(MQ[NQuants-1]->Fi)+128;
			else OB->RealDir=byte(MQ[0]->Fi);
		}
		XorBlockComplexUnit(this);
	}

	void CheckConnection(){
		if(LeaderID!=0xFFFF){
			OneObject* OB=Group[LeaderID];
			if((!OB)||OB->Sdoxlo||OB->Serial!=LeaderSN)LeaderID=0xFFFF;
		};
		if(TaleID!=0xFFFF){
			OneObject* OB=Group[TaleID];
			if((!OB)||OB->Sdoxlo||OB->Serial!=TaleSN)TaleID=0xFFFF;
		};
		if(LeaderID==0xFFFF&&TaleID==0xFFFF){
			void SetOrderedStateForComplexObjectLink(OneObject* OBJ);
			OneObject* OB=Group[ObjIndex];
			//if(!((OB->LocalOrder&&OB->LocalOrder->DoLink==&SetOrderedStateForComplexObjectLink)||OB->Attack))GroundStandState=0;
			if(OB->DestX>0||DestDir!=-1){
				//GroundStandState=0;
				GroundMotionState=4;
			}else{
				//GroundStandState=OB->LocalNewState*4+12;
				GroundMotionState=4;
			}
		}else
		if(LeaderID!=0xFFFF&&TaleID!=0xFFFF){
			GroundStandState=3;
			GroundMotionState=7;			
		}else
		if(LeaderID!=0xFFFF){
			GroundStandState=2;
			GroundMotionState=6;
		}else{
			GroundStandState=1;
			GroundMotionState=5;
		};

	};
	void PerformShift(float dx,float dy);
	void PerformBackShift(float dx,float dy);
	void PerformRotation(char Final);
	OneComplexObject(){
		memset(this,0,sizeof *this);
	};
	~OneComplexObject(){
		for(int i=0;i<4;i++)if(MQ[i])delete(MQ[i]);
		memset(this,0,sizeof *this);
	};
	void TryToTransform(int FinState);
	void TryToTransformToStandState();
	void TryToTransformToMotionState();
	void GetHeadPoint(int& x,int& y){
		x=MQ[0]->xc+((MQ[0]->ForcePointR[0]*TCos[byte(MQ[0]->Fi)])>>4);
		y=MQ[0]->yc+((MQ[0]->ForcePointR[0]*TSin[byte(MQ[0]->Fi)])>>4);
	};
	void GetHeadPoint(float& x,float& y){
		x=MQ[0]->xc+(float(MQ[0]->ForcePointR[0])*cos(MQ[0]->Fi*AngScale)*16.0);
		y=MQ[0]->yc+(float(MQ[0]->ForcePointR[0])*sin(MQ[0]->Fi*AngScale)*16.0);
	};
	void GetTalePoint(int& x,int& y){
		x=MQ[NQuants-1]->xc+((MQ[NQuants-1]->ForcePointR[1]*TCos[byte(MQ[NQuants-1]->Fi)])>>4);
		y=MQ[NQuants-1]->yc+((MQ[NQuants-1]->ForcePointR[1]*TSin[byte(MQ[NQuants-1]->Fi)])>>4);
	};
	void GetTalePoint(float& x,float& y){
		float fi=MQ[NQuants-1]->Fi*AngScale;
		x=MQ[NQuants-1]->xc+float(MQ[NQuants-1]->ForcePointR[1])*cos(fi)*16.0f;
		y=MQ[NQuants-1]->yc+float(MQ[NQuants-1]->ForcePointR[1])*sin(fi)*16.0f;
	};
};

ComplexObjDesc CODS;
OneComplexObject* CGroup[1024];
int MaxCObject=0;
void XorBlockComplexUnit(OneComplexObject* COB);
void OneComplexObject::PerformRotation(char Final){
	XorBlockComplexUnit(this);
	char FI=MQ[0]->Fi;
	char DFI=FI-Final;
	if(abs(int(DFI))>8){
		if(StartState==GroundMotionState){
			int GSP=GameSpeed;
			if(GSP<128)GSP=128;
			int R=MQ[0]->ForcePointR[0];
			int dx=(-GSP*R*TSin[byte(FI)])>>17;
			int dy=( GSP*R*TCos[byte(FI)])>>17;
			if(DFI>0){
				dx=-dx;
				dy=-dy;
			};
			PerformShift(dx,dy);
		}else TryToTransformToMotionState();//StandState();
	}else{
		MQ[0]->Fi=Final;
		MQ[0]->Fi0=Final;
		//TryToTransformToMotionState();//StandState();
	};
	int x,y;
	OneObject* OB=Group[ObjIndex];
	if(OB->BackMotion)GetTalePoint(x,y);
	else GetHeadPoint(x,y);
	//GetHeadPoint(x,y);
	OB->RealY=(OB->RZ<<5)+y;
	OB->RealX=x;
	RealX=OB->RealX;
	RealY=OB->RealY;
	OB->RZ=GetHeight(int(RealX)/16,int(RealY)/16);
	addrand(OB->RZ);
	OB->RealDir=byte(MQ[0]->Fi);
	XorBlockComplexUnit(this);
};
float sq_norma(float x,float y);
void OneComplexObject::PerformShift(float dx,float dy){
	XorBlockComplexUnit(this);
	float n=sq_norma(dx,dy);
	for(int j=0;j<NQuants;j++){
		SingleAxeQuant* SA=MQ[j];
		if(j==0){
			SA->MoveQuant(0,dx,dy);
		}else{
			SingleAxeQuant* SP=MQ[j-1];
			float fi=SP->Fi*3.1415/128.0;
			float XC0=SP->xc+(SP->ForcePointR[1]*cos(fi)*16.0);
			float YC0=SP->yc+(SP->ForcePointR[1]*sin(fi)*16.0);

			fi=SA->Fi*3.1415/128.0;
			float XC1=SA->xc+(SA->ForcePointR[0]*cos(fi)*16.0);
			float YC1=SA->yc+(SA->ForcePointR[0]*sin(fi)*16.0);
			float DX=XC0-XC1;
			float DY=YC0-YC1;
			SA->MoveQuant(0,DX,DY);
		};
	};
	if(TaleID!=0xFFFF){
		OneObject* OB=Group[TaleID];
		if(OB&&!OB->Sdoxlo){
			if(OB->CObjIndex!=0xFFFF){
				OneComplexObject* COB=CGroup[OB->CObjIndex];
				if(COB){
					OneComplexUnitDesc* OCUD=CODS.UnitDesc[CGroup[OB->CObjIndex]->CDIndex];
					float xc,yc;
					if(CODS.UnitDesc[CDIndex]->InverseTaleMotion){
						GetHeadPoint(xc,yc);
					}else{
						GetTalePoint(xc,yc);
					};
					if(!OCUD->InverseTaleMotion){
						float x,y;
						COB->GetTalePoint(x,y);
						COB->PerformBackShift(xc-x,yc-y);
						COB->RealX=OB->RealY=OB->RZ*32+y;
						COB->RealY=OB->RealX=x;
					}else{
						float x,y;
						COB->GetHeadPoint(x,y);
						COB->PerformShift(xc-x,yc-y);
						COB->RealX=OB->RealY=OB->RZ*32+y;
						COB->RealY=OB->RealX=x;
					};
				};
			};
		};
	};
	XorBlockComplexUnit(this);
};
void OneComplexObject::PerformBackShift(float dx,float dy){
	XorBlockComplexUnit(this);
	for(int j=NQuants-1;j>=0;j--){
		SingleAxeQuant* SA=MQ[j];
		if(j==NQuants-1){
			SA->MoveQuant(1,dx,dy);
		}else{
			SingleAxeQuant* SP=MQ[j+1];
            float fi=SP->Fi*3.1415/128.0;
			float XC0=SP->xc+float(SP->ForcePointR[0])*cos(fi)*16.0f;
			float YC0=SP->yc+float(SP->ForcePointR[0])*sin(fi)*16.0f;

			fi=SA->Fi*3.1415/128.0;
			float XC1=SA->xc+float(SA->ForcePointR[1])*cos(fi)*16.0f;
			float YC1=SA->yc+float(SA->ForcePointR[1])*sin(fi)*16.0f;

			SA->MoveQuant(1,XC0-XC1,YC0-YC1);
		};
	};
	if(TaleID!=0xFFFF){
		OneObject* OB=Group[TaleID];
		if(OB&&!OB->Sdoxlo){
			if(OB->CObjIndex!=0xFFFF){
				OneComplexObject* COB=CGroup[OB->CObjIndex];
				if(COB){
					OneComplexUnitDesc* OCUD=CODS.UnitDesc[OB->CObjIndex];
					float xc,yc;
					if(CODS.UnitDesc[CDIndex]->InverseTaleMotion){
						GetTalePoint(xc,yc);
					}else{
						GetHeadPoint(xc,yc);
					};
					if(OCUD->InverseTaleMotion){
						int x,y;
						GetTalePoint(x,y);
						COB->PerformBackShift(xc-x,yc-y);
					}else{
						float x,y;
						GetHeadPoint(x,y);
						COB->PerformBackShift(xc-x,yc-y);
					};
				};
			};
		};
	};
	XorBlockComplexUnit(this);
};


void ReadChainObjectsDesc(){
	CODS.ReadChainObjectsDesc();
};
word GetComplexObjectIndex(char* Name){
	return CODS.GetUnitID(Name);
};
extern int RealLx;
extern int RealLy;
extern int tmtmt;

void DrawComplexObject(OneComplexObject* COB,int sdx,int sdy,bool DrawOnlyModels)
{
	int N=COB->NQuants;
	OneComplexUnitDesc* COD=CODS.UnitDesc[COB->CDIndex];
	sdx<<=4;
	sdy<<=4;
	int RLX=(RealLx<<(4+5-Shifter))+128*16;
	int RLY=(RealLy<<(4+5-Shifter))+128*16;
	OneObject* OB=NULL;
	if(COB->ObjIndex!=0xFFFF){
		OB=Group[COB->ObjIndex];
	};
	if(!OB)return;

	bool GetObjectVisibilityInFog(int x,int y,int z,OneObject* OB);
	if(!GetObjectVisibilityInFog(OB->RealX>>4,OB->RealY>>4,OB->RZ,OB))return;

	bool CheckObjectVisibility(int x,int y,int z,int R);
	if(!CheckObjectVisibility(OB->RealX>>4,OB->RealY>>4,OB->RZ,250))return;
	
	for(int i=0;i<N;i++){
		SingleAxeQuant* SA=COB->MQ[i];
		int COS=TCos[byte(SA->Fi)];
		int SIN=TSin[byte(SA->Fi)];
		
		OneMQuantDesc* OCD=COD->Chain[i];
		QuantPartState* QPS=OCD->States[COB->FinalState];
        int gx=SA->xc-sdx;
        int gy=(SA->yc/2)-sdy;
        if(COB->StartState==COB->FinalState){
DR1:
			if(QPS){
				int NForw=0;
				for(int p=0;p<QPS->NElm;p++){
					bool DRAWIT=1;
					for(int j=0;j<COB->NHelpers;j++){
						int q=COB->HelpersPos[j]>>8;
						int qp=COB->HelpersPos[j]&255;
						if(q==i&&qp==p){
							if(COB->HelpersIDS[j]==0xFFFF)DRAWIT=0;
						}
					}
					if(DRAWIT){
						QuantSingleElement* QSE=QPS->Elm+p;
						if(QSE->AnmDir==3){
							//FORWARD MOTION
							if(NForw<MaxForw){
								float fdx,fdy,fi;
								COB->ForwDist[NForw]+=SA->GetShiftOfPoint(QSE->dx,QSE->dy,fdx,fdy);
								//COB->ForwDx[NForw]=(((COB->ForwDx[NForw])/1.3f)+fdx)/(1+1/1.3f);
								//COB->ForwDy[NForw]=(((COB->ForwDy[NForw])/1.3f)+fdy)/(1+1/1.3f);
								COB->ForwDx[NForw]=((COB->ForwDx[NForw])*47+fdx)/48;
								COB->ForwDy[NForw]=((COB->ForwDy[NForw])*47+fdy)/48;
								fi=atan2f(COB->ForwDy[NForw],COB->ForwDx[NForw])/AngScale;

								fi+=QSE->dfi;
								int T=COB->ForwDist[NForw];
								if(QSE->RotDir)T=-T;
								//int COS=TCos[byte(fi)];
								//int SIN=TSin[byte(fi)];
								int dx=gx+((QSE->dx*COS-QSE->dy*SIN)>>4);
								int dy=gy+((QSE->dy*COS+QSE->dx*SIN)>>5);
								if(QSE->ANM)QSE->ANM->DrawAt(dx,dy,fi,T,OB,OB?OB->NNUM:0,DrawOnlyModels);
								NForw++;
							}
						}else{
							int T=0;
							int A0=(SA->LeftAngle+SA->RightAngle)/2;
							int DA=(SA->RightAngle-SA->LeftAngle)*abs(QSE->dy)/128;
							switch(QSE->AnmDir){
							case 0:
								T=A0-DA;
								break;
							case 1:
								T=A0;
								break;
							case 2:
								T=A0+DA;
								break;
							};
							if(QSE->RotDir)T=-T;
							int dx=gx+((QSE->dx*COS-QSE->dy*SIN)>>4);
							int dy=gy+((QSE->dy*COS+QSE->dx*SIN)>>5);
							if(QSE->ANM)QSE->ANM->DrawAt(dx,dy,SA->Fi+QSE->dfi,T,OB,OB?OB->NNUM:0,DrawOnlyModels);
						}
					}
				}
			}
		}else{
			//drawing intermediate states
			OneMQuantDesc* OMQD=CODS.UnitDesc[COB->CDIndex]->Chain[i];
			QuantPartTransform* QPT=OMQD->TRANS[COB->StartState+COB->FinalState*NStates];
			if(QPT&&QPT->QP_Type==0&&QPS){
				for(int p=0;p<QPT->NParts;p++){
					bool DRAWIT=1;
					for(int j=0;j<COB->NHelpers;j++){
						int q=COB->HelpersPos[j]>>8;
						int qp=COB->HelpersPos[j]&255;
						if(q==i&&qp==p){
							if(COB->HelpersIDS[j]==0xFFFF)DRAWIT=0;
						}
					}
					if(DRAWIT){
						if(QPT->NInQPTE[p]==0||QPT->MaxTransfTime<=(COB->TransTime>>8)){
							QuantSingleElement* QSE=QPS->Elm+p;
							int T=0;
							int A0=(SA->LeftAngle+SA->RightAngle)/2;
							int DA=(SA->RightAngle-SA->LeftAngle)*QSE->dy/128;
							switch(QSE->AnmDir){
							case 0:
								T=A0-DA;
								break;
							case 1:
								T=A0;
								break;
							case 2:
								T=A0+DA;
								break;
							};
							int dx=gx+((QSE->dx*COS-QSE->dy*SIN)>>4);
							int dy=gy+((QSE->dy*COS+QSE->dx*SIN)>>5);
							if(QSE->ANM)QSE->ANM->DrawAt(dx,dy,SA->Fi+QSE->dfi,T,OB,OB?OB->NNUM:0,DrawOnlyModels);
						}else{
							QuantPartTransformElement* QPTE=QPT->QPTE[p];
							int N=QPT->NInQPTE[p];
							int T=COB->TransTime>>8;
							for(int v=0;v<N;v++){
								if(T>=QPTE->StartTime&&T<QPTE->StartTime+QPTE->TimeAmount){
									int D=QPTE->TimeAmount;
									int DT=T-QPTE->StartTime;
									int x=QPTE->x0+(QPTE->x1-QPTE->x0)*DT/D;
									int y=QPTE->y0+(QPTE->y1-QPTE->y0)*DT/D;
									int Fi=QPTE->Fi0+(QPTE->Fi1-QPTE->Fi0)*DT/D;
									int FID=QPTE->StartFrame+(QPTE->EndFrame+1-QPTE->StartFrame)*DT/D;
									int dx=gx+((x*COS-y*SIN)>>4);
									int dy=gy+((y*COS+x*SIN)>>5);
									if(QPTE->ANM)QPTE->ANM->DrawAt(dx,dy,Fi+byte(SA->Fi),FID,OB,OB?OB->NNUM:0,DrawOnlyModels);
									break;
								};
								QPTE++;
							}
						}
					}
				}
			}else goto DR1;
		};
	};
} // DrawComplexObject

void DrawAllComplexObjects(bool DrawOnlyModels){
    GPS.SetCurrentDiffuse( 0xFFFFFFFF );
    static int shID = IRS->GetShaderID( "sprite_buildings" );
    GPS.SetShader( shID );
    for(int i=0;i<MaxCObject;i++){
		OneComplexObject* OCO=CGroup[i];
		if(OCO){
			DrawComplexObject(OCO,mapx<<5,mapy<<4,DrawOnlyModels);
		};
	};
};
void ClearAllComplexObjects(){
	for(int i=0;i<MaxCObject;i++){
		if(CGroup[i])delete(CGroup[i]);
		CGroup[i]=NULL;
	}
	MaxCObject=0;
}
void DeleteOneComplexObject(int idx){
	if(idx>=0&&idx<MaxCObject&&CGroup[idx]){
        delete(CGroup[idx]);
		CGroup[idx]=NULL;
		if(idx==MaxCObject-1)MaxCObject--;
	}
}
word GetDir(int dx,int dy);
word getDir(int dx,int dy);
void GetForceActOnBar(int x,int y,int L,int& fx,int& fy,int Media=0);
void XorBlockComplexUnit(OneObject* OB);
void MakeSeveralStepsBack(OneObject* OB,char Dir,int N);
bool CheckStopRuleWhenLocked(OneObject* OB);
int PreCheckMotionRules(OneObject* OB);
void SetPositionsForHelpers(OneComplexObject* COB){
	OneComplexUnitDesc* OCUD=CODS.UnitDesc[COB->CDIndex];	
	if(COB->StartState==COB->FinalState){
		for(int i=0;i<COB->NHelpers;i++){
            int q=COB->HelpersPos[i]>>8;
			int qp=COB->HelpersPos[i]&255;
            int uid=COB->HelpersIDS[i];
			OneObject* OB=Group[uid];
			if(OB){
				SingleAxeQuant* SAQ=COB->MQ[q];
				OneMQuantDesc* OMQD=OCUD->Chain[q];
				QuantPartState* QPS=OMQD->States[COB->StartState];
				if(qp<QPS->NElm){
					//QuantSingleElement* QSE=QPS->Elm[qp];
					//int SIN=TSin[COB->]
					//OB->RealX=int(COB->RealX)+
				}
			}
		}
	}
}
bool OneComplexObject::GetPositionCoor(int QuantIdx,int QuantPos,int&x,int&y){
	OneComplexUnitDesc* OCUD=CODS.UnitDesc[CDIndex];	
	OneObject* OB=Group[ObjIndex];
	if(!OB)return false;
	SingleAxeQuant* SAQ=MQ[QuantIdx];
	OneMQuantDesc* OMQD=OCUD->Chain[QuantIdx];
	QuantPartState* QPS=OMQD->States[FinalState];
	if(StartState==FinalState){
simp:;			
		if(QuantPos<QPS->NElm){
			QuantSingleElement* QSE=QPS->Elm+QuantPos;
			float SIN=sin(SAQ->Fi*AngScale);
			float COS=cos(SAQ->Fi*AngScale);
			addrand(OB->RZ);
			x=(int(SAQ->xc)>>4)+QSE->dx*COS-QSE->dy*SIN;
			y=(int(SAQ->yc)>>4)+QSE->dx*SIN+QSE->dy*COS+OB->RZ*2;
			addrand(x);
			addrand(y);
			return true;
		}
	}else{
		QuantPartTransform* QPT=OMQD->TRANS[StartState+FinalState*NStates];
		if(QPT&&QPT->QP_Type==0&&QPS){			
			if(QPT->NInQPTE[QuantPos]==0||QPT->MaxTransfTime<=(TransTime>>8))goto simp;                                                            
			else{
				QuantPartTransformElement* QPTE=QPT->QPTE[QuantPos];
				int N=QPT->NInQPTE[QuantPos];
				int T=TransTime>>8;
				for(int v=0;v<N;v++){
					if(T>=QPTE->StartTime&&T<QPTE->StartTime+QPTE->TimeAmount){
						int D=QPTE->TimeAmount;
						int DT=T-QPTE->StartTime;
						int xx=QPTE->x0+(QPTE->x1-QPTE->x0)*DT/D;
						int yy=QPTE->y0+(QPTE->y1-QPTE->y0)*DT/D;
						float SIN=sin(SAQ->Fi*AngScale);
						float COS=cos(SAQ->Fi*AngScale);
						x=(int(SAQ->xc)>>4)+xx*COS-yy*SIN;
						y=(int(SAQ->yc)>>4)+xx*SIN+yy*COS+OB->RZ*2;
						addrand(x);
						addrand(y);
						return true;
					}
					QPTE++;
				}                    
			}
		}else goto simp;
	}
	return false;
}
bool DieComplexObject(OneObject* OB){
	if(OB->CObjIndex==0xFFFF)return true;
	addrand(OB->Index);
	addrand(OB->RealX);
	addrand(OB->RealY);
	OneComplexObject* COB=CGroup[OB->CObjIndex];
	if(!COB)return true;
	OneComplexUnitDesc* OCUD=CODS.UnitDesc[COB->CDIndex];
	int IDXS[8];
	int NIDXS=0;
	XorBlockComplexUnit(COB);
	int Stage=1;
	bool totdeath=0;
	if(COB->TaleID!=0xFFFF){
		OneObject* TOB=Group[COB->TaleID];
		if(TOB && TOB->Sdoxlo==0 && TOB->Serial==COB->TaleSN){
            TOB->Die();
			if(!TOB->Sdoxlo)TOB->Die();
		}
	}
	if(COB->LeaderID!=0xFFFF){
		OneObject* LOB=Group[COB->LeaderID];
		if(LOB && LOB->Sdoxlo==0 && LOB->Serial==COB->LeaderSN){
			totdeath=true;
		}
	}
	if(!OB->Hidden){
		for(int i=0;i<COB->NHelpers;i++)if(COB->HelpersIDS[i]!=0xFFFF){
			addrand(i);
			int q =COB->HelpersPos[i]>>8;
			int qp=COB->HelpersPos[i]&255;
			int x,y;
			if(COB->GetPositionCoor(q,qp,x,y)){
				addrand(x);
				addrand(y);
				OneMQuantDesc* OMQ=OCUD->Chain[q];
				for(int p=0;p<OMQ->NHelpers;p++){
					if(OMQ->HelpersPos[p]==qp){
						if(COB->HelpersSNS[i]!=0xFFFF&&COB->HelpersIDS[i]!=0xFFFF){
							OneObject* OBJ=Group[COB->HelpersIDS[i]];
							if(OBJ&&OBJ->Serial==COB->HelpersSNS[i]){
								OBJ->StandGround=0;
								OBJ->UnlimitedMotion=0;
								if(OMQ->HelpersOpt[p]&1)OBJ->Die();
								else{
									//OBJ->NewMonsterSendTo((OBJ->RealX*3-OB->RealX*2),(OBJ->RealY*3-OB->RealY*2),128+16,0);                            
									PanicUnit(OBJ);
								}
							}
						}else{
							COB->HelpersIDS[i]=0xFFFF;
							addrand(x<<4);
							addrand(y<<4);
							int idx=NATIONS[OB->NNUM].CreateNewMonsterAt(x<<4,y<<4,OMQ->Helpers[p],1,-1);
							if(idx!=-1){						
								OneObject* OBJ=Group[idx];
								if(OMQ->HelpersOpt[p]&1)OBJ->Die();
								else{
									//OBJ->NewMonsterSendTo((OBJ->RealX*3-OB->RealX*2),(OBJ->RealY*3-OB->RealY*2),128+16,0);
									PanicUnit(OBJ);
								}
								if(NIDXS<8){
									IDXS[NIDXS]=idx;
									NIDXS++;
								}
								Stage=0;
							}
						}
					}
				}						
			}else{
				XorBlockComplexUnit(COB);
				delete(COB);
				CGroup[OB->CObjIndex]=NULL;
				return true;
			}
		}	
		for(int i=0;i<OCUD->NQuants;i++){
			OneMQuantDesc* OMQD=OCUD->Chain[i];
			SingleAxeQuant* SAQ=COB->MQ[i];
			for(int j=0;j<OMQD->NExpl;j++){
				ExplodeInfo* EI=OMQD->ExpInf+j;
				if(EI->Stage==Stage){
					totdeath=1;
					if(EI->Particles){
						Create3DAnmObject(EI->Particles,SAQ->xc/16,SAQ->yc/16,GetHeight(SAQ->xc/16,SAQ->yc/16),SAQ->xc/16,SAQ->yc/16,-10,NULL,0,0xFFFF);
					}
				}
			}
		}
	}
	if(OB->Hidden)totdeath=true;
	if(NIDXS){		
		if(totdeath){
			for(int i=0;i<NIDXS;i++)Group[IDXS[i]]->Die();
		}else{
			int minny=1000000;
			int bid=-1;
			for(int i=0;i<NIDXS;i++){
				int y=Group[IDXS[i]]->RealY>>4;
				if(y<minny){
					minny=y;
					bid=IDXS[i];
				}
			}
			Group[bid]->Die();
		}
	}
	COB->Lx=4;
	XorBlockComplexUnit(COB);
	COB->NoAttack=1;
	COB->NoMove=1;
	if(totdeath){
		XorBlockComplexUnit(COB);
		delete(COB);
		CGroup[OB->CObjIndex]=NULL;
	}else{
		OB->Life=OB->MoreCharacter->Life;
		OB->MathMask=OB->newMons->MathMask;
	}
	return totdeath;
}
void SetHelpersPositions(OneComplexObject* COB){
	for(int i=0;i<COB->NHelpers;i++)if(COB->HelpersIDS[i]!=0xFFFF&&COB->HelpersSNS[i]!=0xFFFF){
		int q =COB->HelpersPos[i]>>8;
		int qp=COB->HelpersPos[i]&255;
		int x,y;
		if(COB->GetPositionCoor(q,qp,x,y)){
			OneObject* OB=Group[COB->HelpersIDS[i]];
			if(OB&&OB->Serial==COB->HelpersSNS[i]){
				OB->RealX=x<<4;
				OB->RealY=y<<4;
                OB->RZ=GetTotalHeight0(x,y);//Group[COB->ObjIndex]->RZ;
				addrand(OB->RZ);
				OB->DestX=-1;
				OB->DestY=-1;
				OB->PrioryLevel=0;
				OB->StandGround=1;						
				//OB->OverEarth=CODS.OMQD[q]->States[0]->Elm[qp].ANM->NA.AddHeight;
				OB->OverEarth=CODS.UnitDesc[COB->CDIndex]->Chain[q]->States[0]->Elm[qp].ANM->NA.AddHeight;
				void AttackObjLink(OneObject* OBJ);
				while(OB->LocalOrder&&OB->LocalOrder->DoLink!=&AttackObjLink){
					OB->DeleteLastOrder();
				}
				//if(!OB->Attack)OB->ClearOrders();
			}
		}
	}
}
bool TestFillingAbility(OneObject* OB);
void MotionHandlerForComplexObjects(OneObject* OB){

	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);

	if(OB->CObjIndex!=0xFFFF){
		OB->PathDelay=0;
		OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB){
            OB->UsesDestDidection=false;
			if(COB->NoMove){
				OB->DestX=-1;
                SetHelpersPositions(COB);
				return;
			}
			COB->CheckConnection();
			if(COB->LeaderID!=0xFFFF){
				SetHelpersPositions(COB);
				return;
			}
			int F=COB->FinalState;
			int S=COB->StartState;
			if( (S==8 || S>=12) && S!=F && (F==12||F==16||F==20) && COB->TransTime>1024 && COB->Charged==false){
				COB->TestResSubtract();
				if(COB->ResSubtracted){
					COB->Charged=true;
				}
			}
			bool AllReady=1;
			OneComplexObject* CURCOB=COB;
			if(OB->BackMotion)OB->RealDir=byte(COB->MQ[COB->NQuants-1]->Fi)+128;
			else OB->RealDir=byte(COB->MQ[0]->Fi);
			if(OB->DestX>0)COB->DestDir=-1;
			if(OB->DestX>0||COB->DestDir!=-1){
				void CancelRechargement(OneObject* OB);
				CancelRechargement(OB);
			}
			if(COB->GroundStandState==8&&!OB->LocalOrder){
				COB->GroundStandState=12+OB->LocalNewState*4;
			}
			if(COB->StartState==8 && COB->FinalState==8 && !OB->LocalOrder){
				COB->GroundStandState=12+OB->LocalNewState*4;
				COB->FinalState=COB->GroundStandState;
			}
			do{
				if(CURCOB->StartState!=CURCOB->FinalState){
					bool fail=0;
					for(int i=0;i<CURCOB->NQuants;i++){
						OneMQuantDesc* OMQD=CODS.UnitDesc[CURCOB->CDIndex]->Chain[i];
						QuantPartTransform* QPT=OMQD->TRANS[CURCOB->StartState+CURCOB->FinalState*NStates];
						if(QPT){
							if(QPT->QP_Type!=255&&(CURCOB->TransTime>>8)<QPT->MaxTransfTime)fail=1;
						}else fail=1;
					};
					if(!fail){
						CURCOB->StartState=CURCOB->FinalState;
						assert(!(COB->StartState==1&&COB->FinalState==2));
						assert(!(CURCOB->StartState==1&&CURCOB->FinalState==2));
						CURCOB->TransTime=0;
					}else{
						CURCOB->TransTime+=GameSpeed;
					};
					if(fail)AllReady=0;
				};
				if(CURCOB->TaleID!=0xFFFF){
					OneObject* NOB=Group[CURCOB->TaleID];
					if(NOB){
						if(NOB->CObjIndex!=0xFFFF){
							CURCOB=CGroup[NOB->CObjIndex];
							if(!CURCOB)break;
						}else break;
					}else break;
				}else break;
			}while(1);
			if(AllReady){
				//OB->RealX=COB->MQ[0]->xc;
				//OB->RealY=COB->MQ[0]->yc;
				//OB->x=(OB->RealX-((OB->Lx)<<7))>>8;
				//OB->y=(OB->RealY-((OB->Lx)<<7))>>8;
				int RULE=OB->DestX>=0?PreCheckMotionRules(OB):0;
				if(COB->DestDir!=-1){
					COB->PerformRotation(COB->DestDir);
					if(COB->DestDir==OB->RealDir)COB->DestDir=-1;
				}
				if(OB->DestX<0&&COB->DestDir==-1){
                    OB->StandTime++;
				}else{
					OB->StandTime=0;
				}
				if((OB->DestX<0&&OB->StandTime>5)||RULE==1){
					if(COB->DestDir==-1){
						int S=COB->StartState;
						int F=COB->FinalState;
						COB->TestResSubtract();
						if( COB->ResSubtracted && S==F && F==COB->GroundStandState && !COB->Charged ){
							if(S==F){
								if(OB->LocalNewState==0){
									COB->StartState=16;
									COB->FinalState=12;
									COB->GroundStandState=COB->FinalState;
									COB->TransTime=0;
								}else{
									COB->StartState=12;
									COB->FinalState=12+OB->LocalNewState*4;
									COB->GroundStandState=COB->FinalState;
									COB->TransTime=0;
								}
							}
						}else{
							COB->TryToTransformToStandState();
						}
					}
					SetHelpersPositions(COB);
					return;
				};
				int RX,RY;
				bool Back=OB->BackMotion;
				if(Back){
					COB->GetTalePoint(RX,RY);
				}else{
					RX=COB->RealX;
					RY=COB->RealY;
				};
				float dx=OB->DestX-RX;
				float dy=OB->DestY-RY;
				if(OB->DestX<0){
					dx=0;
					dy=0;
				}
				int LX=OB->Lx;
				int fx=0;
				int fy=0;
				XorBlockComplexUnit(COB);
				if(!OB->UnlimitedMotion)GetForceActOnBar(OB->RealX>>4,OB->RealY>>4,LX,fx,fy,0);//OB->Media);
				XorBlockComplexUnit(COB);
				int N=Norma(dx,dy);
				if(RULE==2){
					fx-=(dy*64)/N;
					fy+=(dx*64)/N;
				};
				int MaxD=N<16*80?70:32;
				if(OB->UnlimitedMotion)MaxD=80;
				else{
					MaxD+=GameSpeed/32;
					if(MaxD>70)MaxD=70;
				}

				//if(COB->NQuants==1)MaxD=127;
				if(fx||fy){
					int FN=Norma(fx,fy);
					if(FN>300){
						if(CheckStopRuleWhenLocked(OB)){
							OB->DestX=-1;
							OB->DestY=-1;
							COB->TryToTransformToStandState();
							return;
						};
						if(MaxD<68){
							MaxD=68;
							//OB->NIPoints=0;
							//int SC=(dx*fx+dy*fy)*256/Norma(dx,dy)/Norma(fx,fy);
							//if(SC>100){
							//	MakeSeveralStepsBack(OB,GetDir(fx,fy)+128,64);
								//return;
							//};
						};
					};
					if(N>128){
						dx=(dx*256)/N;
						dy=(dy*256)/N;
					};
					dx+=fx;
					dy+=fy;
					
					int N=Norma(dx,dy);
				};
				//if(COB->NQuants==1)MaxD=127;
				int N0=64;//Back?32:64;
				if(OB->UnlimitedMotion)N0=16;
				if(N>N0){
					if(COB->StartState==COB->GroundMotionState){
						char Ang=char(GetDir(dx,dy));
						int R=Norma(dx,dy)>>4;
						bool StopMode=0;
						if(Back){
							char A0=char(int(COB->MQ[COB->NQuants-1]->Fi));
							A0=128+A0;
							char D=Ang-A0;
							if(abs(D)>16){
								//if(D<0)D=-16;
								//else D=16;
								COB->PerformRotation(Ang+128);
								SetHelpersPositions(COB);
								return;
							};
							Ang=A0+D;
							dx=TCos[byte(Ang)];
							dy=TSin[byte(Ang)];
							int N0=Norma(dx,dy);
							//if(N0>N){
								dx=dx*N/N0;
								dy=dy*N/N0;
							//};
						}else{
							char A0=char(int(COB->MQ[0]->Fi));
							char D=Ang-A0;
							if(abs(D)>MaxD){
								if(COB->NQuants!=1||R<128||OB->newMons->MinRotator<=8){
									if(D<0)D=-MaxD;
									else D=MaxD;
								}else{
									COB->PerformRotation(Ang);
									SetHelpersPositions(COB);
									return;
								}
							};
							Ang=A0+D;
							dx=TCos[byte(Ang)];
							dy=TSin[byte(Ang)];
							int N0=Norma(dx,dy);
							dx=dx*N/N0;
							dy=dy*N/N0;
						};
						int V=(N-N0+12)>>1;
						if(StopMode)V=0;
						//int MSP=(OB->GroupSpeed*GameSpeed*OB->OB->UnitSpeed)>>(8+6);
						int sp=OB->GroupSpeed;
						bool stun=false;
						if(OB->ActiveAbility){							
							OB->ActiveAbility->ActiveAbilities.modifyMotionSpeed(sp,sp,stun);
						}
						int MSP=(sp*GameSpeed)>>8;
						if(OB->newMons->SpeedScaleOnTrees){
							if(MFIELDS[2].CheckPt(OB->x,OB->y)){
								MSP=(MSP*OB->newMons->SpeedScaleOnTrees)>>8;
							}
						}
						///if(MSP>180)MSP=180;
						if(V>MSP)V=MSP;
						dx=dx*V/N;
						dy=dy*V/N;
						XorBlockComplexUnit(COB);
						COB->RealX+=dx;
						COB->RealY+=dy;
						XorBlockComplexUnit(COB);

						addrand(OB->RealX);
						addrand(OB->RealY);
						OB->RealX=COB->RealX;
						OB->RealY=COB->RealY;
						addrand(OB->RealX);
						addrand(OB->RealY);

						OB->x=(OB->RealX-((OB->Lx)<<7))>>8;
						OB->y=(OB->RealY-((OB->Lx)<<7))>>8;
						int rx=OB->RealX>>4;
						int ry=OB->RealY>>4;
						float GetFHeight(int x,int y);
						float z3=(GetFHeight(rx-24,ry)+GetFHeight(rx+24,ry)+GetFHeight(rx,ry-24)+GetFHeight(rx,ry+24))/4.0f;//+GetHeight(rx+18,ry)+GetHeight(rx,ry-18)+GetHeight(rx,ry+18)+
							  //GetHeight(rx-18,ry-18)+GetHeight(rx+18,ry-18)+GetHeight(rx-18,ry+18)+GetHeight(rx+18,ry+18);
						OB->RZ=z3;//>>3;
						addrand(OB->RZ);
						float zz=z3*32.0f;//<<2;
						if(Back){
							float x,y;
							COB->GetTalePoint(x,y);
							float Gdy=OB->RealY-zz-y;
							float Gdx=OB->RealX-x;
							COB->PerformBackShift(Gdx,Gdy);
						}else{ 
							float x,y;
							COB->GetHeadPoint(x,y);
							float Gdy=COB->RealY-zz-y;
							float Gdx=COB->RealX-x;
							//Gdx/=8.0f;
							//Gdy/=8.0f;
							Gdx*=0.1;
							Gdy*=0.1;
							int N=Norma(Gdx,Gdy);
							if(N>256){
								//int Sp=OB->newMons->MotionDist;
								//Gdx=Gdx*256/N;
								//Gdy=Gdy*256/N;

							};
							COB->PerformShift(Gdx,Gdy);
						};
					}else{
						//COB->TryToTransform(COB->GroundMotionState);
						COB->TryToTransformToMotionState();
					};
				}else{
ONSTOP:
					OB->DestX=-1;
					OB->DestY=-1;
					//COB->TryToTransform(COB->GroundStandState);
					if(OB->StandTime>5)COB->TryToTransformToStandState();
				};				
			}			
			SetHelpersPositions(COB);
		}
	}
};
#define CONFI(x) ((byte(x)+2)&252)
void SetStartConditionsForComplexObject(OneComplexObject* COB,OneComplexUnitDesc* COD,int x,int y,byte Dir){
	for(int i=0;i<MaxForw;i++)COB->ForwDist[i]=0;
	for(int j=0;j<COB->NQuants;j++){
		SingleAxeQuant* SA=COB->MQ[j];
		SA->Fi=Dir;
		SA->zc=0;
		SA->ForcePointR[0]=COD->Chain[j]->X1;
		SA->ForcePointR[1]=COD->Chain[j]->X2;
		if(j==0){
			SA->xc=x<<4;
			SA->yc=y<<4;
		}else{
			SingleAxeQuant* SP=COB->MQ[j-1];
			SA->xc=SP->xc+((SP->ForcePointR[1]*TCos[CONFI(SP->Fi)])>>4)-((SA->ForcePointR[0]*TCos[CONFI(SA->Fi)])>>4);
			SA->yc=SP->yc+((SP->ForcePointR[1]*TSin[CONFI(SP->Fi)])>>4)-((SA->ForcePointR[0]*TSin[CONFI(SA->Fi)])>>4);
		};
		SA->xc0=SA->xc;
		SA->yc0=SA->yc;
		SA->zc0=SA->zc;
		SA->AxeL=64;//COD->Chain[j]->R;//AxeDistance!!!  //COD->OCD[j].AxeDist;
		SA->LeftAngle=0;
		SA->RightAngle=0;
	};
};
void OneComplexObject::TryToTransform(int FinState){
	if(StartState==FinalState&&StartState!=FinState){
		int T01=FinalState&3;
		int T02=FinalState>>2;
		int T11=FinState&3;
		int T12=FinState>>2;
		if(T01==T11){
			OneMQuantDesc* OMQD=CODS.UnitDesc[CDIndex]->Chain[0];
			QuantPartTransform* QPT=OMQD->TRANS[StartState+FinState*NStates];
			if(QPT)FinalState=FinState;
			else{
				FinalState=T11;
				QuantPartTransform* QPT=OMQD->TRANS[StartState+FinState*NStates];
				if(!QPT){
					if(StartState==8)FinalState=12;
				}
			}
		}else if(T02==0)FinalState=T11;
		else FinalState=T01;
		/*
		ComplexUnitDesc* OCUD=CODS.UnitDesc[CDIndex];
		QuantPartTransform* QPT=OMQD->TRANS[StartState+FinalState*NStates];
		bool ready
		if(QPT&&QPT->QP_Type==255)StartState=FinalState;
		*/
	};
	/*
	if(TaleID!=0xFFFF){
		OneObject* TOB=Group[TaleID];
		if(TOB&&TOB->Serial==TaleSN&&TOB->CObjIndex!=0xFFFF){
			OneComplexObject* COB=CGroup[TOB->CObjIndex];
			if(COB)COB->TryToTransform(FinState);
		};
	};
	*/
};
void OneComplexObject::TryToTransformToStandState(){
	if(StartState==8)return;//attack
	TryToTransform(GroundStandState);
	if(TaleID!=0xFFFF){
		OneObject* TOB=Group[TaleID];
		if(TOB&&TOB->Serial==TaleSN&&TOB->CObjIndex!=0xFFFF){
			OneComplexObject* COB=CGroup[TOB->CObjIndex];
			if(COB)COB->TryToTransformToStandState();
		};
	};
};
void OneComplexObject::TryToTransformToMotionState(){
	TryToTransform(GroundMotionState);
	if(TaleID!=0xFFFF){
		OneObject* TOB=Group[TaleID];
		if(TOB&&TOB->Serial==TaleSN&&TOB->CObjIndex!=0xFFFF){
			OneComplexObject* COB=CGroup[TOB->CObjIndex];
			if(COB)COB->TryToTransformToMotionState();
		};
	};
};
int CreateComplexObjectAt(int x,int y,word Type,byte NI,word Owner,byte StartDir){
	OneObject* OB=Group[Owner];
	int ID=-1;
	for(int i=0;i<MaxCObject;i++)if(!CGroup[i]){
		ID=i;
		break;
	};
	if(ID==-1){
		if(MaxCObject<1023){
			ID=MaxCObject;
			MaxCObject++;
		}else return -1;		
	};
	OneComplexObject* COB=CGroup[ID]=new OneComplexObject;
	OneComplexUnitDesc* COD=CODS.UnitDesc[Type];
	y-=OB->RZ*2;
	COB->CDIndex=Type;
	COB->NI=NI;
	COB->ObjIndex=Owner;
	for(int i=0;i<COD->NQuants;i++)COB->MQ[i]=new SingleAxeQuant;
	COB->NQuants=COD->NQuants;
	SetStartConditionsForComplexObject(COB,COD,x,y,StartDir);
	COB->Lx=Group[Owner]->Lx;
	COB->StartState=0;
	COB->FinalState=0;
	COB->LeaderID=0xFFFF;
	COB->TaleID=0xFFFF;
	float xx,yy;
	COB->GetHeadPoint(xx,yy);
	COB->RealX=xx;
	COB->RealY=yy+OB->RZ*32;
	COB->DestDir=-1;
	COB->Charged=true;
	COB->ResSubtracted=true;
	COB->Lockpoints=false;
	OB->RealX=COB->RealX;
	OB->RealY=COB->RealY;
	OB->MathMask=OB->newMons->CO_MathMask;
	XorBlockComplexUnit(COB);
	//creating extra objects
	int NH=0;
	OneComplexUnitDesc* OCUD=CODS.UnitDesc[COB->CDIndex];
	for(int i=0;i<OCUD->NQuants;i++){
        OneMQuantDesc* OMQ=OCUD->Chain[i];
		for(int j=0;j<OMQ->NHelpers;j++){
			//int id=NATIONS[NI].CreateNewMonsterAt(0,0,OMQ->Helpers[i],1);
			//if(id!=-1){
				//OneObject* OB=Group[id];
				if(NH<MaxHelperUnits){
					if(OMQ->HelpersOpt[j]&1){
						int id=NATIONS[NI].CreateNewMonsterAt(0,0,OMQ->Helpers[j],1);
						if(id!=-1){
							OneObject* OB=Group[id];
							OB->NotSavable=1;
							OB->CantRestrictMotion=1;
							OB->NotSelectable=1;
							COB->HelpersIDS[NH]=id;
							COB->HelpersSNS[NH]=OB->Serial;
							COB->HelpersPos[NH]=(i<<8)+OMQ->HelpersPos[j];							
							NH++;
						}else{
							COB->HelpersIDS[NH]=0;//id;
							COB->HelpersSNS[NH]=0xFFFF;
							COB->HelpersPos[NH]=(i<<8)+OMQ->HelpersPos[j];
							NH++;
						}
					}else{
						COB->HelpersIDS[NH]=0;//id;
						COB->HelpersSNS[NH]=0xFFFF;
						COB->HelpersPos[NH]=(i<<8)+OMQ->HelpersPos[j];
						NH++;
					}
				}
			//}
		}
	}
	COB->NHelpers=NH;
	SetPositionsForHelpers(COB);
	return ID;
};
void SetPositionForCOB(OneObject* OB,int x,int y,int Dir){
	OneComplexObject* COB=CGroup[OB->CObjIndex];
	if(COB){
		int Type=OB->newMons->ComplexObjIndex;	
		OneComplexUnitDesc* COD=CODS.UnitDesc[Type];
		SingleAxeQuant* SA=COB->MQ[0];
		int d=Norma(SA->xc/16-x,SA->yc/16-y);
		if(d<1)d=1;
		int r0=8*GameSpeed/256;
		if(r0<2)r0=2;
		if(r0>20)r0=20;
		if(d>r0){
			int dx=SA->xc-x*16;
			int dy=SA->yc-y*16;
			dx=dx*128/d;
			dy=dy*128/d;
            OB->DestX=x*16-dx;
			OB->DestY=y*16-dy;
		}else{
			OB->DestX=-1;
			OB->DestY=-1;
			void ChangeUnitCoor(OneObject* OB,int newX,int newY);
			word M=OB->CObjIndex;
			OB->CObjIndex=0xFFFF;
			addrand(x);
			addrand(y);
			ChangeUnitCoor(OB,x<<4,y<<4);
			OB->CObjIndex=M;
			COB->RealX=x<<4;
			COB->RealY=y<<4;
			SetStartConditionsForComplexObject(COB,COD,x,y,Dir);
			OB->UnlimitedMotion=false;
		}
	}
}
void SetPositionForCOB_ONLY(OneObject* OB,int x,int y,int Dir){
	OneComplexObject* COB=CGroup[OB->CObjIndex];
	if(COB){
		int Type=OB->newMons->ComplexObjIndex;	
		OneComplexUnitDesc* COD=CODS.UnitDesc[Type];
		SingleAxeQuant* SA=COB->MQ[0];		
		OB->DestX=-1;
		OB->DestY=-1;		
		COB->RealX=x<<4;
		COB->RealY=y<<4;
		SetStartConditionsForComplexObject(COB,COD,x,y,Dir);
		OB->UnlimitedMotion=false;
	}
}
bool CheckObjectLikeHead(word MID){
	OneObject* OB=Group[MID];
	if(OB&&!OB->Sdoxlo){
		if(OB->CObjIndex==0xFFFF)return false;
		OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB->TaleID!=0xFFFF)return false;
		OneComplexUnitDesc* OCUD=CODS.UnitDesc[COB->CDIndex];
		return OCUD->CanBeHead;
	};
	return false;
};
bool CheckObjectLikeTale(word MID){
	OneObject* OB=Group[MID];
	if(OB&&!OB->Sdoxlo){
		if(OB->CObjIndex==0xFFFF)return false;
		OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB->TaleID!=0xFFFF)return false;
		OneComplexUnitDesc* OCUD=CODS.UnitDesc[COB->CDIndex];
		return OCUD->CanBeTale;
	};
	return false;
};
void ConnectToObjectLink(OneObject* OBJ){
	word Head=OBJ->LocalOrder->info.BuildObj.ObjIndex;
	word HSN= OBJ->LocalOrder->info.BuildObj.SN;
	byte STAGE=OBJ->LocalOrder->info.BuildObj.AttMethod;
	OneObject* HOB=Group[Head];
	if(HOB&&HOB->Serial==HSN&&!HOB->Sdoxlo){
		OneComplexObject*TCOB=CGroup[OBJ->CObjIndex];
		OneComplexObject* HCOB=CGroup[HOB->CObjIndex];
		OneComplexUnitDesc* TOCUD=CODS.UnitDesc[HCOB->CDIndex];
		OneComplexUnitDesc* HOCUD=CODS.UnitDesc[HCOB->CDIndex];
		int xhh,yhh,xht,yht;
		HCOB->GetHeadPoint(xhh,yhh);
		HCOB->GetTalePoint(xht,yht);
		int dx=xht-xhh;
		int dy=yht-yhh;
		int N=Norma(dx,dy);
		int RR=200;
		if(OBJ->BackMotion)RR=0;
		dx=dx*RR*16/N;
		dy=dy*RR*16/N;
		
		int xdst=xht+dx;
		int ydst=yht+dy;

		int txc=TCOB->MQ[0]->xc;
		int tyc=TCOB->MQ[0]->yc+OBJ->RZ*32;

		int dx1=xdst-txc;
		int dy1=ydst-tyc;
		int N1=Norma(dx1,dy1);
		int dr=TCOB->MQ[0]->ForcePointR[0]+20;
		if(N1>90*16){
			dx1=dx1*dr*16/N1;
			dy1=dy1*dr*16/N1;
		}else{
			dx1=0;
			dy1=0;
		}
		xdst+=dx1;
		ydst+=dy1;

		int xth,yth;
		if(OBJ->BackMotion)TCOB->GetTalePoint(xth,yth);
		else TCOB->GetHeadPoint(xth,yth);
		int R=Norma(xth-xdst,yth-ydst);
		
		if(N1>40*16&&!OBJ->BackMotion){
			if(TCOB->FinalState!=TCOB->StartState&&TCOB->FinalState==0)TCOB->FinalState=TCOB->StartState;
			XorBlockComplexUnit(TCOB);
			XorBlockComplexUnit(HCOB);
			bool C=CheckBar( (xdst>>8)-6,(ydst>>8)-6,13,13);
			XorBlockComplexUnit(HCOB);
			XorBlockComplexUnit(TCOB);			
			if(C){
				HOB->NewMonsterPreciseSendTo(HOB->RealX+TCos[HOB->RealDir]*4,HOB->RealY+TSin[HOB->RealDir]*4,128+16,0);
				//OBJ->DeleteLastOrder();
				return;
			}
			OBJ->NewMonsterPreciseSendTo(xdst,ydst,128+16,1);
			return;
		}else{
			if(TCOB->FinalState==0)TCOB->FinalState=TCOB->StartState;
			OBJ->DestX=xht;
			OBJ->DestY=yht;
			TCOB->SetBackState(1);
			OBJ->UnlimitedMotion=1;
			OBJ->ImSelected=0;
			OBJ->Selected=0;
			if(Norma(xth-xht,yth-yht)<64){
				TCOB->SetBackState(0);
				OBJ->DestX=-1;
				OBJ->DestY=-1;
				OBJ->DeleteLastOrder();
				OBJ->BackMotion=0;
				HCOB->TaleID=OBJ->Index;
				HCOB->TaleSN=OBJ->Serial;
				TCOB->LeaderID=HOB->Index;
				TCOB->LeaderSN=HOB->Serial;
				HCOB->GroundMotionState=5;
				HCOB->GroundStandState=1;
				TCOB->GroundMotionState=6;
				TCOB->GroundStandState=2;
				HCOB->TryToTransformToStandState();
				TCOB->TryToTransformToStandState();
				XorBlockComplexUnit(TCOB);
				return;

			}
		}
	}else{
		OBJ->DeleteLastOrder();
		//OBJ->BackMotion=0;
	}
}
void ConnectToObject(word Head,word Tail){
	OneObject* HOB=Group[Head];
	if(HOB&&HOB->CObjIndex!=0xFFFF&&!HOB->Sdoxlo){
		OneComplexObject*HCOB=CGroup[HOB->CObjIndex];
		if(HCOB){
			OneComplexUnitDesc* HOCUD=CODS.UnitDesc[HCOB->CDIndex];
			if(HOCUD->CanBeHead){
				OneObject* TOB=Group[Tail];
				if(TOB&&!TOB->Sdoxlo){
					OneComplexObject*TCOB=CGroup[TOB->CObjIndex];
					if(TCOB){
						OneComplexUnitDesc* TOCUD=CODS.UnitDesc[TCOB->CDIndex];
						if(TOCUD->CanBeTale){
							HOB->UnlimitedMotion=0;
							addrand(1006);
							Order1* OR1=HOB->CreateOrder(0);
							if(OR1){
								OR1->info.BuildObj.ObjIndex=Tail;
								OR1->info.BuildObj.SN=TOB->Serial;
								OR1->info.BuildObj.AttMethod=0;
								OR1->DoLink=&ConnectToObjectLink;
								HOB->BackMotion=0;
							}
						};
					};
				};
			};
		};
	};
};
void DisconnectFromObjectLink(OneObject* OBJ){
    OneComplexObject* HCOB=CGroup[OBJ->CObjIndex];
	if(HCOB&&HCOB->TaleID!=0xFFFF){
		OneObject* TOB=Group[HCOB->TaleID];
		if(TOB&&TOB->Serial==HCOB->TaleSN){
			OneComplexObject* TCOB=CGroup[TOB->CObjIndex];
			if(TCOB){
				if(TCOB->StartState==TCOB->FinalState){
					TOB->UnlimitedMotion=0;
					TCOB->SetBackState(0);
					TCOB->LeaderID=0xFFFF;
					HCOB->TaleID=0xFFFF;
					TCOB->GroundStandState=0;
					HCOB->GroundStandState=0;
					if(TCOB->FinalState==0){
						OBJ->DeleteLastOrder();
						return;
					}else{
						TCOB->TryToTransform(0);
						XorBlockComplexUnit(TCOB);
						return;
					}
				}
				return;
			}
		}
	}
	OBJ->DeleteLastOrder();
}
void DisconnectFromObject(OneObject* Head){
	if(Head->CObjIndex!=0xFFFF){
		OneComplexObject* HCOB=CGroup[Head->CObjIndex];
		if(HCOB&&HCOB->TaleID!=0xFFFF){
			addrand(1005);
            Order1* OR1=Head->CreateOrder(0);
			if(OR1){
				OR1->DoLink=&DisconnectFromObjectLink;
			}
		}
	}
}
bool TestDisconnectAbility(word HeadID){
	if(HeadID!=0xFFFF){
		OneObject* OB=Group[HeadID];
		if(OB&&OB->CObjIndex!=0xFFFF&&!OB->Sdoxlo){
            OneComplexObject* HCOB=CGroup[OB->CObjIndex];
			if(HCOB->TaleID!=0xFFFF){
				return !(OB->LocalOrder&&OB->LocalOrder->DoLink==&DisconnectFromObjectLink);
			}
		}
	}
	return false;
}
bool TestFillingAbility(OneObject* OB){
	if(OB->CObjIndex==0xFFFF)return false;
	OneComplexObject* COB=CGroup[OB->CObjIndex];
	if(COB){
		for(int i=0;i<COB->NHelpers;i++)if(COB->HelpersIDS[i]==0xFFFF)return true;
	}
	return false;
}
typedef bool EnumUnitsCallback(OneObject* OB,int x,int y,int r,DWORD param);
int EnumAllUnitsInCircle(int x,int y,int R,EnumUnitsCallback* fn,DWORD param);

struct FillObjParam{
	byte NI;
	int x0;
	int y0;
	char dir0;
	word* IDS;
	word* SNS;
	int N;
	word DestID;
	int MinR;
};
bool fnFillObjCallback(OneObject* OB,int x,int y,int r,DWORD param){
	if(OB->CObjIndex!=0xFFFF)return false;	
	if(OB->newMons->Usage==CostlyID  || OB->UnlimitedMotion)return false;
	if(OB->newMons->DontFillCannon)return false;
	FillObjParam* FOP=(FillObjParam*)param;
	if(OB->NNUM!=FOP->NI)return false;
	int R=Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
	int dd=abs(int(char(GetDir((OB->RealX>>4)-FOP->x0,(OB->RealY>>4)-FOP->y0)-FOP->dir0)));        
	//if(dd<40)R>>=2;
	//else R<<=2;
	if(R<FOP->MinR){		
		for(int i=0;i<FOP->N;i++)if(OB->Index==FOP->IDS[i])return false;
		FOP->DestID=OB->Index;
		FOP->MinR=R;
		return true;
	}
	return false;
}
void FillObjectByUnitsLink(OneObject* OBJ){
	bool something=0;
	bool somethingwrong=0;	
	if(TestFillingAbility(OBJ)){
		OneComplexObject* COB=CGroup[OBJ->CObjIndex];
		if(COB){
			COB->Charged=false;
			COB->ResSubtracted=false;
			int x=OBJ->GetAttX();
			int y=OBJ->GetAttY();
			for(int i=0;i<COB->NHelpers&&i<MaxFillObj;i++){
				if(COB->HelpersIDS[i]==0xFFFF){
					somethingwrong=1;
                    FillObjParam FOP;
					FOP.IDS=&OBJ->LocalOrder->info.FillObject.IDS[0];
					FOP.SNS=&OBJ->LocalOrder->info.FillObject.SNS[0];
					FOP.x0=OBJ->RealX>>4;
					FOP.y0=OBJ->RealY>>4;
					FOP.NI=OBJ->NNUM;
					int xxx,yyy;
					if(COB->GetPositionCoor(COB->HelpersPos[i]>>8,COB->HelpersPos[i]&255,x,y)){
						FOP.dir0=GetDir(x-FOP.x0,y-FOP.y0);
						if(FOP.IDS[i]==0xFFFF){
							FOP.N=MaxFillObj;
							FOP.MinR=1000000;
							FOP.DestID=0xFFFF;						
							if(EnumAllUnitsInCircle(x,y,512,&fnFillObjCallback,(DWORD)&FOP)){						
								OneObject* OB=Group[FOP.DestID];
								OBJ->LocalOrder->info.FillObject.IDS[i]=OB->Index;
								OBJ->LocalOrder->info.FillObject.SNS[i]=OB->Serial;
								OB->StandGround=0;
								if(OB->BrigadeID!=0xFFFF){
									void RemoveFromFormation(OneObject* OB);
									RemoveFromFormation(OB);
								}
								OB->UnitSpeed=64;
								OB->NewAnm=OB->newMons->GetAnimation(anm_Stand);
								OB->delay=0;
								OB->SetZeroFrame();
								OB->NewState=0;
								OB->LocalNewState=0;

								OB->NewMonsterPreciseSendTo(x<<4,y<<4,128+17,0);
								something=1;
							}
						}else{
							something=1;
							OneObject* OB=Group[FOP.IDS[i]];
							if(OB&&(!OB->Sdoxlo)&&OB->Serial==FOP.SNS[i]){
								if(!(OB->LocalOrder&&OB->PrioryLevel==17)){
									if(Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y)<20){
										COB->HelpersIDS[i]=0;
										OB->Die();
										OB->Sdoxlo=30000;
										somethingwrong=0;
										for(int q=0;q<COB->NHelpers&&q<MaxFillObj;q++)if(COB->HelpersIDS[q]==0xFFFF)somethingwrong=1;
										OBJ->MathMask=OBJ->newMons->CO_MathMask;;
									}else{
										OBJ->LocalOrder->info.FillObject.IDS[i]=0xFFFF;
										OBJ->LocalOrder->info.FillObject.SNS[i]=0xFFFF;
									}
								}                                                        
							}else{
								OBJ->LocalOrder->info.FillObject.IDS[i]=0xFFFF;
								OBJ->LocalOrder->info.FillObject.SNS[i]=0xFFFF;
							}
						}
					}
				}
			}
		}
		if(!somethingwrong){
			COB->NoAttack=0;
			COB->NoMove=0;
		}
		if(!something){
			OBJ->DeleteLastOrder();
		}
	}else{
		OBJ->DeleteLastOrder();
	}
}
void DrawCannonFillRadius(OneObject* OB){
	int x=OB->GetAttX()>>4;
	int y=OB->GetAttY()>>4;
	void DrawColoredCircle(int xc,int yc,int R0,int R1,DWORD LineColor1,DWORD LineColor2,DWORD FillColor1,DWORD FillColor2);
	FList.AddCircleSomewhere(x,y,0,500,0x00FFFFFF,0x00FFFFFF,EngSettings.FillCannonFillColor1,EngSettings.FillCannonFillColor2);
}
void FillObjectByUnits(OneObject* OB){
	if(TestFillingAbility(OB)){
        Order1* OR1=OB->CreateOrder(0);
		if(OR1){
			OR1->DoLink=&FillObjectByUnitsLink;
			OR1->PrioryLevel=16;
			for(int j=0;j<MaxFillObj;j++){
				OR1->info.FillObject.IDS[j]=0xFFFF;
				OR1->info.FillObject.SNS[j]=0xFFFF;
			}
		}
	}
}
int CONNECTPTR=4;
void SetCurPtr(int v);
void CmdConnectObjs(byte NI,word ID,word CurObj);
void TestConnector(word CurObj,byte NI){
	if(ImNSL[NI]==1&&ImSelm[NI]){
		word ID=ImSelm[NI][0];
		if(ID!=0xFFFF&&ID!=CurObj){
			if(CheckObjectLikeTale(ID)&&CheckObjectLikeHead(CurObj)){
				SetCurPtr(CONNECTPTR);
				if(Rpressed){
					Rpressed=0;
					CmdConnectObjs(NI,CurObj,ID);
				}
			}
			if(CheckObjectLikeTale(CurObj)&&CheckObjectLikeHead(ID)){
				SetCurPtr(CONNECTPTR);
				if(Rpressed){
					Rpressed=0;
					CmdConnectObjs(NI,ID,CurObj);
				}
			}
		}
	}
}
bool CheckDirectConnectionToDestPoint(int xc,int yc,int x,int y){
	if(Norma(xc-x,yc-y)>30)return false;
	int sx=xc>x?-1:1;
	int sy=yc>y?-1:1;
	int ax=abs(xc-x);
	int ay=abs(yc-y);
	int xp=xc;
	int yp=yc;
	if(ax>ay){
		int cum=ax>>1;
		while(xc!=x){
			xc+=sx;
			cum+=ay;
			if(cum>=ax){
				cum-=ax;
				yc+=sy;
			};
			if(CheckBar(xc,yc,3,3))return false;			
		};
	}else{
		int cum=ay>>1;
		while(yc!=y){
			yc+=sy;
			cum+=ax;
			if(cum>=ay){
				cum-=ay;
				xc+=sx;
			};
			if(CheckBar(xc,yc,3,3))return false;
		};
	};
	return true;
};
void GetForceActOnBar(int x,int y,int L,int& fx,int& fy,int Media){
	MotionField* MF=MFIELDS+Media;
	fx=0;
	fy=0;
	int cx=(x-(L<<3))>>4;
	int cy=(y-(L<<3))>>4;
	for(int dx=0;dx<L;dx++){
		for(int dy=0;dy<L;dy++){
			if(MF->CheckPt(cx+dx,cy+dy)){
				int DX=dx+dx-L+1;
				int DY=dy+dy-L+1;
				//int DX=x-((cx+dx)<<4)-8;
				//int DY=y-((cy+dy)<<4)-8;
				int d=Norma(DX,DY);
				if(d<L){
					if(d<1)d=1;
					fx-=DX*256/d/d;
					fy-=DY*256/d/d;
				};
			};
		};
	};
	int N=Norma(fx,fy);
	if(N>256){
		fx=(fx<<8)/N;
		fy=(fy<<8)/N;
	};
};
void xLine(int x,int y,int x1,int y1,byte c);
void CBar(int x,int y,int Lx,int Ly,byte c);
void DrawForceOnUnit(OneObject* OB){
	return;
	if(LMode)return;
	int dx=mapx<<5;
	int dy=mapy<<4;
	if(OB){
		int LX=OB->Lx;
		int fx,fy;
		XorBlockComplexUnit(OB);
		GetForceActOnBar(OB->RealX>>4,OB->RealY>>4,LX,fx,fy,0);//OB->Media);
		XorBlockComplexUnit(OB);
		int X0=(OB->RealX>>4)-dx;
		int Y0=(OB->RealY>>5)-dy-OB->RZ;
		xLine(X0,Y0,X0+fx,Y0+fy,255);
		CBar(X0-1,Y0-1,3,3,255);
		Xbar((int(OB->x)<<4)-dx,(int(OB->y)<<3)-dy,LX<<4,LX<<3,clrRed);
	};
};
void BXorBar(int x,int y,int Lx);
void XorBlockComplexUnit(OneComplexObject* COB){
	int L=COB->Lx;
	int L3=L<<7;
	int N=COB->NQuants;
	OneObject* OB=Group[COB->ObjIndex];
	for(int i=0;i<N;i++){		
		int x=(int(COB->RealX)-L3)>>8;
		int y=(int(COB->RealY)-L3)>>8;
		for(int i=0;i<NMFIELDS;i++)if(i!=1)MFIELDS[i].BXorBar(x,y,L);
	};
	COB->Lockpoints=!COB->Lockpoints;
};
void UnLockComplexObject(OneObject* OB){
	word CID=OB->CObjIndex;
	if(CID!=0xFFFF){
		OneComplexObject* COB=CGroup[CID];
		if(COB){
			if(COB->Lockpoints)XorBlockComplexUnit(COB);
			COB->Lockpoints=0;
		}
	}
}
void UnLockComplexObjects(){
    for(int i=0;i<MaxCObject;i++){
        OneComplexObject* COB=CGroup[i];
        if(COB){
            if(COB->Lockpoints)XorBlockComplexUnit(COB);
            COB->Lockpoints=0;
        }
    }
}
void LockComplexObjects(){
    for(int i=0;i<MaxCObject;i++){
        OneComplexObject* COB=CGroup[i];
        if(COB){
            if(!COB->Lockpoints)XorBlockComplexUnit(COB);
			COB->Lockpoints=1;
        }
    }
}
void LockComplexObject(OneObject* OB){
	word CID=OB->CObjIndex;
	if(CID!=0xFFFF){
		OneComplexObject* COB=CGroup[CID];
		if(COB){
			if(!COB->Lockpoints)XorBlockComplexUnit(COB);
			COB->Lockpoints=1;
		}
	}
}
void ForceLockComplexObject(OneObject* OB){
	word CID=OB->CObjIndex;
	if(CID!=0xFFFF){
		OneComplexObject* COB=CGroup[CID];
		if(COB){
			XorBlockComplexUnit(COB);
			COB->Lockpoints=1;
		}
	}
}
void XorBlockComplexUnit(OneObject* OB){
	word CID=OB->CObjIndex;
	if(CID!=0xFFFF){
		OneComplexObject* COB=CGroup[CID];
		if(COB){
			XorBlockComplexUnit(COB);
		};
	};
};
void MakeSeveralStepsBackLink(OneObject* OBJ){
	if(!OBJ->LocalOrder->info.MoveToXY.PrevDist--){
		OBJ->BackMotion=0;
		OBJ->DeleteLastOrder();
		return;
	};
	OBJ->BackMotion=1;
	OneComplexObject* COB=CGroup[OBJ->CObjIndex];
	int x,y;
	COB->GetTalePoint(x,y);
	byte DIR=OBJ->LocalOrder->info.MoveToXY.Times;
	OBJ->DestX=(x<<4)+(int(TCos[DIR])<<2);
	OBJ->DestY=(y<<4)+(int(TSin[DIR])<<2);
	addrand(OBJ->DestX);
	addrand(OBJ->DestY);
};
void MakeSeveralStepsBack(OneObject* OB,char Dir,int N){
	if(rando()<16384)Dir+=128;
	if(OB->CObjIndex==0xFFFF)return;
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&MakeSeveralStepsBackLink)return;
	addrand(1005);
	Order1* OR1=OB->CreateOrder(1);
	if(OR1){
		OR1->info.MoveToXY.PrevDist=N;
		OR1->info.MoveToXY.Times=Dir;
		OR1->DoLink=&MakeSeveralStepsBackLink;
	};
};
bool CheckSqIntersection(int x0,int y0,int L0,int x1,int y1,int L1){
	int d=x0+x0+L0-x1-x1-L1;
	if(abs(d)>=L0+L1)return false;
	d=y0+y0+L0-y1-y1-L1;
	if(abs(d)>=L0+L1)return false;
	return true;
};
bool CheckCObjIntersection(OneComplexObject* COB,int x,int y,int L){
	int Ln=COB->Lx;
	int L3n=Ln<<7;
	for(int i=0;i<COB->NQuants;i++){
		int xn=(int(COB->MQ[i]->xc)-L3n)>>8;
		int yn=(int(COB->MQ[i]->yc)-L3n)>>8;
		if(CheckSqIntersection(xn,yn,Ln,x,y,L))return true;
	};
	return false;
};
OneObject* GetLockOwner(word MyID,int x,int y,int L){
	int cx=(x>>3)+1;
	int cy=(y>>3)+1;
	for(int r=0;r<3;r++){
		char* dx=Rarr[r].xi;
		char* dy=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int i=0;i<N;i++){
			int xx=cx+dx[i];
			int yy=cy+dy[i];
			if(xx>=0&&xx<VAL_MAXCX&&yy>=0&&yy<VAL_MAXCX){
				int cell=xx+(yy<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF&&MID!=MyID){
							OneObject* OB=Group[MID];
							if(OB&&OB->Index!=MyID&&OB->CObjIndex!=0xFFFF&&!OB->Sdoxlo){
								OneComplexObject* COB=CGroup[OB->CObjIndex];
								if(COB&&CheckCObjIntersection(COB,x,y,L))return OB;
							};
						};
					};
				};
			};
		};
	};
	return NULL;
};
bool CheckStopRuleWhenLocked(OneObject* OB){
	OneObject* OPP=GetLockOwner(OB->Index,OB->x,OB->y,OB->Lx);
	if(OPP&&OPP->DestX>=0){
		int MyDx=OB->DestX-OB->RealX;
		int MyDy=OB->DestY-OB->RealY;
		int OpDx=OPP->DestX-OPP->RealX;
		int OpDy=OPP->DestY-OPP->RealY;
		return (OpDx*MyDy-OpDy*MyDx)>0;
	};
	return false;
};
void MoveAllFoolsAwayFromTheWay(OneObject* OB);
int PreCheckMotionRules(OneObject* OB){
	return 0;//baba z vozu - kobili legche!
	if(OB->CObjIndex==0xFFFF)return false;

	if(OB->newMons->Radius2>16*32)MoveAllFoolsAwayFromTheWay(OB);

	OneComplexObject* MCOB=CGroup[OB->CObjIndex];
	int XP=MCOB->MQ[0]->xc;
	int YP=MCOB->MQ[0]->yc;
	byte FI=MCOB->MQ[0]->Fi;
	XP+=int(TCos[FI])*12;
	YP+=int(TSin[FI])*12;
	int Ln=MCOB->Lx;
	int L3n=Ln<<7;
	int xn=(XP-L3n)>>8;
	int yn=(YP-L3n)>>8;
	OneObject* OPP=GetLockOwner(OB->Index,xn,yn,OB->Lx);
	if(OPP && OPP->DestX>=0 && OPP!=OB){
		int MyDx=OB->DestX-OB->RealX;
		int MyDy=OB->DestY-OB->RealY;
		int OpDx=OPP->DestX-OPP->RealX;
		int OpDy=OPP->DestY-OPP->RealY;
		int r=Norma(OPP->RealX-OB->RealX,OPP->RealY-OB->RealY)>>4;
		int NM=Norma(MyDx,MyDy);
		int NO=Norma(OpDx,OpDy);
		MyDx=(MyDx<<8)/NM;
		MyDy=(MyDy<<8)/NM;
		OpDx=(OpDx<<8)/NO;
		OpDy=(OpDy<<8)/NO;
		int SP=(MyDx*OpDx+MyDy*OpDy)>>8;
		if(SP<-120)return 2;
		int VP=(OpDx*MyDy-OpDy*MyDx)>>8;
		return VP>0 && abs(SP)<128 && r>20 ? 1:0;
	};
	
	return false;
};



int EnumAllUnitsInCircle(int x,int y,int R,EnumUnitsCallback* fn,DWORD param){
	int cx=(x>>7)+1;
	int cy=(y>>7)+1;
	int D=(R>>7)+2;
	int NN=0;
	for(int r=0;r<D;r++){
		char* dx=Rarr[r].xi;
		char* dy=Rarr[r].yi;
		int N=Rarr[r].N;
		for(int i=0;i<N;i++){
			int xx=cx+dx[i];
			int yy=cy+dy[i];
			if(xx>=0&&xx<VAL_MAXCX&&yy>=0&&yy<VAL_MAXCX){
				int cell=xx+(yy<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&(!OB->Sdoxlo)&&Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y)<R){
								if(fn(OB,x,y,R,param))NN++;
							};
						};
					};
				};
			};
		};
	};
	return NN;
};
void NewMonsterSendToLink(OneObject* OB);
bool MoveFools(OneObject* OB,int x,int y,int R,DWORD param){
	if(OB->CObjIndex!=0xFFFF)return 0;
	byte Mask=param>>8;
	if(!(Mask&OB->NMask))return 0;
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&NewMonsterSendToLink&&OB->PrioryLevel==2)return 0;
	if(OB->BrigadeID==0xFFFF&&OB->StandGround)return 0;
	if(OB->BrigadeID!=0xFFFF){
		Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
		if(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD)return false;
	}    
	byte dir=byte(param);	
	int dx=TCos[dir];
	int dy=TSin[dir];
	int DX=(OB->RealX>>4)-x;
	int DY=(OB->RealY>>4)-y;
	if(DX*dy-DY*dx<0){
		dir+=64;
	}else{
		dir-=64;
	};	
	OB->NewMonsterSendTo(OB->RealX+(TCos[dir]<<2),OB->RealY+(TSin[dir]<<2),128+2,1);
	OB->UnitSpeed=64;
	return 1;
};
void MoveAllFoolsAwayFromTheWay(OneObject* OB){
	if(OB->DestX>=0){
		if(OB->CObjIndex==0xFFFF)return;
		OneComplexObject* MCOB=CGroup[OB->CObjIndex];
		int XP=MCOB->RealX;
		int YP=MCOB->RealY;
		byte FI=MCOB->MQ[0]->Fi;
		DWORD P=DWORD(FI)+(DWORD(OB->NMask)<<8);
		XP+=int(TCos[FI])*2;
		YP+=int(TSin[FI])*2;
		EnumAllUnitsInCircle(XP>>4,YP>>4,85,&MoveFools,P);
		XP+=int(TCos[FI])*4;
		YP+=int(TSin[FI])*4;
		EnumAllUnitsInCircle(XP>>4,YP>>4,65,&MoveFools,P);
	};
};
byte GetOctant(byte Dir){
	return ((Dir+8+512)>>4)&15;
}
OneObject* GetBldInPoint(int x,int y){
	int ofst=(x>>7)+((y>>7)<<VAL_SHFCX)+VAL_MAXCX+1;
	int MID=0xFFFF;
	word GetOneBld(int cell,int pos);
	for(int pos=0;(MID=GetOneBld(ofst,pos))!=0xFFFF;pos++){
		OneObject* OB=Group[MID];
		if(OB&&!(OB->Sdoxlo))return OB;
	}					
	return NULL;
}
void AttackPointByComplexObjectLink(OneObject* OBJ,int x,int y){
	if(OBJ->CObjIndex==0xFFFF)return;
	int x0=x;
	int y0=y;

	int AState=0;
	int srcx=OBJ->RealX;
	int srcy=OBJ->RealY;
	int R=(Norma(x-srcx,y-srcy)>>4)*92/100;
	AdvCharacter* ADC=OBJ->MoreCharacter;
	int DH=OBJ->RZ-GetTotalHeight0(x>>4,y>>4);
	if(DH<0)DH=0;
	else DH=(DH*EngSettings.CannonAddShotDistPer100_Height)/100;

	if(R>ADC->AttackRadius2[OBJ->LocalNewState]+DH){
		OBJ->CreatePath(x>>8,y>>8);
		return;
	}
	OBJ->DeletePath();
	OBJ->DestX=-1;
	if(R<ADC->AttackRadius1[OBJ->LocalNewState]/*||R>ADC->AttackRadius2[OBJ->LocalNewState]*/){
		OBJ->DeleteLastOrder();
		return;
	}

	OneComplexObject* COB=CGroup[OBJ->CObjIndex];
	if(COB==NULL||COB->NoAttack){
		OBJ->DeleteLastOrder();
		return;
	}
	if( COB->StartState==0 && COB->GroundStandState==8 ){		
		COB->GroundStandState=12+OBJ->LocalNewState*4;
	}
	if( COB->StartState!=COB->FinalState || COB->Charged==false || COB->ResSubtracted==false )return;	
	char enmdir=GetDir(x-srcx,y-srcy);
	byte EnmOct=GetOctant(enmdir);	
	byte MyOct=GetOctant(COB->MQ[0]->Fi);
	if(EnmOct!=MyOct){
		byte EnmOct1=GetOctant(enmdir+1);
		byte EnmOct2=GetOctant(enmdir-1);
		if(EnmOct1==MyOct)EnmOct=MyOct;
		if(EnmOct2==MyOct)EnmOct=MyOct;
	}
	COB->DestDir=-1;
	int State=OBJ->LocalNewState*4+12;
	char desDir=(EnmOct*16);
	char mydir=COB->MQ[0]->Fi;
	int ddr=abs(int(char(desDir-mydir)));

	if((COB->FinalState==State||COB->FinalState==8)&&ddr<6){//12-RATTACK 8-ATTACK
		if(EnmOct!=MyOct){
			COB->FinalState=0;
			return;
		}
		if(COB->FinalState==8){
			//pubuxxx is there
			//int R=Norma(x-srcx,y-srcy)>>4;
			int AState=OBJ->LocalNewState;
			//AdvCharacter* ADC=OBJ->MoreCharacter;
			//for(int i=0;i<NAttTypes;i++)if(R>=ADC->DetRadius1[i]&&R<ADC->DetRadius2[i])AState=i;
			Weapon* WP=OBJ->newMons->DamWeap[AState];
			if(WP){
				byte Ang=COB->MQ[0]->Fi;
				OneComplexUnitDesc* COD=CODS.UnitDesc[COB->CDIndex];
				OneMQuantDesc* OCD=COD->Chain[0];
				int xs=int(COB->MQ[0]->xc/16)+((int(TCos[Ang])*OCD->AttackX)>>8)-((int(TSin[Ang])*OCD->AttackY)>>8);
				int ys=int(COB->MQ[0]->yc/16)+OBJ->RZ*2+((int(TSin[Ang])*OCD->AttackX)>>8)+((int(TCos[Ang])*OCD->AttackY)>>8);
				int Z=R<512?OBJ->RZ+OCD->AttackZ:GetHeight(x>>4,y>>4);

				AdvCharacter* ADC=OBJ->MoreCharacter;
				if(ADC->Razbros){
					int R=Norma(OBJ->RealX-x,OBJ->RealY-y)>>9;
					x+=(int(rando()>>5)-512)*R*ADC->Razbros/5000;
					y+=(int(rando()>>5)-512)*R*ADC->Razbros/5000;
				}
				Create3DAnmObject(WP,xs,ys,OBJ->RZ+OCD->AttackZ,x>>4,y>>4,Z,OBJ,OBJ->LocalNewState,0xFFFF);
				COB->Charged=true;
				COB->ResSubtracted=false;
			}
			COB->TestResSubtract();
			COB->GroundStandState=State;
			COB->TryToTransform(State);
			if(!COB->ResSubtracted){
				void CancelRechargement(OneObject* OB);
				COB->TransTime=1;
				CancelRechargement(OBJ);                
			}
			if(OBJ->LocalOrder->info.BuildObj.SN)OBJ->LocalOrder->info.BuildObj.SN--;
			if(!OBJ->LocalOrder->info.BuildObj.SN){
				//checking if enemy building is there
				bool fin=true;
				int ofst=(x0>>11)+((y0>>11)<<VAL_SHFCX)+VAL_MAXCX+1;
				int MID=0xFFFF;
				word GetOneBld(int cell,int pos);
				for(int pos=0;(MID=GetOneBld(ofst,pos))!=0xFFFF;pos++){
                    OneObject* OB=Group[MID];
					if(OB&&!(OB->NMask&OBJ->NMask)){
						fin=false;
					}
				}					
				if(fin)OBJ->DeleteLastOrder();
			}
			return;
		}else{
			//running attack animation
			COB->GroundStandState=8;
			COB->TryToTransform(8);
			return;
		}
	}else{
		if(MyOct==EnmOct&&ddr<6){			
			COB->GroundStandState=State;
			COB->TryToTransform(State);
			return;
		}else{
			if(COB->FinalState!=0&&COB->FinalState!=4){
				COB->TryToTransform(0);
				COB->GroundStandState=0;
				COB->GroundMotionState=4;
				return;
			}else{
				//need to perform rotation there
				COB->DestDir=desDir;
				return;
			}
		}
	}
}
void SetOrderedStateForComplexObjectLink(OneObject* OBJ);
bool CheckCType(int x){
	return x==12||x==16||x==20;
}
void CancelRechargement(OneObject* OB){	
	if(OB->CObjIndex==0xFFFF)return;	
	OneComplexObject* COB=CGroup[OB->CObjIndex];		
	if(COB){
		int S=COB->StartState & 252;
		int F=COB->FinalState & 252;
		if( COB->TransTime>0 && S!=F && ( (S==8/*Attack*/) || (S>=12 && F>=12) ) ){
			COB->StartState=COB->FinalState;
			COB->TransTime=0;
			COB->Charged=false;
		}
	}
}
CEXPORT void SetOrderedStateForComplexObject(OneObject* OB,int State){
	if(OB->CObjIndex==0xFFFF||State==OB->LocalNewState)return;
	if(!OB->newMons->DamWeap[State])return;
	CancelRechargement(OB);
	OneComplexObject* COB=CGroup[OB->CObjIndex];		
	if(COB){
		COB->Charged=false;
		OB->DestX=-1;
		void ClearCannonAutoShot(OneObject* OB);
		ClearCannonAutoShot(OB);
		OB->ClearOrders();
		if(COB->DestDir!=-1)COB->DestDir=-1;
		if(COB->StartState!=COB->FinalState&&CheckCType(COB->FinalState)&&(CheckCType(COB->StartState)||COB->StartState==8)){
			if(COB->FinalState==State*4+12)return;
			COB->StartState=COB->FinalState;
			COB->FinalState=State*4+12;
			COB->GroundStandState=COB->FinalState;
			OB->NewState=State;
			OB->LocalNewState=State;
			if(OB->LocalOrder&&OB->LocalOrder->DoLink==SetOrderedStateForComplexObjectLink){
				OB->LocalOrder->info.AttackXY.wep=State;
			}
			return;
		}
		if(OB->LocalOrder&&OB->LocalOrder->DoLink==SetOrderedStateForComplexObjectLink)return;
		if(OB->Attack){
			if(OB->LocalNewState==State)return;
		}
		addrand(1004);
		Order1* OR1=OB->CreateOrder(0);
		if(OR1){
			OR1->DoLink=&SetOrderedStateForComplexObjectLink;
			OR1->info.AttackXY.wep=State;
		}
	}
}

void SetOrderedStateForComplexObjectLink(OneObject* OBJ){
	if(OBJ->CObjIndex!=0xFFFF){		
		OneComplexObject* COB=CGroup[OBJ->CObjIndex];		
		if(COB){
			int State=int(OBJ->LocalOrder->info.AttackXY.wep)*4+12;
			if(COB->StartState!=COB->FinalState){
				if(COB->FinalState==State){
					OBJ->NewState=OBJ->LocalOrder->info.AttackXY.wep;
					OBJ->LocalNewState=OBJ->NewState;
					OBJ->GroundState=OBJ->NewState;
					COB->GroundStandState=COB->FinalState;
				}
				return;
			}		
			if(COB->FinalState==State){			
				OBJ->NewState=OBJ->LocalOrder->info.AttackXY.wep;
				OBJ->LocalNewState=OBJ->NewState;
				OBJ->DeleteLastOrder();
				return;
			}		
			COB->TryToTransform(State);
			if(COB->FinalState==State)COB->GroundStandState=State;
		}
		return;
	}else OBJ->DeleteLastOrder();
}
bool GetPushkaChargeState(OneObject* OB,int& ChargeType,int& ChargeStage){
	if(OB->CObjIndex==0xFFFF)return false;
	ChargeStage=0;	
	OneComplexObject* COB=CGroup[OB->CObjIndex];
	if(OB->Sdoxlo||!COB)return false;
	ChargeStage=COB->Charged?100:0;		
	int STS=COB->StartState;
	int FST=COB->FinalState;
	ChargeType=OB->LocalNewState;
	if(!COB->Charged)return true;
	if(STS==FST && STS!=8){		
        ChargeStage=100;
		return true;
	}
	if(CheckCType(FST)&&(CheckCType(STS)||STS==8)){
		OneMQuantDesc* OMQD=CODS.UnitDesc[COB->CDIndex]->Chain[0];
		QuantPartTransform* QPT=OMQD->TRANS[COB->StartState+COB->FinalState*NStates];
		int maxt=1;
		if(QPT&&QPT->QP_Type==0){
			for(int i=0;i<QPT->NParts;i++){
				if(maxt<QPT->MaxTransfTime)maxt=QPT->MaxTransfTime;
			}
			ChargeStage=100*COB->TransTime/256/maxt;
			return true;
		}else ChargeStage=0;
	}
	return true;
}
void DrawTrapezia(int xx,int yy,int R1,int R2,byte ANG1,byte ANG2,DWORD C0,DWORD C1,DWORD C2,DWORD C3){
	int x0=xx+((int(TCos[ANG1])*R1)>>8);
	int y0=yy+((int(TSin[ANG1])*R1)>>8);
	int x1=xx+((int(TCos[ANG1])*R2)>>8);
	int y1=yy+((int(TSin[ANG1])*R2)>>8);
	int x2=xx+((int(TCos[ANG2])*R2)>>8);
	int y2=yy+((int(TSin[ANG2])*R2)>>8);
	int x3=xx+((int(TCos[ANG2])*R1)>>8);
	int y3=yy+((int(TSin[ANG2])*R1)>>8);
	int N=3+(R2-R1)/40;
	if(N>0){
		for(int i=0;i<N;i++){

			int x0_=x0+(x1-x0)*i/N;
			int y0_=y0+(y1-y0)*i/N;
			int W=i*255/N;
			DWORD C0_=MixDWORD(C1,C0,W,255-W);

			int x1_=x0+(x1-x0)*(i+1)/N;
			int y1_=y0+(y1-y0)*(i+1)/N;
			W=(i+1)*255/N;
			DWORD C1_=MixDWORD(C1,C0,W,255-W);

			int x2_=x3+(x2-x3)*(i+1)/N;
			int y2_=y3+(y2-y3)*(i+1)/N;
			W=(i+1)*255/N;
			DWORD C2_=MixDWORD(C2,C3,W,255-W);

			int x3_=x3+(x2-x3)*i/N;
			int y3_=y3+(y2-y3)*i/N;
			W=i*255/N;
			DWORD C3_=MixDWORD(C2,C3,W,255-W);			

			int H0=GetTotalHeight0(x0_,y0_);
			int H1=GetTotalHeight0(x1_,y1_);
			int H2=GetTotalHeight0(x2_,y2_);
			int H3=GetTotalHeight0(x3_,y3_);

			Vector3D V1=SkewPt(x0_,y0_,max(H0,0));
			Vector3D V2=SkewPt(x1_,y1_,max(H1,0));
			Vector3D V3=SkewPt(x2_,y2_,max(H2,0));
			Vector3D V4=SkewPt(x3_,y3_,max(H3,0));
			GPS.DrawFillRect(V1,V2,V4,V3,C0_,C1_,C2_,C3_);
		}
	}
}
void ShowAttackRangePreview(OneObject* OB){
	if(OB->CObjIndex==0xFFFF)return;
	AdvCharacter* ADC=OB->MoreCharacter;
	OneComplexObject* COB=CGroup[OB->CObjIndex];		
	if(COB==NULL)return;
	//if(COB->FinalState==8||CheckCType(COB->FinalState)){
        int R1=ADC->AttackRadius1[OB->LocalNewState];
		int R2=ADC->AttackRadius2[OB->LocalNewState];
		byte ANG1=GetOctant(OB->RealDir)*16-8;
		byte ANG2=ANG1+16;
		int xx=OB->RealX>>4;
		int yy=OB->RealY>>4;
		if(R1>64){
			byte ANG=ANG1+8;			
			int H0=GetTotalHeight0(xx,yy);
			int R20=R2;
			for(int i=0;i<8;i++){
                int x1=xx+(TCos[ANG]*R2)/256;
				int y1=yy+(TSin[ANG]*R2)/256;
				int H1=GetTotalHeight0(x1,y1);
				if(H1<H0)R2=R20+((H0-H1)*EngSettings.CannonAddShotDistPer100_Height)/100;
			}
		}
		DrawTrapezia(xx,yy,R1,R2,ANG1,ANG2,0x55FF0000,0x35FF0000,0x35FF0000,0x55FF0000);
		if(R1==0){
			int DA=16;
			int R3=R2*11/6;
			DrawTrapezia(xx,yy,R1,R2,ANG1-DA,ANG1,0x00FF0000,0x00FF0000,0x35FF0000,0x55FF0000);//left
			DrawTrapezia(xx,yy,R1,R2,ANG2,ANG2+DA,0x55FF0000,0x35FF0000,0x00FF0000,0x10FF0000);//right
			DrawTrapezia(xx,yy,R2,R3,ANG1-DA,ANG1,0x00FF0000,0x00FF0000,0x00FF0000,0x35FF0000);//left forw
			DrawTrapezia(xx,yy,R2,R3,ANG2,ANG2+DA,0x35FF0000,0x00FF0000,0x00FF0000,0x00FF0000);//right forw
			DrawTrapezia(xx,yy,R2,R3,ANG1,ANG2,0x35FF0000,0x00FF0000,0x00FF0000,0x35FF0000);//forw
		}
	//}
	GPS.FlushBatches();
}
#define MSXH (240<<ADDSH)
float GetFHeight0(int x,int y){
	if(x<0)x=0;
	if(y<32)y=32;
	if(x>MSXH*32)x=MSXH*32;
	if(y>MSXH*32)y=MSXH*32;
	int nx=x>>5;
	float H0;
	if(nx&1){
		int dd=(x&31);
		int dy=dd>>1;
		int oy=15-dy;
		int y1=(y+oy)>>5;
		int dy1=(y+oy)&31;
		if(dy1>32-dd){
			int Vert2=nx+y1*VertInLine+1;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert3-1;
			float H1=THMap[Vert1];
			float H2=THMap[Vert2];
			float H3=THMap[Vert3];
			float x0=(nx<<5);
			float y0=(y1<<5)+16;
			float HH=H1+(((x-x0)*(((H2+H3)/2.0f)-H1))/32.0f)+(((y-y0)*(H3-H2))/32.0f);
			H0=HH;
		}else{
			int Vert2=nx+y1*VertInLine;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert2+1;
			float H1=THMap[Vert1];
			float H2=THMap[Vert2];
			float H3=THMap[Vert3];
			float x0=(nx<<5)+32;
			float y0=(y1<<5);
			float HH=H1-(((x-x0)*(((H2+H3)/2.0f)-H1))/32.0f)+(((y-y0)*(H3-H2))/32.0f);
			H0=HH;
		};
	}else{
		int dd=(x&31);
		int dy=dd>>1;
		int oy=15-dy;
		int y1=(y+dy)>>5;
		int dy1=(y+dy)&31;
		if(dy1<dd){
			int Vert1=nx+y1*VertInLine;
			int Vert2=Vert1+1;
			int Vert3=Vert2+VertInLine;
			float H1=THMap[Vert1];
			float H2=THMap[Vert2];
			float H3=THMap[Vert3];
			float x0=(nx<<5);
			float y0=(y1<<5);
			float HH=H1+(((x-x0)*(((H2+H3)/2.0f)-H1))/32.0f)+(((y-y0)*(H3-H2))/32.0f);
			H0=HH;
		}else{
			int Vert2=nx+y1*VertInLine;
			int Vert3=Vert2+VertInLine;
			int Vert1=Vert3+1;
			float H1=THMap[Vert1];
			float H2=THMap[Vert2];
			float H3=THMap[Vert3];
			float x0=(nx<<5)+32;
			float y0=(y1<<5)+16;
			float HH=H1-(((x-x0)*(((H2+H3)/2.0f)-H1))/32.0f)+(((y-y0)*(H3-H2))/32.0f);
			H0=HH;
		};
	};
	if(H0<2)H0=2;	
	return H0;
};
void ErrBox(char* sz,...);
bool ReadWinString(GFILE* F,char* STR,int Max);
void ReadHelpersForComplexObjects(){
	if(!NATIONS->NMon)return;
	//cleaning helpers
	for(int i=0;i<CODS.N_OMQD;i++){
		CODS.OMQD[i]->NHelpers=0;
		CODS.OMQD[i]->NExpl=0;		
	}
	//reading helpers
	//QuantName UnitID QuantPos
	GFILE* F=Gopen("ComplexObjects\\helpers.dat","r");
	int cline=1;
	if(F){
		char s[512];
		while(ReadWinString(F,s,511)){
			if(s[0]!='/'){
				if(s[0]=='$'){
					char qid[128];
					char wep[128];
					int dx,dy,st;
                    int z=sscanf(s,"%s%s%d%d%s%d",qid,qid,&dx,&dy,wep,&st);
					if(z==6){
						int qidx=-1;						
						for(int i=0;i<CODS.N_OMQD;i++)if(!strcmp(qid,CODS.OMQD[i]->ID))qidx=i;						
						if(qidx==-1)ErrBox("helpers.dat, line %d : unknown MQuant ID: %s",cline,qid);
						if(CODS.OMQD[qidx]->NExpl<MaxExpInf){
							int ne=CODS.OMQD[qidx]->NExpl;
							ExplodeInfo* EI=CODS.OMQD[qidx]->ExpInf+ne;
							Weapon* GetWeaponByName(char* Name);
							EI->Particles=GetWeaponByName(wep);
							if(!EI->Particles)ErrBox("helpers.dat,line %d : unknown weapon, %s.",cline,wep);
							EI->x=dx;
							EI->y=dy;
							EI->Stage=st;
							CODS.OMQD[qidx]->NExpl++;                                                						
						}else ErrBox("helpers.dat,line %d : too many $EXPLODE directives for one quant.",cline);
					}else ErrBox("helpers.dat,line %d : not enough parameters for $EXPLODE.",cline);
				}else{
					char qid[128];
					char uid[128];
					char qopt[128]="";
					int  qpos=0;
					int z=sscanf(s,"%s%s%d%s",qid,uid,&qpos,qopt);
					if(z>=3){
						int qidx=-1;
						int uidx=-1;
						for(int i=0;i<CODS.N_OMQD;i++)if(!strcmp(qid,CODS.OMQD[i]->ID))qidx=i;
						for(int i=0;i<NATIONS->NMon;i++)if(NATIONS->Mon[i]->MonsterID && !strcmp(uid,NATIONS->Mon[i]->MonsterID))uidx=i;
						if(uidx==-1)ErrBox("helpers.dat, line %d : unknown monster ID: %s",cline,uid);
						else{
							if(qidx==-1)ErrBox("helpers.dat, line %d : unknown MQuant ID: %s",cline,qid);
							else{
								if(CODS.OMQD[qidx]->NHelpers>=MaxHelperUnits)ErrBox("helpers.dat, line %d : too many helpers for quant %s",cline,qid);
								else{
									CODS.OMQD[qidx]->Helpers[CODS.OMQD[qidx]->NHelpers]=uidx;
									CODS.OMQD[qidx]->HelpersPos[CODS.OMQD[qidx]->NHelpers]=qpos;
									CODS.OMQD[qidx]->HelpersOpt[CODS.OMQD[qidx]->NHelpers]|=2;//can't move if unit killed
									if(strstr(qopt,"attack")){
										CODS.OMQD[qidx]->HelpersOpt[CODS.OMQD[qidx]->NHelpers]|=1;
										CODS.OMQD[qidx]->HelpersOpt[CODS.OMQD[qidx]->NHelpers]&=0xFFFF-2;
									}
									if(strstr(qopt,"nomove")){
										CODS.OMQD[qidx]->HelpersOpt[CODS.OMQD[qidx]->NHelpers]|=2;										
									}
									CODS.OMQD[qidx]->NHelpers++;

								}
							}
						}
					}else ErrBox("helpers.dat,line %d : not enough parameters.",cline);
				}
			}
			cline++;
		}
		Gclose(F);
	}
}
bool CheckObjectForCapture(OneObject* OB){
	if(!OB->newMons->Capture)return false;
	if(OB->CObjIndex!=0xFFFF){
        OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB){
			for(int i=0;i<COB->NHelpers;i++)if(COB->HelpersIDS[i]!=0xFFFF)return false;
		}
	}
	return true;
}
bool CheckObjectForFreeStatus(OneObject* OB){	
	if(OB->CObjIndex!=0xFFFF){
		OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB){
			for(int i=0;i<COB->NHelpers;i++)if(COB->HelpersIDS[i]!=0xFFFF)return false;
		}
	}
	return true;
}
int GetDestDir(OneObject* OB){
	if(OB->CObjIndex!=0xFFFF){		
		OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB){
            return COB->DestDir;
		}
	}
	return -1;
}
bool SetDestDirForComplexObject(OneObject* OB,byte DestDir){
	if(OB->CObjIndex!=0xFFFF){		
		OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB&&COB->DestDir==OB->RealDir){
			return true;
		}
        if(COB->NQuants>1)return true;
		if(COB->StartState==COB->FinalState){
			if(COB->StartState==COB->GroundStandState || COB->StartState==COB->GroundMotionState){
				if(COB)COB->DestDir=DestDir;
				return true;
			}else{
				COB->TryToTransform(COB->GroundStandState);
			}
		}
	}
	return false;
}
Vector3D GetSelectionBarPos(OneObject* OB){
	Vector3D V;
	if(OB->CObjIndex!=0xFFFF){
        OneComplexObject* COB=CGroup[OB->CObjIndex];
		if(COB){
			SingleAxeQuant* SA=COB->MQ[0];
			float x=COB->MQ[0]->xc/16;
			float y=COB->MQ[0]->yc/16;
			float z=0;
			for(int i=0;i<10;i++){
				int H=GetTotalHeight0(x,y);
				y+=(H-z)*2;
				z=H;
			}
			V.set(x,y,z);			
		}
	}else{
        V.set(OB->RealX/16,OB->RealY/16,OB->RZ); //+OB->OverEarth
	}
	return V;
}
int OneObject::GetAttX(){
	if(CObjIndex!=0xFFFF){
		OneComplexObject* COB=CGroup[CObjIndex];
		if(COB){
			SingleAxeQuant* SA=COB->MQ[0];
			return int(COB->MQ[0]->xc);			
		}
	}
	return RealX;
}
int OneObject::GetAttY(){
	if(CObjIndex!=0xFFFF){
		OneComplexObject* COB=CGroup[CObjIndex];
		if(COB){
			SingleAxeQuant* SA=COB->MQ[0];
			float x=COB->MQ[0]->xc/16;
			float y=COB->MQ[0]->yc/16;
			float z=0;
			for(int i=0;i<4;i++){
				int H=GetTotalHeight0(x,y);
				y+=(H-z)*2;
				z=H;
			}
			return y*16;			
		}		
	}
	return RealY;
}
void SaveComplexObjects(SaveBuf* SB){
	int N=0;
	for(int i=0;i<MaxCObject;i++)if(CGroup[i])N++;
	xBlockWrite(SB,&N,4);
	for(int i=0;i<MaxCObject;i++)if(CGroup[i]){
		xBlockWrite(SB,&i,4);
        OneComplexObject* COB=CGroup[i];
		int n=0;
		int j;
		for(j=0;j<8&&COB->MQ[j];j++);
		xBlockWrite(SB,&j,4);
		for(int j=0;j<8;j++){
			SingleAxeQuant* SA=COB->MQ[j];
			if(SA){
                xBlockWrite(SB,SA,sizeof SingleAxeQuant);
			}else break;
		}
		xBlockWrite(SB,&COB->ForwDist,sizeof(OneComplexObject)-4*8);
	}
}
void LoadComplexObjects(SaveBuf* SB){
	int N=0;
	xBlockRead(SB,&N,4);	
	for(int i=0;i<N;i++){
		OneComplexObject* COB=new OneComplexObject;
		int p=0;
		xBlockRead(SB,&p,4);
		CGroup[p]=COB;
		int n=0;		
		xBlockRead(SB,&n,4);
		for(int j=0;j<n;j++){
			SingleAxeQuant* SA=new SingleAxeQuant;
			COB->MQ[j]=SA;			
			xBlockRead(SB,SA,sizeof SingleAxeQuant);
		}
		xBlockRead(SB,&COB->ForwDist,sizeof(OneComplexObject)-4*8);
		if(p>=MaxCObject)MaxCObject=p+1;
	}
}
void SetIllusion(OneObject* OBJ,bool State){
	OBJ->Illusion=State;
	if(OBJ->CObjIndex!=0xFFFF){
		OneComplexObject* COB=CGroup[OBJ->CObjIndex];
		if(COB){
			for(int i=0;i<COB->NHelpers;i++){
				word id=COB->HelpersIDS[i];
				if(id!=0xFFFF){
					OneObject* OB=Group[id];
					if(OB&&OB->Serial==COB->HelpersSNS[i]){
                        OB->Illusion=State;
					}
				}
			}
		}
	}
}
void SetInvisiblen(OneObject* OBJ,bool State){
	OBJ->Invisible=State;
	if(OBJ->CObjIndex!=0xFFFF){
		OneComplexObject* COB=CGroup[OBJ->CObjIndex];
		if(COB){
			for(int i=0;i<COB->NHelpers;i++){
				word id=COB->HelpersIDS[i];
				if(id!=0xFFFF){
					OneObject* OB=Group[id];
					if(OB&&OB->Serial==COB->HelpersSNS[i]){
						OB->Invisible=State;
					}
				}
			}
		}
	}
}
void StopCannonRotation(OneObject* OB){
	if(OB){
		if(OB->CObjIndex!=0xFFFF){
			OneComplexObject* COB=CGroup[OB->CObjIndex];
			if(COB){
				COB->DestDir=-1;
				//CancelRechargement(OB);
			}
		}
	}
}
class ComplexObjectEnumerator:public ProcEnumerator{
public:
	virtual DWORD GetValue(const char* ID){
		for(int i=0;i<CODS.NUnits;i++){
			if(!strcmp(ID,CODS.UnitDesc[i]->ID))return i;
		}
		return 0xFFFF;
	}	
	virtual char* GetValue(DWORD ID){
		if(ID==0xFFFF)return "---none---";
		else return CODS.UnitDesc[ID]->ID;		
	}	
	virtual int   GetAmount(){
		return CODS.NUnits+1;
	}
	virtual char* GetIndexedString(int idx){
		return GetValue(GetIndexedValue(idx));
	}
	virtual DWORD GetIndexedValue (int idx){
		return idx>0?idx-1:0xFFFF;
	}
	virtual char* GetCategory(int idx){
		return NULL;
	}	
};
void CreateComplexObjectsEnumerator(){
	ComplexObjectEnumerator* CE=new ComplexObjectEnumerator;
	Enumerator* E=ENUM.Get("COMPLEXOBJECTS");
	E->SetProcEnum(CE);
}