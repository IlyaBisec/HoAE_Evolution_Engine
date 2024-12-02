//#ifdef THEOVORB_EXPORTS
//#define THEOVORB_API __declspec(dllexport)
//#else
//#define THEOVORB_API __declspec(dllimport)
//#endif
//
//#include <windows.h>
//#include <stdio.h>
//
//#define TV_FAIL 0
//#define TV_OK 1
//#define TV_EOF 2
//
//#define TV_VOLUME_MIN 0
//#define TV_VOLUME_MAX 100
//
//#define TV_COLOR_FORMAT_RGB 1
//#define TV_COLOR_FORMAT_ARGB 2
//#define TV_COLOR_FORMAT_RGBA 3
//
//typedef unsigned char TV_VOLUME;
//typedef char TV_PANNING;
//
//typedef void * TV_AH;
//typedef void * TV_VH;
//
//// Initializes theovorb library, allocates memory, creates helper threads, DirectSound buffers, etc
//THEOVORB_API int tv_Init();
//
//// Closes theovorb library, frees memory, destroys helper threads, DirectSound buffers, etc
//THEOVORB_API int tv_Done();
//
//// Opens sound for playing
//// The handle returned can be used to pause and stop sound, to set volume and panning.
//// Resources are not closed automatically, consider using tv_StreamClose() function
//THEOVORB_API TV_AH tv_StreamOpen( const char *pcszFileName );
//
//// Returns length of the stream in seconds
//THEOVORB_API int tv_StreamLength( TV_AH tv_ah_handle );
//
//// Starts playing stream
//THEOVORB_API void tv_StreamPlay( TV_AH tv_ah_handle );
//
//// Stops sound started with tv_StreamOpen() and releases resources
//THEOVORB_API void tv_StreamClose( TV_AH tv_ah_handle );
//
//// Returns TRUE when the stream is fineshed, otherwise FALSE
//THEOVORB_API BYTE tv_StreamFinished( TV_AH tv_ah_handle );
//
//// Sets volume [0..100] for the given stream
//THEOVORB_API void tv_StreamSetVolume( TV_AH tv_ah_handle , TV_VOLUME tv_volume );
//
//// Opens video file and returns handle
//THEOVORB_API TV_VH tv_VideoOpen( const char *pcszFileName );
//
//// Closes video handle and releases resources
//THEOVORB_API int tv_VideoClose( TV_VH tv_vh_handle );
//
//// Returns TRUE when the stream is fineshed, otherwise FALSE
//THEOVORB_API int tv_VideoFinished( TV_VH tv_vh_handle );
//
//// Plays video opened with tv_VideoOpen()
//// pRect = NULL for full screen video
//THEOVORB_API void tv_VideoPlay( TV_VH tv_vh_handle , BOOL bLooped , RECT *pRect );
//
//// Resizes video overlay window played with tv_VideoPlay()
//// pRect = NULL for full screen video
//THEOVORB_API void tv_VideoResize( TV_VH tv_vh_handle , RECT *pRect );
