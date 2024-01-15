#include "stdheader.h"
#include "ExMapGenerator.h"
__declspec(dllexport)
void SetCurrentProgress(char*,int,int);
#define MaxTex  8
#define MaxFact 8
#define MaxCond 64
int mrand();
TreesScope TSC;
void ClearTreesGroups(){
	TSC.Clear();
}
void AddTreesToGroup(char* Section,SprGroup* SG,int Index){
	TSC.Add(Section,SG,Index);
}
class TexSet:public BaseClass{
public:
	int TextureSet[MaxTex];
	int FactureSet[MaxFact];
	TexSet(){
		for(int i=0;i<MaxTex;i++){
			TextureSet[i]=-1;
			FactureSet[i]=-1;
		}
	}
	virtual int GetAmountOfElements(){
		for(int i=0;i<MaxTex;i++)if(TextureSet[i]==-1)return i*2+1;
		return MaxTex*2;
	}
	virtual bool DelElement(int index){
		int idx=index/2;
		if(idx<MaxTex){
			if(idx<MaxTex-1){
				memcpy(TextureSet+idx,TextureSet+idx+1,(MaxTex-idx-1)*4);
				memcpy(FactureSet+idx,FactureSet+idx+1,(MaxTex-idx-1)*4);
			}
			TextureSet[MaxTex-1]=-1;
			FactureSet[MaxTex-1]=-1;
            return true;
		}else return false;
	}
	SAVE(TexSet)
	REG_MEMBER_EX(_gtexture,TextureSet[0],Tex0);
	REG_MEMBER_EX(_gfacture,FactureSet[0],Facture0);
	REG_MEMBER_EX(_gtexture,TextureSet[1],Tex1);
	REG_MEMBER_EX(_gfacture,FactureSet[1],Facture1);
	REG_MEMBER_EX(_gtexture,TextureSet[2],Tex2);
	REG_MEMBER_EX(_gfacture,FactureSet[2],Facture2);
	REG_MEMBER_EX(_gtexture,TextureSet[3],Tex3);
	REG_MEMBER_EX(_gfacture,FactureSet[3],Facture3);
	REG_MEMBER_EX(_gtexture,TextureSet[4],Tex4);
	REG_MEMBER_EX(_gfacture,FactureSet[4],Facture4);
	REG_MEMBER_EX(_gtexture,TextureSet[5],Tex5);
	REG_MEMBER_EX(_gfacture,FactureSet[5],Facture5);
	REG_MEMBER_EX(_gtexture,TextureSet[6],Tex6);
	REG_MEMBER_EX(_gfacture,FactureSet[6],Facture6);
	REG_MEMBER_EX(_gtexture,TextureSet[7],Tex7);
	REG_MEMBER_EX(_gfacture,FactureSet[7],Facture7);
	ENDSAVE
};
class OneTreesSet:public BaseClass{
public:
	bool Enabled;
	int Trees;
	float MinDistance;
	float WholeDensity;
	_fractal Density;
	OneTreesSet(){
		Enabled=0;
		WholeDensity=1.0f;
		MinDistance=0.3f;
	}
	SAVE(OneTreesSet)
	REG_MEMBER(_bool,Enabled);
	REG_ENUM(_index,Trees,TREESGROUPS);
	REG_MEMBER(_float01,MinDistance);
	REG_MEMBER(_float0_10,WholeDensity);
	REG_AUTO(Density);
	ENDSAVE
};

#define MaxTrees 16
class TreesGroup:public BaseClass{
public:
	OneTreesSet Trees[MaxTrees];
	virtual int GetAmountOfElements(){
		if(Trees[MaxTrees-1].Enabled)return MaxTrees;
		for(int i=MaxTrees-2;i>=0;i--)if(Trees[i].Enabled)return i+2;
		return 1;
	}
	SAVE(TreesGroup)
	for(int i=0;i<MaxTrees;i++){
		char cc[32];
		sprintf(cc,"TreesSet%d",i);
		REG_AUTO_EX2(Trees[i],cc);
	}
	ENDSAVE
};
struct TexResult{
	int   t1;//textures
	int   t2;
	int   w1;//weights
	int   w2;
	int   f1;//factures
	int   f2;
	float fWeight1;
	float fWeight2;
    int   TreesGrp;//index of trees group
	float TreesDensity;
	float TreesDist;
	byte  LockMask;
};
class WeightMap:public BaseClass{
public:
    bool   Enabled;
	MapRef Map;
	WeightMap(){
		Enabled=0;
		Map.TopLayerIndex=3;
	}
	SAVE(WeightMap)
	REG_MEMBER(_bool,Enabled);
	REG_AUTO(Map);
	ENDSAVE
};
class LockInfo:public BaseClass{
public:
	LockInfo(){
		Lock0=0;
		Lock1=0;
		Lock2=0;
		Lock3=0;
	}
	bool Lock0;
	bool Lock1;
	bool Lock2;
	bool Lock3;
	SAVE(LockInfo)
	REG_MEMBER_EX(_bool,Lock0,LockLandUnits);
	REG_MEMBER_EX(_bool,Lock2,LockHorces);	
	ENDSAVE
};
class TCondition:public BaseClass{
public:
	_str Description;
	bool Enabled;
	bool Smoothing;
	int StartHeight;
	int FinHeight;
	int MaxHeightDiff;
	int StartAngle;
	int FinAngle;
	int MaxAngleDiff;
	int CheckDistance;
	int DistanceCheckType;//0-gradient,1-radius,2-isoline
	bool ExcludeCentralPoint;
	WeightMap Mask;
	float WholeWeight;
	_fractal WeightVariation;
	_fractal TexturesMixing;	
	float FacturesWeight;
	_fractal FactWeightVariaton;
	TexSet Textures;
	LockInfo Locking;
    TreesGroup Trees;
	virtual const char* GetThisElementView(){
		if(!Description.pchar())return NULL;
		static char cc[128];
		sprintf(cc,"Condition: %s",Description.pchar());
		return cc;
	};
	TCondition(){
		Enabled=0;
		Smoothing=0;
		StartHeight=0;
		FinHeight=0;
		MaxHeightDiff=0;
		StartAngle=0;
		FinAngle=0;
		MaxAngleDiff=0;
		WholeWeight=1.0f;
		CheckDistance=0;
		ExcludeCentralPoint=0;
		DistanceCheckType=0;
		WholeWeight=1.0f;
		FacturesWeight=1.0f;
		FactWeightVariaton.Center=0.5f;
		FactWeightVariaton.Contrast=0;
		//memset(TextureSet,0,sizeof TextureSet);
		//memset(FactureSet,0,sizeof FactureSet);
	}
	void GetLocalResult(int x,int y,int Height,int Angle,TexResult& result);
	bool GetResult(int x,int y,int Height,int Angle,TexResult& result);
	SAVE(TCondition)
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);
	REG_MEMBER(_bool,Smoothing);
	REG_MEMBER(_int,StartHeight);
	REG_MEMBER(_int,FinHeight);
	REG_MEMBER(_int,MaxHeightDiff);
	REG_MEMBER(_int,StartAngle);
	REG_MEMBER(_int,FinAngle);
	REG_MEMBER(_int,MaxAngleDiff);
	REG_MEMBER(_int,CheckDistance);
	REG_ENUM(_index,DistanceCheckType,DISTCHECKTYPE);
	REG_MEMBER(_bool,ExcludeCentralPoint);
	REG_AUTO(Mask);
	REG_MEMBER(_float0_10,WholeWeight);
	REG_AUTO(WeightVariation);
	REG_AUTO(TexturesMixing);
	REG_MEMBER(_float01,FacturesWeight);
	REG_AUTO(FactWeightVariaton);
	REG_AUTO(Textures);
	REG_AUTO(Locking);
	REG_AUTO(Trees);
	ENDSAVE
};
#define NTConditionMembers 6 
#define NTerrParamsMembers 6 
class TerrParams:public BaseClass{
public:
	TerrParams(){
		Enabled=false;
		GlobalWeight=1.0f;
		WeightPreferring=0;
		Mask.Map.TopLayerIndex=0;
	}
	_str Description;
	bool Enabled;
	float GlobalWeight;
	_fractal WeightVariation;
	int WeightPreferring;
	WeightMap Mask;
	TCondition Condition[MaxCond];
	virtual int GetAmountOfElements(){
		if(Condition[MaxCond-1].Enabled||Condition[MaxCond-1].Textures.GetAmountOfElements()>1)return MaxCond+NTerrParamsMembers;
		for(int i=MaxCond-1;i>=0;i--)if(Condition[i].Enabled||Condition[i].Textures.GetAmountOfElements()>1)return i+2+NTerrParamsMembers;
		return 1+NTerrParamsMembers;
	}
	virtual const char* GetThisElementView(){
		if(!Description.pchar())return NULL;
		static char cc[128];
		sprintf(cc,"Terrain: %s",Description.pchar());
		return cc;
	};
	SAVE(TerrParams)
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);
	REG_MEMBER(_float0_10,GlobalWeight);
	REG_AUTO(WeightVariation);
	REG_AUTO(Mask);
	REG_ENUM(_index,WeightPreferring,WeightPreferring);
	for(int i=0;i<MaxCond;i++){
		char cc[16];
		sprintf(cc,"Rule%d",i);
		REG_AUTO_EX2(Condition[i],cc);
	}	
	ENDSAVE
};
class OneReliefRule:public BaseClass{
public:
	OneReliefRule(){
		Enabled=0;
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
	_str     Description;
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
	virtual const char* GetThisElementView(){
		if(!Description.pchar())return NULL;
		static char cc[128];
		sprintf(cc,"Relief: %s",Description.pchar());
		return cc;
	};
	SAVE(OneReliefRule)
	REG_AUTO  (Description);
	REG_MEMBER(_bool,Enabled);
	REG_MEMBER(_int,AverageHeight);
	REG_MEMBER(_int,StartHeight);
	REG_ENUM  (_index,HeightOperation,HEIGHTOP);
	REG_MEMBER(_int,SmoothSteps);
	REG_MEMBER(_int,SmoothMaxVariation);
	REG_AUTO  (SmoothVariation);
	REG_ENUM  (_index,SmoothType,SMOOTHTYPE);
	REG_MEMBER(_bool,CutOffTop);
	REG_AUTO  (Layer);
	REG_AUTO  (EdgesShape);
	REG_AUTO  (TopShape);
	REG_AUTO  (TopModulation);
	REG_MEMBER(_bool,PaintWater);
	REG_MEMBER(_DWORD,WaterColor1);
	REG_MEMBER(_DWORD,WaterColor2);
	REG_AUTO(WaterMixing);
	ENDSAVE
	int GetHeightInPoint(int x,int y,int& sm,DWORD& WColor,int& Weight){
		sm=0;
        int sz=240*32<<ADDSH;
		float fx=float(x)/sz;
		float fy=float(y)/sz;
        BasePaintMap* BPM=MLayers->Maps[Layer.TopLayerIndex];
		float H0=BPM->GetGreyPrecise(fx,fy);
		Weight=int(H0);
		if(PaintWater&&Weight>0){
			int fv=WaterMixing.GetFValue(x,y)*255;
			DWORD C=MixDWORD(WaterColor1,WaterColor2,fv,255-fv);
			int WM=Weight*6;
			if(WM>255)WM=255;
			if(WM<0)WM=0;
            WColor=MixDWORD(C,WColor,WM,255-WM);			
		}
		float Edge=EdgesShape.GetFValue(x,y);
		float H=(H0+(Edge-0.5f)*128.0f-64.0f)*2.0f;
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
		return int(H);
	}
};
#define MaxRRules 32
#define NReliefGeneratorMembers 2
class ReliefGenerator:public BaseClass{
public:
	int StartHeight;
	_fractal GlobalModulation;
	virtual int GetAmountOfElements(){
		if(Rules[MaxRRules-1].Enabled||Rules[MaxRRules-1].AverageHeight)return MaxRRules+NReliefGeneratorMembers;
		for(int i=MaxRRules-1;i>=0;i--)if(Rules[i].Enabled||Rules[i].AverageHeight)return i+2+NReliefGeneratorMembers;
		return 1+NReliefGeneratorMembers;
	}
	OneReliefRule Rules[MaxRRules];
	SAVE(ReliefGenerator)
	REG_MEMBER(_int,StartHeight);
    REG_AUTO(GlobalModulation);
	for(int i=0;i<MaxRRules;i++){
		char s[32];
		sprintf(s,"Rule%d",i);
		REG_AUTO_EX2(Rules[i],s);
	}
	ENDSAVE
	int GetHeightInPoint(int x,int y,int& sm,DWORD& WColor){
		int H=StartHeight*2*GlobalModulation.GetFValue(x,y);
		sm=0;
		for(int i=0;i<MaxRRules;i++)if(Rules[i].Enabled){
			int sm1=0;
			int W;
			int rr=Rules[i].GetHeightInPoint(x,y,sm1,WColor,W);
			sm+=sm1;
			switch(Rules[i].HeightOperation){
			case 0:
				H+=rr;
				break;
			case 1:
				H=min(rr,H);
				break;
			case 2:
				H=max(rr,H);
				break;
			case 3:
                H=(rr*W+H*(255-W))/255;
				break;
			}
		}
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
							THMap[vert]=(THMap[vert]*2+THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
								THMap[vert+1]+THMap[vert-VertInLine+1]+THMap[vert-VertInLine-1]+3)/8;
						}else{
							THMap[vert]=(THMap[vert]*2+THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
								THMap[vert+1]+THMap[vert+VertInLine+1]+THMap[vert+VertInLine-1]+3)/8;
						};
					};
					Smooth[vert]--;
					ch=1;
				}
			}
		}while(ch);
	}
};
#define MaxTerr 16
#define NLandsScopeMembers 6
class SrcClass:public BaseClass{
public:
SAVE(SrcClass)
ENDSAVE
};
class CClass1:public SrcClass{
public:
	SAVE(CClass1)
	REG_PARENT(SrcClass);
	ENDSAVE
};
class CClass2:public SrcClass{
public:
	SAVE(CClass2)
	REG_PARENT(SrcClass);
	ENDSAVE
};
class LandsScope:public BaseClass{
public:
	virtual int GetAmountOfElements(){
		if(Terr[MaxTerr-1].Enabled||Terr[MaxTerr-1].GetAmountOfElements()>NTerrParamsMembers+1)return MaxTerr+NLandsScopeMembers;
		for(int i=MaxTerr-2;i>=0;i--)if(Terr[i].Enabled||Terr[i].GetAmountOfElements()>NTerrParamsMembers+1)return i+NLandsScopeMembers+2;
		return 1+NLandsScopeMembers;
	}
	LandsScope(){
		REG_CLASS(SrcClass);
		REG_CLASS(CClass1);
		REG_CLASS(CClass2);
		PostProcessType=1;
		PostStagesAmount=8;
		GenerateTextures=1;
		GenerateRelief=0;
	}
	int PostProcessType;
	int PostStagesAmount;
	bool GenerateTextures;
	bool GenerateRelief;	
	MapRef AllMaps;
	ReliefGenerator Relief;
	//ClassArray<SrcClass> LA;
	TerrParams Terr[MaxTerr];	
	bool GetResult(int x,int y,int Height,int Angle,TexResult& result);
	SAVE(LandsScope)
	REG_AUTO(AllMaps);
	REG_ENUM(_index,PostProcessType,PostProcessType);
	REG_MEMBER(_int,PostStagesAmount);
	REG_MEMBER(_bool,GenerateTextures);
	REG_MEMBER(_bool,GenerateRelief);
	REG_AUTO(Relief);
	//REG_AUTO(LA);
	REG_AUTO_EX(Terr[0],Terrain0);
	REG_AUTO_EX(Terr[1],Terrain1);
	REG_AUTO_EX(Terr[2],Terrain2);
	REG_AUTO_EX(Terr[3],Terrain3);
	REG_AUTO_EX(Terr[4],Terrain4);
	REG_AUTO_EX(Terr[5],Terrain5);
	REG_AUTO_EX(Terr[6],Terrain6);
	REG_AUTO_EX(Terr[7],Terrain7);
	REG_AUTO_EX(Terr[8],Terrain8);
	REG_AUTO_EX(Terr[9],Terrain9);
	REG_AUTO_EX(Terr[10],Terrain10);
	REG_AUTO_EX(Terr[11],Terrain11);
	REG_AUTO_EX(Terr[12],Terrain12);
	REG_AUTO_EX(Terr[13],Terrain13);
	REG_AUTO_EX(Terr[14],Terrain14);
	REG_AUTO_EX(Terr[15],Terrain15);
	ENDSAVE
};
void TCondition::GetLocalResult(int x,int y,int Height,int Angle,TexResult& result){
	result.TreesGrp=-1;
	result.TreesDensity=0;
	result.TreesDist=0;
	int WT=0;
	if(Height>StartHeight&&Height<FinHeight)WT=255;
	else{
		int dh=Height<StartHeight?StartHeight-Height:Height-FinHeight;
		if(MaxHeightDiff<1)MaxHeightDiff=1;
		WT=255*(MaxHeightDiff-dh)/MaxHeightDiff;
		if(WT<0)WT=0;
	}
	int WA=0;
	if(Angle>StartAngle&&Angle<FinAngle)WA=255;
	else{
		int dh=Angle<StartAngle?StartAngle-Angle:Angle-FinAngle;
		if(MaxAngleDiff<1)MaxAngleDiff=1;
		WA=255*(MaxAngleDiff-dh)/MaxAngleDiff;
		if(WA<0)WA=0;
	}
	int W0=WA*WT/255;
	float fw=WeightVariation.GetFValue(x,y);
	W0=int(float(W0)*WholeWeight*fw);
	int NT=(Textures.GetAmountOfElements()-1)>>1;
	if(NT>1){
		int TW=TexturesMixing.GetFValue(x,y)*256*(NT-1);
		int T0=TW/256;
		int T1=T0+1;	
		int W=255-(TW%256);
		if(W<128){
			swap(T0,T1);
			W=255-W;
		}
		result.t1=Textures.TextureSet[T0];
		result.t2=Textures.TextureSet[T1];
		result.f1=Textures.FactureSet[T0];
		result.f2=Textures.FactureSet[T1];
		result.w1=W0*W/255;
		result.w2=W0*(255-W)/255;
	}else{
		if(NT==1){
            result.t1=Textures.TextureSet[0];
			result.t2=Textures.TextureSet[0];
			result.f1=Textures.FactureSet[0];
			result.f2=Textures.FactureSet[0];
			result.w1=W0;
			result.w2=0;
		}else{
			result.w1=0;
			result.w2=0;
			result.t1=0;
			result.t2=0;
			result.f1=0;
			result.f2=0;
			result.TreesDensity=0;
			result.TreesGrp=-1;
		}
	}
	result.fWeight1=FacturesWeight*FactWeightVariaton.GetFValue(x,y)*2.0f;
	result.fWeight2=result.fWeight1;
	result.LockMask=int(Locking.Lock0)+(int(Locking.Lock1)<<1)+(int(Locking.Lock2)<<2)+(int(Locking.Lock3)<<3);
	if(Trees.Trees[0].Enabled){
		float Weights[MaxTrees];
		int idxs[MaxTrees];
		int nw=0;
		float sw=0;
		for(int i=0;i<MaxTrees;i++)if(Trees.Trees[i].Enabled){
            Weights[nw]=Trees.Trees[i].WholeDensity*Trees.Trees[0].Density.GetFValue(x,y);
			float r=Trees.Trees[i].MinDistance;
			idxs[nw]=i;
			sw+=Weights[nw]*r;
			nw++;
		}
        float p=sw*float(mrand())/32767.0;
		sw=0;
		int pp=0;
		int bidx=0;
		for(int i=0;i<MaxTrees;i++)if(Trees.Trees[i].Enabled){
			float r=Trees.Trees[i].MinDistance;
            sw+=Weights[pp]*r;
			if(p<=sw){
                bidx=pp;
				break;
			}
			pp++;
		}
		result.TreesGrp=Trees.Trees[idxs[bidx]].Trees;
		result.TreesDensity=fw*Weights[bidx];
		result.TreesDist=Trees.Trees[idxs[bidx]].MinDistance;
		//assert(result.t1!=0);
	}
}
int GetAngleInPoint(int x,int y){
	int DHX=GetHeight(x+64,y)-GetHeight(x-64,y);
	int DHY=GetHeight(x,y+64)-GetHeight(x,y-64);
	int DHZ=128;
	int N=sqrt(DHX*DHX+DHY*DHY);
	return int(atan2f(N,DHZ)*180.0f/3.1415f);
}
bool GetGradPoint(int &x,int &y,int r){
	int HL=GetHeight(x-32,y);
	int HR=GetHeight(x+32,y);
	int HU=GetHeight(x,y-32);
	int HD=GetHeight(x,y+32);
	int dx=HR-HL;
	int dy=HD-HU;
	if(dx||dy){
        int N=Norma(dx,dy);
		dx=dx*r/N;
		dy=dy*r/N;
		x-=dx;
		y-=dy;
	}else return false;
}
bool GetIsoPoint(int &x,int &y,int r){
	int HL=GetHeight(x-32,y);
	int HR=GetHeight(x+32,y);
	int HU=GetHeight(x,y-32);
	int HD=GetHeight(x,y+32);
	int dx=HR-HL;
	int dy=HD-HU;
	if(dx||dy){
		int N=Norma(dx,dy);
		dx=dx*r/N;
		dy=dy*r/N;
		x=dy;
		y=-dx;
	}else return false;
}
bool TCondition::GetResult(int x,int y,int Height,int Angle,TexResult& result){
	static int dxy[12]={-2,-1,0,-2,2,-1,2,1,0,2,-2,1};
	if(CheckDistance==0){
        GetLocalResult(x,y,Height,Angle,result);
	}else{
		int RR=CheckDistance>>1;
		if(DistanceCheckType==0){//gradient
			int xb=x;
			int yb=y;
			int minr=100000;
			for(int i=0;i<6;i++){
				int x1=x+dxy[i+i  ]*RR;
				int y1=y+dxy[i+i+1]*RR;
				int x10=x1;
				int y10=y1;
				if(GetGradPoint(x1,y1,RR*2)){
					int r=Norma(x1-x,y1-y);
					if(r<minr){
                        minr=r;
						xb=x10;
						yb=y10;
					}
				}
			}
			GetLocalResult(xb,yb,GetHeight(xb,yb),GetAngleInPoint(xb,yb),result);
		}else
		if(DistanceCheckType==1){//radius
			TexResult best;
			int maxw=0;
			for(int i=0;i<6;i++){
				int x1=x+dxy[i+i  ]*RR;
				int y1=y+dxy[i+i+1]*RR;
				TexResult tr;
				GetLocalResult(x1,y1,GetHeight(x1,y1),GetAngleInPoint(x1,y1),tr);
				if(tr.w1+tr.w2>=maxw){
					best=tr;
					maxw=tr.w1+tr.w2;
				}
			}
			result=best;
		}else
		if(DistanceCheckType==0){//isoline
			int xb=x;
			int yb=y;
			int xbi=x;
			int ybi=y;
			int minr=100000;
			for(int i=0;i<6;i++){
				int x1=x+dxy[i+i  ]*RR;
				int y1=y+dxy[i+i+1]*RR;
				int x10=x1;
				int y10=y1;
				if(GetIsoPoint(x1,y1,RR*2)){
					int r=Norma(x10+x1-x,y10+y1-y);
					if(r<minr){
						minr=r;
						xb=x10;
						yb=y10;
						int x1=x-dxy[i+i  ]*RR;
						int y1=y-dxy[i+i+1]*RR;
					}
				}
			}
			TexResult tr1;
			TexResult tr2;
			GetLocalResult(xb,yb,GetHeight(xb,yb),GetAngleInPoint(xb,yb),tr1);
			GetLocalResult(xbi,ybi,GetHeight(xbi,ybi),GetAngleInPoint(xbi,ybi),tr2);
			if(tr1.w1+tr1.w2>tr2.w1+tr2.w2){
                result=tr1;
			}else{
				result=tr2;
			}
		}
		if(ExcludeCentralPoint){
			TexResult tr;
			GetLocalResult(x,y,Height,Angle,tr);
			int ww1=tr.w1+tr.w2;
			int ww2=result.w1+result.w2;
			int wmax=max(ww1,ww2);
			if(wmax){
				result.w1=(wmax-ww1)*result.w1/wmax;
				result.w2=(wmax-ww1)*result.w2/wmax;
			}
		}
	}
	if(Mask.Enabled){
		int ms=240*32<<ADDSH;
		float xx=float(x)/ms;
		float yy=float(y)/ms;
		BasePaintMap* BPM=MLayers->Maps[Mask.Map.TopLayerIndex];
        float M=BPM->GetGreyPrecise(xx,yy);
		result.w1=result.w1*M/256;
		result.w2=result.w2*M/256;
	}
	return Smoothing;
}

bool LandsScope::GetResult(int x,int y,int Height,int Angle,TexResult& result){
	int Weights [64];
	int Factures[64];
	byte Smooth[64];
	byte Trees[64];
	float TWeights[64];
	float MinDistance[64];
	float FW[64];
	byte  LockMask[64];
	memset(Weights ,0,sizeof Weights );
	int WS=0;
	int WMAX=0;
	int WMAXNEXT=0;
	int WEIGHTS[MaxTerr];
	float x0=float(3840<<ADDSH);
	float y0=float(3840<<ADDSH);
	for(int j=0;j<MaxTerr;j++)if(Terr[j].Enabled){
		float _fx[]={0,0,-1,1, 0,0,-1, 1,-1,1};
		float _fy[]={0,0, 0,0,-1,1,-1,-1, 1,1};
		float _fc[]={0,2, 0,0, 0,0, 0, 0, 0,0};
		float fx=_fx[Terr[j].WeightPreferring];
		float fy=_fy[Terr[j].WeightPreferring];
		float fc=_fc[Terr[j].WeightPreferring];
		float w0=Terr[j].GlobalWeight;
		float w=Terr[j].WeightVariation.GetFValue(x,y);
        w=256*w0*(w+(x-x0)*fx/x0+(y-y0)*fy/y0+(1-sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0))/x0)*fc);
		if(Terr[j].Mask.Enabled){
			int ms=240*64<<ADDSH;
			float xx=float(x)/ms;
			float yy=float(x)/ms;
			BasePaintMap* BPM=MLayers->Maps[Terr[j].Mask.Map.TopLayerIndex];
			float M=BPM->GetGreyPrecise(xx,yy);
			w*=M/256;
		}
		WEIGHTS[j]=int(w);
		if(WEIGHTS[j]<1)WEIGHTS[j]=1;
		WS+=WEIGHTS[j];
		if(WEIGHTS[j]>WMAX){
			WMAXNEXT=WMAX;
			WMAX=WEIGHTS[j];
		}else{
			if(WEIGHTS[j]>WMAXNEXT){
				WMAXNEXT=WEIGHTS[j];
			}
		}
	};
	for(int j=0;j<MaxTerr;j++)if(Terr[j].Enabled){
		int W0=WEIGHTS[j];
		int W=W0*W0*W0/WS;
		if(W<4)W=4;
		if(abs(W0-WMAX)<=WMAX/100){
			TerrParams* TP=Terr+j;
			for(int i=0;i<MaxCond;i++)if(TP->Condition[i].Enabled){
				TexResult tr;
				bool smooth=TP->Condition[i].GetResult(x,y,Height,Angle,tr);
				tr.w1=(tr.w1*W)>>8;
				tr.w2=(tr.w2*W)>>8;
				if(tr.t1>=0&&tr.t1<255&&tr.w1>0){
					if(tr.w1>Weights[tr.t1]){
						Factures[tr.t1]=tr.f1;
						if(WEIGHTS[j]==WMAX){
							Smooth[tr.t1]=smooth;
						}
						Trees[tr.t1]=tr.TreesGrp;
						TWeights[tr.t1]=short(tr.TreesDensity*256.0f);
						MinDistance[tr.t1]=tr.TreesDist;
						FW[tr.t1]=tr.fWeight1;
						LockMask[tr.t1]=tr.LockMask;
					}
					Weights[tr.t1]+=tr.w1;
					if(tr.t2>0&&tr.t2<256){
						if(tr.w2>Weights[tr.t2]){
							Factures[tr.t2]=tr.f2;
							FW[tr.t2]=tr.fWeight2;
						}
					}
				}
			}
		}
	}
	bool sm=0;
	int maxw=0;
	int btex=0;
	int bfac=0;
	for(int i=0;i<64;i++){
		if(Weights[i]>maxw){
			maxw=Weights[i];
			btex=i;
			bfac=Factures[i];
		}
	}
	if(maxw){
		Weights[btex]=0;
		sm=Smooth[btex];
	}
	result.t1=btex;
	result.f1=bfac;
	result.w1=maxw;
	result.TreesGrp=Trees[btex];
	result.TreesDensity=TWeights[btex];
	result.TreesDist=MinDistance[btex];
	result.fWeight1=FW[btex];
	result.fWeight2=FW[btex];
	result.LockMask=LockMask[btex];
	maxw=0;
	for(int i=0;i<64;i++){
		if(Weights[i]>maxw){
			maxw=Weights[i];
			btex=i;
			bfac=Factures[i];
		}
	}
	if(maxw){
		Weights[btex]=0;
	}
	result.t2=btex;
	result.f2=bfac;
	result.w2=maxw;	
	return sm||(WMAXNEXT>WMAX*3/4);
}
extern int RealLx;
extern int RealLy;

void SetCurrentProgress(char* Message,int Stage,int maxStage);
bool MMItemChoose(SimpleDialog* SD);
//CEXPORT int ItemChoose;
bool ProcessLoadingFile(char* Mask,char* DestName,int Header);
bool ProcessSavingFile(char* Mask,char* DestName,int Header,bool clear);

void SaveMLayersToFileInCompactForm(char* filename);
bool LoadMLayersFromFileInCompactForm(char* filename);

void PaintMapWith(){
	
}
//////////////////////////////////////////////////////////////////////////
void DrawColoredRect(int x,int y,int Lx,int Ly,DWORD Diffuse,int Fade);
__declspec(dllexport)
void SetCurrentProgress(char* Message,int Stage,int maxStage){
	if(!bActive)return;
	bool CheckIfProgressActive();
	if(CheckIfProgressActive()){
		ShowLoadProgress(Message,Stage,maxStage);
        return;
	}
	int T=GetTickCount();
	static int t0=T-1000;
	if(T-t0>300){
		char s[512];
		sprintf(s,"%s : %d%%",Message,maxStage?Stage*100/maxStage:100);
		int L=GetRLCStrWidth(s,&BigWhiteFont)/2;
		int L1=200;
		int x0=RealLx/2;
		int y0=RealLy/2;
		GPS.DrawFillRect(x0-L1,y0,L1*2,60,0xFF000000);
		GPS.FlushBatches();
		DrawColoredRect(x0-L1,y0,L1*2,60,0xFFFF8000,95);
		ShowString(x0-L,y0+8,s,&BigWhiteFont);
		void GSYSDRAW();
		GSYSDRAW();
		T=t0;
	}
}
extern int RealLx;
extern int RealLy;
#define PROP_LX 350
#define PROP_DY 60
#define PROP_DY1 300
#define PROP_DX 10
extern bool LockManualScroll;
void ProcessPropertyEditor(BaseClass* BC){
	static int InPropEd=0;
	if(InPropEd)return;
	LockManualScroll=1;
	InPropEd=1;
	DialogsSystem DSS;
	ClassEditor CE;
	CE.CreateFromClass(&DSS,RealLx-PROP_LX-PROP_DX,PROP_DY,RealLx-PROP_DX,RealLy-PROP_DY1,BC,3);
	extern int LastKey;
	void ProcessScreen();
	LastKey=0;
	extern bool OptHidden;
	extern int Inform;
	bool OH=OptHidden;
	do{
		OptHidden=1;
		Inform=0;
		ProcessScreen();
		ProcessMessages();
		DSS.ProcessDialogs();
		CE.Process();		
		DSS.RefreshView();
		if(LastKey==27||LastKey==13)break;
	}while(true);
	OptHidden=OH;
	InPropEd=0;
	LockManualScroll=0;
}
void Convert(ExLandsScope* TP){
	mkdir("XmlMaps");
	LandsScope* LS=new LandsScope;
	LS->SafeReadFromFile("Landscape.xml");
	TP->reset_class(TP);	
	
	TP->PostProcessType=LS->PostProcessType;
	TP->PostStagesAmount=LS->PostStagesAmount;
	TP->GenerateTextures=LS->GenerateTextures;
	TP->GenerateRelief=LS->GenerateRelief;

	TP->AllMaps.PenRadius=LS->AllMaps.PenRadius;
	TP->AllMaps.PenWidth=LS->AllMaps.PenWidth;
	TP->AllMaps.ActionType=LS->AllMaps.ActionType;
	TP->AllMaps.TopLayerIndex=LS->AllMaps.TopLayerIndex;
	
	TP->Relief.StartHeight=LS->Relief.StartHeight;

	TP->Relief.GlobalModulation.Scale=LS->Relief.GlobalModulation.Scale;
	TP->Relief.GlobalModulation.Center=LS->Relief.GlobalModulation.Center;
	TP->Relief.GlobalModulation.Contrast=LS->Relief.GlobalModulation.Contrast;
	TP->Relief.GlobalModulation.Mapping=LS->Relief.GlobalModulation.Mapping;
	
	int NR=LS->Relief.GetAmountOfElements()-NReliefGeneratorMembers-1;
	for(int i=0;i<NR;i++){
		ReliefRule* RRL=new ReliefRule;
		TP->Relief.Rules.Add(RRL);

		RRL->Description=LS->Relief.Rules[i].Description;
		RRL->Enabled=LS->Relief.Rules[i].Enabled;
		RRL->AverageHeight=LS->Relief.Rules[i].AverageHeight;
		RRL->StartHeight=LS->Relief.Rules[i].StartHeight;
		RRL->HeightOperation=LS->Relief.Rules[i].HeightOperation;
		RRL->SmoothSteps=LS->Relief.Rules[i].SmoothSteps;
		RRL->SmoothMaxVariation=LS->Relief.Rules[i].SmoothMaxVariation;

		RRL->SmoothVariation.Scale=LS->Relief.Rules[i].SmoothVariation.Scale;
		RRL->SmoothVariation.Center=LS->Relief.Rules[i].SmoothVariation.Center;
		RRL->SmoothVariation.Contrast=LS->Relief.Rules[i].SmoothVariation.Contrast;
		RRL->SmoothVariation.Mapping=LS->Relief.Rules[i].SmoothVariation.Mapping;

		RRL->SmoothType=LS->Relief.Rules[i].SmoothType;
		RRL->CutOffTop=LS->Relief.Rules[i].CutOffTop;

		RRL->Layer.PenRadius=LS->Relief.Rules[i].Layer.PenRadius;
		RRL->Layer.PenWidth=LS->Relief.Rules[i].Layer.PenWidth;
		RRL->Layer.ActionType=LS->Relief.Rules[i].Layer.ActionType;
		RRL->Layer.TopLayerIndex=LS->Relief.Rules[i].Layer.TopLayerIndex;

		RRL->EdgesShape.Scale=LS->Relief.Rules[i].EdgesShape.Scale;
		RRL->EdgesShape.Center=LS->Relief.Rules[i].EdgesShape.Center;
		RRL->EdgesShape.Contrast=LS->Relief.Rules[i].EdgesShape.Contrast;
		RRL->EdgesShape.Mapping=LS->Relief.Rules[i].EdgesShape.Mapping;

		RRL->TopShape.Scale=LS->Relief.Rules[i].TopShape.Scale;
		RRL->TopShape.Center=LS->Relief.Rules[i].TopShape.Center;
		RRL->TopShape.Contrast=LS->Relief.Rules[i].TopShape.Contrast;
		RRL->TopShape.Mapping=LS->Relief.Rules[i].TopShape.Mapping;

		RRL->TopModulation.Scale=LS->Relief.Rules[i].TopModulation.Scale;
		RRL->TopModulation.Center=LS->Relief.Rules[i].TopModulation.Center;
		RRL->TopModulation.Contrast=LS->Relief.Rules[i].TopModulation.Contrast;
		RRL->TopModulation.Mapping=LS->Relief.Rules[i].TopModulation.Mapping;

		RRL->PaintWater=LS->Relief.Rules[i].PaintWater;
		RRL->WaterColor1=LS->Relief.Rules[i].WaterColor1;
		RRL->WaterColor2=LS->Relief.Rules[i].WaterColor2;

		RRL->WaterMixing.Scale=LS->Relief.Rules[i].WaterMixing.Scale;
		RRL->WaterMixing.Center=LS->Relief.Rules[i].WaterMixing.Center;
		RRL->WaterMixing.Contrast=LS->Relief.Rules[i].WaterMixing.Contrast;
		RRL->WaterMixing.Mapping=LS->Relief.Rules[i].WaterMixing.Mapping;
	}
	int NT=LS->GetAmountOfElements()-NLandsScopeMembers-1;
	if(NT>0){
		int count=0;
		for(int i=0;i<NT;i++){
			TP->Terr.Add(new ExTerrain);
			TP->Terr[count]->Description=LS->Terr[i].Description;
			TP->Terr[count]->Enabled=LS->Terr[i].Enabled;
			TP->Terr[count]->GlobalWeight=LS->Terr[i].GlobalWeight;

			TP->Terr[count]->WeightVariation.Scale=LS->Terr[i].WeightVariation.Scale;
			TP->Terr[count]->WeightVariation.Center=LS->Terr[i].WeightVariation.Center;
			TP->Terr[count]->WeightVariation.Contrast=LS->Terr[i].WeightVariation.Contrast;
			TP->Terr[count]->WeightVariation.Mapping=LS->Terr[i].WeightVariation.Mapping;

			TP->Terr[count]->WeightPreferring=LS->Terr[i].WeightPreferring;

			TP->Terr[count]->Mask.Enabled=LS->Terr[i].Mask.Enabled;
			TP->Terr[count]->Mask.Map.PenRadius=LS->Terr[i].Mask.Map.PenRadius;
			TP->Terr[count]->Mask.Map.PenWidth=LS->Terr[i].Mask.Map.PenWidth;
			TP->Terr[count]->Mask.Map.ActionType=LS->Terr[i].Mask.Map.ActionType;
			TP->Terr[count]->Mask.Map.TopLayerIndex=LS->Terr[i].Mask.Map.TopLayerIndex;
			int NC=LS->Terr[i].GetAmountOfElements()-NTerrParamsMembers-1;
			if(NC>0){
				for(int ii=0;ii<NC;ii++){
					ObjectRules* ORL=new ObjectRules;
					TP->Terr[count]->Rules.Add(ORL);

					ORL->Description=LS->Terr[i].Condition[ii].Description;
					ORL->Enabled=LS->Terr[i].Condition[ii].Enabled;
					ORL->Action.Smoothing=LS->Terr[i].Condition[ii].Smoothing;
					ORL->Condition.StartHeight=LS->Terr[i].Condition[ii].StartHeight;
					ORL->Condition.FinHeight=LS->Terr[i].Condition[ii].FinHeight;
					ORL->Condition.MaxHeightDiff=LS->Terr[i].Condition[ii].MaxHeightDiff;
					ORL->Condition.StartAngle=LS->Terr[i].Condition[ii].StartAngle;
					ORL->Condition.FinAngle=LS->Terr[i].Condition[ii].FinAngle;
					ORL->Condition.MaxAngleDiff=LS->Terr[i].Condition[ii].MaxAngleDiff;
					ORL->Condition.CheckDistance=LS->Terr[i].Condition[ii].CheckDistance;
					ORL->Condition.DistanceCheckType=LS->Terr[i].Condition[ii].DistanceCheckType;
					ORL->Condition.ExcludeCentralPoint=LS->Terr[i].Condition[ii].ExcludeCentralPoint;

					ORL->Action.Mask.Enabled=LS->Terr[i].Condition[ii].Mask.Enabled;
					ORL->Action.Mask.Map.PenRadius=LS->Terr[i].Condition[ii].Mask.Map.PenRadius;
					ORL->Action.Mask.Map.PenWidth=LS->Terr[i].Condition[ii].Mask.Map.PenWidth;
					ORL->Action.Mask.Map.ActionType=LS->Terr[i].Condition[ii].Mask.Map.ActionType;
					ORL->Action.Mask.Map.TopLayerIndex=LS->Terr[i].Condition[ii].Mask.Map.TopLayerIndex;

					ORL->Action.WholeWeight=LS->Terr[i].Condition[ii].WholeWeight;

					ORL->Action.WeightVariation.Scale=LS->Terr[i].Condition[ii].WeightVariation.Scale;
					ORL->Action.WeightVariation.Center=LS->Terr[i].Condition[ii].WeightVariation.Center;
					ORL->Action.WeightVariation.Contrast=LS->Terr[i].Condition[ii].WeightVariation.Contrast;
					ORL->Action.WeightVariation.Mapping=LS->Terr[i].Condition[ii].WeightVariation.Mapping;

					ORL->Action.TexturesMixing.Scale=LS->Terr[i].Condition[ii].TexturesMixing.Scale;
					ORL->Action.TexturesMixing.Center=LS->Terr[i].Condition[ii].TexturesMixing.Center;
					ORL->Action.TexturesMixing.Contrast=LS->Terr[i].Condition[ii].TexturesMixing.Contrast;
					ORL->Action.TexturesMixing.Mapping=LS->Terr[i].Condition[ii].TexturesMixing.Mapping;

					ORL->Action.FacturesWeight=LS->Terr[i].Condition[ii].FacturesWeight;
					
					ORL->Action.FactWeightVariaton.Scale=LS->Terr[i].Condition[ii].FactWeightVariaton.Scale;
					ORL->Action.FactWeightVariaton.Center=LS->Terr[i].Condition[ii].FactWeightVariaton.Center;
					ORL->Action.FactWeightVariaton.Contrast=LS->Terr[i].Condition[ii].FactWeightVariaton.Contrast;
					ORL->Action.FactWeightVariaton.Mapping=LS->Terr[i].Condition[ii].FactWeightVariaton.Mapping;

					int tex_count=LS->Terr[i].Condition[ii].Textures.GetAmountOfElements()/2;
					if(tex_count>0){
						int count2=0;
						for(int iii=0;iii<tex_count;iii++){
							ORL->Action.Textures.Add(new TexFact);
							ORL->Action.Textures[count2]->Texture=LS->Terr[i].Condition[ii].Textures.TextureSet[iii];
							ORL->Action.Textures[count2]->Facture=LS->Terr[i].Condition[ii].Textures.FactureSet[iii];
							count2++;
						}
					}

					ORL->Action.Locking.Lock0=LS->Terr[i].Condition[ii].Locking.Lock0;
					ORL->Action.Locking.Lock2=LS->Terr[i].Condition[ii].Locking.Lock2;

					int tree_count=LS->Terr[i].Condition[ii].Trees.GetAmountOfElements()-1;
					if(tree_count>0){
						int count2=0;
						for(int iii=0;iii<tree_count;iii++){
							if(LS->Terr[i].Condition[ii].Trees.Trees[iii].Enabled){
								ORL->Action.Trees.Add(new Tree);
								ORL->Action.Trees[count2]->Trees=LS->Terr[i].Condition[ii].Trees.Trees[iii].Trees;
								ORL->Action.Trees[count2]->MinDistance=LS->Terr[i].Condition[ii].Trees.Trees[iii].MinDistance;
								ORL->Action.Trees[count2]->WholeDensity=LS->Terr[i].Condition[ii].Trees.Trees[iii].WholeDensity;

								ORL->Action.Trees[count2]->Density.Scale=LS->Terr[i].Condition[ii].Trees.Trees[iii].Density.Scale;
								ORL->Action.Trees[count2]->Density.Center=LS->Terr[i].Condition[ii].Trees.Trees[iii].Density.Center;
								ORL->Action.Trees[count2]->Density.Contrast=LS->Terr[i].Condition[ii].Trees.Trees[iii].Density.Contrast;
								ORL->Action.Trees[count2]->Density.Mapping=LS->Terr[i].Condition[ii].Trees.Trees[iii].Density.Mapping;
								count2++;
							}
						}
					}
				}
			}
			count++;
		}
	}
	delete(LS);
}
