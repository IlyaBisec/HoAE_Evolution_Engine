#pragma once

#include "..\Surface\Surface.h"
#include "Track.h"

namespace Surface {

class Navigator : public GameExtension {
public:
	Navigator() {
		m_Track.IsActive = false;
		m_fForceDrawAt = false;
		m_fLinkToHero = false;
		m_ShowSpline = NULL;
		m_Track.LeaveBlackScreen = false;
		m_Track.TrackWasStarted = false;
	}

	enum Modes {
		M_TRACK = 1,
		M_TUMBLE = 2,
		M_DOLLY = 3,
		M_ROLL = 4
	};
	int Mode() {
		return m_Mode;
	}
	bool OnGameSaving(xmlQuote &);
	bool OnGameLoading(xmlQuote &);
	void OnDrawOnMapAfterLandscape();
	int m_Mode;
	bool m_fForceDrawAt;
	void LinkToHeroToggle();
	void SetAtToHero();
	OneObject *m_pHero;
	bool m_fLinkToHero;
	word m_HeroDir;
	void ControlHero();
	bool m_fCameraWasForced;
	bool m_fAddTurning;
    bool m_fDisableHeroManagement;
	bool m_PrevFogArgsUseFromMap;
	float m_pFinalFogNearPlane, m_pFinalFogFarPlane, m_pCameraMovingStage;
	int m_pMinRotator;
	bool m_fUnitIsGround;
	int m_UnitCylinderHeight;

	struct Args {
		// InGame:
		double toEyeEx;
		// InGame && Adv:
		double Azim;
		// Adv:
		dVector At;
		double toEye;
		double Elev;
		double Roll;
		double ExtraElev;
		double ExtraAzim;

		void Defaults() {
			toEyeEx = 0;
			Azim = 0;
			Elev = 0;
			At = dVector::Origin;
			toEye = 0;
			Roll = 0;
		}
	} m_Args;

	cList<Args> ArgsStack;
	void PushArgs() { ArgsStack.Add(m_Args); }
	void PopArgs() { m_Args = ArgsStack.GetBack(); ArgsStack.RemoveAt(ArgsStack.Count() - 1); }
	bool OnMapUnLoading();
	void OnGameStart();
	void OnEditorStart();

	//*************************************************************************
	// TendToArgs
	//*************************************************************************
	struct TendToArgs {
		bool m_RotationSpringIsActive;
		bool m_DollySpringIsActive;
		bool m_LiftSpringIsActive;

		void DollySpring(bool IsActive) {
			m_DollySpringIsActive = IsActive;
		}
		void RotationSpring(bool IsActive) {
			m_RotationSpringIsActive = IsActive;
		}

		void SetSprings(bool Rotation, bool Dolly, bool Lift) {
			m_RotationSpringIsActive = Rotation;
			m_DollySpringIsActive = Dolly;
			m_LiftSpringIsActive = Lift;
		}
		
		bool m_fWasDollied;

		float m_MovingStage;
		float m_toEyeEx;
		float m_Azim;
		
		void Control(int index, float d);
		void OnMouseWheel(int Delta);
		void OnInsertDelete(const float Sign);
		void OnPageUpPageDown(const float Sign);
		void Update();
		void Lift(const float Sign);
		void Dolly(const float Sign);
		void DollyNow(const float Sign);
		void Rotate(const float Sign);
		void Defaults();
		bool IsTendingToDefaults();
		TendToArgs() {
			Defaults();
		}

#define TendToArgs_DefMovingStage	0.0f
#define TendToArgs_DeftoEyeEx		0.0f
#define	TendToArgs_DefAzim			0.0f

	} m_TendToArgs;

	static double PackedAngleToRad(word PrecPackedAngle);
	static word RadToPackedAngle(double RadAngle);
	static bool IsOnPause();
	static void CalcArgsFromICam(double &Azim, double &Elev, double &toEye, dVector &At);
	void CopyToKey(CameraSplineKey *pKey);
	void CopyFromKey(const CameraSplineKey *pKey);
	void Set(const cVec3 &Pos, const cAngles &Orient, const float ToEye);

	void Copy(const SurfCamera &);
//	void Copy(const Matrix4D &);
	void Play(CameraTrack *, bool fCapture = false);

	void ShowSpline(CameraSpline *pSpline) { m_ShowSpline = pSpline; }
	void HideSpline() { m_ShowSpline = NULL; }
	bool IsShowingSpline(const CameraSpline *pSpline) const { return m_ShowSpline == pSpline; }

	void Toggle(bool FreeCamera);
	void Home();
	void CalcCamArgs(dVector *pPos, dVector *pDir, dVector *pRight, dVector *pUp);

	void OnGameEnd();
	void RewindTrackToEnd();
	void UpdateTrack();
	bool IsPlayingTrack() const {
		return m_Track.IsActive;
	}
	bool ShowInTrackFrame() const {
		return m_Track.IsActive && m_Track.ShowFrame;// && GetPlayingTrackMaxSegKeysCount() > 2;
	}
	bool IsWaterflood() const { 
		return m_Track.Waterflood; 
	}
	bool IsPlayingTrack(const CameraTrack *) const;
	CameraTrack * GetCurPlayingTrack() { return m_Track.IsActive ? m_Track.pSrc : NULL; }
	double GetPlayedMs() const { return m_Track.IsActive ? m_Track.PlayedMs : 0.0; }
	
	void StopTrack();
	void SetupCamera();

	bool OnMouseWheel(int);
	bool OnMouseHandling(int xScreen, int yScreen, bool &LMB, bool &RMB, int, int, bool);
	void OnDrawOnMapAfterFogOfWar();
	void OnDrawOnMapOverAll();
	void DrawEditorInterface();
	void LinkToUnitToggle();

	static void TrackHelper(const dPlane &Plane, int xPrev, int yPrev, int xCur, int yCur, dVector *pTrack);
	void InGame_Track(int xPrev, int yPrev, int xCur, int yCur);
	void InGame_Tumble(int xPrev, int yPrev, int xCur, int yCur);
	void InGame_Dolly(int dx, int dy);
	void Adv_Track(int xPrev, int yPrev, int xCur, int yCur);
	void Adv_Tumble(int dx, int dy);
	void Adv_Dolly(int dx, int dy);
	void Adv_Roll(int dx, int dy);
	dPlane m_TrackPlane;

	CameraSpline *m_ShowSpline;
	
	class Track {
	public:
		bool IsActive;
		bool IsCapturing;
		CameraTrack *pSrc;

		cList<CameraSpline::Event *> Events;
		void CheckEvents(const float CurTimeMs);
		
		struct Segment {
			CameraSpline::Channels Channels;
			bool KeyableBlur;
			
			float DurationMs;
			float AccelTime;
			float DecelTime;
			float FadeInTime;
			float FadeOutTime;
			bool SwapFadeInFadeOut;
			bool Closed;

			// Anim args:
			int AnimFrameFm;
			int AnimFrameTo;

			float AccumTimeMs;

			bool ForceCameraParams;
			float StartFogNearPlane;
			float StartFogFarPlane;
			float FinalFogNearPlane;
			float FinalFogFarPlane;
			float CameraMovingStage;
			DWORD FogColor;

			bool OverrideSkyShift;
			int SkyShift;
			bool FogOfWar;

			bool EarlyShowUI;
			bool DontPosCamera;
			bool OnlyPosToFirstKey;
			bool FirstKeyIsHeroPosWhenHeroIsSelected;
			bool SkipableDuringPlayback;
			
			OneObject *pUnit;
			float TotalUnitDistance;
			int EndAtDistance;

			cStr GroupName;

			bool FirstKeyIsLastCamPosIfNoHero;
			float LastAzim, LastMapX, LastMapY, LastToEyeEx, LastCameraMovingStage;
		};

		cList<Segment> Segments;
		int nPrevSegment, nCurSegment; // If they are different, then nCurSegment is not played yet!
		bool Repeat;
		bool HideUI;
		bool LeaveBlackScreen;
		bool LeaveBlackMap;
		bool ShowFrame;
		bool TrackWasStarted; // At least one....
		int nFramesWasShown; // Some hack during track...
		bool IsSkipping, IsSkipped;
		cInterpolateAccelDecelSine<float> SkipFunc;
		
		cInterpolateAccelDecelSine<float> AlphaFunc;
		
		double StartTimeMs, PlayedMs, DurationMs;
		
		int nFrame;
		char FolderToCaptureIn[MAX_PATH];

		void SetCameraParams(const Segment &);
		void RestoreCameraParams();

		cList<cStr> m_LoadedResources;
		cStr TrackName;

		bool Waterflood;
		int WaterlineFm, WaterlineTo;
	private:
		bool m_FogArgsUseFromMapWasForced;
		bool m_CameraWasForced;
		float m_PrevStartFogNearPlane;
		float m_PrevStartFogFarPlane;
		float m_PrevFinalFogNearPlane;
		float m_PrevFinalFogFarPlane;
		float m_PrevCameraMovingStage;
		DWORD m_PrevFogColor;

		// Blur:
		bool m_BlurWasOverridden;
		float m_PrevBlur;

		// Sky Shift:
		bool m_SkyShiftWasOverridden;
		int m_PrevSkyShift;
	} m_Track;

	float GetPlayingTrackDuration() const { return IsPlayingTrack() ? m_Track.Segments.GetBack().AccumTimeMs : 0.0f; }
	int GetPlayingTrackTotalKeysCount() const;
	int GetPlayingTrackMaxSegKeysCount() const;

	void ClearBlackScreen();
	bool OnCheatEntering(const char *);

	void HideUI(); // This func sets all flags needed to hide UI!
	void ShowUI(); // Clear all flags neede to hide UI!
	
	static const int FPS = 25;

	void FreeTrack();
	void MoveAt(double s);
};

} // Surface

extern Surface::Navigator g_Navigator;