#ifndef ADDStruct_header
#define ADDStruct_header

#include "..\cossacks2\resfile.h"
#include <DString.h>
#include "maptemplates.h"
//#include "stdheader.h"


class CSelPicture{
public:
	CSelPicture(){ /*Enable=false;*/ fid=-1; sid=-1; }
	//bool Enable;
	DialogsSystem DS;
	int x,y,Lx,Ly;
	static int fid,sid;	// file, sprite
	void Init(int FileID);
	void Process();
	static bool OnClick(SimpleDialog* SD){ sid=SD->UserParam; return 0; }
};
struct FunctionsList
{
	char FName[64];
	VCall* Function;
};
class ADSStruct
{
public:
	ADSStruct();
	static DialogsSystem DS;
	static int SelectedElement;
	bool Active;
	static DialogsSystem Menu;
	static bool OnClick(SimpleDialog* SD);
	bool IsMenuInit;
	void InitMenu();
	void DrawProcess();
	static void LoadScreenFromFile(char* f, DialogsSystem* S);
	static void SaveScreenToFile(char* f, DialogsSystem* S);
	static int GetMiddleFrame(DString *From, char *Frame, DString *Rez);
	static int DS_NID;
	static int Command;
	void ProcessCommands();
	static CSelPicture SelPict;
	static DynArray<FunctionsList> OnUserClick;
	static VCall* GetFunctionByName(char* fname, DynArray<FunctionsList>*FL);
	static char* GetNameByFunction(VCall* ff, DynArray<FunctionsList> *FL);
	bool Property;
	DialogsSystem PropPanel;
};
#include "xmlQuote.h"


#endif