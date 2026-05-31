#ifndef __CCOMBINEEXTENSION__
#define __CCOMBINEEXTENSION__

#include "..\GameExtension.h"
#include "..\surface\Surface.h"

namespace Surface {

class CCombineExtension : public GameExtension
{
public:
	CCombineExtension(SurfMap *pUniversalMap)
	{
		Release();
		m_pUniversalMap = pUniversalMap;
		m_pRawMesh = NULL;
		m_Dlg.SetParent(g_hWnd);
		m_Dlg.SetInitialPath("/Meshes");
	}
	~CCombineExtension() { Release(); }
	void Release(void)
	{
		Clear();
		m_pUniversalMap = NULL;
	}
	void Clear(void)
	{
		m_bActive = false;
		m_bRedden = false;
		m_bBrowseForModel = true;
		m_CoordX = m_CoordY = m_CoordZ = 0;
		m_ScaleMin = m_ScalingSpeed = 0.0f;
		m_Scale = cVec3::Zero;
		m_T.setIdentity();
		m_Cache.Free();
		if(m_pRawMesh) {
			delete m_pRawMesh;
			m_pRawMesh = NULL;
		}
	}
	virtual bool CheckActivityOfEditor() { return m_bActive; }
	virtual void ClearActivityOfEditor();
	void ActivateEditor();
	bool BrowseForModel();
	bool LoadModel();
	void Cache();
	void OnDrawOnMapOverAll();

	virtual bool OnMouseHandling(int mx, int my, bool &LeftPressed, bool &RightPressed,
		int MapCoordX, int MapCoordY, bool OverMiniMap);
	void DrawEditorInterface();
	virtual bool OnMouseWheel(int Delta);

	bool m_bBrowseForModel;

	Matrix4D m_T;
	cVec3 m_Scale;

private:

	cWin32::MeshFileDialog m_Dlg;
	SurfMap *m_pUniversalMap;
	bool m_bActive, m_bRedden;
	int m_CoordX, m_CoordY, m_CoordZ;
	DWORD m_LoadTime;
	static const DWORD dMinTime = 0;//5000; // (ms)

	
	cStr m_FileName;
	SurfMesh m_Cache;

	cRawMesh *m_pRawMesh;
	float m_ScaleMin, m_ScalingSpeed;	

};

} // Surface

#endif // __CCOMBINEEXTENSION__