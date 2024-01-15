#ifndef __NavExt__
#define __NavExt__

#include "..\stdheader.h"
#include "..\GameExtension.h"
#include "Surface.h"

struct NavArgs {
	typedef enum { InGame, Adv } eType;
	eType Type;
	bool fNavMode;

	// InGame:
	float toEyeEx;
	// InGame && Adv:
	double Azim;
	// Adv:
	Vector3D At;
	double toEye;
	double Elev;

	void Defaults() {
		toEyeEx = 0.0f;
		Azim = 0.0f;
		At = Vector3D::null;
		toEye = 20.0;
		Elev = 0;//comms::Rad(45);
		Type = InGame;
		fNavMode = false;
	}

	NavArgs() {
		Defaults();
	}
};

class NavExt : public GameExtension {
public:
	NavArgs Args;
	virtual bool OnMouseHandling(int xScreen, int yScreen, bool &LMB, bool &RMB, int, int, bool);
private:
	void InGame_Track(int dx, int dy);
	void InGame_Tumble(int xPrev, int yPrev, int xCur, int yCur);
	void InGame_Dolly(int dx, int dy);
	void Adv_Track(int xPrev, int yPrev, int xCur, int yCur);
	void Adv_Tumble(int dx, int dy);
	void Adv_Dolly(int dx, int dy);
};

extern NavExt g_NavExt;

#endif // __NavExt__