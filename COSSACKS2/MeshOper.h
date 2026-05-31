#pragma once
class BasicVertex {
public:
	dVector Pos;
	dVector N;
	float	u;
	float	v;
	DWORD MtlID;
};
class BasicMesh;
class BasicTriangle {
public:
	dVector N;
	int nIndA;
	int nIndB;
	int nIndC;
	DWORD MtlID;//matherial ID
	dVector CalcN(BasicMesh& m);
	dVector CalcD(BasicMesh& m);
	double  getQ(BasicMesh& m, int v1 = -1, int v2 = -1);
	bool HaveEdge(int v1, int v2) {
		return (v1 == nIndA && v2 == nIndB)
			|| (v1 == nIndB && v2 == nIndA)
			|| (v1 == nIndA && v2 == nIndC)
			|| (v1 == nIndC && v2 == nIndA)
			|| (v1 == nIndC && v2 == nIndB)
			|| (v1 == nIndB && v2 == nIndC);
	}
};
class BasicEdge {
public:
	int Vert1;
	int Vert2;
};
class BasicMesh {
public:
	uni_hash<BasicVertex>			Verts;
	uni_hash<BasicTriangle>			Triangles;
	uni_hash<BasicEdge, DWORDS2>		Edges;


	uni_hash<DWORD>			VertsNearVerts;
	uni_hash<DWORD>			FacesNearVerts;
	uni_hash<DWORD>			EdgesNearVerts;
	uni_hash<DWORD>			FacesNearEdge;
	uni_hash<DWORD>			EdgesNearFace;
	uni_hash<DWORD>			FacesNearFace;
	uni_hash<DWORD, DWORDS3> TriPresence;

	uni_hash<DWORD, DWORDS2> SelectedEdges;

	DWORD TriangleMaxID;
	DWORD VertexMaxID;

	BasicMesh() {
		TriangleMaxID = 0;
		VertexMaxID = 0;
	}

	DWORD AddVertex(dVector V, DWORD ID = 0xFFFFFFFF, DWORD MtlID = 0xFFFFFFFF, float u = 0, float v = 0);
	DWORD AddVertex(Vector3D V, DWORD ID = 0xFFFFFFFF, DWORD MtlID = 0xFFFFFFFF, float u = 0, float v = 0);
	DWORD AddVertex(BasicVertex& BV, DWORD ID = 0xFFFFFFFF);
	void  RemoveVertex(DWORD ID);
	void  RemoveTriangle(DWORD ID);
	int AddTriangle(DWORD V1, DWORD V2, DWORD V3, DWORD MTLID);
	int AddEdge(int V1, int V2);
	void RemoveDeadVerts(BasicMesh& dest);
	void CopyTo(BasicMesh& dest, DWORD StartV = 0);
	void ReadFromPly(const char* name);
	void WriteToPly(const char* name);
	void reset();
	dVector getV(int idx);
	//------simplest ops----------//
	void shift(dVector D);
	void randshift(double Amplitude);
	void invertNormals();
	void scale(double s);
	dVector size();
	double  maxsize();
	bool CheckMesh();
	bool CheckIfVertexIsInside(DWORD ID);
	void CalcVertNormals();
	//-----simplification---------//
	bool collapse(int from, int to);
	double check_collapse(int from, int to);//value>10000 - collapse not possible
	//-----complex operations-----//
	void boolean(BasicMesh& m1, BasicMesh& m2, bool Subtract = false);
	void soften_edges(BasicMesh& dest, float Width);
	void select_hard_edges(float Threshold = 0.7);
	void smart_smooth(int nsteps = 10, double force = 0.15f, double dotp_threshold = 0.85);
	void resolve_dist_threshold();//after this function all verts will be in different cells 1x1
};

