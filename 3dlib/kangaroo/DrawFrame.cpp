
#include "StdAfx.h" #include "common.h" 
#include "DrawFrame.h"

class TexFrame:public BaseClass{
public:	
	TexFrame(){};
	TexFrame(Rct& uv,float a,float b,float c,float d,float e,float f){
		x=(uv.x+a*uv.w)/8;
		y=(uv.y+b*uv.h)/8;
		w=c*uv.w/8;
		h=d*uv.h/8;
		xc=(uv.x+e*uv.w)/8;
		yc=(uv.y+f*uv.h)/8;
	}
	int x,y,w,h,xc,yc;	
};

bool DrawTexFrame(int TextureID,int Lx,int Ly,TexFrame& tf,int x,int y,Rct& Window,DWORD Color){
	Rct rxy(x+tf.x-tf.xc,y+tf.y-tf.yc,tf.w,tf.h);    
	if(Window.Clip(rxy)){
		Rct ruv=rxy;
		ruv.x-=x-tf.xc-0.5f;
		ruv.y-=y-tf.yc-0.5f;
		ruv.x/=Lx;
		ruv.w/=Lx;
		ruv.y/=Ly;
		ruv.h/=Ly;		
		rsQuad2D(Vector3D(rxy.x,rxy.y,0),Vector3D(rxy.x+rxy.w,rxy.y,0),Vector3D(rxy.x,rxy.y+rxy.h,0),Vector3D(rxy.x+rxy.w,rxy.y+rxy.h,0),Color,Color,Color,Color,ruv);
		return true;
	}
	return false;
}
void DrawFrame(Rct& Rect,int Texture,Rct& uvc,DWORD Color){
    if(Texture==-1){
        static int T=IRS->GetTextureID("stdbutton.dds");
        Texture=T;
    }
	int Lx=IRS->GetTextureWidth(Texture);
	int Ly=IRS->GetTextureHeight(Texture);

	Rct uv=uvc;
	uv.x*=Lx;
	uv.y*=Ly;
	uv.w*=Lx;
	uv.h*=Ly;

	float x=Rect.x;
	float y=Rect.y;
	float x1=Rect.x+Rect.w-1;
	float y1=Rect.y+Rect.h-1;
	TexFrame LT(uv,0,0,2,2,1,1);
	TexFrame RT(uv,6,0,2,2,7,1);
	TexFrame LB(uv,0,6,2,2,1,7);
	TexFrame RB(uv,6,6,2,2,7,7);
	TexFrame Top(uv,2,0,4,2,2,1);
	TexFrame Bottom(uv,2,6,4,2,2,7);
	TexFrame Left(uv,0,2,2,4,1,2);
	TexFrame Right(uv,6,2,2,4,7,2);
	TexFrame TexFiller(uv,2,2,4,4,2,2);
	float FillerLeftMargin = Lx/8;
	float FillerTopMargin = Ly/8;
	float FillerRightMargin = Lx/8;
	float FillerBottomMargin = Ly/8;

	float RealLx=10000;
	float RealLy=10000;
    
	rsSetTexture(Texture);
	static int sh=IRS->GetShaderID("hud2");
	rsSetShader(sh);
	
	//filler
	if(TexFiller.w>0 && TexFiller.h>0){
		Rct Win(x+FillerLeftMargin,y+FillerTopMargin,x1-x-FillerLeftMargin-FillerRightMargin,y1-y-FillerTopMargin-FillerBottomMargin);
		int Nx=2+(Win.w/TexFiller.w);
		int Ny=2+(Win.h/TexFiller.h);
		TexFiller.xc=TexFiller.x;
		TexFiller.yc=TexFiller.y;
		for(int ix=0;ix<Nx;ix++){
			for(int iy=0;iy<Ny;iy++){
				DrawTexFrame(Texture,Lx,Ly,TexFiller,x+ix*TexFiller.w,y+iy*TexFiller.h,Win,Color);
			}
		}
	}
	//drawing corners
	DrawTexFrame(Texture,Lx,Ly,LT,x,y,Rct(0,0,(x+x1)/2,(y+y1)/2),Color);
	DrawTexFrame(Texture,Lx,Ly,RT,x1,y,Rct((x+x1)/2,0,RealLx-(x+x1)/2,(y+y1)/2),Color);
	DrawTexFrame(Texture,Lx,Ly,LB,x,y1,Rct(0,(y+y1)/2,(x+x1)/2,RealLy-(y+y1)/2),Color);
	DrawTexFrame(Texture,Lx,Ly,RB,x1,y1,Rct((x+x1)/2,(y+y1)/2,RealLx-(x+x1)/2,RealLy-(y+y1)/2),Color);        
	//drawing h/v lines
	if(Top.w>0){
		Top.xc=Top.x;
		float w=x1-x-(LT.x+LT.w-LT.xc)-(RT.xc-RT.x);
		if(w>0){
			//Rct TopWin(x+LT.w+LT.x-LT.xc,0,w,RealLy);
			Rct TopWin(x+LT.w+LT.x-LT.xc,0,w,(y+y1)/2);
			int N=1+(TopWin.w/Top.w);
			for(int i=0;i<N;i++){
				DrawTexFrame(Texture,Lx,Ly,Top,TopWin.x+i*Top.w,y,TopWin,Color);
			}
		}
	}
	if(Bottom.w>0){
		Bottom.xc=Bottom.x;
		float w=x1-x-(LB.x+LB.w-LB.xc)-(RB.xc-RB.x);
		if(w>0){
			//Rct BtmWin(x+LB.w+LB.x-LB.xc,0,w,RealLy);
			Rct BtmWin(x+LB.w+LB.x-LB.xc,(y+y1)/2,w,RealLy-(y+y1)/2);
			int N=1+(BtmWin.w/Bottom.w);
			for(int i=0;i<N;i++){
				DrawTexFrame(Texture,Lx,Ly,Bottom,BtmWin.x+i*Bottom.w,y1,BtmWin,Color);
			}
		}
	}
	if(Left.h>0){
		Left.yc=Left.y;
		float h=y1-y-(LT.y+LT.h-LT.yc)-(LB.yc-LB.y);
		if(h>0){
			//Rct LeftWin(0,y+LT.y+LT.h-LT.yc,RealLx,h);
			Rct LeftWin(0,y+LT.y+LT.h-LT.yc,(x+x1)/2,h);
			int N=1+(LeftWin.h/Left.h);
			for(int i=0;i<N;i++){
				DrawTexFrame(Texture,Lx,Ly,Left,x,LeftWin.y+i*Left.h,LeftWin,Color);
			}
		}
	}
	if(Right.h>0){
		Right.yc=Left.y;
		float h=y1-y-(RT.y+RT.h-RT.yc)-(RB.yc-RB.y);
		if(h>0){
			//Rct RightWin(0,y+RT.y+LT.h-RT.yc,RealLx,h);
			Rct RightWin((x+x1)/2,y+RT.y+LT.h-RT.yc,RealLx-(x+x1)/2,h);
			int N=1+(RightWin.h/Right.h);
			for(int i=0;i<N;i++){
				DrawTexFrame(Texture,Lx,Ly,Right,x1,RightWin.y+i*Right.h,RightWin,Color);
			}
		}
	}        
	rsFlush();
	rsSetTexture( -1 );
	rsRestoreShader();
}