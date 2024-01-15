#ifdef NICEVOPYL_EXPORTS
#define NICEVOPYL_API __declspec(dllexport)
#else
#define NICEVOPYL_API __declspec(dllimport)
#endif

#pragma once

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <mmsystem.h>
#include <dsound.h>

#include <list>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

#include "include\codec.h"
#include "include\vorbisfile.h"

typedef unsigned short VPL_STATUS;
typedef double VPL_POS;

#define VPL_CHUNK_SIZE 4096
#define VPL_DEFAULT_ENTRIES 128

#define	VPL_CLASS_NAME	"VplPhClass"
#define	VPL_WINDOW_NAME "VplPhWindow"

#if(WINVER >= 0x0500)
#define PHANTOM_WINDOW HWND_MESSAGE
#else /* WINVER >= 0x0500 */
#define PHANTOM_WINDOW ((HWND)-3)
#endif /* WINVER >= 0x0500 */

#define COLLECT_STATS

#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)

#define DX_CHECK(LINE,expr) if((dx_stat=expr)!=DS_OK)MessageBox(NULL,QUOTE(LINE),QUOTE(dx_stat),MB_ICONERROR | MB_OK)

#define VPL_OK				1
#define VPL_OK_EOF			2
#define VPL_ERR_NO_INI		3
#define VPL_ERR_NO_FILE		4
#define VPL_ERR_BAD_FILE	5
#define VPL_ERR_UNKNOWN		255

