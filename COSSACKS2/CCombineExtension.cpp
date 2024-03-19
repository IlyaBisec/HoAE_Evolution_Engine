#include "CCombineExtension.h"

extern UniversalMap g_UniversalMap;
CCombineExtension g_CombineExtension(&g_UniversalMap);

// ================================================================================
// CCombineExtension::BrowseForModel
// ================================================================================
bool CCombineExtension::BrowseForModel()
{
	// Storing current directory:
	char szCurDirectory[4096];
	szCurDirectory[0] = 0;
	GetCurrentDirectory(4096, szCurDirectory);

	bool ProcessLoadSave(int, int, int, int, _str&, const char *, const char *, bool, ParentFrame *);
	extern int LastKey;
	LastKey = 0;
	const int dLoadSaveOffset = 100;

	_str strBuffer;
	bool bResult = ProcessLoadSave(dLoadSaveOffset, dLoadSaveOffset, RealLx - 2 * dLoadSaveOffset,
		RealLy - 2 * dLoadSaveOffset, strBuffer, ".c2m", "Models/", true, NULL);

	// Restoring working directory:
	SetCurrentDirectory(szCurDirectory);
	if(bResult)
	{
		m_ModelName = strBuffer;
		return true;
	}
	return false;
} // CCombineExtension::BrowseForModel

// ================================================================================
// CCombineExtension::LoadModel
// ================================================================================
bool CCombineExtension::LoadModel()
{
	Clear();

	// Loading model:
	DWORD idModel = IMM->GetModelID(m_ModelName.str);
	if(idModel == 0xFFFFFFFF) return false;
	AABoundBox BBox = IMM->GetBoundBox(idModel);
	float dx = BBox.maxv.x - BBox.minv.x, dy = BBox.maxv.y - BBox.minv.y, dz = BBox.maxv.z - BBox.minv.z, mx = 0.0f;
	mx = max(dx, dy);
	mx = max(mx, dz);
	if(mx < 1.0f) return false; // This model is too small!

	float cx = (BBox.minv.x + BBox.maxv.x) / 2.0f, cy = (BBox.minv.y + BBox.maxv.y) / 2.0f,
		cz = (BBox.minv.z + BBox.maxv.z) / 2.0f;
	const float NormalFitSide = 300.0f, // In cube with this side we will fit the model!
		MinFitSide = 50.0f, fScalingSteps = 10.0f;
	m_fScale = NormalFitSide / mx, m_fScaleMin = MinFitSide / mx;
	m_fScalingSpeed = (m_fScale - m_fScaleMin) / fScalingSteps;

	// Essentially loading geometry:
	Matrix4D m4dLocal;
	int NGeometries = IMM->GetNumGeometries(idModel);
	CIndices cIndices;
	COneMapVertices cVertices;
	for(int nG = 0; nG < NGeometries; nG++)
	{
		IGeometry *pIGeometry = IMM->GetGeometry(idModel, nG, m4dLocal);
		int NVertices = pIGeometry->GetNumVertices(), NIndices = pIGeometry->GetNumIndices();
		// caching indices to cIndices:
		WORD *pIndices = pIGeometry->GetIndices();
		if(!pIndices) continue;
		for(int i = 0; i < NIndices; i++)
		{
			cIndices.Add(*pIndices);
			pIndices++;
		}
		// caching vertices to cVertices:
		VertexIterator vtxIterator;
		pIGeometry->GetVertexIterator(vtxIterator);
		OneMapVertex v;
		for(i = 0; i < NVertices; i++)
		{
			Vector3D v3dPos = vtxIterator.pos(i);
			// Scaling:
			float x = v3dPos.x * m4dLocal.e00 + v3dPos.y * m4dLocal.e10 + v3dPos.z * m4dLocal.e20 + m4dLocal.e30,
				y = v3dPos.x * m4dLocal.e01 + v3dPos.y * m4dLocal.e11 + v3dPos.z * m4dLocal.e21 + m4dLocal.e31,
				z = v3dPos.x * m4dLocal.e02 + v3dPos.y * m4dLocal.e12 + v3dPos.z * m4dLocal.e22 + m4dLocal.e32;
			v.x = (short)(x - cx), v.y = (short)(y - cy), v.z = (short)(z - cz);
			cVertices.Add(v);
		}
		m_Mesh.Sum(cVertices, cIndices);
		cVertices.Clear(), cIndices.Clear();
	}
	return m_Mesh.Vert.NValues && m_Mesh.Idxs.NValues;
} // CCombineExtension::LoadModel


#include "SurfaceGenerator.cpp"
// ================================================================================
// CCombineExtension::Cache
// ================================================================================
void CCombineExtension::Cache()
{
	m_Cache.Release();
	m_Cache.Vert.AddValues(m_Mesh.Vert.GetValues(), m_Mesh.Vert.NValues);
	m_Cache.Idxs.AddValues(m_Mesh.Idxs.GetValues(), m_Mesh.Idxs.NValues);

	for(int i = 0; i < m_Cache.Vert.NValues; i++)
	{
		OneMapVertex &v = m_Cache.Vert[i];
		float x = v.x, y = v.y, z = v.z;
		// Scaling:
		x *= m_fScale, y *= m_fScale, z *= m_fScale;
		// Rotation:
		float X = x * m_T.e00 + y * m_T.e10 + z * m_T.e20 + m_CoordX,
			Y = x * m_T.e01 + y * m_T.e11 + z * m_T.e21 + m_CoordY,
			Z = x * m_T.e02 + y * m_T.e12 + z * m_T.e22 + m_CoordZ;
		v.x = short(X), v.y = short(Y), v.z = short(Z);

		v.Color = 0xFFFFFFFF;
	}
	m_Cache.CalcNormals();
	m_Cache.Shade(false, m_pUniversalMap->m_pSun);
	SURF.TexturizeSurface(m_Cache);
} // CCombineExtension::Cache

// ================================================================================
// CCombineExtension::OnMouseHandling
// ================================================================================
bool CCombineExtension::OnMouseHandling(int mx, int my, bool &LeftPressed,
			bool &RightPressed, int MapCoordX, int MapCoordY, bool OverMiniMap)
{
	if(!m_bActive) return false;
	m_CoordX = MapCoordX, m_CoordY = MapCoordY;

	void ResetGroundCache();
	extern UniversalMap g_UniversalMap;
	int x0 = m_Cache.Vert[0].x, y0 = m_Cache.Vert[0].y, x1 = x0, y1 = y0;
	bool bUnion = LeftPressed, bDifference = RightPressed && !(GetKeyState(VK_CONTROL) & 0x8000);
	if(bUnion || bDifference)
	{
		ClearActivityOfEditor();
		m_pUniversalMap->Backup_Store();
		if(bUnion) m_pUniversalMap->Sum(m_Cache);
		else m_pUniversalMap->Difference(m_Cache);
		int x0, y0, x1, y1;
		m_Cache.CalcBoundRect(x0, y0, x1, y1);
		m_pUniversalMap->SupplementMapOfHeightsInRect(x0, y0, x1, y1);
		ResetGroundCache();
	}
	return true;
} // CCombineExtension::OnMouseHandling

// ================================================================================
// CCombineExtension::OnMouseWheel
// ================================================================================
bool CCombineExtension::OnMouseWheel(int Delta)
{
	if(!m_bActive) return false;
	const float fRotationSpeed = 0.2f;
	const int iSpeedAlongZ = 10;
	bool bShift = GetKeyState(VK_LSHIFT) & 0x8000, bAlt = GetKeyState(VK_MENU) & 0x8000,
		bControl = GetKeyState(VK_LCONTROL) & 0x8000;
	Matrix4D T;
	if(bShift) // Shifting along Z axis:
	{
		if(Delta > 0) m_CoordZ += iSpeedAlongZ;
		else m_CoordZ -= iSpeedAlongZ;
	}
	else
		if(bAlt && bControl) // Rotation along Y axis:
		{
			if(Delta > 0) T.rotation(Vector3D::oY, fRotationSpeed);
			else T.rotation(Vector3D::oY, -fRotationSpeed);
			m_T *= T;
		}
		else
			if(bAlt) // Scaling:
			{
				if(Delta > 0) m_fScale += m_fScalingSpeed;
				else m_fScale -= m_fScalingSpeed;
				if(m_fScale < m_fScaleMin) m_fScale = m_fScaleMin;
			}
			else
				if(bControl) // Rotation along X axis:
				{
					if(Delta > 0) T.rotation(Vector3D::oX, fRotationSpeed);
					else T.rotation(Vector3D::oX, -fRotationSpeed);
					m_T *= T;
				}
				else // Rotation along Z axis:
				{
					if(Delta > 0) T.rotation(Vector3D::oZ, fRotationSpeed);
					else T.rotation(Vector3D::oZ, -fRotationSpeed);
					m_T *= T;
				}
	return true;
} // CCombineExtension::OnMouseWheel

// ================================================================================
// CCombineExtension::OnDrawOnMapOverAll
// ================================================================================
void CCombineExtension::OnDrawOnMapOverAll()
{
	if(!m_bActive) return;
	// Inverting normals (if needed):
	extern int LastKey;
	if(LastKey == 'I' || LastKey == 'i')
	{
		m_Mesh.InvertNormals();
		LastKey = 0;
	}
	// -------------------------------------------------
	Cache();
	VertexBasedMeshRef Ref;
	Ref.pOrigin = &m_Cache;
	int NTri = m_Cache.Idxs.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++) Ref.nTriangles.Add(nTri);
	OneSplitMesh SplittedMesh;
	//SplittedMesh.AdvSplitMesh(&Ref);
	SplittedMesh.SpeedySplit(Ref);
	SplittedMesh.Draw();
	SplittedMesh.Release();
	Ref.Release();
	// Info:
	char Buffer[4096];
	strcpy(Buffer, "Combine: ");
	strcat(Buffer, m_ModelName.str);
	ShowString(10, RealLy - 40, Buffer, &BigWhiteFont);
	// ===========================================
	if(GetKeyState(VK_RETURN) & 0x8000)
	{
		ClearActivityOfEditor();
		m_pUniversalMap->Backup_Store();
		m_pUniversalMap->Sum(m_Cache);
		void ResetGroundCache(void);
		ResetGroundCache();
	}
} // CCombineExtension::OnDrawOnMapOverAll