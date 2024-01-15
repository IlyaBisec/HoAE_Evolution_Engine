#include "stdheader.h"
#include "mLodder.h"
#include "..\Surface\Surface.h"
#include "ITerrain.h"
#include "SurfSplit.h"

struct OneFaceInfo{
	DWORD ID;
    cVec3 Normal;
	cVec3 Tangent;
	cVec3 Binormal;
	DWORD NearFaces[3];
	DWORD Verts[3];
};
/*
bool uv_based_GeometryCallback( SurfRef &Ref, bool fAuxiliaryCall ) {
	eSurfFx fx = fAuxiliaryCall ? Surface::SFX_AUX : (g_SurfEditor.Wireframe ? Surface::SFX_WIRE : Surface::SFX_DEFAULT);	
    
	//gathering faces
	static DynArray<OneFaceInfo>   FacesInfo;	
	static uni_hash<DWORD,DWORDS2> Edges;
	FacesInfo.FastClear();

	int n = Ref.idTris.Size();
	for(int i=0;i<n;i++){
		OneFaceInfo FI;
		DWORD Tri=Ref.idTris[i];
		FI.ID=Tri;	
		int i0,i1,i2;
		Ref.pOrig->GetTri(Tri,i0,i1,i2);

		FI.Verts[0]=i0;
		FI.Verts[1]=i1;
		FI.Verts[2]=i2;

		Ref.pOrig->GetTri(Tri,FI.Normal);
                
	}
}
*/