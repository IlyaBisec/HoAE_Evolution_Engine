#include <windows.h>
#include <ddraw.h>

#include "rad.h"
#include "bink.h"

static void Clear_to_black( HWND window )
{
  PAINTSTRUCT ps;
  HDC dc;

  //
  // Get the repaint DC and then fill the window with black.
  //

  dc = BeginPaint( window, &ps );

  PatBlt( dc, 0, 0, 4096, 4096, BLACKNESS );

  EndPaint( window, &ps );
}
static void Calc_window_values( HWND window,
                                S32* out_window_x,
                                S32* out_window_y,
                                S32* out_client_x,
                                S32* out_client_y,
                                S32* out_extra_width,
                                S32* out_extra_height )
{
  RECT r, c;
  POINT p;

  //
  // Get the position of the upper-left client coordinate (in screen space).
  //

  p.x = 0;
  p.y = 0;
  ClientToScreen( window, &p );

  *out_window_x = p.x;
  *out_window_y = p.y;

  //
  // Get the current window rect (in screen space).
  //

  GetWindowRect( window, &r );

  *out_client_x = p.x - r.left;
  *out_client_y = p.y - r.top;

  //
  // Get the client rectangle of the window.
  //

  GetClientRect( window, &c );

  *out_extra_width = ( r.right - r.left ) - ( c.right - c.left );
  *out_extra_height = ( r.bottom - r.top ) - ( c.bottom - c.top );
}
extern bool KeyPressed;
extern bool Lpressed;
int BinkT0=0;
bool MUSTEXIT=0;
bool Show_next_frame( HBINK bink,
                             HWND window,
                             LPDIRECTDRAWSURFACE surface,
                             DDSURFACEDESC* surface_description,
                             S32 surface_type,
                             S32 window_x,
                             S32 window_y,
                             S32 software_cursor )
{
	S32 count = 0;
	s32 res0=BinkDoFrame( bink );
	s32 res=BinkCopyToBuffer( bink,
                    surface_description->lpSurface,
                    surface_description->lPitch,
                    bink->Height,
                    window_x, window_y,
                    surface_type );
	if ( bink->FrameNum>= bink->Frames-2){
		return false;
		MUSTEXIT=1;
	}else{
		if(KeyPressed||Lpressed){
			MUSTEXIT=1;
		};
		KeyPressed=0;
		Lpressed=0;
		if(MUSTEXIT&&GetTickCount()-BinkT0>3000)return false;
		BinkNextFrame( bink );
		return true;
	};
	
}


//#########################y###################################################
//##                                                                        ##
//##  Good_sleep_us - sleeps for a specified number of MICROseconds.        ##
//##    The task switcher in Windows has a latency of 15 ms.  That means    ##
//##    you can ask for a Sleep of one millisecond and actually get a       ##
//##    sleep of 15 ms!  In normal applications, this is no big deal,       ##
//##    however, with a video player at 30 fps, 15 ms is almost half our    ##
//##    frame time!  The Good_sleep_us function times each sleep and keeps  ##
//##    the average sleep time to what you requested.  It also give more    ##
//##    accuracy than Sleep - Good_sleep_us() uses microseconds instead of  ##
//##    milliseconds.                                                       ##
//##                                                                        ##
//############################################################################

static void Good_sleep_us( S32 microseconds )
{
  static S32 total_sleep=0;
  static S32 slept_in_advance=0;
  static U64 frequency=1000;
  static S32 got_frequency=0;

  //
  // If this is the first time called, get the high-performance timer count.
  //

  if ( !got_frequency )
  {
    got_frequency = 1;
    QueryPerformanceFrequency( ( LARGE_INTEGER* )&frequency );
  }

  total_sleep += microseconds;

  //
  // Have we exceeded our reserve of slept microseconds?
  //

  if (( total_sleep - slept_in_advance ) > 1000)
  {
    U64 start, end;
    total_sleep -= slept_in_advance;

    //
    // Do the timed sleep.
    //

    QueryPerformanceCounter( ( LARGE_INTEGER* )&start );
    Sleep( total_sleep / 1000 );
    QueryPerformanceCounter( ( LARGE_INTEGER* )&end );

    //
    // Calculate delta time in microseconds.
    //

    end = ( (end - start) * (U64)1000000 ) / frequency;

    //
    // Keep track of how much extra we slept.
    //

    slept_in_advance = ( U32 )end - total_sleep;
    total_sleep %= 1000;
  }
}
extern HWND hwnd;
static HBINK Bink = 0;
static S32 Window_x, Window_y;
static S32 Client_offset_x, Client_offset_y;
extern LPDIRECTDRAWSURFACE     lpDDSPrimary;
extern DDSURFACEDESC           ddsd;
void ClearRGB();
void PlayBinkFile(char* path){
	if(MUSTEXIT)return;
	BinkT0=GetTickCount();
	if(!lpDDSPrimary)return;
	try{
		ClearRGB();
		HWND window=hwnd;
		S32 software_cursor;
		S32 extra_width, extra_height;
		HCURSOR cursor = 0;
		MSG msg;
		Calc_window_values( window,
						  &Window_x, &Window_y,
						  &Client_offset_x, &Client_offset_y,
						  &extra_width, &extra_height );
		software_cursor = BinkIsSoftwareCursor( lpDDSPrimary, cursor );
		s32 res=BinkSoundUseDirectSound( 0 );
		S32 surface_type=BinkDDSurfaceType(lpDDSPrimary);
		Bink = BinkOpen(path,BINKNOSKIP);
		if(Bink){
			for ( ; ; ){
				//
				// Are there any messages to handle?
				//
				if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) ){
					//
					// Yup, handle them.
					//
					if ( msg.message == WM_QUIT )
						break;
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}else{
					//
					// Is it time for a new Bink frame?
					//
					if ( !BinkWait( Bink ) ){
						//
						// Yup, draw the next frame.
						//
						if(!Show_next_frame( Bink,
								 window,
								 lpDDSPrimary,
								 &ddsd,
								 surface_type,
								 Window_x,
								 Window_y+(480-Bink->Height)/2,
								 software_cursor ))goto FINBINK;
					 }else{
						//
						// Nope, give the rest of the system a chance to run (500 MICROseconds).
						//
						Good_sleep_us( 500 );
					  }

					}
				}
				//
				// Close the Bink file.
				//
	FINBINK:
				if ( Bink ){
					BinkClose( Bink );
					Bink = 0;
				}
		};
	}catch(...){
	};
};
