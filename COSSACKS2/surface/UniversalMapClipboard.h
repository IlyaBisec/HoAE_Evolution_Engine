#ifndef __UniversalMapClipboard__
#define __UniversalMapClipboard__

#include "..\stdheader.h"
#include "Surface.h"

class ClVertex
{
public:
	OneMapVertex v;
	int index, lindex;
	bool fromStuffing;
	float fAngle;
	bool toTheLeft;
};
typedef DynArray<ClVertex> ClVertices;

class UniversalMapClipboard
{
public:
	static const int	c_MinOffset, c_MinRadius;
	VertexBasedMesh		m_Mesh, m_Cache;
	CIndices			m_Contour;
	UniversalMap		*m_pUniversalMap;
	UniversalMapClipboard(UniversalMap *pUniversalMap)
	{
		Release();
		m_pUniversalMap = pUniversalMap;
	}
	~UniversalMapClipboard() { Release(); }
	int	m_Radius;
	void Clear(void);
	void Release(void);
	void Rotate(int dir = 1);
	void Offset(int ox, int oy, int oz);
	void Redden(void);
	void Cache(void);
	void Draw(int CoordX, int CoordY, int CoordZ, bool bRedden = false);
	void Update(int *oi, int *i, OneMapVertex *v, COneMapVertices &rVertices, nHashTable &rTable, int *flags);
	bool InRound(int cx, int cy, int r, const OneMapVertex &rVertex);
	void AlignWithStencil(VertexBasedMesh &rMesh, VertexBasedMesh &rStencil, int cx, int cy, int R);
	void ExtractlVertices(const CIndices &rContour, ClVertices &rlVertices,
		bool fromStuffing, float cx, float cy, float l);
	bool Loop(CIndices &rPairs, CIndices &rContour);
	void Copy(int cx, int cy, int r);
	void Paste(int CoordX, int CoordY, int CoordZ);
	bool m_bInvalid;
private:
	OneSplitMesh		m_SplittedClipboard;
	bool				m_bRedden;
	int					m_ShiftX, m_ShiftY, m_ShiftZ;
	float				m_fAlpha;
};

#endif