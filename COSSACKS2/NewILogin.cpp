#include "stdheader.h"

void StartExplorer();
void FinExplorer();
void RunExplorer(int Index,char* ref,int x,int y,int x1,int y1);
void ProcessExplorer(int Index);
//CEXPORT int ItemChoose;

typedef char* fnGetAccessKey(int);
typedef void fnSetAccessKey(int,char*);
extern fnGetAccessKey* GetAccessKey;
extern fnSetAccessKey* SetAccessKey;

bool MCHOOSE(SimpleDialog* SD){
	ItemChoose=SD->UserParam;
	return true;
};
char ACCESS[16]="";
void SlowLoadPalette(LPCSTR lpFileName);
void ExplorerOpenRef(int Index,char* ref);
void ShowCentralMessage(char* Message,int GPIDX);
void DarkScreen();
#include "Dialogs\NewLogin.h"