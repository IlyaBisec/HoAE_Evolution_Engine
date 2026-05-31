#include "stdheader.h"
#include "Manipulators.h"
#include "Surface.h"
#include "../CurrentMapOptions.h"

//-------------------------------------------------------------------------
// BindManipulators
//-------------------------------------------------------------------------
void BindManipulators(cVec3 *pScale, cQuat *pRotate, cVec3 *pTranslate) {
	// Is manipulators extension active?
	if(IsActiveSurfTool("Manipulators")) {
		// Free it.
		ClearSurfTool();
	}
	// Bind new fields:
	Surface::Manipulators::s_pScale = pScale;
	Surface::Manipulators::s_pRotate = pRotate;
	Surface::Manipulators::s_pTranslate = pTranslate;
	
	ActivateSurfTool("Manipulators");
} // BindManipulators

namespace Surface {
	void gRenderStub::DrawPrimitives(const int Topology, const cVertex::Format &VertexFormat, const void *Verts, const int NVerts, const int *Indices, const int NIndices, const cMaterial *Material) {
		static int idShader = IRS->GetShaderID("Manipulators");
		BaseMesh bm;
		if(cRender::Topology::PointList == Topology) {
			const cVertex::TransformedColored *vb = (const cVertex::TransformedColored *)Verts;

			// Creating BaseMesh with TnL vb:
			bm.create(NVerts, NIndices, vfVertexTnL);

			// Lock vb:
			VertexTnL *ToVerts = (VertexTnL *)bm.getVertexData();

			// Filling BaseMesh vb:
			for(int nVert = 0; nVert < NVerts; nVert++) {
				ToVerts->x = vb->Pos.x;
				ToVerts->y = vb->Pos.y;
				ToVerts->z = vb->Pos.z;
				ToVerts->w = vb->Pos.w;
				ToVerts->u = ToVerts->v = 0.0f;
				ToVerts->diffuse = ToVerts->specular = cColor::FromRgba(vb->Color).ToBgra();
				vb++;
				ToVerts++;
			}

			// Finish BaseMesh:
			bm.setPriType(ptPointList);
			bm.setNVert(NVerts);
			bm.setNInd(0);
			bm.setNPri(NVerts);
			bm.setTexture(-1);
			bm.setShader(idShader);

			// Draw:
			DrawBM(bm);
		} else if(cRender::Topology::TriangleList == Topology) {
			const cVertex::PositionColored *vb = (const cVertex::PositionColored *)Verts;
			// Creating BaseMesh:
			BaseMesh bm;
			bm.create(NVerts, NIndices, vfVERTEX);
			VertexTS *ToVerts = (VertexTS *)bm.getVertexData();
			word *ToIndices = bm.getIndices();
			for(int nVert = 0; nVert < NVerts; nVert++) {
				ToVerts->x = vb->Pos.x;
				ToVerts->y = vb->Pos.y;
				ToVerts->z = vb->Pos.z;
				ToVerts->w = 1.0f;
				ToVerts->u = ToVerts->v = 0.0f;
				ToVerts->diffuse = ToVerts->specular = cColor::FromRgba(vb->Color).ToBgra();
				vb++;
				ToVerts++;
			}
			const int NTris = NIndices / 3;
			for(int nTri = 0; nTri < NTris; nTri++) {
				ToIndices[0] = Indices[0];
				ToIndices[1] = Indices[2];
				ToIndices[2] = Indices[1];
				ToIndices += 3;
				Indices += 3;
			}
			bm.setNVert(NVerts);
			bm.setNInd(NIndices);
			bm.setNPri(NIndices / 3);
			bm.setShader(idShader);
			bm.setTexture(-1);

			DrawBM(bm);
		}
	}
	void gRenderStub::DrawLine(const cVec3 &Fm, const cVec3 &To, const cColor &Color, const float Width) {
		rsLine(*(const Vector3D *)&Fm, *(const Vector3D *)&To, Color.ToBgra());
		rsFlush();
	}

	// Manipulators binded with:
	cVec3 *Manipulators::s_pScale = NULL;
	cQuat *Manipulators::s_pRotate = NULL;
	cVec3 *Manipulators::s_pTranslate = NULL;
	
	//-------------------------------------------------------------------------
	// Manipulators.ctor
	//-------------------------------------------------------------------------
	Manipulators::Manipulators() {
		m_IsActive = false;

		cRender::SetInstance(new gRenderStub);
		m_pViewer = new cViewer;
		m_pViewer->SetRightHanded(false); // Left Handed? Oh no... That's twisted frame!
		cRender::GetInstance()->SetViewer(m_pViewer);

		m_idTool = -1;
		m_CurMode = MODE_NONE;
		m_PrevMode = MODE_MOVE;
		m_Space = SPACE_WORLD;

		m_AllowScale = false;
		m_AllowRotate = false;
	} // Manipulators.ctor

	//-------------------------------------------------------------------------
	// Manipulators::CheckActivityOfEditor
	//-------------------------------------------------------------------------
	bool Manipulators::CheckActivityOfEditor() {
		return m_IsActive;
	} // Manipulators::CheckActivityOfEditor

	//-------------------------------------------------------------------------
	// Manipulators::ActivateEditor
	//-------------------------------------------------------------------------
	void Manipulators::ActivateEditor() {
		if(m_IsActive) {
			return;
		}

		if(NULL == s_pTranslate) { // Nothing to bind with....
			return;
		}

		m_AllowScale = s_pScale != NULL && s_pRotate != NULL && s_pTranslate != NULL;
		m_AllowRotate = s_pRotate != NULL && s_pTranslate != NULL;
		
		m_IsActive = true;

		extern bool g_WERKeysAreBlocked;
		g_WERKeysAreBlocked = true;

		// Init Input Manager:
		cInput::SetInstance(new cWin32::InputManager);
		// Well, this is really hack, but... Input manager in fn "Init",
		// subclasses parent window proc to handle some messages. Among them is WM_MOUSEWHEEL.
		// In our case of manipulators integration, we're not using mouse wheel.
		// So we should "unsubclass" parent window after "Init" fn to save wheel functionality
		// for other application extensions. For example "Free Camera".
		LONG_PTR OldWndProc = GetWindowLongPtr(g_hWnd, GWLP_WNDPROC);
		cInput::GetInstance()->Init(g_hWnd);
		if(GetWindowLongPtr(g_hWnd, GWLP_WNDPROC)) { // If window was really subclassed...
			SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, OldWndProc);
		}
		
		// Fix prev mode with allowed flags:
		if(MODE_SCALE == m_PrevMode && !m_AllowScale) {
			m_PrevMode = MODE_MOVE;
		}
		if(MODE_ROTATE == m_PrevMode && !m_AllowRotate) {
			m_PrevMode = MODE_MOVE;
		}

		// Fix space with allowed object space flag:
		if(SPACE_OBJECT == m_Space && !m_AllowRotate) {
			m_Space = SPACE_WORLD;
		}
		
		SetMode(m_PrevMode);
	} // Manipulators::ActivateEditor

	//-------------------------------------------------------------------------
	// Manipulators::ClearActivityOfEditor
	//-------------------------------------------------------------------------
	void Manipulators::ClearActivityOfEditor() {
		if(!m_IsActive) {
			return;
		}
	
		m_IsActive = false;

		extern bool g_WERKeysAreBlocked;
		g_WERKeysAreBlocked = false;

		m_PrevMode = m_CurMode;
		SetMode(MODE_NONE);

		// Free Input Manager:
		cInput::GetInstance()->Free();
	} // Manipulators::ClearActivityOfEditor

	//-------------------------------------------------------------------------
	// Manipulators::OnDrawOnMapOverAll
	//-------------------------------------------------------------------------
	void Manipulators::OnDrawOnMapOverAll() {
		if(!m_IsActive) {
			return;
		}
		cAssert(m_idTool != -1); // Some widget always must be allocated.

		// Read current camera params and pass them to cViewer:
		const Vector3D Pos = ICam->GetPosition();
		const Vector3D Right = ICam->GetRight();
		const Vector3D Up = ICam->GetUp();
		Vector3D Forward = ICam->GetDirection();
		
		const cMat3 Orient(cMat3::RowsCtor, *(const cVec3 *)&Right, *(const cVec3 *)&Up, *(const cVec3 *)&Forward);
		const cAngles Angles = Orient.ToAngles();
		const cMat3 T = Angles.ToMat3();

		m_pViewer->SetViewPort(cRectI(0, 0, RealLx, RealLy));
		m_pViewer->SetPos(*(const cVec3 *)&Pos);
		m_pViewer->SetOrient(Angles);
		m_pViewer->SetZNearFar(ICam->GetZn(), ICam->GetZf());
		m_pViewer->SetFov(MOptions.CamArgs.GetYFovDeg());

		cInput::Acquire();
		// If in free - camera navigation mode, we should clear all input manager events.
		// So handle all events queue.
		if(cInput::IsDown(cInput::Alt) || (g_SurfEditor.FreeCamera && cInput::IsDown(cInput::Control))) {
			while(cInput::Event *pEvent = cInput::GetEvent()) {
				delete pEvent;
			}
			// Send left mouse button up event to cur tool:
			cWidgets::GetWidget(m_idTool)->OnButtonUp(cInput::LeftButton);
		}

		cWidgets::HandleInputEvents();

		cWidgets::Render();
		
		if(cInput::IsDown(cInput::W)) {
			SetMode(MODE_MOVE);
		} else if(cInput::IsDown(cInput::E)) {
			if(m_AllowRotate) {
				SetMode(MODE_ROTATE);
			}
		} else if(cInput::IsDown(cInput::R)) {
			if(m_AllowScale) {
				SetMode(MODE_SCALE);
			}
		}
		// World or Object space:
		const bool ObjectSpace = GetKeyState(VK_CAPITAL) & 0x1;
		if(ObjectSpace && m_Space != SPACE_OBJECT && m_AllowRotate) { // Object space is allowed only when is binded with rotation component.
			m_Space = SPACE_OBJECT;
			SetMode(m_CurMode);
		} else if(!ObjectSpace && m_Space != SPACE_WORLD) {
			m_Space = SPACE_WORLD;
			SetMode(m_CurMode);
		}
	} // Manipulators::OnDrawOnMapOverAll

	//-------------------------------------------------------------------------
	// Manipulators::SetMode
	//-------------------------------------------------------------------------
	void Manipulators::SetMode(MODE Tool) {
		if(m_idTool != -1) {
			cWidgets::FreeWidget(m_idTool);
			m_idTool = -1;
		}
		m_CurMode = Tool;
		if(NULL == s_pTranslate || MODE_NONE == m_CurMode) {
			return;
		}
		switch(m_CurMode) {
			case MODE_MOVE:
				m_idTool = cWidgets::AddWidget(new cMoveTool, LAYER_TOOLS);
				((cMoveTool *)cWidgets::GetWidget(m_idTool))->Bind(s_pRotate, s_pTranslate);
				if(SPACE_OBJECT == m_Space) {
					((cMoveTool *)cWidgets::GetWidget(m_idTool))->SetSpace(cMoveTool::SPACE_OBJECT);
				}
				break;
			case MODE_ROTATE:
				m_idTool = cWidgets::AddWidget(new cRotateTool, LAYER_TOOLS);
				((cRotateTool *)cWidgets::GetWidget(m_idTool))->Bind(s_pRotate, s_pTranslate);
				if(SPACE_OBJECT == m_Space) {
					((cRotateTool *)cWidgets::GetWidget(m_idTool))->SetSpace(cRotateTool::SPACE_OBJECT);
				}
				break;
			case MODE_SCALE:
				m_idTool = cWidgets::AddWidget(new cScaleTool, LAYER_TOOLS);
				((cScaleTool *)cWidgets::GetWidget(m_idTool))->Bind(s_pScale, s_pRotate, s_pTranslate);
				break;
			default:
				break;
		}
	} // Manipulators::SetMode

	// Manipulators::OnMouseHandling
	bool Manipulators::OnMouseHandling(int, int, bool &, bool &, int, int, bool) {
		return m_IsActive;
	}
} // Surface