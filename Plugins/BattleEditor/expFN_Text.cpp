#include "stdafx.h"

void	DeclareteDLL_expFN_Text(){

	GI::AddFunctionToDeclarationList("TEXT_ShowHint");
	GI::AddParamsToFunctionInList("TEXT_ShowHint",	"STR"	, "strTEXT"	);
	GI::AddParamsToFunctionInList("TEXT_ShowHint",	"INT"	, "iTIME"	);

	GI::AddFunctionToDeclarationList("TEXT_CreateParamString");
	GI::AddParamsToFunctionInList("TEXT_CreateParamString",	"STR"	, "strDestTextID"	);
	GI::AddParamsToFunctionInList("TEXT_CreateParamString",	"STR"	, "strSourceTextID"	);
	GI::AddParamsToFunctionInList("TEXT_CreateParamString",	"ARRAY"	, "arrPARS"			);	

};

EDITOR_FUNCTION	TEXT_ShowHint						(__PARAMS__){
	__STR( strTEXT );
	__INT( iTIME   );
	
	GI::PrintConsoleMessage2( strTEXT.pchar(), iTIME );

	END_OK;
};

EDITOR_FUNCTION	TEXT_CreateParamString				(__PARAMS__){
	__STR	( strDestTextID   );
	__STR	( strSourceTextID );
	__ARRAY	( arrPARS		  );

	if ( GI::PrepareString( strDestTextID, strSourceTextID, arrPARS ) )	return 1;

	END_FAIL;
};

//////////////////////////////////////////////////////////////////////////




















