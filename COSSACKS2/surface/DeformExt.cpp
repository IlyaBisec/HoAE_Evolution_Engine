#include "DeformExt.h"

// DeformExt::ApproxByPlane
bool DeformExt::ApproxByPlane(const SelExtParams &Params, cVector &Orig, cVector &Dir) {
	const DWORDS &VertsTo = Params.VertsTo;
	if(VertsTo.Count() < 3) return false;
	const VertexBasedMeshRef &Ref = Params.RefTo;
	cVectors vecs;
	for(int i = 0; i < VertsTo.Count(); i++) {
		const OneMapVertex &vtx = Ref.pOrigin->Vert[VertsTo[i]];
		vecs.Add(cVector(vtx.x, vtx.y, vtx.z));
	}
	return cSolver::AggregateOfPointsApproximation(vecs, Orig, Dir, 1.);
} // DeformExt::ApproxByPlane

// DeformExt::DeformOneVertex
void DeformExt::DeformOneVertex(const cVector &Orig, const cVector &Dir, int Radius, OneMapVertex &vtx) {
	if(Radius < 1) return;
	cVector vec(vtx.x, vtx.y, vtx.z);
	double l = vec.DistToLine(Orig, Dir);
	if(l < Radius) {
		double k = - (Square(l) / double(Square(Radius)) - 1.);
		cVector d = k * ExtOptions::dDeform * Dir;
		vtx.x += d.x;
		vtx.y += d.y;
		vtx.z += d.z;
	}
} // DefomrExt::DeformOneVertex


// DeformExt::OnLMB
DWORD DeformExt::OnLMB(SelExtParams &Params) {
	cVector Orig, Dir;
	if(!ApproxByPlane(Params, Orig, Dir)) return 0;
	for(int i = 0; i < Params.VertsTo.Count(); i++) {
		DeformOneVertex(Orig, Dir, Params.Radius, Params.RefTo.pOrigin->Vert[Params.VertsTo[i]]);
	}
	return SelExt::IF_GEOMETRY;
} // DeformExt::OnLMB

// DeformExt::OnRMB
DWORD DeformExt::OnRMB(SelExtParams &Params) {
	return 0;
} // DeformExt::OnRMB

// DeformExt::OnDraw
void DeformExt::OnDraw(SelExtParams &Params) {
	if(!Params.RefTo.m_TriNums.NValues) return;
	assert(Params.VertsTo.Count() >= 3);
	cVector Orig, Dir;
	if(!ApproxByPlane(Params, Orig, Dir)) return;
	DrawGrid(Params.RefTo, comms::argbWhite);
	// ---------------------------------
	VertexBasedMesh vbm;
	cVector E = Orig + 100 * Dir;
	vbm.AddLine(OneMapVertex(Orig.x, Orig.y, Orig.z), OneMapVertex(E.x, E.y, E.z), comms::argbRed);
	VertexBasedMeshRef Ref;
	vbm.GetRefAll(Ref);
	OneSplitMesh Splitted;
	Splitted.Split(Ref, 1);
	Splitted.Draw();
} // DeformExt::OnDraw