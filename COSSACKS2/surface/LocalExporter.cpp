#include "stdheader.h"
#include "LocalExporter.h"
#include "LocalGenerator.h"
#include "SurfaceGenerator.h"
#include "SurfExporter.h"
#include "CmdExt.h"

namespace Surface {

//-----------------------------------------------------------------------------
// LocalExporter::OnActivate
//-----------------------------------------------------------------------------
void LocalExporter::OnActivate() {
	LocalGenerator::InitMaskLayer(m_idMaskLayer, m_ClrMaskLayer);
	g_SelType = ST_INCYLINDER;
	ResetGroundCache();
}

//-----------------------------------------------------------------------------
// LocalExporter::OnLMB
//-----------------------------------------------------------------------------
DWORD LocalExporter::OnLMB(SelExtArgs &Args) {
	if(m_idMaskLayer < 0) return 0;
	SurfLayerDrawRound(m_idMaskLayer, Args.Center, Args.Radius);
	SaveGenLayers();
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// LocalExporter::OnRMB
//-----------------------------------------------------------------------------
DWORD LocalExporter::OnRMB(SelExtArgs &Args) {
	if(m_idMaskLayer < 0) return 0;
	SurfLayerClearRound(m_idMaskLayer, Args.Center, Args.Radius);
	SaveGenLayers();
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// LocalExporter::OnDraw
//-----------------------------------------------------------------------------
void LocalExporter::OnDraw(SelExtArgs &Args) {
	if(m_idMaskLayer < 0) return;
	ShowPen(Args.Center.x, Args.Center.y, Args.Radius, m_ClrMaskLayer);
}

//-----------------------------------------------------------------------------
// LocalExporter::OnClear
//-----------------------------------------------------------------------------
void LocalExporter::OnClear() {
	if(m_idMaskLayer >= 0) {
		SurfRef Ref;
		g_UniversalMap.GetRefAlongSurfLayer(Ref, m_idMaskLayer, SR_TRIS);
		SurfMesh Mesh;
		Ref.ToSurfMesh(Mesh);
		if(Ref.idTris.Count()) {
			SurfExporter::Export(Mesh);
		}

		SurfLayerClear(m_idMaskLayer);
		SaveGenLayers();
	}
	DistField::ClearMaps();
	ResetGroundCache();
}

} // Surface