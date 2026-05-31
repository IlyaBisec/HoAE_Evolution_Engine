#include "stdheader.h"
#include "SpraySystem.h"

SprayCell NullCell={0,0,0,0,0,0,0,0};

SpraySystem SpraySys;

// --- SpraySystem ---

int SquadNx;
int SquadNy;
int SquadNSH;
int SquadN;

int MapLx;
int MapLy;
int MapSH;

void InitSpraySystem(){
	SpraySys.Init();
};

void SpraySystem::Init(){
	MapLx=8192<<ADDSH;
	MapLy=8192<<ADDSH;
	MapSH=13+ADDSH;

	SquadNx=MapLx>>SquadSH;
	SquadNy=MapLy>>SquadSH;
	SquadNSH=13+ADDSH-SquadSH;
	SquadN=SquadNx*SquadNy;

	if(Squads) Free();
	Squads=(SpraySquad**)malloc(sizeof(SpraySquad*)*SquadN);
}

bool SpraySystem::AddDust(int x, int y, byte r, byte g, byte b, float a){
	if(x<0||y<0||x>=MapLx||y>=MapLy) return false;
	int xx=x>>SquadSH;
	int yy=y>>SquadSH;
	int sid=xx+(yy<<SquadNSH);
	if(sid<0||sid>=SquadN) return false;
	SpraySquad* SQD=Squads[sid];
	if(!SQD){
		SQD=(SpraySquad*)malloc(sizeof(SpraySquad));
		SQD->Init();
		Squads[sid]=SQD;
	}
	xx=(x-(xx<<SquadSH))>>SpraySH;
	yy=(y-(yy<<SquadSH))>>SpraySH;
	int cid=xx+(yy<<SprayNSH);
	SprayCell* SC=SQD->Cells+cid;	
	if(int(SC->A)+int(a)>255){
		SC->A=255;
	}else{
		SC->A+=a;
	}
	SC->R=r;
	SC->G=g;
	SC->B=b;
	return true;
};
int GTm=0;
void SpraySystem::Process(){
	if(!EngSettings.DrawShotsFog)return;
	GTm=GetTickCount()/2000;
	SpraySquad** SQD=Squads;
	for(int iy=0;iy<SquadNy;iy++){
		for(int ix=0;ix<SquadNx;ix++){
			if(*SQD){
				if(!(*SQD)->Process(ix<<3,iy<<3)){
					free(*SQD);
					*SQD=NULL;
				};
			}
			SQD++;
		};
	}
};

extern RLCFont  YellowFont;
byte FogWaves[64*64];
bool FW_setup=0;
#define FW(x,y) FogWaves[((x)&63)+(((y)&63)<<6)]
void doFW_Setup(){
	FW_setup=1;
	{
		for(int ix=0;ix<64;ix++){
			for(int iy=0;iy<64;iy++){
				int ofs=ix+(iy<<6);
				FogWaves[ofs]=(rand()&255);
			}
		}
	}
	for(int p=0;p<5;p++){
		byte FogWaves2[64*64];		
		for(int ix=0;ix<64;ix++){
			for(int iy=0;iy<64;iy++){
				int mid=(int(FW(ix-1,iy))+int(FW(ix+1,iy))+int(FW(ix,iy-1))+int(FW(ix,iy+1)))>>2;
				FogWaves2[ix+(iy<<6)]=mid;
			}
		}
		memcpy(FogWaves,FogWaves2,sizeof FogWaves);
	}
}
#define DT0 2000
extern float fMapX;
extern float fMapY;
DWORD GetSpraySysDust(int x,int y,void* param){
	if(!FW_setup)doFW_Setup();

	int xx=mapx+(x<<(5-Shifter));
	int yy=mapy+(y<<(5-Shifter));	
	
	int vv =SpraySys.GetSprayDust(xx,yy);
	vv=(vv*3)/7;
	if(vv>160)vv=160;
	return (vv<<24)+0xBFBFBF;
};

SprayCell* SpraySystem::GetSprayCell(int x,int y){
	// x,y - 32x32

	//x<<=5;
	//y<<=5;
	//int xx=x>>SquadSH;
	//int yy=y>>SquadSH;
	//int sid=xx+(yy<<SquadNSH);

	if(x<0||y<0||x>=(MapLx>>5)||y>=(MapLy>>5)) return &NullCell;
	
	int x0=x>>3;
	int y0=y>>3;
	int sid=x0+(y0*SquadNy);
	
	if(sid>=SquadN) return &NullCell;
	SpraySquad* SQD=Squads[sid];
	if(!SQD) return &NullCell;

	//xx=(x-(xx<<SquadSH))>>SpraySH;
	//yy=(y-(yy<<SquadSH))>>SpraySH;
	
	int cid=(x-(x0<<3))+((y-(y0<<3))<<SprayNSH);	

	return SQD->Cells+cid;
};
/*
SprayCell* SpraySystem::GetSprayCell(int x,int y){
	// x,y - 32x32

	//x<<=5;
	//y<<=5;
	//int xx=x>>SquadSH;
	//int yy=y>>SquadSH;
	//int sid=xx+(yy<<SquadNSH);

	if(x<0||y<0||x>=(MapLx>>5)||y>=(MapLy>>5)) return &NullCell;
	
	int x0=x>>3;
	int y0=y>>3;
	int sid=x0+(y0*SquadNy);
	
	if(sid>=SquadN) return &NullCell;
	SpraySquad* SQD=Squads[sid];
	if(!SQD) return &NullCell;

	//xx=(x-(xx<<SquadSH))>>SpraySH;
	//yy=(y-(yy<<SquadSH))>>SpraySH;
	
	int cid=(x-(x0<<3))+((y-(y0<<3))<<SprayNSH);	

	return SQD->Cells+cid;
};
*/
int SpraySystem::GetSprayDust(int x,int y){
	if(x<0||y<0||x>=MapLx||y>=MapLy) return 0;
	x<<=5;
	y<<=5;
	//x-=50;
	//y-=50;
	//int R=255*8/sqrt(64+x*x+y*y);
	//return R;
	int xx=x>>SquadSH;
	int yy=y>>SquadSH;
	int sid=xx+(yy<<SquadNSH);
	if(sid>=SquadN) return 0;
	SpraySquad* SQD=Squads[sid];
	if(!SQD) return 0;
	xx=(x-(xx<<SquadSH))>>SpraySH;
	yy=(y-(yy<<SquadSH))>>SpraySH;
	int cid=xx+(yy<<SprayNSH);
	SprayCell* SC=SQD->Cells+cid;
	
	//char buf[256];
	//sprintf(buf,"%d",SC->a);	
	//ShowString(x-(mapx<<5),y-(mapy<<4),buf,&YellowFont);

	return SC->a;
};

extern int RealLx;
extern int RealLy;
DWORD GetEffDensity(int x,int y){
	int xx=x/32;
	int yy=y/32;
	xx&=0xFFFE;
	yy&=0xFFFE;
	DWORD A=0;
	int A0=SpraySys.GetSprayDust(xx,yy);
	int A1=SpraySys.GetSprayDust(xx+2,yy);
	int A2=SpraySys.GetSprayDust(xx,yy+2);
	int A3=SpraySys.GetSprayDust(xx+2,yy+2);
	if(A0||A1||A2||A3){
		int dx=x-xx*32;
		int dy=y-yy*32;
		int aa=A0+(A1-A0)*dx/64+(A2-A0)*dy/64+(A3+A0-A1-A2)*dx*dy/64/64;
		if(aa>16){
			int GetFractalVal(int x,int y);
			int xxx=x*4;
			int yyy=y*4;
			int V=GetFractalVal(xxx,yyy);
			aa=aa*(V+128)/(512+128);
		}
		if(aa<0)aa=0;
		if(aa>255)aa=255;
		A=aa;
		return A;
	}
	return 0;
}
void DrawDustInWorldSpace(){
	const Vector3D* SCorners;
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
			//DWORD A=GetFOW2(V->x,V->y/2);
			int xx=V->x/32;
			int yy=V->y/32;
			DWORD A=0;
			int A0=SpraySys.GetSprayDust(xx,yy);
			int A1=SpraySys.GetSprayDust(xx+1,yy);
			int A2=SpraySys.GetSprayDust(xx,yy+1);
			int A3=SpraySys.GetSprayDust(xx+1,yy+1);
			if(A0||A1||A2||A3){
				int dx=V->x-xx*32;
				int dy=V->y-yy*32;
				int aa=A0+(A1-A0)*dx/32+(A2-A0)*dy/32+(A3+A0-A1-A2)*dx*dy/32/32;
				if(aa>16){
					int GetFractalVal(int x,int y);
					int xxx=V->x*4;
					int yyy=V->y*4;
					int V=GetFractalVal(xxx,yyy);
					aa=aa*(V+128)/(512+128);
				}
				if(aa<0)aa=0;
				if(aa>255)aa=255;
				A=aa;
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
	DrawBM(*BM);
}
void SpraySystem::Draw(){
	//if(!EngSettings.DrawShotsFog)return;
	//DrawDustInWorldSpace();
	return;
	if(!DXSquare){
		DXSquare=new SquareObject;
	};
	int Nx=RealLx>>8;
	if(Nx<<8!=RealLx)Nx++;
	int Ny=RealLy>>8;
	if(Ny<<8!=RealLy)Ny++;
	bool first=1;
	for(int iy=0;iy<Ny;iy++){
		for(int ix=0;ix<Nx;ix++){

			int ofs=(ix<<3)+(iy<<11);
			if(DXSquare->CreateRGBA(ix<<8,iy<<8,&GetSpraySysDust,ix<<3,iy<<4,qt_32,qt_16,(void*)ofs,0,0,1,1)){
				VertexTnL* VER=(VertexTnL*)DXSquare->BM->getVertexData();
				int NV=DXSquare->BM->getNVert();				
				
				
				float T=GetTickCount()/51000.0;
				for(int i=0;i<NV;i++){
					float px=(float(mapx<<(Shifter))+VER->x)/900.0;
					float py=(float(mapy<<(Shifter-1))+VER->y)/900.0;
					VER->v=px*0.8+py*0.7+T*0.3+cos(px*0.21);
					VER->u=py*0.7-px*0.8+T*0.3+sin(T*0.1+py*0.21);
					VER++;
				};
				

				if(first){
					//if(GetKeyState(VK_MENU)&0x8000)FogOfWar->BM->setShader(IRS->GetShaderID("wfog"));
					//else FogOfWar->BM->setShader(IRS->GetShaderID("fow"));
					static int sh=IRS->GetShaderID("dust");
					static int tx=IRS->GetTextureID("tx3.tga");
					DXSquare->BM->setShader(sh);
					DXSquare->BM->setTexture(tx);
					//FogOfWar->BM->setTexture(IRS->GetTextureID("temp\\tx1.tga"),1);
					first=0;
				};
				DrawBM(*DXSquare->BM);

#ifdef COMLEXFOG
				VER=(VertexTnL*)DXSquare->BM->getVertexData();
				for(i=0;i<NV;i++){
					float px=(float(mapx<<(Shifter))+VER->x)/1200.0;
					float py=(float(mapy<<(Shifter-1))+VER->y)/1200.0;
					VER->u=px+px+sin(-T);
					VER->v=py+py+sin(-T*1.213);
					//VER->u2=px+px+cos(px*1.61+T*0.94);
					//VER->v2=py+py+sin(py*1.12+T*1.113);
					VER++;
				};
				DrawBM(*DXSquare->BM);
#endif

			};
		};
	};
	
};

// --- SpraySquad ---

void SpraySquad::Init(){
	memset(Cells,0,sizeof(Cells));
};

bool SpraySquad::Process(int x, int y){	
	bool empty=true;
	SprayCell* CL=Cells;
	for(int iy=0;iy<SprayNy;iy++){
		for(int ix=0;ix<SprayNx;ix++){
			if(CL->Process(x+ix,y+iy)){
				empty=false;
			}
			CL++;
		}
	}
	return !empty;
};

// --- SprayCell ---

bool SprayCell::Process(int x, int y){			
	if(a>8){
		int dx=(rand()%3)-1;
		int dy=(rand()%3)-1;
		float aa=(rand()&1)+A/10.0f;
		float aa1=(rand()&1)+A/30.0f;
		static int rv=32;
		if(aa&&(rand()&1023)<rv){
			int v=FW((x+GTm)/10,(y-GTm)/10);
			int d=1+(rand()%3);
			if(v<128){				
				SpraySys.AddDust((x+d)<<5,y<<5,r,g,b,aa);
				SpraySys.AddDust((x-d)<<5,y<<5,r,g,b,aa1);
				SpraySys.AddDust(x<<5,(y+d)<<5,r,g,b,aa);
				SpraySys.AddDust(x<<5,(y-d)<<5,r,g,b,aa);
			}else{
				SpraySys.AddDust((x-d)<<5,y<<5,r,g,b,aa1);
				SpraySys.AddDust((x+d)<<5,y<<5,r,g,b,aa);
				SpraySys.AddDust(x<<5,(y+d)<<5,r,g,b,aa);
				SpraySys.AddDust(x<<5,(y-d)<<5,r,g,b,aa);
			};
		};		
	};	
	float AA=A;		
	static int v1=11;
	float N=30.0f+FW(x,y)/10.0f;
	float DA=A/N;	
	AA-=DA/3;
	if(AA<0)AA=0;	
	A=AA;

	if(!(A>1||a>1)) return false;

	int aa=a;
	if(A>a){
		float DA=((A-a)/32.0f);		
		aa+=DA;
	}else{
		float DA=((a-A)/32.0f);		
		aa-=DA;
	}
	if(aa>=0)a=aa;
		else a=0;
	
	if(A<1&&a<1) return false;
		else return true;
}

// --- ---

void UnLoadSpraySystem(){
//	SpraySys.Free();
};

int Norma(int,int);
void AddOneDust(int x, int y){
	if(!EngSettings.DrawShotsFog)return;
	if(!SpraySys.Squads) InitSpraySystem();//SpraySys.Init();
	int xx0=x>>5;
	int yy0=y>>5;
	for(int ix=-4;ix<=4;ix++){
		for(int iy=-4;iy<=4;iy++){
			int x0=(xx0+ix)<<5;
			int y0=(yy0+iy)<<5;
			int R=Norma(x0-x,y0-y);
			int DP=120*150/(150+R*R);
			SpraySys.AddDust(x0,y0,255,255,255,DP);
		};
	};
	/*
	SpraySys.AddDust(x,y,255,255,255,25);
	
	SpraySys.AddDust(x-32,y,255,255,255,15);
	SpraySys.AddDust(x+32,y,255,255,255,15);
	SpraySys.AddDust(x,y-32,255,255,255,15);
	SpraySys.AddDust(x,y+32,255,255,255,15);
	*/

	//SpraySys.AddDust(x-32,y-32,255,255,255,25);
	//SpraySys.AddDust(x+32,y+32,255,255,255,25);
	//SpraySys.AddDust(x+32,y-32,255,255,255,25);
	//SpraySys.AddDust(x-32,y+32,255,255,255,25);
}

void DrawDust(){
	SpraySys.Draw();
}

void ProcessDust(){
	SpraySys.Process();
}
