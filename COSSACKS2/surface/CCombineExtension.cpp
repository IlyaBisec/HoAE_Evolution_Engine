#include "stdheader.h"
#include "CCombineExtension.h"
#include "..\Camera\Navigator.h"
#include "SurfaceGenerator.h"
#include "Typify.h"
#include "SelExt.h"
#include "..\AllTools.h"

Surface::CCombineExtension g_CombineExtension(&g_UniversalMap);
extern SurfaceGenerator SURF;

namespace Surface {

void CCombineExtension::ClearActivityOfEditor() {
	m_bActive = false;
	g_fZXCKeysAreBlocked = false;
	g_fWheelIsBlocked = false;
}

void CCombineExtension::ActivateEditor() {
	if(m_bActive) return;
	SURF.DontGenerateTrees = true;
	if(m_bBrowseForModel) BrowseForModel();
	if(!LoadModel()) return;
	WaitForInputIdle(NULL, 500);
	m_LoadTime = GetTickCount();

	if(!m_pRawMesh) {
		return;
	}
	if(!m_pRawMesh->GetRaw().Count()) {
		return;
	}
	m_CoordX = m_CoordY = m_CoordZ = 0;
	extern int LastMx, LastMy;
	m_CoordZ = cMath::Max(0, (int)SurfMap::THMapZ(LastMx, LastMy));
	m_T.setIdentity();
	m_bActive = true;
	m_bRedden = false;
	g_fZXCKeysAreBlocked = true;
	g_fWheelIsBlocked = true;
}

// OFNHookProc
UINT_PTR CALLBACK OFNHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	if(uiMsg == WM_NOTIFY) {
		OFNOTIFY *pNotify = (OFNOTIFY*)lParam;
		if(pNotify->hdr.code == CDN_INITDONE) {
			return cWin32::CenterWindow(GetParent(hDlg), g_hWnd);
		}
	}
	return FALSE;
} // OFNHookProc

// ================================================================================
// CCombineExtension::BrowseForModel
// ================================================================================
bool CCombineExtension::BrowseForModel() {
	if(!m_Dlg.DoModal()) {
		return false;
	}
	m_FileName = m_Dlg.GetFilePathName();
	return true;
} // CCombineExtension::BrowseForModel

// ================================================================================
// CCombineExtension::LoadModel
// ================================================================================
class BooleanHistory:public BaseClass{
public:
	_str ObjName;
	Matrix3D TM;
	cVec3 Scale;
};
class BHistory{
	ClassArray<BooleanHistory> BH;
	int CurrentItem;
public:
	void ExchangeAtInit(const char* ObjName,CCombineExtension* ext){
		for(int i=0;i<BH.GetAmount();i++){
			if(BH[i]->ObjName.equal(ObjName)){				
				ext->m_T=BH[i]->TM;
				ext->m_Scale=BH[i]->Scale;
				CurrentItem=i;
				return;
			}
		}
		BooleanHistory* B=new BooleanHistory;
		B->ObjName=ObjName;
		B->TM=ext->m_T;
		B->Scale=ext->m_Scale;
		BH.Add(B);
        CurrentItem=BH.GetAmount()-1;
	}
	void ExchangeAtProcess(CCombineExtension* ext){
		BooleanHistory* B=BH[CurrentItem];		
		B->TM=ext->m_T;
		B->Scale=ext->m_Scale;
	}
};

BHistory _bhistory;

bool CCombineExtension::LoadModel() {
	Clear();

	if(m_FileName.IsEmpty()) {
		return false;
	}
	m_pRawMesh = cIO::LoadMesh(m_FileName);
	if(!m_pRawMesh) {
		return false;
	}
	if(!m_pRawMesh->IsValid(true)) {
		return false;
	}
	if(m_pRawMesh->GetRaw().IsEmpty()) {
		return false;
	}

	m_pRawMesh->Triangulate();

	cSphere S = cSphere::FromPoints(m_pRawMesh->GetPositions().ToPtr(), m_pRawMesh->GetPositions().Count());
	m_pRawMesh->GetPositions() -= S.GetCenter();
	m_pRawMesh->GetPositions() *= 300.0f * cVec3(1.0f / S.GetRadius());
	
	cVec3 bbMin, bbMax;
	cBounds bb = cBounds::FromPoints(m_pRawMesh->GetPositions().ToPtr(), m_pRawMesh->GetPositions().Count());
	m_pRawMesh->GetPositions() -= cVec3::AxisY * bb.GetMin();
	
	m_Scale.Set(1);
	m_ScalingSpeed = 0.1;
	m_ScaleMin = 0.1;

	m_pRawMesh->InvertRaw();
	cMath::SwapChannels((float *)m_pRawMesh->GetPositions().ToPtr(), m_pRawMesh->GetPositions().Count(), 3, 1, 2);
	_bhistory.ExchangeAtInit(m_FileName.ToCharPtr(),this);
	return true;
} // CCombineExtension::LoadModel

Vector3D TransformCoordinate(const cVec3 &u, const Matrix4D &R, const Vector3D &T) {
	Vector3D r;
	r.x = u.x * R.e00 + u.y * R.e10 + u.z * R.e20 + T.x;
	r.y = u.x * R.e01 + u.y * R.e11 + u.z * R.e21 + T.y;
	r.z = u.x * R.e02 + u.y * R.e12 + u.z * R.e22 + T.z;
	return r;
}

// ================================================================================
// CCombineExtension::Cache
// ================================================================================
void CCombineExtension::Cache() {
	m_Cache.Free();
	if(!m_pRawMesh) {
		return;
	}
	Vector3D T(m_CoordX, m_CoordY, m_CoordZ);
	for(int i = 0; i < m_pRawMesh->GetPositions().Count(); i++) {
		const cVec3 &u = m_pRawMesh->GetPositions()[i];
		// Scaling:
		Vector3D t = TransformCoordinate(u * m_Scale, m_T, T);
        SurfVertex sv;
        sv.x = t.x;
        sv.y = t.y;
        sv.z = t.z;
        sv.SurfaceType=0;
        sv.Color=0xFFFFFFFF;

		m_Cache.VERTS.Add(sv);
	}
	int index = 0;
	while(index < m_pRawMesh->GetRaw().Count()) {
		const int Deg = m_pRawMesh->GetRaw()[index][0];
		const int idMtl = m_pRawMesh->GetRaw()[index][1];
		for(int k = 1; k <= 3; k++) {
			const int i0 = m_pRawMesh->GetRaw()[index + k][0];
			const int it = m_pRawMesh->GetRaw()[index + k][1];
			m_Cache.TRIS.Add(i0);
			if(idMtl > m_Cache.VERTS[i0].SurfaceType) {
				m_Cache.VERTS[i0].SurfaceType = idMtl;
			}
			// If texcoords are avail, we should use them:
			if(!m_pRawMesh->GetTexCoords().IsEmpty()) {
				const cVec2 &TexCoord = m_pRawMesh->GetTexCoords()[it];
				m_Cache.VERTS[i0].u = TexCoord.x;
				m_Cache.VERTS[i0].v = TexCoord.y;
			}
		}
		index += Deg + 1;
	}
	m_bRedden = false;

	m_Cache.CalcNormals();
	m_pUniversalMap->m_Shader.Shade(m_Cache, I_LOCAL);
	SURF.TexturizeSurface(m_Cache);

	// If texcoors are avail, we should use them:
	if(AllTools::KeepUV){
		if(!m_pRawMesh->GetTexCoords().IsEmpty()) {
			index = 0;
			while(index < m_pRawMesh->GetRaw().Count()) {
				const int Deg = m_pRawMesh->GetRaw()[index][0];
				for(int k = 1; k <= 3; k++) {
					const int ip = m_pRawMesh->GetRaw()[index + k][0];
					const int it = m_pRawMesh->GetRaw()[index + k][1];
					const cVec2 &TexCoord = m_pRawMesh->GetTexCoords()[it];
					m_Cache.VERTS[ip].TexCoord() = TexCoord;
				}
				index += Deg + 1;
			}
		}
	}
} // CCombineExtension::Cache

// ================================================================================
// CCombineExtension::OnMouseHandling
// ================================================================================

bool CCombineExtension::OnMouseHandling(int mx, int my, bool &LeftPressed,
			bool &RightPressed, int MapCoordX, int MapCoordY, bool OverMiniMap)
{
	if(!m_bActive) return false;
    Ray3D ray = ICam->GetPickRay( mx, my );
    Vector3D iPt;
    ray.IntersectPlane(Plane::xOy,iPt);
	m_CoordX = iPt.x, m_CoordY = iPt.y;

	void ResetGroundCache();
	int x0 = m_Cache.VERTS[0].x, y0 = m_Cache.VERTS[0].y, x1 = x0, y1 = y0;
	bool bUnion = LeftPressed, bDifference = RightPressed;
	_bhistory.ExchangeAtProcess(this);
	if((bUnion || bDifference) && !g_Navigator.m_Mode && GetTickCount() - m_LoadTime > CCombineExtension::dMinTime) {
		ClearActivityOfEditor();
		if(!m_bRedden)
		{
//			m_pUniversalMap->Backup_Store();

			//if(bUnion) m_pUniversalMap->Sum(m_Cache);
//			if(bUnion) m_pUniversalMap->Union(m_Cache);
//			else m_pUniversalMap->Difference(m_Cache);
			if(GetKeyState(VK_SHIFT) < 0) SimplifySurface(m_Cache);
			if(g_SurfEditor.BooleansSum) m_pUniversalMap->BooleansSum(m_Cache);
            else{
                /*
                Vector3D T(m_CoordX, m_CoordY, m_CoordZ);
                for(int i = 0; i < m_RawMesh.VERTS.Count(); i++) {
		            const cVec3 &u = m_RawMesh.VERTS[i];
		            // Scaling:
		            Vector3D t = TransformCoordinate(u * m_Scale, m_T, T);
                    m_RawMesh.VERTS[i] = cVec3( t.x,t.z,t.y );
                }
                for(int i = 0; i < m_RawMesh.TRIS.Count();i+=3) {
                    swap(m_RawMesh.TRIS[i],m_RawMesh.TRIS[i+1]);
                }
                m_pUniversalMap->Booleans(m_RawMesh, bUnion ? BO_UNION : BO_DIFFERENCE);
                */
                m_pUniversalMap->Booleans(m_Cache, bUnion ? BO_UNION : BO_DIFFERENCE);
            }
			SurfVertex c;
			int r;
			m_Cache.CalcBoundSphere(c, r);
			//			m_pUniversalMap->SupplementMapOfHeightsInRect(x0, y0, x1, y1);
//			m_pUniversalMap->FillMapOfHeights(SurfVertex(x0, y0), SurfVertex(x1, y1));
//			VertexBasedMeshRef Ref;
//			int cx = (bbMin.x + bbMax.x) / 2, cy = (bbMin.y + bbMax.y) / 2, r = Max(bbMax.x - bbMin.x, bbMax.y - bbMin.y);
//			m_pUniversalMap->GetQuadsRefCylinder(c.x, c.y, r, Ref);
//			Ref.FillMapOfHeights();
			SurfRef Ref;
			m_pUniversalMap->QUADS.GetRefCylinder(Ref, c, r);
			Ref.Complete(SR_TRIS, SR_QUADS);
			Ref.FillMapOfHeights();
			m_pUniversalMap->CalcNormals();	

			ResetGroundCache();
		}
	}
	return true;
} // CCombineExtension::OnMouseHandling

// ================================================================================
// CCombineExtension::OnMouseWheel
// ================================================================================
bool CCombineExtension::OnMouseWheel(int Delta) {
	if(!m_bActive) return false;
	const float cRotSpeed = 0.2f;
	const int cAlongZSpeed = 10;

	float Gear = GetKeyState(VK_TAB) < 0 ? 4.0f : 1.0f;
	float CurRotSpeed = cRotSpeed / Gear;
	float CurAlongZSpeed = cAlongZSpeed / Gear;
	bool fShift = GetKeyState(VK_LSHIFT) & 0x8000, fScale = GetKeyState(VK_MENU) & 0x8000,
		fGlobalRotation = GetKeyState(VK_LCONTROL) & 0x8000;
	Matrix4D T;

	// Mask of ZXC keys:
	cVec3 Mask = cVec3::Zero;
	if(GetKeyState('z') < 0 || GetKeyState('Z') < 0) {
		Mask += cVec3::AxisZ;
	}
	if(GetKeyState('x') < 0 || GetKeyState('X') < 0) {
		Mask += cVec3::AxisX;
	}
	if(GetKeyState('c') < 0 || GetKeyState('C') < 0) {
		Mask += cVec3::AxisY;
	}

	if(fShift) {
		if(Delta > 0) m_CoordZ += CurAlongZSpeed;
		else m_CoordZ -= CurAlongZSpeed;
	} else if(fScale) {
		if(Mask.IsZero()) Mask.Set(1);
		if(Delta > 0) m_Scale += Mask * m_ScalingSpeed * cVec3(1);
		else m_Scale -= Mask * m_ScalingSpeed;
		if(m_Scale.x < m_ScaleMin) m_Scale.x = m_ScaleMin;
		if(m_Scale.y < m_ScaleMin) m_Scale.y = m_ScaleMin;
		if(m_Scale.z < m_ScaleMin) m_Scale.z = m_ScaleMin;
	} else if(fGlobalRotation) {
		if(Mask.IsZero()) Mask = cVec3::AxisZ;
		if(Mask.z > 0) {
			T.rotation(Vector3D::oZ, cMath::Sign(Delta) * CurRotSpeed);
			m_T *= T;
		}
		if(Mask.x > 0) {
			T.rotation(Vector3D::oX, cMath::Sign(Delta) * CurRotSpeed);
			m_T *= T;
		}
		if(Mask.y > 0) {
			T.rotation(Vector3D::oY, cMath::Sign(Delta) * CurRotSpeed);
			m_T *= T;
		}
	} else {
		if(Mask.IsZero()) Mask = cVec3::AxisZ;
		if(Mask.z > 0) {
			T.rotation(Vector3D::oZ, cMath::Sign(Delta) * CurRotSpeed);
			m_T = T *= m_T;
		}
		if(Mask.x > 0) {
			T.rotation(Vector3D::oX, cMath::Sign(Delta) * CurRotSpeed);
			m_T = T *= m_T;
		}
		if(Mask.y > 0) {
			T.rotation(Vector3D::oY, cMath::Sign(Delta) * CurRotSpeed);
			m_T = T *= m_T;
		}
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
	static bool fWasPressed = false;
	bool fIsPressed = GetKeyState(VK_SPACE) < 0;
	if(!fIsPressed) fWasPressed = false;
	if(fIsPressed && !fWasPressed) {
		m_pRawMesh->InvertRaw();
		fWasPressed = true;
	}

	Cache();

	m_Cache.Draw(Surface::SFX_BOOLEANS);
	// Axes:
	Vector3D O(m_CoordX, m_CoordY, m_CoordZ);
	Vector3D xAxis = TransformCoordinate(cVec3::AxisX, m_T, Vector3D::null);
	Vector3D yAxis = TransformCoordinate(cVec3::AxisY, m_T, Vector3D::null);
	Vector3D zAxis = TransformCoordinate(cVec3::AxisZ, m_T, Vector3D::null);
	DrawArrow(O, xAxis, cColor::Red.ToBgra(), 100, 0.1, 0.1);
	DrawArrow(O, yAxis, cColor::Green.ToBgra(), 100, 0.1, 0.1);
	DrawArrow(O, zAxis, cColor::Blue.ToBgra(), 100, 0.1, 0.1);

	// Info:
	char Buffer[4096];
	strcpy(Buffer, "Combine: ");
	strcat(Buffer, m_FileName.ToCharPtr());
	ShowString(200, RealLy - 40, Buffer, &BigWhiteFont);
	// ===========================================
	if(GetKeyState(VK_RETURN) & 0x8000)
	{
		ClearActivityOfEditor();
//		m_pUniversalMap->Backup_Store();
//		m_pUniversalMap->Sum(m_Cache);
		ResetGroundCache();
	}
} // CCombineExtension::OnDrawOnMapOverAll

//-----------------------------------------------------------------------------
// DrawEditorInterface
//-----------------------------------------------------------------------------
void CCombineExtension::DrawEditorInterface() {
	if(!m_bActive) return;
	Typify::TypeControl();
}

} // Surface