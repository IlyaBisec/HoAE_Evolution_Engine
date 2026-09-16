#define _CRT_SECURE_NO_WARNINGS

// ilya_sadochkov 15/11/2024
// TVPlugin - updating the plugin for reading vorbis, 
// theora & ogg formats for new systems

#ifndef _TV_PLUGIN_H_
#define _TV_PLUGIN_H_

#include <windows.h>
#include <d3d9.h>
#include "dsound.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dsound.lib")

#include <theoradec.h>
#include <vorbisfile.h>
#include <ogg.h>

#ifdef TVPLUGIN_EXPORTS
#define TVPLUGIN_API __declspec(dllexport)
#else
#define TVPLUGIN_API __declspec(dllimport)
#endif TVPLUGIN_API 

// Macros for resource free
#define SAFE_RELEASE(p) if (p) { (p)->Release(); (p) = NULL; }
#define SAFE_FREE(p) if (p) { free(p); (p) = NULL; }

#define TV_VOLUME_MIN 0
#define TV_VOLUME_MAX 100
#define TV_DSBVOLUME_MIN -10000
#define TV_DSBVOLUME_MAX 0

#define TV_COLOR_FORMAT_RGB 1
#define TV_COLOR_FORMAT_ARGB 2
#define TV_COLOR_FORMAT_RGBA 3

typedef unsigned char TV_VOLUME;
typedef char TV_PANNING;

// Func status
typedef enum TV_STATUS
{
	TV_OK,
	TV_FAIL,
	TV_EOF
} TV_STATUS;

// Structures audio & video contexts
typedef struct TV_HANDLE
{
	// File descryptors
	FILE *file;
	OggVorbis_File vorbisFile;

	ogg_sync_state oggSyncState;            // Input buffer control
	ogg_page oggPage;                       // 1 Page store
	ogg_stream_state theoraStreamState;     // Stream state
	ogg_stream_state vorbisStreamState;
	
	// Decoders
	th_dec_ctx *theoraDecoder;      // Pointer to theora decorder
	th_setup_info *theoraSetup;     // Pointer to setting of theora decorder for init
	th_ycbcr_buffer ycbcrBuffer;

	// DirectSound
	void *internalHandle;

	// Parameters
	LONG volume;
	BOOL isLooped;
} TV_HANDLE;

// Type of
typedef TV_HANDLE *TV_AH;
typedef TV_HANDLE *TV_VH;


// ---------------------
// INIT AND DONE
// ---------------------
	// Initializes TVPlugin library, allocates memory, creates helper threads, DirectSound buffers, etc
extern "C"	__declspec(dllexport) TVPLUGIN_API int tv_Init(HWND hWnd);

	// Closes TVPlugin library, frees memory, destroys helper threads, DirectSound buffers, etc
extern "C"	__declspec(dllexport) TVPLUGIN_API int tv_Done();

// ---------------------
//		   AUDIO
// ---------------------

	// Opens sound for playing
	// The handle returned can be used to pause and stop sound, to set volume and panning.
	// Resources are not closed automatically, consider using tv_StreamClose() function
extern "C"	__declspec(dllexport) TVPLUGIN_API TV_AH tv_StreamOpen(const char *pcszFileName);

	// Returns length of the stream in seconds
extern "C"	__declspec(dllexport) TVPLUGIN_API int tv_StreamLength(TV_AH tv_ah_handle);

	// Starts playing stream
extern "C"	__declspec(dllexport) TVPLUGIN_API void tv_StreamPlay(TV_AH tv_ah_handle);

	// Stops sound started with tv_StreamOpen() and releases resources
extern "C"	__declspec(dllexport) TVPLUGIN_API void tv_StreamClose(TV_AH tv_ah_handle);

	// Returns TRUE when the stream is fineshed, otherwise FALSE
extern "C"	__declspec(dllexport) TVPLUGIN_API BYTE tv_StreamFinished(TV_AH tv_ah_handle);

	// Sets volume [0..100] for the given stream
extern "C"	__declspec(dllexport) TVPLUGIN_API void tv_StreamSetVolume(TV_AH tv_ah_handle, TV_VOLUME tv_volume);

// ---------------------
//		   VIDEO
// ---------------------

	// Opens video file and returns handle
extern "C"	__declspec(dllexport) TVPLUGIN_API TV_VH tv_VideoOpen(const char *pcszFileName);

	// Closes video handle and releases resources
extern "C"	__declspec(dllexport) TVPLUGIN_API int tv_VideoClose(TV_VH tv_vh_handle);

	// Returns TRUE when the stream is fineshed, otherwise FALSE
extern "C"	__declspec(dllexport) TVPLUGIN_API int tv_VideoFinished(TV_VH tv_vh_handle);

	// Plays video opened with tv_VideoOpen()
	// pRect = NULL for full screen video
extern "C"	__declspec(dllexport) TVPLUGIN_API void tv_VideoPlay(TV_VH tv_vh_handle, BOOL bLooped, RECT *pRect);

	// Resizes video overlay window played with tv_VideoPlay()
	// pRect = NULL for full screen video
extern "C"	__declspec(dllexport) TVPLUGIN_API void tv_VideoResize(TV_VH tv_vh_handle, RECT *pRect);


#endif // _TV_PLUGIN_H_
