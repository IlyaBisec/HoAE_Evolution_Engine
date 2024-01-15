/*****************************************************************************/
/*    File:    kAssert.h
/*    Desc:    Assertion tools
/*    Author:    Ruslan Shestopalyuk
/*    Date:    25.02.2003
/*****************************************************************************/
#ifndef __KASSERT_H__
#define __KASSERT_H__

/*****************************************************************************/
/*    Enum:    FDResult
/*    Desc:    assertion failure dialog results
/*****************************************************************************/
enum FDResult
{
    fdrUnknown    = 0,
    fdrAbort    = 1,
    fdrDebug    = 2,
    fdrIgnore    = 3
}; // enum FDResult

const int c_MaxExpressionLen    = 64; 
const int c_MaxAssertMessageLen = 128;
/*****************************************************************************/
/*    Class:    AssertionFailure
/*    Desc:    Single assertion failure case
/*****************************************************************************/
class AssertionFailure
{
public:
    AssertionFailure( const char* expr, const char* msg, const char* cFile, int cLine );
    AssertionFailure();

    const char* GetShortFileName();

    bool        equal( const AssertionFailure& assf );

protected:
    char        file[_MAX_PATH];
    int            line;

    char        expression    [c_MaxExpressionLen];
    char        message        [c_MaxAssertMessageLen];

private:
    friend class AssertMgr;

    static void    CopyStr( char* dest, const char* src, int maxLen );
}; // class Assertion

const int c_MaxAssertIgnoreListSize = 64;
/*****************************************************************************/
/*    Class:    AssertMgr    
/*    Desc:    Deals with assert failures
/*****************************************************************************/
class AssertMgr
{
    static FDResult                lastDlgResult;
    static AssertionFailure        lastFailure;
    static bool                    dlgActive;
    static bool                    doIgnoreAlways;
    static HWND                    hParentWnd;
    static HWND                    hFailureDialogWnd;

public:
    static void            Init( HWND hWnd );

    static FDResult        Failure( const AssertionFailure& asf, bool& ignore );
    static INT CALLBACK    FailureDlgProc( HWND    hWnd, 
                                        UINT    msg, 
                                        WPARAM    wParam, 
                                        LPARAM    lParam );

    static void            ShowLogText();

}; // class AssertMgr


#ifndef _INLINES
#undef  assert
#define assert(A)        {if (!(A)) { \
                                    AssertionFailure ass(#A,NULL,__FILE__,__LINE__);\
                                    static bool __ignore_always = false;\
                                    FDResult result = AssertMgr::Failure( ass, __ignore_always );\
                                    if (result == fdrDebug) \
                                    {__asm int 3 }\
                                    else if (result == fdrAbort) exit( 1 );\
                                    }}
#define massert(A,M)    {if (!(A)) { \
                                    AssertionFailure ass(#A,M,__FILE__,__LINE__);\
                                    static bool __ignore_always = false;\
                                    FDResult result = AssertMgr::Failure( ass, __ignore_always );\
                                    if (result == fdrAbort)\
                                    {\
                                        exit( 1 );\
                                    }else if (result == fdrDebug)\
                                    {\
                                    __asm {int 3};\
                                    }\
                                    }}
#define todo(A)            {massert(!"TODO!",(A));}
#else
#define assert(A)        ;
#define massert(A,M)    ;
#define todo(A)            ;
#endif // _DEBUG

#define rtcheck(A,M)

#endif // __KASSERT_H__