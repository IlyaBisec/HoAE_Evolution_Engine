#pragma once

#include "kSystemDialogs.h"

#define pi_callback(msg,w,func) w->AddCallback(msg,new wpcallback<PenInterface>(PenInterface::instance(),&PenInterface::##func));
#define pi_callback1(msg,w,func,param1) w->AddCallback(msg,new wpcallback<PenInterface>(PenInterface::instance(),&PenInterface::##func,param1));
#define pi_callback2(msg,w,func,param1,param2) w->AddCallback(msg,new wpcallback<PenInterface>(PenInterface::instance(),&PenInterface::##func,param1,param2));
#define pi_callback3(msg,w,func,param1,param2,param3) w->AddCallback(msg,new wpcallback<PenInterface>(PenInterface::instance(),&PenInterface::##func,param1,param2,param3));
#define pi_callback4(msg,w,func,param1,param2,param3,param4) w->AddCallback(msg,new wpcallback<PenInterface>(PenInterface::instance(),&PenInterface::##func,param1,param2,param3,param4));
#define pi_callback5(msg,w,func,param1,param2,param3,param4,param5) w->AddCallback(msg,new wpcallback<PenInterface>(PenInterface::instance(),&PenInterface::##func,param1,param2,param3,param4,param5));

extern PolyMesh PMS;
DWORD V4D2DW(Vector4D& V);
const int icn_lx=4;
const int icn_ly=16;

class BasicPenShape{    
	int PreviewTexture;
public:
	BasicPenShape();
	virtual float   GetWeight(float dx,float dy){return 1;}
    virtual float   GetPreviewWeight(float dx,float dy);
    virtual float   GetStrokedWeight(float d,float length){return GetWeight(0,d);}
	void            CreatePreview(int TextureID);
	virtual int     GetPreviewTexture();
	Vector3D		GetNxy(float x,float y);
    virtual float   GetAspect(){return 1;};
};

class RadialPenShape:public BasicPenShape{    
public:
	virtual float   GetWeight(float dx,float dy){return GetWeight(sqrt(dx*dx+dy*dy));}
    virtual float   GetWeight(float r){return 1;}
	float			GetWxy(float x,float y);	
};
class ConePen:public RadialPenShape{
public:
    virtual float GetWeight(float r);    
};
class SimplePen:public RadialPenShape{
public:
    virtual float GetWeight(float r);    
};
class SharpPen:public RadialPenShape{
public:
    virtual float GetWeight(float r);    
};
class SpherePen:public RadialPenShape{
public:
    virtual float GetWeight(float r);    
};
class VolcanoPen:public RadialPenShape{
public:
    virtual float GetWeight(float r);    
};
class PlanePen:public RadialPenShape{
public:
	virtual float GetWeight(float r);	
};
class PatternPen:public BasicPenShape{
public:	
	TexturePattern P;
    TexturePattern P_str;
	virtual float GetWeight(float dx,float dy);	
    virtual float GetStrokedWeight(float d,float length);    
};
class StripPen:public PatternPen{
public:
	virtual float	GetAspect();
    virtual float   GetPreviewWeight(float dx,float dy);
};
//...
class OnePenStyle{
public:
    OnePenStyle();
    ~OnePenStyle();
    union{
        struct{
            TexturePattern* DeepPattern;
            TexturePattern* ColorPattern;
            TexturePattern* OpacityPattern;
            TexturePattern* SpecularPattern;
			bool EnableDeepPattern;
			bool EnableColorPattern;
			bool EnableOpacityPattern;
			bool EnableSpecularPattern;
        };
		struct{
			TexturePattern* Patterns[4];        
			bool EnablePattern[4];
		};
    };
    void MakePreview(Rct& rct);
    void MakePreview(int Index,Rct& rct);
    void SelectTexture(int Index);
	void ToXML(xmlQuote& xml);
	void FromXML(xmlQuote& xml);
};
struct ModPointInfo{
    int x;
    int y;
    int Face;
    float StrokeLength;
    //SurfPoint3D* Curr;
    //SurfPoint3D* Orig;
    SurfPoint3D*   Base;
    OneLayerPoint* Curr;
    OneLayerPoint* Orig;
    float dx;
    float dy;
    float xalong;
    float yalong;
    Vector3D N;
};

class ColorShader{
public:
	virtual void Modify(Vector4D& src,bool sign);
	virtual int GetIcon();
	virtual int GetIconTexture();
	virtual const char* GetHint();
};
class ColorShaderDesaturate : public ColorShader{
public:
	virtual void Modify(Vector4D& src,bool sign);
	virtual int GetIcon();
	virtual const char* GetHint();
};
class ColorShaderColorize : public ColorShader{
public:
	virtual void Modify(Vector4D& src,bool sign);	
	virtual int GetIcon();
	virtual const char* GetHint();
};
class ColorShaderDarken : public ColorShader{
public:
	virtual void Modify(Vector4D& src,bool sign);
	virtual int GetIcon();
	virtual const char* GetHint();
};
class ColorShaderLighten : public ColorShader{
public:
	virtual void Modify(Vector4D& src,bool sign);
	virtual int GetIcon();
	virtual const char* GetHint();
};
class GeneralPen{
public:    
    static DynArray<BasicPenShape*>    RadialShapes;
    static int                          CurrentShape;
    static BasicPenShape*              CurrentShapePtr;

    static DynArray<BasicPenShape*>     Strips;
    static int                          CurrentStrip;
    static BasicPenShape*               CurrentStripPtr;

    static DynArray<OnePenStyle*>       Pens;
    static int                          CurrentPen;
    static OnePenStyle*                 CurrentPenPtr;    

    static DynArray<TexturePattern*>    Patterns;
    static int                          CurrentPattern;
    static TexturePattern*              CurrentPatternPtr;

	static DynArray<ColorShader*>	    Shaders;

    static float                        PenRadius;
    static float                        DeepDegree;
	static float                        SmoothDegree;
    static float                        ColorOpacity;
    static float                        SpecularOpacity;
    static float                        SpecularDegree;
    static float                        PenRotation;
    static int                          MaskingType;
    static int                          ColorOpType;
    static int                          DeepOpType;
    static Vector4D                     Color;	
	static bool							EnableChannel[4];
	static int							PenOpType;//0-draw, 1-grow, 2-pick color
	static int							PenPressureType;//0-variate deep&radius,1-variate deep, 2-no variation,3-spots
	static int							NumAccumulatedPoints;
    static int                          SymmetryMask;// 1 - xsymmetry 2 - ysymmetry 4 - zsymmetry
    static int                          SymmetryType;
	static Vector3D						SymmCenter;
	static int							MappingType;
	static int							ViewType;

	static void							Init();
    static void                         ModifyPoint(float scale,ModPointInfo& pi,float R,float R1 );
	static void                         ModifyPointGrow(float scale,ModPointInfo& pi,float R,float R1);
	static void                         ModifyPointPick(float scale,ModPointInfo& pi,float R,float R1);
	static void							SavePen();
	static void							LoadPen();
	static void							TransformSymm(Vector3D& V,Vector3D mul);
    static void                         PushSymmParam(int ofs);
    static void                         PopSymmParam();    

    void                                DrawPreview(Rct& Rect);
    void                                Handle();
    void                                AddNewPen();
	static void							RenderOnPen(Vector3D Cender,float Radius,bool ShowMtl,bool ShowShape);
};
