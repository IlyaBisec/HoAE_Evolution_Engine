#pragma once
class ResDiscr{
public:
	bool Enabled;
	char Name[128];
	char ResIDS[32];
	ResDiscr();
};
void LoadRDS();
//extern int RESRC[8][8];
extern int RESADD[8][8];
//bool CheckCost(byte NI,byte ResID,word Amount);
extern ResDiscr RDS[8];
extern int OrderIcon[4];
extern int FREE_ORDER_ICON;
extern int STAND_GROUND_ICON;
extern int NOT_STAND_GROUND_ICON;
extern int STAND_GRUND_ORDER_ICON;
extern int ORDERS_LIST_ICON;
extern int EATT_X;
extern int EATT_Y;
extern int EATT_Icon;
extern int DATT_X;
extern int DATT_Y;
extern int DATT_Icon;
extern int GATT_X;
extern int GATT_Y;
extern int GATT_Icon;
extern int PATR_X;
extern int PATR_Y;
extern int PATR_Icon;
extern int SHOTMYUNITS_ICON;
extern int SHOTMYUNITS_X;
extern int SHOTMYUNITS_Y;
extern int KEEPMYUNITS_ICON;
extern int KEEPMYUNITS_X;
extern int KEEPMYUNITS_Y;
extern int ARMATTACK_X;
extern int ARMATTACK_Y;
extern int ARMATTACK_ICON;

extern int ANYATTACK_X;
extern int ANYATTACK_Y;
extern int ANYATTACK_ICON;
struct vsDiffLevel{
	int Index;
	char Name[64];
	int Enable;
};
struct SingleHero_Desc{
	int PortFileID;
	char* DescriptionID;
};
struct SingleAI_Desc{
	char* NationID;
	char* Message;
	char* Peasant_ID;
	int NPeas;
	int NLandAI;
	char** LandAI;
	int NWaterAI;
	char** WaterAI;	
	SingleHero_Desc Hero;
};
class AI_Description{
public:
	int NAi;
	int NComp;
	SingleAI_Desc* Ai;
	AI_Description();
	~AI_Description();
	void Init();
	//
	DynArray<vsDiffLevel> Diff; // difficulty level
};
extern AI_Description GlobalAI;