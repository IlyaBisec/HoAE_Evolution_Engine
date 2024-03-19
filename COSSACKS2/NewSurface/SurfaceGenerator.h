#ifndef __SURFACEGENERATOR__
#define	__SURFACEGENERATOR__

#include "..\stdheader.h"
#include "surface.h"
#include "..\Extensions.h"
#include "..\SquareMap.h"
#define surfScale (1.0f/(240*64))
#define MaxDistSet 64

class DistField{
public:
	static SquareMap<float>* map[MaxDistSet];
	static void ClearMaps();
	static float Get(int Index,float x,float y);
};

struct PrecompiledVertex{
	int Angle;
	int Height;
	int Azimut;
	OneMapVertex* Vertex;
	int VertexIndex;
	void CreateFromVertex(OneMapVertex& V);
};

class OneGenCondition:public BaseClass{
public:
	SAVE(OneGenCondition);	
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V){
		return 1.0f;
	}
};

class OneSurfaceRule:public BaseClass{
public:
	_str Description;
	bool Enabled;

	int Map;
	ClassArray<OneGenCondition> ExtraConditions;

	int TopHeight;
	int BoundWidth;

	SubSection Variate_Hill_Width_Params;

	bool VariateWidth;
	int  ChangedWidth;
	float RandomShiftModulator;
	float ShapeCurvingModulator;
	_fractal WidthVariationLow;

	SubSection Variate_Edge_Curvature_Params;

	bool AllowShapeCurving;
	_fractalLine ShapeCurving;

	SubSection Variate_Top_Height_Params;

	bool AllowModulation;
	bool ModulateAfterTexturing;
	float ModulationFactor;
	_fractal ModulationVariation;
	_fractal GlobalModulationVariation;

	SubSection Variate_Vertical_Slope_Params;

	bool AllowRandomShift;
	float DegreeOfRandomShift;
	_fractal RandomShiftFractal;	

	OneSurfaceRule(){		
		Enabled=true;
		TopHeight=150;
		BoundWidth=32;

		AllowShapeCurving=false;
		AllowModulation=false;
		ModulateAfterTexturing=true;
		ModulationFactor=1.0f;		

		AllowRandomShift=false;
		DegreeOfRandomShift=0.5f;		
	}

	SAVE(OneSurfaceRule);
	SAVE_SECTION(0xFFFFFFFF);
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);		

	REG_ENUM(_index,Map,LAYERSLIST);
	REG_AUTO(ExtraConditions);

	REG_MEMBER(_int,TopHeight);
	REG_MEMBER(_int,BoundWidth);

	REG_AUTO(Variate_Hill_Width_Params);

	REG_MEMBER(_bool,VariateWidth);

	SAVE_SECTION(16);
	REG_MEMBER(_int,ChangedWidth);		
	REG_AUTO(WidthVariationLow);	
	SAVE_SECTION(0xFFFFFFFF);

	REG_AUTO(Variate_Edge_Curvature_Params);

	REG_MEMBER(_bool,AllowShapeCurving);
	SAVE_SECTION(64);
	REG_MEMBER(_float,ShapeCurvingModulator);	
	SAVE_SECTION(2);
	REG_AUTO(ShapeCurving);
	SAVE_SECTION(0xFFFFFFFF);

	REG_AUTO(Variate_Top_Height_Params);

	REG_MEMBER(_bool,AllowModulation);
	SAVE_SECTION(4);
	REG_MEMBER(_bool,ModulateAfterTexturing);
	REG_MEMBER(_float,ModulationFactor);
	REG_AUTO(ModulationVariation);
	REG_AUTO(GlobalModulationVariation);
	SAVE_SECTION(0xFFFFFFFF);

	REG_AUTO(Variate_Vertical_Slope_Params);

	REG_MEMBER(_bool,AllowRandomShift);
	SAVE_SECTION(32);
	REG_MEMBER(_float,RandomShiftModulator);
	SAVE_SECTION(8);
	REG_MEMBER(_float01,DegreeOfRandomShift);
	REG_AUTO(RandomShiftFractal);
	SAVE_SECTION(0xFFFFFFFF);
	ENDSAVE;
	virtual DWORD GetClassMask(){
		return (AllowShapeCurving?2:0)
			+(AllowModulation?4:0)+(AllowRandomShift?8:0)
			+(VariateWidth?16:0)+512+(VariateWidth&&AllowRandomShift?32:0)
			+(VariateWidth&&AllowShapeCurving?64:0);
	}
	float GetHeight(int x,int y,float& Weight,int z=0);
	float PerformShift(Vector3D V,Vector3D& DV);//returns weight of vector shift
	virtual const char* GetThisElementView(const char* Local){
		return Description.str;
	};
};

class OneSurfaceType:public BaseClass{
public:	
	_str Description;
	bool Enabled;
	bool UseMap;
	//MapRef Map;
	int Map;
	int BasicHeight;
	ClonesArray<OneSurfaceRule> SurfaceRules;
	OneSurfaceType(){
		Enabled=true;
	}
	SAVE(OneSurfaceType);
	SAVE_SECTION(0xFFFFFFFF);
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);
	REG_MEMBER(_bool,UseMap);
	SAVE_SECTION(1);
	//REG_AUTO(Map);
	REG_ENUM(_index,Map,LAYERSLIST);
	SAVE_SECTION(0xFFFFFFFF);
	REG_MEMBER(_int,BasicHeight);
	REG_AUTO(SurfaceRules);
	ENDSAVE;
	virtual DWORD GetClassMask(){
		return UseMap?1:2;
	}
	virtual const char* GetThisElementView(const char* Local){
		return Description.str;
	};
};

class OneTexturingCondition:public OneGenCondition{
public:
	bool Enabled;
	OneTexturingCondition(){
		Enabled=true;
	}
	SAVE(OneTexturingCondition);
	REG_MEMBER(_bool,Enabled);
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V){
		return 1.0f;
	}
};
class OneTexturePair:public BaseClass{
public:
	DWORD Color;
	int Facture;
	OneTexturePair(){
		Color=0xFF808080;
		Facture=0;
	}
	SAVE(OneTexturePair);
	REG_MEMBER(_color,Color);
	REG_MEMBER(_gfacture,Facture);
	ENDSAVE;
	bool ForceSimplification(){return true;}
};

class CheckSpecialLayer:public OneTexturingCondition{
public:
	//MapRef Layer;
	int Layer;
	SAVE(CheckSpecialLayer);
	REG_PARENT(OneTexturingCondition);
	REG_BASE(OneGenCondition);
	//REG_AUTO(Layer);
	REG_ENUM(_index,Layer,LAYERSLIST);
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V);
	const char* GetThisElementView(const char* LocalName){
		char* B=GetGlobalBuffer();
		Enumerator* E=ENUM.Get("LAYERSLIST");
		sprintf(B,"%s: %s",LocalName,E->Get(Layer));
		return B;
	}
};
class CheckSpecialSurfaceType:public OneTexturingCondition{
public:
	//MapRef Layer;
	int SurfaceType;
	SAVE(CheckSpecialSurfaceType);
	REG_PARENT(OneTexturingCondition);
	REG_BASE(OneGenCondition);
	//REG_AUTO(Layer);
	REG_ENUM(_index,SurfaceType,SURFACETYPE);
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V);
	const char* GetThisElementView(const char* LocalName){
		char* B=GetGlobalBuffer();
		Enumerator* E=ENUM.Get("SURFACETYPE");
		sprintf(B,"%s: %s",LocalName,E->Get(SurfaceType));
		return B;
	}
};

class CheckIfNotInLayer:public OneTexturingCondition{
public:
	//MapRef Layer;
	int Layer;
	SAVE(CheckIfNotInLayer);
	REG_PARENT(OneTexturingCondition);
	REG_BASE(OneGenCondition);
	//REG_AUTO(Layer);
	REG_ENUM(_index,Layer,LAYERSLIST);
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V);
	const char* GetThisElementView(const char* LocalName){
		char* B=GetGlobalBuffer();
		Enumerator* E=ENUM.Get("LAYERSLIST");
		sprintf(B,"%s: %s",LocalName,E->Get(Layer));
		return B;
	}
};

class CheckIfNearLayer:public OneTexturingCondition{
public:
	//MapRef Layer;
	int Layer;
	int StartDistance;
	int FinalDistance;
	SAVE(CheckIfNearLayer);
	REG_PARENT(OneTexturingCondition);
	REG_BASE(OneGenCondition);
	REG_ENUM(_index,Layer,LAYERSLIST);
	REG_MEMBER(_int,StartDistance);
	REG_MEMBER(_int,FinalDistance);
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V);
	const char* GetThisElementView(const char* LocalName){
		char* B=GetGlobalBuffer();
		Enumerator* E=ENUM.Get("LAYERSLIST");
		sprintf(B,"%s: %s (%d-%d)",LocalName,E->Get(Layer),StartDistance,FinalDistance);
		return B;
	}
};

class CheckHeight:public OneTexturingCondition{
public:
	int StartHeight;
	int FinalHeight;
	int HeightDiff;
	SAVE(CheckHeight);
	REG_PARENT(OneTexturingCondition);
	REG_BASE(OneGenCondition);
	REG_MEMBER(_int,StartHeight);
	REG_MEMBER(_int,FinalHeight);
	REG_MEMBER(_int,HeightDiff);	
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V);
	const char* GetThisElementView(const char* LocalName){
		char* B=GetGlobalBuffer();
		sprintf(B,"%s   H:%d-%d",LocalName,StartHeight,FinalHeight);
		return B;
	}

};

class CheckHeightAndAngle:public OneTexturingCondition{
public:
	int StartHeight;
	int FinalHeight;
	int HeightDiff;
	int StartAngle;
	int FinalAngle;
	int AngleDiff;

	SAVE(CheckHeightAndAngle);
	REG_PARENT(OneTexturingCondition);
	REG_MEMBER(_int,StartHeight);
	REG_MEMBER(_int,FinalHeight);
	REG_MEMBER(_int,HeightDiff);
	REG_MEMBER(_int,StartAngle);
	REG_MEMBER(_int,FinalAngle);
	REG_MEMBER(_int,AngleDiff);
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V);
	const char* GetThisElementView(const char* LocalName){
		char* B=GetGlobalBuffer();
		sprintf(B,"%s   H:%d-%d A:%d-%d",LocalName,StartHeight,FinalHeight,StartAngle,FinalAngle);
		return B;
	}
};

class CheckVariatedHeightAndAngle:public OneTexturingCondition{
public:
	int StartHeight;
	int FinalHeight;
	int VariatedAddHeight;
	_fractal VariationLow;
	int HeightDiff;
	int StartAngle;
	int FinalAngle;
	int AngleDiff;

	SAVE(CheckVariatedHeightAndAngle);
	REG_PARENT(OneTexturingCondition);
	REG_MEMBER(_int,StartHeight);
	REG_MEMBER(_int,FinalHeight);
	REG_MEMBER(_int,VariatedAddHeight);
	REG_AUTO(VariationLow);
	REG_MEMBER(_int,HeightDiff);
	REG_MEMBER(_int,StartAngle);
	REG_MEMBER(_int,FinalAngle);
	REG_MEMBER(_int,AngleDiff);
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V);
};

void GetTexString(char* dest,ClonesArray<OneTexturePair>& tex);

class TexPairList:public  ClonesArray<OneTexturePair>{
public:
	virtual const char* GetThisElementView(const char* Local){
		char* s=GetGlobalBuffer();
		GetTexString(s,*this);
		return s;
	};
};

class OneTexturingRule:public BaseClass{
public:
	OneTexturingRule(){
		UseGlobalWeight=0;
		GlobalWeight=1.0f;
		Enabled=true;
	}
	_str Description;
	bool Enabled;
	ClassArray<OneTexturingCondition> Conditions;

	bool UseGlobalWeight;
	float GlobalWeight;
	_fractal GlobalWeightVariation;

	_fractal TexturesMixing;

	TexPairList Textures;	

	SAVE(OneTexturingRule);
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);
	REG_AUTO(Conditions);		
	REG_MEMBER(_bool,UseGlobalWeight);
	SAVE_SECTION(1);
	REG_MEMBER(_float0_10,GlobalWeight);
	REG_AUTO(GlobalWeightVariation);
	SAVE_SECTION(0xFFFFFFFF);
	REG_AUTO(TexturesMixing);
	REG_AUTO(Textures);
	ENDSAVE;
	DWORD GetClassMask(){
		return UseGlobalWeight?1:2;
	}
	virtual const char* GetThisElementView(const char* Local){
		char* s=GetGlobalBuffer();
		GetTexString(s,Textures);
		if(Description.str)strcat(s,Description.str);
		return s;
	};
};

class OneTexturingStyle:public BaseClass{
public:
	OneTexturingStyle(){
		Enabled=true;
	}
	_str Description;
	bool Enabled;
	//bool UseMap;
	//MapRef Map;
	//int Map;
	ClassArray<OneTexturingCondition> Conditions;
	ClonesArray<OneTexturingRule> TexturingRules;
	SAVE(OneTexturingStyle);
	REG_AUTO(Description);
	SAVE_SECTION(0xFFFFFFFF);
	REG_MEMBER(_bool,Enabled);
	//REG_MEMBER(_bool,UseMap);
	//SAVE_SECTION(1);
	//REG_AUTO(Map);
	//REG_ENUM(_index,Map,LAYERSLIST);
	REG_AUTO(Conditions);
	SAVE_SECTION(0xFFFFFFFF);
	REG_AUTO(TexturingRules);
	ENDSAVE;
	//virtual DWORD GetClassMask(){
	//	return UseMap?1:2;
	//}
	virtual const char* GetThisElementView(const char* Local){
		return Description.str;
	};
};

class SurfaceGenerator;
class GeneratorSurfaceFunction:public BaseSurfaceFunction{
public:

	int MapLx,MapLy;
	_fractalLine* ShapeOfBound;

	SurfaceGenerator* GPtr;
	virtual void ModifyVertex(OneMapVertex& V);
	virtual bool ShiftVertex(OneMapVertex& V);
};
class SurfaceGenerator:public BaseClass{
public:
	void PerformTest();
	class ClearSurface:public BaseFunction{
	public:
		void EvaluateFunction(){
			for(int i=0;i<MLayers->Maps.GetAmount();i++){
				MLayers->Maps[i]->Clear();
			}
		};
	};
	MapRef Maps;
	ClearSurface ClearMaps;
	int Size;
	_fractalLine ShapeOfBound;
	ClonesArray<OneSurfaceType> Relief;
	ClonesArray<OneTexturingStyle> Texturing;
	bool DontGenerateRelief;
	SAVE(SurfaceGenerator);
	REG_AUTO2(Maps,"-grid 2000 -maxLx 480 -maxLy 480");	
	REG_MEMBER(_int,Size);
	REG_AUTO(Relief);
	REG_AUTO(Texturing);
	REG_MEMBER(_bool,DontGenerateRelief);
	REG_AUTO(ShapeOfBound);
	REG_AUTO(ClearMaps);
	ENDSAVE;
	float GetHeight(int x,int y);
	bool PerformShift(Vector3D& V);
	void FillVertexTextures(PrecompiledVertex* V);	
	void TexturizeSurface(VertexBasedMesh& M);
	void TexturizeSurface(VertexBasedMesh& M,int xc,int yc,int R1,int R2);
};

class OneLayerColor:public BaseClass{
public:
	_str Name;
	DWORD Color;
	bool ThisLayerDeterminesRace;
	bool DontGenerateWater;
	Delimiter D;
	SAVE(OneLayerColor);
	REG_AUTO(Name);
	REG_MEMBER(_color,Color);
	REG_MEMBER(_bool,ThisLayerDeterminesRace);
	REG_MEMBER(_bool,DontGenerateWater);
	REG_AUTO(D);
	ENDSAVE;
	bool ForceSimplification(){return true;}
};

class LayersList:public BaseClass{
public:
	ClonesArray<OneLayerColor> Layers;
	SAVE(LayersList);
	REG_AUTO(Layers);
	ENDSAVE;
};
extern LayersList LAYERSLIST;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class OneVertexTempDeformator:public BaseClass{
public:	
	OneVertexTempDeformator(){};
	~OneVertexTempDeformator(){};
	void Init(int VertexIndex);
	int VertexIndex;
	int MaxFinalWeight;
    OneMapVertex StartVertex;
	OneMapVertex DestVertex;
	OneMapVertex CurVertex;
	int  StartTime;
	int  PrevTime;
	int  TimeToWait;
	int  TimeToTransformToFinalState;
	int  TimeToStayInFinalState;
	int  TimeToReturnInStartState;
	bool DestVertexDone;
	bool ForeverChange;
	//////////////////////////////////////////////////////////////////////////	
	void BlendVertsCoor    (OneMapVertex& V1,OneMapVertex& V2,int W1,int W2,OneMapVertex& Dest);
	void BlendVertsColors  (OneMapVertex& V1,OneMapVertex& V2,int W1,int W2,OneMapVertex& Dest);
	void BlendVerts        (OneMapVertex& V1,OneMapVertex& V2,int W1,int W2,OneMapVertex& Dest);
	virtual bool PerformDeformation();//returns false if nothing changed
	virtual void CalcDestVertex()  {};
	virtual void OnStart()         {};
	virtual void OnBeginToStay()   {};
	virtual void OnEndStay()       {};
	virtual void OnEnd()           {};
};
class EffectToBorn{
public:
	_str  WeaponToBorn;
	float WeaponScale;
	int   WeaponBornProbability;
	void  copyto(EffectToBorn& dest){
		dest.WeaponToBorn=WeaponToBorn;
		dest.WeaponScale=WeaponScale;
		dest.WeaponBornProbability=WeaponBornProbability;
	}
};
class ChangeTerraType:public OneVertexTempDeformator{
public:
	EffectToBorn StartEffect;
	EffectToBorn FinalEffect;
	virtual void CalcDestVertex();
	virtual void OnStart();	
	virtual void OnEndStay();	
};
class ChangeTerraHeight:public OneVertexTempDeformator{	
public:
	Vector3D DCoor;
	Vector3D Normal;
	virtual void CalcDestVertex();
};
class TempTerraMorph:public BaseClass{
public:
    ClassArray<OneVertexTempDeformator> Deformations;
	void AddDeformator(OneVertexTempDeformator* D);
	void AddTerraTypeDeformator(int VertexIndex,int WaitTime,int ChangeTime,int StayTime,int UndoTime,int MaxWeight=255,bool Forever=false,EffectToBorn* start=NULL,EffectToBorn* final=NULL);
	void AddTerraHeightDeformator(int VertexIndex,int WaitTime,int ChangeTime,int StayTime,int UndoTime,
		Vector3D DCoor,Vector3D normal);
	void Process();
};
extern TempTerraMorph TMORPH;
#endif