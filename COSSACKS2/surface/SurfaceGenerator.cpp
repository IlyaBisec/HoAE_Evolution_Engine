#include "stdheader.h"
#include "SurfaceGenerator.h"
#include "ITerrain.h"
#include "IWater.h"
#include "..\WeaponSystem.h"
#include "..\Rasterizator.h"
#include "SelExt.h"
#include "SurfaceGenerator.h"
#include "Layer.h"
#include "..\LoadSave.h"
#include "..\comms\comms.h"
#include "..\UndoMaster.h"
#include "..\UnitAbility.h"

extern SurfaceGenerator SURF;
void InitLayersSystem(){
	bool LoadMLayersFromFileInCompactForm(char* filename);
	LoadMLayersFromFileInCompactForm("Surface\\SurfaceShape.dat");
	SURF.reset_class(&SURF);
	SURF.SafeReadFromFile("Surface\\Surface3D.xml");
}

void GetTexString(char* dest,ClonesArray<OneTexturePair>& tex){
	int N=tex.GetAmount();
	for(int i=0;i<N;i++){
		DWORD C1,C2;
		int GetFacture(int);
		int tn=GetFacture(tex[i]->Facture); 
		DWORD C=tex[i]->Color;
		C=MulDWORD(C,512);
		C1=C|0xFF000000;
		C2=C&0xFFFFFF;
		if(i==0){
			if(N==1)C2=C1;
			sprintf(dest,"{tid %d 0 0 64 32  0 0 64 32  %X %X %X %X}",tn,C1,C2,C1,C2);
			dest+=strlen(dest);			
		}else
			if(i==N-1){			
				sprintf(dest,"{tid %d -64 0 64 32  0 0 64 32  %X %X %X %X 0}",tn,C2,C1,C2,C1);
				dest+=strlen(dest);			
			}else{
				sprintf(dest,"{tid %d -64 0 64 32  0 0 64 32  %X %X %X %X 0}",tn,C2,C1,C2,C1);
				dest+=strlen(dest);
				sprintf(dest,"{tid %d 0 0 64 32  0 0 64 32  %X %X %X %X}",tn,C1,C2,C1,C2);
				dest+=strlen(dest);
			}
	}
}

// RACES_ENUM
DWORD fn_RECREATE_RACES_ENUM(RefreshableEnumeratorContext* in, EnumeratorOutputStream *out,DWORD PrevCallHash){
	if(PrevCallHash != LAYERSLIST.Layers.GetAmount()){
		for(int i=0;i < LAYERSLIST.Layers.GetAmount(); i++)
			if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace) {
				out->add(LAYERSLIST.Layers[i]->Name.pchar(), i, NULL);
			}
	}
	return LAYERSLIST.Layers.GetAmount();
}

void fn_CREATE_RACES_ENUM() {
	RefreshableEnumerator *RE = new RefreshableEnumerator(fn_RECREATE_RACES_ENUM);
	Enumerator *E = ENUM.Get("RACES");
	E->SetProcEnum(RE);
}

// ENUM: SURFLAYERS
DWORD RECREATE_ENUM_SURFLAYERS(RefreshableEnumeratorContext *in, EnumeratorOutputStream *out, DWORD PrevCallHash) {
	// Filtering layers:
	cList<int> ids;
	for(int i = 0; i < LAYERSLIST.Layers.GetAmount(); i++) {
		if(LAYERSLIST.Layers[i]->IsSurfLayer()) {
			ids.Add(i);
		}
	}
	if(PrevCallHash != ids.Count()) {
		for(int n = 0; n < ids.Count(); n++) {
			char *LayerName = LAYERSLIST.Layers[ids[n]]->Name.pchar();
			out->add(LayerName, n, NULL);
		}
	}
	return ids.Count();
}

void CREATE_ENUM_SURFLAYERS() {
	RefreshableEnumerator *re = new RefreshableEnumerator(RECREATE_ENUM_SURFLAYERS);
	Enumerator *e = ENUM.Get("SURFLAYERS");
	e->SetProcEnum(re);
}

// ENUM: SURFTYPES
DWORD FN_ENUM_SURFTYPES(RefreshableEnumeratorContext *in, EnumeratorOutputStream *out, DWORD PrevCallHash) {
	if(PrevCallHash == 3) return PrevCallHash;
	out->add("Default", 0, NULL);
	out->add("Architecture", 1, NULL);
	out->add("Forest", 2, NULL);
	return 3;
}

void CREATE_ENUM_SURFTYPES() {
	Enumerator *pEnum = ENUM.Get("SURFTYPES");
	assert(pEnum);
	pEnum->SetProcEnum(new RefreshableEnumerator(&FN_ENUM_SURFTYPES));
}

// ENUM: DISPLACEMENTMAPS
DWORD FN_ENUM_DISPLACEMENTMAPS(RefreshableEnumeratorContext *in, EnumeratorOutputStream *out, DWORD PrevCallHash) {
	static int id = 0;
	static bool f = false;
	if(f) return id;
	f = true;
	cList<cWin32::FileInfo> Files;
	if(cWin32::SearchFiles(Files, "Textures\\Displacement Maps", "bmp")) {
		for(int i = 0; i < Files.Count(); i++) {
			const cWin32::FileInfo &r = Files[i];
			int n = IRS->GetTextureID(r.Path);

			int p;
			BYTE *pBits = IRS->LockTexBits(n, p);
			int W = IRS->GetTextureWidth(n);
			int H = IRS->GetTextureHeight(n);
			IRS->UnlockTexBits(n);
			if(!pBits) continue;

			if(n >= 0) {
				out->add(r.Name, id, NULL);
				id++;
			}
		}
	}
	return id;
}

void CREATE_ENUM_DISPLACEMENTMAPS() {
	Enumerator *pEnum = ENUM.Get("DISPLACEMENTMAPS");
	assert(pEnum);
	pEnum->SetProcEnum(new RefreshableEnumerator(&FN_ENUM_DISPLACEMENTMAPS));
}

//-----------------------------------------------------------------------------
// fn_SURFACE_ILLUMINATIONTYPE_ENUM
//-----------------------------------------------------------------------------
DWORD fn_SURFACE_ILLUMINATIONTYPE_ENUM(RefreshableEnumeratorContext* in, EnumeratorOutputStream* out, DWORD PrevCallHash) {
	if(PrevCallHash == 3) return PrevCallHash;
	out->add("None", Surface::I_NONE, NULL);
	out->add("Local", Surface::I_LOCAL, NULL);
	out->add("Direct", Surface::I_DIRECT, NULL);
	return 3;
};

//-----------------------------------------------------------------------------
// RegisterSurfaceClasses
//-----------------------------------------------------------------------------
void RegisterSurfaceClasses() {
	REG_CLASS(SurfaceGenerator);
	REG_CLASS(OneTexturingStyle);
	REG_CLASS(OneSurfaceType);
	REG_CLASS(OneSurfaceRule);
	REG_CLASS(OneTexturingRule);
	REG_CLASS(OneTexturePair);
	REG_CLASS(OneGenCondition);
	REG_CLASS(OneTexturingCondition);
	REG_CLASS(CheckHeightAndAngle);
	REG_CLASS(CheckHeight);
	REG_CLASS(CheckVariatedHeightAndAngle);
	REG_CLASS(CheckSpecialLayer);
	REG_CLASS(CheckIfNotInLayer);
	REG_CLASS(CheckIfNearLayer);
	REG_CLASS(OneLayerColor);
	REG_CLASS(CheckSpecialSurfaceType);
	REG_CLASS(CheckIfSlopeIsNear);

	Enumerator *E = ENUM.Get("SURFACE_ILLUMINATIONTYPE_ENUM");
	E->SetProcEnum(new RefreshableEnumerator(&fn_SURFACE_ILLUMINATIONTYPE_ENUM));
	fn_CREATE_RACES_ENUM();
	CREATE_ENUM_SURFTYPES();
	CREATE_ENUM_SURFLAYERS();
	CREATE_ENUM_DISPLACEMENTMAPS();
	REG_CLASS(CameraSplineKey);
	REG_CLASS(CameraSpline);
	REG_CLASS(CameraTrack);

	REG_CLASS(SurfEditor);
	REG_CLASS(SurfArchway);
	REG_CLASS(SurfCamera);
}

//----------------------------------------------------------------------------------------------------
// SaveNewSurface
//----------------------------------------------------------------------------------------------------
void SaveNewSurface(ResFile F) {
    assert(g_UniversalMap.TRIS.Count() && g_UniversalMap.VERTS.Count() && "Trying to save empty map!");
    if(!g_UniversalMap.TRIS.Count() || !g_UniversalMap.VERTS.Count()) return;
    //x-2 y-2 z-2 Color-3 Shadow-1 Facture-1 nx-1 ny-1 nz-1 stype-1 total: 15
    // Magic:
    int Magic = '7HSM';
    RBlockWrite(F, &Magic, 4);
    // Header:
    int NVerts = g_UniversalMap.VERTS.Count();
    int NTris = g_UniversalMap.TRIS.Count();
    int nSize = 4 + 4 + 4 + (15+10+4-3) * NVerts + 4 * NTris;
    RBlockWrite(F, &nSize, 4);
    RBlockWrite(F, &NVerts, 4);	
    RBlockWrite(F, &NTris, 4);
    // x:
    SurfVertex *V = &g_UniversalMap.VERTS[0];
    short *sbuf = new short[NVerts];
    float *fbuf = new float[NVerts];
    byte *bbuf = new byte[NVerts];
    for(int i = 0; i < NVerts; i++, V++) {
        fbuf[i] = V->x;
    }
    RBlockWrite(F, fbuf, NVerts * sizeof(fbuf[0]));
    // y:
    V = &g_UniversalMap.VERTS[0];
    for(i = 0; i < NVerts; i++, V++) {
        fbuf[i] = V->y;
    }
    RBlockWrite(F, fbuf, NVerts * sizeof(fbuf[0]));
    // z:
    V = &g_UniversalMap.VERTS[0];
    for(i = 0; i < NVerts; i++, V++) {
        fbuf[i] = V->z;
    }		
    RBlockWrite(F, fbuf, NVerts * sizeof(fbuf[0]));
    // Color(b):
    V = &g_UniversalMap.VERTS[0];
    for(i = 0; i < NVerts; i++, V++) {
        bbuf[i] = V->Color & 255;
    }
    RBlockWrite(F, bbuf, NVerts);
    // Color(g):
    V = &g_UniversalMap.VERTS[0];
    for(i = 0; i < NVerts; i++, V++) {
        bbuf[i] = (V->Color >> 8) & 255;
    }
    RBlockWrite(F, bbuf, NVerts);
    // Color(r):
    V = &g_UniversalMap.VERTS[0];
    for(i = 0; i < NVerts; i++, V++) {
        bbuf[i] = (V->Color >> 16) & 255;		
    }
    RBlockWrite(F, bbuf, NVerts);

	V = &g_UniversalMap.VERTS[0];
	for(i = 0; i < NVerts; i++, V++) {
		fbuf[i] = V->u;
	}
	RBlockWrite(F, fbuf, NVerts * sizeof(fbuf[0]));

	V = &g_UniversalMap.VERTS[0];
	for(i = 0; i < NVerts; i++, V++) {
		fbuf[i] = V->v;
	}
	RBlockWrite(F, fbuf, NVerts * sizeof(fbuf[0]));

    // Luminance:
    V = &g_UniversalMap.VERTS[0];
    for(i = 0; i < g_UniversalMap.VERTS.Count(); i++,V++) {
        bbuf[i] = V->Luminance;
    }
    RBlockWrite(F, bbuf, NVerts);
    // nFacture:
    V = &g_UniversalMap.VERTS[0];
    for(i = 0; i < NVerts; i++, V++) {
        bbuf[i] = V->nFacture;
    }
    RBlockWrite(F, bbuf, NVerts);    
    // SurfType & RaceIndex:
    V = &g_UniversalMap.VERTS[0];
    for(int i = 0; i < NVerts; i++, V++) {
        bbuf[i] = int(V->SurfaceType) + int(V->RaceIndex) * 16;
    }
    RBlockWrite(F, bbuf, NVerts);
    // TRIS:
    RBlockWrite(F, &g_UniversalMap.TRIS[0], g_UniversalMap.TRIS.Count() * 4);
    delete[] bbuf;
    delete[] sbuf;
    delete[] fbuf;
}
void SaveNewSurfaceOLD3(ResFile F) {
}
void SaveNewSurfaceOLD2(ResFile F) {
}
void SaveNewSurfaceOLD(ResFile F) {
}

void CreateHeightmap(){
	_clearfp();
	_controlfp(_PC_24|_RC_DOWN|_DN_FLUSH,_MCW_PC|_MCW_RC|_MCW_DN);
	int N=VertInLine*MaxTH;
	memset(THMap,0xFF,N*2);
	g_UniversalMap.FillMapOfHeights();
	_controlfp(_PC_64|_RC_NEAR|_DN_SAVE,_MCW_PC|_MCW_RC|_MCW_DN);
	IWater->Reset();
}
void SelectSurfaceType(bool New);
void LoadNewSurface(ResFile F){
	SelectSurfaceType(true);
	int Nv=0;
	int Ni=0;
	RBlockRead(F,&Nv,4);
	RBlockRead(F,&Ni,4);
	//g_UniversalMap.BackUp();
	g_UniversalMap.Free();
	g_UniversalMap.TRIS.SetCount(Ni, 0);
	SurfVertex V;
	g_UniversalMap.VERTS.Add(V,Nv);
	for(int i=0;i<g_UniversalMap.VERTS.Count();i++){
		SurfVertex* V=&g_UniversalMap.VERTS[i];
		RBlockRead(F,&V->x,2);
		RBlockRead(F,&V->y,2);
		RBlockRead(F,&V->z,2);
		RBlockRead(F,&V->Color,4);
		RBlockRead(F,&V->Luminance,1);
		RBlockRead(F,&V->nFacture,1);
		RBlockRead(F,&V->nx,1);
		RBlockRead(F,&V->ny,1);
		RBlockRead(F,&V->nz,1);
		V->SurfaceType=0;
		int c;
		RBlockRead(F,&c,1);
	}
	RBlockRead(F,&g_UniversalMap.TRIS[0],g_UniversalMap.TRIS.Count() * 4);
	//g_UniversalMap.InitUniversalMapQuad(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	g_UniversalMap.QUADS.Init();
	CreateHeightmap();
}

//-----------------------------------------------------------------------------
// LoadNewSurface2
//-----------------------------------------------------------------------------
int FacturesUsage[256];
int BestFactures[256];
int BestFWeight[256];
void CalsCtdUV(){
	int n=g_UniversalMap.VERTS.Count();
	for(int i=0;i<n;i++){
		SurfMapper::CalcUV(g_UniversalMap.VERTS[i]);
	}
}
void LoadNewSurface2(ResFile F) {
    memset(FacturesUsage,0,sizeof FacturesUsage);
    memset(BestFactures,0,sizeof BestFactures);
    memset(BestFWeight,0,sizeof BestFWeight);
	SelectSurfaceType(true);
	int NVerts = 0, NTris = 0;
	RBlockRead(F, &NVerts, 4);
	RBlockRead(F, &NTris, 4);
	assert(NVerts && NTris && "Trying to load empty map!");
	//g_UniversalMap.BackUp();
	g_UniversalMap.Free();
	g_UniversalMap.TRIS.SetCount(NTris, 0);
	SurfVertex u;
	g_UniversalMap.VERTS.Add(u, NVerts);
	short *sbuf = new short[NVerts];
	byte *bbuf = new byte[NVerts];
	// x:
	SurfVertex *V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, sbuf, NVerts * sizeof(sbuf[0]));
	for(int i = 0; i < NVerts; i++, V++) {
		V->x = sbuf[i];
        V->Rotation = 0;
	}
	// y:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, sbuf, NVerts * sizeof(sbuf[0]));
	for(i = 0; i < NVerts; i++, V++) {
		V->y = sbuf[i];
	}
	// z:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, sbuf, NVerts * sizeof(sbuf[0]));
	for(i = 0; i < NVerts; i++, V++) {
		V->z = sbuf[i];
	}
	// Color(b):
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		((BYTE*)(&V->Color))[0] = bbuf[i];
	}
	// Color(g):
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		((BYTE*)(&V->Color))[1] = bbuf[i];
	}
	// Color(r):
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		((BYTE*)(&V->Color))[2] = bbuf[i];
	}
	// Luminance:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->Luminance = bbuf[i];
	}
	// nFacture:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->nFacture = bbuf[i];
        FacturesUsage[V->nFacture]++;
	}
	// nx:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->nx = bbuf[i];
	}
	// ny:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->ny = bbuf[i];
	}
	// nz:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->nz = bbuf[i];
	}
	// SurfaceType & RaceIndex:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(int i = 0; i < NVerts; i++, V++) {
		V->SurfaceType = bbuf[i] & 15;
		V->RaceIndex = bbuf[i] >> 4;
	}

	delete[] sbuf;
	delete[] bbuf;

    int nfUsed=0;
    for(int i=0;i<256;i++){
        if(FacturesUsage[i])nfUsed++;
        BestFactures[i]=i;
        BestFWeight[i]=FacturesUsage[i];
    }
    bool ch=false;
    do{
        ch=false;
        for(int i=1;i<256;i++){
            if(BestFWeight[i]>BestFWeight[i-1]){
                swap(BestFWeight[i],BestFWeight[i-1]);
                swap(BestFactures[i],BestFactures[i-1]);
                ch=true;
            }
        }
    }while(ch);
    /*
    int vb=BestFWeight[16];
    for(int i=0;i<NVerts;i++){
        SurfVertex& V=g_UniversalMap.VERTS[i];
        if(FacturesUsage[V.nFacture]>vb){
            V.nFacture=0;
            V.Color=0xFF008000;
        }
    }
    */
	// TRIS:
	RBlockRead(F, &g_UniversalMap.TRIS[0], 4 * NTris);
	g_UniversalMap.QUADS.Init();
}
void LoadNewSurface3(ResFile F) {
	SelectSurfaceType(true);
	int NVerts = 0, NTris = 0;
	RBlockRead(F, &NVerts, 4);
	RBlockRead(F, &NTris, 4);
	assert(NVerts && NTris && "Trying to load empty map!");
	//g_UniversalMap.BackUp();
	g_UniversalMap.Free();
	g_UniversalMap.TRIS.SetCount(NTris, 0);
	SurfVertex u;
	g_UniversalMap.VERTS.Add(u, NVerts);
	short *sbuf = new short[NVerts];
    float *fbuf = new float[NVerts];
	byte *bbuf = new byte[NVerts];
	// x:
	SurfVertex *V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
	for(int i = 0; i < NVerts; i++, V++) {
		V->x = fbuf[i];
	}
	// y:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
	for(i = 0; i < NVerts; i++, V++) {
		V->y = fbuf[i];
	}
	// z:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
	for(i = 0; i < NVerts; i++, V++) {
		V->z = fbuf[i];
	}
	// Color(b):
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		((BYTE*)(&V->Color))[0] = bbuf[i];
	}
	// Color(g):
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		((BYTE*)(&V->Color))[1] = bbuf[i];
	}
	// Color(r):
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		((BYTE*)(&V->Color))[2] = bbuf[i];
	}
	// Luminance:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->Luminance = bbuf[i];
	}
	// nFacture:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->nFacture = bbuf[i];
	}
	// nx:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->nx = bbuf[i];
	}
	// ny:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->ny = bbuf[i];
	}
	// nz:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(i = 0; i < NVerts; i++, V++) {
		V->nz = bbuf[i];
	}
	// SurfaceType & RaceIndex:
	V = &g_UniversalMap.VERTS[0];
	RBlockRead(F, bbuf, NVerts);
	for(int i = 0; i < NVerts; i++, V++) {
		V->SurfaceType = bbuf[i] & 15;
		V->RaceIndex = bbuf[i] >> 4;
	}

	delete[] sbuf;
    delete[] fbuf;
	delete[] bbuf;

	// TRIS:
	RBlockRead(F, &g_UniversalMap.TRIS[0], 4 * NTris);
	g_UniversalMap.QUADS.Init();
}
void LoadNewSurface4(ResFile F) {
    SelectSurfaceType(true);
    int NVerts = 0, NTris = 0;
    RBlockRead(F, &NVerts, 4);
    RBlockRead(F, &NTris, 4);
    assert(NVerts && NTris && "Trying to load empty map!");
    //g_UniversalMap.BackUp();
    g_UniversalMap.Free();
    g_UniversalMap.TRIS.SetCount(NTris, 0);
    SurfVertex u;
    g_UniversalMap.VERTS.Add(u, NVerts);
    short *sbuf = new short[NVerts];
    float *fbuf = new float[NVerts];
    byte *bbuf = new byte[NVerts];
    // x:
    SurfVertex *V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
    for(int i = 0; i < NVerts; i++, V++) {
        V->x = fbuf[i];
    }
    // y:
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
    for(i = 0; i < NVerts; i++, V++) {
        V->y = fbuf[i];
    }
    // z:
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
    for(i = 0; i < NVerts; i++, V++) {
        V->z = fbuf[i];
    }
    // Color(b):
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(i = 0; i < NVerts; i++, V++) {
        ((BYTE*)(&V->Color))[0] = bbuf[i];
    }
    // Color(g):
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(i = 0; i < NVerts; i++, V++) {
        ((BYTE*)(&V->Color))[1] = bbuf[i];
    }
    // Color(r):
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(i = 0; i < NVerts; i++, V++) {
        ((BYTE*)(&V->Color))[2] = bbuf[i];
    }
    //rotation
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(i = 0; i < NVerts; i++, V++) {
        V->Rotation = bbuf[i];
    }
    // Luminance:
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(i = 0; i < NVerts; i++, V++) {
        V->Luminance = bbuf[i];
    }
    // nFacture:
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(i = 0; i < NVerts; i++, V++) {
        V->nFacture = bbuf[i];
    }
    // nx:
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(i = 0; i < NVerts; i++, V++) {
        V->nx = bbuf[i];
    }
    // ny:
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(i = 0; i < NVerts; i++, V++) {
        V->ny = bbuf[i];
    }
    // nz:
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(i = 0; i < NVerts; i++, V++) {
        V->nz = bbuf[i];
    }
    // SurfaceType & RaceIndex:
    V = &g_UniversalMap.VERTS[0];
    RBlockRead(F, bbuf, NVerts);
    for(int i = 0; i < NVerts; i++, V++) {
        V->SurfaceType = bbuf[i] & 15;
        V->RaceIndex = bbuf[i] >> 4;
    }

    delete[] sbuf;
    delete[] fbuf;
    delete[] bbuf;

    // TRIS:
    RBlockRead(F, &g_UniversalMap.TRIS[0], 4 * NTris);
    g_UniversalMap.QUADS.Init();
	CalsCtdUV();
	g_UniversalMap.CalcNormals();
}
// This function is used in Surf BackUp process, so you should replace it there!
void LoadNewSurface5(ResFile F) {
    SelectSurfaceType(true);
    int NVerts = 0, NTris = 0;
    RBlockRead(F, &NVerts, 4);
    RBlockRead(F, &NTris, 4);
//    assert(NVerts && NTris && "Trying to load empty map!");
    //g_UniversalMap.BackUp();
    g_UniversalMap.Free();
	if(NVerts && NTris) {
		g_UniversalMap.TRIS.SetCount(NTris, 0);
		SurfVertex u;
		g_UniversalMap.VERTS.Add(u, NVerts);
		short *sbuf = new short[NVerts];
		float *fbuf = new float[NVerts];
		byte *bbuf = new byte[NVerts];
		// x:
		SurfVertex *V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
		for(int i = 0; i < NVerts; i++, V++) {
			V->x = fbuf[i];
		}
		// y:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
		for(i = 0; i < NVerts; i++, V++) {
			V->y = fbuf[i];
		}
		// z:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
		for(i = 0; i < NVerts; i++, V++) {
			V->z = fbuf[i];
		}
		// Color(b):
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			((BYTE*)(&V->Color))[0] = bbuf[i];
		}
		// Color(g):
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			((BYTE*)(&V->Color))[1] = bbuf[i];
		}
		// Color(r):
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			((BYTE*)(&V->Color))[2] = bbuf[i];
		}
		//rotation
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->Rotation = bbuf[i];
		}
		//scale
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->Scale = bbuf[i];
		}
		//dU
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->dU = bbuf[i];
		}
		//dV
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->dV = bbuf[i];
		}
		// Luminance:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->Luminance = bbuf[i];
		}
		// nFacture:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->nFacture = bbuf[i];
		}
		// nx:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->nx = bbuf[i];
		}
		// ny:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->ny = bbuf[i];
		}
		// nz:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->nz = bbuf[i];
		}
		// SurfaceType & RaceIndex:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(int i = 0; i < NVerts; i++, V++) {
			V->SurfaceType = bbuf[i] & 15;
			V->RaceIndex = bbuf[i] >> 4;
		}

		delete[] sbuf;
		delete[] fbuf;
		delete[] bbuf;

		// TRIS:
		RBlockRead(F, &g_UniversalMap.TRIS[0], 4 * NTris);
	}
    g_UniversalMap.QUADS.Init();
	CalsCtdUV();
	g_UniversalMap.CalcNormals();
}
void LoadNewSurface6(ResFile F,bool Conv) {
	SelectSurfaceType(true);
	int NVerts = 0, NTris = 0;
	RBlockRead(F, &NVerts, 4);
	RBlockRead(F, &NTris, 4);
	//    assert(NVerts && NTris && "Trying to load empty map!");
	//g_UniversalMap.BackUp();
	g_UniversalMap.Free();
	if(NVerts && NTris) {
		g_UniversalMap.TRIS.SetCount(NTris, 0);
		SurfVertex u;
		g_UniversalMap.VERTS.Add(u, NVerts);
		short *sbuf = new short[NVerts];
		float *fbuf = new float[NVerts];
		byte *bbuf = new byte[NVerts];
		// x:
		SurfVertex *V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
		for(int i = 0; i < NVerts; i++, V++) {
			V->x = fbuf[i];
		}
		// y:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
		for(i = 0; i < NVerts; i++, V++) {
			V->y = fbuf[i];
		}
		// z:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
		for(i = 0; i < NVerts; i++, V++) {
			V->z = fbuf[i];
		}
		// Color(b):
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			((BYTE*)(&V->Color))[0] = bbuf[i];
		}
		// Color(g):
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			((BYTE*)(&V->Color))[1] = bbuf[i];
		}
		// Color(r):
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			((BYTE*)(&V->Color))[2] = bbuf[i];
		}
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
		for(i = 0; i < NVerts; i++, V++) {
			V->u = fbuf[i];
		}
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, fbuf, NVerts * sizeof(fbuf[0]));
		for(i = 0; i < NVerts; i++, V++) {
			V->v = fbuf[i];
		}		
		// Luminance:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->Luminance = bbuf[i];
		}
		// nFacture:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(i = 0; i < NVerts; i++, V++) {
			V->nFacture = bbuf[i];
		}		
		// SurfaceType & RaceIndex:
		V = &g_UniversalMap.VERTS[0];
		RBlockRead(F, bbuf, NVerts);
		for(int i = 0; i < NVerts; i++, V++) {
			V->SurfaceType = bbuf[i] & 15;
			V->RaceIndex = bbuf[i] >> 4;
		}
		if(Conv){
			V = &g_UniversalMap.VERTS[0];
			for(int i=0;i<NVerts;i++){
                SurfMapper m(V->nFacture);
				V->u/=m.uScale*m.uScale;
				V->v/=m.vScale*m.vScale;
				V++;
			}
		}

		delete[] sbuf;
		delete[] fbuf;
		delete[] bbuf;

		// TRIS:
		RBlockRead(F, &g_UniversalMap.TRIS[0], 4 * NTris);
	}
	g_UniversalMap.QUADS.Init();
	g_UniversalMap.CalcNormals();
}
void SaveSurfaceChangeToSB(SaveBuf* SB){
    int NVert=g_UniversalMap.VERTS.Count();
    byte *bbuf = new byte[NVert];
    DWORD *dbuf = new DWORD[NVert];
    xBlockWrite(SB,&NVert,4);
    for(int i=0;i<NVert;i++){
        SurfVertex& sv=g_UniversalMap.VERTS[i];
        bbuf[i]=sv.nFacture;
    }
    xBlockWrite(SB,bbuf,NVert);
    for(int i=0;i<NVert;i++){
        SurfVertex& sv=g_UniversalMap.VERTS[i];
        dbuf[i]=sv.Color;
        bbuf[i]=sv.SurfaceType+sv.RaceIndex*16;
    }
    xBlockWrite(SB,dbuf,NVert*4);
    xBlockWrite(SB,bbuf,NVert);
    delete[]bbuf;
    delete[]dbuf;
    for(int i=0;i<MLayers->Maps.GetAmount();i++){
        if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
            GreyMap* BPM=(GreyMap*)MLayers->Maps[i];
            int lx,ly;
            BPM->GetSize(lx,ly);            
            int sz=lx*ly/8;			
            xBlockWrite(SB,BPM->V.GetValues(),sz);
        }
    }
}
void LoadSurfaceFromSB(SaveBuf* SB){
    int NVert=g_UniversalMap.VERTS.Count();
    int SNVert;
    xBlockRead(SB,&SNVert,4);
    byte *bbuf = new byte[SNVert];
    DWORD *dbuf = new DWORD[SNVert];
    xBlockRead(SB,bbuf,SNVert);
    if(NVert>SNVert)NVert=SNVert;
    for(int i=0;i<NVert;i++){
        SurfVertex& sv=g_UniversalMap.VERTS[i];
        sv.nFacture=bbuf[i];
    }
    xBlockRead(SB,dbuf,SNVert*4);
    xBlockRead(SB,bbuf,SNVert);
    for(int i=0;i<NVert;i++){
        SurfVertex& sv=g_UniversalMap.VERTS[i];
        sv.Color=dbuf[i];
        sv.SurfaceType=dbuf[i]&15;
        sv.RaceIndex=bbuf[i]>>4;
    }
    delete[]bbuf;
    delete[]dbuf;
    for(int i=0;i<MLayers->Maps.GetAmount();i++){
        if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
            GreyMap* BPM=(GreyMap*)MLayers->Maps[i];
            int lx,ly;
            BPM->GetSize(lx,ly);            
            int sz=lx*ly/8;			
            xBlockRead(SB,BPM->V.GetValues(),sz);
        }
    }
}
LayersList LAYERSLIST;
void ReadLayers(){
	LAYERSLIST.reset_class(&LAYERSLIST);
	LAYERSLIST.SafeReadFromFile("Surface\\Layers.xml");
}
void TestLayersColors(){	
	if(LAYERSLIST.Layers.GetAmount()&&MLayers){
		for(int i=0;i<MLayers->Maps.GetAmount();i++){
			if(i<LAYERSLIST.Layers.GetAmount()){
				((GreyMap*)MLayers->Maps[i])->Color=LAYERSLIST.Layers[i]->Color;
				*MLayers->MapsNames[i]=LAYERSLIST.Layers[i]->Name;
			}
		}
		for(int i=MLayers->Maps.GetAmount();i<LAYERSLIST.Layers.GetAmount();i++){
			MLayers->Maps.Add(new GreyMap(LAYERSLIST.Layers[i]->Color));
			_str* s=new _str;
			*s=LAYERSLIST.Layers[i]->Name;
			MLayers->MapsNames.Add(s);
		}
		Enumerator* E1=ENUM.Get("LAYERSLIST");
		E1->Clear();
		for(int i=0;i<MLayers->Maps.GetAmount();i++){
			char* s=MLayers->MapsNames[i]->pchar();
			E1->Add(s?s:"???");
		}
	}
	LAYERSLIST.CreateBoundFactures();
	LAYERSLIST.CreateLayerIndex();
}
void AddLayersEditor(){
	AddStdEditor("MapLayersEditor",&LAYERSLIST,"Surface\\Layers.xml",RCE_DEFAULT);
}

float CheckIfNearLayer::GetConditionWeight(PrecompiledVertex* V){	
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	float w=DistField::Get(Layer,float(x)*surfScale,float(y)*surfScale);
	int D=fabs(w*16);	
	if(StartDistance!=FinalDistance){
        float ww=(FinalDistance-D)*255/(FinalDistance-StartDistance);
		if(ww>255)ww=255;
		if(ww<0)ww=0;
		return ww/255.0f;
	}	
	return 0.0f;
}

SurfaceGenerator SURF;

float CheckIfNotInLayer::GetConditionWeight(PrecompiledVertex* V){
	BasePaintMap* BPM=MLayers->Maps[Layer];
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	SURF.ShapeOfBound.ShiftXY(x,y);
	return (255-BPM->GetGreyPrecise(x*surfScale,y*surfScale))/255.0f;
}
float CheckSpecialLayer::RaceLayerWeight=0;
int   CheckSpecialLayer::BestRaceLayerIndex=0;
float CheckSpecialLayer::GetConditionWeight(PrecompiledVertex* V){
	BasePaintMap* BPM=MLayers->Maps[Layer];
	int x=V->Vertex->x;
	int y=V->Vertex->y;
	SURF.ShapeOfBound.ShiftXY(x,y);

	float v1=BPM->GetGreyPrecise((x-80)*surfScale,y*surfScale);
	float v2=BPM->GetGreyPrecise((x+80)*surfScale,y*surfScale);
	float v3=BPM->GetGreyPrecise(x*surfScale,(y+80)*surfScale);
	float v4=BPM->GetGreyPrecise(x*surfScale,(y-80)*surfScale);

	float w=(v1+v2+v3+v4)/4/255.0f;
	if(w>RaceLayerWeight && LAYERSLIST.Layers[Layer]->ThisLayerDeterminesRace){
		RaceLayerWeight=w;
		BestRaceLayerIndex=Layer;
	}
	return w;
}
float CheckSpecialSurfaceType::GetConditionWeight(PrecompiledVertex* V){
	return float(V->Vertex->SurfaceType==SurfaceType);
}


float CheckVariatedHeightAndAngle::GetConditionWeight(PrecompiledVertex* V){
	float af,hf;
	int DH=VariatedAddHeight*VariationLow.GetFValue(V->Vertex->x,V->Vertex->y);
	int H0=StartHeight+DH;
	int H1=FinalHeight+DH;
	if(V->Height>=H0&&V->Height<=H1)hf=1.0f;
	else{
		if(HeightDiff){
			if(V->Height<H1&&V->Height>H0-HeightDiff){
				hf=1-float(H0-V->Height)/HeightDiff;
			}else
				if(V->Height>H1&&V->Height<H1+HeightDiff){
					hf=1-float(V->Height-H1)/HeightDiff;
				}else hf=0.0f;
		}else hf=0.0f;
	}
	if(V->Angle>=StartAngle&&V->Angle<=FinalAngle)af=1.0f;
	else{
		if(AngleDiff){
			if(V->Angle<StartAngle&&V->Angle>StartAngle-AngleDiff){
				af=1-float(StartAngle-V->Angle)/AngleDiff;
			}else
				if(V->Angle>FinalAngle&&V->Angle<FinalAngle+AngleDiff){
					af=1-float(V->Angle-FinalAngle)/AngleDiff;
				}else af=0.0f;
		}else af=0.0f;
	}
	return hf*af;
}

float CheckHeight::GetConditionWeight(PrecompiledVertex* V){
	float hf;
	if(V->Height>=StartHeight&&V->Height<=FinalHeight)hf=1.0f;
	else{
		if(HeightDiff){
			if(V->Height<StartHeight&&V->Height>StartHeight-HeightDiff){
				hf=1-float(StartHeight-V->Height)/HeightDiff;
			}else
				if(V->Height>FinalHeight&&V->Height<FinalHeight+HeightDiff){
					hf=1-float(V->Height-FinalHeight)/HeightDiff;
				}else hf=0.0f;
		}else hf=0.0f;
	}
	return hf;
}

float CheckIfSlopeIsNear::GetConditionWeight(PrecompiledVertex* V){
	int x0=V->Vertex->x;
	int y0=V->Vertex->y;
    int R=RadiusMin+(RadiusMax-RadiusMin)*VariationLow.GetFValue(x0,y0);
	int D=abs(DistField::Get(SpecialDistField,x0/15360.0f,y0/15360.0f))*24;
	if(R<1)R=1;
    if(D>R)return 0;
	float v=1-float(D)/R;
    return v;
}
float CheckHeightAndAngle::GetConditionWeight(PrecompiledVertex* V){
	float af,hf;
	if(V->Height>=StartHeight&&V->Height<=FinalHeight)hf=1.0f;
	else{
		if(HeightDiff){
			if(V->Height<StartHeight&&V->Height>StartHeight-HeightDiff){
				hf=1-float(StartHeight-V->Height)/HeightDiff;
			}else
				if(V->Height>FinalHeight&&V->Height<FinalHeight+HeightDiff){
					hf=1-float(V->Height-FinalHeight)/HeightDiff;
				}else hf=0.0f;
		}else hf=0.0f;
	}
	if(V->Angle>=StartAngle&&V->Angle<=FinalAngle)af=1.0f;
	else{
		if(AngleDiff){
			if(V->Angle<StartAngle&&V->Angle>StartAngle-AngleDiff){
				af=1-float(StartAngle-V->Angle)/AngleDiff;
			}else
				if(V->Angle>FinalAngle&&V->Angle<FinalAngle+AngleDiff){
					af=1-float(V->Angle-FinalAngle)/AngleDiff;
				}else af=0.0f;
		}else af=0.0f;
	}
	return hf*af;
}

float DistField::Get(int Index,float x,float y){
	if(!map[Index]){
		map[Index]=new SquareMap<float>;
		SquareMap<float>* FL=map[Index];
		FL->SetSize(512,512,0);//(512,512,0)
		if(Index==SpecialDistField){
			for(int ix=0;ix<512;ix++){
				for(int iy=0;iy<512;iy++){
					int xx=(ix*15360)/512.0f;//15360
					int yy=(iy*15360)/512.0f;//15360
					int HL=GetHeight(xx-20,yy);
					int HR=GetHeight(xx+20,yy);
					int HU=GetHeight(xx,yy-20);
					int HD=GetHeight(xx,yy+20);
					int dh=Norma(HR-HL,HU-HD);
					if(dh<50)FL->Set(ix,iy,-10000);
					else FL->Set(ix,iy,10000);
				}
			}            
		}else{
			BasePaintMap* BPM=MLayers->Maps[Index];	
			for(int ix=0;ix<512;ix++){
				for(int iy=0;iy<512;iy++){
					float v=BPM->GetGreyPrecise(float(ix)/512.0f,float(iy)/512.0f);
					if(v<128)FL->Set(ix,iy,-10000);
					else FL->Set(ix,iy,10000);
				}
			}
		}
		DynArray<DWORD> Edges;
		Edges.Check(4096);
		for(int ix=0;ix<512;ix++){
			for(int iy=0;iy<512;iy++){
				float v=FL->Get(ix,iy);
				float vL=FL->Get(ix-1,iy);
				float vR=FL->Get(ix+1,iy);
				float vU=FL->Get(ix,iy-1);
				float vD=FL->Get(ix,iy+1);
				bool add=false;
				if(v>5000){
					if(vL<-5000||vR<-5000||vU<-5000||vD<-5000)add=true;
				}else{
					if(vL>5000||vR>5000||vU>5000||vD>5000)add=true;
				}
				if(add){
					DWORD XY=ix+(iy<<16);
					Edges.Add(XY);
				}
			}
		}
		for(int i=0;i<Edges.GetAmount();i++){
			int x=Edges[i]&0xFFFF;
			int y=Edges[i]>>16;
			float v=FL->Get(x,y);
			if(v<0)FL->Set(x,y,-1);
			else FL->Set(x,y,1);
		}
		DynArray<DWORD> NextEdges;
		NextEdges.Check(4096);
		bool change=false;
		float curV=3;
		do{
			change=false;
			for(int i=0;i<Edges.GetAmount();i++){
				int x=Edges[i]&0xFFFF;
				int y=Edges[i]>>16;
				float vL=FL->Get(x-1,y);
				float vR=FL->Get(x+1,y);
				float vU=FL->Get(x,y-1);
				float vD=FL->Get(x,y+1);				
				if(vL<-5000){
					NextEdges.Add((x-1)+(y<<16));
					FL->Set(x-1,y,-curV);
					change=true;
				}
				if(vL>5000){
					NextEdges.Add((x-1)+(y<<16));
					FL->Set(x-1,y,curV);
					change=true;
				}
				if(vR<-5000){
					NextEdges.Add((x+1)+(y<<16));
					FL->Set(x+1,y,-curV);
					change=true;
				}
				if(vR>5000){
					NextEdges.Add((x+1)+(y<<16));
					FL->Set(x+1,y,curV);
					change=true;
				}
				if(vU<-5000){
					NextEdges.Add(x+((y-1)<<16));
					FL->Set(x,y-1,-curV);
					change=true;
				}
				if(vU>5000){
					NextEdges.Add(x+((y-1)<<16));
					FL->Set(x,y-1,curV);
					change=true;
				}
				if(vD<-5000){
					NextEdges.Add(x+((y+1)<<16));
					FL->Set(x,y+1,-curV);
					change=true;
				}
				if(vD>5000){
					NextEdges.Add(x+((y+1)<<16));
					FL->Set(x,y+1,curV);
					change=true;
				}
			}
			if(change){
				Edges.FastClear();
				for(int i=0;i<NextEdges.GetAmount();i++){
					Edges.Add(NextEdges[i]);
				}
				NextEdges.FastClear();
			}
			curV+=2;
		}while(change);
		//smoothing
		SquareMap<float> temp;
		FL->CopyTo(temp);
		for(int i=0;i<2;i++){		
			for(int ix=1;ix<511;ix++){
				for(int iy=1;iy<511;iy++){
					float va=(FL->Get(ix-1,iy)+FL->Get(ix+1,iy)+FL->Get(ix,iy-1)+FL->Get(ix,iy+1))/4;
					temp.Set(ix,iy,va);
				}			
			}
			temp.CopyTo(*FL);
		}
	}
	//map  created, now we can get value
	SquareMap<float>* FL=map[Index];
	int lx=512;
	int ly=512;
	x*=lx;
	y*=ly;
	int xx=int(x);
	int yy=int(y);
	float f1=FL->Get(xx,yy);
	float f2=FL->Get(xx+1,yy);
	float f3=FL->Get(xx,yy+1);
	float f4=FL->Get(xx+1,yy+1);
	float dx=x-xx;
	float dy=y-yy;
	return f1+dx*(f2-f1)+dy*(f3-f1)+dx*dy*(f4+f1-f3-f2);    
}

SquareMap<float>* DistField::map[MaxDistSet];

float OneSurfaceRule::GetHeight(int x,int y,float& Weight,int z){
	/////
	int H=TopHeight;
	if(AllowModulation&&!ModulateAfterTexturing){
		H*=ModulationFactor*ModulationVariation.GetFValue(x,y)*GlobalModulationVariation.GetFValue(x,y);
	}
	bool specwidth = VariateWidth && SetWidthInSpecialLayer && MLayers->Maps[SpecialLayer]->GetGreyPrecise(float(x)*surfScale,float(y)*surfScale)>128;

	if(AllowRandomShift && !specwidth){
		float wL=DistField::Get(Map,float(x-64)*surfScale,float(y)*surfScale);
		float wR=DistField::Get(Map,float(x+64)*surfScale,float(y)*surfScale);
		float wU=DistField::Get(Map,float(x)*surfScale,float(y-64)*surfScale);
		float wD=DistField::Get(Map,float(x)*surfScale,float(y+64)*surfScale);

		float nx=(wR-wL)/4;
		float ny=(wD-wU)/4;

		float nn=sqrt(nx*nx+ny*ny);
		if(nn>0){
			nx/=nn;
			ny/=nn;
		}
		float dr=(RandomShiftFractal.GetFValue(x+y,x-y)-0.5f)*DegreeOfRandomShift*RandomShiftModulator;		
		extern int zcur;
		float dz=0;
		if(H>0){
			int HBase=30;
			int H2=HBase+H;
			dz=H2!=0?(zcur)*(H2-zcur)*2/H2:0;
			if(dz<0)dz=0;
		}else{
			dz = H != 0 ? zcur*zcur/H : 0;
		}
		x-=nx*dr*dz;
		y-=ny*dr*dz;
	}	
	/////
	Weight=0;
	if(!Enabled)return 0;	
	int dx=x;
	int dy=y;
	if(AllowShapeCurving && !specwidth){
		ShapeCurving.ShiftXY(dx,dy);
		dx-=x;
		dy-=y;
	}else{
		dx=0;
		dy=0;
	}
	float bw=0.0f;
	float cBoundWidth=BoundWidth;
	if(VariateWidth){				
		if(specwidth){
            cBoundWidth=WidthInSpecialLayer;
		}else{			
			bw=WidthVariationLow.GetFValue(x,y);
			cBoundWidth=cBoundWidth*(1-bw)+bw*ChangedWidth;
		}
		if(AllowShapeCurving){
			dx*=(ShapeCurvingModulator*bw+1-bw);
			dy*=(ShapeCurvingModulator*bw+1-bw);
		}		
	}
	float w=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy)*surfScale);

	if(cBoundWidth<1)cBoundWidth=1;	
	Weight=0.5f+float(w)*16/cBoundWidth;
	if(Weight<0.0f)Weight=0.0f;
	if(Weight>1.0f)Weight=1.0f;
	Weight=Weight*Weight*(2-Weight)*(2-Weight);
	int N=ExtraConditions.GetAmount();
	if(N){
		PrecompiledVertex V;
		V.Angle=0;
		V.Azimut=0;
		SurfVertex MV;
		MV.x=x;
		MV.y=y;
		MV.z=z;
		V.Vertex=&MV;
		V.VertexIndex=0;

		for(int i=0;i<N;i++){			
			V.Height=z;
			float WW=ExtraConditions[i]->GetConditionWeight(&V);
			Weight*=WW;
		}
	}
	return H;	
}

float OneSurfaceRule::PerformShift(Vector3D V,Vector3D& DV){//returns weight of vector shift
	int x=V.x;
	int y=V.y;
	DV=Vector3D(0,0,0);
	if(!(Enabled&&(AllowRandomShift||ModulateAfterTexturing)))return 0;	
	int dx=x;
	int dy=y;
	if(AllowShapeCurving){
		ShapeCurving.ShiftXY(dx,dy);
		dx-=x;
		dy-=y;
	}else{
		dx=0;
		dy=0;
	}
	float bw=0.0f;
	float cBoundWidth=BoundWidth;
	float cDegreeOfRandomShift=DegreeOfRandomShift;
	if(VariateWidth){		
		bw=WidthVariationLow.GetFValue(x,y);
		cBoundWidth=cBoundWidth*(1-bw)+bw*ChangedWidth;
		if(AllowShapeCurving){
			dx*=(ShapeCurvingModulator*bw+1-bw);
			dy*=(ShapeCurvingModulator*bw+1-bw);
		}
		if(AllowRandomShift){
			cDegreeOfRandomShift*=(RandomShiftModulator*bw+1-bw);
		}
	}

	float w=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy)*surfScale);	

	float Weight=0.0f;
	if(cBoundWidth<1)cBoundWidth=1;	
	Weight=0.5f+float(w)*16/cBoundWidth;
	if(Weight<0.0f)Weight=0.0f;
	if(Weight>1.0f)Weight=1.0f;
	//Weight=Weight*Weight*(2-Weight)*(2-Weight);

	if(AllowModulation&&ModulateAfterTexturing){
		DV.z=V.z*Weight*(ModulationFactor*ModulationVariation.GetFValue(x,y)*GlobalModulationVariation.GetFValue(x,y)-1.0f);
	}
	/*
	if(AllowRandomShift){
		float wL=DistField::Get(Map,float(x+dx-64)*surfScale,float(y+dy)*surfScale);
		float wR=DistField::Get(Map,float(x+dx+64)*surfScale,float(y+dy)*surfScale);
		float wU=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy-64)*surfScale);
		float wD=DistField::Get(Map,float(x+dx)*surfScale,float(y+dy+64)*surfScale);

		float nx=(wR-wL)/4;
		float ny=(wD-wU)/4;

		float nn=sqrt(nx*nx+ny*ny);
		if(nn>0){
			nx/=nn;
			ny/=nn;
		}

		float w1=1.0f-Weight;
		w1=w1*w1;

		float dr=RandomShiftFractal.GetFValue(x+y,x-y)-0.5f;

		DV.x+=Weight*w1*Weight*w1*dr*nx*Weight*cDegreeOfRandomShift*8000;
		DV.y+=Weight*w1*Weight*w1*dr*ny*Weight*cDegreeOfRandomShift*8000;
	}	
	*/
	return Weight;
}

float SurfaceGenerator::GetHeight(int x,int y){
	float SumH=0;
	float SumW=0;
	for(int i=0;i<Relief.GetAmount();i++)if(Relief[i]->Enabled){
		OneSurfaceType* OST=Relief[i];
		float SumHC=0;
		float SumWC=0;	
		int sr=OST->SurfaceRules.GetAmount();
		int ns=0;
		for(int j=0;j<sr;j++)if(OST->SurfaceRules[j]->Enabled)ns++;
		for(int j=0;j<sr;j++)if(OST->SurfaceRules[j]->Enabled){
			OneSurfaceRule* OSR=OST->SurfaceRules[j];
			float W=0;
			int zz=0;
			if(SumWC>0.01){
				zz=SumHC/SumWC;
			}
			float H=OSR->GetHeight(x,y,W,zz);
			SumHC+=H*W*ns+OST->BasicHeight*(1-W);
			SumWC++;           
		}
		if(SumWC<0.001){
			SumWC=1;
			SumHC=OST->BasicHeight;
		}
		if(OST->UseMap){
			BasePaintMap* BPM=MLayers->Maps[OST->Map];			
			float w=BPM->GetGreyPrecise(float(x)*surfScale,float(y)*surfScale);
			SumHC*=float(w)/255;
			SumWC*=float(w)/255;
		}
		SumH+=SumHC;
		SumW+=SumWC;
	}
	if(SumW<0.001)SumW=1;
	return SumH/SumW;
}

bool SurfaceGenerator::PerformShift(Vector3D& V){
	Vector3D SumV=Vector3D(0,0,0);
	float SumW=0;
	for(int i=0;i<Relief.GetAmount();i++)if(Relief[i]->Enabled){
		OneSurfaceType* OST=Relief[i];
		Vector3D SumVC=Vector3D(0,0,0);
		float SumWC=0;		
		for(int j=0;j<OST->SurfaceRules.GetAmount();j++)if(OST->SurfaceRules[j]->Enabled){
			OneSurfaceRule* OSR=OST->SurfaceRules[j];
			Vector3D DV=Vector3D(0,0,0);
			float W=OSR->PerformShift(V,DV);
			DV*=W;
			SumVC+=DV;
			SumWC+=W;           
		}
		if(SumWC<0.001){
			SumWC=1;			
		}
		if(OST->UseMap){
			BasePaintMap* BPM=MLayers->Maps[OST->Map];
			float w=BPM->GetGreyPrecise(V.x*surfScale,V.y*surfScale);
			SumVC*=float(w)/255;
			SumWC*=float(w)/255;
		}
		SumV+=SumVC;
		SumW+=SumWC;
	}
	if(SumW<0.001)SumW=1;
	SumV/=SumW;
	V+=SumV;
	return true;
}
int zcur;
void GeneratorSurfaceFunction::ModifyVertex(SurfVertex& V)
{
	zcur=V.z;
	V.z=GPtr->GetHeight(V.x,V.y)+GPtr->ExHeight.get(V.x/15360.0,V.y/15360.0)*GPtr->HeightmapScale/255.0;
}

bool GeneratorSurfaceFunction::ShiftVertex(SurfVertex& V){
	Vector3D VV=Vector3D(V.x,V.y,V.z);
	GPtr->PerformShift(VV);
	//assert(VV.x>-200);
	//assert(VV.y>-200);
	V.x=VV.x;		
	V.y=VV.y;
	V.z=VV.z;
	//assert(V.x>-200);
	//assert(V.y>-200);
	return true;
}

float sq_norma(float x,float y);
void PrecompiledVertex::CreateFromVertex(SurfVertex& V){
	Height=V.z;
	Angle=atan2(sq_norma(V.nx,V.ny),float(V.nz))*180/3.1415;
	Vertex=&V;
}
bool WholeMapGenerationPass=false;
void SurfaceGenerator::FillVertexTextures(PrecompiledVertex* V){
	int x=V->Vertex->x;
	int y=V->Vertex->y;
    int z=V->Vertex->z;
	float R=0;
	float G=0;
	float B=0;
	float SummW=0;
	int FacturesWeight[80];
	OneTexturingRule* Rules[80];
	memset(&FacturesWeight,0,sizeof FacturesWeight);
	memset(&Rules,0,sizeof Rules);
	int MinFacture=127;
	int MaxFacture=0;

	CheckSpecialLayer::BestRaceLayerIndex=-1;
	CheckSpecialLayer::RaceLayerWeight=0;
	V->Vertex->RaceIndex=0;

	for(int i=0;i<Texturing.GetAmount();i++)if(Texturing[i]->Enabled){
		float cr=1.0f;
		OneTexturingStyle* OTS=Texturing[i];
		float sw=1.0f;		

		for(int q=0;q<OTS->Conditions.GetAmount();q++){
			sw*=OTS->Conditions[q]->GetConditionWeight(V);
		}		
		if(CheckSpecialLayer::BestRaceLayerIndex!=-1){
			V->Vertex->RaceIndex=CheckSpecialLayer::BestRaceLayerIndex-LAYERSLIST.MinRaceLandIndex+1;
		}
		//if(OTS->UseMap){
		//	BasePaintMap* BPM=MLayers->Maps[OTS->Map];
		//	sw=BPM->GetGreyPrecise(x*surfScale,y*surfScale);
		//	sw=sw*sw;
		//}		
		if(sw>0.1){
			sw*=sw*sw*sw*sw;
			for(int j=0;j<OTS->TexturingRules.GetAmount();j++){
				OneTexturingRule* OTR=OTS->TexturingRules[j];
				if(OTR->Enabled&&OTR->Textures.GetAmount()){
					float w0=1.0f;
					for(int q=0;q<OTR->Conditions.GetAmount();q++){
						w0*=OTR->Conditions[q]->GetConditionWeight(V);
						if(w0<0.01)break;
					}					
					if(w0>0.01){
						w0*=sw;
						if(OTR->UseGlobalWeight){
							w0*=OTR->GlobalWeight*OTR->GlobalWeightVariation.GetFValue(x,y);
						}
						if(w0>0.001){
							int T1,T2;
							DWORD C1,C2;
							float w1,w2;
                            int NT=OTR->Textures.GetAmount();
							if(NT>1){
								T1=OTR->Textures[0]->Facture;
								C1=OTR->Textures[0]->Color;
                                int TI=1+mrand()%(NT-1);
								T2=OTR->Textures[TI]->Facture;
								C2=OTR->Textures[TI]->Color;								
                                if(OTR->Use3DMixing){
                                    float ww1=OTR->TexturesMixing.GetFValue(x*4,y*4);
                                    float ww2=OTR->TexturesMixing.GetFValue(x*4,z*4);
                                    float ww3=OTR->TexturesMixing.GetFValue(z*4,y*4);
								    w1=(ww1+ww2+ww3)/3;
                                    if(w1<0)w1=0;
                                    if(w1>1)w1=1;
                                }else{
                                    w1=OTR->TexturesMixing.GetFValue(x,y);
                                }
								w2=1.0f-w1;
							}else{
								//one texture
								T1=T2=OTR->Textures[0]->Facture;
								C1=C2=OTR->Textures[0]->Color;
								w1=0;
								w2=1.0f;
							}
							float CB=(C1&255)*w1+(C2&255)*w2;
							float CG=((C1>>8)&255)*w1+((C2>>8)&255)*w2;
							float CR=((C1>>16)&255)*w1+((C2>>16)&255)*w2;
							R+=CR*w0;
							G+=CG*w0;
							B+=CB*w0;
							SummW+=w0;
							assert(T1<128&&T2<128);
							FacturesWeight[T1]+=w0*w1*1000;
							FacturesWeight[T2]+=w0*w2*1000;
							if(T1<MinFacture)MinFacture=T1;
							if(T2<MinFacture)MinFacture=T2;
							if(T1>MaxFacture)MaxFacture=T1;
							if(T2>MaxFacture)MaxFacture=T2;
							Rules[T1]=OTR;
							Rules[T2]=OTR;							
						}
					}
				}
			}
		}
	}
	if(SummW>0.001){
		R/=SummW;
		G/=SummW;
		B/=SummW;
		if(R>255)R=255;
		if(G>255)G=255;
		if(B>255)B=255;
		int fc=0;
		int maxw=0;
		OneTexturingRule* OTR=NULL;
		for(int i=MinFacture;i<=MaxFacture;i++){
			if(FacturesWeight[i]>maxw){
				maxw=FacturesWeight[i];
				fc=i;
				OTR=Rules[i];
			}
		}
		V->Vertex->Color=0xFF000000+(int(R)<<16)+(int(G)<<8)+int(B);
		V->Vertex->nFacture=fc;		
		SurfMapper::CalcUV(*V->Vertex);		
		if(OTR){
			float fv=0;
            if(!DontGenerateTrees){
			    if(WholeMapGenerationPass && OTR->EnableTreesGenertion && OTR->Trees.GetAmount() && (fv=OTR->TreesDistribution.GetFValue(x,y))>=0.499){
				    int xx=x+(mrand()%7-3);
				    int yy=y+(mrand()%7-3);
				    if(abs(GetHeight(x,y)-V->Vertex->z)<20){
					    int p=mrand()%OTR->Trees.GetAmount();
					    OneTreeToGenerate* OTG=OTR->Trees[p];
					    void AddSpriteScaled(int x,int y,SprGroup* SG,int id,int Radius,float Scale,float ExtraRotation,float);
					    float es=(fv-0.5)*2;
					    es=OTG->ExtraScaleOnFractalBound*(1-es)+es;
					    AddSpriteScaled(xx,yy,&TREES,OTG->TreeID,OTG->Radius,
						    (OTG->MinScale+(OTG->MaxScale-OTG->MinScale)*mrand()/32768)*es
						    ,OTG->RandomAngle*((mrand()%256)-128)/128.0f,0);
				    }
			    }
            }
		}
	}
}

// SurfaceGenerator::TexturizeVertex
void SurfaceGenerator::TexturizeVertex(SurfVertex &r) {
	PrecompiledVertex pv;
	pv.CreateFromVertex(r);
	FillVertexTextures(&pv);
}

void SurfaceGenerator::TexturizeSurface(SurfMesh& M){	
	WholeMapGenerationPass=true;
	for(int i=0;i<M.VERTS.Count();i++){
		SurfVertex* V=&M.VERTS[i];
		PrecompiledVertex PV;
		PV.CreateFromVertex(*V);
		FillVertexTextures(&PV);
	}
	WholeMapGenerationPass=false;
}
extern bool EditMapMode;
void SurfaceGenerator::TexturizeSurface(SurfMesh& M, int xc, int yc, int R1, int R2){
	if(g_SurfEditor.LockSurface && EditMapMode) return;

	SurfRef Ref;
	g_UniversalMap.GetRefCylinder(Ref, SurfVertex(xc, yc), R2 - 4, Surface::SR_VERTS);
	for(int i=0; i < Ref.idVerts.Count(); i++) {
		SurfVertex *V = &M.VERTS[Ref.idVerts[i]];
		SurfVertex V0 = *V;
		PrecompiledVertex PV;
		PV.CreateFromVertex(*V);
		FillVertexTextures(&PV);
		int r = sq_norma(V0.x - xc, V0.y - yc);
		if(r > R1) {
			int ww = (r - R1) * 255 / (R2 - R1);
			int GetFractalValEx(int x, int y, int Type);
			ww = (ww * GetFractalValEx(V->x * 5,V->y * 5, 1) / 256);
			if(ww > 240) ww = 255;
			if(ww < 0) ww = 0;
			DWORD CC = MixDWORD(V0.Color, V->Color, ww, 255 - ww);
			V->Color = CC;
			if(r > (R1 + R2) / 2) V->nFacture = V0.nFacture;
		}
	}
	Rct rc(xc - R2 - 100, yc - R2 - 100, 2 * R2 + 200, 2 * R2 + 200);
	ITerra->InvalidateGeometry(&rc);
    IWater->Refresh( rc );
}
void ChangeTreesInRadiusWithEffect(int x,int y,int R,int LandType);
void SlowTexturizeSurface(int xc,int yc,int R1,int R2
						  ,int RiseTime,int GrowTime,int RemainTime,int EliminateTime,bool Forever
						  ,char* BirthWeapon,int BirthProbability
						  ,char* UndoWeapon ,int UndoProbability){
	if(g_SurfEditor.LockSurface && EditMapMode ) return;

	SurfRef Ref;
	g_UniversalMap.GetRefCylinder(Ref, SurfVertex(xc, yc), R2 - 4, Surface::SR_VERTS);
	for(int i=0;i<Ref.idVerts.Count();i++){
		SurfVertex* V=g_UniversalMap.VERTS + Ref.idVerts[i];
		int r=sq_norma(V->x-xc,V->y-yc);		
		int ww=255;
		if(r<R2){
			if(r>R1){
				ww=255-(r-R1)*255/(R2-R1);
				int GetFractalValEx(int x,int y,int Type);
				ww=(ww*GetFractalValEx(V->x*5,V->y*5,1)/256);
				if(ww>255)ww=255;
				if(ww<0)ww=0;	
			}
			int ww2=128+(ww-128)*4;
			if(ww2<0)ww2=0;
			if(ww2>255)ww2=255;
			if(ww2>0){
				EffectToBorn E1;
				E1.WeaponToBorn=BirthWeapon;
				E1.WeaponBornProbability=BirthProbability;
				EffectToBorn E2;
				E2.WeaponToBorn=UndoWeapon;
				E2.WeaponBornProbability=UndoProbability;
				TMORPH.AddTerraTypeDeformator(Ref.idVerts[i],(GrowTime*(255-ww)/255+GrowTime*r/R2),RiseTime,RemainTime,EliminateTime,ww2,Forever,&E1,&E2);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// WorldToGenerator
//-----------------------------------------------------------------------------
float WorldToGenerator(int WorldCoord) {
	const float d = (240.0f * 64.0f) / 512.0f;
	return float(WorldCoord) / d;
}

//-----------------------------------------------------------------------------
// ReadRaceIndex
//-----------------------------------------------------------------------------
int ReadRaceIndex(const SurfVertex &Center) {
	Enumerator *E = ENUM.Get("RACES");
	int idRace = E->GetVal(0);
	int X = (int)WorldToGenerator(Center.x);
	int Y = (int)WorldToGenerator(Center.y);
	for(int l = 0; l < MLayers->MapsNames.GetAmount(); l++) {
		if(LAYERSLIST.Layers[l]->ThisLayerDeterminesRace) {
			GreyMap *pMap = (GreyMap*)MLayers->Maps[l];
			if(pMap->GetPixelColor(X, Y) > 0) {
				idRace = l;
			}
		}
	}
	return idRace;
}

//-----------------------------------------------------------------------------
// SurfaceGenerator::Rotate
//-----------------------------------------------------------------------------
void SurfaceGenerator::Rotate(const bool Left) {
	// MessageBox:
	cStr Text("Are you sure you want to rotate all layers to the ");
	cStr Caption("Confirm Rotate ");
	if(Left) {
		Text << "left";
		Caption << "Left";
	} else {
		Text << "right";
		Caption << "Right";
	}
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) != IDYES) {
		return;
	}

	// Enuming all maps (and rotating):
	for(int l = 0; l < MLayers->MapsNames.GetAmount(); l++) {
		GreyMap *Map = (GreyMap *)MLayers->Maps[l];
		int Width, Height;
		Map->GetSize(Width, Height);
		if(Width != Height) {
			continue;
		}
		const int Dim = cMath::Min(Width, Height);
		GreyMap Temp;
		Temp.SetSize(Dim, Dim);
		for(int y = 0; y < Dim; y++) {
			for(int x = 0; x < Dim; x++) {
				Temp.SetGreyScale(x, y, Map->GetGreyScale(Left ? Dim - 1 - y : y, Left ? x : Dim - 1 - x));
			}
		}
		for(int y = 0; y < Dim; y++) {
			for(int x = 0; x < Dim; x++) {
				Map->SetGreyScale(x, y, Temp.GetGreyScale(x, y));
			}
		}
		Map->CreateTexturePart(0, 0, Width, Height);
	}
} // SurfaceGenerator::RotateLeft

void SurfaceGenerator::RotateLeft() {
	Rotate(true);
}

void SurfaceGenerator::RotateRight() {
	Rotate(false);
}

//-----------------------------------------------------------------------------
// SurfaceGenerator::Shift
//-----------------------------------------------------------------------------
void SurfaceGenerator::Shift(const int Sx, const int Sy) {
	// MessageBox:
	cStr Text("Are you sure you want to shift all layers by [");
	Text << Sx << ", " << Sy << "]?";
	const char *Caption = "Confirm Shift";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) != IDYES) {
		return;
	}

	// Enuming all maps (and rotating):
	for(int l = 0; l < MLayers->MapsNames.GetAmount(); l++) {
		GreyMap *Map = (GreyMap *)MLayers->Maps[l];
		int Width, Height;
		Map->GetSize(Width, Height);
		GreyMap Temp;
		Temp.SetSize(Width, Height);
		for(int y = 0; y < Height; y++) {
			for(int x = 0; x < Width; x++) {
				int sx = x - Sx;
				int sy = y - Sy;
				if(sx >= Width) {
					sx -= Width;
				}
				if(sy >= Height) {
					sy -= Height;
				}
				if(sx < 0) {
					sx += Width;
				}
				if(sy < 0) {
					sy += Height;
				}
				Temp.SetGreyScale(x, y, Map->GetGreyScale(sx, sy));
			}
		}
		for(int y = 0; y < Height; y++) {
			for(int x = 0; x < Width; x++) {
				Map->SetGreyScale(x, y, Temp.GetGreyScale(x, y));
			}
		}
		Map->CreateTexturePart(0, 0, Width, Height);
	}
} // SurfaceGenerator::Shift

void SurfaceGenerator::ShiftHorizontal() {
	Shift(CurShift, 0);
}

void SurfaceGenerator::ShiftVertical() {
	Shift(0, CurShift);
}

//-----------------------------------------------------------------------------
// SurfLayerDrawRound	
//-----------------------------------------------------------------------------
void SurfLayerDrawRound(int idLayer, const SurfVertex &Center, int Radius) {
	if(idLayer < MLayers->Maps.NValues) {
		GreyMap *GRM = (GreyMap*)MLayers->Maps[idLayer];
		GRM->PreciseSetValueInRound(WorldToGenerator(Center.x), WorldToGenerator(Center.y), WorldToGenerator(Radius), 0);
	}
}

//-----------------------------------------------------------------------------
// SurfLayerClearRound
//-----------------------------------------------------------------------------
void SurfLayerClearRound(int idLayer, const SurfVertex &Center, int Radius) {
	if(idLayer < MLayers->Maps.NValues) {
		GreyMap *GRM = (GreyMap*)MLayers->Maps[idLayer];
		GRM->PreciseClearValueInRound(WorldToGenerator(Center.x), WorldToGenerator(Center.y), WorldToGenerator(Radius), 0);
	}
}

//-----------------------------------------------------------------------------
// SurfLayerClear
//-----------------------------------------------------------------------------
void SurfLayerClear(int idLayer) {
	if(idLayer < MLayers->Maps.NValues) {
		GreyMap *pMap = (GreyMap *)MLayers->Maps[idLayer];
		pMap->Clear();
	}
}

//-----------------------------------------------------------------------------
// SurfLayerIsSet
//-----------------------------------------------------------------------------
bool SurfLayerIsSet(int idLayer, int x, int y) {
	if(idLayer >= MLayers->Maps.NValues) return false;
	GreyMap *pMap = (GreyMap *)MLayers->Maps[idLayer];
	return pMap->GetPixelColor((int)WorldToGenerator(x), (int)WorldToGenerator(y)) > 0;
}

//-----------------------------------------------------------------------------
// BlendSurfLayers
//-----------------------------------------------------------------------------
void MixWithSurfLayers(int x, int y, DWORD &Color, int nActive) {
	cColor Clr = cColor::FromBgra(Color);
	float Alpha = Clr.a;
	int nMixed = 1;
	
	for(int i = 0; i < LAYERSLIST.Layers.GetAmount(); i++) {
		OneLayerColor *pLayer = LAYERSLIST.Layers[i];
		if(pLayer->ThisLayerDeterminesRace) continue;
//		if(pLayer->IsUsedAsMask && fIncludeMasks) continue;
		if(pLayer->WillEraseOthers) continue;
		GreyMap *pMap = (GreyMap *)MLayers->Maps[i];
		assert(pMap && "There is no entry in MLayers! Why?");
		if(!pMap) continue;
		if(pMap->GetPixelColor((int)WorldToGenerator(x), (int)WorldToGenerator(y)) > 0) {
			const DWORD argb = pMap->Color;
			if(nActive == i) {
				const int x = 2;
				Clr += float(x) * cColor::FromBgra(argb);
				nMixed += x;
			} else {
				Clr += cColor::FromBgra(argb);
				nMixed++;
			}
		}
	}

	Clr /= (float)nMixed;
	Clr.a = Alpha;
	Color = Clr.ToBgra();
}

//-----------------------------------------------------------------------------
// SurfGenClear
//-----------------------------------------------------------------------------
void SurfGenClear() {
	for(int i = 0; i < MLayers->Maps.GetAmount(); i++) {
		MLayers->Maps[i]->Clear();
	}
}

//-----------------------------------------------------------------------------
// RaceRound
//-----------------------------------------------------------------------------
void RaceRound(int idRace, const SurfVertex &Center, int Radius) {
	float X = WorldToGenerator(Center.x);
	float Y = WorldToGenerator(Center.y);
	float R = WorldToGenerator(Radius);
	for(int l = 0; l < LAYERSLIST.Layers.GetAmount(); l++) {
		if(LAYERSLIST.Layers[l]->ThisLayerDeterminesRace) {
			GreyMap *pMap = (GreyMap*)MLayers->Maps[l];
			pMap->PreciseClearValueInRound(X, Y, R, 0);
		}
	}
	GreyMap *pMap = (GreyMap*)MLayers->Maps[idRace];
	pMap->PreciseSetValueInRound(X, Y, R, 0);
}

//-------------------------------------------------------------------------------------------------
// RaceRef
//-------------------------------------------------------------------------------------------------
void RaceRef(int idRace, SurfRef &Ref, const SurfVertex &Center, int Radius, bool TexturizeVerts) {
	float X = WorldToGenerator(Center.x);
	float Y = WorldToGenerator(Center.y);
	float R = WorldToGenerator(Radius);
	for(int l = 0; l < MLayers->MapsNames.GetAmount(); l++) {
		if(LAYERSLIST.Layers[l]->ThisLayerDeterminesRace) {
			GreyMap *GRM = (GreyMap*)MLayers->Maps[l];
			GRM->PreciseClearValueInRound(X, Y, R, 0);
		}
	}
	GreyMap *GRM = (GreyMap*)MLayers->Maps[idRace];
	GRM->PreciseSetValueInRound(X, Y, R, 0);
	if(TexturizeVerts) {
		for(int i = 0; i < Ref.idVerts.Count(); i++) {
			SurfVertex &r = Ref.pOrig->VERTS[Ref.idVerts[i]];
			if(SelExt::IsSelected(r, Center, Radius)) {
				SURF.TexturizeVertex(r);
			}
		}
	}
}

void ApplyLandType(char* From,char* To,int xc,int yc,int R1,int R2){
	float d=15360.0f/512.0f;
	for(int i=0;i<MLayers->MapsNames.GetAmount();i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){		
		GreyMap* GRM=(GreyMap*)MLayers->Maps[i];
		GRM->PreciseClearValueInRound(float(xc)/d,float(yc)/d,float(R1)/d,float(R2-R1)/d);		
	}
	for(int i=0;i<MLayers->MapsNames.GetAmount();i++){
		if(MLayers->MapsNames[i]->equal(To)){
			GreyMap* GRM=(GreyMap*)MLayers->Maps[i];
			GRM->PreciseSetValueInRound(float(xc)/d,float(yc)/d,float(R1)/d,float(R2-R1)/d);
			SURF.TexturizeSurface(g_UniversalMap,xc,yc,R1,R2);
			break;
		}
	}
}
void ApplyLandTypeSlow(char* From,char* To,int xc,int yc,int R1,int R2
					   ,int RiseTime,int GrowTime,int RemainTime,int EliminateTime,bool Forever
					   ,char* BirthWeapon,int BirthProbability
					   ,char* UndoWeapon ,int UndoProbability){
	float d=15360.0f/512.0f;
	for(int i=0;i<MLayers->MapsNames.GetAmount();i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
		GreyMap* GRM=(GreyMap*)MLayers->Maps[i];
		GRM->PreciseClearValueInRound(float(xc)/d,float(yc)/d,float(R1)/d,float(R2-R1)/d);
	}	
	for(int i=0;i<MLayers->MapsNames.GetAmount();i++){
		if(MLayers->MapsNames[i]->equal(To)){
			ChangeTreesInRadiusWithEffect(xc,yc,R2,i);
			GreyMap* GRM=(GreyMap*)MLayers->Maps[i];
			GRM->PreciseSetValueInRound(float(xc)/d,float(yc)/d,float(R1)/d,float(R2-R1)/d);
			SlowTexturizeSurface(xc,yc,R1,R2,RiseTime/2,GrowTime,RemainTime,EliminateTime,Forever,BirthWeapon,BirthProbability,UndoWeapon,UndoProbability);
			break;
		}
	}
}
int RA_Values[17]={
	255,160,210,193,
	240,145,160,195,
	210,255,200,190,
	150,210,180,220,255};
int GetChasmModulator(byte Angle){
    int A1=Angle>>4;
	int A2=A1+1;
	int DA=Angle-(A1<<4);
	return RA_Values[A1]+(RA_Values[A2]-RA_Values[A1])*DA/16;
}
void HideRoadsInRadius(int x,int y,int R,bool Hide);
void ApplyLandChasm(int xc,int yc,int R,int Deep
					,int RiseTime,int GrowTime,int RemainTime,int EliminateTime){
	if(g_SurfEditor.LockSurface && EditMapMode) return;
    void HideRoadsInRadius(int x,int y,int R,bool Hide);
    HideRoadsInRadius(xc,yc,R+128,true);
	SurfRef Ref;
	g_UniversalMap.GetRefCylinder(Ref, SurfVertex(xc, yc), R, Surface::SR_VERTS);
	for(int i=0;i<Ref.idVerts.Count();i++){
		SurfVertex* V=g_UniversalMap.VERTS + Ref.idVerts[i];
		int dx=V->x-xc;
		int dy=V->y-yc;
		word GetDir(int dx,int dy);
		word GetDir(int dx,int dy);
		byte A=GetDir(dx,dy)+xc;
		int RR=R*GetChasmModulator(A)/255;
		int r=sq_norma(dx,dy);
		int ww=255;
		int DW=60;
		int DH2=30;
		if(r<RR+DW){
			float DR=float(RR-r);
			int DX=0;
			int DY=0;
			int DH;
			Vector3D nr;
			if(DR>0){
				nr=Vector3D(-dx,-dy,0);
				DH=-Deep*(RR-r)/RR-DH2;
			}else{
                nr=Vector3D(0,0,1);
				DH=-(DW+DR)*DH2/DW;
			}
			nr.normalize();
			if(DR>0){
				int DD=(r-RR)*sqrt(RR-r)/sqrt(RR);
				DX=-3*dx*DD/RR;
				DY=-3*dy*DD/RR;
			}
			int ww=(RR-r)*255/RR;
			int DT=rando()%RiseTime/8+RiseTime*(255-ww)*7/255/8;
			TMORPH.AddTerraHeightDeformator(Ref.idVerts[i],DT,RiseTime-DT,RemainTime-DT,EliminateTime,Vector3D(DX,DY,DH),nr);
		}
	}
}

extern int TrueTime;
void OnDie(int Killer,int Victim);

void FallUnitLink(OneObject* OB){	
	int dt=TrueTime-OB->LocalOrder->info.Produce.Progress;
	if(dt<0)return;	
	int sp=800+(OB->Index%30)*30;
    OB->OverEarth=OB->LocalOrder->info.Produce.NStages-dt*dt/sp;
	if(OB->OverEarth<-1500){
		word CastID=OB->LocalOrder->info.Produce.ObjIndex;
		word CastSN=OB->LocalOrder->info.Produce.ID;
		if(CastID!=0xFFFF){
			OneObject* Caster=Group[CastID];
			if(Caster&&Caster->Serial==CastSN){
				OnDie(CastID,OB->Index);
			}
		}
        OB->ClearOrders();
		OB->Die();
        OB->Sdoxlo=6000;
		OB->OverEarth=-1500;
	}
}
void FallUnit(OneObject* OB,int Timeout){
    OB->UnlimitedMotion=false;
	Order1* OR=OB->CreateOrder(0);
    OR->OrderType=0x37;
	if(OR){
		OR->info.Produce.Progress=TrueTime+Timeout;
		OR->info.Produce.NStages=OB->OverEarth;

		OneObject* Caster=GlobalUnitsContext::getCaster();
		if(Caster){
			OR->info.Produce.ObjIndex=Caster->Index;
			OR->info.Produce.ID=Caster->Serial;
		}else{
			OR->info.Produce.ObjIndex=0xFFFF;
		}
        
		OR->DoLink=&FallUnitLink;
	}
	OB->Serial++;
	OB->UnlimitedMotion=true;
    if(OB->NewBuilding && !OB->Sdoxlo){
        NewAnimation* NA=OB->newMons->Animations->Get(anm_Death);
        if(NA->Enabled){
            OB->LoLayer=NA;
            OB->NewAnm=NA;
            OB->SetZeroFrame();
        }
    }
}
typedef float fn_GetHeight(float x,float y,float xc,float yc,void* param);
_fractalLine FL_Chasm;
void ApplyArbitraryChasm(int xc,int yc,int R,float Deep,float SlopeScale,fn_GetHeight* getHeight,void* param
						,int RiseTime,int GrowTime,int RemainTime,int EliminateTime){
	if(g_SurfEditor.LockSurface && EditMapMode) return;

	static float F_scale=4.0;
    FL_Chasm.Scale=F_scale;
	SurfRef Ref;
	g_UniversalMap.GetRefCylinder(Ref, SurfVertex(xc, yc), R+100, Surface::SR_TRIS);
    SubdivideArgs sa;
    sa.EdgeMaxLen=35;
    sa.Criterion=SubdivideArgs::C_EDGELEN;
    sa.fAddRefToQuads=true;
    Ref.Subdivide(sa);
    Ref.Subdivide(sa);
    Ref.Complete(Surface::SR_VERTS,Surface::SR_TRIS);
    void HideRoadsInRadius(int x,int y,int R,bool Hide);
    HideRoadsInRadius(xc,yc,R+128,true);

    Rct rc(xc-R-200, yc-R-200, R*2+400, R*2+400);
    ITerra->InvalidateGeometry(&rc);


	static float Xscale=1.4*SlopeScale*R*R/300/300/2;
	for(int i=0;i<Ref.idVerts.Count();i++){
		SurfVertex* V=g_UniversalMap.VERTS + Ref.idVerts[i];		
		int xx=V->x;int yy=V->y;
		//FL_Chasm.ShiftXY(xx,yy);
		float H=getHeight(xx,yy,xc,yc,param);
		if(H>0){
            Vector3D V(
				getHeight(xx-16,yy,xc,yc,param)-getHeight(xx+16,yy,xc,yc,param)
				,getHeight(xx,yy-16,xc,yc,param)-getHeight(xx,yy+16,xc,yc,param),32);
			V.x*=Deep;
			V.y*=Deep;
			V.normalize();
			int ww=H*255;
			int DT=rando()%RiseTime/8+RiseTime*(255-ww)*7/255/8;
			static float clampV=4;
			float HClamp=H*clampV;
			if(HClamp>1)HClamp=1;
			float H2=H*(1-H)*HClamp*HClamp*Deep;			
			TMORPH.AddTerraHeightDeformator(Ref.idVerts[i],DT,RiseTime-DT,RemainTime-DT,EliminateTime,Vector3D(H2*V.x*Xscale,H2*V.y*Xscale,-H*Deep),V);
		}
	}
	//falling units

	itr_UnitsInRadius.Create(xc,yc,R);
	while(OneObject* OB=itr_UnitsInRadius.Next()){
		HeroVariableStorage* DetectHero(OneObject* OB);
		itr_AA.Create(OB);
		bool NotFall=false;
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			UnitAbility* uA=AA->GetA();
			if (uA&&uA->ImpossibleToFall)
			{ NotFall=true;}
		}
		if(!DetectHero(OB)&!NotFall){
			float H=getHeight(OB->RealX/16,OB->RealY/16,xc,yc,param);
			int ww=H*255;
			H*=Deep;
			int DT=30+RiseTime*(255-ww)/255;
			if(H>5 && OB->LockType!=3)FallUnit(OB,DT);//fall if not hi-flying
		}else{
			for(int i=0;i<512;i++){
				byte A=rando()&255;
				int RR=R+rando()&127;
                int x=xc+(TCos[A]*R)/256;
				int y=yc+(TSin[A]*R)/256;
				if(!CheckBar(x/16-3,y/16-3,7,7)){
					void ChangeUnitCoor(OneObject* OB,int newX,int newY);
					addrand(x);
					addrand(y);
					ChangeUnitCoor(OB,x<<4,y<<4);
				}
			}
		}
	}
	itr_GetBuildingsInRadius.Create(xc,yc,R);
	while(OneObject* OB=itr_GetBuildingsInRadius.Next()){
		itr_AA.Create(OB);
		bool NotFall=false;
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			UnitAbility* uA=AA->GetA();
			if (uA&&uA->ImpossibleToFall)
			{ NotFall=true;}
		}
		if (!NotFall){
		float H=getHeight(OB->RealX/16,OB->RealY/16,xc,yc,param);
		int ww=H*255;
		H*=Deep;
		int DT=30+RiseTime*(255-ww)/255;
		if(H>5)FallUnit(OB,DT);
		}
	}
    itr_GetSpritesInRadius.Create(xc,yc,R);
    while(OneSprite* OS=itr_GetSpritesInRadius.Next()){
        float H=getHeight(OS->x,OS->y,xc,yc,param);
        int ww=H*255;
        H*=Deep;
        int DT=30+RiseTime*(255-ww)/255;
        if(H>5){
            void EraseSprite(int Index);
            EraseSprite(OS->Index);
        }        
    }
}

struct fn_GetHeightForRound_param{
	float R;
	float Deep;
};
float fn_GetHeightForRound(float x,float y,float xc,float yc,void* param){	
	fn_GetHeightForRound_param* P=(fn_GetHeightForRound_param*)param;
	word GetDir(int dx,int dy);
	byte A=GetDir(x-xc,y-yc)+xc;
	float R=sq_norma(x-xc,y-yc);
	R=R*255/GetChasmModulator(A);	
	if(R>P->R+64)return 0;
	R=P->R-R;
    //float H1=R*R/92;
	float H1=(R+64)/2;
	float H2=R*P->Deep/P->R;
	if(H2<0)H2=0;
	return max(H1,H2)/P->Deep;
}
void ApplyRoundChasm(int xc,int yc,int R,int Deep,int RiseTime,int GrowTime,int RemainTime,int EliminateTime){
    fn_GetHeightForRound_param P;
	P.R=R;
	P.Deep=Deep;
	ApplyArbitraryChasm(xc,yc,R,Deep,1,&fn_GetHeightForRound,&P,RiseTime,GrowTime,RemainTime,EliminateTime);
}
struct fn_GetHeightForRow_param{
	float R1;
	float R2;
	float minR;
	float SIN,COS;
	float Deep;
};
float fn_GetHeightForRow(float x,float y,float xc,float yc,void* param){	
	fn_GetHeightForRow_param* P=(fn_GetHeightForRow_param*)param;
	x-=xc;y-=yc;
	float xr=P->R1-fabs(x*P->COS+y*P->SIN);
	float yr=P->R2-fabs(-y*P->SIN+y*P->COS);
	if(xr<0 || yr<0)return 0;
	float RR=min(xr,yr);
	float H1=((RR*RR/64)*RR/64)*RR/64;
	//if(RR>64)H1=P->Deep;
	//float H2=RR*P->Deep/P->minR;
	float H2=P->Deep;
	//if(H2>P->Deep)H2=P->Deep;
	return min(H1,H2)/P->Deep;
}
float fn_GetHeightElliptic(float x,float y,float xc,float yc,void* param){	
	fn_GetHeightForRow_param* P=(fn_GetHeightForRow_param*)param;
	x-=xc;y-=yc;
	float xr=P->R1-fabs(x*P->COS+y*P->SIN);
	float yr=P->R1-fabs(-y*P->SIN+y*P->COS)*P->R1/P->R2;
	if(xr<0 || yr<0)return 0;
	float RR=sq_norma(xr,yr);
	float H1=(RR*RR/64);
	//if(RR>64)H1=P->Deep;
	//float H2=RR*P->Deep/P->minR;
	float H2=P->Deep;
	//if(H2>P->Deep)H2=P->Deep;
	return min(H1,H2)/P->Deep;
}
void ApplyRowChasm(int xc,int yc,int R1,int R2,byte Dir,int Deep,int RiseTime,int GrowTime,int RemainTime,int EliminateTime){
	fn_GetHeightForRow_param P;
	P.R1=R1;
	P.R2=R2;
	P.Deep=Deep;
	P.minR=min(R1,R2);
	P.SIN=TSin[Dir]/255.0f;
	P.COS=TCos[Dir]/255.0f;
	float d=min(R1,R2)/float(max(R1,R2));
	ApplyArbitraryChasm(xc,yc,max(R1,R2),Deep,d*d,&fn_GetHeightForRow,&P,RiseTime,GrowTime,RemainTime,EliminateTime);
}

					
void ApplyStandartChasm(int x,int y,int R,int Time){
	ApplyRoundChasm(x,y,R,450,400,500,Time,500);
}
void TestRuleApply(int x,int y,int r){
	//SURF.TexturizeSurface(g_UniversalMap,x,y,r,r+200);
	//if(GetKeyState('A'))ApplyLandTypeSlow("IceDruids","Necro",x,y,r,r+400,200,5000,5000,5000,false,"",0,"",0);
	//else ApplyLandTypeSlow("NecroDruids","Ice",x,y,r,r+400,200,5000,5000,3000,false,"Blizard",1,"",0);
	//ApplyLandChasm(x,y,r+100,450,400,500,5000,500);
	//ApplyRoundChasm(x,y,r+100,450,400,500,5000,500);
	static int xp=0;
	static int yp=0;
	if(x!=xp && y!=yp){
		ApplyRoundChasm(x,y,r+100,450,400,500,5000,500);
		//ApplyLandChasm(x,y,r+100,450,400,500,5000,500);
	}
	xp=x;
	yp=y;
}
void DistField::ClearMaps(){
	for(int i=0;i<MaxDistSet;i++)if(map[i]){
		delete(map[i]);
		map[i]=NULL;
	}
}

void SurfaceGenerator::PerformTest(){
/*	DistField::ClearMaps();
	GeneratorSurfaceFunction SF;
	SF.GPtr=this;
	SurfMap VBMesh;
	Vector3D v3dSun(2, 1, 1);
//	VBMesh.CreateSurface(150,150,32,&SF);
//	VBMesh.SurfMesh::ModifySurface(2,&SF);
	VBMesh.CalcNormals();
	SURF.TexturizeSurface(VBMesh);
	for(int i = 0; i < VBMesh.Vert.GetAmount(); i++)
		if(VBMesh.Vert[i].nFacture > 100) VBMesh.Vert[i].nFacture = 100;

	//	VBMesh.CalcShadow(v3dSun, 0.4, 80, 255);
	//VBMesh.InitUniversalMapQuad(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	VBMesh.QUADS.Init();

	//OneSplitMesh OSMesh;
	VertexBasedMeshRef VBMRef;
	VBMesh.GetMeshRefSphere(Vector3D(0,0,0),5000,&VBMRef);
	//OSMesh.AdvSplitMesh(&VBMRef);
//	OSMesh.Split(VBMRef);
	DistField::ClearMaps();
	float A = 0;

	// ----------------------------------------
	extern SurfMap g_UniversalMap;
	g_UniversalMap.GetMeshRefBox(0, 0, 10000, 20000, &VBMRef);
	//OSMesh.AdvSplitMesh(&VBMRef);
//	OSMesh.Split(VBMRef);
	do{
		A += 0.08;
		//processing
		//MESH.TestDraw();	
		//NvertexOut(MESH);
		/*		v3dSun.x = cos(A),
		v3dSun.y = 1,
		v3dSun.z = sin(A);
		MESH.CalcShadow(v3dSun, 0.4, 80, 255);
		SPM.AdvSplitMesh(&MR);*/
/*		OSMesh.Draw();
		//VBMesh.TestDraw();

		FlipPages();
		ProcessMessages();
		IRS->ClearDevice( 0, true, true );
		extern float fMapX;
		extern float fMapY;
		float fstep=0.5;
		if(GetKeyState(VK_LEFT)&0x8000){			
			fMapX-=fstep;
		}
		if(GetKeyState(VK_RIGHT)&0x8000){			
			fMapX+=fstep;;
		}
		if(GetKeyState(VK_UP)&0x8000){			
			fMapY-=fstep;
		}
		if(GetKeyState(VK_DOWN)&0x8000){			
			fMapY+=fstep;
		}
		void SetupCamera();
		SetupCamera();
	}while(!(GetKeyState(VK_SPACE)&0x8000));	*/
}

bool LoadMLayersFromFileInCompactForm(char* filename);
void SaveMLayersToFileInCompactForm(char* filename);
void SaveGenLayers() { SaveMLayersToFileInCompactForm("Surface\\SurfaceShape.dat"); }
extern bool DontShowHeight;
void CreateMiniMap();
int SurfaceGenMinimap=-1;

//-----------------------------------------------------------------------------
// SimplifySurfMesh : void(SurfMesh &)
//-----------------------------------------------------------------------------
void SimplifySurfMesh(SurfMesh &Mesh) {
	SimplifySurface(Mesh);
	g_SetOfIntPool.Free();
}

//-----------------------------------------------------------------------------
// OptimizeSurfMesh : void(SurfMesh &, bool)
//-----------------------------------------------------------------------------
void OptimizeSurfMesh(SurfMesh &Mesh, bool fHoldBoundary) { // so we can call the whole simplification process at any time.....
	SubdivideArgs sArg;
	sArg.Criterion = SubdivideArgs::C_DOT;
	sArg.Dot=0.88f;
	sArg.Scale=2.5f;
	sArg.fAddRefToQuads=false;				
	Mesh.Subdivide(sArg);
	SimplifySurfMesh(Mesh);
	Mesh.CalcNormals();
	Mesh.Subdivide(sArg);
	if(!fHoldBoundary) Mesh.Relax(0.3,2,0);
}

namespace Surface {

	bool GeneratorOnePass(cStr &NextPassName) {
		static int nCurPass = -1;
		static GeneratorSurfaceFunction SF;
		static int NX;
		if(-1 == nCurPass) {			
			nCurPass = 0;
			NextPassName = "Generator (0/3) : Init";
			return true;
		} else if(0 == nCurPass) { // Init
			SurfaceUndo::DropWholeMap();
			SaveMLayersToFileInCompactForm("Surface\\SurfaceShape.dat");
			if(!g_UniversalMap.TRIS.Count()) SURF.DontGenerateRelief = false;
			//g_UniversalMap.BackUp();
			if(!SURF.DontGenerateRelief) {
				g_UniversalMap.Free();
			}
			DistField::ClearMaps();
			SF.GPtr = &SURF;
			SF.ShapeOfBound = &SURF.ShapeOfBound;
			int cellsize = 32;
			NX = SURF.Size / cellsize;
			if(NX < 40) NX = 40;
			if(NX > 512) NX = 512;
			SF.MapLx = NX * cellsize;
			SF.MapLy = NX * cellsize;
			nCurPass = 1;
			NextPassName = "Generator (1/3) : Geometry";
			return true;
		} else if(1 == nCurPass) { // Geometry
			if(!SURF.DontGenerateRelief) {
				SURF.ExHeight.Load(SURF.BasicHeightBitmap.pchar());
				g_UniversalMap.ApplyMCubes(32, NX, NX, &SF);
				g_UniversalMap.Relax(0.5,2,0);
				//g_UniversalMap.Relax(1.0,10,0.7);
				g_UniversalMap.CalcNormals();
				if(SURF.OptimizeSurface){
					//OptimizeSurfMesh(g_UniversalMap);
                    SimplifySurfMesh(g_UniversalMap);
				}
			}
			g_UniversalMap.QUADS.Init();
			g_UniversalMap.QUADS.FillMapOfHeights();

			nCurPass = 2;
			NextPassName = "Generator (2/3) : Textures";
			return true;
		} else if(2 == nCurPass) { // Texture
			void DeleteAllSprites();
            if(!SURF.DontGenerateTrees)DeleteAllSprites();
			SURF.TexturizeSurface(g_UniversalMap);
			nCurPass = 3;
			NextPassName = "Generator (3/3) : Final";
			return true;
		} else if(3 == nCurPass) { // Finalizing
			g_UniversalMap.QUADS.Init();
			g_UniversalMap.QUADS.FillMapOfHeights();
			g_UniversalMap.m_Shader.Shade(g_UniversalMap, Surface::I_LOCAL);

			DistField::ClearMaps();
			// Info: 
			int N = 0, maxNFacture = 0;
			int GetNFactures();
			maxNFacture = GetNFactures() - 1;
			N = g_UniversalMap.VERTS.Count();
			for(int i = 0; i < N; i++)
				if(g_UniversalMap.VERTS[i].nFacture > maxNFacture)
					g_UniversalMap.VERTS[i].nFacture = maxNFacture;
			char Buffer[256], Str[4096];
			SurfVertex bbMin, bbMax;
			g_UniversalMap.CalcBoundBox(bbMin, bbMax);
			sprintf(Buffer, "bbMin = [%d, %d, %d]\tbbMax = [%d, %d, %d]\r\n",
				bbMin.x, bbMin.y, bbMin.z, bbMax.x, bbMax.y, bbMax.z);
			strcpy(Str, Buffer);
			sprintf(Buffer, "NVerts() == %d", g_UniversalMap.NVerts());
			strcat(Str, Buffer);
			sprintf(Buffer, "\r\nNTris() == %d", g_UniversalMap.NTris());
			strcat(Str, Buffer);
			bool FacturesInUse[SPLITMESH_MAXNFACTURES];
			ZeroMemory(FacturesInUse, sizeof(FacturesInUse));
			N = g_UniversalMap.VERTS.Count();
			for(i = 0; i < N; i++) FacturesInUse[g_UniversalMap.VERTS[i].nFacture] = true;
			maxNFacture = GetNFactures() - 1;
			strcat(Str, "\r\nNumbers of factures in use: ");
			for(i = 0; i <= maxNFacture; i++)
				if(FacturesInUse[i] == true)
				{
					sprintf(Buffer, " %d", i);
					strcat(Str, Buffer);
				}
				MessageBox(NULL, Str, "rce_SurfaceCallback stats:", 0);
			nCurPass = -1;
			NextPassName = "";
			return false;
		}
	}
}

//Options=1-init, 2-process, 3-ok pressed, 4-cancel pressed 
bool rce_SurfaceCallback(ClassEditor* CE,BaseClass* BC,int Options){
	SurfaceGenerator *pSurfGenerator = (SurfaceGenerator*)BC;
	assert(pSurfGenerator);

	if(Options==1){
		if(SurfaceGenMinimap==-1||SURF.ReCreateMinimap){
			DontShowHeight=true;
			CreateMiniMap();
			int GetMinimapTexture();
			int T=GetMinimapTexture();
			if(SurfaceGenMinimap==-1)
				SurfaceGenMinimap=IRS->CreateTexture("SurfaceGeneratorMinimap",256,256,cfARGB4444,1,tmpManaged);
			//IRS->CopyTexture(SurfaceGenMinimap,T);
			int pitch=0;
			BYTE* dest=IRS->LockTexBits(SurfaceGenMinimap,pitch);
			BYTE* src=IRS->LockTexBits(T,pitch);
			if(dest && src){
                memcpy(dest,src,pitch*256);
			}
			IRS->UnlockTexBits(T);
			IRS->UnlockTexBits(SurfaceGenMinimap);
		}

		pSurfGenerator->m_fActive = true;
		LastKey=0;
		KeyPressed=false;
		TestLayersColors();	
		if(!LoadMLayersFromFileInCompactForm("Surface\\SurfaceShape.dat")){
			bool CheckIfFileExists(char* Name);
			if(CheckIfFileExists("Surface\\SurfaceShape.xml")){
				MLayers->reset_class(MLayers);
				MLayers->SafeReadFromFile("Surface\\SurfaceShape.xml");
				TestLayersColors();	
			}
		}
	}
	if(Options==2){
		if(MLayers->Maps.GetAmount()){
			Enumerator* E=ENUM.Get("LAYERSLIST");
			for(int i=0;i<MLayers->Maps.GetAmount();i++){
				MLayers->Maps[i]->Desc.Clear();
				MLayers->Maps[i]->Desc=E->Get(i);
				MLayers->Maps[i]->Desc+=" : ";
			}
			int NR=SURF.Relief.GetAmount();
			for(int i=0;i<NR;i++){
				_str* s=&MLayers->Maps[SURF.Relief[i]->Map]->Desc;				
				_str* ads=&SURF.Relief[i]->Description;
				if(ads->pchar()&&ads->pchar()[0]){
					*s+=*ads;
					*s+=" ";
				}
				int NR=SURF.Relief[i]->SurfaceRules.GetAmount();
				for(int j=0;j<NR;j++){
					_str* s=&MLayers->Maps[SURF.Relief[i]->SurfaceRules[j]->Map]->Desc;
					_str* ads=&SURF.Relief[i]->SurfaceRules[j]->Description;
					if(ads->pchar()&&ads->pchar()[0]){
						*s+=*ads;
						*s+=" ";
					}
				}

			}
			for(int i=0;i<SURF.Texturing.GetAmount();i++){
				/*if(SURF.Texturing[i]->UseMap){
				_str* s=&MLayers->Maps[SURF.Texturing[i]->Map]->Desc;
				_str* ads=&SURF.Texturing[i]->Description;
				if(ads->pchar()&&ads->pchar()[0]){
				*s+=*ads;
				*s+=" ";
				}                    
				}*/
				for(int j=0;j<SURF.Texturing[i]->TexturingRules.GetAmount();j++){
					OneTexturingRule* TR=SURF.Texturing[i]->TexturingRules[j];
					for(int q=0;q<TR->Conditions.GetAmount();q++){
						CheckSpecialLayer* L=dynamic_cast<CheckSpecialLayer*>(TR->Conditions[q]);
						if(L){
							_str* s=&MLayers->Maps[L->Layer]->Desc;
							_str* ads=&TR->Description;
							if(ads->pchar()&&ads->pchar()[0]){
								*s+=*ads;
								*s+=" ";
							}
						}
					}					
				}
			}
			for(int i=0;i<MLayers->Maps.GetAmount();i++){
				_str* s=&MLayers->Maps[i]->Desc;
				if(s&&s->pchar()){
					void Translit(char* s);
					Translit(s->pchar());
				}
			}
		}
	}
	if(Options==3 && pSurfGenerator->m_fActive && !SURF.DoNothing){//Ok
		ActivateSurfTool("Generate");
	}
	if(3 == Options || 4 == Options) {
		SaveGenLayers();
//		remove("Surface\\Surface3D.xml");
//		SURF.WriteToFile("Surface\\Surface3D.xml");
		pSurfGenerator->m_fActive = false;
		DontShowHeight=false;
		CreateMiniMap();
	}	
	return !pSurfGenerator->m_fActive;
}

const DWORD SurfOptions = RCE_CENTRAL_POSITION;
void ShowSurfGenerator() {
	ClearSurfTool();
	EditClass("SurfGenerator", (BaseClass*)(&SURF), "Surface\\Surface3D.xml", SurfOptions,
		rce_SurfaceCallback, NULL);
}

void RegisterSurfaceEditor(){
	if(!MLayers){
		MLayers=new MapsScope;
		MLayers->SetDefaults();
		void TestLayersColors();
		TestLayersColors();
	}
	//SURF.Test.SetParent(&SURF);
	bool SurfaceEditorProcessClassCallback(ClassEditor *, BaseClass *, int);
	AddStdEditor("SurfEditor", (BaseClass*)(&g_SurfEditor), "Surface\\Editor.xml", SurfOptions, SurfaceEditorProcessClassCallback, NULL, 'X');
	AddStdEditor("SurfGenerator",&SURF,"Surface\\Surface3D.xml", SurfOptions, rce_SurfaceCallback, NULL,'G');
	if(GetKeyState(VK_CONTROL)&0x8000){
		void SelectEditor();
		SelectEditor();
	}
}
//////////////////////////////////////////////////////////////////////////
//OneVertexTempDeformator
//////////////////////////////////////////////////////////////////////////

void OneVertexTempDeformator::BlendVertsCoor(SurfVertex& V1,SurfVertex& V2,int W1,int W2,SurfVertex& Dest){
	int DW=(W1+W2)/2;
	Dest.x=(int(V1.x)*W1+int(V2.x)*W2+DW)/255;
	Dest.y=(int(V1.y)*W1+int(V2.y)*W2+DW)/255;
	Dest.z=(int(V1.z)*W1+int(V2.z)*W2+DW)/255;

	Dest.nx=(int(V1.nx)*W1+int(V2.nx)*W2+DW)/255;
	Dest.ny=(int(V1.ny)*W1+int(V2.ny)*W2+DW)/255;
	Dest.nz=(int(V1.nz)*W1+int(V2.nz)*W2+DW)/255;
}
void OneVertexTempDeformator::BlendVertsColors(SurfVertex& V1,SurfVertex& V2,int W1,int W2,SurfVertex& Dest){
	int DW=(W1+W2)/2;
    Dest.Luminance = (int(V1.Luminance)*W1+int(V2.Luminance)*W2+DW)/255;
	Dest.Color  = MixDWORD(V1.Color,V2.Color,W1,W2);
	if(W2>40){
		Dest.nFacture=V2.nFacture;
		Dest.RaceIndex=V2.RaceIndex;
		Dest.SurfaceType=V2.SurfaceType;
	}else{
		Dest.nFacture=V1.nFacture;
		Dest.RaceIndex=V1.RaceIndex;
		Dest.SurfaceType=V1.SurfaceType;
	}
}
void OneVertexTempDeformator::BlendVerts(SurfVertex& V1,SurfVertex& V2,int W1,int W2,SurfVertex& Dest){
    BlendVertsCoor(V1,V2,W1,W2,Dest);
	BlendVertsColors(V1,V2,W1,W2,Dest);
}
int ScaledGameTime=0;
bool OneVertexTempDeformator::PerformDeformation(){
	int t=ScaledGameTime-TimeToWait-StartTime;
	if(t>0){
		int w=0;
		if(t<TimeToTransformToFinalState){
            w=t*255/TimeToTransformToFinalState;
		}else{
			t-=TimeToTransformToFinalState;
			if(t<TimeToStayInFinalState){
                w=255;                
			}else{				
				t-=TimeToStayInFinalState;
				if(t<TimeToReturnInStartState){
					if(!ForeverChange){
						w=255-t*255/TimeToReturnInStartState;
					}else w=255;
				}else if(ForeverChange)w=255;
			}
		}
		if(w<0)w=0;
		if(w>MaxFinalWeight)w=MaxFinalWeight;
		BlendVerts(DestVertex,StartVertex,w,255-w,CurVertex);
		return true;
	}
	return false;    
}
void ChangeTerraType::CalcDestVertex(){	
	SurfVertex* V=g_UniversalMap.VERTS + VertexIndex;	
	SurfVertex V0=*V;
	PrecompiledVertex PV;
	PV.CreateFromVertex(*V);
	SURF.FillVertexTextures(&PV);
	DestVertex=*V;
	*V=V0;
}
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
void ChangeTerraType::OnStart(){
	if(StartEffect.WeaponToBorn.pchar()[0]&&rando()<StartEffect.WeaponBornProbability*328){
        CreateNewActiveWeapon(StartEffect.WeaponToBorn.pchar(),0xFFFF,CurVertex.x,CurVertex.y,CurVertex.z,0xFFFF,CurVertex.x,CurVertex.y,CurVertex.z+1000,NULL);
	}
}	
void ChangeTerraType::OnEndStay(){
	if(FinalEffect.WeaponToBorn.pchar()[0] && rando()<FinalEffect.WeaponBornProbability*328){
		CreateNewActiveWeapon(FinalEffect.WeaponToBorn.pchar(),0xFFFF,CurVertex.x,CurVertex.y,CurVertex.z,0xFFFF,CurVertex.x,CurVertex.y,CurVertex.z+1000,NULL);
	}
}
void TempTerraMorph::AddDeformator(OneVertexTempDeformator* D){
	OneVertexTempDeformator* df;
    int sl=D->VertexIndex % DeformSlots;
	int n=Deformations[sl].GetAmount();
	ClassArray<OneVertexTempDeformator>& DEF=Deformations[sl];
	for(int i=0;i<n;i++)if( (df=DEF[i])->VertexIndex==D->VertexIndex){
		if(df->ForeverChange)D->StartVertex=df->CurVertex;
		else D->StartVertex=df->StartVertex;		
		delete(df);
		DEF[i]=D;
        return;
	}
	DEF.Add(D);
}
void OneVertexTempDeformator::Init(int VertIndex){
	SurfVertex* V=g_UniversalMap.VERTS + VertIndex;	
	StartVertex=*V;
	CurVertex=*V;
	StartTime=PrevTime=ScaledGameTime;
	DestVertexDone=false;
	TimeToWait=0;
	TimeToTransformToFinalState=1000;
	TimeToStayInFinalState=2000;
	TimeToReturnInStartState=1000;
	VertexIndex=VertIndex;
	MaxFinalWeight=255;
	ForeverChange=false;
}
void TempTerraMorph::AddTerraTypeDeformator(int VertexIndex,int WaitTime,int ChangeTime,int StayTime,int UndoTime,int MaxWeight,bool Forever,EffectToBorn* start,EffectToBorn* final){
	SurfVertex* V=g_UniversalMap.VERTS + VertexIndex;
	ChangeTerraType* ctt=new ChangeTerraType;
	ctt->Init(VertexIndex);
	ctt->TimeToWait=WaitTime;
	ctt->TimeToTransformToFinalState=ChangeTime;
	ctt->TimeToStayInFinalState=StayTime;
	ctt->TimeToReturnInStartState=UndoTime;
	ctt->MaxFinalWeight=MaxWeight;
	ctt->ForeverChange=Forever;
	if(start){
        start->copyto(ctt->StartEffect);
	}
	if(final){
		final->copyto(ctt->FinalEffect);
	}
	AddDeformator(ctt);
}
void TempTerraMorph::AddTerraHeightDeformator(int VertexIndex,int WaitTime,int ChangeTime,int StayTime,int UndoTime,
											  Vector3D DCoor,Vector3D normal){
	SurfVertex* V=g_UniversalMap.VERTS + VertexIndex;
	ChangeTerraHeight* ctt=new ChangeTerraHeight;
	ctt->Init(VertexIndex);
	ctt->TimeToWait=WaitTime;
	ctt->TimeToTransformToFinalState=ChangeTime;
	ctt->TimeToStayInFinalState=StayTime;
	ctt->TimeToReturnInStartState=UndoTime;
	ctt->MaxFinalWeight=0xFF;
	ctt->ForeverChange=false;
	ctt->DCoor=DCoor;
	ctt->Normal=normal;
	AddDeformator(ctt);
}
void ChangeTerraHeight::CalcDestVertex(){
	SurfVertex* V=g_UniversalMap.VERTS + VertexIndex;	
	SurfVertex V0=*V;	
	if(V->z>5){
		V->x+=DCoor.x;
		V->y+=DCoor.y;
		V->z+=DCoor.z;
	}
	V->nx=Normal.x*127;
	V->ny=Normal.y*127;
	V->nz=Normal.z*127;

	PrecompiledVertex PV;
	PV.CreateFromVertex(*V);
	SURF.FillVertexTextures(&PV);
	DestVertex=*V;
	*V=V0;    
}
int GetScaledGameTime();
void TempTerraMorph::Process(){
    ScaledGameTime=GetScaledGameTime();
	for(int sl=0;sl<DeformSlots;sl++){
		int n=Deformations[sl].GetAmount();
		ClassArray<OneVertexTempDeformator>& DEF=Deformations[sl];
		for(int i=0;i<n;i++){
			OneVertexTempDeformator* d=DEF[i];
			SurfVertex* V=g_UniversalMap.VERTS + d->VertexIndex;
			int dt=ScaledGameTime-d->StartTime;
			if(dt>d->TimeToWait){
				int dtp=d->PrevTime-d->StartTime;
				int tt=d->TimeToWait;
				if(dtp<=d->TimeToWait){
					d->OnStart();	
				}
				tt+=d->TimeToTransformToFinalState;			
				if(dtp<=tt && dt>tt){
					d->OnBeginToStay();
				}
				tt+=d->TimeToStayInFinalState;
				if(dtp<=tt && dt>tt){
					d->OnEndStay();
				}
				tt+=d->TimeToReturnInStartState;
				if(dtp<=tt && dt>tt){
					d->OnEnd();
				}			
				if(!d->DestVertexDone){
					d->CalcDestVertex();
					d->DestVertexDone=true;
				}
				d->PerformDeformation();
				if(memcmp(&d->CurVertex,V,sizeof SurfVertex)){
					*V=d->CurVertex;
					Rct r(V->x-64,V->y-64,128,128);
					ITerra->InvalidateGeometry(&r);
                    IWater->Refresh( r );
				}
				if(dt>tt){
					delete(d);
					DEF[i]=NULL;
					DEF.Del(i,1);
					n--;
					i--;
					d=NULL;
				}
			}
			if(d)d->PrevTime=ScaledGameTime;
		}
	}
}
TempTerraMorph TMORPH;
void ClearTerramorph(){
	for(int i=0;i<DeformSlots;i++)TMORPH.Deformations[i].Clear();
}
void ProcessTerramorph(){
	TMORPH.Process();
}
DWORD fnRecreateLandsEnumerator(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	if(PrevCallHash!=LAYERSLIST.Layers.GetAmount()){
		for(int i=0;i<LAYERSLIST.Layers.GetAmount();i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
			out->add(LAYERSLIST.Layers[i]->Name.pchar(),i,NULL);
		}
	}
	return LAYERSLIST.Layers.GetAmount();
}
void CreateLandLayersEnumerator(){
    RefreshableEnumerator* RE=new RefreshableEnumerator(fnRecreateLandsEnumerator);
    Enumerator* E=ENUM.Get("LANDLAYERS");
	E->SetProcEnum(RE);
}
DWORD fnRecreateBoundsEnumerator(RefreshableEnumeratorContext* in,EnumeratorOutputStream* out,DWORD PrevCallHash){
	if(PrevCallHash!=LAYERSLIST.Layers.GetAmount()){
		out->add("unknown_land",-1,NULL);
		for(int i=0;i<LAYERSLIST.Layers.GetAmount();i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
			out->add(LAYERSLIST.Layers[i]->Name.pchar(),i,NULL);
			char b[80];
			sprintf(b,"%s_bound",LAYERSLIST.Layers[i]->Name.pchar());
			out->add(b,i+65536,NULL);
		}
	}
	return LAYERSLIST.Layers.GetAmount();
}
void CreateLandBoundsEnumerator(){
	RefreshableEnumerator* RE=new RefreshableEnumerator(fnRecreateBoundsEnumerator);
	Enumerator* E=ENUM.Get("LANDBOUNDS");
	E->SetProcEnum(RE);
}

//////////////////////////////////////////////////////////////////////////
/////////////////////     TREES TRANSFORMATION     ///////////////////////
//////////////////////////////////////////////////////////////////////////
class OneTreeInSequence:public BaseClass{
public:
	int TreeType;
	SAVE(OneTreeInSequence);
		REG_ENUM(_index,TreeType,TREESTYPES);
	ENDSAVE;
	virtual bool ForceSimplification(){return true;}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$TreeType$%";
		return true;
	}
};
class xTreesList:public ClonesArray<OneTreeInSequence>{
public:
	virtual bool ForceSimplification(){return true;}
};
class TreesSequence:public BaseClass{
public:
	int Nation;
	xTreesList Trees;
	SAVE(TreesSequence);
		REG_CLASS(OneTreeInSequence);
		REG_ENUM(_index,Nation,LANDLAYERS);
		REG_AUTO(Trees);
	ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$Nation$% : %$Trees,,$%";
		return true;
	}
};
class TreesExchangeList:public BaseClass{
public:
	ClonesArray<TreesSequence> TreesGroup;
	SAVE(TreesExchangeList);
		REG_CLASS(TreesSequence);
		REG_AUTO(TreesGroup);
	ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$TreesGroup,/$%";
		return true;
	}
};
class OneWeaponToChangeTree:public BaseClass{
public:
	int LandType;
    _str WeaponToChangeTree;
	Delimiter D;
	SAVE(OneWeaponToChangeTree);
		REG_ENUM(_index,LandType,LANDLAYERS);
		REG_ENUM(_strindex,WeaponToChangeTree,WeaponModificatorEnum);
		REG_AUTO(D);
	ENDSAVE;
	virtual bool ForceSimplification(){return true;}
};
class TreesExchangeInfo:public BaseClass{
public:
	ClonesArray<OneWeaponToChangeTree> WeaponsToChangeTrees;
	ClonesArray<TreesExchangeList> TreesExchange;
	SAVE(TreesExchangeInfo);
		REG_AUTO(WeaponsToChangeTrees);
		REG_AUTO(TreesExchange);
		REG_CLASS(OneWeaponToChangeTree);
	ENDSAVE;
};
TreesExchangeInfo TreesExchangeScope;
bool ChangeTreeType(OneSprite* OS,int LandType){
	if(OS->SG!=&TREES)return false;
    int Type=OS->SGIndex;
	for(int i=0;i<TreesExchangeScope.TreesExchange.GetAmount();i++){
		TreesExchangeList* TX=TreesExchangeScope.TreesExchange[i];
		for(int j=0;j<TX->TreesGroup.GetAmount();j++){
			TreesSequence* TS=TX->TreesGroup[j];
			for(int k=0;k<TS->Trees.GetAmount();k++){
				if(TS->Trees[k]->TreeType==Type){
					if(LandType!=TS->Nation){
						for(int q=0;q<TX->TreesGroup.GetAmount();q++){
							if(TX->TreesGroup[q]->Nation==LandType){
								TreesSequence* TS=TX->TreesGroup[q];
								if(TS->Trees.GetAmount()){
									int id=TS->Trees[rando()%TS->Trees.GetAmount()]->TreeType;
									OS->SGIndex=id;
									OS->OC=OS->SG->Objects[id];
									return true;
								}
							}
						}
					}
					return false;
				}
			}
		}
	}
	return false;
}
int GetSpriteLandType(OneSprite* OS){
	if(OS->SG!=&TREES)return -1;
	int Type=OS->SGIndex;
	for(int i=0;i<TreesExchangeScope.TreesExchange.GetAmount();i++){
		TreesExchangeList* TX=TreesExchangeScope.TreesExchange[i];
		for(int j=0;j<TX->TreesGroup.GetAmount();j++){
			TreesSequence* TS=TX->TreesGroup[j];
			for(int k=0;k<TS->Trees.GetAmount();k++){
				if(TS->Trees[k]->TreeType==Type){
					return TS->Nation;
				}
			}
		}
	}
	return -1;
}
void ChangeTreesInRadius(int x,int y,int R,int LandType){
	itr_GetSpritesInRadius.Create(x,y,R+1);
	while(OneSprite* OS=itr_GetSpritesInRadius.Next()){
		ChangeTreeType(OS,LandType);
	}
}
void ChangeTreesInRadiusWithEffect(int x,int y,int R,int LandType){
	if(itr_GetSpritesInRadius.Create(x,y,R+1)){		
		char* name=NULL;
		while(OneSprite* OS=itr_GetSpritesInRadius.Next()){
			if(!name){
				name="";
				for(int i=0;i<TreesExchangeScope.WeaponsToChangeTrees.GetAmount();i++){
					if(TreesExchangeScope.WeaponsToChangeTrees[i]->LandType==LandType){
						name=TreesExchangeScope.WeaponsToChangeTrees[i]->WeaponToChangeTree.pchar();
						break;
					}
				}
			}
			int tp=GetSpriteLandType(OS);
			if(tp!=-1 && tp!=LandType){
				if(name[0])CreateNewActiveWeapon(name,0xFFFF,OS->x,OS->y,OS->z,0xFFFF,OS->x,OS->y,OS->z+100,NULL);
				else ChangeTreeType(OS,LandType);
			}
		}
	}
}
void RegTreesExchangeEditor(){
	REG_CLASS(TreesExchangeList);
	TreesExchangeScope.SafeReadFromFile("Surface\\TreesExchange.xml");
	AddStdEditor("TreesExchangeEditor",&TreesExchangeScope,"Surface\\TreesExchange.xml",RCE_DEFAULT);
}

class SurfFuncForMC:public CubeTriangulator{
public:
	BaseSurfaceFunction* BF;
	int CellSize;
	virtual float GetValueF(float x,float y,float z){
		SurfVertex MV;
		x*=CellSize;
		y*=CellSize;
		z*=CellSize;
		MV.x=x;
		MV.y=y;
		MV.z=z;
        BF->ModifyVertex(MV);		
		int mv=-CellSize*14;
		if(MV.z<mv)MV.z=mv;
		float v=MV.z-z;
		GeneratorSurfaceFunction* GS=(GeneratorSurfaceFunction*)BF;
		if(x<1024 || y<1024 || x>GS->MapLx-1024 || y>GS->MapLy-1024){			
			int xx=x;
			int yy=y;
            GS->ShapeOfBound->ShiftXY(xx,yy);
			static float sq=-1600;
			float dz=z/4+z*z/sq;
			float v1=128-xx-dz;
			float v2=128-yy-dz;
			float v3=xx-GS->MapLx+200-dz;
			float v4=yy-GS->MapLy+200-dz;
            v1=min(-v1,-v2);
			v3=min(-v3,-v4);
			v1=min(v1,v3);
			//if(v<v1 && v<-CellSize*14+20)v=-CellSize*14+20;			
			v=min(v,v1);
		}
		return v;
	}
};

namespace Surface {
	void SurfMesh::ApproximateGenerator() {
		if(!VERTS.Count() || !TRIS.Count()) return;
		static const int ShiftIterations = 10;
		static const float dShift = 6.0f;
		static const int EdgeMaxLen = 32;
		static const float FirstRelaxAmount = 0.3f;
		static const int FirstRelaxIterations = 10;
		static const float SecondRelaxAmount = 0.3f;
		static const int SecondRelaxIterations = 1;


		// GeneratorSurfaceFunction
		GeneratorSurfaceFunction *sf = new GeneratorSurfaceFunction;
		sf->GPtr = &SURF;
		sf->ShapeOfBound = &SURF.ShapeOfBound;
		int CellSize = 32;
		int NX = cMath::Clamp(SURF.Size / CellSize, 40, 512);
		sf->MapLx = NX * CellSize;
		sf->MapLy = NX * CellSize;
		// SurfFuncForMC
		SurfFuncForMC *mc = new SurfFuncForMC;
		mc->CellSize = 1;
		mc->BF = sf;

		cBitMap BoundMap;
		GatherVertsBoundaryStats(BoundMap);
		SubdivideArgs Args;
		Args.Criterion = SubdivideArgs::C_EDGELEN;
		Args.EdgeMaxLen = EdgeMaxLen;
		Args.fAddRefToQuads = false;
		cVec3 u[3], n, NewPos;
		float w[3];
		int k, nVert;
		for(int I = 0; I < ShiftIterations; I++) {
			for(nVert = 0; nVert < VERTS.Count(); nVert++) {
				if(BoundMap[nVert]) continue;
				SurfVertex &r = VERTS[nVert];
				u[0].Set(r.x, r.y, r.z);
				n = cVec3::Normalize(cVec3(r.nx, r.ny, r.nz));
				u[1] = u[0] - dShift * n;
				u[2] = u[0] + dShift * n;
				for(k = 0; k < 3; k++) {
					w[k] = fabs(mc->GetValueF(u[k].x, u[k].y, u[k].z));
				}
				NewPos = u[cMath::MinIndex(w[0], w[1], w[2])];
				r.x = short(NewPos.x);
				r.y = short(NewPos.y);
				r.z = short(NewPos.z);
			}
			CalcNormals();
		}
		delete mc;
		delete sf;

		Subdivide(Args);
		Relax(FirstRelaxAmount, FirstRelaxIterations, 0, true);
		SimplifySurface(*this, 0);
		Relax(SecondRelaxAmount, SecondRelaxIterations, 0, true);
		CalcNormals();
		SURF.TexturizeSurface(*this);
		g_UniversalMap.m_Shader.Shade(*this, I_LOCAL);
	}
}


void SurfMesh::ApplyMCubes(int CellSize,int Nx,int Ny,BaseSurfaceFunction* Func){
	WholeTriangulation* WT=new WholeTriangulation;
	SurfFuncForMC* MC=new SurfFuncForMC;
	MC->CellSize=CellSize;
	MC->BF=Func;
	float sq6=sqrt(6.0f);
	float sq2=sqrt(2.0f);
	float sq3=sqrt(3.0f);
	float zscale=1.8;
	Matrix3D M(2/sq6,0,1/sq3,-1/sq6,1/sq2,1/sq3,-zscale/sq6,-zscale/sq2,zscale/sq3);
	Matrix4D M4=M;
	MC->SetTransform(M4,false);
	MC->GetFullTriangilationRecursive(5,5,0 , -15,-15,-16 , Nx+5,Ny+5,35 , *WT);
	Free();
	for(int i=0;i<WT->HVert.numElem();i++){
		SurfVertex MV;
		tagVector& tv=WT->HVert.elem(i);
		MV.x=tv.vec.x*CellSize;
		MV.y=tv.vec.y*CellSize;
		MV.z=tv.vec.z*CellSize;
		MV.Color=0xFF808080;
		MV.nFacture=0;
   		VERTS.Add(MV);
	}
	for(int i=0;i<WT->Idxs.size();i++){
		TRIS.Add(WT->Idxs[i]);
	}
	for(int i=0;i < NVerts();i++)
	{
		Func->ShiftVertex(VERTS[i]);
	}
	delete(WT);
	delete(MC);
}
class RasterMiniMap:public Rasterizator{
public:
	WORD* ptr;
	float* zb;
	int Lx;
	int Ly;
	int wndX;
	int wndY;
	int wndX1;
	int wndY1;
	WORD C;
	virtual void OnPoint(int x,int y,float z){
		if(x>=wndX && x<=wndX1 && y>=wndY && y<=wndY1){
			int ofs=x+y*Lx;
			if(zb == NULL || z>zb[ofs]){
				ptr[ofs]=C;
				if(zb)zb[ofs]=z;
			}
		}
	}
};
WORD DWORD2WORD(DWORD C);
extern bool DontShowHeight;

//-----------------------------------------------------------------------------
// CalcMiniMapTriColor
//-----------------------------------------------------------------------------
WORD CalcMiniMapTriColor(const SurfVertex &u) {
	const DWORD ShallowWaterColor = 0xff6088b6; // 96, 136, 182
	const DWORD DeepWaterColor = 0xff48668d; // 72, 102, 141
	const int DeepWaterDepth = 70;

	bool CheckPointForChasm(int x, int y);
	WORD Res = 0;
	if(u.z < 0.0f && !CheckPointForChasm((int)u.x, (int)u.y)) {
		// Water:
		const int l = (int)(255.0f * cMath::LerperClamp01(0.0f, DeepWaterDepth, -u.z));
		const DWORD CurWaterColor = MixDWORD(ShallowWaterColor, DeepWaterColor, 255 - l, l);
		const DWORD CurColor = MixDWORD(u.Color, CurWaterColor, 255 - l, l);
		Res = DWORD2WORD(CurColor) | 0xf000;
	} else {
		// Land:
		Res = DWORD2WORD(MulDWORD(u.Color, int(u.Luminance) * 2)) | 0xF000;
	}
	return Res;
}

void RenderSurfaceColorsToMiniMapPart(WORD* ptr,int Lx,int Ly,Rct& rect){

    SurfRef ref;
    g_UniversalMap.QUADS.GetRefRect(ref,rect.x,rect.y,rect.x+rect.w,rect.y+rect.h);
    ref.Complete(SR_TRIS, SR_QUADS);
    int ni=ref.idTris.Count();
    int h=!DontShowHeight;
    RasterMiniMap RM;
    RM.ptr=ptr;
    RM.Lx=Lx;
    RM.Ly=Ly;
    RM.wndX=0;
    RM.wndY=0;
    RM.wndX1=Lx-1;
    RM.wndY1=Ly-1;
    RM.zb=NULL;
    for(int i=0;i<ni;i++){
        SurfVertex* mv[3];
        g_UniversalMap.GetTri(ref.idTris[i],&mv[0]);
        int mx1=mv[0]->x*Lx/16384;
        int mx2=mv[1]->x*Lx/16384;
        int mx3=mv[2]->x*Lx/16384;

        if( (mx1>=0 && mx1<Lx) || 
            (mx1>=0 && mx1<Lx) ||
            (mx1>=0 && mx1<Lx) ){			
                int y1=(mv[0]->y)*Ly/16384;
                int y2=(mv[1]->y)*Ly/16384;
                int y3=(mv[2]->y)*Ly/16384;
//                RM.C=DWORD2WORD(MulDWORD(mv[0]->Color,int(mv[0]->Luminance)*2)) | 0xF000;
				RM.C = CalcMiniMapTriColor(*mv[0]); // ?????????????????????????????????????????????
                RM.Rasterize(Vector3D(mx1,y1,0),Vector3D(mx2,y2,0),Vector3D(mx3,y3,0));
            }

    }
}
void RenderSurfaceColorsToMiniMapPart(WORD* ptr,int Lx,int Ly,int wndX,int wndY,int wndLx,int wndLy){
	int ni=g_UniversalMap.TRIS.Count();
	int wndX1=wndX+wndLx-1;
	int wndY1=wndY+wndLy-1;
	if(wndX<0)wndX=0;
	if(wndY<0)wndY=0;
	if(wndX1>=Lx)wndX1=Lx-1;
	if(wndY1>=Ly)wndY1=Ly-1;
	RasterMiniMap RM;
	RM.ptr=ptr;
	RM.Lx=Lx;
	RM.Ly=Ly;
	RM.wndX=wndX;
	RM.wndY=wndY;
	RM.wndX1=wndX1;
	RM.wndY1=wndY1;
    RM.zb=NULL;
    /*
	RM.zb=new float[Lx*Ly];
	for(int i=wndX;i<=wndX1;i++){
		for(int j=wndY;j<=wndY1;j++){
            RM.zb[i+j*Lx]=-10000;
			ptr[i+j*Lx]=0;
		}
	}
    */
	int h=!DontShowHeight;
	for(int i=0;i<ni;i+=3){
        SurfVertex& mv1=g_UniversalMap.VERTS[g_UniversalMap.TRIS[i  ]];
		SurfVertex& mv2=g_UniversalMap.VERTS[g_UniversalMap.TRIS[i+1]];
		SurfVertex& mv3=g_UniversalMap.VERTS[g_UniversalMap.TRIS[i+2]];

		int mx1=mv1.x*Lx/16384;
		int mx2=mv2.x*Lx/16384;
		int mx3=mv3.x*Lx/16384;

		if( (mx1>=wndX && mx1<=wndX1) || 
			(mx1>=wndX && mx1<=wndX1) ||
			(mx1>=wndX && mx1<=wndX1) ){			
            int y1=(mv1.y)*Ly/16384;
			int y2=(mv2.y)*Ly/16384;
			int y3=(mv3.y)*Ly/16384;
//			RM.C=DWORD2WORD(MulDWORD(mv1.Color,int(mv1.Luminance)*2)) | 0xF000;
			RM.C = CalcMiniMapTriColor(mv1); // ?????????????????????????????????????????????????
			RM.Rasterize(Vector3D(mx1,y1,0),Vector3D(mx2,y2,0),Vector3D(mx3,y3,0));
		}
	
    }
	//delete[]RM.zb;
}
int GetLandTypeInPoint(int x,int y){
	float maxw=0;
	int lt=-1;
	int n=MLayers->Maps.GetAmount();
	for(int i=0;i<n;i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
        float w=MLayers->Maps[i]->GetGreyPrecise(x/15360.0,y/15360.0);
		if(w>maxw){
			lt=i;
			maxw=w;
		}
	}
	return lt;
}
int GetNFactures();
int GetFactureBoundState(int f);
void LayersList::CreateBoundFactures(){
	int N=GetNFactures();
	for(int i=0;i<N;i++){
		int s=GetFactureBoundState(i);
		if(s>65535){
			s&=65535;
			if(s<N){
                LAYERSLIST.Layers[s]->BoundaryFactures.Add(i);
			}
		}
	}
}
void RecreateBoundaryFacturesList(){
	LAYERSLIST.CreateLayerIndex();
	LAYERSLIST.CreateBoundFactures();
}
//void CalculateBoundary(SurfRef& Ref){
//	int n=Ref.idTris.Count();
//	for(int i=0;i<n;i++){
//		int nt=Ref.idTris[i];
//		int i0,i1,i2;
//		Ref.pOrig->GetTri(nt,i0,i1,i2);
//		SurfVertex& v1=Ref.pOrig->VERTS[i0];
//		SurfVertex& v2=Ref.pOrig->VERTS[i1];
//		SurfVertex& v3=Ref.pOrig->VERTS[i2];		
//
//		int L1=GetFactureBoundState(v1.nFacture);
//		int L2=GetFactureBoundState(v2.nFacture);
//		int L3=GetFactureBoundState(v3.nFacture);
//
//		int L1X=L1==-1 ? -1 : L1&65535;
//		int L2X=L2==-1 ? -1 : L2&65535;
//		int L3X=L3==-1 ? -1 : L3&65535;
//
//		if( L1>=0 && L1<65536 && ( L2X!=L1X || L3X!=L1X) ){
//			DynArray<int>& di=LAYERSLIST.Layers[L1]->BoundaryFactures;
//			int nf=di.GetAmount();
//			if(nf)v1.nFacture=di[rand()%nf];
//		}
//
//		if( L2>=0 && L2<65536 && ( L1X!=L2X || L3X!=L2X) ){
//			DynArray<int>& di=LAYERSLIST.Layers[L2]->BoundaryFactures;
//			int nf=di.GetAmount();
//			if(nf)v2.nFacture=di[rand()%nf];
//		}
//
//		if( L3>=0 && L3<65536 && ( L1X!=L3X || L2X!=L3X) ){
//			DynArray<int>& di=LAYERSLIST.Layers[L3]->BoundaryFactures;
//			int nf=di.GetAmount();
//			if(nf)v3.nFacture=di[rand()%nf];
//		}
//	}
//}
void CalculateBoundary(SurfRef& Ref){
	int n=Ref.idTris.Count();
	for(int i=0;i<n;i++){
		int nt=Ref.idTris[i];
		int i0,i1,i2;
		Ref.pOrig->GetTri(nt,i0,i1,i2);
		SurfVertex& v1=Ref.pOrig->VERTS[i0];
		SurfVertex& v2=Ref.pOrig->VERTS[i1];
		SurfVertex& v3=Ref.pOrig->VERTS[i2];		

		/*int L1=LAYERSLIST.GetLayerIndex(v1.Color);
		int L2=LAYERSLIST.GetLayerIndex(v2.Color);
		int L3=LAYERSLIST.GetLayerIndex(v3.Color);*/

		int L1=v1.RaceIndex?v1.RaceIndex+LAYERSLIST.MinRaceLandIndex-1:0xFF;
		int L2=v2.RaceIndex?v2.RaceIndex+LAYERSLIST.MinRaceLandIndex-1:0xFF;
		int L3=v3.RaceIndex?v3.RaceIndex+LAYERSLIST.MinRaceLandIndex-1:0xFF;
		int NN=LAYERSLIST.Layers.GetAmount();
		
		if( L1<NN && ( L2!=L1 || L3!=L1) && v1.nz>110){
			ClonesArray<BoundaryFacture>& di=LAYERSLIST.Layers[L1]->Boundary;
			int nf=di.GetAmount();
			if(nf)v1.nFacture=di[rand()%nf]->Facture;
		}

		if( L2<NN && ( L1!=L2 || L3!=L2) && v2.nz>110){
			ClonesArray<BoundaryFacture>& di=LAYERSLIST.Layers[L2]->Boundary;
			int nf=di.GetAmount();
			if(nf)v2.nFacture=di[rand()%nf]->Facture;
		}

		if( L3<NN && ( L1!=L3 || L2!=L3) && v3.nz>110){
			ClonesArray<BoundaryFacture>& di=LAYERSLIST.Layers[L3]->Boundary;
			int nf=di.GetAmount();
			if(nf)v3.nFacture=di[rand()%nf]->Facture;
		}
	}
}
void LayersList::CreateLayerIndex(){
	for(int i=0;i<LAYERSLIST.Layers.GetAmount();i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
        MinRaceLandIndex=i;
		return;
	}
	for(int r=0;r<16;r++){
		for(int g=0;g<16;g++){
			for(int b=0;b<16;b++){
				byte idx=0xFF;
				int mind=100000000;
				for(int i=0;i<LAYERSLIST.Layers.GetAmount();i++)if(LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
					ClonesArray<ColorSample>& CA=LAYERSLIST.Layers[i]->Colors;
					for(int p=0;p<CA.GetAmount();p++){
						DWORD C=CA[p]->Color;
						if(C){
							int R=(C>>20)&15;
							int G=(C>>12)&15;
							int B=(C>>4 )&15;
							int N=sqrt(R*R+G*G+B*B);
							if(N==0)N=1;
                            R=R*256/N;
							G=G*256/N;
							B=B*256/N;
							N=sqrt(r*r+g*g+b*b);
							if(N==0)N=1;
							int rr=r*256/N;
							int gg=g*256/N;
							int bb=b*256/N;
							int d=(rr-R)*(rr-R)+(gg-G)*(gg-G)+(bb-B)*(bb-B);
							if(d<mind){
								idx=i;
								mind=d;
							}
						}
					}
				}
				LayerIndex[r+g*16+b*16*16]=idx;
			}
		}
	}
}
int LayersList::GetLayerIndex(DWORD C){
	int R=(C>>20)&15;
	int G=(C>>12)&15;
	int B=(C>>4 )&15;
	return LayerIndex[R+G*16+B*16*16];
}
bool CheckPointForChasm(int x,int y){
	int NL=MLayers->Maps.GetAmount();	
	for(int i=0;i<NL;i++)if(LAYERSLIST.Layers[i]->DontGenerateWater){
		if(DistField::Get(i,x/15360.0,y/15360.0)>-15){
			return true;
		}
	}
    return false;
}
bool ProcessLoadSave(int x,int y,int Lx,int Ly,_str& str,const char* mask,const char* StartDir,bool Load,ParentFrame* Back);
byte GetShadowInPoint(int x,int y){
	SurfRef Ref;
	g_UniversalMap.GetRefCylinder(Ref, SurfVertex(x, y), 64, Surface::SR_VERTS);
	int minr=10000;
	byte sh=128;
	for(int i=0; i < Ref.idVerts.Count(); i++) {
		SurfVertex& V = g_UniversalMap.VERTS[Ref.idVerts[i]];
		int d=Norma(x-V.x,y-V.y);
		if(d<minr){
			minr=d;
			sh=V.Luminance;
		}
	}
	return sh;
}
void SurfaceGenerator::SaveCurrentLayer(){
	if(Maps.TopLayerIndex!=-1){
		GreyMap* GM=(GreyMap*)MLayers->Maps[Maps.TopLayerIndex];
		int N=GM->Lx*GM->Ly;
		BYTE* x=new BYTE[N*3];
		int i3=0;
		for(int i=0;i<N;i++){
			x[i3]=x[i3+1]=x[i3+2]=GM->GetGreyScale(i%GM->Lx,i/GM->Lx);
            i3+=3;
		}
		_str Name;
		if(ProcessLoadSave(0,0,RealLx,RealLy,Name,"*.bmp","",false,NULL)){
			SaveToBMP24(Name.pchar(),GM->Lx,GM->Ly,x);
		}
		delete[]x;
	}
}
float GetInterpValueFromBitmap(BYTE* data,int bLx,int bLy,float x,float y){//x,y=[0..1]
	if(x<0)x=0;
	if(y<0)y=0;
	if(x>1)x=1;
	if(y>1)y=1;
	float x1=float(x)*(bLx-1);
	float y1=float(y)*(bLy-1);
	int xi=floorf(x1);
	int yi=floorf(y1);
	int xi1=xi+1;
	int yi1=yi+1;
	if(xi1>bLx-1)xi1=bLx-1;
	if(yi1>bLy-1)yi1=bLy-1;
	float dx=x1-xi;
	float dy=y1-yi;
	float C0=data[(xi+yi*bLx)*3];
	float C1=data[(xi1+yi*bLx)*3];
	float C2=data[(xi+yi1*bLx)*3];
	float C3=data[(xi1+yi1*bLx)*3];
	float C=C0+(C1-C0)*dx+(C2-C0)*dy+(C0+C3-C1-C2)*dx*dy;
	if(C<0)C=0;
	if(C>255)C=255;  
	return C;
}
void SurfaceGenerator::LoadCurrentLayer(){
	_str Name;
	if(ProcessLoadSave(0,0,RealLx,RealLy,Name,"*.bmp","",true,NULL)){
		BitHeight BH;
		if(BH.Load(Name.pchar())){
			GreyMap* GM=(GreyMap*)MLayers->Maps[Maps.TopLayerIndex];
			for(int y=0;y<GM->Ly;y++){
				for(int x=0;x<GM->Lx;x++){
					float x1=float(x)/(GM->Lx-1);
					float y1=float(y)/(GM->Ly-1);
					GM->SetGreyScale(x,y,BH.get(x1,y1));
				}
			}
			GM->CreateTexturePart(0,0,GM->Lx,GM->Ly);
		}
	}	
}
void SurfaceGenerator::ClearCurrentLayer(){
	if(Maps.TopLayerIndex!=-1){
        MLayers->Maps[Maps.TopLayerIndex]->Clear();
	}
}
SurfaceGenerator::SurfaceGenerator(){
    
}
SurfaceGenerator::~SurfaceGenerator(){
    
}
BitHeight::BitHeight(){
	Data=NULL;
	bLx=2;
	bLy=2;
}
BitHeight::~BitHeight(){
    if(Data)free(Data);
	Data=NULL;
}
float BitHeight::get(float x,float y){
	if(Data)return GetInterpValueFromBitmap(Data,bLx,bLy,x,y);
	else return 0.0f;
}
bool BitHeight::Load(char* Name){
	Clear();
	BMPformat BM;
	bool R=ReadBMP24(Name,&BM,&Data);
	bLx=BM.biWidth;
	bLy=BM.biHeight;
	return R;
}
void BitHeight::Clear(){
	if(Data)free(Data);
	Data=NULL;
}