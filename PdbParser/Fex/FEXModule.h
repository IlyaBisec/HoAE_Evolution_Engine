#ifndef FEXMODULE_H
#define FEXMODULE_H

//-----------------------------------------------------------------------------------
#define FEX_LOGGING_ENABLE
#define FEX_FUNC_HANDLER
//-----------------------------------------------------------------------------------
enum {
	FEX_BLOCK_TYPE_VERSION = 1,
	FEX_BLOCK_TYPE_MODULESDUMP,
	FEX_BLOCK_TYPE_CONTEXTDUMP, 
	FEX_BLOCK_TYPE_STACKDUMP, 
	FEX_BLOCK_TYPE_CALLSTACK,
	FEX_BLOCK_TYPE_FUNCADDRESSES,
	FEX_BLOCK_TYPE_CALLADDRESSES,
	FEX_BLOCK_TYPE_LASTERROR,
	FEX_BLOCK_TYPE_ERRNO,
	FEX_BLOCK_TYPE_DXVERSION,
	FEX_BLOCK_TYPE_DISKSPACE,
	FEX_BLOCK_TYPE_SYSTEMVERSION,
	FEX_BLOCK_TYPE_MEMSTATUS,
	FEX_BLOCK_TYPE_VIDEONAME,
	FEX_BLOCK_TYPE_AUDIONAME,
	FEX_BLOCK_TYPE_TEXTINFO
};
//-----------------------------------------------------------------------------------
extern unsigned FEX_end_esp;
extern LPTOP_LEVEL_EXCEPTION_FILTER FEX_OrigFilter;
//-----------------------------------------------------------------------------------
#ifdef FEX_FUNC_HANDLER
	#define FEX_BEGIN() __asm {mov eax,esp} __asm {mov FEX_end_esp,eax} FEX_Install(); 
	#define FEX_END() SetUnhandledExceptionFilter(FEX_OrigFilter);
#else
	#define FEX_BEGIN() __asm {mov eax,esp} __asm {mov FEX_end_esp,eax}\
							  FEX_Install(); __try {
	#define FEX_END() } __except(FEX_CrashHandlerExceptionFilter(GetExceptionInformation())){};
#endif
//-----------------------------------------------------------------------------------
LONG __stdcall FEX_CrashHandlerExceptionFilter(EXCEPTION_POINTERS* pEx);
void FEX_Install(void);
//-----------------------------------------------------------------------------------

#endif
