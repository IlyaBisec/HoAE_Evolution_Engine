#pragma once
extern int StratLx;
extern int StratLy;
extern int StratSH;
class CellInfo{
public:
#ifdef CONQUEST
	
#else
	word NSword;
	word NStrel;
	word NCaval;
	word NPush;
	word NMort;
	word NTow;
#endif
	word Total;
	word NUnits;
	short x;
	short y;
	word ArmyID;
	byte Changed;
};
#ifdef CONQUEST
class ArmyInfo{
public:
	byte MinX;
	byte MinY;
	byte MaxX;
	byte MaxY;
	word N;
	word NUnits;
	short xc;
	short yc;
};
#else
class ArmyInfo{
public:
	word NSword;
	word NStrel;
	word NCaval;
	word NPush;
	word NMort;
	word NTow;
	byte MinX;
	byte MinY;
	byte MaxX;
	byte MaxY;
	word N;
};
#endif

class GlobalArmyInfo{
public:
	word* ArmDistr;
	CellInfo* CIN;
	int NCIN;
	int MaxCIN;
	ArmyInfo* AINF;
	int NArmy;
	int MaxArmy;
	GlobalArmyInfo();
	~GlobalArmyInfo();
	void Clear();
	void ResearchArmyDistribution(byte NI);
	void Show(int x,int y);
};
#define MinCUnits 8