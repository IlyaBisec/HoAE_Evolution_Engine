#ifndef SpraySystem_def
#define SpraySystem_def

///#include "SquareObject.h"

// размеры одного квадрата в Cell
#define SquadLx 256
#define SquadLy 256

#define SquadSH 8

// размеры всей карты в квадратах
#define SprayLx 32
#define SprayLy 32

#define SpraySH 5

#define SprayNx (SquadLx>>SpraySH)
#define SprayNy (SquadLy>>SpraySH)

#define SprayNSH (SquadSH-SpraySH)

#define SprayN SprayNx*SprayNy

struct SprayCell{	
	byte R,G,B;//,A; // необходимое значение	
	byte r,g,b;//,a; // current
	float A;
	float a;

	bool Process(int x, int y);
};

class  SpraySquad{
public:
	void Init();
	SprayCell Cells[SprayN];
	
	bool Process(int x, int y);
};

extern int SquadN;

class SpraySystem{
public:
	void Clear(){ memset(this,0,sizeof(*this)); }
	SpraySystem(){ Clear(); }	
	void Free(){
		if(Squads){
			for(int i=0;i<SquadN;i++) if(Squads[i]){
				free(Squads[i]);
			}
			free(Squads);
		}
		if(DXSquare) free(DXSquare);
		Clear();
	}
	void Init();

	SpraySquad** Squads;//SquadN
	
	bool AddDust(int x, int y, byte r, byte g, byte b, float a);
	int GetSprayDust(int x,int y);
	SprayCell* GetSprayCell(int x,int y);

	SquareObject* DXSquare;

	void Process();
	void Draw();

};

extern SpraySystem SpraySys;

void InitSpraySystem();
void UnLoadSpraySystem();
void AddOneDust(int x, int y);
void DrawDust();

#endif //SpraySystem_def