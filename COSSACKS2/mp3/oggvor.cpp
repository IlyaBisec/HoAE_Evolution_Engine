#include "oggvor.h"
#include "kLog.h"

#define MAX_STREAM_HANDLE 6
#define MAX_STREAM_VOLUME 100

static TV_AH aStreamHandles[ MAX_STREAM_HANDLE ] = 
{ NULL , NULL , NULL , NULL , NULL , NULL };
static BYTE aStreamVolumes[ MAX_STREAM_HANDLE ] = 
{ MAX_STREAM_VOLUME , MAX_STREAM_VOLUME , MAX_STREAM_VOLUME , MAX_STREAM_VOLUME , MAX_STREAM_VOLUME , MAX_STREAM_VOLUME };

int ov_Check( BYTE ucStream )
{
	if ( ucStream > MAX_STREAM_HANDLE - 1 ) {
		Log.Error( "TheoVorb: handle [%i] out of bounds [%i]" , ucStream , MAX_STREAM_HANDLE - 1 );
		return 1;
	} else
		return 0;
}

void ov_Play( LPCSTR pcszFileName , BYTE ucStream )
{
	TV_AH _ah = NULL;

	if ( GetFileAttributes( pcszFileName ) == INVALID_FILE_ATTRIBUTES ) {
		Log.Error( "TheoVorb: can't find file [%s] to play" , pcszFileName );
		return;
	}

	if ( ov_Check( ucStream ) )
		return;

	if ( aStreamHandles[ ucStream ] ) {
		Log.Warning( "TheoVorb: attempting to play [%s] in non-closed stream [%i]" , pcszFileName , ucStream );
		Log.Warning( "TheoVorb: stopping non-closed stream [%i] first" , ucStream );
		ov_Stop( ucStream );
	}

	_ah = tv_StreamOpen( pcszFileName );

	if ( ! _ah ) {
		Log.Error( "TheoVorb: can't open file [%s] to play [file format error]" , pcszFileName );
		return;
	}

	tv_StreamSetVolume( _ah , aStreamVolumes[ ucStream ] );

	tv_StreamPlay( _ah );

	aStreamHandles[ ucStream ] = _ah;
}

__declspec( dllexport )
void ov_Stop( BYTE ucStream )
{
	if ( ov_Check( ucStream ) )
		return;

	if ( aStreamHandles[ ucStream ] ) {
		tv_StreamClose( aStreamHandles[ ucStream ] );
		aStreamHandles[ ucStream ] = NULL;
	} else
		Log.Warning( "TheoVorb: attempting to stop non-playing stream [%i]" , ucStream );
}

void ov_SetVolume(BYTE ucVolume,BYTE ucStream)
{
	if ( ov_Check( ucStream ) )
		return;

	aStreamVolumes[ ucStream ] = ucVolume;

	if ( aStreamHandles[ ucStream ] )
		tv_StreamSetVolume( aStreamHandles[ ucStream ] , aStreamVolumes[ ucStream ] );
}

WORD ov_GetStreamLength(BYTE ucStream) // возвращает значение в секундах
{
	if ( ov_Check( ucStream ) )
		return 0;

	if ( aStreamHandles[ ucStream ] ) {
		return tv_StreamLength( aStreamHandles[ ucStream ] );
	} else {
		Log.Warning( "TheoVorb: attempting to get length of non-playing stream [%i]" , ucStream );
		return 0;
	}
}

bool ov_StreamFinished(BYTE ucStream)
{
	if ( ov_Check( ucStream ) )
		return true;

	if ( aStreamHandles[ ucStream ] ) {
		return tv_StreamFinished( aStreamHandles[ ucStream ] );
	} else {
		Log.Warning( "TheoVorb: attempting to query non-playing stream [%i]" , ucStream );
		return true;
	}
}
