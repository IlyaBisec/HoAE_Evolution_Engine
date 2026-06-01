#include "common.h"
#include "comms\comms.h"
#include "comms\csplinebezier.h"
#include "UndoMaster.h"
#include "BasicTool.h"
#include "SculptTool.h"

SculptTool m_SculptTool;

SculptTool::SculptTool(){
    InstallTool(&m_SculptTool,"SculptTool");
    m_Active=false;
}
IconInfo SculptTool::GetIconInfo(){
    IconInfo I;
    I.IconID=35;
    return I;
}
void SculptTool::Modify(Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M){
    Vector3D v1=pos*M->Scale;
    float d=v1.distance(SurfaceModificator::CurrPos)*1.2/SurfaceModificator::Radius;
    d=1-d*d;    
    if(d>0){
        d*=d;
        pos+=AvgNormal*d*SurfaceModificator::Radius*GeneralPen::DeepDegree*(0.2f/M->Scale);
    }
    smd=0;
}
void SculptTool::ModifySmooth(Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M){
    Vector3D v1=pos*M->Scale;
    float d=v1.distance(SurfaceModificator::CurrPos)/SurfaceModificator::Radius;
    d=1-d*d;    
    smd=0;    
    if(d>0){        
        smd=d*d;  
    }
}
void SculptTool::ModifyTangent(Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M){
	Vector3D v1=pos*M->Scale;
	float d=v1.distance(SurfaceModificator::CurrPos)*1.2/SurfaceModificator::Radius;
	d=1-d*d;    
	if(d>0){
		d*=d;
        Vector3D dd=SurfaceModificator::CurrPos-SurfaceModificator::PrevPos;
        Vector3D dn=AvgNormal*AvgNormal.dot(dd);
        dd-=dn;
		pos+=dd*d*(GeneralPen::DeepDegree/M->Scale);
	}
    smd=0;
}
void SculptTool::ModifyMove(Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M){
	Vector3D v1=pos*M->Scale;
	float d=v1.distance(SurfaceModificator::CurrPos)*1.2/SurfaceModificator::Radius;
	d=1-d*d;    
	if(d>0){
		d*=d;
		pos+=(SurfaceModificator::CurrPos-SurfaceModificator::PrevPos)*d*(GeneralPen::DeepDegree/M->Scale);
	}
    smd=0;
}
void SculptTool::ModifyCollapse(Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M){    
    Vector3D v1=pos*M->Scale;
    Vector3D dv=v1-SurfaceModificator::CurrPos;
    Vector3D dvN=AvgNormal*AvgNormal.dot(dv);
    dv-=dvN;
    float d=v1.distance(SurfaceModificator::CurrPos)*1.2/SurfaceModificator::Radius;
    d=1-d*d;    
    if(d>0){
        d*=d;
        d*=1-d;
        dv.normalize();
        pos+=dv*d*SurfaceModificator::Radius*GeneralPen::DeepDegree*(0.1f/M->Scale);
    }    
    smd=0;
}
void SculptTool::ModifyPlanar(Vector3D& pos,float& smd,Vector3D AvgNormal,PolyMesh* M){
	Vector3D v1=pos*M->Scale;
	Vector3D dv=v1-SurfaceModificator::CurrPos;
	Vector3D dvN=AvgNormal*AvgNormal.dot(dv);
	dv-=dvN;
	float d=v1.distance(SurfaceModificator::CurrPos)*1.2/SurfaceModificator::Radius;
	d=1-d*d;
	smd=0;
	if(d>0){
		d*=d;
		dvN*=(1-d*0.5);
		dv+=dvN;
		dv+=SurfaceModificator::CurrPos;
		dv/=M->Scale;
		pos=dv;
		smd=d*0.2;
	}    	
}
void SculptTool::Apply(){
	float smooth=0.00;
    float dd=GeneralPen::DeepDegree;
	fnVertModificator* mod=&Modify;
	if(GetKeyState(VK_SHIFT)&0x8000){
		smooth=GeneralPen::SmoothDegree;//*0.3f;
        mod=&ModifySmooth;
	}
    if(GetKeyState(VK_CONTROL)&0x8000){
		GeneralPen::DeepDegree*=-1;
	}
	if(GetKeyState('Z')&0x8000){
		mod=&ModifyMove;
	}    
    if(GetKeyState('X')&0x8000){
		mod=&ModifyTangent;
	}    
    if(GetKeyState('Q')&0x8000){
        GeneralPen::DeepDegree*=-1;
		mod=&ModifyCollapse;
	}    
    if(GetKeyState('A')&0x8000){
		mod=&ModifyCollapse;
	}    
	if(GetKeyState('W')&0x8000){
		mod=&ModifyPlanar;
	}    
	SurfaceModificator::ApplySculptModification(mod,smooth);
	if(GeneralPen::SymmetryMask){
		int xm=GeneralPen::SymmetryMask&1?1:0;
		int ym=GeneralPen::SymmetryMask&2?1:0;
		int zm=GeneralPen::SymmetryMask&4?1:0;
		for(int x=0;x<=xm;x++){
			for(int y=0;y<=ym;y++){
				for(int z=0;z<=zm;z++){
					if(x||y||z){
						Vector3D mul(x?-1:1,y?-1:1,z?-1:1);
                        int ofs=x+y*2+z*4;                                    
                        GeneralPen::PushSymmParam(ofs);
						SurfaceModificator::CreateVertList();
						SurfaceModificator::ApplySculptModification(mod,smooth);
                        GeneralPen::PopSymmParam();						
					}
				}
			}
		}    
	}
    GeneralPen::DeepDegree=dd;
}
void SculptTool::Process(){    
    if(BasicTool::Pick.OnObject && Widgets::lPressed && !BasicTool::Locked){
        Apply();
    }       
}