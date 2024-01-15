#include "stdheader.h"
#include "Deform.h"

namespace Surface {

//-----------------------------------------------------------------------------------
// ApproxByPlane
//-----------------------------------------------------------------------------------
bool Deform::ApproxByPlane(const SurfRef &Ref, dVector &Orig, dVector &Dir) {
	if(Ref.idVerts.Count() < 3) return false;
	dVectors us;
	for(int iVert = 0; iVert < Ref.idVerts.Count(); iVert++) {
		int nVert = Ref.idVerts[iVert];
		const SurfVertex &v = Ref.pOrig->VERTS[nVert];
		us.Add(dVector(v.x, v.y, v.z));
	}
	return cSolver::AggregateOfPointsApproximation(us, Orig, Dir, 1.);
}

//-----------------------------------------------------------------------------------------
// PlanarDeformOneVertex
//-----------------------------------------------------------------------------------------
void Deform::PlanarDeformOneVertex(const SurfVertex &Centre, int Radius, SurfVertex &vtx) {
	if(vtx.InCylinder(Centre, Radius)) {
		vtx.z = Centre.z;
	}
}

//----------------------------------------------------------------------------------------------------------------------------
// DeformOneVertex
//----------------------------------------------------------------------------------------------------------------------------
void Deform::DeformOneVertex(const dVector &Orig, const dVector &Dir, const SurfVertex &Centre, int Radius, SurfVertex &vtx) {
	if(Radius < 1) return;
	dVector u(vtx.x, vtx.y, vtx.z);
	double l = u.DistToLine(Orig, Dir);
	if(l < Radius) {
		double k = - (cMath::Square(l) / double(cMath::Square(Radius)) - 1.);
		dVector d = k * Deform::dDeform * Dir;
		vtx.x += d.x;
		vtx.y += d.y;
		vtx.z += d.z;
	}
}

//-----------------------------------------------------------------------------
// Deform::DisplaceOneVertex : void(SurfVertex &)
//-----------------------------------------------------------------------------
void Deform::DisplaceOneVertex(SurfVertex &u) {
}

//------------------------------------------------------------------------------------------
// Process
//------------------------------------------------------------------------------------------
DWORD Deform::Process(SurfRef &Ref, eDeformType Type, const SurfVertex &Centre, int Radius ,SelExtArgs *Args) {
	assert(Ref.pOrig);
	dVector Orig, Dir;
	if(!ApproxByPlane(Ref, Orig, Dir)) return 0;
	bool fPlanar = GetKeyState(VK_CONTROL) < 0;
	for(int iVert = 0; iVert < Ref.idVerts.Count(); iVert++) {
		int nVert = Ref.idVerts[iVert];
		SurfVertex &r = Ref.pOrig->VERTS[nVert];
		if(fPlanar) PlanarDeformOneVertex(Centre, Radius, r);
		else DeformOneVertex(Orig, DT_PULL == Type ? Dir : - Dir, Centre, Radius, r);
	}
//	Ref.Subdivide(80);
//	if(!Args->PreserveUV)Ref.Relax(0.2, 1);
	return SelExt::IF_GEOMETRY;
}

//-----------------------------------------------------------------------------
// OnLMB
//-----------------------------------------------------------------------------
int DeformBorder=500;
DWORD Deform::OnLMB(SelExtArgs &Args) {
	SurfaceUndo::SaveSurfaceRect(
		cVec2(Args.Center.x-Args.Radius-DeformBorder,Args.Center.y-Args.Radius-DeformBorder),
		cVec2(Args.Center.x+Args.Radius+DeformBorder,Args.Center.y+Args.Radius+DeformBorder),true,false);
	return Process(Args.Ref, DT_PULL, Args.Center, Args.Radius, &Args);
}

//-----------------------------------------------------------------------------
// OnRMB
//-----------------------------------------------------------------------------
DWORD Deform::OnRMB(SelExtArgs &Args) {
	SurfaceUndo::SaveSurfaceRect(
		cVec2(Args.Center.x-Args.Radius-DeformBorder,Args.Center.y-Args.Radius-DeformBorder),
		cVec2(Args.Center.x+Args.Radius+DeformBorder,Args.Center.y+Args.Radius+DeformBorder),true,false);
	return Process(Args.Ref, DT_PUSH, Args.Center, Args.Radius, &Args);
}

//-----------------------------------------------------------------------------
// OnDraw
//-----------------------------------------------------------------------------
void Deform::OnDraw(SelExtArgs &Args) {
	dVector Orig, Dir;
	if(!ApproxByPlane(Args.Ref, Orig, Dir)) return;
	SelExt::DrawGrid(Args.Ref, cColor::Yellow.ToBgra());
	Vector3D O(Orig.x, Orig.y, Orig.z);
	Vector3D D(Dir.x, Dir.y, Dir.z);
	const int Len = 100;
	Vector3D Start = O - Len / 2 * D;
	DrawArrow(Start, D, cColor::Red.ToBgra(), 100, 0.1, 0.1);
}

} // Surface