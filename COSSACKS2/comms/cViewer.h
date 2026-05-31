#pragma once

//*****************************************************************************
// Viewer
//*****************************************************************************

class cViewer : public cWidget {
public:
	enum eUpAxis { UpAxisY, UpAxisZ };
	
	struct CtorArgs {
		CtorArgs(const eUpAxis UpAxis = UpAxisY) { SetDefaults(UpAxis); }
		void SetDefaults(const eUpAxis);
		
		cVec3 Pos;
		cAngles Orient;
		float Fov;
		float zNear;
		float zFar;
		eUpAxis UpAxis;
		bool IsRightHanded;
		bool IsInfinite;
	};

	cViewer(const CtorArgs & = CtorArgs());

protected:
	cVec3 m_Pos;
	cAngles m_Orient;
	cRectI m_ViewPort;
	eUpAxis m_UpAxis;
	
	bool m_IsRightHanded;
	bool m_IsInfinite;
	float m_Fov;
	float m_zNear;
	float m_zFar;
	
	// Cached values:
	cVec3 m_Forward;
	cVec3 m_Right;
	cVec3 m_Up;
	float m_Aspect;
	
	cMat4 m_World2View;
	cMat4 m_View2World;
	
	cMat4 m_View2Proj;
	cMat4 m_Proj2View;
	
	cMat4 m_Proj2Screen;
	cMat4 m_Screen2Proj;
	
	cMat4 m_World2Proj;
	cMat4 m_Proj2World;
	
	cMat4 m_World2Screen;
	cMat4 m_Screen2World;
	
	cMat4 m_View2Screen;
	cMat4 m_Screen2View;
	
	cPlane m_Planes[6];
	
	void Update();

public:
	const eUpAxis GetUpAxis() const { return m_UpAxis; }
	void SetUpAxis(const eUpAxis UpAxis);
	
	const cVec3 & GetPos() const { return m_Pos; }
	void SetPos(const cVec3 &Pos) { m_Pos = Pos; Update(); }
	void MovePos(const cVec3 &Delta) { m_Pos += Delta; Update(); }
	
	const cAngles & GetOrient() const { return m_Orient; }
	void SetOrient(const cAngles &Orient) { m_Orient = Orient; Update(); }
	void MoveOrient(const cAngles &Delta) { m_Orient += Delta; Update(); }

	const cRectI & GetViewPort() const { return m_ViewPort; }
	void SetViewPort(const cRectI &ViewPort) { m_ViewPort = ViewPort; Update(); }
	float GetAspect() const { return m_Aspect; }
	
	float GetFov() const { return m_Fov; }
	void SetFov(const float Fov) { m_Fov = Fov; Update(); }
	
	float GetZNear() const { return m_zNear; }
	float GetZFar() const { return m_zFar; }
	void SetZNearFar(const float zNear, const float zFar) { m_zNear = zNear; m_zFar = zFar; Update(); }

	const cVec3 & GetForward() const { return m_Forward; }
	const cVec3 & GetRight() const { return m_Right; }
	const cVec3 & GetUp() const { return m_Up; }
	
	bool IsRightHanded() const { return m_IsRightHanded; }
	void SetRightHanded(bool Flag) { m_IsRightHanded = Flag; Update(); }

	bool IsInfinite() const { return m_IsInfinite; }
	void SetInfinite(const bool IsInfinite) { m_IsInfinite = IsInfinite; Update(); }
	
	const cMat4 & GetViewMatrix() const { return m_World2View; }
	const cMat4 & GetViewMatrixInverse() const { return m_View2World; }

	const cMat4 & GetProjectionMatrix() const { return m_View2Proj; }
	const cMat4 & GetProjectionMatrixInverse() const { return m_Proj2View; }

	const cMat4 & GetScreenMatrix() const { return m_Proj2Screen; }
	const cMat4 & GetScreenMatrixInverse() const { return m_Screen2Proj; }

	const cMat4 & GetViewProjectionMatrix() const { return m_World2Proj; }
	const cMat4 & GetViewProjectionMatrixInverse() const { return m_Proj2World; }

	const cMat4 & GetViewProjectionScreenMatrix() const { return m_World2Screen; }
	const cMat4 & GetViewProjectionScreenMatrixInverse() const { return m_Screen2World; }

	const cMat4 & GetProjectionScreenMatrix() const { return m_View2Screen; }
	const cMat4 & GetProjectionScreenMatrixInverse() const { return m_Screen2View; }
	
	const cSeg GetPickRay(const int xScreen, const int yScreen) const;
	
	bool CullPoint(const cVec3 &p) const;
	bool CullBounds(const cBounds &b) const;
};

//*****************************************************************************
// FreeViewer
//*****************************************************************************

class cFreeViewer : public cViewer {
private:
	cVec3 m_DefLookAt;
	cAngles m_DefOrient;
	float m_DefToEye;
	
	cVec3 m_LookAt;
	float m_ToEye;

	void Update();

	struct Mode {
		static const int None = 0;
		static const int Track = 1;
		static const int Tumble = 2;
		static const int Dolly = 3;
	};
	int m_Mode;
	bool m_DrawTargetUntilAlt;
public:
	struct CtorArgs : cViewer::CtorArgs {
		CtorArgs(const eUpAxis UpAxis = UpAxisY) { SetDefaults(UpAxis); }
		void SetDefaults(const eUpAxis);
		cVec3 DefLookAt;
		cAngles DefOrient;
		float DefToEye;
	};
	cFreeViewer(const CtorArgs & = CtorArgs());
	void Home();
	void Track(const int Dx, const int Dy, const float Step = 0.001f);
	void Tumble(const int Dx, const int Dy, const float dAngle = 0.3f);
	void Dolly(const int Dx, const int Dy, const float Step = 0.2f, const float MinDistance = 0.0f);
	void Roll(const int Dx, const int Dy, const float dAngle = 0.3f);
	void Aim(const int Delta, const float Speed = 1.2f, const float MinDistance = 0.0f);

	bool OnButtonDown(const int Code);
	bool OnButtonUp(const int Code);
	bool OnMouseMove(const int DeltaX, const int DeltaY);
	bool OnMouseWheel(const int Delta);
	
	void OnRender();
};