#ifndef _ogg_vor_h_included_
#define _ogg_vor_h_included_

#include <windows.h>
#include "../TVPlugin/TVPlugin.h"

void ov_Play( LPCSTR pcszFileName , BYTE ucStream = 0 );
__declspec( dllexport )
void ov_Stop( BYTE ucStream = 0 );
void ov_SetVolume( BYTE ucVolume , BYTE ucStream = 0 );
WORD ov_GetStreamLength( BYTE ucStream = 0 );
bool ov_StreamFinished( BYTE ucStream = 0 );

#endif // _ogg_vor_h_included_
