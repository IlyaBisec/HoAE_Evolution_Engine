#pragma once
#include "MapTemplates.h"
#include "vui_Action.h"

//////////////////////////////////////////////////////////////////////////

class cvi_Oboz
{
public:
	cvi_Oboz(void);
	~cvi_Oboz(void);
};

// vui_Actions
regA_(cvi_Act_Oboz,vfS,
	ClassRef<GPPicture> ResPic;
	ClassRef<TextButton> ResAmount;
	ClassRef<GPPicture> OwnerPic;
	ClassRef<TextButton> OwnerName;
	SAVE(cvi_Act_Oboz) REG_PARENT(vui_Action); 
		REG_AUTO(ResPic); 
		REG_AUTO(ResAmount);
		REG_AUTO(OwnerPic);		
		REG_AUTO(OwnerName);
	ENDSAVE; 
);

//
CIMPORT void dipf_SetSelOboz(OneObject* OB, byte& ResID, int& ResAmount, byte& Owner);
CEXPORT char* GetPlayerName(byte Nat);
DWORD GetNatColor( int natIdx );