#include "..\COSSACKS2\comms\comms.h"
struct branch_point{
    float T;
    Vector3D Pos;
    float Radius;
};
class ParametricBranch{
public:	
    DynArray<branch_point> Line;
    float R;
    float L;  
    int NS;
    int NP;

    int CurrentLOD;
    int CurrentLevel;

    virtual Vector3D GetPoint(float T,float Length){
        return Vector3D(0,0,T*Length);
    }
    virtual float GetRadiusScale(float T,float Angle,float Length){
        return 1-T;
    }
    virtual void Init(float Length){};
    virtual Vector3D GetInterpPoint(float T);
	float GetDistToTube(Vector3D P,float& T);
	bool CheckIfPointInsideTube(Vector3D V);
    void Clear();
    void CreateLine(float AngleThreshold,int StartSubDiv,float Lenght,float Radius,int NSegments);
    int  GetRequiredVerts();
    int  GetRequiredIdxs();
    void CreateGeometry(BaseMesh& DestBM,Matrix4D& Transform,int NSegments,int NTexCycles,int TextureID);
};

class ParametricBranchWithStartAngle:public ParametricBranch{
public:
	float BlendLength;
	Vector3D StartDirection;
	Matrix4D Transform;
	float Gravity;

    virtual void Init(float Length){};    

	virtual Vector3D GetPoint(float T,float Length){
		Vector3D spos(Transform.e30,Transform.e31,Transform.e32);
        if(T<0)T=0;
		//if(T==0)return spos;
		float x=T*Length/(BlendLength+0.01);
		float w2=1-exp(-x);
		float w1=(x-w2)*BlendLength/Length;
		Vector3D F=GetOrgPoint(T,Length);
		Transform.transformVec(F);
		F*=w2*w2;
		//F+=spos;
		F+=StartDirection*w2*BlendLength;
        F+=spos;
		//gravity applying using approximate hack
		Vector3D vz=Vector3D::oZ;
		Transform.transformVec(vz);
		Vector3D vz1=vz;
		vz1.z+=Gravity;
		vz1.normalize();
		vz1-=vz;
		vz1*=Length*T*T;
		F+=vz1;
		return F;
	}
	virtual Vector3D GetOrgPoint(float T,float Length){
		//return Vector3D(cos(T*5)*Length/4,cos(T*7)*Length/4,T*Length);
		float GetSimpleNoise1D(float T);
		float GetFractalNoise1D(float T,float FractalSharpness,int Seed);
		return Vector3D(GetFractalNoise1D(T*5,0.3,123)*Length/4,0,T*Length);//Vector3D(cos(T*5)*Length/8,cos(T*7)*Length/8,T*Length);
	}
};

//multiple branches generator
class OneTreeBranch:public BaseClass,public ParametricBranchWithStartAngle{
public:
    cSplineTCB3 Spline;
	ClonesArray<OneTreeBranch> SubBranches;
	SubSection Parameters;    

	//-------------------//
	Delimiter D;
	bool    Enabled;	
	//-------------------//
	//float   MinLength;
	//float   MaxLength;
    int     Sides;
    int     BarkTexture;
	float	NoiseDegree;
	float	NoiseSharpness;
    float   NoiseFrequency;    
	int	    ShapeSeed;
	//-------------------//
	float	GravityFactor;
	float	StartAngleToParent;
	float	FinalangleToParent;
	bool	InversedAlignmentToParent;
	int		AngleSeed;
	//-------------------//    
	float	MinProportion;
	float	MaxProportion;
    float   RelativeRootLength;
	float	BranchToRootRelativeSize;
	float	RootRadiusRelativeToParent;
	int		ProportionSeed;
	//-------------------//
	float   DividingStartPosition;
	float   DividingEndPosition;
	int		MinBranchesOnEachDivision;
	int		MaxBranchesOnEachDivision;
	int		MinDivisions;
	int		MaxDivisions;
	int		DividingSeed;
    //-------------------//
    float   CurvingThresholdForSimplification;
    float   RootCurvatureAmplitude;
    float   RootCurvatureFrequency;
    int     RootCurvatureSeed;
	//-------------------//
	//Billboards generating
	bool	EnableBillboards;
	bool	DisableBranchGeometry;
	float	MinDistanceToExistingBillboards;
	int		BillboardTextureID;
	int		NPictPerBillboardX;
	int		NPictPerBillboardY;
    DWORD   BillboardColor;
	float	BillboardMinSize;
	float	BillboardMaxSize;
	float	BillboardScale;
	int		BillboardSeed; 

    void Randomize();
	SAVE(OneTreeBranch){
		REG_MEMBER(_bool,Enabled);
		//-------------------//
		REG_AUTO(SubBranches);
		//REG_AUTO(Parameters);

		REG_ISLIDER(Sides,2,16,8);
        REG_MEMBER(_TextureID,BarkTexture);
        REG_FSLIDER(CurvingThresholdForSimplification,0,15.0f,5.0f);
		REG_FSLIDER(NoiseDegree,0,4,0.5);
		REG_FSLIDER(NoiseSharpness,0,1,0.5);
        REG_FSLIDER(NoiseFrequency,0,10,2);
		//REG_MEMBER(_int,ShapeSeed);
        REG_ISLIDER(ShapeSeed,0,32767,16384);
        //-------------------//
		REG_AUTO(D);
        REG_FSLIDER(RootCurvatureAmplitude,0,1,0);
        REG_FSLIDER(RootCurvatureFrequency,0,10,1);
        REG_ISLIDER(RootCurvatureSeed,0,32767,16384);
		//-------------------//
		REG_AUTO(D);
		REG_FSLIDER(MinProportion,0,50,12);
		REG_FSLIDER(MaxProportion,0,50,18);
        REG_FSLIDER(RelativeRootLength,0,1,0.3);
		REG_FSLIDER(BranchToRootRelativeSize,0,1,0.6);
		REG_FSLIDER(RootRadiusRelativeToParent,0,1,1);
		//REG_MEMBER(_int,ProportionSeed);
        REG_ISLIDER(ProportionSeed,0,32767,16384);
		//-------------------//
        REG_AUTO(D);
		REG_FSLIDER(GravityFactor,-1,1,0);
		REG_FSLIDER(StartAngleToParent,0,1,0.3);
		REG_FSLIDER(FinalangleToParent,0,1,0.3);
		REG_MEMBER(_bool,InversedAlignmentToParent);
		//REG_MEMBER(_int,AngleSeed);
        REG_ISLIDER(AngleSeed,0,32767,16384);
		//-------------------//
		REG_AUTO(D);
		REG_FSLIDER(DividingStartPosition,0,1,0.3);
		REG_FSLIDER(DividingEndPosition,0,1,0.9);
		REG_ISLIDER(MinBranchesOnEachDivision,1,10,1);
		REG_ISLIDER(MaxBranchesOnEachDivision,1,10,1);
		REG_ISLIDER(MinDivisions,0,10,3);
		REG_ISLIDER(MaxDivisions,0,10,4);
		//REG_MEMBER(_int,DividingSeed);
        REG_ISLIDER(DividingSeed,0,32767,16384);
		//--------------------//
		REG_AUTO(D);
		REG_MEMBER(_bool,EnableBillboards);
		SAVE_SECTION(2);
		REG_MEMBER(_bool,DisableBranchGeometry);
		REG_MEMBER(_float,MinDistanceToExistingBillboards);
		REG_MEMBER(_TextureID,BillboardTextureID);
		REG_MEMBER(_int,NPictPerBillboardX);
		REG_MEMBER(_int,NPictPerBillboardY);
        REG_MEMBER(_color,BillboardColor);
		REG_MEMBER(_float,BillboardMinSize);
		REG_MEMBER(_float,BillboardMaxSize);
		REG_FSLIDER(BillboardScale,0,10,1);
		REG_ISLIDER(BillboardSeed,0,32767,16384);        

	}ENDSAVE;		
	DWORD GetClassMask(){
		if(EnableBillboards)return 2;
		else return 1;
	}
	__constructor(OneTreeBranch){
		Enabled=true;	
		
		Sides=8;
        BarkTexture=IRS->GetTextureID("RedOakBark.tga");        
		NoiseDegree=0.8;
		NoiseSharpness=0.3;
        NoiseFrequency=5;
		ShapeSeed=rand();

        CurvingThresholdForSimplification=10;
        RootCurvatureAmplitude=0;
        RootCurvatureFrequency=3;
        RootCurvatureSeed=rand();
		
		GravityFactor=0.0;
		StartAngleToParent=0.2;
		FinalangleToParent=0.35;
		InversedAlignmentToParent=false;
		AngleSeed=rand();
		
		MinProportion=8;
		MaxProportion=10;
        RelativeRootLength=0.3;
		BranchToRootRelativeSize=0.6;
		RootRadiusRelativeToParent=0.9;
		ProportionSeed=rand();
		
		DividingStartPosition=0.4;
		DividingEndPosition=0.995;
		MinBranchesOnEachDivision=1;
		MaxBranchesOnEachDivision=2;
		MinDivisions=3;
		MaxDivisions=5;
		DividingSeed=rand();

		EnableBillboards=false;
		DisableBranchGeometry=false;
		MinDistanceToExistingBillboards=0;
		BillboardTextureID=IRS->GetTextureID("RedOakLeaves_RT_2.tga");
		NPictPerBillboardX=1;
		NPictPerBillboardY=1;
        BillboardColor=0x80808080;
		BillboardMinSize=10;
		BillboardMaxSize=20;
		BillboardScale=1;
		BillboardSeed=rand();
	}
	void GenerateMesh(BaseMesh&	bm,BaseMesh& bbm,Matrix4D& Transform,float RootRadius,OneTreeBranch* NextB);
    virtual void Init(float Length);
    virtual Vector3D GetOrgPoint(float T,float Length);
    virtual float GetRadiusScale(float T,float Angle,float Length);
};

class WholeTree:public BaseClass{
public:	
	static int GlobalSeed;
    int LOD;
    void SaveParametersToXML();
    void LoadParametersFromXML();
    void AutoSaveToC2M();
    void ReCreateTree();
    void GenerateRandom();
	ClonesArray<OneTreeBranch> Branches;
	//bm  - branches mesh
	//bbm - billboards mesh
	void GenerateMesh(BaseMesh&	bm,BaseMesh& bbm,float RootRadius);
	SAVE(WholeTree){        
		CreateDirectory("Trees",0);
		REG_LOADSAVE("Trees\\","*.tree.xml");		
        REG_METHOD(SaveParametersToXML);
        REG_METHOD(LoadParametersFromXML);
        REG_METHOD(AutoSaveToC2M);
        REG_METHOD(ReCreateTree);
        REG_METHOD(GenerateRandom);
        REG_MEMBER(_int,LOD);
		REG_AUTO(Branches);
	}ENDSAVE;	
};