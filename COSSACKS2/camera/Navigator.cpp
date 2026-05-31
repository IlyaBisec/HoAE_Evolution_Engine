#include "stdheader.h"
#include "vCamera.h"
#include "..\ua_Ability.h"
#include "..\ComplexShooter.h"
#include "..\CurrentMapOptions.h"
#include "..\GameInterface.h"
#include "Navigator.h"
#include "..\comms\comms.h"
#include "SelExt.h"
#include "..\cvi_MainMenu.h"
#include "CmdExt.h"
#include "CollidingCamera.h"
#include "TankController.h"
#include "IWater.h"
#include "..\Surface\Messager.h"

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
// Navigator::GetPlayingTrackTotalKeysCount
//-----------------------------------------------------------------------------
int Navigator::GetPlayingTrackTotalKeysCount() const {
	if(!IsPlayingTrack()) {
		return 0;
	}
	int NumKeys = 0;
	for(int nSeg = 0; nSeg < m_Track.Segments.Count(); nSeg++) {
		NumKeys += m_Track.Segments[nSeg].Channels.Pos.GetValuesCount();
	}
	return NumKeys;
} // GetPlayingTrackTotalKeysCount

//-----------------------------------------------------------------------------
// Navigator::GetPlayingTrackMaxSegKeysCount
//-----------------------------------------------------------------------------
int Navigator::GetPlayingTrackMaxSegKeysCount() const {
	if(!IsPlayingTrack()) {
		return 0;
	}
	int MaxNumKeys = 0;
	for(int nSeg = 0; nSeg < m_Track.Segments.Count(); nSeg++) {
		MaxNumKeys = cMath::Max(MaxNumKeys, m_Track.Segments[nSeg].Channels.Pos.GetValuesCount());
	}
	return MaxNumKeys;
} // GetPlayingTrackMaxSegKeysCount

bool Navigator::OnMapUnLoading() {
	if(IsPlayingTrack()) {
		FreeTrack();
	}
	ClearBlackScreen();

	void SetCentralUnit(OneObject *);	// To prevent centering previously linked to hero camera to random unit
	SetCentralUnit(NULL);	// after *NEXT MISSION START* and *LOADING GAME ON ANOTHER MAP* we should set central unit to NULL!

	return true;
}

void Navigator::ClearBlackScreen() {
	m_Track.LeaveBlackScreen = false;
	m_Track.LeaveBlackMap = false;
	ShowUI();
}

bool Navigator::OnCheatEntering(const char *CheatStr) {
	if(cStr::EqualsNoCase(CheatStr, "clearblackscreen")) {
		ClearBlackScreen();
		m_Track.TrackWasStarted = true; // for Start
		return true;
	}
	return false;
}

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

	bool Ins = false;
	bool Del = false;
	if( !(KeysAreFocusedOnInputBox || (GetKeyState(VK_CONTROL)&0x8000)) ){
		Ins = GetKeyState(VK_INSERT) < 0;
		Del = GetKeyState(VK_DELETE) < 0;
	}
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
		m_Track.CheckEvents(cMath::Infinity); // Exec all remaining events.
		m_Track.RestoreCameraParams();
		// Leave fog of war:
		extern int FogMode;
		if(FogMode != m_Track.Segments.GetBack().FogOfWar) {
			FogMode = m_Track.Segments.GetBack().FogOfWar;
			void RenderMinimap();
			RenderMinimap();
		}
		// UI:
		if(m_Track.HideUI) {
			if(!m_Track.LeaveBlackScreen) {
				ShowUI();
			}
		}
		// Waterflood:
		if(m_Track.Waterflood) {
			IWater->SetWaterline(0.0f);
			IWater->SetBoilingCoef(0.0f);
			IWater->Refresh(ITerra->GetExtents());
		}

		m_Track.IsActive = false;
		if(m_Track.IsCapturing) {
			IRS->TimeOverrideDisable();
		}
		// Copying loaded tex names to named track:
		if(m_Track.TrackName.Length()) {
			for(int t = 0; t < MOptions.CameraTracks.NValues; t++) {
				cStr Name(MOptions.CameraTracks[t]->Name.cpchar());
				if(cStr::Equals(Name.ToCharPtr(), m_Track.TrackName.ToCharPtr())) {
					MOptions.CameraTracks[t]->Track.LoadedResources.Clear();
					for(int i = 0; i < m_Track.m_LoadedResources.Count(); i++) {
						_str *p = new _str(m_Track.m_LoadedResources[i].ToCharPtr());
						MOptions.CameraTracks[t]->Track.LoadedResources.Add(p);
					}
				}
			}
		}
/*		cList<cStr> Intersection;
		for(int i = 0; i < m_Track.m_GetTextureIDStats.Count(); i++) {
			const cStr &s = m_Track.m_GetTextureIDStats[i];
			if(m_Track.m_LoadTextureStats.Contains(s)) {
				Intersection.Add(s);
			}
		}
		for(int i = 0; i < m_Track.m_LoadTextureStats.Count(); i++) {
			const cStr &s = m_Track.m_LoadTextureStats[i];
			if(m_Track.m_GetTextureIDStats.Contains(s) && !Intersection.Contains(s)) {
				Intersection.Add(s);
			}
		}*/
		// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		m_Args.Roll = 0.0;
	}
}

//-----------------------------------------------------------------------------
// Home
//-----------------------------------------------------------------------------
void Navigator::Home() {
	bool PlayingOneKeySeg = false;
	if(m_Track.IsActive && m_Track.nCurSegment < m_Track.Segments.Count()) {
		if(m_Track.Segments[m_Track.nCurSegment].OnlyPosToFirstKey) {
			PlayingOneKeySeg = true;
		}
	}
	
	void SetCameraPosEx(float, float);
	if(m_fLinkToHero) {
		LinkToHeroToggle();
		m_Args.Elev = cMath::Pi;
	}
	if(m_Track.IsActive) {
		if(!PlayingOneKeySeg) {
			RewindTrackToEnd();
		}
	}
	// Trying to match in game camera to free:
	if(m_Args.At != dVector::Origin) {
		dVector Dir;
		CalcCamArgs(NULL, &Dir, NULL, NULL);
		dPlane Z(dVector::zAxis, 0);
		dVector X;
		if(Z.RayIntersection(m_Args.At, Dir, NULL, &X)) {
			extern float fMapX;
			extern float fMapY;
			fMapX = X.x / c_WorldGridStepX;
			fMapY = X.y / c_WorldGridStepY;
			void LimitCamera();
			if(!PlayingOneKeySeg) {
				LimitCamera();
			}
		}
	}
	m_Args.Defaults();
	g_SurfEditor.FreeCamera = false;
	g_SurfEditor.ForceZNear = false;
	if(!PlayingOneKeySeg) {
		FreeTrack();
	}
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
			m_TendToArgs.m_MovingStage = CameraMovingStage = m_pCameraMovingStage;
			MOptions.FogArgs.final_FogNearPlane = m_pFinalFogNearPlane;
			MOptions.FogArgs.final_FogFarPlane = m_pFinalFogFarPlane;
			MOptions.FogArgs.UseFromMap = m_PrevFogArgsUseFromMap;
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
			m_PrevFogArgsUseFromMap = MOptions.FogArgs.UseFromMap;
			m_pFinalFogNearPlane = MOptions.FogArgs.final_FogNearPlane;
			m_pFinalFogFarPlane = MOptions.FogArgs.final_FogFarPlane;
			m_pCameraMovingStage = CameraMovingStage;
			m_TendToArgs.m_MovingStage = CameraMovingStage = EngSettings.ThirdPersonCamera.CameraMovingStage;
			MOptions.FogArgs.final_FogNearPlane = EngSettings.ThirdPersonCamera.final_FogNearPlane;
			MOptions.FogArgs.final_FogFarPlane = EngSettings.ThirdPersonCamera.final_FogFarPlane;
			MOptions.FogArgs.UseFromMap = true;
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
	static bool fSpaceWasPressed = false;

    if(m_fDisableHeroManagement)return;

	bool fForward = (GetKeyState(VK_UP) < 0 || (GetKeyState('E') < 0 && EngSettings.ThirdPersonCamera.ESDF) ||
		(GetKeyState('W') < 0 && EngSettings.ThirdPersonCamera.WASD));
	bool fBackward = (GetKeyState(VK_DOWN) < 0 || (GetKeyState('D') < 0 && EngSettings.ThirdPersonCamera.ESDF) ||
		(GetKeyState('S') < 0 && EngSettings.ThirdPersonCamera.WASD));
	bool fRight = (GetKeyState(VK_RIGHT) < 0 || (GetKeyState('F') < 0 && EngSettings.ThirdPersonCamera.ESDF) ||
		(GetKeyState('D') < 0 && EngSettings.ThirdPersonCamera.WASD));
	bool fLeft = (GetKeyState(VK_LEFT) < 0 || (GetKeyState('S') < 0 && EngSettings.ThirdPersonCamera.ESDF) ||
		(GetKeyState('A') < 0 && EngSettings.ThirdPersonCamera.WASD));
	bool fSpace = GetKeyState(VK_SPACE) < 0;
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

	if(fRight) {
		m_HeroDir += 256;
	}
	if(fLeft) {
		m_HeroDir -= 256;
	}

	m_pHero->RealDirPrecise = m_HeroDir;
	m_pHero->RealDir = m_HeroDir / 256;

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
	
	if(dSign && !fSpaceWasPressed && !fSpace) {
		if(dSign > 0) {
			m_pHero->BackMotion = false;
		} else {
			m_pHero->BackMotion = true;
		}
		int lDestX = m_pHero->RealX + dSign * int(TCos[CurAngle >> 8] * 4);
		int lDestY = m_pHero->RealY + dSign * int(TSin[CurAngle >> 8] * 4);
		int DestX = m_pHero->RealX + dSign * int(TCos[CurAngle >> 8] * 32);
		int DestY = m_pHero->RealY + dSign * int(TSin[CurAngle >> 8] * 32);

		// Calcing speed based upon lock points:
		MotionField *pMF = &MFIELDS[m_pHero->LockType];
		assert(pMF);
		int ix = lDestX / 256, iy = lDestY / 256, Sum = 0;
		if(!pMF->CheckPt(ix, iy)) {
			m_pHero->DestX = DestX;
			m_pHero->DestY = DestY;
		} else {
			m_pHero->DestX = -1;
			m_pHero->DestY = -1;
		}
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
	const bool SlowNavigation = GetKeyState(VK_CONTROL) < 0 && g_SurfEditor.FreeCamera;
	bool fTumble = (LMB && (GetKeyState(VK_MENU) < 0 || SlowNavigation)) || (m_fLinkToHero && g_fMMBIsDown);
	bool fDolly = RMB && (GetKeyState(VK_MENU) < 0 || SlowNavigation);
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
	
	void LimitCamera();
	LimitCamera();
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
	void LimitCamera();
	LimitCamera();
	m_TendToArgs.DollySpring(false);
}

//-----------------------------------------------------------------------------
// Adv_Track
//-----------------------------------------------------------------------------
void Navigator::Adv_Track(int xPrev, int yPrev, int xCur, int yCur) {
	const bool Slowly = GetKeyState(VK_CONTROL) < 0;
	dVector d;
	if(Slowly) {
		const Vector3D p = ICam->GetPosition();
		const Vector3D n = ICam->GetDir();
		const dVector Pos(p.x, p.y, p.z);
		const dVector Normal(n.x, n.y, n.z);
		dPlane P(Normal, Pos + 50.0 * Normal);
		TrackHelper(P, xPrev, yPrev, xCur, yCur, &d);
	} else {
		TrackHelper(m_TrackPlane, xPrev, yPrev, xCur, yCur, &d);
	}
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

	if(g_SurfEditor.FreeCamera && (GetKeyState(VK_MENU) < 0 || GetKeyState(VK_CONTROL) < 0)) {
		const bool Slowly = GetKeyState(VK_CONTROL) < 0;
		MoveAt((double)Delta / (Slowly ? 30.0 : 1.0));
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
// Navigator::HideUI
//-----------------------------------------------------------------------------
void Navigator::HideUI() {
	GSets.CGame.ViewMask = 0;
	extern bool FullMini;
	extern bool MiniMapVisible;
	FullMini = false;
	MiniMapVisible = false;
}

//-----------------------------------------------------------------------------
// Navigator::ShowUI
//-----------------------------------------------------------------------------
void Navigator::ShowUI() {
	GSets.CGame.ViewMask = 0xff;
	extern bool FullMini;
	extern bool MiniMapVisible;
	FullMini = true;
	MiniMapVisible = true;
}

//-----------------------------------------------------------------------------
// Navigator::OnGameStart
//-----------------------------------------------------------------------------
void Navigator::OnGameStart() {
	m_Track.TrackWasStarted = false;
}

// Navigator::OnGameSaving
bool Navigator::OnGameSaving(xmlQuote &xml) {
	xml.Assign_int(0); // some data
	return true;
}

// Navigator::OnGameLoading
bool Navigator::OnGameLoading(xmlQuote &xml) {
	int i = xml.Get_int();
	assert(i == 0);
	m_Track.TrackWasStarted = true;

	return true;
}

//-----------------------------------------------------------------------------
// Navigator::OnEditorStart
//-----------------------------------------------------------------------------
void Navigator::OnEditorStart() {
	m_Track.TrackWasStarted = true;
}

//-----------------------------------------------------------------------------
// OnDrawAlways
//-----------------------------------------------------------------------------
void Navigator::OnDrawOnMapAfterFogOfWar() {
	if(m_Track.IsActive) {
		m_Track.nFramesWasShown++;
	}
	if(m_Track.IsActive && m_Track.nCurSegment < m_Track.Segments.Count()) {
		// Force Fog Of War:
		extern int FogMode;
		if(m_Track.Segments[m_Track.nCurSegment].FogOfWar != FogMode) {
			FogMode = m_Track.Segments[m_Track.nCurSegment].FogOfWar;
			void RenderMinimap();
			RenderMinimap();
		}
	}

	// Drawing fade rect:
	bool ShowFadeRect = false;
	cColor FadeRectColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Precalcing alpha for skip:
	float SkipAlpha = 0.0f;
	if(m_Track.IsActive && m_Track.IsSkipping && !m_Track.IsSkipped) {
		SkipAlpha = cMath::Clamp01(m_Track.SkipFunc.GetCurrentValue((float)m_Track.PlayedMs));
		ShowFadeRect = true;
	}

	if(m_Track.IsActive && m_Track.nCurSegment < m_Track.Segments.Count()) {
		float MaxSpeed = m_Track.AlphaFunc.GetCurrentSpeed(m_Track.AlphaFunc.GetStartTime() + m_Track.AlphaFunc.GetAccelTime());
		float CurAlpha = cMath::Clamp01(1.0f - m_Track.AlphaFunc.GetCurrentSpeed((float)m_Track.PlayedMs) / MaxSpeed);
		if(m_Track.Segments[m_Track.nCurSegment].SwapFadeInFadeOut) {
			FadeRectColor.a = 1.0f - CurAlpha;
		} else {
			FadeRectColor.a = CurAlpha;
		}
		ShowFadeRect = true;
	} else if(m_Track.LeaveBlackScreen || (MOptions.HoldBlackScreenTillTrackStarts && !m_Track.TrackWasStarted) || m_Track.LeaveBlackMap) {
		FadeRectColor.a = 1.0f;
		ShowFadeRect = true;
		if(m_Track.LeaveBlackScreen || (MOptions.HoldBlackScreenTillTrackStarts && !m_Track.TrackWasStarted)) {
			HideUI();
		}
	}
	FadeRectColor.a = cMath::Max(SkipAlpha, FadeRectColor.a);
	if(ShowFadeRect && !cMath::IsZero(FadeRectColor.a)) {
		if(m_Track.nCurSegment < m_Track.Segments.Count()) {
			if(cMath::IsOne(FadeRectColor.a) && m_Track.Segments[m_Track.nCurSegment].DurationMs < 200) {
				HideUI();
			}
		}
		static int idTex = IRS->GetTextureID("white");
		static int idSh = SurfFx(SFX_AUX);
		BaseMesh bm;
		SelExt::GenRect(bm, cVec2::Zero, cVec2(RealLx, RealLy), FadeRectColor, idTex, idSh);
		DrawBM(bm);
	}
}
void AddCube(SurfMesh &M, const cVec3 &Center, const cVec3 &Size, const cColor &Color);
//-----------------------------------------------------------------------------
// OnDrawOnMapOverAll
//-----------------------------------------------------------------------------
void Navigator::OnDrawOnMapOverAll() {
	CameraSpline::PreviewAndEdit(m_ShowSpline);
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

	if(GetKeyState('Z') < 0 && GetKeyState('R') < 0) {
		m_Args.Roll = 0.0f;
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
		if(!(GetKeyState(VK_MENU) < 0) && !(GetKeyState(VK_CONTROL) < 0)) {
			m_fForceDrawAt = false;
		}
		// Drawing Cross:
		const cVec3 CrossCenter(m_Args.At.x, m_Args.At.y, m_Args.At.z);
		if((m_Mode || m_fForceDrawAt) && !m_Track.IsActive && !m_fLinkToHero) {
			const float dCross = 100;
			SurfMesh m;
			const float d = 2.0f;//cMath::Max(2.0f, float(m_Args.toEye / 500.0f));
			AddCube(m, CrossCenter, cVec3(dCross, d, d), cColor::Yellow);
			AddCube(m, CrossCenter, cVec3(d, dCross, d), cColor::Yellow);
			AddCube(m, CrossCenter, cVec3(d, d, dCross), cColor::Yellow);
			if(!cMath::IsZero(m_Args.toEye)) {
				m.Draw(Surface::SFX_AUX);
			}
//			IRS->Flush();
//			AddLine(m, SurfVertex(m_Args.At.x - dCross, m_Args.At.y, m_Args.At.z),
//				SurfVertex(m_Args.At.x + dCross, m_Args.At.y, m_Args.At.z), cColor::Yellow.ToArgb(), d);
//			AddLine(m, SurfVertex(m_Args.At.x, m_Args.At.y - dCross, m_Args.At.z),
//				SurfVertex(m_Args.At.x, m_Args.At.y + dCross, m_Args.At.z), cColor::Yellow.ToArgb(), d);
//			AddLine(m, SurfVertex(m_Args.At.x, m_Args.At.y, m_Args.At.z - dCross),
//				SurfVertex(m_Args.At.x, m_Args.At.y, m_Args.At.z + dCross), cColor::Yellow.ToArgb(), d);
//			SurfRef r;
//			m.GetRefAll(r, SR_TRIS);
//			SelExt::DrawSwatch(m);
//			r.Draw(Surface::SFX_AUX);
		}

		GetCursorPos(&pt);
		bool fLeft = false;///*GetKeyState(VK_LEFT) < 0 || */pt.x == 0;
		bool fRight = false;///*GetKeyState(VK_RIGHT) < 0 || */pt.x == RealLx - 1;
		bool fUp = false;///*GetKeyState(VK_UP) < 0 || */pt.y == 0;
		bool fDown = false;///*GetKeyState(VK_DOWN) < 0 || */pt.y == RealLy - 1;
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
	const bool Slowly = GetKeyState(VK_CONTROL) < 0;
	const float Speed = Slowly ? 0.0005f : 0.003f;
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
	const Vector3D dir = ICam->GetDirection();
	const dVector Dir(dir.x, dir.y, dir.z);
	bool ShiftSimultaneously = GetKeyState(VK_SHIFT) < 0;
	const bool ShiftSlowly = GetKeyState(VK_CONTROL) < 0;

	double d = double(dx + dy) * (ShiftSlowly ? 0.2 : 6.0);

	if(ShiftSimultaneously) {
		m_Args.At -= d * Dir;
	} else {
		m_Args.toEye += d;
		
		const double toEyeMin = 0.0;
		if(m_Args.toEye < toEyeMin) {
			double r = toEyeMin - m_Args.toEye;
			m_Args.toEye = toEyeMin;
			m_Args.At += r * Dir;
		}
		cMath::Clamp(m_Args.toEye, toEyeMin, cMath::DoubleMaxValue);
	}
}

//-----------------------------------------------------------------------------
// Navigator::Set
//-----------------------------------------------------------------------------
void Navigator::Set(const cVec3 &Pos, const cAngles &Orient, const float ToEye) {
//	assert(g_SurfEditor.FreeCamera);

	m_Args.Defaults();
	m_Args.At.Set(Pos.x, Pos.y, Pos.z);
	m_Args.Azim = cMath::Rad(Orient.Yaw);
	m_Args.Elev = cMath::Rad(Orient.Pitch) + cMath::Pi;
	m_Args.Roll = cMath::Rad(Orient.Roll);
	m_Args.toEye = ToEye;
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
	const Track::Segment &S = m_Track.Segments.GetBack();
	Set(S.Channels.Pos.GetValue(S.Channels.Pos.GetValuesCount() - 1), S.Channels.Orient.GetValue(S.Channels.Orient.GetValuesCount() - 1),
		S.Channels.ToEye.GetValue(S.Channels.ToEye.GetValuesCount() - 1));
	StopTrack();
	extern int FogMode;
	if(S.FogOfWar) {
		FogMode = 1;
	} else {
		FogMode = 0;
	}
	void RenderMinimap(); // To render fog of war tex.
	RenderMinimap();
}

//-----------------------------------------------------------------------------
// Navigator::Track::SetCameraParams
//-----------------------------------------------------------------------------
void Navigator::Track::SetCameraParams(const Navigator::Track::Segment &S) {
	m_CameraWasForced = S.ForceCameraParams && !S.DontPosCamera && !S.OnlyPosToFirstKey;
	m_SkyShiftWasOverridden = S.OverrideSkyShift;
	m_BlurWasOverridden = S.KeyableBlur;
	
	// If we force fog args but map doesn't have custom, we should
	// temporary enable using map args before overridding them!
	if(!MOptions.FogArgs.UseFromMap && (m_CameraWasForced || m_SkyShiftWasOverridden)) {
		m_FogArgsUseFromMapWasForced = true;
		MOptions.FogArgs.SetFromEngineSettings();
		MOptions.FogArgs.UseFromMap = true;
	} else {
		m_FogArgsUseFromMapWasForced = false;
	}

	if(m_CameraWasForced) {
		extern float CameraMovingStage;
		m_PrevStartFogNearPlane = MOptions.FogArgs.start_FogNearPlane;
		m_PrevStartFogFarPlane = MOptions.FogArgs.start_FogFarPlane;
		m_PrevFinalFogNearPlane = MOptions.FogArgs.final_FogNearPlane;
		m_PrevFinalFogFarPlane = MOptions.FogArgs.final_FogFarPlane;
		m_PrevCameraMovingStage = CameraMovingStage;
		m_PrevFogColor = MOptions.FogArgs.HorisonColorModulator;
		CameraMovingStage = S.CameraMovingStage;
		MOptions.FogArgs.start_FogNearPlane = S.StartFogNearPlane;
		MOptions.FogArgs.start_FogFarPlane = S.StartFogFarPlane;
		MOptions.FogArgs.final_FogNearPlane = S.FinalFogNearPlane;
		MOptions.FogArgs.final_FogFarPlane = S.FinalFogFarPlane;
		MOptions.FogArgs.HorisonColorModulator = S.FogColor;
	}
	if(m_SkyShiftWasOverridden) {
		m_PrevSkyShift = MOptions.FogArgs.SkyShift;
		MOptions.FogArgs.SkyShift = S.SkyShift;
	}
	// Storing Blur:
	if(m_BlurWasOverridden) {
		m_PrevBlur = GSets.SVOpt.MotionBlurAlpha;
	}
	extern int FogMode;
	if(S.FogOfWar) {
		FogMode = 1;
	} else {
		FogMode = 0;
	}
}

//-----------------------------------------------------------------------------
// Navigator::Track::RestoreCameraParams
//-----------------------------------------------------------------------------
void Navigator::Track::RestoreCameraParams() {
	if(m_FogArgsUseFromMapWasForced) {
		MOptions.FogArgs.UseFromMap = false;
	}
	if(m_CameraWasForced) {
		extern float CameraMovingStage;
		CameraMovingStage = m_PrevCameraMovingStage;
		MOptions.FogArgs.start_FogNearPlane = m_PrevStartFogNearPlane;
		MOptions.FogArgs.start_FogFarPlane = m_PrevStartFogFarPlane;
		MOptions.FogArgs.final_FogNearPlane = m_PrevFinalFogNearPlane;
		MOptions.FogArgs.final_FogFarPlane = m_PrevFinalFogFarPlane;
		MOptions.FogArgs.HorisonColorModulator = m_PrevFogColor;
	}
	if(m_SkyShiftWasOverridden) {
		MOptions.FogArgs.SkyShift = m_PrevSkyShift;
	}
	if(m_BlurWasOverridden) {
		GSets.SVOpt.MotionBlurAlpha = m_PrevBlur;
	}
}

// NavigatorTrackOnLoadResource
void NavigatorTrackOnLoadResource(const char *FileName) {
	cStr Name(FileName);
	if(g_Navigator.m_Track.m_LoadedResources.IndexOfStr(Name) == -1) {
		g_Navigator.m_Track.m_LoadedResources.Add(Name);
	}
}

//-----------------------------------------------------------------------------
// Navigator::Play
//-----------------------------------------------------------------------------
void Navigator::Play(CameraTrack *pTrack, bool fCapture) {
	if(IsOnPause()) {
		Home();
		return;
	}
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
	NotEmptyKeysCounts.SetCount(SplinesCount, 0);

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
	CullSpline.SetCount(SplinesCount, true);
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

	m_Track.DurationMs = 0.0;
	// Creating segments:
	for(int nSpline = 0; nSpline < SplinesCount; nSpline++) {
		if(CullSpline[nSpline]) {
			continue;
		}
		
		const CameraSpline *pSpline = m_Track.pSrc->Splines[nSpline];
		Track::Segment S;

		S.DurationMs = cMath::Ms(pSpline->GetDurationSec() / pTrack->PlaySpeed);
		S.AccumTimeMs = S.DurationMs;
		if(m_Track.Segments.Count()) {
			S.AccumTimeMs += m_Track.Segments.GetBack().AccumTimeMs;
		}

		S.AnimFrameFm = pSpline->AnimFrameFm;
		S.AnimFrameTo = pSpline->AnimFrameTo;

		S.AccelTime = cMath::Ms(pSpline->AccelTime / pTrack->PlaySpeed);
		S.DecelTime = cMath::Ms(pSpline->DecelTime / pTrack->PlaySpeed);
		S.FadeInTime = cMath::Ms(pSpline->FadeInTime / pTrack->PlaySpeed);
		S.FadeOutTime = cMath::Ms(pSpline->FadeOutTime / pTrack->PlaySpeed);
		S.Closed = pSpline->Closed;
		S.SwapFadeInFadeOut = pSpline->SwapFadeInFadeOut;

		S.KeyableBlur = pSpline->KeyableBlur;
		
		S.ForceCameraParams = true;
		S.StartFogNearPlane = pSpline->FogNearPlaneStart;
		S.StartFogFarPlane = pSpline->FogFarPlaneStart;
		S.FinalFogNearPlane = pSpline->FogNearPlaneEnd;
		S.FinalFogFarPlane = pSpline->FogFarPlaneEnd;
		S.CameraMovingStage = pSpline->CameraMovingStage;
		S.FogColor = pSpline->FogColor;

		S.OverrideSkyShift = pSpline->OverrideSkyShift;
		S.SkyShift = pSpline->SkyShift;

		S.FogOfWar = pSpline->FogOfWar;

		S.EarlyShowUI = pSpline->EarlyShowUI;
		S.DontPosCamera = pSpline->DontPosCamera;
		S.OnlyPosToFirstKey = pSpline->OnlyPosToFirstKey;
		S.FirstKeyIsHeroPosWhenHeroIsSelected = false;//pSpline->FirstKeyIsHeroPosWhenHeroIsSelected;
		S.FirstKeyIsLastCamPosIfNoHero = pSpline->FirstKeyIsLastCamPosIfNoHero;
		if(S.FirstKeyIsHeroPosWhenHeroIsSelected || S.FirstKeyIsLastCamPosIfNoHero) { // Storing last cam args:
			S.LastAzim = g_Navigator.m_Args.Azim;
			S.LastMapX = fMapX;
			S.LastMapY = fMapY;
			S.LastToEyeEx = g_Navigator.m_Args.toEyeEx;
			extern float CameraMovingStage;
			S.LastCameraMovingStage = CameraMovingStage;
		}
		S.SkipableDuringPlayback = pSpline->SkipableDuringPlayback;

		// Link to Unit:
		S.EndAtDistance = pSpline->EndAtDistance;
		S.TotalUnitDistance = 0.0f;
		S.pUnit = NULL;
		if(pSpline->LinkPosToUnit) {
			for(int u = 0; u < MAXOBJECT; u++) {
				OneObject *pOO = Group[u];
				if(!pOO) {
					continue;
				}
				if(pOO->UniqID == pSpline->idUnit) {
					S.pUnit = pOO;
					break;
				}
			}
		}

		S.GroupName = "";
		if(pSpline->LinkPosToGroup) {
			S.GroupName.Copy(pSpline->GroupName.cpchar());
		}

		pSpline->GetChannels(S.Channels, pTrack->PlaySpeed, true);

		// Events:
		cList<CameraSpline::Event *> Events;
		pSpline->GetEvents(Events, pTrack->PlaySpeed);
		if(Events.Count()) {
			// Shift events (if spline is not first):
			if(m_Track.Segments.Count() > 0) {
				for(int i = 0; i < Events.Count(); i++) {
					Events[i]->TimeMs += m_Track.Segments.GetBack().AccumTimeMs;
				}
			}
			m_Track.Events.Append(Events);
			Events.Free(); // Leave contents.
		}
	
		m_Track.Segments.Add(S);
		m_Track.DurationMs += S.DurationMs;
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
	m_Track.nPrevSegment = -1;
	/*m_Track.nPrevSegment = */m_Track.nCurSegment = 0;
	m_Track.Repeat = pTrack->Repeat;
	m_Track.HideUI = pTrack->HideUI;
	m_Track.LeaveBlackScreen = pTrack->LeaveBlackScreen;
	m_Track.LeaveBlackMap = pTrack->LeaveBlackMap;
	m_Track.ShowFrame = pTrack->ShowFrame;
	m_Track.nFramesWasShown = 0;
	// UI:
	if(m_Track.HideUI) {
		GSets.CGame.ViewMask = 0; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		HideUI();
	}

	// Waterflood:
	m_Track.Waterflood = pTrack->Waterflood;
	m_Track.WaterlineFm = pTrack->WaterlineFm;
	m_Track.WaterlineTo = pTrack->WaterlineTo;

	m_Track.IsSkipping = false;
	m_Track.IsSkipped = false;
	m_Track.IsActive = true;


	m_Track.m_LoadedResources.Clear();
	for(int i = 0; i < m_Track.pSrc->LoadedResources.NValues; i++) {
		m_Track.m_LoadedResources.Add(cStr(m_Track.pSrc->LoadedResources[i]->cpchar()));
	}
	IRM->RegCallbackOnLockData(NavigatorTrackOnLoadResource);
	m_Track.TrackName = m_Track.pSrc->GetName();
	// Precaching resources:
	int id = -1;
	for(i = 0; i < m_Track.m_LoadedResources.Count(); i++) {
		cStr FileName = m_Track.m_LoadedResources[i];
		// Finding extension:
		cStr Ext;
		cList<char> lExt;
		lExt.Add(0);
		for(int s = FileName.Length() - 1; s >= 0; s--) {
			if(FileName[s] == '.') {
				break;
			}
			lExt.InsertAt(0, FileName[s]);
		}
		if(lExt.Count() > 1) {
			Ext.Copy(lExt.ToPtr());
			if(cStr::EqualsNoCase(Ext.ToCharPtr(), "dds") || cStr::EqualsNoCase(Ext.ToCharPtr(), "tga")) {
				id = IRS->GetTextureID(FileName.ToCharPtr());
				if(id > -1) {
					IRS->SetTexture(id);
				}
			} else if(cStr::EqualsNoCase(Ext.ToCharPtr(), "c2m")) {
				id = IMM->GetModelID(FileName.ToCharPtr());
				if(id > -1) {
					BaseMesh *pG = IMM->GetGeometry(id, 0);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Navigator::Track::CheckEvents
//-----------------------------------------------------------------------------
void Navigator::Track::CheckEvents(const float CurTimeMs) {
	while(Events.Count()) {
		const CameraSpline::Event *pEvent = Events[0];
		if(pEvent->TimeMs <= CurTimeMs) {
			// Exec event:
			be_CBOOL *pValue = g_beVAR_STORE.BOOL_LIST.GetByName(pEvent->VarName);
			if(!pValue) {
				cStr Msg("Event \"");
				Msg << pEvent->VarName << "\" == " << (pEvent->VarValue ? "true" : "false") << " with time " << cStr::FromFloat(pEvent->TimeMs) << " (Ms) ";
				Msg << "is not found.";
				g_Messager.ShowMessage(Msg);
			} else {
				pValue->SetValue(pEvent->VarValue);
			}

			delete Events[0];
			Events.RemoveAt(0);
		} else {
			break;
		}
	}
} // Navigator::Track::CheckEvents

//-----------------------------------------------------------------------------
// Navigator
//-----------------------------------------------------------------------------
void Navigator::UpdateTrack() {
	if(!m_Track.IsActive) {
		return;
	}

	double dTime = 0;
	const double CurTimeMs = cTimer::GetClockMilliseconds();

	// Trying to correct time if following unit in FIRST spline:
	cVec3 UnitPos;
	Track::Segment &UnitSegment = m_Track.Segments[0];
	if(UnitSegment.pUnit) {
		UnitPos.x = (float)UnitSegment.pUnit->RealX / 16.0f;
		UnitPos.y = (float)UnitSegment.pUnit->RealY / 16.0f;
		
		pUnit pu(UnitSegment.pUnit);
		const iVector2D iDest = pu.GetGlobalDesination();
		const cVec2 Dst((float)iDest.x, (float)iDest.y);

		if(Dst.IsZero() || cVec2::Equals(UnitPos.ToVec2(), Dst) || m_Track.nFramesWasShown < 2) {
			m_Track.StartTimeMs = CurTimeMs;
		} else {
			if(0.0f == UnitSegment.TotalUnitDistance) { // Calcing TotalUnitDistance:
				UnitSegment.TotalUnitDistance = cVec2::Distance(UnitPos.ToVec2(), Dst) - (float)UnitSegment.EndAtDistance;
			}
			
			const float CurUnitDistance = cVec2::Distance(UnitPos.ToVec2(), Dst) - (float)UnitSegment.EndAtDistance;
			const float CurUnitProgress = cMath::Clamp01(1.0f - CurUnitDistance / UnitSegment.TotalUnitDistance);

			const double TotalTimeMs = m_Track.Segments.GetBack().AccumTimeMs;
			if(CurUnitProgress < 1.0f && iDest.x != -1 && iDest.y != -1) {
				m_Track.StartTimeMs = CurTimeMs - (double)CurUnitProgress * TotalTimeMs;
			}
		}
		
		int R, CH, H;
		UnitSegment.pUnit->GetCylinder(R, CH, H);
		UnitPos.z = (int)UnitSegment.pUnit->RZ + UnitSegment.pUnit->OverEarth + CH;
	}

	if(UnitSegment.GroupName.Length()) {
		pGroup pg(UnitSegment.GroupName.ToCharPtr());
		const iVector2D gPos = pg.GetCenter();
		UnitPos.x = gPos.x;
		UnitPos.y = gPos.y;
		UnitPos.z = 0;
		if(pg._getamount()) {
			RawUnit ru = pg._getunit(0);
			int R, CH, H;
			ru.object()->GetCylinder(R, CH, H);
			UnitPos.z = (int)ru.object()->RZ + ru.object()->OverEarth + CH;
		}
	}

	if(m_Track.IsCapturing) {
		dTime = 1000.0 / Navigator::FPS;
	} else {
		dTime = CurTimeMs - (m_Track.StartTimeMs + m_Track.PlayedMs);
	}
	
	if(IsOnPause() || (m_Track.nFramesWasShown < 2)) {
		m_Track.StartTimeMs += dTime;
		dTime = 0;
	} else {
		m_Track.TrackWasStarted = true;
//		MOptions.HoldBlackScreenTillTrackStarts = false;
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

	for(; m_Track.nCurSegment < m_Track.Segments.Count(); m_Track.nCurSegment++) {
		if(m_Track.PlayedMs <= m_Track.Segments[m_Track.nCurSegment].AccumTimeMs) {
			break;
		}
	}

	// Closed Spline:
	if(m_Track.nPrevSegment != -1 && m_Track.nPrevSegment != m_Track.nCurSegment && m_Track.Segments[m_Track.nPrevSegment].Closed) {
		m_Track.nCurSegment = m_Track.nPrevSegment;
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
		if(!m_Track.Repeat || m_Track.IsSkipping) {
			FreeTrack();
			const Track::Segment &S = m_Track.Segments.GetBack();
			if(!S.DontPosCamera && !S.OnlyPosToFirstKey && !S.pUnit) {
				Set(S.Channels.Pos.GetValue(S.Channels.Pos.GetValuesCount() - 1), S.Channels.Orient.GetValue(S.Channels.Orient.GetValuesCount() - 1),
					S.Channels.ToEye.GetValue(S.Channels.ToEye.GetValuesCount() - 1));
			}
			if(g_SurfEditor.FreeCamera) {
				Toggle(false);
			} else {
//				extern float CameraMovingStage;
//				CameraMovingStage = m_TendToArgs.m_MovingStage;
//				m_Args.toEyeEx = m_TendToArgs.m_toEyeEx;
//				m_Args.Azim = m_TendToArgs.m_Azim;
			}
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
			m_Track.nPrevSegment = -1;
		}
	}
	
	Track::Segment &CurSegment = m_Track.Segments[m_Track.nCurSegment];
	if(m_Track.nPrevSegment != m_Track.nCurSegment) {
		m_Track.RestoreCameraParams();
		m_Track.SetCameraParams(CurSegment);
		// Early Show UI:
		if(m_Track.Segments[m_Track.nCurSegment].EarlyShowUI) {
			ShowUI();
		}
		if(m_Track.Segments[m_Track.nCurSegment].DontPosCamera) { // FreeCamera is not needed
			if(g_SurfEditor.FreeCamera) {
				g_SurfEditor.FreeCamera = false;
				g_Navigator.Toggle(false);
			}
		} else if(m_Track.Segments[m_Track.nCurSegment].OnlyPosToFirstKey) { // FreeCamera is needed for the first key only
			if(!g_SurfEditor.FreeCamera) {
				g_SurfEditor.FreeCamera = true;
				g_Navigator.Toggle(true);
			}
			const Track::Segment &S = m_Track.Segments[m_Track.nCurSegment];
			// Hero (if it is selected):
			cVec3 Pos = S.Channels.Pos.GetValue(S.Channels.Pos.GetValuesCount() - 1);
			bool WasSetToHero = false;
			if(S.FirstKeyIsHeroPosWhenHeroIsSelected) {
				OneObject * GetCentralUnit();
				const OneObject *pOB = GetCentralUnit();
				if(pOB) {
					const float Hx = (pOB->RealX / 16) / 32.0f;
					const float Hy = (pOB->RealY / 16 - GetTotalHeight(pOB->RealX / 16, pOB->RealY / 16)) / 32.0f;
					Pos.Set(Hx, Hy, 0.0f);
					WasSetToHero = true;
				}
			}
			Set(Pos, S.Channels.Orient.GetValue(S.Channels.Orient.GetValuesCount() - 1), S.Channels.ToEye.GetValue(S.Channels.ToEye.GetValuesCount() - 1));
			if(g_SurfEditor.FreeCamera) {
				g_SurfEditor.FreeCamera = false;
				g_Navigator.Toggle(false);
			}
			// Setting to last cam pos (in game camera):
			if(S.FirstKeyIsHeroPosWhenHeroIsSelected || S.FirstKeyIsLastCamPosIfNoHero) {
				if(!WasSetToHero) {
					fMapX = S.LastMapX;
					fMapY = S.LastMapY;
				}
				m_TendToArgs.m_Azim = m_Args.Azim = S.LastAzim;
				m_TendToArgs.m_toEyeEx = m_Args.toEyeEx = S.LastToEyeEx;
				extern float CameraMovingStage;
				m_TendToArgs.m_MovingStage = CameraMovingStage = S.LastCameraMovingStage;
			}
		} else {
			if(!g_SurfEditor.FreeCamera) {
				g_SurfEditor.FreeCamera = true;
				g_Navigator.Toggle(true);
			}
		}
	}

	m_Track.nPrevSegment = m_Track.nCurSegment;

	const float AccumTimeMs = m_Track.nCurSegment > 0 ? m_Track.Segments[m_Track.nCurSegment - 1].AccumTimeMs : 0.0f;
	
	cInterpolateAccelDecelLinear<float> tFunc;
	tFunc.Init(CurSegment.Channels.Pos.GetTimeStart(), CurSegment.AccelTime, CurSegment.DecelTime, CurSegment.Channels.Pos.GetDuration(),
		CurSegment.Channels.Pos.GetTimeStart(), CurSegment.Channels.Pos.GetTimeEnd());
	m_Track.AlphaFunc.Init(CurSegment.Channels.Pos.GetTimeStart() + AccumTimeMs, CurSegment.FadeInTime, CurSegment.FadeOutTime,
		CurSegment.Channels.Pos.GetDuration(), 0.0f, 1.0f);
	
	const float tf = tFunc.GetCurrentValue((float)m_Track.PlayedMs - AccumTimeMs);
	cVec3 CurPos = CurSegment.Channels.Pos.GetCurrentValue(tf);

	// Matching cam target with unit pos:
	if(CurSegment.pUnit || CurSegment.GroupName.Length()) {
		CurPos = UnitPos;
	}

	const cAngles CurOrient = CurSegment.Channels.Orient.GetCurrentValue(tf);
	const float CurToEye = CurSegment.Channels.ToEye.GetCurrentValue(tf);
	
	if(!m_Track.Segments[m_Track.nCurSegment].DontPosCamera && !m_Track.Segments[m_Track.nCurSegment].OnlyPosToFirstKey) {
		Set(CurPos, CurOrient, CurToEye);
	}

	// Blur:
	if(CurSegment.KeyableBlur) {
		const float Blur = cMath::Clamp01(CurSegment.Channels.Blur.GetCurrentValue(tf));
		GSets.SVOpt.MotionBlurAlpha = 1.0f - Blur;
	}
	
	// Waterflood:
	if(m_Track.Waterflood) {
		const float CurProgress = cMath::LerperClamp01(0.0f, (float)m_Track.DurationMs, (float)m_Track.PlayedMs);
		const float CurWaterline = cMath::Lerp((float)m_Track.WaterlineFm, (float)m_Track.WaterlineTo, CurProgress);
		IWater->SetWaterline(CurWaterline);
		IWater->SetBoilingCoef(1.0f);
		IWater->Refresh(IWater->CalcVisibleWaterRect());
	}

	// Events:
	m_Track.CheckEvents((float)m_Track.PlayedMs);

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

	// Esc press handler:
	if(m_Track.Segments[m_Track.nCurSegment].SkipableDuringPlayback) {
		if(GetKeyState(VK_ESCAPE) < 0 && !m_Track.IsSkipping) {
			m_Track.IsSkipping = true;
			const float SkipDurationMs = 1000.0f;
			m_Track.SkipFunc.Init((float)m_Track.PlayedMs, 0.0f, SkipDurationMs, SkipDurationMs, 0.0f, 1.0f);
		}
	}

	// Skip handler:
	if(m_Track.Segments[m_Track.nCurSegment].SkipableDuringPlayback && m_Track.IsSkipping && !m_Track.IsSkipped &&
		1.0f == m_Track.SkipFunc.GetCurrentValue((float)m_Track.PlayedMs)) {
			m_Track.IsSkipped = true;
			// Looking for first nonskipable spline:
			for(; m_Track.nCurSegment < m_Track.Segments.Count(); m_Track.nCurSegment++) {
				if(!m_Track.Segments[m_Track.nCurSegment].SkipableDuringPlayback) {
					break;
				}
			}
			if(m_Track.nCurSegment < m_Track.Segments.Count()) {
				double tStartMs = (double)m_Track.Segments[m_Track.nCurSegment].AccumTimeMs -
					(double)m_Track.Segments[m_Track.nCurSegment].DurationMs;
				m_Track.StartTimeMs = CurTimeMs - tStartMs;
				m_Track.PlayedMs = (float)tStartMs;
				UpdateTrack();
			}
		}
}

void Navigator::OnDrawOnMapAfterLandscape(){
	CameraTrack *Track = GetCurPlayingTrack();
	if(Track) {
		if(m_Track.nCurSegment >= 0 && m_Track.nCurSegment < m_Track.Segments.Count()) {
			const float AccumTimeMs = m_Track.nCurSegment > 0 ? m_Track.Segments[m_Track.nCurSegment - 1].AccumTimeMs : 0.0f;
			const float l = cMath::Lerper(0.0f, m_Track.Segments[m_Track.nCurSegment].Channels.Pos.GetDuration(), m_Track.PlayedMs - AccumTimeMs);
			Track->FrameFm = m_Track.Segments[m_Track.nCurSegment].AnimFrameFm;
			Track->FrameTo = m_Track.Segments[m_Track.nCurSegment].AnimFrameTo;
			Track->Slider = l;
		}
	}
	for(int i = 0; i < MOptions.CameraTracks.GetAmount(); i++) {
		MOptions.CameraTracks[i]->Track.ShowAnimation();
	}
}

} // Surface

// by vital
bool isFlood(){
	return g_Navigator.IsPlayingTrack() && g_Navigator.IsWaterflood();
}
