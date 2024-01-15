#define QuadSize 256
#define GrassQuantSize 32
_inline DWORD DW(DWORD x,DWORD y){
	return x+(y<<16);
}
class OneQuad{
public:
    StaticMesh	bm;
	int			QuadX;
	int			QuadY;
	int			LastRanderFrame;
	AABoundBox	AB;
};
class QuadsRenderer{
public:
	static DynArray<QuadsRenderer*> QuadsGenerators;

	QuadsRenderer();
	~QuadsRenderer();
	static uni_hash<OneQuad*>	QuadList;
	static uni_hash<BOOL>		IsReady;    
    
	static void SetDirtyRect(Rct rect);
	static void KillQuad(int qx,int qy);
	static void ReCreateQuad(int qx,int qy);
    static void Render();	
	virtual void FillQuad(Rct rct,DynArray<BaseMesh*>& bm);

};
struct GrassPoint{
	GrassPoint(){};
	GrassPoint(BYTE H,BYTE T){
		Height=H;
		Type=T;
	}
	GrassPoint(BYTE H);
	BYTE Height;
	BYTE Type;
};
class OneGrassType:public BaseClass{
public:
	Delimiter D;
	DWORD ColorModulator;
	DWORD ColorModulatorForSmallSize;
	int TextureID;
	int ShadowTextureID;
	float Scale;
	float Frequency;
	void Refresh();

	__constructor(OneGrassType);

	SAVE(OneGrassType){
        REG_AUTO(D);
		REG_MEMBER(_color,ColorModulator);
		REG_MEMBER(_color,ColorModulatorForSmallSize);
		REG_MEMBER(_TextureID,TextureID);
		REG_MEMBER(_TextureID,ShadowTextureID);
		REG_MEMBER(_float,Scale);
		REG_FSLIDER(Frequency,0,1,1);
		REG_METHOD(&OneGrassType::Refresh);
	}ENDSAVE;

	bool ForceSimplification(){
		return true;
	}
};
class GrassRenderer:public QuadsRenderer{
public:
	static ClonesArray<OneGrassType> GrassProps;
	static uni_hash<GrassPoint>		GrassHeight;
	static byte CurrentGrassType;

	
	static void SetGrassHeight(int x,int y,int H);
	static int  GetGrassHeight(int x,int y);
	static int  GetInterpGrassHeight(int x,int y);	
	static Vector3D GetInterpGrassGradient(int x,int y);
	static void SetGrassRound(int x,int y,int R,int D,int H);
	static void ClearGrassRound(int x,int y,int R,int D,int H);
	virtual void FillQuad(Rct rct,DynArray<BaseMesh*>& bm);
	static void SaveGrass(ResFile F);
	static void LoadGrass(ResFile F);
	static void ClearGrass();
};
