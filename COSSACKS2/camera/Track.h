#pragma once
#include "..\stdheader.h"
#include "comms\comms.h"

class CameraSpline;
//-----------------------------------------------------------------------------
// CameraSplineKey
//-----------------------------------------------------------------------------
class CameraSplineKey : public BaseFunction {
public:
	float xAt, yAt, zAt, Azim, Elev, Roll, toEye, TimeShift, Tension, Continuity, Bias;
	float Blur;
	_str VarName;
	bool VarValue;
	Delimiter Separator;
	
	void SetEmpty();
	bool IsEmpty() const;

	__constructor(CameraSplineKey) {
		SetEmpty();
	}
	
	void CopyPostFXArgsFromGameSettings();
	void SetFromCurCamera();
	int GetId();

	float GetSplineTimeSec() const;
	float GetParentSplineDurationSec() const;
	float GetAnimFrame() const;
	CameraSpline * GetParentSpline();
	
	void RemoveSelf();

	class CameraSplineKeySet : public BaseFunction {
		void EvaluateFunction();
		SAVE(CameraSplineKeySet);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} Set;

	class CameraSplineKeyRemove : public BaseFunction {
	public:
		void EvaluateFunction();
		SAVE(CameraSplineKeyRemove);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} Remove;

	void SetToCamera();
	void SetToAnimations();
	void EvaluateFunction();
	bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
	bool CheckIfElementInvisible(int, void *, DWORD);

	SAVE(CameraSplineKey);
		REG_PARENT(BaseFunction);
		REG_AUTO(Set);
		REG_AUTO(Remove);

#define CameraSplineKey_DefTimeShift	1.0f
#define CameraSplineKey_DefTension		0.0f
#define CameraSplineKey_DefContinuity	0.0f
#define CameraSplineKey_DefBias			0.0f
		
		//REG_FSLIDER(TimeShift, 0.0f, 10.0f, CameraSplineKey_DefTimeShift);
		REG_MEMBER(_float, TimeShift);
		REG_AUTO(Separator);
		REG_FSLIDER(Tension, 0.0f, 1.0f, CameraSplineKey_DefTension);
		REG_FSLIDER(Continuity, 0.0f, 1.0f, CameraSplineKey_DefContinuity);
		REG_FSLIDER(Bias, 0.0f, 1.0f, CameraSplineKey_DefBias);
		REG_AUTO(Separator);

#define CameraSplineKey_DefBlur		0.0f
		
		REG_FSLIDER(Blur, 0.0f, 1.0f, CameraSplineKey_DefBlur);
		REG_METHOD(&CameraSplineKey::CopyPostFXArgsFromGameSettings);
		REG_AUTO(Separator);
		REG_MEMBER(_str, VarName);
		REG_MEMBER(_bool, VarValue);
		REG_AUTO(Separator);
		REG_MEMBER(_float, xAt);
		REG_MEMBER(_float, yAt);
		REG_MEMBER(_float, zAt);
		REG_MEMBER(_float, Azim);
		REG_MEMBER(_float, Elev);
		REG_MEMBER(_float, Roll);
		REG_MEMBER(_float, toEye);
	ENDSAVE;
}; // CameraSplineKey

//-----------------------------------------------------------------------------
// CameraSpline
//-----------------------------------------------------------------------------
	class CameraSpline : public BaseClass {
public:
	~CameraSpline();
	
	ClonesArray<CameraSplineKey> Keys;
	bool KeyableBlur;
	float AccelTime;
	float DecelTime;
	float FadeInTime;
	float FadeOutTime;
	bool SwapFadeInFadeOut;
	bool Closed;
	bool Skip;
	bool LinkPosToUnit;
	DWORD idUnit;
	int EndAtDistance;
	bool LinkPosToGroup;
	_str GroupName;
	bool Quake;
	float Amplitude;
	float SamplingPeriodMs;

	Delimiter Separator;
	int FogNearPlaneStart;
	int FogFarPlaneStart;
	int FogNearPlaneEnd;
	int FogFarPlaneEnd;
	DWORD FogColor;
	float CameraMovingStage;
	bool OverrideSkyShift;
	int SkyShift;
	bool FogOfWar;
	bool EarlyShowUI;
	bool DontPosCamera;
	bool OnlyPosToFirstKey;
	bool FirstKeyIsHeroPosWhenHeroIsSelected;
	bool FirstKeyIsLastCamPosIfNoHero;
	bool SkipableDuringPlayback;
	// Anim args:
	int AnimFrameFm;
	int AnimFrameTo;

	void Defaults();
	
	__constructor(CameraSpline) {
		Defaults();
	}
	
	int GetNotEmptyKeysCount() const;
	float GetDurationSec() const;
	void SetDurationSec(const float dSec); // Linear spline retime.

	struct Channels {
		cSplineTCB3 Pos;
		cSplineTCBA Orient;
		cSplineTCB1 ToEye;
		cSplineTCB1 Blur;
	};
	void GetChannels(Channels &Ch, const float PlaySpeed, const bool AllowResampling) const;

	struct Event {
		float TimeMs;
		cStr VarName;
		bool VarValue;
	};
	void GetEvents(cList<Event *> &Events, const float PlaySpeed) const;
	
	bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
	bool OnChangeMember(BaseClass *, void *, void *, const char *);
	
	class CameraSplineCopyIdFromSelUnit : public BaseFunction {
	public:
		void EvaluateFunction();
		SAVE(CameraSplineCopyIdFromSelUnit)
			REG_PARENT(BaseFunction);
		ENDSAVE;
	} CopyIdFromSelUnit;

	class CameraSplineShow : public BaseFunction {
	public:
		float DeltaTime;

		void Defaults();
		void EvaluateFunction();
		__constructor(CameraSplineShow) {
			Defaults();
		}

		SAVE(CameraSplineShow);
			REG_PARENT(BaseFunction);

#define CameraSplineShow_DefDeltaTime	0.05f

			REG_FSLIDER(DeltaTime, 0.01f, 0.2f, CameraSplineShow_DefDeltaTime);
		ENDSAVE;
	} Show;

	class CameraSplineSetConstantSpeed : public BaseClass {
	public:
		void Defaults();

		__constructor(CameraSplineSetConstantSpeed) {
			Defaults();
		}
		
		int DurationSec;
		float DurationFr;
		void SetDurationSec(const float dSec) {
			DurationSec = (int)dSec;
			DurationFr = dSec - DurationSec;
		}
		float GetDurationSec() const {
			return (float)DurationSec + DurationFr;
		}

		typedef enum { CSB_POS, CSB_ROT } eConstantSpeedBy;

		void SetConstantSpeed(const eConstantSpeedBy By);
		
		class CameraSplineSetConstantSpeedByPos : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(CameraSplineSetConstantSpeedByPos);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} ByPosition;

		class CameraSplineSetConstantSpeedByRot : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(CameraSplineSetConstantSpeedByRot);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} ByRotation;

		SAVE(CameraSplineSetConstantSpeed);
#define CameraSplineSetConstantSpeed_DefDurationSec	10
#define CameraSplineSetConstantSpeed_DefDurationFr	0.0f
			REG_ISLIDER(DurationSec, 0, 60, CameraSplineSetConstantSpeed_DefDurationSec);
			REG_FSLIDER(DurationFr, 0.0f, 1.0f, CameraSplineSetConstantSpeed_DefDurationFr);
			REG_AUTO(ByPosition);
			REG_AUTO(ByRotation);
		ENDSAVE;
	} SetConstantSpeed;

	class CameraSplineCopyFogArgsFromEngineSettings : public BaseFunction {
	public:
		void EvaluateFunction();
		SAVE(CameraSplineCopyFogArgsFromEngineSettings);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	} CopyFogArgsFromEngineSettings;

	SAVE(CameraSpline);
		REG_CLASS(CameraSplineKey);
		REG_AUTO(Separator);
		REG_AUTO(Keys);
		
		REG_AUTO(Separator);
		REG_MEMBER(_bool, KeyableBlur);
		REG_AUTO(Separator);
		REG_FSLIDER(AccelTime, 0.0f, 20.0f, 0.0f);
		REG_FSLIDER(DecelTime, 0.0f, 20.0f, 0.0f);
		REG_FSLIDER(FadeInTime, 0.0f, 10.0f, 0.0f);
		REG_FSLIDER(FadeOutTime, 0.0f, 10.0f, 0.0f);
		REG_MEMBER(_bool, SwapFadeInFadeOut);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, Closed);
		REG_MEMBER(_bool, Skip);
		REG_AUTO(Separator);

#define CameraSpline_DefFogNearPlaneStart	3100
#define CameraSpline_DefFogFarPlaneStart	4050
#define CameraSpline_DefFogNearPlaneEnd		1500
#define CameraSpline_DefFogFarPlaneEnd		4200
#define CameraSpline_DefCameraMovingStage	1.0f


		//REG_ISLIDER(FogNearPlaneStart, 100, 10000, CameraSpline_DefFogNearPlaneStart);
		//REG_ISLIDER(FogFarPlaneStart, 100, 10000, CameraSpline_DefFogFarPlaneStart);
		//REG_ISLIDER(FogNearPlaneEnd, 100, 5000, CameraSpline_DefFogNearPlaneEnd);
		//REG_ISLIDER(FogFarPlaneEnd, 100, 5000, CameraSpline_DefFogFarPlaneEnd);
		REG_MEMBER(_int, FogNearPlaneStart);
		REG_MEMBER(_int, FogFarPlaneStart);
		REG_MEMBER(_int, FogNearPlaneEnd);
		REG_MEMBER(_int, FogFarPlaneEnd);
		REG_MEMBER(_color, FogColor);
		REG_AUTO(CopyFogArgsFromEngineSettings);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, OverrideSkyShift);
		REG_MEMBER(_int, SkyShift);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, FogOfWar);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, EarlyShowUI);
		REG_MEMBER(_bool, DontPosCamera);
		REG_MEMBER(_bool, OnlyPosToFirstKey);
		REG_MEMBER(_bool, FirstKeyIsHeroPosWhenHeroIsSelected);
		REG_MEMBER(_bool, FirstKeyIsLastCamPosIfNoHero);
		REG_MEMBER(_bool, SkipableDuringPlayback);
		REG_AUTO(Separator);
		REG_FSLIDER(CameraMovingStage, 0.0f, 1.0f, CameraSpline_DefCameraMovingStage);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, LinkPosToUnit);
		REG_ISLIDER(EndAtDistance, 0, 2000, 0);
		REG_MEMBER(_DWORD, idUnit);
		REG_AUTO(CopyIdFromSelUnit);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, LinkPosToGroup);
		REG_MEMBER(_str, GroupName);
		REG_AUTO(Separator);
		REG_AUTO(SetConstantSpeed);
		REG_AUTO(Separator);
		REG_AUTO(Show);
//		REG_AUTO(Hide);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, Quake);

#define CameraSpline_DefAmplitude			10.0f
#define CameraSpline_DefSamplingPeriodMs	50.0f

		REG_FSLIDER(Amplitude, 0.0f, 100.0f, CameraSpline_DefAmplitude);
		REG_FSLIDER(SamplingPeriodMs, 1.0f, 100.0f, CameraSpline_DefSamplingPeriodMs);

		REG_AUTO(Separator);
		REG_MEMBER(_int, AnimFrameFm);
		REG_MEMBER(_int, AnimFrameTo);
	ENDSAVE;

	static void PreviewAndEdit(CameraSpline *pSpline);
}; // CameraSpline

class CameraTrack;
//-----------------------------------------------------------------------------
// CameraTrackUnitAnimation
//-----------------------------------------------------------------------------
class CameraTrackUnitAnimation : public BaseClass {
public:
	int idAnimation;
	int FrameFm;
	int FrameTo;
	Delimiter Separator;
	
	CameraTrack * GetParentTrack();
	void CalcFrameRangeFromAnimation();

	SAVE(CameraTrackUnitAnimation);
		REG_MEMBER(_ModelID, idAnimation);
		REG_MEMBER(_int, FrameFm);
		REG_MEMBER(_int, FrameTo);
		REG_AUTO(Separator);
		REG_METHOD(&CameraTrackUnitAnimation::CalcFrameRangeFromAnimation);
	ENDSAVE;

	__constructor(CameraTrackUnitAnimation);
}; // CameraTrackUnitAnimation

//-----------------------------------------------------------------------------
// CameraTrackUnitModel
//-----------------------------------------------------------------------------
class CameraTrackUnitModel : public BaseClass {
public:
	int idModel;
	Vector3D Position;
	bool OnGround;
	void MoveToScreenCenter();
	float Scale;
	float Angle;
	bool ShowPreview;
	DWORD TFactor;
	void ReloadModel();
	
	int StartFrame;
	ClonesArray<CameraTrackUnitAnimation> Animations;
	Delimiter Separator;
	
	SAVE(CameraTrackUnitModel);
		REG_MEMBER(_ModelID, idModel);
		REG_METHOD(&CameraTrackUnitModel::ReloadModel);
		REG_AUTO(Separator);
		REG_MEMBER(_float, Position.x);
		REG_MEMBER(_float, Position.y);
		REG_MEMBER(_float, Position.z);
		REG_MEMBER(_bool, OnGround);
		REG_METHOD(&CameraTrackUnitModel::MoveToScreenCenter);
		REG_FSLIDER(Scale, 0.1f, 3.0f, 1.0f);
		REG_FSLIDER(Angle, 0.0f, 360.0f, 0.0f);
		REG_AUTO(Separator);
		REG_MEMBER(_color, TFactor);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, ShowPreview);
		REG_AUTO(Separator);
		REG_MEMBER(_int, StartFrame);
		REG_CLASS(CameraTrackUnitAnimation);
		REG_AUTO(Animations);
	ENDSAVE;
	__constructor(CameraTrackUnitModel);
}; // CameraTrackUnitModel

//-----------------------------------------------------------------------------
// CameraTrack
//-----------------------------------------------------------------------------
class CameraTrack : public BaseClass {
public:
	ClonesArray<CameraSpline> Splines;

	// Animations:
	ClonesArray<CameraTrackUnitModel> Animations;
	int FrameFm;
	int FrameTo;
	float Slider;
	int CurFrame;
	float CurTime;
	bool FitWithCamera;
	float GetCurFrame() const;
	float GetCurTimeSec() const;
	float GetCurTimeShiftSec() const;
	float GetCurDurationSec() const;
	CameraSpline * GetSplineWithFrameRange(const int FrameFm, const int FrameTo);
	void AddSpline();
	void AddKey();
	void RetimeSpline();
	int AnimationsFPS;

	ClassArray<_str> LoadedResources;
	bool Repeat;
	bool HideUI;
	bool LeaveBlackScreen;
	bool LeaveBlackMap;
	bool ShowFrame;
	Delimiter Separator;
	bool Waterflood;
	int WaterlineFm;
	int WaterlineTo;
	float PlaySpeed;

#define CameraTrack_AnimationsFPSDef 30

	__constructor(CameraTrack) {
		PlaySpeed = 1.0f;
		AnimationsFPS = CameraTrack_AnimationsFPSDef;
	}
	
	void Play();
	void ShowAnimation();
	bool IsPlaying() const;
	void Stop();
	void Free();

	const cStr GetName() const;
	
	class CameraTrackPreview : public BaseFunction {
	public:
		void EvaluateFunction();
		SAVE(CameraTrackPreview);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} Preview;
	
	class CameraTrackCapture : public BaseFunction {
	public:
		void EvaluateFunction();
		SAVE(CameraTrackCapture);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} Capture;
	
	float GetDuration() const;
	bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
	bool ForceSimplification();
	bool OnChangeMember(BaseClass *, void *, void *, const char *);
	void ClearSkip();
	void SetSkip();

	SAVE(CameraTrack);
		REG_CLASS(CameraSpline);
		REG_CLASS(CameraTrackUnitModel);
		REG_AUTO(Preview);
		REG_AUTO(Capture);
		REG_MEMBER(_bool, Repeat);
		REG_MEMBER(_bool, HideUI);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, LeaveBlackScreen);
		REG_MEMBER(_bool, LeaveBlackMap);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, ShowFrame);
		REG_AUTO(Separator);
		REG_FSLIDER(PlaySpeed, 0.1f, 3.0f, 1.0f);
		REG_AUTO(Separator);
		REG_AUTO(Splines);
		REG_AUTO(Separator);
		REG_METHOD(&CameraTrack::SetSkip);
		REG_METHOD(&CameraTrack::ClearSkip);
		REG_AUTO(Separator);
        REG_AUTO(Animations);
		REG_AUTO(Separator);
		REG_MEMBER(_int, FrameFm);
		REG_MEMBER(_int, FrameTo);
		REG_FSLIDER(Slider, 0.0f, 1.0f, 0.0f);
		READONLY REG_MEMBER(_int, CurFrame);
		READONLY REG_MEMBER(_float, CurTime);
		REG_MEMBER(_bool, FitWithCamera);
		REG_METHOD(&CameraTrack::AddSpline);
		REG_METHOD(&CameraTrack::AddKey);
		REG_AUTO(Separator);
		REG_METHOD(&CameraTrack::RetimeSpline);
		REG_AUTO(Separator);
		REG_ISLIDER(AnimationsFPS, 1, 50, CameraTrack_AnimationsFPSDef);
		REG_AUTO(Separator);
		REG_MEMBER(_bool, Waterflood);

#define CameraTrack_DefWaterlineFm	0
#define CameraTrack_DefWaterlineTo	0

		REG_ISLIDER(WaterlineFm, 0, 1000, CameraTrack_DefWaterlineFm);
		REG_ISLIDER(WaterlineTo, 0, 1000, CameraTrack_DefWaterlineTo);
		REG_AUTO(Separator);
		REG_AUTO(LoadedResources);
	ENDSAVE;
}; // CameraTrack