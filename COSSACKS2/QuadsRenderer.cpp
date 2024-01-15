#include "stdheader.h"
#include "unichash.h"
#include "QuadsRender.h"
#include "ITerrain.h"

uni_hash<OneQuad*>			QuadsRenderer::QuadList;
DynArray<QuadsRenderer*>	QuadsRenderer::QuadsGenerators;    
uni_hash<BOOL>				QuadsRenderer::IsReady;
uni_hash<GrassPoint>		GrassRenderer::GrassHeight;
byte						GrassRenderer::CurrentGrassType=0;
ClonesArray<OneGrassType>	GrassRenderer::GrassProps;

QuadsRenderer::QuadsRenderer(){
	for(int i=0;i<QuadsGenerators.GetAmount();i++){
		if(!memcmp(QuadsGenerators[i],this,4))return;
	}
	QuadsGenerators.Add(this);
}
QuadsRenderer::~QuadsRenderer(){
	for(int i=0;i<QuadsGenerators.GetAmount();i++){
		if(QuadsGenerators[i]==this){
			QuadsGenerators.Del(i,1);
			i--;
		}
	}
}
void QuadsRenderer::KillQuad(int qx,int qy){
	DWORD H=qx+(qy<<16);
	scan_key(QuadList,H,OneQuad** Q){
        delete(*Q);
	}scan_end;
	QuadList.del(H);
}
void QuadsRenderer::ReCreateQuad(int qx,int qy){
	KillQuad(qx,qy);
	Rct rct(qx*QuadSize,qy*QuadSize,QuadSize,QuadSize);
	for(int i=0;i<QuadsGenerators.GetAmount();i++){
		DynArray<BaseMesh*> bms;
        QuadsGenerators[i]->FillQuad(rct,bms);
		for(int j=0;j<bms.GetAmount();j++){
			OneQuad* Q=new OneQuad;
			Q->bm.FromMesh(*bms[j]);
			Q->bm.setShader(bms[j]->getShader());
			Q->bm.setTexture(bms[j]->getTexture());
			Q->bm.setTexture(bms[j]->getTexture(1),1);
			Q->QuadX=qx;
			Q->QuadX=qy;
			extern int tmtmt;
			Q->LastRanderFrame=tmtmt;
			Q->AB=bms[j]->GetAABB();
			QuadList.add(DWORD(qx+(qy<<16)),Q);
			//delete(bms[j]);
		}
	}
}
void QuadsRenderer::SetDirtyRect(Rct rect){
    int  x0=rect.x/QuadSize;
	int  y0=rect.y/QuadSize;
	int  x1=(rect.x+rect.w-1)/QuadSize;
	int  y1=(rect.y+rect.h-1)/QuadSize;
	for(int i=x0;i<=x1;i++){
		for(int j=y0;j<=y1;j++){
            IsReady.del(DWORD(i+(j<<16)));
		}
	}
}
void QuadsRenderer::FillQuad(Rct rct,DynArray<BaseMesh*>& bm){
    	
}
extern ICamera*	ICam;
void QuadsRenderer::Render(){
	IRS->ResetWorldTM();
	Frustum& F=(Frustum&)ICam->GetFrustum();
	Vector3D V[4];
	F.Intersection(Plane::xOy,&V[0]);
	int xmin= 16000;
	int xmax=0;
	int ymin=16000;
	int ymax=0;
	for(int i=0;i<4;i++){
		int x=V[i].x;
		int y=V[i].y;
		if(x<xmin)xmin=x;
		if(x>xmax)xmax=x;
		if(y<ymin)ymin=y;
		if(y>ymax)ymax=y;
	}
	int qx=xmin/QuadSize;
	int qy=ymin/QuadSize;
	int qx1=xmax/QuadSize;
	int qy1=ymax/QuadSize;
	for(int iy=qy1;iy>=qy;iy--){
		for(int ix=qx;ix<=qx1;ix++){		
			DWORD H=ix+(iy<<16);
			AABoundBox AB(Rct(ix*QuadSize,iy*QuadSize,QuadSize,QuadSize),0,100);
			if(F.Overlap(AB)){
				if(!IsReady.get(H)){
					KillQuad(ix,iy);
					ReCreateQuad(ix,iy);
					BOOL X=1;
					IsReady.add_once(H,X);
				}			
				OneQuad** Q=QuadList.get(H);
				if(Q){
					IRS->SetShader((*Q)->bm.getShader());
					(*Q)->bm.render();
				}
			}
		}
	}
	IRS->Flush();
}
_inline DWORD W2DW(WORD x){
	return ((x&15)<<4)+(((x>>4)&15)<<12)+(((x>>8)&15)<<20);
}
int ps_rand(DWORD v){
	return randoma[v%8192];
}
DWORD GetMinimapColor(int x,int y,WORD* C){
	x-=64;
	y-=92;
	if(x<0)x=0;
	if(y<0)y=0;
	if(x>16384)x=16384;
	if(y>16384)y=16384;
	int x0=x>>6;
	int y0=y>>6;
	int dx=(x&63);
	int dy=(y&63);
	int ofs=x0+y0*256;
    DWORD C1=W2DW(C[ofs]);
	DWORD C2=W2DW(C[ofs+1]);	
	DWORD C3=W2DW(C[ofs+256]);
	DWORD C4=W2DW(C[ofs+257]);
	DWORD R=0;
	for(int i=0;i<4;i++){		
		int CH1=C1&255;
		int CH2=C2&255;
		int CH3=C3&255;
		int CH4=C4&255;
		C1>>=8;
		C2>>=8;
		C3>>=8;
		C4>>=8;
		int CC=CH1+(CH2-CH1)*dx/64+(CH3-CH1)*dy/64+(CH4+CH1-CH3-CH2)*dx*dy/64/64+(ps_rand(x+y*173)%40)-20;
		if(CC>255)CC=255;
		if(CC<0)CC=0;
		R|=(CC<<(i<<3));
	}
	return R;
}
BYTE GHeight[4096];
bool GH_Init=false;
float GetGrassHeight(int x,int y,WORD* C){
	if(!GH_Init){
		for(int R=0;R<16;R++){
			for(int G=0;G<16;G++){
				for(int B=0;B<16;B++){
					int D=G*25-B*20-R*8;
					int P=B+G*16+R*256;
					GHeight[P]=D>0?128:0;
				}
			}
		}
		GH_Init=true;
	}
	x-=64;
	y-=92;
	if(x<0)x=0;
	if(y<0)y=0;
	if(x>16384)x=16384;
	if(y>16384)y=16384;
	int x0=x>>6;
	int y0=y>>6;
	int dx=(x&63);
	int dy=(y&63);
	int ofs=x0+y0*256;

	int C1=GHeight[C[ofs]&0xFFF];
	int C2=GHeight[C[ofs+1]&0xFFF];
	int C3=GHeight[C[ofs+256]&0xFFF];
	int C4=GHeight[C[ofs+257]&0xFFF];	
	int R=C1+(C2-C1)*dx/64+(C3-C1)*dy/64+(C4+C1-C3-C2)*dx*dy/64/64;
	return R;
}
int GetMinimapTexture();
void GrassRenderer::FillQuad(Rct rct,DynArray<BaseMesh*>& bm){
	const bool UseBillboards=true;
	const bool UseScaling=true;
	static int tex;
	static int tex1;
	static int Nx=18;
	int N=Nx*Nx;
	static float grass_size=25;
	static BaseMesh* BM;
	if(!BM){
		BM=new BaseMesh;	
		BM->create(N*4,N*6);
	}
	Vertex2t* V=(Vertex2t*)BM->getVertexData();
	int i4=0;
	word* idx=BM->getIndices();
	int Tex=GetMinimapTexture();
	int pitch;
	BYTE* B=IRS->LockTexBits(Tex,pitch);
	int NH=0;
	int szx;
	int szy;
	int ny;
	float dv;
	static OneGrassType stdg;
	DWORD V0=rct.y*1791+rct.x*18337;
	for(int iy=0;iy<N;iy++){		
			int xc=rct.x+rct.w*ps_rand(iy+V0)/32768;			
			int yc=rct.y+(N-iy)*rct.h/N;			
			DWORD Px=xc/GrassQuantSize;
			DWORD Py=yc/GrassQuantSize;
			DWORD P=Px+(Py<<16);
			GrassPoint* GP=GrassHeight.get(P);
			if(GP){
				float zc=GetHeight(xc,yc);
				float u0=(ps_rand(iy+V0+int(xc))&1)/4.0f;
				float v0=0;			
				OneGrassType* gt=&stdg;
				if(GP){
					//v0=float(GP->Type)/ny;
					if(GP->Type<GrassRenderer::GrassProps.GetAmount()){
						gt=GrassRenderer::GrassProps[GP->Type];
					}
				}
				tex=gt->TextureID;
				tex1=gt->ShadowTextureID;
				szx=IRS->GetTextureWidth(tex);
	            szy=IRS->GetTextureHeight(tex);
				ny=szy*4/szx;
	            if(ny==0)ny=1;
	            dv=1.0f/ny;
				if(ps_rand(V0+yc)/32768.0f<gt->Frequency){
					float a=0;//rand()*c_PI/32768.0f;
					float _cos=1;//cos(a);
					float _sin=0;//sin(a);
					float sign=1;
					if(ps_rand(V0+xc+yc)<16384){
						_cos=-_cos;
						sign=-1;
					}
					bool WasOne=false;
					//int dzX=abs(GetHeight(xc+32,yc)-GetHeight(xc-32,yc));
					//int dzY=abs(GetHeight(xc,yc+32)-GetHeight(xc,yc-32));			
					//if(dzX<40 && dzY<40){
						static float scl=30;					
						float gsize=grass_size*(1.0+(ps_rand(xc+yc)%15-7)/scl)*gt->Scale;
						DWORD gh=255;

						if(UseScaling && GP){
							gsize*=float(GP->Height)/128;
							gh=GP->Height*2;
							if(gh>255)gh=255;
						}
						if(gsize>0 && GP){
							for(int q=0;q<2;q++){
								for(int p=0;p<2;p++){
									DWORD D=GetMinimapColor(xc,yc,(WORD*)B);					
									float dx=(q*2-1)*grass_size;
									V->x=UseBillboards?0:dx*_cos;
									V->y=UseBillboards?0:dx*_sin;					
									V->x+=xc;
									V->y+=yc;
									int H;
									if(!UseScaling){
										Vector3D G=GetInterpGrassGradient(V->x,V->y);
										H=G.z;							
										if(H>0){
											NH++;
											WasOne=true;
											G.z=0;
											static float coef=-0.3;
											int m=275+int(G.dot(Vector3D(2,1,0)))*coef;
											if(m<0)m=0;
											DWORD ModDWORD2XRGB(DWORD C1,DWORD C2);
											D=ModDWORD2XRGB(MulDWORD(D,m),gt->ColorModulator);
										}
									}else{
										WasOne=true;
										H=128;
										NH++;
                                        DWORD CL=MixDWORD(gt->ColorModulator,gt->ColorModulatorForSmallSize,gh,255-gh);
										DWORD ModDWORD2XRGB(DWORD C1,DWORD C2);
										D=ModDWORD2XRGB(D,gt->ColorModulator);
									}
									V->z=zc;
									V->u=u0+q*0.25;
									V->v=v0+p*dv;
									V->u2=(1-p-(128-H)/128.0)*gsize*1.6;						
									V->v2=UseBillboards?sign*(q*2-1)*gsize:0;
									V->diffuse=D;
									V++;
								}
							}
							if(WasOne){
								idx[0]=i4  ;
								idx[1]=i4+1;
								idx[2]=i4+2;

								idx[3]=i4+2;
								idx[4]=i4+1;
								idx[5]=i4+3;

								idx+=6;
								i4+=4;
							}else{
								V-=4;
							}
						}
					//}
				}
			}
		//}
	}
	BM->setNPri(N*2);
	BM->setNVert(N*4);
	BM->setNInd(N*6);
	BM->setTexture(tex);
	BM->setTexture(tex1,1);
	int GetMinimapTexture();
	//BM->setTexture(GetMinimapTexture(),1);
	static int sh=IRS->GetShaderID("grass");
	BM->setShader(sh);
	IRS->UnlockTexBits(Tex);
	if(NH)bm.Add(BM);	
}
void CreateQuadRenderers(){
    GrassRenderer* GR=new GrassRenderer;
}
void ClearQuads(){
	static int R=0;
	if(R==0){
		GrassRenderer::ClearGrassRound(0,0,32768,128,0);
		R=1;
	}
}
void RenderQuads(){
	static int R=0;
	if(R==0){
		QuadsRenderer::SetDirtyRect(Rct(100,100,2024,2024));
		R=1;
	}
	QuadsRenderer::Render();
}
void MakeDirtyGrassQuad(Rct rct){
	QuadsRenderer::SetDirtyRect(rct);
}
void GrassRenderer::SetGrassHeight(int x,int y,int H){
	DWORD D=DW(x,y);
	BYTE h=H;	
	if(H>0){
        GrassHeight.add_once(D,GrassPoint(h));
	}else{
        GrassHeight.del(D);
	}
}
int GrassRenderer::GetGrassHeight(int x,int y){
    DWORD D=DW(x,y);
	GrassPoint* H=GrassHeight.get(D);
	return H?H->Height:0;
}
int  GrassRenderer::GetInterpGrassHeight(int x,int y){
	int dx=x%GrassQuantSize;
	int dy=y%GrassQuantSize;
	x/=GrassQuantSize;
	y/=GrassQuantSize;
	int H1=GetGrassHeight(x,y);
	int H2=GetGrassHeight(x+1,y);
	int H3=GetGrassHeight(x,y+1);
	int H4=GetGrassHeight(x+1,y+1);
    return H1+(H2-H1)*dx/GrassQuantSize+(H3-H1)*dy/GrassQuantSize+(H4+H1-H2-H3)*dx*dy/GrassQuantSize/GrassQuantSize;
}
Vector3D  GrassRenderer::GetInterpGrassGradient(int x,int y){
	int dx=x%GrassQuantSize;
	int dy=y%GrassQuantSize;
	x/=GrassQuantSize;
	y/=GrassQuantSize;
	int H1=GetGrassHeight(x,y);
	int H2=GetGrassHeight(x+1,y);
	int H3=GetGrassHeight(x,y+1);
	int H4=GetGrassHeight(x+1,y+1);
	Vector3D V;
	V.z=H1+(H2-H1)*dx/GrassQuantSize+(H3-H1)*dy/GrassQuantSize+(H4+H1-H2-H3)*dx*dy/GrassQuantSize/GrassQuantSize;
	V.x=(H2-H1)+(H4+H1-H2-H3)*dy/GrassQuantSize;
	V.y=(H3-H1)+(H4+H1-H2-H3)*dx/GrassQuantSize;
	return V;
}
void GrassRenderer::SetGrassRound(int x,int y,int R,int D,int H){
    int x0=(x-R-D-32)/GrassQuantSize;
	int y0=(y-R-D-32)/GrassQuantSize;
	int x1=(x+R+D+32)/GrassQuantSize;
	int y1=(y+R+D+32)/GrassQuantSize;
	for(int xx=x0;xx<=x1;xx++){
		for(int yy=y0;yy<=y1;yy++){
			int xr=xx*GrassQuantSize;
			int yr=yy*GrassQuantSize;
            int H0=GetGrassHeight(xx,yy);
			int r=sq_norma(xr-x,yr-y);
			int H1=0;
			if(r<R)H1=H;
			else if(r<R+D)H1=H*(R+D-r)/D;
			if(H1>H0){
				SetGrassHeight(xx,yy,H1);
			}
		}
	}
	MakeDirtyGrassQuad(Rct(x-R-D,y-R-D,(R+D)*2,(R+D)*2));
}
void GrassRenderer::ClearGrassRound(int x,int y,int R,int D,int H){
	int x0=(x-R-D-32)/GrassQuantSize;
	int y0=(y-R-D-32)/GrassQuantSize;
	int x1=(x+R+D+32)/GrassQuantSize;
	int y1=(y+R+D+32)/GrassQuantSize;
	for(int xx=x0;xx<=x1;xx++){
		for(int yy=y0;yy<=y1;yy++){
			int xr=xx*GrassQuantSize;
			int yr=yy*GrassQuantSize;
			int H0=GetGrassHeight(xx,yy);
			int r=sq_norma(xr-x,yr-y);
			int H1=128;
			if(r<R)H1=H;
			else if(r<R+D)H1=128+(H-128)*(R+D-r)/D;
			if(H1<H0){
				SetGrassHeight(xx,yy,H1);
			}
		}
	}
	MakeDirtyGrassQuad(Rct(x-R-D,y-R-D,(R+D)*2,(R+D)*2));
}
void GrassRenderer::SaveGrass(ResFile F){	
	BYTE* B=NULL;
	int sz=GrassHeight.simple_serialize(&B);
	DWORD D='GRS1';
	RBlockWrite(F,&D,4);
    D=4+sz;
	RBlockWrite(F,&D,4);
	D=sz;
	RBlockWrite(F,&D,4);
	RBlockWrite(F,B,sz);
	delete[]B;
}
void GrassRenderer::LoadGrass(ResFile F){
    int sz=0;
	RBlockRead(F,&sz,4);
	BYTE* B=new BYTE[sz];
	RBlockRead(F,B,sz);
	GrassHeight.simple_unserialize(B);
	delete[]B;
	MakeDirtyGrassQuad(Rct(0,0,16384,16384));
}
void GrassRenderer::ClearGrass(){
	QuadList.reset();
	//QuadList.del_all();
	GrassHeight.reset();
	//GrassHeight.del_all();
	MakeDirtyGrassQuad(Rct(0,0,16384,16384));
}
GrassPoint::GrassPoint(BYTE H){
	Height=H;
	Type=GrassRenderer::CurrentGrassType;
}
void OneGrassType::Init(){
	ColorModulator=0xFF808080;
	ColorModulatorForSmallSize=0xFF40FF40;
	TextureID=IRS->GetTextureID("Texture\\grass\\!trava.dds");
	ShadowTextureID=IRS->GetTextureID("Texture\\grass\\!trava_bg.dds");
	Scale=1;
	Frequency=1;
}
void CheckGrassPresence(int xc,int yc,int R){
	int x0=(xc-R-100)/GrassQuantSize;
	int x1=(xc+R+100)/GrassQuantSize;
	int y0=(yc-R-100)/GrassQuantSize;
	int y1=(yc+R+100)/GrassQuantSize;
	int Lx=(x1-x0+1);
	int Ly=(y1-y0+1);
	itr_GetSpritesInRadius.Create(xc,yc,R+250);
	static DynArray<GrassPoint> Pts;
	Pts.FastClear();
	GrassPoint gp;
	gp.Height=0;
	Pts.Add(gp,Lx*Ly);
	while(OneSprite* OS=itr_GetSpritesInRadius.Next()){
		ObjCharacter* OC=OS->OC;
		if(OC->GrassHeight){
			int gr1=OC->GrassRadius-32;
			int gr2=OC->GrassRadius+32;
			int H=OC->GrassHeight;
			int p=0;
			for(int y=y0;y<=y1;y++){
				for(int x=x0;x<=x1;x++){
					int xr=x*GrassQuantSize;
					int yr=y*GrassQuantSize;
					int d=Norma(xr-OS->x,yr-OS->y);
					if(d<gr2){
                        int Hc=0;
						if(d<gr1)Hc=H+1;
						else Hc=1+(gr2-d)*H/(gr2-gr1);
						GrassPoint& gpt=Pts[p];
						if(Hc>gpt.Height){
							gpt.Height=Hc;
							gpt.Type=OC->GrassType;
						}
					}
					p++;                    
				}
			}
		}
	}
	int p=0;
	bool change=false;
	for(int y=y0;y<=y1;y++){
		for(int x=x0;x<=x1;x++){
            DWORD D=DW(x,y);
			GrassPoint* GP=GrassRenderer::GrassHeight.get(D);
			if(GP){
				if(Pts[p].Height==0){
					GrassRenderer::GrassHeight.del(D);					
				}else{
					GP->Height=Pts[p].Height;
					GP->Type=Pts[p].Type;
				}
				change=true;
			}else{
				if(Pts[p].Height>0){
                    gp.Height=Pts[p].Height;
					gp.Type=Pts[p].Type;
                    GrassRenderer::GrassHeight.add(D,gp);
					change=true;
				}
			}
            p++;
		}
	}
	if(change){
		QuadsRenderer::SetDirtyRect(Rct(xc-R,yc-R,R+R,R+R));
	}
}
void OneGrassType::Refresh(){
	GrassRenderer::IsReady.reset();
}
void RegGrassEditor(){
	REG_CLASS(OneGrassType);
	GrassRenderer::GrassProps.SafeReadFromFile("\grass.xml");
	AddStdEditor("GrassProperties",&GrassRenderer::GrassProps,"\grass.xml",RCE_DEFAULT3);
}