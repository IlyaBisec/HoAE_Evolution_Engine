/*****************************************************************************/
/*	File:	rsDXErr.h
/*	Desc:	DirectX errors handling
/*	Author:	Ruslan Shestopalyuk
/*	Date:	30.01.2003
/*****************************************************************************/
#ifndef __RSDXERR_H__
#define __RSDXERR_H__

const char* GetD3DErrorDesc( HRESULT hresult );
const char* GetDispChangeErrorDesc( LONG result );

#define DX_CHK(A)		{	HRESULT hRes = (HRESULT)(A);		\
							if(hRes != S_OK) \
							{\
								Log.ErrorFL( (__FILE__), (__LINE__), \
									GlueString( "DirectX Error: %s", GetD3DErrorDesc( hRes ) ) );\
							};\
						};		
							
#define SAFE_RELEASE(A)      { if(A) {	int __nRef = (A)->Release(); \
										(A)=NULL; \
										assert( __nRef == 0 ); } }
#define SAFE_DECREF(A)			{ if(A) { (A)->Release(); } }
#define FORCE_RELEASE(A)		{ if(A) {	while((A)->Release()); (A)=NULL; } }

#endif // __RSDXERR_H__