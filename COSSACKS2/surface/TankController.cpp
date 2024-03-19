#include "stdheader.h"
#include "TankController.h"
#include "vCamera.h"
#include "..\ua_Ability.h"
#include "..\ComplexShooter.h"
#include "..\CurrentMapOptions.h"
#include "..\GameInterface.h"
#include "CollidingCamera.h"
#include "..\Camera\Navigator.h"

Surface::TankController g_TankController;
extern int FogMode;
Matrix4D * GetActiveBone(int, char *);
extern bool EditMapMode;
void SetCentralUnit(OneObject *);
//extern bool g_fPauseOnEscape;

namespace Surface {

//-----------------------------------------------------------------------------
// TankController::OnInitAfterMapLoading
//-----------------------------------------------------------------------------
void TankController::OnInitAfterMapLoading() {
	if(IsActive()) {
		Free();
	}
	if(!EditMapMode && MOptions.LinkCameraToTank) {
		Activate();
	}
}

//-----------------------------------------------------------------------------
// TankController::OnGameEnd
//-----------------------------------------------------------------------------
void TankController::OnGameEnd() {
	if(IsActive()) {
		Free();
	}
}

//-----------------------------------------------------------------------------
// TankController::CalcAt
//-----------------------------------------------------------------------------
	dVector TankController::CalcAt() {// const {
    assert(m_pTank);

/*	bool h = GetKeyState(VK_CONTROL) < 0; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	bool Up = GetKeyState(VK_ADD) < 0; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	bool Down = GetKeyState(VK_SUBTRACT) < 0; // <<<<<<<<<<<<<<<<<<<<<<<<<<<
	if(h && Up) {
		Offset.z++;
	} else if(Up) {
		m_Args.toEye++;
	}
	if(h && Down) Offset.z--;
	else if(Down) {
		m_Args.toEye--;
	}*/

	int x = m_pTank->RealX / 16;
    int y = m_pTank->RealY / 16;
	return dVector(x, y, m_pTank->RZ + EngSettings.TankController.ZOffset);
}

//-----------------------------------------------------------------------------
// TankController::CurAzim : float()
//-----------------------------------------------------------------------------
float TankController::CurAzim() {
	return cMath::Lerp(0, cMath::TwoPi, float(GetCurAzim(m_pTank)) / float(0xffff));
}

//-----------------------------------------------------------------------------
// TankController::CurAzim : void(float)
//-----------------------------------------------------------------------------
void TankController::CurAzim(float Angle) {
	Angle = cMath::AngleNormalizeTwoPi(Angle);
	float t = float(0xffff) * Angle / cMath::TwoPi;
	int n = int(t + cMath::Sign(t) * 0.5f);
	SetCurAzim(m_pTank, n);
}

//-----------------------------------------------------------------------------
// CurTankTurretBoneAzim : float()
//-----------------------------------------------------------------------------
float TankController::CurTankTurretBoneAzim() {
	Matrix4D *pTM = GetActiveBone(m_pTank->Index, "T_Bashja");
	if(!pTM) return 0;
	Vector3D Angles = pTM->EulerXYZ();
	return cMath::AngleNormalizeTwoPi(Angles.z);
}

//-----------------------------------------------------------------------------
// TankController::CurTankGunBoneElev : float()
//-----------------------------------------------------------------------------
float TankController::CurTankGunBoneElev() {
	Matrix4D *pTM = GetActiveBone(m_pTank->Index, "Dulo");
	if(!pTM) return 0;
	Vector3D Angles = pTM->EulerXYZ();
	return cMath::AngleNormalizeTwoPi(Angles.y);
}

//-----------------------------------------------------------------------------
// TankController::SelectTank
//-----------------------------------------------------------------------------
void TankController::SelectTank() {
	pSelected Sel(MyNation);
	pImSelected ImSel(MyNation);
	Sel.Clear();
	ImSel.Clear();
	pUnit U(m_pTank);
	Sel += U;
	ImSel += U;
//	SetCentralUnit(m_pTank);
}

//-----------------------------------------------------------------------------
// Activate
//-----------------------------------------------------------------------------
void TankController::Activate() {
    const bool HideUI = false;
    const bool ForceCameraParams = true;

	if(m_fActive) return;

	if(g_CollidingCamera.IsActive()) return;

    // Searching tank's OneObject:
    m_pTank = NULL;

    for(int i = 0; i < ULIMIT; i++) {
        OneObject *pOB = Group[i];
        if(!pOB) continue;
		if(!pOB->Life) continue;
		cStr Name(pOB->newMons->Message);
		strlwr(Name.ToCharPtr());
		//if(strstr(Name.ToChar(), "tecuabr")) {
			if(pOB->ActiveAbility) {
				if(pOB->ActiveAbility->BornNNUM == MyNation) {
					m_pTank = pOB;
					break;
				}
			}
		//}
	}
	if(!m_pTank) return;

	SelectTank();

	m_pTank->NoSearchVictim = true;
	m_TankDir = m_pTank->RealDirPrecise;

	m_Args.dMouseX = m_Args.dMouseY = 0;

	m_Args.toEye = EngSettings.TankController.ToEye;
	
	m_fActive = true;

    m_xCenter = RealLx / 2;
    m_yCenter = RealLy / 2;
    SetCursorPos(m_xCenter, m_yCenter);
    while(ShowCursor(FALSE) >= 0);

    // HideUI:
    m_fUIWasHidden = /*EngSettings.CollidingCamera.*/HideUI;
    g_SurfEditor.Update();
    if(m_fUIWasHidden) {
        g_SurfEditor.HideUI = true;
    }
    g_SurfEditor.FreeCamera = false;
    g_SurfEditor.Update();
    m_fCameraWasForced = /*EngSettings.CollidingCamera.*/ForceCameraParams;

    if(/*EngSettings.CollidingCamera.*/ForceCameraParams) {
        extern float CameraMovingStage;
        m_pFinalFogNearPlane = MOptions.FogArgs.final_FogNearPlane;
        m_pFinalFogFarPlane = MOptions.FogArgs.final_FogFarPlane;
        m_pCameraMovingStage = CameraMovingStage;
        CameraMovingStage = EngSettings.CollidingCamera.CameraMovingStage;
        MOptions.FogArgs.final_FogFarPlane = EngSettings.CollidingCamera.final_FogFarPlane;
        MOptions.FogArgs.final_FogNearPlane = EngSettings.CollidingCamera.final_FogNearPlane;
    }
    
    m_FogMode = FogMode;
//    FogMode = 0;

//	g_fPauseOnEscape = true;
	m_fOnPause = Navigator::IsOnPause();
	ActivateSurfTool(NULL);
	m_TankUnitSpeed = m_pTank->UnitSpeed;
}

//-----------------------------------------------------------------------------
// TankController::Free
//-----------------------------------------------------------------------------
void TankController::Free() {
	if(!m_fActive) return;
    m_fActive = false;
    while(ShowCursor(TRUE) <= 0);
    if(m_fUIWasHidden) {
        g_SurfEditor.Update();
        g_SurfEditor.HideUI = false;
        g_SurfEditor.Update();
    }
    if(m_fCameraWasForced) {
        extern float CameraMovingStage;
        CameraMovingStage = m_pCameraMovingStage;
        MOptions.FogArgs.final_FogNearPlane = m_pFinalFogNearPlane;
        MOptions.FogArgs.final_FogFarPlane = m_pFinalFogFarPlane;
    }
    FogMode = m_FogMode;

//	g_fPauseOnEscape = true;
}

//--------------------------------------------------------------------------------------------------
// CalcCamArgs
//--------------------------------------------------------------------------------------------------
void TankController::CalcCamArgs(dVector *pPos, dVector *pDir, dVector *pRight, dVector *pUp) {
	const double dAzim = - cMath::HalfPi;
	static int d = - 125;

    // Ref:
	dVector c0 = dVector::Cartesian(1.0, - m_Args.Azim + dAzim, - (m_Args.Elev + cMath::Rad(d)));
	dVector c1 = dVector::Cartesian(1.0, - m_Args.Azim + dAzim, - (m_Args.Elev + cMath::Rad(d) + cMath::Rad(45)));
    dVector r0(c0.x, c0.z, c0.y);
    dVector r1(c1.x, c1.z, c1.y);
    dVector Ref = r1 - r0;
    Ref.Normalize();
    // Dir:
    dVector Dir = r0;
    Dir.Normalize();
    if(pDir) pDir->Copy(Dir);
    // Pos:
    dVector Pos = m_Args.At - m_Args.toEye * Dir;
    if(pPos) pPos->Copy(Pos);
    // Right:
    dVector Right = Ref.Cross(Dir);
    Right.Normalize();
    if(pRight) pRight->Copy(Right);
    // Up:
    dVector Up = Dir.Cross(Right);
    Up.Normalize();
    if(pUp) pUp->Copy(Up);
}

// TankController::PackedAngleToRad
/*float TankController::PackedAngleToRad(WORD PrecisePackedAngle) {
	return cMath::Rad(PrecisePackedAngle * 360.0 / 65535.0);
}*/

//-----------------------------------------------------------------------------
// SetupCamera
//-----------------------------------------------------------------------------
void TankController::SetupCamera() {
    const bool WASD = true;
    const bool ESDF = true;

    if(!m_fActive) return;
    
	m_Args.At = CalcAt();

	bool fForward = GetKeyState('W') < 0;
	bool fBackward = GetKeyState('S') < 0;
	bool fRight = GetKeyState('D') < 0;
	bool fLeft = GetKeyState('A') < 0;

	word CurAngle = m_TankDir;
	int dSign = 0;

	if(fForward) {
		dSign = 1;
	}
	if(fBackward) {
		dSign = -1;
	}

	if(dSign && fRight) {
		CurAngle += dSign * 16 * 256;
		m_TankDir = m_pTank->RealDirPrecise;
	}
	if(dSign && fLeft) {
		CurAngle -= dSign * 16 * 256;
		m_TankDir = m_pTank->RealDirPrecise;
	}

	m_pTank->ClearOrders();
	m_pTank->NoSearchVictim=true;

	if(dSign) {		
		if(dSign > 0) {
			m_pTank->BackMotion = false;
		} else {
			m_pTank->BackMotion = true;
		}
		int DestX = m_pTank->RealX + dSign * int(TCos[CurAngle >> 8] * 16);
		int DestY = m_pTank->RealY + dSign * int(TSin[CurAngle >> 8] * 16);

		// Calcing speed based upon lock points:
		MotionField *pMF = &MFIELDS[m_pTank->LockType];
		assert(pMF);
		int ix = DestX / 256, iy = DestY / 256, Sum = 0;
		const int d = 4; // == (dSquare - 1) / 2
		int Full = cMath::Square(2 * d + 1);
		for(int dx = - d; dx <= d; dx++) {
			for(int dy = - d; dy <= d; dy++) {
				Sum += int(!pMF->CheckPt(ix + dx, iy + dy));
			}
		}
//		if(Sum && Sum < 2 * Full / 3) {
//			Sum = Full / 5;
//		}
		m_pTank->UnitSpeed = byte(0xff & DWORD(float(m_TankUnitSpeed) * float(Sum) / float(Full) + 0.5f));
		if(m_pTank->UnitSpeed) {
			m_pTank->DestX = DestX;
			m_pTank->DestY = DestY;
		} else {
			m_pTank->DestX = -1;
			m_pTank->DestY = -1;
		}
	} else {
		m_pTank->DestX = -1;
		m_pTank->DestY = -1;
	}

    const float AzimDelta = 0.001f; // const

	if(m_Args.dMouseX) {
		CurAzim(CurAzim() + m_Args.dMouseX * AzimDelta);
		m_Args.dMouseX = 0;
	}
	if(m_Args.dMouseY) {
		int E = GetCurElev(m_pTank);
		E += m_Args.dMouseY * 100;
		E = cMath::Clamp(E, 0, 0xffff);
		SetCurElev(m_pTank, E);
		m_Args.dMouseY = 0;
	}

	m_Args.Azim = CurTankTurretBoneAzim();
	m_Args.Elev = CurTankGunBoneElev() + cMath::Rad(float(EngSettings.TankController.ElevationShift));

    dVector Pos, Dir, Right, Up;
    CalcCamArgs(&Pos, &Dir, &Right, &Up);
	// Aligning camera with height:
/*	short MinZ = SurfMap::THMapZ(Pos.x, Pos.y);
	const float HeightOffset = 100; // const
	float h = 0;
	if(g_UniversalMap.PickHeight(dVector(Pos.x, Pos.y, 10000), &h)) {
		Pos.z = cMath::Max((float)Pos.z, h + HeightOffset);
	}*/

    ICam->SetPosition(Vector3D(Pos.x, Pos.y, Pos.z));
    ICam->SetOrientation(Vector3D(Right.x, Right.y, Right.z),
        Vector3D(Up.x, Up.y, Up.z), Vector3D(Dir.x, Dir.y, Dir.z));
	if(GetKeyState(VK_LBUTTON)&0x8000)RequireShot(m_pTank,0);
	if(GetKeyState(VK_RBUTTON)&0x8000)RequireShot(m_pTank,1);
}

//-----------------------------------------------------------------------------
// Toggle
//-----------------------------------------------------------------------------
void TankController::Toggle() {
    if(IsActive()) {
        Free();
    } else {
        Activate();
    }
}

//--------------------------------------------------------------------------------------
// CameraMessage : bool(HWND, UINT, WPARAM, LPARAM)
//--------------------------------------------------------------------------------------
bool TankController::CameraMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    if(!m_fActive) return false;
	if(GetForegroundWindow() != g_hWnd) return false;

	// Pause menu && cursor:
	bool fOnPause = Navigator::IsOnPause();
	if(fOnPause && !m_fOnPause) {
		while(ShowCursor(TRUE) < 0);
		m_fOnPause = fOnPause;
	} else if(!fOnPause && m_fOnPause) {
		while(ShowCursor(FALSE) >= 0);
		SetCursorPos(m_xCenter, m_yCenter);
		m_Args.dMouseX = m_Args.dMouseY = 0;
		SelectTank();
		m_fOnPause = fOnPause;
	}

	if(m_fOnPause) return false;

//	if(WM_KEYDOWN == Msg || WM_KEYUP == Msg || WM_CHAR == Msg) {
//		if(VK_F12 == wParam) { // Main menu...
//			while(ShowCursor(TRUE) < 0);
//		}
//	}

	static POINTS pts;
    int dx, dy;
    switch(Msg) {
	case WM_SETCURSOR:
		while(ShowCursor(FALSE) >= 0);
		return true;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
		//RequireShot(m_pTank);
		return true;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
    case WM_MOUSEMOVE:
        pts = MAKEPOINTS(lParam);
        dx = pts.x - m_xCenter;
        dy = pts.y - m_yCenter;
        if(dx || dy) {
            SetCursorPos(m_xCenter, m_yCenter);
			m_Args.dMouseX += dx;
			m_Args.dMouseY -= dy;
		}
        return true;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        return true;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
		if(wParam == 'W' || wParam == 'A' || wParam == 'S' || wParam == 'D') return true;
        if(VK_ESCAPE == wParam || VK_HOME == wParam) {
			if(EditMapMode) {
				Free();
				return true;
			}
		}
		if(EditMapMode) return true; // in editor we eat all keys....
        return false;
	default:
		return false;
    }
}

OneObject* TankController::GetTank(){
    return m_pTank;
}
}; // Surface