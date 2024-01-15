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
int GeneralPen::MaskingType=0;
int GeneralPen::ColorOpType=0;
int GeneralPen::DeepOpType=0;
bool GeneralPen::EnableChannel[4]={true,true,false,false};
int	GeneralPen::PenOpType=0;
int	GeneralPen::PenPressureType=0;
int GeneralPen::SymmetryMask=0;
int GeneralPen::SymmetryType=0;
Vector3D GeneralPen::SymmCenter=Vector3D(0,0,0);

Vector4D GeneralPen::Color=Vector4D(0,0,1,1);

void GeneralPen::AddNewPen(){
        OnePenStyle* OPS=new OnePenStyle;
        Pens.Add(OPS);
        CurrentPen=Pens.GetAmount()-1;
        CurrentPenPtr=Pens[CurrentPen];
		GeneralPen::SavePen();
}
void GeneralPen::ModifyPoint(float scale,ModPointInfo& pi,float R,float R1){
	if(PenOpType==1){
		ModifyPointGrow(scale,pi,R,R1);
		return;
	}
    float r=sqrt(pi.dx*pi.dx+pi.dy*pi.dy);
    if(r<1){
        Vector3D uv=pi.Orig->Pos*scale;
        TexturePattern::ToUV(uv);
        float Deg=1;
        if(CurrentShapePtr){
            //if(pi.StrokeLength>0)Deg=CurrentShapePtr->GetStrokedWeight(pi.dy,pi.StrokeLength);
            //else 
            Deg=CurrentShapePtr->GetWeight(pi.dx,pi.dy);
            Deg*=1-pi.Curr->Freeze;
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
			float OldD=abs(pi.N.dot(pi.Curr->Pos-pi.Orig->Pos));
			if(abs(DeepDeg)>OldD){
				pi.Curr->Pos=pi.Orig->Pos+pi.N*DeepDeg;
			}			
        }
		if(EnableChannel[1] || EnableChannel[3]){
			Vector4D CL=DW2V4D(pi.Orig->Color);
			Vector4D CC=DW2V4D(pi.Curr->Color);
			float c_opacity=1;
			if(EnableChannel[1]){
				float ClrDeg=Deg*ColorOpacity;
				if(ClrDeg>1)ClrDeg=1;
				Vector4D C=Color;
				C*=255.0f;
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
			if(EnableChannel[3]){
				float SpecDeg=SpecularDegree*255;
				float SpecOp=SpecularOpacity*c_opacity*Deg;
				if(SpecOp>1)SpecOp=1;
				if(CurrentPenPtr->SpecularPattern && CurrentPenPtr->EnablePattern[3]){
					Vector4D v=CurrentPenPtr->SpecularPattern->getPixelUV(uv.x,uv.y);
					SpecDeg*=v.y/255.0f;
				}
				if(SpecDeg>255)SpecDeg=255;
				CL.w=CL.w*(1-SpecOp)+SpecDeg*SpecOp;			
				if(abs(CL.w-SpecDeg)>abs(CC.w-SpecDeg)){
					CL.w=CC.w;
				}			
			}
			pi.Curr->Color=V4D2DW(CL);
		}
    }
}
void GeneralPen::ModifyPointGrow(float scale,ModPointInfo& pi,float R,float R1){
	float r=sqrt(pi.dx*pi.dx+pi.dy*pi.dy);
	if(r<1){
		Vector3D uv=pi.Orig->Pos*scale;
		TexturePattern::ToUV(uv);
		float Deg=r<1 ?  1 : 0;
		if(CurrentShapePtr){
			if(pi.StrokeLength>0)Deg=CurrentShapePtr->GetStrokedWeight(pi.dy,pi.StrokeLength);
			else Deg=CurrentShapePtr->GetWeight(pi.dx,pi.dy);
            Deg*=1-pi.Curr->Freeze;
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
			pi.Curr->Pos+=pi.N*DeepDeg*0.1f;
		}		
		if(EnableChannel[1] || EnableChannel[3]){
			Vector4D CL=DW2V4D(pi.Curr->Color);
			float c_opacity=1;
			if(EnableChannel[1]){
				float ClrDeg=Deg*ColorOpacity;
				if(ClrDeg>1)ClrDeg=1;
				Vector4D C=Color;
				C*=255.0f;
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
				CL.x=inv*CL.x+ClrDeg*C.x;
				CL.y=inv*CL.y+ClrDeg*C.y;
				CL.z=inv*CL.z+ClrDeg*C.z;			
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
				CL.w=CL.w*(1-SpecOp)+SpecDeg*SpecOp;			
			}
			pi.Curr->Color=V4D2DW(CL);
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
	}
}
void GeneralPen::TransformSymm(Vector3D& V,Vector3D mul){
	V=SymmCenter+(V-SymmCenter)*mul;
}
void GeneralPen::RenderOnPen(Vector3D _center,float radius){
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
                    rsSetShader(sh);			
                    Vector3D center=_center;					
					if(xx)center.x=GeneralPen::SymmCenter.x*2-center.x;
                    if(yy)center.y=GeneralPen::SymmCenter.y*2-center.y;
                    if(zz)center.z=GeneralPen::SymmCenter.z*2-center.z;                
	                Matrix4D cTM;
	                Vector3D z( cam->GetDirection() );
	                Vector3D x, y;
	                z.CreateBasis( x, y );
	                cTM.fromBasis( x, y, z, center );
	                float phiStep = c_DoublePI / float( nSegments );
	                Vector3D a, b;
	                Vector3D tc=center;
	                TexturePattern::ToUV(tc);
	                for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
	                {
		                a.set( radius * cos( phi ), radius * sin( phi ), 0.0f );
		                b.set( radius * cos( phi + phiStep), radius * sin( phi + phiStep ), 0.0f );

		                a *= cTM;
		                b *= cTM;		
		                Vector3D ta=a;
		                TexturePattern::ToUV(ta);
		                Vector3D tb=b;
		                TexturePattern::ToUV(tb);
		                rsPoly( a, b, center, ta.x, ta.y, tb.x, tb.y, tc.x, tc.y,0x80FFFFFF,0x80FFFFFF,0x80FFFFFF );
	                }
                    rsFlush();
                    rsRestoreShader();
                    rsEnableZ(false);
                    bool first=true;
                    Vector3D prev;
                    for(float dx=-1;dx<1;dx+=0.01){
                        float Deg=GeneralPen::DeepDegree*radius;
                        if(CurrentShapePtr){
                            Deg*=CurrentShapePtr->GetWeight(dx,0);                            
                        }   
                        Vector3D C=center+T*dx*radius;
                        Vector3D uv=C;
                        TexturePattern::ToUV(uv);
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
	rsFlush();
	rsRestoreShader();
}
