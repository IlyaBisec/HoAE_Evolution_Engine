#include "stdheader.h"
#include "unitability.h"
#include "GameInterface.h"
#include "CurrentMapOptions.h"
#include ".\cvi_InGamePanel.h"
#include "Mesh\UniHash.h"
#include "QuadsRender.h"
#include "MapSprites.h"
void PrintTest(char ccc, word value  ){
char sim;
char val=value;
sprintf(sim,ccc,val);
AssignHint1(sim,1000);
};