#include "StdAfx.h" #include "common.h" 
#include "TexturePattern.h"
#include "xmmintrin.h"



TexturePattern::TexturePattern(){
	SizeX=0;
	SizeY=0;
	Data=NULL;
	TextureID=-1;
}
TexturePattern::~TexturePattern(){
    clear();
}
bool TexturePattern::LoadTexture(const char* Name){
	int ID=IRS->GetTextureID(Name);
    setTexture(ID);
	return ID!=-1;
}
void TexturePattern::clear(){
	if(Data)delete(Data);
	SizeX=0;
	SizeY=0;
	Data=NULL;
	TextureID=-1;	
}
Vector4D DW2V4D(DWORD C){
	//return Vector4D(C&255,(C>>8)&255,(C>>16)&255,(C>>24)&255);
	Vector4D V;
	DWORD I[4];
	__asm{
		xor		eax,eax
		xor		ebx,ebx
		mov		al,BYTE PTR C
		mov		bl,BYTE PTR C+1
		mov		I,eax
		mov		I+4,ebx
		mov		al,BYTE PTR C+2
		mov		bl,BYTE PTR C+3
		mov		I+8,eax
		mov		I+12,ebx
		
		CVTPI2PS xmm0,I
		CVTPI2PS xmm1,I[8]
		MOVLPS   V.x,xmm0
		MOVLPS   V.z,xmm1		
		EMMS
	}
	return V;
}
DWORD V4D2DW(Vector4D& V){
	/*
	__declspec(align(16)) __m128 V1;
	memcpy(&V1,&V,16);
	__m64 M=_mm_cvtps_pi8(V1);
	return *(DWORD*)&M.m64_u32;
	*/

	
    //clamp(V.x,0.0f,255.0f);
    //clamp(V.y,0.0f,255.0f);
    //clamp(V.z,0.0f,255.0f);
    //clamp(V.w,0.0f,255.0f);
    return (DWORD(lrintf(V.w))<<24)+(lrintf(V.z)<<16)+(lrintf(V.y)<<8)+lrintf(V.x);
	
	/*
	__declspec(align(16)) int I[4];
	
	V1=V;
	DWORD R;
	__asm{	
		
		EMMS
		CVTTPS2PI       mm0,V.x
		CVTTPS2PI       mm1,V.z
		MOVQ          I,mm0
		MOVQ          I[8],mm1	
		EMMS
		
		//CVTTPS2DQ		xmm0,V1
		//MOVAPD		I,xmm0
		
		mov				eax,I[0]
		mov				ebx,I[4]
        mov				BYTE PTR R  ,al
		mov				BYTE PTR R+1,bl
		mov				eax,I[8]
		mov				ebx,I[12]
		mov				BYTE PTR R+2,al
		mov				BYTE PTR R+3,bl
	}
	return R;
	*/
}
Vector4D TexturePattern::getPixelUV(float u,float v){
	if(Data){
		float ui;
		float vi;
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
		Vector4D V1=DW2V4D(Data[x0+y0*SizeX]);
		Vector4D V2=DW2V4D(Data[x1+y0*SizeX]);
		Vector4D V3=DW2V4D(Data[x0+y1*SizeX]);
		Vector4D V4=DW2V4D(Data[x1+y1*SizeX]);
		V2-=V1;V3-=V1;
		V4-=V2;V4-=V3;V4-=V1;
		V2*=du;V3*=dv;
		V4*=du*dv;
		V1+=V2;V1+=V3;V1+=V4;
        return V1;
	}
	return Vector4D(1,1,1,1);
}
Vector4D TexturePattern::getPixel(Vector3D pos){
	WorldToUVSpace.transformPt(pos);	
	return getPixelUV(pos.x,pos.y);
}
int TexturePattern::getWidth(){
    return SizeX;
}
int TexturePattern::getHeight(){
    return SizeY;
}
void TexturePattern::setTexture(int ID){
	clear();
	if(ID!=-1){
		int pitch;
		IRS->LockTexBits(ID,pitch);
		IRS->UnlockTexBits(ID);
        SizeX=IRS->GetTextureWidth(ID);
		SizeY=IRS->GetTextureHeight(ID);
		if(SizeX>0 && SizeY>0){
			Data=new DWORD[SizeX*SizeY];            
			BYTE* p=IRS->LockTexBits(ID,pitch);
			if(p){
				ColorFormat cf=IRS->GetTextureFormat(ID);
				if(cf==cfARGB8888 || cf==cfXRGB8888 ){
					for(int i=0;i<SizeY;i++){
						memcpy(Data+i*SizeX,p+pitch*i,SizeX*4);
					}                    
				}else
				if(cf==cfRGB888){
					for(int i=0;i<SizeY;i++){
						for(int x=0;x<SizeX;x++){
							memcpy(Data+i*SizeX+x*4,p+pitch*i+x*3,3);
						}
					}
				}				
				IRS->UnlockTexBits(ID);
                Average=Vector3D::null;
                int pos=0;
                for(int y=0;y<SizeY;y++){
                    for(int x=0;x<SizeX;x++){
                        Vector4D v=DW2V4D(Data[pos++]);
                        Average+=v;
                    }
                }
                Average/=SizeX*SizeY;
			}
		}
	}
	TextureID=ID;
}
int TexturePattern::getTexture(){
    return TextureID;
}
void TexturePattern::CreateWorldToUVSpace(){
    ICamera* cam=GetCamera();
	WorldToUVSpace=cam->GetWorldTM();
	WorldToUVSpace*=cam->GetViewTM();
	WorldToUVSpace*=cam->GetProjTM();
	ScreenProp sp=IRS->GetScreenProp();
	float sx=sp.m_Width/2;
	float sy=sp.m_Height/2;
	Matrix4D ps(
		sx, 0, 0, 0,
		 0,sy, 0, 0,
		 0, 0, 1, 0,
		sx,sy, 0, 1);
	WorldToUVSpace*=ps;
    WorldToUVSpace*=ScreenToUVSpace;
	ProjCX=sp.m_Width>>1;
	ProjCY=sp.m_Height>>1;
}
void TexturePattern::RenderOnPen(Vector3D center,float radius){
	bool rp=!!(GetKeyState(VK_RBUTTON)&0x8000);
	if(rp)radius*=10;
	static int sh=IRS->GetShaderID("hudZDisable_L");
	rsSetShader(sh);
	rsSetTexture(TextureID);
	ICamera* cam=GetCamera();
	int nSegments=32;
	Matrix4D cTM;
	Vector3D z( cam->GetDirection() );
	Vector3D x, y;
	z.CreateBasis( x, y );
	cTM.fromBasis( x, y, z, center );

	float phiStep = c_DoublePI / float( nSegments );
	Vector3D a, b;

	Vector3D tc=center;
    ToUV(tc);

	for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
	{
		a.set( radius * cos( phi ), radius * sin( phi ), 0.0f );
		b.set( radius * cos( phi + phiStep), radius * sin( phi + phiStep ), 0.0f );

		a *= cTM;
		b *= cTM;		
		Vector3D ta=a;
		ToUV(ta);
		Vector3D tb=b;
		ToUV(tb);
		rsPoly( a, b, center, ta.x, ta.y, tb.x, tb.y, tc.x, tc.y,0x80FFFFFF,0x80FFFFFF,0x80FFFFFF );
	}
	rsFlush();
	rsRestoreShader();
}
void TexturePattern::tex_reset(){	
	dAngle=0;
	dScale=1;
	dShift=Vector3D(0,0,0);
	if(SizeX>0 && SizeY>0){		
		ScreenToUVSpace.st(Vector3D(1.0f/SizeX,1.0f/SizeY,1),Vector3D::null);
	}
}
void TexturePattern::tex_rotate(float ScrX,float ScrY,float Angle){
	dAngle+=Angle;
	Matrix4D m2;
	m2.translation(Vector3D(-ScrX,-ScrY,0));
	Matrix4D m;
	m.srt(1,Vector3D::oZ,Angle,Vector3D(ScrX,ScrY,0));
    ScreenToUVSpace.mulLeft(m);
	ScreenToUVSpace.mulLeft(m2);
}
void TexturePattern::tex_translate	(float dx,float dy){
	dShift+=Vector3D(dx,dy,0);
	Matrix4D m;
	m.translation(Vector3D(dx,dy,0));
	ScreenToUVSpace.mulLeft(m);
}
void TexturePattern::tex_scale(float ScrX,float ScrY,float scalex,float scaley){
	dScale*=scalex;
	Matrix4D m2;
	m2.translation(Vector3D(-ScrX,-ScrY,0));
	Matrix4D m;
	m.st(Vector3D(scalex,scaley,1),Vector3D(ScrX,ScrY,0));
	ScreenToUVSpace.mulLeft(m);
	ScreenToUVSpace.mulLeft(m2);
}
void TexturePattern::ToUV(Vector3D& pos,Vector3D* n){	
	if(GeneralPen::MappingType==0){
		Vector4D v(pos);
		v*=IRS->GetWorldViewProjTM();
		v/=v.w;
		v.x=(v.x+1.0f)*ProjCX;
		v.y=(v.y+1.0f)*ProjCY;
		v*=ScreenToUVSpace;
		pos.x=v.x;
		pos.y=v.y;
	}else{
		float ax=abs(n->x);
		float ay=abs(n->y);
		float az=abs(n->z);
		if(ax>ay && ax>az){
			pos=Vector3D(pos.y,pos.z,0);
		}else
		if(ay>ax && ay>az){
			pos=Vector3D(-pos.x,pos.z,0);
		}else
		if(az>ax && az>ay){
			pos=Vector3D(pos.x,pos.y,0);
		}
		Vector4D v(pos);
		v*=ScreenToUVSpace;
		pos.x=v.x;
		pos.y=v.y;
	}
}
void TexturePattern::ProcessMouse(){
	bool rp=!!(GetKeyState(VK_RBUTTON)&0x8000);
	bool ALT=!!(GetKeyState(VK_MENU)&0x8000);
	bool CTRL=!!(GetKeyState(VK_CONTROL)&0x8000);
	bool SHIFT=!!(GetKeyState(VK_SHIFT)&0x8000);
	POINT Pt;
	GetCursorPos2(&Pt);
	float dx=Pt.x-LastPosX;
	float dy=Pt.y-LastPosY;
	if(rp){
		if(CTRL){
			tex_rotate(LastFreePosX,LastFreePosY,dx/100.0f);
		}else
		if(SHIFT){
			tex_scale(LastFreePosX,LastFreePosY,1.0f+dx/100.0f,1.0f+dx/100.0f);
		}else
		if(!ALT){
			tex_translate(-dx,dy);
		}		
	}else{	
		LastFreePosX=Pt.x;
		LastFreePosY=Pt.y;
	}
	LastPosX=Pt.x;
	LastPosY=Pt.y;
}
void TexturePattern::CreateStroke(TexturePattern* src){
    clear();
    DWORD ID=src->TextureID;
	if(ID!=-1){
        SizeX=IRS->GetTextureWidth(ID);
		SizeY=IRS->GetTextureHeight(ID);
		if(SizeX>0 && SizeY>0){
			Data=new DWORD[SizeX*SizeY];
            int sz2=SizeX/2;
            for(int y=0;y<SizeY;y++){
                int ofs=y*SizeX;
                int ofs0=ofs;
                for(int x=0;x<SizeX;x++){
                    if(x>=sz2){
                        Data[ofs]=Data[ofs-1];
                    }else
                    if(x==0){
                        Data[ofs]=src->Data[ofs+sz2];
                    }else{
                        DWORD D1=(Data[ofs-1]>>8)&255;
                        DWORD D2=(src->Data[ofs0+sz2+x]>>8)&255;
                        DWORD D3=(src->Data[ofs0+sz2-x]>>8)&255;
                        D1=max(D2,D1);
                        D1=max(D3,D1);
                        Data[ofs]=(D1<<16)+(D1<<8)+D1;
                    }
                    ofs++;
                }
            }
		}
	}
	TextureID=ID;
}
float TexturePattern::LastFreePosX;//position when RMB is unpressed
float TexturePattern::LastFreePosY;
float TexturePattern::LastPosX;
float TexturePattern::LastPosY;
float TexturePattern::ProjCX;
float TexturePattern::ProjCY;
Matrix4D TexturePattern::ScreenToUVSpace;
Matrix4D TexturePattern::WorldToUVSpace;	
float TexturePattern::dScale=1;
float TexturePattern::dAngle=0;
Vector3D TexturePattern::dShift(0,0,0);