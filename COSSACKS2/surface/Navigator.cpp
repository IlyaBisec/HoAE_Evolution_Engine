#include "stdheader.h"
#include "vCamera.h"
#include "..\ua_Ability.h"
#include "..\ComplexShooter.h"
#include "..\CurrentMapOptions.h"
#include "..\GameInterface.h"
#include "Navigator.h"
#include "comms\comms.h"
#include "SelExt.h"
#include "..\cvi_MainMenu.h"
#include "CmdExt.h"
#include "CollidingCamera.h"
#include "TankController.h"

extern float fMapX;
extern float fMapY;
OneObject * GetCentralUnit();
extern bool LockPause;
extern bool NOPAUSE;
extern bool MakeMenu;
extern int MenuType;
extern byte PauseMode;
extern bool g_WASDKeysAreBlocked;
extern bool g_ESDFKeysAreBlocked;
extern cvi_MainMenu v_MainMenu;
extern bool KeysAreFocusedOnInputBox;

Surface::Navigator g_Navigator;

namespace Surface {

//-----------------------------------------------------------------------------
// TendToArgs::Lift
//-----------------------------------------------------------------------------
void Navigator::TendToArgs::Lift(const float Sign) {
	m_MovingStage += Sign * EngSettings.RTSModeCamera.LiftSpeed;
	m_MovingStage = cMath::Clamp(m_MovingStage, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// TendToArgs::DollyNow
//-----------------------------------------------------------------------------
void Navigator::TendToArgs::DollyNow(const float Sign) {
	m_fWasDollied = true;
	DollySpring(true);
	if(Sign < 0) {
		m_toEyeEx = EngSettings.RTSModeCamera.DollyMin;
	} else {
		m_toEyeEx = EngSettings.RTSModeCamera.DollyMax;
	}
}

//-----------------------------------------------------------------------------
// TendToArgs::Dolly
//-----------------------------------------------------------------------------
void Navigator::TendToArgs::Dolly(const float Sign) {
	DollySpring(true);
	m_toEyeEx += Sign * EngSettings.RTSModeCamera.DollySpeed;
	m_toEyeEx = cMath::Clamp(m_toEyeEx, (float)EngSettings.RTSModeCamera.DollyMin, (float)EngSettings.RTSModeCamera.DollyMax);
}

//-----------------------------------------------------------------------------
// TendToArgs::Rotate
//-----------------------------------------------------------------------------
void Navigator::TendToArgs::Rotate(const float Sign) {
	RotationSpring(true);
	m_Azim += Sign * EngSettings.RTSModeCamera.RotationSpeed;
	m_Azim = cMath::AngleNormalizeTwoPi(m_Azim);
}

//-----------------------------------------------------------------------------
// Navigator::TendToArgs::Update
//-----------------------------------------------------------------------------
void Navigator::TendToArgs::Update() {
	if(g_CollidingCamera.IsActive() || g_TankController.IsActive()) {
		return;
	}

	// This is not correct, because we should check if PageUp / PageDown
	// are really assigned to lift!

	bool PageUp = GetKeyState(VK_PRIOR) < 0;
	bool PageDown = GetKeyState(VK_NEXT) < 0;
	if(PageUp && !PageDown) {
		OnPageUpPageDown(-1.0f);
	}
	if(PageDown && !PageUp) {
		OnPageUpPageDown(1.0f);
	}

	extern float CameraMovingStage;
	if(g_SurfEditor.FreeCamera) {
		if(!g_Navigator.IsPlayingTrack()) {
			CameraMovingStage = m_MovingStage;
		}
		return;
	}

//	m_fWasDollied = false;
//	if(EngSettings.RTSModeCamera.OnMouseWheel == EngineSettings::RTSModeCameraArgs::DOLLY) {
//		m_fWasDollied = true;
//	}
	m_fWasDollied = true;

	// Time:
	static DWORD msPrev = 0;
	DWORD msCur = GetTickCount();
	const float msDelta = 40.0f;
	float k = float(msCur - msPrev) / msDelta;
	msPrev = msCur;
	
	// Keys:
	if(GetKeyState(VK_HOME) < 0) {
		Defaults();
	}

	bool Ins = GetKeyState(VK_INSERT) < 0;
	bool Del = GetKeyState(VK_DELETE) < 0 && !KeysAreFocusedOnInputBox;
	if(Ins && !Del) {
		OnInsertDelete(-1.0f);
	}
	if(Del && !Ins) {
		OnInsertDelete(1.0f);
	}

	if(!m_fWasDollied && m_DollySpringIsActive) {
		m_toEyeEx = 0.0f;
	}

	// Rotation:
	if(m_RotationSpringIsActive) {
		float dAngle = cMath::AngleDeltaRad(m_Azim, g_Navigator.m_Args.Azim);
		float t = dAngle * EngSettings.RTSModeCamera.RotationSpringConstant;
		float d = dAngle * EngSettings.RTSModeCamera.RotationSpringConstant;
		float kd = k * d;
		kd = cMath::Sign(kd) * cMath::Clamp(cMath::Abs(kd), 0.0f, cMath::Abs(dAngle));
		g_Navigator.m_Args.Azim += kd;
	} else { // Tracing:
		m_Azim = cMath::AngleNormalizeTwoPi(g_Navigator.m_Args.Azim);
		g_Navigator.m_Args.Azim = m_Azim;
	}

	// Dolly:
	if(m_DollySpringIsActive) {
		float dToEyeEx = m_toEyeEx - g_Navigator.m_Args.toEyeEx;
		float t = k * dToEyeEx * EngSettings.RTSModeCamera.DollySpringConstant;
		float d = cMath::Sign(t) * cMath::Clamp(cMath::Abs(t), 0.0f, cMath::Abs(dToEyeEx));
		g_Navigator.m_Args.toEyeEx += d;
	} else { // Tracing:
		m_toEyeEx = g_Navigator.m_Args.toEyeEx;
		m_toEyeEx = cMath::Clamp(m_toEyeEx, (float)EngSettings.RTSModeCamera.DollyMin, (float)EngSettings.RTSModeCamera.DollyMax);
		g_Navigator.m_Args.toEyeEx = m_toEyeEx;
	}

	// Lift:
	if(m_LiftSpringIsActive) {
		float dLift = m_MovingStage - CameraMovingStage;
		float t = k * dLift * EngSettings.RTSModeCamera.LiftSpringConstant;
		float d = cMath::Sign(t) * cMath::Clamp(cMath::Abs(t), 0.0f, cMath::Abs(dLift));
		CameraMovingStage += d;
	} else { // Tracing:
		m_MovingStage = cMath::Clamp01(CameraMovingStage);
		CameraMovingStage = m_MovingStage;
	}
}

//-----------------------------------------------------------------------------
// TendToArgs::IsTendingToDefaults
//-----------------------------------------------------------------------------
bool Navigator::TendToArgs::IsTendingToDefaults() {
	return TendToArgs_DefMovingStage == m_MovingStage &&
		TendToArgs_DeftoEyeEx == m_toEyeEx && TendToArgs_DefAzim == m_Azim;
}

//-----------------------------------------------------------------------------
// Navigator::TendToArgs::Defaults
//-----------------------------------------------------------------------------
void Navigator::TendToArgs::Defaults() {
	SetSprings(true, true, true);
	
	m_MovingStage = TendToArgs_DefMovingStage;
	m_toEyeEx = TendToArgs_DeftoEyeEx;
	m_Azim = TendToArgs_DefAzim;
}

void Navigator::TendToArgs::Control(int index, float d) {
	switch(index) {
		case EngineSettings::RTSModeCameraArgs::ROTATE:
			Rotate(d);
			break;
		case EngineSettings::RTSModeCameraArgs::DOLLY:
//			if(EngSettings.RTSModeCamera.OnMouseWheel == EngineSettings::RTSModeCameraArgs::DOLLY) {
				Dolly(d);
//			} else {
//				DollyNow(d);
//			}
			break;
		case EngineSettings::RTSModeCameraArgs::LIFT:
			Lift(d);
			break;
		default:
			assert(0);
	}
}

void Navigator::TendToArgs::OnMouseWheel(int Delta) {
	extern int HeightEditMode;
	if(HeightEditMode) return;
	Control(EngSettings.RTSModeCamera.OnMouseWheel, float(Delta) / 120.0f);
}

void Navigator::TendToArgs::OnInsertDelete(const float Sign) {
	Control(EngSettings.RTSModeCamera.OnInsertDelete, Sign);
}

void Navigator::TendToArgs::OnPageUpPageDown(const float Sign) {
	Control(EngSettings.RTSModeCamera.OnPageUpPageDown, Sign);
}


void AddLine(SurfMesh &, const SurfVertex &, const SurfVertex &, DWORD, int);

//-----------------------------------------------------------------------------
// Navigator::LinkToUnitToggle
//-----------------------------------------------------------------------------
void Navigator::LinkToUnitToggle() {
	if(!m_fLinkToHero) {
		Toggle(true);
		LinkToHeroToggle();
	} else {
		Home();
	}
}

//-----------------------------------------------------------------------------
// IsOnPause
//-----------------------------------------------------------------------------
bool Navigator::IsOnPause() {
	return !NOPAUSE || LockPause || PauseMode /*|| (v_MainMenu.GameMode && !v_MainMenu.Exit)*/;
}

//-----------------------------------------------------------------------------
// OnGameEnd
//-----------------------------------------------------------------------------
void Navigator::OnGameEnd() {
	Home();
}

//----------------------------------------------------------------------------------------
// CalcArgsFromICam
//----------------------------------------------------------------------------------------
void Navigator::CalcArgsFromICam(double &Azim, double &Elev, double &toEye, dVector &At) {
	Vector3D pos = ICam->GetPosition();
	Vector3D dir = ICam->GetDirection();
	dVector Dir(dir.x, dir.z, dir.y);
	double r, Phi, Psi;
	Dir.Spherical(r, Azim, Elev);
	toEye = 0;
	At.Set(pos.x, pos.y, pos.z);
	SurfRef Ref;
	double s;
	cMath::Swap(Dir.y, Dir.z);
	if(g_UniversalMap.Pick(Ref, At, Dir, &s)) {
		At += s * Dir;
		toEye = s;
	}
}

//-----------------------------------------------------------------------------
// CopyToKey
//-----------------------------------------------------------------------------
void Navigator::CopyToKey(CameraSplineKey *pKey) {
	assert(pKey);
	assert(g_SurfEditor.FreeCamera);

	pKey->xAt = float(m_Args.At.x);
	pKey->yAt = float(m_Args.At.y);
	pKey->zAt = float(m_Args.At.z);
	pKey->Azim = float(m_Args.Azim);
	pKey->Elev = float(m_Args.Elev);
	pKey->toEye = float(m_Args.toEye);
	pKey->Roll = float(m_Args.Roll);
}

//-----------------------------------------------------------------------------
// CopyFromKey
//-----------------------------------------------------------------------------
void Navigator::CopyFromKey(const CameraSplineKey *pKey) {
	assert(pKey);
	assert(g_SurfEditor.FreeCamera);

	m_Args.Defaults();
	m_Args.At.Set(pKey->xAt, pKey->yAt, pKey->zAt);
	m_Args.Azim = pKey->Azim;
	m_Args.Elev = pKey->Elev;
	m_Args.Roll = pKey->Roll;
	m_Args.toEye = pKey->toEye;
}

//-----------------------------------------------------------------------------
// Toggle
//-----------------------------------------------------------------------------
void Navigator::Toggle(bool FreeCamera) {
	Vector3D pos, dir;
	dVector Dir;
	if(FreeCamera) {
		CalcArgsFromICam(m_Args.Azim, m_Args.Elev, m_Args.toEye, m_Args.At);
		m_fForceDrawAt = false;
		g_SurfEditor.FreeCamera = g_SurfEditor.prevFreeCamera = true;
		g_SurfEditor.ForceZNear = true;
		g_SurfEditor.Update();
	} else {
		Home();
	}
    m_fDisableHeroManagement = false;
}

//-----------------------------------------------------------------------------
// FreeTrack
//-----------------------------------------------------------------------------
void Navigator::FreeTrack() {
	if(m_Track.IsActive) {
		m_Track.RestoreCameraParams();
		// UI:
		if(m_Track.HideUI) {
			GSets.CGame.ViewMask = 0xff;
		}
		m_Track.IsActive = false;
		if(m_Track.IsCapturing) {
			IRS->TimeOverrideDisable();
		}
	}
}

//-----------------------------------------------------------------------------
// Home
//-----------------------------------------------------------------------------
void Navigator::Home() {
	void SetCameraPosEx(float, float);
	if(m_fLinkToHero) {
		LinkToHeroToggle();
		m_Args.Elev = cMath::Pi;
	}
	if(m_Track.IsActive) {
		RewindTrackToEnd();
	}
	// Trying to match in game camera to free:
	if(m_Args.At != dVector::Origin) {
		dVector Dir;
		CalcCamArgs(NULL, &Dir, NULL, NULL);
		dPlane Z(dVector::zAxis, 0);
		dVector X;
		if(Z.RayIntersection(m_Args.At, Dir, NULL, &X)) {
			float x = (float(X.x) - 1024/32 * 16) / 32.0f;
			float y = (float(X.y) - 768/16 * 16) / 32.0f;
			SetCameraPosEx(x, y);
		}
	}
	m_Args.Defaults();
	g_SurfEditor.FreeCamera = false;
	g_SurfEditor.ForceZNear = false;
	FreeTrack();
	m_Mode = 0;
	m_TendToArgs.Defaults();
}

//-----------------------------------------------------------------------------
// LinkToHero
//-----------------------------------------------------------------------------
void Navigator::LinkToHeroToggle() {
	extern float CameraMovingStage;
	if(m_fLinkToHero) {
		if(m_fCameraWasForced) {
			CameraMovingStage = m_pCameraMovingStage;
			EngSettings.final_FogNearPlane = m_pFinalFogNearPlane;
			EngSettings.final_FogFarPlane = m_pFinalFogFarPlane;
		}
		m_fLinkToHero = false;
		g_WASDKeysAreBlocked = false;
		g_ESDFKeysAreBlocked = false;
		if(m_fUnitIsGround) { // restoring MinRotator for ground units:
			m_pHero->newMons->MinRotator = m_pMinRotator;
		}
		return;
	}
	
	m_pHero = NULL;

	pImSelected ImSel(MyNation);
	if(ImSel._getamount()) {
		RawUnit ru = ImSel._getunit(0);
		m_pHero = ru.object();
	}


/*    for(int i = 0; i < ULIMIT; i++) {
		OneObject *pOB = Group[i];
		if(!pOB) continue;
		if(!pOB->Life) continue;
		HeroVariableStorage * GetHeroVariableStorage(OneObject *);
		if(GetHeroVariableStorage(pOB)) {
			m_pHero = pOB;
		}
	}*/
	if(m_pHero) {
		m_fLinkToHero = true;
		m_pHero->NoSearchVictim = true;
		m_HeroDir = m_pHero->RealDirPrecise;
		m_Args.Azim = cMath::TwoPi - PackedAngleToRad(m_HeroDir) + cMath::HalfPi;
		if(EngSettings.ThirdPersonCamera.WASD) {
			g_WASDKeysAreBlocked = true;
		} else if(EngSettings.ThirdPersonCamera.ESDF) {
			g_ESDFKeysAreBlocked = true;
		}
		m_Args.toEye = EngSettings.ThirdPersonCamera.toEye;
		m_Args.ExtraElev = 0;
		m_Args.ExtraAzim = 0;
		m_fAddTurning = false;

		// Store camera args:
		m_fCameraWasForced = false;
		if(EngSettings.ThirdPersonCamera.ForceCameraParams) {
			m_fCameraWasForced = true;
			m_pFinalFogNearPlane = EngSettings.final_FogNearPlane;
			m_pFinalFogFarPlane = EngSettings.final_FogFarPlane;
			m_pCameraMovingStage = CameraMovingStage;
			CameraMovingStage = EngSettings.ThirdPersonCamera.CameraMovingStage;
			EngSettings.final_FogNearPlane = EngSettings.ThirdPersonCamera.final_FogNearPlane;
			EngSettings.final_FogFarPlane = EngSettings.ThirdPersonCamera.final_FogFarPlane;
		}

		// Limiting unit rotaion speed (if ground):
		m_pMinRotator = m_pHero->newMons->MinRotator;
		m_fUnitIsGround = m_pHero->LockType < 3;
		if(m_fUnitIsGround) { // unit is ground
			void MarkObjectLikeModified(OneObject *);
			MarkObjectLikeModified(m_pHero);
			m_pHero->newMons->MinRotator = EngSettings.ThirdPersonCamera.GroundUnitMinRotator;
		}

		// Getting unit cylinder:
		int R, H;
		m_pHero->GetCylinder(R, m_UnitCylinderHeight, H);
	}
}

//-----------------------------------------------------------------------------
// Navigator::ControlHero
//-----------------------------------------------------------------------------
void Navigator::ControlHero() {
	if(!m_fLinkToHero) return;
    
	SetAtToHero();

    	if(m_fDisableHeroManagement)return;

	bool fForward = (GetKeyState(VK_UP) < 0 || (GetKeyState('E') < 0 && EngSettings.ThirdPersonCamera.ESDF) ||
		(GetKeyState('W') < 0 && EngSettings.ThirdPersonCamera.WASD));
	bool fBackward = (GetKeyState(VK_DOWN) < 0 || (GetKeyState('D') < 0 && EngSettings.ThirdPersonCamera.ESDF) ||
		(GetKeyState('S') < 0 && EngSettings.ThirdPersonCamera.WASD));
	bool fRight = (GetKeyState(VK_RIGHT) < 0 || (GetKeyState('F') < 0 && EngSettings.ThirdPersonCamera.ESDF) ||
		(GetKeyState('D') < 0 && EngSettings.ThirdPersonCamera.WASD));
	bool fLeft = (GetKeyState(VK_LEFT) < 0 || (GetKeyState('S') < 0 && EngSettings.ThirdPersonCamera.ESDF) ||
		(GetKeyState('A') < 0 && EngSettings.ThirdPersonCamera.WASD));
	
	if(fRight || fLeft) m_fAddTurning = true;

	word CurAngle = m_HeroDir;

/*	void RotUnit(OneObject *, char, byte, byte);
	if(fRight) {
		RotUnit(m_pHero, byte(int(m_pHero->RealDirPrecise) + 1), 0, 128 + 16);
		m_HeroDir = m_pHero->RealDirPrecise;
	}

	if(fLeft) {
		RotUnit(m_pHero, byte(int(m_pHero->RealDirPrecise) - 1), 0, 128 + 16);
		m_HeroDir = m_pHero->RealDirPrecise;
	}*/

/*	if(fRight) {
		m_HeroDir += 256;
	}
	if(fLeft) {
		m_HeroDir -= 256;
	}

	m_pHero->RealDirPrecise = m_HeroDir;
	m_pHero->RealDir = m_HeroDir / 256;*/


	int dSign = 0;

	if(fForward) {
		dSign = 1;
	}
	if(fBackward) {
		dSign = -1;
	}

	if(dSign && fRight) {
		CurAngle += dSign * 32 * 256;
		m_HeroDir = m_pHero->RealDirPrecise;
	}
	if(dSign && fLeft) {
		CurAngle -= dSign * 32 * 256;
		m_HeroDir = m_pHero->RealDirPrecise;
	}
	
	if(dSign) {
		if(dSign > 0) {
			m_pHero->BackMotion = false;
		} else {
			m_pHero->BackMotion = true;
		}
		int lDestX = m_pHero->RealX + dSign * int(TCos[CurAngle >> 8] * 4);
		int lDestY = m_pHero->RealX + dSign * int(TCos[CurAngle >> 8] * 4);
		int DestX = m_pHero->RealX + dSign * int(TCos[CurAngle >> 8] * 32);
		int DestY = m_pHero->RealY + dSign * int(TSin[CurAngle >> 8] * 32);

		// Calcing speed based upon lock points:
		MotionField *pMF = &MFIELDS[m_pHero->LockType];
		assert(pMF);
		int ix = lDestX / 256, iy = lDestY / 256, Sum = 0;
//		if(!pMF->CheckPt(ix, iy)) {
			m_pHero->DestX = DestX;
			m_pHero->DestY = DestY;
/*		} else {
			m_pHero->DestX = -1;
			m_pHero->DestY = -1;
		}*/
	} else {
		m_pHero->DestX = -1;
		m_pHero->DestY = -1;
	}
/*
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
	m_Args.Elev = CurTankGunBoneElev();

    dVector Pos, Dir, Right, Up;
    CalcCamArgs(&Pos, &Dir, &Right, &Up);
	// Aligning camera with height:
/*	short MinZ = SurfMap::THMapZ(Pos.x, Pos.y);
	const float HeightOffset = 100; // const
	float h = 0;
	if(g_UniversalMap.PickHeight(dVector(Pos.x, Pos.y, 10000), &h)) {
		Pos.z = cMath::Max((float)Pos.z, h + HeightOffset);
	}*/

/*    ICam->SetPosition(Vector3D(Pos.x, Pos.y, Pos.z));
    ICam->SetOrientation(Vector3D(Right.x, Right.y, Right.z),
        Vector3D(Up.x, Up.y, Up.z), Vector3D(Dir.x, Dir.y, Dir.z));*/

	static bool fSpaceWasPressed = false;
	bool fSpace = GetKeyState(VK_SPACE) < 0;
	if(fSpace && !fSpaceWasPressed) {
		fSpaceWasPressed = true;
		double UnitAngle = cMath::TwoPi - PackedAngleToRad(m_pHero->RealDirPrecise) + cMath::HalfPi;
		dVector Dir = dVector::Cartesian(1.0, UnitAngle, cMath::HalfPi);
		cMath::Swap(Dir.y, Dir.z);
		m_pHero->ClearOrders();
		int MaxAttackRadius = m_pHero->GetMaxAttackRadius();
		double R = 0.8 * MaxAttackRadius;
		dVector Cur(m_Args.At.x, m_Args.At.y, 0);
		dVector ToAttack = Cur + R * Dir;
		m_pHero->NewAttackPoint((int)ToAttack.x, (int)ToAttack.y, 128 + 16, 0, 1);
	}
	if(!fSpace) {
		fSpaceWasPressed = false;
	}
}

//-----------------------------------------------------------------------------
// PackedAngleToRad
//-----------------------------------------------------------------------------
double Navigator::PackedAngleToRad(word PrecPackedAngle) {
	return double(PrecPackedAngle) / double(0xffff) * cMath::TwoPi;
}

//---------------------------------------------------------------------------------
// RadToPackedAngle
//---------------------------------------------------------------------------------
word Navigator::RadToPackedAngle(double RadAngle) {
	return word(cMath::AngleNormalizeTwoPi(RadAngle) / cMath::TwoPi * double(0xffff));
}

//-----------------------------------------------------------------------------
// SetAtToHero
//-----------------------------------------------------------------------------
void Navigator::SetAtToHero() {
	if(m_fLinkToHero) {
		int x = m_pHero->RealX / 16;
		int y = m_pHero->RealY / 16;
		int zOffset = m_pHero->LockType >= 3 ? EngSettings.ThirdPersonCamera.FlyingUnitZOffset : EngSettings.ThirdPersonCamera.GroundUnitZOffset;
		m_Args.At.Set(x, y, m_pHero->RZ + zOffset + m_pHero->OverEarth + m_UnitCylinderHeight);

		// Additional turning:
		if(m_fAddTurning) {
			m_Args.ExtraAzim -= m_Args.ExtraAzim / 20.0;
			m_Args.ExtraElev -= m_Args.ExtraElev / 20.0;
		}
		
		m_Args.Azim = cMath::TwoPi - PackedAngleToRad(m_pHero->RealDirPrecise/*m_HeroDir*/) + cMath::HalfPi + m_Args.ExtraAzim;
		m_Args.Elev = cMath::TwoPi - PackedAngleToRad(m_pHero->ForceX) + cMath::HalfPi + m_Args.ExtraElev + cMath::Rad(EngSettings.ThirdPersonCamera.ElevOffset);
		m_Args.Elev = cMath::AngleNormalizeTwoPi(m_Args.Elev);
		m_Args.Roll = PackedAngleToRad(m_pHero->ForceY);// + cMath::HalfPi;
	}
}

//----------------------------------------------------------------------------------------------
// CalcCamArgs
//----------------------------------------------------------------------------------------------
void Navigator::CalcCamArgs(dVector *pPos, dVector *pDir, dVector *pRight, dVector *pUp) {
	ControlHero();

	// Ref:
	dVector c0 = dVector::Cartesian(1.0, m_Args.Azim, m_Args.Elev);
	dVector c1 = dVector::Cartesian(1.0, m_Args.Azim - m_Args.Roll, m_Args.Elev - cMath::Rad(45));
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

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	cVec3 At(float(m_Args.At.x), float(m_Args.At.z), float(m_Args.At.y));
	cMat4 M = cMat4::OfLookAt(At, m_Args.Azim, m_Args.Elev, m_Args.toEye, 0, cMat4::CS_RIGHTHANDED);
	cVec3 X = M.GetAxisX();
	cVec3 Y = M.GetAxisY();
	cVec3 Z = M.GetAxisZ();
	cVec3 t = M.GetTransl();
}

//-----------------------------------------------------------------------------
// SetupCamera
//-----------------------------------------------------------------------------
void Navigator::SetupCamera() {
	dVector Pos, Dir, Right, Up;

	CalcCamArgs(&Pos, &Dir, &Right, &Up);
	// ICam:
	ICam->SetPosition(Vector3D((float)Pos.x, (float)Pos.y, (float)Pos.z));
	ICam->SetOrientation(Vector3D(-(float)Right.x, -(float)Right.y, -(float)Right.z),
		Vector3D((float)Up.x, (float)Up.y, (float)Up.z),
		Vector3D((float)Dir.x, (float)Dir.y, (float)Dir.z));
}

//-----------------------------------------------------------------------------------------------
// OnMouseHandling
//-----------------------------------------------------------------------------------------------
bool Navigator::OnMouseHandling(int xScreen, int yScreen, bool &LMB, bool &RMB, int, int, bool) {
	if(!g_fWheelIsBlocked) {
		extern short WheelDelta;
		if(WheelDelta) {
			m_TendToArgs.OnMouseWheel(WheelDelta);
			WheelDelta = 0;
		}
	}

	static int xPrev = 0, yPrev = 0;

	extern bool g_fMMBIsDown;
	bool fTrack = m_fLinkToHero ? false : g_fMMBIsDown;
	bool fTumble = (LMB && GetKeyState(VK_MENU) < 0) || (m_fLinkToHero && g_fMMBIsDown);
	bool fDolly = RMB && GetKeyState(VK_MENU) < 0;
	bool fRoll = fTumble && fDolly;
	if(fRoll) {
		fTumble = false;
		fDolly = false;
	}

	if(!fTrack && !fTumble && !fDolly && !fRoll) {
		m_Mode = 0;
		return false;
	}

	if(fTrack || fTumble || m_Mode) {
		m_fAddTurning = false;
	}

	if((M_TRACK == m_Mode && !fTrack) || (M_TUMBLE == m_Mode && !fTumble) ||
		(M_DOLLY == m_Mode && !fDolly) || (M_ROLL == m_Mode && !fRoll)) {
		m_Mode = 0;
	}

	if(m_Mode) {
		int dx = xPrev - xScreen;
		int dy = yPrev - yScreen;
		if(dx || dy) {
			if(g_SurfEditor.FreeCamera) {
				switch(m_Mode) {
                    case M_TRACK:
						Adv_Track(xPrev, yPrev, xScreen, yScreen);
						break;
					case M_TUMBLE:
						Adv_Tumble(dx, dy);
						break;
					case M_DOLLY:
						Adv_Dolly(dx, dy);
						break;
					case M_ROLL:
						Adv_Roll(dx, dy);
						break;
				}
			} else {
				switch(m_Mode) {
					case M_TRACK:
						InGame_Track(xPrev, yPrev, xScreen, yScreen);
						break;
					case M_TUMBLE:
						InGame_Tumble(xPrev, yPrev, xScreen, yScreen);
						break;
					case M_DOLLY:
						InGame_Dolly(dx, dy);
						break;
				}
			}
		}
	} else {
		if(fTrack) {
			m_Mode = M_TRACK;
			SurfRef Ref;
			if(g_SurfEditor.FreeCamera) {
				dVector Pos, Dir;
				CalcCamArgs(&Pos, &Dir, NULL, NULL);
				if(g_UniversalMap.Pick(Ref, xScreen, yScreen)) {
					const SurfVertex &r = Ref.pOrig->VERTS[Ref.idVerts[0]];
					m_TrackPlane.SetFromNormalAndPoint(- Dir, dVector(r.x, r.y, r.z));
				} else {
					const double dInSens = 2000;
					double s = m_Args.toEye;
					cMath::Clamp(s, dInSens, cMath::DoubleMaxValue);
					dVector Pt = Pos + s * Dir;
					m_TrackPlane.SetFromNormalAndPoint(- Dir, Pt);
				}
			} else {
				int z = 50;
				if(g_UniversalMap.Pick(Ref, xScreen, yScreen)) {
					const SurfVertex &r = Ref.pOrig->VERTS[Ref.idVerts[0]];
					z = r.z;
				}
				m_TrackPlane = dPlane(dVector::zAxis, z);
//				m_TrackPlane = cPlane::FromPointNormal(z, dVector::zAxis);//.Set(dVector::zAxis, z);
			}
		} else if(fTumble) {
			m_Mode = M_TUMBLE;
		}
		else if(fDolly) {
			m_Mode = M_DOLLY;
		} else if(fRoll) {
			m_Mode = M_ROLL;
		}
	}
	xPrev = xScreen;
	yPrev = yScreen;

	return true;
}

//-----------------------------------------------------------------------------
// TrackHelper
//-----------------------------------------------------------------------------
void Navigator::TrackHelper(const dPlane &Plane, int xPrev, int yPrev, int xCur, int yCur, dVector *pTrack) {
	Line3D l0 = ICam->GetPickRay(xPrev, yPrev);
	Line3D l1 = ICam->GetPickRay(xCur, yCur);
	Vector3D O = l0.Orig();
	Vector3D D = l0.Dir();
	dVector CrossPrev;
	assert(Plane.RayIntersection(dVector(O.x, O.y, O.z), dVector(D.x, D.y, D.z), NULL, &CrossPrev));
	O = l1.Orig();
	D = l1.Dir();
	dVector CrossCur;
	assert(Plane.RayIntersection(dVector(O.x, O.y, O.z), dVector(D.x, D.y, D.z), NULL, &CrossCur));
	pTrack->Copy(CrossPrev - CrossCur);
}

//-----------------------------------------------------------------------------
// InGame_Track
//-----------------------------------------------------------------------------
void Navigator::InGame_Track(int xPrev, int yPrev, int xCur, int yCur) {
	dVector d;
	TrackHelper(m_TrackPlane, xPrev, yPrev, xCur, yCur, &d);
	fMapX += (float)d.x / c_WorldGridStepX;
	fMapY += (float)d.y / c_WorldGridStepY;
}

//-----------------------------------------------------------------------------
// InGame_Tumble
//-----------------------------------------------------------------------------
void Navigator::InGame_Tumble(int xPrev, int yPrev, int xCur, int yCur) {
	// 1.75
	const float Speed = 1.0f, RSingular = 300., // r / RSpeed == 1
		RHole = 200.;
	float xA = xPrev - RealLx / 2, yA = yPrev - RealLy / 2,
		xB = xCur - RealLx / 2, yB = yCur - RealLy / 2;
	// Considering shift along y axis
	float cx = (xA + xB) / 2;
	Vector3D A(cx, yA, 0);
	Vector3D B(cx, yB, 0);
	Vector3D R;
	if(A.normalize() && B.normalize()) {
		R.cross(A, B);
		float cy = (yA + yB) / 2;
		float r = sqrt(cx * cx + cy * cy);
		if(r > RHole) m_Args.Azim -= Speed * R.z;
		else {
			float k = r / RHole;
			k *= k;
			m_Args.Azim -= 2.0f * Speed * k * R.z;
		}
	}
	// Considering shift along x axis
	A.set(xA, RSingular, 0);
	B.set(xB, RSingular, 0);
	A.normalize(); // length always > c_Epsilon
	B.normalize();
	R.cross(A, B);
	m_Args.Azim -= Speed * R.z;
	m_TendToArgs.RotationSpring(false);
}

//-----------------------------------------------------------------------------
// InGame_Dolly
//-----------------------------------------------------------------------------
void Navigator::InGame_Dolly(int dx, int dy) {
	m_Args.toEyeEx += 2 * (dx + dy);
	m_TendToArgs.DollySpring(false);
}

//-----------------------------------------------------------------------------
// Adv_Track
//-----------------------------------------------------------------------------
void Navigator::Adv_Track(int xPrev, int yPrev, int xCur, int yCur) {
	dVector d;
	TrackHelper(m_TrackPlane, xPrev, yPrev, xCur, yCur, &d);
	m_Args.At += d;
}

//-----------------------------------------------------------------------------
// MoveAt
//-----------------------------------------------------------------------------
void Navigator::MoveAt(double s) {
	const double d = 1.0;
	dVector Dir;
	CalcCamArgs(NULL, &Dir, NULL, NULL);
	double t = s * d;
	if(s < 0) {
		if(fabs(t) > m_Args.toEye) {
			t = - m_Args.toEye;
		}
	}
	m_Args.At += t * Dir;
	m_Args.toEye += t;
}

//-----------------------------------------------------------------------------
// OnMouseWheel
//-----------------------------------------------------------------------------
bool Navigator::OnMouseWheel(int Delta) {
	if(!Delta) return false;

	if(g_SurfEditor.FreeCamera && GetKeyState(VK_MENU) < 0) {
		MoveAt(Delta);
		m_fForceDrawAt = true;
		return true;
	} else {
//		if(!g_fWheelIsBlocked) {
//			m_TendToArgs.OnMouseWheel(Delta);
//		}
	}
	return false;
}

void AddMarker(SurfMesh &, const SurfVertex &, const SurfVertex &, int, DWORD);

// Navigator::DrawEditorInterface
void Navigator::DrawEditorInterface() {

}

//-----------------------------------------------------------------------------
// OnDrawOnMapOverAll
//-----------------------------------------------------------------------------
void Navigator::OnDrawOnMapOverAll() {
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*	pImSelected ImSel(MyNation);
	if(ImSel._getamount()) {
		RawUnit ru = ImSel._getunit(0);
		OneObject *pOO = ru.object();


		Vector3D Pos((float)pOO->RealX / 16.0f, (float)pOO->RealY / 16.0f, (float)pOO->RZ);
		WorldToClosestScreenPoint(Pos);

		static int idTex = IRS->GetTextureID("white");
		static int idSh = SurfFx(SFX_AUX);
		cColor Color(1.0f, 0.0f, 0.0f, 0.5f);
		BaseMesh bm;
		const float w = 100.0f;
		const float h = 50.0f;
		SelExt::GenRect(bm, cVec2(Pos.x - w / 2.0f, Pos.y - h / 2.0f), cVec2(w, h), Color, idTex, idSh);
		DrawBM(bm);
	}*/
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	// Toggle free camera:
	if(GetKeyState(VK_MENU) < 0 && GetKeyState('F') < 0) {
		if(!g_SurfEditor.FreeCamera) {
			g_SurfEditor.FreeCamera = true;
			g_SurfEditor.Update();
			g_Navigator.Toggle(true);
		}
	}
	// Drawing track:
	if(m_Track.IsActive && m_Track.nCurSegment < m_Track.Segments.Count()) {
		float MaxSpeed = m_Track.AlphaFunc.GetCurrentSpeed(m_Track.AlphaFunc.GetStartTime() + m_Track.AlphaFunc.GetAccelTime());
		float CurAlpha = cMath::Clamp01(1.0f - m_Track.AlphaFunc.GetCurrentSpeed((float)m_Track.PlayedMs) / MaxSpeed);
		if(m_Track.Segments[m_Track.nCurSegment].SwapFadeInFadeOut) {
			CurAlpha = 1.0f - CurAlpha;
		}
		
		if(!cMath::IsZero(CurAlpha)) {
			static int idTex = IRS->GetTextureID("white");
			static int idSh = SurfFx(SFX_AUX);
			cColor Color(0.0f, 0.0f, 0.0f, CurAlpha);
			BaseMesh bm;
			SelExt::GenRect(bm, cVec2::Zero, cVec2(RealLx, RealLy), Color, idTex, idSh);
			DrawBM(bm);
		}
	}
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*	if(m_Track.TRACK.GetKeysCount()) {
		const float tStart = m_Track.TRACK.GetTimeStart();
		const float tEnd = m_Track.TRACK.GetTimeEnd();
		SurfMesh sm;
		for(float tCur = tStart; tCur <= tEnd + cMath::Epsilon; tCur += 100) {
			cTrack::Key KeyCur = m_Track.TRACK.GetCurrentKey((float)tCur);
//			DrawCube(Vector3D(KeyCur.Pos.x, KeyCur.Pos.y, KeyCur.Pos.z), cColor::Red.ToAbgr(), 10.0f);
			SurfVertex l0;
			l0.x = KeyCur.Pos.x, l0.y = KeyCur.Pos.y, l0.z = KeyCur.Pos.z;
			SurfVertex l1 = l0;
			l0.z -= 10;
			l1.z += 10;
			AddMarker(sm, l0, l1, 10, cColor::Red.ToArgb());
		}
		sm.Draw();
	}*/
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	if(m_Track.IsActive && m_Track.IsCapturing) {
		GameSpeed = int(256.0 / (25.0 / double(Navigator::FPS)) + 0.5);
	}

	// ShowDeviceProperties:
	if(GSets.SVOpt.TrueColor.ShowDeviceProperties) {
		int W, H, Bpp, RR;
		IRS->GetDeviceDisplayMode(&W, &H, &Bpp, &RR);
		cStr Str("Display: ");
		Str << W << " x " << H << " x " << Bpp << " bpp @ " << RR << "Hz";
		Str << " BackBuffer: " << (IRS->TrueColorIsEnabled() ? "TrueColor" : "HiColor");
		CmdExt::MessageBoxArgs Args;
		Args.Align = CmdExt::A_CORNER;
		CmdExt::MessageBox(Str, Args);
	}

	const double dAngle = cMath::Rad(1.6);
	POINT pt;
	if(g_SurfEditor.FreeCamera) {
		if(!(GetKeyState(VK_MENU) < 0)) {
			m_fForceDrawAt = false;
		}
		// Drawing Cross:
		if((m_Mode || m_fForceDrawAt) && !m_Track.IsActive && !m_fLinkToHero) {
			const int dCross = 100;
			SurfMesh m;
			int d = cMath::Max(2, int(m_Args.toEye / 500));
			AddLine(m, SurfVertex(m_Args.At.x - dCross, m_Args.At.y, m_Args.At.z),
				SurfVertex(m_Args.At.x + dCross, m_Args.At.y, m_Args.At.z), cColor::Yellow.ToArgb(), d);
			AddLine(m, SurfVertex(m_Args.At.x, m_Args.At.y - dCross, m_Args.At.z),
				SurfVertex(m_Args.At.x, m_Args.At.y + dCross, m_Args.At.z), cColor::Yellow.ToArgb(), d);
			AddLine(m, SurfVertex(m_Args.At.x, m_Args.At.y, m_Args.At.z - dCross),
				SurfVertex(m_Args.At.x, m_Args.At.y, m_Args.At.z + dCross), cColor::Yellow.ToArgb(), d);
			SurfRef r;
			m.GetRefAll(r, SR_TRIS);
			SelExt::DrawSwatch(m);
			r.Draw(Surface::SFX_AUX);
		}

		GetCursorPos(&pt);
		bool fLeft = GetKeyState(VK_LEFT) < 0 || pt.x == 0;
		bool fRight = GetKeyState(VK_RIGHT) < 0 || pt.x == RealLx - 1;
		bool fUp = GetKeyState(VK_UP) < 0 || pt.y == 0;
		bool fDown = GetKeyState(VK_DOWN) < 0 || pt.y == RealLy - 1;
		bool fCtrl = GetKeyState(VK_CONTROL) < 0;
		if(!fLeft && !fRight && !fUp && !fDown) return;

		if(m_Mode) {
			fLeft = false;
			fRight = false;
			fUp = false;
			fDown = false;
		}

		Vector3D Dir = ICam->GetDirection();
		Vector3D Pos = ICam->GetPosition();
		dVector u = m_Args.At - dVector(Pos.x, Pos.y, Pos.z);
		dVector d = dVector::Origin;

		if(fCtrl && fUp) d += 10 * dVector::zAxis;
		else if(fCtrl && fDown) d -= 10 * dVector::zAxis;
		else if(fUp) d += 60 * dVector(Dir.x, Dir.y, 0).Normal();
		else if(fDown) d += 60 * dVector(- Dir.x, - Dir.y, 0).Normal();
		
		if(fCtrl && fRight) d += 30 * dVector(- Dir.y, Dir.x, 0).Normal();
		else if(fCtrl && fLeft) d += 30 * dVector(Dir.y, - Dir.x, 0).Normal();
		else if(fRight) {
			m_Args.Azim -= dAngle;
			u *= dMatrix::OfRotation(dVector::zAxis, dAngle);
		} else if(fLeft) {
			m_Args.Azim += dAngle;
			u *= dMatrix::OfRotation(dVector::zAxis, - dAngle);
		}
		d += (u + dVector(Pos.x, Pos.y, Pos.z)) - m_Args.At;
		m_Args.At += d;
	}
}

//-----------------------------------------------------------------------------
// Adv_Tumble
//-----------------------------------------------------------------------------
void Navigator::Adv_Tumble(int dx, int dy) {
	const float Speed = 0.003f;
	double da = Speed * float(dx);
	double de = - Speed * float(dy);
	if(m_fLinkToHero) {
		m_Args.ExtraAzim = cMath::AngleNormalizePi(m_Args.ExtraAzim + da);
		m_Args.ExtraElev += de;
		if(dx || dy) {
			m_fAddTurning = false;
		}
	} else {
		m_Args.Azim += da;
		m_Args.Elev += de;
	}
	m_Args.Azim = cMath::AngleNormalizeTwoPi(m_Args.Azim);
	m_Args.Elev = cMath::AngleNormalizePi(m_Args.Elev);
}

void Navigator::Adv_Roll(int dx, int dy) {
	const float Speed = 0.003f;
	float d = Speed * float(dx) - Speed * float(dy);
	m_Args.Roll += d;
	m_Args.Roll = cMath::AngleNormalizePi(m_Args.Roll);
}

//-----------------------------------------------------------------------------
// Adv_Dolly
//-----------------------------------------------------------------------------
void Navigator::Adv_Dolly(int dx, int dy) {
	double d = 6 * (dx + dy);
	m_Args.toEye += d;
	const double toEyeMin = 0.0;
	if(m_Args.toEye < toEyeMin) {
		double r = toEyeMin - m_Args.toEye;
		m_Args.toEye = toEyeMin;
		dVector Dir;
		Vector3D dir = ICam->GetDirection();
		Dir.x = dir.x, Dir.y = dir.y, Dir.z = dir.z;
		m_Args.At += r * Dir;
	}
	cMath::Clamp(m_Args.toEye, toEyeMin, cMath::DoubleMaxValue);
}

//-----------------------------------------------------------------------------
// Navigator::SetFromSplineKey
//-----------------------------------------------------------------------------
void Navigator::SetFromSplineKey(const cTrack::Key &Src) {
	assert(g_SurfEditor.FreeCamera);

	m_Args.Defaults();
	m_Args.At.Set(Src.Pos.x, Src.Pos.y, Src.Pos.z);
	m_Args.Azim = cMath::Rad(Src.Orient.Yaw);
	m_Args.Elev = cMath::Rad(Src.Orient.Pitch) + cMath::Pi;
	m_Args.Roll = cMath::Rad(Src.Orient.Roll);
	m_Args.toEye = Src.Offset;
}

// Copy : void(const SurfCamera &)
void Navigator::Copy(const SurfCamera &Src) {
	m_Args.Defaults();
	m_Args.At = dVector(Src.xAt, Src.yAt, Src.zAt);
	m_Args.toEye = Src.toEye;
	m_Args.Azim = cMath::Rad(Src.Azim);
	m_Args.Elev = cMath::Rad(Src.Elev);
	m_Args.toEyeEx = Src.toEyeEx;
}

// IsPlayingTrack
bool Navigator::IsPlayingTrack(const CameraTrack *pTrack) const {
	return m_Track.IsActive && m_Track.pSrc == pTrack;
}

void Navigator::StopTrack() {
	FreeTrack();
}

//-----------------------------------------------------------------------------
// RewindTrackToEnd
//-----------------------------------------------------------------------------
void Navigator::RewindTrackToEnd() {
	if(!m_Track.IsActive) return;
	const cTrack &BackSpline = m_Track.Segments.GetBack().Spline;
	SetFromSplineKey(BackSpline.GetKey(BackSpline.GetKeysCount() - 1));
	StopTrack();
}

//-----------------------------------------------------------------------------
// Navigator::Track::SetCameraParams
//-----------------------------------------------------------------------------
void Navigator::Track::SetCameraParams(const Navigator::Track::Segment &S) {
	m_CameraWasForced = S.ForceCameraParams;
	if(m_CameraWasForced) {
		extern float CameraMovingStage;
		m_PrevFinalFogNearPlane = EngSettings.final_FogNearPlane;
		m_PrevFinalFogFarPlane = EngSettings.final_FogFarPlane;
		m_PrevCameraMovingStage = CameraMovingStage;
		CameraMovingStage = S.CameraMovingStage;
		EngSettings.final_FogNearPlane = S.FinalFogNearPlane;
		EngSettings.final_FogFarPlane = S.FinalFogFarPlane;
	}
}

//-----------------------------------------------------------------------------
// Navigator::Track::RestoreCameraParams
//-----------------------------------------------------------------------------
void Navigator::Track::RestoreCameraParams() {
	if(m_CameraWasForced) {
		extern float CameraMovingStage;
		CameraMovingStage = m_PrevCameraMovingStage;
		EngSettings.final_FogNearPlane = m_PrevFinalFogNearPlane;
		EngSettings.final_FogFarPlane = m_PrevFinalFogFarPlane;
	}
}

//-----------------------------------------------------------------------------
// Navigator::Play
//-----------------------------------------------------------------------------
void Navigator::Play(CameraTrack *pTrack, bool fCapture) {
	FreeTrack();
	m_Track.pSrc = pTrack;
	m_Track.IsCapturing = fCapture;

	m_Track.Segments.Free();
	int SplinesCount = m_Track.pSrc->Splines.NValues;
	if(SplinesCount < 1) {
		return;
	}

	// Stats per splines:
	cList<int> NotEmptyKeysCounts;
	NotEmptyKeysCounts.Lock(SplinesCount, 0);

	for(int nSpline = 0; nSpline < SplinesCount; nSpline++) {
		const CameraSpline *pSpline = m_Track.pSrc->Splines[nSpline];
		if(pSpline->Skip) {
			continue;
		}
		for(int nKey = 0; nKey < pSpline->Keys.NValues; nKey++) {
			const CameraSplineKey *pKey = pSpline->Keys[nKey];
			if(!pKey->IsEmpty()) {
				NotEmptyKeysCounts[nSpline]++;
			}
		}
	}

	// Culling based on stats:
	bool CullTrack = true;
	cList<bool> CullSpline;
	CullSpline.Lock(SplinesCount, true);
	for(nSpline = 0; nSpline < SplinesCount; nSpline++) {
		const CameraSpline *pSpline = m_Track.pSrc->Splines[nSpline];
		if(pSpline->Skip) {
			continue;
		}
		if(NotEmptyKeysCounts[nSpline] >= 2 && pSpline->GetDurationSec() > 0) {
			CullSpline[nSpline] = false;
			CullTrack = false; // At least one spline we will play
		}
	}
	if(CullTrack) {
		return;
	}

	// Creating segments:
	for(int nSpline = 0; nSpline < SplinesCount; nSpline++) {
		if(CullSpline[nSpline]) {
			continue;
		}
		
		const CameraSpline *pSpline = m_Track.pSrc->Splines[nSpline];
		Track::Segment S;

		S.DurationMs = cMath::Ms(pSpline->GetDurationSec());
		S.AccumTimeMs = S.DurationMs;
		if(m_Track.Segments.Count()) {
			S.AccumTimeMs += m_Track.Segments.GetBack().AccumTimeMs;
		}
		S.AccelTime = cMath::Ms(pSpline->AccelTime);
		S.DecelTime = cMath::Ms(pSpline->DecelTime);
		S.FadeInTime = cMath::Ms(pSpline->FadeInTime);
		S.FadeOutTime = cMath::Ms(pSpline->FadeOutTime);
		S.Closed = pSpline->Closed;
		S.SwapFadeInFadeOut = pSpline->SwapFadeInFadeOut;

		S.ForceCameraParams = pSpline->ForceCameraParams;
		S.FinalFogNearPlane = pSpline->final_FogNearPlane;
		S.FinalFogFarPlane = pSpline->final_FogFarPlane;
		S.CameraMovingStage = pSpline->CameraMovingStage;


		float FirstTimeShiftMs = 0.0f; // for closed splines
		bool FirstTimeShiftWasSkiped = false;
		float tCurMs = 0.0f;
		for(int nKey = 0; nKey < pSpline->Keys.NValues; nKey++) {
			const CameraSplineKey *pKey = pSpline->Keys[nKey];
			if(pKey->IsEmpty()) {
				continue;
			}
			cVec3 Pos(pKey->xAt, pKey->yAt, pKey->zAt);
			cAngles Orient(cMath::Deg(pKey->Elev - cMath::Pi), cMath::Deg(pKey->Azim), cMath::Deg(pKey->Roll));
			float ToEye = pKey->toEye;
			if(!FirstTimeShiftWasSkiped) {
				FirstTimeShiftMs = cMath::Ms(pKey->TimeShift);
				FirstTimeShiftWasSkiped = true;
			} else {
				tCurMs += cMath::Ms(pKey->TimeShift);
			}
			S.Spline.AddKey(cTrack::Key(tCurMs, Pos, Orient, ToEye, 0));
		}

//		S.Spline.ReCreateCVs();
//		S.Spline.MakeTimeUniform(S.DurationMs);

		if(S.Closed) {
			tCurMs += FirstTimeShiftMs;
			const cTrack::Key &ClosingKey = S.Spline.GetKey(0);
			S.Spline.AddKey(cTrack::Key(tCurMs, ClosingKey.Pos, ClosingKey.Orient, ClosingKey.Offset, 0));
			S.Spline.Setup(true);
		}

		assert(cMath::Equals(S.DurationMs, tCurMs, 0.01f));
		
		m_Track.Segments.Add(S);
	}
	
	// Prepearing folder for capturing (if needed):
	if(m_Track.IsCapturing) {
		m_Track.nFrame = 0;
		SYSTEMTIME st;
		GetLocalTime(&st);
		CreateDirectory("ScreenShots", NULL);
		sprintf(m_Track.FolderToCaptureIn, "ScreenShots\\Track.%d.%.2d.%.2d.(%.2d %.2d %.2d)",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		CreateDirectory(m_Track.FolderToCaptureIn, NULL);
	}

	m_Track.SetCameraParams(m_Track.Segments[0]);

	m_Track.PlayedMs = 0.0f;
	m_Track.StartTimeMs = cTimer::GetClockMilliseconds();
	m_Track.nCurSegment = 0;
	m_Track.Repeat = pTrack->Repeat;
	m_Track.HideUI = pTrack->HideUI;
	// UI:
	if(m_Track.HideUI) {
		GSets.CGame.ViewMask = 0;
	}
	m_Track.IsActive = true;
}

//-----------------------------------------------------------------------------
// Navigator

//-----------------------------------------------------------------------------
void Navigator::UpdateTrack() {
	if(!m_Track.IsActive || !g_SurfEditor.FreeCamera) {
		return;
	}
	
	double dTime = 0;
	const double CurTimeMs = cTimer::GetClockMilliseconds();
	if(m_Track.IsCapturing) {
		dTime = 1000.0 / Navigator::FPS;
	} else {
		dTime = CurTimeMs - (m_Track.StartTimeMs + m_Track.PlayedMs);
	}
	
	if(IsOnPause()) {
		m_Track.StartTimeMs += dTime;
		dTime = 0;
	}

	// Oh..... twisted hack......
	if(m_Track.IsCapturing && m_Track.nFrame < 2) {
		dTime = 0;
	}

	m_Track.PlayedMs += dTime;

	if(m_Track.IsCapturing) {
		float Sec = float(m_Track.PlayedMs / 1000.0);
		IRS->TimeOverrideEnable(Sec);
	}

	int nPrevCurSegment = m_Track.nCurSegment;
	for(; m_Track.nCurSegment < m_Track.Segments.Count(); m_Track.nCurSegment++) {
		if(m_Track.PlayedMs <= m_Track.Segments[m_Track.nCurSegment].AccumTimeMs) {
			break;
		}
	}

	// Closed Spline:
	if(nPrevCurSegment != m_Track.nCurSegment && m_Track.Segments[nPrevCurSegment].Closed) {
		m_Track.nCurSegment = nPrevCurSegment;
		float PrevAccumTimeMs = 0.0f;
		if(m_Track.nCurSegment > 0) {
			PrevAccumTimeMs = m_Track.Segments[m_Track.nCurSegment - 1].AccumTimeMs;
		}
		Track::Segment &S = m_Track.Segments[m_Track.nCurSegment];
		m_Track.PlayedMs = cMath::Periodic((float)m_Track.PlayedMs, PrevAccumTimeMs, S.AccumTimeMs);
		m_Track.StartTimeMs = CurTimeMs - m_Track.PlayedMs;
		S.AccelTime = S.DecelTime = S.FadeInTime = S.FadeOutTime = 0.0f;
	}

	if(m_Track.nCurSegment >= m_Track.Segments.Count()) {
		if(!m_Track.Repeat) {
			FreeTrack();
			const cTrack &BackSpline = m_Track.Segments.GetBack().Spline;
			SetFromSplineKey(BackSpline.GetKey(BackSpline.GetKeysCount() - 1));
			Toggle(false);
			return;
		} else { // Repeat:
			m_Track.PlayedMs = cMath::Periodic((float)m_Track.PlayedMs, 0.0f, m_Track.Segments.GetBack().AccumTimeMs);
			m_Track.StartTimeMs = CurTimeMs - m_Track.PlayedMs;
			m_Track.nCurSegment = 0;
			for(; m_Track.nCurSegment < m_Track.Segments.Count(); m_Track.nCurSegment++) {
				if(m_Track.PlayedMs <= m_Track.Segments[m_Track.nCurSegment].AccumTimeMs) {
					break;
				}
			}
			nPrevCurSegment = -1;
		}
	}
	
	const Track::Segment &CurSegment = m_Track.Segments[m_Track.nCurSegment];
	if(nPrevCurSegment != m_Track.nCurSegment) {
		m_Track.RestoreCameraParams();
		m_Track.SetCameraParams(CurSegment);
	}

	cTrack TempSpline;
	TempSpline.Copy(CurSegment.Spline);
//	TempSpline.ShiftTime((float)m_Track.StartTimeMs);
	if(m_Track.nCurSegment > 0) {
		TempSpline.ShiftTime(m_Track.Segments[m_Track.nCurSegment - 1].AccumTimeMs);
	}
	
	cInterpolateAccelDecelLinear<float> tFunc;
	tFunc.Init(TempSpline.GetTimeStart(), CurSegment.AccelTime, CurSegment.DecelTime, TempSpline.GetDuration(),
		TempSpline.GetTimeStart(), TempSpline.GetTimeEnd());
	m_Track.AlphaFunc.Init(TempSpline.GetTimeStart(), CurSegment.FadeInTime, CurSegment.FadeOutTime,
		TempSpline.GetDuration(), 0.0f, 1.0f);
	
	const tf = tFunc.GetCurrentValue((float)m_Track.PlayedMs);
	const cTrack::Key K = TempSpline.GetCurrentKey(tf);
//	const cVec3 d = TempSpline.GetCurrent2ndDerivative(tf);
	
	SetFromSplineKey(K);
//	static const float dRoll = 0.1f;
//	m_Args.Roll = dRoll * d.To2().Length();

	// Capture:
	if(m_Track.IsCapturing) {
		char FileName[128];
		if(m_Track.nFrame >= 2) {
			sprintf(FileName, "Frame%.4d.jpg", m_Track.nFrame - 2);
			cStr FullPath(m_Track.FolderToCaptureIn);
			FullPath += "\\";
			FullPath += FileName;
			IRS->ScreenShotJPG(FullPath);
		}
		m_Track.nFrame++;
	}
}

} // Surface