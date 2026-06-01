#include "stdheader.h"
#include "Scissors.h"
#include "CmdExt.h"

void GSYSDRAW();

namespace Surface {

bool ScissorsLMBWasPressed = false;
//-----------------------------------------------------------------------------
// OnLMB
//-----------------------------------------------------------------------------
DWORD Scissors::OnLMB(SelExtArgs &Args) {
	if(!ScissorsLMBWasPressed) {
		SurfaceUndo::DropRefToUndo(Args.Ref);
		ScissorsLMBWasPressed = true;
		Args.Ref.idTris.Sort();
		DWORDS TRIS;
		int iTri = 0, i3[3], k;
		int NTris = Args.Ref.pOrig->NTris();
		int nTri;
		for(nTri = 0; nTri < NTris; nTri++) {
			if(iTri >= Args.Ref.idTris.Count()) break;
			if(nTri != Args.Ref.idTris[iTri]) {
				Args.Ref.pOrig->GetTri(nTri, i3);
				for(k = 0; k < 3; k++) {
					TRIS.Add(i3[k]);
				}
			} else {
				iTri++;
			}
		}
		// Append tail...
		for(; nTri < NTris; nTri++) {
			Args.Ref.pOrig->GetTri(nTri, i3);
			for(k = 0; k < 3; k++) {
				TRIS.Add(i3[k]);
			}
		}
		Args.Ref.pOrig->TRIS.Copy(TRIS);
		Args.Ref.idVerts.Free(), Args.Ref.idTris.Free();
		g_UniversalMap.QUADS.Init();
		return IF_CACHE;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// OnActivate
//-----------------------------------------------------------------------------
void Scissors::OnActivate() {
	CmdExt::MessageBox(cStr("Backing up..."));
	GSYSDRAW();
	//g_UniversalMap.BackUp();
}

//-----------------------------------------------------------------------------
// OnClear
//-----------------------------------------------------------------------------
void Scissors::OnClear() {
	CmdExt::MessageBox(cStr("Compiling..."));
	GSYSDRAW();
	g_UniversalMap.QUADS.Compile();
	g_UniversalMap.QUADS.Init();
}

//-----------------------------------------------------------------------------
// OnDraw
//-----------------------------------------------------------------------------
void Scissors::OnDraw(SelExtArgs &Args) {
	if(!Lpressed) ScissorsLMBWasPressed = false;
	SelExt::DrawGrid(Args.Ref, cColor::Red.ToBgra());
}

} // Surface