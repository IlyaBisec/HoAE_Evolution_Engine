#ifndef VIDEO_LAYER_H
#define VIDEO_LAYER_H

//#include "GP_Draw.h"
#include <math.h>
//#include "d3d8.h"
#include "SquareObject.h"

#include "Bink\rad.h"
#include "Bink\bink.h"

// -----------------------------------------------------------------------------------------------------------------------------
// Old Bink Player
// -----------------------------------------------------------------------------------------------------------------------------
class BinkPlayer{
public:
	bool Ready;
	byte* OutBuffer;
	int Lx,Ly;
	HBINK Bink;
	char FileName[64];
	bool Open(char* Name);
	void SetupBuffer(int Lx,int Ly);
	bool GetCurrentFrame();
	void Close();
	BinkPlayer();
	~BinkPlayer();
};
// -----------------------------------------------------------------------------------------------------------------------------
class VideoLayer{
public:
	int Texture;
	BaseMesh* BM;
	int Lx,Ly;
	VideoLayer();
	~VideoLayer();
	void ShowRGB_Buffer(byte* src,int Lx,int Ly,int x,int y,int SizeX,DWORD Diffuse,float sx=1.0f,float sy=1.0f);
};
bool PlayBink(char* Name,int PlayerID);
byte* GetNextBinkFrame(int& Lx,int& Ly,int PlayerID);
#define maxPar 12
struct FormSituation{
	int FormID;
	int UnitID;
	int FinalStateOrder;
	//0 -no order
	//1 -ORDER:MOVE motion
	//2 -ORDER:ATTMOVE motion with attack
	//3 -ORDER:RIFLE enable rifle attack
	//4 -ORDER:NORIFLE enable rifle attack
	//5 -ORDER:MELEE melee attack
	//6 -ORDER:GRENADE grenade attack
	//7 -ORDER:L1 enable line 1 attack
	//8 -ORDER:L2 enable line 2 attack
	//9 -ORDER:L3 enable line 3 attack
	//10-ORDER:DL1 disable line 1 attack
	//11-ORDER:DL2 disable line 2 attack
	//12-ORDER:DL3 disable line 3 attack
	//13-ORDER:TOLINE transform to line
	//15-ORDDR:TOKARE transform to kare
	//16-ORDER:TOMOTION transform to motion formation
	//17-ORDER:FIILFORM fill formation
	//18-ORDER:STANDGROUND make stand ground
	union{
		struct{
			int NShooters;
			int NMelee;
			int NDying;
			int NMoving;
			int NStanding;
			int NAttMoving;
			int NAttStanding;
			int Panic;
			int Tired;
			int NRecharging;
			int NPAttack;
			int NUAttack;
			int NFireReady;
			int DistToMainTarget;
			int IsStandGround;
			int Arsenal;
			int Food;//GEC
			int Morale;//GEC
		};
		int Direct[maxPar];
	};
};
#endif