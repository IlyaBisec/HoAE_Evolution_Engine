#include "NavExt.h"
#include "comms\comms.h"

//-----------------------------------------------------------------------------
// NavExt::OnMouseHandling : bool(int, int, bool, bool, int, int, bool)
//-----------------------------------------------------------------------------
bool NavExt::OnMouseHandling(int xScreen, int yScreen, bool &LMB, bool &RMB, int, int, bool) {
	static int xPrev = 0, yPrev = 0;

	extern bool g_fMMBIsDown;
	bool fTrack = g_fMMBIsDown;
	bool fTumble = LMB && GetKeyState(VK_MENU) < 0;
	bool fDolly = RMB && GetKeyState(VK_MENU) < 0;

	extern bool CheckIfNewTerrain(void);
	if(CheckIfNewTerrain() == false) fTumble = false;

	if(!fTrack && !fTumble && !fDolly) {
		Args.fNavMode = false;
		return false;
	}

	if(Args.fNavMode) {
		int dx = xPrev - xScreen;
		int dy = yPrev - yScreen;
		if(dx || dy) {
			if(NavArgs::InGame == Args.Type) {
				if(fTrack) InGame_Track(dx, dy);
				else if(fTumble) InGame_Tumble(xPrev, yPrev, xScreen, yScreen);
				else if(fDolly) InGame_Dolly(dx, dy);
			} else if(NavArgs::Adv == Args.Type) {
				if(fTrack) Adv_Track(xPrev, yPrev, xScreen, yScreen);
				else if(fTumble) Adv_Tumble(dx, dy);
				else if(fDolly) Adv_Dolly(dx, dy);
			}
		}
	} else {
		Args.fNavMode = true;
	}
	xPrev = xScreen;
	yPrev = yScreen;

	return true;
}

//-----------------------------------------------------------------------------
// NavExt::InGame_Track : void(int, int)
//-----------------------------------------------------------------------------
void NavExt::InGame_Track(int dx, int dy) {
	const float c_Speed = 0.15f;
	const float c_LModeSpeed = 2.0f * c_Speed;
	// CurSpeed:
	extern bool LMode;
	float CurSpeed = LMode ? c_LModeSpeed : c_Speed;

	Vector3D Dir;
	Dir = ICam->GetDir();
	Dir.z = 0;
	float fx = float(dx) * CurSpeed / 2.0f, fy = float(dy) * CurSpeed;
	float Dx = fx, Dy = fy;
	if(cUtils::NotZero(Dir.normalize())) {
		Dx = - fx * Dir.y - fy * Dir.x;
		Dy = fx * Dir.x - fy * Dir.y;
	}

	extern float fMapX;
	extern float fMapY;
	
	fMapX += Dx;
	fMapY += Dy;
}

//-----------------------------------------------------------------------------
// NavExt::InGame_Tumble : void(int, int, int, int)
//-----------------------------------------------------------------------------
void NavExt::InGame_Tumble(int xPrev, int yPrev, int xCur, int yCur) {
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
		if(r > RHole) Args.Azim -= Speed * R.z;
		else {
			float k = r / RHole;
			k *= k;
			Args.Azim -= 2.0f * Speed * k * R.z;
		}
	}
	// Considering shift along x axis
	A.set(xA, RSingular, 0);
	B.set(xB, RSingular, 0);
	A.normalize(); // length always > c_Epsilon
	B.normalize();
	R.cross(A, B);
	Args.Azim -= Speed * R.z;
}

//-----------------------------------------------------------------------------
// NavExt::InGame_Dolly : void(int, int)
//-----------------------------------------------------------------------------
void NavExt::InGame_Dolly(int dx, int dy) {
	Args.toEyeEx += 2 * (dx + dy);
}

//-----------------------------------------------------------------------------
// NavExt::Adv_Track : void(int, int, int, int)
//-----------------------------------------------------------------------------
void NavExt::Adv_Track(int xPrev, int yPrev, int xCur, int yCur) {
	const float Speed = 30.0f;
	Line3D l0 = ICam->GetPickRay(xPrev, yPrev);
	Line3D l1 = ICam->GetPickRay(xCur, yCur);
	Vector3D A = l0.Orig();
	Vector3D B = l1.Orig();
	Vector3D d = A - B;
	d *= Speed;
	Args.At += d;
}

//-----------------------------------------------------------------------------
// NavExt::Adv_Tumble : void(int, int)
//-----------------------------------------------------------------------------
void NavExt::Adv_Tumble(int dx, int dy) {
	const float Speed = 0.003f;
	Args.Azim -= Speed * float(dx);
	Args.Elev -= Speed * float(dy);
}

//-----------------------------------------------------------------------------
// NavExt::Adv_Dolly : void(int, int)
//-----------------------------------------------------------------------------
void NavExt::Adv_Dolly(int dx, int dy) {
	Args.toEyeEx += (dx + dy);
}

NavExt g_NavExt;