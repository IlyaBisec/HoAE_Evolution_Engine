#include "comms.h"

#ifdef _DEBUG

//-------------------------------------------------------------------------------------------------
// cAssertDlg
//-------------------------------------------------------------------------------------------------
bool cAssertDlg(const char *Exp, const char *Msg, const char *File, int Line, bool &IgnoreAlways) {
	// Exe File Name:
	char ExeFilePathName[MAX_PATH];
	GetModuleFileName(NULL, ExeFilePathName, MAX_PATH);
	const char *ExeFileName = strrchr(ExeFilePathName, '\\');
	ExeFileName = ExeFileName != NULL ? ExeFileName + 1 : ExeFilePathName;
	
	// Source File Name:
	const char *SourceFileName = strrchr(File, '\\');
	SourceFileName = SourceFileName != NULL ? SourceFileName + 1 : File;
	
	// Log:
	char Buffer[1024];
	sprintf(Buffer, "--------------------------------------------------\r\nAssertion failed!\r\n\r\nProgram: %s\r\nFile: %s\r\nLine: %d\r\n\r\nExpression: %s\r\nMessage: %s\r\n\r\n", ExeFileName, SourceFileName, Line, Exp, Msg != NULL ? Msg : "");

	const int id = cLog::ShowModalMessage(cLog::DM_ASSERT, true, Buffer, NULL);
	
	switch(id) {
		case cLog::idIgnoreAlways:
			IgnoreAlways = true;
		case cLog::idIgnore:
			return false;
		case cLog::idDebug:
		default:
			return true;
	}
} // cAssertDlg

#endif // _DEBUG