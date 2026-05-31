#include "stdheader.h"
#include "CCtrlVExtension.h"

extern CUniversalMapClipboard g_UniversalMapClipboard;
extern UniversalMap g_UniversalMap;
CCtrlVExtension g_CtrlVExtension(&g_UniversalMapClipboard, &g_UniversalMap);

// ================================================================================
// CCtrlVExtension::CheckActivityOfEditor
// ================================================================================
bool CCtrlVExtension::CheckActivityOfEditor() { return IsActive; }

// ================================================================================
// CCtrlVExtension::ClearActivityOfEditor
// ================================================================================
void CCtrlVExtension::ClearActivityOfEditor() { IsActive = false; }

// ================================================================================
// CCtrlVExtension::ActivateEditor
// ================================================================================
void CCtrlVExtension::ActivateEditor()
{
	IsActive = m_pUniversalMapClipboard->Vertices.NValues &&
		m_pUniversalMapClipboard->Indices.NValues &&
		m_pUniversalMapClipboard->Contour.NValues;
	CoordX = CoordY = CoordZ = 0;
}

// ================================================================================
// CCtrlVExtension::OnMouseWheel
// ================================================================================
bool CCtrlVExtension::OnMouseWheel(int Delta)
{
	if(!IsActive || !Delta) return false;
	const int dZ = 10;
	if(GetKeyState(VK_LSHIFT) & 0x8000) CoordZ += (Delta > 0 ? dZ : - dZ);
	else m_pUniversalMapClipboard->Rotate(Delta);
	const int MaxDCoordZ = 200;
	if(CoordZ > MaxDCoordZ) CoordZ = MaxDCoordZ;
	if(CoordZ < - MaxDCoordZ) CoordZ = - MaxDCoordZ;
	return true;
}

// ================================================================================
// CCtrlVExtension::OnMouseHandling
// ================================================================================
bool CCtrlVExtension::OnMouseHandling(int mx, int my, bool &LeftPressed,
										bool &RightPressed, int MapCoordX,
										int MapCoordY, bool OverMiniMap)
{
	if(!IsActive) return false;
	CoordX = MapCoordX, CoordY = MapCoordY;

	void ResetGroundCache();
	if(LeftPressed == true)
	{
		IsActive = false;
		m_pUniversalMap->CtrlV(*m_pUniversalMapClipboard, CoordX, CoordY, CoordZ);
		ResetGroundCache();
		return true;
	}
	return true;
}

// ================================================================================
// CCtrlVExtension::OnDrawOnMapOverAll
// ================================================================================
void CCtrlVExtension::OnDrawOnMapOverAll()
{
	if(!IsActive) return;
	if(!m_pUniversalMapClipboard->Vertices.NValues || !m_pUniversalMapClipboard->Indices.NValues ||
		!m_pUniversalMapClipboard->Contour.NValues) return;
	int x0 = CoordX - m_pUniversalMapClipboard->r, y0 = CoordY - m_pUniversalMapClipboard->r,
		x1 = CoordX + m_pUniversalMapClipboard->r, y1 = CoordY + m_pUniversalMapClipboard->r;
	bool bRedden = false;
	if((x0 - m_pUniversalMap->QuadX0 < UniversalMap::c_CtrlCVOffset) ||
		(y0 - m_pUniversalMap->QuadY0 < UniversalMap::c_CtrlCVOffset) ||
		(m_pUniversalMap->QuadX0 + m_pUniversalMap->UMLx - x1 < UniversalMap::c_CtrlCVOffset) ||
		(m_pUniversalMap->QuadY0 + m_pUniversalMap->UMLy - y1 < UniversalMap::c_CtrlCVOffset)) bRedden = true;
	m_pUniversalMapClipboard->Draw(CoordX, CoordY, CoordZ, bRedden);
	// Info:
	ShowString(10, RealLy - 40, "Paste", &BigRedFont);
}