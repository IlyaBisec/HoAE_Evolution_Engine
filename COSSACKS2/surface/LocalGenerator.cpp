#include "stdheader.h"
#include "LocalGenerator.h"
#include "SurfaceGenerator.h"
#include "CmdExt.h"

void GSYSDRAW();

namespace Surface {

//-----------------------------------------------------------------------------
// LocalGenerator::InitMaskLayer : void(int &, DWORD &)
//-----------------------------------------------------------------------------
void LocalGenerator::InitMaskLayer(int &id, DWORD &Color) {
	id = Layer::FindLayerByName("LocalGeneratorMask", &Color);
	assert(id >= 0 && "Layer \"LocalGeneratorMask\" is not found!");

	if(id >= 0) {
		SurfLayerClear(id);
		SaveGenLayers();
	}
}

//-----------------------------------------------------------------------------
// LocalGenerator::OnActivate
//-----------------------------------------------------------------------------
void LocalGenerator::OnActivate() {
	InitMaskLayer(m_idMaskLayer, m_ClrMaskLayer);
	g_SelType = ST_INCYLINDER;
	ResetGroundCache();
}

//-----------------------------------------------------------------------------
// LocalGenerator::OnLMB
//-----------------------------------------------------------------------------
DWORD LocalGenerator::OnLMB(SelExtArgs &Args) {
	if(m_idMaskLayer < 0) return 0;	
	SurfLayerDrawRound(m_idMaskLayer, Args.Center, Args.Radius);
	SaveGenLayers();
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// LocalGenerator::OnRMB
//-----------------------------------------------------------------------------
DWORD LocalGenerator::OnRMB(SelExtArgs &Args) {
	if(m_idMaskLayer < 0) return 0;
	SurfLayerClearRound(m_idMaskLayer, Args.Center, Args.Radius);
	SaveGenLayers();
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// LocalGenerator::OnDraw
//-----------------------------------------------------------------------------
void LocalGenerator::OnDraw(SelExtArgs &Args) {
	if(m_idMaskLayer < 0) return;
	ShowPen(Args.Center.x, Args.Center.y, Args.Radius, m_ClrMaskLayer);
}

//-----------------------------------------------------------------------------
// LocalGenerator::OnClear
//-----------------------------------------------------------------------------
void LocalGenerator::OnClear() {
	if(m_idMaskLayer >= 0) {
		// Trying to get ref. Is it empty?
		SurfRef Ref;
		g_UniversalMap.GetRefAlongSurfLayer(Ref, m_idMaskLayer, SR_TRIS);
		if(Ref.idTris.Count()) { // it is not empty....
			char Text[] = "Are you sure you want to generate locally?";
			char Caption[] = "Confirm Generate Locally";
			if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
				// BackingUp:
				CmdExt::DrawMessageBox(cStr("Backing up..."));
				g_UniversalMap.BackUp();
				// Generating:
				SurfRef Ref;
				g_UniversalMap.CutRefAlongSurfLayer(Ref, m_idMaskLayer, SR_TRIS);
				SurfMesh Mesh;
				Ref.ToSurfMesh(Mesh);
				cStr Msg("Generating locally");
				Msg += cStr(" (") + g_SurfEditor.LocalGenerator.Passes + cStr(" passes)");
				CmdExt::MessageBoxArgs mba;
				mba.SliderLen = g_SurfEditor.LocalGenerator.Passes;
				for(int k = 0; k < g_SurfEditor.LocalGenerator.Passes; k++) {
					mba.SliderPos = k;
					CmdExt::DrawMessageBox(Msg, mba);
					Mesh.ApproximateGenerator();
				}
				// 100%
				mba.SliderPos = mba.SliderLen;
				CmdExt::DrawMessageBox(Msg, mba);

				g_UniversalMap.Sum(Mesh);
				g_UniversalMap.QUADS.Compile();
				g_UniversalMap.FillMapOfHeights();
			}
		}
		SurfLayerClear(m_idMaskLayer);
		SaveGenLayers();
	}
	DistField::ClearMaps();
	ResetGroundCache();
}

} // Surface