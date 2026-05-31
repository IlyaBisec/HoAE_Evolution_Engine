#include "..\COSSACKS2\common.h"
#include "..\IntExplorer\ParseRQ.h"
#include <DString.h>
#include "import.h"
#include "BasicDiploRelation.h"
#include "DIP_SimpleBuilding.h"
#include "DiplomacySystem.h"
#include "BattleDialogs.h"
#include "UnitInterface.h"
#include "CityPopulation.h"

bool ReadWinString(GFILE* F,char* STR,int Max);
CIMPORT
void ActivateDipDialog(char* request);
CIMPORT
void SendDipCommand(char* Data,int size);

