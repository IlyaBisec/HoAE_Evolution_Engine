#include "TestEditor.h"
#include "TreesScope.h"
#include "CurrentMapOptions.h"
#include "SquareMap.h"
///void addmr1(int v,char* s,int L);
//#define addmr(v) addmr1(v,__FILE__,__LINE__)
__declspec(dllexport)
void SetCurrentProgress(char*,int,int);
class ExTerrain:public DescribedBaseClass{
public:
	ExTerrain(){		
		Enabled=true;
		GlobalWeight=1.0f;
		WeightPreferring=0;
		Mask.Map.TopLayerIndex=0;
	}
	bool Enabled;
	float GlobalWeight;
	_fractal WeightVariation;
	int WeightPreferring;
	ExWeightMap Mask;
	Array<ObjectRules> Rules;
	SubSection Advanced;
	SAVE(ExTerrain)	
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);	
	REG_CLASS(ObjectRules);
	REG_AUTO(Rules);
	REG_AUTO(Advanced);
	REG_MEMBER(_float0_10,GlobalWeight);
	REG_AUTO(WeightVariation);
	REG_AUTO(Mask);
	REG_ENUM(_index,WeightPreferring,WeightPreferring);
	ENDSAVE
};

///////////////////////////////////////////////////////////////////////////////////////////
// Users menu //
///////////////////////////////////////////////////////////////////////////////////////////

class U_Tree:public BaseClass{
public:
	int Trees;
	float MinDistance;
	float WholeDensity;
	U_Tree(){
		MinDistance=0.3f;
		WholeDensity=1.0f;
	}
	SAVE(U_Tree)
	REG_ENUM(_index,Trees,TREESGROUPS);
	REG_MEMBER(_float01,MinDistance);
	REG_MEMBER(_float0_10,WholeDensity);
	ENDSAVE
};
class U_Condition:public BaseClass{
public:
	bool Smoothing;
	int StartHeight;
	int FinHeight;
	int StartAngle;
	int FinAngle;
	U_Condition(){
		Smoothing=0;
		StartHeight=0;
		FinHeight=0;
		StartAngle=0;
		FinAngle=0;
	}
	SAVE(U_Condition)
	REG_MEMBER(_bool,Smoothing);
	REG_MEMBER(_int,StartHeight);
	REG_MEMBER(_int,FinHeight);
	REG_MEMBER(_int,StartAngle);
	REG_MEMBER(_int,FinAngle);
	ENDSAVE
};
class U_ObjectRules:public DescribedBaseClass{
public:
	bool Enabled;
	U_Condition Condition;
	Array<TexFact> Textures;
	ExLockInfo Locking;
    Array<U_Tree> Trees;
	U_ObjectRules(){
		Enabled=true;
	}
	SAVE(U_ObjectRules)
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);
	REG_AUTO(Condition);
	REG_CLASS(TexFact);
	REG_AUTO(Textures);
	REG_AUTO(Locking);
	REG_CLASS(U_Tree);
	REG_AUTO(Trees);
	ENDSAVE
};
class U_Terrain:public DescribedBaseClass{
public:
	U_Terrain(){
		Enabled=true;
		GlobalWeight=1.0f;
	}
	bool Enabled;
	float GlobalWeight;
	Array<U_ObjectRules> Rules;
	SAVE(U_Terrain)
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);
	REG_MEMBER(_float0_10,GlobalWeight);
	REG_CLASS(U_ObjectRules);
	REG_AUTO(Rules);
	ENDSAVE
};
class U_Relief:public DescribedBaseClass{
public:
	U_Relief(){
		Enabled=1;
	    GlobalWeight=1.0f;
		AverageHeight=0;
		StartHeight=0;
		PaintWater=0;
		WaterMixing=1;
	}
	bool     Enabled;
	float    GlobalWeight;
	int      AverageHeight;
	int      StartHeight;
	bool     PaintWater;
	float    WaterMixing;
	SAVE(U_Relief)
	REG_AUTO  (Description);
	REG_MEMBER(_bool,Enabled);
	REG_MEMBER(_float0_10,GlobalWeight);
	REG_MEMBER(_int,AverageHeight);
	REG_MEMBER(_int,StartHeight);
	REG_MEMBER(_bool,PaintWater);
	REG_MEMBER(_float0_10,WaterMixing);
	ENDSAVE
};
class UsersMenu:public BaseClass{
public:
	UsersMenu(){
		Enabled=false;
	}
	bool Enabled;
	Array<U_Relief> Reliefs;
	Array<U_Terrain> Terrains;
	SAVE(UsersMenu)
	REG_MEMBER(_bool,Enabled);
	REG_CLASS(U_Relief);
	REG_AUTO(Reliefs);
	REG_CLASS(U_Terrain);
	REG_AUTO(Terrains);
	ENDSAVE
};

///////////////////////////////////////////////////////////////////////////////////////////
// Users menu //
///////////////////////////////////////////////////////////////////////////////////////////

class ReliefRule:public DescribedBaseClass{
public:
	ReliefRule(){
		Enabled=1;
		AverageHeight=0;
        SmoothSteps=0;
		CutOffTop=true;
		HeightOperation=0;
		StartHeight=0;
		SmoothMaxVariation=0;
		PaintWater=0;
		WaterColor1=0xFF808080;
		WaterColor2=0xFF808080;
		WaterMixing.Contrast=0;
	}
	bool     Enabled;
	int      AverageHeight;
	int      StartHeight;
	int      HeightOperation;
	int      SmoothSteps;
	int		 SmoothMaxVariation;
	_fractal SmoothVariation;
	int      SmoothType;
	bool     CutOffTop;
	MapRef   Layer;
	_fractalLine EdgesShape;
	_fractal TopShape;
	_fractal TopModulation;
	bool  PaintWater;
	DWORD WaterColor1;
	DWORD WaterColor2;
	_fractal WaterMixing;
	SAVE(ReliefRule)
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);
	REG_MEMBER(_int,AverageHeight);
	REG_MEMBER(_int,StartHeight);
	REG_ENUM  (_index,HeightOperation,HEIGHTOP);
	REG_ENUM  (_index,SmoothType,SMOOTHTYPE);
	REG_MEMBER(_int,SmoothSteps);
	REG_MEMBER(_int,SmoothMaxVariation);
	REG_AUTO  (SmoothVariation);	
	//REG_MEMBER(_bool,CutOffTop);
	REG_AUTO  (Layer);
	REG_AUTO  (EdgesShape);
	REG_AUTO  (TopShape);
	REG_AUTO  (TopModulation);
	//REG_MEMBER(_bool,PaintWater);
	//REG_MEMBER(_DWORD,WaterColor1);
	//REG_MEMBER(_DWORD,WaterColor2);
	//REG_AUTO(WaterMixing);
	ENDSAVE
	int GetHeightInPoint(int x,int y,int& sm,DWORD& WColor,int& Weight){
		sm=0;
        int sz=240*32<<ADDSH;

        EdgesShape.ShiftXY(x,y);

		float fx=float(x)/sz;
		float fy=float(y)/sz;
        BasePaintMap* BPM=MLayers->Maps[Layer.TopLayerIndex];		

		float H0=BPM->GetGreyPrecise(fx,fy);
		Weight=int(H0);
		addmr(Weight);
		if(PaintWater&&Weight>0){
			int fv=WaterMixing.GetFValue(x,y)*255;
			DWORD C=MixDWORD(WaterColor1,WaterColor2,fv,255-fv);
			int WM=Weight*6;
			if(WM>255)WM=255;
			if(WM<0)WM=0;
            WColor=MixDWORD(C,WColor,WM,255-WM);
		}
		float Edge=EdgesShape.GetFValue(x,y);
		float H=H0;//(H0+(Edge-0.5f)*128.0f-64.0f)*2.0f;
		if(H<0)H=0;
		if(CutOffTop&&H>255)H=255;
		H=StartHeight+H*AverageHeight*TopShape.GetFValue(x,y)*TopModulation.GetFValue(x,y)*4.0f/255.0;
		if(SmoothType==0){
			if(H0>0.001){
				sm=SmoothSteps;
				sm=SmoothSteps;
				if(SmoothMaxVariation)sm+=int(float(SmoothMaxVariation)*(SmoothVariation.GetFValue(x,y)+0.01));
			}
		}else{
			float dhx=fabs(BPM->GetGreyPrecise(fx+0.01,fy)-BPM->GetGreyPrecise(fx-0.01,fy));
			if(dhx<0.00001){
				float dhy=fabs(BPM->GetGreyPrecise(fx,fy+0.01)-BPM->GetGreyPrecise(fx,fy-0.01));
				if(dhy<0.00001)return int(H);
			}
			sm=SmoothSteps;
			if(SmoothMaxVariation)sm+=int(float(SmoothMaxVariation)*(SmoothVariation.GetFValue(x,y)+0.01));
		}
		addmr(int(H));
		return int(H);
	}
};
class ExReliefGenerator:public BaseClass{
public:
	int StartHeight;
	_fractal GlobalModulation;
	Array<ReliefRule> Rules;
	SAVE(ExReliefGenerator)
	REG_MEMBER(_int,StartHeight);
    REG_AUTO(GlobalModulation);
	REG_CLASS(ReliefRule);
	REG_AUTO(Rules);
	ENDSAVE
	int GetHeightInPoint(int x,int y,int& sm,DWORD& WColor){
		float f=GlobalModulation.GetFValue(x,y);
		addmr(x);
		addmr(y);
		addmr(GlobalModulation.phaseX);
		addmr(GlobalModulation.phaseY);
		addmr(f*1000);
		int H=StartHeight*2*f;
		addmr(H);
		sm=0;
		for(int i=0;i<Rules.GetAmount();i++)if(Rules[i]->Enabled){
			int sm1=0;
			int W;
			int rr=Rules[i]->GetHeightInPoint(x,y,sm1,WColor,W);
			addmr(i);
			addmr(rr);			
			sm+=sm1;
			switch(Rules[i]->HeightOperation){
			case 0:
				addmr(H);
				addmr(rr);
				H+=rr;				
				addmr(H);				
				break;
			case 1:
				H=min(rr,H);
				addmr(H);
				break;
			case 2:
				H=max(rr,H);
				addmr(H);
				break;
			case 3:
                H=(rr*W+H*(255-W))/255;
				addmr(H);
				break;
			}
		}
		addmr(H);
		return H;
	}
	void GenerateHeight(){
		byte* Smooth=new byte[MaxPointIndex];
		memset(Smooth,0,MaxPointIndex);
		for(int ix=0;ix<VertInLine;ix++){
			SetCurrentProgress("Relief",ix,VertInLine);
			for(int iy=0;iy<MaxTH-1;iy++){
				int x=ix<<5;
				int y=iy<<5;
				if(ix&1)y-=16;
				int sm=0;
				int ofs=ix+iy*VertInLine;
				DWORD Color=0xFF808080;
				THMap[ofs]=GetHeightInPoint(x,y,sm,Color);
                addmr(THMap[ofs]);
				void SetWColorInPoint(int x,int y,DWORD Color);
				SetWColorInPoint(x,y,Color);
				if(sm>200)sm=200;
				Smooth[ofs]=sm;
			}
		}
		bool ch=0;
		do{
			ch=0;
			for(int vert=0;vert<MaxPointIndex;vert++){
				if(Smooth[vert]){
					int Lin=vert/VertInLine;
					int Lpos=vert%VertInLine;
					if(Lin>0&&Lpos>0&&Lpos<VertInLine-1&&Lin<MaxTH-1){
						if(Lpos&1){
							THMap[vert]=(THMap[vert]*6+THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
								THMap[vert+1]+THMap[vert-VertInLine+1]+THMap[vert-VertInLine-1]+4)/12;
						}else{
							THMap[vert]=(THMap[vert]*6+THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
								THMap[vert+1]+THMap[vert+VertInLine+1]+THMap[vert+VertInLine-1]+4)/12;
						};
					};
					addmr(THMap[vert]);
					Smooth[vert]--;
					ch=1;
				}
			}
		}while(ch);
	}
};
//------RANDOM PATTERN GENERATION------//
template<class X>class SimpleClonesArray:public ClassArray<X>{
public:
	virtual int GetExpansionRules(){
		return 1;//0-no expansion 1-expand with base type only 2-expand with child classes
	}
	virtual bool ForceSimplification(){return true;}
};
class OneLayerCondition:public BaseClass{
public:
	virtual bool Check(int x,int y){return true;}
	virtual bool ForceSimplification(){return true;}
	SAVE(OneLayerCondition);
	ENDSAVE;
};
class _IsInsideTheLayer:public OneLayerCondition{
public:
	int IsInsideTheLayer;
	virtual bool Check(int x,int y);
	SAVE(_IsInsideTheLayer);
		REG_PARENT(OneLayerCondition);
		REG_ENUM(_index,IsInsideTheLayer,LAYERSLIST);
	ENDSAVE;
};
class _IsOutsideTheLayer:public OneLayerCondition{
public:
	int IsOutsideTheLayer;
	virtual bool Check(int x,int y);
	SAVE(_IsOutsideTheLayer);
		REG_PARENT(OneLayerCondition);
		REG_ENUM(_index,IsOutsideTheLayer,LAYERSLIST);
	ENDSAVE;
};
class _FarOutsideTheLayer:public OneLayerCondition{
public:
	int FarOutsideTheLayer;
	virtual bool Check(int x,int y);
	SAVE(_FarOutsideTheLayer);
		REG_PARENT(OneLayerCondition);
		REG_ENUM(_index,FarOutsideTheLayer,LAYERSLIST);
	ENDSAVE;
};
class _DeepInsideTheLayer:public OneLayerCondition{
public:
	int DeepInsideTheLayer;
	virtual bool Check(int x,int y);
	SAVE(_DeepInsideTheLayer);
		REG_PARENT(OneLayerCondition);
		REG_ENUM(_index,DeepInsideTheLayer,LAYERSLIST);
	ENDSAVE;
};
class _OR_conditions:public OneLayerCondition{
public:
	ClassArray<OneLayerCondition> OR;
	virtual bool Check(int x,int y);
	SAVE(_OR_conditions);
		REG_PARENT(OneLayerCondition);
		REG_AUTO(OR);
	ENDSAVE;
};
class _NOT_condition:public OneLayerCondition{
public:
	ClassPtr<OneLayerCondition> NOT;
	virtual bool Check(int x,int y);
	SAVE(_NOT_condition);
		REG_PARENT(OneLayerCondition);
		REG_AUTO(NOT);
	ENDSAVE;
};
class _IsOnBoundOfTheLayer:public OneLayerCondition{
public:
	int IsOnBoundOfTheLayer;
	virtual bool Check(int x,int y);
	SAVE(_IsOnBoundOfTheLayer);
		REG_PARENT(OneLayerCondition);
		REG_ENUM(_index,IsOnBoundOfTheLayer,LAYERSLIST);
	ENDSAVE;
};
class OneRandomLayer:public BaseClass{
public:
	OneRandomLayer(){
		//Constant_start=0.5f;
		//Constant_final=0.5f;
		LinearX_start=0.5f;
		LinearX_final=0.5f;
		LinearY_start=0.5f;
		LinearY_final=0.5f;
		SquareX_start=0.5f;
		SquareX_final=0.5f;
		SquareY_start=0.5f;
		SquareY_final=0.5f;
		FillingPercent=0.3f;
	}
	void Perform();
	bool CheckConditions(int x,int y);
	class ORL_Test:public BaseFunction{
	public:
		void EvaluateFunction(){
			OneRandomLayer* ORL=get_parent<OneRandomLayer>();
			if(ORL){
				ORL->Perform();
				BasePaintMap* GM=MLayers->Maps[ORL->AffectedLayer];
				GM->CreateTexturePart(0,0,512,512);
			}
		}
		MapRef Map;
		SAVE(ORL_Test)
			REG_PARENT(BaseFunction);
			REG_AUTO(Map);
		ENDSAVE;
	};
	ORL_Test Test;
	_str Description;
	int AffectedLayer;
	float FillingPercent;
	_fractalLayer PiecesShape;
	SubSection Distortion;

	//float Constant_start;
	//float Constant_final;

	float LinearX_start;
	float LinearX_final;

	float LinearY_start;
	float LinearY_final;

	float SquareX_start;
	float SquareX_final;

	float SquareY_start;
	float SquareY_final;
	SubSection ConditionsList;
	ClassArray<OneLayerCondition> Conditions;
	SAVE(OneRandomLayer);
		REG_AUTO(Test);
		REG_AUTO(Description);
		REG_ENUM(_index,AffectedLayer,LAYERSLIST);
		REG_MEMBER(_float01,FillingPercent);
		REG_AUTO(PiecesShape);
		REG_AUTO(Distortion);
		//REG_MEMBER(_float01,Constant_start);
		//REG_MEMBER(_float01,Constant_final);
		REG_MEMBER(_float01,LinearX_start);
		REG_MEMBER(_float01,LinearX_final);
		REG_MEMBER(_float01,LinearY_start);
		REG_MEMBER(_float01,LinearY_final);
		REG_MEMBER(_float01,SquareX_start);
		REG_MEMBER(_float01,SquareX_final);
		REG_MEMBER(_float01,SquareY_start);
		REG_MEMBER(_float01,SquareY_final);
		REG_AUTO(ConditionsList);
		REG_AUTO(Conditions);
	ENDSAVE;
	virtual const char* GetThisElementView(const char* Local){
		if(Description.pchar()&&Description.pchar()[0])return Description.pchar();
		return Local;
	}
};
class OneRandomLandscapeSample:public BaseClass{
public:
	_str Description;
	SimpleClonesArray<OneRandomLayer> Samples;
	SAVE(OneRandomLandscapeSample);
		REG_AUTO(Description);
		REG_AUTO(Samples);
	ENDSAVE;
	virtual const char* GetThisElementView(const char* Local){
		if(Description.pchar()&&Description.pchar()[0])return Description.pchar();
		return Local;
	}
};
class OneGroundSample:public BaseClass{
public:
	_str Name;
	SAVE(OneGroundSample);
		REG_FILEPATH(Name,".smp");
	ENDSAVE;
	const char* GetThisElementView(const char* Local){
		if(Name.pchar()&&Name.pchar()[0])return Name.pchar();
		return Local;
	}
    bool ForceSimplification(){return true;}
};
typedef ClonesArray<OneGroundSample> SamplesSet;
class OneReqPiece:public BaseClass{
public:
	_str Name;
    bool Neytral;
	SamplesSet Samples;
	int MinRadiusOfNearZone;
	int MaxRadiusOfNearZone;
	int AmountInNearZone;
	int AmountInFarZone;
	SAVE(OneReqPiece);
		REG_AUTO(Name);
		REG_MEMBER(_bool,Neytral);
		REG_AUTO(Samples);
		REG_MEMBER(_int,MinRadiusOfNearZone);
		REG_MEMBER(_int,MaxRadiusOfNearZone);
		REG_MEMBER(_int,AmountInNearZone);
		REG_MEMBER(_int,AmountInFarZone);
	ENDSAVE;
	const char* GetThisElementView(const char* Local){
		if(Name.pchar()&&Name[0])return Name.pchar();
		return Local;
	}
};
class StartPositionTempStruct{
public:
	SquareMap<word> Zone;
	SquareMap<word> Start;
	SquareMap<word> Pieces;
	SquareMap<word> Voronoi;
	int BestX[32];
	int BestY[32];
	int bestV;
	DynArray<DWORD> xy_bad;
};
class OneStartPositionType:public ReferableBaseClass{
public:	
	class OSPT_Test:public BaseFunction{
	public:
		void EvaluateFunction(){
			OneStartPositionType* ORL=get_parent<OneStartPositionType>();
			if(ORL){
				//ORL->SetPositions(NPL);
			}
		}
		int NPL;
		MapRef Map;
		SAVE(OSPT_Test)
			REG_PARENT(BaseFunction);
			REG_MEMBER(_int,NPL);
			REG_AUTO(Map);
		ENDSAVE;
	};
	bool SetPositions(StartPositionTempStruct* temp,char* Nats,bool CheckHeight,bool DivideByWater);
	bool SetPieces(StartPositionTempStruct* temp,char* Nats,bool CheckHeight,bool DivideByWater);
	OSPT_Test Test;
	ClassArray<OneLayerCondition> AccessConditions;
	ClassArray<OneLayerCondition> StartPlaceConditions;
	ClassArray<OneLayerCondition> PiecesConditions;
    ClonesArray<OneReqPiece> StartPieces;
	SamplesSet FarPieces;
	SAVE(OneStartPositionType);
		REG_AUTO(Test);
		REG_AUTO(Name);
		REG_AUTO(AccessConditions);
		REG_AUTO(StartPlaceConditions);
		REG_AUTO(PiecesConditions);
		REG_AUTO(StartPieces);
		REG_AUTO(FarPieces);
	ENDSAVE;
	const char* GetThisElementView(const char* Local){
		if(Name.pchar()&&Name[0])return Name.pchar();
		return Local;
	}
};
class OneRandomLandscapeType:public BaseClass{
public:
	void Perform(int Code,float);
	void Resample(int LayerType);
	class ORLT_Test:public BaseFunction{
	public:
		void EvaluateFunction(){
			OneRandomLandscapeType* ORL=get_parent<OneRandomLandscapeType>();
			if(ORL){
				ORL->Perform(MapCode,1.0f);				
				for(int i=0;i<ORL->Lands.GetAmount();i++){
					int n=ORL->Lands[i]->Samples.GetAmount();
					for(int j=0;j<n;j++){
						BasePaintMap* GM=MLayers->Maps[ORL->Lands[i]->Samples[j]->AffectedLayer];					
						GM->CreateTexturePart(0,0,512,512);
					}
				}
			}
		}
		void MRandomize(){
			OneRandomLandscapeType* ORL=get_parent<OneRandomLandscapeType>();
			if(ORL){
				MapCode=rand();
			}
		}
		int MapCode;
		MapRef Map;
		SAVE(ORLT_Test)
			REG_PARENT(BaseFunction);
		REG_MEMBER(_int,MapCode);
		REG_AUTO(Map);
		ENDSAVE;
	};
	_str Description;
	ORLT_Test Generate;
	//OneStartPositionType StartPosition;
	SimpleClonesArray<OneRandomLandscapeSample> Lands;
	SAVE(OneRandomLandscapeType);
	REG_AUTO(Description);
	REG_AUTO(Generate);
	//REG_AUTO(StartPosition);
	REG_AUTO(Lands);
	ENDSAVE;
	virtual const char* GetThisElementView(const char* Local){
		if(Description.pchar()&&Description.pchar()[0])return Description.pchar();
		return Local;
	}
};
class StartResourcesEx:public StartResources{
public:
	_str Name;
	SAVE(StartResourcesEx);
		REG_AUTO(Name);
		REG_PARENT(StartResources);
	ENDSAVE;
	virtual const char* GetThisElementView(const char* Local){
		if(Name.pchar()&&Name.pchar()[0])return Name.pchar();
		return Local;
	}
};
class RandomMapDescription:public BaseClass{
	bool IsInit;
public:
	RandomMapDescription(){
		IsInit=false;
	}
	void Init(){
		if(!IsInit){
			reset_class(this);
            SafeReadFromFile("RandomMapDesc.xml");
			IsInit=true;
		}
	}
	ClonesArray<OneRandomLandscapeType> Landscapes;
    ClonesArray<OneStartPositionType> StartPositions;
	ClonesArray<StartResourcesEx> StartRes;
	SAVE(RandomMapDescription);
		REG_AUTO(Landscapes);
		REG_AUTO(StartPositions);
		REG_AUTO(StartRes);
	ENDSAVE;
};
//--------TERRAIN GENERATOR ROOT CLASS-------/
class LayersArray:public ClonesArray<_str>{
public:
	const char* GetElementView(int Index,const char* LocalName){
		DWORD C=0xFF000000;
		if(MLayers&&Index<MLayers->Maps.GetAmount()){
			C=MLayers->Maps[Index]->GetPaletteColor(1);
		}
		char* b=GetGlobalBuffer();
		sprintf(b,"{#bar%X 48}",C);
		return b;
	}
};
class ExLandsScope:public BaseClass{
public:
	ExLandsScope(){
		PostProcessType=1;
		PostStagesAmount=8;
		GenerateTextures=1;
		GenerateRelief=0;
	}
	void Generate();
	int PostProcessType;
	int PostStagesAmount;
	bool GenerateTextures;
	bool GenerateRelief;
	MapRef AllMaps;
	//ClonesArray<OneRandomLandscapeType> RandomMap;
	//ClonesArray<OneStartPositionType> StartPositions;
	ExReliefGenerator Relief;
	Array<ExTerrain> Terr;
	LayersArray PredefinedNamesOfLayers;
	UsersMenu UsersMenu;
	bool GetResult(int x,int y,int Height,int Angle,int Tex,int Fact,int FactW,ExTexResult& result);
	SAVE(ExLandsScope)
		REG_AUTO2(AllMaps,"-maxLx 300 -maxLy 300");		
		//REG_AUTO(RandomMap);
		//REG_AUTO(StartPositions);
		REG_ENUM(_index,PostProcessType,PostProcessType);
		REG_MEMBER(_int,PostStagesAmount);
		REG_MEMBER(_bool,GenerateTextures);
		REG_MEMBER(_bool,GenerateRelief);		
		REG_AUTO(Relief);
		REG_CLASS(ExTerrain);
		REG_AUTO(Terr);
		REG_AUTO(PredefinedNamesOfLayers);
	ENDSAVE
};
void SetTextureInPointUsingPreset(int Vertex,int Texture);
void SetCurrentPreset(char* Name);
bool CheckIfPresetActive();