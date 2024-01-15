#line 1 "c:\\work\\cossacks\\vopyl\\nicevopyl\\stdafx.cpp"




#line 1 "c:\\work\\cossacks\\vopyl\\nicevopyl\\stdafx.h"





#pragma once




#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"


























#line 28 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"





#pragma once
#line 35 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

















































#line 85 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"




















#line 106 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"



#line 110 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"



#line 114 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"



#line 118 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"



#line 122 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"





#line 128 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"




#line 133 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 134 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"







#line 142 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 143 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"



#pragma warning(disable:4514)

#pragma warning(disable:4103)
#line 150 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

#pragma warning(push)
#line 153 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#pragma warning(disable:4001)
#pragma warning(disable:4201)
#pragma warning(disable:4214)
#line 157 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"















#pragma once
#line 18 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"






#line 25 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"







#pragma pack(push,8)
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"


extern "C" {
#line 38 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"










#line 49 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"
#line 50 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"






#line 57 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"





typedef enum _EXCEPTION_DISPOSITION {
    ExceptionContinueExecution,
    ExceptionContinueSearch,
    ExceptionNestedException,
    ExceptionCollidedUnwind
} EXCEPTION_DISPOSITION;











struct _EXCEPTION_RECORD;
struct _CONTEXT;

EXCEPTION_DISPOSITION __cdecl _except_handler (
    struct _EXCEPTION_RECORD *ExceptionRecord,
    void * EstablisherFrame,
    struct _CONTEXT *ContextRecord,
    void * DispatcherContext
    );






























#line 119 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"















unsigned long __cdecl _exception_code(void);
void *        __cdecl _exception_info(void);
int           __cdecl _abnormal_termination(void);

#line 139 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"













}
#line 154 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"


#pragma pack(pop)
#line 158 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"

#line 160 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\excpt.h"
#line 158 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"















#pragma once
#line 18 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"






#line 25 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"







#pragma pack(push,8)
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"


extern "C" {
#line 38 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"







#line 46 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"
#line 47 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"





typedef __w64 unsigned int   uintptr_t;
#line 54 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"

#line 56 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"








typedef char *  va_list;
#line 66 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"

#line 68 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"





#line 74 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"











#line 86 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"


#line 89 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"













#line 103 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"

































































































#line 201 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"


}
#line 205 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"


#pragma pack(pop)
#line 209 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"

#line 211 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\stdarg.h"
#line 159 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 160 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"















#line 17 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
#line 18 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"












extern "C" {
#line 32 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"











typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;
#line 51 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"








#line 60 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
#line 61 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"



#line 65 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"



#line 69 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"



#line 73 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"



#line 77 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"



#line 81 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"











#line 93 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"






#line 100 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"



#line 104 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
#line 105 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"


























#line 132 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"







#line 140 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL            *PBOOL;
typedef BOOL             *LPBOOL;
typedef BYTE            *PBYTE;
typedef BYTE             *LPBYTE;
typedef int             *PINT;
typedef int              *LPINT;
typedef WORD            *PWORD;
typedef WORD             *LPWORD;
typedef long             *LPLONG;
typedef DWORD           *PDWORD;
typedef DWORD            *LPDWORD;
typedef void             *LPVOID;
typedef const void       *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;


#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





















extern "C" {
#line 24 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"














#pragma once
#line 17 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"






#line 24 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"



extern "C" {
#line 29 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"


















#line 48 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"


typedef unsigned short wchar_t;

#line 53 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"


typedef unsigned short wint_t;
typedef unsigned short wctype_t;

#line 59 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"



#line 63 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"


 extern const unsigned short _wctype[];
 extern const unsigned short *_pctype;
 extern const wctype_t *_pwctype;
#line 69 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"










                                













 int __cdecl _isctype(int, int);
 int __cdecl isalpha(int);
 int __cdecl isupper(int);
 int __cdecl islower(int);
 int __cdecl isdigit(int);
 int __cdecl isxdigit(int);
 int __cdecl isspace(int);
 int __cdecl ispunct(int);
 int __cdecl isalnum(int);
 int __cdecl isprint(int);
 int __cdecl isgraph(int);
 int __cdecl iscntrl(int);
 int __cdecl toupper(int);
 int __cdecl tolower(int);
 int __cdecl _tolower(int);
 int __cdecl _toupper(int);
 int __cdecl __isascii(int);
 int __cdecl __toascii(int);
 int __cdecl __iscsymf(int);
 int __cdecl __iscsym(int);

#line 115 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"







 int __cdecl iswalpha(wint_t);
 int __cdecl iswupper(wint_t);
 int __cdecl iswlower(wint_t);
 int __cdecl iswdigit(wint_t);
 int __cdecl iswxdigit(wint_t);
 int __cdecl iswspace(wint_t);
 int __cdecl iswpunct(wint_t);
 int __cdecl iswalnum(wint_t);
 int __cdecl iswprint(wint_t);
 int __cdecl iswgraph(wint_t);
 int __cdecl iswcntrl(wint_t);
 int __cdecl iswascii(wint_t);
 int __cdecl isleadbyte(int);

 wchar_t __cdecl towupper(wchar_t);
 wchar_t __cdecl towlower(wchar_t);

 int __cdecl iswctype(wint_t, wctype_t);


 int __cdecl is_wctype(wint_t, wctype_t);



#line 147 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"











 extern int __mb_cur_max;

 int __cdecl ___mb_cur_max_func(void);
#line 162 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"





 int __cdecl _chvalidator(int, int);



#line 172 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"






















































































#line 259 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"

#line 261 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"








#line 270 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"
















#line 287 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"

#line 289 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"


}
#line 293 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"


#line 296 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\ctype.h"
#line 26 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




#line 31 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"








#line 42 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


#line 45 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





#line 51 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


#line 54 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"









#line 64 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



#line 68 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"








#line 77 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



#line 81 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


















#line 100 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



#line 104 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef unsigned long POINTER_64_INT;
#line 107 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 109 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



#line 113 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 115 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"






















#pragma once
#line 25 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"


extern "C" {
#line 29 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"

typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;





typedef signed int LONG32, *PLONG32;





typedef unsigned int ULONG32, *PULONG32;
typedef unsigned int DWORD32, *PDWORD32;







#line 59 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"

















#line 77 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"











#line 89 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"
    typedef __w64 int INT_PTR, *PINT_PTR;
    typedef __w64 unsigned int UINT_PTR, *PUINT_PTR;

    typedef __w64 long LONG_PTR, *PLONG_PTR;
    typedef __w64 unsigned long ULONG_PTR, *PULONG_PTR;

    

#line 98 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"
#line 99 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"



























































































































































typedef unsigned short UHALF_PTR, *PUHALF_PTR;
typedef short HALF_PTR, *PHALF_PTR;
typedef __w64 long SHANDLE_PTR;
typedef __w64 unsigned long HANDLE_PTR;
















#line 275 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"























typedef ULONG_PTR SIZE_T, *PSIZE_T;
typedef LONG_PTR SSIZE_T, *PSSIZE_T;





typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;





typedef __int64 LONG64, *PLONG64;






typedef unsigned __int64 ULONG64, *PULONG64;
typedef unsigned __int64 DWORD64, *PDWORD64;





typedef ULONG_PTR KAFFINITY;
typedef KAFFINITY *PKAFFINITY;


}
#line 331 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"

#line 333 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\basetsd.h"
#line 117 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






#line 124 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






#line 131 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 132 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






#line 139 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 140 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



#line 144 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






#line 151 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 152 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






#line 159 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 160 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






#line 167 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 168 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






#line 175 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 176 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




#line 181 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 183 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 184 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






#line 191 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 192 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"








#line 201 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 202 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"














#line 217 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





typedef void *PVOID;
typedef void *  PVOID64;







#line 232 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
















#line 249 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"








typedef char CHAR;
typedef short SHORT;
typedef long LONG;
#line 261 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






typedef wchar_t WCHAR;    



#line 272 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef WCHAR *PWCHAR;
typedef WCHAR *LPWCH, *PWCH;
typedef const WCHAR *LPCWCH, *PCWCH;
typedef WCHAR *NWPSTR;
typedef WCHAR *LPWSTR, *PWSTR;
typedef WCHAR  *LPUWSTR, *PUWSTR;

typedef const WCHAR *LPCWSTR, *PCWSTR;
typedef const WCHAR  *LPCUWSTR, *PCUWSTR;




typedef CHAR *PCHAR;
typedef CHAR *LPCH, *PCH;

typedef const CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR;
typedef CHAR *LPSTR, *PSTR;
typedef const CHAR *LPCSTR, *PCSTR;























typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE , *PTBYTE ;

#line 320 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef LPSTR LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;


#line 327 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



typedef SHORT *PSHORT;  
typedef LONG *PLONG;    


typedef void *HANDLE;




#line 340 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
typedef HANDLE *PHANDLE;





typedef BYTE   FCHAR;
typedef WORD   FSHORT;
typedef DWORD  FLONG;





typedef LONG HRESULT;

#line 357 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


    


#line 363 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"









#line 373 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"













#line 387 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
















typedef char CCHAR;          
typedef DWORD LCID;         
typedef PDWORD PLCID;       
typedef WORD   LANGID;      






















#line 430 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
typedef struct _FLOAT128 {
    __int64 LowPart;
    __int64 HighPart;
} FLOAT128;

typedef FLOAT128 *PFLOAT128;









typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;














#line 462 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef LONGLONG *PLONGLONG;
typedef ULONGLONG *PULONGLONG;



typedef LONGLONG USN;



#line 473 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
#line 483 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;



#line 491 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
typedef union _ULARGE_INTEGER {
    struct {
        DWORD LowPart;
        DWORD HighPart;
    };
    struct {
        DWORD LowPart;
        DWORD HighPart;
    } u;
#line 501 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
    ULONGLONG QuadPart;
} ULARGE_INTEGER;

typedef ULARGE_INTEGER *PULARGE_INTEGER;








typedef struct _LUID {
    DWORD LowPart;
    LONG HighPart;
} LUID, *PLUID;


typedef ULONGLONG  DWORDLONG;
typedef DWORDLONG *PDWORDLONG;






















#line 544 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"































































#line 608 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"











ULONGLONG
__stdcall
Int64ShllMod32 (
    ULONGLONG Value,
    DWORD ShiftCount
    );

LONGLONG
__stdcall
Int64ShraMod32 (
    LONGLONG Value,
    DWORD ShiftCount
    );

ULONGLONG
__stdcall
Int64ShrlMod32 (
    ULONGLONG Value,
    DWORD ShiftCount
    );


#pragma warning(push)
#line 643 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#pragma warning(disable:4035)               

__inline ULONGLONG
__stdcall
Int64ShllMod32 (
    ULONGLONG Value,
    DWORD ShiftCount
    )
{
    __asm    {
        mov     ecx, ShiftCount
        mov     eax, dword ptr [Value]
        mov     edx, dword ptr [Value+4]
        shld    edx, eax, cl
        shl     eax, cl
    }
}

__inline LONGLONG
__stdcall
Int64ShraMod32 (
    LONGLONG Value,
    DWORD ShiftCount
    )
{
    __asm {
        mov     ecx, ShiftCount
        mov     eax, dword ptr [Value]
        mov     edx, dword ptr [Value+4]
        shrd    eax, edx, cl
        sar     edx, cl
    }
}

__inline ULONGLONG
__stdcall
Int64ShrlMod32 (
    ULONGLONG Value,
    DWORD ShiftCount
    )
{
    __asm    {
        mov     ecx, ShiftCount
        mov     eax, dword ptr [Value]
        mov     edx, dword ptr [Value+4]
        shrd    eax, edx, cl
        shr     edx, cl
    }
}


#pragma warning(pop)


#line 698 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




























































#line 759 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





typedef BYTE  BOOLEAN;           
typedef BOOLEAN *PBOOLEAN;       





typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, * PRLIST_ENTRY;






typedef struct _SINGLE_LIST_ENTRY {
    struct _SINGLE_LIST_ENTRY *Next;
} SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;





typedef struct LIST_ENTRY32 {
    DWORD Flink;
    DWORD Blink;
} LIST_ENTRY32;
typedef LIST_ENTRY32 *PLIST_ENTRY32;

typedef struct LIST_ENTRY64 {
    ULONGLONG Flink;
    ULONGLONG Blink;
} LIST_ENTRY64;
typedef LIST_ENTRY64 *PLIST_ENTRY64;


#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"




















#line 22 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;
#line 29 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"




































#line 67 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"








typedef GUID *LPGUID;
#line 77 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"



typedef const GUID *LPCGUID;
#line 82 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"




typedef GUID IID;
typedef IID *LPIID;


typedef GUID CLSID;
typedef CLSID *LPCLSID;


typedef GUID FMTID;
typedef FMTID *LPFMTID;







#line 104 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"







#line 112 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 113 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"







#line 121 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 122 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"







#line 130 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 131 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"







#line 139 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 140 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"

#line 142 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"




#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"















#pragma once
#line 18 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"






#line 25 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"



extern "C" {
#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"








#line 39 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"















#line 55 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"






typedef __w64 unsigned int   size_t;
#line 63 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"

#line 65 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"










#line 76 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"
























        void *  __cdecl memcpy(void *, const void *, size_t);
        int     __cdecl memcmp(const void *, const void *, size_t);
        void *  __cdecl memset(void *, int, size_t);
        char *  __cdecl _strset(char *, int);
        char *  __cdecl strcpy(char *, const char *);
        char *  __cdecl strcat(char *, const char *);
        int     __cdecl strcmp(const char *, const char *);
        size_t  __cdecl strlen(const char *);
#line 109 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"
 void *  __cdecl _memccpy(void *, const void *, int, size_t);
 void *  __cdecl memchr(const void *, int, size_t);
 int     __cdecl _memicmp(const void *, const void *, size_t);



#line 116 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"
 void *  __cdecl memmove(void *, const void *, size_t);
#line 118 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"


 char *  __cdecl strchr(const char *, int);
 int     __cdecl _strcmpi(const char *, const char *);
 int     __cdecl _stricmp(const char *, const char *);
 int     __cdecl strcoll(const char *, const char *);
 int     __cdecl _stricoll(const char *, const char *);
 int     __cdecl _strncoll(const char *, const char *, size_t);
 int     __cdecl _strnicoll(const char *, const char *, size_t);
 size_t  __cdecl strcspn(const char *, const char *);
 char *  __cdecl _strdup(const char *);
 char *  __cdecl _strerror(const char *);
 char *  __cdecl strerror(int);
 char *  __cdecl _strlwr(char *);
 char *  __cdecl strncat(char *, const char *, size_t);
 int     __cdecl strncmp(const char *, const char *, size_t);
 int     __cdecl _strnicmp(const char *, const char *, size_t);
 char *  __cdecl strncpy(char *, const char *, size_t);
 char *  __cdecl _strnset(char *, int, size_t);
 char *  __cdecl strpbrk(const char *, const char *);
 char *  __cdecl strrchr(const char *, int);
 char *  __cdecl _strrev(char *);
 size_t  __cdecl strspn(const char *, const char *);
 char *  __cdecl strstr(const char *, const char *);
 char *  __cdecl strtok(char *, const char *);
 char *  __cdecl _strupr(char *);
 size_t  __cdecl strxfrm (char *, const char *, size_t);





 void * __cdecl memccpy(void *, const void *, int, size_t);
 int __cdecl memicmp(const void *, const void *, size_t);
 int __cdecl strcmpi(const char *, const char *);
 int __cdecl stricmp(const char *, const char *);
 char * __cdecl strdup(const char *);
 char * __cdecl strlwr(char *);
 int __cdecl strnicmp(const char *, const char *, size_t);
 char * __cdecl strnset(char *, int, size_t);
 char * __cdecl strrev(char *);
        char * __cdecl strset(char *, int);
 char * __cdecl strupr(char *);

#line 163 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"






 wchar_t * __cdecl wcscat(wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcschr(const wchar_t *, wchar_t);
 int __cdecl wcscmp(const wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcscpy(wchar_t *, const wchar_t *);
 size_t __cdecl wcscspn(const wchar_t *, const wchar_t *);
 size_t __cdecl wcslen(const wchar_t *);
 wchar_t * __cdecl wcsncat(wchar_t *, const wchar_t *, size_t);
 int __cdecl wcsncmp(const wchar_t *, const wchar_t *, size_t);
 wchar_t * __cdecl wcsncpy(wchar_t *, const wchar_t *, size_t);
 wchar_t * __cdecl wcspbrk(const wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcsrchr(const wchar_t *, wchar_t);
 size_t __cdecl wcsspn(const wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcsstr(const wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcstok(wchar_t *, const wchar_t *);
 wchar_t * __cdecl _wcserror(int);
 wchar_t * __cdecl __wcserror(const wchar_t *);

 wchar_t * __cdecl _wcsdup(const wchar_t *);
 int __cdecl _wcsicmp(const wchar_t *, const wchar_t *);
 int __cdecl _wcsnicmp(const wchar_t *, const wchar_t *, size_t);
 wchar_t * __cdecl _wcsnset(wchar_t *, wchar_t, size_t);
 wchar_t * __cdecl _wcsrev(wchar_t *);
 wchar_t * __cdecl _wcsset(wchar_t *, wchar_t);

 wchar_t * __cdecl _wcslwr(wchar_t *);
 wchar_t * __cdecl _wcsupr(wchar_t *);
 size_t __cdecl wcsxfrm(wchar_t *, const wchar_t *, size_t);
 int __cdecl wcscoll(const wchar_t *, const wchar_t *);
 int __cdecl _wcsicoll(const wchar_t *, const wchar_t *);
 int __cdecl _wcsncoll(const wchar_t *, const wchar_t *, size_t);
 int __cdecl _wcsnicoll(const wchar_t *, const wchar_t *, size_t);







 wchar_t * __cdecl wcsdup(const wchar_t *);
 int __cdecl wcsicmp(const wchar_t *, const wchar_t *);
 int __cdecl wcsnicmp(const wchar_t *, const wchar_t *, size_t);
 wchar_t * __cdecl wcsnset(wchar_t *, wchar_t, size_t);
 wchar_t * __cdecl wcsrev(wchar_t *);
 wchar_t * __cdecl wcsset(wchar_t *, wchar_t);
 wchar_t * __cdecl wcslwr(wchar_t *);
 wchar_t * __cdecl wcsupr(wchar_t *);
 int __cdecl wcsicoll(const wchar_t *, const wchar_t *);

#line 218 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"


#line 221 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"



}
#line 226 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"

#line 228 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\string.h"
#line 147 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"



__inline int InlineIsEqualGUID(const GUID & rguid1, const GUID & rguid2)
{
   return (
      ((unsigned long *) &rguid1)[0] == ((unsigned long *) &rguid2)[0] &&
      ((unsigned long *) &rguid1)[1] == ((unsigned long *) &rguid2)[1] &&
      ((unsigned long *) &rguid1)[2] == ((unsigned long *) &rguid2)[2] &&
      ((unsigned long *) &rguid1)[3] == ((unsigned long *) &rguid2)[3]);
}

__inline int IsEqualGUID(const GUID & rguid1, const GUID & rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(GUID));
}











#line 175 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"

















__inline int operator==(const GUID & guidOne, const GUID & guidOther)
{
    return IsEqualGUID(guidOne,guidOther);
}

__inline int operator!=(const GUID & guidOne, const GUID & guidOther)
{
    return !(guidOne == guidOther);
}
#line 202 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 203 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 204 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 205 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 206 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\guiddef.h"
#line 803 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




typedef struct  _OBJECTID {     
    GUID Lineage;
    DWORD Uniquifier;
} OBJECTID;
#line 812 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"























































































#line 900 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





























































































































































































#line 1090 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




























































































































































































#line 1279 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
























#line 1304 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


  












































#line 1352 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
  
#line 1354 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




typedef ULONG_PTR KSPIN_LOCK;
typedef KSPIN_LOCK *PKSPIN_LOCK;




















































































































































































































































































































































































































































#line 1797 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"














#line 1812 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
struct _TEB *
NtCurrentTeb(void);
#line 1815 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"











































































































































































































#pragma warning(push)
#line 2020 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#pragma warning(disable:4164)   
                                

#pragma function(_enable)
#pragma function(_disable)
#line 2026 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


#pragma warning(pop)


#line 2032 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 2034 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 2035 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

































BYTE 
__readfsbyte (
     DWORD Offset
    );
 
WORD  
__readfsword (
     DWORD Offset
    );
 
DWORD
__readfsdword (
     DWORD Offset
    );
 
void
__writefsbyte (
     DWORD Offset,
     BYTE  Data
    );
 
void
__writefsword (
     DWORD Offset,
     WORD   Data
    );
 
void
__writefsdword (
     DWORD Offset,
     DWORD Data
    );
 
#pragma intrinsic(__readfsbyte)
#pragma intrinsic(__readfsword)
#pragma intrinsic(__readfsdword)
#pragma intrinsic(__writefsbyte)
#pragma intrinsic(__writefsword)
#pragma intrinsic(__writefsdword)

#line 2109 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 2111 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






_inline PVOID GetFiberData( void )    { return *(PVOID *) (ULONG_PTR) __readfsdword (0x10);}
_inline PVOID GetCurrentFiber( void ) { return (PVOID) (ULONG_PTR) __readfsdword (0x10);}


















#line 2138 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 2139 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


































#line 2174 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



typedef struct _FLOATING_SAVE_AREA {
    DWORD   ControlWord;
    DWORD   StatusWord;
    DWORD   TagWord;
    DWORD   ErrorOffset;
    DWORD   ErrorSelector;
    DWORD   DataOffset;
    DWORD   DataSelector;
    BYTE    RegisterArea[80];
    DWORD   Cr0NpxState;
} FLOATING_SAVE_AREA;

typedef FLOATING_SAVE_AREA *PFLOATING_SAVE_AREA;











typedef struct _CONTEXT {

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    DWORD ContextFlags;

    
    
    
    
    

    DWORD   Dr0;
    DWORD   Dr1;
    DWORD   Dr2;
    DWORD   Dr3;
    DWORD   Dr6;
    DWORD   Dr7;

    
    
    
    

    FLOATING_SAVE_AREA FloatSave;

    
    
    
    

    DWORD   SegGs;
    DWORD   SegFs;
    DWORD   SegEs;
    DWORD   SegDs;

    
    
    
    

    DWORD   Edi;
    DWORD   Esi;
    DWORD   Ebx;
    DWORD   Edx;
    DWORD   Ecx;
    DWORD   Eax;

    
    
    
    

    DWORD   Ebp;
    DWORD   Eip;
    DWORD   SegCs;              
    DWORD   EFlags;             
    DWORD   Esp;
    DWORD   SegSs;

    
    
    
    
    

    BYTE    ExtendedRegisters[512];

} CONTEXT;



typedef CONTEXT *PCONTEXT;



#line 2294 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





typedef struct _LDT_ENTRY {
    WORD    LimitLow;
    WORD    BaseLow;
    union {
        struct {
            BYTE    BaseMid;
            BYTE    Flags1;     
            BYTE    Flags2;     
            BYTE    BaseHi;
        } Bytes;
        struct {
            DWORD   BaseMid : 8;
            DWORD   Type : 5;
            DWORD   Dpl : 2;
            DWORD   Pres : 1;
            DWORD   LimitHi : 4;
            DWORD   Sys : 1;
            DWORD   Reserved_0 : 1;
            DWORD   Default_Big : 1;
            DWORD   Granularity : 1;
            DWORD   BaseHi : 8;
        } Bits;
    } HighWord;
} LDT_ENTRY, *PLDT_ENTRY;

#line 2325 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

























































































































































































































































































































































#line 2671 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"










#line 2682 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





































































































































































































































#line 2912 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




































































































































































































































#line 3141 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
























#line 3166 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"














































































































































































































































































































































































typedef struct _EXCEPTION_RECORD {
    DWORD    ExceptionCode;
    DWORD ExceptionFlags;
    struct _EXCEPTION_RECORD *ExceptionRecord;
    PVOID ExceptionAddress;
    DWORD NumberParameters;
    ULONG_PTR ExceptionInformation[15];
    } EXCEPTION_RECORD;

typedef EXCEPTION_RECORD *PEXCEPTION_RECORD;

typedef struct _EXCEPTION_RECORD32 {
    DWORD    ExceptionCode;
    DWORD ExceptionFlags;
    DWORD ExceptionRecord;
    DWORD ExceptionAddress;
    DWORD NumberParameters;
    DWORD ExceptionInformation[15];
} EXCEPTION_RECORD32, *PEXCEPTION_RECORD32;

typedef struct _EXCEPTION_RECORD64 {
    DWORD    ExceptionCode;
    DWORD ExceptionFlags;
    DWORD64 ExceptionRecord;
    DWORD64 ExceptionAddress;
    DWORD NumberParameters;
    DWORD __unusedAlignment;
    DWORD64 ExceptionInformation[15];
} EXCEPTION_RECORD64, *PEXCEPTION_RECORD64;





typedef struct _EXCEPTION_POINTERS {
    PEXCEPTION_RECORD ExceptionRecord;
    PCONTEXT ContextRecord;
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
typedef PVOID PACCESS_TOKEN;            
typedef PVOID PSECURITY_DESCRIPTOR;     
typedef PVOID PSID;     







































typedef DWORD ACCESS_MASK;
typedef ACCESS_MASK *PACCESS_MASK;
























































typedef struct _GENERIC_MAPPING {
    ACCESS_MASK GenericRead;
    ACCESS_MASK GenericWrite;
    ACCESS_MASK GenericExecute;
    ACCESS_MASK GenericAll;
} GENERIC_MAPPING;
typedef GENERIC_MAPPING *PGENERIC_MAPPING;












#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"























#pragma warning(disable:4103)

#pragma pack(push,4)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"
#line 3690 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef struct _LUID_AND_ATTRIBUTES {
    LUID Luid;
    DWORD Attributes;
    } LUID_AND_ATTRIBUTES, * PLUID_AND_ATTRIBUTES;
typedef LUID_AND_ATTRIBUTES LUID_AND_ATTRIBUTES_ARRAY[1];
typedef LUID_AND_ATTRIBUTES_ARRAY *PLUID_AND_ATTRIBUTES_ARRAY;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 3699 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


































typedef struct _SID_IDENTIFIER_AUTHORITY {
    BYTE  Value[6];
} SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;
#line 3737 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




typedef struct _SID {
   BYTE  Revision;
   BYTE  SubAuthorityCount;
   SID_IDENTIFIER_AUTHORITY IdentifierAuthority;



   DWORD SubAuthority[1];
#line 3750 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
} SID, *PISID;
#line 3752 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





                                                



#line 3762 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


typedef enum _SID_NAME_USE {
    SidTypeUser = 1,
    SidTypeGroup,
    SidTypeDomain,
    SidTypeAlias,
    SidTypeWellKnownGroup,
    SidTypeDeletedAccount,
    SidTypeInvalid,
    SidTypeUnknown,
    SidTypeComputer
} SID_NAME_USE, *PSID_NAME_USE;

typedef struct _SID_AND_ATTRIBUTES {
    PSID Sid;
    DWORD Attributes;
    } SID_AND_ATTRIBUTES, * PSID_AND_ATTRIBUTES;

typedef SID_AND_ATTRIBUTES SID_AND_ATTRIBUTES_ARRAY[1];
typedef SID_AND_ATTRIBUTES_ARRAY *PSID_AND_ATTRIBUTES_ARRAY;





















































































































































































typedef enum {

    WinNullSid                                  = 0,
    WinWorldSid                                 = 1,
    WinLocalSid                                 = 2,
    WinCreatorOwnerSid                          = 3,
    WinCreatorGroupSid                          = 4,
    WinCreatorOwnerServerSid                    = 5,
    WinCreatorGroupServerSid                    = 6,
    WinNtAuthoritySid                           = 7,
    WinDialupSid                                = 8,
    WinNetworkSid                               = 9,
    WinBatchSid                                 = 10,
    WinInteractiveSid                           = 11,
    WinServiceSid                               = 12,
    WinAnonymousSid                             = 13,
    WinProxySid                                 = 14,
    WinEnterpriseControllersSid                 = 15,
    WinSelfSid                                  = 16,
    WinAuthenticatedUserSid                     = 17,
    WinRestrictedCodeSid                        = 18,
    WinTerminalServerSid                        = 19,
    WinRemoteLogonIdSid                         = 20,
    WinLogonIdsSid                              = 21,
    WinLocalSystemSid                           = 22,
    WinLocalServiceSid                          = 23,
    WinNetworkServiceSid                        = 24,
    WinBuiltinDomainSid                         = 25,
    WinBuiltinAdministratorsSid                 = 26,
    WinBuiltinUsersSid                          = 27,
    WinBuiltinGuestsSid                         = 28,
    WinBuiltinPowerUsersSid                     = 29,
    WinBuiltinAccountOperatorsSid               = 30,
    WinBuiltinSystemOperatorsSid                = 31,
    WinBuiltinPrintOperatorsSid                 = 32,
    WinBuiltinBackupOperatorsSid                = 33,
    WinBuiltinReplicatorSid                     = 34,
    WinBuiltinPreWindows2000CompatibleAccessSid = 35,
    WinBuiltinRemoteDesktopUsersSid             = 36,
    WinBuiltinNetworkConfigurationOperatorsSid  = 37,
    WinAccountAdministratorSid                  = 38,
    WinAccountGuestSid                          = 39,
    WinAccountKrbtgtSid                         = 40,
    WinAccountDomainAdminsSid                   = 41,
    WinAccountDomainUsersSid                    = 42,
    WinAccountDomainGuestsSid                   = 43,
    WinAccountComputersSid                      = 44,
    WinAccountControllersSid                    = 45,
    WinAccountCertAdminsSid                     = 46,
    WinAccountSchemaAdminsSid                   = 47,
    WinAccountEnterpriseAdminsSid               = 48,
    WinAccountPolicyAdminsSid                   = 49,
    WinAccountRasAndIasServersSid               = 50,
    WinNTLMAuthenticationSid                    = 51,
    WinDigestAuthenticationSid                  = 52,
    WinSChannelAuthenticationSid                = 53,
    WinThisOrganizationSid                      = 54,
    WinOtherOrganizationSid                     = 55,
    WinBuiltinIncomingForestTrustBuildersSid    = 56,
    WinBuiltinPerfMonitoringUsersSid            = 57,
    WinBuiltinPerfLoggingUsersSid               = 58,

} WELL_KNOWN_SID_TYPE;






















































































typedef struct _ACL {
    BYTE  AclRevision;
    BYTE  Sbz1;
    WORD   AclSize;
    WORD   AceCount;
    WORD   Sbz2;
} ACL;
typedef ACL *PACL;





















typedef struct _ACE_HEADER {
    BYTE  AceType;
    BYTE  AceFlags;
    WORD   AceSize;
} ACE_HEADER;
typedef ACE_HEADER *PACE_HEADER;








































































































typedef struct _ACCESS_ALLOWED_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD SidStart;
} ACCESS_ALLOWED_ACE;

typedef ACCESS_ALLOWED_ACE *PACCESS_ALLOWED_ACE;

typedef struct _ACCESS_DENIED_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD SidStart;
} ACCESS_DENIED_ACE;
typedef ACCESS_DENIED_ACE *PACCESS_DENIED_ACE;

typedef struct _SYSTEM_AUDIT_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD SidStart;
} SYSTEM_AUDIT_ACE;
typedef SYSTEM_AUDIT_ACE *PSYSTEM_AUDIT_ACE;

typedef struct _SYSTEM_ALARM_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD SidStart;
} SYSTEM_ALARM_ACE;
typedef SYSTEM_ALARM_ACE *PSYSTEM_ALARM_ACE;




typedef struct _ACCESS_ALLOWED_OBJECT_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD Flags;
    GUID ObjectType;
    GUID InheritedObjectType;
    DWORD SidStart;
} ACCESS_ALLOWED_OBJECT_ACE, *PACCESS_ALLOWED_OBJECT_ACE;

typedef struct _ACCESS_DENIED_OBJECT_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD Flags;
    GUID ObjectType;
    GUID InheritedObjectType;
    DWORD SidStart;
} ACCESS_DENIED_OBJECT_ACE, *PACCESS_DENIED_OBJECT_ACE;

typedef struct _SYSTEM_AUDIT_OBJECT_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD Flags;
    GUID ObjectType;
    GUID InheritedObjectType;
    DWORD SidStart;
} SYSTEM_AUDIT_OBJECT_ACE, *PSYSTEM_AUDIT_OBJECT_ACE;

typedef struct _SYSTEM_ALARM_OBJECT_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD Flags;
    GUID ObjectType;
    GUID InheritedObjectType;
    DWORD SidStart;
} SYSTEM_ALARM_OBJECT_ACE, *PSYSTEM_ALARM_OBJECT_ACE;






typedef struct _ACCESS_ALLOWED_CALLBACK_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD SidStart;
    
} ACCESS_ALLOWED_CALLBACK_ACE, *PACCESS_ALLOWED_CALLBACK_ACE;

typedef struct _ACCESS_DENIED_CALLBACK_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD SidStart;
    
} ACCESS_DENIED_CALLBACK_ACE, *PACCESS_DENIED_CALLBACK_ACE;

typedef struct _SYSTEM_AUDIT_CALLBACK_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD SidStart;
    
} SYSTEM_AUDIT_CALLBACK_ACE, *PSYSTEM_AUDIT_CALLBACK_ACE;

typedef struct _SYSTEM_ALARM_CALLBACK_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD SidStart;
    
} SYSTEM_ALARM_CALLBACK_ACE, *PSYSTEM_ALARM_CALLBACK_ACE;

typedef struct _ACCESS_ALLOWED_CALLBACK_OBJECT_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD Flags;
    GUID ObjectType;
    GUID InheritedObjectType;
    DWORD SidStart;
    
} ACCESS_ALLOWED_CALLBACK_OBJECT_ACE, *PACCESS_ALLOWED_CALLBACK_OBJECT_ACE;

typedef struct _ACCESS_DENIED_CALLBACK_OBJECT_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD Flags;
    GUID ObjectType;
    GUID InheritedObjectType;
    DWORD SidStart;
    
} ACCESS_DENIED_CALLBACK_OBJECT_ACE, *PACCESS_DENIED_CALLBACK_OBJECT_ACE;

typedef struct _SYSTEM_AUDIT_CALLBACK_OBJECT_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD Flags;
    GUID ObjectType;
    GUID InheritedObjectType;
    DWORD SidStart;
    
} SYSTEM_AUDIT_CALLBACK_OBJECT_ACE, *PSYSTEM_AUDIT_CALLBACK_OBJECT_ACE;

typedef struct _SYSTEM_ALARM_CALLBACK_OBJECT_ACE {
    ACE_HEADER Header;
    ACCESS_MASK Mask;
    DWORD Flags;
    GUID ObjectType;
    GUID InheritedObjectType;
    DWORD SidStart;
    
} SYSTEM_ALARM_CALLBACK_OBJECT_ACE, *PSYSTEM_ALARM_CALLBACK_OBJECT_ACE;















typedef enum _ACL_INFORMATION_CLASS {
    AclRevisionInformation = 1,
    AclSizeInformation
} ACL_INFORMATION_CLASS;






typedef struct _ACL_REVISION_INFORMATION {
    DWORD AclRevision;
} ACL_REVISION_INFORMATION;
typedef ACL_REVISION_INFORMATION *PACL_REVISION_INFORMATION;





typedef struct _ACL_SIZE_INFORMATION {
    DWORD AceCount;
    DWORD AclBytesInUse;
    DWORD AclBytesFree;
} ACL_SIZE_INFORMATION;
typedef ACL_SIZE_INFORMATION *PACL_SIZE_INFORMATION;


























typedef WORD   SECURITY_DESCRIPTOR_CONTROL, *PSECURITY_DESCRIPTOR_CONTROL;

























































































typedef struct _SECURITY_DESCRIPTOR_RELATIVE {
    BYTE  Revision;
    BYTE  Sbz1;
    SECURITY_DESCRIPTOR_CONTROL Control;
    DWORD Owner;
    DWORD Group;
    DWORD Sacl;
    DWORD Dacl;
    } SECURITY_DESCRIPTOR_RELATIVE, *PISECURITY_DESCRIPTOR_RELATIVE;

typedef struct _SECURITY_DESCRIPTOR {
   BYTE  Revision;
   BYTE  Sbz1;
   SECURITY_DESCRIPTOR_CONTROL Control;
   PSID Owner;
   PSID Group;
   PACL Sacl;
   PACL Dacl;

   } SECURITY_DESCRIPTOR, *PISECURITY_DESCRIPTOR;


















































typedef struct _OBJECT_TYPE_LIST {
    WORD   Level;
    WORD   Sbz;
    GUID *ObjectType;
} OBJECT_TYPE_LIST, *POBJECT_TYPE_LIST;















typedef enum _AUDIT_EVENT_TYPE {
    AuditEventObjectAccess,
    AuditEventDirectoryServiceAccess
} AUDIT_EVENT_TYPE, *PAUDIT_EVENT_TYPE;













































typedef struct _PRIVILEGE_SET {
    DWORD PrivilegeCount;
    DWORD Control;
    LUID_AND_ATTRIBUTES Privilege[1];
    } PRIVILEGE_SET, * PPRIVILEGE_SET;






















































typedef enum _SECURITY_IMPERSONATION_LEVEL {
    SecurityAnonymous,
    SecurityIdentification,
    SecurityImpersonation,
    SecurityDelegation
    } SECURITY_IMPERSONATION_LEVEL, * PSECURITY_IMPERSONATION_LEVEL;











































#line 4796 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


















typedef enum _TOKEN_TYPE {
    TokenPrimary = 1,
    TokenImpersonation
    } TOKEN_TYPE;
typedef TOKEN_TYPE *PTOKEN_TYPE;







typedef enum _TOKEN_INFORMATION_CLASS {
    TokenUser = 1,
    TokenGroups,
    TokenPrivileges,
    TokenOwner,
    TokenPrimaryGroup,
    TokenDefaultDacl,
    TokenSource,
    TokenType,
    TokenImpersonationLevel,
    TokenStatistics,
    TokenRestrictedSids,
    TokenSessionId,
    TokenGroupsAndPrivileges,
    TokenSessionReference,
    TokenSandBoxInert,
    TokenAuditPolicy,
    MaxTokenInfoClass  
} TOKEN_INFORMATION_CLASS, *PTOKEN_INFORMATION_CLASS;






typedef struct _TOKEN_USER {
    SID_AND_ATTRIBUTES User;
} TOKEN_USER, *PTOKEN_USER;

typedef struct _TOKEN_GROUPS {
    DWORD GroupCount;
    SID_AND_ATTRIBUTES Groups[1];
} TOKEN_GROUPS, *PTOKEN_GROUPS;


typedef struct _TOKEN_PRIVILEGES {
    DWORD PrivilegeCount;
    LUID_AND_ATTRIBUTES Privileges[1];
} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES;


typedef struct _TOKEN_OWNER {
    PSID Owner;
} TOKEN_OWNER, *PTOKEN_OWNER;


typedef struct _TOKEN_PRIMARY_GROUP {
    PSID PrimaryGroup;
} TOKEN_PRIMARY_GROUP, *PTOKEN_PRIMARY_GROUP;


typedef struct _TOKEN_DEFAULT_DACL {
    PACL DefaultDacl;
} TOKEN_DEFAULT_DACL, *PTOKEN_DEFAULT_DACL;

typedef struct _TOKEN_GROUPS_AND_PRIVILEGES {
    DWORD SidCount;
    DWORD SidLength;
    PSID_AND_ATTRIBUTES Sids;
    DWORD RestrictedSidCount;
    DWORD RestrictedSidLength;
    PSID_AND_ATTRIBUTES RestrictedSids;
    DWORD PrivilegeCount;
    DWORD PrivilegeLength;
    PLUID_AND_ATTRIBUTES Privileges;
    LUID AuthenticationId;
} TOKEN_GROUPS_AND_PRIVILEGES, *PTOKEN_GROUPS_AND_PRIVILEGES;


















typedef struct _TOKEN_AUDIT_POLICY_ELEMENT {
    DWORD Category;
    DWORD PolicyMask;
} TOKEN_AUDIT_POLICY_ELEMENT, *PTOKEN_AUDIT_POLICY_ELEMENT;

typedef struct _TOKEN_AUDIT_POLICY {
    DWORD PolicyCount;
    TOKEN_AUDIT_POLICY_ELEMENT Policy[1];
} TOKEN_AUDIT_POLICY, *PTOKEN_AUDIT_POLICY;









typedef struct _TOKEN_SOURCE {
    CHAR SourceName[8];
    LUID SourceIdentifier;
} TOKEN_SOURCE, *PTOKEN_SOURCE;


typedef struct _TOKEN_STATISTICS {
    LUID TokenId;
    LUID AuthenticationId;
    LARGE_INTEGER ExpirationTime;
    TOKEN_TYPE TokenType;
    SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
    DWORD DynamicCharged;
    DWORD DynamicAvailable;
    DWORD GroupCount;
    DWORD PrivilegeCount;
    LUID ModifiedId;
} TOKEN_STATISTICS, *PTOKEN_STATISTICS;



typedef struct _TOKEN_CONTROL {
    LUID TokenId;
    LUID AuthenticationId;
    LUID ModifiedId;
    TOKEN_SOURCE TokenSource;
    } TOKEN_CONTROL, *PTOKEN_CONTROL;








typedef BOOLEAN SECURITY_CONTEXT_TRACKING_MODE,
                    * PSECURITY_CONTEXT_TRACKING_MODE;







typedef struct _SECURITY_QUALITY_OF_SERVICE {
    DWORD Length;
    SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;
    SECURITY_CONTEXT_TRACKING_MODE ContextTrackingMode;
    BOOLEAN EffectiveOnly;
    } SECURITY_QUALITY_OF_SERVICE, * PSECURITY_QUALITY_OF_SERVICE;






typedef struct _SE_IMPERSONATION_STATE {
    PACCESS_TOKEN Token;
    BOOLEAN CopyOnOpen;
    BOOLEAN EffectiveOnly;
    SECURITY_IMPERSONATION_LEVEL Level;
} SE_IMPERSONATION_STATE, *PSE_IMPERSONATION_STATE;




typedef DWORD SECURITY_INFORMATION, *PSECURITY_INFORMATION;































#line 5028 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



#line 5032 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


























typedef struct _JOB_SET_ARRAY {
    HANDLE JobHandle;   
    DWORD MemberLevel;  
    DWORD Flags;        
} JOB_SET_ARRAY, *PJOB_SET_ARRAY;




typedef struct _NT_TIB {
    struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList;
    PVOID StackBase;
    PVOID StackLimit;
    PVOID SubSystemTib;
    union {
        PVOID FiberData;
        DWORD Version;
    };
    PVOID ArbitraryUserPointer;
    struct _NT_TIB *Self;
} NT_TIB;
typedef NT_TIB *PNT_TIB;




typedef struct _NT_TIB32 {
    DWORD ExceptionList;
    DWORD StackBase;
    DWORD StackLimit;
    DWORD SubSystemTib;
    union {
        DWORD FiberData;
        DWORD Version;
    };
    DWORD ArbitraryUserPointer;
    DWORD Self;
} NT_TIB32, *PNT_TIB32;

typedef struct _NT_TIB64 {
    DWORD64 ExceptionList;
    DWORD64 StackBase;
    DWORD64 StackLimit;
    DWORD64 SubSystemTib;
    union {
        DWORD64 FiberData;
        DWORD Version;
    };
    DWORD64 ArbitraryUserPointer;
    DWORD64 Self;
} NT_TIB64, *PNT_TIB64;




#line 5114 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






typedef struct _QUOTA_LIMITS {
    SIZE_T PagedPoolLimit;
    SIZE_T NonPagedPoolLimit;
    SIZE_T MinimumWorkingSetSize;
    SIZE_T MaximumWorkingSetSize;
    SIZE_T PagefileLimit;
    LARGE_INTEGER TimeLimit;
} QUOTA_LIMITS, *PQUOTA_LIMITS;






typedef struct _QUOTA_LIMITS_EX {
    SIZE_T PagedPoolLimit;
    SIZE_T NonPagedPoolLimit;
    SIZE_T MinimumWorkingSetSize;
    SIZE_T MaximumWorkingSetSize;
    SIZE_T PagefileLimit;
    LARGE_INTEGER TimeLimit;
    SIZE_T Reserved1;
    SIZE_T Reserved2;
    SIZE_T Reserved3;
    SIZE_T Reserved4;
    DWORD  Flags;
    DWORD  Reserved5;
} QUOTA_LIMITS_EX, *PQUOTA_LIMITS_EX;

typedef struct _IO_COUNTERS {
    ULONGLONG  ReadOperationCount;
    ULONGLONG  WriteOperationCount;
    ULONGLONG  OtherOperationCount;
    ULONGLONG ReadTransferCount;
    ULONGLONG WriteTransferCount;
    ULONGLONG OtherTransferCount;
} IO_COUNTERS;
typedef IO_COUNTERS *PIO_COUNTERS;


typedef struct _JOBOBJECT_BASIC_ACCOUNTING_INFORMATION {
    LARGE_INTEGER TotalUserTime;
    LARGE_INTEGER TotalKernelTime;
    LARGE_INTEGER ThisPeriodTotalUserTime;
    LARGE_INTEGER ThisPeriodTotalKernelTime;
    DWORD TotalPageFaultCount;
    DWORD TotalProcesses;
    DWORD ActiveProcesses;
    DWORD TotalTerminatedProcesses;
} JOBOBJECT_BASIC_ACCOUNTING_INFORMATION, *PJOBOBJECT_BASIC_ACCOUNTING_INFORMATION;

typedef struct _JOBOBJECT_BASIC_LIMIT_INFORMATION {
    LARGE_INTEGER PerProcessUserTimeLimit;
    LARGE_INTEGER PerJobUserTimeLimit;
    DWORD LimitFlags;
    SIZE_T MinimumWorkingSetSize;
    SIZE_T MaximumWorkingSetSize;
    DWORD ActiveProcessLimit;
    ULONG_PTR Affinity;
    DWORD PriorityClass;
    DWORD SchedulingClass;
} JOBOBJECT_BASIC_LIMIT_INFORMATION, *PJOBOBJECT_BASIC_LIMIT_INFORMATION;

typedef struct _JOBOBJECT_EXTENDED_LIMIT_INFORMATION {
    JOBOBJECT_BASIC_LIMIT_INFORMATION BasicLimitInformation;
    IO_COUNTERS IoInfo;
    SIZE_T ProcessMemoryLimit;
    SIZE_T JobMemoryLimit;
    SIZE_T PeakProcessMemoryUsed;
    SIZE_T PeakJobMemoryUsed;
} JOBOBJECT_EXTENDED_LIMIT_INFORMATION, *PJOBOBJECT_EXTENDED_LIMIT_INFORMATION;

typedef struct _JOBOBJECT_BASIC_PROCESS_ID_LIST {
    DWORD NumberOfAssignedProcesses;
    DWORD NumberOfProcessIdsInList;
    ULONG_PTR ProcessIdList[1];
} JOBOBJECT_BASIC_PROCESS_ID_LIST, *PJOBOBJECT_BASIC_PROCESS_ID_LIST;

typedef struct _JOBOBJECT_BASIC_UI_RESTRICTIONS {
    DWORD UIRestrictionsClass;
} JOBOBJECT_BASIC_UI_RESTRICTIONS, *PJOBOBJECT_BASIC_UI_RESTRICTIONS;

typedef struct _JOBOBJECT_SECURITY_LIMIT_INFORMATION {
    DWORD SecurityLimitFlags ;
    HANDLE JobToken ;
    PTOKEN_GROUPS SidsToDisable ;
    PTOKEN_PRIVILEGES PrivilegesToDelete ;
    PTOKEN_GROUPS RestrictedSids ;
} JOBOBJECT_SECURITY_LIMIT_INFORMATION, *PJOBOBJECT_SECURITY_LIMIT_INFORMATION ;

typedef struct _JOBOBJECT_END_OF_JOB_TIME_INFORMATION {
    DWORD EndOfJobTimeAction;
} JOBOBJECT_END_OF_JOB_TIME_INFORMATION, *PJOBOBJECT_END_OF_JOB_TIME_INFORMATION;

typedef struct _JOBOBJECT_ASSOCIATE_COMPLETION_PORT {
    PVOID CompletionKey;
    HANDLE CompletionPort;
} JOBOBJECT_ASSOCIATE_COMPLETION_PORT, *PJOBOBJECT_ASSOCIATE_COMPLETION_PORT;

typedef struct _JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION {
    JOBOBJECT_BASIC_ACCOUNTING_INFORMATION BasicInfo;
    IO_COUNTERS IoInfo;
} JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION, *PJOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION;

typedef struct _JOBOBJECT_JOBSET_INFORMATION {
    DWORD MemberLevel;
} JOBOBJECT_JOBSET_INFORMATION, *PJOBOBJECT_JOBSET_INFORMATION;


















































































typedef enum _JOBOBJECTINFOCLASS {
    JobObjectBasicAccountingInformation = 1,
    JobObjectBasicLimitInformation,
    JobObjectBasicProcessIdList,
    JobObjectBasicUIRestrictions,
    JobObjectSecurityLimitInformation,
    JobObjectEndOfJobTimeInformation,
    JobObjectAssociateCompletionPortInformation,
    JobObjectBasicAndIoAccountingInformation,
    JobObjectExtendedLimitInformation,
    JobObjectJobSetInformation,
    MaxJobObjectInfoClass
    } JOBOBJECTINFOCLASS;
























typedef enum _LOGICAL_PROCESSOR_RELATIONSHIP {
    RelationProcessorCore,
    RelationNumaNode
} LOGICAL_PROCESSOR_RELATIONSHIP;



typedef struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION {
    ULONG_PTR   ProcessorMask;
    LOGICAL_PROCESSOR_RELATIONSHIP Relationship;
    union {
        struct {
            BYTE  Flags;
        } ProcessorCore;
        struct {
            DWORD NodeNumber;
        } NumaNode;
        ULONGLONG  Reserved[2];
    };
} SYSTEM_LOGICAL_PROCESSOR_INFORMATION, *PSYSTEM_LOGICAL_PROCESSOR_INFORMATION;




















































typedef struct _MEMORY_BASIC_INFORMATION {
    PVOID BaseAddress;
    PVOID AllocationBase;
    DWORD AllocationProtect;
    SIZE_T RegionSize;
    DWORD State;
    DWORD Protect;
    DWORD Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

typedef struct _MEMORY_BASIC_INFORMATION32 {
    DWORD BaseAddress;
    DWORD AllocationBase;
    DWORD AllocationProtect;
    DWORD RegionSize;
    DWORD State;
    DWORD Protect;
    DWORD Type;
} MEMORY_BASIC_INFORMATION32, *PMEMORY_BASIC_INFORMATION32;

typedef struct _MEMORY_BASIC_INFORMATION64 {
    ULONGLONG BaseAddress;
    ULONGLONG AllocationBase;
    DWORD     AllocationProtect;
    DWORD     __alignment1;
    ULONGLONG RegionSize;
    DWORD     State;
    DWORD     Protect;
    DWORD     Type;
    DWORD     __alignment2;
} MEMORY_BASIC_INFORMATION64, *PMEMORY_BASIC_INFORMATION64;

























































































































































typedef struct _FILE_NOTIFY_INFORMATION {
    DWORD NextEntryOffset;
    DWORD Action;
    DWORD FileNameLength;
    WCHAR FileName[1];
} FILE_NOTIFY_INFORMATION, *PFILE_NOTIFY_INFORMATION;






typedef union _FILE_SEGMENT_ELEMENT {
    PVOID64 Buffer;
    ULONGLONG Alignment;
}FILE_SEGMENT_ELEMENT, *PFILE_SEGMENT_ELEMENT;









typedef struct _REPARSE_GUID_DATA_BUFFER {
    DWORD  ReparseTag;
    WORD   ReparseDataLength;
    WORD   Reserved;
    GUID   ReparseGuid;
    struct {
        BYTE   DataBuffer[1];
    } GenericReparseBuffer;
} REPARSE_GUID_DATA_BUFFER, *PREPARSE_GUID_DATA_BUFFER;













































































typedef enum _SYSTEM_POWER_STATE {
    PowerSystemUnspecified = 0,
    PowerSystemWorking     = 1,
    PowerSystemSleeping1   = 2,
    PowerSystemSleeping2   = 3,
    PowerSystemSleeping3   = 4,
    PowerSystemHibernate   = 5,
    PowerSystemShutdown    = 6,
    PowerSystemMaximum     = 7
} SYSTEM_POWER_STATE, *PSYSTEM_POWER_STATE;



typedef enum {
    PowerActionNone = 0,
    PowerActionReserved,
    PowerActionSleep,
    PowerActionHibernate,
    PowerActionShutdown,
    PowerActionShutdownReset,
    PowerActionShutdownOff,
    PowerActionWarmEject
} POWER_ACTION, *PPOWER_ACTION;

typedef enum _DEVICE_POWER_STATE {
    PowerDeviceUnspecified = 0,
    PowerDeviceD0,
    PowerDeviceD1,
    PowerDeviceD2,
    PowerDeviceD3,
    PowerDeviceMaximum
} DEVICE_POWER_STATE, *PDEVICE_POWER_STATE;







typedef DWORD EXECUTION_STATE;

typedef enum {
    LT_DONT_CARE,
    LT_LOWEST_LATENCY
} LATENCY_TIME;

















typedef struct CM_Power_Data_s {
    DWORD               PD_Size;
    DEVICE_POWER_STATE  PD_MostRecentPowerState;
    DWORD               PD_Capabilities;
    DWORD               PD_D1Latency;
    DWORD               PD_D2Latency;
    DWORD               PD_D3Latency;
    DEVICE_POWER_STATE  PD_PowerStateMapping[7];
    SYSTEM_POWER_STATE  PD_DeepestSystemWake;
} CM_POWER_DATA, *PCM_POWER_DATA;



typedef enum {
    SystemPowerPolicyAc,
    SystemPowerPolicyDc,
    VerifySystemPolicyAc,
    VerifySystemPolicyDc,
    SystemPowerCapabilities,
    SystemBatteryState,
    SystemPowerStateHandler,
    ProcessorStateHandler,
    SystemPowerPolicyCurrent,
    AdministratorPowerPolicy,
    SystemReserveHiberFile,
    ProcessorInformation,
    SystemPowerInformation,
    ProcessorStateHandler2,
    LastWakeTime,                                   
    LastSleepTime,                                  
    SystemExecutionState,
    SystemPowerStateNotifyHandler,
    ProcessorPowerPolicyAc,
    ProcessorPowerPolicyDc,
    VerifyProcessorPowerPolicyAc,
    VerifyProcessorPowerPolicyDc,
    ProcessorPowerPolicyCurrent,
    SystemPowerStateLogging,
    SystemPowerLoggingEntry
} POWER_INFORMATION_LEVEL;







typedef struct {
    DWORD       Granularity;
    DWORD       Capacity;
} BATTERY_REPORTING_SCALE, *PBATTERY_REPORTING_SCALE;






typedef struct {
    POWER_ACTION    Action;
    DWORD           Flags;
    DWORD           EventCode;
} POWER_ACTION_POLICY, *PPOWER_ACTION_POLICY;



















typedef struct {
    BOOLEAN                 Enable;
    BYTE                    Spare[3];
    DWORD                   BatteryLevel;
    POWER_ACTION_POLICY     PowerPolicy;
    SYSTEM_POWER_STATE      MinSystemState;
} SYSTEM_POWER_LEVEL, *PSYSTEM_POWER_LEVEL;
















typedef struct _SYSTEM_POWER_POLICY {
    DWORD                   Revision;       

    
    POWER_ACTION_POLICY     PowerButton;
    POWER_ACTION_POLICY     SleepButton;
    POWER_ACTION_POLICY     LidClose;
    SYSTEM_POWER_STATE      LidOpenWake;
    DWORD                   Reserved;

    
    POWER_ACTION_POLICY     Idle;
    DWORD                   IdleTimeout;
    BYTE                    IdleSensitivity;

    
    
    BYTE                    DynamicThrottle;

    BYTE                    Spare2[2];

    
    SYSTEM_POWER_STATE      MinSleep;
    SYSTEM_POWER_STATE      MaxSleep;
    SYSTEM_POWER_STATE      ReducedLatencySleep;
    DWORD                   WinLogonFlags;

    
    DWORD                   Spare3;
    DWORD                   DozeS4Timeout;

    
    DWORD                   BroadcastCapacityResolution;
    SYSTEM_POWER_LEVEL      DischargePolicy[4];

    
    DWORD                   VideoTimeout;
    BOOLEAN                 VideoDimDisplay;
    DWORD                   VideoReserved[3];

    
    DWORD                   SpindownTimeout;

    
    BOOLEAN                 OptimizeForPower;
    BYTE                    FanThrottleTolerance;
    BYTE                    ForcedThrottle;
    BYTE                    MinThrottle;
    POWER_ACTION_POLICY     OverThrottled;

} SYSTEM_POWER_POLICY, *PSYSTEM_POWER_POLICY;


typedef struct _PROCESSOR_POWER_POLICY_INFO {

    
    DWORD                   TimeCheck;                      
    DWORD                   DemoteLimit;                    
    DWORD                   PromoteLimit;                   

    
    BYTE                    DemotePercent;
    BYTE                    PromotePercent;
    BYTE                    Spare[2];

    
    DWORD                   AllowDemotion:1;
    DWORD                   AllowPromotion:1;
    DWORD                   Reserved:30;

} PROCESSOR_POWER_POLICY_INFO, *PPROCESSOR_POWER_POLICY_INFO;


typedef struct _PROCESSOR_POWER_POLICY {
    DWORD                       Revision;       

    
    BYTE                        DynamicThrottle;
    BYTE                        Spare[3];

    
    DWORD                       DisableCStates:1;
    DWORD                       Reserved:31;

    
    
    
    DWORD                       PolicyCount;
    PROCESSOR_POWER_POLICY_INFO Policy[3];

} PROCESSOR_POWER_POLICY, *PPROCESSOR_POWER_POLICY;


typedef struct _ADMINISTRATOR_POWER_POLICY {

    
    SYSTEM_POWER_STATE      MinSleep;
    SYSTEM_POWER_STATE      MaxSleep;

    
    DWORD                   MinVideoTimeout;
    DWORD                   MaxVideoTimeout;

    
    DWORD                   MinSpindownTimeout;
    DWORD                   MaxSpindownTimeout;
} ADMINISTRATOR_POWER_POLICY, *PADMINISTRATOR_POWER_POLICY;


typedef struct {
    
    BOOLEAN             PowerButtonPresent;
    BOOLEAN             SleepButtonPresent;
    BOOLEAN             LidPresent;
    BOOLEAN             SystemS1;
    BOOLEAN             SystemS2;
    BOOLEAN             SystemS3;
    BOOLEAN             SystemS4;           
    BOOLEAN             SystemS5;           
    BOOLEAN             HiberFilePresent;
    BOOLEAN             FullWake;
    BOOLEAN             VideoDimPresent;
    BOOLEAN             ApmPresent;
    BOOLEAN             UpsPresent;

    
    BOOLEAN             ThermalControl;
    BOOLEAN             ProcessorThrottle;
    BYTE                ProcessorMinThrottle;
    BYTE                ProcessorMaxThrottle;
    BYTE                spare2[4];

    
    BOOLEAN             DiskSpinDown;
    BYTE                spare3[8];

    
    BOOLEAN             SystemBatteriesPresent;
    BOOLEAN             BatteriesAreShortTerm;
    BATTERY_REPORTING_SCALE BatteryScale[3];

    
    SYSTEM_POWER_STATE  AcOnLineWake;
    SYSTEM_POWER_STATE  SoftLidWake;
    SYSTEM_POWER_STATE  RtcWake;
    SYSTEM_POWER_STATE  MinDeviceWakeState; 
    SYSTEM_POWER_STATE  DefaultLowLatencyWake;
} SYSTEM_POWER_CAPABILITIES, *PSYSTEM_POWER_CAPABILITIES;

typedef struct {
    BOOLEAN             AcOnLine;
    BOOLEAN             BatteryPresent;
    BOOLEAN             Charging;
    BOOLEAN             Discharging;
    BOOLEAN             Spare1[4];

    DWORD               MaxCapacity;
    DWORD               RemainingCapacity;
    DWORD               Rate;
    DWORD               EstimatedTime;

    DWORD               DefaultAlert1;
    DWORD               DefaultAlert2;
} SYSTEM_BATTERY_STATE, *PSYSTEM_BATTERY_STATE;










#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"























#pragma warning(disable:4103)

#pragma pack(push,4)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"
#line 6054 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"







#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"























#pragma warning(disable:4103)

#pragma pack(push,2)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 6062 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"









#line 6072 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef struct _IMAGE_DOS_HEADER {      
    WORD   e_magic;                     
    WORD   e_cblp;                      
    WORD   e_cp;                        
    WORD   e_crlc;                      
    WORD   e_cparhdr;                   
    WORD   e_minalloc;                  
    WORD   e_maxalloc;                  
    WORD   e_ss;                        
    WORD   e_sp;                        
    WORD   e_csum;                      
    WORD   e_ip;                        
    WORD   e_cs;                        
    WORD   e_lfarlc;                    
    WORD   e_ovno;                      
    WORD   e_res[4];                    
    WORD   e_oemid;                     
    WORD   e_oeminfo;                   
    WORD   e_res2[10];                  
    LONG   e_lfanew;                    
  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_OS2_HEADER {      
    WORD   ne_magic;                    
    CHAR   ne_ver;                      
    CHAR   ne_rev;                      
    WORD   ne_enttab;                   
    WORD   ne_cbenttab;                 
    LONG   ne_crc;                      
    WORD   ne_flags;                    
    WORD   ne_autodata;                 
    WORD   ne_heap;                     
    WORD   ne_stack;                    
    LONG   ne_csip;                     
    LONG   ne_sssp;                     
    WORD   ne_cseg;                     
    WORD   ne_cmod;                     
    WORD   ne_cbnrestab;                
    WORD   ne_segtab;                   
    WORD   ne_rsrctab;                  
    WORD   ne_restab;                   
    WORD   ne_modtab;                   
    WORD   ne_imptab;                   
    LONG   ne_nrestab;                  
    WORD   ne_cmovent;                  
    WORD   ne_align;                    
    WORD   ne_cres;                     
    BYTE   ne_exetyp;                   
    BYTE   ne_flagsothers;              
    WORD   ne_pretthunks;               
    WORD   ne_psegrefbytes;             
    WORD   ne_swaparea;                 
    WORD   ne_expver;                   
  } IMAGE_OS2_HEADER, *PIMAGE_OS2_HEADER;

typedef struct _IMAGE_VXD_HEADER {      
    WORD   e32_magic;                   
    BYTE   e32_border;                  
    BYTE   e32_worder;                  
    DWORD  e32_level;                   
    WORD   e32_cpu;                     
    WORD   e32_os;                      
    DWORD  e32_ver;                     
    DWORD  e32_mflags;                  
    DWORD  e32_mpages;                  
    DWORD  e32_startobj;                
    DWORD  e32_eip;                     
    DWORD  e32_stackobj;                
    DWORD  e32_esp;                     
    DWORD  e32_pagesize;                
    DWORD  e32_lastpagesize;            
    DWORD  e32_fixupsize;               
    DWORD  e32_fixupsum;                
    DWORD  e32_ldrsize;                 
    DWORD  e32_ldrsum;                  
    DWORD  e32_objtab;                  
    DWORD  e32_objcnt;                  
    DWORD  e32_objmap;                  
    DWORD  e32_itermap;                 
    DWORD  e32_rsrctab;                 
    DWORD  e32_rsrccnt;                 
    DWORD  e32_restab;                  
    DWORD  e32_enttab;                  
    DWORD  e32_dirtab;                  
    DWORD  e32_dircnt;                  
    DWORD  e32_fpagetab;                
    DWORD  e32_frectab;                 
    DWORD  e32_impmod;                  
    DWORD  e32_impmodcnt;               
    DWORD  e32_impproc;                 
    DWORD  e32_pagesum;                 
    DWORD  e32_datapage;                
    DWORD  e32_preload;                 
    DWORD  e32_nrestab;                 
    DWORD  e32_cbnrestab;               
    DWORD  e32_nressum;                 
    DWORD  e32_autodata;                
    DWORD  e32_debuginfo;               
    DWORD  e32_debuglen;                
    DWORD  e32_instpreload;             
    DWORD  e32_instdemand;              
    DWORD  e32_heapsize;                
    BYTE   e32_res3[12];                
    DWORD  e32_winresoff;
    DWORD  e32_winreslen;
    WORD   e32_devid;                   
    WORD   e32_ddkver;                  
  } IMAGE_VXD_HEADER, *PIMAGE_VXD_HEADER;


#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 6184 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 6185 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





typedef struct _IMAGE_FILE_HEADER {
    WORD    Machine;
    WORD    NumberOfSections;
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;






















































typedef struct _IMAGE_DATA_DIRECTORY {
    DWORD   VirtualAddress;
    DWORD   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;







typedef struct _IMAGE_OPTIONAL_HEADER {
    
    
    

    WORD    Magic;
    BYTE    MajorLinkerVersion;
    BYTE    MinorLinkerVersion;
    DWORD   SizeOfCode;
    DWORD   SizeOfInitializedData;
    DWORD   SizeOfUninitializedData;
    DWORD   AddressOfEntryPoint;
    DWORD   BaseOfCode;
    DWORD   BaseOfData;

    
    
    

    DWORD   ImageBase;
    DWORD   SectionAlignment;
    DWORD   FileAlignment;
    WORD    MajorOperatingSystemVersion;
    WORD    MinorOperatingSystemVersion;
    WORD    MajorImageVersion;
    WORD    MinorImageVersion;
    WORD    MajorSubsystemVersion;
    WORD    MinorSubsystemVersion;
    DWORD   Win32VersionValue;
    DWORD   SizeOfImage;
    DWORD   SizeOfHeaders;
    DWORD   CheckSum;
    WORD    Subsystem;
    WORD    DllCharacteristics;
    DWORD   SizeOfStackReserve;
    DWORD   SizeOfStackCommit;
    DWORD   SizeOfHeapReserve;
    DWORD   SizeOfHeapCommit;
    DWORD   LoaderFlags;
    DWORD   NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[16];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_ROM_OPTIONAL_HEADER {
    WORD   Magic;
    BYTE   MajorLinkerVersion;
    BYTE   MinorLinkerVersion;
    DWORD  SizeOfCode;
    DWORD  SizeOfInitializedData;
    DWORD  SizeOfUninitializedData;
    DWORD  AddressOfEntryPoint;
    DWORD  BaseOfCode;
    DWORD  BaseOfData;
    DWORD  BaseOfBss;
    DWORD  GprMask;
    DWORD  CprMask[4];
    DWORD  GpValue;
} IMAGE_ROM_OPTIONAL_HEADER, *PIMAGE_ROM_OPTIONAL_HEADER;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
    WORD        Magic;
    BYTE        MajorLinkerVersion;
    BYTE        MinorLinkerVersion;
    DWORD       SizeOfCode;
    DWORD       SizeOfInitializedData;
    DWORD       SizeOfUninitializedData;
    DWORD       AddressOfEntryPoint;
    DWORD       BaseOfCode;
    ULONGLONG   ImageBase;
    DWORD       SectionAlignment;
    DWORD       FileAlignment;
    WORD        MajorOperatingSystemVersion;
    WORD        MinorOperatingSystemVersion;
    WORD        MajorImageVersion;
    WORD        MinorImageVersion;
    WORD        MajorSubsystemVersion;
    WORD        MinorSubsystemVersion;
    DWORD       Win32VersionValue;
    DWORD       SizeOfImage;
    DWORD       SizeOfHeaders;
    DWORD       CheckSum;
    WORD        Subsystem;
    WORD        DllCharacteristics;
    ULONGLONG   SizeOfStackReserve;
    ULONGLONG   SizeOfStackCommit;
    ULONGLONG   SizeOfHeapReserve;
    ULONGLONG   SizeOfHeapCommit;
    DWORD       LoaderFlags;
    DWORD       NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[16];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;
















typedef IMAGE_OPTIONAL_HEADER32             IMAGE_OPTIONAL_HEADER;
typedef PIMAGE_OPTIONAL_HEADER32            PIMAGE_OPTIONAL_HEADER;


#line 6376 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef struct _IMAGE_NT_HEADERS64 {
    DWORD Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

typedef struct _IMAGE_NT_HEADERS {
    DWORD Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

typedef struct _IMAGE_ROM_HEADERS {
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_ROM_OPTIONAL_HEADER OptionalHeader;
} IMAGE_ROM_HEADERS, *PIMAGE_ROM_HEADERS;





typedef IMAGE_NT_HEADERS32                  IMAGE_NT_HEADERS;
typedef PIMAGE_NT_HEADERS32                 PIMAGE_NT_HEADERS;
#line 6401 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





























































typedef struct ANON_OBJECT_HEADER {
    WORD    Sig1;            
    WORD    Sig2;            
    WORD    Version;         
    WORD    Machine;
    DWORD   TimeDateStamp;
    CLSID   ClassID;         
    DWORD   SizeOfData;      
} ANON_OBJECT_HEADER;







typedef struct _IMAGE_SECTION_HEADER {
    BYTE    Name[8];
    union {
            DWORD   PhysicalAddress;
            DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

































































#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"























#pragma warning(disable:4103)

#pragma pack(push,2)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 6559 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 6560 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





typedef struct _IMAGE_SYMBOL {
    union {
        BYTE    ShortName[8];
        struct {
            DWORD   Short;     
            DWORD   Long;      
        } Name;
        DWORD   LongName[2];    
    } N;
    DWORD   Value;
    SHORT   SectionNumber;
    WORD    Type;
    BYTE    StorageClass;
    BYTE    NumberOfAuxSymbols;
} IMAGE_SYMBOL;
typedef IMAGE_SYMBOL  *PIMAGE_SYMBOL;


































































































#line 6680 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




#line 6685 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





#line 6691 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




#line 6696 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



#line 6700 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


#line 6703 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





typedef union _IMAGE_AUX_SYMBOL {
    struct {
        DWORD    TagIndex;                      
        union {
            struct {
                WORD    Linenumber;             
                WORD    Size;                   
            } LnSz;
           DWORD    TotalSize;
        } Misc;
        union {
            struct {                            
                DWORD    PointerToLinenumber;
                DWORD    PointerToNextFunction;
            } Function;
            struct {                            
                WORD     Dimension[4];
            } Array;
        } FcnAry;
        WORD    TvIndex;                        
    } Sym;
    struct {
        BYTE    Name[18];
    } File;
    struct {
        DWORD   Length;                         
        WORD    NumberOfRelocations;            
        WORD    NumberOfLinenumbers;            
        DWORD   CheckSum;                       
        SHORT   Number;                         
        BYTE    Selection;                      
    } Section;
} IMAGE_AUX_SYMBOL;
typedef IMAGE_AUX_SYMBOL  *PIMAGE_AUX_SYMBOL;



typedef enum IMAGE_AUX_SYMBOL_TYPE {
    IMAGE_AUX_SYMBOL_TYPE_TOKEN_DEF = 1,
} IMAGE_AUX_SYMBOL_TYPE;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"























#pragma warning(disable:4103)

#pragma pack(push,2)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 6750 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef struct IMAGE_AUX_SYMBOL_TOKEN_DEF {
    BYTE  bAuxType;                  
    BYTE  bReserved;                 
    DWORD SymbolTableIndex;
    BYTE  rgbReserved[12];           
} IMAGE_AUX_SYMBOL_TOKEN_DEF;

typedef IMAGE_AUX_SYMBOL_TOKEN_DEF  *PIMAGE_AUX_SYMBOL_TOKEN_DEF;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 6761 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





















typedef struct _IMAGE_RELOCATION {
    union {
        DWORD   VirtualAddress;
        DWORD   RelocCount;             
    };
    DWORD   SymbolTableIndex;
    WORD    Type;
} IMAGE_RELOCATION;
typedef IMAGE_RELOCATION  *PIMAGE_RELOCATION;






































































































































































































































































































typedef struct _IMAGE_LINENUMBER {
    union {
        DWORD   SymbolTableIndex;               
        DWORD   VirtualAddress;                 
    } Type;
    WORD    Linenumber;                         
} IMAGE_LINENUMBER;
typedef IMAGE_LINENUMBER  *PIMAGE_LINENUMBER;




#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 7098 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#line 7099 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





typedef struct _IMAGE_BASE_RELOCATION {
    DWORD   VirtualAddress;
    DWORD   SizeOfBlock;

} IMAGE_BASE_RELOCATION;
typedef IMAGE_BASE_RELOCATION  * PIMAGE_BASE_RELOCATION;





























typedef struct _IMAGE_ARCHIVE_MEMBER_HEADER {
    BYTE     Name[16];                          
    BYTE     Date[12];                          
    BYTE     UserID[6];                         
    BYTE     GroupID[6];                        
    BYTE     Mode[8];                           
    BYTE     Size[10];                          
    BYTE     EndHeader[2];                      
} IMAGE_ARCHIVE_MEMBER_HEADER, *PIMAGE_ARCHIVE_MEMBER_HEADER;











typedef struct _IMAGE_EXPORT_DIRECTORY {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   Name;
    DWORD   Base;
    DWORD   NumberOfFunctions;
    DWORD   NumberOfNames;
    DWORD   AddressOfFunctions;     
    DWORD   AddressOfNames;         
    DWORD   AddressOfNameOrdinals;  
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;





typedef struct _IMAGE_IMPORT_BY_NAME {
    WORD    Hint;
    BYTE    Name[1];
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack8.h"























#pragma warning(disable:4103)

#pragma pack(push,8)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack8.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack8.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack8.h"
#line 7183 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef struct _IMAGE_THUNK_DATA64 {
    union {
        ULONGLONG ForwarderString;  
        ULONGLONG Function;         
        ULONGLONG Ordinal;
        ULONGLONG AddressOfData;    
    } u1;
} IMAGE_THUNK_DATA64;
typedef IMAGE_THUNK_DATA64 * PIMAGE_THUNK_DATA64;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 7195 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef struct _IMAGE_THUNK_DATA32 {
    union {
        DWORD ForwarderString;      
        DWORD Function;             
        DWORD Ordinal;
        DWORD AddressOfData;        
    } u1;
} IMAGE_THUNK_DATA32;
typedef IMAGE_THUNK_DATA32 * PIMAGE_THUNK_DATA32;












typedef void
(__stdcall *PIMAGE_TLS_CALLBACK) (
    PVOID DllHandle,
    DWORD Reason,
    PVOID Reserved
    );

typedef struct _IMAGE_TLS_DIRECTORY64 {
    ULONGLONG   StartAddressOfRawData;
    ULONGLONG   EndAddressOfRawData;
    ULONGLONG   AddressOfIndex;         
    ULONGLONG   AddressOfCallBacks;     
    DWORD   SizeOfZeroFill;
    DWORD   Characteristics;
} IMAGE_TLS_DIRECTORY64;
typedef IMAGE_TLS_DIRECTORY64 * PIMAGE_TLS_DIRECTORY64;

typedef struct _IMAGE_TLS_DIRECTORY32 {
    DWORD   StartAddressOfRawData;
    DWORD   EndAddressOfRawData;
    DWORD   AddressOfIndex;             
    DWORD   AddressOfCallBacks;         
    DWORD   SizeOfZeroFill;
    DWORD   Characteristics;
} IMAGE_TLS_DIRECTORY32;
typedef IMAGE_TLS_DIRECTORY32 * PIMAGE_TLS_DIRECTORY32;












typedef IMAGE_THUNK_DATA32              IMAGE_THUNK_DATA;
typedef PIMAGE_THUNK_DATA32             PIMAGE_THUNK_DATA;

typedef IMAGE_TLS_DIRECTORY32           IMAGE_TLS_DIRECTORY;
typedef PIMAGE_TLS_DIRECTORY32          PIMAGE_TLS_DIRECTORY;
#line 7261 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    union {
        DWORD   Characteristics;            
        DWORD   OriginalFirstThunk;         
    };
    DWORD   TimeDateStamp;                  
                                            
                                            
                                            

    DWORD   ForwarderChain;                 
    DWORD   Name;
    DWORD   FirstThunk;                     
} IMAGE_IMPORT_DESCRIPTOR;
typedef IMAGE_IMPORT_DESCRIPTOR  *PIMAGE_IMPORT_DESCRIPTOR;





typedef struct _IMAGE_BOUND_IMPORT_DESCRIPTOR {
    DWORD   TimeDateStamp;
    WORD    OffsetModuleName;
    WORD    NumberOfModuleForwarderRefs;

} IMAGE_BOUND_IMPORT_DESCRIPTOR,  *PIMAGE_BOUND_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_BOUND_FORWARDER_REF {
    DWORD   TimeDateStamp;
    WORD    OffsetModuleName;
    WORD    Reserved;
} IMAGE_BOUND_FORWARDER_REF, *PIMAGE_BOUND_FORWARDER_REF;



















typedef struct _IMAGE_RESOURCE_DIRECTORY {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    WORD    NumberOfNamedEntries;
    WORD    NumberOfIdEntries;

} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;


















typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY {
    union {
        struct {
            DWORD NameOffset:31;
            DWORD NameIsString:1;
        };
        DWORD   Name;
        WORD    Id;
    };
    union {
        DWORD   OffsetToData;
        struct {
            DWORD   OffsetToDirectory:31;
            DWORD   DataIsDirectory:1;
        };
    };
} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;










typedef struct _IMAGE_RESOURCE_DIRECTORY_STRING {
    WORD    Length;
    CHAR    NameString[ 1 ];
} IMAGE_RESOURCE_DIRECTORY_STRING, *PIMAGE_RESOURCE_DIRECTORY_STRING;


typedef struct _IMAGE_RESOURCE_DIR_STRING_U {
    WORD    Length;
    WCHAR   NameString[ 1 ];
} IMAGE_RESOURCE_DIR_STRING_U, *PIMAGE_RESOURCE_DIR_STRING_U;











typedef struct _IMAGE_RESOURCE_DATA_ENTRY {
    DWORD   OffsetToData;
    DWORD   Size;
    DWORD   CodePage;
    DWORD   Reserved;
} IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;





typedef struct {
    DWORD   Size;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   GlobalFlagsClear;
    DWORD   GlobalFlagsSet;
    DWORD   CriticalSectionDefaultTimeout;
    DWORD   DeCommitFreeBlockThreshold;
    DWORD   DeCommitTotalFreeThreshold;
    DWORD   LockPrefixTable;            
    DWORD   MaximumAllocationSize;
    DWORD   VirtualMemoryThreshold;
    DWORD   ProcessHeapFlags;
    DWORD   ProcessAffinityMask;
    WORD    CSDVersion;
    WORD    Reserved1;
    DWORD   EditList;                   
    DWORD   SecurityCookie;             
    DWORD   SEHandlerTable;             
    DWORD   SEHandlerCount;
} IMAGE_LOAD_CONFIG_DIRECTORY32, *PIMAGE_LOAD_CONFIG_DIRECTORY32;

typedef struct {
    DWORD      Size;
    DWORD      TimeDateStamp;
    WORD       MajorVersion;
    WORD       MinorVersion;
    DWORD      GlobalFlagsClear;
    DWORD      GlobalFlagsSet;
    DWORD      CriticalSectionDefaultTimeout;
    ULONGLONG  DeCommitFreeBlockThreshold;
    ULONGLONG  DeCommitTotalFreeThreshold;
    ULONGLONG  LockPrefixTable;         
    ULONGLONG  MaximumAllocationSize;
    ULONGLONG  VirtualMemoryThreshold;
    ULONGLONG  ProcessAffinityMask;
    DWORD      ProcessHeapFlags;
    WORD       CSDVersion;
    WORD       Reserved1;
    ULONGLONG  EditList;                
    ULONGLONG  SecurityCookie;          
    ULONGLONG  SEHandlerTable;          
    ULONGLONG  SEHandlerCount;
} IMAGE_LOAD_CONFIG_DIRECTORY64, *PIMAGE_LOAD_CONFIG_DIRECTORY64;





typedef IMAGE_LOAD_CONFIG_DIRECTORY32     IMAGE_LOAD_CONFIG_DIRECTORY;
typedef PIMAGE_LOAD_CONFIG_DIRECTORY32    PIMAGE_LOAD_CONFIG_DIRECTORY;
#line 7452 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"










typedef struct _IMAGE_CE_RUNTIME_FUNCTION_ENTRY {
    DWORD FuncStart;
    DWORD PrologLen : 8;
    DWORD FuncLen : 22;
    DWORD ThirtyTwoBit : 1;
    DWORD ExceptionFlag : 1;
} IMAGE_CE_RUNTIME_FUNCTION_ENTRY, * PIMAGE_CE_RUNTIME_FUNCTION_ENTRY;

typedef struct _IMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY {
    ULONGLONG BeginAddress;
    ULONGLONG EndAddress;
    ULONGLONG ExceptionHandler;
    ULONGLONG HandlerData;
    ULONGLONG PrologEndAddress;
} IMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY, *PIMAGE_ALPHA64_RUNTIME_FUNCTION_ENTRY;

typedef struct _IMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY {
    DWORD BeginAddress;
    DWORD EndAddress;
    DWORD ExceptionHandler;
    DWORD HandlerData;
    DWORD PrologEndAddress;
} IMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY, *PIMAGE_ALPHA_RUNTIME_FUNCTION_ENTRY;

typedef struct _IMAGE_RUNTIME_FUNCTION_ENTRY {
    DWORD BeginAddress;
    DWORD EndAddress;
    DWORD UnwindInfoAddress;
} _IMAGE_RUNTIME_FUNCTION_ENTRY, *_PIMAGE_RUNTIME_FUNCTION_ENTRY;

typedef  _IMAGE_RUNTIME_FUNCTION_ENTRY  IMAGE_IA64_RUNTIME_FUNCTION_ENTRY;
typedef _PIMAGE_RUNTIME_FUNCTION_ENTRY PIMAGE_IA64_RUNTIME_FUNCTION_ENTRY;








#line 7503 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"




#line 7508 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef  _IMAGE_RUNTIME_FUNCTION_ENTRY  IMAGE_RUNTIME_FUNCTION_ENTRY;
typedef _PIMAGE_RUNTIME_FUNCTION_ENTRY PIMAGE_RUNTIME_FUNCTION_ENTRY;

#line 7513 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





typedef struct _IMAGE_DEBUG_DIRECTORY {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   Type;
    DWORD   SizeOfData;
    DWORD   AddressOfRawData;
    DWORD   PointerToRawData;
} IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;















typedef struct _IMAGE_COFF_SYMBOLS_HEADER {
    DWORD   NumberOfSymbols;
    DWORD   LvaToFirstSymbol;
    DWORD   NumberOfLinenumbers;
    DWORD   LvaToFirstLinenumber;
    DWORD   RvaToFirstByteOfCode;
    DWORD   RvaToLastByteOfCode;
    DWORD   RvaToFirstByteOfData;
    DWORD   RvaToLastByteOfData;
} IMAGE_COFF_SYMBOLS_HEADER, *PIMAGE_COFF_SYMBOLS_HEADER;






typedef struct _FPO_DATA {
    DWORD       ulOffStart;             
    DWORD       cbProcSize;             
    DWORD       cdwLocals;              
    WORD        cdwParams;              
    WORD        cbProlog : 8;           
    WORD        cbRegs   : 3;           
    WORD        fHasSEH  : 1;           
    WORD        fUseBP   : 1;           
    WORD        reserved : 1;           
    WORD        cbFrame  : 2;           
} FPO_DATA, *PFPO_DATA;





typedef struct _IMAGE_DEBUG_MISC {
    DWORD       DataType;               
    DWORD       Length;                 
                                        
    BOOLEAN     Unicode;                
    BYTE        Reserved[ 3 ];
    BYTE        Data[ 1 ];              
} IMAGE_DEBUG_MISC, *PIMAGE_DEBUG_MISC;








typedef struct _IMAGE_FUNCTION_ENTRY {
    DWORD   StartingAddress;
    DWORD   EndingAddress;
    DWORD   EndOfPrologue;
} IMAGE_FUNCTION_ENTRY, *PIMAGE_FUNCTION_ENTRY;

typedef struct _IMAGE_FUNCTION_ENTRY64 {
    ULONGLONG   StartingAddress;
    ULONGLONG   EndingAddress;
    union {
        ULONGLONG   EndOfPrologue;
        ULONGLONG   UnwindInfoAddress;
    };
} IMAGE_FUNCTION_ENTRY64, *PIMAGE_FUNCTION_ENTRY64;





















typedef struct _IMAGE_SEPARATE_DEBUG_HEADER {
    WORD        Signature;
    WORD        Flags;
    WORD        Machine;
    WORD        Characteristics;
    DWORD       TimeDateStamp;
    DWORD       CheckSum;
    DWORD       ImageBase;
    DWORD       SizeOfImage;
    DWORD       NumberOfSections;
    DWORD       ExportedNamesSize;
    DWORD       DebugDirectorySize;
    DWORD       SectionAlignment;
    DWORD       Reserved[2];
} IMAGE_SEPARATE_DEBUG_HEADER, *PIMAGE_SEPARATE_DEBUG_HEADER;

typedef struct _NON_PAGED_DEBUG_INFO {
    WORD        Signature;
    WORD        Flags;
    DWORD       Size;
    WORD        Machine;
    WORD        Characteristics;
    DWORD       TimeDateStamp;
    DWORD       CheckSum;
    DWORD       SizeOfImage;
    ULONGLONG   ImageBase;
    
    
} NON_PAGED_DEBUG_INFO, *PNON_PAGED_DEBUG_INFO;







#line 7664 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"



                                                









typedef struct _ImageArchitectureHeader {
    unsigned int AmaskValue: 1;                 
                                                
    int :7;                                     
    unsigned int AmaskShift: 8;                 
    int :16;                                    
    DWORD FirstEntryRVA;                        
} IMAGE_ARCHITECTURE_HEADER, *PIMAGE_ARCHITECTURE_HEADER;

typedef struct _ImageArchitectureEntry {
    DWORD FixupInstRVA;                         
    DWORD NewInst;                              
} IMAGE_ARCHITECTURE_ENTRY, *PIMAGE_ARCHITECTURE_ENTRY;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 7692 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"








typedef struct IMPORT_OBJECT_HEADER {
    WORD    Sig1;                       
    WORD    Sig2;                       
    WORD    Version;
    WORD    Machine;
    DWORD   TimeDateStamp;              
    DWORD   SizeOfData;                 

    union {
        WORD    Ordinal;                
        WORD    Hint;
    };

    WORD    Type : 2;                   
    WORD    NameType : 3;               
    WORD    Reserved : 11;              
} IMPORT_OBJECT_HEADER;

typedef enum IMPORT_OBJECT_TYPE
{
    IMPORT_OBJECT_CODE = 0,
    IMPORT_OBJECT_DATA = 1,
    IMPORT_OBJECT_CONST = 2,
} IMPORT_OBJECT_TYPE;

typedef enum IMPORT_OBJECT_NAME_TYPE
{
    IMPORT_OBJECT_ORDINAL = 0,          
    IMPORT_OBJECT_NAME = 1,             
    IMPORT_OBJECT_NAME_NO_PREFIX = 2,   
    IMPORT_OBJECT_NAME_UNDECORATE = 3,  
                                        
} IMPORT_OBJECT_NAME_TYPE;





typedef enum ReplacesCorHdrNumericDefines
{

    COMIMAGE_FLAGS_ILONLY               =0x00000001,
    COMIMAGE_FLAGS_32BITREQUIRED        =0x00000002,
    COMIMAGE_FLAGS_IL_LIBRARY           =0x00000004,
    COMIMAGE_FLAGS_STRONGNAMESIGNED     =0x00000008,
    COMIMAGE_FLAGS_TRACKDEBUGDATA       =0x00010000,


    COR_VERSION_MAJOR_V2                =2,
    COR_VERSION_MAJOR                   =COR_VERSION_MAJOR_V2,
    COR_VERSION_MINOR                   =0,
    COR_DELETED_NAME_LENGTH             =8,
    COR_VTABLEGAP_NAME_LENGTH           =8,


    NATIVE_TYPE_MAX_CB                  =1,   
    COR_ILMETHOD_SECT_SMALL_MAX_DATASIZE=0xFF,


    IMAGE_COR_MIH_METHODRVA             =0x01,
    IMAGE_COR_MIH_EHRVA                 =0x02,    
    IMAGE_COR_MIH_BASICBLOCK            =0x08,


    COR_VTABLE_32BIT                    =0x01,          
    COR_VTABLE_64BIT                    =0x02,          
    COR_VTABLE_FROM_UNMANAGED           =0x04,          
    COR_VTABLE_CALL_MOST_DERIVED        =0x10,          


    IMAGE_COR_EATJ_THUNK_SIZE           =32,            


    
    MAX_CLASS_NAME                      =1024,
    MAX_PACKAGE_NAME                    =1024,
} ReplacesCorHdrNumericDefines;


typedef struct IMAGE_COR20_HEADER
{
    
    DWORD                   cb;              
    WORD                    MajorRuntimeVersion;
    WORD                    MinorRuntimeVersion;
    
    
    IMAGE_DATA_DIRECTORY    MetaData;        
    DWORD                   Flags;           
    DWORD                   EntryPointToken;
    
    
    IMAGE_DATA_DIRECTORY    Resources;
    IMAGE_DATA_DIRECTORY    StrongNameSignature;

    
    IMAGE_DATA_DIRECTORY    CodeManagerTable;
    IMAGE_DATA_DIRECTORY    VTableFixups;
    IMAGE_DATA_DIRECTORY    ExportAddressTableJumps;

    
    IMAGE_DATA_DIRECTORY    ManagedNativeHeader;
    
} IMAGE_COR20_HEADER, *PIMAGE_COR20_HEADER;

#line 7806 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"














#line 7821 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
























#line 7846 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





#line 7852 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"










#line 7863 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef union _SLIST_HEADER {
    ULONGLONG Alignment;
    struct {
        SINGLE_LIST_ENTRY Next;
        WORD   Depth;
        WORD   Sequence;
    };
} SLIST_HEADER, *PSLIST_HEADER;

#line 7874 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 7876 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


__declspec(dllimport)
void
__stdcall
RtlInitializeSListHead (
     PSLIST_HEADER ListHead
    );

__declspec(dllimport)
PSINGLE_LIST_ENTRY
RtlFirstEntrySList (
     const SLIST_HEADER *ListHead
    );

__declspec(dllimport)
PSINGLE_LIST_ENTRY
__stdcall
RtlInterlockedPopEntrySList (
     PSLIST_HEADER ListHead
    );

__declspec(dllimport)
PSINGLE_LIST_ENTRY
__stdcall
RtlInterlockedPushEntrySList (
     PSLIST_HEADER ListHead,
     PSINGLE_LIST_ENTRY ListEntry
    );

__declspec(dllimport)
PSINGLE_LIST_ENTRY
__stdcall
RtlInterlockedFlushSList (
     PSLIST_HEADER ListHead
    );

__declspec(dllimport)
WORD  
__stdcall
RtlQueryDepthSList (
     PSLIST_HEADER ListHead
    );













































__declspec(dllimport)
SIZE_T
__stdcall
RtlCompareMemory (
    const void *Source1,
    const void *Source2,
    SIZE_T Length
    );








































#line 8013 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"






#line 8020 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


__forceinline
PVOID
RtlSecureZeroMemory(
     PVOID ptr,
     SIZE_T cnt
    )
{
    volatile char *vptr = (volatile char *)ptr;
    while (cnt) {
        *vptr = 0;
        vptr++;
        cnt--;
    }
    return ptr;
}
#line 8038 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


typedef struct _MESSAGE_RESOURCE_ENTRY {
    WORD   Length;
    WORD   Flags;
    BYTE  Text[ 1 ];
} MESSAGE_RESOURCE_ENTRY, *PMESSAGE_RESOURCE_ENTRY;



typedef struct _MESSAGE_RESOURCE_BLOCK {
    DWORD LowId;
    DWORD HighId;
    DWORD OffsetToEntries;
} MESSAGE_RESOURCE_BLOCK, *PMESSAGE_RESOURCE_BLOCK;

typedef struct _MESSAGE_RESOURCE_DATA {
    DWORD NumberOfBlocks;
    MESSAGE_RESOURCE_BLOCK Blocks[ 1 ];
} MESSAGE_RESOURCE_DATA, *PMESSAGE_RESOURCE_DATA;

typedef struct _OSVERSIONINFOA {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    CHAR   szCSDVersion[ 128 ];     
} OSVERSIONINFOA, *POSVERSIONINFOA, *LPOSVERSIONINFOA;

typedef struct _OSVERSIONINFOW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR  szCSDVersion[ 128 ];     
} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW, RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;





typedef OSVERSIONINFOA OSVERSIONINFO;
typedef POSVERSIONINFOA POSVERSIONINFO;
typedef LPOSVERSIONINFOA LPOSVERSIONINFO;
#line 8085 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

typedef struct _OSVERSIONINFOEXA {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    CHAR   szCSDVersion[ 128 ];     
    WORD   wServicePackMajor;
    WORD   wServicePackMinor;
    WORD   wSuiteMask;
    BYTE  wProductType;
    BYTE  wReserved;
} OSVERSIONINFOEXA, *POSVERSIONINFOEXA, *LPOSVERSIONINFOEXA;
typedef struct _OSVERSIONINFOEXW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    WCHAR  szCSDVersion[ 128 ];     
    WORD   wServicePackMajor;
    WORD   wServicePackMinor;
    WORD   wSuiteMask;
    BYTE  wProductType;
    BYTE  wReserved;
} OSVERSIONINFOEXW, *POSVERSIONINFOEXW, *LPOSVERSIONINFOEXW, RTL_OSVERSIONINFOEXW, *PRTL_OSVERSIONINFOEXW;





typedef OSVERSIONINFOEXA OSVERSIONINFOEX;
typedef POSVERSIONINFOEXA POSVERSIONINFOEX;
typedef LPOSVERSIONINFOEXA LPOSVERSIONINFOEX;
#line 8121 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


























































ULONGLONG
__stdcall
VerSetConditionMask(
          ULONGLONG   ConditionMask,
          DWORD   TypeMask,
          BYTE    Condition
        );


typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD   Type;
    WORD   CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Spare[ 2 ];
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG, RTL_RESOURCE_DEBUG, *PRTL_RESOURCE_DEBUG;




typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;

    
    
    
    

    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;        
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;        
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef void (__stdcall * RTL_VERIFIER_DLL_LOAD_CALLBACK) (
    PWSTR DllName,
    PVOID DllBase,
    SIZE_T DllSize,
    PVOID Reserved
    );

typedef void (__stdcall * RTL_VERIFIER_DLL_UNLOAD_CALLBACK) (
    PWSTR DllName,
    PVOID DllBase,
    SIZE_T DllSize,
    PVOID Reserved
    );

typedef struct _RTL_VERIFIER_THUNK_DESCRIPTOR {

    PCHAR ThunkName;
    PVOID ThunkOldAddress;
    PVOID ThunkNewAddress;

} RTL_VERIFIER_THUNK_DESCRIPTOR, *PRTL_VERIFIER_THUNK_DESCRIPTOR;

typedef struct _RTL_VERIFIER_DLL_DESCRIPTOR {

    PWCHAR DllName;
    DWORD DllFlags;
    PVOID DllAddress;
    PRTL_VERIFIER_THUNK_DESCRIPTOR DllThunks;

} RTL_VERIFIER_DLL_DESCRIPTOR, *PRTL_VERIFIER_DLL_DESCRIPTOR;

typedef struct _RTL_VERIFIER_PROVIDER_DESCRIPTOR {

    
    
    

    DWORD Length;        
    PRTL_VERIFIER_DLL_DESCRIPTOR ProviderDlls;
    RTL_VERIFIER_DLL_LOAD_CALLBACK ProviderDllLoadCallback;
    RTL_VERIFIER_DLL_UNLOAD_CALLBACK ProviderDllUnloadCallback;
    
    
    
    
        
    PWSTR VerifierImage;
    DWORD VerifierFlags;
    DWORD VerifierDebug;
    
    PVOID RtlpGetStackTraceAddress;
    PVOID RtlpDebugPageHeapCreate;
    PVOID RtlpDebugPageHeapDestroy;

} RTL_VERIFIER_PROVIDER_DESCRIPTOR, *PRTL_VERIFIER_PROVIDER_DESCRIPTOR;












































































































void
__stdcall
RtlApplicationVerifierStop (
    ULONG_PTR Code,
    PCHAR Message,
    ULONG_PTR Param1, PCHAR Description1,
    ULONG_PTR Param2, PCHAR Description2,
    ULONG_PTR Param3, PCHAR Description3,
    ULONG_PTR Param4, PCHAR Description4
    );

typedef LONG (__stdcall *PVECTORED_EXCEPTION_HANDLER)(
    struct _EXCEPTION_POINTERS *ExceptionInfo
    );








typedef enum _HEAP_INFORMATION_CLASS {

    HeapCompatibilityInformation

} HEAP_INFORMATION_CLASS;


DWORD   
__stdcall
RtlSetHeapInformation (
     PVOID HeapHandle,
     HEAP_INFORMATION_CLASS HeapInformationClass,
     PVOID HeapInformation ,
     SIZE_T HeapInformationLength 
    );

DWORD   
__stdcall
RtlQueryHeapInformation (
     PVOID HeapHandle,
     HEAP_INFORMATION_CLASS HeapInformationClass,
     PVOID HeapInformation ,
     SIZE_T HeapInformationLength ,
     PSIZE_T ReturnLength 
    );





DWORD
__stdcall
RtlMultipleAllocateHeap (
     PVOID HeapHandle,
     DWORD Flags,
     SIZE_T Size,
     DWORD Count,
     PVOID * Array
    );

DWORD
__stdcall
RtlMultipleFreeHeap (
     PVOID HeapHandle,
     DWORD Flags,
     DWORD Count,
     PVOID * Array
    );
    











typedef void (__stdcall * WAITORTIMERCALLBACKFUNC) (PVOID, BOOLEAN );   
typedef void (__stdcall * WORKERCALLBACKFUNC) (PVOID );                 
typedef void (__stdcall * APC_CALLBACK_FUNCTION) (DWORD   , PVOID, PVOID); 



typedef enum _ACTIVATION_CONTEXT_INFO_CLASS {
    ActivationContextBasicInformation                       = 1,
    ActivationContextDetailedInformation                    = 2,
    AssemblyDetailedInformationInActivationContext          = 3,
    FileInformationInAssemblyOfAssemblyInActivationContext  = 4,
    MaxActivationContextInfoClass,

    
    
    
    AssemblyDetailedInformationInActivationContxt           = 3,
    FileInformationInAssemblyOfAssemblyInActivationContxt   = 4
} ACTIVATION_CONTEXT_INFO_CLASS;




typedef struct _ACTIVATION_CONTEXT_QUERY_INDEX {
    DWORD ulAssemblyIndex; 
    DWORD ulFileIndexInAssembly; 
} ACTIVATION_CONTEXT_QUERY_INDEX, * PACTIVATION_CONTEXT_QUERY_INDEX;

typedef const struct _ACTIVATION_CONTEXT_QUERY_INDEX * PCACTIVATION_CONTEXT_QUERY_INDEX;







typedef struct _ASSEMBLY_FILE_DETAILED_INFORMATION {
    DWORD ulFlags;
    DWORD ulFilenameLength;
    DWORD ulPathLength; 

    PCWSTR lpFileName;
    PCWSTR lpFilePath;   
} ASSEMBLY_FILE_DETAILED_INFORMATION, *PASSEMBLY_FILE_DETAILED_INFORMATION;
typedef const ASSEMBLY_FILE_DETAILED_INFORMATION *PCASSEMBLY_FILE_DETAILED_INFORMATION;










typedef struct _ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION {
    DWORD ulFlags;
    DWORD ulEncodedAssemblyIdentityLength;      
    DWORD ulManifestPathType;                   
    DWORD ulManifestPathLength;                 
    LARGE_INTEGER liManifestLastWriteTime;      
    DWORD ulPolicyPathType;                     
    DWORD ulPolicyPathLength;                   
    LARGE_INTEGER liPolicyLastWriteTime;        
    DWORD ulMetadataSatelliteRosterIndex;
    
    DWORD ulManifestVersionMajor;               
    DWORD ulManifestVersionMinor;               
    DWORD ulPolicyVersionMajor;                 
    DWORD ulPolicyVersionMinor;                 
    DWORD ulAssemblyDirectoryNameLength;        

    PCWSTR lpAssemblyEncodedAssemblyIdentity;
    PCWSTR lpAssemblyManifestPath;
    PCWSTR lpAssemblyPolicyPath;
    PCWSTR lpAssemblyDirectoryName;

    DWORD  ulFileCount;
} ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION, * PACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION;

typedef const struct _ACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION * PCACTIVATION_CONTEXT_ASSEMBLY_DETAILED_INFORMATION ;

typedef struct _ACTIVATION_CONTEXT_DETAILED_INFORMATION {
    DWORD dwFlags;
    DWORD ulFormatVersion;
    DWORD ulAssemblyCount;
    DWORD ulRootManifestPathType;
    DWORD ulRootManifestPathChars;
    DWORD ulRootConfigurationPathType;
    DWORD ulRootConfigurationPathChars;
    DWORD ulAppDirPathType;
    DWORD ulAppDirPathChars;
    PCWSTR lpRootManifestPath;
    PCWSTR lpRootConfigurationPath;
    PCWSTR lpAppDirPath;
} ACTIVATION_CONTEXT_DETAILED_INFORMATION, *PACTIVATION_CONTEXT_DETAILED_INFORMATION;

typedef const struct _ACTIVATION_CONTEXT_DETAILED_INFORMATION *PCACTIVATION_CONTEXT_DETAILED_INFORMATION;













































typedef struct _EVENTLOGRECORD {
    DWORD  Length;        
    DWORD  Reserved;      
    DWORD  RecordNumber;  
    DWORD  TimeGenerated; 
    DWORD  TimeWritten;   
    DWORD  EventID;
    WORD   EventType;
    WORD   NumStrings;
    WORD   EventCategory;
    WORD   ReservedFlags; 
    DWORD  ClosingRecordNumber; 
    DWORD  StringOffset;  
    DWORD  UserSidLength;
    DWORD  UserSidOffset;
    DWORD  DataLength;
    DWORD  DataOffset;    
    
    
    
    
    
    
    
    
    
    
    
} EVENTLOGRECORD, *PEVENTLOGRECORD;






#pragma warning(push)
#line 8641 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"
#pragma warning(disable : 4200)
typedef struct _EVENTSFORLOGFILE{
	DWORD			ulSize;
    WCHAR   		szLogicalLogFile[256];        
    DWORD			ulNumRecords;
	EVENTLOGRECORD 	pEventLogRecords[];
}EVENTSFORLOGFILE, *PEVENTSFORLOGFILE;

typedef struct _PACKEDEVENTINFO{
    DWORD               ulSize;  
    DWORD               ulNumEventsForLogFile; 
    DWORD 				ulOffsets[];           
}PACKEDEVENTINFO, *PPACKEDEVENTINFO;


#pragma warning(pop)


#line 8660 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"





















































                                                    


                                                    


                                                    


                                                    
                                                    


























































                                            






























































typedef enum _CM_SERVICE_NODE_TYPE {
    DriverType               = 0x00000001,
    FileSystemType           = 0x00000002,
    Win32ServiceOwnProcess   = 0x00000010,
    Win32ServiceShareProcess = 0x00000020,
    AdapterType              = 0x00000004,
    RecognizerType           = 0x00000008
} SERVICE_NODE_TYPE;

typedef enum _CM_SERVICE_LOAD_TYPE {
    BootLoad    = 0x00000000,
    SystemLoad  = 0x00000001,
    AutoLoad    = 0x00000002,
    DemandLoad  = 0x00000003,
    DisableLoad = 0x00000004
} SERVICE_LOAD_TYPE;

typedef enum _CM_ERROR_CONTROL_TYPE {
    IgnoreError   = 0x00000000,
    NormalError   = 0x00000001,
    SevereError   = 0x00000002,
    CriticalError = 0x00000003
} SERVICE_ERROR_TYPE;










typedef struct _TAPE_ERASE {
    DWORD Type;
    BOOLEAN Immediate;
} TAPE_ERASE, *PTAPE_ERASE;












typedef struct _TAPE_PREPARE {
    DWORD Operation;
    BOOLEAN Immediate;
} TAPE_PREPARE, *PTAPE_PREPARE;










typedef struct _TAPE_WRITE_MARKS {
    DWORD Type;
    DWORD Count;
    BOOLEAN Immediate;
} TAPE_WRITE_MARKS, *PTAPE_WRITE_MARKS;









typedef struct _TAPE_GET_POSITION {
    DWORD Type;
    DWORD Partition;
    LARGE_INTEGER Offset;
} TAPE_GET_POSITION, *PTAPE_GET_POSITION;
















typedef struct _TAPE_SET_POSITION {
    DWORD Method;
    DWORD Partition;
    LARGE_INTEGER Offset;
    BOOLEAN Immediate;
} TAPE_SET_POSITION, *PTAPE_SET_POSITION;























































































typedef struct _TAPE_GET_DRIVE_PARAMETERS {
    BOOLEAN ECC;
    BOOLEAN Compression;
    BOOLEAN DataPadding;
    BOOLEAN ReportSetmarks;
    DWORD DefaultBlockSize;
    DWORD MaximumBlockSize;
    DWORD MinimumBlockSize;
    DWORD MaximumPartitionCount;
    DWORD FeaturesLow;
    DWORD FeaturesHigh;
    DWORD EOTWarningZoneSize;
} TAPE_GET_DRIVE_PARAMETERS, *PTAPE_GET_DRIVE_PARAMETERS;





typedef struct _TAPE_SET_DRIVE_PARAMETERS {
    BOOLEAN ECC;
    BOOLEAN Compression;
    BOOLEAN DataPadding;
    BOOLEAN ReportSetmarks;
    DWORD EOTWarningZoneSize;
} TAPE_SET_DRIVE_PARAMETERS, *PTAPE_SET_DRIVE_PARAMETERS;





typedef struct _TAPE_GET_MEDIA_PARAMETERS {
    LARGE_INTEGER Capacity;
    LARGE_INTEGER Remaining;
    DWORD BlockSize;
    DWORD PartitionCount;
    BOOLEAN WriteProtected;
} TAPE_GET_MEDIA_PARAMETERS, *PTAPE_GET_MEDIA_PARAMETERS;





typedef struct _TAPE_SET_MEDIA_PARAMETERS {
    DWORD BlockSize;
} TAPE_SET_MEDIA_PARAMETERS, *PTAPE_SET_MEDIA_PARAMETERS;









typedef struct _TAPE_CREATE_PARTITION {
    DWORD Method;
    DWORD Count;
    DWORD Size;
} TAPE_CREATE_PARTITION, *PTAPE_CREATE_PARTITION;











typedef struct _TAPE_WMI_OPERATIONS {
   DWORD Method;
   DWORD DataBufferSize;
   PVOID DataBuffer;
} TAPE_WMI_OPERATIONS, *PTAPE_WMI_OPERATIONS;




typedef enum _TAPE_DRIVE_PROBLEM_TYPE {
   TapeDriveProblemNone, TapeDriveReadWriteWarning,
   TapeDriveReadWriteError, TapeDriveReadWarning,
   TapeDriveWriteWarning, TapeDriveReadError,
   TapeDriveWriteError, TapeDriveHardwareError,
   TapeDriveUnsupportedMedia, TapeDriveScsiConnectionError,
   TapeDriveTimetoClean, TapeDriveCleanDriveNow,
   TapeDriveMediaLifeExpired, TapeDriveSnappedTape
} TAPE_DRIVE_PROBLEM_TYPE;



























#line 9152 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"











#line 9164 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"


}
#line 9168 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 9170 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnt.h"

#line 167 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
#line 168 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"


typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;





#line 179 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"



#line 183 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

#line 185 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"










struct HWND__ { int unused; }; typedef struct HWND__ *HWND;
struct HHOOK__ { int unused; }; typedef struct HHOOK__ *HHOOK;



#line 201 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

typedef WORD                ATOM;

typedef HANDLE          *SPHANDLE;
typedef HANDLE           *LPHANDLE;
typedef HANDLE              HGLOBAL;
typedef HANDLE              HLOCAL;
typedef HANDLE              GLOBALHANDLE;
typedef HANDLE              LOCALHANDLE;






typedef int ( __stdcall *FARPROC)();
typedef int ( __stdcall *NEARPROC)();
typedef int (__stdcall *PROC)();
#line 220 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"




#line 225 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"



typedef void * HGDIOBJ;


#line 232 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
#line 233 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

struct HKEY__ { int unused; }; typedef struct HKEY__ *HKEY;
typedef HKEY *PHKEY;


struct HACCEL__ { int unused; }; typedef struct HACCEL__ *HACCEL;
#line 240 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

struct HBITMAP__ { int unused; }; typedef struct HBITMAP__ *HBITMAP;
struct HBRUSH__ { int unused; }; typedef struct HBRUSH__ *HBRUSH;
#line 244 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

struct HCOLORSPACE__ { int unused; }; typedef struct HCOLORSPACE__ *HCOLORSPACE;
#line 247 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

struct HDC__ { int unused; }; typedef struct HDC__ *HDC;
#line 250 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
struct HGLRC__ { int unused; }; typedef struct HGLRC__ *HGLRC;          
struct HDESK__ { int unused; }; typedef struct HDESK__ *HDESK;
struct HENHMETAFILE__ { int unused; }; typedef struct HENHMETAFILE__ *HENHMETAFILE;

struct HFONT__ { int unused; }; typedef struct HFONT__ *HFONT;
#line 256 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
struct HICON__ { int unused; }; typedef struct HICON__ *HICON;

struct HMENU__ { int unused; }; typedef struct HMENU__ *HMENU;
#line 260 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
struct HMETAFILE__ { int unused; }; typedef struct HMETAFILE__ *HMETAFILE;
struct HINSTANCE__ { int unused; }; typedef struct HINSTANCE__ *HINSTANCE;
typedef HINSTANCE HMODULE;      

struct HPALETTE__ { int unused; }; typedef struct HPALETTE__ *HPALETTE;
struct HPEN__ { int unused; }; typedef struct HPEN__ *HPEN;
#line 267 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
struct HRGN__ { int unused; }; typedef struct HRGN__ *HRGN;
struct HRSRC__ { int unused; }; typedef struct HRSRC__ *HRSRC;
struct HSTR__ { int unused; }; typedef struct HSTR__ *HSTR;
struct HTASK__ { int unused; }; typedef struct HTASK__ *HTASK;
struct HWINSTA__ { int unused; }; typedef struct HWINSTA__ *HWINSTA;
struct HKL__ { int unused; }; typedef struct HKL__ *HKL;



struct HMONITOR__ { int unused; }; typedef struct HMONITOR__ *HMONITOR;
struct HWINEVENTHOOK__ { int unused; }; typedef struct HWINEVENTHOOK__ *HWINEVENTHOOK;
#line 279 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
struct HUMPD__ { int unused; }; typedef struct HUMPD__ *HUMPD;
#line 281 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"


typedef int HFILE;
typedef HICON HCURSOR;      



#line 289 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

typedef DWORD   COLORREF;
typedef DWORD   *LPCOLORREF;



typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT,  *NPRECT,  *LPRECT;

typedef const RECT * LPCRECT;

typedef struct _RECTL       
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECTL, *PRECTL, *LPRECTL;

typedef const RECTL * LPCRECTL;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT,  *NPPOINT,  *LPPOINT;

typedef struct _POINTL      
{
    LONG  x;
    LONG  y;
} POINTL, *PPOINTL;

typedef struct tagSIZE
{
    LONG        cx;
    LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef SIZE               SIZEL;
typedef SIZE               *PSIZEL, *LPSIZEL;

typedef struct tagPOINTS
{

    SHORT   x;
    SHORT   y;



#line 345 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"
} POINTS, *PPOINTS, *LPPOINTS;





typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;



































}
#line 392 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

#line 394 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windef.h"

#line 162 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"












#pragma once
#line 15 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"













#line 29 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





#line 35 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





#line 41 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


extern "C" {
#line 45 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"














































































































#line 156 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"









#line 166 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


























































typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        };

        PVOID Pointer;
    };

    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;



















typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;


typedef DWORD (__stdcall *PTHREAD_START_ROUTINE)(
    LPVOID lpThreadParameter
    );
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;






#line 293 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

typedef RTL_CRITICAL_SECTION_DEBUG CRITICAL_SECTION_DEBUG;
typedef PRTL_CRITICAL_SECTION_DEBUG PCRITICAL_SECTION_DEBUG;
typedef PRTL_CRITICAL_SECTION_DEBUG LPCRITICAL_SECTION_DEBUG;


typedef PLDT_ENTRY LPLDT_ENTRY;


#line 307 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"









































































































typedef struct _COMMPROP {
    WORD wPacketLength;
    WORD wPacketVersion;
    DWORD dwServiceMask;
    DWORD dwReserved1;
    DWORD dwMaxTxQueue;
    DWORD dwMaxRxQueue;
    DWORD dwMaxBaud;
    DWORD dwProvSubType;
    DWORD dwProvCapabilities;
    DWORD dwSettableParams;
    DWORD dwSettableBaud;
    WORD wSettableData;
    WORD wSettableStopParity;
    DWORD dwCurrentTxQueue;
    DWORD dwCurrentRxQueue;
    DWORD dwProvSpec1;
    DWORD dwProvSpec2;
    WCHAR wcProvChar[1];
} COMMPROP,*LPCOMMPROP;







typedef struct _COMSTAT {
    DWORD fCtsHold : 1;
    DWORD fDsrHold : 1;
    DWORD fRlsdHold : 1;
    DWORD fXoffHold : 1;
    DWORD fXoffSent : 1;
    DWORD fEof : 1;
    DWORD fTxim : 1;
    DWORD fReserved : 25;
    DWORD cbInQue;
    DWORD cbOutQue;
} COMSTAT, *LPCOMSTAT;
















typedef struct _DCB {
    DWORD DCBlength;      
    DWORD BaudRate;       
    DWORD fBinary: 1;     
    DWORD fParity: 1;     
    DWORD fOutxCtsFlow:1; 
    DWORD fOutxDsrFlow:1; 
    DWORD fDtrControl:2;  
    DWORD fDsrSensitivity:1; 
    DWORD fTXContinueOnXoff: 1; 
    DWORD fOutX: 1;       
    DWORD fInX: 1;        
    DWORD fErrorChar: 1;  
    DWORD fNull: 1;       
    DWORD fRtsControl:2;  
    DWORD fAbortOnError:1; 
    DWORD fDummy2:17;     
    WORD wReserved;       
    WORD XonLim;          
    WORD XoffLim;         
    BYTE ByteSize;        
    BYTE Parity;          
    BYTE StopBits;        
    char XonChar;         
    char XoffChar;        
    char ErrorChar;       
    char EofChar;         
    char EvtChar;         
    WORD wReserved1;      
} DCB, *LPDCB;

typedef struct _COMMTIMEOUTS {
    DWORD ReadIntervalTimeout;          
    DWORD ReadTotalTimeoutMultiplier;   
    DWORD ReadTotalTimeoutConstant;     
    DWORD WriteTotalTimeoutMultiplier;  
    DWORD WriteTotalTimeoutConstant;    
} COMMTIMEOUTS,*LPCOMMTIMEOUTS;

typedef struct _COMMCONFIG {
    DWORD dwSize;               
    WORD wVersion;              
    WORD wReserved;             
    DCB dcb;                    
    DWORD dwProviderSubType;    

    DWORD dwProviderOffset;     

    DWORD dwProviderSize;       
    WCHAR wcProviderData[1];    
} COMMCONFIG,*LPCOMMCONFIG;

typedef struct _SYSTEM_INFO {
    union {
        DWORD dwOemId;          
        struct {
            WORD wProcessorArchitecture;
            WORD wReserved;
        };
    };
    DWORD dwPageSize;
    LPVOID lpMinimumApplicationAddress;
    LPVOID lpMaximumApplicationAddress;
    DWORD_PTR dwActiveProcessorMask;
    DWORD dwNumberOfProcessors;
    DWORD dwProcessorType;
    DWORD dwAllocationGranularity;
    WORD wProcessorLevel;
    WORD wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;




































typedef struct _MEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    SIZE_T dwTotalPhys;
    SIZE_T dwAvailPhys;
    SIZE_T dwTotalPageFile;
    SIZE_T dwAvailPageFile;
    SIZE_T dwTotalVirtual;
    SIZE_T dwAvailVirtual;
} MEMORYSTATUS, *LPMEMORYSTATUS;
























































































typedef struct _EXCEPTION_DEBUG_INFO {
    EXCEPTION_RECORD ExceptionRecord;
    DWORD dwFirstChance;
} EXCEPTION_DEBUG_INFO, *LPEXCEPTION_DEBUG_INFO;

typedef struct _CREATE_THREAD_DEBUG_INFO {
    HANDLE hThread;
    LPVOID lpThreadLocalBase;
    LPTHREAD_START_ROUTINE lpStartAddress;
} CREATE_THREAD_DEBUG_INFO, *LPCREATE_THREAD_DEBUG_INFO;

typedef struct _CREATE_PROCESS_DEBUG_INFO {
    HANDLE hFile;
    HANDLE hProcess;
    HANDLE hThread;
    LPVOID lpBaseOfImage;
    DWORD dwDebugInfoFileOffset;
    DWORD nDebugInfoSize;
    LPVOID lpThreadLocalBase;
    LPTHREAD_START_ROUTINE lpStartAddress;
    LPVOID lpImageName;
    WORD fUnicode;
} CREATE_PROCESS_DEBUG_INFO, *LPCREATE_PROCESS_DEBUG_INFO;

typedef struct _EXIT_THREAD_DEBUG_INFO {
    DWORD dwExitCode;
} EXIT_THREAD_DEBUG_INFO, *LPEXIT_THREAD_DEBUG_INFO;

typedef struct _EXIT_PROCESS_DEBUG_INFO {
    DWORD dwExitCode;
} EXIT_PROCESS_DEBUG_INFO, *LPEXIT_PROCESS_DEBUG_INFO;

typedef struct _LOAD_DLL_DEBUG_INFO {
    HANDLE hFile;
    LPVOID lpBaseOfDll;
    DWORD dwDebugInfoFileOffset;
    DWORD nDebugInfoSize;
    LPVOID lpImageName;
    WORD fUnicode;
} LOAD_DLL_DEBUG_INFO, *LPLOAD_DLL_DEBUG_INFO;

typedef struct _UNLOAD_DLL_DEBUG_INFO {
    LPVOID lpBaseOfDll;
} UNLOAD_DLL_DEBUG_INFO, *LPUNLOAD_DLL_DEBUG_INFO;

typedef struct _OUTPUT_DEBUG_STRING_INFO {
    LPSTR lpDebugStringData;
    WORD fUnicode;
    WORD nDebugStringLength;
} OUTPUT_DEBUG_STRING_INFO, *LPOUTPUT_DEBUG_STRING_INFO;

typedef struct _RIP_INFO {
    DWORD dwError;
    DWORD dwType;
} RIP_INFO, *LPRIP_INFO;


typedef struct _DEBUG_EVENT {
    DWORD dwDebugEventCode;
    DWORD dwProcessId;
    DWORD dwThreadId;
    union {
        EXCEPTION_DEBUG_INFO Exception;
        CREATE_THREAD_DEBUG_INFO CreateThread;
        CREATE_PROCESS_DEBUG_INFO CreateProcessInfo;
        EXIT_THREAD_DEBUG_INFO ExitThread;
        EXIT_PROCESS_DEBUG_INFO ExitProcess;
        LOAD_DLL_DEBUG_INFO LoadDll;
        UNLOAD_DLL_DEBUG_INFO UnloadDll;
        OUTPUT_DEBUG_STRING_INFO DebugString;
        RIP_INFO RipInfo;
    } u;
} DEBUG_EVENT, *LPDEBUG_EVENT;


typedef PCONTEXT LPCONTEXT;
typedef PEXCEPTION_RECORD LPEXCEPTION_RECORD;
typedef PEXCEPTION_POINTERS LPEXCEPTION_POINTERS;
#line 750 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"














#line 765 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"















































































































































































































typedef struct _OFSTRUCT {
    BYTE cBytes;
    BYTE fFixedDisk;
    WORD nErrCode;
    WORD Reserved1;
    WORD Reserved2;
    CHAR szPathName[128];
} OFSTRUCT, *LPOFSTRUCT, *POFSTRUCT;




































































































































































































#line 1177 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"




















































































































#line 1294 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
LONG
__stdcall
InterlockedIncrement(
      LONG volatile *lpAddend
    );

__declspec(dllimport)
LONG
__stdcall
InterlockedDecrement(
      LONG volatile *lpAddend
    );

__declspec(dllimport)
LONG
__stdcall
InterlockedExchange(
      LONG volatile *Target,
     LONG Value
    );




__declspec(dllimport)
LONG
__stdcall
InterlockedExchangeAdd(
      LONG volatile *Addend,
     LONG Value
    );

__declspec(dllimport)
LONG
__stdcall
InterlockedCompareExchange (
      LONG volatile *Destination,
     LONG Exchange,
     LONG Comperand
    );


__declspec(dllimport)
LONGLONG
__stdcall
InterlockedCompareExchange64 (
      LONGLONG volatile *Destination,
     LONGLONG Exchange,
     LONGLONG Comperand
    );



LONGLONG
__forceinline
InterlockedAnd64 (
      LONGLONG volatile *Destination,
     LONGLONG Value
    )
{
    LONGLONG Old;

    do {
        Old = *Destination;
    } while (InterlockedCompareExchange64(Destination,
                                          Old & Value,
                                          Old) != Old);

    return Old;
}

LONGLONG
__forceinline
InterlockedOr64 (
      LONGLONG volatile *Destination,
     LONGLONG Value
    )
{
    LONGLONG Old;

    do {
        Old = *Destination;
    } while (InterlockedCompareExchange64(Destination,
                                          Old | Value,
                                          Old) != Old);

    return Old;
}

LONGLONG
__forceinline
InterlockedXor64 (
      LONGLONG volatile *Destination,
     LONGLONG Value
    )
{
    LONGLONG Old;

    do {
        Old = *Destination;
    } while (InterlockedCompareExchange64(Destination,
                                          Old ^ Value,
                                          Old) != Old);

    return Old;
}

LONGLONG
__forceinline
InterlockedIncrement64(
      LONGLONG volatile *Addend
    )
{
    LONGLONG Old;

    do {
        Old = *Addend;
    } while (InterlockedCompareExchange64(Addend,
                                          Old + 1,
                                          Old) != Old);

    return Old + 1;
}

LONGLONG
__forceinline
InterlockedDecrement64(
      LONGLONG volatile *Addend
    )
{
    LONGLONG Old;

    do {
        Old = *Addend;
    } while (InterlockedCompareExchange64(Addend,
                                          Old - 1,
                                          Old) != Old);

    return Old - 1;
}

LONGLONG
__forceinline
InterlockedExchange64(
      LONGLONG volatile *Target,
     LONGLONG Value
    )
{
    LONGLONG Old;

    do {
        Old = *Target;
    } while (InterlockedCompareExchange64(Target,
                                          Value,
                                          Old) != Old);

    return Old;
}

LONGLONG
__forceinline
InterlockedExchangeAdd64(
      LONGLONG volatile *Addend,
     LONGLONG Value
    )
{
    LONGLONG Old;

    do {
        Old = *Addend;
    } while (InterlockedCompareExchange64(Addend,
                                          Old + Value,
                                          Old) != Old);

    return Old;
}

#line 1474 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"







__forceinline
PVOID
__cdecl
__InlineInterlockedCompareExchangePointer (
      PVOID volatile *Destination,
     PVOID ExChange,
     PVOID Comperand
    )
{
    return((PVOID)(LONG_PTR)InterlockedCompareExchange((LONG volatile *)Destination, (LONG)(LONG_PTR)ExChange, (LONG)(LONG_PTR)Comperand));
}








#line 1501 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

#line 1503 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



__declspec(dllimport)
void
__stdcall
InitializeSListHead (
     PSLIST_HEADER ListHead
    );

__declspec(dllimport)
PSINGLE_LIST_ENTRY
__stdcall
InterlockedPopEntrySList (
     PSLIST_HEADER ListHead
    );

__declspec(dllimport)
PSINGLE_LIST_ENTRY
__stdcall
InterlockedPushEntrySList (
     PSLIST_HEADER ListHead,
     PSINGLE_LIST_ENTRY ListEntry
    );

__declspec(dllimport)
PSINGLE_LIST_ENTRY
__stdcall
InterlockedFlushSList (
     PSLIST_HEADER ListHead
    );

__declspec(dllimport)
USHORT
__stdcall
QueryDepthSList (
     PSLIST_HEADER ListHead
    );

#line 1543 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

#line 1545 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

#line 1547 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
FreeResource(
         HGLOBAL hResData
        );

__declspec(dllimport)
LPVOID
__stdcall
LockResource(
         HGLOBAL hResData
        );







int
__stdcall



#line 1574 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"
WinMain(
     HINSTANCE hInstance,
     HINSTANCE hPrevInstance,
     LPSTR lpCmdLine,
     int nShowCmd
    );

__declspec(dllimport)
BOOL
__stdcall
FreeLibrary(
      HMODULE hLibModule
    );


__declspec(dllimport)
__declspec(noreturn)
void
__stdcall
FreeLibraryAndExitThread(
     HMODULE hLibModule,
     DWORD dwExitCode
    );

__declspec(dllimport)
BOOL
__stdcall
DisableThreadLibraryCalls(
     HMODULE hLibModule
    );

__declspec(dllimport)
FARPROC
__stdcall
GetProcAddress(
     HMODULE hModule,
     LPCSTR lpProcName
    );

__declspec(dllimport)
DWORD
__stdcall
GetVersion( void );

__declspec(dllimport)
HGLOBAL
__stdcall
GlobalAlloc(
     UINT uFlags,
     SIZE_T dwBytes
    );

__declspec(dllimport)
HGLOBAL
__stdcall
GlobalReAlloc(
     HGLOBAL hMem,
     SIZE_T dwBytes,
     UINT uFlags
    );

__declspec(dllimport)
SIZE_T
__stdcall
GlobalSize(
     HGLOBAL hMem
    );

__declspec(dllimport)
UINT
__stdcall
GlobalFlags(
     HGLOBAL hMem
    );


__declspec(dllimport)
LPVOID
__stdcall
GlobalLock(
     HGLOBAL hMem
    );


__declspec(dllimport)
HGLOBAL
__stdcall
GlobalHandle(
     LPCVOID pMem
    );


__declspec(dllimport)
BOOL
__stdcall
GlobalUnlock(
     HGLOBAL hMem
    );


__declspec(dllimport)
HGLOBAL
__stdcall
GlobalFree(
     HGLOBAL hMem
    );

__declspec(dllimport)
SIZE_T
__stdcall
GlobalCompact(
     DWORD dwMinFree
    );

__declspec(dllimport)
void
__stdcall
GlobalFix(
     HGLOBAL hMem
    );

__declspec(dllimport)
void
__stdcall
GlobalUnfix(
     HGLOBAL hMem
    );

__declspec(dllimport)
LPVOID
__stdcall
GlobalWire(
     HGLOBAL hMem
    );

__declspec(dllimport)
BOOL
__stdcall
GlobalUnWire(
     HGLOBAL hMem
    );

__declspec(dllimport)
void
__stdcall
GlobalMemoryStatus(
      LPMEMORYSTATUS lpBuffer
    );

typedef struct _MEMORYSTATUSEX {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORDLONG ullTotalPhys;
    DWORDLONG ullAvailPhys;
    DWORDLONG ullTotalPageFile;
    DWORDLONG ullAvailPageFile;
    DWORDLONG ullTotalVirtual;
    DWORDLONG ullAvailVirtual;
    DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;

__declspec(dllimport)
BOOL
__stdcall
GlobalMemoryStatusEx(
      LPMEMORYSTATUSEX lpBuffer
    );

__declspec(dllimport)
HLOCAL
__stdcall
LocalAlloc(
     UINT uFlags,
     SIZE_T uBytes
    );

__declspec(dllimport)
HLOCAL
__stdcall
LocalReAlloc(
     HLOCAL hMem,
     SIZE_T uBytes,
     UINT uFlags
    );

__declspec(dllimport)
LPVOID
__stdcall
LocalLock(
     HLOCAL hMem
    );

__declspec(dllimport)
HLOCAL
__stdcall
LocalHandle(
     LPCVOID pMem
    );

__declspec(dllimport)
BOOL
__stdcall
LocalUnlock(
     HLOCAL hMem
    );

__declspec(dllimport)
SIZE_T
__stdcall
LocalSize(
     HLOCAL hMem
    );

__declspec(dllimport)
UINT
__stdcall
LocalFlags(
     HLOCAL hMem
    );

__declspec(dllimport)
HLOCAL
__stdcall
LocalFree(
     HLOCAL hMem
    );

__declspec(dllimport)
SIZE_T
__stdcall
LocalShrink(
     HLOCAL hMem,
     UINT cbNewSize
    );

__declspec(dllimport)
SIZE_T
__stdcall
LocalCompact(
     UINT uMinFree
    );

__declspec(dllimport)
BOOL
__stdcall
FlushInstructionCache(
     HANDLE hProcess,
     LPCVOID lpBaseAddress,
     SIZE_T dwSize
    );

__declspec(dllimport)
LPVOID
__stdcall
VirtualAlloc(
     LPVOID lpAddress,
     SIZE_T dwSize,
     DWORD flAllocationType,
     DWORD flProtect
    );

__declspec(dllimport)
BOOL
__stdcall
VirtualFree(
     LPVOID lpAddress,
     SIZE_T dwSize,
     DWORD dwFreeType
    );

__declspec(dllimport)
BOOL
__stdcall
VirtualProtect(
      LPVOID lpAddress,
      SIZE_T dwSize,
      DWORD flNewProtect,
     PDWORD lpflOldProtect
    );

__declspec(dllimport)
SIZE_T
__stdcall
VirtualQuery(
     LPCVOID lpAddress,
     PMEMORY_BASIC_INFORMATION lpBuffer,
     SIZE_T dwLength
    );

__declspec(dllimport)
LPVOID
__stdcall
VirtualAllocEx(
     HANDLE hProcess,
     LPVOID lpAddress,
     SIZE_T dwSize,
     DWORD flAllocationType,
     DWORD flProtect
    );

__declspec(dllimport)
UINT
__stdcall
GetWriteWatch(
     DWORD  dwFlags,
     PVOID  lpBaseAddress,
     SIZE_T dwRegionSize,
      PVOID *lpAddresses,
      PULONG_PTR lpdwCount,
     PULONG lpdwGranularity
    );

__declspec(dllimport)
UINT
__stdcall
ResetWriteWatch(
     LPVOID lpBaseAddress,
     SIZE_T dwRegionSize
    );

__declspec(dllimport)
SIZE_T
__stdcall
GetLargePageMinimum(
    void
    );

__declspec(dllimport)
BOOL
__stdcall
VirtualFreeEx(
     HANDLE hProcess,
     LPVOID lpAddress,
     SIZE_T dwSize,
     DWORD dwFreeType
    );

__declspec(dllimport)
BOOL
__stdcall
VirtualProtectEx(
      HANDLE hProcess,
      LPVOID lpAddress,
      SIZE_T dwSize,
      DWORD flNewProtect,
     PDWORD lpflOldProtect
    );

__declspec(dllimport)
SIZE_T
__stdcall
VirtualQueryEx(
     HANDLE hProcess,
     LPCVOID lpAddress,
     PMEMORY_BASIC_INFORMATION lpBuffer,
     SIZE_T dwLength
    );

__declspec(dllimport)
HANDLE
__stdcall
HeapCreate(
     DWORD flOptions,
     SIZE_T dwInitialSize,
     SIZE_T dwMaximumSize
    );

__declspec(dllimport)
BOOL
__stdcall
HeapDestroy(
      HANDLE hHeap
    );


__declspec(dllimport)
LPVOID
__stdcall
HeapAlloc(
     HANDLE hHeap,
     DWORD dwFlags,
     SIZE_T dwBytes
    );

__declspec(dllimport)
LPVOID
__stdcall
HeapReAlloc(
     HANDLE hHeap,
     DWORD dwFlags,
     LPVOID lpMem,
     SIZE_T dwBytes
    );

__declspec(dllimport)
BOOL
__stdcall
HeapFree(
     HANDLE hHeap,
     DWORD dwFlags,
     LPVOID lpMem
    );

__declspec(dllimport)
SIZE_T
__stdcall
HeapSize(
     HANDLE hHeap,
     DWORD dwFlags,
     LPCVOID lpMem
    );

__declspec(dllimport)
BOOL
__stdcall
HeapValidate(
     HANDLE hHeap,
     DWORD dwFlags,
     LPCVOID lpMem
    );

__declspec(dllimport)
SIZE_T
__stdcall
HeapCompact(
     HANDLE hHeap,
     DWORD dwFlags
    );

__declspec(dllimport)
HANDLE
__stdcall
GetProcessHeap( void );

__declspec(dllimport)
DWORD
__stdcall
GetProcessHeaps(
     DWORD NumberOfHeaps,
     PHANDLE ProcessHeaps
    );

typedef struct _PROCESS_HEAP_ENTRY {
    PVOID lpData;
    DWORD cbData;
    BYTE cbOverhead;
    BYTE iRegionIndex;
    WORD wFlags;
    union {
        struct {
            HANDLE hMem;
            DWORD dwReserved[ 3 ];
        } Block;
        struct {
            DWORD dwCommittedSize;
            DWORD dwUnCommittedSize;
            LPVOID lpFirstBlock;
            LPVOID lpLastBlock;
        } Region;
    };
} PROCESS_HEAP_ENTRY, *LPPROCESS_HEAP_ENTRY, *PPROCESS_HEAP_ENTRY;







__declspec(dllimport)
BOOL
__stdcall
HeapLock(
     HANDLE hHeap
    );

__declspec(dllimport)
BOOL
__stdcall
HeapUnlock(
     HANDLE hHeap
    );


__declspec(dllimport)
BOOL
__stdcall
HeapWalk(
     HANDLE hHeap,
      LPPROCESS_HEAP_ENTRY lpEntry
    );


__declspec(dllimport)
BOOL
__stdcall
HeapSetInformation (
     HANDLE HeapHandle, 
     HEAP_INFORMATION_CLASS HeapInformationClass,
     PVOID HeapInformation ,
     SIZE_T HeapInformationLength 
    );

__declspec(dllimport)
BOOL
__stdcall
HeapQueryInformation (
     HANDLE HeapHandle, 
     HEAP_INFORMATION_CLASS HeapInformationClass,
     PVOID HeapInformation ,
     SIZE_T HeapInformationLength ,
     PSIZE_T ReturnLength 
    );













#line 2100 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

#line 2102 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetBinaryTypeA(
     LPCSTR lpApplicationName,
     LPDWORD lpBinaryType
    );
__declspec(dllimport)
BOOL
__stdcall
GetBinaryTypeW(
     LPCWSTR lpApplicationName,
     LPDWORD lpBinaryType
    );




#line 2122 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetShortPathNameA(
     LPCSTR lpszLongPath,
     LPSTR  lpszShortPath,
     DWORD    cchBuffer
    );
__declspec(dllimport)
DWORD
__stdcall
GetShortPathNameW(
     LPCWSTR lpszLongPath,
     LPWSTR  lpszShortPath,
     DWORD    cchBuffer
    );




#line 2144 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetLongPathNameA(
     LPCSTR lpszShortPath,
     LPSTR  lpszLongPath,
     DWORD    cchBuffer
    );
__declspec(dllimport)
DWORD
__stdcall
GetLongPathNameW(
     LPCWSTR lpszShortPath,
     LPWSTR  lpszLongPath,
     DWORD    cchBuffer
    );




#line 2166 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetProcessAffinityMask(
     HANDLE hProcess,
     PDWORD_PTR lpProcessAffinityMask,
     PDWORD_PTR lpSystemAffinityMask
    );

__declspec(dllimport)
BOOL
__stdcall
SetProcessAffinityMask(
     HANDLE hProcess,
     DWORD_PTR dwProcessAffinityMask
    );











#line 2195 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetProcessTimes(
     HANDLE hProcess,
     LPFILETIME lpCreationTime,
     LPFILETIME lpExitTime,
     LPFILETIME lpKernelTime,
     LPFILETIME lpUserTime
    );

__declspec(dllimport)
BOOL
__stdcall
GetProcessIoCounters(
     HANDLE hProcess,
     PIO_COUNTERS lpIoCounters
    );

__declspec(dllimport)
BOOL
__stdcall
GetProcessWorkingSetSize(
     HANDLE hProcess,
     PSIZE_T lpMinimumWorkingSetSize,
     PSIZE_T lpMaximumWorkingSetSize
    );

__declspec(dllimport)
BOOL
__stdcall
GetProcessWorkingSetSizeEx(
     HANDLE hProcess,
     PSIZE_T lpMinimumWorkingSetSize,
     PSIZE_T lpMaximumWorkingSetSize,
     PDWORD Flags
    );

__declspec(dllimport)
BOOL
__stdcall
SetProcessWorkingSetSize(
     HANDLE hProcess,
     SIZE_T dwMinimumWorkingSetSize,
     SIZE_T dwMaximumWorkingSetSize
    );

__declspec(dllimport)
BOOL
__stdcall
SetProcessWorkingSetSizeEx(
     HANDLE hProcess,
     SIZE_T dwMinimumWorkingSetSize,
     SIZE_T dwMaximumWorkingSetSize,
     DWORD Flags
    );


__declspec(dllimport)
HANDLE
__stdcall
OpenProcess(
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     DWORD dwProcessId
    );

__declspec(dllimport)
HANDLE
__stdcall
GetCurrentProcess(
    void
    );

__declspec(dllimport)
DWORD
__stdcall
GetCurrentProcessId(
    void
    );

__declspec(dllimport)
__declspec(noreturn)
void
__stdcall
ExitProcess(
     UINT uExitCode
    );

__declspec(dllimport)
BOOL
__stdcall
TerminateProcess(
     HANDLE hProcess,
     UINT uExitCode
    );

__declspec(dllimport)
BOOL
__stdcall
GetExitCodeProcess(
     HANDLE hProcess,
     LPDWORD lpExitCode
    );


__declspec(dllimport)
void
__stdcall
FatalExit(
     int ExitCode
    );

__declspec(dllimport)
LPSTR
__stdcall
GetEnvironmentStrings(
    void
    );

__declspec(dllimport)
LPWSTR
__stdcall
GetEnvironmentStringsW(
    void
    );





#line 2328 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetEnvironmentStringsA(
     LPSTR NewEnvironment
    );
__declspec(dllimport)
BOOL
__stdcall
SetEnvironmentStringsW(
     LPWSTR NewEnvironment
    );




#line 2346 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
FreeEnvironmentStringsA(
     LPSTR
    );
__declspec(dllimport)
BOOL
__stdcall
FreeEnvironmentStringsW(
     LPWSTR
    );




#line 2364 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
void
__stdcall
RaiseException(
     DWORD dwExceptionCode,
     DWORD dwExceptionFlags,
     DWORD nNumberOfArguments,
     const ULONG_PTR *lpArguments
    );

__declspec(dllimport)
LONG
__stdcall
UnhandledExceptionFilter(
     struct _EXCEPTION_POINTERS *ExceptionInfo
    );

typedef LONG (__stdcall *PTOP_LEVEL_EXCEPTION_FILTER)(
    struct _EXCEPTION_POINTERS *ExceptionInfo
    );
typedef PTOP_LEVEL_EXCEPTION_FILTER LPTOP_LEVEL_EXCEPTION_FILTER;

__declspec(dllimport)
LPTOP_LEVEL_EXCEPTION_FILTER
__stdcall
SetUnhandledExceptionFilter(
     LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter
    );








































































#line 2466 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
CreateThread(
     LPSECURITY_ATTRIBUTES lpThreadAttributes,
     SIZE_T dwStackSize,
     LPTHREAD_START_ROUTINE lpStartAddress,
     LPVOID lpParameter,
     DWORD dwCreationFlags,
     LPDWORD lpThreadId
    );

__declspec(dllimport)
HANDLE
__stdcall
CreateRemoteThread(
     HANDLE hProcess,
     LPSECURITY_ATTRIBUTES lpThreadAttributes,
     SIZE_T dwStackSize,
     LPTHREAD_START_ROUTINE lpStartAddress,
     LPVOID lpParameter,
     DWORD dwCreationFlags,
     LPDWORD lpThreadId
    );

__declspec(dllimport)
HANDLE
__stdcall
GetCurrentThread(
    void
    );

__declspec(dllimport)
DWORD
__stdcall
GetCurrentThreadId(
    void
    );

__declspec(dllimport)
DWORD
__stdcall
GetProcessIdOfThread(
    HANDLE Thread
    );

__declspec(dllimport)
DWORD
__stdcall
GetThreadId(
    HANDLE Thread
    );

__declspec(dllimport)
DWORD
__stdcall
GetProcessId(
    HANDLE Process
    );

__declspec(dllimport)
DWORD
__stdcall
GetCurrentProcessorNumber(
    void
    );

__declspec(dllimport)
DWORD_PTR
__stdcall
SetThreadAffinityMask(
     HANDLE hThread,
     DWORD_PTR dwThreadAffinityMask
    );









#line 2551 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetProcessPriorityBoost(
     HANDLE hProcess,
     BOOL bDisablePriorityBoost
    );

__declspec(dllimport)
BOOL
__stdcall
GetProcessPriorityBoost(
     HANDLE hProcess,
     PBOOL pDisablePriorityBoost
    );

__declspec(dllimport)
BOOL
__stdcall
RequestWakeupLatency(
     LATENCY_TIME latency
    );

__declspec(dllimport)
BOOL
__stdcall
IsSystemResumeAutomatic(
    void
    );

__declspec(dllimport)
HANDLE
__stdcall
OpenThread(
    DWORD dwDesiredAccess,
    BOOL bInheritHandle,
    DWORD dwThreadId
    );

__declspec(dllimport)
BOOL
__stdcall
SetThreadPriority(
     HANDLE hThread,
     int nPriority
    );

__declspec(dllimport)
BOOL
__stdcall
SetThreadPriorityBoost(
     HANDLE hThread,
     BOOL bDisablePriorityBoost
    );

__declspec(dllimport)
BOOL
__stdcall
GetThreadPriorityBoost(
     HANDLE hThread,
     PBOOL pDisablePriorityBoost
    );

__declspec(dllimport)
int
__stdcall
GetThreadPriority(
     HANDLE hThread
    );

__declspec(dllimport)
BOOL
__stdcall
GetThreadTimes(
     HANDLE hThread,
     LPFILETIME lpCreationTime,
     LPFILETIME lpExitTime,
     LPFILETIME lpKernelTime,
     LPFILETIME lpUserTime
    );











#line 2644 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
__declspec(noreturn)
void
__stdcall
ExitThread(
     DWORD dwExitCode
    );

__declspec(dllimport)
BOOL
__stdcall
TerminateThread(
      HANDLE hThread,
     DWORD dwExitCode
    );

__declspec(dllimport)
BOOL
__stdcall
GetExitCodeThread(
     HANDLE hThread,
     LPDWORD lpExitCode
    );

__declspec(dllimport)
BOOL
__stdcall
GetThreadSelectorEntry(
     HANDLE hThread,
     DWORD dwSelector,
     LPLDT_ENTRY lpSelectorEntry
    );

__declspec(dllimport)
EXECUTION_STATE
__stdcall
SetThreadExecutionState(
     EXECUTION_STATE esFlags
    );

__declspec(dllimport)
DWORD
__stdcall
GetLastError(
    void
    );

__declspec(dllimport)
void
__stdcall
SetLastError(
     DWORD dwErrCode
    );

















#line 2716 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"
#line 2717 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



__declspec(dllimport)
BOOL
__stdcall
GetOverlappedResult(
     HANDLE hFile,
     LPOVERLAPPED lpOverlapped,
     LPDWORD lpNumberOfBytesTransferred,
     BOOL bWait
    );

__declspec(dllimport)
HANDLE
__stdcall
CreateIoCompletionPort(
     HANDLE FileHandle,
     HANDLE ExistingCompletionPort,
     ULONG_PTR CompletionKey,
     DWORD NumberOfConcurrentThreads
    );

__declspec(dllimport)
BOOL
__stdcall
GetQueuedCompletionStatus(
      HANDLE CompletionPort,
     LPDWORD lpNumberOfBytesTransferred,
     PULONG_PTR lpCompletionKey,
     LPOVERLAPPED *lpOverlapped,
      DWORD dwMilliseconds
    );

__declspec(dllimport)
BOOL
__stdcall
PostQueuedCompletionStatus(
     HANDLE CompletionPort,
     DWORD dwNumberOfBytesTransferred,
     ULONG_PTR dwCompletionKey,
     LPOVERLAPPED lpOverlapped
    );






__declspec(dllimport)
UINT
__stdcall
SetErrorMode(
     UINT uMode
    );

__declspec(dllimport)
BOOL
__stdcall
ReadProcessMemory(
     HANDLE hProcess,
     LPCVOID lpBaseAddress,
     LPVOID lpBuffer,
     SIZE_T nSize,
     SIZE_T * lpNumberOfBytesRead
    );

__declspec(dllimport)
BOOL
__stdcall
WriteProcessMemory(
     HANDLE hProcess,
     LPVOID lpBaseAddress,
     LPCVOID lpBuffer,
     SIZE_T nSize,
     SIZE_T * lpNumberOfBytesWritten
    );


__declspec(dllimport)
BOOL
__stdcall
GetThreadContext(
     HANDLE hThread,
      LPCONTEXT lpContext
    );

__declspec(dllimport)
BOOL
__stdcall
SetThreadContext(
     HANDLE hThread,
     const CONTEXT *lpContext
    );
#line 2812 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
SuspendThread(
     HANDLE hThread
    );

__declspec(dllimport)
DWORD
__stdcall
ResumeThread(
     HANDLE hThread
    );



















#line 2846 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"








#line 2855 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"











#line 2867 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
void
__stdcall
DebugBreak(
    void
    );

__declspec(dllimport)
BOOL
__stdcall
WaitForDebugEvent(
     LPDEBUG_EVENT lpDebugEvent,
     DWORD dwMilliseconds
    );

__declspec(dllimport)
BOOL
__stdcall
ContinueDebugEvent(
     DWORD dwProcessId,
     DWORD dwThreadId,
     DWORD dwContinueStatus
    );

__declspec(dllimport)
BOOL
__stdcall
DebugActiveProcess(
     DWORD dwProcessId
    );

__declspec(dllimport)
BOOL
__stdcall
DebugActiveProcessStop(
     DWORD dwProcessId
    );

__declspec(dllimport)
BOOL
__stdcall
DebugSetProcessKillOnExit(
     BOOL KillOnExit
    );

__declspec(dllimport)
BOOL
__stdcall
DebugBreakProcess (
     HANDLE Process
    );

__declspec(dllimport)
void
__stdcall
InitializeCriticalSection(
     LPCRITICAL_SECTION lpCriticalSection
    );

__declspec(dllimport)
void
__stdcall
EnterCriticalSection(
      LPCRITICAL_SECTION lpCriticalSection
    );

__declspec(dllimport)
void
__stdcall
LeaveCriticalSection(
      LPCRITICAL_SECTION lpCriticalSection
    );

















#line 2958 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"








#line 2967 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
void
__stdcall
DeleteCriticalSection(
      LPCRITICAL_SECTION lpCriticalSection
    );

__declspec(dllimport)
BOOL
__stdcall
SetEvent(
     HANDLE hEvent
    );

__declspec(dllimport)
BOOL
__stdcall
ResetEvent(
     HANDLE hEvent
    );

__declspec(dllimport)
BOOL
__stdcall
PulseEvent(
     HANDLE hEvent
    );

__declspec(dllimport)
BOOL
__stdcall
ReleaseSemaphore(
     HANDLE hSemaphore,
     LONG lReleaseCount,
     LPLONG lpPreviousCount
    );

__declspec(dllimport)
BOOL
__stdcall
ReleaseMutex(
     HANDLE hMutex
    );

__declspec(dllimport)
DWORD
__stdcall
WaitForSingleObject(
     HANDLE hHandle,
     DWORD dwMilliseconds
    );

__declspec(dllimport)
DWORD
__stdcall
WaitForMultipleObjects(
     DWORD nCount,
     const HANDLE *lpHandles,
     BOOL bWaitAll,
     DWORD dwMilliseconds
    );

__declspec(dllimport)
void
__stdcall
Sleep(
     DWORD dwMilliseconds
    );

__declspec(dllimport)
HGLOBAL
__stdcall
LoadResource(
     HMODULE hModule,
     HRSRC hResInfo
    );

__declspec(dllimport)
DWORD
__stdcall
SizeofResource(
     HMODULE hModule,
     HRSRC hResInfo
    );


__declspec(dllimport)
ATOM
__stdcall
GlobalDeleteAtom(
     ATOM nAtom
    );

__declspec(dllimport)
BOOL
__stdcall
InitAtomTable(
     DWORD nSize
    );

__declspec(dllimport)
ATOM
__stdcall
DeleteAtom(
     ATOM nAtom
    );

__declspec(dllimport)
UINT
__stdcall
SetHandleCount(
     UINT uNumber
    );

__declspec(dllimport)
DWORD
__stdcall
GetLogicalDrives(
    void
    );

__declspec(dllimport)
BOOL
__stdcall
LockFile(
     HANDLE hFile,
     DWORD dwFileOffsetLow,
     DWORD dwFileOffsetHigh,
     DWORD nNumberOfBytesToLockLow,
     DWORD nNumberOfBytesToLockHigh
    );

__declspec(dllimport)
BOOL
__stdcall
UnlockFile(
     HANDLE hFile,
     DWORD dwFileOffsetLow,
     DWORD dwFileOffsetHigh,
     DWORD nNumberOfBytesToUnlockLow,
     DWORD nNumberOfBytesToUnlockHigh
    );

__declspec(dllimport)
BOOL
__stdcall
LockFileEx(
     HANDLE hFile,
     DWORD dwFlags,
     DWORD dwReserved,
     DWORD nNumberOfBytesToLockLow,
     DWORD nNumberOfBytesToLockHigh,
     LPOVERLAPPED lpOverlapped
    );




__declspec(dllimport)
BOOL
__stdcall
UnlockFileEx(
     HANDLE hFile,
     DWORD dwReserved,
     DWORD nNumberOfBytesToUnlockLow,
     DWORD nNumberOfBytesToUnlockHigh,
     LPOVERLAPPED lpOverlapped
    );

typedef struct _BY_HANDLE_FILE_INFORMATION {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD dwVolumeSerialNumber;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD nNumberOfLinks;
    DWORD nFileIndexHigh;
    DWORD nFileIndexLow;
} BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION, *LPBY_HANDLE_FILE_INFORMATION;

__declspec(dllimport)
BOOL
__stdcall
GetFileInformationByHandle(
     HANDLE hFile,
     LPBY_HANDLE_FILE_INFORMATION lpFileInformation
    );

__declspec(dllimport)
DWORD
__stdcall
GetFileType(
     HANDLE hFile
    );

__declspec(dllimport)
DWORD
__stdcall
GetFileSize(
     HANDLE hFile,
     LPDWORD lpFileSizeHigh
    );

__declspec(dllimport)
BOOL
__stdcall
GetFileSizeEx(
    HANDLE hFile,
    PLARGE_INTEGER lpFileSize
    );


__declspec(dllimport)
HANDLE
__stdcall
GetStdHandle(
     DWORD nStdHandle
    );

__declspec(dllimport)
BOOL
__stdcall
SetStdHandle(
     DWORD nStdHandle,
     HANDLE hHandle
    );

__declspec(dllimport)
BOOL
__stdcall
WriteFile(
     HANDLE hFile,
     LPCVOID lpBuffer,
     DWORD nNumberOfBytesToWrite,
     LPDWORD lpNumberOfBytesWritten,
     LPOVERLAPPED lpOverlapped
    );

__declspec(dllimport)
BOOL
__stdcall
ReadFile(
     HANDLE hFile,
     LPVOID lpBuffer,
     DWORD nNumberOfBytesToRead,
     LPDWORD lpNumberOfBytesRead,
     LPOVERLAPPED lpOverlapped
    );

__declspec(dllimport)
BOOL
__stdcall
FlushFileBuffers(
     HANDLE hFile
    );

__declspec(dllimport)
BOOL
__stdcall
DeviceIoControl(
     HANDLE hDevice,
     DWORD dwIoControlCode,
     LPVOID lpInBuffer,
     DWORD nInBufferSize,
     LPVOID lpOutBuffer,
     DWORD nOutBufferSize,
     LPDWORD lpBytesReturned,
     LPOVERLAPPED lpOverlapped
    );

__declspec(dllimport)
BOOL
__stdcall
RequestDeviceWakeup(
     HANDLE hDevice
    );

__declspec(dllimport)
BOOL
__stdcall
CancelDeviceWakeupRequest(
     HANDLE hDevice
    );

__declspec(dllimport)
BOOL
__stdcall
GetDevicePowerState(
     HANDLE hDevice,
     BOOL *pfOn
    );

__declspec(dllimport)
BOOL
__stdcall
SetMessageWaitingIndicator(
     HANDLE hMsgIndicator,
     ULONG ulMsgCount
    );

__declspec(dllimport)
BOOL
__stdcall
SetEndOfFile(
     HANDLE hFile
    );

__declspec(dllimport)
DWORD
__stdcall
SetFilePointer(
     HANDLE hFile,
     LONG lDistanceToMove,
     PLONG lpDistanceToMoveHigh,
     DWORD dwMoveMethod
    );

__declspec(dllimport)
BOOL
__stdcall
SetFilePointerEx(
    HANDLE hFile,
    LARGE_INTEGER liDistanceToMove,
    PLARGE_INTEGER lpNewFilePointer,
    DWORD dwMoveMethod
    );

__declspec(dllimport)
BOOL
__stdcall
FindClose(
      HANDLE hFindFile
    );

__declspec(dllimport)
BOOL
__stdcall
GetFileTime(
     HANDLE hFile,
     LPFILETIME lpCreationTime,
     LPFILETIME lpLastAccessTime,
     LPFILETIME lpLastWriteTime
    );

__declspec(dllimport)
BOOL
__stdcall
SetFileTime(
     HANDLE hFile,
     const FILETIME *lpCreationTime,
     const FILETIME *lpLastAccessTime,
     const FILETIME *lpLastWriteTime
    );

__declspec(dllimport)
BOOL
__stdcall
SetFileValidData(
     HANDLE hFile,
     LONGLONG ValidDataLength
    );

__declspec(dllimport)
BOOL
__stdcall
SetFileShortNameA(
     HANDLE hFile,
     LPCSTR lpShortName
    );
__declspec(dllimport)
BOOL
__stdcall
SetFileShortNameW(
     HANDLE hFile,
     LPCWSTR lpShortName
    );




#line 3351 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
CloseHandle(
      HANDLE hObject
    );

__declspec(dllimport)
BOOL
__stdcall
DuplicateHandle(
     HANDLE hSourceProcessHandle,
     HANDLE hSourceHandle,
     HANDLE hTargetProcessHandle,
     LPHANDLE lpTargetHandle,
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     DWORD dwOptions
    );

__declspec(dllimport)
BOOL
__stdcall
GetHandleInformation(
     HANDLE hObject,
     LPDWORD lpdwFlags
    );

__declspec(dllimport)
BOOL
__stdcall
SetHandleInformation(
     HANDLE hObject,
     DWORD dwMask,
     DWORD dwFlags
    );






__declspec(dllimport)
DWORD
__stdcall
LoadModule(
     LPCSTR lpModuleName,
     LPVOID lpParameterBlock
    );

__declspec(dllimport)
UINT
__stdcall
WinExec(
     LPCSTR lpCmdLine,
     UINT uCmdShow
    );

__declspec(dllimport)
BOOL
__stdcall
ClearCommBreak(
     HANDLE hFile
    );

__declspec(dllimport)
BOOL
__stdcall
ClearCommError(
     HANDLE hFile,
     LPDWORD lpErrors,
     LPCOMSTAT lpStat
    );

__declspec(dllimport)
BOOL
__stdcall
SetupComm(
     HANDLE hFile,
     DWORD dwInQueue,
     DWORD dwOutQueue
    );

__declspec(dllimport)
BOOL
__stdcall
EscapeCommFunction(
     HANDLE hFile,
     DWORD dwFunc
    );

__declspec(dllimport)
BOOL
__stdcall
GetCommConfig(
     HANDLE hCommDev,
     LPCOMMCONFIG lpCC,
      LPDWORD lpdwSize
    );

__declspec(dllimport)
BOOL
__stdcall
GetCommMask(
     HANDLE hFile,
     LPDWORD lpEvtMask
    );

__declspec(dllimport)
BOOL
__stdcall
GetCommProperties(
     HANDLE hFile,
     LPCOMMPROP lpCommProp
    );

__declspec(dllimport)
BOOL
__stdcall
GetCommModemStatus(
     HANDLE hFile,
     LPDWORD lpModemStat
    );

__declspec(dllimport)
BOOL
__stdcall
GetCommState(
     HANDLE hFile,
     LPDCB lpDCB
    );

__declspec(dllimport)
BOOL
__stdcall
GetCommTimeouts(
     HANDLE hFile,
     LPCOMMTIMEOUTS lpCommTimeouts
    );

__declspec(dllimport)
BOOL
__stdcall
PurgeComm(
     HANDLE hFile,
     DWORD dwFlags
    );

__declspec(dllimport)
BOOL
__stdcall
SetCommBreak(
     HANDLE hFile
    );

__declspec(dllimport)
BOOL
__stdcall
SetCommConfig(
     HANDLE hCommDev,
     LPCOMMCONFIG lpCC,
     DWORD dwSize
    );

__declspec(dllimport)
BOOL
__stdcall
SetCommMask(
     HANDLE hFile,
     DWORD dwEvtMask
    );

__declspec(dllimport)
BOOL
__stdcall
SetCommState(
     HANDLE hFile,
     LPDCB lpDCB
    );

__declspec(dllimport)
BOOL
__stdcall
SetCommTimeouts(
     HANDLE hFile,
     LPCOMMTIMEOUTS lpCommTimeouts
    );

__declspec(dllimport)
BOOL
__stdcall
TransmitCommChar(
     HANDLE hFile,
     char cChar
    );

__declspec(dllimport)
BOOL
__stdcall
WaitCommEvent(
     HANDLE hFile,
     LPDWORD lpEvtMask,
     LPOVERLAPPED lpOverlapped
    );


__declspec(dllimport)
DWORD
__stdcall
SetTapePosition(
     HANDLE hDevice,
     DWORD dwPositionMethod,
     DWORD dwPartition,
     DWORD dwOffsetLow,
     DWORD dwOffsetHigh,
     BOOL bImmediate
    );

__declspec(dllimport)
DWORD
__stdcall
GetTapePosition(
     HANDLE hDevice,
     DWORD dwPositionType,
     LPDWORD lpdwPartition,
     LPDWORD lpdwOffsetLow,
     LPDWORD lpdwOffsetHigh
    );

__declspec(dllimport)
DWORD
__stdcall
PrepareTape(
     HANDLE hDevice,
     DWORD dwOperation,
     BOOL bImmediate
    );

__declspec(dllimport)
DWORD
__stdcall
EraseTape(
     HANDLE hDevice,
     DWORD dwEraseType,
     BOOL bImmediate
    );

__declspec(dllimport)
DWORD
__stdcall
CreateTapePartition(
     HANDLE hDevice,
     DWORD dwPartitionMethod,
     DWORD dwCount,
     DWORD dwSize
    );

__declspec(dllimport)
DWORD
__stdcall
WriteTapemark(
     HANDLE hDevice,
     DWORD dwTapemarkType,
     DWORD dwTapemarkCount,
     BOOL bImmediate
    );

__declspec(dllimport)
DWORD
__stdcall
GetTapeStatus(
     HANDLE hDevice
    );

__declspec(dllimport)
DWORD
__stdcall
GetTapeParameters(
     HANDLE hDevice,
     DWORD dwOperation,
     LPDWORD lpdwSize,
     LPVOID lpTapeInformation
    );




__declspec(dllimport)
DWORD
__stdcall
SetTapeParameters(
     HANDLE hDevice,
     DWORD dwOperation,
     LPVOID lpTapeInformation
    );




__declspec(dllimport)
BOOL
__stdcall
Beep(
     DWORD dwFreq,
     DWORD dwDuration
    );

__declspec(dllimport)
int
__stdcall
MulDiv(
     int nNumber,
     int nNumerator,
     int nDenominator
    );

__declspec(dllimport)
void
__stdcall
GetSystemTime(
     LPSYSTEMTIME lpSystemTime
    );

__declspec(dllimport)
void
__stdcall
GetSystemTimeAsFileTime(
     LPFILETIME lpSystemTimeAsFileTime
    );

__declspec(dllimport)
BOOL
__stdcall
SetSystemTime(
     const SYSTEMTIME *lpSystemTime
    );

__declspec(dllimport)
void
__stdcall
GetLocalTime(
     LPSYSTEMTIME lpSystemTime
    );

__declspec(dllimport)
BOOL
__stdcall
SetLocalTime(
     const SYSTEMTIME *lpSystemTime
    );

__declspec(dllimport)
void
__stdcall
GetSystemInfo(
     LPSYSTEM_INFO lpSystemInfo
    );



















#line 3729 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"








#line 3738 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
IsProcessorFeaturePresent(
     DWORD ProcessorFeature
    );

typedef struct _TIME_ZONE_INFORMATION {
    LONG Bias;
    WCHAR StandardName[ 32 ];
    SYSTEMTIME StandardDate;
    LONG StandardBias;
    WCHAR DaylightName[ 32 ];
    SYSTEMTIME DaylightDate;
    LONG DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;

__declspec(dllimport)
BOOL
__stdcall
SystemTimeToTzSpecificLocalTime(
     LPTIME_ZONE_INFORMATION lpTimeZoneInformation,
     LPSYSTEMTIME lpUniversalTime,
     LPSYSTEMTIME lpLocalTime
    );

__declspec(dllimport)
BOOL
__stdcall
TzSpecificLocalTimeToSystemTime(
     LPTIME_ZONE_INFORMATION lpTimeZoneInformation,
     LPSYSTEMTIME lpLocalTime,
     LPSYSTEMTIME lpUniversalTime
    );

__declspec(dllimport)
DWORD
__stdcall
GetTimeZoneInformation(
     LPTIME_ZONE_INFORMATION lpTimeZoneInformation
    );

__declspec(dllimport)
BOOL
__stdcall
SetTimeZoneInformation(
     const TIME_ZONE_INFORMATION *lpTimeZoneInformation
    );






__declspec(dllimport)
BOOL
__stdcall
SystemTimeToFileTime(
     const SYSTEMTIME *lpSystemTime,
     LPFILETIME lpFileTime
    );

__declspec(dllimport)
BOOL
__stdcall
FileTimeToLocalFileTime(
     const FILETIME *lpFileTime,
     LPFILETIME lpLocalFileTime
    );

__declspec(dllimport)
BOOL
__stdcall
LocalFileTimeToFileTime(
     const FILETIME *lpLocalFileTime,
     LPFILETIME lpFileTime
    );

__declspec(dllimport)
BOOL
__stdcall
FileTimeToSystemTime(
     const FILETIME *lpFileTime,
     LPSYSTEMTIME lpSystemTime
    );

__declspec(dllimport)
LONG
__stdcall
CompareFileTime(
     const FILETIME *lpFileTime1,
     const FILETIME *lpFileTime2
    );

__declspec(dllimport)
BOOL
__stdcall
FileTimeToDosDateTime(
     const FILETIME *lpFileTime,
     LPWORD lpFatDate,
     LPWORD lpFatTime
    );

__declspec(dllimport)
BOOL
__stdcall
DosDateTimeToFileTime(
     WORD wFatDate,
     WORD wFatTime,
     LPFILETIME lpFileTime
    );

__declspec(dllimport)
DWORD
__stdcall
GetTickCount(
    void
    );

__declspec(dllimport)
BOOL
__stdcall
SetSystemTimeAdjustment(
     DWORD dwTimeAdjustment,
     BOOL  bTimeAdjustmentDisabled
    );

__declspec(dllimport)
BOOL
__stdcall
GetSystemTimeAdjustment(
     PDWORD lpTimeAdjustment,
     PDWORD lpTimeIncrement,
     PBOOL  lpTimeAdjustmentDisabled
    );


__declspec(dllimport)
DWORD
__stdcall
FormatMessageA(
     DWORD dwFlags,
     LPCVOID lpSource,
     DWORD dwMessageId,
     DWORD dwLanguageId,
     LPSTR lpBuffer,
     DWORD nSize,
     va_list *Arguments
    );
__declspec(dllimport)
DWORD
__stdcall
FormatMessageW(
     DWORD dwFlags,
     LPCVOID lpSource,
     DWORD dwMessageId,
     DWORD dwLanguageId,
     LPWSTR lpBuffer,
     DWORD nSize,
     va_list *Arguments
    );




#line 3905 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"
#line 3906 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"










__declspec(dllimport)
BOOL
__stdcall
CreatePipe(
     PHANDLE hReadPipe,
     PHANDLE hWritePipe,
     LPSECURITY_ATTRIBUTES lpPipeAttributes,
     DWORD nSize
    );

__declspec(dllimport)
BOOL
__stdcall
ConnectNamedPipe(
     HANDLE hNamedPipe,
     LPOVERLAPPED lpOverlapped
    );

__declspec(dllimport)
BOOL
__stdcall
DisconnectNamedPipe(
     HANDLE hNamedPipe
    );

__declspec(dllimport)
BOOL
__stdcall
SetNamedPipeHandleState(
     HANDLE hNamedPipe,
     LPDWORD lpMode,
     LPDWORD lpMaxCollectionCount,
     LPDWORD lpCollectDataTimeout
    );

__declspec(dllimport)
BOOL
__stdcall
GetNamedPipeInfo(
     HANDLE hNamedPipe,
     LPDWORD lpFlags,
     LPDWORD lpOutBufferSize,
     LPDWORD lpInBufferSize,
     LPDWORD lpMaxInstances
    );

__declspec(dllimport)
BOOL
__stdcall
PeekNamedPipe(
     HANDLE hNamedPipe,
     LPVOID lpBuffer,
     DWORD nBufferSize,
     LPDWORD lpBytesRead,
     LPDWORD lpTotalBytesAvail,
     LPDWORD lpBytesLeftThisMessage
    );

__declspec(dllimport)
BOOL
__stdcall
TransactNamedPipe(
     HANDLE hNamedPipe,
     LPVOID lpInBuffer,
     DWORD nInBufferSize,
     LPVOID lpOutBuffer,
     DWORD nOutBufferSize,
     LPDWORD lpBytesRead,
     LPOVERLAPPED lpOverlapped
    );

__declspec(dllimport)
HANDLE
__stdcall
CreateMailslotA(
     LPCSTR lpName,
     DWORD nMaxMessageSize,
     DWORD lReadTimeout,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );
__declspec(dllimport)
HANDLE
__stdcall
CreateMailslotW(
     LPCWSTR lpName,
     DWORD nMaxMessageSize,
     DWORD lReadTimeout,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );




#line 4010 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetMailslotInfo(
     HANDLE hMailslot,
     LPDWORD lpMaxMessageSize,
     LPDWORD lpNextSize,
     LPDWORD lpMessageCount,
     LPDWORD lpReadTimeout
    );

__declspec(dllimport)
BOOL
__stdcall
SetMailslotInfo(
     HANDLE hMailslot,
     DWORD lReadTimeout
    );

__declspec(dllimport)
LPVOID
__stdcall
MapViewOfFile(
     HANDLE hFileMappingObject,
     DWORD dwDesiredAccess,
     DWORD dwFileOffsetHigh,
     DWORD dwFileOffsetLow,
     SIZE_T dwNumberOfBytesToMap
    );

__declspec(dllimport)
BOOL
__stdcall
FlushViewOfFile(
     LPCVOID lpBaseAddress,
     SIZE_T dwNumberOfBytesToFlush
    );

__declspec(dllimport)
BOOL
__stdcall
UnmapViewOfFile(
     LPCVOID lpBaseAddress
    );





__declspec(dllimport)
BOOL
__stdcall
EncryptFileA(
     LPCSTR lpFileName
    );
__declspec(dllimport)
BOOL
__stdcall
EncryptFileW(
     LPCWSTR lpFileName
    );




#line 4077 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
DecryptFileA(
     LPCSTR lpFileName,
     DWORD    dwReserved
    );
__declspec(dllimport)
BOOL
__stdcall
DecryptFileW(
     LPCWSTR lpFileName,
     DWORD    dwReserved
    );




#line 4097 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"
















__declspec(dllimport)
BOOL
__stdcall
FileEncryptionStatusA(
    LPCSTR lpFileName,
    LPDWORD  lpStatus
    );
__declspec(dllimport)
BOOL
__stdcall
FileEncryptionStatusW(
    LPCWSTR lpFileName,
    LPDWORD  lpStatus
    );




#line 4132 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"







typedef
DWORD
(__stdcall *PFE_EXPORT_FUNC)(
    PBYTE pbData,
    PVOID pvCallbackContext,
    ULONG ulLength
    );

typedef
DWORD
(__stdcall *PFE_IMPORT_FUNC)(
    PBYTE pbData,
    PVOID pvCallbackContext,
    PULONG ulLength
    );











__declspec(dllimport)
DWORD
__stdcall
OpenEncryptedFileRawA(
     LPCSTR        lpFileName,
     ULONG           ulFlags,
     PVOID *         pvContext
    );
__declspec(dllimport)
DWORD
__stdcall
OpenEncryptedFileRawW(
     LPCWSTR        lpFileName,
     ULONG           ulFlags,
     PVOID *         pvContext
    );




#line 4186 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
ReadEncryptedFileRaw(
     PFE_EXPORT_FUNC pfExportCallback,
     PVOID           pvCallbackContext,
     PVOID           pvContext
    );

__declspec(dllimport)
DWORD
__stdcall
WriteEncryptedFileRaw(
     PFE_IMPORT_FUNC pfImportCallback,
     PVOID           pvCallbackContext,
     PVOID           pvContext
    );

__declspec(dllimport)
void
__stdcall
CloseEncryptedFileRaw(
     PVOID           pvContext
    );





__declspec(dllimport)
int
__stdcall
lstrcmpA(
     LPCSTR lpString1,
     LPCSTR lpString2
    );
__declspec(dllimport)
int
__stdcall
lstrcmpW(
     LPCWSTR lpString1,
     LPCWSTR lpString2
    );




#line 4235 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
int
__stdcall
lstrcmpiA(
     LPCSTR lpString1,
     LPCSTR lpString2
    );
__declspec(dllimport)
int
__stdcall
lstrcmpiW(
     LPCWSTR lpString1,
     LPCWSTR lpString2
    );




#line 4255 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
LPSTR
__stdcall
lstrcpynA(
     LPSTR lpString1,
     LPCSTR lpString2,
     int iMaxLength
    );
__declspec(dllimport)
LPWSTR
__stdcall
lstrcpynW(
     LPWSTR lpString1,
     LPCWSTR lpString2,
     int iMaxLength
    );




#line 4277 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
LPSTR
__stdcall
lstrcpyA(
     LPSTR lpString1,
     LPCSTR lpString2
    );
__declspec(dllimport)
LPWSTR
__stdcall
lstrcpyW(
     LPWSTR lpString1,
     LPCWSTR lpString2
    );




#line 4297 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
LPSTR
__stdcall
lstrcatA(
      LPSTR lpString1,
     LPCSTR lpString2
    );
__declspec(dllimport)
LPWSTR
__stdcall
lstrcatW(
      LPWSTR lpString1,
     LPCWSTR lpString2
    );




#line 4317 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
int
__stdcall
lstrlenA(
     LPCSTR lpString
    );
__declspec(dllimport)
int
__stdcall
lstrlenW(
     LPCWSTR lpString
    );




#line 4335 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HFILE
__stdcall
OpenFile(
     LPCSTR lpFileName,
     LPOFSTRUCT lpReOpenBuff,
     UINT uStyle
    );

__declspec(dllimport)
HFILE
__stdcall
_lopen(
     LPCSTR lpPathName,
     int iReadWrite
    );

__declspec(dllimport)
HFILE
__stdcall
_lcreat(
     LPCSTR lpPathName,
     int  iAttribute
    );

__declspec(dllimport)
UINT
__stdcall
_lread(
     HFILE hFile,
     LPVOID lpBuffer,
     UINT uBytes
    );

__declspec(dllimport)
UINT
__stdcall
_lwrite(
     HFILE hFile,
     LPCSTR lpBuffer,
     UINT uBytes
    );

__declspec(dllimport)
long
__stdcall
_hread(
     HFILE hFile,
     LPVOID lpBuffer,
     long lBytes
    );

__declspec(dllimport)
long
__stdcall
_hwrite(
     HFILE hFile,
     LPCSTR lpBuffer,
     long lBytes
    );

__declspec(dllimport)
HFILE
__stdcall
_lclose(
      HFILE hFile
    );

__declspec(dllimport)
LONG
__stdcall
_llseek(
     HFILE hFile,
     LONG lOffset,
     int iOrigin
    );

__declspec(dllimport)
BOOL
__stdcall
IsTextUnicode(
     const void* lpBuffer,
     int cb,
      LPINT lpi
    );

typedef
void
(__stdcall *PFLS_CALLBACK_FUNCTION) (
     PVOID lpFlsData
    );



__declspec(dllimport)
DWORD
__stdcall
FlsAlloc(
     PFLS_CALLBACK_FUNCTION lpCallback 
    );

__declspec(dllimport)
PVOID
__stdcall
FlsGetValue(
     DWORD dwFlsIndex
    );

__declspec(dllimport)
BOOL
__stdcall
FlsSetValue(
     DWORD dwFlsIndex,
     PVOID lpFlsData
    );

__declspec(dllimport)
BOOL
__stdcall
FlsFree(
     DWORD dwFlsIndex
    );



__declspec(dllimport)
DWORD
__stdcall
TlsAlloc(
    void
    );

__declspec(dllimport)
LPVOID
__stdcall
TlsGetValue(
     DWORD dwTlsIndex
    );

__declspec(dllimport)
BOOL
__stdcall
TlsSetValue(
     DWORD dwTlsIndex,
     LPVOID lpTlsValue
    );

__declspec(dllimport)
BOOL
__stdcall
TlsFree(
     DWORD dwTlsIndex
    );

typedef
void
(__stdcall *LPOVERLAPPED_COMPLETION_ROUTINE)(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped
    );

__declspec(dllimport)
DWORD
__stdcall
SleepEx(
     DWORD dwMilliseconds,
     BOOL bAlertable
    );

__declspec(dllimport)
DWORD
__stdcall
WaitForSingleObjectEx(
     HANDLE hHandle,
     DWORD dwMilliseconds,
     BOOL bAlertable
    );

__declspec(dllimport)
DWORD
__stdcall
WaitForMultipleObjectsEx(
     DWORD nCount,
     const HANDLE *lpHandles,
     BOOL bWaitAll,
     DWORD dwMilliseconds,
     BOOL bAlertable
    );











#line 4537 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
ReadFileEx(
     HANDLE hFile,
     LPVOID lpBuffer,
     DWORD nNumberOfBytesToRead,
     LPOVERLAPPED lpOverlapped,
     LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
    );

__declspec(dllimport)
BOOL
__stdcall
WriteFileEx(
     HANDLE hFile,
     LPCVOID lpBuffer,
     DWORD nNumberOfBytesToWrite,
     LPOVERLAPPED lpOverlapped,
     LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
    );

__declspec(dllimport)
BOOL
__stdcall
BackupRead(
     HANDLE hFile,
     LPBYTE lpBuffer,
     DWORD nNumberOfBytesToRead,
     LPDWORD lpNumberOfBytesRead,
     BOOL bAbort,
     BOOL bProcessSecurity,
     LPVOID *lpContext
    );

__declspec(dllimport)
BOOL
__stdcall
BackupSeek(
     HANDLE hFile,
     DWORD  dwLowBytesToSeek,
     DWORD  dwHighBytesToSeek,
     LPDWORD lpdwLowByteSeeked,
     LPDWORD lpdwHighByteSeeked,
     LPVOID *lpContext
    );

__declspec(dllimport)
BOOL
__stdcall
BackupWrite(
     HANDLE hFile,
     LPBYTE lpBuffer,
     DWORD nNumberOfBytesToWrite,
     LPDWORD lpNumberOfBytesWritten,
     BOOL bAbort,
     BOOL bProcessSecurity,
     LPVOID *lpContext
    );




typedef struct _WIN32_STREAM_ID {
        DWORD          dwStreamId ;
        DWORD          dwStreamAttributes ;
        LARGE_INTEGER  Size ;
        DWORD          dwStreamNameSize ;
        WCHAR          cStreamName[ 1 ] ;
} WIN32_STREAM_ID, *LPWIN32_STREAM_ID ;



























__declspec(dllimport)
BOOL
__stdcall
ReadFileScatter(
     HANDLE hFile,
     FILE_SEGMENT_ELEMENT aSegmentArray[],
     DWORD nNumberOfBytesToRead,
     LPDWORD lpReserved,
     LPOVERLAPPED lpOverlapped
    );

__declspec(dllimport)
BOOL
__stdcall
WriteFileGather(
     HANDLE hFile,
     FILE_SEGMENT_ELEMENT aSegmentArray[],
     DWORD nNumberOfBytesToWrite,
     LPDWORD lpReserved,
     LPOVERLAPPED lpOverlapped
    );


















#line 4675 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

typedef struct _STARTUPINFOA {
    DWORD   cb;
    LPSTR   lpReserved;
    LPSTR   lpDesktop;
    LPSTR   lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;
typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;




typedef STARTUPINFOA STARTUPINFO;
typedef LPSTARTUPINFOA LPSTARTUPINFO;
#line 4723 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



typedef struct _WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR   cFileName[ 260 ];
    CHAR   cAlternateFileName[ 14 ];





} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
typedef struct _WIN32_FIND_DATAW {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    WCHAR  cFileName[ 260 ];
    WCHAR  cAlternateFileName[ 14 ];





} WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;





typedef WIN32_FIND_DATAA WIN32_FIND_DATA;
typedef PWIN32_FIND_DATAA PWIN32_FIND_DATA;
typedef LPWIN32_FIND_DATAA LPWIN32_FIND_DATA;
#line 4769 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

typedef struct _WIN32_FILE_ATTRIBUTE_DATA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
} WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;

__declspec(dllimport)
HANDLE
__stdcall
CreateMutexA(
     LPSECURITY_ATTRIBUTES lpMutexAttributes,
     BOOL bInitialOwner,
     LPCSTR lpName
    );
__declspec(dllimport)
HANDLE
__stdcall
CreateMutexW(
     LPSECURITY_ATTRIBUTES lpMutexAttributes,
     BOOL bInitialOwner,
     LPCWSTR lpName
    );




#line 4800 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
OpenMutexA(
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     LPCSTR lpName
    );
__declspec(dllimport)
HANDLE
__stdcall
OpenMutexW(
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     LPCWSTR lpName
    );




#line 4822 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
CreateEventA(
     LPSECURITY_ATTRIBUTES lpEventAttributes,
     BOOL bManualReset,
     BOOL bInitialState,
     LPCSTR lpName
    );
__declspec(dllimport)
HANDLE
__stdcall
CreateEventW(
     LPSECURITY_ATTRIBUTES lpEventAttributes,
     BOOL bManualReset,
     BOOL bInitialState,
     LPCWSTR lpName
    );




#line 4846 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
OpenEventA(
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     LPCSTR lpName
    );
__declspec(dllimport)
HANDLE
__stdcall
OpenEventW(
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     LPCWSTR lpName
    );




#line 4868 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
CreateSemaphoreA(
     LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
     LONG lInitialCount,
     LONG lMaximumCount,
     LPCSTR lpName
    );
__declspec(dllimport)
HANDLE
__stdcall
CreateSemaphoreW(
     LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
     LONG lInitialCount,
     LONG lMaximumCount,
     LPCWSTR lpName
    );




#line 4892 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
OpenSemaphoreA(
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     LPCSTR lpName
    );
__declspec(dllimport)
HANDLE
__stdcall
OpenSemaphoreW(
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     LPCWSTR lpName
    );




#line 4914 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"








































































#line 4987 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
CreateFileMappingA(
     HANDLE hFile,
     LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
     DWORD flProtect,
     DWORD dwMaximumSizeHigh,
     DWORD dwMaximumSizeLow,
     LPCSTR lpName
    );
__declspec(dllimport)
HANDLE
__stdcall
CreateFileMappingW(
     HANDLE hFile,
     LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
     DWORD flProtect,
     DWORD dwMaximumSizeHigh,
     DWORD dwMaximumSizeLow,
     LPCWSTR lpName
    );




#line 5015 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
OpenFileMappingA(
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     LPCSTR lpName
    );
__declspec(dllimport)
HANDLE
__stdcall
OpenFileMappingW(
     DWORD dwDesiredAccess,
     BOOL bInheritHandle,
     LPCWSTR lpName
    );




#line 5037 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetLogicalDriveStringsA(
     DWORD nBufferLength,
     LPSTR lpBuffer
    );
__declspec(dllimport)
DWORD
__stdcall
GetLogicalDriveStringsW(
     DWORD nBufferLength,
     LPWSTR lpBuffer
    );




#line 5057 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"























#line 5081 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
HMODULE
__stdcall
LoadLibraryA(
     LPCSTR lpLibFileName
    );
__declspec(dllimport)
HMODULE
__stdcall
LoadLibraryW(
     LPCWSTR lpLibFileName
    );




#line 5100 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HMODULE
__stdcall
LoadLibraryExA(
     LPCSTR lpLibFileName,
     HANDLE hFile,
     DWORD dwFlags
    );
__declspec(dllimport)
HMODULE
__stdcall
LoadLibraryExW(
     LPCWSTR lpLibFileName,
     HANDLE hFile,
     DWORD dwFlags
    );




#line 5122 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"








__declspec(dllimport)
DWORD
__stdcall
GetModuleFileNameA(
     HMODULE hModule,
     LPSTR lpFilename,
     DWORD nSize
    );
__declspec(dllimport)
DWORD
__stdcall
GetModuleFileNameW(
     HMODULE hModule,
     LPWSTR lpFilename,
     DWORD nSize
    );




#line 5151 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HMODULE
__stdcall
GetModuleHandleA(
     LPCSTR lpModuleName
    );
__declspec(dllimport)
HMODULE
__stdcall
GetModuleHandleW(
     LPCWSTR lpModuleName
    );




#line 5169 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"




















































#line 5222 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"
#line 5223 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
CreateProcessA(
     LPCSTR lpApplicationName,
     LPSTR lpCommandLine,
     LPSECURITY_ATTRIBUTES lpProcessAttributes,
     LPSECURITY_ATTRIBUTES lpThreadAttributes,
     BOOL bInheritHandles,
     DWORD dwCreationFlags,
     LPVOID lpEnvironment,
     LPCSTR lpCurrentDirectory,
     LPSTARTUPINFOA lpStartupInfo,
     LPPROCESS_INFORMATION lpProcessInformation
    );
__declspec(dllimport)
BOOL
__stdcall
CreateProcessW(
     LPCWSTR lpApplicationName,
     LPWSTR lpCommandLine,
     LPSECURITY_ATTRIBUTES lpProcessAttributes,
     LPSECURITY_ATTRIBUTES lpThreadAttributes,
     BOOL bInheritHandles,
     DWORD dwCreationFlags,
     LPVOID lpEnvironment,
     LPCWSTR lpCurrentDirectory,
     LPSTARTUPINFOW lpStartupInfo,
     LPPROCESS_INFORMATION lpProcessInformation
    );




#line 5259 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



__declspec(dllimport)
BOOL
__stdcall
SetProcessShutdownParameters(
     DWORD dwLevel,
     DWORD dwFlags
    );

__declspec(dllimport)
BOOL
__stdcall
GetProcessShutdownParameters(
     LPDWORD lpdwLevel,
     LPDWORD lpdwFlags
    );

__declspec(dllimport)
DWORD
__stdcall
GetProcessVersion(
     DWORD ProcessId
    );

__declspec(dllimport)
void
__stdcall
FatalAppExitA(
     UINT uAction,
     LPCSTR lpMessageText
    );
__declspec(dllimport)
void
__stdcall
FatalAppExitW(
     UINT uAction,
     LPCWSTR lpMessageText
    );




#line 5304 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
void
__stdcall
GetStartupInfoA(
     LPSTARTUPINFOA lpStartupInfo
    );
__declspec(dllimport)
void
__stdcall
GetStartupInfoW(
     LPSTARTUPINFOW lpStartupInfo
    );




#line 5322 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
LPSTR
__stdcall
GetCommandLineA(
    void
    );
__declspec(dllimport)
LPWSTR
__stdcall
GetCommandLineW(
    void
    );




#line 5340 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetEnvironmentVariableA(
     LPCSTR lpName,
     LPSTR lpBuffer,
     DWORD nSize
    );
__declspec(dllimport)
DWORD
__stdcall
GetEnvironmentVariableW(
     LPCWSTR lpName,
     LPWSTR lpBuffer,
     DWORD nSize
    );




#line 5362 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetEnvironmentVariableA(
     LPCSTR lpName,
     LPCSTR lpValue
    );
__declspec(dllimport)
BOOL
__stdcall
SetEnvironmentVariableW(
     LPCWSTR lpName,
     LPCWSTR lpValue
    );




#line 5382 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
ExpandEnvironmentStringsA(
     LPCSTR lpSrc,
     LPSTR lpDst,
     DWORD nSize
    );
__declspec(dllimport)
DWORD
__stdcall
ExpandEnvironmentStringsW(
     LPCWSTR lpSrc,
     LPWSTR lpDst,
     DWORD nSize
    );




#line 5404 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetFirmwareEnvironmentVariableA(
     LPCSTR lpName,
     LPCSTR lpGuid,
     PVOID   pBuffer,
     DWORD    nSize
    );
__declspec(dllimport)
DWORD
__stdcall
GetFirmwareEnvironmentVariableW(
     LPCWSTR lpName,
     LPCWSTR lpGuid,
     PVOID   pBuffer,
     DWORD    nSize
    );




#line 5428 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetFirmwareEnvironmentVariableA(
     LPCSTR lpName,
     LPCSTR lpGuid,
     PVOID    pValue,
     DWORD    nSize
    );
__declspec(dllimport)
BOOL
__stdcall
SetFirmwareEnvironmentVariableW(
     LPCWSTR lpName,
     LPCWSTR lpGuid,
     PVOID    pValue,
     DWORD    nSize
    );




#line 5452 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
void
__stdcall
OutputDebugStringA(
     LPCSTR lpOutputString
    );
__declspec(dllimport)
void
__stdcall
OutputDebugStringW(
     LPCWSTR lpOutputString
    );




#line 5471 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HRSRC
__stdcall
FindResourceA(
     HMODULE hModule,
     LPCSTR lpName,
     LPCSTR lpType
    );
__declspec(dllimport)
HRSRC
__stdcall
FindResourceW(
     HMODULE hModule,
     LPCWSTR lpName,
     LPCWSTR lpType
    );




#line 5493 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HRSRC
__stdcall
FindResourceExA(
     HMODULE hModule,
     LPCSTR lpType,
     LPCSTR lpName,
     WORD    wLanguage
    );
__declspec(dllimport)
HRSRC
__stdcall
FindResourceExW(
     HMODULE hModule,
     LPCWSTR lpType,
     LPCWSTR lpName,
     WORD    wLanguage
    );




#line 5517 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


typedef BOOL (__stdcall* ENUMRESTYPEPROCA)(HMODULE hModule, LPSTR lpType,
        LONG_PTR lParam);
typedef BOOL (__stdcall* ENUMRESTYPEPROCW)(HMODULE hModule, LPWSTR lpType,
        LONG_PTR lParam);




#line 5528 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"
typedef BOOL (__stdcall* ENUMRESNAMEPROCA)(HMODULE hModule, LPCSTR lpType,
        LPSTR lpName, LONG_PTR lParam);
typedef BOOL (__stdcall* ENUMRESNAMEPROCW)(HMODULE hModule, LPCWSTR lpType,
        LPWSTR lpName, LONG_PTR lParam);




#line 5537 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"
typedef BOOL (__stdcall* ENUMRESLANGPROCA)(HMODULE hModule, LPCSTR lpType,
        LPCSTR lpName, WORD  wLanguage, LONG_PTR lParam);
typedef BOOL (__stdcall* ENUMRESLANGPROCW)(HMODULE hModule, LPCWSTR lpType,
        LPCWSTR lpName, WORD  wLanguage, LONG_PTR lParam);




#line 5546 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"






















#line 5569 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
EnumResourceTypesA(
     HMODULE hModule,
     ENUMRESTYPEPROCA lpEnumFunc,
     LONG_PTR lParam
    );
__declspec(dllimport)
BOOL
__stdcall
EnumResourceTypesW(
     HMODULE hModule,
     ENUMRESTYPEPROCW lpEnumFunc,
     LONG_PTR lParam
    );




#line 5591 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
EnumResourceNamesA(
     HMODULE hModule,
     LPCSTR lpType,
     ENUMRESNAMEPROCA lpEnumFunc,
     LONG_PTR lParam
    );
__declspec(dllimport)
BOOL
__stdcall
EnumResourceNamesW(
     HMODULE hModule,
     LPCWSTR lpType,
     ENUMRESNAMEPROCW lpEnumFunc,
     LONG_PTR lParam
    );




#line 5616 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
EnumResourceLanguagesA(
     HMODULE hModule,
     LPCSTR lpType,
     LPCSTR lpName,
     ENUMRESLANGPROCA lpEnumFunc,
     LONG_PTR lParam
    );
__declspec(dllimport)
BOOL
__stdcall
EnumResourceLanguagesW(
     HMODULE hModule,
     LPCWSTR lpType,
     LPCWSTR lpName,
     ENUMRESLANGPROCW lpEnumFunc,
     LONG_PTR lParam
    );




#line 5642 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
BeginUpdateResourceA(
     LPCSTR pFileName,
     BOOL bDeleteExistingResources
    );
__declspec(dllimport)
HANDLE
__stdcall
BeginUpdateResourceW(
     LPCWSTR pFileName,
     BOOL bDeleteExistingResources
    );




#line 5662 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
UpdateResourceA(
     HANDLE      hUpdate,
     LPCSTR     lpType,
     LPCSTR     lpName,
     WORD        wLanguage,
     LPVOID      lpData,
     DWORD       cbData
    );
__declspec(dllimport)
BOOL
__stdcall
UpdateResourceW(
     HANDLE      hUpdate,
     LPCWSTR     lpType,
     LPCWSTR     lpName,
     WORD        wLanguage,
     LPVOID      lpData,
     DWORD       cbData
    );




#line 5690 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
EndUpdateResourceA(
     HANDLE      hUpdate,
     BOOL        fDiscard
    );
__declspec(dllimport)
BOOL
__stdcall
EndUpdateResourceW(
     HANDLE      hUpdate,
     BOOL        fDiscard
    );




#line 5710 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
ATOM
__stdcall
GlobalAddAtomA(
     LPCSTR lpString
    );
__declspec(dllimport)
ATOM
__stdcall
GlobalAddAtomW(
     LPCWSTR lpString
    );




#line 5728 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
ATOM
__stdcall
GlobalFindAtomA(
     LPCSTR lpString
    );
__declspec(dllimport)
ATOM
__stdcall
GlobalFindAtomW(
     LPCWSTR lpString
    );




#line 5746 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
UINT
__stdcall
GlobalGetAtomNameA(
     ATOM nAtom,
     LPSTR lpBuffer,
     int nSize
    );
__declspec(dllimport)
UINT
__stdcall
GlobalGetAtomNameW(
     ATOM nAtom,
     LPWSTR lpBuffer,
     int nSize
    );




#line 5768 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
ATOM
__stdcall
AddAtomA(
     LPCSTR lpString
    );
__declspec(dllimport)
ATOM
__stdcall
AddAtomW(
     LPCWSTR lpString
    );




#line 5786 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
ATOM
__stdcall
FindAtomA(
     LPCSTR lpString
    );
__declspec(dllimport)
ATOM
__stdcall
FindAtomW(
     LPCWSTR lpString
    );




#line 5804 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
UINT
__stdcall
GetAtomNameA(
     ATOM nAtom,
     LPSTR lpBuffer,
     int nSize
    );
__declspec(dllimport)
UINT
__stdcall
GetAtomNameW(
     ATOM nAtom,
     LPWSTR lpBuffer,
     int nSize
    );




#line 5826 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
UINT
__stdcall
GetProfileIntA(
     LPCSTR lpAppName,
     LPCSTR lpKeyName,
     INT nDefault
    );
__declspec(dllimport)
UINT
__stdcall
GetProfileIntW(
     LPCWSTR lpAppName,
     LPCWSTR lpKeyName,
     INT nDefault
    );




#line 5848 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetProfileStringA(
     LPCSTR lpAppName,
     LPCSTR lpKeyName,
     LPCSTR lpDefault,
     LPSTR lpReturnedString,
     DWORD nSize
    );
__declspec(dllimport)
DWORD
__stdcall
GetProfileStringW(
     LPCWSTR lpAppName,
     LPCWSTR lpKeyName,
     LPCWSTR lpDefault,
     LPWSTR lpReturnedString,
     DWORD nSize
    );




#line 5874 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
WriteProfileStringA(
     LPCSTR lpAppName,
     LPCSTR lpKeyName,
     LPCSTR lpString
    );
__declspec(dllimport)
BOOL
__stdcall
WriteProfileStringW(
     LPCWSTR lpAppName,
     LPCWSTR lpKeyName,
     LPCWSTR lpString
    );




#line 5896 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetProfileSectionA(
     LPCSTR lpAppName,
     LPSTR lpReturnedString,
     DWORD nSize
    );
__declspec(dllimport)
DWORD
__stdcall
GetProfileSectionW(
     LPCWSTR lpAppName,
     LPWSTR lpReturnedString,
     DWORD nSize
    );




#line 5918 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
WriteProfileSectionA(
     LPCSTR lpAppName,
     LPCSTR lpString
    );
__declspec(dllimport)
BOOL
__stdcall
WriteProfileSectionW(
     LPCWSTR lpAppName,
     LPCWSTR lpString
    );




#line 5938 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
UINT
__stdcall
GetPrivateProfileIntA(
     LPCSTR lpAppName,
     LPCSTR lpKeyName,
     INT nDefault,
     LPCSTR lpFileName
    );
__declspec(dllimport)
UINT
__stdcall
GetPrivateProfileIntW(
     LPCWSTR lpAppName,
     LPCWSTR lpKeyName,
     INT nDefault,
     LPCWSTR lpFileName
    );




#line 5962 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetPrivateProfileStringA(
     LPCSTR lpAppName,
     LPCSTR lpKeyName,
     LPCSTR lpDefault,
     LPSTR lpReturnedString,
     DWORD nSize,
     LPCSTR lpFileName
    );
__declspec(dllimport)
DWORD
__stdcall
GetPrivateProfileStringW(
     LPCWSTR lpAppName,
     LPCWSTR lpKeyName,
     LPCWSTR lpDefault,
     LPWSTR lpReturnedString,
     DWORD nSize,
     LPCWSTR lpFileName
    );




#line 5990 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
WritePrivateProfileStringA(
     LPCSTR lpAppName,
     LPCSTR lpKeyName,
     LPCSTR lpString,
     LPCSTR lpFileName
    );
__declspec(dllimport)
BOOL
__stdcall
WritePrivateProfileStringW(
     LPCWSTR lpAppName,
     LPCWSTR lpKeyName,
     LPCWSTR lpString,
     LPCWSTR lpFileName
    );




#line 6014 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetPrivateProfileSectionA(
     LPCSTR lpAppName,
     LPSTR lpReturnedString,
     DWORD nSize,
     LPCSTR lpFileName
    );
__declspec(dllimport)
DWORD
__stdcall
GetPrivateProfileSectionW(
     LPCWSTR lpAppName,
     LPWSTR lpReturnedString,
     DWORD nSize,
     LPCWSTR lpFileName
    );




#line 6038 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
WritePrivateProfileSectionA(
     LPCSTR lpAppName,
     LPCSTR lpString,
     LPCSTR lpFileName
    );
__declspec(dllimport)
BOOL
__stdcall
WritePrivateProfileSectionW(
     LPCWSTR lpAppName,
     LPCWSTR lpString,
     LPCWSTR lpFileName
    );




#line 6060 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
DWORD
__stdcall
GetPrivateProfileSectionNamesA(
     LPSTR lpszReturnBuffer,
     DWORD nSize,
     LPCSTR lpFileName
    );
__declspec(dllimport)
DWORD
__stdcall
GetPrivateProfileSectionNamesW(
     LPWSTR lpszReturnBuffer,
     DWORD nSize,
     LPCWSTR lpFileName
    );




#line 6083 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetPrivateProfileStructA(
     LPCSTR lpszSection,
     LPCSTR lpszKey,
     LPVOID   lpStruct,
     UINT     uSizeStruct,
     LPCSTR szFile
    );
__declspec(dllimport)
BOOL
__stdcall
GetPrivateProfileStructW(
     LPCWSTR lpszSection,
     LPCWSTR lpszKey,
     LPVOID   lpStruct,
     UINT     uSizeStruct,
     LPCWSTR szFile
    );




#line 6109 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
WritePrivateProfileStructA(
     LPCSTR lpszSection,
     LPCSTR lpszKey,
     LPVOID   lpStruct,
     UINT     uSizeStruct,
     LPCSTR szFile
    );
__declspec(dllimport)
BOOL
__stdcall
WritePrivateProfileStructW(
     LPCWSTR lpszSection,
     LPCWSTR lpszKey,
     LPVOID   lpStruct,
     UINT     uSizeStruct,
     LPCWSTR szFile
    );




#line 6135 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
UINT
__stdcall
GetDriveTypeA(
     LPCSTR lpRootPathName
    );
__declspec(dllimport)
UINT
__stdcall
GetDriveTypeW(
     LPCWSTR lpRootPathName
    );




#line 6154 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
UINT
__stdcall
GetSystemDirectoryA(
     LPSTR lpBuffer,
     UINT uSize
    );
__declspec(dllimport)
UINT
__stdcall
GetSystemDirectoryW(
     LPWSTR lpBuffer,
     UINT uSize
    );




#line 6174 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetTempPathA(
     DWORD nBufferLength,
     LPSTR lpBuffer
    );
__declspec(dllimport)
DWORD
__stdcall
GetTempPathW(
     DWORD nBufferLength,
     LPWSTR lpBuffer
    );




#line 6194 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
UINT
__stdcall
GetTempFileNameA(
     LPCSTR lpPathName,
     LPCSTR lpPrefixString,
     UINT uUnique,
     LPSTR lpTempFileName
    );
__declspec(dllimport)
UINT
__stdcall
GetTempFileNameW(
     LPCWSTR lpPathName,
     LPCWSTR lpPrefixString,
     UINT uUnique,
     LPWSTR lpTempFileName
    );




#line 6218 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
UINT
__stdcall
GetWindowsDirectoryA(
     LPSTR lpBuffer,
     UINT uSize
    );
__declspec(dllimport)
UINT
__stdcall
GetWindowsDirectoryW(
     LPWSTR lpBuffer,
     UINT uSize
    );




#line 6238 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
UINT
__stdcall
GetSystemWindowsDirectoryA(
     LPSTR lpBuffer,
     UINT uSize
    );
__declspec(dllimport)
UINT
__stdcall
GetSystemWindowsDirectoryW(
     LPWSTR lpBuffer,
     UINT uSize
    );




#line 6258 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"




















































#line 6311 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"
#line 6312 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetCurrentDirectoryA(
     LPCSTR lpPathName
    );
__declspec(dllimport)
BOOL
__stdcall
SetCurrentDirectoryW(
     LPCWSTR lpPathName
    );




#line 6330 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetCurrentDirectoryA(
     DWORD nBufferLength,
     LPSTR lpBuffer
    );
__declspec(dllimport)
DWORD
__stdcall
GetCurrentDirectoryW(
     DWORD nBufferLength,
     LPWSTR lpBuffer
    );




#line 6350 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"









































#line 6392 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetDiskFreeSpaceA(
     LPCSTR lpRootPathName,
     LPDWORD lpSectorsPerCluster,
     LPDWORD lpBytesPerSector,
     LPDWORD lpNumberOfFreeClusters,
     LPDWORD lpTotalNumberOfClusters
    );
__declspec(dllimport)
BOOL
__stdcall
GetDiskFreeSpaceW(
     LPCWSTR lpRootPathName,
     LPDWORD lpSectorsPerCluster,
     LPDWORD lpBytesPerSector,
     LPDWORD lpNumberOfFreeClusters,
     LPDWORD lpTotalNumberOfClusters
    );




#line 6418 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetDiskFreeSpaceExA(
     LPCSTR lpDirectoryName,
     PULARGE_INTEGER lpFreeBytesAvailableToCaller,
     PULARGE_INTEGER lpTotalNumberOfBytes,
     PULARGE_INTEGER lpTotalNumberOfFreeBytes
    );
__declspec(dllimport)
BOOL
__stdcall
GetDiskFreeSpaceExW(
     LPCWSTR lpDirectoryName,
     PULARGE_INTEGER lpFreeBytesAvailableToCaller,
     PULARGE_INTEGER lpTotalNumberOfBytes,
     PULARGE_INTEGER lpTotalNumberOfFreeBytes
    );




#line 6442 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
CreateDirectoryA(
     LPCSTR lpPathName,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );
__declspec(dllimport)
BOOL
__stdcall
CreateDirectoryW(
     LPCWSTR lpPathName,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );




#line 6462 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
CreateDirectoryExA(
     LPCSTR lpTemplateDirectory,
     LPCSTR lpNewDirectory,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );
__declspec(dllimport)
BOOL
__stdcall
CreateDirectoryExW(
     LPCWSTR lpTemplateDirectory,
     LPCWSTR lpNewDirectory,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );




#line 6484 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
RemoveDirectoryA(
     LPCSTR lpPathName
    );
__declspec(dllimport)
BOOL
__stdcall
RemoveDirectoryW(
     LPCWSTR lpPathName
    );




#line 6502 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetFullPathNameA(
     LPCSTR lpFileName,
     DWORD nBufferLength,
     LPSTR lpBuffer,
     LPSTR *lpFilePart
    );
__declspec(dllimport)
DWORD
__stdcall
GetFullPathNameW(
     LPCWSTR lpFileName,
     DWORD nBufferLength,
     LPWSTR lpBuffer,
     LPWSTR *lpFilePart
    );




#line 6526 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"








__declspec(dllimport)
BOOL
__stdcall
DefineDosDeviceA(
     DWORD dwFlags,
     LPCSTR lpDeviceName,
     LPCSTR lpTargetPath
    );
__declspec(dllimport)
BOOL
__stdcall
DefineDosDeviceW(
     DWORD dwFlags,
     LPCWSTR lpDeviceName,
     LPCWSTR lpTargetPath
    );




#line 6555 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
QueryDosDeviceA(
     LPCSTR lpDeviceName,
     LPSTR lpTargetPath,
     DWORD ucchMax
    );
__declspec(dllimport)
DWORD
__stdcall
QueryDosDeviceW(
     LPCWSTR lpDeviceName,
     LPWSTR lpTargetPath,
     DWORD ucchMax
    );




#line 6577 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



__declspec(dllimport)
HANDLE
__stdcall
CreateFileA(
     LPCSTR lpFileName,
     DWORD dwDesiredAccess,
     DWORD dwShareMode,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes,
     DWORD dwCreationDisposition,
     DWORD dwFlagsAndAttributes,
     HANDLE hTemplateFile
    );
__declspec(dllimport)
HANDLE
__stdcall
CreateFileW(
     LPCWSTR lpFileName,
     DWORD dwDesiredAccess,
     DWORD dwShareMode,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes,
     DWORD dwCreationDisposition,
     DWORD dwFlagsAndAttributes,
     HANDLE hTemplateFile
    );




#line 6609 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"













#line 6623 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetFileAttributesA(
     LPCSTR lpFileName,
     DWORD dwFileAttributes
    );
__declspec(dllimport)
BOOL
__stdcall
SetFileAttributesW(
     LPCWSTR lpFileName,
     DWORD dwFileAttributes
    );




#line 6643 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetFileAttributesA(
     LPCSTR lpFileName
    );
__declspec(dllimport)
DWORD
__stdcall
GetFileAttributesW(
     LPCWSTR lpFileName
    );




#line 6661 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

typedef enum _GET_FILEEX_INFO_LEVELS {
    GetFileExInfoStandard,
    GetFileExMaxInfoLevel
} GET_FILEEX_INFO_LEVELS;

__declspec(dllimport)
BOOL
__stdcall
GetFileAttributesExA(
     LPCSTR lpFileName,
     GET_FILEEX_INFO_LEVELS fInfoLevelId,
     LPVOID lpFileInformation
    );
__declspec(dllimport)
BOOL
__stdcall
GetFileAttributesExW(
     LPCWSTR lpFileName,
     GET_FILEEX_INFO_LEVELS fInfoLevelId,
     LPVOID lpFileInformation
    );




#line 6688 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
DWORD
__stdcall
GetCompressedFileSizeA(
     LPCSTR lpFileName,
     LPDWORD lpFileSizeHigh
    );
__declspec(dllimport)
DWORD
__stdcall
GetCompressedFileSizeW(
     LPCWSTR lpFileName,
     LPDWORD lpFileSizeHigh
    );




#line 6708 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
DeleteFileA(
     LPCSTR lpFileName
    );
__declspec(dllimport)
BOOL
__stdcall
DeleteFileW(
     LPCWSTR lpFileName
    );




#line 6726 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





























#line 6756 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"











































#line 6800 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
FindFirstFileA(
     LPCSTR lpFileName,
     LPWIN32_FIND_DATAA lpFindFileData
    );
__declspec(dllimport)
HANDLE
__stdcall
FindFirstFileW(
     LPCWSTR lpFileName,
     LPWIN32_FIND_DATAW lpFindFileData
    );




#line 6820 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
FindNextFileA(
     HANDLE hFindFile,
     LPWIN32_FIND_DATAA lpFindFileData
    );
__declspec(dllimport)
BOOL
__stdcall
FindNextFileW(
     HANDLE hFindFile,
     LPWIN32_FIND_DATAW lpFindFileData
    );




#line 6840 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
DWORD
__stdcall
SearchPathA(
     LPCSTR lpPath,
     LPCSTR lpFileName,
     LPCSTR lpExtension,
     DWORD nBufferLength,
     LPSTR lpBuffer,
     LPSTR *lpFilePart
    );
__declspec(dllimport)
DWORD
__stdcall
SearchPathW(
     LPCWSTR lpPath,
     LPCWSTR lpFileName,
     LPCWSTR lpExtension,
     DWORD nBufferLength,
     LPWSTR lpBuffer,
     LPWSTR *lpFilePart
    );




#line 6869 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
CopyFileA(
     LPCSTR lpExistingFileName,
     LPCSTR lpNewFileName,
     BOOL bFailIfExists
    );
__declspec(dllimport)
BOOL
__stdcall
CopyFileW(
     LPCWSTR lpExistingFileName,
     LPCWSTR lpNewFileName,
     BOOL bFailIfExists
    );




#line 6891 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"











































#line 6935 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
MoveFileA(
     LPCSTR lpExistingFileName,
     LPCSTR lpNewFileName
    );
__declspec(dllimport)
BOOL
__stdcall
MoveFileW(
     LPCWSTR lpExistingFileName,
     LPCWSTR lpNewFileName
    );




#line 6955 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
MoveFileExA(
     LPCSTR lpExistingFileName,
     LPCSTR lpNewFileName,
     DWORD dwFlags
    );
__declspec(dllimport)
BOOL
__stdcall
MoveFileExW(
     LPCWSTR lpExistingFileName,
     LPCWSTR lpNewFileName,
     DWORD dwFlags
    );




#line 6977 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



























#line 7005 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"








#line 7014 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"































#line 7046 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





























#line 7076 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"







































#line 7116 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



__declspec(dllimport)
HANDLE
__stdcall
CreateNamedPipeA(
     LPCSTR lpName,
     DWORD dwOpenMode,
     DWORD dwPipeMode,
     DWORD nMaxInstances,
     DWORD nOutBufferSize,
     DWORD nInBufferSize,
     DWORD nDefaultTimeOut,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );
__declspec(dllimport)
HANDLE
__stdcall
CreateNamedPipeW(
     LPCWSTR lpName,
     DWORD dwOpenMode,
     DWORD dwPipeMode,
     DWORD nMaxInstances,
     DWORD nOutBufferSize,
     DWORD nInBufferSize,
     DWORD nDefaultTimeOut,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );




#line 7150 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetNamedPipeHandleStateA(
     HANDLE hNamedPipe,
     LPDWORD lpState,
     LPDWORD lpCurInstances,
     LPDWORD lpMaxCollectionCount,
     LPDWORD lpCollectDataTimeout,
     LPSTR lpUserName,
     DWORD nMaxUserNameSize
    );
__declspec(dllimport)
BOOL
__stdcall
GetNamedPipeHandleStateW(
     HANDLE hNamedPipe,
     LPDWORD lpState,
     LPDWORD lpCurInstances,
     LPDWORD lpMaxCollectionCount,
     LPDWORD lpCollectDataTimeout,
     LPWSTR lpUserName,
     DWORD nMaxUserNameSize
    );




#line 7180 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
CallNamedPipeA(
     LPCSTR lpNamedPipeName,
     LPVOID lpInBuffer,
     DWORD nInBufferSize,
     LPVOID lpOutBuffer,
     DWORD nOutBufferSize,
     LPDWORD lpBytesRead,
     DWORD nTimeOut
    );
__declspec(dllimport)
BOOL
__stdcall
CallNamedPipeW(
     LPCWSTR lpNamedPipeName,
     LPVOID lpInBuffer,
     DWORD nInBufferSize,
     LPVOID lpOutBuffer,
     DWORD nOutBufferSize,
     LPDWORD lpBytesRead,
     DWORD nTimeOut
    );




#line 7210 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
WaitNamedPipeA(
     LPCSTR lpNamedPipeName,
     DWORD nTimeOut
    );
__declspec(dllimport)
BOOL
__stdcall
WaitNamedPipeW(
     LPCWSTR lpNamedPipeName,
     DWORD nTimeOut
    );




#line 7230 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetVolumeLabelA(
     LPCSTR lpRootPathName,
     LPCSTR lpVolumeName
    );
__declspec(dllimport)
BOOL
__stdcall
SetVolumeLabelW(
     LPCWSTR lpRootPathName,
     LPCWSTR lpVolumeName
    );




#line 7250 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
void
__stdcall
SetFileApisToOEM( void );

__declspec(dllimport)
void
__stdcall
SetFileApisToANSI( void );

__declspec(dllimport)
BOOL
__stdcall
AreFileApisANSI( void );

__declspec(dllimport)
BOOL
__stdcall
GetVolumeInformationA(
     LPCSTR lpRootPathName,
     LPSTR lpVolumeNameBuffer,
     DWORD nVolumeNameSize,
     LPDWORD lpVolumeSerialNumber,
     LPDWORD lpMaximumComponentLength,
     LPDWORD lpFileSystemFlags,
     LPSTR lpFileSystemNameBuffer,
     DWORD nFileSystemNameSize
    );
__declspec(dllimport)
BOOL
__stdcall
GetVolumeInformationW(
     LPCWSTR lpRootPathName,
     LPWSTR lpVolumeNameBuffer,
     DWORD nVolumeNameSize,
     LPDWORD lpVolumeSerialNumber,
     LPDWORD lpMaximumComponentLength,
     LPDWORD lpFileSystemFlags,
     LPWSTR lpFileSystemNameBuffer,
     DWORD nFileSystemNameSize
    );




#line 7297 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
CancelIo(
     HANDLE hFile
    );





__declspec(dllimport)
BOOL
__stdcall
ClearEventLogA (
     HANDLE hEventLog,
     LPCSTR lpBackupFileName
    );
__declspec(dllimport)
BOOL
__stdcall
ClearEventLogW (
     HANDLE hEventLog,
     LPCWSTR lpBackupFileName
    );




#line 7328 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
BackupEventLogA (
     HANDLE hEventLog,
     LPCSTR lpBackupFileName
    );
__declspec(dllimport)
BOOL
__stdcall
BackupEventLogW (
     HANDLE hEventLog,
     LPCWSTR lpBackupFileName
    );




#line 7348 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
CloseEventLog (
      HANDLE hEventLog
    );

__declspec(dllimport)
BOOL
__stdcall
DeregisterEventSource (
      HANDLE hEventLog
    );

__declspec(dllimport)
BOOL
__stdcall
NotifyChangeEventLog(
     HANDLE  hEventLog,
     HANDLE  hEvent
    );

__declspec(dllimport)
BOOL
__stdcall
GetNumberOfEventLogRecords (
     HANDLE hEventLog,
     PDWORD NumberOfRecords
    );

__declspec(dllimport)
BOOL
__stdcall
GetOldestEventLogRecord (
     HANDLE hEventLog,
     PDWORD OldestRecord
    );

__declspec(dllimport)
HANDLE
__stdcall
OpenEventLogA (
     LPCSTR lpUNCServerName,
     LPCSTR lpSourceName
    );
__declspec(dllimport)
HANDLE
__stdcall
OpenEventLogW (
     LPCWSTR lpUNCServerName,
     LPCWSTR lpSourceName
    );




#line 7406 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
RegisterEventSourceA (
     LPCSTR lpUNCServerName,
     LPCSTR lpSourceName
    );
__declspec(dllimport)
HANDLE
__stdcall
RegisterEventSourceW (
     LPCWSTR lpUNCServerName,
     LPCWSTR lpSourceName
    );




#line 7426 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
HANDLE
__stdcall
OpenBackupEventLogA (
     LPCSTR lpUNCServerName,
     LPCSTR lpFileName
    );
__declspec(dllimport)
HANDLE
__stdcall
OpenBackupEventLogW (
     LPCWSTR lpUNCServerName,
     LPCWSTR lpFileName
    );




#line 7446 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
ReadEventLogA (
      HANDLE     hEventLog,
      DWORD      dwReadFlags,
      DWORD      dwRecordOffset,
      LPVOID     lpBuffer,
      DWORD      nNumberOfBytesToRead,
      DWORD      *pnBytesRead,
      DWORD      *pnMinNumberOfBytesNeeded
    );
__declspec(dllimport)
BOOL
__stdcall
ReadEventLogW (
      HANDLE     hEventLog,
      DWORD      dwReadFlags,
      DWORD      dwRecordOffset,
      LPVOID     lpBuffer,
      DWORD      nNumberOfBytesToRead,
      DWORD      *pnBytesRead,
      DWORD      *pnMinNumberOfBytesNeeded
    );




#line 7476 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
ReportEventA (
      HANDLE     hEventLog,
      WORD       wType,
      WORD       wCategory,
      DWORD      dwEventID,
      PSID       lpUserSid,
      WORD       wNumStrings,
      DWORD      dwDataSize,
      LPCSTR   *lpStrings,
      LPVOID     lpRawData
    );
__declspec(dllimport)
BOOL
__stdcall
ReportEventW (
      HANDLE     hEventLog,
      WORD       wType,
      WORD       wCategory,
      DWORD      dwEventID,
      PSID       lpUserSid,
      WORD       wNumStrings,
      DWORD      dwDataSize,
      LPCWSTR   *lpStrings,
      LPVOID     lpRawData
    );




#line 7510 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"




typedef struct _EVENTLOG_FULL_INFORMATION
{
    DWORD    dwFull;
}
EVENTLOG_FULL_INFORMATION, *LPEVENTLOG_FULL_INFORMATION;

__declspec(dllimport)
BOOL
__stdcall
GetEventLogInformation (
       HANDLE     hEventLog,
       DWORD      dwInfoLevel,
      LPVOID     lpBuffer,
       DWORD      cbBufSize,
      LPDWORD    pcbBytesNeeded
    );







__declspec(dllimport)
BOOL
__stdcall
DuplicateToken(
     HANDLE ExistingTokenHandle,
     SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
     PHANDLE DuplicateTokenHandle
    );

__declspec(dllimport)
BOOL
__stdcall
GetKernelObjectSecurity (
     HANDLE Handle,
     SECURITY_INFORMATION RequestedInformation,
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     DWORD nLength,
     LPDWORD lpnLengthNeeded
    );

__declspec(dllimport)
BOOL
__stdcall
ImpersonateNamedPipeClient(
     HANDLE hNamedPipe
    );

__declspec(dllimport)
BOOL
__stdcall
ImpersonateSelf(
     SECURITY_IMPERSONATION_LEVEL ImpersonationLevel
    );


__declspec(dllimport)
BOOL
__stdcall
RevertToSelf (
    void
    );

__declspec(dllimport)
BOOL
__stdcall
SetThreadToken (
     PHANDLE Thread,
     HANDLE Token
    );

__declspec(dllimport)
BOOL
__stdcall
AccessCheck (
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     HANDLE ClientToken,
     DWORD DesiredAccess,
     PGENERIC_MAPPING GenericMapping,
     PPRIVILEGE_SET PrivilegeSet,
     LPDWORD PrivilegeSetLength,
     LPDWORD GrantedAccess,
     LPBOOL AccessStatus
    );



































#line 7636 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
OpenProcessToken (
     HANDLE ProcessHandle,
     DWORD DesiredAccess,
     PHANDLE TokenHandle
    );


__declspec(dllimport)
BOOL
__stdcall
OpenThreadToken (
     HANDLE ThreadHandle,
     DWORD DesiredAccess,
     BOOL OpenAsSelf,
     PHANDLE TokenHandle
    );


__declspec(dllimport)
BOOL
__stdcall
GetTokenInformation (
     HANDLE TokenHandle,
     TOKEN_INFORMATION_CLASS TokenInformationClass,
     LPVOID TokenInformation,
     DWORD TokenInformationLength,
     PDWORD ReturnLength
    );


__declspec(dllimport)
BOOL
__stdcall
SetTokenInformation (
     HANDLE TokenHandle,
     TOKEN_INFORMATION_CLASS TokenInformationClass,
     LPVOID TokenInformation,
     DWORD TokenInformationLength
    );


__declspec(dllimport)
BOOL
__stdcall
AdjustTokenPrivileges (
     HANDLE TokenHandle,
     BOOL DisableAllPrivileges,
     PTOKEN_PRIVILEGES NewState,
     DWORD BufferLength,
     PTOKEN_PRIVILEGES PreviousState,
     PDWORD ReturnLength
    );


__declspec(dllimport)
BOOL
__stdcall
AdjustTokenGroups (
     HANDLE TokenHandle,
     BOOL ResetToDefault,
     PTOKEN_GROUPS NewState,
     DWORD BufferLength,
     PTOKEN_GROUPS PreviousState,
     PDWORD ReturnLength
    );


__declspec(dllimport)
BOOL
__stdcall
PrivilegeCheck (
     HANDLE ClientToken,
     PPRIVILEGE_SET RequiredPrivileges,
     LPBOOL pfResult
    );


__declspec(dllimport)
BOOL
__stdcall
AccessCheckAndAuditAlarmA (
     LPCSTR SubsystemName,
     LPVOID HandleId,
     LPSTR ObjectTypeName,
     LPSTR ObjectName,
     PSECURITY_DESCRIPTOR SecurityDescriptor,
     DWORD DesiredAccess,
     PGENERIC_MAPPING GenericMapping,
     BOOL ObjectCreation,
     LPDWORD GrantedAccess,
     LPBOOL AccessStatus,
     LPBOOL pfGenerateOnClose
    );
__declspec(dllimport)
BOOL
__stdcall
AccessCheckAndAuditAlarmW (
     LPCWSTR SubsystemName,
     LPVOID HandleId,
     LPWSTR ObjectTypeName,
     LPWSTR ObjectName,
     PSECURITY_DESCRIPTOR SecurityDescriptor,
     DWORD DesiredAccess,
     PGENERIC_MAPPING GenericMapping,
     BOOL ObjectCreation,
     LPDWORD GrantedAccess,
     LPBOOL AccessStatus,
     LPBOOL pfGenerateOnClose
    );




#line 7755 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





















































































































































#line 7905 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
ObjectOpenAuditAlarmA (
     LPCSTR SubsystemName,
     LPVOID HandleId,
     LPSTR ObjectTypeName,
     LPSTR ObjectName,
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     HANDLE ClientToken,
     DWORD DesiredAccess,
     DWORD GrantedAccess,
     PPRIVILEGE_SET Privileges,
     BOOL ObjectCreation,
     BOOL AccessGranted,
     LPBOOL GenerateOnClose
    );
__declspec(dllimport)
BOOL
__stdcall
ObjectOpenAuditAlarmW (
     LPCWSTR SubsystemName,
     LPVOID HandleId,
     LPWSTR ObjectTypeName,
     LPWSTR ObjectName,
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     HANDLE ClientToken,
     DWORD DesiredAccess,
     DWORD GrantedAccess,
     PPRIVILEGE_SET Privileges,
     BOOL ObjectCreation,
     BOOL AccessGranted,
     LPBOOL GenerateOnClose
    );




#line 7946 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
ObjectPrivilegeAuditAlarmA (
     LPCSTR SubsystemName,
     LPVOID HandleId,
     HANDLE ClientToken,
     DWORD DesiredAccess,
     PPRIVILEGE_SET Privileges,
     BOOL AccessGranted
    );
__declspec(dllimport)
BOOL
__stdcall
ObjectPrivilegeAuditAlarmW (
     LPCWSTR SubsystemName,
     LPVOID HandleId,
     HANDLE ClientToken,
     DWORD DesiredAccess,
     PPRIVILEGE_SET Privileges,
     BOOL AccessGranted
    );




#line 7975 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
ObjectCloseAuditAlarmA (
     LPCSTR SubsystemName,
     LPVOID HandleId,
     BOOL GenerateOnClose
    );
__declspec(dllimport)
BOOL
__stdcall
ObjectCloseAuditAlarmW (
     LPCWSTR SubsystemName,
     LPVOID HandleId,
     BOOL GenerateOnClose
    );




#line 7998 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
ObjectDeleteAuditAlarmA (
     LPCSTR SubsystemName,
     LPVOID HandleId,
     BOOL GenerateOnClose
    );
__declspec(dllimport)
BOOL
__stdcall
ObjectDeleteAuditAlarmW (
     LPCWSTR SubsystemName,
     LPVOID HandleId,
     BOOL GenerateOnClose
    );




#line 8021 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
PrivilegedServiceAuditAlarmA (
     LPCSTR SubsystemName,
     LPCSTR ServiceName,
     HANDLE ClientToken,
     PPRIVILEGE_SET Privileges,
     BOOL AccessGranted
    );
__declspec(dllimport)
BOOL
__stdcall
PrivilegedServiceAuditAlarmW (
     LPCWSTR SubsystemName,
     LPCWSTR ServiceName,
     HANDLE ClientToken,
     PPRIVILEGE_SET Privileges,
     BOOL AccessGranted
    );




#line 8048 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"










































#line 8091 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
IsValidSid (
     PSID pSid
    );


__declspec(dllimport)
BOOL
__stdcall
EqualSid (
     PSID pSid1,
     PSID pSid2
    );


__declspec(dllimport)
BOOL
__stdcall
EqualPrefixSid (
    PSID pSid1,
    PSID pSid2
    );


__declspec(dllimport)
DWORD
__stdcall
GetSidLengthRequired (
     UCHAR nSubAuthorityCount
    );


__declspec(dllimport)
BOOL
__stdcall
AllocateAndInitializeSid (
     PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority,
     BYTE nSubAuthorityCount,
     DWORD nSubAuthority0,
     DWORD nSubAuthority1,
     DWORD nSubAuthority2,
     DWORD nSubAuthority3,
     DWORD nSubAuthority4,
     DWORD nSubAuthority5,
     DWORD nSubAuthority6,
     DWORD nSubAuthority7,
     PSID *pSid
    );

__declspec(dllimport)
PVOID
__stdcall
FreeSid(
     PSID pSid
    );

__declspec(dllimport)
BOOL
__stdcall
InitializeSid (
     PSID Sid,
     PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority,
     BYTE nSubAuthorityCount
    );


__declspec(dllimport)
PSID_IDENTIFIER_AUTHORITY
__stdcall
GetSidIdentifierAuthority (
     PSID pSid
    );


__declspec(dllimport)
PDWORD
__stdcall
GetSidSubAuthority (
     PSID pSid,
     DWORD nSubAuthority
    );


__declspec(dllimport)
PUCHAR
__stdcall
GetSidSubAuthorityCount (
     PSID pSid
    );


__declspec(dllimport)
DWORD
__stdcall
GetLengthSid (
     PSID pSid
    );


__declspec(dllimport)
BOOL
__stdcall
CopySid (
     DWORD nDestinationSidLength,
     PSID pDestinationSid,
     PSID pSourceSid
    );


__declspec(dllimport)
BOOL
__stdcall
AreAllAccessesGranted (
     DWORD GrantedAccess,
     DWORD DesiredAccess
    );


__declspec(dllimport)
BOOL
__stdcall
AreAnyAccessesGranted (
     DWORD GrantedAccess,
     DWORD DesiredAccess
    );


__declspec(dllimport)
void
__stdcall
MapGenericMask (
     PDWORD AccessMask,
     PGENERIC_MAPPING GenericMapping
    );


__declspec(dllimport)
BOOL
__stdcall
IsValidAcl (
     PACL pAcl
    );


__declspec(dllimport)
BOOL
__stdcall
InitializeAcl (
     PACL pAcl,
     DWORD nAclLength,
     DWORD dwAclRevision
    );


__declspec(dllimport)
BOOL
__stdcall
GetAclInformation (
     PACL pAcl,
     LPVOID pAclInformation,
     DWORD nAclInformationLength,
     ACL_INFORMATION_CLASS dwAclInformationClass
    );


__declspec(dllimport)
BOOL
__stdcall
SetAclInformation (
     PACL pAcl,
     LPVOID pAclInformation,
     DWORD nAclInformationLength,
     ACL_INFORMATION_CLASS dwAclInformationClass
    );


__declspec(dllimport)
BOOL
__stdcall
AddAce (
      PACL pAcl,
     DWORD dwAceRevision,
     DWORD dwStartingAceIndex,
     LPVOID pAceList,
     DWORD nAceListLength
    );


__declspec(dllimport)
BOOL
__stdcall
DeleteAce (
      PACL pAcl,
     DWORD dwAceIndex
    );


__declspec(dllimport)
BOOL
__stdcall
GetAce (
     PACL pAcl,
     DWORD dwAceIndex,
     LPVOID *pAce
    );


__declspec(dllimport)
BOOL
__stdcall
AddAccessAllowedAce (
      PACL pAcl,
     DWORD dwAceRevision,
     DWORD AccessMask,
     PSID pSid
    );












#line 8323 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
AddAccessDeniedAce (
      PACL pAcl,
     DWORD dwAceRevision,
     DWORD AccessMask,
     PSID pSid
    );












#line 8347 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
AddAuditAccessAce(
      PACL pAcl,
     DWORD dwAceRevision,
     DWORD dwAccessMask,
     PSID pSid,
     BOOL bAuditSuccess,
     BOOL bAuditFailure
    );























































#line 8415 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
FindFirstFreeAce (
     PACL pAcl,
     LPVOID *pAce
    );


__declspec(dllimport)
BOOL
__stdcall
InitializeSecurityDescriptor (
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     DWORD dwRevision
    );


__declspec(dllimport)
BOOL
__stdcall
IsValidSecurityDescriptor (
     PSECURITY_DESCRIPTOR pSecurityDescriptor
    );


__declspec(dllimport)
DWORD
__stdcall
GetSecurityDescriptorLength (
     PSECURITY_DESCRIPTOR pSecurityDescriptor
    );


__declspec(dllimport)
BOOL
__stdcall
GetSecurityDescriptorControl (
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     PSECURITY_DESCRIPTOR_CONTROL pControl,
     LPDWORD lpdwRevision
    );










#line 8469 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetSecurityDescriptorDacl (
      PSECURITY_DESCRIPTOR pSecurityDescriptor,
     BOOL bDaclPresent,
     PACL pDacl,
     BOOL bDaclDefaulted
    );


__declspec(dllimport)
BOOL
__stdcall
GetSecurityDescriptorDacl (
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     LPBOOL lpbDaclPresent,
     PACL *pDacl,
     LPBOOL lpbDaclDefaulted
    );


__declspec(dllimport)
BOOL
__stdcall
SetSecurityDescriptorSacl (
      PSECURITY_DESCRIPTOR pSecurityDescriptor,
     BOOL bSaclPresent,
     PACL pSacl,
     BOOL bSaclDefaulted
    );


__declspec(dllimport)
BOOL
__stdcall
GetSecurityDescriptorSacl (
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     LPBOOL lpbSaclPresent,
     PACL *pSacl,
     LPBOOL lpbSaclDefaulted
    );


__declspec(dllimport)
BOOL
__stdcall
SetSecurityDescriptorOwner (
      PSECURITY_DESCRIPTOR pSecurityDescriptor,
     PSID pOwner,
     BOOL bOwnerDefaulted
    );


__declspec(dllimport)
BOOL
__stdcall
GetSecurityDescriptorOwner (
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     PSID *pOwner,
     LPBOOL lpbOwnerDefaulted
    );


__declspec(dllimport)
BOOL
__stdcall
SetSecurityDescriptorGroup (
      PSECURITY_DESCRIPTOR pSecurityDescriptor,
     PSID pGroup,
     BOOL bGroupDefaulted
    );


__declspec(dllimport)
BOOL
__stdcall
GetSecurityDescriptorGroup (
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     PSID *pGroup,
     LPBOOL lpbGroupDefaulted
    );


__declspec(dllimport)
DWORD
__stdcall
SetSecurityDescriptorRMControl(
      PSECURITY_DESCRIPTOR SecurityDescriptor,
     PUCHAR RMControl 
    );

__declspec(dllimport)
DWORD
__stdcall
GetSecurityDescriptorRMControl(
     PSECURITY_DESCRIPTOR SecurityDescriptor,
     PUCHAR RMControl
    );

__declspec(dllimport)
BOOL
__stdcall
CreatePrivateObjectSecurity (
     PSECURITY_DESCRIPTOR ParentDescriptor,
     PSECURITY_DESCRIPTOR CreatorDescriptor,
     PSECURITY_DESCRIPTOR * NewDescriptor,
     BOOL IsDirectoryObject,
     HANDLE Token,
     PGENERIC_MAPPING GenericMapping
    );










































#line 8624 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetPrivateObjectSecurity (
     SECURITY_INFORMATION SecurityInformation,
     PSECURITY_DESCRIPTOR ModificationDescriptor,
     PSECURITY_DESCRIPTOR *ObjectsSecurityDescriptor,
     PGENERIC_MAPPING GenericMapping,
     HANDLE Token
    );













#line 8649 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetPrivateObjectSecurity (
     PSECURITY_DESCRIPTOR ObjectDescriptor,
     SECURITY_INFORMATION SecurityInformation,
     PSECURITY_DESCRIPTOR ResultantDescriptor,
     DWORD DescriptorLength,
     PDWORD ReturnLength
    );


__declspec(dllimport)
BOOL
__stdcall
DestroyPrivateObjectSecurity (
      PSECURITY_DESCRIPTOR * ObjectDescriptor
    );


__declspec(dllimport)
BOOL
__stdcall
MakeSelfRelativeSD (
     PSECURITY_DESCRIPTOR pAbsoluteSecurityDescriptor,
     PSECURITY_DESCRIPTOR pSelfRelativeSecurityDescriptor,
     LPDWORD lpdwBufferLength
    );


__declspec(dllimport)
BOOL
__stdcall
MakeAbsoluteSD (
     PSECURITY_DESCRIPTOR pSelfRelativeSecurityDescriptor,
     PSECURITY_DESCRIPTOR pAbsoluteSecurityDescriptor,
     LPDWORD lpdwAbsoluteSecurityDescriptorSize,
     PACL pDacl,
     LPDWORD lpdwDaclSize,
     PACL pSacl,
     LPDWORD lpdwSaclSize,
     PSID pOwner,
     LPDWORD lpdwOwnerSize,
     PSID pPrimaryGroup,
     LPDWORD lpdwPrimaryGroupSize
    );


__declspec(dllimport)
BOOL
__stdcall
MakeAbsoluteSD2 (
     PSECURITY_DESCRIPTOR pSelfRelativeSecurityDescriptor,
     LPDWORD lpdwBufferSize
    );

__declspec(dllimport)
BOOL
__stdcall
SetFileSecurityA (
     LPCSTR lpFileName,
     SECURITY_INFORMATION SecurityInformation,
     PSECURITY_DESCRIPTOR pSecurityDescriptor
    );
__declspec(dllimport)
BOOL
__stdcall
SetFileSecurityW (
     LPCWSTR lpFileName,
     SECURITY_INFORMATION SecurityInformation,
     PSECURITY_DESCRIPTOR pSecurityDescriptor
    );




#line 8727 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
GetFileSecurityA (
     LPCSTR lpFileName,
     SECURITY_INFORMATION RequestedInformation,
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     DWORD nLength,
     LPDWORD lpnLengthNeeded
    );
__declspec(dllimport)
BOOL
__stdcall
GetFileSecurityW (
     LPCWSTR lpFileName,
     SECURITY_INFORMATION RequestedInformation,
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
     DWORD nLength,
     LPDWORD lpnLengthNeeded
    );




#line 8754 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
SetKernelObjectSecurity (
     HANDLE Handle,
     SECURITY_INFORMATION SecurityInformation,
     PSECURITY_DESCRIPTOR SecurityDescriptor
    );

__declspec(dllimport)
HANDLE
__stdcall
FindFirstChangeNotificationA(
     LPCSTR lpPathName,
     BOOL bWatchSubtree,
     DWORD dwNotifyFilter
    );
__declspec(dllimport)
HANDLE
__stdcall
FindFirstChangeNotificationW(
     LPCWSTR lpPathName,
     BOOL bWatchSubtree,
     DWORD dwNotifyFilter
    );




#line 8786 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
FindNextChangeNotification(
     HANDLE hChangeHandle
    );

__declspec(dllimport)
BOOL
__stdcall
FindCloseChangeNotification(
     HANDLE hChangeHandle
    );















#line 8816 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
VirtualLock(
     LPVOID lpAddress,
     SIZE_T dwSize
    );

__declspec(dllimport)
BOOL
__stdcall
VirtualUnlock(
     LPVOID lpAddress,
     SIZE_T dwSize
    );

__declspec(dllimport)
LPVOID
__stdcall
MapViewOfFileEx(
     HANDLE hFileMappingObject,
     DWORD dwDesiredAccess,
     DWORD dwFileOffsetHigh,
     DWORD dwFileOffsetLow,
     SIZE_T dwNumberOfBytesToMap,
     LPVOID lpBaseAddress
    );

__declspec(dllimport)
BOOL
__stdcall
SetPriorityClass(
     HANDLE hProcess,
     DWORD dwPriorityClass
    );

__declspec(dllimport)
DWORD
__stdcall
GetPriorityClass(
     HANDLE hProcess
    );

__declspec(dllimport)
BOOL
__stdcall
IsBadReadPtr(
     const void *lp,
     UINT_PTR ucb
    );

__declspec(dllimport)
BOOL
__stdcall
IsBadWritePtr(
     LPVOID lp,
     UINT_PTR ucb
    );

__declspec(dllimport)
BOOL
__stdcall
IsBadHugeReadPtr(
     const void *lp,
     UINT_PTR ucb
    );

__declspec(dllimport)
BOOL
__stdcall
IsBadHugeWritePtr(
     LPVOID lp,
     UINT_PTR ucb
    );

__declspec(dllimport)
BOOL
__stdcall
IsBadCodePtr(
     FARPROC lpfn
    );

__declspec(dllimport)
BOOL
__stdcall
IsBadStringPtrA(
     LPCSTR lpsz,
     UINT_PTR ucchMax
    );
__declspec(dllimport)
BOOL
__stdcall
IsBadStringPtrW(
     LPCWSTR lpsz,
     UINT_PTR ucchMax
    );




#line 8918 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
LookupAccountSidA(
     LPCSTR lpSystemName,
     PSID Sid,
     LPSTR Name,
      LPDWORD cbName,
     LPSTR ReferencedDomainName,
      LPDWORD cbReferencedDomainName,
     PSID_NAME_USE peUse
    );
__declspec(dllimport)
BOOL
__stdcall
LookupAccountSidW(
     LPCWSTR lpSystemName,
     PSID Sid,
     LPWSTR Name,
      LPDWORD cbName,
     LPWSTR ReferencedDomainName,
      LPDWORD cbReferencedDomainName,
     PSID_NAME_USE peUse
    );




#line 8948 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
LookupAccountNameA(
     LPCSTR lpSystemName,
     LPCSTR lpAccountName,
     PSID Sid,
      LPDWORD cbSid,
     LPSTR ReferencedDomainName,
      LPDWORD cbReferencedDomainName,
     PSID_NAME_USE peUse
    );
__declspec(dllimport)
BOOL
__stdcall
LookupAccountNameW(
     LPCWSTR lpSystemName,
     LPCWSTR lpAccountName,
     PSID Sid,
      LPDWORD cbSid,
     LPWSTR ReferencedDomainName,
      LPDWORD cbReferencedDomainName,
     PSID_NAME_USE peUse
    );




#line 8978 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
LookupPrivilegeValueA(
     LPCSTR lpSystemName,
     LPCSTR lpName,
     PLUID   lpLuid
    );
__declspec(dllimport)
BOOL
__stdcall
LookupPrivilegeValueW(
     LPCWSTR lpSystemName,
     LPCWSTR lpName,
     PLUID   lpLuid
    );




#line 9000 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
LookupPrivilegeNameA(
     LPCSTR lpSystemName,
     PLUID   lpLuid,
     LPSTR lpName,
      LPDWORD cbName
    );
__declspec(dllimport)
BOOL
__stdcall
LookupPrivilegeNameW(
     LPCWSTR lpSystemName,
     PLUID   lpLuid,
     LPWSTR lpName,
      LPDWORD cbName
    );




#line 9024 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
LookupPrivilegeDisplayNameA(
     LPCSTR lpSystemName,
     LPCSTR lpName,
     LPSTR lpDisplayName,
      LPDWORD cbDisplayName,
     LPDWORD lpLanguageId
    );
__declspec(dllimport)
BOOL
__stdcall
LookupPrivilegeDisplayNameW(
     LPCWSTR lpSystemName,
     LPCWSTR lpName,
     LPWSTR lpDisplayName,
      LPDWORD cbDisplayName,
     LPDWORD lpLanguageId
    );




#line 9050 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
AllocateLocallyUniqueId(
     PLUID Luid
    );

__declspec(dllimport)
BOOL
__stdcall
BuildCommDCBA(
     LPCSTR lpDef,
     LPDCB lpDCB
    );
__declspec(dllimport)
BOOL
__stdcall
BuildCommDCBW(
     LPCWSTR lpDef,
     LPDCB lpDCB
    );




#line 9077 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
BuildCommDCBAndTimeoutsA(
     LPCSTR lpDef,
     LPDCB lpDCB,
     LPCOMMTIMEOUTS lpCommTimeouts
    );
__declspec(dllimport)
BOOL
__stdcall
BuildCommDCBAndTimeoutsW(
     LPCWSTR lpDef,
     LPDCB lpDCB,
     LPCOMMTIMEOUTS lpCommTimeouts
    );




#line 9099 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
CommConfigDialogA(
     LPCSTR lpszName,
     HWND hWnd,
      LPCOMMCONFIG lpCC
    );
__declspec(dllimport)
BOOL
__stdcall
CommConfigDialogW(
     LPCWSTR lpszName,
     HWND hWnd,
      LPCOMMCONFIG lpCC
    );




#line 9121 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetDefaultCommConfigA(
     LPCSTR lpszName,
     LPCOMMCONFIG lpCC,
      LPDWORD lpdwSize
    );
__declspec(dllimport)
BOOL
__stdcall
GetDefaultCommConfigW(
     LPCWSTR lpszName,
     LPCOMMCONFIG lpCC,
      LPDWORD lpdwSize
    );




#line 9143 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetDefaultCommConfigA(
     LPCSTR lpszName,
     LPCOMMCONFIG lpCC,
     DWORD dwSize
    );
__declspec(dllimport)
BOOL
__stdcall
SetDefaultCommConfigW(
     LPCWSTR lpszName,
     LPCOMMCONFIG lpCC,
     DWORD dwSize
    );




#line 9165 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





#line 9171 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetComputerNameA (
     LPSTR lpBuffer,
      LPDWORD nSize
    );
__declspec(dllimport)
BOOL
__stdcall
GetComputerNameW (
     LPWSTR lpBuffer,
      LPDWORD nSize
    );




#line 9191 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
SetComputerNameA (
     LPCSTR lpComputerName
    );
__declspec(dllimport)
BOOL
__stdcall
SetComputerNameW (
     LPCWSTR lpComputerName
    );




#line 9209 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"











































































































































































#line 9381 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetUserNameA (
     LPSTR lpBuffer,
      LPDWORD nSize
    );
__declspec(dllimport)
BOOL
__stdcall
GetUserNameW (
     LPWSTR lpBuffer,
      LPDWORD nSize
    );




#line 9401 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"













#line 9415 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





#line 9421 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


#line 9424 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



__declspec(dllimport)
BOOL
__stdcall
LogonUserA (
     LPCSTR lpszUsername,
     LPCSTR lpszDomain,
     LPCSTR lpszPassword,
     DWORD dwLogonType,
     DWORD dwLogonProvider,
     PHANDLE phToken
    );
__declspec(dllimport)
BOOL
__stdcall
LogonUserW (
     LPCWSTR lpszUsername,
     LPCWSTR lpszDomain,
     LPCWSTR lpszPassword,
     DWORD dwLogonType,
     DWORD dwLogonProvider,
     PHANDLE phToken
    );




#line 9454 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
LogonUserExA (
     LPCSTR lpszUsername,
     LPCSTR lpszDomain,
     LPCSTR lpszPassword,
     DWORD dwLogonType,
     DWORD dwLogonProvider,
     PHANDLE phToken           ,
     PSID  *ppLogonSid       ,
     PVOID *ppProfileBuffer  ,
     LPDWORD pdwProfileLength  ,
     PQUOTA_LIMITS pQuotaLimits 
    );
__declspec(dllimport)
BOOL
__stdcall
LogonUserExW (
     LPCWSTR lpszUsername,
     LPCWSTR lpszDomain,
     LPCWSTR lpszPassword,
     DWORD dwLogonType,
     DWORD dwLogonProvider,
     PHANDLE phToken           ,
     PSID  *ppLogonSid       ,
     PVOID *ppProfileBuffer  ,
     LPDWORD pdwProfileLength  ,
     PQUOTA_LIMITS pQuotaLimits 
    );




#line 9490 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
ImpersonateLoggedOnUser(
     HANDLE  hToken
    );

__declspec(dllimport)
BOOL
__stdcall
CreateProcessAsUserA (
     HANDLE hToken,
     LPCSTR lpApplicationName,
     LPSTR lpCommandLine,
     LPSECURITY_ATTRIBUTES lpProcessAttributes,
     LPSECURITY_ATTRIBUTES lpThreadAttributes,
     BOOL bInheritHandles,
     DWORD dwCreationFlags,
     LPVOID lpEnvironment,
     LPCSTR lpCurrentDirectory,
     LPSTARTUPINFOA lpStartupInfo,
     LPPROCESS_INFORMATION lpProcessInformation
    );
__declspec(dllimport)
BOOL
__stdcall
CreateProcessAsUserW (
     HANDLE hToken,
     LPCWSTR lpApplicationName,
     LPWSTR lpCommandLine,
     LPSECURITY_ATTRIBUTES lpProcessAttributes,
     LPSECURITY_ATTRIBUTES lpThreadAttributes,
     BOOL bInheritHandles,
     DWORD dwCreationFlags,
     LPVOID lpEnvironment,
     LPCWSTR lpCurrentDirectory,
     LPSTARTUPINFOW lpStartupInfo,
     LPPROCESS_INFORMATION lpProcessInformation
    );




#line 9535 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"











































#line 9579 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
ImpersonateAnonymousToken(
     HANDLE ThreadHandle
    );

__declspec(dllimport)
BOOL
__stdcall
DuplicateTokenEx(
     HANDLE hExistingToken,
     DWORD dwDesiredAccess,
     LPSECURITY_ATTRIBUTES lpTokenAttributes,
     SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
     TOKEN_TYPE TokenType,
     PHANDLE phNewToken);

__declspec(dllimport)
BOOL
__stdcall
CreateRestrictedToken(
     HANDLE ExistingTokenHandle,
     DWORD Flags,
     DWORD DisableSidCount,
     PSID_AND_ATTRIBUTES SidsToDisable ,
     DWORD DeletePrivilegeCount,
     PLUID_AND_ATTRIBUTES PrivilegesToDelete ,
     DWORD RestrictedSidCount,
     PSID_AND_ATTRIBUTES SidsToRestrict ,
     PHANDLE NewTokenHandle
    );


__declspec(dllimport)
BOOL
__stdcall
IsTokenRestricted(
     HANDLE TokenHandle
    );

__declspec(dllimport)
BOOL
__stdcall
IsTokenUntrusted(
     HANDLE TokenHandle
    );


BOOL
__stdcall
CheckTokenMembership(
     HANDLE TokenHandle ,
     PSID SidToCheck,
     PBOOL IsMember
    );











































































































































#line 9776 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"




















































#line 9829 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





__declspec(dllimport)
BOOL
__stdcall
QueryPerformanceCounter(
     LARGE_INTEGER *lpPerformanceCount
    );

__declspec(dllimport)
BOOL
__stdcall
QueryPerformanceFrequency(
     LARGE_INTEGER *lpFrequency
    );



__declspec(dllimport)
BOOL
__stdcall
GetVersionExA(
      LPOSVERSIONINFOA lpVersionInformation
    );
__declspec(dllimport)
BOOL
__stdcall
GetVersionExW(
      LPOSVERSIONINFOW lpVersionInformation
    );




#line 9867 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



__declspec(dllimport)
BOOL
__stdcall
VerifyVersionInfoA(
     LPOSVERSIONINFOEXA lpVersionInformation,
     DWORD dwTypeMask,
     DWORDLONG dwlConditionMask
    );
__declspec(dllimport)
BOOL
__stdcall
VerifyVersionInfoW(
     LPOSVERSIONINFOEXW lpVersionInformation,
     DWORD dwTypeMask,
     DWORDLONG dwlConditionMask
    );




#line 9891 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winerror.h"












#pragma once
#line 15 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winerror.h"

































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 15249 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winerror.h"



















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 17125 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winerror.h"







































#line 17165 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winerror.h"


























































































































































































#line 17352 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winerror.h"






















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































 
 
 
 
 






















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 27162 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winerror.h"
#line 9897 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"





























typedef struct _SYSTEM_POWER_STATUS {
    BYTE ACLineStatus;
    BYTE BatteryFlag;
    BYTE BatteryLifePercent;
    BYTE Reserved1;
    DWORD BatteryLifeTime;
    DWORD BatteryFullLifeTime;
}   SYSTEM_POWER_STATUS, *LPSYSTEM_POWER_STATUS;

BOOL
__stdcall
GetSystemPowerStatus(
     LPSYSTEM_POWER_STATUS lpSystemPowerStatus
    );

BOOL
__stdcall
SetSystemPowerState(
     BOOL fSuspend,
     BOOL fForce
    );

#line 9949 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"













































































































































































































































































































































































#line 10315 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"




















































#line 10368 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"






































































#line 10439 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"















































#line 10487 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"









































#line 10529 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"























#line 10553 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



















































#line 10605 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


__declspec(dllimport)
BOOL
__stdcall
ProcessIdToSessionId(
      DWORD dwProcessId,
     DWORD *pSessionId
    );
















#line 10631 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

__declspec(dllimport)
BOOL
__stdcall
GetLogicalProcessorInformation(
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer,
    PDWORD ReturnedLength
    );





__declspec(dllimport)
BOOL
__stdcall
GetNumaHighestNodeNumber(
    PULONG HighestNodeNumber
    );

__declspec(dllimport)
BOOL
__stdcall
GetNumaProcessorNode(
    UCHAR Processor,
    PUCHAR NodeNumber
    );

__declspec(dllimport)
BOOL
__stdcall
GetNumaNodeProcessorMask(
    UCHAR Node,
    PULONGLONG ProcessorMask
    );

__declspec(dllimport)
BOOL
__stdcall
GetNumaAvailableMemoryNode(
    UCHAR Node,
    PULONGLONG AvailableBytes
    );






#line 10681 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"
#line 10682 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"


}
#line 10686 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"



#line 10690 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winbase.h"

#line 163 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"























#line 25 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"









#line 35 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


extern "C" {
#line 39 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
















































#line 88 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




#line 93 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

































#line 127 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"













#line 141 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

















#line 159 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
















#line 176 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


#line 179 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"














































































#line 258 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"









typedef struct _DRAWPATRECT {
        POINT ptPosition;
        POINT ptSize;
        WORD wStyle;
        WORD wPattern;
} DRAWPATRECT, *PDRAWPATRECT;
#line 274 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

#line 276 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"







































































































typedef struct _PSINJECTDATA {

    DWORD   DataBytes;      
    WORD    InjectionPoint; 
    WORD    PageNumber;     

    

} PSINJECTDATA, *PPSINJECTDATA;

































































typedef struct _PSFEATURE_OUTPUT {

    BOOL bPageIndependent;
    BOOL bSetPageDevice;

} PSFEATURE_OUTPUT, *PPSFEATURE_OUTPUT;





typedef struct _PSFEATURE_CUSTPAPER {

    LONG lOrientation;
    LONG lWidth;
    LONG lHeight;
    LONG lWidthOffset;
    LONG lHeightOffset;

} PSFEATURE_CUSTPAPER, *PPSFEATURE_CUSTPAPER;
















































typedef struct  tagXFORM
  {
    FLOAT   eM11;
    FLOAT   eM12;
    FLOAT   eM21;
    FLOAT   eM22;
    FLOAT   eDx;
    FLOAT   eDy;
  } XFORM, *PXFORM,  *LPXFORM;


typedef struct tagBITMAP
  {
    LONG        bmType;
    LONG        bmWidth;
    LONG        bmHeight;
    LONG        bmWidthBytes;
    WORD        bmPlanes;
    WORD        bmBitsPixel;
    LPVOID      bmBits;
  } BITMAP, *PBITMAP,  *NPBITMAP,  *LPBITMAP;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack1.h"























#pragma warning(disable:4103)

#pragma pack(push,1)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack1.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack1.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack1.h"
#line 544 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
typedef struct tagRGBTRIPLE {
        BYTE    rgbtBlue;
        BYTE    rgbtGreen;
        BYTE    rgbtRed;
} RGBTRIPLE;
#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 550 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;
typedef RGBQUAD * LPRGBQUAD;


















typedef LONG   LCSCSTYPE;


typedef LONG    LCSGAMUTMATCH;


























typedef long            FXPT16DOT16,  *LPFXPT16DOT16;
typedef long            FXPT2DOT30,  *LPFXPT2DOT30;




typedef struct tagCIEXYZ
{
        FXPT2DOT30 ciexyzX;
        FXPT2DOT30 ciexyzY;
        FXPT2DOT30 ciexyzZ;
} CIEXYZ;
typedef CIEXYZ   *LPCIEXYZ;

typedef struct tagICEXYZTRIPLE
{
        CIEXYZ  ciexyzRed;
        CIEXYZ  ciexyzGreen;
        CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;
typedef CIEXYZTRIPLE     *LPCIEXYZTRIPLE;






typedef struct tagLOGCOLORSPACEA {
    DWORD lcsSignature;
    DWORD lcsVersion;
    DWORD lcsSize;
    LCSCSTYPE lcsCSType;
    LCSGAMUTMATCH lcsIntent;
    CIEXYZTRIPLE lcsEndpoints;
    DWORD lcsGammaRed;
    DWORD lcsGammaGreen;
    DWORD lcsGammaBlue;
    CHAR   lcsFilename[260];
} LOGCOLORSPACEA, *LPLOGCOLORSPACEA;
typedef struct tagLOGCOLORSPACEW {
    DWORD lcsSignature;
    DWORD lcsVersion;
    DWORD lcsSize;
    LCSCSTYPE lcsCSType;
    LCSGAMUTMATCH lcsIntent;
    CIEXYZTRIPLE lcsEndpoints;
    DWORD lcsGammaRed;
    DWORD lcsGammaGreen;
    DWORD lcsGammaBlue;
    WCHAR  lcsFilename[260];
} LOGCOLORSPACEW, *LPLOGCOLORSPACEW;




typedef LOGCOLORSPACEA LOGCOLORSPACE;
typedef LPLOGCOLORSPACEA LPLOGCOLORSPACE;
#line 664 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

#line 666 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


typedef struct tagBITMAPCOREHEADER {
        DWORD   bcSize;                 
        WORD    bcWidth;
        WORD    bcHeight;
        WORD    bcPlanes;
        WORD    bcBitCount;
} BITMAPCOREHEADER,  *LPBITMAPCOREHEADER, *PBITMAPCOREHEADER;

typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER,  *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;


typedef struct {
        DWORD        bV4Size;
        LONG         bV4Width;
        LONG         bV4Height;
        WORD         bV4Planes;
        WORD         bV4BitCount;
        DWORD        bV4V4Compression;
        DWORD        bV4SizeImage;
        LONG         bV4XPelsPerMeter;
        LONG         bV4YPelsPerMeter;
        DWORD        bV4ClrUsed;
        DWORD        bV4ClrImportant;
        DWORD        bV4RedMask;
        DWORD        bV4GreenMask;
        DWORD        bV4BlueMask;
        DWORD        bV4AlphaMask;
        DWORD        bV4CSType;
        CIEXYZTRIPLE bV4Endpoints;
        DWORD        bV4GammaRed;
        DWORD        bV4GammaGreen;
        DWORD        bV4GammaBlue;
} BITMAPV4HEADER,  *LPBITMAPV4HEADER, *PBITMAPV4HEADER;
#line 714 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


typedef struct {
        DWORD        bV5Size;
        LONG         bV5Width;
        LONG         bV5Height;
        WORD         bV5Planes;
        WORD         bV5BitCount;
        DWORD        bV5Compression;
        DWORD        bV5SizeImage;
        LONG         bV5XPelsPerMeter;
        LONG         bV5YPelsPerMeter;
        DWORD        bV5ClrUsed;
        DWORD        bV5ClrImportant;
        DWORD        bV5RedMask;
        DWORD        bV5GreenMask;
        DWORD        bV5BlueMask;
        DWORD        bV5AlphaMask;
        DWORD        bV5CSType;
        CIEXYZTRIPLE bV5Endpoints;
        DWORD        bV5GammaRed;
        DWORD        bV5GammaGreen;
        DWORD        bV5GammaBlue;
        DWORD        bV5Intent;
        DWORD        bV5ProfileData;
        DWORD        bV5ProfileSize;
        DWORD        bV5Reserved;
} BITMAPV5HEADER,  *LPBITMAPV5HEADER, *PBITMAPV5HEADER;




#line 747 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"









#line 757 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO,  *LPBITMAPINFO, *PBITMAPINFO;

typedef struct tagBITMAPCOREINFO {
    BITMAPCOREHEADER    bmciHeader;
    RGBTRIPLE           bmciColors[1];
} BITMAPCOREINFO,  *LPBITMAPCOREINFO, *PBITMAPCOREINFO;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"























#pragma warning(disable:4103)

#pragma pack(push,2)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 769 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BITMAPFILEHEADER,  *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;
#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 777 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"





typedef struct tagFONTSIGNATURE
{
    DWORD fsUsb[4];
    DWORD fsCsb[2];
} FONTSIGNATURE, *PFONTSIGNATURE, *LPFONTSIGNATURE;

typedef struct tagCHARSETINFO
{
    UINT ciCharset;
    UINT ciACP;
    FONTSIGNATURE fs;
} CHARSETINFO, *PCHARSETINFO,  *NPCHARSETINFO,  *LPCHARSETINFO;






typedef struct tagLOCALESIGNATURE
{
    DWORD lsUsb[4];
    DWORD lsCsbDefault[2];
    DWORD lsCsbSupported[2];
} LOCALESIGNATURE, *PLOCALESIGNATURE, *LPLOCALESIGNATURE;


#line 809 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 810 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




typedef struct tagHANDLETABLE
  {
    HGDIOBJ     objectHandle[1];
  } HANDLETABLE, *PHANDLETABLE,  *LPHANDLETABLE;

typedef struct tagMETARECORD
  {
    DWORD       rdSize;
    WORD        rdFunction;
    WORD        rdParm[1];
  } METARECORD;
typedef struct tagMETARECORD  *PMETARECORD;
typedef struct tagMETARECORD   *LPMETARECORD;

typedef struct tagMETAFILEPICT
  {
    LONG        mm;
    LONG        xExt;
    LONG        yExt;
    HMETAFILE   hMF;
  } METAFILEPICT,  *LPMETAFILEPICT;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"























#pragma warning(disable:4103)

#pragma pack(push,2)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 837 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
typedef struct tagMETAHEADER
{
    WORD        mtType;
    WORD        mtHeaderSize;
    WORD        mtVersion;
    DWORD       mtSize;
    WORD        mtNoObjects;
    DWORD       mtMaxRecord;
    WORD        mtNoParameters;
} METAHEADER;
typedef struct tagMETAHEADER  *PMETAHEADER;
typedef struct tagMETAHEADER   *LPMETAHEADER;

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 851 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


typedef struct tagENHMETARECORD
{
    DWORD   iType;              
    DWORD   nSize;              
    DWORD   dParm[1];           
} ENHMETARECORD, *PENHMETARECORD, *LPENHMETARECORD;

typedef struct tagENHMETAHEADER
{
    DWORD   iType;              
    DWORD   nSize;              
                                
    RECTL   rclBounds;          
    RECTL   rclFrame;           
    DWORD   dSignature;         
    DWORD   nVersion;           
    DWORD   nBytes;             
    DWORD   nRecords;           
    WORD    nHandles;           
                                
    WORD    sReserved;          
    DWORD   nDescription;       
                                
    DWORD   offDescription;     
                                
    DWORD   nPalEntries;        
    SIZEL   szlDevice;          
    SIZEL   szlMillimeters;     

    DWORD   cbPixelFormat;      
                                
    DWORD   offPixelFormat;     
                                
    DWORD   bOpenGL;            
                                
#line 889 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

    SIZEL   szlMicrometers;     
#line 892 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

} ENHMETAHEADER, *PENHMETAHEADER, *LPENHMETAHEADER;

#line 896 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"















    typedef BYTE BCHAR;
#line 913 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



typedef struct tagTEXTMETRICA
{
    LONG        tmHeight;
    LONG        tmAscent;
    LONG        tmDescent;
    LONG        tmInternalLeading;
    LONG        tmExternalLeading;
    LONG        tmAveCharWidth;
    LONG        tmMaxCharWidth;
    LONG        tmWeight;
    LONG        tmOverhang;
    LONG        tmDigitizedAspectX;
    LONG        tmDigitizedAspectY;
    BYTE        tmFirstChar;
    BYTE        tmLastChar;
    BYTE        tmDefaultChar;
    BYTE        tmBreakChar;
    BYTE        tmItalic;
    BYTE        tmUnderlined;
    BYTE        tmStruckOut;
    BYTE        tmPitchAndFamily;
    BYTE        tmCharSet;
} TEXTMETRICA, *PTEXTMETRICA,  *NPTEXTMETRICA,  *LPTEXTMETRICA;
typedef struct tagTEXTMETRICW
{
    LONG        tmHeight;
    LONG        tmAscent;
    LONG        tmDescent;
    LONG        tmInternalLeading;
    LONG        tmExternalLeading;
    LONG        tmAveCharWidth;
    LONG        tmMaxCharWidth;
    LONG        tmWeight;
    LONG        tmOverhang;
    LONG        tmDigitizedAspectX;
    LONG        tmDigitizedAspectY;
    WCHAR       tmFirstChar;
    WCHAR       tmLastChar;
    WCHAR       tmDefaultChar;
    WCHAR       tmBreakChar;
    BYTE        tmItalic;
    BYTE        tmUnderlined;
    BYTE        tmStruckOut;
    BYTE        tmPitchAndFamily;
    BYTE        tmCharSet;
} TEXTMETRICW, *PTEXTMETRICW,  *NPTEXTMETRICW,  *LPTEXTMETRICW;






typedef TEXTMETRICA TEXTMETRIC;
typedef PTEXTMETRICA PTEXTMETRIC;
typedef NPTEXTMETRICA NPTEXTMETRIC;
typedef LPTEXTMETRICA LPTEXTMETRIC;
#line 973 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 974 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"















#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"























#pragma warning(disable:4103)

#pragma pack(push,4)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack4.h"
#line 990 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
typedef struct tagNEWTEXTMETRICA
{
    LONG        tmHeight;
    LONG        tmAscent;
    LONG        tmDescent;
    LONG        tmInternalLeading;
    LONG        tmExternalLeading;
    LONG        tmAveCharWidth;
    LONG        tmMaxCharWidth;
    LONG        tmWeight;
    LONG        tmOverhang;
    LONG        tmDigitizedAspectX;
    LONG        tmDigitizedAspectY;
    BYTE        tmFirstChar;
    BYTE        tmLastChar;
    BYTE        tmDefaultChar;
    BYTE        tmBreakChar;
    BYTE        tmItalic;
    BYTE        tmUnderlined;
    BYTE        tmStruckOut;
    BYTE        tmPitchAndFamily;
    BYTE        tmCharSet;
    DWORD   ntmFlags;
    UINT    ntmSizeEM;
    UINT    ntmCellHeight;
    UINT    ntmAvgWidth;
} NEWTEXTMETRICA, *PNEWTEXTMETRICA,  *NPNEWTEXTMETRICA,  *LPNEWTEXTMETRICA;
typedef struct tagNEWTEXTMETRICW
{
    LONG        tmHeight;
    LONG        tmAscent;
    LONG        tmDescent;
    LONG        tmInternalLeading;
    LONG        tmExternalLeading;
    LONG        tmAveCharWidth;
    LONG        tmMaxCharWidth;
    LONG        tmWeight;
    LONG        tmOverhang;
    LONG        tmDigitizedAspectX;
    LONG        tmDigitizedAspectY;
    WCHAR       tmFirstChar;
    WCHAR       tmLastChar;
    WCHAR       tmDefaultChar;
    WCHAR       tmBreakChar;
    BYTE        tmItalic;
    BYTE        tmUnderlined;
    BYTE        tmStruckOut;
    BYTE        tmPitchAndFamily;
    BYTE        tmCharSet;
    DWORD   ntmFlags;
    UINT    ntmSizeEM;
    UINT    ntmCellHeight;
    UINT    ntmAvgWidth;
} NEWTEXTMETRICW, *PNEWTEXTMETRICW,  *NPNEWTEXTMETRICW,  *LPNEWTEXTMETRICW;






typedef NEWTEXTMETRICA NEWTEXTMETRIC;
typedef PNEWTEXTMETRICA PNEWTEXTMETRIC;
typedef NPNEWTEXTMETRICA NPNEWTEXTMETRIC;
typedef LPNEWTEXTMETRICA LPNEWTEXTMETRIC;
#line 1055 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 1056 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


typedef struct tagNEWTEXTMETRICEXA
{
    NEWTEXTMETRICA  ntmTm;
    FONTSIGNATURE   ntmFontSig;
}NEWTEXTMETRICEXA;
typedef struct tagNEWTEXTMETRICEXW
{
    NEWTEXTMETRICW  ntmTm;
    FONTSIGNATURE   ntmFontSig;
}NEWTEXTMETRICEXW;



typedef NEWTEXTMETRICEXA NEWTEXTMETRICEX;
#line 1073 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 1074 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

#line 1076 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



typedef struct tagPELARRAY
  {
    LONG        paXCount;
    LONG        paYCount;
    LONG        paXExt;
    LONG        paYExt;
    BYTE        paRGBs;
  } PELARRAY, *PPELARRAY,  *NPPELARRAY,  *LPPELARRAY;


typedef struct tagLOGBRUSH
  {
    UINT        lbStyle;
    COLORREF    lbColor;
    ULONG_PTR    lbHatch;    
  } LOGBRUSH, *PLOGBRUSH,  *NPLOGBRUSH,  *LPLOGBRUSH;

typedef struct tagLOGBRUSH32
  {
    UINT        lbStyle;
    COLORREF    lbColor;
    ULONG       lbHatch;
  } LOGBRUSH32, *PLOGBRUSH32,  *NPLOGBRUSH32,  *LPLOGBRUSH32;

typedef LOGBRUSH            PATTERN;
typedef PATTERN             *PPATTERN;
typedef PATTERN         *NPPATTERN;
typedef PATTERN          *LPPATTERN;


typedef struct tagLOGPEN
  {
    UINT        lopnStyle;
    POINT       lopnWidth;
    COLORREF    lopnColor;
  } LOGPEN, *PLOGPEN,  *NPLOGPEN,  *LPLOGPEN;

typedef struct tagEXTLOGPEN {
    DWORD       elpPenStyle;
    DWORD       elpWidth;
    UINT        elpBrushStyle;
    COLORREF    elpColor;
    ULONG_PTR    elpHatch;     
    DWORD       elpNumEntries;
    DWORD       elpStyleEntry[1];
} EXTLOGPEN, *PEXTLOGPEN,  *NPEXTLOGPEN,  *LPEXTLOGPEN;



typedef struct tagPALETTEENTRY {
    BYTE        peRed;
    BYTE        peGreen;
    BYTE        peBlue;
    BYTE        peFlags;
} PALETTEENTRY, *PPALETTEENTRY,  *LPPALETTEENTRY;
#line 1135 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




typedef struct tagLOGPALETTE {
    WORD        palVersion;
    WORD        palNumEntries;
    PALETTEENTRY        palPalEntry[1];
} LOGPALETTE, *PLOGPALETTE,  *NPLOGPALETTE,  *LPLOGPALETTE;
#line 1145 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"





typedef struct tagLOGFONTA
{
    LONG      lfHeight;
    LONG      lfWidth;
    LONG      lfEscapement;
    LONG      lfOrientation;
    LONG      lfWeight;
    BYTE      lfItalic;
    BYTE      lfUnderline;
    BYTE      lfStrikeOut;
    BYTE      lfCharSet;
    BYTE      lfOutPrecision;
    BYTE      lfClipPrecision;
    BYTE      lfQuality;
    BYTE      lfPitchAndFamily;
    CHAR      lfFaceName[32];
} LOGFONTA, *PLOGFONTA,  *NPLOGFONTA,  *LPLOGFONTA;
typedef struct tagLOGFONTW
{
    LONG      lfHeight;
    LONG      lfWidth;
    LONG      lfEscapement;
    LONG      lfOrientation;
    LONG      lfWeight;
    BYTE      lfItalic;
    BYTE      lfUnderline;
    BYTE      lfStrikeOut;
    BYTE      lfCharSet;
    BYTE      lfOutPrecision;
    BYTE      lfClipPrecision;
    BYTE      lfQuality;
    BYTE      lfPitchAndFamily;
    WCHAR     lfFaceName[32];
} LOGFONTW, *PLOGFONTW,  *NPLOGFONTW,  *LPLOGFONTW;






typedef LOGFONTA LOGFONT;
typedef PLOGFONTA PLOGFONT;
typedef NPLOGFONTA NPLOGFONT;
typedef LPLOGFONTA LPLOGFONT;
#line 1195 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




typedef struct tagENUMLOGFONTA
{
    LOGFONTA elfLogFont;
    BYTE     elfFullName[64];
    BYTE     elfStyle[32];
} ENUMLOGFONTA, * LPENUMLOGFONTA;

typedef struct tagENUMLOGFONTW
{
    LOGFONTW elfLogFont;
    WCHAR    elfFullName[64];
    WCHAR    elfStyle[32];
} ENUMLOGFONTW, * LPENUMLOGFONTW;




typedef ENUMLOGFONTA ENUMLOGFONT;
typedef LPENUMLOGFONTA LPENUMLOGFONT;
#line 1219 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


typedef struct tagENUMLOGFONTEXA
{
    LOGFONTA    elfLogFont;
    BYTE        elfFullName[64];
    BYTE        elfStyle[32];
    BYTE        elfScript[32];
} ENUMLOGFONTEXA,  *LPENUMLOGFONTEXA;
typedef struct tagENUMLOGFONTEXW
{
    LOGFONTW    elfLogFont;
    WCHAR       elfFullName[64];
    WCHAR       elfStyle[32];
    WCHAR       elfScript[32];
} ENUMLOGFONTEXW,  *LPENUMLOGFONTEXW;




typedef ENUMLOGFONTEXA ENUMLOGFONTEX;
typedef LPENUMLOGFONTEXA LPENUMLOGFONTEX;
#line 1242 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 1243 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



























#line 1271 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



#line 1275 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



#line 1279 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"






#line 1286 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"








































#line 1327 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




                                    

                                    

                                    



































typedef struct tagPANOSE
{
    BYTE    bFamilyType;
    BYTE    bSerifStyle;
    BYTE    bWeight;
    BYTE    bProportion;
    BYTE    bContrast;
    BYTE    bStrokeVariation;
    BYTE    bArmStyle;
    BYTE    bLetterform;
    BYTE    bMidline;
    BYTE    bXHeight;
} PANOSE, * LPPANOSE;

















































































































typedef struct tagEXTLOGFONTA {
    LOGFONTA    elfLogFont;
    BYTE        elfFullName[64];
    BYTE        elfStyle[32];
    DWORD       elfVersion;     
    DWORD       elfStyleSize;
    DWORD       elfMatch;
    DWORD       elfReserved;
    BYTE        elfVendorId[4];
    DWORD       elfCulture;     
    PANOSE      elfPanose;
} EXTLOGFONTA, *PEXTLOGFONTA,  *NPEXTLOGFONTA,  *LPEXTLOGFONTA;
typedef struct tagEXTLOGFONTW {
    LOGFONTW    elfLogFont;
    WCHAR       elfFullName[64];
    WCHAR       elfStyle[32];
    DWORD       elfVersion;     
    DWORD       elfStyleSize;
    DWORD       elfMatch;
    DWORD       elfReserved;
    BYTE        elfVendorId[4];
    DWORD       elfCulture;     
    PANOSE      elfPanose;
} EXTLOGFONTW, *PEXTLOGFONTW,  *NPEXTLOGFONTW,  *LPEXTLOGFONTW;






typedef EXTLOGFONTA EXTLOGFONT;
typedef PEXTLOGFONTA PEXTLOGFONT;
typedef NPEXTLOGFONTA NPEXTLOGFONT;
typedef LPEXTLOGFONTA LPEXTLOGFONT;
#line 1532 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
















































































#line 1613 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




#line 1618 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



#line 1622 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



#line 1626 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

































































































                             

                             

                             





#line 1734 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"










































































#line 1809 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"





































#line 1847 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



























typedef struct _devicemodeA {
    BYTE   dmDeviceName[32];
    WORD dmSpecVersion;
    WORD dmDriverVersion;
    WORD dmSize;
    WORD dmDriverExtra;
    DWORD dmFields;
    union {
      
      struct {
        short dmOrientation;
        short dmPaperSize;
        short dmPaperLength;
        short dmPaperWidth;
        short dmScale;
        short dmCopies;
        short dmDefaultSource;
        short dmPrintQuality;
      };
      
      struct {
        POINTL dmPosition;
        DWORD  dmDisplayOrientation;
        DWORD  dmDisplayFixedOutput; 
      };
    };
    short dmColor;
    short dmDuplex;
    short dmYResolution;
    short dmTTOption;
    short dmCollate;
    BYTE   dmFormName[32];
    WORD   dmLogPixels;
    DWORD  dmBitsPerPel;
    DWORD  dmPelsWidth;
    DWORD  dmPelsHeight;
    union {
        DWORD  dmDisplayFlags;
        DWORD  dmNup;
    };
    DWORD  dmDisplayFrequency;

    DWORD  dmICMMethod;
    DWORD  dmICMIntent;
    DWORD  dmMediaType;
    DWORD  dmDitherType;
    DWORD  dmReserved1;
    DWORD  dmReserved2;

    DWORD  dmPanningWidth;
    DWORD  dmPanningHeight;
#line 1926 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 1927 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
} DEVMODEA, *PDEVMODEA, *NPDEVMODEA, *LPDEVMODEA;
typedef struct _devicemodeW {
    WCHAR  dmDeviceName[32];
    WORD dmSpecVersion;
    WORD dmDriverVersion;
    WORD dmSize;
    WORD dmDriverExtra;
    DWORD dmFields;
    union {
      
      struct {
        short dmOrientation;
        short dmPaperSize;
        short dmPaperLength;
        short dmPaperWidth;
        short dmScale;
        short dmCopies;
        short dmDefaultSource;
        short dmPrintQuality;
      };
      
      struct {
        POINTL dmPosition;
        DWORD  dmDisplayOrientation;
        DWORD  dmDisplayFixedOutput; 
      };
    };
    short dmColor;
    short dmDuplex;
    short dmYResolution;
    short dmTTOption;
    short dmCollate;
    WCHAR  dmFormName[32];
    WORD   dmLogPixels;
    DWORD  dmBitsPerPel;
    DWORD  dmPelsWidth;
    DWORD  dmPelsHeight;
    union {
        DWORD  dmDisplayFlags;
        DWORD  dmNup;
    };
    DWORD  dmDisplayFrequency;

    DWORD  dmICMMethod;
    DWORD  dmICMIntent;
    DWORD  dmMediaType;
    DWORD  dmDitherType;
    DWORD  dmReserved1;
    DWORD  dmReserved2;

    DWORD  dmPanningWidth;
    DWORD  dmPanningHeight;
#line 1980 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 1981 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
} DEVMODEW, *PDEVMODEW, *NPDEVMODEW, *LPDEVMODEW;






typedef DEVMODEA DEVMODE;
typedef PDEVMODEA PDEVMODE;
typedef NPDEVMODEA NPDEVMODE;
typedef LPDEVMODEA LPDEVMODE;
#line 1993 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"








#line 2002 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"










#line 2013 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


#line 2016 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"






















#line 2039 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


#line 2042 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"












































































#line 2119 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




















































#line 2172 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"







#line 2180 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"












































#line 2225 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
















#line 2242 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

















































#line 2292 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

typedef struct _DISPLAY_DEVICEA {
    DWORD  cb;
    CHAR   DeviceName[32];
    CHAR   DeviceString[128];
    DWORD  StateFlags;
    CHAR   DeviceID[128];
    CHAR   DeviceKey[128];
} DISPLAY_DEVICEA, *PDISPLAY_DEVICEA, *LPDISPLAY_DEVICEA;
typedef struct _DISPLAY_DEVICEW {
    DWORD  cb;
    WCHAR  DeviceName[32];
    WCHAR  DeviceString[128];
    DWORD  StateFlags;
    WCHAR  DeviceID[128];
    WCHAR  DeviceKey[128];
} DISPLAY_DEVICEW, *PDISPLAY_DEVICEW, *LPDISPLAY_DEVICEW;





typedef DISPLAY_DEVICEA DISPLAY_DEVICE;
typedef PDISPLAY_DEVICEA PDISPLAY_DEVICE;
typedef LPDISPLAY_DEVICEA LPDISPLAY_DEVICE;
#line 2318 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




















typedef struct _RGNDATAHEADER {
    DWORD   dwSize;
    DWORD   iType;
    DWORD   nCount;
    DWORD   nRgnSize;
    RECT    rcBound;
} RGNDATAHEADER, *PRGNDATAHEADER;

typedef struct _RGNDATA {
    RGNDATAHEADER   rdh;
    char            Buffer[1];
} RGNDATA, *PRGNDATA,  *NPRGNDATA,  *LPRGNDATA;





typedef struct _ABC {
    int     abcA;
    UINT    abcB;
    int     abcC;
} ABC, *PABC,  *NPABC,  *LPABC;

typedef struct _ABCFLOAT {
    FLOAT   abcfA;
    FLOAT   abcfB;
    FLOAT   abcfC;
} ABCFLOAT, *PABCFLOAT,  *NPABCFLOAT,  *LPABCFLOAT;






typedef struct _OUTLINETEXTMETRICA {
    UINT    otmSize;
    TEXTMETRICA otmTextMetrics;
    BYTE    otmFiller;
    PANOSE  otmPanoseNumber;
    UINT    otmfsSelection;
    UINT    otmfsType;
     int    otmsCharSlopeRise;
     int    otmsCharSlopeRun;
     int    otmItalicAngle;
    UINT    otmEMSquare;
     int    otmAscent;
     int    otmDescent;
    UINT    otmLineGap;
    UINT    otmsCapEmHeight;
    UINT    otmsXHeight;
    RECT    otmrcFontBox;
     int    otmMacAscent;
     int    otmMacDescent;
    UINT    otmMacLineGap;
    UINT    otmusMinimumPPEM;
    POINT   otmptSubscriptSize;
    POINT   otmptSubscriptOffset;
    POINT   otmptSuperscriptSize;
    POINT   otmptSuperscriptOffset;
    UINT    otmsStrikeoutSize;
     int    otmsStrikeoutPosition;
     int    otmsUnderscoreSize;
     int    otmsUnderscorePosition;
    PSTR    otmpFamilyName;
    PSTR    otmpFaceName;
    PSTR    otmpStyleName;
    PSTR    otmpFullName;
} OUTLINETEXTMETRICA, *POUTLINETEXTMETRICA,  *NPOUTLINETEXTMETRICA,  *LPOUTLINETEXTMETRICA;
typedef struct _OUTLINETEXTMETRICW {
    UINT    otmSize;
    TEXTMETRICW otmTextMetrics;
    BYTE    otmFiller;
    PANOSE  otmPanoseNumber;
    UINT    otmfsSelection;
    UINT    otmfsType;
     int    otmsCharSlopeRise;
     int    otmsCharSlopeRun;
     int    otmItalicAngle;
    UINT    otmEMSquare;
     int    otmAscent;
     int    otmDescent;
    UINT    otmLineGap;
    UINT    otmsCapEmHeight;
    UINT    otmsXHeight;
    RECT    otmrcFontBox;
     int    otmMacAscent;
     int    otmMacDescent;
    UINT    otmMacLineGap;
    UINT    otmusMinimumPPEM;
    POINT   otmptSubscriptSize;
    POINT   otmptSubscriptOffset;
    POINT   otmptSuperscriptSize;
    POINT   otmptSuperscriptOffset;
    UINT    otmsStrikeoutSize;
     int    otmsStrikeoutPosition;
     int    otmsUnderscoreSize;
     int    otmsUnderscorePosition;
    PSTR    otmpFamilyName;
    PSTR    otmpFaceName;
    PSTR    otmpStyleName;
    PSTR    otmpFullName;
} OUTLINETEXTMETRICW, *POUTLINETEXTMETRICW,  *NPOUTLINETEXTMETRICW,  *LPOUTLINETEXTMETRICW;






typedef OUTLINETEXTMETRICA OUTLINETEXTMETRIC;
typedef POUTLINETEXTMETRICA POUTLINETEXTMETRIC;
typedef NPOUTLINETEXTMETRICA NPOUTLINETEXTMETRIC;
typedef LPOUTLINETEXTMETRICA LPOUTLINETEXTMETRIC;
#line 2451 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"





#line 2457 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


typedef struct tagPOLYTEXTA
{
    int       x;
    int       y;
    UINT      n;
    LPCSTR    lpstr;
    UINT      uiFlags;
    RECT      rcl;
    int      *pdx;
} POLYTEXTA, *PPOLYTEXTA,  *NPPOLYTEXTA,  *LPPOLYTEXTA;
typedef struct tagPOLYTEXTW
{
    int       x;
    int       y;
    UINT      n;
    LPCWSTR   lpstr;
    UINT      uiFlags;
    RECT      rcl;
    int      *pdx;
} POLYTEXTW, *PPOLYTEXTW,  *NPPOLYTEXTW,  *LPPOLYTEXTW;






typedef POLYTEXTA POLYTEXT;
typedef PPOLYTEXTA PPOLYTEXT;
typedef NPPOLYTEXTA NPPOLYTEXT;
typedef LPPOLYTEXTA LPPOLYTEXT;
#line 2490 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

typedef struct _FIXED {

    WORD    fract;
    short   value;



#line 2499 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
} FIXED;


typedef struct _MAT2 {
     FIXED  eM11;
     FIXED  eM12;
     FIXED  eM21;
     FIXED  eM22;
} MAT2,  *LPMAT2;



typedef struct _GLYPHMETRICS {
    UINT    gmBlackBoxX;
    UINT    gmBlackBoxY;
    POINT   gmptGlyphOrigin;
    short   gmCellIncX;
    short   gmCellIncY;
} GLYPHMETRICS,  *LPGLYPHMETRICS;













#line 2532 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



#line 2536 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"







typedef struct tagPOINTFX
{
    FIXED x;
    FIXED y;
} POINTFX, * LPPOINTFX;

typedef struct tagTTPOLYCURVE
{
    WORD    wType;
    WORD    cpfx;
    POINTFX apfx[1];
} TTPOLYCURVE, * LPTTPOLYCURVE;

typedef struct tagTTPOLYGONHEADER
{
    DWORD   cb;
    DWORD   dwType;
    POINTFX pfxStart;
} TTPOLYGONHEADER, * LPTTPOLYGONHEADER;













































typedef struct tagGCP_RESULTSA
    {
    DWORD   lStructSize;
    LPSTR     lpOutString;
    UINT  *lpOrder;
    int   *lpDx;
    int   *lpCaretPos;
    LPSTR   lpClass;
    LPWSTR  lpGlyphs;
    UINT    nGlyphs;
    int     nMaxFit;
    } GCP_RESULTSA, * LPGCP_RESULTSA;
typedef struct tagGCP_RESULTSW
    {
    DWORD   lStructSize;
    LPWSTR    lpOutString;
    UINT  *lpOrder;
    int   *lpDx;
    int   *lpCaretPos;
    LPSTR   lpClass;
    LPWSTR  lpGlyphs;
    UINT    nGlyphs;
    int     nMaxFit;
    } GCP_RESULTSW, * LPGCP_RESULTSW;




typedef GCP_RESULTSA GCP_RESULTS;
typedef LPGCP_RESULTSA LPGCP_RESULTS;
#line 2638 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 2639 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

typedef struct _RASTERIZER_STATUS {
    short   nSize;
    short   wFlags;
    short   nLanguageID;
} RASTERIZER_STATUS,  *LPRASTERIZER_STATUS;






typedef struct tagPIXELFORMATDESCRIPTOR
{
    WORD  nSize;
    WORD  nVersion;
    DWORD dwFlags;
    BYTE  iPixelType;
    BYTE  cColorBits;
    BYTE  cRedBits;
    BYTE  cRedShift;
    BYTE  cGreenBits;
    BYTE  cGreenShift;
    BYTE  cBlueBits;
    BYTE  cBlueShift;
    BYTE  cAlphaBits;
    BYTE  cAlphaShift;
    BYTE  cAccumBits;
    BYTE  cAccumRedBits;
    BYTE  cAccumGreenBits;
    BYTE  cAccumBlueBits;
    BYTE  cAccumAlphaBits;
    BYTE  cDepthBits;
    BYTE  cStencilBits;
    BYTE  cAuxBuffers;
    BYTE  iLayerType;
    BYTE  bReserved;
    DWORD dwLayerMask;
    DWORD dwVisibleMask;
    DWORD dwDamageMask;
} PIXELFORMATDESCRIPTOR, *PPIXELFORMATDESCRIPTOR,  *LPPIXELFORMATDESCRIPTOR;

































typedef int (__stdcall* OLDFONTENUMPROCA)(const LOGFONTA *, const TEXTMETRICA *, DWORD, LPARAM);
typedef int (__stdcall* OLDFONTENUMPROCW)(const LOGFONTW *, const TEXTMETRICW *, DWORD, LPARAM);




#line 2720 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"








#line 2729 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

typedef OLDFONTENUMPROCA    FONTENUMPROCA;
typedef OLDFONTENUMPROCW    FONTENUMPROCW;



typedef FONTENUMPROCA FONTENUMPROC;
#line 2737 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

typedef int (__stdcall* GOBJENUMPROC)(LPVOID, LPARAM);
typedef void (__stdcall* LINEDDAPROC)(int, int, LPARAM);











#line 2752 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



__declspec(dllimport) int __stdcall AddFontResourceA( LPCSTR);
__declspec(dllimport) int __stdcall AddFontResourceW( LPCWSTR);




#line 2762 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


__declspec(dllimport) BOOL  __stdcall AnimatePalette(  HPALETTE,  UINT,   UINT,  const PALETTEENTRY *);
__declspec(dllimport) BOOL  __stdcall Arc(  HDC,  int,  int,  int,  int,  int,  int,  int,  int);
__declspec(dllimport) BOOL  __stdcall BitBlt(  HDC,  int,  int,  int,  int,  HDC,  int,  int,  DWORD);
__declspec(dllimport) BOOL  __stdcall CancelDC(  HDC);
__declspec(dllimport) BOOL  __stdcall Chord(  HDC,  int,  int,  int,  int,  int,  int,  int,  int);
__declspec(dllimport) int   __stdcall ChoosePixelFormat(  HDC,  const PIXELFORMATDESCRIPTOR *);
__declspec(dllimport) HMETAFILE  __stdcall CloseMetaFile(  HDC);
__declspec(dllimport) int     __stdcall CombineRgn(  HRGN,  HRGN,  HRGN,  int);
__declspec(dllimport) HMETAFILE __stdcall CopyMetaFileA(  HMETAFILE,  LPCSTR);
__declspec(dllimport) HMETAFILE __stdcall CopyMetaFileW(  HMETAFILE,  LPCWSTR);




#line 2779 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) HBITMAP __stdcall CreateBitmap(  int,  int,  UINT,  UINT,  const void *);
__declspec(dllimport) HBITMAP __stdcall CreateBitmapIndirect(  const BITMAP *);
__declspec(dllimport) HBRUSH  __stdcall CreateBrushIndirect(  const LOGBRUSH *);
__declspec(dllimport) HBITMAP __stdcall CreateCompatibleBitmap(  HDC,  int,  int);
__declspec(dllimport) HBITMAP __stdcall CreateDiscardableBitmap(  HDC,  int,  int);
__declspec(dllimport) HDC     __stdcall CreateCompatibleDC(  HDC);
__declspec(dllimport) HDC     __stdcall CreateDCA(  LPCSTR,  LPCSTR,  LPCSTR,  const DEVMODEA *);
__declspec(dllimport) HDC     __stdcall CreateDCW(  LPCWSTR,  LPCWSTR,  LPCWSTR,  const DEVMODEW *);




#line 2792 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) HBITMAP __stdcall CreateDIBitmap(  HDC,  const BITMAPINFOHEADER *,  DWORD,  const void *,  const BITMAPINFO *,  UINT);
__declspec(dllimport) HBRUSH  __stdcall CreateDIBPatternBrush(  HGLOBAL,  UINT);
__declspec(dllimport) HBRUSH  __stdcall CreateDIBPatternBrushPt(  const void *,  UINT);
__declspec(dllimport) HRGN    __stdcall CreateEllipticRgn(  int,  int,  int,  int);
__declspec(dllimport) HRGN    __stdcall CreateEllipticRgnIndirect(  const RECT *);
__declspec(dllimport) HFONT   __stdcall CreateFontIndirectA(  const LOGFONTA *);
__declspec(dllimport) HFONT   __stdcall CreateFontIndirectW(  const LOGFONTW *);




#line 2804 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) HFONT   __stdcall CreateFontA(  int,  int,  int,  int,  int,  DWORD,
                              DWORD,  DWORD,  DWORD,  DWORD,  DWORD,
                              DWORD,  DWORD,  LPCSTR);
__declspec(dllimport) HFONT   __stdcall CreateFontW(  int,  int,  int,  int,  int,  DWORD,
                              DWORD,  DWORD,  DWORD,  DWORD,  DWORD,
                              DWORD,  DWORD,  LPCWSTR);




#line 2815 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) HBRUSH  __stdcall CreateHatchBrush(  int,  COLORREF);
__declspec(dllimport) HDC     __stdcall CreateICA(  LPCSTR,  LPCSTR,  LPCSTR,  const DEVMODEA *);
__declspec(dllimport) HDC     __stdcall CreateICW(  LPCWSTR,  LPCWSTR,  LPCWSTR,  const DEVMODEW *);




#line 2824 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) HDC     __stdcall CreateMetaFileA(  LPCSTR);
__declspec(dllimport) HDC     __stdcall CreateMetaFileW(  LPCWSTR);




#line 2831 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) HPALETTE __stdcall CreatePalette(  const LOGPALETTE *);
__declspec(dllimport) HPEN    __stdcall CreatePen(  int,  int,  COLORREF);
__declspec(dllimport) HPEN    __stdcall CreatePenIndirect(  const LOGPEN *);
__declspec(dllimport) HRGN    __stdcall CreatePolyPolygonRgn(  const POINT *,  const INT *,  int,  int);
__declspec(dllimport) HBRUSH  __stdcall CreatePatternBrush(  HBITMAP);
__declspec(dllimport) HRGN    __stdcall CreateRectRgn(  int,  int,  int,  int);
__declspec(dllimport) HRGN    __stdcall CreateRectRgnIndirect(  const RECT *);
__declspec(dllimport) HRGN    __stdcall CreateRoundRectRgn(  int,  int,  int,  int,  int,  int);
__declspec(dllimport) BOOL    __stdcall CreateScalableFontResourceA(  DWORD,  LPCSTR,  LPCSTR,  LPCSTR);
__declspec(dllimport) BOOL    __stdcall CreateScalableFontResourceW(  DWORD,  LPCWSTR,  LPCWSTR,  LPCWSTR);




#line 2846 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) HBRUSH  __stdcall CreateSolidBrush(  COLORREF);

__declspec(dllimport) BOOL __stdcall DeleteDC(  HDC);
__declspec(dllimport) BOOL __stdcall DeleteMetaFile(  HMETAFILE);
__declspec(dllimport) BOOL __stdcall DeleteObject(  HGDIOBJ);
__declspec(dllimport) int  __stdcall DescribePixelFormat(  HDC,  int,  UINT,  LPPIXELFORMATDESCRIPTOR);





typedef UINT   (__stdcall* LPFNDEVMODE)(HWND, HMODULE, LPDEVMODE, LPSTR, LPSTR, LPDEVMODE, LPSTR, UINT);

typedef DWORD  (__stdcall* LPFNDEVCAPS)(LPSTR, LPSTR, UINT, LPSTR, LPDEVMODE);






































#line 2899 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

















#line 2917 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

















#line 2935 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) int  __stdcall DeviceCapabilitiesA(  LPCSTR,  LPCSTR,  WORD,
                                 LPSTR,  const DEVMODEA *);
__declspec(dllimport) int  __stdcall DeviceCapabilitiesW(  LPCWSTR,  LPCWSTR,  WORD,
                                 LPWSTR,  const DEVMODEW *);




#line 2945 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) int  __stdcall DrawEscape(  HDC,  int,  int,  LPCSTR);
__declspec(dllimport) BOOL __stdcall Ellipse(  HDC,  int,  int,  int,  int);


__declspec(dllimport) int  __stdcall EnumFontFamiliesExA(  HDC,  LPLOGFONTA,  FONTENUMPROCA,  LPARAM,  DWORD);
__declspec(dllimport) int  __stdcall EnumFontFamiliesExW(  HDC,  LPLOGFONTW,  FONTENUMPROCW,  LPARAM,  DWORD);




#line 2957 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 2958 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) int  __stdcall EnumFontFamiliesA(  HDC,  LPCSTR,  FONTENUMPROCA,  LPARAM);
__declspec(dllimport) int  __stdcall EnumFontFamiliesW(  HDC,  LPCWSTR,  FONTENUMPROCW,  LPARAM);




#line 2966 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) int  __stdcall EnumFontsA(  HDC,  LPCSTR,   FONTENUMPROCA,  LPARAM);
__declspec(dllimport) int  __stdcall EnumFontsW(  HDC,  LPCWSTR,   FONTENUMPROCW,  LPARAM);




#line 2973 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


__declspec(dllimport) int  __stdcall EnumObjects(  HDC,  int,  GOBJENUMPROC,  LPARAM);


#line 2979 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


__declspec(dllimport) BOOL __stdcall EqualRgn(  HRGN,  HRGN);
__declspec(dllimport) int  __stdcall Escape(  HDC,  int,  int,  LPCSTR,   LPVOID);
__declspec(dllimport) int  __stdcall ExtEscape(  HDC,  int,  int,  LPCSTR,   int,  LPSTR);
__declspec(dllimport) int  __stdcall ExcludeClipRect(  HDC,  int,  int,  int,  int);
__declspec(dllimport) HRGN __stdcall ExtCreateRegion(  const XFORM *,  DWORD,  const RGNDATA *);
__declspec(dllimport) BOOL  __stdcall ExtFloodFill(  HDC,  int,  int,  COLORREF,  UINT);
__declspec(dllimport) BOOL   __stdcall FillRgn(  HDC,  HRGN,  HBRUSH);
__declspec(dllimport) BOOL   __stdcall FloodFill(  HDC,  int,  int,  COLORREF);
__declspec(dllimport) BOOL   __stdcall FrameRgn(  HDC,  HRGN,  HBRUSH,  int,  int);
__declspec(dllimport) int   __stdcall GetROP2(  HDC);
__declspec(dllimport) BOOL  __stdcall GetAspectRatioFilterEx(  HDC,  LPSIZE);
__declspec(dllimport) COLORREF __stdcall GetBkColor(  HDC);




#line 2998 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) int   __stdcall GetBkMode(  HDC);
__declspec(dllimport) LONG  __stdcall GetBitmapBits(  HBITMAP,  LONG,  LPVOID);
__declspec(dllimport) BOOL  __stdcall GetBitmapDimensionEx(  HBITMAP,  LPSIZE);
__declspec(dllimport) UINT  __stdcall GetBoundsRect(  HDC,  LPRECT,  UINT);

__declspec(dllimport) BOOL  __stdcall GetBrushOrgEx(  HDC,  LPPOINT);

__declspec(dllimport) BOOL  __stdcall GetCharWidthA(  HDC,  UINT,  UINT,  LPINT);
__declspec(dllimport) BOOL  __stdcall GetCharWidthW(  HDC,  UINT,  UINT,  LPINT);




#line 3013 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall GetCharWidth32A(  HDC,  UINT,  UINT,   LPINT);
__declspec(dllimport) BOOL  __stdcall GetCharWidth32W(  HDC,  UINT,  UINT,   LPINT);




#line 3020 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall GetCharWidthFloatA(  HDC,  UINT,  UINT,  PFLOAT);
__declspec(dllimport) BOOL  __stdcall GetCharWidthFloatW(  HDC,  UINT,  UINT,  PFLOAT);




#line 3027 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall GetCharABCWidthsA(  HDC,  UINT,  UINT,  LPABC);
__declspec(dllimport) BOOL  __stdcall GetCharABCWidthsW(  HDC,  UINT,  UINT,  LPABC);




#line 3034 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall GetCharABCWidthsFloatA(  HDC,  UINT,  UINT,  LPABCFLOAT);
__declspec(dllimport) BOOL  __stdcall GetCharABCWidthsFloatW(  HDC,  UINT,  UINT,  LPABCFLOAT);




#line 3041 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) int   __stdcall GetClipBox(  HDC,   LPRECT);
__declspec(dllimport) int   __stdcall GetClipRgn(  HDC,  HRGN);
__declspec(dllimport) int   __stdcall GetMetaRgn(  HDC,  HRGN);
__declspec(dllimport) HGDIOBJ __stdcall GetCurrentObject(  HDC,  UINT);
__declspec(dllimport) BOOL  __stdcall GetCurrentPositionEx(  HDC,   LPPOINT);
__declspec(dllimport) int   __stdcall GetDeviceCaps(  HDC,  int);
__declspec(dllimport) int   __stdcall GetDIBits(  HDC,  HBITMAP,  UINT,  UINT,   LPVOID,   LPBITMAPINFO,  UINT);
__declspec(dllimport) DWORD __stdcall GetFontData(  HDC,  DWORD,  DWORD,  LPVOID,  DWORD);
__declspec(dllimport) DWORD __stdcall GetGlyphOutlineA(  HDC,  UINT,  UINT,  LPGLYPHMETRICS,  DWORD,  LPVOID,  const MAT2 *);
__declspec(dllimport) DWORD __stdcall GetGlyphOutlineW(  HDC,  UINT,  UINT,  LPGLYPHMETRICS,  DWORD,  LPVOID,  const MAT2 *);




#line 3056 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) int   __stdcall GetGraphicsMode(  HDC);
__declspec(dllimport) int   __stdcall GetMapMode(  HDC);
__declspec(dllimport) UINT  __stdcall GetMetaFileBitsEx(  HMETAFILE,  UINT,   LPVOID);
__declspec(dllimport) HMETAFILE   __stdcall GetMetaFileA(  LPCSTR);
__declspec(dllimport) HMETAFILE   __stdcall GetMetaFileW(  LPCWSTR);




#line 3066 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) COLORREF __stdcall GetNearestColor(  HDC,  COLORREF);
__declspec(dllimport) UINT  __stdcall GetNearestPaletteIndex(  HPALETTE,  COLORREF);
__declspec(dllimport) DWORD __stdcall GetObjectType(  HGDIOBJ h);



__declspec(dllimport) UINT __stdcall GetOutlineTextMetricsA(  HDC,  UINT,  LPOUTLINETEXTMETRICA);
__declspec(dllimport) UINT __stdcall GetOutlineTextMetricsW(  HDC,  UINT,  LPOUTLINETEXTMETRICW);




#line 3079 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

#line 3081 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) UINT  __stdcall GetPaletteEntries(  HPALETTE,  UINT,  UINT,  LPPALETTEENTRY);
__declspec(dllimport) COLORREF __stdcall GetPixel(  HDC,  int,  int);
__declspec(dllimport) int   __stdcall GetPixelFormat(  HDC);
__declspec(dllimport) int   __stdcall GetPolyFillMode(  HDC);
__declspec(dllimport) BOOL  __stdcall GetRasterizerCaps(  LPRASTERIZER_STATUS,  UINT);
__declspec(dllimport) int   __stdcall GetRandomRgn ( HDC,  HRGN,  INT);
__declspec(dllimport) DWORD __stdcall GetRegionData(  HRGN,  DWORD,   LPRGNDATA);
__declspec(dllimport) int   __stdcall GetRgnBox(  HRGN,   LPRECT);
__declspec(dllimport) HGDIOBJ __stdcall GetStockObject(  int);
__declspec(dllimport) int   __stdcall GetStretchBltMode(  HDC);
__declspec(dllimport) UINT  __stdcall GetSystemPaletteEntries(  HDC,  UINT,  UINT,  LPPALETTEENTRY);
__declspec(dllimport) UINT  __stdcall GetSystemPaletteUse(  HDC);
__declspec(dllimport) int   __stdcall GetTextCharacterExtra(  HDC);
__declspec(dllimport) UINT  __stdcall GetTextAlign(  HDC);
__declspec(dllimport) COLORREF __stdcall GetTextColor(  HDC);

__declspec(dllimport) BOOL  __stdcall GetTextExtentPointA(
                     HDC,
                     LPCSTR,
                     int,
                     LPSIZE
                    );
__declspec(dllimport) BOOL  __stdcall GetTextExtentPointW(
                     HDC,
                     LPCWSTR,
                     int,
                     LPSIZE
                    );




#line 3115 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) BOOL  __stdcall GetTextExtentPoint32A(
                     HDC,
                     LPCSTR,
                     int,
                     LPSIZE
                    );
__declspec(dllimport) BOOL  __stdcall GetTextExtentPoint32W(
                     HDC,
                     LPCWSTR,
                     int,
                     LPSIZE
                    );




#line 3133 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) BOOL  __stdcall GetTextExtentExPointA(
                     HDC,
                     LPCSTR,
                     int,
                     int,
                     LPINT,
                     LPINT,
                     LPSIZE
                    );
__declspec(dllimport) BOOL  __stdcall GetTextExtentExPointW(
                     HDC,
                     LPCWSTR,
                     int,
                     int,
                     LPINT,
                     LPINT,
                     LPSIZE
                    );




#line 3157 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) int __stdcall GetTextCharset(  HDC hdc);
__declspec(dllimport) int __stdcall GetTextCharsetInfo(  HDC hdc,  LPFONTSIGNATURE lpSig,  DWORD dwFlags);
__declspec(dllimport) BOOL __stdcall TranslateCharsetInfo(   DWORD  *lpSrc,   LPCHARSETINFO lpCs,  DWORD dwFlags);
__declspec(dllimport) DWORD __stdcall GetFontLanguageInfo(  HDC );
__declspec(dllimport) DWORD __stdcall GetCharacterPlacementA(   HDC,  LPCSTR,  int,  int,   LPGCP_RESULTSA,  DWORD);
__declspec(dllimport) DWORD __stdcall GetCharacterPlacementW(   HDC,  LPCWSTR,  int,  int,   LPGCP_RESULTSW,  DWORD);




#line 3169 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 3170 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
















































































































































































#line 3347 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


__declspec(dllimport) BOOL  __stdcall GetViewportExtEx(  HDC,  LPSIZE);
__declspec(dllimport) BOOL  __stdcall GetViewportOrgEx(  HDC,  LPPOINT);
__declspec(dllimport) BOOL  __stdcall GetWindowExtEx(  HDC,  LPSIZE);
__declspec(dllimport) BOOL  __stdcall GetWindowOrgEx(  HDC,  LPPOINT);

__declspec(dllimport) int  __stdcall IntersectClipRect(  HDC,  int,  int,  int,  int);
__declspec(dllimport) BOOL __stdcall InvertRgn(  HDC,  HRGN);
__declspec(dllimport) BOOL __stdcall LineDDA(  int,  int,  int,  int,  LINEDDAPROC,  LPARAM);
__declspec(dllimport) BOOL __stdcall LineTo(  HDC,  int,  int);
__declspec(dllimport) BOOL __stdcall MaskBlt(  HDC,  int,  int,  int,  int,
               HDC,  int,  int,  HBITMAP,  int,  int,  DWORD);
__declspec(dllimport) BOOL __stdcall PlgBlt(  HDC,  const POINT *,  HDC,  int,  int,  int,
                      int,  HBITMAP,  int,  int);

__declspec(dllimport) int  __stdcall OffsetClipRgn( HDC,  int,  int);
__declspec(dllimport) int  __stdcall OffsetRgn( HRGN,  int,  int);
__declspec(dllimport) BOOL __stdcall PatBlt( HDC,  int,  int,  int,  int,  DWORD);
__declspec(dllimport) BOOL __stdcall Pie( HDC,  int,  int,  int,  int,  int,  int,  int,  int);
__declspec(dllimport) BOOL __stdcall PlayMetaFile( HDC,  HMETAFILE);
__declspec(dllimport) BOOL __stdcall PaintRgn( HDC,  HRGN);
__declspec(dllimport) BOOL __stdcall PolyPolygon( HDC,  const POINT *,  const INT *,  int);
__declspec(dllimport) BOOL __stdcall PtInRegion( HRGN,  int,  int);
__declspec(dllimport) BOOL __stdcall PtVisible( HDC,  int,  int);
__declspec(dllimport) BOOL __stdcall RectInRegion( HRGN,  const RECT *);
__declspec(dllimport) BOOL __stdcall RectVisible( HDC,  const RECT *);
__declspec(dllimport) BOOL __stdcall Rectangle( HDC,  int,  int,  int,  int);
__declspec(dllimport) BOOL __stdcall RestoreDC( HDC,  int);
__declspec(dllimport) HDC  __stdcall ResetDCA( HDC,  const DEVMODEA *);
__declspec(dllimport) HDC  __stdcall ResetDCW( HDC,  const DEVMODEW *);




#line 3383 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) UINT __stdcall RealizePalette( HDC);
__declspec(dllimport) BOOL __stdcall RemoveFontResourceA( LPCSTR);
__declspec(dllimport) BOOL __stdcall RemoveFontResourceW( LPCWSTR);




#line 3391 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall RoundRect( HDC,  int,  int,  int,  int,  int,  int);
__declspec(dllimport) BOOL __stdcall ResizePalette( HPALETTE,  UINT);

__declspec(dllimport) int  __stdcall SaveDC( HDC);
__declspec(dllimport) int  __stdcall SelectClipRgn( HDC,  HRGN);
__declspec(dllimport) int  __stdcall ExtSelectClipRgn( HDC,  HRGN,  int);
__declspec(dllimport) int  __stdcall SetMetaRgn( HDC);
__declspec(dllimport) HGDIOBJ __stdcall SelectObject( HDC,  HGDIOBJ);
__declspec(dllimport) HPALETTE __stdcall SelectPalette( HDC,  HPALETTE,  BOOL);
__declspec(dllimport) COLORREF __stdcall SetBkColor( HDC,  COLORREF);




#line 3406 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


__declspec(dllimport) int   __stdcall SetBkMode( HDC,  int);
__declspec(dllimport) LONG  __stdcall SetBitmapBits( HBITMAP,  DWORD,  const void *);

__declspec(dllimport) UINT  __stdcall SetBoundsRect( HDC,  const RECT *,  UINT);
__declspec(dllimport) int   __stdcall SetDIBits( HDC,  HBITMAP,  UINT,  UINT,  const void *,  const BITMAPINFO *,  UINT);
__declspec(dllimport) int   __stdcall SetDIBitsToDevice( HDC,  int,  int,  DWORD,  DWORD,  int,
         int,  UINT,  UINT,  const void *,  const BITMAPINFO *,  UINT);
__declspec(dllimport) DWORD __stdcall SetMapperFlags( HDC,  DWORD);
__declspec(dllimport) int   __stdcall SetGraphicsMode( HDC hdc,  int iMode);
__declspec(dllimport) int   __stdcall SetMapMode( HDC,  int);


__declspec(dllimport) DWORD __stdcall SetLayout( HDC,  DWORD);
__declspec(dllimport) DWORD __stdcall GetLayout( HDC);
#line 3423 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) HMETAFILE   __stdcall SetMetaFileBitsEx( UINT,  const BYTE *);
__declspec(dllimport) UINT  __stdcall SetPaletteEntries( HPALETTE,  UINT,  UINT,  const PALETTEENTRY *);
__declspec(dllimport) COLORREF __stdcall SetPixel( HDC,  int,  int,  COLORREF);
__declspec(dllimport) BOOL   __stdcall SetPixelV( HDC,  int,  int,  COLORREF);
__declspec(dllimport) BOOL  __stdcall SetPixelFormat( HDC,  int,  const PIXELFORMATDESCRIPTOR *);
__declspec(dllimport) int   __stdcall SetPolyFillMode( HDC,  int);
__declspec(dllimport) BOOL   __stdcall StretchBlt( HDC,  int,  int,  int,  int,  HDC,  int,  int,  int,  int,  DWORD);
__declspec(dllimport) BOOL   __stdcall SetRectRgn( HRGN,  int,  int,  int,  int);
__declspec(dllimport) int   __stdcall StretchDIBits( HDC,  int,  int,  int,  int,  int,  int,  int,  int,  const
        void *,  const BITMAPINFO *,  UINT,  DWORD);
__declspec(dllimport) int   __stdcall SetROP2( HDC,  int);
__declspec(dllimport) int   __stdcall SetStretchBltMode( HDC,  int);
__declspec(dllimport) UINT  __stdcall SetSystemPaletteUse( HDC,  UINT);
__declspec(dllimport) int   __stdcall SetTextCharacterExtra( HDC,  int);
__declspec(dllimport) COLORREF __stdcall SetTextColor( HDC,  COLORREF);
__declspec(dllimport) UINT  __stdcall SetTextAlign( HDC,  UINT);
__declspec(dllimport) BOOL  __stdcall SetTextJustification( HDC,  int,  int);
__declspec(dllimport) BOOL  __stdcall UpdateColors( HDC);








typedef USHORT COLOR16;

typedef struct _TRIVERTEX
{
    LONG    x;
    LONG    y;
    COLOR16 Red;
    COLOR16 Green;
    COLOR16 Blue;
    COLOR16 Alpha;
}TRIVERTEX,*PTRIVERTEX,*LPTRIVERTEX;

typedef struct _GRADIENT_TRIANGLE
{
    ULONG Vertex1;
    ULONG Vertex2;
    ULONG Vertex3;
} GRADIENT_TRIANGLE,*PGRADIENT_TRIANGLE,*LPGRADIENT_TRIANGLE;

typedef struct _GRADIENT_RECT
{
    ULONG UpperLeft;
    ULONG LowerRight;
}GRADIENT_RECT,*PGRADIENT_RECT,*LPGRADIENT_RECT;

typedef struct _BLENDFUNCTION
{
    BYTE   BlendOp;
    BYTE   BlendFlags;
    BYTE   SourceConstantAlpha;
    BYTE   AlphaFormat;
}BLENDFUNCTION,*PBLENDFUNCTION;














__declspec(dllimport) BOOL  __stdcall AlphaBlend(  HDC,  int,  int,  int,  int,  HDC,  int,  int,  int,  int,  BLENDFUNCTION);

__declspec(dllimport) BOOL  __stdcall TransparentBlt( HDC, int, int, int, int, HDC, int, int, int, int, UINT);











__declspec(dllimport) BOOL  __stdcall GradientFill(  HDC,  PTRIVERTEX,  ULONG,  PVOID,  ULONG,  ULONG);

#line 3513 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




__declspec(dllimport) BOOL  __stdcall PlayMetaFileRecord(  HDC,  LPHANDLETABLE,  LPMETARECORD,  UINT);
typedef int (__stdcall* MFENUMPROC)(  HDC,  HANDLETABLE *,  METARECORD *,  int,  LPARAM);
__declspec(dllimport) BOOL  __stdcall EnumMetaFile(   HDC,  HMETAFILE,  MFENUMPROC,  LPARAM);

typedef int (__stdcall* ENHMFENUMPROC)(HDC, HANDLETABLE *, const ENHMETARECORD *, int, LPARAM);



__declspec(dllimport) HENHMETAFILE __stdcall CloseEnhMetaFile(  HDC);
__declspec(dllimport) HENHMETAFILE __stdcall CopyEnhMetaFileA(  HENHMETAFILE,  LPCSTR);
__declspec(dllimport) HENHMETAFILE __stdcall CopyEnhMetaFileW(  HENHMETAFILE,  LPCWSTR);




#line 3533 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) HDC   __stdcall CreateEnhMetaFileA(  HDC,  LPCSTR,  const RECT *,  LPCSTR);
__declspec(dllimport) HDC   __stdcall CreateEnhMetaFileW(  HDC,  LPCWSTR,  const RECT *,  LPCWSTR);




#line 3540 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall DeleteEnhMetaFile(  HENHMETAFILE);
__declspec(dllimport) BOOL  __stdcall EnumEnhMetaFile(  HDC,  HENHMETAFILE,  ENHMFENUMPROC,
                                         LPVOID,  const RECT *);
__declspec(dllimport) HENHMETAFILE  __stdcall GetEnhMetaFileA(  LPCSTR);
__declspec(dllimport) HENHMETAFILE  __stdcall GetEnhMetaFileW(  LPCWSTR);




#line 3550 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) UINT  __stdcall GetEnhMetaFileBits(  HENHMETAFILE,  UINT,  LPBYTE);
__declspec(dllimport) UINT  __stdcall GetEnhMetaFileDescriptionA(  HENHMETAFILE,  UINT,   LPSTR );
__declspec(dllimport) UINT  __stdcall GetEnhMetaFileDescriptionW(  HENHMETAFILE,  UINT,   LPWSTR );




#line 3558 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) UINT  __stdcall GetEnhMetaFileHeader(  HENHMETAFILE,  UINT,  LPENHMETAHEADER );
__declspec(dllimport) UINT  __stdcall GetEnhMetaFilePaletteEntries(  HENHMETAFILE,  UINT,  LPPALETTEENTRY );
__declspec(dllimport) UINT  __stdcall GetEnhMetaFilePixelFormat(  HENHMETAFILE,  UINT,
                                                   PIXELFORMATDESCRIPTOR *);
__declspec(dllimport) UINT  __stdcall GetWinMetaFileBits(  HENHMETAFILE,  UINT,  LPBYTE,  INT,  HDC);
__declspec(dllimport) BOOL  __stdcall PlayEnhMetaFile(  HDC,  HENHMETAFILE,  const RECT *);
__declspec(dllimport) BOOL  __stdcall PlayEnhMetaFileRecord(  HDC,  LPHANDLETABLE,  const ENHMETARECORD *,  UINT);
__declspec(dllimport) HENHMETAFILE  __stdcall SetEnhMetaFileBits(  UINT,  const BYTE *);
__declspec(dllimport) HENHMETAFILE  __stdcall SetWinMetaFileBits(  UINT,  const BYTE *,  HDC,  const METAFILEPICT *);
__declspec(dllimport) BOOL  __stdcall GdiComment(  HDC,  UINT,  const BYTE *);

#line 3570 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



__declspec(dllimport) BOOL __stdcall GetTextMetricsA(  HDC,  LPTEXTMETRICA);
__declspec(dllimport) BOOL __stdcall GetTextMetricsW(  HDC,  LPTEXTMETRICW);




#line 3580 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

#line 3582 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



typedef struct tagDIBSECTION {
    BITMAP              dsBm;
    BITMAPINFOHEADER    dsBmih;
    DWORD               dsBitfields[3];
    HANDLE              dshSection;
    DWORD               dsOffset;
} DIBSECTION,  *LPDIBSECTION, *PDIBSECTION;

__declspec(dllimport) BOOL __stdcall AngleArc(  HDC,  int,  int,  DWORD,  FLOAT,  FLOAT);
__declspec(dllimport) BOOL __stdcall PolyPolyline(  HDC,  const POINT *,  const DWORD *,  DWORD);
__declspec(dllimport) BOOL __stdcall GetWorldTransform(  HDC,  LPXFORM);
__declspec(dllimport) BOOL __stdcall SetWorldTransform(  HDC,  const XFORM *);
__declspec(dllimport) BOOL __stdcall ModifyWorldTransform(  HDC,  const XFORM *,  DWORD);
__declspec(dllimport) BOOL __stdcall CombineTransform(  LPXFORM,  const XFORM *,  const XFORM *);
__declspec(dllimport) HBITMAP __stdcall CreateDIBSection(  HDC,  const BITMAPINFO *,  UINT,  void **,  HANDLE,  DWORD);
__declspec(dllimport) UINT __stdcall GetDIBColorTable(  HDC,  UINT,  UINT,  RGBQUAD *);
__declspec(dllimport) UINT __stdcall SetDIBColorTable(  HDC,  UINT,  UINT,  const RGBQUAD *);




































typedef struct  tagCOLORADJUSTMENT {
    WORD   caSize;
    WORD   caFlags;
    WORD   caIlluminantIndex;
    WORD   caRedGamma;
    WORD   caGreenGamma;
    WORD   caBlueGamma;
    WORD   caReferenceBlack;
    WORD   caReferenceWhite;
    SHORT  caContrast;
    SHORT  caBrightness;
    SHORT  caColorfulness;
    SHORT  caRedGreenTint;
} COLORADJUSTMENT, *PCOLORADJUSTMENT,  *LPCOLORADJUSTMENT;

__declspec(dllimport) BOOL __stdcall SetColorAdjustment(  HDC,  const COLORADJUSTMENT *);
__declspec(dllimport) BOOL __stdcall GetColorAdjustment(  HDC,  LPCOLORADJUSTMENT);
__declspec(dllimport) HPALETTE __stdcall CreateHalftonePalette(  HDC);


typedef BOOL (__stdcall* ABORTPROC)(  HDC,  int);


#line 3662 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

typedef struct _DOCINFOA {
    int     cbSize;
    LPCSTR   lpszDocName;
    LPCSTR   lpszOutput;

    LPCSTR   lpszDatatype;
    DWORD    fwType;
#line 3671 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
} DOCINFOA, *LPDOCINFOA;
typedef struct _DOCINFOW {
    int     cbSize;
    LPCWSTR  lpszDocName;
    LPCWSTR  lpszOutput;

    LPCWSTR  lpszDatatype;
    DWORD    fwType;
#line 3680 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
} DOCINFOW, *LPDOCINFOW;




typedef DOCINFOA DOCINFO;
typedef LPDOCINFOA LPDOCINFO;
#line 3688 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




#line 3693 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) int __stdcall StartDocA( HDC,  const DOCINFOA *);
__declspec(dllimport) int __stdcall StartDocW( HDC,  const DOCINFOW *);




#line 3701 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) int __stdcall EndDoc( HDC);
__declspec(dllimport) int __stdcall StartPage( HDC);
__declspec(dllimport) int __stdcall EndPage( HDC);
__declspec(dllimport) int __stdcall AbortDoc( HDC);
__declspec(dllimport) int __stdcall SetAbortProc( HDC,  ABORTPROC);

__declspec(dllimport) BOOL __stdcall AbortPath( HDC);
__declspec(dllimport) BOOL __stdcall ArcTo( HDC,  int,  int,  int,  int,  int,  int,  int,  int);
__declspec(dllimport) BOOL __stdcall BeginPath( HDC);
__declspec(dllimport) BOOL __stdcall CloseFigure( HDC);
__declspec(dllimport) BOOL __stdcall EndPath( HDC);
__declspec(dllimport) BOOL __stdcall FillPath( HDC);
__declspec(dllimport) BOOL __stdcall FlattenPath( HDC);
__declspec(dllimport) int  __stdcall GetPath( HDC,  LPPOINT,  LPBYTE,  int);
__declspec(dllimport) HRGN __stdcall PathToRegion( HDC);
__declspec(dllimport) BOOL __stdcall PolyDraw( HDC,  const POINT *,  const BYTE *,  int);
__declspec(dllimport) BOOL __stdcall SelectClipPath( HDC,  int);
__declspec(dllimport) int  __stdcall SetArcDirection( HDC,  int);
__declspec(dllimport) BOOL __stdcall SetMiterLimit( HDC,  FLOAT,  PFLOAT);
__declspec(dllimport) BOOL __stdcall StrokeAndFillPath( HDC);
__declspec(dllimport) BOOL __stdcall StrokePath( HDC);
__declspec(dllimport) BOOL __stdcall WidenPath( HDC);
__declspec(dllimport) HPEN __stdcall ExtCreatePen( DWORD,  DWORD,  const LOGBRUSH *,  DWORD,  const DWORD *);
__declspec(dllimport) BOOL __stdcall GetMiterLimit( HDC,  PFLOAT);
__declspec(dllimport) int  __stdcall GetArcDirection( HDC);

__declspec(dllimport) int   __stdcall GetObjectA(  HGDIOBJ,  int,  LPVOID);
__declspec(dllimport) int   __stdcall GetObjectW(  HGDIOBJ,  int,  LPVOID);




#line 3734 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall MoveToEx(  HDC,  int,  int,  LPPOINT);
__declspec(dllimport) BOOL  __stdcall TextOutA(  HDC,  int,  int,  LPCSTR,  int);
__declspec(dllimport) BOOL  __stdcall TextOutW(  HDC,  int,  int,  LPCWSTR,  int);




#line 3742 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall ExtTextOutA(  HDC,  int,  int,  UINT,  const RECT *,  LPCSTR,  UINT,  const INT *);
__declspec(dllimport) BOOL  __stdcall ExtTextOutW(  HDC,  int,  int,  UINT,  const RECT *,  LPCWSTR,  UINT,  const INT *);




#line 3749 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall PolyTextOutA(  HDC,  const POLYTEXTA *,  int);
__declspec(dllimport) BOOL  __stdcall PolyTextOutW(  HDC,  const POLYTEXTW *,  int);




#line 3756 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) HRGN  __stdcall CreatePolygonRgn(  const POINT *,  int,  int);
__declspec(dllimport) BOOL  __stdcall DPtoLP(  HDC,   LPPOINT,  int);
__declspec(dllimport) BOOL  __stdcall LPtoDP(  HDC,   LPPOINT,  int);
__declspec(dllimport) BOOL  __stdcall Polygon(  HDC,  const POINT *,  int);
__declspec(dllimport) BOOL  __stdcall Polyline(  HDC,  const POINT *,  int);

__declspec(dllimport) BOOL  __stdcall PolyBezier(  HDC,  const POINT *,  DWORD);
__declspec(dllimport) BOOL  __stdcall PolyBezierTo(  HDC,  const POINT *,  DWORD);
__declspec(dllimport) BOOL  __stdcall PolylineTo(  HDC,  const POINT *,  DWORD);

__declspec(dllimport) BOOL  __stdcall SetViewportExtEx(  HDC,  int,  int,  LPSIZE);
__declspec(dllimport) BOOL  __stdcall SetViewportOrgEx(  HDC,  int,  int,  LPPOINT);
__declspec(dllimport) BOOL  __stdcall SetWindowExtEx(  HDC,  int,  int,  LPSIZE);
__declspec(dllimport) BOOL  __stdcall SetWindowOrgEx(  HDC,  int,  int,  LPPOINT);

__declspec(dllimport) BOOL  __stdcall OffsetViewportOrgEx(  HDC,  int,  int,  LPPOINT);
__declspec(dllimport) BOOL  __stdcall OffsetWindowOrgEx(  HDC,  int,  int,  LPPOINT);
__declspec(dllimport) BOOL  __stdcall ScaleViewportExtEx(  HDC,  int,  int,  int,  int,  LPSIZE);
__declspec(dllimport) BOOL  __stdcall ScaleWindowExtEx(  HDC,  int,  int,  int,  int,  LPSIZE);
__declspec(dllimport) BOOL  __stdcall SetBitmapDimensionEx(  HBITMAP,  int,  int,  LPSIZE);
__declspec(dllimport) BOOL  __stdcall SetBrushOrgEx(  HDC,  int,  int,  LPPOINT);

__declspec(dllimport) int   __stdcall GetTextFaceA(  HDC,  int,  LPSTR);
__declspec(dllimport) int   __stdcall GetTextFaceW(  HDC,  int,  LPWSTR);




#line 3786 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



typedef struct tagKERNINGPAIR {
   WORD wFirst;
   WORD wSecond;
   int  iKernAmount;
} KERNINGPAIR, *LPKERNINGPAIR;

__declspec(dllimport) DWORD __stdcall GetKerningPairsA(  HDC,  DWORD,  LPKERNINGPAIR);
__declspec(dllimport) DWORD __stdcall GetKerningPairsW(  HDC,  DWORD,  LPKERNINGPAIR);




#line 3802 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) BOOL  __stdcall GetDCOrgEx(  HDC,  LPPOINT);
__declspec(dllimport) BOOL  __stdcall FixBrushOrgEx(  HDC,  int,  int,  LPPOINT);
__declspec(dllimport) BOOL  __stdcall UnrealizeObject(  HGDIOBJ);

__declspec(dllimport) BOOL  __stdcall GdiFlush();
__declspec(dllimport) DWORD __stdcall GdiSetBatchLimit(  DWORD);
__declspec(dllimport) DWORD __stdcall GdiGetBatchLimit();









typedef int (__stdcall* ICMENUMPROCA)(LPSTR, LPARAM);
typedef int (__stdcall* ICMENUMPROCW)(LPWSTR, LPARAM);




#line 3826 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

__declspec(dllimport) int         __stdcall SetICMMode(  HDC,  int);
__declspec(dllimport) BOOL        __stdcall CheckColorsInGamut(  HDC,  LPVOID,  LPVOID,  DWORD);
__declspec(dllimport) HCOLORSPACE __stdcall GetColorSpace(  HDC);
__declspec(dllimport) BOOL        __stdcall GetLogColorSpaceA(  HCOLORSPACE,  LPLOGCOLORSPACEA,  DWORD);
__declspec(dllimport) BOOL        __stdcall GetLogColorSpaceW(  HCOLORSPACE,  LPLOGCOLORSPACEW,  DWORD);




#line 3837 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) HCOLORSPACE __stdcall CreateColorSpaceA(  LPLOGCOLORSPACEA);
__declspec(dllimport) HCOLORSPACE __stdcall CreateColorSpaceW(  LPLOGCOLORSPACEW);




#line 3844 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) HCOLORSPACE __stdcall SetColorSpace(  HDC,  HCOLORSPACE);
__declspec(dllimport) BOOL        __stdcall DeleteColorSpace(  HCOLORSPACE);
__declspec(dllimport) BOOL        __stdcall GetICMProfileA(  HDC,   LPDWORD,  LPSTR);
__declspec(dllimport) BOOL        __stdcall GetICMProfileW(  HDC,   LPDWORD,  LPWSTR);




#line 3853 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL        __stdcall SetICMProfileA(  HDC,  LPSTR);
__declspec(dllimport) BOOL        __stdcall SetICMProfileW(  HDC,  LPWSTR);




#line 3860 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL        __stdcall GetDeviceGammaRamp(  HDC,  LPVOID);
__declspec(dllimport) BOOL        __stdcall SetDeviceGammaRamp(  HDC,  LPVOID);
__declspec(dllimport) BOOL        __stdcall ColorMatchToTarget(  HDC,  HDC,  DWORD);
__declspec(dllimport) int         __stdcall EnumICMProfilesA(  HDC,  ICMENUMPROCA,  LPARAM);
__declspec(dllimport) int         __stdcall EnumICMProfilesW(  HDC,  ICMENUMPROCW,  LPARAM);




#line 3870 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL        __stdcall UpdateICMRegKeyA(  DWORD,  LPSTR,  LPSTR,  UINT);
__declspec(dllimport) BOOL        __stdcall UpdateICMRegKeyW(  DWORD,  LPWSTR,  LPWSTR,  UINT);




#line 3877 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
#line 3878 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


__declspec(dllimport) BOOL        __stdcall ColorCorrectPalette(  HDC,  HPALETTE,  DWORD,  DWORD);
#line 3882 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"









#line 3892 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




















































































































#line 4009 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




















#line 4030 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"









#line 4040 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



typedef struct tagEMR
{
    DWORD   iType;              
    DWORD   nSize;              
                                
} EMR, *PEMR;



typedef struct tagEMRTEXT
{
    POINTL  ptlReference;
    DWORD   nChars;
    DWORD   offString;          
    DWORD   fOptions;
    RECTL   rcl;
    DWORD   offDx;              
                                
} EMRTEXT, *PEMRTEXT;



typedef struct tagABORTPATH
{
    EMR     emr;
} EMRABORTPATH,      *PEMRABORTPATH,
  EMRBEGINPATH,      *PEMRBEGINPATH,
  EMRENDPATH,        *PEMRENDPATH,
  EMRCLOSEFIGURE,    *PEMRCLOSEFIGURE,
  EMRFLATTENPATH,    *PEMRFLATTENPATH,
  EMRWIDENPATH,      *PEMRWIDENPATH,
  EMRSETMETARGN,     *PEMRSETMETARGN,
  EMRSAVEDC,         *PEMRSAVEDC,
  EMRREALIZEPALETTE, *PEMRREALIZEPALETTE;

typedef struct tagEMRSELECTCLIPPATH
{
    EMR     emr;
    DWORD   iMode;
} EMRSELECTCLIPPATH,    *PEMRSELECTCLIPPATH,
  EMRSETBKMODE,         *PEMRSETBKMODE,
  EMRSETMAPMODE,        *PEMRSETMAPMODE,

  EMRSETLAYOUT,         *PEMRSETLAYOUT,
#line 4088 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
  EMRSETPOLYFILLMODE,   *PEMRSETPOLYFILLMODE,
  EMRSETROP2,           *PEMRSETROP2,
  EMRSETSTRETCHBLTMODE, *PEMRSETSTRETCHBLTMODE,
  EMRSETICMMODE,        *PEMRSETICMMODE,
  EMRSETTEXTALIGN,      *PEMRSETTEXTALIGN;

typedef struct tagEMRSETMITERLIMIT
{
    EMR     emr;
    FLOAT   eMiterLimit;
} EMRSETMITERLIMIT, *PEMRSETMITERLIMIT;

typedef struct tagEMRRESTOREDC
{
    EMR     emr;
    LONG    iRelative;          
} EMRRESTOREDC, *PEMRRESTOREDC;

typedef struct tagEMRSETARCDIRECTION
{
    EMR     emr;
    DWORD   iArcDirection;      
                                
} EMRSETARCDIRECTION, *PEMRSETARCDIRECTION;

typedef struct tagEMRSETMAPPERFLAGS
{
    EMR     emr;
    DWORD   dwFlags;
} EMRSETMAPPERFLAGS, *PEMRSETMAPPERFLAGS;

typedef struct tagEMRSETTEXTCOLOR
{
    EMR     emr;
    COLORREF crColor;
} EMRSETBKCOLOR,   *PEMRSETBKCOLOR,
  EMRSETTEXTCOLOR, *PEMRSETTEXTCOLOR;

typedef struct tagEMRSELECTOBJECT
{
    EMR     emr;
    DWORD   ihObject;           
} EMRSELECTOBJECT, *PEMRSELECTOBJECT,
  EMRDELETEOBJECT, *PEMRDELETEOBJECT;

typedef struct tagEMRSELECTPALETTE
{
    EMR     emr;
    DWORD   ihPal;              
} EMRSELECTPALETTE, *PEMRSELECTPALETTE;

typedef struct tagEMRRESIZEPALETTE
{
    EMR     emr;
    DWORD   ihPal;              
    DWORD   cEntries;
} EMRRESIZEPALETTE, *PEMRRESIZEPALETTE;

typedef struct tagEMRSETPALETTEENTRIES
{
    EMR     emr;
    DWORD   ihPal;              
    DWORD   iStart;
    DWORD   cEntries;
    PALETTEENTRY aPalEntries[1];
} EMRSETPALETTEENTRIES, *PEMRSETPALETTEENTRIES;

typedef struct tagEMRSETCOLORADJUSTMENT
{
    EMR     emr;
    COLORADJUSTMENT ColorAdjustment;
} EMRSETCOLORADJUSTMENT, *PEMRSETCOLORADJUSTMENT;

typedef struct tagEMRGDICOMMENT
{
    EMR     emr;
    DWORD   cbData;             
    BYTE    Data[1];
} EMRGDICOMMENT, *PEMRGDICOMMENT;

typedef struct tagEMREOF
{
    EMR     emr;
    DWORD   nPalEntries;        
    DWORD   offPalEntries;      
    DWORD   nSizeLast;          
                                
                                
} EMREOF, *PEMREOF;

typedef struct tagEMRLINETO
{
    EMR     emr;
    POINTL  ptl;
} EMRLINETO,   *PEMRLINETO,
  EMRMOVETOEX, *PEMRMOVETOEX;

typedef struct tagEMROFFSETCLIPRGN
{
    EMR     emr;
    POINTL  ptlOffset;
} EMROFFSETCLIPRGN, *PEMROFFSETCLIPRGN;

typedef struct tagEMRFILLPATH
{
    EMR     emr;
    RECTL   rclBounds;          
} EMRFILLPATH,          *PEMRFILLPATH,
  EMRSTROKEANDFILLPATH, *PEMRSTROKEANDFILLPATH,
  EMRSTROKEPATH,        *PEMRSTROKEPATH;

typedef struct tagEMREXCLUDECLIPRECT
{
    EMR     emr;
    RECTL   rclClip;
} EMREXCLUDECLIPRECT,   *PEMREXCLUDECLIPRECT,
  EMRINTERSECTCLIPRECT, *PEMRINTERSECTCLIPRECT;

typedef struct tagEMRSETVIEWPORTORGEX
{
    EMR     emr;
    POINTL  ptlOrigin;
} EMRSETVIEWPORTORGEX, *PEMRSETVIEWPORTORGEX,
  EMRSETWINDOWORGEX,   *PEMRSETWINDOWORGEX,
  EMRSETBRUSHORGEX,    *PEMRSETBRUSHORGEX;

typedef struct tagEMRSETVIEWPORTEXTEX
{
    EMR     emr;
    SIZEL   szlExtent;
} EMRSETVIEWPORTEXTEX, *PEMRSETVIEWPORTEXTEX,
  EMRSETWINDOWEXTEX,   *PEMRSETWINDOWEXTEX;

typedef struct tagEMRSCALEVIEWPORTEXTEX
{
    EMR     emr;
    LONG    xNum;
    LONG    xDenom;
    LONG    yNum;
    LONG    yDenom;
} EMRSCALEVIEWPORTEXTEX, *PEMRSCALEVIEWPORTEXTEX,
  EMRSCALEWINDOWEXTEX,   *PEMRSCALEWINDOWEXTEX;

typedef struct tagEMRSETWORLDTRANSFORM
{
    EMR     emr;
    XFORM   xform;
} EMRSETWORLDTRANSFORM, *PEMRSETWORLDTRANSFORM;

typedef struct tagEMRMODIFYWORLDTRANSFORM
{
    EMR     emr;
    XFORM   xform;
    DWORD   iMode;
} EMRMODIFYWORLDTRANSFORM, *PEMRMODIFYWORLDTRANSFORM;

typedef struct tagEMRSETPIXELV
{
    EMR     emr;
    POINTL  ptlPixel;
    COLORREF crColor;
} EMRSETPIXELV, *PEMRSETPIXELV;

typedef struct tagEMREXTFLOODFILL
{
    EMR     emr;
    POINTL  ptlStart;
    COLORREF crColor;
    DWORD   iMode;
} EMREXTFLOODFILL, *PEMREXTFLOODFILL;

typedef struct tagEMRELLIPSE
{
    EMR     emr;
    RECTL   rclBox;             
} EMRELLIPSE,  *PEMRELLIPSE,
  EMRRECTANGLE, *PEMRRECTANGLE;


typedef struct tagEMRROUNDRECT
{
    EMR     emr;
    RECTL   rclBox;             
    SIZEL   szlCorner;
} EMRROUNDRECT, *PEMRROUNDRECT;

typedef struct tagEMRARC
{
    EMR     emr;
    RECTL   rclBox;             
    POINTL  ptlStart;
    POINTL  ptlEnd;
} EMRARC,   *PEMRARC,
  EMRARCTO, *PEMRARCTO,
  EMRCHORD, *PEMRCHORD,
  EMRPIE,   *PEMRPIE;

typedef struct tagEMRANGLEARC
{
    EMR     emr;
    POINTL  ptlCenter;
    DWORD   nRadius;
    FLOAT   eStartAngle;
    FLOAT   eSweepAngle;
} EMRANGLEARC, *PEMRANGLEARC;

typedef struct tagEMRPOLYLINE
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   cptl;
    POINTL  aptl[1];
} EMRPOLYLINE,     *PEMRPOLYLINE,
  EMRPOLYBEZIER,   *PEMRPOLYBEZIER,
  EMRPOLYGON,      *PEMRPOLYGON,
  EMRPOLYBEZIERTO, *PEMRPOLYBEZIERTO,
  EMRPOLYLINETO,   *PEMRPOLYLINETO;

typedef struct tagEMRPOLYLINE16
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   cpts;
    POINTS  apts[1];
} EMRPOLYLINE16,     *PEMRPOLYLINE16,
  EMRPOLYBEZIER16,   *PEMRPOLYBEZIER16,
  EMRPOLYGON16,      *PEMRPOLYGON16,
  EMRPOLYBEZIERTO16, *PEMRPOLYBEZIERTO16,
  EMRPOLYLINETO16,   *PEMRPOLYLINETO16;

typedef struct tagEMRPOLYDRAW
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   cptl;               
    POINTL  aptl[1];            
    BYTE    abTypes[1];         
} EMRPOLYDRAW, *PEMRPOLYDRAW;

typedef struct tagEMRPOLYDRAW16
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   cpts;               
    POINTS  apts[1];            
    BYTE    abTypes[1];         
} EMRPOLYDRAW16, *PEMRPOLYDRAW16;

typedef struct tagEMRPOLYPOLYLINE
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   nPolys;             
    DWORD   cptl;               
    DWORD   aPolyCounts[1];     
    POINTL  aptl[1];            
} EMRPOLYPOLYLINE, *PEMRPOLYPOLYLINE,
  EMRPOLYPOLYGON,  *PEMRPOLYPOLYGON;

typedef struct tagEMRPOLYPOLYLINE16
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   nPolys;             
    DWORD   cpts;               
    DWORD   aPolyCounts[1];     
    POINTS  apts[1];            
} EMRPOLYPOLYLINE16, *PEMRPOLYPOLYLINE16,
  EMRPOLYPOLYGON16,  *PEMRPOLYPOLYGON16;

typedef struct tagEMRINVERTRGN
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   cbRgnData;          
    BYTE    RgnData[1];
} EMRINVERTRGN, *PEMRINVERTRGN,
  EMRPAINTRGN,  *PEMRPAINTRGN;

typedef struct tagEMRFILLRGN
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   cbRgnData;          
    DWORD   ihBrush;            
    BYTE    RgnData[1];
} EMRFILLRGN, *PEMRFILLRGN;

typedef struct tagEMRFRAMERGN
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   cbRgnData;          
    DWORD   ihBrush;            
    SIZEL   szlStroke;
    BYTE    RgnData[1];
} EMRFRAMERGN, *PEMRFRAMERGN;

typedef struct tagEMREXTSELECTCLIPRGN
{
    EMR     emr;
    DWORD   cbRgnData;          
    DWORD   iMode;
    BYTE    RgnData[1];
} EMREXTSELECTCLIPRGN, *PEMREXTSELECTCLIPRGN;

typedef struct tagEMREXTTEXTOUTA
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   iGraphicsMode;      
    FLOAT   exScale;            
    FLOAT   eyScale;            
    EMRTEXT emrtext;            
                                
} EMREXTTEXTOUTA, *PEMREXTTEXTOUTA,
  EMREXTTEXTOUTW, *PEMREXTTEXTOUTW;

typedef struct tagEMRPOLYTEXTOUTA
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   iGraphicsMode;      
    FLOAT   exScale;            
    FLOAT   eyScale;            
    LONG    cStrings;
    EMRTEXT aemrtext[1];        
                                
} EMRPOLYTEXTOUTA, *PEMRPOLYTEXTOUTA,
  EMRPOLYTEXTOUTW, *PEMRPOLYTEXTOUTW;

typedef struct tagEMRBITBLT
{
    EMR     emr;
    RECTL   rclBounds;          
    LONG    xDest;
    LONG    yDest;
    LONG    cxDest;
    LONG    cyDest;
    DWORD   dwRop;
    LONG    xSrc;
    LONG    ySrc;
    XFORM   xformSrc;           
    COLORREF crBkColorSrc;      
    DWORD   iUsageSrc;          
                                
    DWORD   offBmiSrc;          
    DWORD   cbBmiSrc;           
    DWORD   offBitsSrc;         
    DWORD   cbBitsSrc;          
} EMRBITBLT, *PEMRBITBLT;

typedef struct tagEMRSTRETCHBLT
{
    EMR     emr;
    RECTL   rclBounds;          
    LONG    xDest;
    LONG    yDest;
    LONG    cxDest;
    LONG    cyDest;
    DWORD   dwRop;
    LONG    xSrc;
    LONG    ySrc;
    XFORM   xformSrc;           
    COLORREF crBkColorSrc;      
    DWORD   iUsageSrc;          
                                
    DWORD   offBmiSrc;          
    DWORD   cbBmiSrc;           
    DWORD   offBitsSrc;         
    DWORD   cbBitsSrc;          
    LONG    cxSrc;
    LONG    cySrc;
} EMRSTRETCHBLT, *PEMRSTRETCHBLT;

typedef struct tagEMRMASKBLT
{
    EMR     emr;
    RECTL   rclBounds;          
    LONG    xDest;
    LONG    yDest;
    LONG    cxDest;
    LONG    cyDest;
    DWORD   dwRop;
    LONG    xSrc;
    LONG    ySrc;
    XFORM   xformSrc;           
    COLORREF crBkColorSrc;      
    DWORD   iUsageSrc;          
                                
    DWORD   offBmiSrc;          
    DWORD   cbBmiSrc;           
    DWORD   offBitsSrc;         
    DWORD   cbBitsSrc;          
    LONG    xMask;
    LONG    yMask;
    DWORD   iUsageMask;         
    DWORD   offBmiMask;         
    DWORD   cbBmiMask;          
    DWORD   offBitsMask;        
    DWORD   cbBitsMask;         
} EMRMASKBLT, *PEMRMASKBLT;

typedef struct tagEMRPLGBLT
{
    EMR     emr;
    RECTL   rclBounds;          
    POINTL  aptlDest[3];
    LONG    xSrc;
    LONG    ySrc;
    LONG    cxSrc;
    LONG    cySrc;
    XFORM   xformSrc;           
    COLORREF crBkColorSrc;      
    DWORD   iUsageSrc;          
                                
    DWORD   offBmiSrc;          
    DWORD   cbBmiSrc;           
    DWORD   offBitsSrc;         
    DWORD   cbBitsSrc;          
    LONG    xMask;
    LONG    yMask;
    DWORD   iUsageMask;         
    DWORD   offBmiMask;         
    DWORD   cbBmiMask;          
    DWORD   offBitsMask;        
    DWORD   cbBitsMask;         
} EMRPLGBLT, *PEMRPLGBLT;

typedef struct tagEMRSETDIBITSTODEVICE
{
    EMR     emr;
    RECTL   rclBounds;          
    LONG    xDest;
    LONG    yDest;
    LONG    xSrc;
    LONG    ySrc;
    LONG    cxSrc;
    LONG    cySrc;
    DWORD   offBmiSrc;          
    DWORD   cbBmiSrc;           
    DWORD   offBitsSrc;         
    DWORD   cbBitsSrc;          
    DWORD   iUsageSrc;          
    DWORD   iStartScan;
    DWORD   cScans;
} EMRSETDIBITSTODEVICE, *PEMRSETDIBITSTODEVICE;

typedef struct tagEMRSTRETCHDIBITS
{
    EMR     emr;
    RECTL   rclBounds;          
    LONG    xDest;
    LONG    yDest;
    LONG    xSrc;
    LONG    ySrc;
    LONG    cxSrc;
    LONG    cySrc;
    DWORD   offBmiSrc;          
    DWORD   cbBmiSrc;           
    DWORD   offBitsSrc;         
    DWORD   cbBitsSrc;          
    DWORD   iUsageSrc;          
    DWORD   dwRop;
    LONG    cxDest;
    LONG    cyDest;
} EMRSTRETCHDIBITS, *PEMRSTRETCHDIBITS;

typedef struct tagEMREXTCREATEFONTINDIRECTW
{
    EMR     emr;
    DWORD   ihFont;             
    EXTLOGFONTW elfw;
} EMREXTCREATEFONTINDIRECTW, *PEMREXTCREATEFONTINDIRECTW;

typedef struct tagEMRCREATEPALETTE
{
    EMR     emr;
    DWORD   ihPal;              
    LOGPALETTE lgpl;            
                                
} EMRCREATEPALETTE, *PEMRCREATEPALETTE;

typedef struct tagEMRCREATEPEN
{
    EMR     emr;
    DWORD   ihPen;              
    LOGPEN  lopn;
} EMRCREATEPEN, *PEMRCREATEPEN;

typedef struct tagEMREXTCREATEPEN
{
    EMR     emr;
    DWORD   ihPen;              
    DWORD   offBmi;             
    DWORD   cbBmi;              
                                
                                
    DWORD   offBits;            
    DWORD   cbBits;             
    EXTLOGPEN elp;              
} EMREXTCREATEPEN, *PEMREXTCREATEPEN;

typedef struct tagEMRCREATEBRUSHINDIRECT
{
    EMR        emr;
    DWORD      ihBrush;          
    LOGBRUSH32 lb;               
                                 
} EMRCREATEBRUSHINDIRECT, *PEMRCREATEBRUSHINDIRECT;

typedef struct tagEMRCREATEMONOBRUSH
{
    EMR     emr;
    DWORD   ihBrush;            
    DWORD   iUsage;             
    DWORD   offBmi;             
    DWORD   cbBmi;              
    DWORD   offBits;            
    DWORD   cbBits;             
} EMRCREATEMONOBRUSH, *PEMRCREATEMONOBRUSH;

typedef struct tagEMRCREATEDIBPATTERNBRUSHPT
{
    EMR     emr;
    DWORD   ihBrush;            
    DWORD   iUsage;             
    DWORD   offBmi;             
    DWORD   cbBmi;              
                                
                                
    DWORD   offBits;            
    DWORD   cbBits;             
} EMRCREATEDIBPATTERNBRUSHPT, *PEMRCREATEDIBPATTERNBRUSHPT;

typedef struct tagEMRFORMAT
{
    DWORD   dSignature;         
    DWORD   nVersion;           
    DWORD   cbData;             
    DWORD   offData;            
                                
} EMRFORMAT, *PEMRFORMAT;



typedef struct tagEMRGLSRECORD
{
    EMR     emr;
    DWORD   cbData;             
    BYTE    Data[1];
} EMRGLSRECORD, *PEMRGLSRECORD;

typedef struct tagEMRGLSBOUNDEDRECORD
{
    EMR     emr;
    RECTL   rclBounds;          
    DWORD   cbData;             
    BYTE    Data[1];
} EMRGLSBOUNDEDRECORD, *PEMRGLSBOUNDEDRECORD;

typedef struct tagEMRPIXELFORMAT
{
    EMR     emr;
    PIXELFORMATDESCRIPTOR pfd;
} EMRPIXELFORMAT, *PEMRPIXELFORMAT;

typedef struct tagEMRCREATECOLORSPACE
{
    EMR             emr;
    DWORD           ihCS;       
    LOGCOLORSPACEA  lcs;        
} EMRCREATECOLORSPACE, *PEMRCREATECOLORSPACE;

typedef struct tagEMRSETCOLORSPACE
{
    EMR     emr;
    DWORD   ihCS;               
} EMRSETCOLORSPACE,    *PEMRSETCOLORSPACE,
  EMRSELECTCOLORSPACE, *PEMRSELECTCOLORSPACE,
  EMRDELETECOLORSPACE, *PEMRDELETECOLORSPACE;

#line 4671 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"



typedef struct tagEMREXTESCAPE
{
    EMR     emr;
    INT     iEscape;            
    INT     cbEscData;          
    BYTE    EscData[1];         
} EMREXTESCAPE,  *PEMREXTESCAPE,
  EMRDRAWESCAPE, *PEMRDRAWESCAPE;

typedef struct tagEMRNAMEDESCAPE
{
    EMR     emr;
    INT     iEscape;            
    INT     cbDriver;           
    INT     cbEscData;          
    BYTE    EscData[1];         
} EMRNAMEDESCAPE, *PEMRNAMEDESCAPE;



typedef struct tagEMRSETICMPROFILE
{
    EMR     emr;
    DWORD   dwFlags;            
    DWORD   cbName;             
    DWORD   cbData;             
    BYTE    Data[1];            
} EMRSETICMPROFILE,  *PEMRSETICMPROFILE,
  EMRSETICMPROFILEA, *PEMRSETICMPROFILEA,
  EMRSETICMPROFILEW, *PEMRSETICMPROFILEW;



typedef struct tagEMRCREATECOLORSPACEW
{
    EMR             emr;
    DWORD           ihCS;       
    LOGCOLORSPACEW  lcs;        
    DWORD           dwFlags;    
    DWORD           cbData;     
    BYTE            Data[1];    
} EMRCREATECOLORSPACEW, *PEMRCREATECOLORSPACEW;



typedef struct tagCOLORMATCHTOTARGET
{
    EMR     emr;
    DWORD   dwAction;           
    DWORD   dwFlags;            
    DWORD   cbName;             
    DWORD   cbData;             
    BYTE    Data[1];            
} EMRCOLORMATCHTOTARGET, *PEMRCOLORMATCHTOTARGET;

typedef struct tagCOLORCORRECTPALETTE
{
    EMR     emr;
    DWORD   ihPalette;          
    DWORD   nFirstEntry;        
    DWORD   nPalEntries;        
    DWORD   nReserved;          
} EMRCOLORCORRECTPALETTE, *PEMRCOLORCORRECTPALETTE;

typedef struct tagEMRALPHABLEND
{
    EMR     emr;
    RECTL   rclBounds;          
    LONG    xDest;
    LONG    yDest;
    LONG    cxDest;
    LONG    cyDest;
    DWORD   dwRop;
    LONG    xSrc;
    LONG    ySrc;
    XFORM   xformSrc;           
    COLORREF crBkColorSrc;      
    DWORD   iUsageSrc;          
                                
    DWORD   offBmiSrc;          
    DWORD   cbBmiSrc;           
    DWORD   offBitsSrc;         
    DWORD   cbBitsSrc;          
    LONG    cxSrc;
    LONG    cySrc;
} EMRALPHABLEND, *PEMRALPHABLEND;

typedef struct tagEMRGRADIENTFILL
{
    EMR       emr;
    RECTL     rclBounds;          
    DWORD     nVer;
    DWORD     nTri;
    ULONG     ulMode;
    TRIVERTEX Ver[1];
}EMRGRADIENTFILL,*PEMRGRADIENTFILL;

typedef struct tagEMRTRANSPARENTBLT
{
    EMR     emr;
    RECTL   rclBounds;          
    LONG    xDest;
    LONG    yDest;
    LONG    cxDest;
    LONG    cyDest;
    DWORD   dwRop;
    LONG    xSrc;
    LONG    ySrc;
    XFORM   xformSrc;           
    COLORREF crBkColorSrc;      
    DWORD   iUsageSrc;          
                                
    DWORD   offBmiSrc;          
    DWORD   cbBmiSrc;           
    DWORD   offBitsSrc;         
    DWORD   cbBitsSrc;          
    LONG    cxSrc;
    LONG    cySrc;
} EMRTRANSPARENTBLT, *PEMRTRANSPARENTBLT;


#line 4796 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"










#line 4807 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"




__declspec(dllimport) BOOL  __stdcall wglCopyContext(HGLRC, HGLRC, UINT);
__declspec(dllimport) HGLRC __stdcall wglCreateContext(HDC);
__declspec(dllimport) HGLRC __stdcall wglCreateLayerContext(HDC, int);
__declspec(dllimport) BOOL  __stdcall wglDeleteContext(HGLRC);
__declspec(dllimport) HGLRC __stdcall wglGetCurrentContext(void);
__declspec(dllimport) HDC   __stdcall wglGetCurrentDC(void);
__declspec(dllimport) PROC  __stdcall wglGetProcAddress(LPCSTR);
__declspec(dllimport) BOOL  __stdcall wglMakeCurrent(HDC, HGLRC);
__declspec(dllimport) BOOL  __stdcall wglShareLists(HGLRC, HGLRC);
__declspec(dllimport) BOOL  __stdcall wglUseFontBitmapsA(HDC, DWORD, DWORD, DWORD);
__declspec(dllimport) BOOL  __stdcall wglUseFontBitmapsW(HDC, DWORD, DWORD, DWORD);




#line 4827 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"
__declspec(dllimport) BOOL  __stdcall SwapBuffers(HDC);

typedef struct _POINTFLOAT {
    FLOAT   x;
    FLOAT   y;
} POINTFLOAT, *PPOINTFLOAT;

typedef struct _GLYPHMETRICSFLOAT {
    FLOAT       gmfBlackBoxX;
    FLOAT       gmfBlackBoxY;
    POINTFLOAT  gmfptGlyphOrigin;
    FLOAT       gmfCellIncX;
    FLOAT       gmfCellIncY;
} GLYPHMETRICSFLOAT, *PGLYPHMETRICSFLOAT,  *LPGLYPHMETRICSFLOAT;



__declspec(dllimport) BOOL  __stdcall wglUseFontOutlinesA(HDC, DWORD, DWORD, DWORD, FLOAT,
                                           FLOAT, int, LPGLYPHMETRICSFLOAT);
__declspec(dllimport) BOOL  __stdcall wglUseFontOutlinesW(HDC, DWORD, DWORD, DWORD, FLOAT,
                                           FLOAT, int, LPGLYPHMETRICSFLOAT);




#line 4853 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


typedef struct tagLAYERPLANEDESCRIPTOR { 
    WORD  nSize;
    WORD  nVersion;
    DWORD dwFlags;
    BYTE  iPixelType;
    BYTE  cColorBits;
    BYTE  cRedBits;
    BYTE  cRedShift;
    BYTE  cGreenBits;
    BYTE  cGreenShift;
    BYTE  cBlueBits;
    BYTE  cBlueShift;
    BYTE  cAlphaBits;
    BYTE  cAlphaShift;
    BYTE  cAccumBits;
    BYTE  cAccumRedBits;
    BYTE  cAccumGreenBits;
    BYTE  cAccumBlueBits;
    BYTE  cAccumAlphaBits;
    BYTE  cDepthBits;
    BYTE  cStencilBits;
    BYTE  cAuxBuffers;
    BYTE  iLayerPlane;
    BYTE  bReserved;
    COLORREF crTransparent;
} LAYERPLANEDESCRIPTOR, *PLAYERPLANEDESCRIPTOR,  *LPLAYERPLANEDESCRIPTOR;

















































__declspec(dllimport) BOOL  __stdcall wglDescribeLayerPlane(HDC, int, int, UINT,
                                             LPLAYERPLANEDESCRIPTOR);
__declspec(dllimport) int   __stdcall wglSetLayerPaletteEntries(HDC, int, int, int,
                                                 const COLORREF *);
__declspec(dllimport) int   __stdcall wglGetLayerPaletteEntries(HDC, int, int, int,
                                                 COLORREF *);
__declspec(dllimport) BOOL  __stdcall wglRealizeLayerPalette(HDC, int, BOOL);
__declspec(dllimport) BOOL  __stdcall wglSwapLayerBuffers(HDC, UINT);



typedef struct _WGLSWAP
{
    HDC hdc;
    UINT uiFlags;
} WGLSWAP, *PWGLSWAP,  *LPWGLSWAP;



__declspec(dllimport) DWORD __stdcall wglSwapMultipleBuffers(UINT, const WGLSWAP *);

#line 4952 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

#line 4954 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


}
#line 4958 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"


#line 4961 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wingdi.h"

#line 164 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"























#line 25 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






extern "C" {
#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









typedef HANDLE HDWP;
typedef void MENUTEMPLATEA;
typedef void MENUTEMPLATEW;



typedef MENUTEMPLATEA MENUTEMPLATE;
#line 50 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
typedef PVOID LPMENUTEMPLATEA;
typedef PVOID LPMENUTEMPLATEW;



typedef LPMENUTEMPLATEA LPMENUTEMPLATE;
#line 57 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef LRESULT (__stdcall* WNDPROC)(HWND, UINT, WPARAM, LPARAM);



typedef INT_PTR (__stdcall* DLGPROC)(HWND, UINT, WPARAM, LPARAM);
typedef void (__stdcall* TIMERPROC)(HWND, UINT, UINT_PTR, DWORD);
typedef BOOL (__stdcall* GRAYSTRINGPROC)(HDC, LPARAM, int);
typedef BOOL (__stdcall* WNDENUMPROC)(HWND, LPARAM);
typedef LRESULT (__stdcall* HOOKPROC)(int code, WPARAM wParam, LPARAM lParam);
typedef void (__stdcall* SENDASYNCPROC)(HWND, UINT, ULONG_PTR, LRESULT);

typedef BOOL (__stdcall* PROPENUMPROCA)(HWND, LPCSTR, HANDLE);
typedef BOOL (__stdcall* PROPENUMPROCW)(HWND, LPCWSTR, HANDLE);

typedef BOOL (__stdcall* PROPENUMPROCEXA)(HWND, LPSTR, HANDLE, ULONG_PTR);
typedef BOOL (__stdcall* PROPENUMPROCEXW)(HWND, LPWSTR, HANDLE, ULONG_PTR);

typedef int (__stdcall* EDITWORDBREAKPROCA)(LPSTR lpch, int ichCurrent, int cch, int code);
typedef int (__stdcall* EDITWORDBREAKPROCW)(LPWSTR lpch, int ichCurrent, int cch, int code);


typedef BOOL (__stdcall* DRAWSTATEPROC)(HDC hdc, LPARAM lData, WPARAM wData, int cx, int cy);
#line 81 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





















#line 103 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






typedef PROPENUMPROCA        PROPENUMPROC;
typedef PROPENUMPROCEXA      PROPENUMPROCEX;
typedef EDITWORDBREAKPROCA   EDITWORDBREAKPROC;
#line 113 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



typedef BOOL (__stdcall* NAMEENUMPROCA)(LPSTR, LPARAM);
typedef BOOL (__stdcall* NAMEENUMPROCW)(LPWSTR, LPARAM);

typedef NAMEENUMPROCA   WINSTAENUMPROCA;
typedef NAMEENUMPROCA   DESKTOPENUMPROCA;
typedef NAMEENUMPROCW   WINSTAENUMPROCW;
typedef NAMEENUMPROCW   DESKTOPENUMPROCW;












#line 136 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







typedef WINSTAENUMPROCA     WINSTAENUMPROC;
typedef DESKTOPENUMPROCA    DESKTOPENUMPROC;

#line 147 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 156 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




























#line 185 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"















#line 201 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 204 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
wvsprintfA(
     LPSTR,
     LPCSTR,
     va_list arglist);
__declspec(dllimport)
int
__stdcall
wvsprintfW(
     LPWSTR,
     LPCWSTR,
     va_list arglist);




#line 224 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__cdecl
wsprintfA(
     LPSTR,
     LPCSTR,
    ...);
__declspec(dllimport)
int
__cdecl
wsprintfW(
     LPWSTR,
     LPCWSTR,
    ...);




#line 244 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




































#line 281 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









































#line 323 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"















#line 339 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


























#line 366 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








































































































































































#line 535 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









































#line 577 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






#line 584 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





































#line 622 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


















#line 641 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





#line 647 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 652 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 657 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 659 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 662 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
































typedef struct tagCBT_CREATEWNDA
{
    struct tagCREATESTRUCTA *lpcs;
    HWND           hwndInsertAfter;
} CBT_CREATEWNDA, *LPCBT_CREATEWNDA;



typedef struct tagCBT_CREATEWNDW
{
    struct tagCREATESTRUCTW *lpcs;
    HWND           hwndInsertAfter;
} CBT_CREATEWNDW, *LPCBT_CREATEWNDW;




typedef CBT_CREATEWNDA CBT_CREATEWND;
typedef LPCBT_CREATEWNDA LPCBT_CREATEWND;
#line 714 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




typedef struct tagCBTACTIVATESTRUCT
{
    BOOL    fMouse;
    HWND    hWndActive;
} CBTACTIVATESTRUCT, *LPCBTACTIVATESTRUCT;


























#line 750 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



























#line 778 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 782 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 787 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"













































































#line 865 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct
{
    HWND    hwnd;
    RECT    rc;
} SHELLHOOKINFO, *LPSHELLHOOKINFO;




typedef struct tagEVENTMSG {
    UINT    message;
    UINT    paramL;
    UINT    paramH;
    DWORD    time;
    HWND     hwnd;
} EVENTMSG, *PEVENTMSGMSG,  *NPEVENTMSGMSG,  *LPEVENTMSGMSG;

typedef struct tagEVENTMSG *PEVENTMSG,  *NPEVENTMSG,  *LPEVENTMSG;




typedef struct tagCWPSTRUCT {
    LPARAM  lParam;
    WPARAM  wParam;
    UINT    message;
    HWND    hwnd;
} CWPSTRUCT, *PCWPSTRUCT,  *NPCWPSTRUCT,  *LPCWPSTRUCT;





typedef struct tagCWPRETSTRUCT {
    LRESULT lResult;
    LPARAM  lParam;
    WPARAM  wParam;
    UINT    message;
    HWND    hwnd;
} CWPRETSTRUCT, *PCWPRETSTRUCT,  *NPCWPRETSTRUCT,  *LPCWPRETSTRUCT;

#line 908 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




































#line 945 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




typedef struct tagDEBUGHOOKINFO
{
    DWORD   idThread;
    DWORD   idThreadInstaller;
    LPARAM  lParam;
    WPARAM  wParam;
    int     code;
} DEBUGHOOKINFO, *PDEBUGHOOKINFO,  *NPDEBUGHOOKINFO, * LPDEBUGHOOKINFO;




typedef struct tagMOUSEHOOKSTRUCT {
    POINT   pt;
    HWND    hwnd;
    UINT    wHitTestCode;
    ULONG_PTR dwExtraInfo;
} MOUSEHOOKSTRUCT,  *LPMOUSEHOOKSTRUCT, *PMOUSEHOOKSTRUCT;














#line 982 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





typedef struct tagHARDWAREHOOKSTRUCT {
    HWND    hwnd;
    UINT    message;
    WPARAM  wParam;
    LPARAM  lParam;
} HARDWAREHOOKSTRUCT,  *LPHARDWAREHOOKSTRUCT, *PHARDWAREHOOKSTRUCT;
#line 994 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 995 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"














#line 1010 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 1015 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 1025 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






__declspec(dllimport)
HKL
__stdcall
LoadKeyboardLayoutA(
     LPCSTR pwszKLID,
     UINT Flags);
__declspec(dllimport)
HKL
__stdcall
LoadKeyboardLayoutW(
     LPCWSTR pwszKLID,
     UINT Flags);




#line 1048 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
HKL
__stdcall
ActivateKeyboardLayout(
     HKL hkl,
     UINT Flags);







#line 1065 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
int
__stdcall
ToUnicodeEx(
     UINT wVirtKey,
     UINT wScanCode,
     const BYTE *lpKeyState,
     LPWSTR pwszBuff,
     int cchBuff,
     UINT wFlags,
     HKL dwhkl);
#line 1079 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
UnloadKeyboardLayout(
     HKL hkl);

__declspec(dllimport)
BOOL
__stdcall
GetKeyboardLayoutNameA(
     LPSTR pwszKLID);
__declspec(dllimport)
BOOL
__stdcall
GetKeyboardLayoutNameW(
     LPWSTR pwszKLID);




#line 1101 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
int
__stdcall
GetKeyboardLayoutList(
         int nBuff,
         HKL  *lpList);

__declspec(dllimport)
HKL
__stdcall
GetKeyboardLayout(
     DWORD idThread
);
#line 1117 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



typedef struct tagMOUSEMOVEPOINT {
    int   x;
    int   y;
    DWORD time;
    ULONG_PTR dwExtraInfo;
} MOUSEMOVEPOINT, *PMOUSEMOVEPOINT, * LPMOUSEMOVEPOINT;







__declspec(dllimport)
int
__stdcall
GetMouseMovePointsEx(
     UINT             cbSize,
     LPMOUSEMOVEPOINT lppt,
     LPMOUSEMOVEPOINT lpptBuf,
     int              nBufPoints,
     DWORD            resolution
);

#line 1145 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"























__declspec(dllimport)
HDESK
__stdcall
CreateDesktopA(
     LPCSTR lpszDesktop,
     LPCSTR lpszDevice,
     LPDEVMODEA pDevmode,
     DWORD dwFlags,
     ACCESS_MASK dwDesiredAccess,
     LPSECURITY_ATTRIBUTES lpsa);
__declspec(dllimport)
HDESK
__stdcall
CreateDesktopW(
     LPCWSTR lpszDesktop,
     LPCWSTR lpszDevice,
     LPDEVMODEW pDevmode,
     DWORD dwFlags,
     ACCESS_MASK dwDesiredAccess,
     LPSECURITY_ATTRIBUTES lpsa);




#line 1193 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 1195 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 1196 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HDESK
__stdcall
OpenDesktopA(
     LPCSTR lpszDesktop,
     DWORD dwFlags,
     BOOL fInherit,
     ACCESS_MASK dwDesiredAccess);
__declspec(dllimport)
HDESK
__stdcall
OpenDesktopW(
     LPCWSTR lpszDesktop,
     DWORD dwFlags,
     BOOL fInherit,
     ACCESS_MASK dwDesiredAccess);




#line 1218 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HDESK
__stdcall
OpenInputDesktop(
     DWORD dwFlags,
     BOOL fInherit,
     ACCESS_MASK dwDesiredAccess);

__declspec(dllimport)
BOOL
__stdcall
EnumDesktopsA(
     HWINSTA hwinsta,
     DESKTOPENUMPROCA lpEnumFunc,
     LPARAM lParam);
__declspec(dllimport)
BOOL
__stdcall
EnumDesktopsW(
     HWINSTA hwinsta,
     DESKTOPENUMPROCW lpEnumFunc,
     LPARAM lParam);




#line 1246 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
EnumDesktopWindows(
     HDESK hDesktop,
     WNDENUMPROC lpfn,
     LPARAM lParam);

__declspec(dllimport)
BOOL
__stdcall
SwitchDesktop(
     HDESK hDesktop);

__declspec(dllimport)
BOOL
__stdcall
SetThreadDesktop(
     HDESK hDesktop);

__declspec(dllimport)
BOOL
__stdcall
CloseDesktop(
     HDESK hDesktop);

__declspec(dllimport)
HDESK
__stdcall
GetThreadDesktop(
     DWORD dwThreadId);
#line 1279 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
























__declspec(dllimport)
HWINSTA
__stdcall
CreateWindowStationA(
     LPCSTR              lpwinsta,
     DWORD                 dwReserved,
     ACCESS_MASK           dwDesiredAccess,
     LPSECURITY_ATTRIBUTES lpsa);
__declspec(dllimport)
HWINSTA
__stdcall
CreateWindowStationW(
     LPCWSTR              lpwinsta,
     DWORD                 dwReserved,
     ACCESS_MASK           dwDesiredAccess,
     LPSECURITY_ATTRIBUTES lpsa);




#line 1324 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWINSTA
__stdcall
OpenWindowStationA(
     LPCSTR lpszWinSta,
     BOOL fInherit,
     ACCESS_MASK dwDesiredAccess);
__declspec(dllimport)
HWINSTA
__stdcall
OpenWindowStationW(
     LPCWSTR lpszWinSta,
     BOOL fInherit,
     ACCESS_MASK dwDesiredAccess);




#line 1344 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
EnumWindowStationsA(
     WINSTAENUMPROCA lpEnumFunc,
     LPARAM lParam);
__declspec(dllimport)
BOOL
__stdcall
EnumWindowStationsW(
     WINSTAENUMPROCW lpEnumFunc,
     LPARAM lParam);




#line 1362 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
BOOL
__stdcall
CloseWindowStation(
     HWINSTA hWinSta);

__declspec(dllimport)
BOOL
__stdcall
SetProcessWindowStation(
     HWINSTA hWinSta);

__declspec(dllimport)
HWINSTA
__stdcall
GetProcessWindowStation(
    void);
#line 1382 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
BOOL
__stdcall
SetUserObjectSecurity(
     HANDLE hObj,
     PSECURITY_INFORMATION pSIRequested,
     PSECURITY_DESCRIPTOR pSID);

__declspec(dllimport)
BOOL
__stdcall
GetUserObjectSecurity(
     HANDLE hObj,
     PSECURITY_INFORMATION pSIRequested,
      PSECURITY_DESCRIPTOR pSID,
     DWORD nLength,
     LPDWORD lpnLengthNeeded);






typedef struct tagUSEROBJECTFLAGS {
    BOOL fInherit;
    BOOL fReserved;
    DWORD dwFlags;
} USEROBJECTFLAGS, *PUSEROBJECTFLAGS;

__declspec(dllimport)
BOOL
__stdcall
GetUserObjectInformationA(
     HANDLE hObj,
     int nIndex,
     PVOID pvInfo,
     DWORD nLength,
     LPDWORD lpnLengthNeeded);
__declspec(dllimport)
BOOL
__stdcall
GetUserObjectInformationW(
     HANDLE hObj,
     int nIndex,
     PVOID pvInfo,
     DWORD nLength,
     LPDWORD lpnLengthNeeded);




#line 1437 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SetUserObjectInformationA(
     HANDLE hObj,
     int nIndex,
     PVOID pvInfo,
     DWORD nLength);
__declspec(dllimport)
BOOL
__stdcall
SetUserObjectInformationW(
     HANDLE hObj,
     int nIndex,
     PVOID pvInfo,
     DWORD nLength);




#line 1459 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 1461 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


typedef struct tagWNDCLASSEXA {
    UINT        cbSize;
    
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCSTR      lpszMenuName;
    LPCSTR      lpszClassName;
    
    HICON       hIconSm;
} WNDCLASSEXA, *PWNDCLASSEXA,  *NPWNDCLASSEXA,  *LPWNDCLASSEXA;
typedef struct tagWNDCLASSEXW {
    UINT        cbSize;
    
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCWSTR     lpszMenuName;
    LPCWSTR     lpszClassName;
    
    HICON       hIconSm;
} WNDCLASSEXW, *PWNDCLASSEXW,  *NPWNDCLASSEXW,  *LPWNDCLASSEXW;






typedef WNDCLASSEXA WNDCLASSEX;
typedef PWNDCLASSEXA PWNDCLASSEX;
typedef NPWNDCLASSEXA NPWNDCLASSEX;
typedef LPWNDCLASSEXA LPWNDCLASSEX;
#line 1506 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 1507 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct tagWNDCLASSA {
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCSTR      lpszMenuName;
    LPCSTR      lpszClassName;
} WNDCLASSA, *PWNDCLASSA,  *NPWNDCLASSA,  *LPWNDCLASSA;
typedef struct tagWNDCLASSW {
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCWSTR     lpszMenuName;
    LPCWSTR     lpszClassName;
} WNDCLASSW, *PWNDCLASSW,  *NPWNDCLASSW,  *LPWNDCLASSW;






typedef WNDCLASSA WNDCLASS;
typedef PWNDCLASSA PWNDCLASS;
typedef NPWNDCLASSA NPWNDCLASS;
typedef LPWNDCLASSA LPWNDCLASS;
#line 1543 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
IsHungAppWindow(
     HWND hwnd);



__declspec(dllimport)
void
DisableProcessWindowsGhosting(
    void);
#line 1556 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






typedef struct tagMSG {
    HWND        hwnd;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
    DWORD       time;
    POINT       pt;



} MSG, *PMSG,  *NPMSG,  *LPMSG;











#line 1585 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"













































#line 1631 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





















#line 1653 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



































#line 1689 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 1697 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
















typedef struct tagMINMAXINFO {
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
} MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;



















#line 1740 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 1741 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






















typedef struct tagCOPYDATASTRUCT {
    ULONG_PTR dwData;
    DWORD cbData;
    PVOID lpData;
} COPYDATASTRUCT, *PCOPYDATASTRUCT;


typedef struct tagMDINEXTMENU
{
    HMENU   hmenuIn;
    HMENU   hmenuNext;
    HWND    hwndNext;
} MDINEXTMENU, * PMDINEXTMENU,  * LPMDINEXTMENU;
#line 1777 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






















#line 1800 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"










#line 1811 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

















#line 1829 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 1834 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"














#line 1849 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 1851 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






#line 1858 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









































#line 1900 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 1901 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 1903 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 1904 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"























#line 1928 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 1933 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 1936 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 1938 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 1940 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 1950 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"










#line 1961 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"










#line 1972 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



























#line 2000 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 2002 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 2006 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



























#line 2034 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 2037 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 2041 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 2046 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 2050 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






#line 2057 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
























#line 2082 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 2086 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 2090 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 2100 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 2108 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




















#line 2129 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




































#line 2166 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"










#line 2177 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 2178 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
















#line 2195 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
UINT
__stdcall
RegisterWindowMessageA(
     LPCSTR lpString);
__declspec(dllimport)
UINT
__stdcall
RegisterWindowMessageW(
     LPCWSTR lpString);




#line 2212 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"























typedef struct tagWINDOWPOS {
    HWND    hwnd;
    HWND    hwndInsertAfter;
    int     x;
    int     y;
    int     cx;
    int     cy;
    UINT    flags;
} WINDOWPOS, *LPWINDOWPOS, *PWINDOWPOS;




typedef struct tagNCCALCSIZE_PARAMS {
    RECT       rgrc[3];
    PWINDOWPOS lppos;
} NCCALCSIZE_PARAMS, *LPNCCALCSIZE_PARAMS;




























#line 2281 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 2283 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



































#line 2319 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 2322 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


































































#line 2389 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

















#line 2407 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 2412 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





#line 2418 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 2423 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




















#line 2444 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 2448 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"























































__declspec(dllimport)
BOOL
__stdcall
DrawEdge(
     HDC hdc,
      LPRECT qrc,
     UINT edge,
     UINT grfFlags);









#line 2521 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

































#line 2555 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





__declspec(dllimport)
BOOL
__stdcall
DrawFrameControl(
     HDC,
      LPRECT,
     UINT,
     UINT);










#line 2579 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 2582 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
DrawCaption( HWND,  HDC,  const RECT *,  UINT);





__declspec(dllimport)
BOOL
__stdcall
DrawAnimatedRects(
     HWND hwnd,
     int idAni,
     const RECT * lprcFrom,
     const RECT * lprcTo);

#line 2602 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
























#line 2627 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 2630 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 2638 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




















#line 2659 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"










typedef struct tagACCEL {

    BYTE   fVirt;               
    WORD   key;
    WORD   cmd;




#line 2679 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
} ACCEL, *LPACCEL;

typedef struct tagPAINTSTRUCT {
    HDC         hdc;
    BOOL        fErase;
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;

typedef struct tagCREATESTRUCTA {
    LPVOID      lpCreateParams;
    HINSTANCE   hInstance;
    HMENU       hMenu;
    HWND        hwndParent;
    int         cy;
    int         cx;
    int         y;
    int         x;
    LONG        style;
    LPCSTR      lpszName;
    LPCSTR      lpszClass;
    DWORD       dwExStyle;
} CREATESTRUCTA, *LPCREATESTRUCTA;
typedef struct tagCREATESTRUCTW {
    LPVOID      lpCreateParams;
    HINSTANCE   hInstance;
    HMENU       hMenu;
    HWND        hwndParent;
    int         cy;
    int         cx;
    int         y;
    int         x;
    LONG        style;
    LPCWSTR     lpszName;
    LPCWSTR     lpszClass;
    DWORD       dwExStyle;
} CREATESTRUCTW, *LPCREATESTRUCTW;




typedef CREATESTRUCTA CREATESTRUCT;
typedef LPCREATESTRUCTA LPCREATESTRUCT;
#line 2725 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct tagWINDOWPLACEMENT {
    UINT  length;
    UINT  flags;
    UINT  showCmd;
    POINT ptMinPosition;
    POINT ptMaxPosition;
    RECT  rcNormalPosition;



} WINDOWPLACEMENT;
typedef WINDOWPLACEMENT *PWINDOWPLACEMENT, *LPWINDOWPLACEMENT;





#line 2744 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


typedef struct tagNMHDR
{
    HWND      hwndFrom;
    UINT_PTR  idFrom;
    UINT      code;         
}   NMHDR;
typedef NMHDR  * LPNMHDR;

typedef struct tagSTYLESTRUCT
{
    DWORD   styleOld;
    DWORD   styleNew;
} STYLESTRUCT, * LPSTYLESTRUCT;
#line 2760 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"











#line 2772 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



















#line 2792 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






#line 2799 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 2800 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




typedef struct tagMEASUREITEMSTRUCT {
    UINT       CtlType;
    UINT       CtlID;
    UINT       itemID;
    UINT       itemWidth;
    UINT       itemHeight;
    ULONG_PTR  itemData;
} MEASUREITEMSTRUCT,  *PMEASUREITEMSTRUCT,  *LPMEASUREITEMSTRUCT;





typedef struct tagDRAWITEMSTRUCT {
    UINT        CtlType;
    UINT        CtlID;
    UINT        itemID;
    UINT        itemAction;
    UINT        itemState;
    HWND        hwndItem;
    HDC         hDC;
    RECT        rcItem;
    ULONG_PTR   itemData;
} DRAWITEMSTRUCT,  *PDRAWITEMSTRUCT,  *LPDRAWITEMSTRUCT;




typedef struct tagDELETEITEMSTRUCT {
    UINT       CtlType;
    UINT       CtlID;
    UINT       itemID;
    HWND       hwndItem;
    ULONG_PTR  itemData;
} DELETEITEMSTRUCT,  *PDELETEITEMSTRUCT,  *LPDELETEITEMSTRUCT;




typedef struct tagCOMPAREITEMSTRUCT {
    UINT        CtlType;
    UINT        CtlID;
    HWND        hwndItem;
    UINT        itemID1;
    ULONG_PTR   itemData1;
    UINT        itemID2;
    ULONG_PTR   itemData2;
    DWORD       dwLocaleId;
} COMPAREITEMSTRUCT,  *PCOMPAREITEMSTRUCT,  *LPCOMPAREITEMSTRUCT;







__declspec(dllimport)
BOOL
__stdcall
GetMessageA(
     LPMSG lpMsg,
     HWND hWnd,
     UINT wMsgFilterMin,
     UINT wMsgFilterMax);
__declspec(dllimport)
BOOL
__stdcall
GetMessageW(
     LPMSG lpMsg,
     HWND hWnd,
     UINT wMsgFilterMin,
     UINT wMsgFilterMax);




#line 2881 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
BOOL
__stdcall
TranslateMessage(
     const MSG *lpMsg);

__declspec(dllimport)
LRESULT
__stdcall
DispatchMessageA(
     const MSG *lpMsg);
__declspec(dllimport)
LRESULT
__stdcall
DispatchMessageW(
     const MSG *lpMsg);




#line 2904 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SetMessageQueue(
     int cMessagesMax);

__declspec(dllimport)
BOOL
__stdcall
PeekMessageA(
     LPMSG lpMsg,
     HWND hWnd,
     UINT wMsgFilterMin,
     UINT wMsgFilterMax,
     UINT wRemoveMsg);
__declspec(dllimport)
BOOL
__stdcall
PeekMessageW(
     LPMSG lpMsg,
     HWND hWnd,
     UINT wMsgFilterMin,
     UINT wMsgFilterMax,
     UINT wRemoveMsg);




#line 2934 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"













#line 2948 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 2951 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
RegisterHotKey(
     HWND hWnd,
     int id,
     UINT fsModifiers,
     UINT vk);

__declspec(dllimport)
BOOL
__stdcall
UnregisterHotKey(
     HWND hWnd,
     int id);






















#line 2990 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 2999 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




__declspec(dllimport)
BOOL
__stdcall
ExitWindowsEx(
     UINT uFlags,
     DWORD dwReserved);

__declspec(dllimport)
BOOL
__stdcall
SwapMouseButton(
     BOOL fSwap);

__declspec(dllimport)
DWORD
__stdcall
GetMessagePos(
    void);

__declspec(dllimport)
LONG
__stdcall
GetMessageTime(
    void);

__declspec(dllimport)
LPARAM
__stdcall
GetMessageExtraInfo(
    void);


__declspec(dllimport)
LPARAM
__stdcall
SetMessageExtraInfo(
     LPARAM lParam);
#line 3041 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
LRESULT
__stdcall
SendMessageA(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)
LRESULT
__stdcall
SendMessageW(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);




#line 3063 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
LRESULT
__stdcall
SendMessageTimeoutA(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam,
     UINT fuFlags,
     UINT uTimeout,
     PDWORD_PTR lpdwResult);
__declspec(dllimport)
LRESULT
__stdcall
SendMessageTimeoutW(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam,
     UINT fuFlags,
     UINT uTimeout,
     PDWORD_PTR lpdwResult);




#line 3091 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SendNotifyMessageA(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)
BOOL
__stdcall
SendNotifyMessageW(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);




#line 3113 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SendMessageCallbackA(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam,
     SENDASYNCPROC lpResultCallBack,
     ULONG_PTR dwData);
__declspec(dllimport)
BOOL
__stdcall
SendMessageCallbackW(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam,
     SENDASYNCPROC lpResultCallBack,
     ULONG_PTR dwData);




#line 3139 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


































#line 3174 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



























#line 3202 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"











#line 3214 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




















#line 3235 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 3239 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 3242 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




typedef  PVOID           HDEVNOTIFY;
typedef  HDEVNOTIFY     *PHDEVNOTIFY;





#line 3254 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HDEVNOTIFY
__stdcall
RegisterDeviceNotificationA(
     HANDLE hRecipient,
     LPVOID NotificationFilter,
     DWORD Flags
    );
__declspec(dllimport)
HDEVNOTIFY
__stdcall
RegisterDeviceNotificationW(
     HANDLE hRecipient,
     LPVOID NotificationFilter,
     DWORD Flags
    );




#line 3276 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
UnregisterDeviceNotification(
     HDEVNOTIFY Handle
    );
#line 3284 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
BOOL
__stdcall
PostMessageA(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)
BOOL
__stdcall
PostMessageW(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);




#line 3307 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
PostThreadMessageA(
     DWORD idThread,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)
BOOL
__stdcall
PostThreadMessageW(
     DWORD idThread,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);




#line 3329 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 3339 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 3348 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
AttachThreadInput(
     DWORD idAttach,
     DWORD idAttachTo,
     BOOL fAttach);


__declspec(dllimport)
BOOL
__stdcall
ReplyMessage(
     LRESULT lResult);

__declspec(dllimport)
BOOL
__stdcall
WaitMessage(
    void);


__declspec(dllimport)
DWORD
__stdcall
WaitForInputIdle(
     HANDLE hProcess,
     DWORD dwMilliseconds);

__declspec(dllimport)

LRESULT
__stdcall



#line 3386 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
DefWindowProcA(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)

LRESULT
__stdcall



#line 3399 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
DefWindowProcW(
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);




#line 3409 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
void
__stdcall
PostQuitMessage(
     int nExitCode);



__declspec(dllimport)
LRESULT
__stdcall
CallWindowProcA(
     WNDPROC lpPrevWndFunc,
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)
LRESULT
__stdcall
CallWindowProcW(
     WNDPROC lpPrevWndFunc,
     HWND hWnd,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);




#line 3441 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



























#line 3469 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
InSendMessage(
    void);


__declspec(dllimport)
DWORD
__stdcall
InSendMessageEx(
     LPVOID lpReserved);









#line 3492 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
UINT
__stdcall
GetDoubleClickTime(
    void);

__declspec(dllimport)
BOOL
__stdcall
SetDoubleClickTime(
     UINT);

__declspec(dllimport)
ATOM
__stdcall
RegisterClassA(
     const WNDCLASSA *lpWndClass);
__declspec(dllimport)
ATOM
__stdcall
RegisterClassW(
     const WNDCLASSW *lpWndClass);




#line 3520 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
UnregisterClassA(
     LPCSTR lpClassName,
     HINSTANCE hInstance);
__declspec(dllimport)
BOOL
__stdcall
UnregisterClassW(
     LPCWSTR lpClassName,
     HINSTANCE hInstance);




#line 3538 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
GetClassInfoA(
     HINSTANCE hInstance,
     LPCSTR lpClassName,
     LPWNDCLASSA lpWndClass);
__declspec(dllimport)
BOOL
__stdcall
GetClassInfoW(
     HINSTANCE hInstance,
     LPCWSTR lpClassName,
     LPWNDCLASSW lpWndClass);




#line 3558 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
ATOM
__stdcall
RegisterClassExA(
     const WNDCLASSEXA *);
__declspec(dllimport)
ATOM
__stdcall
RegisterClassExW(
     const WNDCLASSEXW *);




#line 3575 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
GetClassInfoExA(
     HINSTANCE,
     LPCSTR,
     LPWNDCLASSEXA);
__declspec(dllimport)
BOOL
__stdcall
GetClassInfoExW(
     HINSTANCE,
     LPCWSTR,
     LPWNDCLASSEXW);




#line 3595 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 3597 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"










#line 3608 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
CreateWindowExA(
     DWORD dwExStyle,
     LPCSTR lpClassName,
     LPCSTR lpWindowName,
     DWORD dwStyle,
     int X,
     int Y,
     int nWidth,
     int nHeight,
     HWND hWndParent,
     HMENU hMenu,
     HINSTANCE hInstance,
     LPVOID lpParam);
__declspec(dllimport)
HWND
__stdcall
CreateWindowExW(
     DWORD dwExStyle,
     LPCWSTR lpClassName,
     LPCWSTR lpWindowName,
     DWORD dwStyle,
     int X,
     int Y,
     int nWidth,
     int nHeight,
     HWND hWndParent,
     HMENU hMenu,
     HINSTANCE hInstance,
     LPVOID lpParam);




#line 3646 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"













#line 3660 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
IsWindow(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
IsMenu(
     HMENU hMenu);

__declspec(dllimport)
BOOL
__stdcall
IsChild(
     HWND hWndParent,
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
DestroyWindow(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
ShowWindow(
     HWND hWnd,
     int nCmdShow);


__declspec(dllimport)
BOOL
__stdcall
AnimateWindow(
     HWND hWnd,
     DWORD dwTime,
     DWORD dwFlags);
#line 3702 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




























































#line 3763 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
BOOL
__stdcall
ShowWindowAsync(
     HWND hWnd,
     int nCmdShow);
#line 3772 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
FlashWindow(
     HWND hWnd,
     BOOL bInvert);


typedef struct {
    UINT  cbSize;
    HWND  hwnd;
    DWORD dwFlags;
    UINT  uCount;
    DWORD dwTimeout;
} FLASHWINFO, *PFLASHWINFO;

__declspec(dllimport)
BOOL
__stdcall
FlashWindowEx(
    PFLASHWINFO pfwi);








#line 3803 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
ShowOwnedPopups(
     HWND hWnd,
     BOOL fShow);

__declspec(dllimport)
BOOL
__stdcall
OpenIcon(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
CloseWindow(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
MoveWindow(
     HWND hWnd,
     int X,
     int Y,
     int nWidth,
     int nHeight,
     BOOL bRepaint);

__declspec(dllimport)
BOOL
__stdcall
SetWindowPos(
     HWND hWnd,
     HWND hWndInsertAfter,
     int X,
     int Y,
     int cx,
     int cy,
     UINT uFlags);

__declspec(dllimport)
BOOL
__stdcall
GetWindowPlacement(
     HWND hWnd,
     WINDOWPLACEMENT *lpwndpl);

__declspec(dllimport)
BOOL
__stdcall
SetWindowPlacement(
     HWND hWnd,
     const WINDOWPLACEMENT *lpwndpl);




__declspec(dllimport)
HDWP
__stdcall
BeginDeferWindowPos(
     int nNumWindows);

__declspec(dllimport)
HDWP
__stdcall
DeferWindowPos(
     HDWP hWinPosInfo,
     HWND hWnd,
     HWND hWndInsertAfter,
     int x,
     int y,
     int cx,
     int cy,
     UINT uFlags);

__declspec(dllimport)
BOOL
__stdcall
EndDeferWindowPos(
     HDWP hWinPosInfo);

#line 3889 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
IsWindowVisible(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
IsIconic(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
AnyPopup(
    void);

__declspec(dllimport)
BOOL
__stdcall
BringWindowToTop(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
IsZoomed(
     HWND hWnd);






















#line 3942 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"














#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"























#pragma warning(disable:4103)

#pragma pack(push,2)


#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 34 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\pshpack2.h"
#line 3957 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




typedef struct {
    DWORD style;
    DWORD dwExtendedStyle;
    WORD cdit;
    short x;
    short y;
    short cx;
    short cy;
} DLGTEMPLATE;
typedef DLGTEMPLATE *LPDLGTEMPLATEA;
typedef DLGTEMPLATE *LPDLGTEMPLATEW;



typedef LPDLGTEMPLATEA LPDLGTEMPLATE;
#line 3977 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
typedef const DLGTEMPLATE *LPCDLGTEMPLATEA;
typedef const DLGTEMPLATE *LPCDLGTEMPLATEW;



typedef LPCDLGTEMPLATEA LPCDLGTEMPLATE;
#line 3984 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




typedef struct {
    DWORD style;
    DWORD dwExtendedStyle;
    short x;
    short y;
    short cx;
    short cy;
    WORD id;
} DLGITEMTEMPLATE;
typedef DLGITEMTEMPLATE *PDLGITEMTEMPLATEA;
typedef DLGITEMTEMPLATE *PDLGITEMTEMPLATEW;



typedef PDLGITEMTEMPLATEA PDLGITEMTEMPLATE;
#line 4004 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
typedef DLGITEMTEMPLATE *LPDLGITEMTEMPLATEA;
typedef DLGITEMTEMPLATE *LPDLGITEMTEMPLATEW;



typedef LPDLGITEMTEMPLATEA LPDLGITEMTEMPLATE;
#line 4011 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


























#pragma warning(disable:4103)

#pragma pack(pop)


#line 33 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"


#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 37 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\poppack.h"
#line 4014 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
CreateDialogParamA(
     HINSTANCE hInstance,
     LPCSTR lpTemplateName,
     HWND hWndParent,
     DLGPROC lpDialogFunc,
     LPARAM dwInitParam);
__declspec(dllimport)
HWND
__stdcall
CreateDialogParamW(
     HINSTANCE hInstance,
     LPCWSTR lpTemplateName,
     HWND hWndParent,
     DLGPROC lpDialogFunc,
     LPARAM dwInitParam);




#line 4038 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
CreateDialogIndirectParamA(
     HINSTANCE hInstance,
     LPCDLGTEMPLATEA lpTemplate,
     HWND hWndParent,
     DLGPROC lpDialogFunc,
     LPARAM dwInitParam);
__declspec(dllimport)
HWND
__stdcall
CreateDialogIndirectParamW(
     HINSTANCE hInstance,
     LPCDLGTEMPLATEW lpTemplate,
     HWND hWndParent,
     DLGPROC lpDialogFunc,
     LPARAM dwInitParam);




#line 4062 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 4072 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 4082 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
INT_PTR
__stdcall
DialogBoxParamA(
     HINSTANCE hInstance,
     LPCSTR lpTemplateName,
     HWND hWndParent,
     DLGPROC lpDialogFunc,
     LPARAM dwInitParam);
__declspec(dllimport)
INT_PTR
__stdcall
DialogBoxParamW(
     HINSTANCE hInstance,
     LPCWSTR lpTemplateName,
     HWND hWndParent,
     DLGPROC lpDialogFunc,
     LPARAM dwInitParam);




#line 4106 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
INT_PTR
__stdcall
DialogBoxIndirectParamA(
     HINSTANCE hInstance,
     LPCDLGTEMPLATEA hDialogTemplate,
     HWND hWndParent,
     DLGPROC lpDialogFunc,
     LPARAM dwInitParam);
__declspec(dllimport)
INT_PTR
__stdcall
DialogBoxIndirectParamW(
     HINSTANCE hInstance,
     LPCDLGTEMPLATEW hDialogTemplate,
     HWND hWndParent,
     DLGPROC lpDialogFunc,
     LPARAM dwInitParam);




#line 4130 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 4140 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 4150 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
EndDialog(
     HWND hDlg,
     INT_PTR nResult);

__declspec(dllimport)
HWND
__stdcall
GetDlgItem(
     HWND hDlg,
     int nIDDlgItem);

__declspec(dllimport)
BOOL
__stdcall
SetDlgItemInt(
     HWND hDlg,
     int nIDDlgItem,
     UINT uValue,
     BOOL bSigned);

__declspec(dllimport)
UINT
__stdcall
GetDlgItemInt(
     HWND hDlg,
     int nIDDlgItem,
     BOOL *lpTranslated,
     BOOL bSigned);

__declspec(dllimport)
BOOL
__stdcall
SetDlgItemTextA(
     HWND hDlg,
     int nIDDlgItem,
     LPCSTR lpString);
__declspec(dllimport)
BOOL
__stdcall
SetDlgItemTextW(
     HWND hDlg,
     int nIDDlgItem,
     LPCWSTR lpString);




#line 4202 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
UINT
__stdcall
GetDlgItemTextA(
     HWND hDlg,
     int nIDDlgItem,
     LPSTR lpString,
     int nMaxCount);
__declspec(dllimport)
UINT
__stdcall
GetDlgItemTextW(
     HWND hDlg,
     int nIDDlgItem,
     LPWSTR lpString,
     int nMaxCount);




#line 4224 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
CheckDlgButton(
     HWND hDlg,
     int nIDButton,
     UINT uCheck);

__declspec(dllimport)
BOOL
__stdcall
CheckRadioButton(
     HWND hDlg,
     int nIDFirstButton,
     int nIDLastButton,
     int nIDCheckButton);

__declspec(dllimport)
UINT
__stdcall
IsDlgButtonChecked(
     HWND hDlg,
     int nIDButton);

__declspec(dllimport)
LRESULT
__stdcall
SendDlgItemMessageA(
     HWND hDlg,
     int nIDDlgItem,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)
LRESULT
__stdcall
SendDlgItemMessageW(
     HWND hDlg,
     int nIDDlgItem,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);




#line 4272 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
GetNextDlgGroupItem(
     HWND hDlg,
     HWND hCtl,
     BOOL bPrevious);

__declspec(dllimport)
HWND
__stdcall
GetNextDlgTabItem(
     HWND hDlg,
     HWND hCtl,
     BOOL bPrevious);

__declspec(dllimport)
int
__stdcall
GetDlgCtrlID(
     HWND hWnd);

__declspec(dllimport)
long
__stdcall
GetDialogBaseUnits(void);

__declspec(dllimport)

LRESULT
__stdcall



#line 4308 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
DefDlgProcA(
     HWND hDlg,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)

LRESULT
__stdcall



#line 4321 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
DefDlgProcW(
     HWND hDlg,
     UINT Msg,
     WPARAM wParam,
     LPARAM lParam);




#line 4331 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 4340 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 4342 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
BOOL
__stdcall
CallMsgFilterA(
     LPMSG lpMsg,
     int nCode);
__declspec(dllimport)
BOOL
__stdcall
CallMsgFilterW(
     LPMSG lpMsg,
     int nCode);




#line 4362 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 4364 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







__declspec(dllimport)
BOOL
__stdcall
OpenClipboard(
     HWND hWndNewOwner);

__declspec(dllimport)
BOOL
__stdcall
CloseClipboard(
    void);




__declspec(dllimport)
DWORD
__stdcall
GetClipboardSequenceNumber(
    void);

#line 4393 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
GetClipboardOwner(
    void);

__declspec(dllimport)
HWND
__stdcall
SetClipboardViewer(
     HWND hWndNewViewer);

__declspec(dllimport)
HWND
__stdcall
GetClipboardViewer(
    void);

__declspec(dllimport)
BOOL
__stdcall
ChangeClipboardChain(
     HWND hWndRemove,
     HWND hWndNewNext);

__declspec(dllimport)
HANDLE
__stdcall
SetClipboardData(
     UINT uFormat,
     HANDLE hMem);

__declspec(dllimport)
HANDLE
__stdcall
GetClipboardData(
     UINT uFormat);

__declspec(dllimport)
UINT
__stdcall
RegisterClipboardFormatA(
     LPCSTR lpszFormat);
__declspec(dllimport)
UINT
__stdcall
RegisterClipboardFormatW(
     LPCWSTR lpszFormat);




#line 4447 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
CountClipboardFormats(
    void);

__declspec(dllimport)
UINT
__stdcall
EnumClipboardFormats(
     UINT format);

__declspec(dllimport)
int
__stdcall
GetClipboardFormatNameA(
     UINT format,
     LPSTR lpszFormatName,
     int cchMaxCount);
__declspec(dllimport)
int
__stdcall
GetClipboardFormatNameW(
     UINT format,
     LPWSTR lpszFormatName,
     int cchMaxCount);




#line 4479 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
EmptyClipboard(
    void);

__declspec(dllimport)
BOOL
__stdcall
IsClipboardFormatAvailable(
     UINT format);

__declspec(dllimport)
int
__stdcall
GetPriorityClipboardFormat(
     UINT *paFormatPriorityList,
     int cFormats);

__declspec(dllimport)
HWND
__stdcall
GetOpenClipboardWindow(
    void);

#line 4506 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





__declspec(dllimport)
BOOL
__stdcall
CharToOemA(
     LPCSTR lpszSrc,
     LPSTR lpszDst);
__declspec(dllimport)
BOOL
__stdcall
CharToOemW(
     LPCWSTR lpszSrc,
     LPSTR lpszDst);




#line 4528 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
OemToCharA(
     LPCSTR lpszSrc,
     LPSTR lpszDst);
__declspec(dllimport)
BOOL
__stdcall
OemToCharW(
     LPCSTR lpszSrc,
     LPWSTR lpszDst);




#line 4546 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
CharToOemBuffA(
     LPCSTR lpszSrc,
     LPSTR lpszDst,
     DWORD cchDstLength);
__declspec(dllimport)
BOOL
__stdcall
CharToOemBuffW(
     LPCWSTR lpszSrc,
     LPSTR lpszDst,
     DWORD cchDstLength);




#line 4566 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
OemToCharBuffA(
     LPCSTR lpszSrc,
     LPSTR lpszDst,
     DWORD cchDstLength);
__declspec(dllimport)
BOOL
__stdcall
OemToCharBuffW(
     LPCSTR lpszSrc,
     LPWSTR lpszDst,
     DWORD cchDstLength);




#line 4586 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
LPSTR
__stdcall
CharUpperA(
      LPSTR lpsz);
__declspec(dllimport)
LPWSTR
__stdcall
CharUpperW(
      LPWSTR lpsz);




#line 4602 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
DWORD
__stdcall
CharUpperBuffA(
      LPSTR lpsz,
     DWORD cchLength);
__declspec(dllimport)
DWORD
__stdcall
CharUpperBuffW(
      LPWSTR lpsz,
     DWORD cchLength);




#line 4620 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
LPSTR
__stdcall
CharLowerA(
      LPSTR lpsz);
__declspec(dllimport)
LPWSTR
__stdcall
CharLowerW(
      LPWSTR lpsz);




#line 4636 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
DWORD
__stdcall
CharLowerBuffA(
      LPSTR lpsz,
     DWORD cchLength);
__declspec(dllimport)
DWORD
__stdcall
CharLowerBuffW(
      LPWSTR lpsz,
     DWORD cchLength);




#line 4654 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
LPSTR
__stdcall
CharNextA(
     LPCSTR lpsz);
__declspec(dllimport)
LPWSTR
__stdcall
CharNextW(
     LPCWSTR lpsz);




#line 4670 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
LPSTR
__stdcall
CharPrevA(
     LPCSTR lpszStart,
     LPCSTR lpszCurrent);
__declspec(dllimport)
LPWSTR
__stdcall
CharPrevW(
     LPCWSTR lpszStart,
     LPCWSTR lpszCurrent);




#line 4688 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
LPSTR
__stdcall
CharNextExA(
      WORD CodePage,
      LPCSTR lpCurrentChar,
      DWORD dwFlags);

__declspec(dllimport)
LPSTR
__stdcall
CharPrevExA(
      WORD CodePage,
      LPCSTR lpStart,
      LPCSTR lpCurrentChar,
      DWORD dwFlags);
#line 4707 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




















__declspec(dllimport)
BOOL
__stdcall
IsCharAlphaA(
     CHAR ch);
__declspec(dllimport)
BOOL
__stdcall
IsCharAlphaW(
     WCHAR ch);




#line 4742 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
IsCharAlphaNumericA(
     CHAR ch);
__declspec(dllimport)
BOOL
__stdcall
IsCharAlphaNumericW(
     WCHAR ch);




#line 4758 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
IsCharUpperA(
     CHAR ch);
__declspec(dllimport)
BOOL
__stdcall
IsCharUpperW(
     WCHAR ch);




#line 4774 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
IsCharLowerA(
     CHAR ch);
__declspec(dllimport)
BOOL
__stdcall
IsCharLowerW(
     WCHAR ch);




#line 4790 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 4792 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
SetFocus(
     HWND hWnd);

__declspec(dllimport)
HWND
__stdcall
GetActiveWindow(
    void);

__declspec(dllimport)
HWND
__stdcall
GetFocus(
    void);

__declspec(dllimport)
UINT
__stdcall
GetKBCodePage(
    void);

__declspec(dllimport)
SHORT
__stdcall
GetKeyState(
     int nVirtKey);

__declspec(dllimport)
SHORT
__stdcall
GetAsyncKeyState(
     int vKey);

__declspec(dllimport)
BOOL
__stdcall
GetKeyboardState(
     PBYTE lpKeyState);

__declspec(dllimport)
BOOL
__stdcall
SetKeyboardState(
     LPBYTE lpKeyState);

__declspec(dllimport)
int
__stdcall
GetKeyNameTextA(
     LONG lParam,
     LPSTR lpString,
     int nSize
    );
__declspec(dllimport)
int
__stdcall
GetKeyNameTextW(
     LONG lParam,
     LPWSTR lpString,
     int nSize
    );




#line 4862 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
GetKeyboardType(
     int nTypeFlag);

__declspec(dllimport)
int
__stdcall
ToAscii(
     UINT uVirtKey,
     UINT uScanCode,
     const BYTE *lpKeyState,
     LPWORD lpChar,
     UINT uFlags);


__declspec(dllimport)
int
__stdcall
ToAsciiEx(
     UINT uVirtKey,
     UINT uScanCode,
     const BYTE *lpKeyState,
     LPWORD lpChar,
     UINT uFlags,
     HKL dwhkl);
#line 4891 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
ToUnicode(
     UINT wVirtKey,
     UINT wScanCode,
     const BYTE *lpKeyState,
     LPWSTR pwszBuff,
     int cchBuff,
     UINT wFlags);

__declspec(dllimport)
DWORD
__stdcall
OemKeyScan(
     WORD wOemChar);

__declspec(dllimport)
SHORT
__stdcall
VkKeyScanA(
     CHAR ch);
__declspec(dllimport)
SHORT
__stdcall
VkKeyScanW(
     WCHAR ch);




#line 4924 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
SHORT
__stdcall
VkKeyScanExA(
     CHAR  ch,
     HKL   dwhkl);
__declspec(dllimport)
SHORT
__stdcall
VkKeyScanExW(
     WCHAR  ch,
     HKL   dwhkl);




#line 4943 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 4944 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





#line 4950 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
void
__stdcall
keybd_event(
     BYTE bVk,
     BYTE bScan,
     DWORD dwFlags,
     ULONG_PTR dwExtraInfo);
















__declspec(dllimport)
void
__stdcall
mouse_event(
     DWORD dwFlags,
     DWORD dx,
     DWORD dy,
     DWORD dwData,
     ULONG_PTR dwExtraInfo);

















































#line 5034 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"












#line 5047 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
UINT
__stdcall
MapVirtualKeyA(
     UINT uCode,
     UINT uMapType);
__declspec(dllimport)
UINT
__stdcall
MapVirtualKeyW(
     UINT uCode,
     UINT uMapType);




#line 5065 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
UINT
__stdcall
MapVirtualKeyExA(
     UINT uCode,
     UINT uMapType,
     HKL dwhkl);
__declspec(dllimport)
UINT
__stdcall
MapVirtualKeyExW(
     UINT uCode,
     UINT uMapType,
     HKL dwhkl);




#line 5086 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 5087 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
GetInputState(
    void);

__declspec(dllimport)
DWORD
__stdcall
GetQueueStatus(
     UINT flags);


__declspec(dllimport)
HWND
__stdcall
GetCapture(
    void);

__declspec(dllimport)
HWND
__stdcall
SetCapture(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
ReleaseCapture(
    void);

__declspec(dllimport)
DWORD
__stdcall
MsgWaitForMultipleObjects(
     DWORD nCount,
     const HANDLE *pHandles,
     BOOL fWaitAll,
     DWORD dwMilliseconds,
     DWORD dwWakeMask);

__declspec(dllimport)
DWORD
__stdcall
MsgWaitForMultipleObjectsEx(
     DWORD nCount,
     const HANDLE *pHandles,
     DWORD dwMilliseconds,
     DWORD dwWakeMask,
     DWORD dwFlags);




















#line 5159 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 5168 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 5171 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



















__declspec(dllimport)
UINT_PTR
__stdcall
SetTimer(
     HWND hWnd,
     UINT_PTR nIDEvent,
     UINT uElapse,
     TIMERPROC lpTimerFunc);

__declspec(dllimport)
BOOL
__stdcall
KillTimer(
     HWND hWnd,
     UINT_PTR uIDEvent);

__declspec(dllimport)
BOOL
__stdcall
IsWindowUnicode(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
EnableWindow(
     HWND hWnd,
     BOOL bEnable);

__declspec(dllimport)
BOOL
__stdcall
IsWindowEnabled(
     HWND hWnd);

__declspec(dllimport)
HACCEL
__stdcall
LoadAcceleratorsA(
     HINSTANCE hInstance,
     LPCSTR lpTableName);
__declspec(dllimport)
HACCEL
__stdcall
LoadAcceleratorsW(
     HINSTANCE hInstance,
     LPCWSTR lpTableName);




#line 5242 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HACCEL
__stdcall
CreateAcceleratorTableA(
     LPACCEL,  int);
__declspec(dllimport)
HACCEL
__stdcall
CreateAcceleratorTableW(
     LPACCEL,  int);




#line 5258 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
DestroyAcceleratorTable(
     HACCEL hAccel);

__declspec(dllimport)
int
__stdcall
CopyAcceleratorTableA(
     HACCEL hAccelSrc,
     LPACCEL lpAccelDst,
     int cAccelEntries);
__declspec(dllimport)
int
__stdcall
CopyAcceleratorTableW(
     HACCEL hAccelSrc,
     LPACCEL lpAccelDst,
     int cAccelEntries);




#line 5284 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
int
__stdcall
TranslateAcceleratorA(
     HWND hWnd,
     HACCEL hAccTable,
     LPMSG lpMsg);
__declspec(dllimport)
int
__stdcall
TranslateAcceleratorW(
     HWND hWnd,
     HACCEL hAccTable,
     LPMSG lpMsg);




#line 5306 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 5308 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

















































































#line 5390 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






#line 5397 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 5401 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 5409 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 5412 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 5416 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 5421 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 5425 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 5427 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 5429 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 5437 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 5441 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 5443 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
int
__stdcall
GetSystemMetrics(
     int nIndex);


#line 5453 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
HMENU
__stdcall
LoadMenuA(
     HINSTANCE hInstance,
     LPCSTR lpMenuName);
__declspec(dllimport)
HMENU
__stdcall
LoadMenuW(
     HINSTANCE hInstance,
     LPCWSTR lpMenuName);




#line 5473 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HMENU
__stdcall
LoadMenuIndirectA(
     const MENUTEMPLATEA *lpMenuTemplate);
__declspec(dllimport)
HMENU
__stdcall
LoadMenuIndirectW(
     const MENUTEMPLATEW *lpMenuTemplate);




#line 5489 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HMENU
__stdcall
GetMenu(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
SetMenu(
     HWND hWnd,
     HMENU hMenu);

__declspec(dllimport)
BOOL
__stdcall
ChangeMenuA(
     HMENU hMenu,
     UINT cmd,
     LPCSTR lpszNewItem,
     UINT cmdInsert,
     UINT flags);
__declspec(dllimport)
BOOL
__stdcall
ChangeMenuW(
     HMENU hMenu,
     UINT cmd,
     LPCWSTR lpszNewItem,
     UINT cmdInsert,
     UINT flags);




#line 5526 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
HiliteMenuItem(
     HWND hWnd,
     HMENU hMenu,
     UINT uIDHiliteItem,
     UINT uHilite);

__declspec(dllimport)
int
__stdcall
GetMenuStringA(
     HMENU hMenu,
     UINT uIDItem,
     LPSTR lpString,
     int nMaxCount,
     UINT uFlag);
__declspec(dllimport)
int
__stdcall
GetMenuStringW(
     HMENU hMenu,
     UINT uIDItem,
     LPWSTR lpString,
     int nMaxCount,
     UINT uFlag);




#line 5559 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
UINT
__stdcall
GetMenuState(
     HMENU hMenu,
     UINT uId,
     UINT uFlags);

__declspec(dllimport)
BOOL
__stdcall
DrawMenuBar(
     HWND hWnd);




#line 5578 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
HMENU
__stdcall
GetSystemMenu(
     HWND hWnd,
     BOOL bRevert);


__declspec(dllimport)
HMENU
__stdcall
CreateMenu(
    void);

__declspec(dllimport)
HMENU
__stdcall
CreatePopupMenu(
    void);

__declspec(dllimport)
BOOL
__stdcall
DestroyMenu(
     HMENU hMenu);

__declspec(dllimport)
DWORD
__stdcall
CheckMenuItem(
     HMENU hMenu,
     UINT uIDCheckItem,
     UINT uCheck);

__declspec(dllimport)
BOOL
__stdcall
EnableMenuItem(
     HMENU hMenu,
     UINT uIDEnableItem,
     UINT uEnable);

__declspec(dllimport)
HMENU
__stdcall
GetSubMenu(
     HMENU hMenu,
     int nPos);

__declspec(dllimport)
UINT
__stdcall
GetMenuItemID(
     HMENU hMenu,
     int nPos);

__declspec(dllimport)
int
__stdcall
GetMenuItemCount(
     HMENU hMenu);

__declspec(dllimport)
BOOL
__stdcall
InsertMenuA(
     HMENU hMenu,
     UINT uPosition,
     UINT uFlags,
     UINT_PTR uIDNewItem,
     LPCSTR lpNewItem
    );
__declspec(dllimport)
BOOL
__stdcall
InsertMenuW(
     HMENU hMenu,
     UINT uPosition,
     UINT uFlags,
     UINT_PTR uIDNewItem,
     LPCWSTR lpNewItem
    );




#line 5667 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
AppendMenuA(
     HMENU hMenu,
     UINT uFlags,
     UINT_PTR uIDNewItem,
     LPCSTR lpNewItem
    );
__declspec(dllimport)
BOOL
__stdcall
AppendMenuW(
     HMENU hMenu,
     UINT uFlags,
     UINT_PTR uIDNewItem,
     LPCWSTR lpNewItem
    );




#line 5691 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
ModifyMenuA(
     HMENU hMnu,
     UINT uPosition,
     UINT uFlags,
     UINT_PTR uIDNewItem,
     LPCSTR lpNewItem
    );
__declspec(dllimport)
BOOL
__stdcall
ModifyMenuW(
     HMENU hMnu,
     UINT uPosition,
     UINT uFlags,
     UINT_PTR uIDNewItem,
     LPCWSTR lpNewItem
    );




#line 5717 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall RemoveMenu(
     HMENU hMenu,
     UINT uPosition,
     UINT uFlags);

__declspec(dllimport)
BOOL
__stdcall
DeleteMenu(
     HMENU hMenu,
     UINT uPosition,
     UINT uFlags);

__declspec(dllimport)
BOOL
__stdcall
SetMenuItemBitmaps(
     HMENU hMenu,
     UINT uPosition,
     UINT uFlags,
     HBITMAP hBitmapUnchecked,
     HBITMAP hBitmapChecked);

__declspec(dllimport)
LONG
__stdcall
GetMenuCheckMarkDimensions(
    void);

__declspec(dllimport)
BOOL
__stdcall
TrackPopupMenu(
     HMENU hMenu,
     UINT uFlags,
     int x,
     int y,
     int nReserved,
     HWND hWnd,
     const RECT *prcRect);








typedef struct tagTPMPARAMS
{
    UINT    cbSize;     
    RECT    rcExclude;  
}   TPMPARAMS;
typedef TPMPARAMS  *LPTPMPARAMS;

__declspec(dllimport)
BOOL
__stdcall
TrackPopupMenuEx(
     HMENU,
     UINT,
     int,
     int,
     HWND,
     LPTPMPARAMS);
#line 5786 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

















typedef struct tagMENUINFO
{
    DWORD   cbSize;
    DWORD   fMask;
    DWORD   dwStyle;
    UINT    cyMax;
    HBRUSH  hbrBack;
    DWORD   dwContextHelpID;
    ULONG_PTR dwMenuData;
}   MENUINFO,  *LPMENUINFO;
typedef MENUINFO const  *LPCMENUINFO;

__declspec(dllimport)
BOOL
__stdcall
GetMenuInfo(
     HMENU,
     LPMENUINFO);

__declspec(dllimport)
BOOL
__stdcall
SetMenuInfo(
     HMENU,
     LPCMENUINFO);

__declspec(dllimport)
BOOL
__stdcall
EndMenu(
        void);







typedef struct tagMENUGETOBJECTINFO
{
    DWORD dwFlags;
    UINT uPos;
    HMENU hmenu;
    PVOID riid;
    PVOID pvObj;
} MENUGETOBJECTINFO, * PMENUGETOBJECTINFO;












#line 5862 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 5871 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

















#line 5889 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


typedef struct tagMENUITEMINFOA
{
    UINT     cbSize;
    UINT     fMask;
    UINT     fType;         
    UINT     fState;        
    UINT     wID;           
    HMENU    hSubMenu;      
    HBITMAP  hbmpChecked;   
    HBITMAP  hbmpUnchecked; 
    ULONG_PTR dwItemData;   
    LPSTR    dwTypeData;    
    UINT     cch;           

    HBITMAP  hbmpItem;      
#line 5907 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
}   MENUITEMINFOA,  *LPMENUITEMINFOA;
typedef struct tagMENUITEMINFOW
{
    UINT     cbSize;
    UINT     fMask;
    UINT     fType;         
    UINT     fState;        
    UINT     wID;           
    HMENU    hSubMenu;      
    HBITMAP  hbmpChecked;   
    HBITMAP  hbmpUnchecked; 
    ULONG_PTR dwItemData;   
    LPWSTR   dwTypeData;    
    UINT     cch;           

    HBITMAP  hbmpItem;      
#line 5924 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
}   MENUITEMINFOW,  *LPMENUITEMINFOW;




typedef MENUITEMINFOA MENUITEMINFO;
typedef LPMENUITEMINFOA LPMENUITEMINFO;
#line 5932 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
typedef MENUITEMINFOA const  *LPCMENUITEMINFOA;
typedef MENUITEMINFOW const  *LPCMENUITEMINFOW;



typedef LPCMENUITEMINFOA LPCMENUITEMINFO;
#line 5939 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
BOOL
__stdcall
InsertMenuItemA(
     HMENU,
     UINT,
     BOOL,
     LPCMENUITEMINFOA
    );
__declspec(dllimport)
BOOL
__stdcall
InsertMenuItemW(
     HMENU,
     UINT,
     BOOL,
     LPCMENUITEMINFOW
    );




#line 5964 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
GetMenuItemInfoA(
     HMENU,
     UINT,
     BOOL,
      LPMENUITEMINFOA
    );
__declspec(dllimport)
BOOL
__stdcall
GetMenuItemInfoW(
     HMENU,
     UINT,
     BOOL,
      LPMENUITEMINFOW
    );




#line 5988 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SetMenuItemInfoA(
     HMENU,
     UINT,
     BOOL,
     LPCMENUITEMINFOA
    );
__declspec(dllimport)
BOOL
__stdcall
SetMenuItemInfoW(
     HMENU,
     UINT,
     BOOL,
     LPCMENUITEMINFOW
    );




#line 6012 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





__declspec(dllimport) UINT    __stdcall GetMenuDefaultItem(  HMENU hMenu,  UINT fByPos,  UINT gmdiFlags);
__declspec(dllimport) BOOL    __stdcall SetMenuDefaultItem(  HMENU hMenu,  UINT uItem,   UINT fByPos);

__declspec(dllimport) BOOL    __stdcall GetMenuItemRect(  HWND hWnd,  HMENU hMenu,  UINT uItem,  LPRECT lprcItem);
__declspec(dllimport) int     __stdcall MenuItemFromPoint(  HWND hWnd,  HMENU hMenu,  POINT ptScreen);
#line 6023 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


















#line 6042 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 6051 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 6054 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 6055 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 6058 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







typedef struct tagDROPSTRUCT
{
    HWND    hwndSource;
    HWND    hwndSink;
    DWORD   wFmt;
    ULONG_PTR dwData;
    POINT   ptDrop;
    DWORD   dwControlData;
} DROPSTRUCT, *PDROPSTRUCT, *LPDROPSTRUCT;











__declspec(dllimport)
DWORD
__stdcall
DragObject(
     HWND,
     HWND,
     UINT,
     ULONG_PTR,
     HCURSOR);

__declspec(dllimport)
BOOL
__stdcall
DragDetect(
     HWND,
     POINT);
#line 6102 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
DrawIcon(
     HDC hDC,
     int X,
     int Y,
     HICON hIcon);


































#line 6146 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 6147 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct tagDRAWTEXTPARAMS
{
    UINT    cbSize;
    int     iTabLength;
    int     iLeftMargin;
    int     iRightMargin;
    UINT    uiLengthDrawn;
} DRAWTEXTPARAMS,  *LPDRAWTEXTPARAMS;
#line 6157 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
int
__stdcall
DrawTextA(
     HDC hDC,
     LPCSTR lpString,
     int nCount,
      LPRECT lpRect,
     UINT uFormat);
__declspec(dllimport)
int
__stdcall
DrawTextW(
     HDC hDC,
     LPCWSTR lpString,
     int nCount,
      LPRECT lpRect,
     UINT uFormat);




#line 6182 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
int
__stdcall
DrawTextExA(
     HDC,
      LPSTR,
     int,
      LPRECT,
     UINT,
     LPDRAWTEXTPARAMS);
__declspec(dllimport)
int
__stdcall
DrawTextExW(
     HDC,
      LPWSTR,
     int,
      LPRECT,
     UINT,
     LPDRAWTEXTPARAMS);




#line 6210 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 6211 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 6213 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
GrayStringA(
     HDC hDC,
     HBRUSH hBrush,
     GRAYSTRINGPROC lpOutputFunc,
     LPARAM lpData,
     int nCount,
     int X,
     int Y,
     int nWidth,
     int nHeight);
__declspec(dllimport)
BOOL
__stdcall
GrayStringW(
     HDC hDC,
     HBRUSH hBrush,
     GRAYSTRINGPROC lpOutputFunc,
     LPARAM lpData,
     int nCount,
     int X,
     int Y,
     int nWidth,
     int nHeight);




#line 6245 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


















#line 6264 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
BOOL
__stdcall
DrawStateA(
     HDC,
     HBRUSH,
     DRAWSTATEPROC,
     LPARAM,
     WPARAM,
     int,
     int,
     int,
     int,
     UINT);
__declspec(dllimport)
BOOL
__stdcall
DrawStateW(
     HDC,
     HBRUSH,
     DRAWSTATEPROC,
     LPARAM,
     WPARAM,
     int,
     int,
     int,
     int,
     UINT);




#line 6299 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 6300 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
LONG
__stdcall
TabbedTextOutA(
     HDC hDC,
     int X,
     int Y,
     LPCSTR lpString,
     int nCount,
     int nTabPositions,
     const INT *lpnTabStopPositions,
     int nTabOrigin);
__declspec(dllimport)
LONG
__stdcall
TabbedTextOutW(
     HDC hDC,
     int X,
     int Y,
     LPCWSTR lpString,
     int nCount,
     int nTabPositions,
     const INT *lpnTabStopPositions,
     int nTabOrigin);




#line 6330 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
DWORD
__stdcall
GetTabbedTextExtentA(
     HDC hDC,
     LPCSTR lpString,
     int nCount,
     int nTabPositions,
     const INT *lpnTabStopPositions);
__declspec(dllimport)
DWORD
__stdcall
GetTabbedTextExtentW(
     HDC hDC,
     LPCWSTR lpString,
     int nCount,
     int nTabPositions,
     const INT *lpnTabStopPositions);




#line 6354 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
UpdateWindow(
     HWND hWnd);

__declspec(dllimport)
HWND
__stdcall
SetActiveWindow(
     HWND hWnd);

__declspec(dllimport)
HWND
__stdcall
GetForegroundWindow(
    void);


__declspec(dllimport)
BOOL
__stdcall
PaintDesktop(
     HDC hdc);

__declspec(dllimport)
void
__stdcall
SwitchToThisWindow(
     HWND hwnd,
     BOOL fUnknown);
#line 6387 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SetForegroundWindow(
     HWND hWnd);



















#line 6413 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
WindowFromDC(
     HDC hDC);

__declspec(dllimport)
HDC
__stdcall
GetDC(
     HWND hWnd);

__declspec(dllimport)
HDC
__stdcall
GetDCEx(
     HWND hWnd,
     HRGN hrgnClip,
     DWORD flags);


















__declspec(dllimport)
HDC
__stdcall
GetWindowDC(
     HWND hWnd);

__declspec(dllimport)
int
__stdcall
ReleaseDC(
     HWND hWnd,
     HDC hDC);

__declspec(dllimport)
HDC
__stdcall
BeginPaint(
     HWND hWnd,
     LPPAINTSTRUCT lpPaint);

__declspec(dllimport)
BOOL
__stdcall
EndPaint(
     HWND hWnd,
     const PAINTSTRUCT *lpPaint);

__declspec(dllimport)
BOOL
__stdcall
GetUpdateRect(
     HWND hWnd,
     LPRECT lpRect,
     BOOL bErase);

__declspec(dllimport)
int
__stdcall
GetUpdateRgn(
     HWND hWnd,
     HRGN hRgn,
     BOOL bErase);

__declspec(dllimport)
int
__stdcall
SetWindowRgn(
     HWND hWnd,
     HRGN hRgn,
     BOOL bRedraw);


__declspec(dllimport)
int
__stdcall
GetWindowRgn(
     HWND hWnd,
     HRGN hRgn);










#line 6520 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
ExcludeUpdateRgn(
     HDC hDC,
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
InvalidateRect(
     HWND hWnd,
     const RECT *lpRect,
     BOOL bErase);

__declspec(dllimport)
BOOL
__stdcall
ValidateRect(
     HWND hWnd,
     const RECT *lpRect);

__declspec(dllimport)
BOOL
__stdcall
InvalidateRgn(
     HWND hWnd,
     HRGN hRgn,
     BOOL bErase);

__declspec(dllimport)
BOOL
__stdcall
ValidateRgn(
     HWND hWnd,
     HRGN hRgn);


__declspec(dllimport)
BOOL
__stdcall
RedrawWindow(
     HWND hWnd,
     const RECT *lprcUpdate,
     HRGN hrgnUpdate,
     UINT flags);


























__declspec(dllimport)
BOOL
__stdcall
LockWindowUpdate(
     HWND hWndLock);

__declspec(dllimport)
BOOL
__stdcall
ScrollWindow(
     HWND hWnd,
     int XAmount,
     int YAmount,
     const RECT *lpRect,
     const RECT *lpClipRect);

__declspec(dllimport)
BOOL
__stdcall
ScrollDC(
     HDC hDC,
     int dx,
     int dy,
     const RECT *lprcScroll,
     const RECT *lprcClip,
     HRGN hrgnUpdate,
     LPRECT lprcUpdate);

__declspec(dllimport)
int
__stdcall
ScrollWindowEx(
     HWND hWnd,
     int dx,
     int dy,
     const RECT *prcScroll,
     const RECT *prcClip,
     HRGN hrgnUpdate,
     LPRECT prcUpdate,
     UINT flags);






#line 6640 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
int
__stdcall
SetScrollPos(
     HWND hWnd,
     int nBar,
     int nPos,
     BOOL bRedraw);

__declspec(dllimport)
int
__stdcall
GetScrollPos(
     HWND hWnd,
     int nBar);

__declspec(dllimport)
BOOL
__stdcall
SetScrollRange(
     HWND hWnd,
     int nBar,
     int nMinPos,
     int nMaxPos,
     BOOL bRedraw);

__declspec(dllimport)
BOOL
__stdcall
GetScrollRange(
     HWND hWnd,
     int nBar,
     LPINT lpMinPos,
     LPINT lpMaxPos);

__declspec(dllimport)
BOOL
__stdcall
ShowScrollBar(
     HWND hWnd,
     int wBar,
     BOOL bShow);

__declspec(dllimport)
BOOL
__stdcall
EnableScrollBar(
     HWND hWnd,
     UINT wSBflags,
     UINT wArrows);


















#line 6712 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SetPropA(
     HWND hWnd,
     LPCSTR lpString,
     HANDLE hData);
__declspec(dllimport)
BOOL
__stdcall
SetPropW(
     HWND hWnd,
     LPCWSTR lpString,
     HANDLE hData);




#line 6732 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HANDLE
__stdcall
GetPropA(
     HWND hWnd,
     LPCSTR lpString);
__declspec(dllimport)
HANDLE
__stdcall
GetPropW(
     HWND hWnd,
     LPCWSTR lpString);




#line 6750 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HANDLE
__stdcall
RemovePropA(
     HWND hWnd,
     LPCSTR lpString);
__declspec(dllimport)
HANDLE
__stdcall
RemovePropW(
     HWND hWnd,
     LPCWSTR lpString);




#line 6768 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
EnumPropsExA(
     HWND hWnd,
     PROPENUMPROCEXA lpEnumFunc,
     LPARAM lParam);
__declspec(dllimport)
int
__stdcall
EnumPropsExW(
     HWND hWnd,
     PROPENUMPROCEXW lpEnumFunc,
     LPARAM lParam);




#line 6788 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
EnumPropsA(
     HWND hWnd,
     PROPENUMPROCA lpEnumFunc);
__declspec(dllimport)
int
__stdcall
EnumPropsW(
     HWND hWnd,
     PROPENUMPROCW lpEnumFunc);




#line 6806 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SetWindowTextA(
     HWND hWnd,
     LPCSTR lpString);
__declspec(dllimport)
BOOL
__stdcall
SetWindowTextW(
     HWND hWnd,
     LPCWSTR lpString);




#line 6824 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
GetWindowTextA(
     HWND hWnd,
     LPSTR lpString,
     int nMaxCount);
__declspec(dllimport)
int
__stdcall
GetWindowTextW(
     HWND hWnd,
     LPWSTR lpString,
     int nMaxCount);




#line 6844 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
GetWindowTextLengthA(
     HWND hWnd);
__declspec(dllimport)
int
__stdcall
GetWindowTextLengthW(
     HWND hWnd);




#line 6860 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
GetClientRect(
     HWND hWnd,
     LPRECT lpRect);

__declspec(dllimport)
BOOL
__stdcall
GetWindowRect(
     HWND hWnd,
     LPRECT lpRect);

__declspec(dllimport)
BOOL
__stdcall
AdjustWindowRect(
      LPRECT lpRect,
     DWORD dwStyle,
     BOOL bMenu);

__declspec(dllimport)
BOOL
__stdcall
AdjustWindowRectEx(
      LPRECT lpRect,
     DWORD dwStyle,
     BOOL bMenu,
     DWORD dwExStyle);





typedef struct tagHELPINFO      
{
    UINT    cbSize;             
    int     iContextType;       
    int     iCtrlId;            
    HANDLE  hItemHandle;        
    DWORD_PTR dwContextId;      
    POINT   MousePos;           
}  HELPINFO,  *LPHELPINFO;

__declspec(dllimport)
BOOL
__stdcall
SetWindowContextHelpId(
     HWND,
     DWORD);

__declspec(dllimport)
DWORD
__stdcall
GetWindowContextHelpId(
     HWND);

__declspec(dllimport)
BOOL
__stdcall
SetMenuContextHelpId(
     HMENU,
     DWORD);

__declspec(dllimport)
DWORD
__stdcall
GetMenuContextHelpId(
     HMENU);

#line 6933 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"















#line 6949 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"











#line 6961 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 6971 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






#line 6978 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"











#line 6990 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
















__declspec(dllimport)
int
__stdcall
MessageBoxA(
     HWND hWnd,
     LPCSTR lpText,
     LPCSTR lpCaption,
     UINT uType);
__declspec(dllimport)
int
__stdcall
MessageBoxW(
     HWND hWnd,
     LPCWSTR lpText,
     LPCWSTR lpCaption,
     UINT uType);




#line 7027 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
MessageBoxExA(
     HWND hWnd,
     LPCSTR lpText,
     LPCSTR lpCaption,
     UINT uType,
     WORD wLanguageId);
__declspec(dllimport)
int
__stdcall
MessageBoxExW(
     HWND hWnd,
     LPCWSTR lpText,
     LPCWSTR lpCaption,
     UINT uType,
     WORD wLanguageId);




#line 7051 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



typedef void (__stdcall *MSGBOXCALLBACK)(LPHELPINFO lpHelpInfo);

typedef struct tagMSGBOXPARAMSA
{
    UINT        cbSize;
    HWND        hwndOwner;
    HINSTANCE   hInstance;
    LPCSTR      lpszText;
    LPCSTR      lpszCaption;
    DWORD       dwStyle;
    LPCSTR      lpszIcon;
    DWORD_PTR   dwContextHelpId;
    MSGBOXCALLBACK      lpfnMsgBoxCallback;
    DWORD       dwLanguageId;
} MSGBOXPARAMSA, *PMSGBOXPARAMSA, *LPMSGBOXPARAMSA;
typedef struct tagMSGBOXPARAMSW
{
    UINT        cbSize;
    HWND        hwndOwner;
    HINSTANCE   hInstance;
    LPCWSTR     lpszText;
    LPCWSTR     lpszCaption;
    DWORD       dwStyle;
    LPCWSTR     lpszIcon;
    DWORD_PTR   dwContextHelpId;
    MSGBOXCALLBACK      lpfnMsgBoxCallback;
    DWORD       dwLanguageId;
} MSGBOXPARAMSW, *PMSGBOXPARAMSW, *LPMSGBOXPARAMSW;





typedef MSGBOXPARAMSA MSGBOXPARAMS;
typedef PMSGBOXPARAMSA PMSGBOXPARAMS;
typedef LPMSGBOXPARAMSA LPMSGBOXPARAMS;
#line 7091 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
MessageBoxIndirectA(
     const MSGBOXPARAMSA *);
__declspec(dllimport)
int
__stdcall
MessageBoxIndirectW(
     const MSGBOXPARAMSW *);




#line 7107 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 7108 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
BOOL
__stdcall
MessageBeep(
     UINT uType);

#line 7117 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
ShowCursor(
     BOOL bShow);

__declspec(dllimport)
BOOL
__stdcall
SetCursorPos(
     int X,
     int Y);

__declspec(dllimport)
HCURSOR
__stdcall
SetCursor(
     HCURSOR hCursor);

__declspec(dllimport)
BOOL
__stdcall
GetCursorPos(
     LPPOINT lpPoint);

__declspec(dllimport)
BOOL
__stdcall
ClipCursor(
     const RECT *lpRect);

__declspec(dllimport)
BOOL
__stdcall
GetClipCursor(
     LPRECT lpRect);

__declspec(dllimport)
HCURSOR
__stdcall
GetCursor(
    void);

__declspec(dllimport)
BOOL
__stdcall
CreateCaret(
     HWND hWnd,
     HBITMAP hBitmap,
     int nWidth,
     int nHeight);

__declspec(dllimport)
UINT
__stdcall
GetCaretBlinkTime(
    void);

__declspec(dllimport)
BOOL
__stdcall
SetCaretBlinkTime(
     UINT uMSeconds);

__declspec(dllimport)
BOOL
__stdcall
DestroyCaret(
    void);

__declspec(dllimport)
BOOL
__stdcall
HideCaret(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
ShowCaret(
     HWND hWnd);

__declspec(dllimport)
BOOL
__stdcall
SetCaretPos(
     int X,
     int Y);

__declspec(dllimport)
BOOL
__stdcall
GetCaretPos(
     LPPOINT lpPoint);

__declspec(dllimport)
BOOL
__stdcall
ClientToScreen(
     HWND hWnd,
      LPPOINT lpPoint);

__declspec(dllimport)
BOOL
__stdcall
ScreenToClient(
     HWND hWnd,
      LPPOINT lpPoint);

__declspec(dllimport)
int
__stdcall
MapWindowPoints(
     HWND hWndFrom,
     HWND hWndTo,
      LPPOINT lpPoints,
     UINT cPoints);

__declspec(dllimport)
HWND
__stdcall
WindowFromPoint(
     POINT Point);

__declspec(dllimport)
HWND
__stdcall
ChildWindowFromPoint(
     HWND hWndParent,
     POINT Point);







__declspec(dllimport) HWND    __stdcall ChildWindowFromPointEx(  HWND,  POINT,  UINT);
#line 7257 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"










































#line 7300 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 7309 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 7310 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 7319 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
DWORD
__stdcall
GetSysColor(
     int nIndex);


__declspec(dllimport)
HBRUSH
__stdcall
GetSysColorBrush(
     int nIndex);


#line 7336 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SetSysColors(
     int cElements,
     const INT * lpaElements,
     const COLORREF * lpaRgbValues);

#line 7346 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
DrawFocusRect(
     HDC hDC,
     const RECT * lprc);

__declspec(dllimport)
int
__stdcall
FillRect(
     HDC hDC,
     const RECT *lprc,
     HBRUSH hbr);

__declspec(dllimport)
int
__stdcall
FrameRect(
     HDC hDC,
     const RECT *lprc,
     HBRUSH hbr);

__declspec(dllimport)
BOOL
__stdcall
InvertRect(
     HDC hDC,
     const RECT *lprc);

__declspec(dllimport)
BOOL
__stdcall
SetRect(
     LPRECT lprc,
     int xLeft,
     int yTop,
     int xRight,
     int yBottom);

__declspec(dllimport)
BOOL
__stdcall
SetRectEmpty(
     LPRECT lprc);

__declspec(dllimport)
BOOL
__stdcall
CopyRect(
     LPRECT lprcDst,
     const RECT *lprcSrc);

__declspec(dllimport)
BOOL
__stdcall
InflateRect(
      LPRECT lprc,
     int dx,
     int dy);

__declspec(dllimport)
BOOL
__stdcall
IntersectRect(
     LPRECT lprcDst,
     const RECT *lprcSrc1,
     const RECT *lprcSrc2);

__declspec(dllimport)
BOOL
__stdcall
UnionRect(
     LPRECT lprcDst,
     const RECT *lprcSrc1,
     const RECT *lprcSrc2);

__declspec(dllimport)
BOOL
__stdcall
SubtractRect(
     LPRECT lprcDst,
     const RECT *lprcSrc1,
     const RECT *lprcSrc2);

__declspec(dllimport)
BOOL
__stdcall
OffsetRect(
      LPRECT lprc,
     int dx,
     int dy);

__declspec(dllimport)
BOOL
__stdcall
IsRectEmpty(
     const RECT *lprc);

__declspec(dllimport)
BOOL
__stdcall
EqualRect(
     const RECT *lprc1,
     const RECT *lprc2);

__declspec(dllimport)
BOOL
__stdcall
PtInRect(
     const RECT *lprc,
     POINT pt);



__declspec(dllimport)
WORD
__stdcall
GetWindowWord(
     HWND hWnd,
     int nIndex);

__declspec(dllimport)
WORD
__stdcall
SetWindowWord(
     HWND hWnd,
     int nIndex,
     WORD wNewWord);

__declspec(dllimport)
LONG
__stdcall
GetWindowLongA(
     HWND hWnd,
     int nIndex);
__declspec(dllimport)
LONG
__stdcall
GetWindowLongW(
     HWND hWnd,
     int nIndex);




#line 7494 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
LONG
__stdcall
SetWindowLongA(
     HWND hWnd,
     int nIndex,
     LONG dwNewLong);
__declspec(dllimport)
LONG
__stdcall
SetWindowLongW(
     HWND hWnd,
     int nIndex,
     LONG dwNewLong);




#line 7514 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

















































#line 7564 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 7572 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 7574 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
WORD
__stdcall
GetClassWord(
     HWND hWnd,
     int nIndex);

__declspec(dllimport)
WORD
__stdcall
SetClassWord(
     HWND hWnd,
     int nIndex,
     WORD wNewWord);

__declspec(dllimport)
DWORD
__stdcall
GetClassLongA(
     HWND hWnd,
     int nIndex);
__declspec(dllimport)
DWORD
__stdcall
GetClassLongW(
     HWND hWnd,
     int nIndex);




#line 7607 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
DWORD
__stdcall
SetClassLongA(
     HWND hWnd,
     int nIndex,
     LONG dwNewLong);
__declspec(dllimport)
DWORD
__stdcall
SetClassLongW(
     HWND hWnd,
     int nIndex,
     LONG dwNewLong);




#line 7627 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

















































#line 7677 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 7685 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 7687 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 7689 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
BOOL
__stdcall
GetProcessDefaultLayout(
     DWORD *pdwDefaultLayout);

__declspec(dllimport)
BOOL
__stdcall
SetProcessDefaultLayout(
     DWORD dwDefaultLayout);
#line 7703 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
GetDesktopWindow(
    void);


__declspec(dllimport)
HWND
__stdcall
GetParent(
     HWND hWnd);

__declspec(dllimport)
HWND
__stdcall
SetParent(
     HWND hWndChild,
     HWND hWndNewParent);

__declspec(dllimport)
BOOL
__stdcall
EnumChildWindows(
     HWND hWndParent,
     WNDENUMPROC lpEnumFunc,
     LPARAM lParam);

__declspec(dllimport)
HWND
__stdcall
FindWindowA(
     LPCSTR lpClassName,
     LPCSTR lpWindowName);
__declspec(dllimport)
HWND
__stdcall
FindWindowW(
     LPCWSTR lpClassName,
     LPCWSTR lpWindowName);




#line 7749 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport) HWND    __stdcall FindWindowExA(  HWND,  HWND,  LPCSTR,  LPCSTR);
__declspec(dllimport) HWND    __stdcall FindWindowExW(  HWND,  HWND,  LPCWSTR,  LPCWSTR);




#line 7758 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport) HWND    __stdcall  GetShellWindow(void);
#line 7761 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport) BOOL    __stdcall  RegisterShellHookWindow(  HWND);
__declspec(dllimport) BOOL    __stdcall  DeregisterShellHookWindow(  HWND);

__declspec(dllimport)
BOOL
__stdcall
EnumWindows(
     WNDENUMPROC lpEnumFunc,
     LPARAM lParam);

__declspec(dllimport)
BOOL
__stdcall
EnumThreadWindows(
     DWORD dwThreadId,
     WNDENUMPROC lpfn,
     LPARAM lParam);



__declspec(dllimport)
int
__stdcall
GetClassNameA(
     HWND hWnd,
     LPSTR lpClassName,
     int nMaxCount);
__declspec(dllimport)
int
__stdcall
GetClassNameW(
     HWND hWnd,
     LPWSTR lpClassName,
     int nMaxCount);




#line 7802 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
GetTopWindow(
     HWND hWnd);





__declspec(dllimport)
DWORD
__stdcall
GetWindowThreadProcessId(
     HWND hWnd,
     LPDWORD lpdwProcessId);








#line 7828 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




__declspec(dllimport)
HWND
__stdcall
GetLastActivePopup(
     HWND hWnd);












#line 7850 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 7853 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HWND
__stdcall
GetWindow(
     HWND hWnd,
     UINT uCmd);







__declspec(dllimport)
HHOOK
__stdcall
SetWindowsHookA(
     int nFilterType,
     HOOKPROC pfnFilterProc);
__declspec(dllimport)
HHOOK
__stdcall
SetWindowsHookW(
     int nFilterType,
     HOOKPROC pfnFilterProc);




#line 7884 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





















#line 7906 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
UnhookWindowsHook(
     int nCode,
     HOOKPROC pfnFilterProc);

__declspec(dllimport)
HHOOK
__stdcall
SetWindowsHookExA(
     int idHook,
     HOOKPROC lpfn,
     HINSTANCE hmod,
     DWORD dwThreadId);
__declspec(dllimport)
HHOOK
__stdcall
SetWindowsHookExW(
     int idHook,
     HOOKPROC lpfn,
     HINSTANCE hmod,
     DWORD dwThreadId);




#line 7935 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
UnhookWindowsHookEx(
     HHOOK hhk);

__declspec(dllimport)
LRESULT
__stdcall
CallNextHookEx(
     HHOOK hhk,
     int nCode,
     WPARAM wParam,
     LPARAM lParam);










#line 7961 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 7962 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








































#line 8003 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 8008 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 8013 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






















#line 8036 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




__declspec(dllimport)
BOOL
__stdcall
CheckMenuRadioItem(
     HMENU,
     UINT,
     UINT,
     UINT,
     UINT);
#line 8050 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




typedef struct {
    WORD versionNumber;
    WORD offset;
} MENUITEMTEMPLATEHEADER, *PMENUITEMTEMPLATEHEADER;

typedef struct {        
    WORD mtOption;
    WORD mtID;
    WCHAR mtString[1];
} MENUITEMTEMPLATE, *PMENUITEMTEMPLATE;


#line 8067 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



























#line 8095 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 8103 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





__declspec(dllimport)
HBITMAP
__stdcall
LoadBitmapA(
     HINSTANCE hInstance,
     LPCSTR lpBitmapName);
__declspec(dllimport)
HBITMAP
__stdcall
LoadBitmapW(
     HINSTANCE hInstance,
     LPCWSTR lpBitmapName);




#line 8125 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HCURSOR
__stdcall
LoadCursorA(
     HINSTANCE hInstance,
     LPCSTR lpCursorName);
__declspec(dllimport)
HCURSOR
__stdcall
LoadCursorW(
     HINSTANCE hInstance,
     LPCWSTR lpCursorName);




#line 8143 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HCURSOR
__stdcall
LoadCursorFromFileA(
     LPCSTR lpFileName);
__declspec(dllimport)
HCURSOR
__stdcall
LoadCursorFromFileW(
     LPCWSTR lpFileName);




#line 8159 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HCURSOR
__stdcall
CreateCursor(
     HINSTANCE hInst,
     int xHotSpot,
     int yHotSpot,
     int nWidth,
     int nHeight,
     const void *pvANDPlane,
     const void *pvXORPlane);

__declspec(dllimport)
BOOL
__stdcall
DestroyCursor(
     HCURSOR hCursor);









#line 8187 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



















#line 8207 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 8211 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
SetSystemCursor(
     HCURSOR hcur,
     DWORD   id);

typedef struct _ICONINFO {
    BOOL    fIcon;
    DWORD   xHotspot;
    DWORD   yHotspot;
    HBITMAP hbmMask;
    HBITMAP hbmColor;
} ICONINFO;
typedef ICONINFO *PICONINFO;

__declspec(dllimport)
HICON
__stdcall
LoadIconA(
     HINSTANCE hInstance,
     LPCSTR lpIconName);
__declspec(dllimport)
HICON
__stdcall
LoadIconW(
     HINSTANCE hInstance,
     LPCWSTR lpIconName);




#line 8245 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport) UINT PrivateExtractIconsA(
     LPCSTR szFileName,
     int      nIconIndex,
     int      cxIcon,
     int      cyIcon,
     HICON   *phicon,
     UINT    *piconid,
     UINT     nIcons,
     UINT     flags);
__declspec(dllimport) UINT PrivateExtractIconsW(
     LPCWSTR szFileName,
     int      nIconIndex,
     int      cxIcon,
     int      cyIcon,
     HICON   *phicon,
     UINT    *piconid,
     UINT     nIcons,
     UINT     flags);




#line 8270 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HICON
__stdcall
CreateIcon(
     HINSTANCE hInstance,
     int nWidth,
     int nHeight,
     BYTE cPlanes,
     BYTE cBitsPixel,
     const BYTE *lpbANDbits,
     const BYTE *lpbXORbits);

__declspec(dllimport)
BOOL
__stdcall
DestroyIcon(
     HICON hIcon);

__declspec(dllimport)
int
__stdcall
LookupIconIdFromDirectory(
     PBYTE presbits,
     BOOL fIcon);


__declspec(dllimport)
int
__stdcall
LookupIconIdFromDirectoryEx(
     PBYTE presbits,
     BOOL  fIcon,
     int   cxDesired,
     int   cyDesired,
     UINT  Flags);
#line 8307 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HICON
__stdcall
CreateIconFromResource(
     PBYTE presbits,
     DWORD dwResSize,
     BOOL fIcon,
     DWORD dwVer);


__declspec(dllimport)
HICON
__stdcall
CreateIconFromResourceEx(
     PBYTE presbits,
     DWORD dwResSize,
     BOOL  fIcon,
     DWORD dwVer,
     int   cxDesired,
     int   cyDesired,
     UINT  Flags);


typedef struct tagCURSORSHAPE
{
    int     xHotSpot;
    int     yHotSpot;
    int     cx;
    int     cy;
    int     cbWidth;
    BYTE    Planes;
    BYTE    BitsPixel;
} CURSORSHAPE,  *LPCURSORSHAPE;
#line 8342 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





















__declspec(dllimport)
HANDLE
__stdcall
LoadImageA(
     HINSTANCE,
     LPCSTR,
     UINT,
     int,
     int,
     UINT);
__declspec(dllimport)
HANDLE
__stdcall
LoadImageW(
     HINSTANCE,
     LPCWSTR,
     UINT,
     int,
     int,
     UINT);




#line 8388 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HANDLE
__stdcall
CopyImage(
     HANDLE,
     UINT,
     int,
     int,
     UINT);








#line 8407 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport) BOOL __stdcall DrawIconEx(  HDC hdc,  int xLeft,  int yTop,
               HICON hIcon,  int cxWidth,  int cyWidth,
               UINT istepIfAniCur,  HBRUSH hbrFlickerFreeDraw,  UINT diFlags);
#line 8412 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
HICON
__stdcall
CreateIconIndirect(
     PICONINFO piconinfo);

__declspec(dllimport)
HICON
__stdcall
CopyIcon(
     HICON hIcon);

__declspec(dllimport)
BOOL
__stdcall
GetIconInfo(
     HICON hIcon,
     PICONINFO piconinfo);




#line 8436 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"












































































































#line 8545 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 8546 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





#line 8552 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 8555 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
LoadStringA(
     HINSTANCE hInstance,
     UINT uID,
     LPSTR lpBuffer,
     int nBufferMax);
__declspec(dllimport)
int
__stdcall
LoadStringW(
     HINSTANCE hInstance,
     UINT uID,
     LPWSTR lpBuffer,
     int nBufferMax);




#line 8577 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"















#line 8593 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 8598 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 8603 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 8604 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





























#line 8634 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 8637 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
















#line 8654 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






#line 8661 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









#line 8671 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"













































#line 8717 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 8722 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 8725 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









































#line 8767 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
















#line 8784 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



















#line 8804 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

























#line 8830 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 8833 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"












#line 8846 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
















#line 8863 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 8865 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"































__declspec(dllimport)
BOOL
__stdcall
IsDialogMessageA(
     HWND hDlg,
     LPMSG lpMsg);
__declspec(dllimport)
BOOL
__stdcall
IsDialogMessageW(
     HWND hDlg,
     LPMSG lpMsg);




#line 8913 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 8915 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
MapDialogRect(
     HWND hDlg,
      LPRECT lpRect);

__declspec(dllimport)
int
__stdcall
DlgDirListA(
     HWND hDlg,
      LPSTR lpPathSpec,
     int nIDListBox,
     int nIDStaticPath,
     UINT uFileType);
__declspec(dllimport)
int
__stdcall
DlgDirListW(
     HWND hDlg,
      LPWSTR lpPathSpec,
     int nIDListBox,
     int nIDStaticPath,
     UINT uFileType);




#line 8946 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"















__declspec(dllimport)
BOOL
__stdcall
DlgDirSelectExA(
     HWND hDlg,
     LPSTR lpString,
     int nCount,
     int nIDListBox);
__declspec(dllimport)
BOOL
__stdcall
DlgDirSelectExW(
     HWND hDlg,
     LPWSTR lpString,
     int nCount,
     int nIDListBox);




#line 8982 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
int
__stdcall
DlgDirListComboBoxA(
     HWND hDlg,
      LPSTR lpPathSpec,
     int nIDComboBox,
     int nIDStaticPath,
     UINT uFiletype);
__declspec(dllimport)
int
__stdcall
DlgDirListComboBoxW(
     HWND hDlg,
      LPWSTR lpPathSpec,
     int nIDComboBox,
     int nIDStaticPath,
     UINT uFiletype);




#line 9006 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
DlgDirSelectComboBoxExA(
     HWND hDlg,
     LPSTR lpString,
     int nCount,
     int nIDComboBox);
__declspec(dllimport)
BOOL
__stdcall
DlgDirSelectComboBoxExW(
     HWND hDlg,
     LPWSTR lpString,
     int nCount,
     int nIDComboBox);




#line 9028 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

























#line 9054 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 9058 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 9066 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


































                                  




























































#line 9162 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 9165 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 9170 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 9174 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 9176 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 9180 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 9182 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"























#line 9206 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





#line 9212 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"












































#line 9257 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 9259 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"











































#line 9303 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 9304 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 9308 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 9312 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 9314 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 9318 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 9319 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




















#line 9340 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 9343 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"














#line 9358 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



#line 9362 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









typedef struct tagSCROLLINFO
{
    UINT    cbSize;
    UINT    fMask;
    int     nMin;
    int     nMax;
    UINT    nPage;
    int     nPos;
    int     nTrackPos;
}   SCROLLINFO,  *LPSCROLLINFO;
typedef SCROLLINFO const  *LPCSCROLLINFO;

__declspec(dllimport) int     __stdcall SetScrollInfo( HWND,  int,  LPCSCROLLINFO,  BOOL);
__declspec(dllimport) BOOL    __stdcall GetScrollInfo( HWND,  int,   LPSCROLLINFO);

#line 9387 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 9388 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 9389 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
















#line 9406 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct tagMDICREATESTRUCTA {
    LPCSTR   szClass;
    LPCSTR   szTitle;
    HANDLE hOwner;
    int x;
    int y;
    int cx;
    int cy;
    DWORD style;
    LPARAM lParam;        
} MDICREATESTRUCTA, *LPMDICREATESTRUCTA;
typedef struct tagMDICREATESTRUCTW {
    LPCWSTR  szClass;
    LPCWSTR  szTitle;
    HANDLE hOwner;
    int x;
    int y;
    int cx;
    int cy;
    DWORD style;
    LPARAM lParam;        
} MDICREATESTRUCTW, *LPMDICREATESTRUCTW;




typedef MDICREATESTRUCTA MDICREATESTRUCT;
typedef LPMDICREATESTRUCTA LPMDICREATESTRUCT;
#line 9436 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct tagCLIENTCREATESTRUCT {
    HANDLE hWindowMenu;
    UINT idFirstChild;
} CLIENTCREATESTRUCT, *LPCLIENTCREATESTRUCT;

__declspec(dllimport)
LRESULT
__stdcall
DefFrameProcA(
     HWND hWnd,
     HWND hWndMDIClient,
     UINT uMsg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)
LRESULT
__stdcall
DefFrameProcW(
     HWND hWnd,
     HWND hWndMDIClient,
     UINT uMsg,
     WPARAM wParam,
     LPARAM lParam);




#line 9465 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)

LRESULT
__stdcall



#line 9474 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
DefMDIChildProcA(
     HWND hWnd,
     UINT uMsg,
     WPARAM wParam,
     LPARAM lParam);
__declspec(dllimport)

LRESULT
__stdcall



#line 9487 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
DefMDIChildProcW(
     HWND hWnd,
     UINT uMsg,
     WPARAM wParam,
     LPARAM lParam);




#line 9497 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
BOOL
__stdcall
TranslateMDISysAccel(
     HWND hWndClient,
     LPMSG lpMsg);

#line 9508 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
UINT
__stdcall
ArrangeIconicWindows(
     HWND hWnd);

__declspec(dllimport)
HWND
__stdcall
CreateMDIWindowA(
     LPCSTR lpClassName,
     LPCSTR lpWindowName,
     DWORD dwStyle,
     int X,
     int Y,
     int nWidth,
     int nHeight,
     HWND hWndParent,
     HINSTANCE hInstance,
     LPARAM lParam
    );
__declspec(dllimport)
HWND
__stdcall
CreateMDIWindowW(
     LPCWSTR lpClassName,
     LPCWSTR lpWindowName,
     DWORD dwStyle,
     int X,
     int Y,
     int nWidth,
     int nHeight,
     HWND hWndParent,
     HINSTANCE hInstance,
     LPARAM lParam
    );




#line 9550 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport) WORD    __stdcall TileWindows(  HWND hwndParent,  UINT wHow,  const RECT * lpRect,  UINT cKids,  const HWND  * lpKids);
__declspec(dllimport) WORD    __stdcall CascadeWindows(  HWND hwndParent,  UINT wHow,  const RECT * lpRect,  UINT cKids,   const HWND  * lpKids);
#line 9555 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 9556 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 9558 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





typedef DWORD HELPPOLY;
typedef struct tagMULTIKEYHELPA {

    DWORD  mkSize;


#line 9570 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
    CHAR   mkKeylist;
    CHAR   szKeyphrase[1];
} MULTIKEYHELPA, *PMULTIKEYHELPA, *LPMULTIKEYHELPA;
typedef struct tagMULTIKEYHELPW {

    DWORD  mkSize;


#line 9579 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
    WCHAR  mkKeylist;
    WCHAR  szKeyphrase[1];
} MULTIKEYHELPW, *PMULTIKEYHELPW, *LPMULTIKEYHELPW;





typedef MULTIKEYHELPA MULTIKEYHELP;
typedef PMULTIKEYHELPA PMULTIKEYHELP;
typedef LPMULTIKEYHELPA LPMULTIKEYHELP;
#line 9591 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct tagHELPWININFOA {
    int  wStructSize;
    int  x;
    int  y;
    int  dx;
    int  dy;
    int  wMax;
    CHAR   rgchMember[2];
} HELPWININFOA, *PHELPWININFOA, *LPHELPWININFOA;
typedef struct tagHELPWININFOW {
    int  wStructSize;
    int  x;
    int  y;
    int  dx;
    int  dy;
    int  wMax;
    WCHAR  rgchMember[2];
} HELPWININFOW, *PHELPWININFOW, *LPHELPWININFOW;





typedef HELPWININFOA HELPWININFO;
typedef PHELPWININFOA PHELPWININFO;
typedef LPHELPWININFOA LPHELPWININFO;
#line 9619 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





































#line 9657 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
BOOL
__stdcall
WinHelpA(
     HWND hWndMain,
     LPCSTR lpszHelp,
     UINT uCommand,
     ULONG_PTR dwData
    );
__declspec(dllimport)
BOOL
__stdcall
WinHelpW(
     HWND hWndMain,
     LPCWSTR lpszHelp,
     UINT uCommand,
     ULONG_PTR dwData
    );




#line 9683 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 9685 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"






__declspec(dllimport)
DWORD
__stdcall
GetGuiResources(
     HANDLE hProcess,
     DWORD uiFlags);

#line 9699 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





















































































#line 9785 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"















#line 9801 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





#line 9807 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"















#line 9823 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 9831 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"












































#line 9876 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





































#line 9914 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 9916 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"












typedef struct tagNONCLIENTMETRICSA
{
    UINT    cbSize;
    int     iBorderWidth;
    int     iScrollWidth;
    int     iScrollHeight;
    int     iCaptionWidth;
    int     iCaptionHeight;
    LOGFONTA lfCaptionFont;
    int     iSmCaptionWidth;
    int     iSmCaptionHeight;
    LOGFONTA lfSmCaptionFont;
    int     iMenuWidth;
    int     iMenuHeight;
    LOGFONTA lfMenuFont;
    LOGFONTA lfStatusFont;
    LOGFONTA lfMessageFont;
}   NONCLIENTMETRICSA, *PNONCLIENTMETRICSA, * LPNONCLIENTMETRICSA;
typedef struct tagNONCLIENTMETRICSW
{
    UINT    cbSize;
    int     iBorderWidth;
    int     iScrollWidth;
    int     iScrollHeight;
    int     iCaptionWidth;
    int     iCaptionHeight;
    LOGFONTW lfCaptionFont;
    int     iSmCaptionWidth;
    int     iSmCaptionHeight;
    LOGFONTW lfSmCaptionFont;
    int     iMenuWidth;
    int     iMenuHeight;
    LOGFONTW lfMenuFont;
    LOGFONTW lfStatusFont;
    LOGFONTW lfMessageFont;
}   NONCLIENTMETRICSW, *PNONCLIENTMETRICSW, * LPNONCLIENTMETRICSW;





typedef NONCLIENTMETRICSA NONCLIENTMETRICS;
typedef PNONCLIENTMETRICSA PNONCLIENTMETRICS;
typedef LPNONCLIENTMETRICSA LPNONCLIENTMETRICS;
#line 9973 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 9974 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 9975 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"















typedef struct tagMINIMIZEDMETRICS
{
    UINT    cbSize;
    int     iWidth;
    int     iHorzGap;
    int     iVertGap;
    int     iArrange;
}   MINIMIZEDMETRICS, *PMINIMIZEDMETRICS, *LPMINIMIZEDMETRICS;



typedef struct tagICONMETRICSA
{
    UINT    cbSize;
    int     iHorzSpacing;
    int     iVertSpacing;
    int     iTitleWrap;
    LOGFONTA lfFont;
}   ICONMETRICSA, *PICONMETRICSA, *LPICONMETRICSA;
typedef struct tagICONMETRICSW
{
    UINT    cbSize;
    int     iHorzSpacing;
    int     iVertSpacing;
    int     iTitleWrap;
    LOGFONTW lfFont;
}   ICONMETRICSW, *PICONMETRICSW, *LPICONMETRICSW;





typedef ICONMETRICSA ICONMETRICS;
typedef PICONMETRICSA PICONMETRICS;
typedef LPICONMETRICSA LPICONMETRICS;
#line 10026 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 10027 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 10028 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct tagANIMATIONINFO
{
    UINT    cbSize;
    int     iMinAnimate;
}   ANIMATIONINFO, *LPANIMATIONINFO;

typedef struct tagSERIALKEYSA
{
    UINT    cbSize;
    DWORD   dwFlags;
    LPSTR     lpszActivePort;
    LPSTR     lpszPort;
    UINT    iBaudRate;
    UINT    iPortState;
    UINT    iActive;
}   SERIALKEYSA, *LPSERIALKEYSA;
typedef struct tagSERIALKEYSW
{
    UINT    cbSize;
    DWORD   dwFlags;
    LPWSTR    lpszActivePort;
    LPWSTR    lpszPort;
    UINT    iBaudRate;
    UINT    iPortState;
    UINT    iActive;
}   SERIALKEYSW, *LPSERIALKEYSW;




typedef SERIALKEYSA SERIALKEYS;
typedef LPSERIALKEYSA LPSERIALKEYS;
#line 10062 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







typedef struct tagHIGHCONTRASTA
{
    UINT    cbSize;
    DWORD   dwFlags;
    LPSTR   lpszDefaultScheme;
}   HIGHCONTRASTA, *LPHIGHCONTRASTA;
typedef struct tagHIGHCONTRASTW
{
    UINT    cbSize;
    DWORD   dwFlags;
    LPWSTR  lpszDefaultScheme;
}   HIGHCONTRASTW, *LPHIGHCONTRASTW;




typedef HIGHCONTRASTA HIGHCONTRAST;
typedef LPHIGHCONTRASTA LPHIGHCONTRAST;
#line 10088 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




















#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\tvout.h"










#pragma once
#line 13 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\tvout.h"





typedef struct _VIDEOPARAMETERS {
    GUID  Guid;                         
    ULONG dwOffset;                     
    ULONG dwCommand;                    
    ULONG dwFlags;                      
    ULONG dwMode;                       
    ULONG dwTVStandard;                 
    ULONG dwAvailableModes;             
    ULONG dwAvailableTVStandard;        
    ULONG dwFlickerFilter;              
    ULONG dwOverScanX;                  
    ULONG dwOverScanY;                  
    ULONG dwMaxUnscaledX;               
    ULONG dwMaxUnscaledY;               
    ULONG dwPositionX;                  
    ULONG dwPositionY;                  
    ULONG dwBrightness;                 
    ULONG dwContrast;                   
    ULONG dwCPType;                     
    ULONG dwCPCommand;                  
    ULONG dwCPStandard;                 
    ULONG dwCPKey;
    ULONG bCP_APSTriggerBits;           
    UCHAR bOEMCopyProtection[256];      
} VIDEOPARAMETERS, *PVIDEOPARAMETERS, *LPVIDEOPARAMETERS;


                                        












































#line 91 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\include\\tvout.h"
#line 10109 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"











#line 10121 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




__declspec(dllimport)
LONG
__stdcall
ChangeDisplaySettingsA(
     LPDEVMODEA  lpDevMode,
     DWORD       dwFlags);
__declspec(dllimport)
LONG
__stdcall
ChangeDisplaySettingsW(
     LPDEVMODEW  lpDevMode,
     DWORD       dwFlags);




#line 10142 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
LONG
__stdcall
ChangeDisplaySettingsExA(
     LPCSTR    lpszDeviceName,
     LPDEVMODEA  lpDevMode,
     HWND        hwnd,
     DWORD       dwflags,
     LPVOID      lParam);
__declspec(dllimport)
LONG
__stdcall
ChangeDisplaySettingsExW(
     LPCWSTR    lpszDeviceName,
     LPDEVMODEW  lpDevMode,
     HWND        hwnd,
     DWORD       dwflags,
     LPVOID      lParam);




#line 10166 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




__declspec(dllimport)
BOOL
__stdcall
EnumDisplaySettingsA(
     LPCSTR lpszDeviceName,
     DWORD iModeNum,
     LPDEVMODEA lpDevMode);
__declspec(dllimport)
BOOL
__stdcall
EnumDisplaySettingsW(
     LPCWSTR lpszDeviceName,
     DWORD iModeNum,
     LPDEVMODEW lpDevMode);




#line 10189 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"



__declspec(dllimport)
BOOL
__stdcall
EnumDisplaySettingsExA(
     LPCSTR lpszDeviceName,
     DWORD iModeNum,
     LPDEVMODEA lpDevMode,
     DWORD dwFlags);
__declspec(dllimport)
BOOL
__stdcall
EnumDisplaySettingsExW(
     LPCWSTR lpszDeviceName,
     DWORD iModeNum,
     LPDEVMODEW lpDevMode,
     DWORD dwFlags);




#line 10213 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




__declspec(dllimport)
BOOL
__stdcall
EnumDisplayDevicesA(
     LPCSTR lpDevice,
     DWORD iDevNum,
     PDISPLAY_DEVICEA lpDisplayDevice,
     DWORD dwFlags);
__declspec(dllimport)
BOOL
__stdcall
EnumDisplayDevicesW(
     LPCWSTR lpDevice,
     DWORD iDevNum,
     PDISPLAY_DEVICEW lpDisplayDevice,
     DWORD dwFlags);




#line 10238 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 10239 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 10241 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 10242 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


__declspec(dllimport)
BOOL
__stdcall
SystemParametersInfoA(
     UINT uiAction,
     UINT uiParam,
      PVOID pvParam,
     UINT fWinIni);
__declspec(dllimport)
BOOL
__stdcall
SystemParametersInfoW(
     UINT uiAction,
     UINT uiParam,
      PVOID pvParam,
     UINT fWinIni);




#line 10265 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


#line 10268 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




typedef struct tagFILTERKEYS
{
    UINT  cbSize;
    DWORD dwFlags;
    DWORD iWaitMSec;            
    DWORD iDelayMSec;           
    DWORD iRepeatMSec;          
    DWORD iBounceMSec;          
} FILTERKEYS, *LPFILTERKEYS;












typedef struct tagSTICKYKEYS
{
    UINT  cbSize;
    DWORD dwFlags;
} STICKYKEYS, *LPSTICKYKEYS;






























#line 10329 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct tagMOUSEKEYS
{
    UINT cbSize;
    DWORD dwFlags;
    DWORD iMaxSpeed;
    DWORD iTimeToMaxSpeed;
    DWORD iCtrlSpeed;
    DWORD dwReserved1;
    DWORD dwReserved2;
} MOUSEKEYS, *LPMOUSEKEYS;


















#line 10359 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef struct tagACCESSTIMEOUT
{
    UINT  cbSize;
    DWORD dwFlags;
    DWORD iTimeOutMSec;
} ACCESSTIMEOUT, *LPACCESSTIMEOUT;
























typedef struct tagSOUNDSENTRYA
{
    UINT cbSize;
    DWORD dwFlags;
    DWORD iFSTextEffect;
    DWORD iFSTextEffectMSec;
    DWORD iFSTextEffectColorBits;
    DWORD iFSGrafEffect;
    DWORD iFSGrafEffectMSec;
    DWORD iFSGrafEffectColor;
    DWORD iWindowsEffect;
    DWORD iWindowsEffectMSec;
    LPSTR   lpszWindowsEffectDLL;
    DWORD iWindowsEffectOrdinal;
} SOUNDSENTRYA, *LPSOUNDSENTRYA;
typedef struct tagSOUNDSENTRYW
{
    UINT cbSize;
    DWORD dwFlags;
    DWORD iFSTextEffect;
    DWORD iFSTextEffectMSec;
    DWORD iFSTextEffectColorBits;
    DWORD iFSGrafEffect;
    DWORD iFSGrafEffectMSec;
    DWORD iFSGrafEffectColor;
    DWORD iWindowsEffect;
    DWORD iWindowsEffectMSec;
    LPWSTR  lpszWindowsEffectDLL;
    DWORD iWindowsEffectOrdinal;
} SOUNDSENTRYW, *LPSOUNDSENTRYW;




typedef SOUNDSENTRYA SOUNDSENTRY;
typedef LPSOUNDSENTRYA LPSOUNDSENTRY;
#line 10427 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








typedef struct tagTOGGLEKEYS
{
    UINT cbSize;
    DWORD dwFlags;
} TOGGLEKEYS, *LPTOGGLEKEYS;















__declspec(dllimport)
void
__stdcall
SetDebugErrorLevel(
     DWORD dwLevel
    );









__declspec(dllimport)
void
__stdcall
SetLastErrorEx(
     DWORD dwErrCode,
     DWORD dwType
    );

__declspec(dllimport)
int
__stdcall
InternalGetWindowText(
     HWND hWnd,
     LPWSTR lpString,
     int nMaxCount);









#line 10495 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"












__declspec(dllimport)
HMONITOR
__stdcall
MonitorFromPoint(
     POINT pt,
     DWORD dwFlags);

__declspec(dllimport)
HMONITOR
__stdcall
MonitorFromRect(
     LPCRECT lprc,
     DWORD dwFlags);

__declspec(dllimport)
HMONITOR
__stdcall
MonitorFromWindow(  HWND hwnd,  DWORD dwFlags);







typedef struct tagMONITORINFO
{
    DWORD   cbSize;
    RECT    rcMonitor;
    RECT    rcWork;
    DWORD   dwFlags;
} MONITORINFO, *LPMONITORINFO;


typedef struct tagMONITORINFOEXA : public tagMONITORINFO
{
    CHAR        szDevice[32];
} MONITORINFOEXA, *LPMONITORINFOEXA;
typedef struct tagMONITORINFOEXW : public tagMONITORINFO
{
    WCHAR       szDevice[32];
} MONITORINFOEXW, *LPMONITORINFOEXW;




typedef MONITORINFOEXA MONITORINFOEX;
typedef LPMONITORINFOEXA LPMONITORINFOEX;
#line 10556 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


















#line 10575 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport) BOOL __stdcall GetMonitorInfoA(  HMONITOR hMonitor,  LPMONITORINFO lpmi);
__declspec(dllimport) BOOL __stdcall GetMonitorInfoW(  HMONITOR hMonitor,  LPMONITORINFO lpmi);




#line 10583 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

typedef BOOL (__stdcall* MONITORENUMPROC)(HMONITOR, HDC, LPRECT, LPARAM);

__declspec(dllimport)
BOOL
__stdcall
EnumDisplayMonitors(
     HDC             hdc,
     LPCRECT         lprcClip,
     MONITORENUMPROC lpfnEnum,
     LPARAM          dwData);








__declspec(dllimport)
void
__stdcall
NotifyWinEvent(
     DWORD event,
     HWND  hwnd,
     LONG  idObject,
     LONG  idChild);

typedef void (__stdcall* WINEVENTPROC)(
    HWINEVENTHOOK hWinEventHook,
    DWORD         event,
    HWND          hwnd,
    LONG          idObject,
    LONG          idChild,
    DWORD         idEventThread,
    DWORD         dwmsEventTime);

__declspec(dllimport)
HWINEVENTHOOK
__stdcall
SetWinEventHook(
     DWORD        eventMin,
     DWORD        eventMax,
     HMODULE      hmodWinEventProc,
     WINEVENTPROC pfnWinEventProc,
     DWORD        idProcess,
     DWORD        idThread,
     DWORD        dwFlags);







#line 10639 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"









__declspec(dllimport)
BOOL
__stdcall
UnhookWinEvent(
     HWINEVENTHOOK hWinEventHook);

































































































































































































































#line 10879 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




















































































































































































































typedef struct tagGUITHREADINFO
{
    DWORD   cbSize;
    DWORD   flags;
    HWND    hwndActive;
    HWND    hwndFocus;
    HWND    hwndCapture;
    HWND    hwndMenuOwner;
    HWND    hwndMoveSize;
    HWND    hwndCaret;
    RECT    rcCaret;
} GUITHREADINFO, *PGUITHREADINFO,  * LPGUITHREADINFO;








#line 11112 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

__declspec(dllimport)
BOOL
__stdcall
GetGUIThreadInfo(
     DWORD idThread,
     PGUITHREADINFO pgui);


__declspec(dllimport)
UINT
__stdcall
GetWindowModuleFileNameA(
     HWND     hwnd,
     LPSTR pszFileName,
     UINT     cchFileNameMax);
__declspec(dllimport)
UINT
__stdcall
GetWindowModuleFileNameW(
     HWND     hwnd,
     LPWSTR pszFileName,
     UINT     cchFileNameMax);




#line 11140 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


































#line 11175 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







typedef struct tagCURSORINFO
{
    DWORD   cbSize;
    DWORD   flags;
    HCURSOR hCursor;
    POINT   ptScreenPos;
} CURSORINFO, *PCURSORINFO, *LPCURSORINFO;



__declspec(dllimport)
BOOL
__stdcall
GetCursorInfo(
     PCURSORINFO pci
);




typedef struct tagWINDOWINFO
{
    DWORD cbSize;
    RECT  rcWindow;
    RECT  rcClient;
    DWORD dwStyle;
    DWORD dwExStyle;
    DWORD dwWindowStatus;
    UINT  cxWindowBorders;
    UINT  cyWindowBorders;
    ATOM  atomWindowType;
    WORD  wCreatorVersion;
} WINDOWINFO, *PWINDOWINFO, *LPWINDOWINFO;



__declspec(dllimport)
BOOL
__stdcall
GetWindowInfo(
     HWND hwnd,
     PWINDOWINFO pwi
);




typedef struct tagTITLEBARINFO
{
    DWORD cbSize;
    RECT  rcTitleBar;
    DWORD rgstate[5+1];
} TITLEBARINFO, *PTITLEBARINFO, *LPTITLEBARINFO;

__declspec(dllimport)
BOOL
__stdcall
GetTitleBarInfo(
     HWND hwnd,
     PTITLEBARINFO pti
);




typedef struct tagMENUBARINFO
{
    DWORD cbSize;
    RECT  rcBar;          
    HMENU hMenu;          
    HWND  hwndMenu;       
    BOOL  fBarFocused:1;  
    BOOL  fFocused:1;     
} MENUBARINFO, *PMENUBARINFO, *LPMENUBARINFO;

__declspec(dllimport)
BOOL
__stdcall
GetMenuBarInfo(
     HWND hwnd,
     LONG idObject,
     LONG idItem,
     PMENUBARINFO pmbi
);




typedef struct tagSCROLLBARINFO
{
    DWORD cbSize;
    RECT  rcScrollBar;
    int   dxyLineButton;
    int   xyThumbTop;
    int   xyThumbBottom;
    int   reserved;
    DWORD rgstate[5+1];
} SCROLLBARINFO, *PSCROLLBARINFO, *LPSCROLLBARINFO;

__declspec(dllimport)
BOOL
__stdcall
GetScrollBarInfo(
     HWND hwnd,
     LONG idObject,
     PSCROLLBARINFO psbi
);




typedef struct tagCOMBOBOXINFO
{
    DWORD cbSize;
    RECT  rcItem;
    RECT  rcButton;
    DWORD stateButton;
    HWND  hwndCombo;
    HWND  hwndItem;
    HWND  hwndList;
} COMBOBOXINFO, *PCOMBOBOXINFO, *LPCOMBOBOXINFO;

__declspec(dllimport)
BOOL
__stdcall
GetComboBoxInfo(
     HWND hwndCombo,
     PCOMBOBOXINFO pcbi
);








__declspec(dllimport)
HWND
__stdcall
GetAncestor(
     HWND hwnd,
     UINT gaFlags
);








__declspec(dllimport)
HWND
__stdcall
RealChildWindowFromPoint(
     HWND hwndParent,
     POINT ptParentClientCoords
);






__declspec(dllimport)
UINT
__stdcall
RealGetWindowClassA(
     HWND  hwnd,
     LPSTR pszType,
     UINT  cchType
);




__declspec(dllimport)
UINT
__stdcall
RealGetWindowClassW(
     HWND  hwnd,
     LPWSTR pszType,
     UINT  cchType
);




#line 11372 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




typedef struct tagALTTABINFO
{
    DWORD cbSize;
    int   cItems;
    int   cColumns;
    int   cRows;
    int   iColFocus;
    int   iRowFocus;
    int   cxItem;
    int   cyItem;
    POINT ptStart;
} ALTTABINFO, *PALTTABINFO, *LPALTTABINFO;

__declspec(dllimport)
BOOL
__stdcall
GetAltTabInfoA(
     HWND hwnd,
     int iItem,
     PALTTABINFO pati,
     LPSTR pszItemText,
     UINT cchItemText
);
__declspec(dllimport)
BOOL
__stdcall
GetAltTabInfoW(
     HWND hwnd,
     int iItem,
     PALTTABINFO pati,
     LPWSTR pszItemText,
     UINT cchItemText
);




#line 11414 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"





__declspec(dllimport)
DWORD
__stdcall
GetListBoxInfo(
     HWND hwnd
);

#line 11427 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 11428 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"








#line 11437 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"











#line 11449 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"















































































































































































































































































































































































#line 11817 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"







#line 11825 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"
#line 11826 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"


}
#line 11830 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"

#line 11832 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winuser.h"




#line 165 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"





















extern "C" {
#line 24 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"








































#line 65 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"




















































































#line 150 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"















































































































































































































































#line 390 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"























































































































#line 510 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"










#line 521 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"





















#line 543 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"


























#line 570 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"























































#line 626 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"



























































typedef DWORD LGRPID;





typedef DWORD LCTYPE;





typedef DWORD CALTYPE;





typedef DWORD CALID;






typedef struct _cpinfo {
    UINT    MaxCharSize;                    
    BYTE    DefaultChar[2];   
    BYTE    LeadByte[12];        
} CPINFO, *LPCPINFO;

typedef struct _cpinfoexA {
    UINT    MaxCharSize;                    
    BYTE    DefaultChar[2];   
    BYTE    LeadByte[12];        
    WCHAR   UnicodeDefaultChar;             
    UINT    CodePage;                       
    CHAR    CodePageName[260];         
} CPINFOEXA, *LPCPINFOEXA;
typedef struct _cpinfoexW {
    UINT    MaxCharSize;                    
    BYTE    DefaultChar[2];   
    BYTE    LeadByte[12];        
    WCHAR   UnicodeDefaultChar;             
    UINT    CodePage;                       
    WCHAR   CodePageName[260];         
} CPINFOEXW, *LPCPINFOEXW;




typedef CPINFOEXA CPINFOEX;
typedef LPCPINFOEXA LPCPINFOEX;
#line 739 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"






typedef struct _numberfmtA {
    UINT    NumDigits;                 
    UINT    LeadingZero;               
    UINT    Grouping;                  
    LPSTR   lpDecimalSep;              
    LPSTR   lpThousandSep;             
    UINT    NegativeOrder;             
} NUMBERFMTA, *LPNUMBERFMTA;
typedef struct _numberfmtW {
    UINT    NumDigits;                 
    UINT    LeadingZero;               
    UINT    Grouping;                  
    LPWSTR  lpDecimalSep;              
    LPWSTR  lpThousandSep;             
    UINT    NegativeOrder;             
} NUMBERFMTW, *LPNUMBERFMTW;




typedef NUMBERFMTA NUMBERFMT;
typedef LPNUMBERFMTA LPNUMBERFMT;
#line 768 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"






typedef struct _currencyfmtA {
    UINT    NumDigits;                 
    UINT    LeadingZero;               
    UINT    Grouping;                  
    LPSTR   lpDecimalSep;              
    LPSTR   lpThousandSep;             
    UINT    NegativeOrder;             
    UINT    PositiveOrder;             
    LPSTR   lpCurrencySymbol;          
} CURRENCYFMTA, *LPCURRENCYFMTA;
typedef struct _currencyfmtW {
    UINT    NumDigits;                 
    UINT    LeadingZero;               
    UINT    Grouping;                  
    LPWSTR  lpDecimalSep;              
    LPWSTR  lpThousandSep;             
    UINT    NegativeOrder;             
    UINT    PositiveOrder;             
    LPWSTR  lpCurrencySymbol;          
} CURRENCYFMTW, *LPCURRENCYFMTW;




typedef CURRENCYFMTA CURRENCYFMT;
typedef LPCURRENCYFMTA LPCURRENCYFMT;
#line 801 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"





enum SYSNLS_FUNCTION{
    COMPARE_STRING    =  0x0001,
};
typedef DWORD NLS_FUNCTION;






typedef struct _nlsversioninfo{ 
    DWORD dwNLSVersionInfoSize; 
    DWORD dwNLSVersion; 
    DWORD dwDefinedVersion; 
} NLSVERSIONINFO, *LPNLSVERSIONINFO; 






typedef LONG    GEOID;
typedef DWORD   GEOTYPE;
typedef DWORD   GEOCLASS;







enum SYSGEOTYPE {
    GEO_NATION      =       0x0001,
    GEO_LATITUDE    =       0x0002,
    GEO_LONGITUDE   =       0x0003,
    GEO_ISO2        =       0x0004,
    GEO_ISO3        =       0x0005,
    GEO_RFC1766     =       0x0006,
    GEO_LCID        =       0x0007,
    GEO_FRIENDLYNAME=       0x0008,
    GEO_OFFICIALNAME=       0x0009,
    GEO_TIMEZONES   =       0x000A,
    GEO_OFFICIALLANGUAGES = 0x000B,
};





enum SYSGEOCLASS {
    GEOCLASS_NATION  = 16,
    GEOCLASS_REGION  = 14,
};








typedef BOOL (__stdcall* LANGUAGEGROUP_ENUMPROCA)(LGRPID, LPSTR, LPSTR, DWORD, LONG_PTR);
typedef BOOL (__stdcall* LANGGROUPLOCALE_ENUMPROCA)(LGRPID, LCID, LPSTR, LONG_PTR);
typedef BOOL (__stdcall* UILANGUAGE_ENUMPROCA)(LPSTR, LONG_PTR);
typedef BOOL (__stdcall* LOCALE_ENUMPROCA)(LPSTR);
typedef BOOL (__stdcall* CODEPAGE_ENUMPROCA)(LPSTR);
typedef BOOL (__stdcall* DATEFMT_ENUMPROCA)(LPSTR);
typedef BOOL (__stdcall* DATEFMT_ENUMPROCEXA)(LPSTR, CALID);
typedef BOOL (__stdcall* TIMEFMT_ENUMPROCA)(LPSTR);
typedef BOOL (__stdcall* CALINFO_ENUMPROCA)(LPSTR);
typedef BOOL (__stdcall* CALINFO_ENUMPROCEXA)(LPSTR, CALID);

typedef BOOL (__stdcall* LANGUAGEGROUP_ENUMPROCW)(LGRPID, LPWSTR, LPWSTR, DWORD, LONG_PTR);
typedef BOOL (__stdcall* LANGGROUPLOCALE_ENUMPROCW)(LGRPID, LCID, LPWSTR, LONG_PTR);
typedef BOOL (__stdcall* UILANGUAGE_ENUMPROCW)(LPWSTR, LONG_PTR);
typedef BOOL (__stdcall* LOCALE_ENUMPROCW)(LPWSTR);
typedef BOOL (__stdcall* CODEPAGE_ENUMPROCW)(LPWSTR);
typedef BOOL (__stdcall* DATEFMT_ENUMPROCW)(LPWSTR);
typedef BOOL (__stdcall* DATEFMT_ENUMPROCEXW)(LPWSTR, CALID);
typedef BOOL (__stdcall* TIMEFMT_ENUMPROCW)(LPWSTR);
typedef BOOL (__stdcall* CALINFO_ENUMPROCW)(LPWSTR);
typedef BOOL (__stdcall* CALINFO_ENUMPROCEXW)(LPWSTR, CALID);
typedef BOOL (__stdcall* GEO_ENUMPROC)(GEOID);


























#line 916 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"



























#line 944 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"



























__declspec(dllimport)
BOOL
__stdcall
IsValidCodePage(
     UINT  CodePage);

__declspec(dllimport)
UINT
__stdcall
GetACP(void);

__declspec(dllimport)
UINT
__stdcall
GetOEMCP(void);

__declspec(dllimport)
BOOL
__stdcall
GetCPInfo(
     UINT       CodePage,
     LPCPINFO  lpCPInfo);

__declspec(dllimport)
BOOL
__stdcall
GetCPInfoExA(
     UINT          CodePage,
     DWORD         dwFlags,
     LPCPINFOEXA  lpCPInfoEx);
__declspec(dllimport)
BOOL
__stdcall
GetCPInfoExW(
     UINT          CodePage,
     DWORD         dwFlags,
     LPCPINFOEXW  lpCPInfoEx);




#line 1013 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
IsDBCSLeadByte(
     BYTE  TestChar);

__declspec(dllimport)
BOOL
__stdcall
IsDBCSLeadByteEx(
     UINT  CodePage,
     BYTE  TestChar);

__declspec(dllimport)
int
__stdcall
MultiByteToWideChar(
     UINT     CodePage,
     DWORD    dwFlags,
     LPCSTR   lpMultiByteStr,
     int      cbMultiByte,
     LPWSTR  lpWideCharStr,
     int      cchWideChar);

__declspec(dllimport)
int
__stdcall
WideCharToMultiByte(
     UINT     CodePage,
     DWORD    dwFlags,
     LPCWSTR  lpWideCharStr,
     int      cchWideChar,
     LPSTR   lpMultiByteStr,
     int      cbMultiByte,
     LPCSTR   lpDefaultChar,
     LPBOOL  lpUsedDefaultChar);






__declspec(dllimport)
int
__stdcall
CompareStringA(
     LCID     Locale,
     DWORD    dwCmpFlags,
     LPCSTR  lpString1,
     int      cchCount1,
     LPCSTR  lpString2,
     int      cchCount2);
__declspec(dllimport)
int
__stdcall
CompareStringW(
     LCID     Locale,
     DWORD    dwCmpFlags,
     LPCWSTR  lpString1,
     int      cchCount1,
     LPCWSTR  lpString2,
     int      cchCount2);




#line 1081 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
int
__stdcall
LCMapStringA(
     LCID     Locale,
     DWORD    dwMapFlags,
     LPCSTR  lpSrcStr,
     int      cchSrc,
     LPSTR  lpDestStr,
     int      cchDest);
__declspec(dllimport)
int
__stdcall
LCMapStringW(
     LCID     Locale,
     DWORD    dwMapFlags,
     LPCWSTR  lpSrcStr,
     int      cchSrc,
     LPWSTR  lpDestStr,
     int      cchDest);




#line 1107 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
int
__stdcall
GetLocaleInfoA(
     LCID     Locale,
     LCTYPE   LCType,
     LPSTR  lpLCData,
     int      cchData);
__declspec(dllimport)
int
__stdcall
GetLocaleInfoW(
     LCID     Locale,
     LCTYPE   LCType,
     LPWSTR  lpLCData,
     int      cchData);




#line 1129 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
SetLocaleInfoA(
     LCID     Locale,
     LCTYPE   LCType,
     LPCSTR  lpLCData);
__declspec(dllimport)
BOOL
__stdcall
SetLocaleInfoW(
     LCID     Locale,
     LCTYPE   LCType,
     LPCWSTR  lpLCData);




#line 1149 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"


__declspec(dllimport)
int
__stdcall
GetCalendarInfoA(
    LCID     Locale,
    CALID    Calendar,
    CALTYPE  CalType,
    LPSTR   lpCalData,
    int      cchData,
    LPDWORD  lpValue);
__declspec(dllimport)
int
__stdcall
GetCalendarInfoW(
    LCID     Locale,
    CALID    Calendar,
    CALTYPE  CalType,
    LPWSTR   lpCalData,
    int      cchData,
    LPDWORD  lpValue);




#line 1176 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
SetCalendarInfoA(
    LCID     Locale,
    CALID    Calendar,
    CALTYPE  CalType,
    LPCSTR  lpCalData);
__declspec(dllimport)
BOOL
__stdcall
SetCalendarInfoW(
    LCID     Locale,
    CALID    Calendar,
    CALTYPE  CalType,
    LPCWSTR  lpCalData);




#line 1198 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"
#line 1199 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
int
__stdcall
GetTimeFormatA(
     LCID             Locale,
     DWORD            dwFlags,
     const SYSTEMTIME *lpTime,
     LPCSTR          lpFormat,
     LPSTR          lpTimeStr,
     int              cchTime);
__declspec(dllimport)
int
__stdcall
GetTimeFormatW(
     LCID             Locale,
     DWORD            dwFlags,
     const SYSTEMTIME *lpTime,
     LPCWSTR          lpFormat,
     LPWSTR          lpTimeStr,
     int              cchTime);




#line 1225 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
int
__stdcall
GetDateFormatA(
     LCID             Locale,
     DWORD            dwFlags,
     const SYSTEMTIME *lpDate,
     LPCSTR          lpFormat,
     LPSTR          lpDateStr,
     int              cchDate);
__declspec(dllimport)
int
__stdcall
GetDateFormatW(
     LCID             Locale,
     DWORD            dwFlags,
     const SYSTEMTIME *lpDate,
     LPCWSTR          lpFormat,
     LPWSTR          lpDateStr,
     int              cchDate);




#line 1251 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
int
__stdcall
GetNumberFormatA(
     LCID             Locale,
     DWORD            dwFlags,
     LPCSTR          lpValue,
     const NUMBERFMTA *lpFormat,
     LPSTR          lpNumberStr,
     int              cchNumber);
__declspec(dllimport)
int
__stdcall
GetNumberFormatW(
     LCID             Locale,
     DWORD            dwFlags,
     LPCWSTR          lpValue,
     const NUMBERFMTW *lpFormat,
     LPWSTR          lpNumberStr,
     int              cchNumber);




#line 1277 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
int
__stdcall
GetCurrencyFormatA(
     LCID               Locale,
     DWORD              dwFlags,
     LPCSTR            lpValue,
     const CURRENCYFMTA *lpFormat,
     LPSTR            lpCurrencyStr,
     int                cchCurrency);
__declspec(dllimport)
int
__stdcall
GetCurrencyFormatW(
     LCID               Locale,
     DWORD              dwFlags,
     LPCWSTR            lpValue,
     const CURRENCYFMTW *lpFormat,
     LPWSTR            lpCurrencyStr,
     int                cchCurrency);




#line 1303 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
EnumCalendarInfoA(
     CALINFO_ENUMPROCA lpCalInfoEnumProc,
     LCID              Locale,
     CALID             Calendar,
     CALTYPE           CalType);
__declspec(dllimport)
BOOL
__stdcall
EnumCalendarInfoW(
     CALINFO_ENUMPROCW lpCalInfoEnumProc,
     LCID              Locale,
     CALID             Calendar,
     CALTYPE           CalType);




#line 1325 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"


__declspec(dllimport)
BOOL
__stdcall
EnumCalendarInfoExA(
     CALINFO_ENUMPROCEXA lpCalInfoEnumProcEx,
     LCID                Locale,
     CALID               Calendar,
     CALTYPE             CalType);
__declspec(dllimport)
BOOL
__stdcall
EnumCalendarInfoExW(
     CALINFO_ENUMPROCEXW lpCalInfoEnumProcEx,
     LCID                Locale,
     CALID               Calendar,
     CALTYPE             CalType);




#line 1348 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"
#line 1349 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
EnumTimeFormatsA(
     TIMEFMT_ENUMPROCA lpTimeFmtEnumProc,
     LCID              Locale,
     DWORD             dwFlags);
__declspec(dllimport)
BOOL
__stdcall
EnumTimeFormatsW(
     TIMEFMT_ENUMPROCW lpTimeFmtEnumProc,
     LCID              Locale,
     DWORD             dwFlags);




#line 1369 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
EnumDateFormatsA(
     DATEFMT_ENUMPROCA lpDateFmtEnumProc,
     LCID              Locale,
     DWORD             dwFlags);
__declspec(dllimport)
BOOL
__stdcall
EnumDateFormatsW(
     DATEFMT_ENUMPROCW lpDateFmtEnumProc,
     LCID              Locale,
     DWORD             dwFlags);




#line 1389 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"


__declspec(dllimport)
BOOL
__stdcall
EnumDateFormatsExA(
     DATEFMT_ENUMPROCEXA lpDateFmtEnumProcEx,
     LCID                Locale,
     DWORD               dwFlags);
__declspec(dllimport)
BOOL
__stdcall
EnumDateFormatsExW(
     DATEFMT_ENUMPROCEXW lpDateFmtEnumProcEx,
     LCID                Locale,
     DWORD               dwFlags);




#line 1410 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"
#line 1411 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"


__declspec(dllimport)
BOOL
__stdcall
IsValidLanguageGroup(
     LGRPID  LanguageGroup,
     DWORD   dwFlags);
#line 1420 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
GetNLSVersion(
      NLS_FUNCTION     Function,
      LCID             Locale,
     LPNLSVERSIONINFO lpVersionInformation);

__declspec(dllimport)
BOOL
__stdcall
IsNLSDefinedString(
     NLS_FUNCTION     Function,
     DWORD            dwFlags,
     LPNLSVERSIONINFO lpVersionInformation,
     LPCWSTR          lpString,
     INT              cchStr);

__declspec(dllimport)
BOOL
__stdcall
IsValidLocale(
     LCID   Locale,
     DWORD  dwFlags);

__declspec(dllimport)
int
__stdcall
GetGeoInfoA(
    GEOID       Location,
    GEOTYPE     GeoType,
    LPSTR     lpGeoData,
    int         cchData,
    LANGID      LangId);
__declspec(dllimport)
int
__stdcall
GetGeoInfoW(
    GEOID       Location,
    GEOTYPE     GeoType,
    LPWSTR     lpGeoData,
    int         cchData,
    LANGID      LangId);




#line 1469 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
EnumSystemGeoID(
    GEOCLASS        GeoClass,
    GEOID           ParentGeoId,
    GEO_ENUMPROC    lpGeoEnumProc);

__declspec(dllimport)
GEOID
__stdcall
GetUserGeoID(
    GEOCLASS    GeoClass);

__declspec(dllimport)
BOOL
__stdcall
SetUserGeoID(
    GEOID       GeoId);

__declspec(dllimport)
LCID
__stdcall
ConvertDefaultLocale(
      LCID   Locale);

__declspec(dllimport)
LCID
__stdcall
GetThreadLocale(void);

__declspec(dllimport)
BOOL
__stdcall
SetThreadLocale(
     LCID  Locale
    );


__declspec(dllimport)
LANGID
__stdcall
GetSystemDefaultUILanguage(void);

__declspec(dllimport)
LANGID
__stdcall
GetUserDefaultUILanguage(void);
#line 1519 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
LANGID
__stdcall
GetSystemDefaultLangID(void);

__declspec(dllimport)
LANGID
__stdcall
GetUserDefaultLangID(void);

__declspec(dllimport)
LCID
__stdcall
GetSystemDefaultLCID(void);

__declspec(dllimport)
LCID
__stdcall
GetUserDefaultLCID(void);







__declspec(dllimport)
BOOL
__stdcall
GetStringTypeExA(
     LCID     Locale,
     DWORD    dwInfoType,
     LPCSTR  lpSrcStr,
     int      cchSrc,
     LPWORD  lpCharType);
__declspec(dllimport)
BOOL
__stdcall
GetStringTypeExW(
     LCID     Locale,
     DWORD    dwInfoType,
     LPCWSTR  lpSrcStr,
     int      cchSrc,
     LPWORD  lpCharType);




#line 1569 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"












__declspec(dllimport)
BOOL
__stdcall
GetStringTypeA(
     LCID     Locale,
     DWORD    dwInfoType,
     LPCSTR   lpSrcStr,
     int      cchSrc,
     LPWORD  lpCharType);

__declspec(dllimport)
BOOL
__stdcall
GetStringTypeW(
     DWORD    dwInfoType,
     LPCWSTR  lpSrcStr,
     int      cchSrc,
     LPWORD  lpCharType);


__declspec(dllimport)
int
__stdcall
FoldStringA(
     DWORD    dwMapFlags,
     LPCSTR  lpSrcStr,
     int      cchSrc,
     LPSTR  lpDestStr,
     int      cchDest);
__declspec(dllimport)
int
__stdcall
FoldStringW(
     DWORD    dwMapFlags,
     LPCWSTR  lpSrcStr,
     int      cchSrc,
     LPWSTR  lpDestStr,
     int      cchDest);




#line 1624 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"


__declspec(dllimport)
BOOL
__stdcall
EnumSystemLanguageGroupsA(
     LANGUAGEGROUP_ENUMPROCA lpLanguageGroupEnumProc,
     DWORD                   dwFlags,
     LONG_PTR                lParam);
__declspec(dllimport)
BOOL
__stdcall
EnumSystemLanguageGroupsW(
     LANGUAGEGROUP_ENUMPROCW lpLanguageGroupEnumProc,
     DWORD                   dwFlags,
     LONG_PTR                lParam);




#line 1645 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
EnumLanguageGroupLocalesA(
     LANGGROUPLOCALE_ENUMPROCA lpLangGroupLocaleEnumProc,
     LGRPID                    LanguageGroup,
     DWORD                     dwFlags,
     LONG_PTR                  lParam);
__declspec(dllimport)
BOOL
__stdcall
EnumLanguageGroupLocalesW(
     LANGGROUPLOCALE_ENUMPROCW lpLangGroupLocaleEnumProc,
     LGRPID                    LanguageGroup,
     DWORD                     dwFlags,
     LONG_PTR                  lParam);




#line 1667 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
EnumUILanguagesA(
     UILANGUAGE_ENUMPROCA lpUILanguageEnumProc,
     DWORD                dwFlags,
     LONG_PTR             lParam);
__declspec(dllimport)
BOOL
__stdcall
EnumUILanguagesW(
     UILANGUAGE_ENUMPROCW lpUILanguageEnumProc,
     DWORD                dwFlags,
     LONG_PTR             lParam);




#line 1687 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"
#line 1688 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
EnumSystemLocalesA(
     LOCALE_ENUMPROCA lpLocaleEnumProc,
     DWORD            dwFlags);
__declspec(dllimport)
BOOL
__stdcall
EnumSystemLocalesW(
     LOCALE_ENUMPROCW lpLocaleEnumProc,
     DWORD            dwFlags);




#line 1706 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

__declspec(dllimport)
BOOL
__stdcall
EnumSystemCodePagesA(
     CODEPAGE_ENUMPROCA lpCodePageEnumProc,
     DWORD              dwFlags);
__declspec(dllimport)
BOOL
__stdcall
EnumSystemCodePagesW(
     CODEPAGE_ENUMPROCW lpCodePageEnumProc,
     DWORD              dwFlags);




#line 1724 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"


#line 1727 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"



}
#line 1732 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"

#line 1734 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnls.h"
#line 167 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 168 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

























extern "C" {
#line 28 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

typedef struct _COORD {
    SHORT X;
    SHORT Y;
} COORD, *PCOORD;

typedef struct _SMALL_RECT {
    SHORT Left;
    SHORT Top;
    SHORT Right;
    SHORT Bottom;
} SMALL_RECT, *PSMALL_RECT;

typedef struct _KEY_EVENT_RECORD {
    BOOL bKeyDown;
    WORD wRepeatCount;
    WORD wVirtualKeyCode;
    WORD wVirtualScanCode;
    union {
        WCHAR UnicodeChar;
        CHAR   AsciiChar;
    } uChar;
    DWORD dwControlKeyState;
} KEY_EVENT_RECORD, *PKEY_EVENT_RECORD;






















typedef struct _MOUSE_EVENT_RECORD {
    COORD dwMousePosition;
    DWORD dwButtonState;
    DWORD dwControlKeyState;
    DWORD dwEventFlags;
} MOUSE_EVENT_RECORD, *PMOUSE_EVENT_RECORD;



















typedef struct _WINDOW_BUFFER_SIZE_RECORD {
    COORD dwSize;
} WINDOW_BUFFER_SIZE_RECORD, *PWINDOW_BUFFER_SIZE_RECORD;

typedef struct _MENU_EVENT_RECORD {
    UINT dwCommandId;
} MENU_EVENT_RECORD, *PMENU_EVENT_RECORD;

typedef struct _FOCUS_EVENT_RECORD {
    BOOL bSetFocus;
} FOCUS_EVENT_RECORD, *PFOCUS_EVENT_RECORD;

typedef struct _INPUT_RECORD {
    WORD EventType;
    union {
        KEY_EVENT_RECORD KeyEvent;
        MOUSE_EVENT_RECORD MouseEvent;
        WINDOW_BUFFER_SIZE_RECORD WindowBufferSizeEvent;
        MENU_EVENT_RECORD MenuEvent;
        FOCUS_EVENT_RECORD FocusEvent;
    } Event;
} INPUT_RECORD, *PINPUT_RECORD;











typedef struct _CHAR_INFO {
    union {
        WCHAR UnicodeChar;
        CHAR   AsciiChar;
    } Char;
    WORD Attributes;
} CHAR_INFO, *PCHAR_INFO;
























typedef struct _CONSOLE_SCREEN_BUFFER_INFO {
    COORD dwSize;
    COORD dwCursorPosition;
    WORD  wAttributes;
    SMALL_RECT srWindow;
    COORD dwMaximumWindowSize;
} CONSOLE_SCREEN_BUFFER_INFO, *PCONSOLE_SCREEN_BUFFER_INFO;

typedef struct _CONSOLE_CURSOR_INFO {
    DWORD  dwSize;
    BOOL   bVisible;
} CONSOLE_CURSOR_INFO, *PCONSOLE_CURSOR_INFO;

typedef struct _CONSOLE_FONT_INFO {
    DWORD  nFont;
    COORD  dwFontSize;
} CONSOLE_FONT_INFO, *PCONSOLE_FONT_INFO;

















#line 198 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"





typedef
BOOL
(__stdcall *PHANDLER_ROUTINE)(
    DWORD CtrlType
    );






























__declspec(dllimport)
BOOL
__stdcall
PeekConsoleInputA(
     HANDLE hConsoleInput,
     PINPUT_RECORD lpBuffer,
     DWORD nLength,
     LPDWORD lpNumberOfEventsRead
    );
__declspec(dllimport)
BOOL
__stdcall
PeekConsoleInputW(
     HANDLE hConsoleInput,
     PINPUT_RECORD lpBuffer,
     DWORD nLength,
     LPDWORD lpNumberOfEventsRead
    );




#line 261 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
ReadConsoleInputA(
     HANDLE hConsoleInput,
     PINPUT_RECORD lpBuffer,
     DWORD nLength,
     LPDWORD lpNumberOfEventsRead
    );
__declspec(dllimport)
BOOL
__stdcall
ReadConsoleInputW(
     HANDLE hConsoleInput,
     PINPUT_RECORD lpBuffer,
     DWORD nLength,
     LPDWORD lpNumberOfEventsRead
    );




#line 285 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
WriteConsoleInputA(
     HANDLE hConsoleInput,
     const INPUT_RECORD *lpBuffer,
     DWORD nLength,
     LPDWORD lpNumberOfEventsWritten
    );
__declspec(dllimport)
BOOL
__stdcall
WriteConsoleInputW(
     HANDLE hConsoleInput,
     const INPUT_RECORD *lpBuffer,
     DWORD nLength,
     LPDWORD lpNumberOfEventsWritten
    );




#line 309 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
ReadConsoleOutputA(
     HANDLE hConsoleOutput,
     PCHAR_INFO lpBuffer,
     COORD dwBufferSize,
     COORD dwBufferCoord,
      PSMALL_RECT lpReadRegion
    );
__declspec(dllimport)
BOOL
__stdcall
ReadConsoleOutputW(
     HANDLE hConsoleOutput,
     PCHAR_INFO lpBuffer,
     COORD dwBufferSize,
     COORD dwBufferCoord,
      PSMALL_RECT lpReadRegion
    );




#line 335 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
WriteConsoleOutputA(
     HANDLE hConsoleOutput,
     const CHAR_INFO *lpBuffer,
     COORD dwBufferSize,
     COORD dwBufferCoord,
      PSMALL_RECT lpWriteRegion
    );
__declspec(dllimport)
BOOL
__stdcall
WriteConsoleOutputW(
     HANDLE hConsoleOutput,
     const CHAR_INFO *lpBuffer,
     COORD dwBufferSize,
     COORD dwBufferCoord,
      PSMALL_RECT lpWriteRegion
    );




#line 361 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
ReadConsoleOutputCharacterA(
     HANDLE hConsoleOutput,
     LPSTR lpCharacter,
      DWORD nLength,
     COORD dwReadCoord,
     LPDWORD lpNumberOfCharsRead
    );
__declspec(dllimport)
BOOL
__stdcall
ReadConsoleOutputCharacterW(
     HANDLE hConsoleOutput,
     LPWSTR lpCharacter,
      DWORD nLength,
     COORD dwReadCoord,
     LPDWORD lpNumberOfCharsRead
    );




#line 387 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
ReadConsoleOutputAttribute(
     HANDLE hConsoleOutput,
     LPWORD lpAttribute,
     DWORD nLength,
     COORD dwReadCoord,
     LPDWORD lpNumberOfAttrsRead
    );

__declspec(dllimport)
BOOL
__stdcall
WriteConsoleOutputCharacterA(
     HANDLE hConsoleOutput,
     LPCSTR lpCharacter,
     DWORD nLength,
     COORD dwWriteCoord,
     LPDWORD lpNumberOfCharsWritten
    );
__declspec(dllimport)
BOOL
__stdcall
WriteConsoleOutputCharacterW(
     HANDLE hConsoleOutput,
     LPCWSTR lpCharacter,
     DWORD nLength,
     COORD dwWriteCoord,
     LPDWORD lpNumberOfCharsWritten
    );




#line 424 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
WriteConsoleOutputAttribute(
     HANDLE hConsoleOutput,
     const WORD *lpAttribute,
     DWORD nLength,
     COORD dwWriteCoord,
     LPDWORD lpNumberOfAttrsWritten
    );

__declspec(dllimport)
BOOL
__stdcall
FillConsoleOutputCharacterA(
     HANDLE hConsoleOutput,
     CHAR  cCharacter,
     DWORD  nLength,
     COORD  dwWriteCoord,
     LPDWORD lpNumberOfCharsWritten
    );
__declspec(dllimport)
BOOL
__stdcall
FillConsoleOutputCharacterW(
     HANDLE hConsoleOutput,
     WCHAR  cCharacter,
     DWORD  nLength,
     COORD  dwWriteCoord,
     LPDWORD lpNumberOfCharsWritten
    );




#line 461 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
FillConsoleOutputAttribute(
     HANDLE hConsoleOutput,
     WORD   wAttribute,
     DWORD  nLength,
     COORD  dwWriteCoord,
     LPDWORD lpNumberOfAttrsWritten
    );

__declspec(dllimport)
BOOL
__stdcall
GetConsoleMode(
     HANDLE hConsoleHandle,
     LPDWORD lpMode
    );

__declspec(dllimport)
BOOL
__stdcall
GetNumberOfConsoleInputEvents(
     HANDLE hConsoleInput,
     LPDWORD lpNumberOfEvents
    );

__declspec(dllimport)
BOOL
__stdcall
GetConsoleScreenBufferInfo(
     HANDLE hConsoleOutput,
     PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo
    );

__declspec(dllimport)
COORD
__stdcall
GetLargestConsoleWindowSize(
     HANDLE hConsoleOutput
    );

__declspec(dllimport)
BOOL
__stdcall
GetConsoleCursorInfo(
     HANDLE hConsoleOutput,
     PCONSOLE_CURSOR_INFO lpConsoleCursorInfo
    );



























#line 539 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
GetNumberOfConsoleMouseButtons(
     LPDWORD lpNumberOfMouseButtons
    );

__declspec(dllimport)
BOOL
__stdcall
SetConsoleMode(
     HANDLE hConsoleHandle,
     DWORD dwMode
    );

__declspec(dllimport)
BOOL
__stdcall
SetConsoleActiveScreenBuffer(
     HANDLE hConsoleOutput
    );

__declspec(dllimport)
BOOL
__stdcall
FlushConsoleInputBuffer(
     HANDLE hConsoleInput
    );

__declspec(dllimport)
BOOL
__stdcall
SetConsoleScreenBufferSize(
     HANDLE hConsoleOutput,
     COORD dwSize
    );

__declspec(dllimport)
BOOL
__stdcall
SetConsoleCursorPosition(
     HANDLE hConsoleOutput,
     COORD dwCursorPosition
    );

__declspec(dllimport)
BOOL
__stdcall
SetConsoleCursorInfo(
     HANDLE hConsoleOutput,
     const CONSOLE_CURSOR_INFO *lpConsoleCursorInfo
    );

__declspec(dllimport)
BOOL
__stdcall
ScrollConsoleScreenBufferA(
     HANDLE hConsoleOutput,
     const SMALL_RECT *lpScrollRectangle,
     const SMALL_RECT *lpClipRectangle,
     COORD dwDestinationOrigin,
     const CHAR_INFO *lpFill
    );
__declspec(dllimport)
BOOL
__stdcall
ScrollConsoleScreenBufferW(
     HANDLE hConsoleOutput,
     const SMALL_RECT *lpScrollRectangle,
     const SMALL_RECT *lpClipRectangle,
     COORD dwDestinationOrigin,
     const CHAR_INFO *lpFill
    );




#line 618 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
SetConsoleWindowInfo(
     HANDLE hConsoleOutput,
     BOOL bAbsolute,
     const SMALL_RECT *lpConsoleWindow
    );

__declspec(dllimport)
BOOL
__stdcall
SetConsoleTextAttribute(
     HANDLE hConsoleOutput,
     WORD wAttributes
    );

__declspec(dllimport)
BOOL
__stdcall
SetConsoleCtrlHandler(
     PHANDLER_ROUTINE HandlerRoutine,
     BOOL Add
    );

__declspec(dllimport)
BOOL
__stdcall
GenerateConsoleCtrlEvent(
     DWORD dwCtrlEvent,
     DWORD dwProcessGroupId
    );

__declspec(dllimport)
BOOL
__stdcall
AllocConsole( void );

__declspec(dllimport)
BOOL
__stdcall
FreeConsole( void );










#line 672 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
DWORD
__stdcall
GetConsoleTitleA(
     LPSTR lpConsoleTitle,
     DWORD nSize
    );
__declspec(dllimport)
DWORD
__stdcall
GetConsoleTitleW(
     LPWSTR lpConsoleTitle,
     DWORD nSize
    );




#line 692 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
SetConsoleTitleA(
     LPCSTR lpConsoleTitle
    );
__declspec(dllimport)
BOOL
__stdcall
SetConsoleTitleW(
     LPCWSTR lpConsoleTitle
    );




#line 710 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
ReadConsoleA(
     HANDLE hConsoleInput,
     LPVOID lpBuffer,
     DWORD nNumberOfCharsToRead,
     LPDWORD lpNumberOfCharsRead,
     LPVOID lpReserved
    );
__declspec(dllimport)
BOOL
__stdcall
ReadConsoleW(
     HANDLE hConsoleInput,
     LPVOID lpBuffer,
     DWORD nNumberOfCharsToRead,
     LPDWORD lpNumberOfCharsRead,
     LPVOID lpReserved
    );




#line 736 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

__declspec(dllimport)
BOOL
__stdcall
WriteConsoleA(
     HANDLE hConsoleOutput,
     const void *lpBuffer,
     DWORD nNumberOfCharsToWrite,
     LPDWORD lpNumberOfCharsWritten,
     LPVOID lpReserved
    );
__declspec(dllimport)
BOOL
__stdcall
WriteConsoleW(
     HANDLE hConsoleOutput,
     const void *lpBuffer,
     DWORD nNumberOfCharsToWrite,
     LPDWORD lpNumberOfCharsWritten,
     LPVOID lpReserved
    );




#line 762 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"



__declspec(dllimport)
HANDLE
__stdcall
CreateConsoleScreenBuffer(
     DWORD dwDesiredAccess,
     DWORD dwShareMode,
     const SECURITY_ATTRIBUTES *lpSecurityAttributes,
     DWORD dwFlags,
     LPVOID lpScreenBufferData
    );

__declspec(dllimport)
UINT
__stdcall
GetConsoleCP( void );

__declspec(dllimport)
BOOL
__stdcall
SetConsoleCP(
     UINT wCodePageID
    );

__declspec(dllimport)
UINT
__stdcall
GetConsoleOutputCP( void );

__declspec(dllimport)
BOOL
__stdcall
SetConsoleOutputCP(
     UINT wCodePageID
    );




















#line 820 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"






























































































































#line 947 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"


}
#line 951 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

#line 953 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\wincon.h"

#line 170 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"
















extern "C" {
#line 19 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"











#line 31 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"





































































































typedef struct tagVS_FIXEDFILEINFO
{
    DWORD   dwSignature;            
    DWORD   dwStrucVersion;         
    DWORD   dwFileVersionMS;        
    DWORD   dwFileVersionLS;        
    DWORD   dwProductVersionMS;     
    DWORD   dwProductVersionLS;     
    DWORD   dwFileFlagsMask;        
    DWORD   dwFileFlags;            
    DWORD   dwFileOS;               
    DWORD   dwFileType;             
    DWORD   dwFileSubtype;          
    DWORD   dwFileDateMS;           
    DWORD   dwFileDateLS;           
} VS_FIXEDFILEINFO;



DWORD
__stdcall
VerFindFileA(
        DWORD uFlags,
        LPSTR szFileName,
        LPSTR szWinDir,
        LPSTR szAppDir,
        LPSTR szCurDir,
        PUINT lpuCurDirLen,
        LPSTR szDestDir,
        PUINT lpuDestDirLen
        );
DWORD
__stdcall
VerFindFileW(
        DWORD uFlags,
        LPWSTR szFileName,
        LPWSTR szWinDir,
        LPWSTR szAppDir,
        LPWSTR szCurDir,
        PUINT lpuCurDirLen,
        LPWSTR szDestDir,
        PUINT lpuDestDirLen
        );




#line 180 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"

DWORD
__stdcall
VerInstallFileA(
        DWORD uFlags,
        LPSTR szSrcFileName,
        LPSTR szDestFileName,
        LPSTR szSrcDir,
        LPSTR szDestDir,
        LPSTR szCurDir,
        LPSTR szTmpFile,
        PUINT lpuTmpFileLen
        );
DWORD
__stdcall
VerInstallFileW(
        DWORD uFlags,
        LPWSTR szSrcFileName,
        LPWSTR szDestFileName,
        LPWSTR szSrcDir,
        LPWSTR szDestDir,
        LPWSTR szCurDir,
        LPWSTR szTmpFile,
        PUINT lpuTmpFileLen
        );




#line 210 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"


DWORD
__stdcall
GetFileVersionInfoSizeA(
        LPCSTR lptstrFilename, 
        LPDWORD lpdwHandle
        );                      

DWORD
__stdcall
GetFileVersionInfoSizeW(
        LPCWSTR lptstrFilename, 
        LPDWORD lpdwHandle
        );                      




#line 230 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"


BOOL
__stdcall
GetFileVersionInfoA(
        LPCSTR lptstrFilename, 
        DWORD dwHandle,         
        DWORD dwLen,            
        LPVOID lpData
        );                      

BOOL
__stdcall
GetFileVersionInfoW(
        LPCWSTR lptstrFilename, 
        DWORD dwHandle,         
        DWORD dwLen,            
        LPVOID lpData
        );                      




#line 254 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"

DWORD
__stdcall
VerLanguageNameA(
        DWORD wLang,
        LPSTR szLang,
        DWORD nSize
        );
DWORD
__stdcall
VerLanguageNameW(
        DWORD wLang,
        LPWSTR szLang,
        DWORD nSize
        );




#line 274 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"

BOOL
__stdcall
VerQueryValueA(
        const LPVOID pBlock,
        LPSTR lpSubBlock,
        LPVOID * lplpBuffer,
        PUINT puLen
        );
BOOL
__stdcall
VerQueryValueW(
        const LPVOID pBlock,
        LPWSTR lpSubBlock,
        LPVOID * lplpBuffer,
        PUINT puLen
        );




#line 296 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"

#line 298 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"


}
#line 302 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"

#line 304 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winver.h"

#line 171 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 172 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"
























extern "C" {
#line 27 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"









typedef ACCESS_MASK REGSAM;





















struct val_context {
    int valuelen;       
    LPVOID value_context;   
    LPVOID val_buff_ptr;    
};

typedef struct val_context  *PVALCONTEXT;

typedef struct pvalueA {           
    LPSTR   pv_valuename;          
    int pv_valuelen;
    LPVOID pv_value_context;
    DWORD pv_type;
}PVALUEA,  *PPVALUEA;
typedef struct pvalueW {           
    LPWSTR  pv_valuename;          
    int pv_valuelen;
    LPVOID pv_value_context;
    DWORD pv_type;
}PVALUEW,  *PPVALUEW;




typedef PVALUEA PVALUE;
typedef PPVALUEA PPVALUE;
#line 85 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

typedef
DWORD _cdecl
QUERYHANDLER (LPVOID keycontext, PVALCONTEXT val_list, DWORD num_vals,
          LPVOID outputbuffer, DWORD  *total_outlen, DWORD input_blen);

typedef QUERYHANDLER  *PQUERYHANDLER;

typedef struct provider_info {
    PQUERYHANDLER pi_R0_1val;
    PQUERYHANDLER pi_R0_allvals;
    PQUERYHANDLER pi_R3_1val;
    PQUERYHANDLER pi_R3_allvals;
    DWORD pi_flags;    
    LPVOID pi_key_context;
}REG_PROVIDER;

typedef struct provider_info  *PPROVIDER;

typedef struct value_entA {
    LPSTR   ve_valuename;
    DWORD ve_valuelen;
    DWORD_PTR ve_valueptr;
    DWORD ve_type;
}VALENTA,  *PVALENTA;
typedef struct value_entW {
    LPWSTR  ve_valuename;
    DWORD ve_valuelen;
    DWORD_PTR ve_valueptr;
    DWORD ve_type;
}VALENTW,  *PVALENTW;




typedef VALENTA VALENT;
typedef PVALENTA PVALENT;
#line 123 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

#line 125 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"


#line 128 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"













__declspec(dllimport)
LONG
__stdcall
RegCloseKey (
     HKEY hKey
    );

__declspec(dllimport)
LONG
__stdcall
RegOverridePredefKey (
     HKEY hKey,
     HKEY hNewHKey
    );

__declspec(dllimport)
LONG
__stdcall
RegOpenUserClassesRoot(
    HANDLE hToken,
    DWORD  dwOptions,
    REGSAM samDesired,
    PHKEY  phkResult
    );

__declspec(dllimport)
LONG
__stdcall
RegOpenCurrentUser(
    REGSAM samDesired,
    PHKEY phkResult
    );

__declspec(dllimport)
LONG
__stdcall
RegDisablePredefinedCache(
    );

__declspec(dllimport)
LONG
__stdcall
RegConnectRegistryA (
     LPCSTR lpMachineName,
     HKEY hKey,
     PHKEY phkResult
    );
__declspec(dllimport)
LONG
__stdcall
RegConnectRegistryW (
     LPCWSTR lpMachineName,
     HKEY hKey,
     PHKEY phkResult
    );




#line 201 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegCreateKeyA (
     HKEY hKey,
     LPCSTR lpSubKey,
     PHKEY phkResult
    );
__declspec(dllimport)
LONG
__stdcall
RegCreateKeyW (
     HKEY hKey,
     LPCWSTR lpSubKey,
     PHKEY phkResult
    );




#line 223 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegCreateKeyExA (
     HKEY hKey,
     LPCSTR lpSubKey,
     DWORD Reserved,
     LPSTR lpClass,
     DWORD dwOptions,
     REGSAM samDesired,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes,
     PHKEY phkResult,
     LPDWORD lpdwDisposition
    );
__declspec(dllimport)
LONG
__stdcall
RegCreateKeyExW (
     HKEY hKey,
     LPCWSTR lpSubKey,
     DWORD Reserved,
     LPWSTR lpClass,
     DWORD dwOptions,
     REGSAM samDesired,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes,
     PHKEY phkResult,
     LPDWORD lpdwDisposition
    );




#line 257 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegDeleteKeyA (
     HKEY hKey,
     LPCSTR lpSubKey
    );
__declspec(dllimport)
LONG
__stdcall
RegDeleteKeyW (
     HKEY hKey,
     LPCWSTR lpSubKey
    );




#line 277 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegDeleteValueA (
     HKEY hKey,
     LPCSTR lpValueName
    );
__declspec(dllimport)
LONG
__stdcall
RegDeleteValueW (
     HKEY hKey,
     LPCWSTR lpValueName
    );




#line 297 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegEnumKeyA (
     HKEY hKey,
     DWORD dwIndex,
     LPSTR lpName,
     DWORD cbName
    );
__declspec(dllimport)
LONG
__stdcall
RegEnumKeyW (
     HKEY hKey,
     DWORD dwIndex,
     LPWSTR lpName,
     DWORD cbName
    );




#line 321 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegEnumKeyExA (
     HKEY hKey,
     DWORD dwIndex,
     LPSTR lpName,
      LPDWORD lpcbName,
     LPDWORD lpReserved,
      LPSTR lpClass,
      LPDWORD lpcbClass,
     PFILETIME lpftLastWriteTime
    );
__declspec(dllimport)
LONG
__stdcall
RegEnumKeyExW (
     HKEY hKey,
     DWORD dwIndex,
     LPWSTR lpName,
      LPDWORD lpcbName,
     LPDWORD lpReserved,
      LPWSTR lpClass,
      LPDWORD lpcbClass,
     PFILETIME lpftLastWriteTime
    );




#line 353 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegEnumValueA (
     HKEY hKey,
     DWORD dwIndex,
     LPSTR lpValueName,
      LPDWORD lpcbValueName,
     LPDWORD lpReserved,
     LPDWORD lpType,
     LPBYTE lpData,
      LPDWORD lpcbData
    );
__declspec(dllimport)
LONG
__stdcall
RegEnumValueW (
     HKEY hKey,
     DWORD dwIndex,
     LPWSTR lpValueName,
      LPDWORD lpcbValueName,
     LPDWORD lpReserved,
     LPDWORD lpType,
     LPBYTE lpData,
      LPDWORD lpcbData
    );




#line 385 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegFlushKey (
     HKEY hKey
    );

__declspec(dllimport)
LONG
__stdcall
RegGetKeySecurity (
     HKEY hKey,
     SECURITY_INFORMATION SecurityInformation,
     PSECURITY_DESCRIPTOR pSecurityDescriptor,
      LPDWORD lpcbSecurityDescriptor
    );

__declspec(dllimport)
LONG
__stdcall
RegLoadKeyA (
     HKEY    hKey,
     LPCSTR  lpSubKey,
     LPCSTR  lpFile
    );
__declspec(dllimport)
LONG
__stdcall
RegLoadKeyW (
     HKEY    hKey,
     LPCWSTR  lpSubKey,
     LPCWSTR  lpFile
    );




#line 424 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegNotifyChangeKeyValue (
     HKEY hKey,
     BOOL bWatchSubtree,
     DWORD dwNotifyFilter,
     HANDLE hEvent,
     BOOL fAsynchronus
    );

__declspec(dllimport)
LONG
__stdcall
RegOpenKeyA (
     HKEY hKey,
     LPCSTR lpSubKey,
     PHKEY phkResult
    );
__declspec(dllimport)
LONG
__stdcall
RegOpenKeyW (
     HKEY hKey,
     LPCWSTR lpSubKey,
     PHKEY phkResult
    );




#line 457 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegOpenKeyExA (
     HKEY hKey,
     LPCSTR lpSubKey,
     DWORD ulOptions,
     REGSAM samDesired,
     PHKEY phkResult
    );
__declspec(dllimport)
LONG
__stdcall
RegOpenKeyExW (
     HKEY hKey,
     LPCWSTR lpSubKey,
     DWORD ulOptions,
     REGSAM samDesired,
     PHKEY phkResult
    );




#line 483 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegQueryInfoKeyA (
     HKEY hKey,
     LPSTR lpClass,
      LPDWORD lpcbClass,
     LPDWORD lpReserved,
     LPDWORD lpcSubKeys,
     LPDWORD lpcbMaxSubKeyLen,
     LPDWORD lpcbMaxClassLen,
     LPDWORD lpcValues,
     LPDWORD lpcbMaxValueNameLen,
     LPDWORD lpcbMaxValueLen,
     LPDWORD lpcbSecurityDescriptor,
     PFILETIME lpftLastWriteTime
    );
__declspec(dllimport)
LONG
__stdcall
RegQueryInfoKeyW (
     HKEY hKey,
     LPWSTR lpClass,
      LPDWORD lpcbClass,
     LPDWORD lpReserved,
     LPDWORD lpcSubKeys,
     LPDWORD lpcbMaxSubKeyLen,
     LPDWORD lpcbMaxClassLen,
     LPDWORD lpcValues,
     LPDWORD lpcbMaxValueNameLen,
     LPDWORD lpcbMaxValueLen,
     LPDWORD lpcbSecurityDescriptor,
     PFILETIME lpftLastWriteTime
    );




#line 523 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegQueryValueA (
     HKEY hKey,
     LPCSTR lpSubKey,
     LPSTR lpValue,
      PLONG   lpcbValue
    );
__declspec(dllimport)
LONG
__stdcall
RegQueryValueW (
     HKEY hKey,
     LPCWSTR lpSubKey,
     LPWSTR lpValue,
      PLONG   lpcbValue
    );




#line 547 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"


__declspec(dllimport)
LONG
__stdcall
RegQueryMultipleValuesA (
     HKEY hKey,
     PVALENTA val_list,
     DWORD num_vals,
     LPSTR lpValueBuf,
      LPDWORD ldwTotsize
    );
__declspec(dllimport)
LONG
__stdcall
RegQueryMultipleValuesW (
     HKEY hKey,
     PVALENTW val_list,
     DWORD num_vals,
     LPWSTR lpValueBuf,
      LPDWORD ldwTotsize
    );




#line 574 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"
#line 575 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegQueryValueExA (
     HKEY hKey,
     LPCSTR lpValueName,
     LPDWORD lpReserved,
     LPDWORD lpType,
      LPBYTE lpData,
      LPDWORD lpcbData
    );
__declspec(dllimport)
LONG
__stdcall
RegQueryValueExW (
     HKEY hKey,
     LPCWSTR lpValueName,
     LPDWORD lpReserved,
     LPDWORD lpType,
      LPBYTE lpData,
      LPDWORD lpcbData
    );




#line 603 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegReplaceKeyA (
     HKEY     hKey,
     LPCSTR  lpSubKey,
     LPCSTR  lpNewFile,
     LPCSTR  lpOldFile
    );
__declspec(dllimport)
LONG
__stdcall
RegReplaceKeyW (
     HKEY     hKey,
     LPCWSTR  lpSubKey,
     LPCWSTR  lpNewFile,
     LPCWSTR  lpOldFile
    );




#line 627 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegRestoreKeyA (
     HKEY hKey,
     LPCSTR lpFile,
     DWORD   dwFlags
    );
__declspec(dllimport)
LONG
__stdcall
RegRestoreKeyW (
     HKEY hKey,
     LPCWSTR lpFile,
     DWORD   dwFlags
    );




#line 649 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegSaveKeyA (
     HKEY hKey,
     LPCSTR lpFile,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );
__declspec(dllimport)
LONG
__stdcall
RegSaveKeyW (
     HKEY hKey,
     LPCWSTR lpFile,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );




#line 671 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegSetKeySecurity (
     HKEY hKey,
     SECURITY_INFORMATION SecurityInformation,
     PSECURITY_DESCRIPTOR pSecurityDescriptor
    );

__declspec(dllimport)
LONG
__stdcall
RegSetValueA (
     HKEY hKey,
     LPCSTR lpSubKey,
     DWORD dwType,
     LPCSTR lpData,
     DWORD cbData
    );
__declspec(dllimport)
LONG
__stdcall
RegSetValueW (
     HKEY hKey,
     LPCWSTR lpSubKey,
     DWORD dwType,
     LPCWSTR lpData,
     DWORD cbData
    );




#line 706 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"


__declspec(dllimport)
LONG
__stdcall
RegSetValueExA (
     HKEY hKey,
     LPCSTR lpValueName,
     DWORD Reserved,
     DWORD dwType,
     const BYTE* lpData,
     DWORD cbData
    );
__declspec(dllimport)
LONG
__stdcall
RegSetValueExW (
     HKEY hKey,
     LPCWSTR lpValueName,
     DWORD Reserved,
     DWORD dwType,
     const BYTE* lpData,
     DWORD cbData
    );




#line 735 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
RegUnLoadKeyA (
     HKEY    hKey,
     LPCSTR lpSubKey
    );
__declspec(dllimport)
LONG
__stdcall
RegUnLoadKeyW (
     HKEY    hKey,
     LPCWSTR lpSubKey
    );




#line 755 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"





__declspec(dllimport)
BOOL
__stdcall
InitiateSystemShutdownA(
     LPSTR lpMachineName,
     LPSTR lpMessage,
     DWORD dwTimeout,
     BOOL bForceAppsClosed,
     BOOL bRebootAfterShutdown
    );
__declspec(dllimport)
BOOL
__stdcall
InitiateSystemShutdownW(
     LPWSTR lpMachineName,
     LPWSTR lpMessage,
     DWORD dwTimeout,
     BOOL bForceAppsClosed,
     BOOL bRebootAfterShutdown
    );




#line 785 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"


__declspec(dllimport)
BOOL
__stdcall
AbortSystemShutdownA(
     LPSTR lpMachineName
    );
__declspec(dllimport)
BOOL
__stdcall
AbortSystemShutdownW(
     LPWSTR lpMachineName
    );




#line 804 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"





#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\reason.h"
















































































































#line 114 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\reason.h"
#line 810 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"


















__declspec(dllimport)
BOOL
__stdcall
InitiateSystemShutdownExA(
     LPSTR lpMachineName,
     LPSTR lpMessage,
     DWORD dwTimeout,
     BOOL bForceAppsClosed,
     BOOL bRebootAfterShutdown,
     DWORD dwReason
    );
__declspec(dllimport)
BOOL
__stdcall
InitiateSystemShutdownExW(
     LPWSTR lpMachineName,
     LPWSTR lpMessage,
     DWORD dwTimeout,
     BOOL bForceAppsClosed,
     BOOL bRebootAfterShutdown,
     DWORD dwReason
    );




#line 855 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"


__declspec(dllimport)
LONG
__stdcall
RegSaveKeyExA (
     HKEY hKey,
     LPCSTR lpFile,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes,
     DWORD Flags
    );
__declspec(dllimport)
LONG
__stdcall
RegSaveKeyExW (
     HKEY hKey,
     LPCWSTR lpFile,
     LPSECURITY_ATTRIBUTES lpSecurityAttributes,
     DWORD Flags
    );




#line 880 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

__declspec(dllimport)
LONG
__stdcall
Wow64Win32ApiEntry (
    DWORD dwFuncNumber,
    DWORD dwFlag,
    DWORD dwRes
    );


}
#line 893 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"


#line 896 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winreg.h"

#line 174 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 175 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"



























extern "C" {
#line 30 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"









































































#line 104 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"






#line 111 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"









#line 121 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"













#line 135 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"



#line 139 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

typedef struct  _NETRESOURCEA {
    DWORD    dwScope;
    DWORD    dwType;
    DWORD    dwDisplayType;
    DWORD    dwUsage;
    LPSTR    lpLocalName;
    LPSTR    lpRemoteName;
    LPSTR    lpComment ;
    LPSTR    lpProvider;
}NETRESOURCEA, *LPNETRESOURCEA;
typedef struct  _NETRESOURCEW {
    DWORD    dwScope;
    DWORD    dwType;
    DWORD    dwDisplayType;
    DWORD    dwUsage;
    LPWSTR   lpLocalName;
    LPWSTR   lpRemoteName;
    LPWSTR   lpComment ;
    LPWSTR   lpProvider;
}NETRESOURCEW, *LPNETRESOURCEW;




typedef NETRESOURCEA NETRESOURCE;
typedef LPNETRESOURCEA LPNETRESOURCE;
#line 167 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"





















#line 189 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"



#line 193 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetAddConnectionA(
      LPCSTR   lpRemoteName,
      LPCSTR   lpPassword,
      LPCSTR   lpLocalName
    );
DWORD __stdcall
WNetAddConnectionW(
      LPCWSTR   lpRemoteName,
      LPCWSTR   lpPassword,
      LPCWSTR   lpLocalName
    );




#line 211 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetAddConnection2A(
      LPNETRESOURCEA lpNetResource,
      LPCSTR       lpPassword,
      LPCSTR       lpUserName,
      DWORD          dwFlags
    );
DWORD __stdcall
WNetAddConnection2W(
      LPNETRESOURCEW lpNetResource,
      LPCWSTR       lpPassword,
      LPCWSTR       lpUserName,
      DWORD          dwFlags
    );




#line 231 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetAddConnection3A(
      HWND           hwndOwner,
      LPNETRESOURCEA lpNetResource,
      LPCSTR       lpPassword,
      LPCSTR       lpUserName,
      DWORD          dwFlags
    );
DWORD __stdcall
WNetAddConnection3W(
      HWND           hwndOwner,
      LPNETRESOURCEW lpNetResource,
      LPCWSTR       lpPassword,
      LPCWSTR       lpUserName,
      DWORD          dwFlags
    );




#line 253 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetCancelConnectionA(
      LPCSTR lpName,
      BOOL     fForce
    );
DWORD __stdcall
WNetCancelConnectionW(
      LPCWSTR lpName,
      BOOL     fForce
    );




#line 269 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetCancelConnection2A(
      LPCSTR lpName,
      DWORD    dwFlags,
      BOOL     fForce
    );
DWORD __stdcall
WNetCancelConnection2W(
      LPCWSTR lpName,
      DWORD    dwFlags,
      BOOL     fForce
    );




#line 287 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetGetConnectionA(
      LPCSTR lpLocalName,
      LPSTR  lpRemoteName,
       LPDWORD  lpnLength
    );
DWORD __stdcall
WNetGetConnectionW(
      LPCWSTR lpLocalName,
      LPWSTR  lpRemoteName,
       LPDWORD  lpnLength
    );




#line 305 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"


DWORD __stdcall
WNetRestoreConnectionA(
     HWND     hwndParent,
     LPCSTR lpDevice
    );
DWORD __stdcall
WNetRestoreConnectionW(
     HWND     hwndParent,
     LPCWSTR lpDevice
    );




#line 322 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"



DWORD __stdcall
WNetUseConnectionA(
     HWND            hwndOwner,
     LPNETRESOURCEA  lpNetResource,
     LPCSTR        lpPassword,
     LPCSTR        lpUserID,
     DWORD           dwFlags,
     LPSTR        lpAccessName,
      LPDWORD     lpBufferSize,
     LPDWORD        lpResult
    );
DWORD __stdcall
WNetUseConnectionW(
     HWND            hwndOwner,
     LPNETRESOURCEW  lpNetResource,
     LPCWSTR        lpPassword,
     LPCWSTR        lpUserID,
     DWORD           dwFlags,
     LPWSTR        lpAccessName,
      LPDWORD     lpBufferSize,
     LPDWORD        lpResult
    );




#line 352 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"
#line 353 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"






DWORD __stdcall
WNetConnectionDialog(
     HWND  hwnd,
     DWORD dwType
    );

DWORD __stdcall
WNetDisconnectDialog(
     HWND  hwnd,
     DWORD dwType
    );


typedef struct _CONNECTDLGSTRUCTA{
    DWORD cbStructure;       
    HWND hwndOwner;          
    LPNETRESOURCEA lpConnRes;
    DWORD dwFlags;           
    DWORD dwDevNum;          
} CONNECTDLGSTRUCTA,  *LPCONNECTDLGSTRUCTA;
typedef struct _CONNECTDLGSTRUCTW{
    DWORD cbStructure;       
    HWND hwndOwner;          
    LPNETRESOURCEW lpConnRes;
    DWORD dwFlags;           
    DWORD dwDevNum;          
} CONNECTDLGSTRUCTW,  *LPCONNECTDLGSTRUCTW;




typedef CONNECTDLGSTRUCTA CONNECTDLGSTRUCT;
typedef LPCONNECTDLGSTRUCTA LPCONNECTDLGSTRUCT;
#line 393 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"














DWORD __stdcall
WNetConnectionDialog1A(
      LPCONNECTDLGSTRUCTA lpConnDlgStruct
    );
DWORD __stdcall
WNetConnectionDialog1W(
      LPCONNECTDLGSTRUCTW lpConnDlgStruct
    );




#line 420 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

typedef struct _DISCDLGSTRUCTA{
    DWORD           cbStructure;      
    HWND            hwndOwner;        
    LPSTR           lpLocalName;      
    LPSTR           lpRemoteName;     
    DWORD           dwFlags;          
} DISCDLGSTRUCTA,  *LPDISCDLGSTRUCTA;
typedef struct _DISCDLGSTRUCTW{
    DWORD           cbStructure;      
    HWND            hwndOwner;        
    LPWSTR          lpLocalName;      
    LPWSTR          lpRemoteName;     
    DWORD           dwFlags;          
} DISCDLGSTRUCTW,  *LPDISCDLGSTRUCTW;




typedef DISCDLGSTRUCTA DISCDLGSTRUCT;
typedef LPDISCDLGSTRUCTA LPDISCDLGSTRUCT;
#line 442 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"




DWORD __stdcall
WNetDisconnectDialog1A(
     LPDISCDLGSTRUCTA lpConnDlgStruct
    );
DWORD __stdcall
WNetDisconnectDialog1W(
     LPDISCDLGSTRUCTW lpConnDlgStruct
    );




#line 459 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"
#line 460 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"





DWORD __stdcall
WNetOpenEnumA(
      DWORD          dwScope,
      DWORD          dwType,
      DWORD          dwUsage,
      LPNETRESOURCEA lpNetResource,
      LPHANDLE       lphEnum
    );
DWORD __stdcall
WNetOpenEnumW(
      DWORD          dwScope,
      DWORD          dwType,
      DWORD          dwUsage,
      LPNETRESOURCEW lpNetResource,
      LPHANDLE       lphEnum
    );




#line 486 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetEnumResourceA(
      HANDLE  hEnum,
       LPDWORD lpcCount,
      LPVOID  lpBuffer,
       LPDWORD lpBufferSize
    );
DWORD __stdcall
WNetEnumResourceW(
      HANDLE  hEnum,
       LPDWORD lpcCount,
      LPVOID  lpBuffer,
       LPDWORD lpBufferSize
    );




#line 506 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetCloseEnum(
     HANDLE   hEnum
    );


DWORD __stdcall
WNetGetResourceParentA(
     LPNETRESOURCEA lpNetResource,
     LPVOID lpBuffer,
      LPDWORD lpcbBuffer
    );
DWORD __stdcall
WNetGetResourceParentW(
     LPNETRESOURCEW lpNetResource,
     LPVOID lpBuffer,
      LPDWORD lpcbBuffer
    );




#line 530 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetGetResourceInformationA(
     LPNETRESOURCEA  lpNetResource,
     LPVOID          lpBuffer,
      LPDWORD         lpcbBuffer,
     LPSTR         *lplpSystem
    );
DWORD __stdcall
WNetGetResourceInformationW(
     LPNETRESOURCEW  lpNetResource,
     LPVOID          lpBuffer,
      LPDWORD         lpcbBuffer,
     LPWSTR         *lplpSystem
    );




#line 550 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"
#line 551 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"








typedef struct  _UNIVERSAL_NAME_INFOA {
    LPSTR    lpUniversalName;
}UNIVERSAL_NAME_INFOA, *LPUNIVERSAL_NAME_INFOA;
typedef struct  _UNIVERSAL_NAME_INFOW {
    LPWSTR   lpUniversalName;
}UNIVERSAL_NAME_INFOW, *LPUNIVERSAL_NAME_INFOW;




typedef UNIVERSAL_NAME_INFOA UNIVERSAL_NAME_INFO;
typedef LPUNIVERSAL_NAME_INFOA LPUNIVERSAL_NAME_INFO;
#line 572 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

typedef struct  _REMOTE_NAME_INFOA {
    LPSTR    lpUniversalName;
    LPSTR    lpConnectionName;
    LPSTR    lpRemainingPath;
}REMOTE_NAME_INFOA, *LPREMOTE_NAME_INFOA;
typedef struct  _REMOTE_NAME_INFOW {
    LPWSTR   lpUniversalName;
    LPWSTR   lpConnectionName;
    LPWSTR   lpRemainingPath;
}REMOTE_NAME_INFOW, *LPREMOTE_NAME_INFOW;




typedef REMOTE_NAME_INFOA REMOTE_NAME_INFO;
typedef LPREMOTE_NAME_INFOA LPREMOTE_NAME_INFO;
#line 590 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

DWORD __stdcall
WNetGetUniversalNameA(
      LPCSTR lpLocalPath,
      DWORD    dwInfoLevel,
      LPVOID   lpBuffer,
       LPDWORD  lpBufferSize
     );
DWORD __stdcall
WNetGetUniversalNameW(
      LPCWSTR lpLocalPath,
      DWORD    dwInfoLevel,
      LPVOID   lpBuffer,
       LPDWORD  lpBufferSize
     );




#line 610 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"





DWORD __stdcall
WNetGetUserA(
      LPCSTR  lpName,
      LPSTR   lpUserName,
       LPDWORD   lpnLength
    );
DWORD __stdcall
WNetGetUserW(
      LPCWSTR  lpName,
      LPWSTR   lpUserName,
       LPDWORD   lpnLength
    );




#line 632 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"











#line 644 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"



DWORD __stdcall
WNetGetProviderNameA(
     DWORD   dwNetType,
     LPSTR lpProviderName,
      LPDWORD lpBufferSize
    );
DWORD __stdcall
WNetGetProviderNameW(
     DWORD   dwNetType,
     LPWSTR lpProviderName,
      LPDWORD lpBufferSize
    );




#line 664 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

typedef struct _NETINFOSTRUCT{
    DWORD cbStructure;
    DWORD dwProviderVersion;
    DWORD dwStatus;
    DWORD dwCharacteristics;
    ULONG_PTR dwHandle;
    WORD  wNetType;
    DWORD dwPrinters;
    DWORD dwDrives;
} NETINFOSTRUCT,  *LPNETINFOSTRUCT;





DWORD __stdcall
WNetGetNetworkInformationA(
     LPCSTR          lpProvider,
     LPNETINFOSTRUCT   lpNetInfoStruct
    );
DWORD __stdcall
WNetGetNetworkInformationW(
     LPCWSTR          lpProvider,
     LPNETINFOSTRUCT   lpNetInfoStruct
    );




#line 695 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"





typedef UINT ( __stdcall *PFNGETPROFILEPATHA) (
    LPCSTR    pszUsername,
    LPSTR     pszBuffer,
    UINT        cbBuffer
    );
typedef UINT ( __stdcall *PFNGETPROFILEPATHW) (
    LPCWSTR    pszUsername,
    LPWSTR     pszBuffer,
    UINT        cbBuffer
    );




#line 715 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

typedef UINT ( __stdcall *PFNRECONCILEPROFILEA) (
    LPCSTR    pszCentralFile,
    LPCSTR    pszLocalFile,
    DWORD       dwFlags
    );
typedef UINT ( __stdcall *PFNRECONCILEPROFILEW) (
    LPCWSTR    pszCentralFile,
    LPCWSTR    pszLocalFile,
    DWORD       dwFlags
    );




#line 731 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"









typedef BOOL ( __stdcall *PFNPROCESSPOLICIESA) (
    HWND        hwnd,
    LPCSTR    pszPath,
    LPCSTR    pszUsername,
    LPCSTR    pszComputerName,
    DWORD       dwFlags
    );
typedef BOOL ( __stdcall *PFNPROCESSPOLICIESW) (
    HWND        hwnd,
    LPCWSTR    pszPath,
    LPCWSTR    pszUsername,
    LPCWSTR    pszComputerName,
    DWORD       dwFlags
    );




#line 759 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"


#line 762 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"





DWORD __stdcall
WNetGetLastErrorA(
      LPDWORD    lpError,
      LPSTR    lpErrorBuf,
      DWORD      nErrorBufSize,
      LPSTR    lpNameBuf,
      DWORD      nNameBufSize
    );
DWORD __stdcall
WNetGetLastErrorW(
      LPDWORD    lpError,
      LPWSTR    lpErrorBuf,
      DWORD      nErrorBufSize,
      LPWSTR    lpNameBuf,
      DWORD      nNameBufSize
    );




#line 788 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"





























#line 818 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"




















#line 839 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"












#line 852 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"






typedef struct _NETCONNECTINFOSTRUCT{
    DWORD cbStructure;
    DWORD dwFlags;
    DWORD dwSpeed;
    DWORD dwDelay;
    DWORD dwOptDataSize;
} NETCONNECTINFOSTRUCT,  *LPNETCONNECTINFOSTRUCT;






DWORD __stdcall
MultinetGetConnectionPerformanceA(
         LPNETRESOURCEA lpNetResource,
         LPNETCONNECTINFOSTRUCT lpNetConnectInfoStruct
        );
DWORD __stdcall
MultinetGetConnectionPerformanceW(
         LPNETRESOURCEW lpNetResource,
         LPNETCONNECTINFOSTRUCT lpNetConnectInfoStruct
        );




#line 886 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"
#line 887 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"


}
#line 891 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

#line 893 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winnetwk.h"

#line 177 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 178 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"




































#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"














































extern "C" {
#line 49 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"














































































































































































































































































































#line 352 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"






































































#line 423 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"




































#line 460 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"











































#line 504 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"



#line 508 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"











#line 520 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"









#line 530 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"


}
#line 534 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"


#line 537 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\stralign.h"

    

                                         



                                                      
#line 215 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"












#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"






























#line 32 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"


extern "C" {
#line 36 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"




































#line 73 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"


















































































































typedef struct _SERVICE_DESCRIPTIONA {
    LPSTR       lpDescription;
} SERVICE_DESCRIPTIONA, *LPSERVICE_DESCRIPTIONA;



typedef struct _SERVICE_DESCRIPTIONW {
    LPWSTR      lpDescription;
} SERVICE_DESCRIPTIONW, *LPSERVICE_DESCRIPTIONW;




typedef SERVICE_DESCRIPTIONA SERVICE_DESCRIPTION;
typedef LPSERVICE_DESCRIPTIONA LPSERVICE_DESCRIPTION;
#line 203 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"




typedef enum _SC_ACTION_TYPE {
        SC_ACTION_NONE          = 0,
        SC_ACTION_RESTART       = 1,
        SC_ACTION_REBOOT        = 2,
        SC_ACTION_RUN_COMMAND   = 3
} SC_ACTION_TYPE;

typedef struct _SC_ACTION {
    SC_ACTION_TYPE  Type;
    DWORD           Delay;
} SC_ACTION, *LPSC_ACTION;

typedef struct _SERVICE_FAILURE_ACTIONSA {
    DWORD       dwResetPeriod;
    LPSTR       lpRebootMsg;
    LPSTR       lpCommand;
    DWORD       cActions;



    SC_ACTION * lpsaActions;
} SERVICE_FAILURE_ACTIONSA, *LPSERVICE_FAILURE_ACTIONSA;
typedef struct _SERVICE_FAILURE_ACTIONSW {
    DWORD       dwResetPeriod;
    LPWSTR      lpRebootMsg;
    LPWSTR      lpCommand;
    DWORD       cActions;



    SC_ACTION * lpsaActions;
} SERVICE_FAILURE_ACTIONSW, *LPSERVICE_FAILURE_ACTIONSW;




typedef SERVICE_FAILURE_ACTIONSA SERVICE_FAILURE_ACTIONS;
typedef LPSERVICE_FAILURE_ACTIONSA LPSERVICE_FAILURE_ACTIONS;
#line 246 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"






struct SC_HANDLE__ { int unused; }; typedef struct SC_HANDLE__ *SC_HANDLE;
typedef SC_HANDLE   *LPSC_HANDLE;

struct SERVICE_STATUS_HANDLE__ { int unused; }; typedef struct SERVICE_STATUS_HANDLE__ *SERVICE_STATUS_HANDLE;





typedef enum _SC_STATUS_TYPE {
        SC_STATUS_PROCESS_INFO      = 0
} SC_STATUS_TYPE;




typedef enum _SC_ENUM_TYPE {
        SC_ENUM_PROCESS_INFO        = 0
} SC_ENUM_TYPE;






typedef struct _SERVICE_STATUS {
    DWORD   dwServiceType;
    DWORD   dwCurrentState;
    DWORD   dwControlsAccepted;
    DWORD   dwWin32ExitCode;
    DWORD   dwServiceSpecificExitCode;
    DWORD   dwCheckPoint;
    DWORD   dwWaitHint;
} SERVICE_STATUS, *LPSERVICE_STATUS;

typedef struct _SERVICE_STATUS_PROCESS {
    DWORD   dwServiceType;
    DWORD   dwCurrentState;
    DWORD   dwControlsAccepted;
    DWORD   dwWin32ExitCode;
    DWORD   dwServiceSpecificExitCode;
    DWORD   dwCheckPoint;
    DWORD   dwWaitHint;
    DWORD   dwProcessId;
    DWORD   dwServiceFlags;
} SERVICE_STATUS_PROCESS, *LPSERVICE_STATUS_PROCESS;






typedef struct _ENUM_SERVICE_STATUSA {
    LPSTR             lpServiceName;
    LPSTR             lpDisplayName;
    SERVICE_STATUS    ServiceStatus;
} ENUM_SERVICE_STATUSA, *LPENUM_SERVICE_STATUSA;
typedef struct _ENUM_SERVICE_STATUSW {
    LPWSTR            lpServiceName;
    LPWSTR            lpDisplayName;
    SERVICE_STATUS    ServiceStatus;
} ENUM_SERVICE_STATUSW, *LPENUM_SERVICE_STATUSW;




typedef ENUM_SERVICE_STATUSA ENUM_SERVICE_STATUS;
typedef LPENUM_SERVICE_STATUSA LPENUM_SERVICE_STATUS;
#line 321 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

typedef struct _ENUM_SERVICE_STATUS_PROCESSA {
    LPSTR                     lpServiceName;
    LPSTR                     lpDisplayName;
    SERVICE_STATUS_PROCESS    ServiceStatusProcess;
} ENUM_SERVICE_STATUS_PROCESSA, *LPENUM_SERVICE_STATUS_PROCESSA;
typedef struct _ENUM_SERVICE_STATUS_PROCESSW {
    LPWSTR                    lpServiceName;
    LPWSTR                    lpDisplayName;
    SERVICE_STATUS_PROCESS    ServiceStatusProcess;
} ENUM_SERVICE_STATUS_PROCESSW, *LPENUM_SERVICE_STATUS_PROCESSW;




typedef ENUM_SERVICE_STATUS_PROCESSA ENUM_SERVICE_STATUS_PROCESS;
typedef LPENUM_SERVICE_STATUS_PROCESSA LPENUM_SERVICE_STATUS_PROCESS;
#line 339 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"





typedef LPVOID  SC_LOCK;

typedef struct _QUERY_SERVICE_LOCK_STATUSA {
    DWORD   fIsLocked;
    LPSTR   lpLockOwner;
    DWORD   dwLockDuration;
} QUERY_SERVICE_LOCK_STATUSA, *LPQUERY_SERVICE_LOCK_STATUSA;
typedef struct _QUERY_SERVICE_LOCK_STATUSW {
    DWORD   fIsLocked;
    LPWSTR  lpLockOwner;
    DWORD   dwLockDuration;
} QUERY_SERVICE_LOCK_STATUSW, *LPQUERY_SERVICE_LOCK_STATUSW;




typedef QUERY_SERVICE_LOCK_STATUSA QUERY_SERVICE_LOCK_STATUS;
typedef LPQUERY_SERVICE_LOCK_STATUSA LPQUERY_SERVICE_LOCK_STATUS;
#line 363 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"







typedef struct _QUERY_SERVICE_CONFIGA {
    DWORD   dwServiceType;
    DWORD   dwStartType;
    DWORD   dwErrorControl;
    LPSTR   lpBinaryPathName;
    LPSTR   lpLoadOrderGroup;
    DWORD   dwTagId;
    LPSTR   lpDependencies;
    LPSTR   lpServiceStartName;
    LPSTR   lpDisplayName;
} QUERY_SERVICE_CONFIGA, *LPQUERY_SERVICE_CONFIGA;
typedef struct _QUERY_SERVICE_CONFIGW {
    DWORD   dwServiceType;
    DWORD   dwStartType;
    DWORD   dwErrorControl;
    LPWSTR  lpBinaryPathName;
    LPWSTR  lpLoadOrderGroup;
    DWORD   dwTagId;
    LPWSTR  lpDependencies;
    LPWSTR  lpServiceStartName;
    LPWSTR  lpDisplayName;
} QUERY_SERVICE_CONFIGW, *LPQUERY_SERVICE_CONFIGW;




typedef QUERY_SERVICE_CONFIGA QUERY_SERVICE_CONFIG;
typedef LPQUERY_SERVICE_CONFIGA LPQUERY_SERVICE_CONFIG;
#line 399 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"







typedef void (__stdcall *LPSERVICE_MAIN_FUNCTIONW)(
    DWORD   dwNumServicesArgs,
    LPWSTR  *lpServiceArgVectors
    );

typedef void (__stdcall *LPSERVICE_MAIN_FUNCTIONA)(
    DWORD   dwNumServicesArgs,
    LPSTR   *lpServiceArgVectors
    );





#line 421 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"






typedef struct _SERVICE_TABLE_ENTRYA {
    LPSTR                       lpServiceName;
    LPSERVICE_MAIN_FUNCTIONA    lpServiceProc;
}SERVICE_TABLE_ENTRYA, *LPSERVICE_TABLE_ENTRYA;
typedef struct _SERVICE_TABLE_ENTRYW {
    LPWSTR                      lpServiceName;
    LPSERVICE_MAIN_FUNCTIONW    lpServiceProc;
}SERVICE_TABLE_ENTRYW, *LPSERVICE_TABLE_ENTRYW;




typedef SERVICE_TABLE_ENTRYA SERVICE_TABLE_ENTRY;
typedef LPSERVICE_TABLE_ENTRYA LPSERVICE_TABLE_ENTRY;
#line 442 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"





typedef void (__stdcall *LPHANDLER_FUNCTION)(
    DWORD    dwControl
    );

typedef DWORD (__stdcall *LPHANDLER_FUNCTION_EX)(
    DWORD    dwControl,
    DWORD    dwEventType,
    LPVOID   lpEventData,
    LPVOID   lpContext
    );






__declspec(dllimport)
BOOL
__stdcall
ChangeServiceConfigA(
    SC_HANDLE    hService,
    DWORD        dwServiceType,
    DWORD        dwStartType,
    DWORD        dwErrorControl,
    LPCSTR     lpBinaryPathName,
    LPCSTR     lpLoadOrderGroup,
    LPDWORD      lpdwTagId,
    LPCSTR     lpDependencies,
    LPCSTR     lpServiceStartName,
    LPCSTR     lpPassword,
    LPCSTR     lpDisplayName
    );
__declspec(dllimport)
BOOL
__stdcall
ChangeServiceConfigW(
    SC_HANDLE    hService,
    DWORD        dwServiceType,
    DWORD        dwStartType,
    DWORD        dwErrorControl,
    LPCWSTR     lpBinaryPathName,
    LPCWSTR     lpLoadOrderGroup,
    LPDWORD      lpdwTagId,
    LPCWSTR     lpDependencies,
    LPCWSTR     lpServiceStartName,
    LPCWSTR     lpPassword,
    LPCWSTR     lpDisplayName
    );




#line 500 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
ChangeServiceConfig2A(
    SC_HANDLE    hService,
    DWORD        dwInfoLevel,
    LPVOID       lpInfo
    );
__declspec(dllimport)
BOOL
__stdcall
ChangeServiceConfig2W(
    SC_HANDLE    hService,
    DWORD        dwInfoLevel,
    LPVOID       lpInfo
    );




#line 522 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
CloseServiceHandle(
    SC_HANDLE   hSCObject
    );

__declspec(dllimport)
BOOL
__stdcall
ControlService(
    SC_HANDLE           hService,
    DWORD               dwControl,
    LPSERVICE_STATUS    lpServiceStatus
    );

__declspec(dllimport)
SC_HANDLE
__stdcall
CreateServiceA(
    SC_HANDLE    hSCManager,
    LPCSTR     lpServiceName,
    LPCSTR     lpDisplayName,
    DWORD        dwDesiredAccess,
    DWORD        dwServiceType,
    DWORD        dwStartType,
    DWORD        dwErrorControl,
    LPCSTR     lpBinaryPathName,
    LPCSTR     lpLoadOrderGroup,
    LPDWORD      lpdwTagId,
    LPCSTR     lpDependencies,
    LPCSTR     lpServiceStartName,
    LPCSTR     lpPassword
    );
__declspec(dllimport)
SC_HANDLE
__stdcall
CreateServiceW(
    SC_HANDLE    hSCManager,
    LPCWSTR     lpServiceName,
    LPCWSTR     lpDisplayName,
    DWORD        dwDesiredAccess,
    DWORD        dwServiceType,
    DWORD        dwStartType,
    DWORD        dwErrorControl,
    LPCWSTR     lpBinaryPathName,
    LPCWSTR     lpLoadOrderGroup,
    LPDWORD      lpdwTagId,
    LPCWSTR     lpDependencies,
    LPCWSTR     lpServiceStartName,
    LPCWSTR     lpPassword
    );




#line 580 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
DeleteService(
    SC_HANDLE   hService
    );

__declspec(dllimport)
BOOL
__stdcall
EnumDependentServicesA(
    SC_HANDLE               hService,
    DWORD                   dwServiceState,
    LPENUM_SERVICE_STATUSA  lpServices,
    DWORD                   cbBufSize,
    LPDWORD                 pcbBytesNeeded,
    LPDWORD                 lpServicesReturned
    );
__declspec(dllimport)
BOOL
__stdcall
EnumDependentServicesW(
    SC_HANDLE               hService,
    DWORD                   dwServiceState,
    LPENUM_SERVICE_STATUSW  lpServices,
    DWORD                   cbBufSize,
    LPDWORD                 pcbBytesNeeded,
    LPDWORD                 lpServicesReturned
    );




#line 615 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
EnumServicesStatusA(
    SC_HANDLE               hSCManager,
    DWORD                   dwServiceType,
    DWORD                   dwServiceState,
    LPENUM_SERVICE_STATUSA  lpServices,
    DWORD                   cbBufSize,
    LPDWORD                 pcbBytesNeeded,
    LPDWORD                 lpServicesReturned,
    LPDWORD                 lpResumeHandle
    );
__declspec(dllimport)
BOOL
__stdcall
EnumServicesStatusW(
    SC_HANDLE               hSCManager,
    DWORD                   dwServiceType,
    DWORD                   dwServiceState,
    LPENUM_SERVICE_STATUSW  lpServices,
    DWORD                   cbBufSize,
    LPDWORD                 pcbBytesNeeded,
    LPDWORD                 lpServicesReturned,
    LPDWORD                 lpResumeHandle
    );




#line 647 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
EnumServicesStatusExA(
    SC_HANDLE                  hSCManager,
    SC_ENUM_TYPE               InfoLevel,
    DWORD                      dwServiceType,
    DWORD                      dwServiceState,
    LPBYTE                     lpServices,
    DWORD                      cbBufSize,
    LPDWORD                    pcbBytesNeeded,
    LPDWORD                    lpServicesReturned,
    LPDWORD                    lpResumeHandle,
    LPCSTR                   pszGroupName
    );
__declspec(dllimport)
BOOL
__stdcall
EnumServicesStatusExW(
    SC_HANDLE                  hSCManager,
    SC_ENUM_TYPE               InfoLevel,
    DWORD                      dwServiceType,
    DWORD                      dwServiceState,
    LPBYTE                     lpServices,
    DWORD                      cbBufSize,
    LPDWORD                    pcbBytesNeeded,
    LPDWORD                    lpServicesReturned,
    LPDWORD                    lpResumeHandle,
    LPCWSTR                   pszGroupName
    );




#line 683 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
GetServiceKeyNameA(
    SC_HANDLE               hSCManager,
    LPCSTR                lpDisplayName,
    LPSTR                 lpServiceName,
    LPDWORD                 lpcchBuffer
    );
__declspec(dllimport)
BOOL
__stdcall
GetServiceKeyNameW(
    SC_HANDLE               hSCManager,
    LPCWSTR                lpDisplayName,
    LPWSTR                 lpServiceName,
    LPDWORD                 lpcchBuffer
    );




#line 707 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
GetServiceDisplayNameA(
    SC_HANDLE               hSCManager,
    LPCSTR                lpServiceName,
    LPSTR                 lpDisplayName,
    LPDWORD                 lpcchBuffer
    );
__declspec(dllimport)
BOOL
__stdcall
GetServiceDisplayNameW(
    SC_HANDLE               hSCManager,
    LPCWSTR                lpServiceName,
    LPWSTR                 lpDisplayName,
    LPDWORD                 lpcchBuffer
    );




#line 731 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
SC_LOCK
__stdcall
LockServiceDatabase(
    SC_HANDLE   hSCManager
    );

__declspec(dllimport)
BOOL
__stdcall
NotifyBootConfigStatus(
    BOOL     BootAcceptable
    );

__declspec(dllimport)
SC_HANDLE
__stdcall
OpenSCManagerA(
    LPCSTR lpMachineName,
    LPCSTR lpDatabaseName,
    DWORD   dwDesiredAccess
    );
__declspec(dllimport)
SC_HANDLE
__stdcall
OpenSCManagerW(
    LPCWSTR lpMachineName,
    LPCWSTR lpDatabaseName,
    DWORD   dwDesiredAccess
    );




#line 767 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
SC_HANDLE
__stdcall
OpenServiceA(
    SC_HANDLE   hSCManager,
    LPCSTR    lpServiceName,
    DWORD       dwDesiredAccess
    );
__declspec(dllimport)
SC_HANDLE
__stdcall
OpenServiceW(
    SC_HANDLE   hSCManager,
    LPCWSTR    lpServiceName,
    DWORD       dwDesiredAccess
    );




#line 789 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
QueryServiceConfigA(
    SC_HANDLE               hService,
    LPQUERY_SERVICE_CONFIGA lpServiceConfig,
    DWORD                   cbBufSize,
    LPDWORD                 pcbBytesNeeded
    );
__declspec(dllimport)
BOOL
__stdcall
QueryServiceConfigW(
    SC_HANDLE               hService,
    LPQUERY_SERVICE_CONFIGW lpServiceConfig,
    DWORD                   cbBufSize,
    LPDWORD                 pcbBytesNeeded
    );




#line 813 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
QueryServiceConfig2A(
    SC_HANDLE   hService,
    DWORD       dwInfoLevel,
    LPBYTE      lpBuffer,
    DWORD       cbBufSize,
    LPDWORD     pcbBytesNeeded
    );
__declspec(dllimport)
BOOL
__stdcall
QueryServiceConfig2W(
    SC_HANDLE   hService,
    DWORD       dwInfoLevel,
    LPBYTE      lpBuffer,
    DWORD       cbBufSize,
    LPDWORD     pcbBytesNeeded
    );




#line 839 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
QueryServiceLockStatusA(
    SC_HANDLE                       hSCManager,
    LPQUERY_SERVICE_LOCK_STATUSA    lpLockStatus,
    DWORD                           cbBufSize,
    LPDWORD                         pcbBytesNeeded
    );
__declspec(dllimport)
BOOL
__stdcall
QueryServiceLockStatusW(
    SC_HANDLE                       hSCManager,
    LPQUERY_SERVICE_LOCK_STATUSW    lpLockStatus,
    DWORD                           cbBufSize,
    LPDWORD                         pcbBytesNeeded
    );




#line 863 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
QueryServiceObjectSecurity(
    SC_HANDLE               hService,
    SECURITY_INFORMATION    dwSecurityInformation,
    PSECURITY_DESCRIPTOR    lpSecurityDescriptor,
    DWORD                   cbBufSize,
    LPDWORD                 pcbBytesNeeded
    );

__declspec(dllimport)
BOOL
__stdcall
QueryServiceStatus(
    SC_HANDLE           hService,
    LPSERVICE_STATUS    lpServiceStatus
    );

__declspec(dllimport)
BOOL
__stdcall
QueryServiceStatusEx(
    SC_HANDLE           hService,
    SC_STATUS_TYPE      InfoLevel,
    LPBYTE              lpBuffer,
    DWORD               cbBufSize,
    LPDWORD             pcbBytesNeeded
    );

__declspec(dllimport)
SERVICE_STATUS_HANDLE
__stdcall
RegisterServiceCtrlHandlerA(
    LPCSTR             lpServiceName,
    LPHANDLER_FUNCTION   lpHandlerProc
    );
__declspec(dllimport)
SERVICE_STATUS_HANDLE
__stdcall
RegisterServiceCtrlHandlerW(
    LPCWSTR             lpServiceName,
    LPHANDLER_FUNCTION   lpHandlerProc
    );




#line 913 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
SERVICE_STATUS_HANDLE
__stdcall
RegisterServiceCtrlHandlerExA(
    LPCSTR                lpServiceName,
    LPHANDLER_FUNCTION_EX   lpHandlerProc,
    LPVOID                  lpContext
    );
__declspec(dllimport)
SERVICE_STATUS_HANDLE
__stdcall
RegisterServiceCtrlHandlerExW(
    LPCWSTR                lpServiceName,
    LPHANDLER_FUNCTION_EX   lpHandlerProc,
    LPVOID                  lpContext
    );




#line 935 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
SetServiceObjectSecurity(
    SC_HANDLE               hService,
    SECURITY_INFORMATION    dwSecurityInformation,
    PSECURITY_DESCRIPTOR    lpSecurityDescriptor
    );

__declspec(dllimport)
BOOL
__stdcall
SetServiceStatus(
    SERVICE_STATUS_HANDLE   hServiceStatus,
    LPSERVICE_STATUS        lpServiceStatus
    );

__declspec(dllimport)
BOOL
__stdcall
StartServiceCtrlDispatcherA(
    const SERVICE_TABLE_ENTRYA *lpServiceStartTable
    );
__declspec(dllimport)
BOOL
__stdcall
StartServiceCtrlDispatcherW(
    const SERVICE_TABLE_ENTRYW *lpServiceStartTable
    );




#line 970 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"


__declspec(dllimport)
BOOL
__stdcall
StartServiceA(
    SC_HANDLE            hService,
    DWORD                dwNumServiceArgs,
    LPCSTR             *lpServiceArgVectors
    );
__declspec(dllimport)
BOOL
__stdcall
StartServiceW(
    SC_HANDLE            hService,
    DWORD                dwNumServiceArgs,
    LPCWSTR             *lpServiceArgVectors
    );




#line 993 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

__declspec(dllimport)
BOOL
__stdcall
UnlockServiceDatabase(
    SC_LOCK     ScLock
    );



}
#line 1005 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"

#line 1007 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\winsvc.h"
#line 228 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 229 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"



#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\mcx.h"












#pragma once
#line 15 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\mcx.h"

typedef struct _MODEMDEVCAPS {
    DWORD   dwActualSize;
    DWORD   dwRequiredSize;
    DWORD   dwDevSpecificOffset;
    DWORD   dwDevSpecificSize;

    
    DWORD   dwModemProviderVersion;
    DWORD   dwModemManufacturerOffset;
    DWORD   dwModemManufacturerSize;
    DWORD   dwModemModelOffset;
    DWORD   dwModemModelSize;
    DWORD   dwModemVersionOffset;
    DWORD   dwModemVersionSize;

    
    DWORD   dwDialOptions;          
    DWORD   dwCallSetupFailTimer;   
    DWORD   dwInactivityTimeout;    
    DWORD   dwSpeakerVolume;        
    DWORD   dwSpeakerMode;          
    DWORD   dwModemOptions;         
    DWORD   dwMaxDTERate;           
    DWORD   dwMaxDCERate;           

    
    BYTE    abVariablePortion [1];
} MODEMDEVCAPS, *PMODEMDEVCAPS, *LPMODEMDEVCAPS;

typedef struct _MODEMSETTINGS {
    DWORD   dwActualSize;
    DWORD   dwRequiredSize;
    DWORD   dwDevSpecificOffset;
    DWORD   dwDevSpecificSize;

    
    DWORD   dwCallSetupFailTimer;       
    DWORD   dwInactivityTimeout;        
    DWORD   dwSpeakerVolume;            
    DWORD   dwSpeakerMode;              
    DWORD   dwPreferredModemOptions;    

    
    DWORD   dwNegotiatedModemOptions;   
    DWORD   dwNegotiatedDCERate;        

    
    BYTE    abVariablePortion [1];
} MODEMSETTINGS, *PMODEMSETTINGS, *LPMODEMSETTINGS;

















































































































































































































































































































































































































































































































































































































































































#line 723 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\mcx.h"
#line 233 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 234 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"


#line 1 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"











extern "C" {
#line 14 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"






struct HIMC__ { int unused; }; typedef struct HIMC__ *HIMC;
struct HIMCC__ { int unused; }; typedef struct HIMCC__ *HIMCC;



#line 26 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

typedef HKL   *LPHKL;
typedef UINT  *LPUINT;

typedef struct tagCOMPOSITIONFORM {
    DWORD dwStyle;
    POINT ptCurrentPos;
    RECT  rcArea;
} COMPOSITIONFORM, *PCOMPOSITIONFORM,  *NPCOMPOSITIONFORM,  *LPCOMPOSITIONFORM;


typedef struct tagCANDIDATEFORM {
    DWORD dwIndex;
    DWORD dwStyle;
    POINT ptCurrentPos;
    RECT  rcArea;
} CANDIDATEFORM, *PCANDIDATEFORM,  *NPCANDIDATEFORM,  *LPCANDIDATEFORM;


typedef struct tagCANDIDATELIST {
    DWORD dwSize;
    DWORD dwStyle;
    DWORD dwCount;
    DWORD dwSelection;
    DWORD dwPageStart;
    DWORD dwPageSize;
    DWORD dwOffset[1];
} CANDIDATELIST, *PCANDIDATELIST,  *NPCANDIDATELIST,  *LPCANDIDATELIST;

typedef struct tagREGISTERWORDA {
    LPSTR   lpReading;
    LPSTR   lpWord;
} REGISTERWORDA, *PREGISTERWORDA,  *NPREGISTERWORDA,  *LPREGISTERWORDA;
typedef struct tagREGISTERWORDW {
    LPWSTR  lpReading;
    LPWSTR  lpWord;
} REGISTERWORDW, *PREGISTERWORDW,  *NPREGISTERWORDW,  *LPREGISTERWORDW;






typedef REGISTERWORDA REGISTERWORD;
typedef PREGISTERWORDA PREGISTERWORD;
typedef NPREGISTERWORDA NPREGISTERWORD;
typedef LPREGISTERWORDA LPREGISTERWORD;
#line 74 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"



typedef struct tagRECONVERTSTRING {
    DWORD dwSize;
    DWORD dwVersion;
    DWORD dwStrLen;
    DWORD dwStrOffset;
    DWORD dwCompStrLen;
    DWORD dwCompStrOffset;
    DWORD dwTargetStrLen;
    DWORD dwTargetStrOffset;
} RECONVERTSTRING, *PRECONVERTSTRING,  *NPRECONVERTSTRING,  *LPRECONVERTSTRING;

#line 89 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"



typedef struct tagSTYLEBUFA {
    DWORD       dwStyle;
    CHAR        szDescription[32];
} STYLEBUFA, *PSTYLEBUFA,  *NPSTYLEBUFA,  *LPSTYLEBUFA;
typedef struct tagSTYLEBUFW {
    DWORD       dwStyle;
    WCHAR       szDescription[32];
} STYLEBUFW, *PSTYLEBUFW,  *NPSTYLEBUFW,  *LPSTYLEBUFW;






typedef STYLEBUFA STYLEBUF;
typedef PSTYLEBUFA PSTYLEBUF;
typedef NPSTYLEBUFA NPSTYLEBUF;
typedef LPSTYLEBUFA LPSTYLEBUF;
#line 111 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"






typedef struct tagIMEMENUITEMINFOA {
    UINT        cbSize;
    UINT        fType;
    UINT        fState;
    UINT        wID;
    HBITMAP     hbmpChecked;
    HBITMAP     hbmpUnchecked;
    DWORD       dwItemData;
    CHAR        szString[80];
    HBITMAP     hbmpItem;
} IMEMENUITEMINFOA, *PIMEMENUITEMINFOA,  *NPIMEMENUITEMINFOA,  *LPIMEMENUITEMINFOA;
typedef struct tagIMEMENUITEMINFOW {
    UINT        cbSize;
    UINT        fType;
    UINT        fState;
    UINT        wID;
    HBITMAP     hbmpChecked;
    HBITMAP     hbmpUnchecked;
    DWORD       dwItemData;
    WCHAR       szString[80];
    HBITMAP     hbmpItem;
} IMEMENUITEMINFOW, *PIMEMENUITEMINFOW,  *NPIMEMENUITEMINFOW,  *LPIMEMENUITEMINFOW;






typedef IMEMENUITEMINFOA IMEMENUITEMINFO;
typedef PIMEMENUITEMINFOA PIMEMENUITEMINFO;
typedef NPIMEMENUITEMINFOA NPIMEMENUITEMINFO;
typedef LPIMEMENUITEMINFOA LPIMEMENUITEMINFO;
#line 150 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

typedef struct tagIMECHARPOSITION {
    DWORD       dwSize;
    DWORD       dwCharPos;
    POINT       pt;
    UINT        cLineHeight;
    RECT        rcDocument;
} IMECHARPOSITION, *PIMECHARPOSITION,  *NPIMECHARPOSITION,  *LPIMECHARPOSITION;

typedef BOOL    (__stdcall* IMCENUMPROC)(HIMC, LPARAM);

#line 162 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"





HKL  __stdcall ImmInstallIMEA( LPCSTR lpszIMEFileName,  LPCSTR lpszLayoutText);
HKL  __stdcall ImmInstallIMEW( LPCWSTR lpszIMEFileName,  LPCWSTR lpszLayoutText);




#line 174 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

HWND __stdcall ImmGetDefaultIMEWnd( HWND);

UINT __stdcall ImmGetDescriptionA( HKL,  LPSTR,  UINT uBufLen);
UINT __stdcall ImmGetDescriptionW( HKL,  LPWSTR,  UINT uBufLen);




#line 184 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

UINT __stdcall ImmGetIMEFileNameA( HKL,  LPSTR,  UINT uBufLen);
UINT __stdcall ImmGetIMEFileNameW( HKL,  LPWSTR,  UINT uBufLen);




#line 192 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

DWORD __stdcall ImmGetProperty( HKL,  DWORD);

BOOL __stdcall ImmIsIME( HKL);

BOOL __stdcall ImmSimulateHotKey( HWND,  DWORD);

HIMC __stdcall ImmCreateContext(void);
BOOL __stdcall ImmDestroyContext( HIMC);
HIMC __stdcall ImmGetContext( HWND);
BOOL __stdcall ImmReleaseContext( HWND,  HIMC);
HIMC __stdcall ImmAssociateContext( HWND,  HIMC);

BOOL __stdcall ImmAssociateContextEx( HWND,  HIMC,  DWORD);
#line 207 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

LONG  __stdcall ImmGetCompositionStringA( HIMC,  DWORD,  LPVOID,  DWORD);
LONG  __stdcall ImmGetCompositionStringW( HIMC,  DWORD,  LPVOID,  DWORD);




#line 215 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

BOOL  __stdcall ImmSetCompositionStringA( HIMC,  DWORD dwIndex,  LPVOID lpComp,  DWORD,  LPVOID lpRead,  DWORD);
BOOL  __stdcall ImmSetCompositionStringW( HIMC,  DWORD dwIndex,  LPVOID lpComp,  DWORD,  LPVOID lpRead,  DWORD);




#line 223 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

DWORD __stdcall ImmGetCandidateListCountA( HIMC,  LPDWORD lpdwListCount);
DWORD __stdcall ImmGetCandidateListCountW( HIMC,  LPDWORD lpdwListCount);




#line 231 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

DWORD __stdcall ImmGetCandidateListA( HIMC,  DWORD deIndex,  LPCANDIDATELIST,  DWORD dwBufLen);
DWORD __stdcall ImmGetCandidateListW( HIMC,  DWORD deIndex,  LPCANDIDATELIST,  DWORD dwBufLen);




#line 239 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

DWORD __stdcall ImmGetGuideLineA( HIMC,  DWORD dwIndex,  LPSTR,  DWORD dwBufLen);
DWORD __stdcall ImmGetGuideLineW( HIMC,  DWORD dwIndex,  LPWSTR,  DWORD dwBufLen);




#line 247 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

BOOL __stdcall ImmGetConversionStatus( HIMC,  LPDWORD,  LPDWORD);
BOOL __stdcall ImmSetConversionStatus( HIMC,  DWORD,  DWORD);
BOOL __stdcall ImmGetOpenStatus( HIMC);
BOOL __stdcall ImmSetOpenStatus( HIMC,  BOOL);


BOOL __stdcall ImmGetCompositionFontA( HIMC,  LPLOGFONTA);
BOOL __stdcall ImmGetCompositionFontW( HIMC,  LPLOGFONTW);




#line 261 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

BOOL __stdcall ImmSetCompositionFontA( HIMC,  LPLOGFONTA);
BOOL __stdcall ImmSetCompositionFontW( HIMC,  LPLOGFONTW);




#line 269 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"
#line 270 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

BOOL    __stdcall ImmConfigureIMEA( HKL,  HWND,  DWORD,  LPVOID);
BOOL    __stdcall ImmConfigureIMEW( HKL,  HWND,  DWORD,  LPVOID);




#line 278 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

LRESULT __stdcall ImmEscapeA( HKL,  HIMC,  UINT,  LPVOID);
LRESULT __stdcall ImmEscapeW( HKL,  HIMC,  UINT,  LPVOID);




#line 286 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

DWORD   __stdcall ImmGetConversionListA( HKL,  HIMC,  LPCSTR,  LPCANDIDATELIST,  DWORD dwBufLen,  UINT uFlag);
DWORD   __stdcall ImmGetConversionListW( HKL,  HIMC,  LPCWSTR,  LPCANDIDATELIST,  DWORD dwBufLen,  UINT uFlag);




#line 294 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

BOOL    __stdcall ImmNotifyIME( HIMC,  DWORD dwAction,  DWORD dwIndex,  DWORD dwValue);

BOOL __stdcall ImmGetStatusWindowPos( HIMC,  LPPOINT);
BOOL __stdcall ImmSetStatusWindowPos( HIMC,  LPPOINT);
BOOL __stdcall ImmGetCompositionWindow( HIMC,  LPCOMPOSITIONFORM);
BOOL __stdcall ImmSetCompositionWindow( HIMC,  LPCOMPOSITIONFORM);
BOOL __stdcall ImmGetCandidateWindow( HIMC,  DWORD,  LPCANDIDATEFORM);
BOOL __stdcall ImmSetCandidateWindow( HIMC,  LPCANDIDATEFORM);

BOOL __stdcall ImmIsUIMessageA( HWND,  UINT,  WPARAM,  LPARAM);
BOOL __stdcall ImmIsUIMessageW( HWND,  UINT,  WPARAM,  LPARAM);




#line 311 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"


UINT __stdcall ImmGetVirtualKey( HWND);

typedef int (__stdcall *REGISTERWORDENUMPROCA)(LPCSTR, DWORD, LPCSTR, LPVOID);
typedef int (__stdcall *REGISTERWORDENUMPROCW)(LPCWSTR, DWORD, LPCWSTR, LPVOID);




#line 322 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

BOOL __stdcall ImmRegisterWordA( HKL,  LPCSTR lpszReading,  DWORD,  LPCSTR lpszRegister);
BOOL __stdcall ImmRegisterWordW( HKL,  LPCWSTR lpszReading,  DWORD,  LPCWSTR lpszRegister);




#line 330 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

BOOL __stdcall ImmUnregisterWordA( HKL,  LPCSTR lpszReading,  DWORD,  LPCSTR lpszUnregister);
BOOL __stdcall ImmUnregisterWordW( HKL,  LPCWSTR lpszReading,  DWORD,  LPCWSTR lpszUnregister);




#line 338 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

UINT __stdcall ImmGetRegisterWordStyleA( HKL,  UINT nItem,  LPSTYLEBUFA);
UINT __stdcall ImmGetRegisterWordStyleW( HKL,  UINT nItem,  LPSTYLEBUFW);




#line 346 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

UINT __stdcall ImmEnumRegisterWordA( HKL,  REGISTERWORDENUMPROCA,  LPCSTR lpszReading,  DWORD,  LPCSTR lpszRegister,  LPVOID);
UINT __stdcall ImmEnumRegisterWordW( HKL,  REGISTERWORDENUMPROCW,  LPCWSTR lpszReading,  DWORD,  LPCWSTR lpszRegister,  LPVOID);




#line 354 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"


BOOL __stdcall ImmDisableIME( DWORD);
BOOL __stdcall ImmEnumInputContext(DWORD idThread, IMCENUMPROC lpfn, LPARAM lParam);
DWORD __stdcall ImmGetImeMenuItemsA( HIMC,  DWORD,  DWORD,  LPIMEMENUITEMINFOA,  LPIMEMENUITEMINFOA,  DWORD);
DWORD __stdcall ImmGetImeMenuItemsW( HIMC,  DWORD,  DWORD,  LPIMEMENUITEMINFOW,  LPIMEMENUITEMINFOW,  DWORD);




#line 365 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

BOOL __stdcall ImmDisableTextFrameService(DWORD idThread);
#line 368 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"








































































































































































































































































#line 633 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"





































































#line 703 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"








#line 712 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"


}
#line 716 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"

#line 718 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\imm.h"
#line 237 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 238 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 239 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 240 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"




#pragma warning(pop)





#line 251 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 252 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 253 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

#line 255 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

#line 257 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"
#line 258 "c:\\program files\\microsoft visual studio .net 2003\\vc7\\platformsdk\\include\\windows.h"

#line 12 "c:\\work\\cossacks\\vopyl\\nicevopyl\\stdafx.h"


#line 6 "c:\\work\\cossacks\\vopyl\\nicevopyl\\stdafx.cpp"



