//-----------------------------------------------------------------------------
// File: DXUtil.cpp
//
// Desc: Shortcut macros and functions for using DX objects
//
// Copyright (c) Microsoft Corporation. All rights reserved
//-----------------------------------------------------------------------------

#include "gRenderPch.h"
#include "DXUtil.h"

#ifdef UNICODE
typedef HINSTANCE(WINAPI *LPShellExecute)(
    HWND hwnd,
    LPCWSTR lpOperation,
    LPCWSTR lpFile,
    LPCWSTR lpParameters,
    LPCWSTR lpDirectory,
    INT nShowCmd
    );
#else
typedef HINSTANCE(WINAPI *LPShellExecute)(
    HWND hwnd,
    LPCSTR lpOperation,
    LPCSTR lpFile,
    LPCSTR lpParameters,
    LPCSTR lpDirectory,
    INT nShowCmd
    );
#endif


#ifndef UNDER_CE

//-----------------------------------------------------------------------------
// DXUtil_GetDXSDKMediaPathCch
//-----------------------------------------------------------------------------
HRESULT DXUtil_GetDXSDKMediaPathCch(
    TCHAR *strDest,
    int cchDest
)
{
    if (strDest == NULL || cchDest < 1)
        return E_INVALIDARG;

    strDest[0] = 0;

    HKEY hKey = NULL;

    LONG lResult = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        _T("Software\\Microsoft\\DirectX SDK"),
        0,
        KEY_READ,
        &hKey
    );

    if (lResult != ERROR_SUCCESS)
        return E_FAIL;

    DWORD dwType = 0;
    DWORD dwSize = (DWORD)(cchDest * sizeof(TCHAR));

    lResult = RegQueryValueEx(
        hKey,
        _T("DX9J3SDK Samples Path"),
        NULL,
        &dwType,
        (BYTE *)strDest,
        &dwSize
    );

    strDest[cchDest - 1] = 0;

    RegCloseKey(hKey);

    if (lResult != ERROR_SUCCESS)
        return E_FAIL;

    if (dwType != REG_SZ && dwType != REG_EXPAND_SZ)
        return E_FAIL;

    const TCHAR *strMedia = _T("\\Media\\");

    if (lstrlen(strDest) + lstrlen(strMedia) + 1 >= cchDest)
        return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);

    _tcscat(strDest, strMedia);

    return S_OK;
}


//-----------------------------------------------------------------------------
// DXUtil_FindMediaFileCch
//-----------------------------------------------------------------------------
HRESULT DXUtil_FindMediaFileCch(
    TCHAR *strDestPath,
    int cchDest,
    LPCTSTR strFilename
)
{
    if (strFilename == NULL ||
        strDestPath == NULL ||
        cchDest < 1)
    {
        return E_INVALIDARG;
    }

    strDestPath[0] = 0;

    TCHAR strLeafName[MAX_PATH];
    TCHAR strExePath[MAX_PATH];
    TCHAR strExeName[MAX_PATH];
    TCHAR strMediaDir[MAX_PATH];
    TCHAR strSearchPath[MAX_PATH];

    TCHAR *strLeafNameTmp = NULL;
    TCHAR *strLastSlash = NULL;

    DWORD dwAttributes;

    strLeafName[0] = 0;
    strExePath[0] = 0;
    strExeName[0] = 0;
    strMediaDir[0] = 0;
    strSearchPath[0] = 0;

    DWORD cchPath = GetFullPathName(
        strFilename,
        MAX_PATH,
        strSearchPath,
        &strLeafNameTmp
    );

    if (cchPath == 0 || cchPath >= MAX_PATH)
        return E_FAIL;

    if (strLeafNameTmp != NULL)
    {
        lstrcpyn(
            strLeafName,
            strLeafNameTmp,
            MAX_PATH
        );
    }

    // Search current directory.
    dwAttributes = GetFileAttributes(strSearchPath);

    if (dwAttributes != INVALID_FILE_ATTRIBUTES)
    {
        lstrcpyn(strDestPath, strSearchPath, cchDest);
        return S_OK;
    }

    // Search ..\
    
     _sntprintf(
    strSearchPath,
        MAX_PATH,
        TEXT("..\\%s"),
        strLeafName
        );

        strSearchPath[MAX_PATH - 1] = 0;

        if (GetFileAttributes(strSearchPath) != INVALID_FILE_ATTRIBUTES)
        {
            lstrcpyn(strDestPath, strSearchPath, cchDest);
            return S_OK;
        }

        // Search ..\..\
        
        _sntprintf(
        strSearchPath,
            MAX_PATH,
            TEXT("..\\..\\%s"),
            strLeafName
            );

            strSearchPath[MAX_PATH - 1] = 0;

            if (GetFileAttributes(strSearchPath) != INVALID_FILE_ATTRIBUTES)
            {
                lstrcpyn(strDestPath, strSearchPath, cchDest);
                return S_OK;
            }

            // Get executable path.
            DWORD exeLen = GetModuleFileName(
                NULL,
                strExePath,
                MAX_PATH
            );

            if (exeLen > 0)
            {
                strExePath[MAX_PATH - 1] = 0;

                strLastSlash = _tcsrchr(
                    strExePath,
                    TEXT('\\')
                );

                if (strLastSlash != NULL)
                {
                    lstrcpyn(
                        strExeName,
                        strLastSlash + 1,
                        MAX_PATH
                    );

                    *strLastSlash = 0;

                    strLastSlash = _tcsrchr(
                        strExeName,
                        TEXT('.')
                    );

                    if (strLastSlash != NULL)
                        *strLastSlash = 0;
                }

                // Executable directory.
                _sntprintf(
                    strSearchPath,
                    MAX_PATH,
                    TEXT("%s\\%s"),
                    strExePath,
                    strLeafName
                );

                strSearchPath[MAX_PATH - 1] = 0;

                if (GetFileAttributes(strSearchPath) != INVALID_FILE_ATTRIBUTES)
                {
                    lstrcpyn(strDestPath, strSearchPath, cchDest);
                    return S_OK;
                }

                // EXE_DIR\..\

                _sntprintf(
                strSearchPath,
                    MAX_PATH,
                    TEXT("%s\\..\\%s"),
                    strExePath,
                    strLeafName
                    );

                    strSearchPath[MAX_PATH - 1] = 0;

                    if (GetFileAttributes(strSearchPath) != INVALID_FILE_ATTRIBUTES)
                    {
                        lstrcpyn(strDestPath, strSearchPath, cchDest);
                        return S_OK;
                    }

                    // EXE_DIR\..\..\

                    _sntprintf(
                    strSearchPath,
                        MAX_PATH,
                        TEXT("%s\\..\\..\\%s"),
                        strExePath,
                        strLeafName
                        );

                        strSearchPath[MAX_PATH - 1] = 0;

                        if (GetFileAttributes(strSearchPath) != INVALID_FILE_ATTRIBUTES)
                        {
                            lstrcpyn(strDestPath, strSearchPath, cchDest);
                            return S_OK;
                        }

                        // EXE_DIR\..\EXE_NAME\

                        _sntprintf(
                        strSearchPath,
                            MAX_PATH,
                            TEXT("%s\\..\\%s\\%s"),
                            strExePath,
                            strExeName,
                            strLeafName
                            );

                            strSearchPath[MAX_PATH - 1] = 0;

                            if (GetFileAttributes(strSearchPath) != INVALID_FILE_ATTRIBUTES)
                            {
                                lstrcpyn(strDestPath, strSearchPath, cchDest);
                                return S_OK;
                            }
            }

            // DirectX SDK media directory.
            HRESULT hr = DXUtil_GetDXSDKMediaPathCch(
                strMediaDir,
                MAX_PATH
            );

            if (SUCCEEDED(hr))
            {
                _sntprintf(
                    strSearchPath,
                    MAX_PATH,
                    TEXT("%s%s"),
                    strMediaDir,
                    strLeafName
                );

                strSearchPath[MAX_PATH - 1] = 0;

                if (GetFileAttributes(strSearchPath) != INVALID_FILE_ATTRIBUTES)
                {
                    lstrcpyn(
                        strDestPath,
                        strSearchPath,
                        cchDest
                    );

                    return S_OK;
                }
            }

            // Preserve original behavior: return the requested filename.
            lstrcpyn(
                strDestPath,
                strFilename,
                cchDest
            );

            return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
}

#endif // !UNDER_CE


//-----------------------------------------------------------------------------
// Registry string read
//-----------------------------------------------------------------------------
HRESULT DXUtil_ReadStringRegKeyCch(
    HKEY hKey,
    LPCTSTR strRegName,
    TCHAR *strDest,
    DWORD cchDest,
    LPCTSTR strDefault
)
{
    if (strDest == NULL || cchDest == 0)
        return E_INVALIDARG;

    strDest[0] = 0;

    DWORD dwType = 0;
    DWORD cbDest = cchDest * sizeof(TCHAR);

    LONG result = RegQueryValueEx(
        hKey,
        strRegName,
        0,
        &dwType,
        (BYTE *)strDest,
        &cbDest
    );

    if (result != ERROR_SUCCESS ||
        (dwType != REG_SZ && dwType != REG_EXPAND_SZ))
    {
        if (strDefault != NULL)
        {
            _tcsncpy(
                strDest,
                strDefault,
                cchDest
            );

            strDest[cchDest - 1] = 0;
        }

        return S_FALSE;
    }

    strDest[cchDest - 1] = 0;

    return S_OK;
}


//-----------------------------------------------------------------------------
// Registry string write
//-----------------------------------------------------------------------------
HRESULT DXUtil_WriteStringRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    LPCTSTR strValue
)
{
    if (strValue == NULL)
        return E_INVALIDARG;

    DWORD cbValue =
        ((DWORD)_tcslen(strValue) + 1) * sizeof(TCHAR);

    if (RegSetValueEx(
        hKey,
        strRegName,
        0,
        REG_SZ,
        (BYTE *)strValue,
        cbValue
    ) != ERROR_SUCCESS)
    {
        return E_FAIL;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Float read
//-----------------------------------------------------------------------------
HRESULT DXUtil_ReadFloatRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    FLOAT *pfDest,
    FLOAT fDefault
)
{
    if (pfDest == NULL)
        return E_INVALIDARG;

    TCHAR sz[256];
    TCHAR strDefault[256];

    _sntprintf(
        strDefault,
        256,
        TEXT("%f"),
        fDefault
    );

    strDefault[255] = 0;

    if (SUCCEEDED(
        DXUtil_ReadStringRegKeyCch(
            hKey,
            strRegName,
            sz,
            256,
            strDefault
        )
    ))
    {
        FLOAT fResult = 0.0f;

        if (_stscanf(sz, TEXT("%f"), &fResult) == 1)
        {
            *pfDest = fResult;
            return S_OK;
        }
    }

    *pfDest = fDefault;

    return S_FALSE;
}


//-----------------------------------------------------------------------------
// Float write
//-----------------------------------------------------------------------------
HRESULT DXUtil_WriteFloatRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    FLOAT fValue
)
{
    TCHAR strValue[256];

    _sntprintf(
        strValue,
        256,
        TEXT("%f"),
        fValue
    );

    strValue[255] = 0;

    return DXUtil_WriteStringRegKey(
        hKey,
        strRegName,
        strValue
    );
}


//-----------------------------------------------------------------------------
// DWORD read
//-----------------------------------------------------------------------------
HRESULT DXUtil_ReadIntRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    DWORD *pdwDest,
    DWORD dwDefault
)
{
    if (pdwDest == NULL)
        return E_INVALIDARG;

    DWORD dwType = 0;
    DWORD dwLength = sizeof(DWORD);

    if (RegQueryValueEx(
        hKey,
        strRegName,
        0,
        &dwType,
        (BYTE *)pdwDest,
        &dwLength
    ) != ERROR_SUCCESS ||
        dwType != REG_DWORD)
    {
        *pdwDest = dwDefault;
        return S_FALSE;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// DWORD write
//-----------------------------------------------------------------------------
HRESULT DXUtil_WriteIntRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    DWORD dwValue
)
{
    if (RegSetValueEx(
        hKey,
        strRegName,
        0,
        REG_DWORD,
        (BYTE *)&dwValue,
        sizeof(DWORD)
    ) != ERROR_SUCCESS)
    {
        return E_FAIL;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// BOOL read
//-----------------------------------------------------------------------------
HRESULT DXUtil_ReadBoolRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    BOOL *pbDest,
    BOOL bDefault
)
{
    if (pbDest == NULL)
        return E_INVALIDARG;

    DWORD dwType = 0;
    DWORD dwLength = sizeof(DWORD);

    DWORD dwValue = 0;

    if (RegQueryValueEx(
        hKey,
        strRegName,
        0,
        &dwType,
        (BYTE *)&dwValue,
        &dwLength
    ) != ERROR_SUCCESS ||
        dwType != REG_DWORD)
    {
        *pbDest = bDefault;
        return S_FALSE;
    }

    *pbDest = (dwValue != 0) ? TRUE : FALSE;

    return S_OK;
}


//-----------------------------------------------------------------------------
// BOOL write
//-----------------------------------------------------------------------------
HRESULT DXUtil_WriteBoolRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    BOOL bValue
)
{
    DWORD dwValue = bValue ? 1 : 0;

    if (RegSetValueEx(
        hKey,
        strRegName,
        0,
        REG_DWORD,
        (BYTE *)&dwValue,
        sizeof(DWORD)
    ) != ERROR_SUCCESS)
    {
        return E_FAIL;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// GUID read
//-----------------------------------------------------------------------------
HRESULT DXUtil_ReadGuidRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    GUID *pGuidDest,
    GUID &guidDefault
)
{
    if (pGuidDest == NULL)
        return E_INVALIDARG;

    DWORD dwType = 0;
    DWORD dwLength = sizeof(GUID);

    if (RegQueryValueEx(
        hKey,
        strRegName,
        0,
        &dwType,
        (LPBYTE)pGuidDest,
        &dwLength
    ) != ERROR_SUCCESS ||
        dwType != REG_BINARY ||
        dwLength != sizeof(GUID))
    {
        *pGuidDest = guidDefault;
        return S_FALSE;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// GUID write
//-----------------------------------------------------------------------------
HRESULT DXUtil_WriteGuidRegKey(
    HKEY hKey,
    LPCTSTR strRegName,
    GUID guidValue
)
{
    if (RegSetValueEx(
        hKey,
        strRegName,
        0,
        REG_BINARY,
        (BYTE *)&guidValue,
        sizeof(GUID)
    ) != ERROR_SUCCESS)
    {
        return E_FAIL;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Timer
//-----------------------------------------------------------------------------
FLOAT __stdcall DXUtil_Timer(TIMER_COMMAND command)
{
    static BOOL     m_bTimerInitialized = FALSE;
    static BOOL     m_bUsingQPF = FALSE;
    static BOOL     m_bTimerStopped = TRUE;
    static LONGLONG m_llQPFTicksPerSec = 0;

    if (!m_bTimerInitialized)
    {
        m_bTimerInitialized = TRUE;

        LARGE_INTEGER qwTicksPerSec;

        m_bUsingQPF =
            QueryPerformanceFrequency(&qwTicksPerSec);

        if (m_bUsingQPF)
            m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

    if (m_bUsingQPF)
    {
        static LONGLONG m_llStopTime = 0;
        static LONGLONG m_llLastElapsedTime = 0;
        static LONGLONG m_llBaseTime = 0;

        LARGE_INTEGER qwTime;

        if (m_llStopTime != 0 &&
            command != TIMER_START &&
            command != TIMER_GETABSOLUTETIME)
        {
            qwTime.QuadPart = m_llStopTime;
        }
        else
        {
            QueryPerformanceCounter(&qwTime);
        }

        if (command == TIMER_GETELAPSEDTIME)
        {
            double fElapsedTime =
                (double)(qwTime.QuadPart - m_llLastElapsedTime) /
                (double)m_llQPFTicksPerSec;

            m_llLastElapsedTime = qwTime.QuadPart;

            return (FLOAT)fElapsedTime;
        }

        if (command == TIMER_GETAPPTIME)
        {
            double fAppTime =
                (double)(qwTime.QuadPart - m_llBaseTime) /
                (double)m_llQPFTicksPerSec;

            return (FLOAT)fAppTime;
        }

        if (command == TIMER_RESET)
        {
            m_llBaseTime = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            m_llStopTime = 0;
            m_bTimerStopped = FALSE;

            return 0.0f;
        }

        if (command == TIMER_START)
        {
            if (m_bTimerStopped)
            {
                m_llBaseTime +=
                    qwTime.QuadPart - m_llStopTime;
            }

            m_llStopTime = 0;
            m_llLastElapsedTime = qwTime.QuadPart;
            m_bTimerStopped = FALSE;

            return 0.0f;
        }

        if (command == TIMER_STOP)
        {
            if (!m_bTimerStopped)
            {
                m_llStopTime = qwTime.QuadPart;
                m_llLastElapsedTime = qwTime.QuadPart;
                m_bTimerStopped = TRUE;
            }

            return 0.0f;
        }

        if (command == TIMER_ADVANCE)
        {
            m_llStopTime +=
                m_llQPFTicksPerSec / 10;

            return 0.0f;
        }

        if (command == TIMER_GETABSOLUTETIME)
        {
            double fTime =
                qwTime.QuadPart /
                (double)m_llQPFTicksPerSec;

            return (FLOAT)fTime;
        }

        return -1.0f;
    }
    else
    {
        static double m_fLastElapsedTime = 0.0;
        static double m_fBaseTime = 0.0;
        static double m_fStopTime = 0.0;

        double fTime;

        if (m_fStopTime != 0.0 &&
            command != TIMER_START &&
            command != TIMER_GETABSOLUTETIME)
        {
            fTime = m_fStopTime;
        }
        else
        {
            fTime = GETTIMESTAMP() * 0.001;
        }

        if (command == TIMER_GETELAPSEDTIME)
        {
            double fElapsedTime =
                fTime - m_fLastElapsedTime;

            m_fLastElapsedTime = fTime;

            return (FLOAT)fElapsedTime;
        }

        if (command == TIMER_GETAPPTIME)
        {
            return (FLOAT)(fTime - m_fBaseTime);
        }

        if (command == TIMER_RESET)
        {
            m_fBaseTime = fTime;
            m_fLastElapsedTime = fTime;
            m_fStopTime = 0;
            m_bTimerStopped = FALSE;

            return 0.0f;
        }

        if (command == TIMER_START)
        {
            if (m_bTimerStopped)
            {
                m_fBaseTime +=
                    fTime - m_fStopTime;
            }

            m_fStopTime = 0.0;
            m_fLastElapsedTime = fTime;
            m_bTimerStopped = FALSE;

            return 0.0f;
        }

        if (command == TIMER_STOP)
        {
            if (!m_bTimerStopped)
            {
                m_fStopTime = fTime;
                m_fLastElapsedTime = fTime;
                m_bTimerStopped = TRUE;
            }

            return 0.0f;
        }

        if (command == TIMER_ADVANCE)
        {
            m_fStopTime += 0.1;
            return 0.0f;
        }

        if (command == TIMER_GETABSOLUTETIME)
        {
            return (FLOAT)fTime;
        }

        return -1.0f;
    }
}


//-----------------------------------------------------------------------------
// String conversions
//-----------------------------------------------------------------------------
HRESULT DXUtil_ConvertAnsiStringToWideCch(
    WCHAR *wstrDestination,
    const CHAR *strSource,
    int cchDestChar
)
{
    if (wstrDestination == NULL ||
        strSource == NULL ||
        cchDestChar < 1)
    {
        return E_INVALIDARG;
    }

    int nResult = MultiByteToWideChar(
        CP_ACP,
        0,
        strSource,
        -1,
        wstrDestination,
        cchDestChar
    );

    wstrDestination[cchDestChar - 1] = 0;

    return (nResult == 0) ? E_FAIL : S_OK;
}


HRESULT DXUtil_ConvertWideStringToAnsiCch(
    CHAR *strDestination,
    const WCHAR *wstrSource,
    int cchDestChar
)
{
    if (strDestination == NULL ||
        wstrSource == NULL ||
        cchDestChar < 1)
    {
        return E_INVALIDARG;
    }

    int nResult = WideCharToMultiByte(
        CP_ACP,
        0,
        wstrSource,
        -1,
        strDestination,
        cchDestChar,
        NULL,
        NULL
    );

    strDestination[cchDestChar - 1] = 0;

    return (nResult == 0) ? E_FAIL : S_OK;
}


HRESULT DXUtil_ConvertGenericStringToAnsiCch(
    CHAR *strDestination,
    const TCHAR *tstrSource,
    int cchDestChar
)
{
    if (strDestination == NULL ||
        tstrSource == NULL ||
        cchDestChar < 1)
    {
        return E_INVALIDARG;
    }

#ifdef _UNICODE
    return DXUtil_ConvertWideStringToAnsiCch(
        strDestination,
        tstrSource,
        cchDestChar
    );
#else
    strncpy(
        strDestination,
        tstrSource,
        cchDestChar
    );

    strDestination[cchDestChar - 1] = '\0';

    return S_OK;
#endif
}


HRESULT DXUtil_ConvertGenericStringToWideCch(
    WCHAR *wstrDestination,
    const TCHAR *tstrSource,
    int cchDestChar
)
{
    if (wstrDestination == NULL ||
        tstrSource == NULL ||
        cchDestChar < 1)
    {
        return E_INVALIDARG;
    }

#ifdef _UNICODE
    wcsncpy(
        wstrDestination,
        tstrSource,
        cchDestChar
    );

    wstrDestination[cchDestChar - 1] = L'\0';

    return S_OK;
#else
    return DXUtil_ConvertAnsiStringToWideCch(
        wstrDestination,
        tstrSource,
        cchDestChar
    );
#endif
}


HRESULT DXUtil_ConvertAnsiStringToGenericCch(
    TCHAR *tstrDestination,
    const CHAR *strSource,
    int cchDestChar
)
{
    if (tstrDestination == NULL ||
        strSource == NULL ||
        cchDestChar < 1)
    {
        return E_INVALIDARG;
    }

#ifdef _UNICODE
    return DXUtil_ConvertAnsiStringToWideCch(
        tstrDestination,
        strSource,
        cchDestChar
    );
#else
    strncpy(
        tstrDestination,
        strSource,
        cchDestChar
    );

    tstrDestination[cchDestChar - 1] = '\0';

    return S_OK;
#endif
}


HRESULT DXUtil_ConvertWideStringToGenericCch(
    TCHAR *tstrDestination,
    const WCHAR *wstrSource,
    int cchDestChar
)
{
    if (tstrDestination == NULL ||
        wstrSource == NULL ||
        cchDestChar < 1)
    {
        return E_INVALIDARG;
    }

#ifdef _UNICODE
    wcsncpy(
        tstrDestination,
        wstrSource,
        cchDestChar
    );

    tstrDestination[cchDestChar - 1] = L'\0';

    return S_OK;
#else
    return DXUtil_ConvertWideStringToAnsiCch(
        tstrDestination,
        wstrSource,
        cchDestChar
    );
#endif
}


//-----------------------------------------------------------------------------
// Debug trace
//-----------------------------------------------------------------------------
VOID DXUtil_Trace(
    LPCTSTR strMsg,
    ...
)
{
#if defined(DEBUG) || defined(_DEBUG)

    if (strMsg == NULL)
        return;

    TCHAR strBuffer[512];

    va_list args;

    va_start(args, strMsg);

    _vsntprintf(
        strBuffer,
        512,
        strMsg,
        args
    );

    va_end(args);

    strBuffer[511] = 0;

    OutputDebugString(strBuffer);

#else

    UNREFERENCED_PARAMETER(strMsg);

#endif
}


//-----------------------------------------------------------------------------
// GUID conversion
//-----------------------------------------------------------------------------
HRESULT DXUtil_ConvertStringToGUID(
    const TCHAR *strSrc,
    GUID *pGuidDest
)
{
    if (strSrc == NULL || pGuidDest == NULL)
        return E_INVALIDARG;

    UINT aiTmp[10];

    if (_stscanf(
        strSrc,
        TEXT("{%8X-%4X-%4X-%2X%2X-%2X%2X%2X%2X%2X%2X}"),
        &pGuidDest->Data1,
        &aiTmp[0],
        &aiTmp[1],
        &aiTmp[2],
        &aiTmp[3],
        &aiTmp[4],
        &aiTmp[5],
        &aiTmp[6],
        &aiTmp[7],
        &aiTmp[8],
        &aiTmp[9]
    ) != 11)
    {
        ZeroMemory(
            pGuidDest,
            sizeof(GUID)
        );

        return E_FAIL;
    }

    pGuidDest->Data2 = (USHORT)aiTmp[0];
    pGuidDest->Data3 = (USHORT)aiTmp[1];

    pGuidDest->Data4[0] = (BYTE)aiTmp[2];
    pGuidDest->Data4[1] = (BYTE)aiTmp[3];
    pGuidDest->Data4[2] = (BYTE)aiTmp[4];
    pGuidDest->Data4[3] = (BYTE)aiTmp[5];
    pGuidDest->Data4[4] = (BYTE)aiTmp[6];
    pGuidDest->Data4[5] = (BYTE)aiTmp[7];
    pGuidDest->Data4[6] = (BYTE)aiTmp[8];
    pGuidDest->Data4[7] = (BYTE)aiTmp[9];

    return S_OK;
}


//-----------------------------------------------------------------------------
// GUID to string
//-----------------------------------------------------------------------------
HRESULT DXUtil_ConvertGUIDToStringCch(
    const GUID *pGuidSrc,
    TCHAR *strDest,
    int cchDestChar
)
{
    if (pGuidSrc == NULL ||
        strDest == NULL ||
        cchDestChar < 1)
    {
        return E_INVALIDARG;
    }

    int nResult = _sntprintf(
        strDest,
        cchDestChar,
        TEXT("{%0.8X-%0.4X-%0.4X-%0.2X%0.2X-%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X}"),
        pGuidSrc->Data1,
        pGuidSrc->Data2,
        pGuidSrc->Data3,
        pGuidSrc->Data4[0],
        pGuidSrc->Data4[1],
        pGuidSrc->Data4[2],
        pGuidSrc->Data4[3],
        pGuidSrc->Data4[4],
        pGuidSrc->Data4[5],
        pGuidSrc->Data4[6],
        pGuidSrc->Data4[7]
    );

    strDest[cchDestChar - 1] = 0;

    if (nResult < 0)
        return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);

    return S_OK;
}


//-----------------------------------------------------------------------------
// CArrayList constructor
//-----------------------------------------------------------------------------
CArrayList::CArrayList(
    ArrayListType Type,
    UINT BytesPerEntry
)
{
    if (Type == AL_REFERENCE)
        BytesPerEntry = sizeof(void *);

    m_ArrayListType = Type;
    m_pData = NULL;
    m_BytesPerEntry = BytesPerEntry;
    m_NumEntries = 0;
    m_NumEntriesAllocated = 0;
}


//-----------------------------------------------------------------------------
// CArrayList destructor
//-----------------------------------------------------------------------------
CArrayList::~CArrayList(void)
{
    SAFE_DELETE_ARRAY(m_pData);

    m_NumEntries = 0;
    m_NumEntriesAllocated = 0;
}


//-----------------------------------------------------------------------------
// CArrayList::Add
//-----------------------------------------------------------------------------
HRESULT CArrayList::Add(void *pEntry)
{
    if (m_BytesPerEntry == 0)
        return E_FAIL;

    if (pEntry == NULL)
        return E_INVALIDARG;

    if (m_pData == NULL ||
        m_NumEntries + 1 > m_NumEntriesAllocated)
    {
        UINT NumEntriesAllocatedNew;

        if (m_NumEntriesAllocated == 0)
            NumEntriesAllocatedNew = 16;
        else
            NumEntriesAllocatedNew =
            m_NumEntriesAllocated * 2;

        BYTE *pDataNew = new BYTE[
            NumEntriesAllocatedNew * m_BytesPerEntry
        ];

        if (pDataNew == NULL)
            return E_OUTOFMEMORY;

        if (m_pData != NULL)
        {
            CopyMemory(
                pDataNew,
                m_pData,
                m_NumEntries * m_BytesPerEntry
            );

            SAFE_DELETE_ARRAY(m_pData);
        }

        m_pData = pDataNew;
        m_NumEntriesAllocated =
            NumEntriesAllocatedNew;
    }

    if (m_ArrayListType == AL_VALUE)
    {
        CopyMemory(
            (BYTE *)m_pData +
            (m_NumEntries * m_BytesPerEntry),
            pEntry,
            m_BytesPerEntry
        );
    }
    else
    {
        *(((void **)m_pData) + m_NumEntries) =
            pEntry;
    }

    m_NumEntries++;

    return S_OK;
}


//-----------------------------------------------------------------------------
// CArrayList::Remove
//-----------------------------------------------------------------------------
void CArrayList::Remove(UINT Entry)
{
    if (Entry >= m_NumEntries)
        return;

    m_NumEntries--;

    BYTE *pData =
        (BYTE *)m_pData +
        (Entry * m_BytesPerEntry);

    UINT nMove =
        m_NumEntries - Entry;

    if (nMove > 0)
    {
        MoveMemory(
            pData,
            pData + m_BytesPerEntry,
            nMove * m_BytesPerEntry
        );
    }

    if (m_ArrayListType == AL_REFERENCE)
    {
        void **ppData = (void **)m_pData;

        ppData[m_NumEntries] = NULL;
    }
}


//-----------------------------------------------------------------------------
// CArrayList::GetPtr
//-----------------------------------------------------------------------------
void *CArrayList::GetPtr(UINT Entry)
{
    if (Entry >= m_NumEntries || m_pData == NULL)
        return NULL;

    if (m_ArrayListType == AL_VALUE)
    {
        return (BYTE *)m_pData +
            (Entry * m_BytesPerEntry);
    }

    return *(((void **)m_pData) + Entry);
}


//-----------------------------------------------------------------------------
// CArrayList::Contains
//-----------------------------------------------------------------------------
bool CArrayList::Contains(void *pEntryData)
{
    if (pEntryData == NULL)
        return false;

    for (UINT iEntry = 0;
        iEntry < m_NumEntries;
        iEntry++)
    {
        if (m_ArrayListType == AL_VALUE)
        {
            if (memcmp(
                GetPtr(iEntry),
                pEntryData,
                m_BytesPerEntry
            ) == 0)
            {
                return true;
            }
        }
        else
        {
            if (GetPtr(iEntry) == pEntryData)
                return true;
        }
    }

    return false;
}


//-----------------------------------------------------------------------------
// BYTE-sized wrappers
//-----------------------------------------------------------------------------
HRESULT DXUtil_ConvertAnsiStringToWideCb(
    WCHAR *wstrDestination,
    const CHAR *strSource,
    int cbDestChar
)
{
    if (cbDestChar <= 0)
        return E_INVALIDARG;

    return DXUtil_ConvertAnsiStringToWideCch(
        wstrDestination,
        strSource,
        cbDestChar / sizeof(WCHAR)
    );
}


HRESULT DXUtil_ConvertWideStringToAnsiCb(
    CHAR *strDestination,
    const WCHAR *wstrSource,
    int cbDestChar
)
{
    if (cbDestChar <= 0)
        return E_INVALIDARG;

    return DXUtil_ConvertWideStringToAnsiCch(
        strDestination,
        wstrSource,
        cbDestChar / sizeof(CHAR)
    );
}


HRESULT DXUtil_ConvertGenericStringToAnsiCb(
    CHAR *strDestination,
    const TCHAR *tstrSource,
    int cbDestChar
)
{
    if (cbDestChar <= 0)
        return E_INVALIDARG;

    return DXUtil_ConvertGenericStringToAnsiCch(
        strDestination,
        tstrSource,
        cbDestChar / sizeof(CHAR)
    );
}


HRESULT DXUtil_ConvertGenericStringToWideCb(
    WCHAR *wstrDestination,
    const TCHAR *tstrSource,
    int cbDestChar
)
{
    if (cbDestChar <= 0)
        return E_INVALIDARG;

    return DXUtil_ConvertGenericStringToWideCch(
        wstrDestination,
        tstrSource,
        cbDestChar / sizeof(WCHAR)
    );
}


HRESULT DXUtil_ConvertAnsiStringToGenericCb(
    TCHAR *tstrDestination,
    const CHAR *strSource,
    int cbDestChar
)
{
    if (cbDestChar <= 0)
        return E_INVALIDARG;

    return DXUtil_ConvertAnsiStringToGenericCch(
        tstrDestination,
        strSource,
        cbDestChar / sizeof(TCHAR)
    );
}


HRESULT DXUtil_ConvertWideStringToGenericCb(
    TCHAR *tstrDestination,
    const WCHAR *wstrSource,
    int cbDestChar
)
{
    if (cbDestChar <= 0)
        return E_INVALIDARG;

    return DXUtil_ConvertWideStringToGenericCch(
        tstrDestination,
        wstrSource,
        cbDestChar / sizeof(TCHAR)
    );
}


HRESULT DXUtil_ReadStringRegKeyCb(
    HKEY hKey,
    LPCTSTR strRegName,
    TCHAR *strDest,
    DWORD cbDest,
    LPCTSTR strDefault
)
{
    if (cbDest == 0)
        return E_INVALIDARG;

    return DXUtil_ReadStringRegKeyCch(
        hKey,
        strRegName,
        strDest,
        cbDest / sizeof(TCHAR),
        strDefault
    );
}


HRESULT DXUtil_ConvertGUIDToStringCb(
    const GUID *pGuidSrc,
    TCHAR *strDest,
    int cbDestChar
)
{
    if (cbDestChar <= 0)
        return E_INVALIDARG;

    return DXUtil_ConvertGUIDToStringCch(
        pGuidSrc,
        strDest,
        cbDestChar / sizeof(TCHAR)
    );
}


#ifndef UNDER_CE

HRESULT DXUtil_GetDXSDKMediaPathCb(
    TCHAR *szDest,
    int cbDest
)
{
    if (cbDest <= 0)
        return E_INVALIDARG;

    return DXUtil_GetDXSDKMediaPathCch(
        szDest,
        cbDest / sizeof(TCHAR)
    );
}


HRESULT DXUtil_FindMediaFileCb(
    TCHAR *szDestPath,
    int cbDest,
    LPCTSTR strFilename
)
{
    if (cbDest <= 0)
        return E_INVALIDARG;

    return DXUtil_FindMediaFileCch(
        szDestPath,
        cbDest / sizeof(TCHAR),
        strFilename
    );
}

#endif // !UNDER_CE