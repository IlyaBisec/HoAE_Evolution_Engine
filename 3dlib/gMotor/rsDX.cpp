/*****************************************************************************/
/*	File:	rsDX.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	30.01.2003
/*****************************************************************************/
#include "stdafx.h"
#include "rsDX.h"

#ifndef _INLINES
#include "rsDX.inl"
#endif // !_INLINES

//#ifdef _DX9 // DirectX9
//#pragma comment (lib, "d3d9.lib"	)  
//#pragma comment (lib, "d3dx9.lib"	)
//#pragma comment (lib, "dxerr9.lib"	)
//#else
//#pragma comment (lib, "d3d8.lib"	)  
//#pragma comment (lib, "d3dx8.lib"	)
//#pragma comment (lib, "dxerr8.lib"	)
//#endif  // DirectX9/DirectX8

IRenderSystem* GetRenderSystem()
{
	return &D3DRenderSystem::instance();
}
IRenderSystem* IRS = &D3DRenderSystem::instance(); 