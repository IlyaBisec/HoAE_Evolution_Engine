#pragma once
#include "MapTemplates.h"
//////////////////////////////////////////////////////////////////////////

class cvi_Alert{
public:
	bool Init; // born
	int Type; // 0 - brigade, 1 - cannon, 2 - settlement, 3 - town center, 4 - battle zone
	int Status; // 0 - netral, 1 - look at me, 2 - seen, 3 - depending commander	
	int Level; // 0 - fow, 1 - !fow, 2 - fire distance, 3 - mele distance
	int LevelTime;
	int CommanderTime;
	int comID; // brig id
	int comSN; // brig color
	//void Process();
};