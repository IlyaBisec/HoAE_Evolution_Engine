#include "stdheader.h"
#include "Extensions.h"
class LTest:public BaseClass{
public:
	float x0;
	float y0;
	float z0;
	float x1;
	float y1;
	float z1;
	float x2;
	float y2;
	float z2;
    void fn_test(){
        x0=rand();
    }
	SAVE(LTest);
		REG_MEMBER(_float01,x0);
		REG_MEMBER(_float01,y0);
		REG_MEMBER(_float01,z0);

		REG_MEMBER(_float01,x1);
		REG_MEMBER(_float01,y1);
		REG_MEMBER(_float01,z1);

		REG_MEMBER(_float01,x2);
		REG_MEMBER(_float01,y2);
		REG_MEMBER(_float01,z2);

        REG_METHOD(&LTest::fn_test);
	ENDSAVE;
};
LTest LTEST;

float ZV[32*32];

void RasterFuncX(int x,int y,float value,void* param){
	if(x>=0&&x<32&&y>=0&&y<=32){
		if(value>ZV[x+y*32])ZV[x+y*32]=value;
	}
}
typedef void RasterFunc(int x,int y,float value,void* param);
bool ProcessLTest(ClassEditor* CE,BaseClass* BC,int Options){
	if(Options==2){
		void TestZBuffer();
		TestZBuffer();
		void PerformRasterisation(Vector3D V1,Vector3D V2,Vector3D V3,RasterFunc* Fn,void* param);
		for(int i=0;i<32*32;i++)ZV[i]=0;

		Vector3D V1(LTEST.x0*32,LTEST.y0*32,LTEST.z0*255);
		Vector3D V2(LTEST.x1*32,LTEST.y1*32,LTEST.z1*255);
		Vector3D V3(LTEST.x2*32,LTEST.y2*32,LTEST.z2*255);

		PerformRasterisation(V1,V2,V3,RasterFuncX,NULL);

		for(int ix=0;ix<32;ix++){
			for(int iy=0;iy<32;iy++){
                float z=ZV[ix+iy*32];
				z+=(ix+iy&1)*16;
				if(z<0)z=0;
				if(z>255)z=255;
				DWORD C=int(z);
				C=0xFF000000+(C<<16)+(C<<8)+C;
				GPS.DrawFillRect(ix*16,iy*16,16,16,C);
			}
		}		
		GPS.DrawFillRect(V1.x*16-2,V1.y*16-2,4,4,0xFFFFFF00);
		GPS.DrawFillRect(V2.x*16-2,V2.y*16-2,4,4,0xFFFFFF00);
		GPS.DrawFillRect(V3.x*16-2,V3.y*16-2,4,4,0xFFFFFF00);
		GPS.FlushBatches();
	}
	return false;
}
void RegLTest(){
    return;
	AddStdEditor("LinearTest",&LTEST,"",
		RCE_RIGHT_POSITION|RCE_EXITONESCAPE|RCE_HIDEHEADER);//,ProcessLTest);
}

#include "Rasterizator.h"

void PerformRasterisationInt(Vector3D V1,Vector3D V2,Vector3D V3,RasterFunc* Fn,bool chxy,void* param){
    int y1=floorf(V1.y);
	int y2=floorf(V2.y);
	int y3=floorf(V3.y);	
	int ymin=min(y1,min(y3,y2));
	int ymax=max(y1,max(y3,y2));
	Vector3D Vt[]={V1,V2,V3,V1};
	//rasterisation of internal area
	for(int i=ymin;i<=ymax;i++){		
		float cy=float(i)+0.5f;
        float cxmin=1000000;
		float czmin;
		float cxmax=-1000000;
		float czmax;
		for(int j=0;j<3;j++){
			if((cy>=Vt[j].y&&cy<Vt[j+1].y)||(cy<=Vt[j].y&&cy>Vt[j+1].y)){
                float cx=Vt[j].x+(Vt[j+1].x-Vt[j].x)*(cy-Vt[j].y)/(Vt[j+1].y-Vt[j].y);
				float cz=Vt[j].z+(Vt[j+1].z-Vt[j].z)*(cy-Vt[j].y)/(Vt[j+1].y-Vt[j].y);
				if(cx<cxmin){
					cxmin=cx;
					czmin=cz;
				}
				if(cx>cxmax){
					cxmax=cx;
					czmax=cz;
				}
			}
		}
		if(cxmin<=cxmax){
            //found
            int imin=floorf(cxmin);
			int imax=floorf(cxmax);
			for(int k=imin;k<=imax;k++){
				float z;
				float x=float(k)+0.5;
				if(x<=imin)z=czmin;
				else if(x>=imax)z=czmax;
				else z=czmin+(x-imin)*(czmax-czmin)/(imax-imin);
				if(chxy)Fn(i,k,z,param);
				else Fn(k,i,z,param);
			}
		}
	}	
}
void PerformRasterisation(Vector3D V1,Vector3D V2,Vector3D V3,RasterFunc* Fn,void* param){
	PerformRasterisationInt(V1,V2,V3,Fn,false,param);	
	Fn(floorf(V1.x),floorf(V1.y),V1.z,param);
	Fn(floorf(V2.x),floorf(V2.y),V2.z,param);
	Fn(floorf(V3.x),floorf(V3.y),V3.z,param);
	swap(V1.x,V1.y);
	swap(V2.x,V2.y);
	swap(V3.x,V3.y);
	PerformRasterisationInt(V1,V2,V3,Fn,true,param);	
}

void Rasterizator::RasterizeInt(Vector3D V1,Vector3D V2,Vector3D V3,bool chxy){
	extern bool GecOptimRasterizator;
	if (GecOptimRasterizator) return;
	int y1=floorf(V1.y);
	int y2=floorf(V2.y);
	int y3=floorf(V3.y);	
	int ymin=min(y1,min(y3,y2));
	int ymax=max(y1,max(y3,y2));
	Vector3D Vt[]={V1,V2,V3,V1};
	//rasterisation of internal area
	for(int i=ymin;i<=ymax;i++){		
		float cy=float(i)+0.5f;
		float cxmin=1000000;
		float czmin;
		float cxmax=-1000000;
		float czmax;
		for(int j=0;j<3;j++){
			if((cy>=Vt[j].y&&cy<Vt[j+1].y)||(cy<=Vt[j].y&&cy>Vt[j+1].y)){
				float cx=Vt[j].x+(Vt[j+1].x-Vt[j].x)*(cy-Vt[j].y)/(Vt[j+1].y-Vt[j].y);
				float cz=Vt[j].z+(Vt[j+1].z-Vt[j].z)*(cy-Vt[j].y)/(Vt[j+1].y-Vt[j].y);
				if(cx<cxmin){
					cxmin=cx;
					czmin=cz;
				}
				if(cx>cxmax){
					cxmax=cx;
					czmax=cz;
				}
			}
		}
		if(cxmin<=cxmax){
			//found
			int imin=floorf(cxmin);
			int imax=floorf(cxmax);
			for(int k=imin;k<=imax;k++){
				float z;
				float x=float(k)+0.5;
				if(x<=cxmin)z=czmin;
				else if(x>=cxmax)z=czmax;
				else z=czmin+(x-cxmin)*(czmax-czmin)/(cxmax-cxmin);
				if(chxy)OnPoint(i,k,z);
				else OnPoint(k,i,z);
			}
		}
	}	
}
void Rasterizator::Rasterize(Vector3D V1,Vector3D V2,Vector3D V3){
	extern bool GecOptimRasterizator;
	if (GecOptimRasterizator) return;
	RasterizeInt(V1,V2,V3,false);	
	//OnPoint(floorf(V1.x),floorf(V1.y),V1.z);
	//OnPoint(floorf(V2.x),floorf(V2.y),V2.z);
	//OnPoint(floorf(V3.x),floorf(V3.y),V3.z);
	//swap(V1.x,V1.y);
	//swap(V2.x,V2.y);
	//swap(V3.x,V3.y);
	//RasterizeInt(V1,V2,V3,true);
}

class DirectionalZBuffer:public Rasterizator{
	int    x0;
	int    y0;
	int    x1;
	int    y1;
	int    Lx;
	int    Ly;	
	int    Np;
	float* Depth;

	Matrix4D M;
	Matrix4D Minv;
	ICamera* Camera;
	bool UseCamera;
	float Bias;

public:

	void Transform(Vector3D& V);
	void TransformInv(Vector3D& V);

	DirectionalZBuffer();
	~DirectionalZBuffer();
	virtual void OnPoint(int x,int y,float z);//for rasterizer
	void Clear();
	void AssignMatrix(const Matrix4D& M);
	void AssignCamera(ICamera* Cam);
	void AssignBias(float Bias);//distance in world 
	void CreateFromTga(ICamera* Cam,char* FileName,float Bias);
	void AddRangePoint(Vector3D V);//world coor
	void AddTriangle(Vector3D V1,Vector3D V2,Vector3D V3);//world coor
    bool CheckIfPointInside(Vector3D V);//world coor
	float GetInterpZ(float x,float y);//local coor
	float GetInterpZ0(float x,float y);//local coor
	float GetSimpleZ(float x,float y);//local coor
	float GetZ(int x,int y);//local coor
	float GetPointDest(Vector3D CurV,Vector3D& DestV);//world coor
	float GetDistanceToSurface(Vector3D CurV);//world coor
	void Draw(float Scale,DWORD Color);
};
//implementation of class DirectionalZBuffer
DirectionalZBuffer::~DirectionalZBuffer(){
	if(Depth)delete[](Depth);
	Depth=NULL;
	Np=0;            	
}
DirectionalZBuffer::DirectionalZBuffer(){
	Depth=NULL;
	Np=0;     
	Bias=0;
}
void DirectionalZBuffer::OnPoint(int x,int y,float z){
	if(Depth){
		z+=0.5;
        x-=x0;
		y-=y0;
		if(x>=0&&y>=0&&x<Lx&&y<Ly){
			int ofs=x+y*Lx;
			float zc=Depth[ofs];
			if(z>zc)Depth[ofs]=z;
		}
	}
}
void DirectionalZBuffer::Clear(){
	if(Depth)delete[](Depth);
	Np=0;
	Depth=NULL;
}
void DirectionalZBuffer::AssignMatrix(const Matrix4D& m){
	Clear();
    M=m;	
	Minv.inverse(m);
	UseCamera=false;
}
void DirectionalZBuffer::AssignCamera(ICamera* Cam){
	Camera=Cam;
	UseCamera=true;
}
void DirectionalZBuffer::Transform(Vector3D& V){
	if(UseCamera){		
        Vector4D v(V);
		Camera->ToSpace(sWorld,sScreen,v);
		V = v;
        V.z=1-V.z;
	}else{
        M.transformPt(V);
	}
}
void DirectionalZBuffer::TransformInv(Vector3D& V){
	if(UseCamera){
		Vector4D V4=V;
		V4.z=1-V4.z;
		Camera->ToSpace(sScreen,sWorld,V4);
		V=V4;
	}else{
		Minv.transformPt(V);
	}
}
void DirectionalZBuffer::AddRangePoint(Vector3D V){
	Transform(V);
	if(Np==0){		
		x0=int(floorf(V.x)-2);
		y0=int(floorf(V.y)-2);
		x1=x0+5;
		y1=y0+5;
		Np++;
	}else{
        int xx=int(floorf(V.x));
		int yy=int(floorf(V.y));
		if(xx<x0+2)x0=xx-2;
		if(yy<y0+2)y0=yy-2;
		if(xx>x1-2)x1=xx+2;
		if(yy>y1-2)y1=yy+2;
	}
	Lx=int(x1-x0+1);
	Ly=int(y1-y0+1);
}
void DirectionalZBuffer::AddTriangle(Vector3D V1,Vector3D V2,Vector3D V3){
	if(!Depth){
		int N=Lx*Ly;
		Depth=new float[N];
        for(int i=0;i<N;i++)Depth[i]=-1000000;
	}
	Transform(V1);
	Transform(V2);
	Transform(V3);
	Rasterize(V1,V2,V3);
}
bool DirectionalZBuffer::CheckIfPointInside(Vector3D V){
    Transform(V);
	return GetInterpZ(V.x,V.y)>V.z;
}
float DirectionalZBuffer::GetInterpZ(float x,float y){
	int ix=int(floorf(x));
	int iy=int(floorf(y));
	float e[3][3];
	for(int i=0;i<=2;i++){
		for(int j=0;j<=2;j++){
			e[i][j]=GetZ(ix+i-1,iy+j-1);
		}
	}
	float ec[4]={0,0,0,0};
	int   nec[4]={0,0,0,0};
	for(int i=0;i<=1;i++){
		for(int j=0;j<=1;j++){
			for(int k=0;k<=1;k++){
				for(int p=0;p<=1;p++){
					float v=e[i+k][j+p];
					if(v>-10000){
						ec[i+j*2]+=v;
						nec[i+j*2]++;
					}
				}
			}
		}
	}
	for(int i=0;i<4;i++){
		if(nec[i])ec[i]/=nec[i];
		else ec[i]=-10000;
	}	
	x-=ix;
	y-=iy;
	float zz=ec[0]+(ec[1]-ec[0])*x+(ec[2]-ec[0])*y+(ec[0]+ec[3]-ec[2]-ec[1])*x*y;
	return zz;	
}
float DirectionalZBuffer::GetInterpZ0(float x,float y){
    float x0=x;
	float y0=y;
	x-=0.5f;
	y-=0.5f;
    float fx=floorf(x);
	float fy=floorf(y);
	x-=fx;
	y-=fy;
	int ix=int(fx);
	int iy=int(fy);
	float z1=GetZ(ix,iy);
	float z2=GetZ(ix+1,iy);
	float z3=GetZ(ix,iy+1);
	float z4=GetZ(ix+1,iy+1);
	return z1+(z2-z1)*x+(z3-z1)*y+(z1+z4-z3-z2)*x*y;
}
float DirectionalZBuffer::GetSimpleZ(float x,float y){
	float fx=floorf(x);
	float fy=floorf(y);
	int ix=int(fx);
	int iy=int(fy);
	return GetZ(ix,iy);
}
float DirectionalZBuffer::GetZ(int x,int y){
	x-=x0;
	y-=y0;
	if(Depth&&x>=0&&y>=0&&x<Lx&&y<Ly)return Depth[x+y*Lx];
	return -1000000;
}
float DirectionalZBuffer::GetPointDest(Vector3D CurV,Vector3D& DestV){
	Transform(CurV);
	float zz=GetInterpZ(CurV.x,CurV.y);
	float dz=fabs(zz-CurV.z);
	if(dz>=4){
        DestV=CurV;
		return 0;
	}
	CurV.z=zz;
	TransformInv(CurV);
	DestV=CurV;
	return 100*exp(-dz*dz*2);
}
float DirectionalZBuffer::GetDistanceToSurface(Vector3D CurV){
	Transform(CurV);
	float zz=GetInterpZ(CurV.x,CurV.y);
	float dz=zz-CurV.z;
	return dz;
}
void DirectionalZBuffer::Draw(float Scale,DWORD Color){
	if(Depth){
		for(int i=0;i<Lx;i++){
			for(int j=0;j<Lx;j++){
				float cx=x0+i+0.5f;
				float cy=y0+j+0.5f;
				float Z0=GetSimpleZ(cx-0.45,cy-0.45);
				float Z1=GetSimpleZ(cx+0.45,cy-0.45);
				float Z2=GetSimpleZ(cx-0.45,cy+0.45);
				float Z3=GetSimpleZ(cx+0.45,cy+0.45);
				Vector3D V1(cx-0.45,cy-0.45,Z0);
				Vector3D V2(cx+0.45,cy-0.45,Z1);
				Vector3D V3(cx-0.45,cy+0.45,Z2);
				Vector3D V4(cx+0.45,cy+0.45,Z3);

				TransformInv(V1);
				TransformInv(V2);
				TransformInv(V3);
				TransformInv(V4);

				V1*=Scale;
				V2*=Scale;
				V3*=Scale;
				V4*=Scale;

				GPS.DrawLine(V1,V2,Color);
				GPS.DrawLine(V2,V4,Color);
				GPS.DrawLine(V4,V3,Color);
				GPS.DrawLine(V3,V1,Color);
				GPS.DrawLine(V1,V4,Color);
				GPS.DrawLine(V2,V3,Color);
			}
		}
	}
	//GPS.FlushBatches();
};
void TestZBuffer(){
	DirectionalZBuffer ZB;
	DirectionalZBuffer ZB1;
	Matrix4D M=Matrix4D::identity;
	Matrix4D M1=M;
	M1.flipYZ();
	Vector3D V1(5,8,2);
	Vector3D V2(10,9,1);
	Vector3D V3(3,14,0);
	Vector3D V4(12,14,3);
	ZB.AssignMatrix(M);
    ZB.AddRangePoint(V1);
	ZB.AddRangePoint(V2);
	ZB.AddRangePoint(V3);
	ZB.AddRangePoint(V4);
	ZB.AddTriangle(V1,V2,V3);
	ZB.AddTriangle(V2,V3,V4);

	ZB1.AssignMatrix(M1);
	ZB1.AddRangePoint(V1);
	ZB1.AddRangePoint(V2);
	ZB1.AddRangePoint(V3);
	ZB1.AddRangePoint(V4);
	ZB1.AddTriangle(V1,V2,V3);
	ZB1.AddTriangle(V2,V3,V4);

	BaseMesh BM;	
	KeyPressed=false;	
	do{
		/*
		BM.createPatch(100,100,Vector3D(0,0,0),Vector3D(1,0,0),Vector3D(0,1,0));
		BM.setShader(IRS->GetShaderID("Wire"));
		Vertex2t* V=(Vertex2t*)BM.getVertexData();
		int N=BM.getNVert();
		for(int i=0;i<N;i++){
			V[i].x*=20;
			V[i].y*=-20;
			V[i].z*=20;
			if(GetKeyState(VK_MENU)&0x8000){
				V[i].z=ZB.GetInterpZ0(V[i].x,V[i].y);
			}else{			
				V[i].z=ZB.GetInterpZ(V[i].x,V[i].y);
			}
			V[i].x*=100;
			V[i].y*=100;
			V[i].z*=100;
			if(V[i].z<0)V[i].z=0;
		}
		*/
        ProcessMessages();
		IRS->ClearDevice();
		ZB.Draw(100,0xFFFF0000);
		ZB1.Draw(100,0xFF00FF00);
		void GlobalHandleMouse();
		GlobalHandleMouse();
		void SetupCamera();
		SetupCamera();
		FlipPages();
	}while(LastKey!=27);
}
//intervace 
