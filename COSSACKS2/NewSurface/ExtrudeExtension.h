#ifndef __ExtrudeExtension__
#define __ExtrudeExtenison__

#include "GameExtension.h"
#include "Surface.h"

extern UniversalMap g_UniversalMap;
extern int g_ToolExtensionRadius;

class ExtrudeExtension : public GameExtension {
public:
	ExtrudeExtension() {
		m_pUniversalMap = &g_UniversalMap;
		m_pRadius = &g_ToolExtensionRadius;
		m_IsActive = false;
	}
	virtual bool CheckActivityOfEditor() {
		return m_IsActive;
	}
	virtual void ClearActivityOfEditor() {
		m_IsActive = false;
	}
	virtual void ActivateEditor() {
		if(m_IsActive) return;
		m_IsActive = true;
		m_Pos = cVector::MinValue;
		m_Cache.Free();
	}
	void Cache() {
		m_Cache.Free();

		//cRawMesh S = cRawMesh::GenGlobeSphere(*m_pRadius, 30, 30);
//		cRawMesh S = cRawMesh::GenGlobeSphere(*m_pRadius, 30, 30);
//		cRawMesh2VertexBasedMesh(S, m_Cache);

		for(int i = 0; i < m_Cache.Vert.NValues; i++) {
			OneMapVertex &v = m_Cache.Vert[i];
			//v.Color = 0x30808080;
		}
		m_Cache.InvertNormals();
		m_Cache.Offset(OneMapVertex(m_Pos.x, m_Pos.y, m_Pos.z));
		m_Cache.CalcNormals();
		m_pUniversalMap->m_Shader.Shade(m_Cache, SSLocalIllumination);
	}

	virtual bool OnMouseHandling(int, int, bool &LMB, bool &RMB, int MapX, int MapY, bool) {
		if(!m_IsActive) return false;
		if(m_Pos == cVector::MinValue) { // init m_Pos:
			m_Z = 50; // <<<<<<<<<<<<<<<<<<<<<<
			m_Pos.Set(MapX, MapY, m_Z);
			Cache();
			return false;
		}

		cVector Pos(MapX, MapY, m_Z);
		m_Pos = Pos;
		Cache();
		if(!LMB) return false; // no action.
//		cVector Pos(MapX, MapY, m_Z);
//		cVector Dir = Pos - m_Pos;
//		if(comms::IsZero(Dir.Length())) return false; // the same Pos.
//		ToolParams Params;
//		m_Pos = Pos;

//		if(!LMB) return false; // no action.
		// Extruding:
//		const double d = 10;
		ToolParams Params;
		Params.cx = m_Pos.x, Params.cy = m_Pos.y, Params.cz = m_Pos.z;
		Params.r = *m_pRadius;
		m_pUniversalMap->Tool(TTExtrude, Params);
		return true;
	}
	virtual void OnDrawOnMapOverAll() {
		if(!m_IsActive) return;
		VertexBasedMeshRef Ref;
		m_Cache.GetRefAll(Ref);
		OneSplitMesh Splitted;
		Splitted.Split(Ref);
		Splitted.Draw();
	}
	virtual bool OnMouseWheel(int Delta) {
		if(!m_IsActive || m_Pos == cVector::MinValue) return false;

		bool Shift = GetKeyState(VK_LSHIFT) & 0x8000;
		if(Shift) {
			m_Z += Delta > 0 ? CPasteExtension::m_dZ : - CPasteExtension::m_dZ;
		} else {
			*m_pRadius += Delta > 0 ? ToolExtension::kRadiusStep : - ToolExtension::kRadiusStep;
			if(*m_pRadius < ToolExtension::kMinRadius) *m_pRadius = ToolExtension::kMinRadius;
			Cache();
		}
		return true;
	}
private:
	bool m_IsActive;
	UniversalMap *m_pUniversalMap;
	int *m_pRadius, m_Z;
	cVector m_Pos;
	VertexBasedMesh m_Cache;
};

#endif // __ExtrudeExtension__