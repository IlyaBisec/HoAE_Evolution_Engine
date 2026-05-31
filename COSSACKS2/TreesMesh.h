#pragma once
class TreeHashElem{
public:
	DWORD       UniqID;
	DWORD       StateHashValue;
	float       SortValue;
	BaseMesh*   BasicGeometry;
	Vertex2t*   CalculatedVerts;
    int         TexID;

	TreeHashElem(){
        UniqID=0xBAADF00D;
		StateHashValue=0xDEADBEEF;
        BasicGeometry=NULL;
		CalculatedVerts=NULL;
	}
	unsigned int	hash	() const {
        return UniqID;
	}
	bool			equal	( const TreeHashElem& el ){
		return UniqID==el.UniqID;
	}
	void			copy	( const TreeHashElem& el ){
		memcpy(this,&el,sizeof TreeHashElem);
	}
	void Create(DWORD ID,DWORD State,Vector3D pos,int ModelID,DWORD Color,Matrix4D& transform);
	void Free();
};

#define num_common_tex 16
class TreesMeshCreator{
	Hash<TreeHashElem,c_DefTableSize,65536>  TreesHash;
	DynArray<TreeHashElem*> TaskToDraw;    
	DynArray<StaticMesh*> Meshes[num_common_tex];    
    int cmTexID[num_common_tex];
	int CurMesh[num_common_tex];
	DWORD SummHash;
	DWORD PrevSummHash;
public:
	void ClearPrevHash();
	void AddTree(DWORD ID,DWORD State,Vector3D pos,int ModelID,DWORD Color,Matrix4D& transform);
	void CreateMeshes();
	void DrawTreesMeshes();
	void DrawTreesPrims();
	void DrawTreesShadows();
	int  GetMeshes(BaseMesh** OutBuf,int MaxSize);
	void Clear();
	void Free();
};
extern TreesMeshCreator TreesMesh;