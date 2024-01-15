#include "NavExt.h"
#include "comms\comms.h"

//-----------------------------------------------------------------------------
// NavArgs::CalcCam
//-----------------------------------------------------------------------------
void NavArgs::CalcCam() {
	// Ref:
	cVector c0 = cVector::Cartesian(1.0, Azim, Elev);
	cVector c1 = cVector::Cartesian(1.0, Azim, Elev - comms::Rad(1.0));
	cVector r0(c0.x, c0.z, c0.y);
	cVector r1(c1.x, c1.z, c1.y);
	cVector Ref = r1 - r0;
	Ref.Normalize();
	// Dir:
	cVector Dir = - r0;
	Dir.Normalize();
	// Pos:
	cVector Pos = At - toEye * Dir;
	// Right:
	cVector Right = Ref.Cross(Dir);
	Right.Normalize();
	// Up:
	cVector Up = Dir.Cross(Right);
	Up.Normalize();
	// Cam:
	Cam.Dir.set((float)Dir.x, (float)Dir.y, (float)Dir.z);
	Cam.Pos.set((float)Pos.x, (float)Pos.y, (float)Pos.z);
	Cam.Right.set(-(float)Right.x, -(float)Right.y, -(float)Right.z);
	Cam.Up.set((float)Up.x, (float)Up.y, (float)Up.z);
} // NavArgs::CalcCam

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
				if(fTrack) InGame_Track(xPrev, yPrev, xScreen, yScreen);
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
// NavExt::TrackHelper
//-----------------------------------------------------------------------------
void NavExt::TrackHelper(const cPlane &Plane, int xPrev, int yPrev, int xCur, int yCur, cVector *pTrack) {
	Line3D l0 = ICam->GetPickRay(xPrev, yPrev);
	Line3D l1 = ICam->GetPickRay(xCur, yCur);
	Vector3D O = l0.Orig();
	Vector3D D = l0.Orig();
	cVector CrossPrev;
	assert(Plane.RayIntersect(cVector(O.x, O.y, O.z), cVector(D.x, D.y, D.z), NULL, &CrossPrev));
	O = l1.Orig();
	D = l1.Dir();
	cVector CrossCur;
	assert(Plane.RayIntersect(cVector(O.x, O.y, O.z), cVector(D.x, D.y, D.z), NULL, &CrossCur));
	pTrack->Copy(CrossPrev - CrossCur);
} // NavExt::TrackHelper

//-----------------------------------------------------------------------------
// NavExt::InGame_Track : void(int, int, int, int)
//-----------------------------------------------------------------------------
void NavExt::InGame_Track(int xPrev, int yPrev, int xCur, int yCur) {
	extern float fMapX;
	extern float fMapY;

	UniversalMap::PickArgs pa(xPrev, yPrev);
	int z = 50;
	if(g_UniversalMap.Pick(&pa)) {
		const OneMapVertex &vtx = pa.RefTo.pOrigin->Vert[pa.VertsTo[0]];
		z = vtx.z;
	}
	cVector d;
	TrackHelper(cPlane(cVector::zAxis, z), xPrev, yPrev, xCur, yCur, &d);
	fMapX += (float)d.x / c_WorldGridStepX;
	fMapY += (float)d.y / c_WorldGridStepY;
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
// NavExt::Adv_Track
//-----------------------------------------------------------------------------
void NavExt::Adv_Track(int xPrev, int yPrev, int xCur, int yCur) {
/*	const float Speed = 30.0f;
	Line3D l0 = ICam->GetPickRay(xPrev, yPrev);
	Line3D l1 = ICam->GetPickRay(xCur, yCur);
	Vector3D A = l0.Orig();
	Vector3D B = l1.Orig();
	Vector3D d = A - B;
	d *= Speed * (1.0f + Args.toEye / 1000.0f);
	Args.At += cVector(d.x, d.y, d.z);*/
	UniversalMap::PickArgs pa(xPrev, yPrev);
	if(g_UniversalMap.Pick(&pa)) {
		const OneMapVertex &vtx = pa.RefTo.pOrigin->Vert[pa.VertsTo[0]];
		cPlane p(cVector(- Args.Cam.Dir.x, - Args.Cam.Dir.y, - Args.Cam.Dir.z), cVector(vtx.x, vtx.y, vtx.z));
		cVector d;
		TrackHelper(p, xPrev, yPrev, xCur, yCur, &d);
		Args.At += d;
		
	}
	Args.CalcCam();
} // NavExt::Adv_Track

//-----------------------------------------------------------------------------
// NavExt::Adv_Tumble
//-----------------------------------------------------------------------------
void NavExt::Adv_Tumble(int dx, int dy) {
	const float Speed = 0.003f;
	Args.Azim += Speed * float(dx);
	Args.Elev += Speed * float(dy);
	comms::AngleNormalize2Pi(Args.Azim);
	comms::AngleNormalizePi(Args.Elev);
	Args.CalcCam();
} // NavExt::Adv_Tumble

//-----------------------------------------------------------------------------
// NavExt::Adv_Dolly
//-----------------------------------------------------------------------------
void NavExt::Adv_Dolly(int dx, int dy) {
	Args.toEye += 6 * (dx + dy);
	const double toEyeMin = 3;
	comms::Clamp(Args.toEye, toEyeMin, comms::DoubleMax);
	Args.CalcCam();
} // NavExt::Adv_Dolly

NavExt g_NavExt;