#include "stdheader.h"
#include "surface.h"
#include "Extensions.h"
#include "SquareMap.h"
#define surfScale (1.0f/(240*64))
#define MaxDistSet 64
class DistField{
public:
	static SquareMap<float>* map[MaxDistSet];
	static void ClearMaps();
	static float Get(int Index,float x,float y);
};
struct PrecompiledVertex;
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
	int   Radius;
	SAVE(OneTreeToGenerate){
		REG_ENUM(TreeID,TREESTYPES);
		REG_MEMBER(_float0_10,MinScale);
		REG_MEMBER(_float0_10,MaxScale);
		REG_MEMBER(_int,Radius);
	}
	__constructor(OneTreeToGenerate){
		TreeiD=0;
		MinScale=0.8;
		MaxScale=1.2;
		Radius=60;
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
struct PrecompiledVertex{
	int Angle;
	int Height;
	int Azimut;
	OneMapVertex* Vertex;
	int VertexIndex;
	void CreateFromVertex(OneMapVertex& V);
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
void GetTexString(char* dest,ClonesArray<OneTexturePair>& tex){
	int N=tex.GetAmount();
	for(int i=0;i<N;i++){
		DWORD C1,C2;
		int GetFacture(int);
		int tn=GetFacture(tex[i]->Facture);
		DWORD C=tex[i]->Color;
		C=MulDWORD(C,512);
		C1=C|0xFF000000;
		C2=C&0xFFFFFF;
		if(i==0){
			if(N==1)C2=C1;
			sprintf(dest,"{tid %d 0 0 64 32  0 0 64 32  %X %X %X %X}",tn,C1,C2,C1,C2);
			dest+=strlen(dest);			
		}else
		if(i==N-1){			
			sprintf(dest,"{tid %d -64 0 64 32  0 0 64 32  %X %X %X %X 0}",tn,C2,C1,C2,C1);
			dest+=strlen(dest);			
		}else{
			sprintf(dest,"{tid %d -64 0 64 32  0 0 64 32  %X %X %X %X 0}",tn,C2,C1,C2,C1);
			dest+=strlen(dest);
			sprintf(dest,"{tid %d 0 0 64 32  0 0 64 32  %X %X %X %X}",tn,C1,C2,C1,C2);
			dest+=strlen(dest);
		}
	}
}
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
};
float OneSurfaceRule::GetHeight(int x,int y,float& Weight,int z){
	Weight=0;
	if(!Enabled)return 0;	
	int dx=x;
	int dy=y;
	if(AllowShapeCurving){
		ShapeCurving.ShiftXY(dx,dy);
		dx-=x;
		dy-=y;
	}else{
		dx=0;
		dy=0;
	}
	float bw=0.0f;
	float cBoundWidth=BoundWidth;
	if(VariateWidth){		
		bw=WidthVariationLow.GetFValue(x,y);
		cBoundWidth=cBoundWidth*(1-bw)+bw*ChangedWidth;
		if(AllowShapeCurving){
			dx*=(ShapeCurvingModulator*bw+1-bw);
			dy*=(ShapeCurvingModulator*bw+1-bw);
		}		
	}
	float w=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy)*surfScale);
	int H=TopHeight;
	if(AllowModulation&&!ModulateAfterTexturing){
		H*=ModulationFactor*ModulationVariation.GetFValue(x,y)*GlobalModulationVariation.GetFValue(x,y);
	}
	if(cBoundWidth<1)cBoundWidth=1;	
	Weight=0.5f+float(w)*16/cBoundWidth;
	if(Weight<0.0f)Weight=0.0f;
	if(Weight>1.0f)Weight=1.0f;
	Weight=Weight*Weight*(2-Weight)*(2-Weight);
	int N=ExtraConditions.GetAmount();
	if(N){
		PrecompiledVertex V;
		V.Angle=0;
		V.Azimut=0;
		OneMapVertex MV;
		MV.x=x;
		MV.y=y;
		MV.z=z;
		V.Vertex=&MV;
		V.VertexIndex=0;

		for(int i=0;i<N;i++){			
			V.Height=z;
			float WW=ExtraConditions[i]->GetConditionWeight(&V);
			Weight*=WW;
		}
	}
	return H;	
}
float OneSurfaceRule::PerformShift(Vector3D V,Vector3D& DV){//returns weight of vector shift
	int x=V.x;
	int y=V.y;
	DV=Vector3D(0,0,0);
	if(!(Enabled&&(AllowRandomShift||ModulateAfterTexturing)))return 0;	
	int dx=x;
	int dy=y;
	if(AllowShapeCurving){
		ShapeCurving.ShiftXY(dx,dy);
		dx-=x;
		dy-=y;
	}else{
		dx=0;
		dy=0;
	}
	float bw=0.0f;
	float cBoundWidth=BoundWidth;
	float cDegreeOfRandomShift=DegreeOfRandomShift;
	if(VariateWidth){		
		bw=WidthVariationLow.GetFValue(x,y);
		cBoundWidth=cBoundWidth*(1-bw)+bw*ChangedWidth;
		if(AllowShapeCurving){
			dx*=(ShapeCurvingModulator*bw+1-bw);
			dy*=(ShapeCurvingModulator*bw+1-bw);
		}
		if(AllowRandomShift){
			cDegreeOfRandomShift*=(RandomShiftModulator*bw+1-bw);
		}
	}

	float w=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy)*surfScale);	

	float Weight=0.0f;
	if(cBoundWidth<1)cBoundWidth=1;	
	Weight=0.5f+float(w)*16/cBoundWidth;
	if(Weight<0.0f)Weight=0.0f;
	if(Weight>1.0f)Weight=1.0f;
	//Weight=Weight*Weight*(2-Weight)*(2-Weight);

	if(AllowModulation&&ModulateAfterTexturing){
		DV.z=V.z*Weight*(ModulationFactor*ModulationVariation.GetFValue(x,y)*GlobalModulationVariation.GetFValue(x,y)-1.0f);
	}
	if(AllowRandomShift){
		float wL=DistField::Get(Map,float(x+dx-64)*surfScale,float(y+dy)*surfScale);
		float wR=DistField::Get(Map,float(x+dx+64)*surfScale,float(y+dy)*surfScale);
		float wU=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy-64)*surfScale);
		float wD=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy+64)*surfScale);

		float nx=(wR-wL)/4;
		float ny=(wD-wU)/4;

		float w1=1.0f-Weight;
		w1=w1*w1;

		float dr=RandomShiftFractal.GetFValue(x+y,x-y)-0.5f;

		DV.x+=Weight*w1*Weight*w1*dr*nx*Weight*cDegreeOfRandomShift*8000;
		DV.y+=Weight*w1*Weight*w1*dr*ny*Weight*cDegreeOfRandomShift*8000;
	}	
	return Weight;
}
float SurfaceGenerator::GetHeight(int x,int y){
	float SumH=0;
	float SumW=0;
	for(int i=0;i<Relief.GetAmount();i++)if(Relief[i]->Enabled){
		OneSurfaceType* OST=Relief[i];
		float SumHC=0;
		float SumWC=0;	
		int sr=OST->SurfaceRules.GetAmount();
		int ns=0;
		for(int j=0;j<sr;j++)if(OST->SurfaceRules[j]->Enabled)ns++;
		for(int j=0;j<sr;j++)if(OST->SurfaceRules[j]->Enabled){
			OneSurfaceRule* OSR=OST->SurfaceRules[j];
			float W=0;
			int zz=0;
			if(SumWC>0.01){
				zz=SumHC/SumWC;
			}
			float H=OSR->GetHeight(x,y,W,zz);
			SumHC+=H*W*ns+OST->BasicHeight*(1-W);
			SumWC++;           
		}
		if(SumWC<0.001){
			SumWC=1;
			SumHC=OST->BasicHeight;
		}
		if(OST->UseMap){
			BasePaintMap* BPM=MLayers->Maps[OST->Map];			
			float w=BPM->GetGreyPrecise(float(x)*surfScale,float(y)*surfScale);
			SumHC*=float(w)/255;
			SumWC*=float(w)/255;
		}
		SumH+=SumHC;
		SumW+=SumWC;
	}
	if(SumW<0.001)SumW=1;
	return SumH/SumW;
}
bool SurfaceGenerator::PerformShift(Vector3D& V){
	Vector3D SumV=Vector3D(0,0,0);
	float SumW=0;
	for(int i=0;i<Relief.GetAmount();i++)if(Relief[i]->Enabled){
		OneSurfaceType* OST=Relief[i];
		Vector3D SumVC=Vector3D(0,0,0);
		float SumWC=0;		
		for(int j=0;j<OST->SurfaceRules.GetAmount();j++)if(OST->SurfaceRules[j]->Enabled){
			OneSurfaceRule* OSR=OST->SurfaceRules[j];
			Vector3D DV=Vector3D(0,0,0);
			float W=OSR->PerformShift(V,DV);
			DV*=W;
			SumVC+=DV;
			SumWC+=W;           
		}
		if(SumWC<0.001){
			SumWC=1;			
		}
		if(OST->UseMap){
			BasePaintMap* BPM=MLayers->Maps[OST->Map];
			float w=BPM->GetGreyPrecise(V.x*surfScale,V.y*surfScale);
			SumVC*=float(w)/255;
			SumWC*=float(w)/255;
		}
		SumV+=SumVC;
		SumW+=SumWC;
	}
	if(SumW<0.001)SumW=1;
	SumV/=SumW;
	V+=SumV;
	return true;
}
void GeneratorSurfaceFunction::ModifyVertex(OneMapVertex& V)
{
	V.z=GPtr->GetHeight(V.x,V.y);
}	
bool GeneratorSurfaceFunction::ShiftVertex(OneMapVertex& V){
	Vector3D VV=Vector3D(V.x,V.y,V.z);
	GPtr->PerformShift(VV);
	assert(VV.x>-200);
	assert(VV.y>-200);
	V.x=VV.x;		
	V.y=VV.y;
	V.z=VV.z;
	assert(V.x>-200);
	assert(V.y>-200);
	return true;
}
float CheckHeightAndAngle::GetConditionWeight(PrecompiledVertex* V){
	float af,hf;
	if(V->Height>=StartHeight&&V->Height<=FinalHeight)hf=1.0f;
	else{
		if(HeightDiff){
			if(V->Height<StartHeight&&V->Height>StartHeight-HeightDiff){
				hf=1-float(StartHeight-V->Height)/HeightDiff;
			}else
				if(V->Height>FinalHeight&&V->Height<FinalHeight+HeightDiff){
					hf=1-float(V->Height-FinalHeight)/HeightDiff;
				}else hf=0.0f;
		}else hf=0.0f;
	}
	if(V->Angle>=StartAngle&&V->Angle<=FinalAngle)af=1.0f;
	else{
		if(AngleDiff){
			if(V->Angle<StartAngle&&V->Angle>StartAngle-AngleDiff){
				af=1-float(StartAngle-V->Angle)/AngleDiff;
			}else
				if(V->Angle>FinalAngle&&V->Angle<FinalAngle+AngleDiff){
					af=1-float(V->Angle-FinalAngle)/AngleDiff;
				}else af=0.0f;
		}else af=0.0f;
	}
	return hf*af;
}
float CheckHeight::GetConditionWeight(PrecompiledVertex* V){
	float hf;
	if(V->Height>=StartHeight&&V->Height<=FinalHeight)hf=1.0f;
	else{
		if(HeightDiff){
			if(V->Height<StartHeight&&V->Height>StartHeight-HeightDiff){
				hf=1-float(StartHeight-V->Height)/HeightDiff;
			}else
				if(V->Height>FinalHeight&&V->Height<FinalHeight+HeightDiff){
					hf=1-float(V->Height-FinalHeight)/HeightDiff;
				}else hf=0.0f;
		}else hf=0.0f;
	}
	return hf;
}

float CheckVariatedHeightAndAngle::GetConditionWeight(PrecompiledVertex* V){
	float af,hf;
	int DH=VariatedAddHeight*VariationLow.GetFValue(V->Vertex->x,V->Vertex->y);
	int H0=StartHeight+DH;
	int H1=FinalHeight+DH;
	if(V->Height>=H0&&V->Height<=H1)hf=1.0f;
	else{
		if(HeightDiff){
			if(V->Height<H1&&V->Height>H0-HeightDiff){
				hf=1-float(H0-V->Height)/HeightDiff;
			}else
				if(V->Height>H1&&V->Height<H1+HeightDiff){
					hf=1-float(V->Height-H1)/HeightDiff;
				}else hf=0.0f;
		}else hf=0.0f;
	}
	if(V->Angle>=StartAngle&&V->Angle<=FinalAngle)af=1.0f;
	else{
		if(AngleDiff){
			if(V->Angle<StartAngle&&V->Angle>StartAngle-AngleDiff){
				af=1-float(StartAngle-V->Angle)/AngleDiff;
			}else
				if(V->Angle>FinalAngle&&V->Angle<FinalAngle+AngleDiff){
					af=1-float(V->Angle-FinalAngle)/AngleDiff;
				}else af=0.0f;
		}else af=0.0f;
	}
	return hf*af;
}
float sq_norma(float x,float y);
void PrecompiledVertex::CreateFromVertex(OneMapVertex& V){
	Height=V.z;
	Angle=atan2(sq_norma(V.nx,V.ny),float(V.nz))*180/3.1415;
	Vertex=&V;
}
bool WholeMapGenerationPass=false;
void SurfaceGenerator::FillVertexTextures(PrecompiledVertex* V){
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	float R=0;
	float G=0;
	float B=0;
	float SummW=0;
	int FacturesWeight[128];
	memset(&FacturesWeight,0,sizeof FacturesWeight);
	int MinFacture=127;
	int MaxFacture=0;
	for(int i=0;i<Texturing.GetAmount();i++)if(Texturing[i]->Enabled){
		float cr=1.0f;
		OneTexturingStyle* OTS=Texturing[i];
		float sw=1.0f;
		for(int q=0;q<OTS->Conditions.GetAmount();q++){
			sw*=OTS->Conditions[q]->GetConditionWeight(V);
		}		
		//if(OTS->UseMap){
		//	BasePaintMap* BPM=MLayers->Maps[OTS->Map];
		//	sw=BPM->GetGreyPrecise(x*surfScale,y*surfScale);
		//	sw=sw*sw;
		//}
		if(sw>0.05){
			for(int j=0;j<OTS->TexturingRules.GetAmount();j++){
				OneTexturingRule* OTR=OTS->TexturingRules[j];
				if(OTR->Enabled&&OTR->Textures.GetAmount()){
					float w0=1.0f;
					for(int q=0;q<OTR->Conditions.GetAmount();q++){
						w0*=OTR->Conditions[q]->GetConditionWeight(V);
						if(w0<0.01)break;
					}					
					if(w0>0.01){
						w0*=sw;
						if(OTR->UseGlobalWeight){
							w0*=OTR->GlobalWeight*OTR->GlobalWeightVariation.GetFValue(x,y);
						}
						if(w0>0.001){
							int T1,T2;
							DWORD C1,C2;
							float w1,w2;
							if(OTR->Textures.GetAmount()>1){
								T1=OTR->Textures[0]->Facture;
								C1=OTR->Textures[0]->Color;
								T2=OTR->Textures[1]->Facture;
								C2=OTR->Textures[1]->Color;								
								w1=OTR->TexturesMixing.GetFValue(x,y);
								w2=1.0f-w1;
							}else{
								//one texture
								T1=T2=OTR->Textures[0]->Facture;
								C1=C2=OTR->Textures[0]->Color;
								w1=0;
								w2=1.0f;
							}
							float CB=(C1&255)*w1+(C2&255)*w2;
							float CG=((C1>>8)&255)*w1+((C2>>8)&255)*w2;
							float CR=((C1>>16)&255)*w1+((C2>>16)&255)*w2;
							R+=CR*w0;
							G+=CG*w0;
							B+=CB*w0;
							SummW+=w0;
							assert(T1<128&&T2<128);
							FacturesWeight[T1]+=w0*w1*1000;
							FacturesWeight[T2]+=w0*w2*1000;
							if(T1<MinFacture)MinFacture=T1;
							if(T2<MinFacture)MinFacture=T2;
							if(T1>MaxFacture)MaxFacture=T1;
							if(T2>MaxFacture)MaxFacture=T2;
							if(WholeMapGenerationPass && OTR->EnableTreesGenertion && OTR->Trees.GetAmount() && OTR->TreesDistribution.GetFValue(x,y)>=0.49){
								int xx=x+(mrand()%31-16);
								int yy=y+(mrand()%31-16);
                                int p=mrand()%OTR->Trees.GetAmount();
								OneTreeToGenerate* OTG=OTR->Trees[p];
								void AddSpriteScaled(int x,int y,SprGroup* SG,int id,int Radius,float Scale);
								AddSpriteScaled(xx,yy,&TREES,OTG->TreeID,OTG->Radius,OTG->MinScale+(OTG->MaxScale-OTG->MinScale)*mrand()/32768);
							}
						}
					}
				}
			}
		}
	}
	if(SummW>0.001){
		R/=SummW;
		G/=SummW;
		B/=SummW;
		if(R>255)R=255;
		if(G>255)G=255;
		if(B>255)B=255;
		int fc=0;
		int maxw=0;
		for(int i=MinFacture;i<=MaxFacture;i++){
			if(FacturesWeight[i]>maxw){
				maxw=FacturesWeight[i];
				fc=i;
			}
		}
		V->Vertex->Color=0xFF000000+(int(R)<<16)+(int(G)<<8)+int(B);
		V->Vertex->nFacture=fc;
	}
}
void SurfaceGenerator::TexturizeSurface(VertexBasedMesh& M){
	WholeMapGenerationPass=true;
	for(int i=0;i<M.Vert.GetAmount();i++){
		OneMapVertex* V=&M.Vert[i];
		PrecompiledVertex PV;
		PV.CreateFromVertex(*V);
		FillVertexTextures(&PV);
	}
	WholeMapGenerationPass=false;
}
SurfaceGenerator SURF;

float CheckSpecialLayer::GetConditionWeight(PrecompiledVertex* V){
	BasePaintMap* BPM=MLayers->Maps[Layer];
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	SURF.ShapeOfBound.ShiftXY(x,y);

	float v1=BPM->GetGreyPrecise((x-80)*surfScale,y*surfScale);
	float v2=BPM->GetGreyPrecise((x+80)*surfScale,y*surfScale);
	float v3=BPM->GetGreyPrecise(x*surfScale,(y+80)*surfScale);
	float v4=BPM->GetGreyPrecise(x*surfScale,(y-80)*surfScale);

	return (v1+v2+v3+v4)/4/255.0f;

}
float CheckIfNotInLayer::GetConditionWeight(PrecompiledVertex* V){
	BasePaintMap* BPM=MLayers->Maps[Layer];
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	SURF.ShapeOfBound.ShiftXY(x,y);
	return (255-BPM->GetGreyPrecise(x*surfScale,y*surfScale))/255.0f;
}
float CheckIfNearLayer::GetConditionWeight(PrecompiledVertex* V){	
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	float w=DistField::Get(Layer,float(x)*surfScale,float(y)*surfScale);
	int D=fabs(w*16);	
	if(StartDistance!=FinalDistance){
        float ww=(FinalDistance-D)*255/(FinalDistance-StartDistance);
		if(ww>255)ww=255;
		if(ww<0)ww=0;
		return ww/255.0f;
	}	
	return 0.0f;
}
void SurfaceGenerator::PerformTest(){
	DistField::ClearMaps();
	GeneratorSurfaceFunction SF;
	SF.GPtr=this;
	UniversalMap VBMesh;
	Vector3D v3dSun(2, 1, 1);
	VBMesh.CreateSurface(150,150,32,&SF);
	VBMesh.VertexBasedMesh::ModifySurface(2,&SF);
	VBMesh.CalcNormals();
	SURF.TexturizeSurface(VBMesh);
	for(int i = 0; i < VBMesh.Vert.GetAmount(); i++)
		if(VBMesh.Vert[i].nFacture > 100) VBMesh.Vert[i].nFacture = 100;

//	VBMesh.CalcShadow(v3dSun, 0.4, 80, 255);
	//VBMesh.InitUniversalMapQuad(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	VBMesh.QuadsSystem_Init();

	OneSplitMesh OSMesh;
	VertexBasedMeshRef VBMRef;
	VBMesh.GetMeshRefSphere(Vector3D(0,0,0),5000,&VBMRef);
	OSMesh.AdvSplitMesh(&VBMRef);
	DistField::ClearMaps();
	float A = 0;

	// ----------------------------------------
	extern UniversalMap g_UniversalMap;
	g_UniversalMap.GetMeshRefBox(0, 0, 10000, 20000, &VBMRef);
	OSMesh.AdvSplitMesh(&VBMRef);
	do{
		A += 0.08;
		//processing
		//MESH.TestDraw();	
		//NvertexOut(MESH);
		/*		v3dSun.x = cos(A),
		v3dSun.y = 1,
		v3dSun.z = sin(A);
		MESH.CalcShadow(v3dSun, 0.4, 80, 255);
		SPM.AdvSplitMesh(&MR);*/
		OSMesh.Draw();
		//VBMesh.TestDraw();

		FlipPages();
		ProcessMessages();
		IRS->ClearDevice(1, true, true);
		extern float fMapX;
		extern float fMapY;
		float fstep=0.5;
		if(GetKeyState(VK_LEFT)&0x8000){			
			fMapX-=fstep;
		}
		if(GetKeyState(VK_RIGHT)&0x8000){			
			fMapX+=fstep;;
		}
		if(GetKeyState(VK_UP)&0x8000){			
			fMapY-=fstep;
		}
		if(GetKeyState(VK_DOWN)&0x8000){			
			fMapY+=fstep;
		}
		void SetupCamera();
		SetupCamera();
	}while(!(GetKeyState(VK_SPACE)&0x8000));	
}
class OneLayerColor:public BaseClass{
public:
	_str Name;
	DWORD Color;
	SAVE(OneLayerColor);
	REG_AUTO(Name);
	REG_MEMBER(_color,Color);
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
LayersList LAYERSLIST;
void TestLayersColors(){
	LAYERSLIST.reset_class(&LAYERSLIST);
	LAYERSLIST.SafeReadFromFile("Layers.xml");
	if(LAYERSLIST.Layers.GetAmount()&&MLayers){
		for(int i=0;i<MLayers->Maps.GetAmount();i++){
			if(i<LAYERSLIST.Layers.GetAmount()){
				((GreyMap*)MLayers->Maps[i])->Color=LAYERSLIST.Layers[i]->Color;
				*MLayers->MapsNames[i]=LAYERSLIST.Layers[i]->Name;
			}
		}
		for(int i=MLayers->Maps.GetAmount();i<LAYERSLIST.Layers.GetAmount();i++){
			MLayers->Maps.Add(new GreyMap(LAYERSLIST.Layers[i]->Color));
			_str* s=new _str;
            *s=LAYERSLIST.Layers[i]->Name;
			MLayers->MapsNames.Add(s);
		}
		Enumerator* E1=ENUM.Get("LAYERSLIST");
		E1->Clear();
		for(int i=0;i<MLayers->Maps.GetAmount();i++){
			char* s=MLayers->MapsNames[i]->str;
			E1->Add(s?s:"???");
		}
	}
}
void AddLayersEditor(){
    AddStdEditor("MapLayersEditor",&LAYERSLIST,"Layers.xml",RCE_DEFAULT);
}
void RegisterSurfaceClasses(){
	REG_CLASS(SurfaceGenerator);
	REG_CLASS(OneTexturingStyle);
	REG_CLASS(OneSurfaceType);
	REG_CLASS(OneSurfaceRule);
	REG_CLASS(OneTexturingRule);
	REG_CLASS(OneTexturePair);
	REG_CLASS(OneGenCondition);
	REG_CLASS(OneTexturingCondition);
	REG_CLASS(CheckHeightAndAngle);
	REG_CLASS(CheckHeight);
	REG_CLASS(CheckVariatedHeightAndAngle);
	REG_CLASS(CheckSpecialLayer);
	REG_CLASS(CheckIfNotInLayer);
	REG_CLASS(CheckIfNearLayer);
	REG_CLASS(OneLayerColor);	
}
SquareMap<float>* DistField::map[MaxDistSet];
void DistField::ClearMaps(){
	for(int i=0;i<MaxDistSet;i++)if(map[i]){
		delete(map[i]);
		map[i]=NULL;
	}
}
float DistField::Get(int Index,float x,float y){
	if(!map[Index]){
		map[Index]=new SquareMap<float>;
		SquareMap<float>* FL=map[Index];
		FL->SetSize(512,512,0);
		BasePaintMap* BPM=MLayers->Maps[Index];	
		for(int ix=0;ix<512;ix++){
			for(int iy=0;iy<512;iy++){
				float v=BPM->GetGreyPrecise(float(ix)/512.0f,float(iy)/512.0f);
				if(v<128)FL->Set(ix,iy,-10000);
				else FL->Set(ix,iy,10000);
			}
		}
		DynArray<DWORD> Edges;
		Edges.Check(4096);
		for(int ix=0;ix<512;ix++){
			for(int iy=0;iy<512;iy++){
				float v=FL->Get(ix,iy);
				float vL=FL->Get(ix-1,iy);
				float vR=FL->Get(ix+1,iy);
				float vU=FL->Get(ix,iy-1);
				float vD=FL->Get(ix,iy+1);
				bool add=false;
				if(v>5000){
					if(vL<-5000||vR<-5000||vU<-5000||vD<-5000)add=true;
				}else{
					if(vL>5000||vR>5000||vU>5000||vD>5000)add=true;
				}
				if(add){
					DWORD XY=ix+(iy<<16);
					Edges.Add(XY);
				}
			}
		}
		for(int i=0;i<Edges.GetAmount();i++){
			int x=Edges[i]&0xFFFF;
			int y=Edges[i]>>16;
			float v=FL->Get(x,y);
			if(v<0)FL->Set(x,y,-1);
			else FL->Set(x,y,1);
		}
		DynArray<DWORD> NextEdges;
		NextEdges.Check(4096);
		bool change=false;
		float curV=3;
		do{
			change=false;
			for(int i=0;i<Edges.GetAmount();i++){
				int x=Edges[i]&0xFFFF;
				int y=Edges[i]>>16;
				float vL=FL->Get(x-1,y);
				float vR=FL->Get(x+1,y);
				float vU=FL->Get(x,y-1);
				float vD=FL->Get(x,y+1);				
				if(vL<-5000){
					NextEdges.Add((x-1)+(y<<16));
					FL->Set(x-1,y,-curV);
					change=true;
				}
				if(vL>5000){
					NextEdges.Add((x-1)+(y<<16));
					FL->Set(x-1,y,curV);
					change=true;
				}
				if(vR<-5000){
					NextEdges.Add((x+1)+(y<<16));
					FL->Set(x+1,y,-curV);
					change=true;
				}
				if(vR>5000){
					NextEdges.Add((x+1)+(y<<16));
					FL->Set(x+1,y,curV);
					change=true;
				}
				if(vU<-5000){
					NextEdges.Add(x+((y-1)<<16));
					FL->Set(x,y-1,-curV);
					change=true;
				}
				if(vU>5000){
					NextEdges.Add(x+((y-1)<<16));
					FL->Set(x,y-1,curV);
					change=true;
				}
				if(vD<-5000){
					NextEdges.Add(x+((y+1)<<16));
					FL->Set(x,y+1,-curV);
					change=true;
				}
				if(vD>5000){
					NextEdges.Add(x+((y+1)<<16));
					FL->Set(x,y+1,curV);
					change=true;
				}
			}
			if(change){
				Edges.FastClear();
				for(int i=0;i<NextEdges.GetAmount();i++){
					Edges.Add(NextEdges[i]);
				}
				NextEdges.FastClear();
			}
			curV+=2;
		}while(change);
		//smoothing
		SquareMap<float> temp;
		FL->CopyTo(temp);
		for(int i=0;i<2;i++){		
			for(int ix=1;ix<511;ix++){
				for(int iy=1;iy<511;iy++){
					float va=(FL->Get(ix-1,iy)+FL->Get(ix+1,iy)+FL->Get(ix,iy-1)+FL->Get(ix,iy+1))/4;
					temp.Set(ix,iy,va);
				}			
			}
			temp.CopyTo(*FL);
		}
	}
	//map  created, now we can get value
	SquareMap<float>* FL=map[Index];
	int lx=512;
	int ly=512;	
	x*=lx;
	y*=ly;
	int xx=int(x);
	int yy=int(y);
	float f1=FL->Get(xx,yy);
	float f2=FL->Get(xx+1,yy);
	float f3=FL->Get(xx,yy+1);
	float f4=FL->Get(xx+1,yy+1);
	float dx=x-xx;
	float dy=y-yy;
	return f1+dx*(f2-f1)+dy*(f3-f1)+dx*dy*(f4+f1-f3-f2);    
}

extern UniversalMap g_UniversalMap;

void SaveMLayersToFileInCompactForm(char* filename);
bool LoadMLayersFromFileInCompactForm(char* filename);

//Options=1-init, 2-process, 3-ok pressed, 4-cancel pressed 
bool rce_SurfaceCallback(ClassEditor* CE,BaseClass* BC,int Options){
	if(Options==1){
		LastKey=0;
		KeyPressed=false;
		TestLayersColors();	
		if(!LoadMLayersFromFileInCompactForm("SurfaceShape.dat")){
			bool CheckIfFileExists(char* Name);
			if(CheckIfFileExists("SurfaceShape.xml")){
				MLayers->reset_class(MLayers);
				MLayers->SafeReadFromFile("SurfaceShape.xml");
				TestLayersColors();	
			}
		}
	}
	if(Options==2){
		if(MLayers->Maps.GetAmount()){
			Enumerator* E=ENUM.Get("LAYERSLIST");
			for(int i=0;i<MLayers->Maps.GetAmount();i++){
				MLayers->Maps[i]->Desc.Clear();
				MLayers->Maps[i]->Desc=E->Get(i);
				MLayers->Maps[i]->Desc+=" : ";
			}
			int NR=SURF.Relief.GetAmount();
			for(int i=0;i<NR;i++){
				_str* s=&MLayers->Maps[SURF.Relief[i]->Map]->Desc;				
				_str* ads=&SURF.Relief[i]->Description;
				if(ads->str&&ads->str[0]){
					*s+=*ads;
					*s+=" ";
				}
				int NR=SURF.Relief[i]->SurfaceRules.GetAmount();
				for(int j=0;j<NR;j++){
					_str* s=&MLayers->Maps[SURF.Relief[i]->SurfaceRules[j]->Map]->Desc;
					_str* ads=&SURF.Relief[i]->SurfaceRules[j]->Description;
					if(ads->str&&ads->str[0]){
						*s+=*ads;
						*s+=" ";
					}
				}

			}
			for(int i=0;i<SURF.Texturing.GetAmount();i++){
				/*if(SURF.Texturing[i]->UseMap){
					_str* s=&MLayers->Maps[SURF.Texturing[i]->Map]->Desc;
					_str* ads=&SURF.Texturing[i]->Description;
					if(ads->str&&ads->str[0]){
						*s+=*ads;
						*s+=" ";
					}                    
				}*/
				for(int j=0;j<SURF.Texturing[i]->TexturingRules.GetAmount();j++){
					OneTexturingRule* TR=SURF.Texturing[i]->TexturingRules[j];
					for(int q=0;q<TR->Conditions.GetAmount();q++){
						CheckSpecialLayer* L=dynamic_cast<CheckSpecialLayer*>(TR->Conditions[q]);
						if(L){
							_str* s=&MLayers->Maps[L->Layer]->Desc;
							_str* ads=&TR->Description;
							if(ads->str&&ads->str[0]){
								*s+=*ads;
								*s+=" ";
							}
						}
					}					
				}
			}
			for(int i=0;i<MLayers->Maps.GetAmount();i++){
				_str* s=&MLayers->Maps[i]->Desc;
				if(s&&s->str){
					void Translit(char* s);
					Translit(s->str);
				}
			}
		}
	}
	if(Options==3){//Ok
		//MLayers->WriteToFile("SurfaceShape.xml");
		SaveMLayersToFileInCompactForm("SurfaceShape.dat");
		if(!g_UniversalMap.Idxs.GetAmount())SURF.DontGenerateRelief=false;
		if(!SURF.DontGenerateRelief){
			g_UniversalMap.Release();
		}
		DistField::ClearMaps();
		GeneratorSurfaceFunction SF;
		SF.GPtr=&SURF;		
		Vector3D a;	
		int NX=SURF.Size/32;
		if(NX<40)NX=40;
		if(NX>512)NX=512;
		if(!SURF.DontGenerateRelief){
			g_UniversalMap.CreateSurface(NX,NX, 32, &SF);
			g_UniversalMap.VertexBasedMesh::ModifySurface(NX<300,&SF);
			g_UniversalMap.Smooth(0.5,2);
		}
		g_UniversalMap.CalcNormals();
		a.x=2;
		a.y=1;
		a.z=1;
		a.normalize();
		SURF.TexturizeSurface(g_UniversalMap);
		g_UniversalMap.SmoothColors(0,0,20000);
		g_UniversalMap.SmoothColors(0,0,20000);
		g_UniversalMap.SmoothColors(0,0,20000);
		int N = 0, maxNFacture = 0;
		int GetNFactures();
		maxNFacture = GetNFactures() - 1;
		N = g_UniversalMap.Vert.GetAmount();
		for(int i = 0; i < N; i++)
			if(g_UniversalMap.Vert[i].nFacture > maxNFacture)
				g_UniversalMap.Vert[i].nFacture = maxNFacture;
		//g_UniversalMap.CalcShadow(a, 0.2, 40, 255);
		//g_UniversalMap.CreateShadow(a, 40, 255);
		//g_UniversalMap.InitUniversalMapQuad(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		g_UniversalMap.QuadsSystem_Init();
		g_UniversalMap.FillMapOfHeights();
		//float CreateNaturalShadow(VertexBasedMesh *, int);
		//CreateNaturalShadow(&g_UniversalMap, 2048);
//		float CreateNaturalShadow(UniversalMap &);
//		CreateNaturalShadow(g_UniversalMap);
		extern CSun g_Sun;
		g_UniversalMap.Shade(true, g_UniversalMap.m_pSun);
		DistField::ClearMaps();
		void ResetGroundCache();
		ResetGroundCache();
		// BEGIN: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		char Buffer[256], Str[4096];
		sprintf(Buffer, "QuadX0 = %d\tQuadY0 = %d\r\n",
			g_UniversalMap.QuadX0, g_UniversalMap.QuadY0);
		strcpy(Str, Buffer);
		int UMLx = 0, UMLy = 0;
		UMLx = g_UniversalMap.QuadLx * g_UniversalMap.QuadsNx,
			UMLy = g_UniversalMap.QuadLy * g_UniversalMap.QuadsNy;
		sprintf(Buffer, "UMLx = %d\tUMLy = %d\r\n", UMLx, UMLy);
		strcat(Str, Buffer);
		sprintf(Buffer, "g_UniversalMap.Vert.GetAmount() == %d", g_UniversalMap.Vert.GetAmount());
		strcat(Str, Buffer);
		sprintf(Buffer, "\r\ng_UnviersalMap.Idxs.GetAmount() == %d", g_UniversalMap.Idxs.GetAmount());
		strcat(Str, Buffer);
		bool FacturesInUse[SPLITMESH_MAXNFACTURES];
		ZeroMemory(FacturesInUse, sizeof(FacturesInUse));
		N = g_UniversalMap.Vert.GetAmount();
		for(i = 0; i < N; i++) FacturesInUse[g_UniversalMap.Vert[i].nFacture] = true;
		maxNFacture = GetNFactures() - 1;
		strcat(Str, "\r\nNumbers of factures in use: ");
		for(i = 0; i <= maxNFacture; i++)
			if(FacturesInUse[i] == true)
			{
				sprintf(Buffer, " %d", i);
				strcat(Str, Buffer);
			}
			MessageBox(NULL, Str, "rce_SurfaceCallback stats:", 0);
		// END: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}
	return false;
}
bool CheckIfNewTerrain(){
	static bool SF=false;
	static bool init=false;
	if(!init){
		FILE* F=fopen("newsurf.dat","r");
		if(F){
			SF=true;			
			fclose(F);
		}
		init=true;
	}
	return SF;
}
void RegisterSurfEditor(){
	if(!MLayers){
		MLayers=new MapsScope;
		MLayers->SetDefaults();
		void TestLayersColors();
		TestLayersColors();
	}
	//SURF.Test.SetParent(&SURF);	
	AddStdEditor("SurfaceEditor",&SURF,"Surface3D.xml",RCE_DEFAULT,rce_SurfaceCallback,NULL,'S');        
	if(GetKeyState(VK_CONTROL)&0x8000){
		void SelectEditor();
		SelectEditor();
	}
}
void SaveNewSurface(ResFile F){
	//x-2 y-2 z-2 Color-4 Shadow-1 Facture-1 nx-1 ny-1 nz-1 res-1 total: 16
	int sz=4+4+4+g_UniversalMap.Vert.GetAmount()*16+g_UniversalMap.Idxs.GetAmount()*4;
	int c='HSEM';
	RBlockWrite(F,&c,4);
	RBlockWrite(F,&sz,4);
	c=g_UniversalMap.Vert.GetAmount();
	RBlockWrite(F,&c,4);	
	c=g_UniversalMap.Idxs.GetAmount();
	RBlockWrite(F,&c,4);
	for(int i=0;i<g_UniversalMap.Vert.GetAmount();i++){
		OneMapVertex* V=&g_UniversalMap.Vert[i];
		RBlockWrite(F,&V->x,2);
		RBlockWrite(F,&V->y,2);
		RBlockWrite(F,&V->z,2);
		RBlockWrite(F,&V->Color,4);
		RBlockWrite(F,&V->Shadow,1);
		RBlockWrite(F,&V->nFacture,1);
		RBlockWrite(F,&V->nx,1);
		RBlockWrite(F,&V->ny,1);
		RBlockWrite(F,&V->nz,1);
		c=0;
		RBlockWrite(F,&c,1);
	}
	RBlockWrite(F,&g_UniversalMap.Idxs[0],g_UniversalMap.Idxs.GetAmount()*4);
}
void SelectSurfaceType(bool New);
void LoadNewSurface(ResFile F){
	SelectSurfaceType(true);
	int Nv=0;
	int Ni=0;
	RBlockRead(F,&Nv,4);
	RBlockRead(F,&Ni,4);
	g_UniversalMap.Release();
	g_UniversalMap.Idxs.Add(0,Ni);
	OneMapVertex V;
	g_UniversalMap.Vert.Add(V,Nv);
	for(int i=0;i<g_UniversalMap.Vert.GetAmount();i++){
		OneMapVertex* V=&g_UniversalMap.Vert[i];
		RBlockRead(F,&V->x,2);
		RBlockRead(F,&V->y,2);
		RBlockRead(F,&V->z,2);
		RBlockRead(F,&V->Color,4);
		RBlockRead(F,&V->Shadow,1);
		RBlockRead(F,&V->nFacture,1);
		RBlockRead(F,&V->nx,1);
		RBlockRead(F,&V->ny,1);
		RBlockRead(F,&V->nz,1);
		int c;
		RBlockRead(F,&c,1);
	}
	RBlockRead(F,&g_UniversalMap.Idxs[0],g_UniversalMap.Idxs.GetAmount()*4);
	//g_UniversalMap.InitUniversalMapQuad(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	g_UniversalMap.QuadsSystem_Init();
	g_UniversalMap.FillMapOfHeights();
}