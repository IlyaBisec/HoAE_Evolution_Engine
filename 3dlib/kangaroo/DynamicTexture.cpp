#include "common.h"
#include "DynamicTexture.h"
#include "RasterOp.h"

DynamicTexture::DynamicTexture(){
    Name=NULL;
    SrcTextureID=-1;
    TextureID=-1;
    SizeX=1;
    SizeY=1;
    QuadSize=64;
    Colors=NULL;
    DirtyQuads=NULL;
}
DynamicTexture::~DynamicTexture(){
    Clear();
}
void DynamicTexture::Clear(){
    if(Name)delete[]Name;
    Name=NULL;
    if(SrcTextureID!=-1)IRS->DeleteTexture(SrcTextureID);
    SrcTextureID=-1;
    if(TextureID!=-1)IRS->DeleteTexture(TextureID);
    TextureID=-1;
    SizeX=1;
    SizeY=1;
    QuadSize=64;
    Colors=NULL;
    DirtyQuads=NULL;
}
bool DynamicTexture::LoadTexture(const char* tex){
    Clear();
    TextureID=IRS->GetTextureID(tex);
    if(TextureID!=-1){
        Name=new char[strlen(tex)+1];
        strcpy(Name,tex);
        SizeX=IRS->GetTextureWidth(TextureID);
        SizeY=IRS->GetTextureHeight(TextureID);
        int pitch;
        BYTE* D=IRS->LockTexBits(TextureID,pitch);
        if(D){
            Colors=new DWORD[SizeX*SizeY];
            memcpy(Colors,D,SizeX*SizeY*4);//not correct in general
            IRS->UnlockTexBits(TextureID);
            DirtySizeX=SizeX/QuadSize;
            DirtySizeY=SizeY/QuadSize;
            DirtyQuads=new BYTE[DirtySizeX*DirtySizeY];
            memset(DirtyQuads,0,DirtySizeX*DirtySizeY);
        }else Clear();
    }
    return TextureID!=-1;
}
void DynamicTexture::SaveTexture(){
    if(TextureID!=-1){
        IRS->SaveTexture(TextureID,Name);
    }
}
void DynamicTexture::CreateTexture(DWORD FillColor){
    SizeX=2048;
    SizeY=2048;
    static int t=0;
    t++;
    char name[256];
    sprintf(name,"texture%03d.tga",t);
    TextureID=IRS->CreateTexture(name,SizeX,SizeY,cfARGB8888,1);
    Colors=new DWORD[SizeX*SizeY];
    memset(Colors,FillColor,SizeX*SizeY*4);//not correct in general    
    DirtySizeX=SizeX/QuadSize;
    DirtySizeY=SizeY/QuadSize;
    DirtyQuads=new BYTE[DirtySizeX*DirtySizeY];
    memset(DirtyQuads,1,DirtySizeX*DirtySizeY);    
}
class TexRast:public RasterOp{
public:
    DynamicTexture* DT;
    DWORD Color;
    virtual void OnPoint(int x,int y,float z,float u,float v){
        x=x%DT->SizeX;
        y=y%DT->SizeY;
        y=DT->SizeY-y-1;
        if(x<0)x+=DT->SizeX;
        if(y<0)y+=DT->SizeY;
        DT->Colors[x+y*DT->SizeX]=Color;
    };
};
void DynamicTexture::SetQuad(float u0,float v0,DWORD Color0,
                            float u1,float v1,DWORD Color1,
                            float u2,float v2,DWORD Color2,
                            float u3,float v3,DWORD Color3){
    TexRast TR;
    TR.Color=Color0;
    TR.DT=this;
    u0*=SizeX;
    v0*=SizeY;
    u1*=SizeX;
    v1*=SizeY;
    u2*=SizeX;
    v2*=SizeY;
    u3*=SizeX;
    v3*=SizeY;
    TR.Rasterize(Vector3D(u0,v0,0),Vector3D(u1,v1,0),Vector3D(u2,v2,0));
    TR.Rasterize(Vector3D(u2,v2,0),Vector3D(u1,v1,0),Vector3D(u3,v3,0));                                
}
void DynamicTexture::SetPixel(float u,float v,Vector4D Color){
    v=1-v;
    int px=u*SizeX;
    int py=v*SizeY;
    px=px%SizeX;
    py=py%SizeX;
    if(px<0)px+=SizeX;
    if(py<0)py+=SizeY;
    Colors[px+py*SizeX]=V4D2DW(Color);
    DirtyQuads[(px/QuadSize)+(py/QuadSize)*DirtySizeX]=1;
}
void DynamicTexture::SetPixel(float u,float v,DWORD Color){
    v=1-v;
    int px=u*SizeX;
    int py=v*SizeY;
    px=px%SizeX;
    py=py%SizeX;
    if(px<0)px+=SizeX;
    if(py<0)py+=SizeY;
    Colors[px+py*SizeX]=Color;
    DirtyQuads[(px/QuadSize)+(py/QuadSize)*DirtySizeX]=1;
}
Vector4D DynamicTexture::GetPixel(float u,float v){
    if(Colors){
		v=1-v;
		u-=ffloorf(u);		
        if(u<0)u+=1;
		v-=ffloorf(v);
        if(v<0)v+=1;
		
		float x=SizeX*u;
		float y=SizeY*v;
		int x0=ffloorf(x);
		int y0=ffloorf(y);
		int x1=(x0+1)%SizeX;
		int y1=(y0+1)%SizeY;
		float du=x-x0;
		float dv=y-y0;
		Vector4D V1=DW2V4D(Colors[x0+y0*SizeX]);
		Vector4D V2=DW2V4D(Colors[x1+y0*SizeX]);
		Vector4D V3=DW2V4D(Colors[x0+y1*SizeX]);
		Vector4D V4=DW2V4D(Colors[x1+y1*SizeX]);
		V2-=V1;V3-=V1;
		V4-=V2;V4-=V3;V4-=V1;
		V2*=du;V3*=dv;
		V4*=du*dv;
		V1+=V2;V1+=V3;V1+=V4;
        return V1;
	}
	return Vector4D(1,1,1,1);
}
void DynamicTexture::Update(){
    int pos=0;
    for(int y=0;y<DirtySizeY;y++){
        for(int x=0;x<DirtySizeX;x++){
            if(DirtyQuads[pos]){
                int pitch;
                BYTE* data=IRS->LockTexBits(TextureID,Rct(x*QuadSize,y*QuadSize,QuadSize,QuadSize),pitch);
                if(data){
                    DWORD* d2=Colors+x*QuadSize+y*QuadSize*SizeX;
                    for(int p=0;p<QuadSize;p++){
                        memcpy(data+p*pitch,d2+p*SizeX,QuadSize*4);                    
                    }
                    DirtyQuads[pos]=0;
                    IRS->UnlockTexBits(TextureID);
                }
            }
            pos++;
        }
    }    
}