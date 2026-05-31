#include "stdheader.h"
#include "SpraySystem.h"

extern IRenderSystem*	IRS;

//---------------------new ground updating system-----------------------//
class GroundUpdate{
public:
	//GroundUpdate();
	~GroundUpdate();
	void Create();
	BaseMesh UpMesh;
	int SYS_textureID;
	int DEF_textureID;
	void Update(byte* ptr,int pitch,int x,int y,int Lx,int Ly);
};
void TnLset(VertexTnL* V,float x,float y,float z,float u,float v,DWORD D,DWORD S);
void GroundUpdate::Create(){
	UpMesh.create(4,6,vfVertexTnL);
	UpMesh.setShader(IRS->GetShaderID("NoDepth"));
	VertexTnL* VTX=(VertexTnL*)UpMesh.getVertexData();
	word* IDX=UpMesh.getIndices();
	TnLset(VTX+0,0,0,1.0,0,0,0xFFFFFFFF,0);
	TnLset(VTX+1,256.0,0,1.0,0,1.0,0xFFFFFFFF,0);
	TnLset(VTX+2,0,256.0,1.0,1.0,0,0xFFFFFFFF,0);
	TnLset(VTX+3,256.0,256.0,1.0,1.0,1.0,0xFFFFFFFF,0);
	IDX[0]=0;
	IDX[1]=1;
	IDX[2]=2;
	IDX[3]=1;
	IDX[4]=3;
	IDX[5]=2;
	DEF_textureID=IRS->CreateTexture("DEF_texture",256,256,cfRGB565,1, tmpManaged);
	UpMesh.setNInd(6);
	UpMesh.setNPri(2);
	UpMesh.setNVert(4);
	UpMesh.setTexture(DEF_textureID);
};
void Copy16(byte* Src,int SrcPitch,byte* Dst,int DstPitch,int Lx,int Ly);
DWORD GU_COLOR=0xFFFFFFFF;
void GroundUpdate::Update(byte* ptr,int pitch,int x,int y,int Lx,int Ly)
{
	int sPitch=0;
	byte* sBits=IRS->LockTexBits(SYS_textureID,sPitch);
	if(sPitch){
		Copy16(ptr,pitch,sBits,sPitch,Lx,Ly);
		IRS->UnlockTexBits(SYS_textureID);
		//IRS->CopyTexture(DEF_textureID,SYS_textureID);
		VertexTnL* VTX=(VertexTnL*)UpMesh.getVertexData();
		float fx=float(x);
		float fy=float(y);
		float fLx=float(Lx);
		float fLy=float(Ly);
		VTX[0].x=fx;
		VTX[0].y=fy;
		VTX[0].diffuse=GU_COLOR;

		VTX[1].x=fx+Lx;
		VTX[1].y=fy;
		VTX[1].u=Lx/256.0;
		VTX[1].v=0;
		VTX[1].diffuse=GU_COLOR;

		VTX[2].x=fx;
		VTX[2].y=fy+Ly;
		VTX[2].u=0;
		VTX[2].v=Ly/256.0;
		VTX[2].diffuse=GU_COLOR;

		VTX[3].x=fx+Lx;
		VTX[3].y=fy+Ly;
		VTX[3].u=Lx/256.0;
		VTX[3].v=Ly/256.0;
		VTX[3].diffuse=GU_COLOR;

		extern bool Copy16dest;
		/*if(Copy16dest)IRS->PushRenderTarget(IRS->GetVBufferID());
		DrawBM(UpMesh);
		if(Copy16dest)IRS->PopRenderTarget();*/
	};
};

GroundUpdate* GUPD=NULL;

void Update8to16(byte* src,int sPitch,int x,int y,int Lx,int Ly){
	if(!GUPD){
		GUPD=new GroundUpdate;
		GUPD->Create();
	};
	int Nx=Lx>>8;
	int Ny=Ly>>8;
	for(int ix=0;ix<Nx;ix++){
		for(int iy=0;iy<Ny;iy++){
			GUPD->Update(src+(ix<<8)+(iy<<8)*sPitch,sPitch,x+(ix<<8),y+(iy<<8),256,256);
		};
	};
	if(Nx<<8!=Lx){
		for(int iy=0;iy<Ny;iy++)
			GUPD->Update(src+(Nx<<8)+(iy<<8)*sPitch,sPitch,x+(Nx<<8),y+(iy<<8),Lx-(Nx<<8),256);

	};
	if(Ny<<8!=Ly){
		for(int ix=0;ix<Nx;ix++)
			GUPD->Update(src+(ix<<8)+(Ny<<8)*sPitch,sPitch,x+(ix<<8),y+(Ny<<8),256,Ly-(Ny<<8));
	};
	if(Nx<<8!=Lx&&Ny<<8!=Ly){
		GUPD->Update(src+(Nx<<8)+(Ny<<8)*sPitch,sPitch,x+(Nx<<8),y+(Ny<<8),Lx-(Nx<<8),Ly-(Ny<<8));
	};

};

struct PiroObject{
	DWORD Color;
	int x,y,z;
	int dz;
	float Angle;
	int vx,vy,vz;
	int vx0,vy0,vz0;
	float Scale;
	float ScaleSpeed;
	float RotSpeed;
	float StartRot;
	float StartScale;
	int LifeTime;
	int MaxLifeTime;
	int PiroType;
};
#define MAXMESH 256
class PiroSystem{
public:
	PiroObject* PObj;
	int NPObj;
	int MaxPObj;
	void AddPiroObject(int x,int y,int z,float vx,float vy,float vz,
		float Scale,float ScaleSpeed,float RotSpeed,int MaxLifeTime,DWORD Color);
	void Process();
	void EraseObject(int Index);
	void Draw();
	void ClearAll();
	int GetDistToNearest(int x,int y){
		int MD=100000;
		for(int i=0;i<NPObj;i++){
			int r=Norma(x-(PObj[i].x>>12),y-(PObj[i].y>>12));
			if(r<MD)MD=r;
		};
		return MD;
	};
	PiroSystem();
	~PiroSystem();
	//----------------graph subsystem-------------
	BaseMesh* PiroMesh;
};
PiroSystem::PiroSystem(){
	memset(this,0,sizeof *this);
};
PiroSystem::~PiroSystem(){
	if(PiroMesh)delete(PiroMesh);
	if(PObj)free(PObj);
};
void PiroSystem::AddPiroObject(int x,int y,int z,float vx,float vy,float vz,
			   float Scale,float ScaleSpeed,float RotSpeed,int MaxLifeTime,DWORD C){
	if(NPObj>=MaxPObj){
		MaxPObj+=256;
		PObj=(PiroObject*)realloc(PObj,MaxPObj*sizeof PiroObject);
	};
	PiroObject* PO=PObj+NPObj;
	PO->x=x<<12;
	PO->y=y<<12;
	PO->z=z<<12;
	PO->vx=int(vx*(2<<12));
	PO->vy=int(vy*(2<<12));
	PO->vz=int(vz*(2<<12));

	PO->vx0=PO->vx;
	PO->vy0=PO->vy;
	PO->vz0=PO->vz;

	PO->Angle=float(rand())/256.0;
	PO->RotSpeed=RotSpeed;
	PO->Scale=Scale;
	PO->ScaleSpeed=ScaleSpeed;
	PO->MaxLifeTime=MaxLifeTime;
	PO->StartRot=RotSpeed;
	PO->StartScale=Scale;
	PO->LifeTime=0;
	PO->dz=0;
	PO->PiroType=rand()&1;
	PO->Color=C;
	NPObj++;
};
void PiroSystem::ClearAll(){
	NPObj=0;
};
void PiroSystem::EraseObject(int Index){
	if(Index<NPObj){
		if(Index<NPObj-1)memcpy(PObj+Index,PObj+Index+1,(NPObj-Index-1)*sizeof PiroObject);
		NPObj--;
	};
};
#define C2 float(1<<12)
float frnd2();
void PiroSystem::Process(){
	PiroObject* PO=PObj;
	for(int i=0;i<NPObj;i++){
		PO->x+=PO->vx;
		PO->y+=PO->vy;
		PO->z+=PO->vz;
		PO->dz+=PO->vz;
		PO->Angle+=PO->RotSpeed;
		PO->StartRot+=frnd2()*0.0005;
		PO->Scale+=PO->ScaleSpeed;
		float az=PO->StartScale/PO->Scale;
		PO->RotSpeed=PO->StartRot*az;
		PO->vx=PO->vx0*az;
		PO->vy=PO->vy0*az;
		///PO->vz-=PO->vz0/18;
		PO->vx0*=0.99;
		PO->vy0*=0.99;
		PO->vz0*=0.99;
		PO->LifeTime++;
		if(PO->LifeTime>PO->MaxLifeTime){
			if(i<NPObj-1){
				memcpy(PO,PO+1,(NPObj-i-1)*sizeof PiroObject);
			};
			NPObj--;
			i--;
		}else{
			PO++;
		};
	};
};

char* PiroNames[]={"piro_my1.tga","piro_my1.tga","piro_my1.tga"};
void PiroSystem::Draw(){
	if(!PiroMesh){
		int i=0;
		//for(int i=0;i<3;i++){
			PiroMesh=new BaseMesh;
			PiroMesh->create(4*MAXMESH,6*MAXMESH,vfVertexTnL);
			PiroMesh->setShader(IRS->GetShaderID("piro"));
			VertexTnL* VTX=(VertexTnL*)PiroMesh->getVertexData();
			word* IDX=PiroMesh->getIndices();
			for(int j=0;j<MAXMESH;j++){
				TnLset(VTX+j*4+0,0,0,0.0,0,0,0xFFFFFFFF,0);
				TnLset(VTX+j*4+1,256.0,0,0.0,0,1.0,0xFFFFFFFF,0);
				TnLset(VTX+j*4+2,0,256.0,0.0,1.0,0,0xFFFFFFFF,0);
				TnLset(VTX+j*4+3,256.0,256.0,0.0,1.0,1.0,0xFFFFFFFF,0);
				int j6=j*6;
				int j4=j*4;
				IDX[j6+0]=j4+0;
				IDX[j6+1]=j4+1;
				IDX[j6+2]=j4+2;
				IDX[j6+3]=j4+1;
				IDX[j6+4]=j4+3;
				IDX[j6+5]=j4+2;
			};
			PiroMesh->setTexture(IRS->GetTextureID(PiroNames[0]));
		//};
	};
	int MinX=(mapx-4)<<5;
	int MinY=(mapy-4)<<4;
	int MaxX=(mapx+smaplx+4)<<5;
	int MaxY=(mapy+smaply+4)<<5;
	float dx=mapx*32;
	float dy=mapy*16;
	PiroObject* PO=PObj;
	VertexTnL* VTX0=(VertexTnL*)PiroMesh->getVertexData();
	VertexTnL* VTX=VTX0;
	int NMS=0;
	float scale=1.0/float(1<<(5-Shifter));
	for(int i=0;i<NPObj;i++){
		int x=PO->x>>12;
		int y=(PO->y>>13)-(PO->z>>12);
		//if(x>MinX&&x<MaxX&&y>MinY&&y<MaxY){
			float xc=float(PO->x)/C2-dx;
			float yc=float((PO->y>>1)-PO->z)/C2-dy;
			float pdx=64.0*PO->Scale*cos(PO->Angle);
			float pdy=64.0*PO->Scale*sin(PO->Angle);
			//float al=PO->StartScale/PO->Scale;
			float D=float(PO->MaxLifeTime-PO->LifeTime)/PO->MaxLifeTime;
			float D2=float(PO->LifeTime)/PO->MaxLifeTime;
			float al=D*D;
			float a2=D2*D2*50;
			if(a2<al)al=a2;
			if(al>1)al=1;
			DWORD c=int(255.0*al);
			DWORD cc=(c<<24)+(PO->Color&0xFFFFFF);
			float sc1=(1.2+sin(PO->Angle+PO->Scale/2.0))/2.0;
			float sc2=(1.2+sin(PO->Angle+PO->Scale/1.2))/2.0;
			xc*=scale;
			yc*=scale;
			pdx*=scale;
			pdy*=scale;
			VTX[0].x=xc+pdx;
			VTX[0].y=yc+pdy;
			VTX[0].diffuse=cc;
			VTX[3].x=xc-pdx;
			VTX[3].y=yc-pdy;
			VTX[3].diffuse=cc;
			VTX[1].x=xc+pdy;
			VTX[1].y=yc-pdx;
			VTX[1].diffuse=cc;
			VTX[2].x=xc-pdy;
			VTX[2].y=yc+pdx;
			VTX[2].diffuse=cc;
			VTX+=4;
			NMS++;
			if(NMS>=MAXMESH-1){
				PiroMesh->setNInd(NMS*6);
				PiroMesh->setNPri(NMS*2);
				PiroMesh->setNVert(NMS*4);
				DrawBM(*PiroMesh);
				VTX=VTX0;
				NMS=0;
			};
		//};
		PO++;
	};
	if(NMS){
		PiroMesh->setNInd(NMS*6);
		PiroMesh->setNPri(NMS*2);
		PiroMesh->setNVert(NMS*4);
		DrawBM(*PiroMesh);
	};
};
bool ps_init=0;
PiroSystem PS;
float frnd(){
	return float(rand())/32767.0;
};
float frnd2(){
	return float(rand())/16384.0-1;
};
void TestFog3();
void TestPiro(){
	
};

void ProcessDust();
extern bool NOPAUSE;
void PiroProcess(){
	if(!NOPAUSE)return;
	PS.Process();
	ProcessDust();
}
void InitTexColors();
void PiroDraw(){	
	PS.Draw();
	DrawDust();
}
extern DWORD TXCOLOR[64];
extern int RealLx;
extern int RealLy;
void AddSmokeOnHorseStep(OneObject* OB){	
	//if(rand()>8192)return;
	//AddOneDust(OB->RealX>>4,(OB->RealY>>4)-(OB->RZ<<1));
	//return;
	if(OB&&!OB->Sdoxlo){
		int x=OB->RealX>>4;
		int y=OB->RealY>>4;
		int dx=x-(mapx<<5);
		int dy=(y-(mapy<<5))>>1;
		int RLX=RealLx<<(5-Shifter);
		int RLY=RealLy<<(5-Shifter);
		if(dx>-32&&dy>-32&&dx<RLX+32&&dy<RLY+32){
			int MR=20;//20+PS.NPObj/2;
			int R=PS.GetDistToNearest(x,y);
			//if((rand()%(16+PS.NPObj/3))<R&&PS.NPObj<1000){
			int tofs=(x>>5)+(y>>5)*VertInLine;
			if(tofs>0&&tofs<MaxPointIndex){
				int T=TexMap[tofs];
				DWORD TC=TXCOLOR[T];

				int TB=TC&255;
				int TG=(TC>>8)&255;
				int TR=(TC>>16)&255;
				int TA=(TR+TG+TG+TB)/4;
				if(TG*8<TA*9&&TA>80&&TB<TA&&TA<180){
					if(rand()>(PS.NPObj+10)*240)
						PS.AddPiroObject(x,y,OB->RZ,0,0,0.05,0.5,0.01+float(rand()&255)/42000,-0.01+float(rand()&255)*0.02/255,40+(rand()&63),TXCOLOR[T]+0x00202020);
				};
			};
			//};
		};
	}
}

