#include "stdheader.h"
#include "Fractal3d.h"
//#include "StdAfx.h"
#define MLX 1024
#define MMSK 1023
#define MSHF 10
short fMap[MLX*MLX];
int mrpos2=20;
int XXP2=0;
extern short randoma[8192];
int mrand2(){
	mrpos2++;
	if(mrpos2>8191)XXP2+=0x3571;
	mrpos2&=8191;
	return (randoma[mrpos2]^XXP2)&32767;
};
int GetRand(int amp){
	return ((mrand2()*amp)>>14)-amp;
};
int GetfMap(int x,int y){
	x&=MMSK;
	y&=MMSK;
	return fMap[x+(y<<MSHF)];
};
void SetfMap(int x,int y,int val){
	if(x<0||y<0||x>=MLX||y>=MLX)return;
	fMap[x+(y<<MSHF)]=short(val);
};
void Generate(int N){
	int LX=MLX>>N;
	short A0=2048;
	int x,y;
	for(x=0;x<MLX;x+=LX){
		for(y=0;y<MLX;y+=LX){
			SetfMap(x,y,GetRand(A0));
		};
	};
	while(LX>1){
		///A0>>=1;
		A0=(A0*9)/20;
		for(x=0;x<MLX;x+=LX){
			for(y=0;y<MLX;y+=LX){
				int L1=LX>>1;
				SetfMap(x+L1,y,GetRand(A0)+((GetfMap(x,y)+GetfMap(x+LX,y))>>1));
				SetfMap(x+L1,y+LX,GetRand(A0)+((GetfMap(x,y+LX)+GetfMap(x+LX,y+LX))>>1));
				SetfMap(x,y+L1,GetRand(A0)+((GetfMap(x,y)+GetfMap(x,y+LX))>>1));
				SetfMap(x+LX,y+L1,GetRand(A0)+((GetfMap(x+LX,y)+GetfMap(x+LX,y+LX))>>1));
				SetfMap(x+L1,y+L1,GetRand(A0)+((GetfMap(x+L1,y)+GetfMap(x+L1,y+LX)+GetfMap(x,y+L1)+GetfMap(x+LX,y+L1))>>2));
			};
		};
		LX>>=1;
	};
	int fmin=100000;
	int fmax=-100000;
	for(x=0;x<MLX;x+=LX){
		for(y=0;y<MLX;y+=LX){
			int f=GetfMap(x,y);
			if(f<fmin)fmin=f;
			if(f>fmax)fmax=f;
		};
	};
	int dx=fmax-fmin;
	for(x=0;x<MLX;x+=LX){
		for(y=0;y<MLX;y+=LX){
			int f=(int(GetfMap(x,y)-fmin)<<9)/dx;
			SetfMap(x,y,(int(GetfMap(x,y)-fmin)<<9)/dx);
		};
	};
	//soft
	for(x=0;x<MLX;x+=LX){
		for(y=0;y<MLX;y+=LX){
			SetfMap(x,y,(GetfMap(x+1,y)+GetfMap(x-1,y)+GetfMap(x,y+1)+GetfMap(x,y-1)+GetfMap(x,y))/5);
		};
	};
};
#define scale 32
bool FGenerated=0;
void addmr1(int v,char* s,int L);
#define addmr(v) addmr1(v,__FILE__,__LINE__)
int GetFractalVal(int x,int y){
	if(!FGenerated){
		FGenerated=1;
		void CreateFractal();
		CreateFractal();
	}
	int x0=(x/scale)&MMSK;
	int y0=(y/scale)&MMSK;
	int x1=(x0+1)&MMSK;
	int y1=(y0+1)&MMSK;
	int dx=x%scale;
	int dy=y%scale;
	int v0=GetfMap(x0,y0);
	int v1=GetfMap(x1,y0);
	int v2=GetfMap(x0,y1);
	int v3=GetfMap(x1,y1);
    addmr(v0);
	addmr(v1);
	addmr(v2);
	addmr(v3);
	return v0+(v1-v0)*dx/scale+(v2-v0)*dy/scale+((v3+v0-v1-v2)*dx/scale)*dy/scale;
};
int GetFractalValEx(int x,int y,int Type){
	int V=GetFractalVal((x+y)*3/4,(x-y)*3/4);
	switch(Type){
		case 0:
			return V;
		case 1://Strips
			V<<=1;
			if(V>512)V=1024-V;
			return V;
		case 2://Combo
			{
				int V1=GetFractalVal((x+y)*3/4+11329,(x-y)*3/4+13799)<<1;
				if(V1>512)V1=1024-V1;
				return V*V1/512;
			}
		case 3://Combo2
			{
				V<<=1;
				int V1=GetFractalVal((x+y)*3/8+11929,(x-y)*3/8+19799);
				if(V>512)V=1024-V;
				return V*V1/512;
			}
		case 4://Circles
			V=V*3-256*3;
			if(V<0)V=-V;
			if(V>512)V=1024-V;
			return V;
		case 5://Isolines2
			V<<=2;
			if(V<512)return V;
			if(V<1024)return 1024-V;
			if(V<1024+512)return V-1024;
			return 2048-V;
		case 6://Isolines3
			V*=6;
			if(V<512)return V;
			if(V<1024)return 1024-V;
			if(V<1024+512)return V-1024;
			if(V<2048)return 2048-V;
			if(V<2048+512)return V-2048;
			return 2048+1024-V;
		case 7://Power2
			return V*V/512;
		case 8://Power3
			return (V*V/4)*V/512/128;
	}

	return 0;
};
void CreateFractal(){
	mrpos2=20;
	XXP2=0;
	Generate(6);	
};
#define max(a,b) (a)>(b)?a:b
#define min(a,b) (a)<(b)?a:b

extern int a2fVertexOffset[8][3];
extern int a2iEdgeConnection[12][2];
int a2iFaces[6][4]={{0,1,2,3},{4,5,6,7},{0,1,5,4},{1,2,6,5},{2,3,7,6},{3,0,4,7}};
typedef unsigned char byte;
void SaveToBMP24(char* Name,int Lx,int Ly,byte* data);

void Fractal3D::CreateEmptyFractal(){
	if(fHeight)delete[]fHeight;
	fHeight=new float[SizeX*SizeX*SizeX];
	for(int x=0;x<SizeX;x++){
		for(int y=0;y<SizeX;y++){
			for(int z=0;z<SizeX;z++){                    
				SetF3D(x,y,z,0);
			}
		}
	}
}
void Fractal3D::SetSize(int L){
	SizeX=L;
	CreateEmptyFractal();
}
void Fractal3D::Create3DFractal(){		
	if(fHeight)delete[]fHeight;
	fHeight=new float[SizeX*SizeX*SizeX];
	int nstart=4;
	int step=SizeX/nstart;		
	for(int x=0;x<SizeX;x++){
		for(int y=0;y<SizeX;y++){
			for(int z=0;z<SizeX;z++){                    
				SetF3D(x,y,z,float(rand())/32768);
			}
		}
	}
	float A0=2;	
	step>>=1;
	while(step>1){				
		A0*=0.5f;
		int d=step/2;
		for(int x=0;x<SizeX;x+=step){
			for(int y=0;y<SizeX;y+=step){
				for(int z=0;z<SizeX;z+=step){
					//edges lookup
					for(int i=0;i<12;i++){
						int v0=a2iEdgeConnection[i][0];
						int v1=a2iEdgeConnection[i][1];

						int x0=a2fVertexOffset[v0][0];
						int y0=a2fVertexOffset[v0][1];
						int z0=a2fVertexOffset[v0][2];

						int x1=a2fVertexOffset[v1][0];
						int y1=a2fVertexOffset[v1][1];
						int z1=a2fVertexOffset[v1][2];

						SetF3D(x+(x0+x1)*d,y+(y0+y1)*d,z+(z0+z1)*d
							,A0*(float(rand())/32768.0f-0.5f)
							+(GetF3D(x+x0*step,y+y0*step,z+z0*step)
							+GetF3D(x+x1*step,y+y1*step,z+z1*step))/2.0f);
					}
					//faces lookup
					for(int i=0;i<6;i++){
						int v0=a2iFaces[i][0];
						int v1=a2iFaces[i][1];
						int v2=a2iFaces[i][2];
						int v3=a2iFaces[i][3];

						int x0=a2fVertexOffset[v0][0];
						int y0=a2fVertexOffset[v0][1];
						int z0=a2fVertexOffset[v0][2];

						int x1=a2fVertexOffset[v1][0];
						int y1=a2fVertexOffset[v1][1];
						int z1=a2fVertexOffset[v1][2];

						int x2=a2fVertexOffset[v2][0];
						int y2=a2fVertexOffset[v2][1];
						int z2=a2fVertexOffset[v2][2];

						int x3=a2fVertexOffset[v3][0];
						int y3=a2fVertexOffset[v3][1];
						int z3=a2fVertexOffset[v3][2];

						SetF3D(x+(x0+x1+x2+x3)*d/2,y+(y0+y1+y2+y3)*d/2,z+(z0+z1+z2+z3)*d/2
							,A0*(float(rand())/32768.0f-0.5f)
							+(GetF3D(x+x0*step,y+y0*step,z+z0*step)
							+GetF3D(x+x1*step,y+y1*step,z+z1*step)
							+GetF3D(x+x2*step,y+y2*step,z+z2*step)
							+GetF3D(x+x3*step,y+y3*step,z+z3*step))/4.0f);                            
					}
					//central point
					float vc=0;
					for(int i=0;i<8;i++){
						int x0=a2fVertexOffset[i][0];
						int y0=a2fVertexOffset[i][1];
						int z0=a2fVertexOffset[i][2];
						vc+=GetF3D(x+x0*step,y+y0*step,z+z0*step);
					}
					SetF3D(x+d,y+d,z+d,vc/8.0f+A0*(float(rand())/32768.0f-0.5f));
				}
			}
		}		
		step>>=1;
	}		
}
void Fractal3D::SaveFractalUVA(char* bmp24_name){
	byte* rgb=new byte[SizeX*SizeX*SizeX*3];
	float maxuv=0;
	float maxa=0;
	float maxH=0;
	float minH=10000;
	float _min=10000;
	for(int x=0;x<SizeX;x++){
		for(int y=0;y<SizeX;y++){
			for(int z=0;z<SizeX;z++){
				float du=fabs((GetF3D(x+1,y,z)-GetF3D(x-1,y,z)));
				float dv=fabs((GetF3D(x,y+1,z)-GetF3D(x,y-1,z)));
				float da=dv;
				float h=GetF3D(x,y,z);
				du=max(du,dv);
				maxuv=max(maxuv,du);
				maxa=max(maxa,da);
				maxH=max(maxH,h);
				minH=min(minH,h);
			}
		}
	}
	for(int x=0;x<SizeX;x++){
		for(int y=0;y<SizeX;y++){
			for(int z=0;z<SizeX;z++){
				float du=128+(GetF3D(x+1,y,z)-GetF3D(x-1,y,z))*120/maxuv;
				float dv=128+(GetF3D(x,y+1,z)-GetF3D(x,y-1,z))*120/maxuv;
				float da=190+(GetF3D(x,y,z+1)-GetF3D(x,y,z-1))*300/maxa;
				if(da<0)da=0;
				if(da>255)da=255;
				int ofs=(x+y*SizeX*SizeX+z*SizeX)*3;
				float h=(GetF3D(x,y,z)-minH)*250/(maxH-minH);
				rgb[ofs  ]=byte(da);
				rgb[ofs+1]=byte(du);
				rgb[ofs+2]=byte(dv);

				//rgb[ofs  ]=byte(h);
				//rgb[ofs+1]=byte(h);
				//rgb[ofs+2]=byte(h);
			}
		}
	}
	SaveToBMP24(bmp24_name,SizeX*SizeX,SizeX,rgb);
	delete[]rgb;
}
void Fractal3D::SaveHMap(char* bmp24_name){
	byte* rgb=new byte[SizeX*SizeX*SizeX*3];
	for(int x=0;x<SizeX;x++){
		for(int y=0;y<SizeX;y++){
			for(int z=0;z<SizeX;z++){
				float h=GetF3D(x,y,z);
				if(h<0)h=0;
				if(h>255)h=255;
				int ofs=(x+y*SizeX*SizeX+z*SizeX)*3;
				rgb[ofs  ]=byte(h);
				rgb[ofs+1]=byte(h);
				rgb[ofs+2]=byte(h);
			}
		}
	}
	SaveToBMP24(bmp24_name,SizeX*SizeX,SizeX,rgb);
	delete[]rgb;	
}
float GetPointToLineDist(Vector3D p1,Vector3D p2,Vector3D p){
	p2-=p1;
	p-=p1;
	float L2=p2.norm();
	float LP=p.norm();
	float sp=p.dot(p2);
	if(sp<=0)return LP;
	if(sp>=L2*L2){
		p-=p2;
		return p.norm();
	}
	Vector3D C=p2;
	C*=sp/L2/L2;
	p-=C;
	return p.norm();
}
void Fractal3D::Add3DLine(Vector3D p1,Vector3D p2,float R,float Val){
    int x0=tmin(p1.x-R-1,p2.x-R-1);
	int y0=tmin(p1.y-R-1,p2.y-R-1);
	int z0=tmin(p1.z-R-1,p2.z-R-1);

	int x1=tmax(p1.x+R+1,p2.x+R+1);
	int y1=tmax(p1.y+R+1,p2.y+R+1);
	int z1=tmax(p1.z+R+1,p2.z+R+1);

	for(int x=x0;x<=x1;x++){
		for(int y=y0;y<=y1;y++){
			for(int z=z0;z<=z1;z++){
				float v=GetF3D(x,y,z);
                float r=GetPointToLineDist(p1,p2,Vector3D(x,y,z));
				if(r<R){
					float V=Val*(1-r*r/R/R);
					SetF3D(x,y,z,tmax(V,v));
				}
			}
		}
	}
}
float Fractal3D::Check3DLine(Vector3D p1,Vector3D p2,float R){
	int x0=tmin(p1.x-R-1,p2.x-R-1);
	int y0=tmin(p1.y-R-1,p2.y-R-1);
	int z0=tmin(p1.z-R-1,p2.z-R-1);

	int x1=tmax(p1.x+R+1,p2.x+R+1);
	int y1=tmax(p1.y+R+1,p2.y+R+1);
	int z1=tmax(p1.z+R+1,p2.z+R+1);
	float fsumm=0;
	float fn=0;
	for(int x=x0;x<=x1;x++){
		for(int y=y0;y<=y1;y++){
			for(int z=z0;z<=z1;z++){
				float v=GetF3D(x,y,z);
				float r=GetPointToLineDist(p1,p2,Vector3D(x,y,z));
				if(r<R){
					float w=(1.0f-r*r*r/R/R/R);
                    fsumm+=v*w;
					fn+=w;
				}
			}
		}
	}
	return fsumm/fn;
}
float Fractal3D::GetAverageDensity(){
	float summ=0;
	int n=0;
	for(int x=0;x<SizeX;x++){
		for(int y=0;y<SizeX;y++){
			for(int z=0;z<SizeX;z++){
				summ+=GetF3D(x,y,z);
				n++;
			}
		}
	}
	return summ/n;
}
void Fractal3D::CreateMassLines(float Density,float Length,float R){
	int n=0;
	do{
		for(int j=0;j<20;j++){
			Vector3D V1,V2;
			float w=10000;
			for(int i=0;i<20;i++){
				Vector3D V(float(rand())*SizeX/32768.0,float(rand())*SizeX/32768.0,float(rand())*SizeX/32768.0);
				Vector3D N(rand()-16384,rand()-16384,rand()*3);
				N.normalize();
				N*=Length;
				N+=V;
				float f=Check3DLine(V,N,R);
				if(f<w){
					w=f;
					V1=V;
					V2=N;
				}
			}
			Add3DLine(V1,V2,R,255);
		}
		float d=GetAverageDensity();
		if(d>=Density || ++n>1000)break;
	}while(true);
}\
Fractal3D fr3d;
void do3D(){
	//fr3d.Create3DFractal();
	//fr3d.SaveFractalUVA("Textures\\Waves.bmp");

	//fr3d.SetSize(32);
	//fr3d.CreateMassLines(96,20,1.0);
	//fr3d.SaveHMap("textures\\lines3d.bmp");
}