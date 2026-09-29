//-----------------------------------------------------------------------------
// File: DXUtil.h
//
// Desc: Helper functions and typing shortcuts for DirectX programming.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//-----------------------------------------------------------------------------
#ifndef DXUTIL_H
#define DXUTIL_H

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if (p) { delete (p);   (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p); (p) = NULL; } }

#ifndef UNDER_CE

HRESULT DXUtil_GetDXSDKMediaPathCch(TCHAR *strDest, int cchDest);
HRESULT DXUtil_GetDXSDKMediaPathCb(TCHAR *szDest, int cbDest);

HRESULT DXUtil_FindMediaFileCch(
    TCHAR *strDestPath,
    int cchDest,
    LPCTSTR strFilename
);

HRESULT DXUtil_FindMediaFileCb(
    TCHAR *szDestPath,
    int cbDest,
    LPCTSTR strFilename
);

#endif // !UNDER_CE


//-----------------------------------------------------------------------------
// Registry helpers
//-----------------------------------------------------------------------------
HRESULT DXUtil_WriteStringRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    LPCTSTR strValue
);

HRESULT DXUtil_WriteFloatRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    FLOAT fValue
);

HRESULT DXUtil_WriteIntRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    DWORD dwValue
);

HRESULT DXUtil_WriteGuidRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    GUID guidValue
);

HRESULT DXUtil_WriteBoolRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    BOOL bValue
);

HRESULT DXUtil_ReadStringRegKeyCch(
    HKEY hKey,
    LPCTSTR strRegName,
    TCHAR *strDest,
    DWORD cchDest,
    LPCTSTR strDefault
);

HRESULT DXUtil_ReadStringRegKeyCb(
    HKEY hKey,
    LPCTSTR strRegName,
    TCHAR *strDest,
    DWORD cbDest,
    LPCTSTR strDefault
);

HRESULT DXUtil_ReadFloatRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    FLOAT *fDest,
    FLOAT fDefault
);

HRESULT DXUtil_ReadIntRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    DWORD *pdwValue,
    DWORD dwDefault
);

HRESULT DXUtil_ReadGuidRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    GUID *pGuidValue,
    GUID &guidDefault
);

HRESULT DXUtil_ReadBoolRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    BOOL *pbValue,
    BOOL bDefault
);


//-----------------------------------------------------------------------------
// Timer
//-----------------------------------------------------------------------------
enum TIMER_COMMAND
{
    TIMER_RESET,
    TIMER_START,
    TIMER_STOP,
    TIMER_ADVANCE,
    TIMER_GETABSOLUTETIME,
    TIMER_GETAPPTIME,
    TIMER_GETELAPSEDTIME
};

FLOAT __stdcall DXUtil_Timer(TIMER_COMMAND command);


//-----------------------------------------------------------------------------
// String conversion
//-----------------------------------------------------------------------------
HRESULT DXUtil_ConvertAnsiStringToWideCch(
    WCHAR *wstrDestination,
    const CHAR *strSource,
    int cchDestChar
);

HRESULT DXUtil_ConvertWideStringToAnsiCch(
    CHAR *strDestination,
    const WCHAR *wstrSource,
    int cchDestChar
);

HRESULT DXUtil_ConvertGenericStringToAnsiCch(
    CHAR *strDestination,
    const TCHAR *tstrSource,
    int cchDestChar
);

HRESULT DXUtil_ConvertGenericStringToWideCch(
    WCHAR *wstrDestination,
    const TCHAR *tstrSource,
    int cchDestChar
);

HRESULT DXUtil_ConvertAnsiStringToGenericCch(
    TCHAR *tstrDestination,
    const CHAR *strSource,
    int cchDestChar
);

HRESULT DXUtil_ConvertWideStringToGenericCch(
    TCHAR *tstrDestination,
    const WCHAR *wstrSource,
    int cchDestChar
);

HRESULT DXUtil_ConvertAnsiStringToWideCb(
    WCHAR *wstrDestination,
    const CHAR *strSource,
    int cbDestChar
);

HRESULT DXUtil_ConvertWideStringToAnsiCb(
    CHAR *strDestination,
    const WCHAR *wstrSource,
    int cbDestChar
);

HRESULT DXUtil_ConvertGenericStringToAnsiCb(
    CHAR *strDestination,
    const TCHAR *tstrSource,
    int cbDestChar
);

HRESULT DXUtil_ConvertGenericStringToWideCb(
    WCHAR *wstrDestination,
    const TCHAR *tstrSource,
    int cbDestChar
);

HRESULT DXUtil_ConvertAnsiStringToGenericCb(
    TCHAR *tstrDestination,
    const CHAR *strSource,
    int cbDestChar
);

HRESULT DXUtil_ConvertWideStringToGenericCb(
    TCHAR *tstrDestination,
    const WCHAR *wstrSource,
    int cbDestChar
);


//-----------------------------------------------------------------------------
// GUID
//-----------------------------------------------------------------------------
HRESULT DXUtil_ConvertGUIDToStringCch(
    const GUID *pGuidSrc,
    TCHAR *strDest,
    int cchDestChar
);

HRESULT DXUtil_ConvertGUIDToStringCb(
    const GUID *pGuidSrc,
    TCHAR *strDest,
    int cbDestChar
);

HRESULT DXUtil_ConvertStringToGUID(
    const TCHAR *strIn,
    GUID *pGuidOut
);


//-----------------------------------------------------------------------------
// Debug printing
//-----------------------------------------------------------------------------
VOID DXUtil_Trace(LPCTSTR strMsg, ...);

#if defined(DEBUG) || defined(_DEBUG)
#define DXTRACE DXUtil_Trace
#else
#define DXTRACE(...) ((void)0)
#endif


//-----------------------------------------------------------------------------
// CArrayList
//-----------------------------------------------------------------------------
enum ArrayListType
{
    AL_VALUE,
    AL_REFERENCE
};

class CArrayList
{
protected:
    ArrayListType   m_ArrayListType;
    void *m_pData;
    UINT            m_BytesPerEntry;
    UINT            m_NumEntries;
    UINT            m_NumEntriesAllocated;

public:
    CArrayList(
        ArrayListType Type,
        UINT BytesPerEntry = 0
    );

    ~CArrayList(void);

    HRESULT Add(void *pEntry);

    void Remove(UINT Entry);

    void *GetPtr(UINT Entry);

    UINT Count(void)
    {
        return m_NumEntries;
    }

    bool Contains(void *pEntryData);

    void Clear(void)
    {
        m_NumEntries = 0;
    }
};


//-----------------------------------------------------------------------------
// WinCE
//-----------------------------------------------------------------------------
#ifdef UNDER_CE

#define CheckDlgButton(hdialog, id, state) \
    ::SendMessage(::GetDlgItem(hdialog, id), BM_SETCHECK, state, 0)

#define IsDlgButtonChecked(hdialog, id) \
    ::SendMessage(::GetDlgItem(hdialog, id), BM_GETCHECK, 0L, 0L)

#define GETTIMESTAMP GetTickCount
#define _TWINCE(x) _T(x)

__inline int GetScrollPos(HWND hWnd, int nBar)
{
    SCROLLINFO si;

    memset(&si, 0, sizeof(si));
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;

    if (!GetScrollInfo(hWnd, nBar, &si))
        return 0;

    return si.nPos;
}

#else

#define GETTIMESTAMP timeGetTime
#define _TWINCE(x) x

#endif // UNDER_CE

#endif // DXUTIL_H