#pragma once

class cIsoSurface {
public:
	void Init(const cVec3 &bbMin, const cVec3 &bbMax, const int NCellsX, const int NCellsY, const int NCellsZ);

	typedef float (*FieldFunc)(const cVec3 &Pos);
	void Update(FieldFunc pFn);

	typedef cVertex::PositionNormal Vertex;

	const cList<Vertex> & GetVertices() const { return m_Verts; }
	const cList<int> & GetIndices() const { return m_Indices; }
private:
	cList<Vertex> m_Verts;
	cList<int> m_Indices;
	
	int m_NCellsX, m_NCellsY, m_NCellsZ;
	
	struct Node {
		cVec3 p;
		float f;
	};
	cList<Node> m_Nodes;
	struct Edge {
		Node *pNode0, *pNode1;
		DWORD i;
	};
	cList<Edge> m_Edges;
	struct Cell {
		Node *pNodes[8];
		Edge *pEdges[12];
	};
	cList<Cell> m_Cells;
	cVec3 m_Dx, m_Dy, m_Dz;
};