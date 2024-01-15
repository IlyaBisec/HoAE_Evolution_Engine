int ConvertTextureToNDDS(int src);
////////////////////////////////////////////////////////////////////////
////////////////////////  F A C T U R E S  /////////////////////////////
////////////////////////////////////////////////////////////////////////

class RefreshSurface:public BaseFunction{
public:
	void EvaluateFunction(){
		void RefreshWater();
		RefreshWater();
		void MakeAllDirtyGBUF();
		MakeAllDirtyGBUF();
	}
	SAVE(RefreshSurface){
		REG_PARENT(BaseFunction);
	}ENDSAVE;
};
class OneFactureInfo:public BaseClass{
public:	
	RefreshSurface Refresh;
	int   FactureID;
	int   UsageForLands;
	bool  UseBumpMap;
	float BumpDegree;
	float PBumpDegree;
	int   BumpTextureID;
	float BumpContrast;
	float BumpBrightness;
	int   NormaleTextureID;
	int   PrevBumpTex;
	int   Mapping;
	float UFlowSpeed;
	float VFlowSpeed;
	float UScale;
	float VScale;
	float UShift;
	float VShift;	
	virtual void reset_class(void* ptr){
		((OneFactureInfo*)(ptr))->Init();
	}
	OneFactureInfo(){
		Init();
	}
	void Init(){
		FactureID=IRS->GetTextureID("Textures\\ground\\tex1.bmp");
		Mapping=0;
		BumpTextureID=-1;
		NormaleTextureID=-1;
		UScale=1.0f;
		VScale=1.0f;
		UShift=0.0f;
		VShift=0.0f;
		BumpDegree=1.0f;
		PBumpDegree=1.0f;
		BumpContrast=0.6f;
		BumpBrightness=1.0f;
		PrevBumpTex=-1;
		UsageForLands=-1;
	}
	class fnConvertToDDS:public BaseFunction{
	public:
		void EvaluateFunction(){
			OneFactureInfo* FI=get_parent<OneFactureInfo>();
			if(FI){			
				int ConvertTextureToNDDS(int src);
                FI->FactureID=ConvertTextureToNDDS(FI->FactureID);				
			}
		}
		SAVE(fnConvertToDDS);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	};
	fnConvertToDDS ConvertToDDS;
	const char* GetThisElementView(const char* LocalName){
		char* s=GetGlobalBuffer();
		if(UseBumpMap){
			sprintf(s,"{$tx %s}{$tx %s} %s",IRS->GetTextureName(FactureID),IRS->GetTextureName(BumpTextureID),Mapping?"[V]":"[H]");
		}else{
			sprintf(s,"{$tx %s} %s - %s",IRS->GetTextureName(FactureID),Mapping?"[V]":"[H]",IRS->GetTextureName(FactureID));
		}		
		int L=strlen(s);
		for(int i=0;i<L;i++)if(s[i]=='\\')s[i]='/';
		return s;
	}
	SAVE(OneFactureInfo);
		REG_AUTO(Refresh);
		REG_MEMBER(_TextureID,FactureID);
		REG_MEMBER(_bool,UseBumpMap);
		SAVE_SECTION(1);
		REG_MEMBER(_float0_10,BumpDegree);
		REG_MEMBER(_TextureID,BumpTextureID);
		//REG_MEMBER(_float01,BumpContrast);
		//REG_MEMBER(_float01,BumpBrightness);
		SAVE_SECTION(0xFFFFFFFF);
		REG_ENUM(_index,UsageForLands,LANDBOUNDS);
		//REG_ENUM(_index,Mapping,Mapping);
		REG_MEMBER(_float,UFlowSpeed);
		REG_MEMBER(_float,VFlowSpeed);
		REG_MEMBER(_float,UScale);
		REG_MEMBER(_float,VScale);
		REG_MEMBER(_float01,UShift);
		REG_MEMBER(_float01,VShift);
		REG_AUTO(ConvertToDDS);
	ENDSAVE;
	DWORD GetClassMask(){
		if(UseBumpMap)return 1;
		else return 2;
	}
	bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);
};
bool launchProgram(const char* appName, const char* cmdLine, bool isWait);
int ConvertTextureToNDDS(int src);
extern ClonesArray<OneFactureInfo> FacturesList;;
int GetNFactures();
int GetFacture(int id);
bool UsesBump(int id);
int GetFactureBump(int id,float& Contrast,float& Bright);
const char* GetFactureName(int id);
int GetFactureUsage(int id);
Vector3D GetSurfaceN(int x,int y);
int GetFactureWeightFactor(int x,int y,int id);
void GetUVInfo(int id,float& du,float& dv,float& su,float& sv);
int Norm(int dx,int dy);

//////////////////////////////////////////////////////////////////////////
////////////////////////// R  O  A  D  S  ////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct OneRoadDesc{
	int TextureID;
	char RoadName[32]; 
	int TexSizeX;
	int TexSizeY;
	int ColorY0;
	int ColorY1;
	int ReliefY0;
	int ReliefY1;
	int RWidth;
	int RScaleX;
	int AlphaFrequency;
	int AFactor;
	int AFactorMin;
	int RFactor;
	int GFactor;
	int BFactor;
	//extended information
	int GroupFirstID;
	int GroupID;
	int MapTextureID;
	int TexRMin;
	int TexRMax;
	int TexSmoothness;
	int FactureID;
	int RoadPreviewID;
	int RoadPreviewSpr;
	//physical
	short Speed; //256-normal speed
	short Tiring;//256-normal  tiring
	short Priory;//256-normal  priory
	//------BUMP------
	int BumpTextureID;
	int B1_Y1;
	int B1_Y2;
	int B2_Y1;
	int B2_Y2;
	//--------------------
	DWORD Diffuse;
};

class OneRoad{
public:
	int Width;
	float* XY;
	DWORD* Weights;
	int Np;
	int Maxp;
	int XMin,YMin,XMax,YMax;
	int xc,yc,zc,R;//for fast detection
	int Type;
	BaseMesh* BM;

	int GetDistance(int x,int y,int precise,int& pos){
		int R0=10000000;
		for(int i=0;i<Np;i+=precise){
			int R=Norm(x-XY[i+i],y-XY[i+i+1]);
			if(R<R0){
				R0=R;
				pos=i;
			};
		};
		return R0;
	};
	void SurroundWithTexture();
};
class RoadDesc{
public:
	BaseMesh* RoadMesh;
	BaseMesh* ColorRoadMesh;
	OneRoadDesc* Roads;
	int NGrps;
	int GrpsElements[256];
	int NRoadsDesc;
	~RoadDesc(){
		if(RoadMesh){
			delete(RoadMesh);
			delete(ColorRoadMesh);
			delete(Roads);
		};
		memset(this,0,sizeof *this);
	};
};
class RoadsSystem{
public:
	RoadDesc* RDesc;
	OneRoad* Roads;
	int NRoads;
	RoadsSystem();
	int AddRoad(int Type);
	void AddPointToRoad(int idx,int x,int y);
	void ClearAll();
	void ClearRoads();
	void Draw(int sx,int sy,int mx,int my,int Lx,int Ly,int scale);
	void DrawVisibleRoads();
	void Init();
	void RefreshRoadsView();
	~RoadsSystem(){
		ClearAll();
	};
};

//////////////////////////////////////////////////////////////////////////

class RoadDescription:public ReferableBaseClass{
public:	
	word PreviewFileID;
	int PreviewSprite;
	int ShaderType;
    int RoadTexture;	
	//int RoadTextureTopY;
	//int RoadTextureBottomY;
	int RoadWidth;
	float RoadScaleX;
	int TextureForGlobalModulation;
	float ScaleForGlobalModulation;
    DWORD Color;
	float WidthVariation;
	float WidthVariationScale;
	float CurvatureVariation;
	float CurvatureVariationScale;

	ClassRef<RoadDescription> AdditionalLayer;

	bool  SpecifyNationForRoad;
    int   NationForRoad;

	SAVE(RoadDescription);
		REG_AUTO(Name);
		//REG_MEMBER(_gpfile,PreviewFileID);
		//REG_SPRITE(PreviewSprite,PreviewFileID);		
		REG_ENUM(_index,ShaderType,ROADSHADERTYPE);
        REG_MEMBER(_TextureID,RoadTexture);
		//REG_MEMBER(_int,RoadTextureTopY);
		//REG_MEMBER(_int,RoadTextureBottomY);
		REG_MEMBER(_float0_10,RoadScaleX);
		REG_MEMBER(_int,RoadWidth);
		REG_MEMBER(_TextureID,TextureForGlobalModulation);
		REG_MEMBER(_float0_10,ScaleForGlobalModulation);
		
		REG_MEMBER(_color,Color);

		REG_MEMBER(_float0_10,WidthVariation);
		REG_MEMBER(_float0_10,WidthVariationScale);
		REG_MEMBER(_float0_10,CurvatureVariation);
		REG_MEMBER(_float0_10,CurvatureVariationScale);

		REG_AUTO(AdditionalLayer);

		REG_MEMBER(_bool,SpecifyNationForRoad);
		SAVE_SECTION(2);
		REG_ENUM(_index,NationForRoad,RACES);
    ENDSAVE;
	DWORD GetClassMask(){
		return SpecifyNationForRoad?3:1;
	}
	void Init(){
        PreviewFileID=GPS.PreLoadGPImage("Roads\\RoadsG");		
		RoadWidth=64;
		Color=0xFF808080;
		RoadScaleX=1.0f;
		TextureForGlobalModulation=IRS->GetTextureID("Roads\\Textures\\grey.dds ");
		//RoadTextureBottomY=256;
		ScaleForGlobalModulation=1.0f;
		WidthVariation=0;
		WidthVariationScale=1.0f;
	}
	int GetRoadWidth(int x,int y){
		if(WidthVariation>0.01){
			int GetFractalVal(int x,int y);
            int R=GetFractalVal(x*WidthVariationScale*2,y*WidthVariationScale*2);
			R=256+(R-256)*WidthVariation/8;
			if(R<0)R=0;
			return RoadWidth*R/256;
		}else return RoadWidth;
	}
	RoadDescription(){
		Init();
	}
	void reset_class(void* Data){
		((RoadDescription*)Data)->Init();
	}	
	const char* GetThisElementView(const char* LocalName){
		char* s=GetGlobalBuffer();
		//sprintf(s,"{tex %s 0 0 64 64 0 0 1 1 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF 64}{tex %s 0 0 64 64 0 0 1 1 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF 0xFFFFFFFF 64}",IRS->GetTextureName(RoadTexture),IRS->GetTextureName(TextureForGlobalModulation));
		sprintf(s,"{$tx %s}{$tx %s}",IRS->GetTextureName(RoadTexture),IRS->GetTextureName(TextureForGlobalModulation));
		return s;
	}
};
extern ClonesArray<RoadDescription> RoadTypes;
extern RoadsSystem ROADSSYS;
