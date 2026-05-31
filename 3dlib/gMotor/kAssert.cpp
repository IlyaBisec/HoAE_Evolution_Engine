/*****************************************************************************/
/*    File:    kAssert.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    25.02.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kAssert.h"
#include "gMotorRC.h"

/*****************************************************************************/
/*    AssertMgr implementation
/*****************************************************************************/
FDResult            AssertMgr::lastDlgResult;
AssertionFailure    AssertMgr::lastFailure;
bool                AssertMgr::dlgActive;
bool                AssertMgr::doIgnoreAlways;
HWND                AssertMgr::hParentWnd;
HWND                AssertMgr::hFailureDialogWnd;

void AssertMgr::Init( HWND hWnd )
{
    hParentWnd = hWnd;
} // AssertMgr::Init

AssertionFailure::AssertionFailure( const char* expr, const char* msg, const char* cFile, int cLine )
{
    CopyStr( expression,    expr,  c_MaxExpressionLen    );    
    CopyStr( file,            cFile, _MAX_PATH            );
    CopyStr( message,        msg,   c_MaxAssertMessageLen);
    line = cLine;
} // AssertionFailure::AssertionFailure

void AssertionFailure::CopyStr( char* dest, const char* src, int maxLen )
{
    if (!src) 
    {
        dest[0] = 0;
        return;
    }
    int nSym = strlen( src );
    if (nSym < maxLen)
    {
        strcpy( dest, src );
    }
    else
    {
        strncpy( dest, src, maxLen - 1 );
        dest[maxLen - 1] = 0;
    }
} // AssertionFailure::CopyStr


const char* AssertionFailure::GetShortFileName()
{
    char sDrive    [_MAX_DRIVE];
    char sDir    [_MAX_PATH];
    char sFile    [_MAX_PATH];
    char sExt    [_MAX_PATH];

    _splitpath( file, sDrive, sDir, sFile, sExt ); 
    return file + strlen( file ) - strlen( sFile ) - strlen( sExt ); 
}

bool AssertionFailure::equal( const AssertionFailure& assf )
{
    if (line == assf.line && 
        !strcmp( file, assf.file ) && 
        !strcmp( expression, assf.expression )) return true;
    return false;
}

AssertionFailure::AssertionFailure()
{
    expression[0]    = 0;
    file[0]            = 0;
    message[0]        = 0;
    line            = 0;
}

#ifdef IDD_ERROR
#define GEC_NO_ASSERT
FDResult AssertMgr::Failure( const AssertionFailure& asf, bool& ignore )
{
	
    if (dlgActive) return fdrUnknown;
    lastFailure = asf;

    if (ignore) return fdrIgnore;

    HINSTANCE hInst = GetModuleHandle( NULL );

    dlgActive = true;

    lastDlgResult = fdrDebug;

#ifndef GEC_NO_ASSERT
    DialogBox(    hInst,
                MAKEINTRESOURCE(IDD_ERROR), 
                hParentWnd,
                FailureDlgProc );
#else //GEC_NO_ASSERT
	doIgnoreAlways=true;	
	ignore=true;
	lastDlgResult = fdrIgnore;
#endif //GEC_NO_ASSERT
    
    dlgActive = false;

    ignore = doIgnoreAlways;
    return lastDlgResult;
} // AssertMgr::Failure

void AssertMgr::ShowLogText()
{
    HWND hLogWnd = GetDlgItem( hFailureDialogWnd, LST_LOG );
    if (!hLogWnd) return;
    
    //  clear list box
    SendMessage( hLogWnd, LB_RESETCONTENT, 0, 0 );

    FILE* fp = fopen( Log.GetLogFilePath(), "rt" );
    if (!fp) return;
    
    char buffer[1024];

    int nRows = 0;
    while (!feof( fp ))
    {
        if (!fgets( buffer, 1024, fp )) break;
        int nSym = strlen( buffer );
        nSym--;
        while (nSym >= 0 && buffer[nSym] < 32)
        {
            buffer[nSym] = 0;
            nSym--;
        }
        
        SendMessage( hLogWnd, LB_ADDSTRING, 0, (LPARAM)buffer );
        nRows++;
    }
    fclose( fp );

    SendMessage( hLogWnd, LB_SETCURSEL, nRows - 1, 0 );
    UpdateWindow( hLogWnd );
} // AssertMgr::ShowLogText


INT CALLBACK    AssertMgr::FailureDlgProc( HWND hWnd, 
                                            UINT msg, 
                                            WPARAM wParam, 
                                            LPARAM lParam )
{    
    HWND hErrMessage;   
    HWND hExpression;
    HWND hFile;            
    HWND hLine;

    hFailureDialogWnd = hWnd;
    
    if (msg == WM_INITDIALOG) 
    {
        hErrMessage     = GetDlgItem( hWnd, ED_MESSAGE );
        hExpression        = GetDlgItem( hWnd, ED_EXPRESSION );
        hFile            = GetDlgItem( hWnd, ED_FILE  );
        hLine            = GetDlgItem( hWnd, ED_LINE );
        
        //  expression
        SetWindowText( hExpression, lastFailure.expression );
        //  error message
        SetWindowText( hErrMessage, lastFailure.message );
        //  file name
        SetWindowText( hFile, lastFailure.GetShortFileName() );
        //  line
        char  lineTxt[64];
        sprintf( lineTxt, "%d", lastFailure.line );
        SetWindowText( hLine, lineTxt );

        ShowLogText();

        return TRUE;
    }
    
    if (msg == WM_CLOSE)
    {
        lastDlgResult = fdrDebug;
        EndDialog( hWnd, IDCANCEL );
        return TRUE;
    }

    if (msg == WM_COMMAND)
    {
        switch (LOWORD( wParam ))
        {
            case IDABORT:  
                lastDlgResult = fdrAbort;
                EndDialog( hWnd, IDCANCEL );
                break;
            case IDDEBUG:  
                lastDlgResult = fdrDebug;
                EndDialog( hWnd, IDCANCEL );
                break;
            case IDIGNORE:                
                lastDlgResult = fdrIgnore;
                //  check if ignore always
                HWND hIgnoreAlways = GetDlgItem( hWnd, IDC_IGNOREALWAYS );
                if (SendMessage( hIgnoreAlways, BM_GETCHECK, 0, 0 ) == BST_CHECKED)
                {
                    doIgnoreAlways = true;
                }
                else
                {
                    doIgnoreAlways = false;
                }

                EndDialog( hWnd, IDCANCEL );
                break;
        }
        return TRUE;
    }
    return FALSE;
} // AssertMgr::FailureDlgProc

#endif  // IDD_ERROR

#ifndef IDD_ERROR
FDResult AssertMgr::Failure( const AssertionFailure& asf, bool& ignore )
{
    lastDlgResult = fdrDebug;
    return lastDlgResult;
} // AssertMgr::Failure

INT CALLBACK    AssertMgr::FailureDlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    return 0;
}

void AssertMgr::ShowLogText()
{
}

#endif // !IDD_ERROR


