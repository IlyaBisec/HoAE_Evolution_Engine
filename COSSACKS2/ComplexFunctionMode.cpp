#include "stdheader.h"
#include "BE_HEADERS.h"

DWORD fn_BE_UO_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	if ( PrevCallHash==3 )	return PrevCallHash;
	
	out->add("----"		, 0,	NULL	);
	out->add("NOT"		, 1,	NULL	);
	out->add("-"		, 2,	NULL	);

	return 3;
};
DWORD fn_BE_US_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	if ( PrevCallHash==5 )	return PrevCallHash;
	
	out->add("----"		, 0,	NULL	);
	out->add("AND"		, 1,	NULL	);
	out->add("OR"		, 2,	NULL	);
	out->add("SUM"		, 3,	NULL	);
	out->add("-SUM"		, 4,	NULL	);

	return 5;
};
DWORD fn_BE_BO_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	if ( PrevCallHash==11 )	return PrevCallHash;
	
	out->add("AND"		, 0,	NULL	);
	out->add("OR"		, 1,	NULL	);
    out->add("-"		, 2,	NULL	);
	out->add("+"		, 3,	NULL	);
	out->add("*"		, 4,	NULL	);
	out->add(">"		, 5,	NULL	);
	out->add(">="		, 6,	NULL	);
	out->add("<"		, 7,	NULL	);
	out->add("<="		, 8,	NULL	);
	out->add("=="		, 9,	NULL	);
	out->add("!="		, 10,	NULL	);

	return 11;
};
DWORD fn_BE_BS_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	if ( PrevCallHash==3 )	return PrevCallHash;
	
	out->add("if (...) then ...", 0,	NULL	);
	out->add("while(...) do ...", 1,	NULL	);
	out->add("do ... while(...)", 2,	NULL	);

	return 3;
};
DWORD fn_BE_TO_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	if ( PrevCallHash==1 )	return PrevCallHash;
	
	out->add("----"		, 0,	NULL	);

	return 1;
};
DWORD fn_BE_TS_CFMODES_ENUM(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	if ( PrevCallHash==1 )	return PrevCallHash;
	
	out->add("----"		, 0,	NULL	);

	return 1;
};

void	be_REG_CLASS_CFMODES(){
	Enumerator* E = NULL;

	E=ENUM.Get("BE_UO_CFMODES_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_UO_CFMODES_ENUM));	

	E=ENUM.Get("BE_US_CFMODES_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_US_CFMODES_ENUM));	

	E=ENUM.Get("BE_BO_CFMODES_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_BO_CFMODES_ENUM));	

	E=ENUM.Get("BE_BS_CFMODES_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_BS_CFMODES_ENUM));	

	E=ENUM.Get("BE_TO_CFMODES_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_TO_CFMODES_ENUM));	

	E=ENUM.Get("BE_TS_CFMODES_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_BE_TS_CFMODES_ENUM));	
};