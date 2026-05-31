#include "stdheader.h"
#include "Surface.h"
#include "SurfSplit.h"
#include "SurfaceGenerator.h"
#include "..\SimpleTools.h"
#include "..\RoadsAndFactures.h"

namespace Surface {

//-----------------------------------------------------------------------------
// SurfSplit::Free
//-----------------------------------------------------------------------------
void SurfSplit::Free() {
	for(int i = 0; i < CACHE.Count(); i++) {
		assert(CACHE[i]);
		delete CACHE[i];
		CACHE[i] = NULL;
	}
	CACHE.Free();
}

//-----------------------------------------------------------------------------
// SurfSplit::Draw
//-----------------------------------------------------------------------------
void SurfSplit::Draw() {
	IRS->ResetWorldTM();

	const float zBias = 1;
	ICam->ShiftZ(zBias);
	ICam->Render();

	for(int i = 0; i < CACHE.Count(); i++) {
		assert(CACHE[i]);
		DrawBM(*CACHE[i]);
	}

	ICam->ShiftZ(- zBias);
	ICam->Render();
}



//static const int SurfSplitCacheSize = 3996;

struct {
	size_t Size() {
		size_t h = 0;
		for(int f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
			h += hTables[f].Size();
		}
		size_t v = 0;
//		v = sizeof(VERTS);
		for(f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
			v += VERTS[f].Size();
		}
		size_t t = 0;
//		t = sizeof(TRIS);
		for(f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
			t += TRIS[f].Size();
		}
		return (h + v + t/* + sizeof(iTRIS) + sizeof(iVERTS)*/) / 1024;
	}

	cHashIndex hTables[SPLITMESH_MAXNFACTURES];
	cList<SurfSplitVertex> VERTS[SPLITMESH_MAXNFACTURES];
	cList<WORD> TRIS[SPLITMESH_MAXNFACTURES];

	SurfSplitCache() {
	}

	void Clear(int f) {
		VERTS[f].Clear();
		TRIS[f].Clear();
		hTables[f].Clear();
	}
} SurfSplitCache;

inline void SurfSplitOneVertex(int f, SurfSplitVertex& s_orig, SurfSplitVertex s,bool first) {  

	float dist = 0;
	cVec3 newP(s.x,s.y,s.z);
	cVec2 uv;
	if(s.oi!=s_orig.oi){
		SurfMapper m(f);
		uv=m.ExtrapolateUV(s_orig,newP);
		s.newU=uv.x;
		s.newV=uv.y;
	}else{
        uv=cVec2(s_orig.u,s_orig.v);
		s.newU=s_orig.u;
		s.newV=s_orig.v;
	}
	if(!s.TriEquals(s_orig)){
		s.Alpha = false;
		s.src_vindex = s_orig.oi;		
		dist=cVec3::Distance(newP,cVec3(s_orig.x,s_orig.y,s_orig.z));		
	}else{		
		s.src_vindex = s.oi;
	}
	if(first)s.Alpha=true;
	s.DistToSrc = dist;	

	int key = SurfSplitCache.hTables[f].GenerateKey(s.oi, 0);
	for(int index = SurfSplitCache.hTables[f].First(key); index != -1; index = SurfSplitCache.hTables[f].Next(index)) {
		SurfSplitVertex& sc = SurfSplitCache.VERTS[f][index];
		if( sc.Equals(s) ){// && sc.src_vindex==s.src_vindex){
			static float Diff=0.1;
			if( abs(sc.newU-s.newU)<Diff && abs(sc.newV-s.newV)<Diff ){
				break;
			}
            /*
			if( sc.DistToSrc > dist  && abs(sc.newU-s.newU)<0.05 && abs(sc.newV-s.newV)<0.05 ){
				sc.DistToSrc = dist;
				sc.src_vindex = s.src_vindex;				
				sc.newU=s.newU;
				sc.newV=s.newV;
			}
			break;
			*/
		} 
	}
	if(-1 == index) {
		index = SurfSplitCache.VERTS[f].Add(s);

		SurfSplitCache.hTables[f].Add(key, index);
	}
	SurfSplitCache.TRIS[f].Add(index);
}

void SurfSplitOneTri(int f, SurfSplitVertex& s_orig, SurfSplitVertex *s3,bool first) {	
	for(int k = 0; k < 3; k++) SurfSplitOneVertex(f, s_orig, s3[k],first);
}

int _2dw_compare(const void * a1, const void *a2){
    DWORD d1=*((DWORD*)a1);
    DWORD d2=*((DWORD*)a2);
    if(d1==d2)return 0;
    return d1<d2 ? -1 : 1;
}
DWORD BT4(DWORD x1,DWORD x2,DWORD x3,DWORD x4){
	return x1+(x2<<8)+(x3<<16)+(x4<<24);
}
__inline int lrintf (float flt){
	int intgr;
	_asm{
		fld flt
		fistp intgr
	};
	return intgr ;
}
void SurfSplit::toCache(const SurfRef& ref,int f, eSurfFx SFX) {

	cList<WORD> &Tris = SurfSplitCache.TRIS[f];

	if(!Tris.Count()) return;

    static DynArray<DWORD> Encoder;    
    Encoder.FastClear();    
    static DynArray<DWORD> EncoderBack;
    EncoderBack.FastClear();
    int N=SurfSplitCache.VERTS[f].Count();
    BaseMesh *pBaseMesh = new BaseMesh();
    assert(pBaseMesh);
    pBaseMesh->create(SurfSplitCache.VERTS[f].Count(), Tris.Count(), vfVERTEX);
    int nT=Tris.Count();
    word* idxs=pBaseMesh->getIndices();    
    for(int i=0;i<Tris.Count();i+=3){
        //int i0=Encoder[(i/3)*2+1]*3;
		int i0=i;
        idxs[i  ]=Tris[i0  ];
        idxs[i+1]=Tris[i0+1];
        idxs[i+2]=Tris[i0+2];
    }
    pBaseMesh->setNInd(Tris.Count());
    pBaseMesh->setNPri(Tris.Count()/3);
	VERTEX *pVerts = (VERTEX*)pBaseMesh->getVertexData();
	assert(pVerts);
	SurfMapper Mapper(f);
	// pVerts:
	uv_pair uv;    
	for(int i = 0; i < N; i++) {
		const SurfSplitVertex &s = SurfSplitCache.VERTS[f][i];
		pVerts->x = s.x, pVerts->y = s.y, pVerts->z = s.z;
		pVerts->diffuse = s.CalcDiffuseColor();
        if(!s.Alpha) pVerts->diffuse &= 0x00ffffff;
		Vector3D N(s.nx,s.ny,s.nz);
		N.normalize();		
		if(s.src_vindex==s.oi){
			pVerts->u = s.u;
			pVerts->v = s.v;
			pVerts->specular=BT4(128+s.nx,128+s.ny,128+s.nz,s.Luminance);
			pVerts->color2=BT4(128+s.tx,128+s.ty,128+s.tz,0);
			Vector3D T(s.tx,s.ty,s.tz);
			Vector3D B(s.bx,s.by,s.bz);
			Vector3D N(s.nx,s.ny,s.nz);
			Vector3D B1;
			B1.cross(T,N);
			if(B1.dot(B)>0)pVerts->color2|=0xFF000000;			
		}else{
			SurfVertex& sv = ref.pOrig->VERTS[s.src_vindex];
			pVerts->u = s.newU;
			pVerts->v = s.newV;
			pVerts->specular=BT4(128+sv.nx,128+sv.ny,128+sv.nz,s.Luminance);
			pVerts->color2=BT4(128+sv.tx,128+sv.ty,128+sv.tz,0);
			Vector3D T(sv.tx,sv.ty,sv.tz);
			Vector3D B(sv.bx,sv.by,sv.bz);
			Vector3D N(sv.nx,sv.ny,sv.nz);
			Vector3D B1;
			B1.cross(T,N);
			if(B1.dot(B)>0)pVerts->color2|=0xFF000000;
		}
		
		pVerts++;
	}
    int nv=N;
    // Finalizing base mesh:
	pBaseMesh->setNVert(nv);
    pBaseMesh->setNInd(nT);
    pBaseMesh->setNPri(nT/3);    
    pBaseMesh->setTexture(Surface::SurfVertex::GetFactureID(f));
    if(-1 == SFX) pBaseMesh->setShader(SurfVertex::GetFx(f));
	else{
		if(SFX == SFX_DEFAULT){
			if(UsesBump(f))pBaseMesh->setShader(SurfFx(SFX_BUMP));
			else pBaseMesh->setShader(SurfFx(SFX));
		}else pBaseMesh->setShader(SurfFx(SFX));
	}
	int sh=pBaseMesh->getShader();
	int MConst=IRS->GetShaderVarID(sh,"UVFlow");
	if(MConst!=-01){
		pBaseMesh->UseMeshConst=true;		
		pBaseMesh->MeshConst=Vector4D(FacturesList[f]->UFlowSpeed,FacturesList[f]->VFlowSpeed,0,0);
		pBaseMesh->ConstIdx=MConst;
	}else{
		pBaseMesh->UseMeshConst=false;
	}
    static int white=IRS->GetTextureID("white.tga");
	pBaseMesh->setTexture(Surface::SurfVertex::GetFactureID2(f),2);
    pBaseMesh->setTexture(white,1);
	CACHE.Add(pBaseMesh);
	// Clean up:

//	cStr Info;
//	Info << "NVerts: " << pBaseMesh->getNVert() << "  NInds: " << pBaseMesh->getNInd();
//	Log.Message(Info.asChar());

	pBaseMesh = NULL;
	SurfSplitCache.Clear(f);
}

void SurfSplit::Split(const SurfRef &Ref, eSurfFx SFX) {
	Free();
	void ClearBaseV();
	ClearBaseV();

	if(!Ref.idTris.Count()) return;

//	cTimer Timer; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	Timer.Begin(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// Split to SurfSplitCache:
	int nTri, oi[3], k, f;
	SurfSplitVertex s[3];	
	for(int iTri = 0; iTri < Ref.idTris.Count(); iTri++) {
		nTri = Ref.idTris[iTri];		
		Ref.pOrig->GetTri(nTri, oi);
		Ref.pOrig->GetTri(nTri, s[0], s[1], s[2]);
		int ordr[3]={0,1,2};
		int nF[3];
		for(int i=0;i<3;i++){
			nF[i]=s[i].nFacture;
			if(UsesBump(nF[i])){
				nF[i]+=1024;
			}
		}
		if(nF[1] <= nF[0] && nF[1] <= nF[2]){
			int oo=oi[0];
			oi[0]=oi[1];
			oi[1]=oi[2];
			oi[2]=oo;
			SurfSplitVertex sv=s[0];
			s[0]=s[1];
			s[1]=s[2];
			s[2]=sv;			
		}else
		if(nF[2] <= nF[0] && nF[2] <= nF[1]){
			int oo=oi[0];
			oi[0]=oi[2];			
			oi[2]=oi[1];
			oi[1]=oo;
			SurfSplitVertex sv=s[0];
			s[0]=s[2];
			s[2]=s[1];			
			s[1]=sv;
		}
		for(k = 0; k < 3; k++) {
			s[k].oi = oi[k];
			s[k].MappingCode = 0;
			s[k].Alpha = true;
		}
		if(SurfSplitCache.TRIS[s[0].nFacture].Count() == 0xffff) {	
			toCache(Ref,s[0].nFacture, SFX);
		}

		SurfSplitOneTri(s[0].nFacture, s[0], s,true);
		if(!s[1].TriEquals(s[0])) {
			if(SurfSplitCache.TRIS[s[1].nFacture].Count() == 0xffff) {
				toCache(Ref,s[1].nFacture, SFX);
			}
			SurfSplitOneTri(s[1].nFacture, s[1], s,false);
		}
		if(!s[2].TriEquals(s[1]) && !s[2].TriEquals(s[0])) {
			if(SurfSplitCache.TRIS[s[2].nFacture].Count() == 0xffff) {
				toCache(Ref,s[2].nFacture, SFX);
			}
			SurfSplitOneTri(s[2].nFacture, s[2], s, false);
		}		
	}

	for(f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
		if(!SurfSplitCache.TRIS[f].Count()) continue;
		toCache(Ref,f, SFX);
	}

//	Timer.End(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	Log.Message("SurfSplit::Split() : t == %f", Timer.m_msMeanTime); // <<<<<<<<
}

//--------------------------------------------------------------------------------------------------------------------------------------------
// SurfSplit::Cache::Modify : void(BaseMesh &, const ModifyArgs &)
//--------------------------------------------------------------------------------------------------------------------------------------------
void SurfSplit::Cache::Modify(BaseMesh &bm, const ModifyArgs &Args) {
	if(Args.Flags & ModifyFlags::MF_TEXTURE) {
		bm.setTexture(Args.idTexture);
	}
	VERTEX *pVerts = (VERTEX*)bm.getVertexData();
	assert(pVerts);
	int NVerts = bm.getNVert();
	for(int j = 0; j < NVerts; j++) {
		if(Args.Flags & ModifyFlags::MF_OFFSET) {
			pVerts->x += Args.Offset.x;
			pVerts->y += Args.Offset.y;
			pVerts->z += Args.Offset.z;
		}
		if(Args.Flags & ModifyFlags::MF_SEA) {
			if(pVerts->z < 1) pVerts->z = 1;
		}
		if(Args.Flags & ModifyFlags::MF_AND) {
			pVerts->diffuse &= Args.DiffuseAndFlag;
		}
		if(Args.Flags & ModifyFlags::MF_OR) {
			pVerts->diffuse |= Args.DiffuseOrFlag;
		}
		if(Args.Flags & ModifyFlags::MF_SUM) {
			pVerts->diffuse = (cColor::FromBgra(pVerts->diffuse) + cColor::FromBgra(Args.DiffuseSum)).ToBgra();
		}
		if(Args.Flags & ModifyFlags::MF_LAYER) {
			MixWithSurfLayers(pVerts->x, pVerts->y, pVerts->diffuse, g_SurfEditor.Layer.LAYER);
		}
		pVerts++;
	}
}

//------------------------------------------------------------------------------------------------------------------------------
// SurfSplit::Cache::Modify : void(const ModifyArgs &)
//------------------------------------------------------------------------------------------------------------------------------
void SurfSplit::Cache::Modify(const ModifyArgs &Args) {
	for(int i = 0; i < Count(); i++) {
		BaseMesh *pBaseMesh = GetAt(i);
		if(!pBaseMesh) continue;
		Modify(*pBaseMesh, Args);
	}
}

} // Surface
