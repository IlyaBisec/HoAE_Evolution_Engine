#pragma once

#ifdef _DEBUG

bool cAssertDlg(const char *Exp, const char *Msg, const char *File, int Line, bool &IgnoreAlways);

#define cAssert(Exp) {\
	static bool IgnoreAlways = false;\
	if(!IgnoreAlways && !(Exp)) {\
		if(cAssertDlg(#Exp, NULL, __FILE__, __LINE__, IgnoreAlways)) {\
			_asm { int 3 }\
		}\
	}\
}

#define cAssertM(Exp, Msg) {\
	static bool IgnoreAlways = false;\
	if(!IgnoreAlways && !(Exp)) {\
		if(cAssertDlg(#Exp, Msg, __FILE__, __LINE__, IgnoreAlways)) {\
			_asm { int 3 }\
		}\
	}\
}

#else

#define cAssert(Exp)		((void)0)
#define cAssertM(Exp, Msg)	((void)0)

#endif // _DEBUG