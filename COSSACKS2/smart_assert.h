#ifndef __SMARTASSERT_H__
#define __SMARTASSERT_H__
#ifdef DIALOGS_API
DIALOGS_API void SmartAssert(const char* File,int Line,const char* expression);
#else
__declspec(dllexport) void SmartAssert(const char* File,int Line,const char* expression);
#endif
#ifndef assert 
#define assert(x) if(!(x))SmartAssert(__FILE__,__LINE__,#x);
#else 
#undef assert
#define assert(x) if(!(x))SmartAssert(__FILE__,__LINE__,#x);
#endif
#endif //__SMARTASSERT_H__