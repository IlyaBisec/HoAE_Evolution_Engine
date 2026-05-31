#ifndef __TankController__
#define __TankController__

#include "..\GameExtension.h"
#include "Surface.h"

namespace Surface {

    class TankController : public GameExtension {
    public:
        TankController() {
            m_fActive = false;
        }

        bool IsActive() { return m_fActive; }
        void Activate();
        void Free();
        void Toggle();

        bool CameraMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
        void SetupCamera();
		void OnGameEnd();
		void OnInitAfterMapLoading();
        OneObject* GetTank();
    private:
        bool m_fActive;
        void CalcCamArgs(dVector *pPos, dVector *pDir, dVector *pRight, dVector *pUp);
		dVector CalcAt();// const;
        struct {
            double Azim;
            double Elev;
            double toEye;
            dVector At;
			int dMouseX;
			int dMouseY;
        } m_Args;
        OneObject *m_pTank;
		word m_TankDir;
		byte m_TankUnitSpeed;

        int m_xCenter, m_yCenter;
        bool m_fUIWasHidden;
        bool m_fCameraWasForced;
        float m_pFinalFogNearPlane;
        float m_pFinalFogFarPlane;
        float m_pCameraMovingStage;
        int m_FogMode;
		bool m_fOnPause;

		float CurAzim();
		void CurAzim(float);

		float CurTankTurretBoneAzim();
		float CurTankGunBoneElev();
		void SelectTank();

//		static float PackedAngleToRad(WORD PrecisePackedAngle);
    };

} // Surface

extern Surface::TankController g_TankController;

#endif // __TankController__