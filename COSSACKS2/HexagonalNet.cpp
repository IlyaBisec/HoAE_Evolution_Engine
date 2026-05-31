#include "stdheader.h"
#include "ITerrain.h"
extern IRenderSystem*	IRS;
#define Vertex Vertex2t
#define vformat vfVertex2t
//template <class Vertex,VertexFormat vformat>
class VKey{
	public:
		int x,y;
		int vx,vy;
		unsigned int	hash	() const {return x+(y*1779)/*+(vx*17991)+(vy*319931)*/;}
		bool			equal	( const VKey& el ){return x==el.x&&y==el.y/*&&vx==el.vx&&vy==el.vy*/;}
		bool			operator ==( const VKey& el ){return x==el.x&&y==el.y/*&&vx==el.vx&&vy==el.vy*/;}
		void			copy	( const VKey& el ){x=el.x;y=el.y/*;vx=el.vx;vy=el.vy*/;}
	};
class HexMesh{
	PointerHash<Vertex,VKey> PHash;
	PointerHash<Vertex,VKey> THash;
public:
	BaseMesh* Mesh;
	int MaxVert;
	int LinkSize;
	void Clear(){
		PHash.reset();
		THash.reset();
	}
	void Setup(int MaxVertices,int LSize){
		LinkSize=LSize;
		if(MaxVertices!=MaxVert||!Mesh){
			if(Mesh)delete(Mesh);
			Mesh=new BaseMesh;
			Mesh->create( MaxVertices,MaxVertices*6,vformat);
			MaxVert=MaxVertices;
		}
		Mesh->setNVert(0);
		Mesh->setNInd(0);
		Mesh->setNPri(0);
	}
	bool AddVertex(Vertex* V){
		int NV;
		if(Mesh&&(NV=Mesh->getNVert())<MaxVert){
			VKey VK;
			int x0=int(V->x);
			int y0=int(V->y);
			int indices[6];
			static int dxy[]={-2,-1,0,-2,2,-1,2,1,0,2,-2,1};
			Vertex* Ventry=(Vertex*)Mesh->getVertexData();
			VK.x=int(V->x);
			VK.y=int(V->y);
			//VK.vx=0;//int(V->u*100.0f);
			//VK.vy=0;//int(V->v*100.0f);

			int idxp=PHash.find(VK);
			int idx0=-1;
			if(idxp!=NO_ELEMENT){
				Vertex* VP=Ventry+idxp;
				idx0=idxp;
				VP->diffuse=V->diffuse;
			}
			if(idx0==-1){
				Vertex* V0=Ventry+NV;
				memcpy(V0,V,sizeof Vertex);
				NV++;
				idx0=PHash.add(VK,V0);
			}
			for(int j=0;j<6;j++){
				VK.x=x0+((dxy[j+j  ]*LinkSize)>>1);
				VK.y=y0+((dxy[j+j+1]*LinkSize)>>1);
				//VK.vx=0;//int(V->u*100.0f);
				//VK.vy=0;//int(V->v*100.0f);
				int eIdx=PHash.find(VK);
				bool NeedAdd=0;
				if(eIdx==NO_ELEMENT){
					Vertex* V1=Ventry+NV;
					eIdx=NV;
					PHash.add(VK,V1);
					NV++;
					*V1=*V;
					//V1->diffuse&=0x00FFFFFF;
					V1->x=(float)VK.x;
					V1->y=(float)VK.y;
				}
				indices[j]=eIdx;
			}
			Mesh->setNVert(NV);
			int NIDX=Mesh->getNInd();
			word* idxs=Mesh->getIndices()+NIDX;
			for(j=0;j<6;j++){
				int idx1=indices[j];
				int idx2=indices[(j+1)%6];
				/*Vertex* V0=Ventry+idx0;
				Vertex* V1=Ventry+idx1;
				Vertex* V2=Ventry+idx2;
				int xcc=int((V0->x+V1->x+V2->x))/3;
				int ycc=int((V0->y+V1->y+V2->y))/3;
				VKey VC;
				VC.x=xcc;
				VC.y=ycc;
				VC.vx=VK.vx;
				VC.vy=VK.vy;
				int tidx=THash.find(VC);
				if(tidx==NO_ELEMENT){*/
                    idxs[0]=idx0;
					idxs[1]=idx1;
					idxs[2]=idx2;
					idxs+=3;
					NIDX+=3;
                    //THash.add(VC,NULL);
				//}
			}
			Mesh->setNInd(NIDX);
			Mesh->setNPri(NIDX/3);
			return true;
        }else return false;
	}
	bool AddVertexOld(Vertex* V){
		int NV;
		if(Mesh&&(NV=Mesh->getNVert())<MaxVert){
			VKey VK;
			int x0=int(V->x);
			int y0=int(V->y);
			int indices[6];
			int dxy[]={-2,-1,0,-2,2,-1,2,1,0,2,-2,1};
			Vertex* Ventry=(Vertex*)Mesh->getVertexData();
			VK.x=int(V->x);
			VK.y=int(V->y);
			VK.vx=int(V->u*100.0f);
			VK.vy=int(V->v*100.0f);

			int idxp=PHash.find(VK);
			int idx0=-1;
			if(idxp!=NO_ELEMENT){
				Vertex* VP=Ventry+idxp;
				idx0=idxp;
				VP->diffuse=V->diffuse;
			}
			if(idx0==-1){
				Vertex* V0=Ventry+NV;
				memcpy(V0,V,sizeof Vertex);
				NV++;
				idx0=PHash.add(VK,V0);
			}
			for(int j=0;j<6;j++){
				VK.x=x0+((dxy[j+j  ]*LinkSize)>>1);
				VK.y=y0+((dxy[j+j+1]*LinkSize)>>1);
				VK.vx=int(V->u*100.0f);
				VK.vy=int(V->v*100.0f);
				int eIdx=PHash.find(VK);
				bool NeedAdd=0;
				if(eIdx==NO_ELEMENT){
					Vertex* V1=Ventry+NV;
					eIdx=NV;
					PHash.add(VK,V1);
					NV++;
					*V1=*V;
					//V1->diffuse&=0x00FFFFFF;
					V1->x=(float)VK.x;
					V1->y=(float)VK.y;
				}
				indices[j]=eIdx;
			}
			Mesh->setNVert(NV);
			int NIDX=Mesh->getNInd();
			word* idxs=Mesh->getIndices()+NIDX;
			for(j=0;j<6;j++){
				int idx1=indices[j];
				int idx2=indices[(j+1)%6];
				Vertex* V0=Ventry+idx0;
				Vertex* V1=Ventry+idx1;
				Vertex* V2=Ventry+idx2;
				int xcc=int((V0->x+V1->x+V2->x))/3;
				int ycc=int((V0->y+V1->y+V2->y))/3;
				VKey VC;
				VC.x=xcc;
				VC.y=ycc;
				VC.vx=VK.vx;
				VC.vy=VK.vy;
				int tidx=THash.find(VC);
				if(tidx==NO_ELEMENT){
					idxs[0]=idx0;
					idxs[1]=idx1;
					idxs[2]=idx2;
					idxs+=3;
					NIDX+=3;
					THash.add(VC,NULL);
				}

			}
			Mesh->setNInd(NIDX);
			Mesh->setNPri(NIDX/3);
			return true;
		}else return false;
	}
	HexMesh(){
		Mesh=NULL;
	}
};
int GetWDeep(int x,int y,void* param);
class WaterHexNet{
public:

	float       CurMapX;
	float       CurMapY;
    Vector3D    CurLookDir;

	float* Vx;
	float* Vy;
	int NVert;
	int MaxVert;

	HexMesh HMesh;
	void CreateMesh(int mapx,int mapy);
	void CheckMesh(int mapx,int mapy);
	void Draw();
	WaterHexNet();
	~WaterHexNet();
};
WaterHexNet::WaterHexNet(){
	CurMapX=-1;
	CurMapY=-1;
    CurLookDir = Vector3D( 0.0f, -cos( c_PI/6.0f ), -sin( c_PI/6.0f ) );
	Vx=NULL;
	Vy=NULL;
	NVert=0;
	MaxVert=0;
}
WaterHexNet::~WaterHexNet(){
}
int CLRS[]={0xFFFFFF,0xFF0000,0x00FF00,0x0000FF};
int GetHeight(int,int);
#define QSize 0.001f
float QuantUV(float x){
	return float(int(((x+QSize)/(QSize*2.0f))))*QSize*2.0f;
}
void GetFlowSpeed(int x,int y,float& vx,float& vy);

int GetAlphaDistorted(int x,int y);

void WaterHexNet::CreateMesh(int mx,int my){
	HMesh.Clear();
	int LSIZE=16<<(5-Shifter);
	int LSHIFT=5;//10-Shifter;
	HMesh.Setup(16384*2,LSIZE<<1);
	
	int x1=mx+smaplx + 8;
	int y1=my+smaply + 8;

	const Vector3D* V3 = GetCameraIntersectionCorners();
	for(int i=0;i<4;i++)
    {
		int xx = V3[i].x/32;
		int yy = V3[i].y/32;
		if(xx<mx)mx=xx;
		if(xx>x1)x1=xx;
		if(yy<my)my=yy;
		if(yy>y1)y1=yy;
	}
	my-=2;
	y1+=4;

	int NN=0;
	int Step=2<<(5-Shifter);
	int DD=16+48<<(5-Shifter);
	Vector3D pos=ICam->GetPosition();
	Vector3D dir=ICam->GetDir();
	float t = -pos.z/dir.z;
	Vector3D VC(pos);
	VC.addWeighted( dir, t );
	for(int ix=mx-1;ix<x1;ix+=Step){
		int vv=0;
		for(int iy=(my-1)*4;iy<y1*4;iy+=Step*3){
			vv++;
			int xr=(ix<<LSHIFT)+(vv&1?0:LSIZE*2);
			int yr=(iy<<LSHIFT)/4;
			int D=GetAlphaDistorted(xr,yr);
			if(D<=0){
                int H1=GetHeight(xr-DD,yr-DD/2);
				if(H1>0){
					int H2=GetHeight(xr+DD,yr-DD/2);
					if(H2>0){
						int H3=GetHeight(xr,yr+DD);
						if(H3<0)D=1;
					}else D=1;
				}else D=1;
			}
			//if(D<0)D=0;
			//if(D>180)D=180;
			if(D>0){
				if(D<0)D=0;
				Vertex V;
				V.x=(float)xr;
				V.y=(float)yr;
				D=((EngSettings.WaterColorModulator>>24)*D)>>8;				
				if(D>255)D=255;
				V.diffuse=0x00FFFF00+(D<<24);//0x00FFFFFF
				int GetWaterLevel(int x,int y);
				V.z=0;//GetWaterLevel(int(V.x),int(V.y));
				//GetFlowSpeed(int(V.x),int(V.y),V.u,V.v);
				V.u=0;//QuantUV(V.u);//x speed
				V.v=0;//QuantUV(V.v);//y speed
				HMesh.AddVertex(&V);
				NN++;
			}
		}
	}
	if((NVert=HMesh.Mesh->getNVert())>MaxVert){
		MaxVert=HMesh.Mesh->getNVert()+1024;
        Vx=(float*)realloc(Vx,MaxVert*4);
		Vy=(float*)realloc(Vy,MaxVert*4);
	}
	Vertex* V=(Vertex*)HMesh.Mesh->getVertexData();
	for(int i=0;i<NVert;i++){
		Vx[i]=V->u;
		Vy[i]=V->v;
		int D=GetAlphaDistorted(V->x,V->y);
		D=((EngSettings.WaterColorModulator>>24)*D)>>8;
		if(D>255)D=255;
		DWORD DC=255;
		if(V->y>VC.y-700){
			int DD=DC*(V->y-VC.y+700)/1400/2;
			if(DD>D/2)DD=D/2;
			DC-=DD/2;
		}
		DC=DC+(DC<<8)+(DC<<16);
		V->diffuse=(D<<24)|DC;//(V->diffuse&0x00FFFFFF);
		V++;
	}
}

#define WS_Size 16
#define WS_ASize (WS_Size*WS_Size)
#define WS_Mask 15
#define WS_Shift 4
extern int RealLx;
extern int RealLy;
extern float* CurWShape;
int GT00=0;

extern float fMapX, fMapY;
void WaterHexNet::CheckMesh(int mapx,int mapy){
    Vector3D lookDir=ICam->GetDir();
	if(fMapX!=CurMapX||fMapY!=CurMapY||!CurLookDir.isEqual(lookDir)){
        CreateMesh(mapx,mapy);
	}
	if(GT00==0)GT00=GetTickCount();
	CurMapX     = fMapX;
	CurMapY     = fMapY;
    CurLookDir  = lookDir;
	int NV=HMesh.Mesh->getNVert();
	Vertex* VER=(Vertex*)HMesh.Mesh->getVertexData();
	float T=(GetTickCount()-GT00)/9000.0;

	Matrix4D reflTM = IRMap ? IRMap->GetReflectionTexTM() : Matrix4D::identity;
	
	float DD=float(8<<(5-Shifter));
	int Mstep=1<<(5-Shifter);
	float fstep=float(Mstep);
	for(int i=0;i<NV;i++){
		float PX0=float(mapx<<5);
		float PY0=float(mapy<<5);
		float px=VER->x;
		float py=VER->y;
		float px0=px/200.0;
		float py0=py/200.0;

		int wx=int(px/32)&WS_Mask;
		int wy=int(py/32)&WS_Mask;
		int wof0=wx+(wy<<WS_Shift);
		float W0=CurWShape[wx+(wy<<WS_Shift)];
		float WL=CurWShape[((wx-1)&WS_Mask)+(wy<<WS_Shift)];
		float WR=CurWShape[((wx+1)&WS_Mask)+(wy<<WS_Shift)];
		float WU=CurWShape[wx+(((wy-1)&WS_Mask)<<WS_Shift)];
		float WD=CurWShape[wx+(((wy+1)&WS_Mask)<<WS_Shift)];

		Vector4D uv( VER->x, VER->y, VER->z, 1.0f );
		WorldToCameraSpace( uv );
		uv *= reflTM;
		uv.normW();
		VER->u2 = uv.x;
		VER->v2 = uv.y;

		/*VER->u2=(VER->x-PX0)/RealLx/fstep+float(WL-WR)*0.005;
		VER->v2=(VER->y-PY0)/RealLy/fstep/2.0+float(WU-WD)*0.005;*/

		/*Vector4D uv(VER->u2, VER->v2, 0.0f, 1.0f);
		reflTM.transformPt( uv );
		uv.normW();

		VER->u2 = uv.x;
		VER->v2	= uv.y;*/

		//VER->y-=W0*3;
		//VER->z=0;
		VER->u=px/256.0+(Vx[i]*T);
		VER->v=py/400.0+(Vy[i]*T);
		//if(!(VER->diffuse>>24))VER->diffuse|=0x80000000;
		VER++;
	}
}
WaterHexNet* WHNet=NULL;

void RebuildWaterMesh()
{
    if (WHNet) WHNet->CreateMesh(mapx,mapy);
}

void CreateWPalette();
extern int WaterTexSysPS;
extern int WaterTexDefPS;
#define WaveLx 260
#define WaveLy 260
inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
class ReflectionImage{
public:
	BaseMesh* Sky;
	BaseMesh* Surf;
	int TargetID;
	int TargetSize;
	int PrevMx;
	int PrevMy;
	ReflectionImage();
	~ReflectionImage();
	void CreateImage(int mx,int my);
};
extern ReflectionImage RIMG;
#define HMULT 4
int GetAlpha(int x,int y){
	int H=GetHeight(x,y)*HMULT;
	if(H>64)H=64;
	H=64-H;
	//if(H>230)H=230;
	//if(H<0)H=0;
	return H;
}
int GetVertexDarkness(int V){
	int vx=V%VertInLine;
	int vy=((V/VertInLine)<<5)-(vx&1?16:0);
	vx=(vx<<5);
	int D=GetAlpha(vx,vy);
	if(D<32)D=0;
	if(D>64)D=64;
	return D;
}
int GetDeepDistorted(int x,int y){
	int H;
	int y0=y;
	int H0=GetHeight(x,y);//(GetTotalHeight(x,y)+GetTotalHeight(x-40,y)+GetTotalHeight(x+40,y))/3;
	int sy0=y0-H0*2;
	if(H0>0)return H0;
	do{
		y-=64;
		int H=GetHeight(x,y);
		int sy1=y-H*2;
		if(y0>sy1&&sy1!=sy0){
			return H0+(H-H0)*(y0-sy0)/(sy1-sy0);
		}
		sy0=sy1;
		H0=H;
	}while(1);
}
int GetAlphaDistorted(int x,int y){
	int H=GetDeepDistorted(x,y)*HMULT;
	H=64-H;
	if(H<0)H=0;
	if(H>255)H=255;
	return H;
}

int GetHeightCB( int x, int y );

void DrawClusterizedWaterWithBump(short* Wave){
	void DrawWaterInWorldSpace(short* Wave);
	//DrawWaterInWorldSpace(Wave);
	//return;
	RunPF((256*5)+6,"DrawWater");
	SetWind(0,0,RealLx,RealLy);
	RIMG.CreateImage(mapx,mapy);
	if(!WHNet){
		WHNet=new WaterHexNet;
		WHNet->CheckMesh(mapx,mapy);
		WaterTexSysPS=IRS->CreateTexture("WaterTexSysPS",256,256,cfV8U8,1, tmpManaged);
		WaterTexDefPS=WaterTexSysPS;
		CreateWPalette();
		WHNet->HMesh.Mesh->setShader(IRS->GetShaderID("BumpWater"));
		WHNet->HMesh.Mesh->setTexture(WaterTexDefPS,0);
		if(RIMG.TargetID>0)WHNet->HMesh.Mesh->setTexture(RIMG.TargetID,1);
		else WHNet->HMesh.Mesh->setTexture(IRS->GetTextureID("Oblaka123g1.tga"),1);
	}else WHNet->CheckMesh(mapx,mapy);	
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
	}
	IRS->SetShader(WHNet->HMesh.Mesh->getShader());
	IRS->SetTexture(WaterTexDefPS,0);
	IRS->SetTexture(RIMG.TargetID,1);
	float BScale=1.0f/float(1<<(5-Shifter));
	BScale=BScale*BScale*0.3;

    IRS->SetBumpTM( Matrix3D(       0.0f, 0.05f*BScale, 0.0f, 
                                    0.02f*BScale, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f ) );

	//  -- added by Silver, 5.01.2004 --
	IRS->ResetWorldTM();
	DrawPrimBM(*WHNet->HMesh.Mesh);
	//IRS->SaveTexture( WaterTexSysPS, "c:\\dumps\\water.dds" );
	IRS->SetViewPort( Rct( 0, 0, RealLx, RealLy ) );
	//IRS->PopRenderTarget();

	StopPF((256*5)+6);
}
#define WMapsSH 6
struct OneExObjPoint{
	char dx,dy;
	bool WasLocked0:1;
	bool WasLocked2:1;
	bool WasLocked3:1;
	bool Lock:1;
	bool UnLock:1;
	bool OverEarth:1;
	short Height;
};
class OneExtraObject{
public:
	int ID;
	Matrix4D M4;
	int xc;
	int yc;
	DynArray<OneExObjPoint> Points;
};
class ExHeightMap:public ESquareNet<short,4,7,-1000,-1000,1000>{
public:
	typedef ESquareNet<short,4,7,-1000,-1000,1000> temp;
	virtual short GetValue(int x,int y){
		int H=GetHeight(x<<4,y<<4);
		return max(H,int(temp::GetValue(x,y)));
	}
	bool CheckSurroundedPoint(int x,int y){
		return GetValue(x,y)>0&&GetValue(x+1,y)>0&&GetValue(x,y+1)>0&&GetValue(x+1,y+1)>0;
	}
};
class ExtraHeightObjects;
class ExtraHeightObjects{
	static OneExtraObject* ExPtr;
	static void EXOB_AddHeight(int x,int y,float z){
		OneExObjPoint OP;
		OP.dx=x-(ExPtr->xc>>4);
		OP.dy=y-(ExPtr->yc>>4);
		OP.Height=z;//*4/3;
		OP.Lock=0;
		int h0=GetHeight(x<<4,y<<4);
		if(h0<0)h0=0;
		if(OP.OverEarth=z>h0)OP.UnLock=1;
		OP.WasLocked0=MFIELDS[0].CheckPt(x,y);
		OP.WasLocked2=MFIELDS[2].CheckPt(x,y);
		OP.WasLocked3=MFIELDS[3].CheckPt(x,y);
		ExPtr->Points.Add(OP);
	}
	static void EXOB_AddLock(int x,int y,float z){
		int N=ExPtr->Points.GetAmount();	
		int dx=x-(ExPtr->xc>>4);
		int dy=y-(ExPtr->yc>>4);
		for(int i=0;i<N;i++){
            OneExObjPoint* OXP=&ExPtr->Points[i];
			if(OXP->dx==dx&&OXP->dy==dy){
				if(OXP->OverEarth){
					OXP->Lock=1;
					OXP->UnLock=0;
					return;
				}
			}
		}		
		OneExObjPoint OP;
		OP.dx=dx;
		OP.dy=dy;
		OP.Height=-100;
		int hL=GetHeight(((x-1)<<4)+8,(y<<4)+8);
		int hR=GetHeight(((x+1)<<4)+8,(y<<4)+8);
		int hU=GetHeight((x<<4)+8,((y-1)<<4)+8);
		int hD=GetHeight((x<<4)+8,((y+1)<<4)+8);
		int hm=min(min(hL,hR),min(hU,hD));
		if(z>=hm-50){
			OP.OverEarth=1;
			OP.Height=z;
			OP.Lock=1;		
			OP.UnLock=0;
			OP.WasLocked0=MFIELDS[0].CheckPt(x,y);
			OP.WasLocked2=MFIELDS[2].CheckPt(x,y);
			OP.WasLocked3=MFIELDS[3].CheckPt(x,y);
			ExPtr->Points.Add(OP);
		}
	}

	void RegisterOneHeightObject(int idx){
		DynArray<OneExObjPoint>* pt=&ObjList[idx].Points;
		int xc=ObjList[idx].xc;
		int yc=ObjList[idx].yc;
		int N=pt->GetAmount();
		for(int i=0;i<N;i++){
			OneExObjPoint* OXP=&(*pt)[i];
			int lpx=(xc>>4)+OXP->dx;
			int lpy=(yc>>4)+OXP->dy;
			OXP->WasLocked0=MFIELDS[0].CheckPt(lpx,lpy);
			OXP->WasLocked2=MFIELDS[2].CheckPt(lpx,lpy);
			OXP->WasLocked3=MFIELDS[3].CheckPt(lpx,lpy);
			int z=OXP->Height;
			/*int zL=ExtraH.GetValue(lpx-1,lpy);
			int zR=ExtraH.GetValue(lpx+1,lpy);
			int zU=ExtraH.GetValue(lpx,lpy-1);
			int zD=ExtraH.GetValue(lpx,lpy+1);*/
			int z0=ExtraH.GetValue(lpx,lpy);;
			if(z>=z0){
				ExtraH.SetValue(lpx,lpy,z);
				MFIELDS[1].BSetPt(lpx,lpy);
			}
			if(OXP->Lock){
				BSetPt(lpx,lpy);
				MFIELDS[1].BSetPt(lpx,lpy);
			}
			if(OXP->UnLock)BClrPt(lpx,lpy);
		}
		for(int i=0;i<N;i++){
			OneExObjPoint* OXP=&(*pt)[i];
			int lpx=(xc>>4)+OXP->dx;
			int lpy=(yc>>4)+OXP->dy;			
			int z0=ExtraH.GetValue(lpx,lpy);
			int z1=ExtraH.GetValue(lpx+1,lpy);
			int z2=ExtraH.GetValue(lpx,lpy+1);
			int z3=ExtraH.GetValue(lpx+1,lpy+1);
			int za=(z0+z1+z2+z3)/4;
			int ma=max(max(abs(z0-za),abs(z1-za)),max(abs(z2-za),abs(z3-za)));
			if(ma>10)BSetPt(lpx,lpy);			
		}
	}
	void UnRegisterOneHeightObject(int idx){
		DynArray<OneExObjPoint>* pt=&ObjList[idx].Points;
		int xc=ObjList[idx].xc;
		int yc=ObjList[idx].yc;
		int N=pt->GetAmount();
		for(int i=0;i<N;i++){
			OneExObjPoint* OXP=&(*pt)[i];
			int lpx=(xc>>4)+OXP->dx;
			int lpy=(yc>>4)+OXP->dy;
			int x0=lpx*16+8-32;
			int y0=lpy*16+8-32;
			int z0=GetHeight(x0,y0);
			if(z0<0)z0=0;
			int z1=GetHeight(x0+64,y0);
			if(z1<0)z1=0;
			int z2=GetHeight(x0,y0+64);
			if(z2<0)z2=0;
			int z3=GetHeight(x0+64,y0+64);
			if(z3<0)z3=0;
			int za=(z0+z1+z2+z3)>>2;

			if(abs(z0-za)>=23||abs(z1-za)>=23||abs(z2-za)>=23||abs(z3-za)>=23||GetHeight(lpx*16,lpy*16)<0){
				BSetPt(lpx,lpy);
				MFIELDS[1].BClrPt(lpx,lpy);
			}else BClrBar(lpx-2,lpy-2,5);			
			/*if(OXP->WasLocked0)MFIELDS[0].BSetPt(lpx,lpy);
			else MFIELDS[0].BClrPt(lpx,lpy);
			if(OXP->WasLocked2)MFIELDS[2].BSetPt(lpx,lpy);
			else MFIELDS[2].BClrPt(lpx,lpy);
			if(OXP->WasLocked3)MFIELDS[3].BSetPt(lpx,lpy);
			else MFIELDS[3].BClrPt(lpx,lpy);	*/		
		}
	}
public:
	DynArray<OneExtraObject> ObjList;
	ExHeightMap ExtraH;
	void Clear(){
		for(int i=0;i<ObjList.GetAmount();i++){
            ObjList[i].Points.Clear();
		}
		ObjList.Clear();
        ExtraH.Clear();
	}
	void Free(){
		Clear();
		ExtraH.Free();
	}
	void AddExHeightObject(DWORD ModelID,Matrix4D* M4,int xc,int yc){
		if(ModelID==-1)return;
		for(int i=0;i<ObjList.GetAmount();i++)
			if(ObjList[i].xc==xc&&ObjList[i].yc==yc)return;
		OneExtraObject OX;		
		OneExtraObject* px=&ObjList[ObjList.Add(OX)];
        px->M4=*M4;
		px->ID=ModelID;
		px->xc=xc;
		px->yc=yc;
		ExPtr=px;
		DWORD navID = IMM->GetNodeID( ModelID, "Navimesh_geom" ); 
		IMM->ScanHeightmap( navID, *M4, 16,16,  EXOB_AddHeight );
		DWORD lockID = IMM->GetNodeID( ModelID, "Lockmesh_geom" ); 
		Matrix4D M4S=*M4;
		//M4S.e30-=8.0f;
		//M4S.e31-=8.0f;
		IMM->ScanHeightmap( lockID, M4S, 16,16, EXOB_AddLock );
		RegisterOneHeightObject(ObjList.GetAmount()-1);
	}
	void DeleteExtraHeightObject(int x,int y){
		for(int i=0;i<ObjList.GetAmount();i++){
			if(ObjList[i].xc==x&&ObjList[i].yc==y){
				UnRegisterHeightObjects();
				ObjList[i].Points.Clear();
                ObjList.Del(i,1);
				RegisterHeightObjects();
				//sur
				bool ClearStandAloneLockPoints(int lx, int ly, int dx, int dy, int ClearSize);
				ClearStandAloneLockPoints((x>>4)-10, (y>>4)-10, 20, 20, 6);
				return;
			}
		}
	}
	void RegisterHeightObjects(){
		for(int i=0;i<ObjList.GetAmount();i++){
			RegisterOneHeightObject(i);
		}
	}
	void UnRegisterHeightObjects(){
		for(int i=ObjList.GetAmount()-1;i>=0;i--){
			UnRegisterOneHeightObject(i);
		}
		ExtraH.Clear();
	}
};
OneExtraObject* ExtraHeightObjects::ExPtr=NULL;
class WaterMaps{
public:
	SquareNet<byte,WMapsSH> RED;
	SquareNet<byte,WMapsSH> GREEN;
	SquareNet<byte,WMapsSH> BLUE;

	SquareNet<byte,WMapsSH> TexR;
	SquareNet<byte,WMapsSH> TexG;
	SquareNet<byte,WMapsSH> TexB;

	SquareNet<short,WMapsSH> VX;
	SquareNet<short,WMapsSH> VY;
    SquareNet<short,WMapsSH> LEVEL;
	ExtraHeightObjects ExObjs;	
};
WaterMaps WMAP;
void FreeWaterMaps(){
	WMAP.RED.Free();
	WMAP.GREEN.Free();
	WMAP.BLUE.Free();
	WMAP.VX.Free();
	WMAP.VY.Free();
	WMAP.LEVEL.Free();	
	WMAP.TexR.Free();
	WMAP.TexG.Free();
	WMAP.TexB.Free();
	WMAP.ExObjs.Free();	
}
void ClearWaterMaps(){
	WMAP.RED.Fill(128);
	WMAP.GREEN.Fill(128);
	WMAP.BLUE.Fill(128);
	WMAP.VX.Fill(0);
	WMAP.VY.Fill(0);
	WMAP.LEVEL.Fill(0);	
	WMAP.TexR.Fill(128);
	WMAP.TexG.Fill(128);
	WMAP.TexB.Fill(128);
	WMAP.ExObjs.Clear();	
}
void SetupWaterMaps(){
	WMAP.RED.Setup();
	WMAP.GREEN.Setup();
	WMAP.BLUE.Setup();
	WMAP.VX.Setup();
	WMAP.VY.Setup();
	WMAP.LEVEL.Setup();
	WMAP.ExObjs.ExtraH.Setup();
	WMAP.TexR.Setup();
	WMAP.TexG.Setup();
	WMAP.TexB.Setup();
	ClearWaterMaps();	
}
extern DWORD TexDiffuse[64];
extern byte* WTexMapEx;
void SetTexturesShadowInSquare(int x0,int y0,int x1,int y1){
	
}
DWORD GetTexDiffuse(int x,int y){
	return 0xFF000000+(DWORD(WMAP.TexR.GetAproxValue(x,y))<<16)+(DWORD(WMAP.TexG.GetAproxValue(x,y))<<8)+WMAP.TexB.GetAproxValue(x,y);
}
extern int LastMx;
extern int LastMy;
int WE_PrevMx=-1;
int WE_PrevMy=-1;
byte WE_ON=-1;
void TurnWaterEditor(byte state){
	WE_PrevMx=-1;
	WE_PrevMy=-1;
	WE_ON=state;
}
extern int WaterEditMode;
extern bool Lpressed;
extern int mouseX;
extern int mouseY;
int Norma(int,int);

int WaterR=128;
int WaterG=128;
int WaterB=128;
short NormWSpeed(short v,int x,int y,void* param){
	int H=-GetHeight(x,y);
	if(H<0)H=0;
	//H=H*H;
	#define maxv 64
	if(H>maxv)H=maxv;
	return (H*int(v)/maxv)*(32+(rand()%24)-12)/maxv;
}
short GetWSpeed(short v,int xc,int yc,int x,int y,void* p){
	int VV=int(p)*(64+(rand()%48)-24)/64;
	int H=-GetHeight(x,y);
	if(H<0)H=0;
	if(H>64)H=64;
	VV=VV*H/64;
	int r=Norma(x-xc,y-yc);
	float rr=float(r)/128.0;
	int M=int(16.0*exp(-rr*rr));
	//int M=512*512*16/(r+512)/(r+512);
    int R=(int(v)*(16-M)+VV*M)/16;
	return R;
}
void ProcessWaterFlowEditor(){
	int LastMx=(mapx<<5)+mouseX;
	int LastMy=(mapy<<5)+mouseY*2;
	if(WaterEditMode!=WE_ON){
		TurnWaterEditor(WaterEditMode);
	}
	if(WE_ON==3){
		if(WE_PrevMx==-1)WE_PrevMx=LastMx;
		if(WE_PrevMy==-1)WE_PrevMy=LastMy;

		if(Lpressed){
			int dx=LastMx-WE_PrevMx;
			int dy=LastMy-WE_PrevMy;
			if(GetKeyState(VK_CONTROL)&0x8000){
				WMAP.RED  .ApplyDefFunction(LastMx,LastMy,WaterR,0,0,100,OP_DISSOLVE,FT_EXP,0,255);
				WMAP.GREEN.ApplyDefFunction(LastMx,LastMy,WaterG,0,0,100,OP_DISSOLVE,FT_EXP,0,255);
				WMAP.BLUE .ApplyDefFunction(LastMx,LastMy,WaterB,0,0,100,OP_DISSOLVE,FT_EXP,0,255);
				RIMG.CreateImage(mapx,mapy);
				WHNet->CreateMesh(mapx,mapy);
			}else{
				if(dx||dy){
					extern int ReliefBrush;
					int N=1+(ReliefBrush<<8);
					int N1=Norma(dx,dy);
					dx=dx*N/N1;
					dy=dy*N/N1;
					//WMAP.VX.ApplyDefFunction(LastMx,LastMy,dx*40,0,0,356,OP_DISSOLVE,FT_LINE,-32767.0f,32767.0f,&NormWSpeed);
					//WMAP.VY.ApplyDefFunction(LastMx,LastMy,dy*40,0,0,356,OP_DISSOLVE,FT_LINE,-32767.0f,32767.0f,&NormWSpeed);
					WMAP.VX.ApplyFn(&GetWSpeed,LastMx,LastMy,(void*)dx);
					WMAP.VY.ApplyFn(&GetWSpeed,LastMx,LastMy,(void*)dy);
					WHNet->CreateMesh(mapx,mapy);
				}
			}
		}
		WE_PrevMx=LastMx;
		WE_PrevMy=LastMy;
	}
}
void GetFlowSpeed(int x,int y,float& vx,float& vy){
	vx=-float(WMAP.VX.GetAproxValue(x,y))/4.0f/256.0;
	vy=-float(WMAP.VY.GetAproxValue(x,y))/4.0f/256.0;
}
int GetWaterLevel(int x,int y){
	return WMAP.LEVEL.GetAproxValue(x,y);
}
void SetWaterColor(int x,int y,byte R,byte G,byte B){
	WMAP.RED.ApplyDefFunction(x,y,R,0,0,256,OP_DISSOLVE,FT_LINE,0,255);
	WMAP.GREEN.ApplyDefFunction(x,y,G,0,0,256,OP_DISSOLVE,FT_LINE,0,255);
	WMAP.BLUE.ApplyDefFunction(x,y,B,0,0,256,OP_DISSOLVE,FT_LINE,0,255);
}
DWORD ModDWORD2X(DWORD C1,DWORD C2);
DWORD GetWaterColor(int x,int y){
	int R=WMAP.RED  .GetAproxValue(x,y);
	int G=WMAP.GREEN.GetAproxValue(x,y);
	int B=WMAP.BLUE .GetAproxValue(x,y);
	DWORD C=0xFF000000+(R<<16)+(G<<8)+B;
	C=ModDWORD2X(C,EngSettings.WaterColorModulator);
    return C;
}
void ResetWaterSys(){
	if(WHNet)WHNet->CurMapX=-1;
}
void SaveWaterMaps(ResFile F){
	int sz[6];
	sz[0]='WBGR';
    RBlockWrite(F,sz,4);
	sz[0]=WMAP.RED.GetBufferSizeToSave();
	sz[1]=WMAP.GREEN.GetBufferSizeToSave();
	sz[2]=WMAP.BLUE.GetBufferSizeToSave();
	sz[3]=WMAP.VX.GetBufferSizeToSave();
	sz[4]=WMAP.VX.GetBufferSizeToSave();
	sz[5]=WMAP.LEVEL.GetBufferSizeToSave();
	int siz=4+24+sz[0]+sz[1]+sz[2]+sz[3]+sz[4]+sz[5];
	RBlockWrite(F,&siz,4);
	RBlockWrite(F,sz,24);
    int maxsz=0;
	for(int i=0;i<6;i++)if(sz[i]>maxsz)maxsz=sz[i];
    char* buf=znew(char,maxsz);
	WMAP.RED.SaveDataTo(buf);
	RBlockWrite(F,buf,sz[0]);
	WMAP.GREEN.SaveDataTo(buf);
	RBlockWrite(F,buf,sz[1]);
	WMAP.BLUE.SaveDataTo(buf);
	RBlockWrite(F,buf,sz[2]);
	WMAP.VX.SaveDataTo(buf);
	RBlockWrite(F,buf,sz[3]);
	WMAP.VY.SaveDataTo(buf);
	RBlockWrite(F,buf,sz[4]);
	WMAP.LEVEL.SaveDataTo(buf);
	RBlockWrite(F,buf,sz[5]);
	free(buf);
}
void LoadWaterMaps(ResFile F){
	int sz[6];
	RBlockRead(F,sz,24);
	int maxsz=0;
	for(int i=0;i<6;i++)if(sz[i]>maxsz)maxsz=sz[i];
	char* buf=znew(char,maxsz);
	RBlockRead(F,buf,sz[0]);
	WMAP.RED.LoadDataFrom(buf);
	RBlockRead(F,buf,sz[1]);
	WMAP.GREEN.LoadDataFrom(buf);
	RBlockRead(F,buf,sz[2]);
	WMAP.BLUE.LoadDataFrom(buf);
	RBlockRead(F,buf,sz[3]);
	WMAP.VX.LoadDataFrom(buf);
	RBlockRead(F,buf,sz[4]);
	WMAP.VY.LoadDataFrom(buf);
	RBlockRead(F,buf,sz[5]);
	WMAP.LEVEL.LoadDataFrom(buf);
	free(buf);
}
void ResetWaterHash(){
	if(WHNet)WHNet->HMesh.Clear();
}
void SetWColorInPoint(int x,int y,DWORD Color){
	int R=(Color>>16)&255;
	int G=(Color>>8 )&255;
	int B=(Color    )&255;
	WMAP.RED  .SetValue(x>>WMapsSH,y>>WMapsSH,R);
	WMAP.GREEN.SetValue(x>>WMapsSH,y>>WMapsSH,G);
	WMAP.BLUE .SetValue(x>>WMapsSH,y>>WMapsSH,B);
}
int GetExtraHeight(int x,int y){
	return WMAP.ExObjs.ExtraH.GetAproxValue(x,y);
}
int GetTotalHeight(int x,int y){
	if(WMAP.ExObjs.ExtraH.TestCoor(x,y)){
		return WMAP.ExObjs.ExtraH.GetAproxValue(x,y);
	}else return GetHeight(x,y);
}
float GetFHeight0(int x,int y);
float GetFHeight(int x,int y){
	if(WMAP.ExObjs.ExtraH.TestCoor(x,y)){
		return WMAP.ExObjs.ExtraH.GetAproxValue(float(x),float(y));
	}else return GetFHeight0(x,y);
}
void SetExtraHeight(int x,int y,short v){
    WMAP.ExObjs.ExtraH.SetValue(x,y,v);
}
void AddExtraHeightObject(int x,int y,int id,Matrix4D* m4){
	//return;
	WMAP.ExObjs.AddExHeightObject(id,m4,x,y);	
}
void CreateAdditionalObjectsLocking(){
	WMAP.ExObjs.RegisterHeightObjects();	
}
void EraseExtraHeightObject(int x,int y){
	WMAP.ExObjs.DeleteExtraHeightObject(x,y);	
}
void ClearExtraHeightObjs(){
	WMAP.ExObjs.Clear();
}