#ifndef __CollidingCamera__
#define __CollidingCamera__

#include "..\GameExtension.h"
#include "..\surface\Surface.h"

namespace Surface {

class CollidingCamera : public GameExtension {
public:
	CollidingCamera() {
		m_fActive = false;
	}
	void Forward() { m_Args.ControlMoveForceSign = 1.0f; }
	void Backward() { m_Args.ControlMoveForceSign = - 1.0f; }
	void Right() { m_Args.ControlStrafeForceSign = 1.0f; }
	void Left() { m_Args.ControlStrafeForceSign = - 1.0f; }
	void Jump() { m_Args.ControlJumpForceSign = 1.0f; }
	void Simulate(const float tStep, const int nSteps);

	bool IsActive() { return m_fActive; }
	void Activate();
	void Free();
	void Toggle();
	
	void CalcOrientation(dVector &Right, dVector &Up, dVector &Dir) const;
	bool CameraMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void SetupCamera();
private:
	bool m_fActive;
	struct {
		cVec3 Pos;
		cVec3 Vel;
		float Azim, MouseAzim;
		float Elev, MouseElev;
		float ControlMoveForceSign;
		float ControlJumpForceSign;
		float ControlStrafeForceSign;
		cTimer Timer;
		double TimeAccum;
	} m_Args;

	int m_xCenter, m_yCenter;
	bool m_fUIWasHidden;
	bool m_fCameraWasForced;
	bool m_PrevFogArgsUseFromMap;
	float m_pFinalFogNearPlane, m_pFinalFogFarPlane, m_pCameraMovingStage;
	int m_FogMode;
	int m_PrevSkyShift;

	struct Node {
		cVec3 p, N;
	};
	struct Nodes : public cList<Node> {
		bool Gather(const cVec3 &StepCenter, const float Radius, const float FullStep);
		static bool Collides(const cVec3 &u, const cVec3 &Center, const float Radius);
		cVec3 CalcCollisionForce(const cVec3 &SubStepCenter, const float Radius, const float k);
	};
};

} // Surface

extern Surface::CollidingCamera g_CollidingCamera;

#endif // __CollidingCamera__