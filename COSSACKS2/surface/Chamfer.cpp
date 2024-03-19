#include "stdheader.h"
#include "Chamfer.h"
#include "..\Mesh\UniHash.h"
#include "..\Mesh\iVector.h"
#include "..\Mesh\MeshOperations.h"

namespace Surface {
	
bool ChamferLMBWasPressed = false;
//-----------------------------------------------------------------------------
// OnLMB
//-----------------------------------------------------------------------------
DWORD Chamfer::OnLMB(SelExtArgs &Args) {
	if(!ChamferLMBWasPressed) {
		ChamferLMBWasPressed = true;
		SurfRef Ref;
		g_UniversalMap.CutRefCylinder(Ref, Args.Center, Args.Radius, SR_TRIS);
		SurfaceUndo::DropRefToUndo(Ref);
		SurfMesh Mesh;
		Ref.ToSurfMesh(Mesh);
		SurfMesh CMesh;

		if(Mesh.TRIS.Count()) {
			BasicMesh *M = new BasicMesh;
			// VERTS ->
			for(int nVert = 0; nVert < Mesh.VERTS.Count(); nVert++) {
				const SurfVertex &r = Mesh.VERTS[nVert];
				M->AddVertex(dVector((double)r.x, (double)r.y, (double)r.z), nVert);
			}
			// TRIS ->
			int i0, i1, i2;
			for(int nTri = 0; nTri < Mesh.NTris(); nTri++) {
				Mesh.GetTri(nTri, i0, i1, i2);
				M->AddTriangle(i0, i1, i2, 0);
			}
			M->select_hard_edges(g_SurfEditor.Chamfer.GetThreshold());
			BasicMesh *C = new BasicMesh;
			M->soften_edges(*C, g_SurfEditor.Chamfer.GetWidth());

			// <- VERTS
			SurfVertex u;
			cList<bool> IsFilled;
			for(nVert = 0; nVert < C->Verts.size(); nVert++) {
				if(nVert < Mesh.VERTS.Count()) {
					u = Mesh.VERTS[nVert]; // Copying attribs from Src Surf Mesh
				}
				const dVector &d = C->Verts[nVert]->Pos;
				u.x = (float)d.x;
				u.y = (float)d.y;
				u.z = (float)d.z;
				CMesh.VERTS.Add(u);
				IsFilled.Add(nVert < Mesh.VERTS.Count());
			}
			// <- TRIS
			int Inds[3];
			for(nTri = 0; nTri < C->Triangles.size(); nTri++) {
				const BasicTriangle *T = C->Triangles[nTri];
				Inds[0] = T->nIndA, Inds[1] = T->nIndB, Inds[2] = T->nIndC;
				CMesh.AddTri(Inds[0], Inds[1], Inds[2]);
				for(int k = 0; k < 3; k++) {
					int iCur = Inds[k];
					if(!IsFilled[iCur]) {
						for(int s = 1; s <= 2; s++) {
							int iSrc = Inds[(k + s) % 3];
							if(IsFilled[iSrc]) {
								SurfVertex &Dst = CMesh.VERTS[iCur];
								SurfVertex &Src = CMesh.VERTS[iSrc];
								float x = Dst.x, y = Dst.y, z = Dst.z;
								Dst = Src;
								Dst.x = x, Dst.y = y, Dst.z = z;
								IsFilled[iCur] = true;
								break;
							}
						}
					}
				}
			}
			
			delete C;
			delete M;
		}

		if(CMesh.TRIS.Count()) {
			CMesh.CalcNormals();
			g_UniversalMap.Sum(CMesh);
		} else {
			g_UniversalMap.Sum(Mesh);
		}
		g_UniversalMap.QUADS.Compile();
		Args.Ref.idVerts.Free();
		Args.Ref.idTris.Free();
		return IF_CACHE;
	}
	return IF_GEOMETRY;
}

//-----------------------------------------------------------------------------
// OnDraw
//-----------------------------------------------------------------------------
void Chamfer::OnDraw(SelExtArgs &Args) {
	if(!Lpressed) {
		ChamferLMBWasPressed = false;
	}
	SelExt::DrawGrid(Args.Ref, cColor::White.ToBgra());
}

} // Surface