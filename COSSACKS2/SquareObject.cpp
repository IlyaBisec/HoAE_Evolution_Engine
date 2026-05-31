#include "stdheader.h"
#define GETVIDX(ofs,VIDX,px,py)\
	VIDX=VertRef[ofs];\
	if(VIDX==-1){\
		TV=VER+NVert;\
		VertRef[ofs]=NVert;\
		VIDX=NVert;\
		NVert++;\
		TV->x=gx+(px)*StepX;\
		TV->y=gy+(py)*StepY;\
		TV->z=0.0;\
		TV->w=1.0;\
		TV->u=tempU[ofs];\
		TV->v=tempV[ofs];\
		TV->diffuse=0xFFFFFF+(DWORD(TempBuf[ofs])<<24);\
	};
#define GETVIDXC(ofs,VIDX,px,py)\
	VIDX=VertRef[ofs];\
	if(VIDX==-1){\
		TV=VER+NVert;\
		VertRef[ofs]=NVert;\
		VIDX=NVert;\
		NVert++;\
		TV->x=gx+(px)*StepX;\
		TV->y=gy+(py)*StepY;\
		TV->z=0.0;\
		TV->w=1.0;\
		/*TV->u=tex_X0+tex_LX*(px)/(NX+1);\
		TV->v=tex_Y0+tex_LY*(py)/(NY+1);*/\
		TV->diffuse=TempBuf[ofs];\
	};
#define GETVIDX1(ofs,VIDX,px,py)\
	VIDX=VertRef[ofs];\
	if(VIDX==-1){\
		TV=VER+NVert;\
		VertRef[ofs]=NVert;\
		VIDX=NVert;\
		NVert++;\
		TV->x=gx+(px)*StepX;\
		TV->y=gy+float((py)*StepY)*Ratio;\
		TV->z=0.0;\
		TV->diffuse=0xFFFFFF+(DWORD(TempBuf[ofs])<<24);\
	};
bool SquareObject::Create(int gx,int gy,tpGetNetValue* FN,int x0,int y0,QuantType QTX,QuantType QTY,void* param,float tex_X0,float tex_Y0,float tex_LX,float tex_LY){
	int NX=1<<QTX;
	int NY=1<<QTY;
	int StepX=256>>QTX;
	int StepY=256>>QTY;
	int ofs=0;
	int N_ff=0;
	int N_00=0;
	int Nt=0;
	byte TempBuf[17*17];
	float tempU[17*17];
	float tempV[17*17];
	for(int iy=0;iy<=NY;iy++){
		for(int ix=0;ix<=NX;ix++){
			int value=FN(x0+ix,y0+iy,param,tempU[ofs],tempV[ofs]);
			if(value<=0)N_00++;
			if(value>=0xFF)N_ff++;
			Nt++;
			TempBuf[ofs]=value;
			ofs++;
		};
	};
	if(N_00==Nt)return false;
	if(BM&&(DimX!=NX||DimY!=NY)){
		delete(BM);
	};
	if(!BM){
		BM=new BaseMesh;
		BM->create((16+1)*(16+1),6*16*16,vfVertexTnL);
	};
	VertexTnL* VER=(VertexTnL*)BM->getVertexData();
	word* IDX=BM->getIndices();
	DimX=NX;
	DimY=NY;
	int NVert=0;
	int NIdx=0;
	VertexTnL* TV;
	int VertRef[17*17];
	memset(VertRef,0xFF,(NX+1)*(NY+1)*4);

	if(N_ff==Nt){
		int VIDX1,VIDX2,VIDX3,VIDX4;
		GETVIDX(0,VIDX1,0,0);
		GETVIDX(NX,VIDX2,NX,0);
		GETVIDX((NX+1)*NY,VIDX3,0,NY);
		GETVIDX((NX+1)*NY+NX,VIDX4,NX,NY);

		IDX[NIdx  ]=VIDX1;
		IDX[NIdx+1]=VIDX2;
		IDX[NIdx+2]=VIDX3;
		IDX[NIdx+3]=VIDX2;
		IDX[NIdx+4]=VIDX4;
		IDX[NIdx+5]=VIDX3;
		NIdx+=6;
		BM->setNInd(NIdx);
		BM->setNPri(NIdx/3);
		BM->setNVert(NVert);
		return true;
	};
	byte SqMarker[16*16];
	memset(SqMarker,0,NX*NY);

	ofs=0;
	int ofs2=0;

	for(iy=0;iy<NY;iy++){
		for(int ix=0;ix<NX;ix++){
			int V1=TempBuf[ofs];
			int V2=TempBuf[ofs+1];
			int V3=TempBuf[ofs+NX+1];
			int V4=TempBuf[ofs+NX+2];
			int va=(V1+V2+V3+V4)>>2;
			if(V1!=va||V2!=va||V3!=va||V4!=va){
				SqMarker[ofs2]=1;
				int VIDX1,VIDX2,VIDX3,VIDX4;
				GETVIDX(ofs,VIDX1,ix,iy);
				GETVIDX(ofs+1,VIDX2,ix+1,iy);
				GETVIDX(ofs+NX+1,VIDX3,ix,iy+1);
				GETVIDX(ofs+NX+2,VIDX4,ix+1,iy+1);
				IDX[NIdx  ]=VIDX1;
				IDX[NIdx+1]=VIDX2;
				IDX[NIdx+2]=VIDX3;
				IDX[NIdx+3]=VIDX2;
				IDX[NIdx+4]=VIDX4;
				IDX[NIdx+5]=VIDX3;
				NIdx+=6;

			}else if(V1==0&&V1==V2&&V1==V3&&V1==V4)SqMarker[ofs2]=1;
			ofs++;
			ofs2++;
		};
		ofs++;
	};
	bool HaveToDo=0;
	
	do{
		HaveToDo=0;
		ofs=0;
		ofs2=0;
		for(iy=0;iy<NY;iy++){
			for(int ix=0;ix<NX;ix++){
				if(!SqMarker[ofs2]){
					HaveToDo=1;
					int TLX=1;
					int TLY=1;
					int ixx=ix+1;
					while(ixx<NX&&!SqMarker[ofs2+TLX]){
						TLX++;
						ixx++;
					};
					bool incr;
					do{
						if(iy+TLY<NY){
							incr=1;
							int dof=NX*TLY;
							for(int dd=0;dd<TLX&&incr;dd++,dof++)if(SqMarker[ofs2+dof])incr=0;
						}else incr=0;
						if(incr)TLY++;
					}while(incr);
					for(int dx=0;dx<TLX;dx++)
						for(int dy=0;dy<TLY;dy++)SqMarker[ofs2+dx+dy*NX]=1;

					int VIDX1,VIDX2,VIDX3,VIDX4;
					GETVIDX(ofs,VIDX1,ix,iy);
					GETVIDX(ofs+TLX,VIDX2,ix+TLX,iy);
					GETVIDX(ofs+TLY*(NX+1),VIDX3,ix,iy+TLY);
					GETVIDX(ofs+TLY*(NX+1)+TLX,VIDX4,ix+TLX,iy+TLY);
					IDX[NIdx  ]=VIDX1;
					IDX[NIdx+1]=VIDX2;
					IDX[NIdx+2]=VIDX3;
					IDX[NIdx+3]=VIDX2;
					IDX[NIdx+4]=VIDX4;
					IDX[NIdx+5]=VIDX3;
					NIdx+=6;
					goto YYY;
				};
				ofs++;
				ofs2++;
			};
			ofs++;
		};
YYY:;

	}while(HaveToDo);
	
	BM->setNInd(NIdx);
	BM->setNPri(NIdx/3);
	BM->setNVert(NVert);
	return true;
};
bool SquareObject::CreateRGBA(int gx,int gy,tpGetNetValueRGBA* FN,int x0,int y0,QuantType QTX,QuantType QTY,void* param,float tex_X0,float tex_Y0,float tex_LX,float tex_LY){
	int NX=1<<QTX;
	int NY=1<<QTY;
	int StepX=256>>QTX;
	int StepY=256>>QTY;
	int ofs=0;
	int N_ff=0;
	int N_00=0;
	int Nt=0;
	DWORD TempBuf[17*17];
	float tempU[17*17];
	float tempV[17*17];
	for(int iy=0;iy<=NY;iy++){
		for(int ix=0;ix<=NX;ix++){
			DWORD value=FN(x0+ix,y0+iy,param);
			DWORD AV=value>>24;
			if(AV<=0)N_00++;
			if(AV>=0xFF)N_ff++;
			Nt++;
			TempBuf[ofs]=value;
			ofs++;
		};
	};
	if(N_00==Nt)return false;
	if(BM&&(DimX!=NX||DimY!=NY)){
		delete(BM);
	};
	if(!BM){
		BM=new BaseMesh;
		BM->create((16+1)*(16+1),6*16*16,vfVertexTnL);
	};
	VertexTnL* VER=(VertexTnL*)BM->getVertexData();
	word* IDX=BM->getIndices();
	DimX=NX;
	DimY=NY;
	int NVert=0;
	int NIdx=0;
	VertexTnL* TV;
	int VertRef[17*17];
	memset(VertRef,0xFF,(NX+1)*(NY+1)*4);

	if(N_ff==Nt){
		int VIDX1,VIDX2,VIDX3,VIDX4;
		GETVIDX(0,VIDX1,0,0);
		GETVIDX(NX,VIDX2,NX,0);
		GETVIDX((NX+1)*NY,VIDX3,0,NY);
		GETVIDX((NX+1)*NY+NX,VIDX4,NX,NY);

		IDX[NIdx  ]=VIDX1;
		IDX[NIdx+1]=VIDX2;
		IDX[NIdx+2]=VIDX3;
		IDX[NIdx+3]=VIDX2;
		IDX[NIdx+4]=VIDX4;
		IDX[NIdx+5]=VIDX3;
		NIdx+=6;
		BM->setNInd(NIdx);
		BM->setNPri(NIdx/3);
		BM->setNVert(NVert);
		return true;
	};
	byte SqMarker[16*16];
	memset(SqMarker,0,NX*NY);

	ofs=0;
	int ofs2=0;

	for(iy=0;iy<NY;iy++){
		for(int ix=0;ix<NX;ix++){
			DWORD V1=TempBuf[ofs];
			DWORD V2=TempBuf[ofs+1];
			DWORD V3=TempBuf[ofs+NX+1];
			DWORD V4=TempBuf[ofs+NX+2];
			int va=0;//V1;
			if(V1!=va||V2!=va||V3!=va||V4!=va){
				SqMarker[ofs2]=1;
				int VIDX1,VIDX2,VIDX3,VIDX4;
				GETVIDXC(ofs,VIDX1,ix,iy);
				GETVIDXC(ofs+1,VIDX2,ix+1,iy);
				GETVIDXC(ofs+NX+1,VIDX3,ix,iy+1);
				GETVIDXC(ofs+NX+2,VIDX4,ix+1,iy+1);
				if((ix+iy)&1){
					IDX[NIdx  ]=VIDX1;
					IDX[NIdx+1]=VIDX2;
					IDX[NIdx+2]=VIDX3;
					IDX[NIdx+3]=VIDX2;
					IDX[NIdx+4]=VIDX4;
					IDX[NIdx+5]=VIDX3;
				}else{
					IDX[NIdx  ]=VIDX1;
					IDX[NIdx+1]=VIDX2;
					IDX[NIdx+2]=VIDX4;
					IDX[NIdx+3]=VIDX1;
					IDX[NIdx+4]=VIDX4;
					IDX[NIdx+5]=VIDX3;
				}
				NIdx+=6;

			}else if(V1==0&&V1==V2&&V1==V3&&V1==V4)SqMarker[ofs2]=1;
			ofs++;
			ofs2++;
		};
		ofs++;
	};
	bool HaveToDo=0;

	do{
		HaveToDo=0;
		ofs=0;
		ofs2=0;
		for(iy=0;iy<NY;iy++){
			for(int ix=0;ix<NX;ix++){
				if(!SqMarker[ofs2]){
					HaveToDo=1;
					int TLX=1;
					int TLY=1;
					int ixx=ix+1;
					while(ixx<NX&&!SqMarker[ofs2+TLX]){
						TLX++;
						ixx++;
					};
					bool incr;
					do{
						if(iy+TLY<NY){
							incr=1;
							int dof=NX*TLY;
							for(int dd=0;dd<TLX&&incr;dd++,dof++)if(SqMarker[ofs2+dof])incr=0;
						}else incr=0;
						if(incr)TLY++;
					}while(incr);
					for(int dx=0;dx<TLX;dx++)
						for(int dy=0;dy<TLY;dy++)SqMarker[ofs2+dx+dy*NX]=1;

					int VIDX1,VIDX2,VIDX3,VIDX4;
					GETVIDX(ofs,VIDX1,ix,iy);
					GETVIDX(ofs+TLX,VIDX2,ix+TLX,iy);
					GETVIDX(ofs+TLY*(NX+1),VIDX3,ix,iy+TLY);
					GETVIDX(ofs+TLY*(NX+1)+TLX,VIDX4,ix+TLX,iy+TLY);
					IDX[NIdx  ]=VIDX1;
					IDX[NIdx+1]=VIDX2;
					IDX[NIdx+2]=VIDX3;
					IDX[NIdx+3]=VIDX2;
					IDX[NIdx+4]=VIDX4;
					IDX[NIdx+5]=VIDX3;
					NIdx+=6;
					goto YYY;
				};
				ofs++;
				ofs2++;
			};
			ofs++;
		};
YYY:;

	}while(HaveToDo);

	BM->setNInd(NIdx);
	BM->setNPri(NIdx/3);
	BM->setNVert(NVert);
	return true;
};
bool SquareObject::Create2T(int gx,int gy,tpGetNetValue* FN,int x0,int y0,int sx,int sy,float Ratio,QuantType QTX,QuantType QTY,void* param,bool Opt){	
	return true;
};
bool SquareObject::CreateTnL2(int gx,int gy,tpGetNetValue* FN,int x0,int y0,QuantType QTX,QuantType QTY,void* param,float tex_X0,float tex_Y0,float tex_LX,float tex_LY){
	return true;
};
SquareObject* FogOfWar=NULL;
int F3_func(int x,int y,void* param){
	int v=(5-x)*20+(13-y)*20;
	if(v<0)v=0;
	if(v>150)v=180;
	return v;
};
void TestFog3(){
	return;	
};
int GETF3D(int ofs);
int GetInterpFOW2(int x,int y);
int GetF(word k);
#define FMIN 700
#define FOGMAX 255
int GetFractalVal(int x,int y);
const Vector3D* SCorners;
int GetFOW(int x,int y,void* param,float& u,float& v){   
	x<<=10-Shifter;
	y<<=9-Shifter;    
	float x1=SCorners[0].x+(SCorners[1].x-SCorners[0].x)*x/RealLx+(SCorners[3].x-SCorners[0].x)*y/RealLy
		+(SCorners[2].x+SCorners[0].x-SCorners[1].x-SCorners[3].x)*x*y/RealLx/RealLy;
	float y1=SCorners[0].y+(SCorners[1].y-SCorners[0].y)*y/RealLx+(SCorners[3].y-SCorners[0].y)*y/RealLy
		+(SCorners[2].y+SCorners[0].y-SCorners[1].y-SCorners[3].y)*x*y/RealLx/RealLy;
	u=x1;
	v=y1;
	y1/=2;
	int S=1<<(5-Shifter);
	if(S>20)S=20;
	int vv=GetInterpFOW2(x1,y1);
	vv=255-(vv-FMIN)/S;	
	vv=vv*(GetFractalVal(x1*4+y1,y1*4+x1)+256)/1024;
	if(vv<0)vv=0;
	if(vv>FOGMAX)vv=FOGMAX;
	return vv;
};
int GetFOW2(int x,int y){   
	int S=1<<(5-Shifter);
	if(S>20)S=20;
	int vv=GetInterpFOW2(x,y);
	vv=255-(vv-FMIN)/S;	
	vv=vv*(GetFractalVal(x*4+y,y*4+x)+256)/1024;
	if(vv<0)vv=0;
	if(vv>FOGMAX)vv=FOGMAX;
	return vv;
};
extern int RealLx;
extern int RealLy;
//#define COMLEXFOG
void SetupCamera();
void DrawFogInWorldSpace(){
	
	IRS->Flush();
	const Vector3D* GetCameraIntersectionCorners();
	SCorners=GetCameraIntersectionCorners();
	static BaseMesh* BM=NULL;
	int NX=40;
	int NY=40;
	if(!BM){
		BM=new BaseMesh;
		BM->createPatch(NX,NY,Vector3D(0,0,0),Vector3D(1,0,0),Vector3D(0,1,0));
		BM->setShader(IRS->GetShaderID("fog_of_war"));
		static int ftx=IRS->GetTextureID("FogOfWar.tga");
		BM->setTexture(ftx);
		BM->setTexture(ftx,1);
	};
	Vertex2t* V=(Vertex2t*)BM->getVertexData();
	float T=GetTickCount()/14000.0;
	for(int iy=0;iy<=NX;iy++){
		for(int ix=0;ix<=NX;ix++){
            float fx=float(ix)/NX;
			float fy=float(iy)/NY;
			V->x=SCorners[0].x+(SCorners[1].x-SCorners[0].x)*fx+(SCorners[3].x-SCorners[0].x)*fy
				+(SCorners[2].x+SCorners[0].x-SCorners[1].x-SCorners[3].x)*fx*fy;
			V->y=SCorners[0].y+(SCorners[1].y-SCorners[0].y)*fx+(SCorners[3].y-SCorners[0].y)*fy
				+(SCorners[2].y+SCorners[0].y-SCorners[1].y-SCorners[3].y)*fx*fy;
			float px=float(V->x)/1200.0;
			float py=float(V->y)/1200.0;
			V->u=px+px+sin(px+T);
			V->v=py+py+sin(py*1.1+T*1.213)/1.5;			
            V->u2=-V->u;
			V->v2=-V->v;
			DWORD A=GetFOW2(V->x,V->y/2);
			if(EngSettings.DrawShotsFog){
				DWORD GetEffDensity(int x,int y);
				DWORD A1=GetEffDensity(V->x,V->y);
				if(A1>A)A=A1;
			}
			V->diffuse=(A<<24);
			DWORD D=A;
			D=255+128-D;
			if(D>255)D=255;
			V->diffuse|=D+(D<<8)+(D<<16);
			V++;
		}
	}
	IRS->SetTextureFactor(EngSettings.FogOfWarColor);
	IRS->SetShader( BM->getShader() );
	IRS->SetTexture( BM->getTexture( 0 ), 0, false );
	IRS->SetTexture( BM->getTexture( 1 ), 1, false );   
	IRS->SetShaderAutoVars();
	DrawPrimBM(*BM);
	IRS->Flush();
}
void SetupCamera();
void DrawFogOfWar(){
	//ICam->Render();
	DrawFogInWorldSpace();
	return;
	const Vector3D* GetCameraIntersectionCorners();
	 SCorners=GetCameraIntersectionCorners();

	if(!FogOfWar){
		FogOfWar=new SquareObject;
	};
	int Nx=RealLx>>8;
	if(Nx<<8!=RealLx)Nx++;
	int Ny=RealLy>>8;
	if(Ny!=RealLy)Ny++;
	bool first=1;
	for(int iy=0;iy<Ny;iy++){
		for(int ix=0;ix<Nx;ix++){
			int ofs=(ix<<3)+(iy<<11);
			if(FogOfWar->Create(ix<<8,iy<<8,&GetFOW,ix<<3,iy<<4,qt_32,qt_16,(void*)ofs,0,0,1,1)){
				Matrix4D M=Matrix4D::identity;
				M.e00= 1.15f;
				M.e11= 0.92f;
				M.e10=-0.2f;
				M.e01= 0.27f;
				IRS->SetTextureTM(M,1);
				VertexTnL* VER=(VertexTnL*)FogOfWar->BM->getVertexData();
				int NV=FogOfWar->BM->getNVert();
				float T=GetTickCount()/14000.0;
				for(int i=0;i<NV;i++){
					float px=float(VER->u)/1200.0;
					float py=float(VER->v)/1200.0;
					VER->u=px+px+sin(px+T);
					VER->v=py+py+sin(py*1.1+T*1.213)/1.5;
					VER++;
				};

				if(first){
					//if(GetKeyState(VK_MENU)&0x8000)FogOfWar->BM->setShader(IRS->GetShaderID("wfog"));
					//else 
					FogOfWar->BM->setShader(IRS->GetShaderID("fog_of_war"));
                    static int ftx=IRS->GetTextureID("FogOfWar.tga");
					FogOfWar->BM->setTexture(ftx);
					FogOfWar->BM->setTexture(ftx,1);
					first=0;
				};
				IRS->SetTextureFactor(EngSettings.FogOfWarColor);
				DrawBM(*FogOfWar->BM);
#ifdef COMLEXFOG
				VER=(VertexTnL*)FogOfWar->BM->getVertexData();
				for(i=0;i<NV;i++){
					float px=(float(mapx<<(Shifter))+VER->x)/1200.0;
					float py=(float(mapy<<(Shifter-1))+VER->y)/1800.0;
					VER->u=px+px+sin(-T);
					VER->v=py+py+sin(-T*1.213);
					//VER->u2=px+px+cos(px*1.61+T*0.94);
					//VER->v2=py+py+sin(py*1.12+T*1.113);
					VER++;
				};
				IRS->SetTextureFactor(EngSettings.FogOfWarColor);
				DrawBM(*FogOfWar->BM);
#endif
			};
		};
	};
};
extern byte* WaterDeep;
SquareObject* Water3D=NULL;
SquareObject* Water3DPS=NULL;
int WaterTexSys=-1;
int WaterTexDef=-1;
int WaterTexSysPS=-1;
int WaterTexDefPS=-1;
word WPALETTE[1024];
word AWPAL[256];
bool WPDone=0;
void CreateWPalette(){
	ResFile F=RReset("wpal.raw");
	if(F!=INVALID_HANDLE_VALUE){
		byte WPL[768];
		RBlockRead(F,WPL,768);
		RClose(F);
		for(int j=0;j<256;j++){
			int cr=WPL[j*3  ]>>3;
			int cg=WPL[j*3+1]>>2;
			int cb=WPL[j*3+2]>>3;
			//int ca=(cb*32+32)>>4;
			//if(ca>15)ca=15;
			int p=128+(j-128)*6;
			if(p<0)p=0;
			if(p>255)p=255;
			WPALETTE[j+512-128]=cb+(cg<<5)+(cr<<11);//+(ca<<12);
			//WPALETTE[j+512-128]=cb+(cg<<4)+(cr<<8)+(ca<<12);
		};
	};
	for(int i=0;i<512-128;i++)WPALETTE[i]=WPALETTE[512-128];
	for(i=512+128;i<1024;i++)WPALETTE[i]=WPALETTE[512+128-1];
	WPDone=1;
	for(i=0;i<256;i++){
		int j=122+(i-122)*20;
		if(j<0)j=0;
		if(j>255)j=255;
		int CR=(j/32)+16-(256/32);
		int CG=(j/64)+16-(256/64);
		int CB=(j/64)+16-(256/64);
		if(CR>15)CR=15;
		if(CR<0)CR=0;
		if(CG>15)CG=15;
		if(CG<0)CG=0;
		if(CB>15)CB=15;
		if(CB<0)CB=0;
		AWPAL[i]=((j>>4)<<12)+(CR<<8)+(CG<<4)+(CB);
	};
};
extern int MaxWX;
extern int MaxWY;
#define ANGL 21
int GetWDeep(int x,int y,void* param){
	//x<<=5-Shifter;
	//y<<=5-Shifter;
	int ofs=x+(y*MaxWX);
	int w=int(WaterDeep[ofs])-128;
	if(w>0){
		int W0=128;
		int DD=0;
		int ww;
		int WP;
		int DN=w+128;
		int DP;
		do{
			WP=W0;
			W0+=ANGL;
			DD+=32;
			ofs-=MaxWX;
			DP=DN;
			DN=WaterDeep[ofs];
		}while(ofs>MaxWX&&DN>W0);
		int D1=DP-WP;
		int D2=W0-DN;
		int DX=32*D2/(D1+D2);
		DD-=DX;
		W0-=DX*ANGL/32;

		W0-=128;

		w=sqrt(W0*W0+DD*DD);
	};
	w<<=1;
	w+=48;
	if(w>235)w=235;
	if(w<0)w=0;
	return w;
};
#define WaveLx 260
#define WaveLy 260
#define WaveSize (WaveLx*WaveLy)
int GetOSTR(int v){
	return v;
	if(v<0)return v/3;
	return (v+v*v/1000)/3;
};
extern short CostHi[32*32];
int tidx=0;
void Draw3DWaterWithReflection(short* Wave);
class ReflectionImage{
public:
	BaseMesh* Sky;
	BaseMesh* Surf;
	int TargetID;
	int TargetSize;
	float PrevMx;
	float PrevMy;
	ReflectionImage();
	~ReflectionImage();
	void CreateImage(int mx,int my);
};
ReflectionImage RIMG;
extern short Wave0[WaveSize*2];
extern short Wave1[WaveSize*2];
extern short Wave2[WaveSize*2];
void RenormWater(){
	int W00=Wave0[10+10*WaveLx];
	int NW0=0;
	int W0=0;
	int W01=0;
	int W02=0;
	for(int ix=1;ix<20;ix++){
		for(int iy=1;iy<20;iy++){
			W0+=short(Wave0[ix*10+(iy*10)*WaveLx]-W00);
			W01+=short(Wave1[ix*10+(iy*10)*WaveLx]-W00);
			W02+=short(Wave2[ix*10+(iy*10)*WaveLx]-W00);
			NW0++;
		};
	};
	W0/=NW0;
	W0+=W00;
	W0=short(W0);

	W01/=NW0;
	W01+=W00;
	W01=short(W01);

	W02/=NW0;
	W02+=W00;
	W02=short(W02);

	int sz=WaveLx*WaveLx;
	int E=0;
	for(int i=0;i<sz;i++){
		int DE=short(Wave0[i]-W0);
		E+=DE*DE/1000;
	};
	E/=sz;
	if(!E)return;
	int E0=9000;
	if(E<E0)E=E0-100;
	if(E>E0)E=E0+100;
	for(i=0;i<sz;i++){
		Wave0[i]=W0+int(short(Wave0[i]-W0))*E0/E;
		Wave1[i]=W01+int(short(Wave1[i]-W01))*E0/E;
		Wave2[i]=W02+int(short(Wave2[i]-W02))*E0/E;
	};

};
bool FIRSTWATER=1;
bool USE3DWATER=0;
#define WS_Size 16
#define WS_ASize (WS_Size*WS_Size)
#define WS_Mask 15
#define WS_Shift 4
float WaterShape0[WS_Size*WS_Size];
float WaterShape1[WS_Size*WS_Size];
float WaterShape2[WS_Size*WS_Size];
float* CurWShape;
int WS_pos=0;
float WS_Average;

void ProcessWaterShape(float* W0,float* W1,float* W2){
	int ofs=0;
	float B=0.0005;
	WS_Average=0;
	for(int iy=0;iy<WS_Size;iy++){
		for(int ix=0;ix<WS_Size;ix++){
			int ofsL=((ix-1)&WS_Mask)+(iy<<WS_Shift);
			int ofsR=((ix+1)&WS_Mask)+(iy<<WS_Shift);
			int ofsU=ix+(((iy-1)&WS_Mask)<<WS_Shift);
			int ofsD=ix+(((iy+1)&WS_Mask)<<WS_Shift);
			float W=W1[ofs];
			WS_Average+=W0[ofs]=B*(W1[ofsL]+W1[ofsR]+W1[ofsU]+W1[ofsD]-4*W)+W+W-W2[ofs];
			ofs++;
		};
	};
	WS_Average/=WS_ASize;
};
bool WS_IsInit=0;
#define GOFS(x,y) (((x)&WS_Mask)+(((y)&WS_Mask)<<WS_Shift))
void InitWShape(){
	int i=0;
	for(int iy=0;iy<WS_Size;iy++){
		for(int ix=0;ix<WS_Size;ix++){
			float v=sqrt((ix-8)*(ix-8)+(iy-8)*(iy-8));
			WaterShape0[i]=sin(v*2.0)*1+float(rand()-16384)/6000.0;
			WaterShape1[i]=WaterShape0[i];
			WaterShape2[i]=WaterShape0[i];
			i++;
		};
	};
	for(int p=0;p<1;p++){
		for(int iy=0;iy<WS_Size;iy++){
			for(int ix=0;ix<WS_Size;ix++){
				WaterShape1[GOFS(ix,iy)]=(
					WaterShape0[GOFS(ix,iy)]+
					WaterShape0[GOFS(ix+1,iy)]+
					WaterShape0[GOFS(ix-1,iy)]+
					WaterShape0[GOFS(ix,iy-1)]+
					WaterShape0[GOFS(ix,iy+1)])/5.0;
			};
		};
		for(int i=0;i<WS_ASize;i++){
			WaterShape0[i]=WaterShape1[i];
			WaterShape2[i]=WaterShape1[i];

		};
	};
};
void ProcessWSurface(){
	if(GetKeyState('Y')&0x8000||!WS_IsInit){
		WS_IsInit=1;
		InitWShape();
	};
	//return;
	switch(WS_pos%3){
	case 0:
		ProcessWaterShape(WaterShape0,WaterShape1,WaterShape2);
		CurWShape=WaterShape0;
		break;
	case 1:
		ProcessWaterShape(WaterShape2,WaterShape0,WaterShape1);
		CurWShape=WaterShape2;
		break;
	case 2:
		ProcessWaterShape(WaterShape1,WaterShape2,WaterShape0);
		CurWShape=WaterShape1;
		break;
	};
	WS_pos++;
};
void DrawClusterizedWaterWithBump(short* Wave);
void Draw3DWater(short* Wave){
	ProcessWSurface();	
	Draw3DWaterWithReflection(Wave);
};


inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

void Draw3DWaterWithReflection(short* Wave){
	DrawClusterizedWaterWithBump(Wave);
};

ReflectionImage::ReflectionImage(){
	memset(this,0,sizeof *this);
};
ReflectionImage::~ReflectionImage(){
	if(Sky)delete(Sky);
	if(Surf)delete(Surf);
	memset(this,0,sizeof *this);
};
DWORD TXCOLOR[64];
bool TCDONE=0;
void InitTexColors(){
	if(TCDONE)return;
	TCDONE=1;
	BMPformat BM;
	byte* data;
	if(ReadBMP24("Textures\\GroundTex.bmp",&BM,&data)){
		for(int ix=0;ix<8;ix++){
			for(int iy=0;iy<8;iy++){
				int RC=0;
				int GC=0;
				int BC=0;
				for(int dx=0;dx<64;dx++){
					for(int dy=0;dy<64;dy++){
						int ofs=(((ix<<6)+dx)+(((iy<<6)+dy)<<9))*3;
						RC+=data[ofs  ];
						GC+=data[ofs+1];
						BC+=data[ofs+2];
					};
				};
				RC>>=12;
				GC>>=12;
				BC>>=12;
				TXCOLOR[ix+iy*8]=0xFF000000+(BC<<16)+(GC<<8)+RC;
			};
		};
		free(data);
	};
};

DWORD GetDW_Light(int idx);
DWORD CORR(DWORD C,int V){
	DWORD L=GetDW_Light(V);

	int LR=L&255;
	int LG=(L>>8)&255;
	int LB=(L>>16)&255;
	int LA=(L>>24)&255;

	int R=C&255;
	int G=(C>>8)&255;
	int B=(C>>16)&255;
	int A=(C>>24)&255;

	R=(R*LR)/128;
	G=(G*LG)/128;
	B=(B*LB)/128;

	return (0x80<<24)+(B<<16)+(G<<8)+R;
}
DWORD GetDwordModulated(DWORD C,int V){
	DWORD C1=0;
	for(int i=24;i>=0;i-=8){
		int CC=int((C>>i)&0xFF)*V/255;
		if(CC>255)CC=255;
		if(CC<0)CC=0;
		C1+=CC<<i;
	}
	return C1;
}
void RecreateWaterDeep();
DWORD GetWaterColor(int x,int y);
#define SkySize 16
void RefreshWater(){
	RIMG.PrevMx=-1;
}
extern float fMapX;
extern float fMapY;
void ReflectionImage::CreateImage(int mx,int my){
	if(PrevMx==fMapX&&PrevMy==fMapY&&!EngSettings.DynamicalReflection)return;//incorrect!!!!!!!!!!!!!!!!!!!!!!!

	PrevMx=fMapX;
	PrevMy=fMapY;

	RecreateWaterDeep();

	if(!Sky){
		void SetWaterColor(int x,int y,byte R,byte G,byte B);
		/*TargetSize=256;
		td.setValues(TargetSize,TargetSize,cfRGB565,mpVRAM,1,tuRenderTarget);
		TargetID=0;
		TargetID=IRS->CreateTexture("WaterTargetID",td);*/
		TargetID = IRMap->GetReflectionTextureID();
		assert(TargetID>0);
		Sky=new BaseMesh;
		Surf=new BaseMesh;
		Sky->create((SkySize+1)*(SkySize+1),SkySize*SkySize*6,vfVertexTnL);
		Surf->create(4096,4096,vfVertexTnL);
		Sky->setTexture(IRS->GetTextureID("Oblaka123g1.tga"));
		Sky->setShader(IRS->GetShaderID("RefSky"));
		Surf->setShader(IRS->GetShaderID("RefSurf"));
	};

	if(TargetID<=0)return;
	VertexTnL* SKV=(VertexTnL*)Sky->getVertexData();
	word* IDX=Sky->getIndices();
	//sky initialisationICamera
	//Vector3D VP=ICam->GetPos();
	//float DD=16*1689/VP.z;
	int cdx=RealLx<<(5-Shifter);
	int cdy=RealLy<<(6-Shifter);
	float DT=float(GetTickCount())/46000;
	float DX=float(fMapX*8)/RealLx;
	float DY=float(fMapY*4)/RealLy;

	for(int y=0;y<=SkySize;y++){
		for(int x=0;x<=SkySize;x++){
            VertexTnL* V=SKV+x+y*(SkySize+1);
			V->x=256*x/SkySize;
			V->y=256*y/SkySize;
			V->w=1.0;
			V->diffuse=0xFFFFFFFF;//GetWaterColor((mapx<<5)+cdx*x/SkySize,(mapy<<5)+cdy*y/SkySize);
			V->u=DX+DT+float(x)/float(SkySize);
			V->v=DY-DT+float(y)/float(SkySize);
		}
	}

	for(y=0;y<SkySize;y++){
		for(int x=0;x<SkySize;x++){
			int v0=x+y*(SkySize+1);
            IDX[0]=v0;
			IDX[1]=v0+1;
			IDX[2]=v0+SkySize+1;
			IDX[3]=v0+SkySize+1;
			IDX[4]=v0+1;
			IDX[5]=v0+SkySize+2;
			IDX+=6;
		}
	}

	Sky->setNInd(SkySize*SkySize*6);
	Sky->setNPri(SkySize*SkySize*2);
	Sky->setNVert((SkySize+1)*(SkySize+1));

	IRS->PushRenderTarget(TargetID);
	DrawBM(*Sky);//sky Drawing
	
	extern float fMapX;
	extern float fMapY;
	int mdy=fMapX*16+64;
	int mdx=fMapY*32;
	VertexTnL* SV=(VertexTnL*)Surf->getVertexData();
	word* VID=Surf->getIndices();
	int NV=0;
	int NI=0;

	int MY0=(my&0xFFFE)-20;
	if(MY0<2)MY0=2;
	int MY1=my+smaply;
	int mx0=mx&0xFFFE;
	int mx1=(mx+smaplx+2)&0xFFFE;
	int dy=VertInLine<<1;
	for(int iy=MY0;iy<MY1;iy+=2){
		int ofs0=iy*VertInLine+mx0;
		for(int ix=mx0;ix<mx1;ix+=2){

			int V0=THMap[ofs0]*3/2;
			int V1=THMap[ofs0+2]*3/2;
			int V2=THMap[ofs0+dy]*3/2;
			int V3=THMap[ofs0+dy+2]*3/2;
			int VA=THMap[ofs0-dy]*3/2;
			int VB=THMap[ofs0-dy+2]*3/2;

			int SYA=(iy<<4)-32+VA-mdy;
			int SYB=(iy<<4)-32+VB-mdy;
			int SY0=(iy<<4)+V0-mdy;
			int SY1=(iy<<4)+V1-mdy;
			int SY2=(iy<<4)+32+V2-mdy;
			int SY3=(iy<<4)+32+V3-mdy;
			if(V0>0&&V1>0){
				if(SY0>SY2||SY1>SY3){
					if(SY0>0||SY1>0||SY2>0||SY3>0){
						if(SY0<RealLy||SY1<RealLy||SY2<RealLy||SY3<RealLy){
							if(SY2>SY0)SY2=SY0;
							if(SY3>SY1)SY3=SY1;
							int X0=(ix<<5)-mdx;
							VID[NI  ]=NV  ;
							VID[NI+1]=NV+1;
							VID[NI+2]=NV+2;
							VID[NI+3]=NV+1;
							VID[NI+4]=NV+3;
							VID[NI+5]=NV+2;
							NI+=6;

							SV[NV].x=float(X0*TargetSize)/RealLx;
							SV[NV].y=float(SY0*TargetSize)/RealLy;
							SV[NV].u=0;
							SV[NV].v=0;
							SV[NV].diffuse=SYA-SY0>20?CORR(TXCOLOR[TexMap[ofs0]],ofs0):0;
							SV[NV].w=1.0;
							NV++;

							SV[NV].x=float((X0+64)*TargetSize)/RealLx;
							SV[NV].y=float(SY1*TargetSize)/RealLy;
							SV[NV].u=0;
							SV[NV].v=0;
							SV[NV].diffuse=SYB-SY1>20?CORR(TXCOLOR[TexMap[ofs0+2]],ofs0+2):0;
							SV[NV].w=1.0;
							NV++;

							SV[NV].x=float(X0*TargetSize)/RealLx;
							SV[NV].y=float(SY2*TargetSize)/RealLy;
							SV[NV].u=0;
							SV[NV].v=0;
							SV[NV].diffuse=SY0-SY2>20?CORR(TXCOLOR[TexMap[ofs0+dy]],ofs0+dy):0;
							SV[NV].w=1.0;
							NV++;

							SV[NV].x=float((X0+64)*TargetSize)/RealLx;
							SV[NV].y=float(SY3*TargetSize)/RealLy;
							SV[NV].u=0;
							SV[NV].v=0;
							SV[NV].diffuse=SY1-SY3>20?CORR(TXCOLOR[TexMap[ofs0+dy+2]],ofs0+dy+2):0;
							SV[NV].w=1.0;
							NV++;
						};
					};
				};
			};
			ofs0+=2;
		};
	};
	Surf->setNInd(NI);
	Surf->setNPri(NI/3);
	Surf->setNVert(NV);
	PrevMx=mx;
	PrevMy=my;
	
	IRMap->Render();
	IRMap->CleanObjects();

	if(Surf->getNInd())DrawBM(*Surf);//drawing reflected surface

	//IRS->SaveTexture( TargetID, "c:\\dumps\\sky.dds" );
	IRS->PopRenderTarget();
}; // ReflectionImage::CreateImage

bool HiChanged=0;

int MinVx=100000;
int MaxVx=0;
int MinVy=100000;
int MaxVy=0;

void ReportHChange(int v){
	int vx=v%VertInLine;
	int vy=v/VertInLine;
	if(vx<MinVx)MinVx=vx;
	if(vx>MaxVx)MaxVx=vx;
	if(vy<MinVy)MinVy=vy;
	if(vy>MaxVy)MaxVy=vy;
	HiChanged=1;
};

int GetHeight(int x,int y);
void RecreateWaterDeep(){
	if(!HiChanged)return;
	void ResetWaterSys();
	ResetWaterSys();
	int ofs=0;
	MinVx-=4;
	MinVy-=4;
	MaxVx+=4;
	MaxVy+=4;
	int m=MaxWX*MaxWY;
	for(int ix=MinVx;ix<=MaxVx;ix++){
		for(int iy=MinVy;iy<=MaxVy;iy++){
			int ofs=ix+iy*MaxWX;
			if(ofs>=0&&ofs<m){
				int Deep=GetHeight(ix<<5,iy<<5);
				int V=128-Deep;
				if(V<0)V=0;
				if(V>255)V=255;
				WaterDeep[ofs]=V;
			};
		};
	};
	HiChanged=0;
	MinVx=100000;
	MaxVx=0;
	MinVy=100000;
	MaxVy=0;
};
extern int WaterTexSys;
extern int WaterTexDef;
extern int WaterTexSysPS;
extern int WaterTexDefPS;
void DrawWaterInWorldSpace(short* Wave){
	const Vector3D* GetCameraIntersectionCorners();
	SCorners=GetCameraIntersectionCorners();
	static BaseMesh* BM=NULL;
	int NX=40;
	int NY=40;
	if(!BM){
		BM=new BaseMesh;
		BM->createPatch(NX,NY,Vector3D(0,0,0),Vector3D(1,0,0),Vector3D(0,1,0));
		WaterTexSysPS=IRS->CreateTexture("WaterTexSysPS",256,256,cfV8U8,1,tmpManaged);
		WaterTexDefPS=WaterTexSysPS;
		void CreateWPalette();
		CreateWPalette();
		BM->setShader(IRS->GetShaderID("BumpWater"));
		BM->setTexture(WaterTexDefPS,0);
		if(RIMG.TargetID>0)BM->setTexture(RIMG.TargetID,1);
		else BM->setTexture(IRS->GetTextureID("Oblaka123g1.tga"),1);
	}		

	int pitch=0;
	byte* Tbits=IRS->LockTexBits(WaterTexSysPS,pitch);
	if(pitch){
		memcpy(Wave+WaveLx*WaveLy-WaveLx*2,Wave+WaveLx*2,WaveLx*2);
		memcpy(Wave+WaveLx*WaveLy-WaveLx,Wave+WaveLx*3,WaveLx*2);
		int ofs=0;
		for(int i=0;i<WaveLy;i++){
			Wave[ofs]=Wave[ofs+256];
			Wave[ofs+1]=Wave[ofs+257];
			Wave[ofs+258]=Wave[ofs+2];
			Wave[ofs+259]=Wave[ofs+3];
			ofs+=WaveLx;
		};
		__asm{
			push	esi
			push	edi
			mov		esi,Wave
			add     esi,(WaveLx+1)*4
			mov		edi,Tbits
			mov		ch,(WaveLx-4)>>1
			cld
			xor		eax,eax
GW4_1:
			mov		cl,WaveLx-4
GW4_2:
			mov		ax,[esi]
			sub		ax,[esi+WaveLx*4]
			sar		ax,5
			add		ax,128
			mov     [edi],al

			mov		bx,[esi-2]
			sub		bx,[esi+2]
			sar		bx,5
			add		bx,128
			mov     [edi+1],bl

			add		esi,2
			add		edi,2
			dec		cl
			jnz		GW4_2
			add		esi,8
			add     esi,WaveLx*2
			dec		ch
			jnz		GW4_1
			pop		edi
			pop		esi
		}
		for(i=0;i<128;i++){
			memcpy(Tbits+128*512+i*512,Tbits+i*512,512);
		}
		IRS->UnlockTexBits(WaterTexSysPS);
		IRS->SetShader(BM->getShader());
		IRS->SetTexture(WaterTexDefPS,0);
		IRS->SetTexture(RIMG.TargetID,1);
		float BScale=1.0f/float(1<<(5-Shifter));
		BScale=BScale*BScale*0.3;

		IRS->SetBumpTM( Matrix3D(   0.0f, 0.05f*BScale, 0.0f, 
			                        0.02f*BScale, 0.0f, 0.0f,
			                        1.0f, 0.0f, 0.0f ) );
	}
	Vertex2t* V=(Vertex2t*)BM->getVertexData();
	float T=GetTickCount()/14000.0;
	for(int iy=0;iy<=NX;iy++){
		for(int ix=0;ix<=NX;ix++){
			float fx=float(ix)/NX;
			float fy=float(iy)/NY;
			V->x=SCorners[0].x+(SCorners[1].x-SCorners[0].x)*fx+(SCorners[3].x-SCorners[0].x)*fy
				+(SCorners[2].x+SCorners[0].x-SCorners[1].x-SCorners[3].x)*fx*fy;
			V->y=SCorners[0].y+(SCorners[1].y-SCorners[0].y)*fx+(SCorners[3].y-SCorners[0].y)*fy
				+(SCorners[2].y+SCorners[0].y-SCorners[1].y-SCorners[3].y)*fx*fy;
			float px=float(V->x)/1200.0;
			float py=float(V->y)/1200.0;
			V->u=px+px+sin(px+T);
			V->v=py+py+sin(py*1.1+T*1.213)/1.5;			
			V->u2=-V->u;
			V->v2=-V->v;
			DWORD A=128;//GetFOW2(V->x,V->y/2);
			V->diffuse=(A<<24);
			DWORD D=A;
			D=255+128-D;
			if(D>255)D=255;
			V->diffuse|=D+(D<<8)+(D<<16);
			V++;
		}
	}
	IRS->SetTextureFactor(EngSettings.FogOfWarColor);
	DrawBM(*BM);
}
