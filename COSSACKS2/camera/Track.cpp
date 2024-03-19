#include "stdheader.h"
#include "..\Surface\Surface.h"
#include "Navigator.h"
#include "SelExt.h"
#include "CmdExt.h"
#include "..\GameInterface.h"
#include "..\CurrentMapOptions.h"
#include "IShadowManager.h"
#include "kContext.h"
#include "..\Surface\Messager.h"
#include "sgGeometry.h"

//*****************************************************************************
// CameraSplineKey
//*****************************************************************************

// CameraSplineKey::SetEmpty
void CameraSplineKey::SetEmpty() {
	xAt = cMath::FloatMaxValue;
	yAt = zAt = 0.0f;
	Azim = Elev = Roll = 0.0f;
	toEye = 0.0f;
	TimeShift = CameraSplineKey_DefTimeShift;
	Tension = CameraSplineKey_DefTension;
	Continuity = CameraSplineKey_DefContinuity;
	Bias = CameraSplineKey_DefBias;
	Blur = CameraSplineKey_DefBlur;
}

// CameraSplineKey::IsEmpty
bool CameraSplineKey::IsEmpty() const {
	return xAt == cMath::FloatMaxValue;
}

// CameraSplineKey::SetFromCurCamera
void CameraSplineKey::SetFromCurCamera() {
	if(g_SurfEditor.FreeCamera) {
		g_Navigator.CopyToKey(this);
	} else {
		double A, E, t;
		dVector At;
		Surface::Navigator::CalcArgsFromICam(A, E, t, At);
		xAt = float(At.x), yAt = float(At.y), zAt = float(At.z);
		Azim = float(A);
		Elev = float(E);
		Roll = 0.0f;
		toEye = float(t);
	}
	// Set parent spline moving stage
	ClonesArray<CameraSplineKey> *pKeys = dynamic_cast<ClonesArray<CameraSplineKey> *>(ParentBC);
	if(pKeys) {
		CameraSpline *pSpline = dynamic_cast<CameraSpline *>(pKeys->GetParent());
		if(pSpline) {
			extern float CameraMovingStage;
			pSpline->CameraMovingStage = cMath::Clamp01(CameraMovingStage);
			pSpline->FogNearPlaneStart = MOptions.FogArgs.start_FogNearPlane;
			pSpline->FogFarPlaneStart = MOptions.FogArgs.start_FogFarPlane;
			pSpline->FogNearPlaneEnd = MOptions.FogArgs.final_FogNearPlane;
			pSpline->FogFarPlaneEnd = MOptions.FogArgs.final_FogFarPlane;
			pSpline->FogColor = MOptions.FogArgs.HorisonColorModulator;
		}
	}
}

// CameraSplineKey::GetId
int CameraSplineKey::GetId() {
	ClonesArray<CameraSplineKey> *pKeys = dynamic_cast<ClonesArray<CameraSplineKey> *>(ParentBC);
	if(pKeys) {
		for(int i = 0; i < pKeys->NValues; i++) {
			if((*pKeys)[i] == this) {
				return i;
			}
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
// CameraSplineKey::RemoveSelf
//-----------------------------------------------------------------------------
void CameraSplineKey::RemoveSelf() {
	ClonesArray<CameraSplineKey> *pKeys = dynamic_cast<ClonesArray<CameraSplineKey> *>(ParentBC);
	assert(pKeys);
	if(!pKeys) {
		return;
	}
	int id = GetId();
	if(id <= -1) {
		return;
	}
	// Add this key time shift to the next (if exists):
	if(id > 0) { // Key is not first.
		if(id + 1 < pKeys->GetAmount()) {
			(*pKeys)[id + 1]->TimeShift += TimeShift;
		}
	}
	pKeys->DelElement(id);
} // ameraSplineKey::RemoveSelf

// CameraSplineKey::CameraSplineKeySet::EvaluateFunction
void CameraSplineKey::CameraSplineKeySet::EvaluateFunction() {
	CameraSplineKey *pKey = dynamic_cast<CameraSplineKey *>(ParentBC);
	assert(pKey);
	if(!pKey) return;

	int n = pKey->GetId();
	if(n <= -1) return;

	cStr Text("Are you sure you want to set key ");
	Text += n;
	Text += " ?";
	char Caption[] = "Confirm Set Key";
	
	if(MessageBox(g_hWnd, Text.ToCharPtr(), Caption, MB_YESNO) == IDYES) {
		pKey->SetFromCurCamera();
	}
}

// CameraSplineKey::CameraSplineKeyRemove::EvaluateFunction
void CameraSplineKey::CameraSplineKeyRemove::EvaluateFunction() {
	CameraSplineKey *pKey = dynamic_cast<CameraSplineKey *>(ParentBC);
	assert(pKey);
	if(!pKey) return;
	int n = pKey->GetId();
	if(n <= -1) return;

	cStr Text("Are you sure you want to remove key ");
	Text += n;
	Text += " ?";
	char Caption[] = "Confirm Remove Key";
	if(MessageBox(g_hWnd, Text.ToCharPtr(), Caption, MB_YESNO) == IDYES) {
		pKey->RemoveSelf();
	}
}

//-----------------------------------------------------------------------------
// CameraSplineKey::GetSplineTimeSec
//-----------------------------------------------------------------------------
float CameraSplineKey::GetSplineTimeSec() const {
	ClonesArray<CameraSplineKey> *Keys = dynamic_cast<ClonesArray<CameraSplineKey> *>(ParentBC);
	if(!Keys) {
		return 0.0f;
	}
	float SplineTimeSec = 0.0f;
	for(int k = 0; k < Keys->GetAmount(); k++) {
		if(k > 0) {
			SplineTimeSec += (*Keys)[k]->TimeShift;
		}
		if((*Keys)[k] == this) {
			break;
		}
	}
	return SplineTimeSec;
} // CameraSplineKey::GetSplineTimeSec

//-----------------------------------------------------------------------------
// CameraSplineKey::GetParentSplineDurationSec
//-----------------------------------------------------------------------------
float CameraSplineKey::GetParentSplineDurationSec() const {
	ClonesArray<CameraSplineKey> *Keys = dynamic_cast<ClonesArray<CameraSplineKey> *>(ParentBC);
	if(!Keys) {
		return 0.0f;
	}
	float DurationSec = 0.0f;
	for(int k = 1; k < Keys->GetAmount(); k++) {
		DurationSec += (*Keys)[k]->TimeShift;
	}
	return DurationSec;
} // CameraSplineKey::GetParentSplineDurationSec

//-----------------------------------------------------------------------------
// CameraSplineKey::GetParentSpline
//-----------------------------------------------------------------------------
CameraSpline * CameraSplineKey::GetParentSpline() {
	ClonesArray<CameraSplineKey> *Keys = dynamic_cast<ClonesArray<CameraSplineKey> *>(ParentBC);
	if(!Keys) {
		return NULL;
	}
	CameraSpline *Spline = dynamic_cast<CameraSpline *>(Keys->GetParent());
	if(!Spline) {
		return NULL;
	}
	return Spline;
} // CameraSplineKey::GetParentSpline

//----------------------------------------------------------------------------------------
// CameraSplineKey::GetAnimFrame
//----------------------------------------------------------------------------------------
float CameraSplineKey::GetAnimFrame() const {
	ClonesArray<CameraSplineKey> *Keys = dynamic_cast<ClonesArray<CameraSplineKey> *>(ParentBC);
	if(!Keys) {
		return 0;
	}
	CameraSpline *Spline = dynamic_cast<CameraSpline *>(Keys->GetParent());
	if(!Spline) {
		return false;
	}
	
	const float SplineTimeSec = GetSplineTimeSec();
	const float ParentSplineDurationSec = GetParentSplineDurationSec();
	const float l = cMath::IsZero(ParentSplineDurationSec) ? 0.0f : SplineTimeSec / ParentSplineDurationSec;
	return cMath::Lerp((float)Spline->AnimFrameFm, (float)Spline->AnimFrameTo, l);
} // CameraSplineKey::GetAnimTimeSec

//-----------------------------------------------------------------------------
// CameraSplineKey::SetToAnimations
//-----------------------------------------------------------------------------
void CameraSplineKey::SetToAnimations() {
	// Set animation preview params (if any):
	ClonesArray<CameraSplineKey> *Keys = dynamic_cast<ClonesArray<CameraSplineKey> *>(ParentBC);
	if(!Keys) {
		return;
	}
	CameraSpline *Spline = dynamic_cast<CameraSpline *>(Keys->GetParent());
	if(!Spline) {
		return;
	}
	ClonesArray<CameraSpline> *Splines = dynamic_cast<ClonesArray<CameraSpline> *>(Spline->GetParent());
	if(!Splines) {
		return;
	}
	CameraTrack *Track = dynamic_cast<CameraTrack *>(Splines->GetParent());
	if(!Track) {
		return;
	}
	Track->FrameFm = Spline->AnimFrameFm;
	Track->FrameTo = Spline->AnimFrameTo;
	const float Delta = (float)Spline->AnimFrameTo - (float)Spline->AnimFrameFm;
	const float AnimFrame = GetAnimFrame();
	Track->Slider = Delta > cMath::Epsilon ? cMath::Lerper((float)Spline->AnimFrameFm, (float)Spline->AnimFrameTo, (float)AnimFrame) : 0.0f;
} // CameraSplineKey::SetToAnimations

//-----------------------------------------------------------------------------
// CameraSplineKey::SetToCamera
//-----------------------------------------------------------------------------
void CameraSplineKey::SetToCamera() {
	// Set camera preview params (if key is not empty):
	if(IsEmpty()) return;
	if(!g_SurfEditor.FreeCamera) {
		g_SurfEditor.FreeCamera = true;
		g_Navigator.Toggle(true);
	}
	g_Navigator.CopyFromKey(this);

	// Set camera moving stage from parent spline:
	extern float CameraMovingStage;
	ClonesArray<CameraSplineKey> *pKeys = dynamic_cast<ClonesArray<CameraSplineKey> *>(ParentBC);
	if(pKeys) {
		CameraSpline *pSpline = dynamic_cast<CameraSpline *>(pKeys->GetParent());
		if(pSpline) {
			extern float CameraMovingStage;
			CameraMovingStage = pSpline->CameraMovingStage;
			g_Navigator.m_TendToArgs.m_MovingStage = CameraMovingStage;
			// FogArgs:
			MOptions.FogArgs.UseFromMap = true;
			MOptions.FogArgs.start_FogNearPlane = pSpline->FogNearPlaneStart;
			MOptions.FogArgs.start_FogFarPlane = pSpline->FogFarPlaneStart;
			MOptions.FogArgs.final_FogNearPlane = pSpline->FogNearPlaneEnd;
			MOptions.FogArgs.final_FogFarPlane = pSpline->FogFarPlaneEnd;
			MOptions.FogArgs.HorisonColorModulator = pSpline->FogColor;
		}
	}
	SetupCamera();
} // CameraSplineKey::SetToCamera

// CameraSplineKey::EvaluateFunction
void CameraSplineKey::EvaluateFunction() {
	SetToAnimations();
	SetToCamera();
}

//-----------------------------------------------------------------------------
// CameraSplineKey::CheckIfElementInvisible
//-----------------------------------------------------------------------------
bool CameraSplineKey::CheckIfElementInvisible(int index, void *, DWORD) {
	// Blur - 8
	// CopyPosFXArgsFromGameSettings - 9
	// Separator - 10
	// VarName - 11
	// VarValue - 12
	// Separator - 13
	if(index > 13) {
		return true;
	}
	if(index < 8) {
		return false;
	}
	if(index > 10) {
		return false;
	}
	CameraSpline *Spline = GetParentSpline();
	assert(Spline);
	if(!Spline) {
		return true;
	}
	const bool CopyButtonIsVisible = Spline->KeyableBlur; // At least one param is keyable.
	switch(index) {
		case 8:
			return !Spline->KeyableBlur;
		case 9:
		case 10:
			return !CopyButtonIsVisible;
		default:
			return true;
	}
} // CameraSplineKey::CheckIfElementInvisible

// CameraSplineKey::GetClassPresentationMask
bool CameraSplineKey::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask  = "Key ";
	int n = GetId();
	if(n > -1) {
		Mask += n;
	}
	if(IsEmpty()) {
		Mask +=	" (Empty)";
	} else {
		Mask += " ";
		Mask += cStr::FromFloat(TimeShift).ToCharPtr();
		Mask += " sec";
	}
	return true;
}

//-----------------------------------------------------------------------------
// CameraSplineKey::CopyPostFXArgsFromGameSettings
//-----------------------------------------------------------------------------
void CameraSplineKey::CopyPostFXArgsFromGameSettings() {
	const char *Text = "Are you sure you want to copy post FX args from game settings?";
	const char *Caption = "Confirm Copy Post FX Args From Game Settings";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) != IDYES) {
		return;
	}
	Blur = 1.0f - GSets.SVOpt.MotionBlurAlpha;
} // CameraSplineKey::CopyPostFXArgsFromGameSettings

//*****************************************************************************
// CameraSpline
//*****************************************************************************

// CameraSpline::Defaults
void CameraSpline::Defaults() {
	AccelTime = DecelTime = 0.0f;
	FadeInTime = FadeOutTime = 0.0f;
	Skip = false;
	FogNearPlaneStart = CameraSpline_DefFogNearPlaneStart;
	FogFarPlaneStart = CameraSpline_DefFogFarPlaneStart;
	FogNearPlaneEnd = CameraSpline_DefFogNearPlaneEnd;
	FogFarPlaneEnd = CameraSpline_DefFogFarPlaneEnd;
	CameraMovingStage = CameraSpline_DefCameraMovingStage;
	FogColor = EngSettings.FogArgs.HorisonColorModulator;
	OverrideSkyShift = false;
	SkyShift = EngineSettings_DefSkyShift;
	EarlyShowUI = false;
	DontPosCamera = false;
	OnlyPosToFirstKey = false;
	FirstKeyIsHeroPosWhenHeroIsSelected = false;
	FirstKeyIsLastCamPosIfNoHero = false;
	SkipableDuringPlayback = true;
	idUnit = 0;

	Quake = false;
	Amplitude = CameraSpline_DefAmplitude;
	SamplingPeriodMs = CameraSpline_DefSamplingPeriodMs;
}

// CameraSpline::GetClassPresentationMask
bool CameraSpline::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Camera spline (";
	Mask += Keys.NValues;
	Mask += " keys, ";
	Mask += cStr::FromFloat(GetDurationSec(), 3).ToCharPtr();
	Mask += " sec";
	if(!(AnimFrameFm == AnimFrameTo && AnimFrameFm == 0)) {
		Mask += ", frames [";
		Mask += AnimFrameFm;
		Mask += ", ";
		Mask += AnimFrameTo;
		Mask += "]";
	}
	Mask += ")";
	return true;
}

// CameraSpline::OnChangeMember
bool CameraSpline::OnChangeMember(BaseClass *, void *, void *, const char *Name) {
	if(cStr::Equals(Name, "DontPosCamera")) {
		if(DontPosCamera) {
			OnlyPosToFirstKey = false;
		}
	}
	if(cStr::Equals(Name, "OnlyPosToFirstKey")) {
		if(OnlyPosToFirstKey) {
			DontPosCamera = false;
		}
	}
	if(!OnlyPosToFirstKey) {
		FirstKeyIsHeroPosWhenHeroIsSelected = false;
	}
	if(!OnlyPosToFirstKey) {
		FirstKeyIsLastCamPosIfNoHero = false;
	}
	return false;
}


// CameraSpline::GetMinMaxSec
/*void CameraSpline::GetMinMaxSec(float &tMin, float &tMax) const {
	tMin = 0.0f, tMax = 0.0f;
	for(int i = 0; i < Keys.NValues; i++) {
		if(Keys[i]->IsEmpty()) {
			continue;
		}
		tMin = Keys[i]->GetTimeSec();
		tMax = Keys[i]->GetTimeSec();
		break;
	}
	for(; i < Keys.NValues; i++) {
		tMin = cMath::Min(tMin, Keys[i]->GetTimeSec());
		tMax = cMath::Max(tMax, Keys[i]->GetTimeSec());
	}
}*/

//-----------------------------------------------------------------------------
// CameraSpline::GetNotEmptyKeysCount
//-----------------------------------------------------------------------------
int CameraSpline::GetNotEmptyKeysCount() const {
	int Count = 0;
	for(int k = 0; k < Keys.NValues; k++) {
		const CameraSplineKey *Key = Keys[k];
		// NULL pointer to key:
		assert(Key);
		if(!Key) {
			continue;
		}
		// Is not empty?
		if(!Key->IsEmpty()) {
			Count++;
		}
	}
	return Count;
} // CameraSpline::GetNotEmptyKeysCount

//-----------------------------------------------------------------------------
// CameraSpline::SetDurationSec
//-----------------------------------------------------------------------------
void CameraSpline::SetDurationSec(const float dSec) {
	const float CurDurationSec = GetDurationSec();
	if(cMath::IsZero(CurDurationSec)) {
		return;
	}
	const float TimeScale = dSec / CurDurationSec;
	for(int k = 0; k < Keys.NValues; k++) {
		CameraSplineKey *Key = Keys[k];
		// NULL pointer to key:
		assert(Key);
		if(!Key) {
			continue;
		}
		Key->TimeShift *= TimeScale;
	}
	SetConstantSpeed.SetDurationSec(dSec);
} // CameraSpline::SetDurationSec

// CameraSpline::GetDurationSec
float CameraSpline::GetDurationSec() const {
	float T = 0.0f;
	bool FirstTimeShiftWasSkiped = false;
	for(int i = 0; i < Keys.NValues; i++) {
		if(Keys[i]->IsEmpty()) {
			continue;
		}
		if(!FirstTimeShiftWasSkiped && !Closed) {
			FirstTimeShiftWasSkiped = true;
			continue;
		}
		T += Keys[i]->TimeShift;
	}
	return T;
}

// CameraSpline::CameraSplineSetConstantSpeed::Defaults
void CameraSpline::CameraSplineSetConstantSpeed::Defaults() {
	DurationSec = CameraSplineSetConstantSpeed_DefDurationSec;
	DurationFr = CameraSplineSetConstantSpeed_DefDurationFr;
}

// CameraSpline.dtor
CameraSpline::~CameraSpline() {
	if(g_Navigator.IsShowingSpline(this)) {
		g_Navigator.HideSpline();
	}
	//Hide.EvaluateFunction();
}

//---------------------------------------------------------------------------------
// CameraSpline::GetEvents
//---------------------------------------------------------------------------------
void CameraSpline::GetEvents(cList<Event *> &Events, const float PlaySpeed) const {
	Events.FreeContents();
	Events.Free();

	if(GetDurationSec() == 0.0f) {
		return;
	}
	bool FirstTimeShiftWasSkipped = false;
	float tCurMs = 0.0f;
	for(int nKey = 0; nKey < Keys.NValues; nKey++) {
		const CameraSplineKey *pKey = Keys[nKey];
		// NULL pointer to key:
		assert(pKey);
		if(!pKey) {
			continue;
		}
		if(pKey->IsEmpty()) {
			continue;
		}
		if(!FirstTimeShiftWasSkipped) {
			FirstTimeShiftWasSkipped = true;
		} else {
			tCurMs += cMath::Ms(pKey->TimeShift) / PlaySpeed;
		}
		if(strlen(pKey->VarName.cpchar()) > 0) {
			Event *pEvent = new Event;
			pEvent->TimeMs = tCurMs;
			pEvent->VarName = pKey->VarName.cpchar();
			pEvent->VarValue = pKey->VarValue;
			Events.Add(pEvent);
			pEvent = NULL;
		}
	}
} // CameraSpline::GetEvents

//-----------------------------------------------------------------------------------------------------------------------------------------
// CameraSpline::GenSplines
//-----------------------------------------------------------------------------------------------------------------------------------------
void CameraSpline::GetChannels(CameraSpline::Channels &Ch, const float PlaySpeed /*= 1.0f*/, const bool AllowResampling /*= true*/) const {
	Ch.Pos.Free();
	Ch.Orient.Free();
	Ch.ToEye.Free();
	Ch.Blur.Free();
	if(GetDurationSec() == 0.0f) {
		return;
	}
	float FirstTimeShiftMs = 0.0f; // for closed splines
	bool FirstTimeShiftWasSkiped = false;
	float tCurMs = 0.0f;
	for(int nKey = 0; nKey < Keys.NValues; nKey++) {
		const CameraSplineKey *pKey = Keys[nKey];
		// NULL pointer to key:
		assert(pKey);
		if(!pKey) {
			continue;
		}
		if(pKey->IsEmpty()) {
			continue;
		}
		if(!FirstTimeShiftWasSkiped) {
			FirstTimeShiftMs = cMath::Ms(pKey->TimeShift) / PlaySpeed;
			FirstTimeShiftWasSkiped = true;
		} else {
			tCurMs += cMath::Ms(pKey->TimeShift) / PlaySpeed;
		}
		Ch.Pos.AddValue(tCurMs, cVec3(pKey->xAt, pKey->yAt, pKey->zAt), pKey->Tension, pKey->Continuity, pKey->Bias);
		Ch.Orient.AddValue(tCurMs, cAngles(cMath::Deg(pKey->Elev - cMath::Pi), cMath::Deg(pKey->Azim), cMath::Deg(pKey->Roll)), pKey->Tension, pKey->Continuity, pKey->Bias);
		Ch.ToEye.AddValue(tCurMs, pKey->toEye, pKey->Tension, pKey->Continuity, pKey->Bias);
		Ch.Blur.AddValue(tCurMs, pKey->Blur, pKey->Tension, pKey->Continuity, pKey->Bias);
	}

	// ReSampling for Quake (only Pos, Orient && ToEye):
	if(AllowResampling && Quake) {
		cSplineTCB3 rsPos;
		cSplineTCBA rsOrient;
		cSplineTCB1 rsToEye;
		const float tStart = Ch.Pos.GetTimeStart(), tEnd = Ch.Pos.GetTimeEnd();
		for(float CurTime = tStart; CurTime <= tEnd; CurTime += SamplingPeriodMs / PlaySpeed) {
			cVec3 CurPos = Ch.Pos.GetCurrentValue(CurTime);
			const cAngles CurOrient = Ch.Orient.GetCurrentValue(CurTime);
			const float CurToEye = Ch.ToEye.GetCurrentValue(CurTime);
			const float w = cMath::RandRange1(), h = cMath::RandRange1();
			cVec3 Right, Up;
			CurOrient.ToVectors(NULL, &Right, &Up);
			const cVec3 Dir = cVec3::Normalize(w * Right + h * Up);
			CurPos += Amplitude * Dir;
			rsPos.AddValue(CurTime, CurPos);
			rsOrient.AddValue(CurTime, CurOrient);
			rsToEye.AddValue(CurTime, CurToEye);
		}
		Ch.Pos.Copy(rsPos);
		Ch.Orient.Copy(rsOrient);
		Ch.ToEye.Copy(rsToEye);
	}

	if(Closed) {
		tCurMs += FirstTimeShiftMs;
		Ch.Pos.Setup(true, tCurMs);
		Ch.Orient.Setup(true, tCurMs);
		Ch.ToEye.Setup(true, tCurMs);
		Ch.Blur.Setup(true, tCurMs);
	}
} // CameraSpline::GenSplines

// CameraSpline::CameraSplineSetConstantSpeed::SetConstantSpeed
void CameraSpline::CameraSplineSetConstantSpeed::SetConstantSpeed(const eConstantSpeedBy By) {
	CameraSpline *pSpline = dynamic_cast<CameraSpline *>(ParentBC);
	assert(pSpline);
	float DurationSec = pSpline->GetDurationSec();
	if(DurationSec == 0.0f) {
		return;
	}
	const char *Titles[] = { "position", "rotation" };
	char Text[128];
	sprintf(Text, "Are you sure you want to set constant speed by %s ?", Titles[By]);
	char Caption[128];
	sprintf(Caption, "Confirm Set Constant Speed By %s", Titles[By]);
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) != IDYES) {
		return;
	}
	cList<CameraSplineKey *> Keys;
	for(int i = 0; i < pSpline->Keys.NValues; i++) {
		if(!pSpline->Keys[i]->IsEmpty()) {
			Keys.Add(pSpline->Keys[i]);
		}
	}
	if(Keys.Count() < 2) {
		return;
	}
	if(Keys.Count() == 2) {
		Keys[1]->TimeShift = GetDurationSec();
	}
	cSplineTCB3 SPos;
	cSplineTCBA SRot;
	float AccumTime = 0.0f;
	for(i = 0; i < Keys.Count(); i++) {
		const CameraSplineKey &k = *Keys[i];
		if(i > 0) {
			AccumTime += k.TimeShift;
		}
		switch(By) {
			case CSB_POS:
				SPos.AddValue(AccumTime, cVec3(k.xAt, k.yAt, k.zAt), k.Tension, k.Continuity, k.Bias);
				break;
			case CSB_ROT:
				SRot.AddValue(AccumTime, cAngles(cMath::Deg(k.Elev - cMath::Pi), cMath::Deg(k.Azim), cMath::Deg(k.Roll)));
				break;
		}
	}
	switch(By) {
		case CSB_POS:
			SPos.SetConstantSpeed(GetDurationSec(), pSpline->Closed);
			for(i = 1; i < Keys.Count(); i++) {
				Keys[i]->TimeShift = SPos.GetTime(i) - SPos.GetTime(i - 1);
			}
			Keys[0]->TimeShift = GetDurationSec() - SPos.GetTime(SPos.GetValuesCount() - 1);
			break;
		case CSB_ROT:
			SRot.SetConstantSpeed(GetDurationSec(), pSpline->Closed);
			for(i = 1; i < Keys.Count(); i++) {
				Keys[i]->TimeShift = SRot.GetTime(i) - SRot.GetTime(i - 1);
			}
			Keys[0]->TimeShift = GetDurationSec() - SRot.GetTime(SRot.GetValuesCount() - 1);
			break;
	}
	if(!pSpline->Closed) {
		Keys[0]->TimeShift = 0.0f;
	}
}

// CameraSpline::CameraSplineCopyIdFromSelUnit::EvaluateFunction
void CameraSpline::CameraSplineCopyIdFromSelUnit::EvaluateFunction() {
	CameraSpline *pSpline = dynamic_cast<CameraSpline *>(ParentBC);

	OneObject *pOO = NULL;
	pImSelected ImSel(MyNation);
	if(ImSel._getamount()) {
		RawUnit ru = ImSel._getunit(0);
		pOO = ru.object();
		if(pOO) {
			const char *Text = "Are you sure you want to copy unit id from sel unit?";
			const char *Caption = "Confirm Copy Unit Id";
			if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
				pSpline->idUnit = pOO->UniqID;
			}
		}
	}
}

// CameraSpline::CameraSplineSetConstantSpeed::CameraSplineSetConstantSpeedByPos::EvaluateFunction
void CameraSpline::CameraSplineSetConstantSpeed::CameraSplineSetConstantSpeedByPos::EvaluateFunction() {
	dynamic_cast<CameraSpline::CameraSplineSetConstantSpeed *>(ParentBC)->SetConstantSpeed(CameraSpline::CameraSplineSetConstantSpeed::CSB_POS);
}

// CameraSpline::CameraSplineSetConstantSpeed::CameraSplineSetConstantSpeedByRot::EvaluateFunction
void CameraSpline::CameraSplineSetConstantSpeed::CameraSplineSetConstantSpeedByRot::EvaluateFunction() {
	dynamic_cast<CameraSpline::CameraSplineSetConstantSpeed *>(ParentBC)->SetConstantSpeed(CameraSpline::CameraSplineSetConstantSpeed::CSB_ROT);
}

// CameraSpline::CameraSplineShow::Defaults
void CameraSpline::CameraSplineShow::Defaults() {
	DeltaTime = CameraSplineShow_DefDeltaTime;
}

// CameraSpline::CameraSplineShow::EvaluateFunction
void CameraSpline::CameraSplineShow::EvaluateFunction() {
	CameraSpline *pSpline = dynamic_cast<CameraSpline *>(ParentBC);
	if(g_Navigator.IsShowingSpline(pSpline)) {
		g_Navigator.HideSpline();
	} else {
		g_Navigator.ShowSpline(pSpline);
	}
}

// CameraSpline::CameraSplineHide::EvaluateFunction
//void CameraSpline::CameraSplineHide::EvaluateFunction() {
//	g_Navigator.HideSpline();
//}

namespace Surface {
	void AddCube(SurfMesh &M, const cVec3 &Center, const cVec3 &Size, const cColor &Color);
	void AddLine(SurfMesh &M, const SurfVertex &l0, const SurfVertex &l1, DWORD argbColor, int d = 1);
}

// CameraSpline::PreviewAndEdit
void CameraSpline::PreviewAndEdit(CameraSpline *pSpline) {
	// Spline Preview:
	static cVec2 PrevMousePos(0);
	static int IndexClicked(-1);
	bool HotKeys = GetKeyState(VK_LBUTTON) < 0 && !(GetKeyState(VK_MENU) < 0);
	if(!HotKeys) {
		PrevMousePos.SetZero();
		IndexClicked = -1;
	}
	if(!pSpline) {
		return;
	}
	POINT pt;
	GetCursorPos(&pt);
	const cVec2 MousePos(pt.x, pt.y);
	// Translating:
	if(IndexClicked > -1) {
		// Searching key:
		int index = 0;
		for(int i = 0; i < pSpline->Keys.NValues; i++) {
			if(pSpline->Keys[i]->IsEmpty()) {
				continue;
			}
			if(index == IndexClicked) {
				break;
			}
			index++;
		}
		if(index == IndexClicked) {
			CameraSplineKey *pKey = pSpline->Keys[index];
			Frustum F = ICam->GetFrustum();
			const Line3D l0 = ICam->GetPickRay(PrevMousePos.x, PrevMousePos.y);
			const Line3D l1 = ICam->GetPickRay(MousePos.x, MousePos.y);
			F.plNear.MoveToPoint(Vector3D(pKey->xAt, pKey->yAt, pKey->zAt));
			Vector3D p0, p1;
			F.plNear.intersect(l0, p0);
			F.plNear.intersect(l1, p1);
			const Vector3D d = p1 - p0;
			pKey->xAt += d.x;
			pKey->yAt += d.y;
			pKey->zAt += d.z;
		}
	}
	PrevMousePos = MousePos;

	CameraSpline::Channels Channels;
	pSpline->GetChannels(Channels, 1.0f, false);
	const float dt = cMath::Ms(pSpline->Show.DeltaTime);
	cList<cVec3> AtCenters, EyeCenters, Dirs;
	// Spline:
	g_Navigator.PushArgs();
	dVector Eye;
	for(float Ms = Channels.Pos.GetTimeStart(); Ms <= Channels.Pos.GetTimeEnd() + cMath::Epsilon; Ms += dt) {
		AtCenters.Add(Channels.Pos.GetCurrentValue(Ms));
		const cAngles A = Channels.Orient.GetCurrentValue(Ms);
		const float t = Channels.ToEye.GetCurrentValue(Ms);
		g_Navigator.Set(AtCenters.GetBack(), A, t);
		g_Navigator.CalcCamArgs(&Eye, NULL, NULL, NULL);
		EyeCenters.Add(cVec3((float)Eye.x, (float)Eye.y, (float)Eye.z));
	}
	g_Navigator.PopArgs();

	// Drawing path in non playing mode:
	if(!g_Navigator.IsPlayingTrack()) {
		using Surface::SurfMesh;
		using Surface::SurfVertex;
		SurfMesh T;
		for(int i = 0; i < AtCenters.Count(); i++) {
			AddCube(T, AtCenters[i], cVec3(10.0f), cColor::Red);
			AddCube(T, EyeCenters[i], cVec3(10.0f), cColor::Cyan);
		}
		T.Draw(Surface::SFX_AUX);
	}

	// Labels:
	Surface::CmdExt::MessageBoxArgs Args;
	Args.Align = Surface::CmdExt::A_SCREEN;
	cList<cVec2> Labels;
	cList<bool> WithinFrustum;
	const float Radius = 12.0f;
	int nOver = -1;
	for(int i = 0; i < Channels.Pos.GetValuesCount(); i++) {
		const cVec3 u = Channels.Pos.GetValue(i);
		Vector3D p(u.x, u.y, u.z);
		WithinFrustum.Add(WorldToClosestScreenPoint(p));
		Labels.Add(cVec2(p.x, p.y));
		Args.Screen = Labels.GetBack();
		if(WithinFrustum.GetBack()) {
			if(cVec2::Distance(Args.Screen, MousePos) <= Radius && (IndexClicked != nOver || IndexClicked == -1)) {
				nOver = i;
				Args.FaceColor = cColor::Red.ToBgra();
			} else {
				Args.FaceColor = cColor::Blue.ToBgra();
			}
		} else {
			Args.FaceColor = cColor::Brown.ToBgra();
		}
		cStr Str;
		Str << i;
		Surface::CmdExt::MessageBox(Str, Args);
	}
	if(nOver > -1 && HotKeys) {
		IndexClicked = nOver;
	}
}

// CameraSpline::CameraSplineCopyFogArgsFromEngineSettings::EvaluateFunction
void CameraSpline::CameraSplineCopyFogArgsFromEngineSettings::EvaluateFunction() {
	const char Text[] = "Are you sure you want to copy \"Camera Spline Fog Args\" from EngineSettings?";
	const char Caption[] = "Confirm Copy \"Camera Spline Fog Args\" From EngineSettings";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		CameraSpline *pSpline = dynamic_cast<CameraSpline *>(ParentBC);
		if(pSpline) {
			pSpline->FogNearPlaneStart = EngSettings.FogArgs.start_FogNearPlane;
			pSpline->FogFarPlaneStart = EngSettings.FogArgs.start_FogFarPlane;
			pSpline->FogNearPlaneEnd = EngSettings.FogArgs.final_FogNearPlane;
			pSpline->FogFarPlaneEnd = EngSettings.FogArgs.final_FogFarPlane;
			pSpline->FogColor = EngSettings.FogArgs.HorisonColorModulator;
		}
	}
}

//*****************************************************************************
// CameraTrack Engine The Best !!!!!
//*****************************************************************************

//-----------------------------------------------------------------------------
// CameraTrack::SetSkip
//-----------------------------------------------------------------------------
void CameraTrack::SetSkip() {
	const char Text[] = "Are you sure you want to set skip flag in all splines?";
	const char Caption[] = "Confirm Set Skip";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		for(int i = 0; i < Splines.NValues; i++) {
			Splines[i]->Skip = true;
		}
	}
} // CameraTrack::SetSkip

//-----------------------------------------------------------------------------
// CameraTrack::ClearSkip
//-----------------------------------------------------------------------------
void CameraTrack::ClearSkip() {
	const char Text[] = "Are you sure you want to clear skip flag in all splines?";
	const char Caption[] = "Confirm Clear Skip";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		for(int i = 0; i < Splines.NValues; i++) {
			Splines[i]->Skip = false;
		}
	}
} // CameraTrack::ClearSkip

// CameraTrack::ForceSimplification
bool CameraTrack::ForceSimplification() {
	return false;
}

// CameraTrack::GetDuration
float CameraTrack::GetDuration() const {
	float T = 0;
	const float TimeScale = 1.0f / PlaySpeed;
	for(int i = 0; i < Splines.NValues; i++) {
		if(Splines[i]->Skip) {
			continue;
		}
		T += Splines[i]->GetDurationSec() * TimeScale;
	}
	return T;
}

// CameraTrack::GetClassPresentationMask
bool CameraTrack::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	int SplinesCount = 0;
	float SplinesDuration = 0;
	const float TimeScale = 1.0f / PlaySpeed;
	for(int i = 0; i < Splines.NValues; i++) {
		const CameraSpline *S = Splines[i];
		if(S->Skip) {
			continue;
		}
		SplinesCount++;
		SplinesDuration += S->GetDurationSec() * TimeScale;
	}
	Mask = "Camera track (";
	Mask += SplinesCount;
	Mask += " splines, ";
	Mask += cStr::FromFloat(SplinesDuration, 3).ToCharPtr();
	Mask += " sec)";
	return true;
}

// CameraTrack::Play
void CameraTrack::Play() {
//	if(!g_SurfEditor.FreeCamera) {
//		g_SurfEditor.FreeCamera = true;
//		g_Navigator.Toggle(true);
//	}
	g_Navigator.Play(this);
}

// CameraTrack::IsPlaying
bool CameraTrack::IsPlaying() const {
	return g_Navigator.IsPlayingTrack(this);
}

// CameraTrack::Stop
void CameraTrack::Stop() {
	g_Navigator.StopTrack();
}

// CameraTrack::Free
void CameraTrack::Free() {
	g_Navigator.RewindTrackToEnd();
	g_Navigator.Toggle(false);
}

// CameraTrack::CameraTrackPreview::EvaluateFunction
void CameraTrack::CameraTrackPreview::EvaluateFunction() {
	CameraTrack *pTrack = dynamic_cast<CameraTrack *>(ParentBC);
	if(!pTrack) return;
	pTrack->Play();
}

// CameraTrack::CameraTrackCapture
void CameraTrack::CameraTrackCapture::EvaluateFunction() {
	char Text[] = "Are you sure you want to capture this track?";
	char Caption[] = "Confirm Capture Track";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		if(!g_SurfEditor.FreeCamera) {
			g_SurfEditor.FreeCamera = true;
			g_Navigator.Toggle(true);
			g_SurfEditor.ForceZNear = false;
		}
		// We should do this lame shit because when start capturing in hidden UI
		// window with nation colors may still be visible on the screen.....
		g_SurfEditor.Update();
		g_SurfEditor.HideUI = false;
		g_SurfEditor.Update();
		g_SurfEditor.HideUI = true;
		g_SurfEditor.m_fActive = false;
		g_SurfEditor.Update();
		// Essentially playing with capture:
		CameraTrack *pTrack = dynamic_cast<CameraTrack *>(ParentBC);
		if(!pTrack) return;
		g_Navigator.Play(pTrack, true);
	}
}

//-------------------------------------------------------------------------------
// CameraTrack::OnChangeMember
//-------------------------------------------------------------------------------
bool CameraTrack::OnChangeMember(BaseClass *, void *, void *, const char *Name) {
	// Anim Slider:
	if(cStr::Equals("Slider", Name) || cStr::Equals("AnimationsFPS", Name) || cStr::Equals("FitWithCamera", Name)) {
		CurTime = GetCurTimeSec();
		CurFrame = GetCurFrame();
		if(FitWithCamera && !g_Navigator.IsPlayingTrack()) {
			CameraSpline *Spline = GetSplineWithFrameRange(FrameFm, FrameTo);
			if(Spline) {
				// Searching first non empty key and set to camera (for cam args, like moving stage and etc.):
				bool IsSet = false;
				for(int nKey = 0; nKey < Spline->Keys.NValues; nKey++) {
					CameraSplineKey *Key = Spline->Keys[nKey];
					assert(Key);
					if(!Key) {
						continue;
					}
					if(!Key->IsEmpty()) {
						Key->SetToCamera();
						IsSet = true;
						break;
					}
				}
				if(IsSet && Spline->Keys.NValues >= 2) { // If !IsSet, there is no nonempty keys. So further processing makes no sense...
					// Set evaluated values as current:
					CameraSpline::Channels Channels;
					Spline->GetChannels(Channels, 1.0f, false);
					const cVec3 CurPos = Channels.Pos.GetCurrentValue(Slider * Channels.Pos.GetDuration());
					const cAngles CurOrient = Channels.Orient.GetCurrentValue(Slider * Channels.Orient.GetDuration());
					const float CurToEye = Channels.ToEye.GetCurrentValue(Slider * Channels.ToEye.GetDuration());
					g_Navigator.Set(CurPos, CurOrient, CurToEye);
					SetupCamera();
				}
			}
		}
	}

	if(cStr::Equals(Name, "LeaveBlackMap")) {
		if(LeaveBlackScreen) {
			LeaveBlackMap = false;
		}
	}
	if(cStr::Equals(Name, "LeaveBlackMap")) {
		if(LeaveBlackMap) {
			LeaveBlackScreen = false;
		}
	}
	return false;
} // CameraTrack::OnChangeMember

// CameraTrack::GetName
const cStr CameraTrack::GetName() const {
	NamedCameraTrack *pNamedTrack = dynamic_cast<NamedCameraTrack *>(ParentBC);
	return cStr(pNamedTrack->Name.cpchar());
}

//bool CameraTrack_PreviewModels = false;
//-----------------------------------------------------------------------------
// CameraTrack::PreviewModels
//-----------------------------------------------------------------------------
/*void CameraTrack::PreviewModels() {
	CameraTrack_PreviewModels = true;
	for(int m = 0; m < Animations.GetAmount(); m++) {
		const CameraTrackUnitModel *Model = Animations[m];
		if(!Model->ShowPreview) {
			continue;
		}
		ShowAnimation(cMath::Ms(Model->GetPreviewTimeSec()));
	}
	CameraTrack_PreviewModels = false;
} // CameraTrack::PreviewModels*/

// CameraTrack::GetCurFrame
float CameraTrack::GetCurFrame() const {
	return cMath::Lerp((float)FrameFm, (float)FrameTo, Slider);
}

// CameraTrack::GetCurDurationSec
float CameraTrack::GetCurDurationSec() const {
	const float TimeFmSec = (float)FrameFm / (float)AnimationsFPS;
	const float TimeToSec = (float)FrameTo / (float)AnimationsFPS;
	return cMath::Clamp(TimeToSec - TimeFmSec, 0.0f, cMath::Infinity);
}

// CameraTrack::GetCurTimeSec
float CameraTrack::GetCurTimeSec() const {
	const float TimeFmSec = (float)FrameFm / (float)AnimationsFPS;
	const float TimeToSec = (float)FrameTo / (float)AnimationsFPS;
	return cMath::Lerp(TimeFmSec, TimeToSec, Slider);
}

// CameraTrack::GetCurTimeShiftSec
float CameraTrack::GetCurTimeShiftSec() const {
	const float TimeFmSec = (float)FrameFm / (float)AnimationsFPS;
	const float TimeToSec = (float)FrameTo / (float)AnimationsFPS;
	return cMath::Lerp(0.0f, TimeToSec - TimeFmSec, Slider);
}

//-----------------------------------------------------------------------------------------
// CameraTrack::GetSplineWithFrameRange
//-----------------------------------------------------------------------------------------
CameraSpline * CameraTrack::GetSplineWithFrameRange(const int FrameFm, const int FrameTo) {
	const int DeltaFrame = FrameTo - FrameFm;
	cStr Msg;
	if(DeltaFrame < 1) { // Invalid range.
		Msg = "Invalid frame range [";
		Msg << FrameFm << ", " << FrameTo << "]";
		g_Messager.ShowMessage(Msg);
		return NULL;
	}
	cList<CameraSpline *> Sl;
	for(int nSpl = 0; nSpl < Splines.NValues; nSpl++) {
		assert(Splines[nSpl]); // NULL pointer to spline!
		if(!Splines[nSpl]) {
			continue;
		}
		if(Splines[nSpl]->AnimFrameFm == FrameFm && Splines[nSpl]->AnimFrameTo == FrameTo) { // Spline is found.
			Sl.Add(Splines[nSpl]);
		}
	}
	if(Sl.Count() == 1) {
		return Sl[0];
	}
	if(Sl.Count() >= 2) {
		Msg = "There are ";
		Msg << Sl.Count() << " splines with frame range [";
		Msg << FrameFm << ", " << FrameTo << "]. Cannot select one.";
		g_Messager.ShowMessage(Msg);
	} else {
		Msg = "There is no spline with frame range [";
		Msg << FrameFm << ", " << FrameTo << "]";
		g_Messager.ShowMessage(Msg);
	}
	return NULL;
} // CameraTrack::GetSplineWithFrameRange

//-----------------------------------------------------------------------------
// CameraTrackUnitAnimation::GetParentTrack
//-----------------------------------------------------------------------------
CameraTrack * CameraTrackUnitAnimation::GetParentTrack() {
	ClonesArray<CameraTrackUnitAnimation> *Animations = dynamic_cast<ClonesArray<CameraTrackUnitAnimation> *>(GetParent());
	assert(Animations);
	if(!Animations) {
		return NULL;
	}
	CameraTrackUnitModel *Model = dynamic_cast<CameraTrackUnitModel *>(Animations->GetParent());
	assert(Model);
	if(!Model) {
		return NULL;
	}
	ClonesArray<CameraTrackUnitModel> *Models = dynamic_cast<ClonesArray<CameraTrackUnitModel> *>(Model->GetParent());
	assert(Models);
	if(!Models) {
		return NULL;
	}
	CameraTrack *Track = dynamic_cast<CameraTrack *>(Models->GetParent());
	assert(Track);
	if(!Track) {
		return NULL;
	}
	return Track;
} // CameraTrackUnitAnimation::GetParentTrack

//-----------------------------------------------------------------------------
// CameraTrackUnitAnimation::CalcFrameRangeFromAnimation
//-----------------------------------------------------------------------------
void CameraTrackUnitAnimation::CalcFrameRangeFromAnimation() {
	CameraTrack *Track = GetParentTrack();
	if(!Track) {
		return;
	}

	const float DurationMs = IMM->GetAnimTime(idAnimation);
	const int Fm = 0;
	const int To = (int)(cMath::Sec(DurationMs) * Track->AnimationsFPS);

	cStr Msg("Are you sure you want to set calculated from animation frame range [");
	Msg << Fm << ", " << To << "]?";
	const char Caption[] = "Confirm Calc Frame Range From Animation";

	if(MessageBox(g_hWnd, Msg, Caption, MB_YESNO) == IDYES) {
		FrameFm = Fm;
		FrameTo = To;
	}
} // CameraTrackUnitAnimation::CalcFrameRangeFromAnimation

//-----------------------------------------------------------------------------
// CameraTrack::RetimeSpline
//-----------------------------------------------------------------------------
void CameraTrack::RetimeSpline() {
	CameraSpline *Spline = GetSplineWithFrameRange(FrameFm, FrameTo);
	if(!Spline) {
		return;
	}

	// Do we have enough keys?
	const int NKeys = Spline->GetNotEmptyKeysCount();
	cStr Msg;
	if(NKeys <= 1) {
		Msg = "Spline with frame range [";
		Msg << FrameFm << ", " << FrameTo << "] contains " << NKeys << " not empty keys. At least 2 is needed!";
		g_Messager.ShowMessage(Msg);
		return;
	}

	const float SplineDurationSec = Spline->GetDurationSec();
	const float CurDurationSec = GetCurDurationSec();
	if(cMath::IsZero(CurDurationSec)) {
		return;
	}

	// Message Box:
	Msg = "Are you sure you want to retime spline with frame range [";
	Msg << FrameFm << ", " << FrameTo << "] and duration " << cStr::FromFloat(SplineDurationSec) << " (sec) to duration ";
	Msg << cStr::FromFloat(CurDurationSec) << " (sec)?";
	const char Caption[] = "Confirm Retime Spline";

	if(MessageBox(g_hWnd, Msg, Caption, MB_YESNO) == IDYES) {
		Spline->SetDurationSec(CurDurationSec);
	}
} // CameraTrack::RetimeSpline

//-----------------------------------------------------------------------------
// CameraTrack::AddKey
//-----------------------------------------------------------------------------
void CameraTrack::AddKey() {
	CameraSpline *Spline = GetSplineWithFrameRange(FrameFm, FrameTo);
	if(!Spline) {
		return;
	}

	// Action args (we should either insert key at or simply added it):
	bool Insert = false, Add = false;
	int InsertAt = 0;
	float TimeShiftSec = 0.0f;

	const float SplineTimeShiftSec = GetCurTimeShiftSec();
	// Should we insert?
	float PrevTimeShiftSec = 0.0f;
	float CurTimeShiftSec = 0.0f;
	for(int k = 0; k < Spline->Keys.NValues; k++) {
		CameraSplineKey *CurKey = Spline->Keys[k];
		// NULL pointer to current key:
		assert(CurKey);
		if(!CurKey) {
			continue;
		}
		if(CurKey->IsEmpty()) {
			continue;
		}
		PrevTimeShiftSec = CurTimeShiftSec;
		if(k > 0) { // We should skip first key time shift.
			CurTimeShiftSec += CurKey->TimeShift;
		}
		if(SplineTimeShiftSec >= PrevTimeShiftSec && SplineTimeShiftSec <= CurTimeShiftSec) { // We should insert key here:
			Insert = true;
			InsertAt = k;
			TimeShiftSec = SplineTimeShiftSec - PrevTimeShiftSec;
			break;
		}
	}
	// No place for insert. Should we simply add?
	if(!Insert) {
		const float SplineDurationSec = Spline->GetDurationSec();
		if(SplineTimeShiftSec >= SplineDurationSec) {
			Add = true;
			TimeShiftSec = SplineTimeShiftSec - SplineDurationSec;
		}
	}
	if(!Insert && !Add) {
		return; // Don't know what to do....
	}

	// Message Box:
	cStr Text("Are you sure you want to ");
	if(Add) {
		Text << "add";
	} else {
		Text << "insert";
	}
	Text << " key ";
	if(Insert) {
		Text << "at index " << InsertAt << " ";
	}
	Text << "with time shift " << cStr::FromFloat(TimeShiftSec) << " (sec)?";
	cStr Caption("Confirm ");
	if(Add) {
		Caption << "Add";
	} else {
		Caption << "Insert";
	}
	Caption << " Key";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) != IDYES) {
		return;
	}

	// Adding or inserting new key:
	CameraSplineKey *Key = new CameraSplineKey;
	Key->SetFromCurCamera();
	Key->TimeShift = TimeShiftSec;
	// Set parent spline CameraMovingStage:
	extern float CameraMovingStage;
	Spline->CameraMovingStage = cMath::Clamp01(CameraMovingStage);
	if(Add) {
		Spline->Keys.Add(Key);
	} else {
		Spline->Keys.Insert(InsertAt, Key);
		// Subtract added key time shift from next key (if any):
		for(int k = InsertAt + 1; k < Spline->Keys.NValues; k++) {
			CameraSplineKey *NextKey = Spline->Keys[k];
			// NULL pointer:
			assert(NextKey);
			if(!NextKey) {
				continue;
			}
			if(NextKey->IsEmpty()) {
				continue;
			}
			// Yes. That's it - our valid next key:
			NextKey->TimeShift -= TimeShiftSec;
			break;
		}
	}
	Key = NULL;
} // CameraTrack::AddKey

//-----------------------------------------------------------------------------
// CameraTrack::AddSpline
//-----------------------------------------------------------------------------
void CameraTrack::AddSpline() {
	const int index = Splines.GetAmount();
	const float TimeFm = (float)FrameFm / (float)AnimationsFPS;
	const float TimeTo = (float)FrameTo / (float)AnimationsFPS;
	const float Duration = TimeTo - TimeFm;

	cStr Text("Are you sure you want to add spline ");
	Text << index << " from frame ";
	Text << FrameFm << " to frame " << FrameTo << " with duration " << cStr::FromFloat(Duration, 2) << " (sec)?";
	const char Caption[] = "Confirm Add Spline";
	
	if(MessageBox(g_hWnd, Text.ToCharPtr(), Caption, MB_YESNO) == IDYES) {
		Splines.Add(new CameraSpline);
		Splines[index]->SetConstantSpeed.SetDurationSec(Duration);
		Splines[index]->AnimFrameFm = FrameFm;
		Splines[index]->AnimFrameTo = FrameTo;
		Splines[index]->Keys.Add(new CameraSplineKey);
		Splines[index]->Keys.Add(new CameraSplineKey);
		Splines[index]->Keys[0]->SetFromCurCamera();
		Splines[index]->Keys[1]->SetFromCurCamera();
		Splines[index]->Keys[0]->TimeShift = 0.0f;
		Splines[index]->Keys[1]->TimeShift = Duration;
		extern float CameraMovingStage;
		Splines[index]->CameraMovingStage = cMath::Clamp01(CameraMovingStage);
	}
} // CameraTrack::AddSpline

void CalcModelBoundary(DWORD idModel, const Matrix4D &W) {
	std::vector<AABoundBox> Bounds;
	std::vector<Matrix4D> Transforms;
	IMM->GetModelBounds(idModel, Bounds, Transforms);
	for(int i = 0; i < Bounds.size(); i++) {
		AABoundBox bb = Bounds[i];
		const Matrix4D &T = Transforms[i];
		if(GetKeyState(VK_SHIFT) < 0) {
			T.transformPt(bb.minv);
			T.transformPt(bb.maxv);
		}
		W.transformPt(bb.minv);
		W.transformPt(bb.maxv);
		DrawAABB(bb, 0x00000000, 0xffffffff);
	}
}

//-----------------------------------------------------------------------------
// CameraTrack::ShowAnimation
//-----------------------------------------------------------------------------
void CameraTrack::ShowAnimation() {
	const float TimeSec = GetCurTimeSec();
	
	// Enuming added models:
	for(int m = 0; m < Animations.GetAmount(); m++) {
		const CameraTrackUnitModel *Model = Animations[m];

		if(!IsPlaying() && !Model->ShowPreview) {
			continue;
		}

		// Should we start playing?
		const float StartTimeSec = (float)Model->StartFrame / (float)AnimationsFPS;
		if(TimeSec < StartTimeSec) {
			continue;
		}

		const float ModelTimeSec = TimeSec - StartTimeSec;
//		const float ModelFrame = (TimeSec - StartTimeSec) * (float)AnimationsFPS;

		// Enuming animations:
		//float TotalFrames = 0.0f;
		float TotalTimeSec = 0.0f;
		for(int a = 0; a < Model->Animations.GetAmount(); a++) {
			const CameraTrackUnitAnimation *Animation = Model->Animations[a];

			// Animation is empty.
//			if(IMM->GetAnimTime(Animation->idAnimation) <= 0.0f) {
//				continue;
//			}

			const float TimeFmSec = (float)Animation->FrameFm / (float)AnimationsFPS;
			const float TimeToSec = (float)Animation->FrameTo / (float)AnimationsFPS;
			const float DurationSec = TimeToSec - TimeFmSec;
			const float PrevTotalTimeSec = TotalTimeSec;
			TotalTimeSec += DurationSec;
			//const float Length = Animation->FrameTo - Animation->FrameFm;
			//TotalFrames += Length;

			// Should we play this anim seg?
			//if(ModelFrame > TotalFrames) {
			if(ModelTimeSec > TotalTimeSec) {
				continue;
			}

			const float SegTimeMs = cMath::Ms(ModelTimeSec - PrevTotalTimeSec + TimeFmSec);
			//const float SegFrame = Animation->FrameTo - (TotalFrames - ModelFrame);
			//const float SegTimeMs = cMath::Ms(SegFrame / CameraTrack_AnimationsFPSDef);
			
			Matrix4D T;
			T.srt(Model->Scale, Vector3D::oZ, cMath::Rad(Model->Angle), Model->Position);
			IRS->SetShaderConst(0, 10000); // What a hack?

			const DWORD Context = (DWORD)Animation;
			PushEntityContext(Context);
			IMM->StartModel(Model->idModel, T, CUR_CONTEXT);
			IMM->AnimateModel(Animation->idAnimation, SegTimeMs, 0);
			const DWORD tf = IRS->GetTextureFactor();
			IRS->SetTextureFactor(Model->TFactor);
			IMM->DrawModel();
			IRS->SetTextureFactor(tf);
			IShadowMgr->AddCaster(Model->idModel, T);
			IMM->RenderShadow(Model->idModel, &T);
			//CalcModelBoundary(Model->idModel, T);
			PopEntityContext();

			// W/o this hack shadows is not drawing correctly for case with single model / animation track.
			//IShadowMgr->AddCaster(0, Matrix4D::identity);

			break;
		}
	}
} // CameraTrack::ShowAnimation

// OneAnimation::MoveToScreenCenter
void CameraTrackUnitModel::MoveToScreenCenter() {
	Position = ScreenToWorldSpace(RealLx / 2, RealLy / 2);
	if(OnGround) {
		Position.z = GetHeight(Position.x, Position.y);
	} else {
		Position.z = 0.0f;
	}
}

// CameraTrackUnitAnimation::Init
void CameraTrackUnitAnimation::Init() {
}

// OneAnimation::Init
void CameraTrackUnitModel::Init() {
    Scale = 1.0f;
	OnGround = true;
	ShowPreview = true;
	TFactor = 0xff808080;
}

//-----------------------------------------------------------------------------
// ::ReloadModel
//-----------------------------------------------------------------------------
void CameraTrackUnitModel::ReloadModel() {
	if(idModel > 0) {
		const char Text[] = "Are you sure you want to reload model?";
		const char Caption[] = "Confirm Reload Model";
		if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
			for(int i = 0; i < Animations.GetAmount(); i++) {
				IMM->ReloadModel(Animations[i]->idAnimation);
			}
			IMM->ReloadModel(idModel);
		}
	}
} // ::ReloadModel