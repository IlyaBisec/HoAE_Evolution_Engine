#pragma once

// Global manipulators access function:
void BindManipulators(cVec3 *pScale, cQuat *pRotate, cVec3 *pTranslate);

namespace Surface {

	class gRenderStub : public cRender {
	private:
		const cViewer *m_pViewer;
	public:
		gRenderStub() {}
		bool Init(const void *) { return true; }
		void Free() {}

		void SetViewer(const cViewer *pViewer) { m_pViewer = pViewer; }
		const cViewer * GetViewer() const { return m_pViewer; }

		idTexture AddTexture(const char *FileName, const bool MipMap) { return cRender::idNone; }
		void SetBlendFunc(const int SourceBlend, const int DestinationBlend) {}
		void SetDepthFunc(const int Compare) {}
		void SetCullMode(const int Cull) {}

		void Clear(const DWORD, const cColor &, const float, const int) {}
		void Present() {}
		void SetTransform(const int, const cMat4 &) {}
		void SetViewPort(const cRectI &) {}

		void DrawPrimitives(const int Topology, const cVertex::Format &VertexFormat, const void *Verts, const int NVerts, const int *Indices, const int NIndices, const cMaterial *Material);
		void DrawLine(const cVec3 &Fm, const cVec3 &To, const cColor &Color, const float Width);
	};

	class Manipulators : public GameExtension {
	public:
		static cVec3 *s_pScale;
		static cQuat *s_pRotate;
		static cVec3 *s_pTranslate;

		Manipulators();
		bool CheckActivityOfEditor();
		void ClearActivityOfEditor();
		void ActivateEditor();
		void OnDrawOnMapOverAll();
		bool OnMouseHandling(int, int, bool &, bool &, int, int, bool);
	private:
		bool m_IsActive;
		
		enum MODE { MODE_NONE, MODE_MOVE, MODE_ROTATE, MODE_SCALE };
		MODE m_CurMode, m_PrevMode;
		enum SPACE { SPACE_WORLD, SPACE_OBJECT };
		SPACE m_Space;
		void SetMode(MODE Tool);
		int m_idTool;
		bool m_AllowScale, m_AllowRotate;
		
		static const int LAYER_TOOLS = 0;
		
		cViewer *m_pViewer;
	};

} // Surface