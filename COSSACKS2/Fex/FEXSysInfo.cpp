//#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>
#include <stdio.h>
#include "Include\\dmusici.h"
#define COMPILE_MULTIMON_STUBS
#include "Include\\multimon.h"
#include "Include\\basetsd.h"
#include "FEXModule.h"

extern char FEX_SystemVersionString[1024];

typedef HRESULT(WINAPI * DIRECTDRAWCREATE)( GUID*, LPDIRECTDRAW*, IUnknown* );
typedef HRESULT(WINAPI * DIRECTDRAWCREATEEX)( GUID*, VOID**, REFIID, IUnknown* );
typedef HRESULT(WINAPI * DIRECTINPUTCREATE)( HINSTANCE, DWORD, LPDIRECTINPUT*,
                                             IUnknown* );
//-----------------------------------------------------------------------------
// Name: FEX_GetDXVersion()
// Desc: This function returns the DirectX version number as follows:
//          0x0000 = No DirectX installed
//          0x0100 = DirectX version 1 installed
//          0x0200 = DirectX 2 installed
//          0x0300 = DirectX 3 installed
//          0x0500 = At least DirectX 5 installed.
//          0x0600 = At least DirectX 6 installed.
//          0x0601 = At least DirectX 6.1 installed.
//          0x0700 = At least DirectX 7 installed.
//          0x0800 = At least DirectX 8 installed.
// 
//       Please note that this code is intended as a general guideline. Your
//       app will probably be able to simply query for functionality (via
//       QueryInterface) for one or two components.
//
//       Please also note:
//          "if( dwDXVersion != 0x500 ) return FALSE;" is VERY BAD. 
//          "if( dwDXVersion <  0x500 ) return FALSE;" is MUCH BETTER.
//       to ensure your app will run on future releases of DirectX.
//-----------------------------------------------------------------------------
DWORD FEX_GetDXVersion()
{
   DIRECTDRAWCREATE     DirectDrawCreate   = NULL;
   DIRECTDRAWCREATEEX   DirectDrawCreateEx = NULL;
   DIRECTINPUTCREATE    DirectInputCreate  = NULL;
   HINSTANCE            hDDrawDLL          = NULL;
   HINSTANCE            hDInputDLL         = NULL;
   HINSTANCE            hD3D8DLL           = NULL;
   HINSTANCE            hDPNHPASTDLL       = NULL;
   LPDIRECTDRAW         pDDraw             = NULL;
   LPDIRECTDRAW2        pDDraw2            = NULL;
   LPDIRECTDRAWSURFACE  pSurf              = NULL;
   LPDIRECTDRAWSURFACE3 pSurf3             = NULL;
   LPDIRECTDRAWSURFACE4 pSurf4             = NULL;
   DWORD                dwDXVersion        = 0;
   HRESULT              hr;

   // First see if DDRAW.DLL even exists.
   hDDrawDLL = LoadLibrary("DDRAW.DLL");
   if(hDDrawDLL == NULL)
   {
		dwDXVersion = 0;
      OutputDebugString("Couldn't LoadLibrary DDraw\r\n");
      return dwDXVersion;
   }

   // See if we can create the DirectDraw object.
   DirectDrawCreate = (DIRECTDRAWCREATE)GetProcAddress(hDDrawDLL, "DirectDrawCreate");
   if(DirectDrawCreate == NULL)
   {
		dwDXVersion = 0;
      FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't GetProcAddress DirectDrawCreate\r\n");
      return dwDXVersion;
   }

   hr = DirectDrawCreate(NULL, &pDDraw, NULL);
   if(FAILED(hr))
   {
      dwDXVersion = 0;
      FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't create DDraw\r\n");
      return dwDXVersion;
   }

   // So DirectDraw exists.  We are at least DX1.
   dwDXVersion = 0x100;

   // Let's see if IID_IDirectDraw2 exists.
   hr = pDDraw->QueryInterface(IID_IDirectDraw2, (VOID**)&pDDraw2);
   if(FAILED(hr))
   {
      // No IDirectDraw2 exists... must be DX1
      pDDraw->Release();
      FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't QI DDraw2\r\n");
      return dwDXVersion;
   }

	// IDirectDraw2 exists. We must be at least DX2
   pDDraw2->Release();
   dwDXVersion = 0x200;

	//-------------------------------------------------------------------------
   // DirectX 3.0 Checks
	//-------------------------------------------------------------------------

   // DirectInput was added for DX3
   hDInputDLL = LoadLibrary("DINPUT.DLL");
   if(hDInputDLL == NULL)
   {
		// No DInput... must not be DX3
      pDDraw->Release();
      FreeLibrary( hDDrawDLL );
      OutputDebugString("Couldn't LoadLibrary DInput\r\n");
      return dwDXVersion;
   }

   DirectInputCreate = (DIRECTINPUTCREATE)GetProcAddress(hDInputDLL,
                                                        "DirectInputCreateA");
   if(DirectInputCreate == NULL)
   {
		// No DInput... must be DX2
      FreeLibrary(hDInputDLL);
      pDDraw->Release();
      FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't GetProcAddress DInputCreate\r\n");
      return dwDXVersion;
   }

   // DirectInputCreate exists. We are at least DX3
   dwDXVersion = 0x300;
   FreeLibrary(hDInputDLL);

   // Can do checks for 3a vs 3b here

	//-------------------------------------------------------------------------
   // DirectX 5.0 Checks
	//-------------------------------------------------------------------------

   // We can tell if DX5 is present by checking for the existence of
   // IDirectDrawSurface3. First, we need a surface to QI off of.
   DDSURFACEDESC ddsd;
   ZeroMemory( &ddsd, sizeof(ddsd) );
   ddsd.dwSize         = sizeof(ddsd);
   ddsd.dwFlags        = DDSD_CAPS;
   ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

   hr = pDDraw->SetCooperativeLevel(NULL, DDSCL_NORMAL);
   if(FAILED(hr))
   {
		// Failure. This means DDraw isn't properly installed.
      pDDraw->Release();
      FreeLibrary(hDDrawDLL);
      dwDXVersion = 0;
      OutputDebugString("Couldn't Set coop level\r\n");
      return dwDXVersion;
   }

   hr = pDDraw->CreateSurface(&ddsd, &pSurf, NULL);
   if(FAILED(hr))
   {
		// Failure. This means DDraw isn't properly installed.
      pDDraw->Release();
      FreeLibrary(hDDrawDLL);
      dwDXVersion = 0;
      OutputDebugString("Couldn't CreateSurface\r\n");
      return dwDXVersion;
   }

   // Query for the IDirectDrawSurface3 interface
   if(FAILED(pSurf->QueryInterface(IID_IDirectDrawSurface3,
                                   (VOID**)&pSurf3)))
   {
		pSurf->Release();
      pDDraw->Release();
      FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't QI DDS3\r\n");
      return dwDXVersion;
   }

   // QI for IDirectDrawSurface3 succeeded. We must be at least DX5
   dwDXVersion = 0x500;
   pSurf3->Release();

	//-------------------------------------------------------------------------
   // DirectX 6.0 Checks
	//-------------------------------------------------------------------------

   // The IDirectDrawSurface4 interface was introduced with DX 6.0
   if(FAILED(pSurf->QueryInterface(IID_IDirectDrawSurface4,
                                   (VOID**)&pSurf4)))
   {
		pSurf->Release();
      pDDraw->Release();
      FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't QI DDS4\r\n");
      return dwDXVersion;
   }

   // IDirectDrawSurface4 was create successfully. We must be at least DX6
   dwDXVersion = 0x600;
   pSurf4->Release();
   pSurf->Release();
   pDDraw->Release();

	//-------------------------------------------------------------------------
   // DirectX 6.1 Checks
	//-------------------------------------------------------------------------

   // Check for DMusic, which was introduced with DX6.1
   LPDIRECTMUSIC pDMusic = NULL;
   CoInitialize(NULL);
   hr = CoCreateInstance(CLSID_DirectMusic, NULL, CLSCTX_INPROC_SERVER,
                         IID_IDirectMusic, (VOID**)&pDMusic);
   if(FAILED(hr))
   {
		FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't create CLSID_DirectMusic\r\n");
      return dwDXVersion;
   }

   // DirectMusic was created successfully. We must be at least DX6.1
   dwDXVersion = 0x601;
   pDMusic->Release();
   CoUninitialize();

	//-------------------------------------------------------------------------
   // DirectX 7.0 Checks
	//-------------------------------------------------------------------------

   // Check for DirectX 7 by creating a DDraw7 object
   LPDIRECTDRAW7 pDD7;
   DirectDrawCreateEx = (DIRECTDRAWCREATEEX)GetProcAddress(hDDrawDLL,
																			  "DirectDrawCreateEx");
   if(NULL == DirectDrawCreateEx)
   {
		FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't GetProcAddress DirectDrawCreateEx\r\n");
      return dwDXVersion;
   }

   if(FAILED(DirectDrawCreateEx(NULL, (VOID**)&pDD7, IID_IDirectDraw7, NULL)))
   {
		FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't DirectDrawCreateEx\r\n");
      return dwDXVersion;
   }

   // DDraw7 was created successfully. We must be at least DX7.0
   dwDXVersion = 0x700;
   pDD7->Release();

	//-------------------------------------------------------------------------
   // DirectX 8.0 Checks
	//-------------------------------------------------------------------------

   // Simply see if D3D8.dll exists.
   hD3D8DLL = LoadLibrary("D3D8.DLL");
   if(hD3D8DLL == NULL)
   {
		FreeLibrary(hDDrawDLL);
      OutputDebugString("Couldn't LoadLibrary D3D8.DLL\r\n");
      return dwDXVersion;
   }

   // D3D8.dll exists. We must be at least DX8.0
   dwDXVersion = 0x800;

	//-------------------------------------------------------------------------
   // DirectX 8.1 Checks
	//-------------------------------------------------------------------------

   // Simply see if dpnhpast.dll exists.
   hDPNHPASTDLL = LoadLibrary("dpnhpast.dll");
   if(hDPNHPASTDLL == NULL)
   {
		FreeLibrary(hDPNHPASTDLL);
      OutputDebugString("Couldn't LoadLibrary dpnhpast.dll\r\n");
      return dwDXVersion;
   }
	FreeLibrary(hDPNHPASTDLL);

   // dpnhpast.dll exists. We must be at least DX8.1
   dwDXVersion = 0x801;

	//-------------------------------------------------------------------------
   // End of checking for versions of DirectX 
	//-------------------------------------------------------------------------

   // Close open libraries and return
   FreeLibrary(hDDrawDLL);
   FreeLibrary(hD3D8DLL);
    
   return dwDXVersion;
}
//-----------------------------------------------------------------------------
bool FEX_GetFreeSpace(int DiskNum, __int64& i64TotalBytes, __int64& i64FreeBytes)
{
	char _disk[8];
	char* disk = _disk;
	disk[0] = 'a';
	disk[1] = ':';
	disk[2] = '\\';
	disk[3] = 0;

	if(DiskNum == -1)
		disk = NULL;
	else
		disk[0] += DiskNum;
	
	__int64 i64FreeBytesToCaller;
	GetDiskFreeSpaceEx(disk, (PULARGE_INTEGER)&i64FreeBytesToCaller,
									 (PULARGE_INTEGER)&i64TotalBytes,
									 (PULARGE_INTEGER)&i64FreeBytes);
	return true;
}
//-----------------------------------------------------------------------------
void FEX_ADD_STR(char* str, ...) 
{
	static char tmp[256];
	if(!str)	return;

	va_list args;
	va_start(args, str);
	vsprintf(tmp, str, args);
	va_end(args);
	strcat(FEX_SystemVersionString, tmp);
}
//-----------------------------------------------------------------------------
char* FEX_GetSystemVersion(void)
{
	#define BUFSIZE 80

	struct myOSVERSIONINFOEX {
		DWORD dwOSVersionInfoSize;
		DWORD dwMajorVersion;
		DWORD dwMinorVersion;
		DWORD dwBuildNumber;
		DWORD dwPlatformId;
		TCHAR szCSDVersion[ 128 ];
		WORD wServicePackMajor;
		WORD wServicePackMinor;
		WORD wSuiteMask;
		BYTE wProductType;
		BYTE wReserved;
	};
   OSVERSIONINFOEX osvi;

   BOOL bOsVersionInfoEx;

	FEX_SystemVersionString[0] = 0;
   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)))
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if(!GetVersionEx((OSVERSIONINFO*)&osvi)) 
			return NULL;
   }

   switch(osvi.dwPlatformId)
   {
      // Tests for Windows NT product family.
      case VER_PLATFORM_WIN32_NT:

			// Test for the product.
         if(osvi.dwMajorVersion <= 4)
            FEX_ADD_STR("Microsoft Windows NT ");
			
         if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
            FEX_ADD_STR("Microsoft Windows 2000 ");

         if(bOsVersionInfoEx)  // Use information from GetVersionEx.
         { 
				// Test for the workstation type.
            if((*(myOSVERSIONINFOEX*)&osvi).wProductType == 0x0000001)//VER_NT_WORKSTATION)
            {
               if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
                  FEX_ADD_STR("Microsoft Windows XP ");

               if((*(myOSVERSIONINFOEX*)&osvi).wSuiteMask & 0x00000200)//VER_SUITE_PERSONAL)
                  FEX_ADD_STR("Home Edition ");
               else
                  FEX_ADD_STR("Professional ");
            }
	         // Test for the server type.
            else 
				if((*(myOSVERSIONINFOEX*)&osvi).wProductType == 0x0000003)//VER_NT_SERVER)
            {
               if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
                  FEX_ADD_STR("Microsoft Windows .NET ");

               if((*(myOSVERSIONINFOEX*)&osvi).wSuiteMask & 0x00000080)//VER_SUITE_DATACENTER)
                  FEX_ADD_STR("DataCenter Server ");
               else 
					{
						if((*(myOSVERSIONINFOEX*)&osvi).wSuiteMask & 0x00000002)//VER_SUITE_ENTERPRISE)
						{
							if(osvi.dwMajorVersion == 4)
								FEX_ADD_STR("Advanced Server ");
							else
								FEX_ADD_STR("Enterprise Server ");
						}
	               else 
						{
							if((*(myOSVERSIONINFOEX*)&osvi).wSuiteMask == 0x00000400)//VER_SUITE_BLADE)
								FEX_ADD_STR("Web Server ");
							else
								FEX_ADD_STR("Server ");
						}
					}
            }
         }
         else   // Use the registry on early versions of Windows NT.
         {
            HKEY hKey;
            char szProductType[BUFSIZE];
            DWORD dwBufLen=BUFSIZE;
            LONG lRet;

            lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
               "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
               0, KEY_QUERY_VALUE, &hKey);
            if(lRet != ERROR_SUCCESS)
                return NULL;

            lRet = RegQueryValueEx(hKey, "ProductType", NULL, NULL,
               (LPBYTE) szProductType, &dwBufLen);
            if((lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE))
                return NULL;

            RegCloseKey(hKey);

            if(lstrcmpi("WINNT", szProductType) == 0)
               FEX_ADD_STR("Professional ");
            if(lstrcmpi("LANMANNT", szProductType) == 0)
               FEX_ADD_STR("Server ");
            if(lstrcmpi("SERVERNT", szProductType) == 0)
               FEX_ADD_STR("Advanced Server ");
         }

			// Display version, service pack (if any), and build number.
         if(osvi.dwMajorVersion <= 4)
         {
            FEX_ADD_STR("version %d.%d %s (Build %d)",
               osvi.dwMajorVersion,
               osvi.dwMinorVersion,
               osvi.szCSDVersion,
               osvi.dwBuildNumber & 0xFFFF);
         }
         else
         { 
            FEX_ADD_STR("%s (Build %d)",
               osvi.szCSDVersion,
               osvi.dwBuildNumber & 0xFFFF);
         }
         break;
      // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:
         if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
         {
             FEX_ADD_STR("Microsoft Windows 95 ");
             if(osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
                FEX_ADD_STR("OSR2 ");
         } 

         if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
         {
             FEX_ADD_STR("Microsoft Windows 98 ");
             if(osvi.szCSDVersion[1] == 'A')
                FEX_ADD_STR("SE ");
         } 

         if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
         {
             FEX_ADD_STR("Microsoft Windows Millennium Edition ");
         } 
         break;
   }
   return FEX_SystemVersionString;
}
//-----------------------------------------------------------------------------
char* FEX_GetDisplayName(void)
{
	struct DISPLAY_DEVICE_FULL
	{
		DWORD  cb;
		TCHAR  DeviceName[32];
		TCHAR  DeviceString[128];
		DWORD  StateFlags;
		TCHAR  DeviceID[128];
		TCHAR  DeviceKey[128];
	};
	DISPLAY_DEVICE_FULL dd;
   ZeroMemory(&dd, sizeof(dd));
   dd.cb = sizeof(dd);
	static char str[1024];
	str[0] = 0;

   HINSTANCE hUser32 = LoadLibrary("user32.dll");
   if(hUser32 == NULL)
   {
      OutputDebugString("Couldn't LoadLibrary user32.dll\r\n");
      return str;
   }

	typedef BOOL (WINAPI *EnumDisplayDevicesFn)(
		LPCTSTR lpDevice,               
		DWORD iDevNum,                  
		PDISPLAY_DEVICE lpDisplayDevice,
		DWORD dwFlags);

	EnumDisplayDevicesFn g_pfnEnumDisplayDevices;

	g_pfnEnumDisplayDevices = (EnumDisplayDevicesFn)GetProcAddress(hUser32, 
																					"EnumDisplayDevicesA");

	if(!g_pfnEnumDisplayDevices)
   {
      OutputDebugString("Couldn't GetProcAddress of EnumDisplayDevicesA\r\n");
      return str;
   }

   for(int i = 0; g_pfnEnumDisplayDevices(NULL, i, (DISPLAY_DEVICE*)&dd, 0); i++)
   {
		// Ignore NetMeeting's mirrored displays
      if((dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) == 0)
      {
			strcat(str, dd.DeviceString);
			strcat(str, "\n");
		}
   }
   FreeLibrary(hUser32);
	return str;
}
//-----------------------------------------------------------------------------
// Winmm.lib is required!
//-----------------------------------------------------------------------------
char* FEX_GetAudioName(void)
{
	WAVEINCAPS wic;
	static char str[(MAXPNAMELEN+1)*16+1] = "";
	for(int i = 0; i < 16; i++)
	{
		ZeroMemory(&wic, sizeof(WAVEINCAPS));
		if(waveInGetDevCaps(i, &wic, sizeof(WAVEINCAPS)) != MMSYSERR_NOERROR) break;
		strcat(str, wic.szPname);
		strcat(str, "\n");
	}
	return str;
}
//-----------------------------------------------------------------------------
