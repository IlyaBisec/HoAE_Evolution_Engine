#pragma once

//*****************************************************************************
// cMesh
//*****************************************************************************
class cMesh {
public:
	typedef cVertex::Bump Vertex;

	struct Edge {
		int Verts[2]; // Verts[0] < Verts[1]
		int Tris[2];
	};

	enum eCopyCtor { CopyCtor };
	enum eSetCtor { SetCtor };
	
	cMesh();
	cMesh(const char *Name, const eSetCtor, Vertex *Verts, const int NVerts, int *Indices, const int NIndices);
	cMesh(const char *Name, const eCopyCtor, const Vertex *Verts, const int NVerts, const int *Indices, const int NIndices);
	~cMesh();

	const cList<Vertex> & GetVertices() const { return m_Verts; }
	cList<Vertex> & GetVertices() { return m_Verts; }
	
	const cList<int> & GetIndices() const { return m_Indices; }
	cList<int> & GetIndices() { return m_Indices; }

	const cList<Edge> & GetEdges() const { return m_Edges; }
	cList<Edge> & GetEdges() { return m_Edges; }
	
	const cList<int> & GetEdgeIndices() const { return m_EdgeIndices; }

	void Transform(const cMat4 &T) const;
	cList<Vertex> & GetTransformedVertices() const { return m_TransformedVerts; }
	
	const cStr & GetName() const { return m_Name; }
	void SetName(const char *Name) { m_Name = Name; }
	
	const cBounds & GetBounds() const { return m_Bounds; }
	
	const cMaterial & GetMaterial() const { return m_Material; }
	cMaterial & GetMaterial() { return m_Material; }
	
	bool RayIntersection(const cVec3 &RayOrig, const cVec3 &RayDir, float &Scale, const bool BackFaceCull = false) const;
	
	void Render() const;
	void RenderEdges(const cColor &Color = cColor::White, const float Width = 1.0f) const;
	
	void CalcNormals();
	void CalcTangents();
	void FillEdges();
	
	void CalcEnvTexCoords();
	void CalcColor(const cColor &LightAmbient);
	void CalcColor(const cVec3 &LightPos, const cColor &LightDiffuse, const cColor &LightSpecular);
	
	void CreateShadowVolume(const cVec3 &LightPos) const;
	cList<cVec4> & GetShadowVolume() const { return m_ShadowVolume; }
	void RenderShadowVolume() const;

	
	struct TorusArgs {
		float RadiusMajor;
		float RadiusMinor;
		int Sections;
		int Slices;
		cVec3 Axis;

		TorusArgs() { SetDefaults(); }
		void SetDefaults();
		void Validate();
	};

	static cMesh * GenTorus(TorusArgs = TorusArgs());
	
	struct SphereArgs {
		float Radius;
		int Sections;
		int Slices;
		cVec3 Axis;

		SphereArgs() { SetDefaults(); }
		void SetDefaults();
		void Validate();
	};

	static cMesh * GenSphere(SphereArgs = SphereArgs());

	struct GlobeArgs {
		float Radius;
		int SubDivs;

		GlobeArgs() { SetDefaults(); }
		void SetDefaults();
		void Validate();
	};

	static cMesh * GenGlobe(GlobeArgs = GlobeArgs());


	struct PlaneArgs {
		float Width;
		float Height;
		int wSubDivs;
		int hSubDivs;
		cVec3 Axis;
		bool TwoSided;
		
		PlaneArgs() { SetDefaults(); }
		void SetDefaults();
		void Validate();
	};
	
	static cMesh * GenPlane(PlaneArgs = PlaneArgs());

protected:
	cStr m_Name;
	cList<Vertex> m_Verts;
	mutable cList<Vertex> m_TransformedVerts;
	mutable cList<cVec4> m_ShadowVolume;
	cList<int> m_Indices; // 3 references to vertices for each triangle.
	cList<Edge> m_Edges;
	cList<int> m_EdgeIndices; // 3 references to edges for each triangle (negative for reversed edge).
	
	cBounds m_Bounds;
	cMaterial m_Material;
	static void CalcTangents(Vertex &t0, Vertex &t1, Vertex &t2);
};

//*****************************************************************************
// cMeshNode
//*****************************************************************************
class cMeshNode {
public:
	struct Link {
		cMeshNode *Node;
		
		cStr Name;
		cVec3 Scale;
		cQuat Rotate;
		cVec3 Translate;
	};
	typedef cList<Link *> Links;
protected:
	cMesh *m_pMesh;
	cList<Link *> m_Links;
public:
	cMeshNode(cMesh *Mesh);
	~cMeshNode();

	bool Attach(const char *LinkName, cMesh *SubMesh, const cVec3 &Scale = cVec3::One, const cQuat &Rotate = cQuat::Identity, const cVec3 &Translate = cVec3::Zero);
	bool Attach(const char *LinkName, cMeshNode *Node, const cVec3 &Scale = cVec3::One, const cQuat &Rotate = cQuat::Identity, const cVec3 &Translate = cVec3::Zero);
	bool Attach(cMeshNode::Link *Link);

	cMesh * GetMesh() const { return m_pMesh; }
	void GatherMeshes(cList<cMesh *> &List);
	void SetMesh(cMesh *Mesh);
	cMeshNode::Link * GetLink(const char *Name) const;

	void Render() const;
	void RenderEdges(const cColor &Color = cColor::White, const float Width = 1.0f) const;
	void RenderShadowVolume(const cVec3 &LightPos) const;
	void Transform(const cMat4 &ParentTransform = cMat4::Identity) const;
	void LoadMaterials();

	const Links::Iterator GetIterator() const { return m_Links.GetIterator(); }
};

template<typename Vertex>
class cDrawMesh {
public:
	struct Edge {
		int Verts[2]; // Verts[0] < Verts[1]
		int Tris[2];
	};

	const cStr & GetName() const { return m_Name; }
	void SetName(const char *Name) { m_Name = Name; }

	const cList<Vertex> & GetVertices() const { return m_Vertices; }
	cList<Vertex> & GetVertices() { return m_Vertices; }

	const cList<int> & GetIndices() const { return m_Indices; }
	cList<int> & GetIndices() { return m_Indices; }

	const cMaterial & GetMaterial() const { return m_Material; }
	cMaterial & GetMaterial() { return m_Material; }

private:
	cStr m_Name;
	cList<Vertex> m_Vertices;
	cList<int> m_Indices; // 3 references to vertices for each triangle.
	cList<Edge> m_Edges;
	cList<int> m_EdgeIndices; // 3 references to edges for each triangle (negative for reversed edge).
	cMaterial m_Material;
};