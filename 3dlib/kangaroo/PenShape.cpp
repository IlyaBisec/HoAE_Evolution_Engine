#include "common.h"

float ConePen::GetWeight(float r){
    return r<1 ? 1-r : 0 ;
}
float SimplePen::GetWeight(float r){	
    return r<1 ? r=1-r,r*r*(2-r)*(2-r) : 0 ;
}
float SharpPen::GetWeight(float r){
    return r<1 ? (1-r)*(1-r) : 0 ;
}
float SpherePen::GetWeight(float r){
    return r<1 ? sqrt(1-r*r) : 0 ;
}
float VolcanoPen::GetWeight(float r){
    return r<1 ? r*r*(1-r)*(1-r)*16 : 0 ;
}
float PlanePen::GetWeight(float r){
    r=(r-0.75)*4;
	if(r<0)r=0;
	return r<1 ? r=1-r,r*r*(2-r)*(2-r) : 0 ;
}
float PatternPen::GetWeight(float dx,float dy){
    return P.getPixelUV((dx+1)/2,(dy+1)/2).y/255.0f;
}
float PatternPen::GetStrokedWeight(float d,float length){
    if(P_str.getWidth()!=P.getWidth()){
        P_str.CreateStroke(&P);
    }
    if(length>1)length=1;
    return P_str.getPixelUV(length/2,(d+1)/2).y/255.0f;
}
float StripPen::GetAspect(){
    if(P.getWidth()!=0){
        return float(P.getHeight())/P.getWidth();
    }else return 1.0f;
}
BasicPenShape::BasicPenShape(){
	PreviewTexture=-1;
}
int BasicPenShape::GetPreviewTexture(){	
	if(PreviewTexture==-1){
		char c[128];
		sprintf(c,"pt%X",DWORD(this));
		PreviewTexture=IRS->CreateTexture(c,64,64,cfARGB8888,1);
		CreatePreview(PreviewTexture);
	}
	return PreviewTexture;
}
void BasicPenShape::CreatePreview(int TextureID){
    int pitch;
    DWORD* data=(DWORD*)IRS->LockTexBits(TextureID,pitch);
    if(data){
        int LX=IRS->GetTextureWidth(TextureID);
        int LY=IRS->GetTextureHeight(TextureID);
        float Aspect=GetAspect();
        float mw=min(LX,LY)/2.2;
        float cx=LX>>1;
        float cy=LY>>1;
		Vector3D Light(-1,1,0.3);
		Light.normalize();
		Vector3D vdir=Vector3D::oZ;
        for(int y=0;y<LY;y++){
            for(int x=0;x<LX;x++){
                float dx=(x-cx)/mw;                
                float dy=(y-cy)/mw;
				if(Aspect>1)dx*=Aspect;
				else dy/=Aspect;
				Vector3D n=GetNxy(dx,dy);
				float d=n.dot(Light)/2+0.5f;
				d*=0.75;
				Vector3D refl=vdir-2*n*n.dot(vdir);
				float r=-refl.dot(Light);
				if(r>0)d+=pow(r,20);
				if(d>1)d=1;                
				DWORD C=DWORD(d*255);
                data[x+y*LX]=0xFF000000+(C<<16)+(C<<8)+C;
            }
        }
        IRS->UnlockTexBits(TextureID);
    }
}
Vector3D BasicPenShape::GetNxy(float x,float y){
	Vector3D v1(GetPreviewWeight(x+0.01,y)-GetPreviewWeight(x-0.01,y),GetPreviewWeight(x,y+0.01)-GetPreviewWeight(x,y-0.01),0.01);
	v1.normalize();
	return v1;
}
float StripPen::GetPreviewWeight(float dx,float dy){
    if(dx<-1)dx=-1;
    if(dx>1)dx=1;
    return GetWeight(dx,dy)*sqrt(1-dx*dx);
}
float BasicPenShape::GetPreviewWeight(float dx,float dy){
	if(dx<-1)return 0;//dx=-1;
	if(dx>1)return 0;//dx=1;
	if(dy<-1)return 0;//dy=-1;
	if(dy>1)return 0;//dy=1;
	return GetWeight(dx,dy);
}
DynArray<BasicPenShape*> GeneralPen::RadialShapes;
int GeneralPen::CurrentShape=0;
BasicPenShape* GeneralPen::CurrentShapePtr=NULL;

DynArray<BasicPenShape*> GeneralPen::Strips;
int GeneralPen::CurrentStrip=0;
BasicPenShape* GeneralPen::CurrentStripPtr=NULL;

DynArray<OnePenStyle*> GeneralPen::Pens;
int GeneralPen::CurrentPen;
OnePenStyle* GeneralPen::CurrentPenPtr=NULL;

DynArray<TexturePattern*> GeneralPen::Patterns;
int GeneralPen::CurrentPattern;
TexturePattern* GeneralPen::CurrentPatternPtr=NULL;

float GeneralPen::PenRadius=1;
float GeneralPen::DeepDegree=1;
float GeneralPen::SmoothDegree=1;
float GeneralPen::ColorOpacity=1;
float GeneralPen::SpecularOpacity=1;
float GeneralPen::SpecularDegree=1;
float GeneralPen::PenRotation=c_PI*3/2;
int GeneralPen::ViewType=-1;
int GeneralPen::MaskingType=0;
int GeneralPen::ColorOpType=0;
int GeneralPen::DeepOpType=0;
bool GeneralPen::EnableChannel[4]={true,true,false,false};
int	GeneralPen::PenOpType=0;
int	GeneralPen::PenPressureType=0;
int GeneralPen::NumAccumulatedPoints=0;
int GeneralPen::SymmetryMask=0;
int GeneralPen::SymmetryType=0;
Vector3D GeneralPen::SymmCenter=Vector3D(0,0,0);
int GeneralPen::MappingType=0;
DynArray<ColorShader*> GeneralPen::Shaders;

Vector4D GeneralPen::Color;

void ColorShader::Modify(Vector4D& src,bool sign){
	src=GeneralPen::Color;
	src*=255;
}
int ColorShader::GetIcon(){
	return 30;
}
int ColorShader::GetIconTexture(){
	return PenInterface::IconsTexture;
}
const char* ColorShader::GetHint(){
	return "COLOROP_DEFAULT_HINT";
}
void ColorShaderDesaturate::Modify(Vector4D& src,bool sign){
	float C=(src.x+src.y+src.z)/3;
	if(sign){
		src=Vector4D(C+(src.x-C)*2,C+(src.y-C)*2,C+(src.z-C)*2,src.w);
		src.x=clamp(src.x,0.0f,255.0f);
		src.y=clamp(src.y,0.0f,255.0f);
		src.z=clamp(src.z,0.0f,255.0f);
	}else{
		src=Vector4D(C,C,C,src.w);
	}		
}
int ColorShaderDesaturate::GetIcon(){
	return 31;
}
const char* ColorShaderDesaturate::GetHint(){
	return "COLOROP_DESATURATE_HINT";
}
void clamp4D(Vector4D& v){
	if(v.x<0)v.x=0;
	if(v.y<0)v.y=0;
	if(v.z<0)v.z=0;
	if(v.x>255)v.x=255;
	if(v.y>255)v.y=255;
	if(v.z>255)v.z=255;
}
void ColorShaderColorize::Modify(Vector4D& src,bool sign){
	float C=(src.x+src.y+src.z)/3;
	if(!sign){
		src=Vector4D(C+(src.x-C)*2,C+(src.y-C)*2,C+(src.z-C)*2,src.w);
		clamp4D(src);
	}else{
		src=Vector4D(C,C,C,src.w);
	}		
}
const char* ColorShaderColorize::GetHint(){
	return "COLOROP_COLORIZE_HINT";
}
int ColorShaderColorize::GetIcon(){
	return 32;
}
void ColorShaderDarken::Modify(Vector4D& src,bool sign){		
	if(sign){
		src*=1.5;
		clamp4D(src);
	}else{
		src/=1.5;
	}
}
int ColorShaderDarken::GetIcon(){
	return 33;
}
const char* ColorShaderDarken::GetHint(){
	return "COLOROP_DARKEN_HINT";
}
void ColorShaderLighten::Modify(Vector4D& src,bool sign){		
	if(!sign){
		src*=1.5;
		clamp4D(src);
	}else{
		src/=1.5;
	}	
}
int ColorShaderLighten::GetIcon(){
	return 34;
}	
const char* ColorShaderLighten::GetHint(){
	return "COLOROP_LIGHTEN_HINT";
}

void GeneralPen::AddNewPen(){
        OnePenStyle* OPS=new OnePenStyle;
        Pens.Add(OPS);
        CurrentPen=Pens.GetAmount()-1;
        CurrentPenPtr=Pens[CurrentPen];
		GeneralPen::SavePen();
}
void CubeMap(Vector3D& uv,Vector3D n){
	float ax=abs(n.x);
	float ay=abs(n.y);
	float az=abs(n.z);
	if(ax>ay && ax>az){
		uv=Vector3D(uv.y,uv.z,0);
	}else
	if(ay>ax && ay>az){
		uv=Vector3D(-uv.x,uv.z,0);
	}else
	if(az>ax && az>ay){
		uv=Vector3D(uv.x,uv.y,0);
	}
	uv*=TexturePattern::dScale/256.0;
	uv+=TexturePattern::dShift;
}
void GeneralPen::ModifyPoint(float scale,ModPointInfo& pi,float R,float R1){
	if(PenOpType==1){
		ModifyPointGrow(scale,pi,R,R1);
		return;
	}
    float r=sqrt(pi.dx*pi.dx+pi.dy*pi.dy);
    if(r<1){
		pi.N=pi.Base->N0;
        Vector3D uv=pi.Base->Pos*scale;		
		TexturePattern::ToUV(uv,&pi.Base->N);		
        float Deg=1;
        if(CurrentShapePtr){
            //if(pi.StrokeLength>0)Deg=CurrentShapePtr->GetStrokedWeight(pi.dy,pi.StrokeLength);
            //else 
            Deg=CurrentShapePtr->GetWeight(pi.dx,pi.dy);
            Deg*=1-pi.Base->Freeze;
        }
        if(CurrentPatternPtr){
            Deg*=CurrentPatternPtr->getPixelUV(pi.dx*2+1,pi.dy*2+1).y;
        }
        if(CurrentStrip>0){
            float aspect=CurrentStripPtr->GetAspect();
            Deg*=CurrentStripPtr->GetWeight(pi.yalong/aspect,pi.xalong);
        }
		Deg*=R1/R;
        if(EnableChannel[0]){			
			float DeepDeg=Deg*DeepDegree*R;
			if(CurrentPenPtr && CurrentPenPtr->EnablePattern[0]){
				if(CurrentPenPtr->DeepPattern){
					Vector4D v=CurrentPenPtr->DeepPattern->getPixelUV(uv.x,uv.y);
                    v.y-=CurrentPenPtr->DeepPattern->Average.y;
					DeepDeg*=v.y/256.0;
				}
			}			
            float OldD=abs(pi.Base->N0.dot(pi.Curr->Dv-pi.Orig->Dv));
			if(abs(DeepDeg)>OldD){
                pi.Curr->Dv=pi.Orig->Dv+pi.Base->N0*DeepDeg;
			}			
        }        
		if(EnableChannel[1] || EnableChannel[3]){
			Vector4D CL=DW2V4D(pi.Orig->Color);
			Vector4D CC=DW2V4D(pi.Curr->Color);
			float c_opacity=1;            
            float transp=0;
            bool DoInit=pi.Curr->Transparency==0;
			if(EnableChannel[1]){
				float ClrDeg=Deg*ColorOpacity;
				if(ClrDeg>1)ClrDeg=1;                                
				Vector4D C=CC;//Color;
				//C*=255.0f;
				GeneralPen::Shaders[GeneralPen::ColorOpType]->Modify(C,false);				
				if(CurrentPenPtr){
					if(CurrentPenPtr->ColorPattern && CurrentPenPtr->EnablePattern[1]){
						Vector4D v=CurrentPenPtr->ColorPattern->getPixelUV(uv.x,uv.y);
						v/=256.0;
						C.x*=v.x;C.y*=v.y;
						C.z*=v.z;
						ClrDeg*=v.w;
						c_opacity=v.w;                        
					}
					if(CurrentPenPtr->OpacityPattern && CurrentPenPtr->EnablePattern[2]){
						Vector4D v=CurrentPenPtr->OpacityPattern->getPixelUV(uv.x,uv.y);
						ClrDeg*=v.y/256.0;
						c_opacity*=v.y/256.0;
					}
				}        
                if(DoInit){
                    CL=C;
                }else{
				    float inv=1-ClrDeg;
				    CL.x=inv*CL.x+ClrDeg*C.x;
				    CL.y=inv*CL.y+ClrDeg*C.y;
				    CL.z=inv*CL.z+ClrDeg*C.z;			
				    float d=abs(CL.x-C.x)+abs(CL.y-C.y)+abs(CL.z-C.z);
				    float d2=abs(CC.x-C.x)+abs(CC.y-C.y)+abs(CC.z-C.z);
				    if(d>d2){
					    CL.x=CC.x;
					    CL.y=CC.y;
					    CL.z=CC.z;
				    }			
                }
                transp=ClrDeg;
			}
			if(EnableChannel[3]){
				float SpecDeg=SpecularDegree*255;
				float SpecOp=SpecularOpacity*c_opacity*Deg;
				if(SpecOp>1)SpecOp=1;                
				if(CurrentPenPtr->SpecularPattern && CurrentPenPtr->EnablePattern[3]){
					Vector4D v=CurrentPenPtr->SpecularPattern->getPixelUV(uv.x,uv.y);
					SpecDeg*=v.y/255.0f;
				}
				if(SpecDeg>255)SpecDeg=255;
                if(DoInit){
                    CL.w=CC.w;
                }else{
				    CL.w=CL.w*(1-SpecOp)+SpecDeg*SpecOp;			
				    if(abs(CL.w-SpecDeg)>abs(CC.w-SpecDeg)){
					    CL.w=CC.w;
				    }			
                }
			}
			pi.Curr->Color=V4D2DW(CL);
            pi.Curr->Transparency=max(pi.Curr->Transparency,transp);
		}
    }
}
void GeneralPen::ModifyPointGrow(float scale,ModPointInfo& pi,float R,float R1){
	float r=sqrt(pi.dx*pi.dx+pi.dy*pi.dy);
	if(r<1){
		Vector3D uv=pi.Base->Pos*scale;
		if(GeneralPen::MappingType==0){
			TexturePattern::ToUV(uv);
		}else{
			CubeMap(uv,pi.Base->N);			
		}
		float Deg=r<1 ?  1 : 0;
		if(CurrentShapePtr){
			if(pi.StrokeLength>0)Deg=CurrentShapePtr->GetStrokedWeight(pi.dy,pi.StrokeLength);
			else Deg=CurrentShapePtr->GetWeight(pi.dx,pi.dy);
            Deg*=1-pi.Base->Freeze;
		}
		if(CurrentPatternPtr){
			Deg*=CurrentPatternPtr->getPixelUV(pi.dx*2+1,pi.dy*2+1).y;
		}
		if(EnableChannel[0]){			
			float DeepDeg=Deg*DeepDegree*R1;
			if(CurrentPenPtr && CurrentPenPtr->EnablePattern[0]){
				if(CurrentPenPtr->DeepPattern){
					Vector4D v=CurrentPenPtr->DeepPattern->getPixelUV(uv.x,uv.y);
                    v.y-=CurrentPenPtr->DeepPattern->Average.y;
					DeepDeg*=v.y/256.0;
				}
			}			
			pi.Curr->Dv+=pi.Base->N0*DeepDeg*0.1f;
		}		
		if(EnableChannel[1] || EnableChannel[3]){
			Vector4D CL=DW2V4D(pi.Curr->Color);
			float c_opacity=1;  
            float transp=0;
            bool DoInit=pi.Curr->Transparency==0;
			if(EnableChannel[1]){
				float ClrDeg=Deg*ColorOpacity;
				if(ClrDeg>1)ClrDeg=1;
				Vector4D C=CL;//Color;
				//C*=255.0f;
				GeneralPen::Shaders[GeneralPen::ColorOpType]->Modify(C,false);
				if(CurrentPenPtr){
					if(CurrentPenPtr->ColorPattern && CurrentPenPtr->EnablePattern[1]){
						Vector4D v=CurrentPenPtr->ColorPattern->getPixelUV(uv.x,uv.y);
						v/=256.0;
						C.x*=v.x;C.y*=v.y;
						C.z*=v.z;
						ClrDeg*=v.w;
						c_opacity=v.w;
					}
					if(CurrentPenPtr->OpacityPattern && CurrentPenPtr->EnablePattern[2]){
						Vector4D v=CurrentPenPtr->OpacityPattern->getPixelUV(uv.x,uv.y);
						ClrDeg*=v.y/256.0;
						c_opacity*=v.y/256.0;
					}
				}            
				float inv=1-ClrDeg;
                if(DoInit){
                    CL=C;
                }else{
				    CL.x=inv*CL.x+ClrDeg*C.x;
				    CL.y=inv*CL.y+ClrDeg*C.y;
				    CL.z=inv*CL.z+ClrDeg*C.z;	
                }
                transp=ClrDeg;
			}
			if(EnableChannel[3]){
				float SpecDeg=SpecularDegree*255;
				if(SpecDeg>255)SpecDeg=255;
				float SpecOp=SpecularOpacity*c_opacity*Deg;
				if(SpecOp>1)SpecOp=1;
				if(CurrentPenPtr->SpecularPattern && CurrentPenPtr->EnablePattern[3]){
					Vector4D v=CurrentPenPtr->SpecularPattern->getPixelUV(uv.x,uv.y);
					SpecDeg*=v.y/255.0f;
				}
                if(DoInit)CL.w=SpecDeg;
				else CL.w=CL.w*(1-SpecOp)+SpecDeg*SpecOp;			
			}
			pi.Curr->Color=V4D2DW(CL);
            pi.Curr->Transparency=max(pi.Curr->Transparency,transp);
		}
	}
}
void GeneralPen::DrawPreview(Rct& Rect){
    
}
void GeneralPen::Handle(){

}
OnePenStyle::OnePenStyle(){
    DeepPattern=NULL;
    ColorPattern=NULL;
    OpacityPattern=NULL;
    SpecularPattern=NULL;
}
OnePenStyle::~OnePenStyle(){
    if(DeepPattern)delete(DeepPattern);
    if(ColorPattern)delete(ColorPattern);
    if(OpacityPattern)delete(OpacityPattern);
    if(SpecularPattern)delete(SpecularPattern);
    DeepPattern=NULL;
    ColorPattern=NULL;
    OpacityPattern=NULL;
    SpecularPattern=NULL;
}
void OnePenStyle::MakePreview(Rct& rct){
        
}
void OnePenStyle::MakePreview(int Index,Rct& rct){
    TexturePattern* TP=Patterns[Index];
    if(TP && TP->TextureID!=-1){
        rsSetTexture(TP->TextureID,0);
        static int sh=IRS->GetShaderID("hud");
        rsSetShader(sh);        
        rsRect(rct,Rct(0,0,1,1),0,0xFFFFFFFF);
        rsFlush();
    }else{
        rsRect(rct,0,0xFF808080);
        rsFlush();
    }
}
void OnePenStyle::SelectTexture(int Index){
    OpenFileDialog dlg;
	dlg.AddFilter( "TGA files" , "*.tga" );
	dlg.AddFilter( "BMP files" , "*.bmp" );
	dlg.AddFilter( "JPG files" , "*.jpg" );
	dlg.SetDefaultExtension( "tga" );
	_chdir(IRM->GetHomeDirectory());
    TexturePattern* &P=Patterns[Index];
    if (dlg.Show()){
        if(!P)P=new TexturePattern;
        P->clear();
        P->LoadTexture(dlg.GetFilePath());
    }
	GeneralPen::SavePen();
}
void GeneralPen::Init(){
	if(!RadialShapes.GetAmount()){
		Color=DW2V4D(RedColor);
		Color/=255.0;

        RadialShapes.Add(new SimplePen);
		RadialShapes.Add(new ConePen);
		RadialShapes.Add(new SharpPen);
		RadialShapes.Add(new SpherePen);
		RadialShapes.Add(new VolcanoPen);
		RadialShapes.Add(new PlanePen);

		FileList flist;
		CreateSortedFileList("Textures\\patterns\\","*.tga",flist,false);
		CreateSortedFileList("Textures\\patterns\\","*.bmp",flist,false);
		CreateSortedFileList("Textures\\patterns\\","*.jpg",flist,false);
		for(int i=0;i<flist.GetAmount();i++){
            PatternPen* pp=new PatternPen;
			pp->P.LoadTexture(flist[i]->pchar());
			RadialShapes.Add(pp);
		}
		flist.Clear();
        CreateSortedFileList("Textures\\strips\\","*.tga",flist,false);
		CreateSortedFileList("Textures\\strips\\","*.bmp",flist,false);
		CreateSortedFileList("Textures\\strips\\","*.jpg",flist,false);
        Strips.Add(new BasicPenShape);
		for(int i=0;i<flist.GetAmount();i++){
            StripPen* pp=new StripPen;
			pp->P.LoadTexture(flist[i]->pchar());
			Strips.Add(pp);
		}
		GeneralPen::LoadPen();		
		CurrentShape=0;
		CurrentShapePtr=RadialShapes[0];
        CurrentStrip=0;
		CurrentStripPtr=Strips[0];
		if(Pens.GetAmount()==0){
			OnePenStyle* P=new OnePenStyle;
			Pens.Add(P);
		}
		CurrentPen=0;
		CurrentPenPtr=Pens[0];	

		Shaders.Add(new ColorShader);
		Shaders.Add(new ColorShaderDesaturate);
		Shaders.Add(new ColorShaderColorize);
		Shaders.Add(new ColorShaderDarken);
		Shaders.Add(new ColorShaderLighten);
	}
}
void GeneralPen::TransformSymm(Vector3D& V,Vector3D mul){
	V=SymmCenter+(V-SymmCenter)*mul;
}
Vector3D vtmp[16];
void GeneralPen::PushSymmParam(int ofs){
    vtmp[0]=PMS.LastPickInfo.Pos;
	vtmp[1]=BasicTool::Pick.PickPos;
    vtmp[2]=SurfaceModificator::PrevPos;
    vtmp[3]=SurfaceModificator::PrevPrevPos;
    vtmp[4]=SurfaceModificator::CurrPos;
    vtmp[5]=SurfaceModificator::CenterPos;

    PMS.LastPickInfo.Pos=SurfaceModificator::CurrPosS[ofs];
	BasicTool::Pick.PickPos=SurfaceModificator::CurrPosS[ofs];
    SurfaceModificator::PrevPos=SurfaceModificator::PrevPosS[ofs];
    SurfaceModificator::PrevPrevPos=SurfaceModificator::PrevPrevPosS[ofs];
    SurfaceModificator::CurrPos=SurfaceModificator::CurrPosS[ofs];
    SurfaceModificator::CenterPos=SurfaceModificator::CenterPosS[ofs];
}
void GeneralPen::PopSymmParam(){
    PMS.LastPickInfo.Pos=vtmp[0];
	BasicTool::Pick.PickPos=vtmp[1];
    SurfaceModificator::PrevPos=vtmp[2];
    SurfaceModificator::PrevPrevPos=vtmp[3];
    SurfaceModificator::CurrPos=vtmp[4];
    SurfaceModificator::CenterPos=vtmp[5];
}
void GeneralPen::RenderOnPen(Vector3D _center,float radius,bool ShowMtl,bool ShowShape){
	bool rp=!!(GetKeyState(VK_RBUTTON)&0x8000);
	if(rp)radius*=10;	

	OnePenStyle* OPS=GeneralPen::CurrentPenPtr;	
	for(int i=0;i<3;i++){
		rsSetTexture(PenInterface::WhiteTexture,i);
		if(OPS->Patterns[i] && OPS->EnablePattern[i])rsSetTexture(OPS->Patterns[i]->TextureID,i);
	}
    static int sh=IRS->GetShaderID("obj_preview_pen");	
	ICamera* cam=GetCamera();
	int nSegments=32;
    BYTE ms=GeneralPen::SymmetryMask;
    if(rp)ms=0;
    int xm=ms&1 ? 1 : 0;
    int ym=ms&2 ? 1 : 0;
    int zm=ms&4 ? 1 : 0;
    Vector3D T,N,B;
    SurfaceModificator::GetTNB(T,N,B);
    for(int xx=0;xx<=xm;xx++){
        for(int yy=0;yy<=ym;yy++){
            for(int zz=0;zz<=zm;zz++){
                if(ms&(xx+(yy<<1)+(zz<<2)) || (xx==0 && yy==0 && zz==0)){
                    if(ShowMtl)rsSetShader(sh);			
                    else rsEnableZ(false);
                    Vector3D center=_center;
                    int ofs=xx+yy*2+zz*4;
                    if(ofs)center=SurfaceModificator::CurrPosS[ofs];					
	                Matrix4D cTM;
	                Vector3D z( cam->GetDirection() );
	                Vector3D x, y;
	                z.CreateBasis( x, y );
	                cTM.fromBasis( x, y, z, center );
	                float phiStep = c_DoublePI / float( nSegments );
	                Vector3D a, b;
	                Vector3D tc=center;					
					TexturePattern::ToUV(tc,&N);					
	                for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
	                {
		                a.set( radius * cos( phi ), radius * sin( phi ), 0.0f );
		                b.set( radius * cos( phi + phiStep), radius * sin( phi + phiStep ), 0.0f );

		                a *= cTM;
		                b *= cTM;		
		                Vector3D ta=a;						
						TexturePattern::ToUV(ta,&N);
						Vector3D tb=b;						
						TexturePattern::ToUV(tb,&N);						
		                if(ShowMtl)rsPoly( a, b, center, ta.x, ta.y, tb.x, tb.y, tc.x, tc.y,0x80FFFFFF,0x80FFFFFF,0x80FFFFFF );
                        else{
                            rsLine(a,b,0xFFFFFF00);
                        }
	                }
                    rsFlush();
                    rsRestoreShader();
                    rsEnableZ(false);
                    bool first=true;
                    Vector3D prev;
                    if(ShowShape){
                        for(float dx=-1;dx<1;dx+=0.01){
                            float Deg=GeneralPen::DeepDegree*radius;
                            if(CurrentShapePtr){
                                Deg*=CurrentShapePtr->GetWeight(dx,0);                            
                            }   
                            Vector3D C=center+T*dx*radius;
                            Vector3D uv=C;						
						    TexturePattern::ToUV(uv,&N);						
                            if(EnableChannel[0]){			                
			                    if(CurrentPenPtr && CurrentPenPtr->EnablePattern[0]){
				                    if(CurrentPenPtr->DeepPattern){
					                    Vector4D v=CurrentPenPtr->DeepPattern->getPixelUV(uv.x,uv.y);
                                        v.y-=CurrentPenPtr->DeepPattern->Average.y;
					                    Deg*=v.y/256.0;
				                    }
			                    }
                            }else Deg=0;                        
                            C-=B*Deg;
                            if(!first){
                                rsLine(prev,C,0xFFFF0000);
                            }
                            first=false;
                            prev=C;
                        }
                    }
                }
            }
        }
    }
	rsFlush();
	rsRestoreShader();
}
