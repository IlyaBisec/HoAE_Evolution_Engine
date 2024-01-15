#pragma once

struct VertexPos{
    Vector3D Pos;	
	float    W;   
    BYTE SelectionDegree;
	BYTE SubdivLevel;
    BYTE TempSelection;	
};
struct VertexUV{
    DWORD PosIndex;
    DWORD NIndex;        
    float u;
    float v;
    Vector3D T;
    Vector3D B;    
    Vector3D uv(){
        return Vector3D(u,v,0);
    }
};
struct SurfPoint3D{
    Vector3D  Pos;
    Vector3D  N;
    DWORD     Color;
    float     Freeze;
};
struct PointInfo{
    SurfPoint3D* pt;
    int         x;
    int         y;
    DWORD       Face;
};
struct FaceInfo{
	BYTE		SubdivLevel;
	BYTE		UVEdgeMask;//1-L 2-T 3-R 4-B 
	tri_DWORD	Pos;
    DWORD       UpFace;
    DWORD       DnFace;
    DWORD       LFace;
    DWORD       RFace;
    BYTE        Complexity;
    SurfPoint3D* Points; 
};
struct MeshInCell{
    StaticMesh Mesh;
    bool       IsValid;
    DynArray<DWORD> OrgVerts;
    MeshInCell();
    ~MeshInCell();
};
struct PickInfo{
    Vector3D Pos;
    float u;
    float v;
	DWORD FaceID;
    bool Picked;
};
struct PickedVertInfo{
	DWORD PosVertexID;
	DWORD UVVertexID;
	DWORD NVertexID;
	Vector3D Pos;
	Vector3D N;
    float DistToCenter;
};
struct TexPixel{
    int         U;
    int         V;
    Vector3D    ScrPos;
    Vector3D    WorldPos;
};
struct TexArray:public RasterOp{
    Vector3D v0;
    Vector3D v1;
    Vector3D v2;
    virtual void OnPoint(int x,int y,float z,float u,float v);
    DynArray<TexPixel> Texels;    
};
struct VertexBackup{
	Vector3D Pos;
	Vector3D N;
	Vector3D N0;
};
class PolyMesh;
class SurfaceModificator;
class TexturePattern;
class TexPointModificator{
public:
	TexPointModificator();
	DWORD Color;
	float Influence;
	virtual bool ModifyPoint(SurfaceModificator* sm,TexPixel* tp,DWORD* color,TexturePattern* TP);
};


class TexturePattern;

struct ModPointInfo;

typedef void fnModificator (float scale,ModPointInfo& pi,float R,float R1);

class SurfaceModificator{
public:		
	SurfaceModificator();
	static uni_hash<VertexBackup> Backup;	
    static float                    PrevR;
    static Vector3D					PrevPrevPos;
	static Vector3D					PrevPos;
	static Vector3D					CurrPos;
	static Vector3D					CenterPos;
	static PickInfo*				Pick;
    static TexArray*                TexPick;
	static PolyMesh*				M;	

	TexturePattern*				HeightPattern;
    TexturePattern*             ColorPattern;//with opacity if need    
    TexturePattern*             PecularPattern;

	TexturePattern*				TP;

    static void                 CreateVolumePick(AABoundBox AABB);

	static DynArray<PickedVertInfo>	NearVerts;
	static DynArray<DWORD>			InsideVerts;
	static DynArray<DWORD>			NearFaces;	

	static float ColorOpacity;
	static float SpecularOpacity;
	static float SpecularLevel;
	static int   MaskingByColorType;//0 - none, 1 - mask dark colors, 2 - mask bright colors 3 - mask by specific color
	static DWORD SpecificColor;
	static int   SpecularSource;//0 - uniform, 1 - special texture, 2 - texture G channel, 3 - texture 1-G channel, 4 - texture Alpha, 5 - texture 1-Alpha
	static int   HeightSource;//0 - uniform, 1 - special texture, 2 - texture G channel, 3 - texture 1-G channel, 4 - texture Alpha, 5 - texture 1-Alpha
	static DWORD DrawColor;
    
	static float Radius;
	static float Pressure;
	static float InfRadius;
	static float Degree;
	float GetDegree				(Vector3D pos,float DistToCenter,Vector4D& Color,float& ColorDegree,float& SpecularDegree);
	void  KillBackup();	
	void  Set(PolyMesh* M,float Radius);
	static float GetDistToRode(Vector3D p);
	static Vector3D GetDVToRode(Vector3D p,float& w);
    static Vector3D GetDVToRodeP(Vector3D p,float& w);
	void  RecreateNormals();
    void  CreateVertList();
    void  CreateTexPick();
	virtual void Apply(){};
	void  ApplyColor(TexPointModificator* tpm,bool ChangeColor,bool ChangeSpecular,TexturePattern* TP);
	virtual bool UpdateDuringStand(){return false;};

    static void GetTNB(Vector3D& T,Vector3D& N,Vector3D& B);
    static void ApplyModFunfcion(fnModificator* fn,bool Undo=true);
    static void ApplyToAll(fnModificator* fn,bool ReCreateNormals);
};
struct OldVertexInfo{
	Vector3D Pos;
	float DistToCenter;
	float Degree;
};
class DrawingModificator:public SurfaceModificator{
public:
	//float GetDegree(Vector3D pos,float DistToCenter);
	virtual void Apply();
};
class SmoothModificator:public SurfaceModificator{
public:
	virtual void Apply();
	virtual bool UpdateDuringStand(){return true;};
};
class AdditiveModificator:public SurfaceModificator{
public:
	virtual void Apply();
	//float GetDegree(Vector3D pos,float DistToCenter,Vector4D& Color,float& ColorDegree);
	virtual bool UpdateDuringStand(){return true;};
};
struct OneFaceBackup{
    SurfPoint3D* Points;
	Vector3D Verts[4];
	Vector3D Normals[4];
    DWORD Face;
    OneFaceBackup();
    ~OneFaceBackup();
};
class OneBackupItem{
public:
    PolyMesh* M;
    DynArray<OneFaceBackup> Backup;
    virtual void Undo();
    void Save(BinStream* bs);
    void Load(BinStream* bs);
    OneBackupItem();
    ~OneBackupItem();
};

#define tbsz 75731
typedef uni_hash<DWORD,DWORD,tbsz> FaceList;
struct ValidAABB:public AABoundBox{
    bool Valid;    
};
class PolyMesh{
public:
    PolyMesh();
    ~PolyMesh();

    int TextureID;
    int SubSurfaceSize;
    DynamicTexture                  Texture;
    DynamicTexture                  Normals;

	int								rtSizeX;
	int								rtSizeY;
	int								NormalsRT;
	int								ColorsRT;
    Vector3D                        StartShift;

    DynArray<VertexPos>             Verts;
    DynArray<Vector3D>              VertsN;
    DynArray<VertexUV>              VertsUV;    
	DynArray<FaceInfo>				FacesInfo;
    
    FaceList                				Faces;//list of NUV verts
    FaceList                				SrcFaces;//list of NUV verts
	uni_hash<DWORD>							DirtyFaces;    

    uni_hash<DWORD,DWORDS2,tbsz>            FacesOnEdges;
    uni_hash<DWORD,tri_DWORD,tbsz>			FacesTreeL0;//verts hash in cells of size <CellSize>    
	uni_hash<ValidAABB,tri_DWORD,tbsz>		FacesBB;
	uni_hash<tri_DWORD,DWORD,tbsz>			VertsInfluence;
	uni_hash<DWORD,tri_DWORD,tbsz>			VertsInCells;
    uni_hash<DWORD,tri_DWORD,tbsz>			FacesTreeL2;//verts hash in cells of size <CellSize*8>
	uni_hash<DWORD,tri_DWORD,tbsz>			ListL2;
    uni_hash<MeshInCell*,tri_DWORD>			Meshes;
    AABoundBox								AABB;
    float									CellSize;//approximately MeshLinearSize/32
    float									Scale;
    PickInfo								LastPickInfo;  
    DynArray<OneBackupItem*>                UndoHistory;
    uni_hash<OneFaceBackup*,DWORD>          FacesHistory;	
    


	void							CreateRenderTargets();
	void							InvalidatePosVertex(DWORD ID);
    DWORD                           AddVertexPos(Vector3D V);
    DWORD                           AddVertexN(Vector3D V);
    DWORD                           AddVertexUV(VertexUV* V);
    void                            AddFaceVertex(DWORD FaceIndex,DWORD VertexNUV_index);
    void                            FinalizeFace(DWORD FaceID);    
    void                            PutFacesInCells();
    void                            RemoveVertex(DWORD VertIndex);
    void                            RemoveFace(DWORD FaceIndex);    
    void                            DivideEdge(DWORD PosVertex1,DWORD PosVertex2);
    void                            DivideFace(DWORD FaceID);
	void							SubdivideQuadMesh(float AvgLength);
	int								GetFaceVerts		(DWORD FaceID,DynArray<DWORD>& Buf);	
    void                            Clear               ();    
    void                            CalcNormals         ();
    bool                            ReadObj             (const char* Name);
    void                            WriteObj            (const char* Name);
    void                            Subdivide           (PolyMesh* Dest);
    void                            CreateMeshes();
    void                            RefineMeshes();
    void                            CreateMeshOfOneCell (tri_DWORD& pos);
    void                            RefineMeshOfOneCell (MeshInCell* M);
    void                            Render();
    PickInfo                        Pick();
    TexArray*                       TexPick(PickInfo* P);
    char*                           printcoor(int v);
	void							SelfTest();
	int 							GetFEA();
    void                            CreateSubSurfaces(int size);
	void							ConvertCoor(DWORD Face,FaceInfo* f1,int& x,int& y);
    PointInfo                       GetPoint(DWORD Face,int x,int y);
    Vector3D                        GetAveragePN(DWORD Face,int x,int y,int Size,Vector3D& N);
	Vector3D                        GetAveragePNC(DWORD Face,int x,int y,int Size,Vector3D* N,Vector4D* C);
    void                            CalcNormal(PointInfo& p);
    void                            CopyFaceToTexture(DWORD Face);
    void                            CreateFaceNormals(DWORD Face);
	void							AddDirtyFace(DWORD FaceID);
	void							SetAllDirty();
	void							UpdateDirtyFaces(bool NormOnly=false);
	void							CreateUVEdgesInfo();
    //undo
    void                            DropFacesHistoryToUndo();
    void                            Undo();
    void                            FreeUndoHistory();
    SurfPoint3D*                    GetFaceHistory(DWORD Face);
    void                            DropFaceToUndo(DWORD Face);
    //load/save
	void							LoadMesh(const char* Name);
	void							SaveMesh(const char* Name);
	void							LoadTexture(const char* Name);
	void							SaveTexture(const char* Name);
	void							SaveNormalmap(const char* Name);
    void                            SaveObjFile(const char* Name,uni_hash<DWORD,DWORD,tbsz>& FList);
    void							SaveSrcObj(const char* Name);
	void							SaveBigObjFile(const char* Name,int Lod);//Lod=0 - max size
    void                            RasterizePN(int uv1,int uv2,int uv3,Vector3D* P,Vector3D* N);
    void                            RasterizeQuad(DWORD Face,Vector3D* P,Vector3D* N);
    void                            CreateDispMap(FaceList& FList,float** DestBuf,bool Normalize);
    void                            CreateTangentSpace(FaceList& FList);
    void                            RasterizeTBN(int uv1,int uv2,int uv3,Vector3D* T,Vector3D* B,Vector3D* N);
    void                            RasterizeQuadTBN(DWORD Face,Vector3D* T,Vector3D* B,Vector3D* N);
    void                            CreateTangentSpaceNormalMap(FaceList& FList,DWORD** namp);
};
