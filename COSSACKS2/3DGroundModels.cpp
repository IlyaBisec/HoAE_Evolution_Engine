#include "stdheader.h"

//#define MAXMODEL 512
//AnimModel* MODELS[MAXMODEL];
//int NModels=0;
//int LoadModel(char* name){
//	assert(NModels<MAXMODEL);
//	if(!MODELS[NModels])MODELS[NModels]=new AnimModel;
//	if(MODELS[NModels]->Load(name)){
//		return NModels++;
//	};
//	return -1;
//};
//void DrawModel(int Index,int SubIndex,float x,float y,float z){
//	Matrix4D M;
//	M.translation(x,y,z);
//	MODELS[Index]->Draw();
//};
//extern IRenderSystem*	IRS;
//void DrawModel(int Index,int SubIndex,float x,float y,float z,float Angle,float scale){
//	Matrix4D M;
//	M.srt(scale,Vector3D(0,0,1),Angle,Vector3D(x,y,z));
//	//M.e20-=0.2;
//	M.e21-=0.13;
//	M.e22*=0.84;
//	//M.e12+=0.2;
//	IRS->SetWorldMatrix(M);
//	MODELS[Index]->Draw();
//};

struct OneGModel3D{
	float x,y,z;
	float Angle;
	float AxeX,AxeY,AxeZ;
	int ModelIndex;
	int SubIndex;
};
#define MAXGMODELS 1024
OneGModel3D* GModels[MAXGMODELS];
int NGModels=0;
int AddGModel(float x,float y,float z,float Angle,int MIndex,int SubIndex){
	int i;
	for(i=0;i<NGModels;i++)if(!GModels[i])break;
	if(i==NGModels){
		if(NGModels>=MAXGMODELS)return -1;
		i=NGModels;
		GModels[i]=new OneGModel3D;
		memset(GModels[i],0,sizeof OneGModel3D);
		NGModels++;	
	};
	OneGModel3D* OGM=GModels[i];
	OGM->x=x;
	OGM->y=y;
	OGM->z=z;
	OGM->Angle=Angle;
	OGM->AxeX=0;
	OGM->AxeY=0;
	OGM->AxeZ=1;
	OGM->ModelIndex=MIndex;
	OGM->SubIndex=SubIndex;
};
void DrawAllGModels(){
	/*for(int i=0;i<NGModels;i++)if(GModels[i]){
		OneGModel3D* OGM=GModels[i];
		DrawModel(OGM->ModelIndex,OGM->SubIndex,OGM->x,OGM->y-32,OGM->z-16,OGM->Angle,0.5);
	};
	IRS->SetWorldMatrix(Matrix4D::identity);*/
};
void LoadAllGModels(){
	/*GFILE* F=Gopen("models.dat","r");
	if(F){
		char cc[64];
		int z;
		do{
			z=Gscanf(F,"%s",cc);
			if(z==1){
				LoadModel(cc);
			};
		}while(z==1);
	};*/
};
//---------------------PLATFORMS SYSTEM--------------------//
struct OnePlatformDesc{
	byte TypeLU;
	byte TypeRU;
	byte TypeLD;
	byte TypeRD;
	byte Level;
	byte Changed;
};
int PlatfLX;
OnePlatformDesc* PlatfMap=NULL;
void ClearPlatformMap();
void AllocPlatformMap(){
	PlatfLX=64<<ADDSH;
	PlatfMap=new OnePlatformDesc[PlatfLX*PlatfLX];
	memset(PlatfMap,0,PlatfLX*PlatfLX*sizeof OnePlatformDesc);
	ClearPlatformMap();
};
void FreePlatformMap(){
	if(PlatfMap)delete[](PlatfMap);
	PlatfMap=NULL;
};
void ClearPlatformMap(){
	if(PlatfMap)memset(PlatfMap,0,PlatfLX*PlatfLX*sizeof OnePlatformDesc);
	int N=PlatfLX*PlatfLX;
	for(int i=0;i<N;i++){
		PlatfMap[i].Level=1;
	};
};
#define LH0 90
extern bool HiChanged;

extern int MinVx;
extern int MaxVx;
extern int MinVy;
extern int MaxVy;
bool LevChanged=0;
int GetLevel(int x,int y){
	int sx=(PlatfLX>>1)+((x-y+128)>>8);
	int sy=(x+y)>>8;
	if(sx>=0&&sy>=0&&sx<PlatfLX&&sy<PlatfLX){
		int of=sx+sy*PlatfLX;
		LevChanged=PlatfMap[of].Changed;
		return PlatfMap[of].Level;
	};
};
void CreatePlatformsSystem(){
	NGModels=0;
	int ofs=0;
	OnePlatformDesc* PD=PlatfMap;
	for(int iy=0;iy<PlatfLX;iy++){
		for(int ix=0;ix<PlatfLX;ix++){
			if(ix>0&&iy>0&&ix<PlatfLX-1&&iy<PlatfLX-1&&PD->Level){
				float xc=-PlatfLX*64+(+ix+iy)*128.0+64.0-10.0;
				float yc=PlatfLX*64+(-ix+iy)*128.0+64.0-10;
				int C=PD->Level;
				int LD=PD[-1].Level;
				int RU=PD[1].Level;
				int LU=PD[-PlatfLX].Level;
				int RD=PD[PlatfLX].Level;
				if(C&&!(LU||RU)){
					AddGModel(xc,yc-128,C*LH0,-c_PI/2+c_PI/4,1,0);
				};
				if(C&&!(LD||RD)){
					AddGModel(xc,yc+128,C*LH0,c_PI/2+c_PI/4,1,0);
				};
				if(C&&!(LU||LD)){
					AddGModel(xc-128,yc,C*LH0,-c_PI+c_PI/4,1,0);
				};
				if(C&&!(RU||RD)){
					AddGModel(xc+128,yc,C*LH0,c_PI/4,1,0);
				};
				if(PD->Level>PD[-1].Level){
					//AddGModel(xc-64,yc+64,PD->Level*64,c_PI/2+c_PI*3/4,0,0);
					AddGModel(xc-64-32,yc+64-32,PD->Level*LH0,c_PI/2+c_PI*3/4,0,0);
					AddGModel(xc-64+32,yc+64+32,PD->Level*LH0,c_PI/2+c_PI*3/4,0,0);
				};
				if(PD->Level>PD[1].Level){
					//AddGModel(xc+64,yc-64,PD->Level*64,c_PI/2-c_PI/4,0,0);
					AddGModel(xc+64-32,yc-64-32,PD->Level*LH0,c_PI/2-c_PI/4,0,0);
					AddGModel(xc+64+32,yc-64+32,PD->Level*LH0,c_PI/2-c_PI/4,0,0);
				};
				if(PD->Level>PD[-PlatfLX].Level){
					//AddGModel(xc-64,yc-64,PD->Level*64,c_PI/2-c_PI*3/4,0,0);
					AddGModel(xc-64-32,yc-64+32,PD->Level*LH0,c_PI/2-c_PI*3/4,0,0);
					AddGModel(xc-64+32,yc-64-32,PD->Level*LH0,c_PI/2-c_PI*3/4,0,0);
				};
				if(PD->Level>PD[PlatfLX].Level){
					//AddGModel(xc+64,yc+64,PD->Level*64,c_PI/2+c_PI/4,0,0);
					AddGModel(xc+64-32,yc+64+32,PD->Level*LH0,c_PI/2+c_PI/4,0,0);
					AddGModel(xc+64+32,yc+64-32,PD->Level*LH0,c_PI/2+c_PI/4,0,0);
				};
			};
			PD++;			
		};
	};
	for(int iy=0;iy<MaxTH;iy++){
		for(int ix=0;ix<VertInLine;ix++){
			int x=ix<<5;
			int y=iy<<5;
			int L=GetLevel(x,y);
			if(LevChanged){
				int H=L*LH0-10;
				if(H>0){
					if(THMap[ofs]<H){
						THMap[ofs]=H;
					};
				}else{
					THMap[ofs]=-90;
					void ReportHChange(int v);
					ReportHChange(ofs);
				};
			};
			ofs++;
		};
	};
	int N=PlatfLX*PlatfLX;
	for(int i=0;i<N;i++)PlatfMap[i].Changed=0;
};
bool M3_Done=0;
void MakeAllDirtyGBUF();
void TestGModels(){
	if(!M3_Done){
		AllocPlatformMap();
		//for(int i=0;i<20;i++)AddGModel(900+300*cos(c_PI*i/10.0),900+300*sin(c_PI*i/10.0),20,(c_PI*i/10.0)+c_PI/2.0,0,0);
		PlatfMap[PlatfLX/2+8*PlatfLX].Level=1;
		PlatfMap[PlatfLX/2+9*PlatfLX].Level=1;
		PlatfMap[PlatfLX/2+1+9*PlatfLX].Level=1;
		PlatfMap[PlatfLX/2+10*PlatfLX].Level=1;
		PlatfMap[PlatfLX/2+1+10*PlatfLX].Level=1;
		CreatePlatformsSystem();		
		MakeAllDirtyGBUF();
		void RecreateWaterDeep();
		RecreateWaterDeep();
	};
	M3_Done=1;
};

extern bool Lpressed;
extern bool Rpressed;
extern int LastMx;
extern int LastMy;
void ResetGroundCache();
void ProcessGModelsEdit(){
	return;
	int x=LastMx;
	int y=LastMy;
	if(GetKeyState('Z')&0x8000){
		if(Lpressed||Rpressed){
			int sx=(PlatfLX>>1)+((x-y+128)>>8);
			int sy=(x+y)>>8;
			if(sx>=0&&sy>=0&&sx<PlatfLX&&sy<PlatfLX){
				PlatfMap[sx+sy*PlatfLX].Level=Lpressed;
				PlatfMap[sx+sy*PlatfLX].Changed=1;
				CreatePlatformsSystem();
				ResetGroundCache();
				MakeAllDirtyGBUF();
			};
		};
	};
}