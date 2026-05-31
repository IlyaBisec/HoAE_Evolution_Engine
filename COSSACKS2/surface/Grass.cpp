#include "stdheader.h"
#include "Chamfer.h"
#include "..\Mesh\UniHash.h"
#include "..\Mesh\iVector.h"
#include "..\Mesh\MeshOperations.h"
#include "..\QuadsRender.h"
#include "Grass.h"

namespace Surface {
	
	//-----------------------------------------------------------------------------
	// OnLMB
	//-----------------------------------------------------------------------------
	DWORD Grass::OnLMB(SelExtArgs &Args) {
		GrassRenderer::SetGrassRound(Args.Center.x,Args.Center.y,Args.Radius,128,128);
		return 0;
	}
	DWORD Grass::OnRMB(SelExtArgs &Args) {
		GrassRenderer::ClearGrassRound(Args.Center.x,Args.Center.y,Args.Radius,128,0);
		return 0;
	}
	//-----------------------------------------------------------------------------
	// OnDraw
	//-----------------------------------------------------------------------------
	void Grass::OnDraw(SelExtArgs &Args) {
		char c[128];
		sprintf(c,"Grass %d (press 1-4 to change)",GrassRenderer::CurrentGrassType+1);
		ShowString(100,RealLy-40,c,&BigWhiteFont);
		if(GetKeyState('1')&0x8000)GrassRenderer::CurrentGrassType=0;
		if(GetKeyState('2')&0x8000)GrassRenderer::CurrentGrassType=1;
		if(GetKeyState('3')&0x8000)GrassRenderer::CurrentGrassType=2;
		if(GetKeyState('4')&0x8000)GrassRenderer::CurrentGrassType=3;
		SelExt::DrawGrid(Args.Ref, cColor::White.ToBgra());
	}

} // Surface