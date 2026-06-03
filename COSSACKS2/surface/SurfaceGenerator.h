#ifndef __SURFACEGENERATOR__
#define	__SURFACEGENERATOR__

#include "..\Surface\Surface.h"
#include "..\Extensions.h"
#include "..\SquareMap.h"
#define surfScale (1.0f/(240*64))
#define MaxDistSet 64
#define SpecialDistField 63

using namespace Surface;

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
	SurfVertex* Vertex;
	int VertexIndex;
	void CreateFromVertex(SurfVertex& V);
};

class OneGenCondition:public BaseClass{
public:
	SAVE(OneGenCondition);	
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V){
		return 1.0f;
	}
};

class OneTreeToGenerate:public BaseClass{
public:
	int   TreeID;
	float MinScale;
	float MaxScale;
	float ExtraScaleOnFractalBound;
	float RandomAngle;
	int   Radius;
	SAVE(OneTreeToGenerate);
		REG_ENUM(_index,TreeID,TREESTYPES);
		REG_MEMBER(_float0_10,MinScale);
		REG_MEMBER(_float0_10,MaxScale);
		REG_MEMBER(_float01,ExtraScaleOnFractalBound);
		REG_MEMBER(_float01,RandomAngle);
		REG_MEMBER(_int,Radius);
	ENDSAVE;
	__constructor(OneTreeToGenerate){
		TreeID=0;
		MinScale=0.8;
		MaxScale=1.2;
		Radius=60;
		ExtraScaleOnFractalBound=0.6;
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
	bool SetWidthInSpecialLayer;
	int  SpecialLayer;
	int  WidthInSpecialLayer;
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
		WidthInSpecialLayer=600;
		SetWidthInSpecialLayer=false;
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
	REG_MEMBER(_bool,SetWidthInSpecialLayer);

	SAVE_SECTION(128)
	REG_ENUM(_index,SpecialLayer,LAYERSLIST);
	REG_MEMBER(_int,WidthInSpecialLayer);

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
			+(VariateWidth&&AllowShapeCurving?64:0)
			+(VariateWidth&&SetWidthInSpecialLayer?128:0);
	}
	float GetHeight(int x,int y,float& Weight,int z=0);
	float PerformShift(Vector3D V,Vector3D& DV);//returns weight of vector shift
	virtual const char* GetThisElementView(const char* Local){
		return Description.pchar();
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
		return Description.pchar();
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
	static float RaceLayerWeight;
	static int   BestRaceLayerIndex;
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
	//REG_ENUM(_index,SurfaceType,SURFACETYPE);
    REG_MEMBER(_int,SurfaceType);
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
class CheckIfSlopeIsNear:public OneTexturingCondition{
public:
	int RadiusMin;
	int RadiusMax;
	_fractal VariationLow;

	SAVE(CheckIfSlopeIsNear);
		REG_PARENT(OneTexturingCondition);
		REG_BASE(OneGenCondition);
		REG_MEMBER(_int,RadiusMin);		
		REG_MEMBER(_int,RadiusMax);		
		REG_AUTO(VariationLow);
	ENDSAVE;
	virtual float GetConditionWeight(PrecompiledVertex* V);
	const char* GetThisElementView(const char* LocalName){
		char* B=GetGlobalBuffer();
		sprintf(B,"%s   R:%d - %d",LocalName,RadiusMin,RadiusMax);
		return B;
	}

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
    bool     Use3DMixing;

	TexPairList Textures;	

	bool EnableTreesGenertion;	
	_fractal TreesDistribution;
	ClonesArray<OneTreeToGenerate> Trees;

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
    REG_MEMBER(_bool,Use3DMixing);
	REG_AUTO(Textures);

	REG_MEMBER(_bool,EnableTreesGenertion);
	SAVE_SECTION(128);
	REG_AUTO(TreesDistribution);
	REG_AUTO(Trees);
	REG_CLASS(OneTreeToGenerate);
	SAVE_SECTION(0xFFFFFFFF);

	ENDSAVE;
	DWORD GetClassMask(){
		return (UseGlobalWeight?1:2)+(EnableTreesGenertion?128:0);
	}
	virtual const char* GetThisElementView(const char* Local){
		char* s=GetGlobalBuffer();
		GetTexString(s,Textures);
		if(Description.pchar())strcat(s,Description.pchar());
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
		return Description.pchar();
	};
};

class SurfaceGenerator;
class GeneratorSurfaceFunction:public BaseSurfaceFunction{
public:

	int MapLx,MapLy;
	_fractalLine* ShapeOfBound;

	SurfaceGenerator* GPtr;
	virtual void ModifyVertex(SurfVertex& V);
	virtual bool ShiftVertex(SurfVertex& V);
};
class BitHeight{
	BYTE* Data;
	int bLx;
	int bLy;
public:
	BitHeight();
	~BitHeight();
	float get(float x,float y);
	bool Load(char* Name);
	void Clear();
};
class SurfaceGenerator:public BaseClass{
public:
	bool m_fActive;
	void PerformTest();
	SurfaceGenerator();
	~SurfaceGenerator();
	class ClearSurface:public BaseFunction{
	public:
		void EvaluateFunction(){
			for(int i=0;i<MLayers->Maps.GetAmount();i++){
				MLayers->Maps[i]->Clear();
			}
		};
	};
	void SaveCurrentLayer();
	void LoadCurrentLayer();
	void ClearCurrentLayer();
	Delimiter Separator;

	// Rotate:
	void RotateLeft();
	void RotateRight();
	void Rotate(const bool Left);

	// Shift:
	int CurShift;
	void ShiftHorizontal();
	void ShiftVertical();
	void Shift(const int Sx, const int Sy);

	MapRef Maps;
	ClearSurface ClearMaps;
	int  Size;
	_str BasicHeightBitmap;
	int  HeightmapScale;

	BitHeight ExHeight;

	_fractalLine ShapeOfBound;
	ClonesArray<OneSurfaceType> Relief;
	ClonesArray<OneTexturingStyle> Texturing;
	bool DontGenerateRelief;
    bool DontGenerateTrees;
	bool ReCreateMinimap;
	bool OptimizeSurface;
	bool DoNothing;

	SAVE(SurfaceGenerator);
		REG_AUTO2(Maps,"-grid 2000 -maxLx 450 -maxLy 450");	
		REG_METHOD(&SurfaceGenerator::SaveCurrentLayer);
		REG_METHOD(&SurfaceGenerator::LoadCurrentLayer);
		REG_METHOD(&SurfaceGenerator::ClearCurrentLayer);
		REG_AUTO(Separator);
		REG_METHOD(&SurfaceGenerator::RotateLeft);
		REG_METHOD(&SurfaceGenerator::RotateRight);
		REG_AUTO(Separator);
		REG_METHOD(&SurfaceGenerator::ShiftHorizontal);
		REG_METHOD(&SurfaceGenerator::ShiftVertical);
		REG_ISLIDER(CurShift, -200, 200, 0);
		REG_AUTO(Separator);
		REG_MEMBER(_int,Size);
		REG_FILEPATH(BasicHeightBitmap,"*.bmp");
		REG_MEMBER(_int,HeightmapScale);
		REG_AUTO(Relief);
		REG_AUTO(Texturing);
		REG_MEMBER(_bool,DontGenerateRelief);
		REG_MEMBER(_bool,ReCreateMinimap);
		REG_MEMBER(_bool,OptimizeSurface);
		REG_MEMBER(_bool,DoNothing);
        REG_MEMBER(_bool,DontGenerateTrees);
		REG_AUTO(ShapeOfBound);
		REG_AUTO(ClearMaps);
	ENDSAVE;

	float GetHeight(int x,int y);
	bool PerformShift(Vector3D& V);
	void FillVertexTextures(PrecompiledVertex* V);	
	void TexturizeVertex(SurfVertex &);
	void TexturizeSurface(SurfMesh& M);
	void TexturizeSurface(SurfMesh& M,int xc,int yc,int R1,int R2);
};
class BoundaryFacture:public BaseClass{
public:
	int Facture;
	SAVE(BoundaryFacture);
		REG_MEMBER(_gfacture,Facture);
	ENDSAVE;
	bool ForceSimplification(){return true;}
};
class ColorSample:public BaseClass{
public:
	DWORD Color;
	SAVE(ColorSample);
	REG_MEMBER(_color,Color);
	ENDSAVE;
	bool ForceSimplification(){return true;}
};
class OneLayerColor:public BaseClass{
public:
	DynArray<int> BoundaryFactures;
	_str Name;
	DWORD Color;
	bool ThisLayerDeterminesRace;
	bool DontGenerateWater;
	bool WillEraseOthers;
	bool IsUsedAsMask;
	bool IsSurfLayer() { // if true, it will be included to ENUM "SURFLAYERS"!
		return !ThisLayerDeterminesRace && !IsUsedAsMask;
	}
	ClonesArray<ColorSample> Colors;
    ClonesArray<BoundaryFacture> Boundary;
	Delimiter D;

	// BumpOnByRaceSubdivisions:
	DWORD BUMPAMPLITUDE;
	float BumpAmplitude, pBumpAmplitude;
	void Update() { BUMPAMPLITUDE = cColor::Pack(BumpAmplitude); }
	void Defaults() {
		BumpAmplitude = pBumpAmplitude = 0.32;
		Update();
	}
	bool OnChangeMember(BaseClass *, void *, void *, const char *) {
		Update();
		if(BumpAmplitude != pBumpAmplitude) {
			pBumpAmplitude = BumpAmplitude;
			return true;
		}
		return false;
	}
	// .ctor
	OneLayerColor() { Defaults(); }
	
	SAVE(OneLayerColor);
		REG_AUTO(Name);
		REG_MEMBER(_color,Color);
		REG_MEMBER(_bool,ThisLayerDeterminesRace);
		REG_MEMBER(_bool,DontGenerateWater);
		REG_MEMBER(_bool, WillEraseOthers);
		REG_MEMBER(_bool, IsUsedAsMask);
		REG_AUTO(Colors);
		REG_AUTO(Boundary);
		REG_MEMBER(_float01, BumpAmplitude);
		REG_AUTO(D);
		REG_CLASS(BoundaryFacture);
		REG_CLASS(ColorSample);
	ENDSAVE;
	bool ForceSimplification(){return true;}
};

class LayersList:public BaseClass{
	byte LayerIndex[16*16*16];
public:
	int MinRaceLandIndex;
	void CreateLayerIndex();
	int GetLayerIndex(DWORD Color);
	ClonesArray<OneLayerColor> Layers;	
	SAVE(LayersList);
	REG_AUTO(Layers);
	ENDSAVE;
	void CreateBoundFactures();
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
    SurfVertex StartVertex;
	SurfVertex DestVertex;
	SurfVertex CurVertex;
	int  StartTime;
	int  PrevTime;
	int  TimeToWait;
	int  TimeToTransformToFinalState;
	int  TimeToStayInFinalState;
	int  TimeToReturnInStartState;
	bool DestVertexDone;
	bool ForeverChange;
	//////////////////////////////////////////////////////////////////////////	
	void BlendVertsCoor    (SurfVertex& V1,SurfVertex& V2,int W1,int W2,SurfVertex& Dest);
	void BlendVertsColors  (SurfVertex& V1,SurfVertex& V2,int W1,int W2,SurfVertex& Dest);
	void BlendVerts        (SurfVertex& V1,SurfVertex& V2,int W1,int W2,SurfVertex& Dest);
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
#define DeformSlots 32
class TempTerraMorph:public BaseClass{
public:
    ClassArray<OneVertexTempDeformator> Deformations[DeformSlots];
	void AddDeformator(OneVertexTempDeformator* D);
	void AddTerraTypeDeformator(int VertexIndex,int WaitTime,int ChangeTime,int StayTime,int UndoTime,int MaxWeight=255,bool Forever=false,EffectToBorn* start=NULL,EffectToBorn* final=NULL);
	void AddTerraHeightDeformator(int VertexIndex,int WaitTime,int ChangeTime,int StayTime,int UndoTime,
		Vector3D DCoor,Vector3D normal);
	void Process();
};
extern TempTerraMorph TMORPH;
extern SurfaceGenerator SURF;
void RaceRef(int idRace, SurfRef &Ref, const SurfVertex &Center, int Radius, bool TexturizeVerts);
int ReadRaceIndex(const SurfVertex &Center);
void SaveGenLayers();
void SurfLayerDrawRound(int idLayer, const SurfVertex &Center, int Radius);
void SurfLayerClearRound(int idLayer, const SurfVertex &Center, int Radius);
void MixWithSurfLayers(int x, int y, DWORD &Color, int nActive = -1);
void RaceRound(int idRace, const SurfVertex &Center, int Radius = 50);
void SurfGenClear();
void SurfLayerClear(int idLayer);
bool SurfLayerIsSet(int idLayer, int x, int y);

#endif