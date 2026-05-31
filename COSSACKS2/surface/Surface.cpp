#include "stdheader.h"
#include "Surface.h"
#include "ITerrain.h"
#include "..\Camera\Navigator.h"
#include "SurfaceGenerator.h"
#include "Colorize.h"
#include "..\CurrentMapOptions.h"
#include "SurfSplit.h"
#include "mLodder.h"
#include "..\CurrentMapOptions.h"
#include "..\html\html.h"
#include "Layer.h"
extern short* DeepMap;
extern bool g_bPerspCamera;
#include "CollidingCamera.h"
#include "Messager.h"

#include "..\..\mesh\UniHash.h"
#include "..\..\mesh\iVector.h"
#include "..\..\mesh\MeshOperations.h"

bool FastBool=false;
float minans=0.2;
float maxans=5;

int getNmtl(BasicMesh* m){
    int N=0;
    scan(m->Triangles,BasicTriangle* T,DWORD* P){
        if(T->MtlID)N++;
    }scan_end;
    return N;
}

//-----------------------------------------------------------------------------
// ConvertMesh : SurfMesh -> BasicMesh
//-----------------------------------------------------------------------------
void ConvertMesh(const SurfMesh &Fm, BasicMesh *To) {
	// Verts:
	for(int i = 0; i < Fm.VERTS.Count(); i++) {
		const SurfVertex &u = Fm.VERTS[i];
		To->AddVertex(dVector(u.x, u.y, u.z), i);
	}
	// Tris:
	const int NTris = Fm.NTris();
	int i[3], idMtl;
	SurfVertex u[3];
	for(int nTri = 0; nTri < NTris; nTri++) {
		Fm.GetTri(nTri, i);
		Fm.GetTri(nTri, u);
		if(u[0].SurfaceType == u[1].SurfaceType && u[0].SurfaceType == u[2].SurfaceType) {
			idMtl = u[0].SurfaceType;
		} else {
			idMtl = 0;
		}
		To->AddTriangle(i[0], i[1], i[2], idMtl);
	}
} // ConvertMesh

//-----------------------------------------------------------------------------
// ConvertMesh: BasicMesh -> SurfMesh
//-----------------------------------------------------------------------------
void ConvertMesh(BasicMesh &Fm, SurfMesh *To) {
	To->Free();

	// Verts:
	SurfVertex u;
	for(int i = 0; i < Fm.Verts.size(); i++) {
		const dVector &p = Fm.Verts[i]->Pos;
		u.x = (float)p.x;
		u.y = (float)p.y;
		u.z = (float)p.z;
		To->VERTS.Add(u);
	}
	// Tris:
	for(int nTri = 0; nTri < Fm.Triangles.size(); nTri++) {
		const BasicTriangle *t = Fm.Triangles[nTri];
		To->TRIS.Add(t->nIndA);
		To->TRIS.Add(t->nIndB);
		To->TRIS.Add(t->nIndC);
		To->VERTS[t->nIndA].SurfaceType = t->MtlID;
		To->VERTS[t->nIndB].SurfaceType = t->MtlID;
		To->VERTS[t->nIndC].SurfaceType = t->MtlID;
	}
} // ConvertMesh

int cHashIndex::GenerateKey(const Surface::SurfVertex &v) const {
	return ((int)v.x + (int)v.y + (int)v.z) & m_HashMask;
}

//Surface::SurfMap g_UniversalMapBackup;
Surface::SurfMap g_UniversalMap(Surface::SurfShader(), NULL);//, &g_UniversalMapBackup);
SurfEditor g_SurfEditor;
extern SurfaceGenerator SURF;

//-----------------------------------------------------------------------------
// SurfArchway
//-----------------------------------------------------------------------------
SurfArchway::SurfArchway()
: cx(0), cy(0), h(0), r(0) {}

SurfArchway::SurfArchway(int cx, int cy, int h, int r) {
	this->cx = cx;
	this->cy = cy;
	this->h = h;
	this->r = r;
}

bool SurfArchway::Equals(const SurfArchway &Src) const {
	const int Epsilon = 200;
	return abs(cx - Src.cx) < Epsilon && abs(cy - Src.cy) < Epsilon;
}


bool SurfArchway::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Archway (Pos = [%$cx$%, %$cy$%], Height = %$h$%, Radius = %$r$%)";
	return true;
}

bool SurfArchway::CheckIfElementInvisible(int, void *, DWORD) {
	return true;
}

//-----------------------------------------------------------------------------
// SurfCamera
//-----------------------------------------------------------------------------
SurfCamera::SurfCamera()
: xAt(0), yAt(0), zAt(0), Azim(0), Elev(0), toEye(0), toEyeEx(0) {}

SurfCamera::SurfCamera(const dVector &At, double Azim, double Elev, double toEye, double toEyeEx) {
	xAt = (float)At.x;
	yAt = (float)At.y;
	zAt = (float)At.z;
	this->Azim = (float)cMath::Deg(Azim);
	this->Elev = (float)cMath::Deg(Elev);
	this->toEye = (float)toEye;
	this->toEyeEx = (float)toEyeEx;
}

bool SurfCamera::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Camera ";
	for(int i = 0; i < g_SurfEditor.SaveCamera.Cameras.NValues; i++) {
		if((SurfCamera*)g_SurfEditor.SaveCamera.Cameras[i] == this) {
			Mask += i;
			break;
		}
	}
	return true;
}

void SurfCamera::EvaluateFunction() {
	if(!g_SurfEditor.FreeCamera) {
		g_SurfEditor.FreeCamera = true;
	}
	g_Navigator.Copy(*this);
	SetupCamera();
	ResetGroundCache();
}

bool SurfCamera::CheckIfElementInvisible(int Index, void *, DWORD) {
	return Index > 0;
}

void SurfCamera::SurfCameraRemove::EvaluateFunction() {
	char RemoveText[] = "Are you sure you want to remove this camera?";
	char RemoveCaption[] = "Confirm Remove Camera";
	if(MessageBox(g_hWnd, RemoveText, RemoveCaption, MB_YESNO) != IDYES) return;
	for(int i = 0; i < g_SurfEditor.SaveCamera.Cameras.NValues; i++) {
		if((SurfCamera*)g_SurfEditor.SaveCamera.Cameras[i] == (SurfCamera*)ParentBC) {
			g_SurfEditor.SaveCamera.Cameras.DelElement(i);
			break;
		}
	}
}

namespace Surface {
	
//-----------------------------------------------------------------------------
// SurfEditor::SurfaceTool::SetName
//-----------------------------------------------------------------------------
void SurfEditor::SurfaceTool::SetName(const char *pStr) {
	m_ToolName.Copy(pStr);
}

#define SURFACE_CLOSEEDITOR	SurfEditor *pSurfaceEditor = (SurfEditor*)ParentBC;\
		assert(pSurfaceEditor);\
		pSurfaceEditor->m_fActive = false;

//-----------------------------------------------------------------------------
// SurfEditor::SurfaceTool::EvaluateFunction
//-----------------------------------------------------------------------------
void SurfEditor::SurfaceTool::EvaluateFunction() {
	SURFACE_CLOSEEDITOR;
	ActivateSurfTool(m_ToolName);
}

//-----------------------------------------------------------------------------
// CmdRefresh
//-----------------------------------------------------------------------------
void SurfEditor::CmdRefresh::Defaults() {
	Illumination = I_NONE;
	LODs = false;
}

void SurfEditor::CmdRefresh::EvaluateFunction() {
	SURFACE_CLOSEEDITOR;
	ActivateSurfTool("Refresh");
}

bool SurfEditor::CmdRefresh::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	const char *Types[] = {"No Illumination", "Local Illumination", "Direct Illumination"};
	Mask = "Refresh (";
	Mask += Types[Illumination];
	Mask += ")";
	return true;
}

bool SurfEditor::CmdRefresh::OnChangeMember(BaseClass *, void *, void *, const char *) {
	if(!LODs) {
		g_UniversalMap.QUADS.fUseLods = false;
	}
	return false;
}

//-----------------------------------------------------------------------------
// CmdSimplify
//-----------------------------------------------------------------------------
void SurfEditor::CmdSimplify::EvaluateFunction() {
	char SimplifyText[] = "Are you sure you want to simplify the whole surface?";
	char SimplifyCaption[] = "Confirm Simplify";
	if(MessageBox(g_hWnd, SimplifyText, SimplifyCaption, MB_YESNO) == IDYES) {
		SURFACE_CLOSEEDITOR;
		ActivateSurfTool("Simplify");
	}
}

//-----------------------------------------------------------------------------
// CmdFillGenerator
//-----------------------------------------------------------------------------
void SurfEditor::CmdFillGenerator::EvaluateFunction() {
	char Text[] = "Are you sure you want to fill generator from surface?";
	char Caption[] = "Confirm Fill Generator";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		SurfGenClear();
		for(int i = 0; i < g_UniversalMap.VERTS.Count(); i++) {
			const SurfVertex &r = g_UniversalMap.VERTS[i];
			int idRace = r.RaceIndex + LAYERSLIST.MinRaceLandIndex - 1;
			if(idRace >= LAYERSLIST.MinRaceLandIndex) {
				RaceRound(idRace, r);
			}
		}
		SaveGenLayers();
		SURFACE_CLOSEEDITOR;
	}
}

//-----------------------------------------------------------------------------
// CmdCutRect
//-----------------------------------------------------------------------------
bool SurfEditor::CmdCutRect::Update() {
	bool fChanged = false;

#define Check(x0, px0, x1, px1) if(x0 != px0) { px0 = x0; fChanged = true; if(x1 < x0) { px1 = x1 = x0; }}
	Check(x0, px0, x1, px1);
	Check(y0, py0, y1, py1);
#undef Check
#define Check(x0, px0, x1, px1) if(x0 != px0) { px0 = x0; fChanged = true; if(x1 > x0) { px1 = x1 = x0; }}
	Check(x1, px1, x0, px0);
	Check(y1, py1, y0, py0);
#undef Check

	const int Lo = 0, Hi = 20000;
	X0 = (int)(cMath::Lerp((float)Lo, (float)Hi, x0) + 0.5f);
	Y0 = (int)(cMath::Lerp((float)Lo, (float)Hi, y0) + 0.5f);
	X1 = (int)(cMath::Lerp((float)Lo, (float)Hi, x1) + 0.5f);
	Y1 = (int)(cMath::Lerp((float)Lo, (float)Hi, y1) + 0.5f);
	return fChanged;
}

void SurfEditor::CmdCutRect::Defaults() {
	x0 = px0 = y0 = py0 = 0.0f;
	x1 = px1 = y1 = px1 = 1.0f;
	Update();
}

bool SurfEditor::CmdCutRect::OnChangeMember(BaseClass *, void *, void *, const char *) { return Update(); }
bool SurfEditor::CmdCutRect::CheckIfElementInvisible(int index, void *, DWORD) { return index > 3; }

bool SurfEditor::CmdCutRect::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Cut Rect (Min(%$X0$%, %$Y0$%), Max(%$X1$%, %$Y1$%))";
	return true;
}

void SurfEditor::CmdCutRect::EvaluateFunction() {
	char Text[] = "Are you sure you want to cut rect from the map?";
	char Caption[] = "Confirm Cut Rect";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) != IDYES) return;
	//g_UniversalMap.BackUp();
	SurfRef Ref;
	g_UniversalMap.QUADS.GetRefRect(Ref, X0, Y0, X1, Y1);
	Ref.Complete(SR_TRIS, SR_QUADS);
	SurfaceUndo::DropRefToUndo(Ref);
	Ref.RemoveUnreferredTris();
	g_UniversalMap.QUADS.Init();
	g_UniversalMap.QUADS.Compile();
	ResetGroundCache();
	SURFACE_CLOSEEDITOR;
}

//-----------------------------------------------------------------------------
// CmdCreateNewMap
//-----------------------------------------------------------------------------
void SurfEditor::CmdCreateNewMap::Defaults() {
	pxSize = xSize = 1.0f;
	pySize = ySize = 1.0f;
	pCell = Cell = 0.5f;
	pBaseHeight = BaseHeight = 0.5f;
	pNoisy = Noisy = false;
	Update();
}

void SurfEditor::CmdCreateNewMap::EvaluateFunction() {
	char NewMapText[] = "Are you sure you want to create new map?";
	char NewMapCaption[] = "Confirm Create New Map";
	if(MessageBox(g_hWnd, NewMapText, NewMapCaption, MB_YESNO) == IDYES) {
		//g_UniversalMap.BackUp();
		g_UniversalMap.Create(Lx, Ly, D, H, N);
		ResetGroundCache();
		SURFACE_CLOSEEDITOR;
	}
}

void SurfEditor::CmdCreateNewMap::Update() {
	Lx = int(MaxLx * xSize + 0.5f);
	Ly = int(MaxLy * ySize + 0.5f);
	D = MinCell + int((MaxCell - MinCell) * Cell + 0.5f);
	H = MinBaseHeight + int((MaxBaseHeight - MinBaseHeight) * BaseHeight + 0.5f);
	N = Noisy;
}

bool SurfEditor::CmdCreateNewMap::OnChangeMember(BaseClass *, void *, void *, const char *) {
	Update();
	if(xSize != pxSize) {
		pxSize = xSize;
		return true;
	} else if(ySize != pySize) {
		pySize = ySize;
		return true;
	} else if(Cell != pCell) {
		pCell = Cell;
		return true;
	} else if(BaseHeight != pBaseHeight) {
		pBaseHeight = BaseHeight;
		return true;
	} else if(Noisy != pNoisy) {
		pNoisy = Noisy;
		return true;
	}
	return false;
}

bool SurfEditor::CmdCreateNewMap::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Create New Map (Size = [%$Lx$% x %$Ly$%], Cell = %$D$%, BaseHeight = %$H$%, Noisy = %$N$%)";
	return true;
}

bool SurfEditor::CmdCreateNewMap::CheckIfElementInvisible(int Index, void *, DWORD) {
	switch(Index) {
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			return true;
		default:
			return false;
	}
}

//-----------------------------------------------------------------------------
// ShaderOptions
//-----------------------------------------------------------------------------
void SurfEditor::ShaderOptions::Defaults() {
	pAzimuth = Azimuth = 0.084f;
	pElevation = Elevation = 0.334f;
	pVar = Var = 0.1f;
	pAmbientIllumination = AmbientIllumination = 0.56f;
	pPhoton = Photon = 0.13f;
	pQtyOfLights = QtyOfLights = 0.52f;
	pSeed = Seed = 0.0f;
	pZBufferSide = ZBufferSide = 0.0f;
	Update();
}

void SurfEditor::ShaderOptions::Update() {
	aAzim = int(360.0f * Azimuth + 0.5f);
	aElev = int(90.0f * Elevation + 0.5f);
	aVar = int(60.0f * Var + 0.5f);
	aAmbientIllumination = AmbientIllumination;
	aPhoton = Photon;
	aQtyOfLights = 1 + int(19.0f * QtyOfLights + 0.5f);
	aSeed = int(100.0f * Seed + 0.5f);
	if(cMath::IsInRange(ZBufferSide, 0.0f, 0.3f)) {
		aZBufferSide = 4096;
	} else if(cMath::IsInRange(ZBufferSide, 0.3f, 0.6f)) {
		aZBufferSide = 6144;
	} else if(cMath::IsInRange(ZBufferSide, 0.6f, 1.0f)) {
		aZBufferSide = 8192;
	}
	g_UniversalMap.m_Shader.Init(cMath::Rad(aAzim), cMath::Rad(90 - aElev),
		aAmbientIllumination, aPhoton, aQtyOfLights, cMath::Rad(aVar), aSeed, aZBufferSide);
}

bool SurfEditor::ShaderOptions::OnChangeMember(BaseClass *, void *, void *, const char *) {
	Update();
	if(Elevation != pElevation) {
		pElevation = Elevation;
		return true;
	} else if(Azimuth != pAzimuth) {
		pAzimuth = Azimuth;
		return true;
	} else if(Var != pVar) {
		pVar = Var;
		return true;
	} else if(AmbientIllumination != pAmbientIllumination) {
		pAmbientIllumination = AmbientIllumination;
		return true;
	} else if(Photon != pPhoton) {
		pPhoton = Photon;
		return true;
	} else if(QtyOfLights != pQtyOfLights) {
		pQtyOfLights = QtyOfLights;
		return true;
	} else if(Seed != pSeed) {
		pSeed = Seed;
		return true;
	} else if(ZBufferSide != pZBufferSide) {
		pZBufferSide = ZBufferSide;
		return true;
	}
	return false;
}

bool SurfEditor::ShaderOptions::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Shader (A = %$aAzim$%,E = %$aElev$%,V = %$aVar$%,Ia = %$aAmbientIllumination$%";
	Mask += ",P = %$aPhoton$%,Q = %$aQtyOfLights$%,S = %$aSeed$%,Z = %$aZBufferSide$%)";
	return true;
}

bool SurfEditor::ShaderOptions::CheckIfElementInvisible(int Index, void *, DWORD) {
	switch(Index) {
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			return true;
		default:
			return false;
	}
}

void SurfEditor::ShaderOptions::ShaderReset::EvaluateFunction() {
	char ResetText[] = "Are you sure you want to reset shader to defaults?";
	char ResetCaption[] = "Confirm Reset Shader";
	if(MessageBox(g_hWnd, ResetText, ResetCaption, MB_YESNO) == IDYES) {
		SurfEditor::ShaderOptions *pShaderOptions = (SurfEditor::ShaderOptions*)ParentBC;
		assert(pShaderOptions);
		pShaderOptions->Defaults();
	}
}

//-----------------------------------------------------------------------------
// LODSOptions
//-----------------------------------------------------------------------------
void SurfEditor::LODSOptions::Defaults() {
	plb = lb = 0.3f;
	Update();
}

void SurfEditor::LODSOptions::Update() {
	LodBias = 100 * lb;
}

bool SurfEditor::LODSOptions::OnChangeMember(BaseClass *, void *, void *, const char *) {
	Update();
	if(lb != plb) {
		plb = lb;
		return true;
	}
	return false;
}

bool SurfEditor::LODSOptions::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "LODS (Bias = %$LodBias$%)";
	return true;
}

bool SurfEditor::LODSOptions::CheckIfElementInvisible(int Index, void *, DWORD) {
	switch(Index) {
		case 1:
			return true;
		default:
			return false;
	}
}

//-----------------------------------------------------------------------------
// CmdShift
//-----------------------------------------------------------------------------
void SurfEditor::CmdShift::Defaults() {
	Delta = 0;
//	pDelta = Delta = 0.5f;
//	Update();
}
/*
void SurfEditor::CmdShift::Update() {
	D = MinD + (MaxD - MinD) * Delta;
}

bool SurfEditor::CmdShift::OnChangeMember(BaseClass *, void *, void *, const char *) {
	Update();
	if(pDelta != Delta) {
		pDelta = Delta;
		return true;
	}
	return false;
}*/

bool SurfEditor::CmdShift::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Shift (Delta = ";
	Mask += Delta;
	Mask += ")";
	return true;
}
/*
bool SurfEditor::CmdShift::CheckIfElementInvisible(int Index, void *, DWORD) {
	switch(Index) {
		case 1:
			return true;
		default:
			return false;
	}
}*/

void SurfEditor::CmdShift::EvaluateFunction() {
	char Text[] = "Are you sure you want to shift map?";
	char Caption[] = "Confirm Shift";
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		//g_UniversalMap.BackUp();
		SurfaceUndo::DropWholeMap();
		for(int i = 0; i < g_UniversalMap.VERTS.Count(); i++) {
			SurfVertex &r = g_UniversalMap.VERTS[i];
			r.z += Delta;
		}
		for(i = 0; i < MOptions.Archways.NValues; i++) {
			SurfArchway *pWay = MOptions.Archways[i];
			pWay->h += Delta;
		}
		g_UniversalMap.QUADS.Init();
		g_UniversalMap.FillMapOfHeights();
		ResetGroundCache();
		SURFACE_CLOSEEDITOR;
	}
}

//-----------------------------------------------------------------------------
// SurfEditor::CmdSaveCamera
//-----------------------------------------------------------------------------
void SurfEditor::CmdSaveCamera::EvaluateFunction() {
	if(!g_SurfEditor.FreeCamera) {
		MessageBox(g_hWnd, "Only FreeCamera can be saved!", "--- ERROR ---", 0);
		return;
	}
	char SaveCameraText[] = "Are you sure you want to save current camera?";
	char SaveCameraCaption[] = "Confirm Save Camera";
	if(MessageBox(g_hWnd, SaveCameraText, SaveCameraCaption, MB_YESNO) == IDYES) {
		Cameras.Add(new SurfCamera(g_Navigator.m_Args.At, g_Navigator.m_Args.Azim,
			g_Navigator.m_Args.Elev, g_Navigator.m_Args.toEye, g_Navigator.m_Args.toEyeEx));
		SURFACE_CLOSEEDITOR;
	}
}

bool SurfEditor::CmdSaveCamera::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Save Cur Camera";
	return true;
}

//-----------------------------------------------------------------------------
// SurfEditor::CmdInvertTris::EvaluateFunction
//-----------------------------------------------------------------------------
void SurfEditor::CmdInvertTris::EvaluateFunction() {
	char InvertTrisText[] = "Are you sure you want to invert tris?";
	char InvertTrisCaption[] = "Confirm Invert Tris";
	if(MessageBox(g_hWnd, InvertTrisText, InvertTrisCaption, MB_YESNO) == IDYES) {
		g_UniversalMap.InvertTris();
		ResetGroundCache();
		SURFACE_CLOSEEDITOR;
	}
}

bool SurfEditor::CmdInvertTris::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Invert Tris";
	return true;
}

//-----------------------------------------------------------------------------
// SurfEditor::CmdExport
//-----------------------------------------------------------------------------
void SurfEditor::CmdExport::EvaluateFunction() {
	ActivateSurfTool("Exporter");
	SURFACE_CLOSEEDITOR;
}

//-----------------------------------------------------------------------------
// CmdUndo
//-----------------------------------------------------------------------------
void SurfEditor::CmdUndo::EvaluateFunction() {
	g_UniversalMap.Undo();
	ResetGroundCache();
	SURFACE_CLOSEEDITOR;
}

//-----------------------------------------------------------------------------
// SurfRacist
//-----------------------------------------------------------------------------
void SurfEditor::SurfRacist::Defaults() {
	Enumerator *E = ENUM.Get("RACES");
	assert(E);
	assert(E->GetAmount());
	Race = E->GetVal(0);
	Update();
}

bool SurfEditor::SurfRacist::Update() {
	bool fChanged = false;
	RACE = Race;
	if(Race != pRace) {
		pRace = Race;
		fChanged = true;
	}
	return fChanged;
}

bool SurfEditor::SurfRacist::OnChangeMember(BaseClass *, void *, void *, const char *) {
	return Update();
}

bool SurfEditor::SurfRacist::CheckIfElementInvisible(int index, void *, DWORD) {
	return index > 0;
}

bool SurfEditor::SurfRacist::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Racist (";
	Mask += MLayers->MapsNames[Race]->pchar();
	Mask += ")";
	return true;
}

//-----------------------------------------------------------------------------
// SurfLayer
//-----------------------------------------------------------------------------
void SurfEditor::SurfLayer::Defaults() {
	Enumerator *E = ENUM.Get("LAYERSLIST");
	assert(E);
	assert(E->GetAmount());
	Layer = E->GetVal(0);
	Update();
}

bool SurfEditor::SurfLayer::Update() {
	bool fChanged = false;
	LAYER = Layer;
	if(Layer != pLayer) {
		pLayer = Layer;
		fChanged = true;
	}
	return fChanged;
}

bool SurfEditor::SurfLayer::OnChangeMember(BaseClass *, void *, void *, const char *) {
	return Update();
}

bool SurfEditor::SurfLayer::CheckIfElementInvisible(int index, void *, DWORD) {
	return index > 0;
}

bool SurfEditor::SurfLayer::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Layer (";
	Mask += MLayers->MapsNames[Layer]->pchar();
	Mask += ")";
	return true;
}

int SurfEditor::SurfLayer::SelLayerIndex() const {
	Enumerator *E = ENUM.Get("SURFLAYERS");
	return Layer::FindLayerByName(E->Get(g_SurfEditor.Layer.LAYER));
}

const char * SurfEditor::SurfLayer::SelLayerName() const {
	Enumerator *E = ENUM.Get("SURFLAYERS");
	return E->Get(g_SurfEditor.Layer.LAYER);
}

DWORD SurfEditor::SurfLayer::SelLayerColor() const {
	Enumerator *E = ENUM.Get("SURFLAYERS");
	int id = Layer::FindLayerByName(E->Get(g_SurfEditor.Layer.LAYER));
	if(id >= 0) {
		return LAYERSLIST.Layers[id]->Color;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// SurfLocalGenerator
//-----------------------------------------------------------------------------
bool SurfEditor::SurfLocalGenerator::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Local Generator (Passes = ";
	Mask += Passes;
	Mask += ")";
	return true;
}

void SurfEditor::SurfLocalGenerator::Defaults() {
	Passes = DefPasses;
}

//-----------------------------------------------------------------------------
// SurfTypify
//-----------------------------------------------------------------------------
bool SurfEditor::SurfTypify::Update() {
	bool fChanged = false;
	TYPE = Type;
	if(Type != pType) {
		pType = Type;
		fChanged = true;
	}
	return fChanged;
}

bool SurfEditor::SurfTypify::OnChangeMember(BaseClass *, void *, void *, const char *) {
	return Update();
}

bool SurfEditor::SurfTypify::CheckIfElementInvisible(int index, void *, DWORD) {
	return index > 0;
}

bool SurfEditor::SurfTypify::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Typify (";
	Enumerator *pEnum = ENUM.Get("SURFTYPES");
	assert(pEnum);
	Mask += pEnum->GetStr(Type);
	Mask += ")";
	return true;
}

//-----------------------------------------------------------------------------
// SurfSmooth
//-----------------------------------------------------------------------------
void SurfEditor::SurfSmooth::Defaults() {
	Amount = pAmount = 0.3f;
	Enumerator *E = ENUM.Get("DISPLACEMENTMAPS");
	assert(E);
	DspMap = pDspMap = 0;
	DSPMAP.Clear();
	if(E->GetAmount()) {
		DSPMAP = E->GetStr(0);
	}
	MapScale = pMapScale = 0.0f;
	MapAmount = pMapAmount = 0.51f;
	Update();
}

void SurfEditor::SurfSmooth::Update() {
	AMOUNT = Amount;
	Enumerator *E = ENUM.Get("DISPLACEMENTMAPS");
	assert(E);
	DSPMAP.Clear();
	if(E->GetAmount()) {
		DSPMAP = E->GetStr(DspMap);
	}
	const float xScale = 10.0f;
	MAPSCALE = 1.0f + xScale * MapScale;
	float t = 100.0f * (MapAmount - 0.5f);
	MAPAMOUNT = int(t + cMath::Sign(t) * 0.5f);
}

bool SurfEditor::SurfSmooth::OnChangeMember(BaseClass *, void *, void *, const char *) {
	Update();
	bool fChanged = false;
	if(Amount != pAmount) {
		pAmount = Amount;
		fChanged = true;
	}
	if(DspMap != pDspMap) {
		pDspMap = DspMap;
		fChanged = true;
	}
	if(MapScale != pMapScale) {
		pMapScale = MapScale;
		fChanged = true;
	}
	if(MapAmount != pMapAmount) {
		pMapAmount = MapAmount;
		fChanged = true;
	}
	return fChanged;
}

bool SurfEditor::SurfSmooth::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Smooth (Amount = %$AMOUNT$%, DspMap = %$DSPMAP$%, MapScale = %$MAPSCALE$%, MapAmount = %$MAPAMOUNT$%)";
	return true;
}

bool SurfEditor::SurfSmooth::CheckIfElementInvisible(int index, void *, DWORD) {
	return index > 4;
}

//-----------------------------------------------------------------------------
// SubdivisionOptions
//-----------------------------------------------------------------------------
void SurfEditor::SubdivisionOptions::Defaults() {
	Dot = pDot = 0.5f;
	Edge = pEdge = 0.5f;
	Scale = pScale = 0.0f;
	Adaptive = pAdaptive = false;
	Update();
}

void SurfEditor::SubdivisionOptions::Update() {
	EDGEMAXLEN = EdgeMaxLenLo + (EdgeMaxLenHi - EdgeMaxLenLo) * Edge;
	DOT = Dot;
	SCALE = 1.0f + ScaleMultiplier * Scale;
	ADAPTIVE = Adaptive;
}

bool SurfEditor::SubdivisionOptions::OnChangeMember(BaseClass *, void *, void *, const char *) {
	Update();
	if(Edge != pEdge) {
		pEdge = Edge;
		return true;
	}
	if(Adaptive != pAdaptive) {
		pAdaptive = Adaptive;
		return true;
	}
	if(Dot != pDot) {
		pDot = Dot;
		return true;
	}
	if(Scale != pScale) {
		pScale = Scale;
		return true;
	}
	return false;
}

bool SurfEditor::SubdivisionOptions::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Subdivision (Adaptive = %$ADAPTIVE$%, ";
	if(Adaptive) Mask += "Dot = %$DOT$%, Scale = %$SCALE$%)";
	else Mask += "EdgeMaxLen = %$EDGEMAXLEN$%)";
	return true;
}

void SurfEditor::SubdivisionOptions::EvaluateFunction() {
	char SubdivisionText[] = "Are you sure you want to subdivide whole surface?";
	char SubdivisionCaption[] = "Confirm Subdivide Whole Surface";
	if(MessageBox(g_hWnd, SubdivisionText, SubdivisionCaption, MB_YESNO) != IDYES) return;
	g_UniversalMap.BackUp();
	g_UniversalMap.Subdivide();
	SURFACE_CLOSEEDITOR;
}

bool SurfEditor::SubdivisionOptions::CheckIfElementInvisible(int index, void *, DWORD) {
	if(Adaptive) {
		if(1 == index) return true;
		return index >= 4;
	} else {
		return index >= 2;
	}
}

//-----------------------------------------------------------------------------
// SurfEditor::Load
//-----------------------------------------------------------------------------
void SurfEditor::Load() {
	g_SurfEditor.Defaults();
	g_SurfEditor.SafeReadFromFile("Surface\\Editor.xml");
	g_SurfEditor.FreeCamera = false;
	g_SurfEditor.HideUI = false;
	g_SurfEditor.Update();
}

//-----------------------------------------------------------------------------
// SurfEditor::Update
//-----------------------------------------------------------------------------
void SurfEditor::Update() {
	CutRect.Update();
	CreateNewMap.Update();
	Subdivision.Update();
//	Shift.Update();
	Shader.Update();
	LODS.Update();

	// HideUI;
	if(pHideUI != HideUI) {
/*		static bools DSSFlags;
		HideUI = !(DSSFlags.Count() > 0);
		extern DialogsSystem ResPan;
		if(HideUI) DSSFlags.Free();
		for(int i = 0; i < ResPan.DSS.GetAmount(); i++) {
			SimpleDialog *pDialog = ResPan.DSS[i];
			if(HideUI) {
				DSSFlags.Add(pDialog->Visible);
				pDialog->Visible = false;
			} else {
				pDialog->Visible = DSSFlags[i];
			}
		}
		if(!HideUI) DSSFlags.Free();
		pHideUI = HideUI;
		// ToolsPanel:
		extern bool ToolsHidden;
		if(HideUI) ToolsHidden = true;
		// MiniMap:
		extern bool MiniMapVisible;
		static bool MiniMapWasVisible = true;
		if(HideUI) {
			MiniMapWasVisible = MiniMapVisible;
			MiniMapVisible = false;
		} else {
			MiniMapVisible = MiniMapWasVisible;
		}
		static bool InfWasVisible = true;
		if(HideUI) {
			InfWasVisible = ext_OnCheatEntering("hideinf");
		} else if(InfWasVisible) ext_OnCheatEntering("showinf");*/
		if(HideUI) {
			ActivateSurfTool(NULL);
		}
		static bool InfWasVisible = true;
		if(HideUI) {
			InfWasVisible = ext_OnCheatEntering("hideinf");
		} else if(InfWasVisible) ext_OnCheatEntering("showinf");
		
		CurrentGameInfo *pGI = &GSets.CGame;
		if(HideUI) {
			pGI->ViewMask = 4;
		} else pGI->ViewMask = 0xff;
		if(HideUI) {
			extern bool ToolsHidden;
			ToolsHidden = true;
			extern bool OptHidden;
			OptHidden = true;
			//extern DialogsSystem TOOL_PARAM;
			//TOOL_PARAM.CloseDialogs();
		}
		pHideUI = HideUI;
	}
}

//------------------------------------------------------------------------------------
// SurfEditor::OnChangeMember
//------------------------------------------------------------------------------------
bool SurfEditor::OnChangeMember(BaseClass *, void *Ptr, void *, const char *Name) {
	// Wireframe, ShowMapOfHeights, ShowNormalInTris, ShowNormalInVerts, ShowSurface
	if(!strcmp(Name, "Wireframe") || !strcmp(Name, "ShowMapOfHeights") ||
		!strcmp(Name, "ShowNormalInTris") || !strcmp(Name, "ShowNormalInVerts") ||
		!strcmp(Name, "ShowSurface")) {
			ResetGroundCache();
	}
	// FreeCamera:
	if(!strcmp(Name, "FreeCamera")) {
		if(prevFreeCamera != FreeCamera) {
			g_Navigator.Toggle(FreeCamera);
			prevFreeCamera = FreeCamera;
		}
	}
	Update();
	return false;
}

//-----------------------------------------------------------------------------
// SurfEditor::CmdTestHtml
//-----------------------------------------------------------------------------
void SurfEditor::CmdTestHtml::EvaluateFunction() {
	HTML::ShowRelativePath("Comics\\comiks_p1.swf");

	DWORD dw = GetTickCount();
	while(HTML::IsActive() && GetTickCount() - dw < 10000) {
		Sleep(20);
	}
	HTML::Close();


//	HTML::Test();
}

//-----------------------------------------------------------------------------
// SurfEditor::CmdSite
//-----------------------------------------------------------------------------
void SurfEditor::CmdSite::EvaluateFunction() {
	HTML::ShowURL("gsc-game.com");
}

//-----------------------------------------------------------------------------
// SurfVertices::CalcBoundBox : void(SurfVertex &, SurfVertex &) const
//-----------------------------------------------------------------------------
void SurfVertices::CalcBoundBox(SurfVertex &bbMin, SurfVertex &bbMax) const {
	bbMin = bbMax = SurfVertex::Origin;
	if(Count()) {
		bbMin = bbMax = GetAt(0);
		for(int i = 1; i < Count(); i++) {
			const SurfVertex &r = GetAt(i);
			r.UpdateBoundBox(bbMin, bbMax);
		}
	}
}

//-----------------------------------------------------------------------------
// SurfVertices::Typify
//-----------------------------------------------------------------------------
void SurfVertices::Typify(int idType) {
	if(Count()) {
		for(int i = 0; i < Count(); i++) {
			SurfVertex &r = GetAt(i);
			r.SurfaceType = idType;
		}
	}
}

//-----------------------------------------------------------------------------
// SurfMesh::Copy : SurfMesh & Copy(const SurfMesh &)
//-----------------------------------------------------------------------------
SurfMesh & SurfMesh::Copy(const SurfMesh &Src) {
	VERTS.Copy(Src.VERTS);
	TRIS.Copy(Src.TRIS);
	return *this;
}

//-----------------------------------------------------------------------------
// SurfMesh.ctor
//-----------------------------------------------------------------------------
SurfMesh::SurfMesh(const SurfMesh &Src) {
	Copy(Src);
}

//-----------------------------------------------------------------------------
// SurfMesh::Free
//-----------------------------------------------------------------------------
void SurfMesh::Free() {
	VERTS.Free();
	TRIS.Free();
}

//-----------------------------------------------------------------------------
// SurfMesh::Relax
//-----------------------------------------------------------------------------
void SurfMesh::Relax(double Amount, int Iterations, float Threshold, bool HoldBoundaryVerts) {
	int T = NTris();
	if(!T) return;
	SurfNodes Nodes;
	int V = NVerts();
	Nodes.SetCount(V);
	int oi[3], k;
	cVec3 u[3];
	double l = cMath::Clamp(Amount, -1., 1.);
	cBitMap BoundMap;
	if(HoldBoundaryVerts) GatherVertsBoundaryStats(BoundMap);
	for(int N = 0; N < Iterations; N++) {
		// Clear Nodes:
		for(k = 0; k < V; k++) {
			Nodes[k].u = cVec3::Zero;
			Nodes[k].n = 0;
		}
		// Enuming tris and considering surroundings for each node:
		for(int nTri = 0; nTri < T; nTri++) {
			GetTri(nTri, oi);
			for(int k = 0; k < 3; k++) {
				const SurfVertex &v = VERTS[oi[k]];
				u[k] = cVec3((float)v.x, (float)v.y, (float)v.z);
			}
			for(k = 0; k < 3; k++) {
				SurfNode &Node = Nodes[oi[k]];
				Node.u += u[(k + 1) % 3] + u[(k + 2) % 3];
				Node.n += 2;
			}
		}
        //Calculating maximal vertex shift distance to apply threshold
		float MaxDist=0;
		for(int nNode = 0; nNode < V; nNode++) {
			SurfNode &Node = Nodes[nNode];
			if(!Node.n) continue;
			Node.u /= Node.n;
			SurfVertex &r = VERTS[nNode];
			//dVector CurPos(r.x, r.y, r.z);
			//dVector Pos = CurPos + l * (Node.u - CurPos);
			float dx = r.x - Node.u.x;
			float dy = r.y - Node.u.y;
			float dz = r.z - Node.u.z;
			float D=sqrt(dx*dx+dy*dy+dz*dz);
			if(D>MaxDist)MaxDist=D;
		}
		float DD=MaxDist*(1-Threshold);
		MaxDist*=Threshold;		
		// Calcing average pos for each node and storing:
		for(int nNode = 0; nNode < V; nNode++) {
			if(HoldBoundaryVerts && BoundMap[nNode]) continue;
			SurfNode &Node = Nodes[nNode];
			if(!Node.n) continue;
			//Node.u /= Node.n;
			SurfVertex &r = VERTS[nNode];
			cVec3 CurPos(r.x, r.y, r.z);			
			float dx = r.x - Node.u.x;
			float dy = r.y - Node.u.y;
			float dz = r.z - Node.u.z;
			float D=sqrt(dx*dx+dy*dy+dz*dz);
			if(D>MaxDist){
				float L=Threshold>0?l*(D-MaxDist)/DD:l;
				cVec3 Pos = CurPos + L * (Node.u - CurPos);
				r.x = (short)Pos.x;
				r.y = (short)Pos.y;
				r.z = (short)Pos.z;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// SurfMesh::InvertTris
//-----------------------------------------------------------------------------
void SurfMesh::InvertTris() {
	int T = NTris();
	for(int nTri = 0; nTri < T; nTri++) {
		Swap(TRIS[3 * nTri + 1], TRIS[3 * nTri + 2]);
	}
}

//-----------------------------------------------------------------------------
// SurfMesh::Sum : void(const SurfMesh &)
//-----------------------------------------------------------------------------
void SurfMesh::Sum(const SurfMesh &Src) {
	int V = VERTS.Count();
	VERTS.Append(Src.VERTS);
	for(int i = 0; i < Src.TRIS.Count(); i++) {
		TRIS.Add(V + Src.TRIS[i]);
	}
}

//-----------------------------------------------------------------------------
// SurfMesh::GenRound : void(const SurfVertex &, int, int)
//-----------------------------------------------------------------------------
void SurfMesh::GenRound(const SurfVertex &Centre, int Radius, int dAngle) {
	Free();
	if(Radius <= 0) return;
	if(dAngle > 120) return;
	SurfVertex c = Centre;
	c.nx = c.ny = 0, c.nz = SurfVertex::NormalLen;
	VERTS.Add(c);
	for(int i = 0; i < 360 / dAngle; i++) {
		dVector u = dVector::Cartesian(Radius, cMath::Rad(i * dAngle));
		VERTS.Add(c);
		VERTS[i + 1].x += u.x;
		VERTS[i + 1].y += u.y;
		if(0 == i) continue;
		AddTri(0, i, i - 1);
	}
	AddTri(0, 1, i - 1);
}

//-----------------------------------------------------------------------------
// SurfMesh::GenPlane : void(int, int, int)
//-----------------------------------------------------------------------------
void SurfMesh::GenPlane(int xLen, int yLen, int Cell) {
	Free();
	assert(Cell > 0);

	int xCells = xLen / Cell, yCells = yLen / Cell;
	for(int iy = 0; iy <= yCells; iy++)
		for(int ix = 0; ix <= xCells; ix++) {
			VERTS.Add(SurfVertex(Cell * ix, Cell * iy));
		}

	for(int yt = 0; yt < yCells; yt++)
		for(int xt = 0; xt < xCells; xt++) {
			int e0 = yt * (xCells + 1) + xt;
			int e1 = (1 + yt) * (xCells + 1) + xt;
			AddTri(e0, e0 + 1, e1 + 1);
			AddTri(e0, e1 + 1, e1);
		}
}

//-----------------------------------------------------------------------------
// SurfMesh::GenHexagonalPlane : void(int, int, int, bool)
//-----------------------------------------------------------------------------
/*void SurfMesh::GenHexagonalPlane(int xLen, int yLen, int Cell, bool Noisy) {
	Free();
	assert(Cell > 0);

	int xCells = xLen / Cell, yCells = yLen / Cell, ix, iy;
	for(iy = 0; iy <= yCells; iy++)
		for(ix = 0; ix <= xCells; ix++) {
			VERTS.Add(SurfVertex(ix * Cell, iy * Cell));
		}
	for(iy = 0; iy < yCells; iy++)
		for(ix = 0; ix < xCells; ix++) {
			VERTS.Add(SurfVertex(ix * Cell + Cell / 2, iy  * Cell + Cell / 2));
		}

	int Shift = (xCells + 1) * (yCells + 1), i0, i1, i2;
	for(int yt = 0; yt < yCells; yt++)
		for(int xt = 0; xt < xCells; xt++) {
			// Left:
			int i0 = yt * xCells + xt + Shift;
			int i1 = yt * (xCells + 1) + xt;
			int i2 = i1 + (xCells + 1);
			AddTri(i0, i2, i1);
			// Right:
			AddTri(i0, i1 + 1, i2 + 1);
			if(0 == yt) { // Top cover:
				AddTri(i0, i1, i1 + 1);
			} else {
				int i4 = i0 - xCells;
				// Left - top:
				AddTri(i0, i1, i4);
				// Right - top:
				AddTri(i0, i4, i1 + 1);
				if(yCells - 1 == yt) { // Bottom cover:
					AddTri(i0, i2 + 1, i2);
				}
			}
	}
	if(Noisy) {
		int A = Cell / 3;
		for(int i = 0; i < VERTS.Count(); i++) {
			SurfVertex &u = VERTS[i];
			u.x += rand() % A - A / 2;
			u.y += rand() % A - A / 2;
		}
	}
}*/

//-----------------------------------------------------------------------------
// SurfMesh::GetRefAll : void(SurfRef &, DWORD)
//-----------------------------------------------------------------------------
void SurfMesh::GetRefAll(SurfRef &Ref, DWORD What) {
	Ref.Free();
	Ref.pOrig = this;
	assert((What & SR_QUADS) == 0);
	if(What & SR_TRIS) {
		int N = NTris();
		for(int nTri = 0; nTri < N; nTri++) {
			Ref.idTris.Add(nTri);
		}
	}
	if(What & SR_VERTS) {
		int N = NVerts();
		for(int nVert = 0; nVert < N; nVert++) {
			Ref.idVerts.Add(nVert);
		}
	}
}

//-----------------------------------------------------------------------------
// SurfMesh::Draw : void(eSurfFx)
//-----------------------------------------------------------------------------
void SurfMesh::Draw(eSurfFx SFX) {
	SurfRef Ref;
	GetRefAll(Ref, SR_TRIS);
	SurfSplit ss;
	ss.Split(Ref, SFX);
	ss.Draw();
}

//-----------------------------------------------------------------------------
// SurfMesh::GetTri : void(int, cVec3 &) const
//-----------------------------------------------------------------------------
void SurfMesh::GetTri(int nTri, cVec3 &Normal) const {
	SurfVertex t[3];
	cVec3 u[3];
	GetTri(nTri, t);
	for(int k = 0; k < 3; k++) {
		const SurfVertex &r = t[k];
		u[k] = cVec3((float)r.x, (float)r.y, (float)r.z);
	}
	Normal = /*cMath::normalize*/(cVec3::Cross(u[1] - u[0], u[2] - u[0]));
}
void SurfMesh::GetTri(int nTri, cVec3 &Normal, cVec3 &Tangent, cVec3 &Binormal) const {
	SurfVertex t[3];
	cVec3 u[3];
	GetTri(nTri, t);
	for(int k = 0; k < 3; k++) {
		const SurfVertex &r = t[k];
		u[k] = cVec3((float)r.x, (float)r.y, (float)r.z);
	}
	Normal = (cVec3::Cross(u[1] - u[0], u[2] - u[0]));

	
	float x1 = u[1].x - u[0].x;
	float x2 = u[2].x - u[0].x;
	float y1 = u[1].y - u[0].y;
	float y2 = u[2].y - u[0].y;
	float z1 = u[1].z - u[0].z;
	float z2 = u[2].z - u[0].z;

	float s1 = t[1].u - t[0].u;
	float s2 = t[2].u - t[0].u;
	float t1 = t[1].v - t[0].v;
	float t2 = t[2].v - t[0].v;

	cMat3 m(x1,x2,Normal.x, y1,y2,Normal.y, z1,z2,Normal.z);
	cMat3 mi;
	cMat3::Invert(m,&mi);
    cVec3 tr(s1,s2,0);
	cVec3 bi(t1,t2,0);
	Tangent=cVec3::Transform(tr,mi);
	Binormal=cVec3::Transform(bi,mi);
	/*

	float r = (s1 * t2 - s2 * t1);
	if(r==0.0f)r=1.0f;	

	Tangent = cVec3((t2 * x1 - t1 * x2) / r, (t2 * y1 - t1 * y2) / r,(t2 * z1 - t1 * z2) / r);
	Binormal = cVec3((s1 * x2 - s2 * x1) / r, (s1 * y2 - s2 * y1) / r,(s1 * z2 - s2 * z1) / r);
	*/
}

//-----------------------------------------------------------------------------
// SurfMesh::GetTri : void(int, dVector &) const
//-----------------------------------------------------------------------------
void SurfMesh::GetTri(int nTri, dVector &Normal) const {
	SurfVertex t[3];
	dVector u[3];
	GetTri(nTri, t);
	for(int k = 0; k < 3; k++) {
		const SurfVertex &r = t[k];
		u[k].Set(r.x, r.y, r.z);
	}
	Normal = (u[1] - u[0]).Cross(u[2] - u[0]).Normal();
}

//-----------------------------------------------------------------------------
// SurfMesh::CalcNormals
//-----------------------------------------------------------------------------
void SurfMesh::CalcNormals() {
	int T = NTris();
	if(!T) return;
    cList<cVec3> Normals;
	cList<cVec4> Tangents;
	cList<cVec4> Binormals;
    cList<DWORD> UV_NearestVert;
	cList<float> UV_Dist;
	cList<DWORD> ZeroWeighted;
    int nv=NVerts();
	Normals.SetCount(nv, cVec3::Zero);
	Tangents.SetCount(nv, cVec4::Zero);
	Binormals.SetCount(nv, cVec4::Zero);
	UV_NearestVert.SetCount(nv);
	UV_Dist.SetCount(nv);
	for(int i=0;i<nv;i++){
		Normals[i]=cVec3::Zero;
		Tangents[i]=cVec4::Zero;
		Binormals[i]=cVec4::Zero;
		UV_NearestVert[i]=0;
		UV_Dist[i]=100000;
	}
	int oi[3];
    cVec3 Normal;
	cVec3 Tangent;
	cVec3 Binormal;
	for(int nTri = 0; nTri < T; nTri++) {
		SurfVertex* sv[3];
		GetTri(nTri, &sv[0]);
		GetTri(nTri, Normal, Tangent,Binormal);
		GetTri(nTri, oi);
		float lt=1.0f/Tangent.Length();		
		float lb=1.0f/Binormal.Length();
		Tangent*=lt*lt;
		Binormal*=lb*lb;
		float anisotropy=lt/lb;	
		if(anisotropy<1)anisotropy=1/anisotropy;		
		bool good_anisotropy=anisotropy>minans && anisotropy<maxans;// && sv[0]->nFacture == sv[1]->nFacture && sv[0]->nFacture==sv[2]->nFacture;
		if(good_anisotropy){
			float A2=cVec3::Dot(Binormal,Tangent);
			A2/=lt*lb;
			if(abs(A2)>0.6)good_anisotropy=false;
		}
		float nn = Normal.LengthSq();
        Normal*=nn;
		float tn = lt;		
		for(int k = 0; k < 3; k++) {			
			int p=oi[k];
			Normals[p] += Normal;
			cVec4& t=Tangents[p];
			cVec4& b=Binormals[p];
			if(good_anisotropy){
				t.Set(t.x+Tangent.x,t.y+Tangent.y,t.z+Tangent.z,t.w+1);
				b.Set(b.x+Binormal.x,b.y+Binormal.y,b.z+Binormal.z,b.w+1);
			}
		}
	}
	for(int iVert = 0; iVert < Normals.Count(); iVert++) {
        cVec3 &n = Normals[iVert];
		n.Normalize();
		cVec3 n0=n;
//        n = cMath::normalize(n);
//		n.Normalize();
		n *= SurfVertex::NormalLen;
		SurfVertex &v = VERTS[iVert];
		v.nx = (short)n.x;
		v.ny = (short)n.y;
		v.nz = (short)n.z;

		//cVec2 uv = SurfMapper::CalcUV(v,cVec3(v.x,v.y,v.z));

		//v.u=uv.x;
		//v.v=uv.y;

        cVec4& t = Tangents[iVert];
		cVec4& b = Binormals[iVert];

		cVec3 t3(t.x,t.y,t.z);
		cVec3 b3(b.x,b.y,b.z);
		if(t.w==0){
			cVec3 t=cVec3::AxisX;
			t=t-n0*cVec3::Dot(n0,t);
			t.Normalize();
			cVec3 b=cVec3::Cross(t,n0);
			float mc=-SurfVertex::NormalLen;			
			v.tx = short(t.x*mc);
			v.ty = short(t.y*mc);
			v.tz = short(t.z*mc);
			v.tscale=256;
			v.bx = short(b.x*mc);
			v.by = short(b.y*mc);
			v.bz = short(b.z*mc);
			v.bscale=256;
		}else{
			t3/=t.w;
			b3/=b.w;
			t.w=t3.Length();
			b.w=b3.Length();		

			float tm=SurfVertex::NormalLen/t.w;
			float bm=SurfVertex::NormalLen/b.w;

			v.tx = short(t3.x*tm);
			v.ty = short(t3.y*tm);
			v.tz = short(t3.z*tm);
			v.tscale = t.w;

			v.bx = short(b3.x*bm);
			v.by = short(b3.y*bm);
			v.bz = short(b3.z*bm);
			v.bscale = b.w;
		}
	}
}

//----------------------------------------------------------------------------------
// SurfMesh::CalcBoundBox
//----------------------------------------------------------------------------------
void SurfMesh::CalcBoundBox(SurfVertex &bbMin, SurfVertex &bbMax) const {
	VERTS.CalcBoundBox(bbMin, bbMax);
}

//-----------------------------------------------------------------------------
// SurfMesh::WeldVertices
//-----------------------------------------------------------------------------
void SurfMesh::WeldVertices() {
	DWORDS Remap;
	VERTS.RemoveDups(&Remap);
	DWORDS Tris;
	int oi[3];
	int T = NTris();
	for(int nTri = 0; nTri < T; nTri++) {
		GetTri(nTri, oi);
		for(int k = 0; k < 3; k++) {
			oi[k] = Remap[oi[k]];
		}
		if(oi[0] != oi[1] && oi[1] != oi[2] && oi[0] != oi[2]) {
			Tris.Append((DWORD*)oi, 3);
		}
	}
	TRIS.Copy(Tris);
}

//-----------------------------------------------------------------------------
// SurfMesh::Subdivide : bool(const SubdivideArgs &)
//-----------------------------------------------------------------------------
bool SurfMesh::Subdivide(const SubdivideArgs &Args) {
	SurfRef Ref;
	GetRefAll(Ref, SR_TRIS);
	return Ref.Subdivide(Args);
}

//-----------------------------------------------------------------------------
// SurfMesh::GatherVertsBoundaryStats : bool(cBitMap &)
//-----------------------------------------------------------------------------
bool SurfMesh::GatherVertsBoundaryStats(cBitMap &BoundMap) {
	if(!VERTS.Count() || !TRIS.Count()) return true;
	SurfRef Ref;
	GetRefAll(Ref, SR_TRIS);
	SurfEdges Edges;
	if(!Ref.GatherEdges(Edges)) return false;
	BoundMap = cBitMap(NVerts());
	for(int nEdge = 0; nEdge < Edges.Count(); nEdge++) {
		const SurfEdge &Edge = Edges[nEdge];
		if(Edge.f1 == -1) {
			assert(Edge.f0 > -1);
			BoundMap.Set(Edge.e0);
			BoundMap.Set(Edge.e1);
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
// SurfMesh::SetFacture : void(int)
//-----------------------------------------------------------------------------
void SurfMesh::SetFacture(int nFacture) {
	assert(nFacture >= 0 && nFacture < GetNFactures());
	for(int i = 0; i < VERTS.Count(); i++) {
		VERTS[i].nFacture = nFacture;
	}
}

//-----------------------------------------------------------------------------
// SurfMesh::GatherEdges : bool(SurfEdges &, DWORDS *)
//-----------------------------------------------------------------------------
bool SurfMesh::GatherEdges(SurfEdges &Edges, DWORDS *pEdgeTris) {
/*	Edges.Free();
	if(pEdgeTris) pEdgeTris->Free();
	if(!TRIS.Count()) return true;
	bool fSuccess = true;
	int V = NVerts();
	cMath::cBitMap Map(Square(V));
	int T = NTris(), i[3], k, e0, e1, index;
	for(int nTri = 0; nTri < T; nTri++) {
		GetTri(nTri, i);
		assert(i[0] != i[1] && i[0] != i[2] && i[1] != i[2] && "Tri is degraded to line or point!");
		for(k = 0; k < 3; k++) {
			e0 = i[k], e1 = i[(k + 1) % 3];
			index = e0 + V * e1];
			assert(Map[index] == 0 && "Detected nonmanifold topology (the T shape)!");
			if(Map[e0 + V * e1] != 0) {
				fSuccess = false;
				goto SurfMeshGatherEdges;
			}
			Map.Set(index);
		}
	}
SurfMeshGatherEdges:
	if(!fSuccess) {
		Edges.Free();
		if(pEdgeTris) pEdgeTris->Free();
	}
	return fSuccess;*/
	return false;
}

//-----------------------------------------------------------------------------
// SurfFx
//-----------------------------------------------------------------------------

int SurfFx(eSurfFx SFX) {
	static int GridStage1 = IRS->GetShaderID("surf_grid1");
	static int GridStage2 = IRS->GetShaderID("surf_grid2");
	static int Samples = IRS->GetShaderID("surf_samples");
	static int Aux = IRS->GetShaderID("surf_aux");
	static int Wire = IRS->GetShaderID("surf_wire");
    static int SolidLo = IRS->GetShaderID("low\\terra_shadowed_hq");
    static int SolidHi = IRS->GetShaderID("terra_shadowed_hq");
    int Solid = GSets.SVOpt.DrawBumpOnLandcape ? SolidHi : SolidLo;
	static int SolidIso = IRS->GetShaderID("terra_shadowed_iso");
	static int Bump = IRS->GetShaderID("terra_shadowed_hq_bump");
	//static int Bump = IRS->GetShaderID("dot3specular");
	static int SwatchStage1 = IRS->GetShaderID("surf_swatch1");
	static int SwatchStage2 = IRS->GetShaderID("surf_swatch2");
	static int Booleans = IRS->GetShaderID("surf_booleans");

	switch(SFX) {
		case SFX_DEFAULT:
			return g_bPerspCamera?Solid:SolidIso;
		case SFX_WIRE:
			return Wire;
		case SFX_BUMP:
			return Bump;
		case SFX_GRIDSTAGE1:
			return GridStage1;
		case SFX_GRIDSTAGE2:
			return GridStage2;
		case SFX_AUX:
			return Aux;
		case SFX_SAMPLES:
			return Samples;
		case SFX_SWATCHSTAGE1:
			return SwatchStage1;
		case SFX_SWATCHSTAGE2:
			return SwatchStage2;
		case SFX_BOOLEANS:
			return Booleans;
		default:
			assert(0);
			return 0;
	}
}

// SurfMapper.ctor
SurfMapper::SurfMapper(int nFacture) {
	float du, dv;
	GetUVInfo(nFacture, du, dv, uScale, vScale);

	// Applying global factures scale based on camera settings:
	uScale *= MOptions.CamArgs.GetFacturesScale();
	vScale *= MOptions.CamArgs.GetFacturesScale();

	fWidth = fHeight = 256.0f;
//	fWidth = IRS->GetTextureWidth(GetFacture(nFacture));
//	fHeight = IRS->GetTextureHeight(GetFacture(nFacture));
}

DWORD SurfMapper::CalcMappingCode(const dVector &Normal) {
	dVector d = Normal.Abs();
	if(d.z >= k * d.x && d.z >= k * d.y) { // Top or Bottom
		if(Normal.z >= 0) return MC_TOP;
		return MC_BOTTOM;
	}
	if(d.x >= d.y) { // Left or Right
		if(Normal.x >= 0) return MC_RIGHT;
		return MC_LEFT;
	} else if(d.x < d.y) { // Back or Front
		if(Normal.y >= 0) return MC_BACK;
		return MC_FRONT;
	}
}

// SurfMapper::CalcMappingCode
DWORD SurfMapper::CalcMappingCode(const SurfVertex &vtx) {
	int lx = abs(vtx.nx), ly = abs(vtx.ny), lz = abs(vtx.nz);
    DWORD MC0=(DWORD(vtx.Rotation & 127)<<3) | (DWORD(vtx.Scale & 127)<<10) | (DWORD(vtx.dU&127)<<17) | (DWORD(vtx.dV&127)<<25);
	if(lz >= k * lx && lz >= k * ly) { // Top or Bottom
		if(vtx.nz >= 0) return MC0+MC_TOP;
		return MC0+MC_BOTTOM;
	}
	if(lx >= ly) { // Left or Right
		if(vtx.nx >= 0) return MC0+MC_RIGHT;
		return MC0+MC_LEFT;
	} else if(lx < ly) { // Back or Front
		if(vtx.ny >= 0) return MC0+MC_BACK;
		return MC0+MC_FRONT;
	}
} // SurfMapper::CalcMappingCode

cVec2 SurfMapper::CalcUV(SurfVertex& v,cVec3 pos){
    DWORD Code=CalcMappingCode(v);	
	SurfMapper Mapper(v.nFacture);
	uv_pair uv=Mapper.CalcMappingUV(pos.x,pos.y,pos.z,Code,Vector3D::oZ,*((SurfSplitVertex*)&v));
	return cVec2(uv.u,uv.v);
}
void  SurfMapper::CalcUV(SurfVertex& v){    	
	cVec3 uvVec(0,0,1);
	cVec2 duv(0,0);
	int dx=abs(v.nx);
	int dy=abs(v.ny);
	int dz=abs(v.nz);
	if(dz>dx && dz>dy){		
		uvVec.x=v.x;
		uvVec.y=v.y;
		duv=cVec2(200,-123);
	}else{
		if(dx>dy && dx>dz){
			uvVec.x=v.y;
			uvVec.y=-v.z;
			duv=cVec2(30,223);
		}else{
			uvVec.x=v.x;
			uvVec.y=-v.z;
			duv=cVec2(-127,345);
		}
	}	
	SurfMapper m(v.nFacture);
	uvVec.x/=m.uScale;
	uvVec.y/=m.vScale;
	cVec3 uv=uvVec/256.f;
	v.u=uv.x;
	v.v=uv.y;    
}
cVec2 SurfMapper::ExtrapolateUV(const SurfVertex& v,cVec3 pos){
	cVec3 t(v.tx,v.ty,v.tz);
	t/=SurfVertex::NormalLen;
	cVec3 b(v.bx,v.by,v.bz);
	b/=SurfVertex::NormalLen;
	cVec3 dr=cVec3(v.x-pos.x,v.y-pos.y,v.z-pos.z);
	float fdu=cVec3::Dot(dr,t)/float(v.tscale);
	float fdv=cVec3::Dot(dr,b)/float(v.bscale);
	cVec2 uv;
	uv.x = v.u-fdu;
	uv.y = v.v-fdv;
	return uv;
}
DWORD CalcSpecular(float _du,float _dv,float AA){    
    float _cos=cos(AA);
    float _sin=sin(AA);
    float du=_du*_cos-_dv*_sin;
    float dv=_dv*_cos+_du*_sin;
	float A=sqrt(du*du+dv*dv);
	if(A!=0.0){
		du/=A;
		dv/=A;
	}
	if(A>1)A=1;
	DWORD R=128+du*126;
	DWORD G=128+dv*126;
	DWORD B=A*254;
	return (R<<16)+(G<<8)+B;
}
uv_pair rotuv(uv_pair uv,float A,float du,float dv,float s){    
    uv.u=(uv.u*s+du)*s;
    uv.v=(uv.v*s+dv)*s;
    float _cos=cos(A);
    float _sin=sin(A);
    uv_pair UV;
    UV.u=uv.u*_cos-uv.v*_sin;
    UV.v=uv.v*_cos+uv.u*_sin;
    UV.specular=uv.specular;
    return UV;
}
uv_pair SurfMapper::CalcMappingUV(const float x, const float y, const float z, DWORD MappingCode,Vector3D N,const SurfSplitVertex& sv) const {
   
    DWORD Layer = 0;
    float A=((MappingCode>>3)&127)*c_PI/64;
    float s=((MappingCode>>10)&127)/64.0;
    float du=((MappingCode>>17)&127)/128.0f;
    float dv=((MappingCode>>25)&127)/128.0f;
    MappingCode &= 7;    

	float i = 1.f / (uScale * fWidth);
	float X = x * i;
	float Y = y * i;
	float Z = z / (vScale * fHeight);

	static Vector3D Light(-2/2.449,-1/2.449,-1/2.449);
    Vector3D L=Light;
	N*=L.dot(N);
	L-=N;
	
	uv_pair uv;
	switch(MappingCode) {
		case MC_TOP:
		case MC_BOTTOM:
			uv.u = X;
			uv.v = Y;// / 2.f;
			uv.specular = CalcSpecular(L.x,L.y,A);
            *((BYTE*)(&uv.specular)+3)=Layer;
			return rotuv(uv,A,du,dv,s);
		case MC_BACK:
			uv.u = X;
			uv.specular = CalcSpecular(L.x,-L.z,A);
			//break;
		case MC_FRONT:
			uv.u = - X;
			uv.specular = CalcSpecular(-L.x,-L.z,A);
			break;
		case MC_LEFT:
			uv.u = Y;
			uv.specular = CalcSpecular(L.y,-L.z,A);
			break;
		case MC_RIGHT:
			uv.u = - Y;
			uv.specular = CalcSpecular(-L.y,-L.z,A);
			break;
	}
	uv.v = - Z;   
    *((BYTE*)(&uv.specular)+3)=Layer;
	return rotuv(uv,A,du,dv,s);
}

// SurfMapper::CalcMappingUV
uv_pair SurfMapper::CalcMappingUV(const SurfVertex &vtx, DWORD MappingCode) const {
	float i = 1.f / (uScale * fWidth);
	float X = float(vtx.x) * i;
	float Y = float(vtx.y) * i;
	float Z = float(vtx.z) / (vScale * fHeight);

	uv_pair uv;
	switch(MappingCode) {
		case MC_TOP:
		case MC_BOTTOM:
			uv.u = X;
			uv.v = Y / 2.f;
			return uv;
		case MC_BACK:
			uv.u = X;
			break;
		case MC_FRONT:
			uv.u = - X;
			break;
		case MC_LEFT:
			uv.u = Y;
			break;
		case MC_RIGHT:
			uv.u = - Y;
			break;
	}
	uv.v = - Z;
	return uv;
} // SurfMapper::CalcMappingUV


// SurfVertex::GetMapper
SurfMapper SurfVertex::GetMapper(int nFacture) {
	return SurfMapper(nFacture);
} // SurfVertex::GetMapper

//-----------------------------------------------------------------------------
// SurfVertex::MappingType : eMappingType() const
//-----------------------------------------------------------------------------
eMappingType SurfVertex::MappingType() const {
	DWORD MappingCode = SurfMapper::CalcMappingCode(*this) & 7;
	switch(MappingCode) {
		case SurfMapper::MC_FRONT:
		case SurfMapper::MC_BACK:
		case SurfMapper::MC_LEFT:
		case SurfMapper::MC_RIGHT:
			return MT_VERTICAL;
		case SurfMapper::MC_TOP:
		case SurfMapper::MC_BOTTOM:
			return MT_HORIZONTAL;
		default:
			assert(0);
	}
}

// SurfVertex::CalcDiffuseColor
DWORD SurfVertex::CalcDiffuseColor() const {	
	DWORD C = Color;
	int l = 2 * int(Luminance);
	DWORD R = 0;
	for(int i = 0; i < 4; i++) {
		int LowByte = C & 0xff;
		C >>= 8;
		int CurByte = (l * LowByte) >> 8;
		if(CurByte > 0xff) CurByte = 0xff;
		R |= (CurByte << (i << 3));
	}
	return R | 0xff000000;
}

// SurfVertex::CalcSpecularColor

extern Vector3D g_LightDir;
DWORD SurfVertex::CalcSpecularColor() const
{	
	return 0xFFFFFFFF;//temp to speed up
	Vector3D N(nx, ny, nz);
	N.normalize();
	Vector3D L = g_LightDir;
	L.reverse();
	return VectorToColor(ToTangentSpace(L, N, Vector3D(1, 0, 0)), 0);	
}

const SurfVertex SurfVertex::Origin(0, 0, 0);
const SurfVertex SurfVertex::MinValue(SHRT_MIN, SHRT_MIN, SHRT_MIN);
const SurfVertex SurfVertex::MaxValue(SHRT_MAX, SHRT_MAX, SHRT_MAX);

//---------------------------------------------------------------------------------
// SurfVertex::UpdateBoundBox : void(SurfVertex &, SurfVertex &) const
//---------------------------------------------------------------------------------
void SurfVertex::UpdateBoundBox(SurfVertex &bbMin, SurfVertex &bbMax) const {
	bbMin.x = cMath::Min(bbMin.x, x);
	bbMin.y = cMath::Min(bbMin.y, y);
	bbMin.z = cMath::Min(bbMin.z, z);
	bbMax.x = cMath::Max(bbMax.x, x);
	bbMax.y = cMath::Max(bbMax.y, y);
	bbMax.z = cMath::Max(bbMax.z, z);
}

//-----------------------------------------------------------------------------
// SurfRef::RemoveUnreferredTris
//-----------------------------------------------------------------------------
void SurfRef::RemoveUnreferredTris() {
	idTris.Sort();
	DWORDS TRIS;
	int iTri = 0, i3[3], k;
	int NTris = pOrig->NTris();
	for(int nTri = 0; nTri < NTris; nTri++) {
		if(iTri >= idTris.Count()) break;
		if(nTri == idTris[iTri]) {
			pOrig->GetTri(nTri, i3);
			for(k = 0; k < 3; k++) {
				TRIS.Add(i3[k]);
			}
			iTri++;
		}
	}
	pOrig->TRIS.Copy(TRIS);
}

//-----------------------------------------------------------------------------
// SurfRef::SmoothColors : void()
//-----------------------------------------------------------------------------
void SurfRef::SmoothColors() {
	assert(pOrig);
	if(!idTris.Count()) return;
	const int kS = 10, ks = 1;
	const int S = kS + 2 * ks;
	int i[3], k, r[3], g[3], b[3], rTotal, gTotal, bTotal, R[3], G[3], B[3];
	SurfVertex *p[3];
	for(int iTri = 0; iTri < idTris.Count(); iTri++) {
		int nTri = idTris[iTri];
		pOrig->GetTri(nTri, i);
		rTotal = gTotal = bTotal = 0;
		for(k = 0; k < 3; k++) {
			p[k] = &pOrig->VERTS[i[k]];
			argb2rgb(p[k]->Color, r[k], g[k], b[k]);
			rTotal += r[k];
			gTotal += g[k];
			bTotal += b[k];
		}
		for(k = 0; k < 3; k++) {
			R[k] = (kS * r[k] + ks * r[(k + 1) % 3] + ks * r[(k + 2) % 3]) / S;
			G[k] = (kS * g[k] + ks * g[(k + 1) % 3] + ks * g[(k + 2) % 3]) / S;
			B[k] = (kS * b[k] + ks * b[(k + 1) % 3] + ks * b[(k + 2) % 3]) / S;
		}
		// Balance:
		int RTotal = 0, GTotal = 0, BTotal = 0;
		for(k = 0; k < 3; k++) {
			RTotal += R[k];
			GTotal += G[k];
			BTotal += B[k];
		}
		int *pR[3] = {&R[0], &R[1], &R[2]}, *pG[3] = {&G[0], &G[1], &G[2]}, *pB[3] = {&B[0], &B[1], &B[2]};
		int nCur = 0;
		for(int nTotalTimes = 0; nTotalTimes < 1000; nTotalTimes++) {
			if(RTotal < rTotal && *pR[nCur] < 255) {
				(*pR[nCur])++;
				RTotal++;
			}
			if(GTotal < gTotal && *pG[nCur] < 255) {
				(*pG[nCur])++;
				GTotal++;
			}
			if(BTotal < bTotal && *pB[nCur] < 255) {
				(*pB[nCur])++;
				BTotal++;
			}
			nCur++;
			if(nCur == 3) nCur = 0;
		}
		for(k = 0; k < 3; k++) {
			rgb2argb(R[k], G[k], B[k], p[k]->Color);
		}
	}
}

const int g_LenOfTHMap = 512 * 512;
cBitMap g_BitMapOfTHMap(g_LenOfTHMap);

//-----------------------------------------------------------------------------
// SurfRef::CalcTHMapIndex
//-----------------------------------------------------------------------------
int SurfRef::CalcTHMapIndex(int x, int y) {
	int x64 = x & 0x3F /* %64 */, x32 = x & 0x1F /* %32 */;
	int y32 = y & 0x1F /* %32 */, y16_32 = (y + 16) & 0x1F /* %32 */;
	int n = -1;
	if(!x64 && !y32) {
		n = x / 32 + VertInLine * y / 32;
	} else if(x64 && !x32 && !y16_32) {
		n = x / 32 + VertInLine * (y + 16) / 32;
	}
	if(!cMath::IsInRange(n, 0, g_LenOfTHMap - 1)) n = -1;
	return n;
}
//--------------------------------------------------------------------------------------------------------
// SurfRef::Tri2THMap : void(const SurfVertex &, const SurfVertex &, const SurfVertex &, int)
//--------------------------------------------------------------------------------------------------------
void SurfRef::Tri2THMap(const SurfVertex &t0, const SurfVertex &t1, const SurfVertex &t2, int MaxHeight) {
	const int x1 = t0.x, x2 = t1.x, x3 = t2.x;
	const int y1 = t0.y, y2 = t1.y, y3 = t2.y;

	// Block size:
	const int qx = 32, qy = 16;

	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;

	// Block deltas:
	const int qDx12 = qy * Dx12, qDx23 = qy * Dx23, qDx31 = qy * Dx31;
	const int qDy12 = qx * Dy12, qDy23 = qx * Dy23, qDy31 = qx * Dy31;

	// Bounding rect:
	int xMin = cMath::Min(x1, x2, x3);
	int xMax = cMath::Max(x1, x2, x3);
	int yMin = cMath::Min(y1, y2, y3);
	int yMax = cMath::Max(y1, y2, y3);

	// Start in corner of block:
	xMin &= ~(qx - 1);
	yMin &= ~(qy - 1);

	// Constant parts of half - space functions:
	int C1 = Dy12 * x1 - Dx12 * y1;
	int C2 = Dy23 * x2 - Dx23 * y2;
	int C3 = Dy31 * x3 - Dx31 * y3;

	// Fill convention:
	if(Dy12 < 0 || (Dy12 == 0 && Dx12 > 0)) C1++;
	if(Dy23 < 0 || (Dy23 == 0 && Dx23 > 0)) C2++;
	if(Dy31 < 0 || (Dy31 == 0 && Dx31 > 0)) C3++;

	// Starting values of the half - space functions at the top of the bounding rectangle:
	int Cy1 = C1 + Dx12 * yMin - Dy12 * xMin;
	int Cy2 = C2 + Dx23 * yMin - Dy23 * xMin;
	int Cy3 = C3 + Dx31 * yMin - Dy31 * xMin;

	// Scanning:
	for(int y = yMin; y < yMax; y += qy) {
		// Starting values for horizontal scan:
		int Cx1 = Cy1, Cx2 = Cy2, Cx3 = Cy3;
		for(int x = xMin; x < xMax; x += qx) {
			if(Cx1 > 0 && Cx2 > 0 && Cx3 > 0) {
				int n = CalcTHMapIndex(x, y);
				if(n > -1) {
					dVector v0(t0.x, t0.y, t0.z), v1(t1.x, t1.y, t1.z), v2(t2.x, t2.y, t2.z);
					dVector u = v1 - v0;
					dVector v = v2 - v0;
					dVector N = u.Cross(v);
					if(!cMath::IsZero(N.z)) {
						N.Normalize();
						double hx = x - x1, hy = y - y1;
						int z = - (N.x * hx + N.y * hy) / N.z + int(t0.z);
						if(z > MaxHeight) {
							if(!g_BitMapOfTHMap[n]) {
								DeepMap[n] = MaxHeight; // ?
								THMap[n] = MaxHeight;
								g_BitMapOfTHMap.Set(n);
							}
							continue;
						}
						if(!g_BitMapOfTHMap[n]) {
							THMap[n] = z;
							DeepMap[n] = z;
							g_BitMapOfTHMap.Set(n);
						} else {
							if(THMap[n] == MaxHeight) {
								THMap[n] = z;
								DeepMap[n] = z; // ?
							} else {
								if(THMap[n] < z) THMap[n] = z;
								if(DeepMap[n] > z) DeepMap[n] = z;
							}
						}
					}
				}
			}
			Cx1 -= qDy12;
			Cx2 -= qDy23;
			Cx3 -= qDy31;
		}
		Cy1 += qDx12;
		Cy2 += qDx23;
		Cy3 += qDx31;
	}
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Debug_CheckRef(const SurfRef &Ref) {
	for(int iTri = 0; iTri < Ref.idTris.Count(); iTri++) {
		int nTri = Ref.idTris[iTri];
		assert(nTri >= 0 && nTri < Ref.pOrig->TRIS.Count());
		int i3[3];
		Ref.pOrig->GetTri(nTri, i3);
		assert(i3[0] >= 0 && i3[0] < Ref.pOrig->VERTS.Count());
		assert(i3[1] >= 0 && i3[1] < Ref.pOrig->VERTS.Count());
		assert(i3[2] >= 0 && i3[2] < Ref.pOrig->VERTS.Count());
	}
} // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//-----------------------------------------------------------------------------
// SurfRef::FillMapOfHeights : void(int) const
//-----------------------------------------------------------------------------
void SurfRef::FillMapOfHeights(int MaxHeight) const {
	assert(pOrig);
	int NTris = idTris.Count();
	if(!NTris) return;

	g_BitMapOfTHMap.ClearAll();
	Debug_CheckRef(*this); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Rasterizing tris to THMap nodes:
	SurfVertex v[3];
	for(int iTri = 0; iTri < NTris; iTri++) {
		int nTri = idTris[iTri];
		pOrig->GetTri(nTri, v);
		Tri2THMap(v[0], v[2], v[1], MaxHeight);
	}
}

//-----------------------------------------------------------------------------
// SurfRef::Copy : SurfRef & (const SurfRef &)
//-----------------------------------------------------------------------------
SurfRef & SurfRef::Copy(const SurfRef &Src) {
	idQuads.Copy(Src.idQuads);
	idTris.Copy(Src.idTris);
	idVerts.Copy(Src.idVerts);
	pOrig = Src.pOrig;
	return *this;
}

//-----------------------------------------------------------------------------
// SurfRef::Free
//-----------------------------------------------------------------------------
void SurfRef::Free() {
	idQuads.Free();
	idTris.Free();
	idVerts.Free();
	pOrig = NULL;
}

//----------------------------------------------------------------------------------------
// SurfRef::CalcBoundBox : void(eSurfRef, SurfVertex &, SurfVertex &) const
//----------------------------------------------------------------------------------------
void SurfRef::CalcBoundBox(eSurfRef By, SurfVertex &bbMin, SurfVertex &bbMax) const {
	assert(pOrig);
	bbMin = bbMax = SurfVertex::Origin;
	SurfVertex q0, q1;
	switch(By) {
		case SR_QUADS:
			if(!idQuads.Count()) break;
			bbMin = SurfVertex::MaxValue;
			bbMax = SurfVertex::MinValue;
			for(int iQ = 0; iQ < idQuads.Count(); iQ++) {
				((SurfMap*)pOrig)->QUADS[idQuads[iQ]].CalcBoundBox(SR_TRIS, q0, q1);
				q0.UpdateBoundBox(bbMin, bbMax);
				q1.UpdateBoundBox(bbMin, bbMax);
			}
			break;
		case SR_TRIS:
			if(!idTris.Count()) break;
			bbMin = SurfVertex::MaxValue;
			bbMax = SurfVertex::MinValue;
			int oi[3];
			for(int iTri = 0; iTri < idTris.Count(); iTri++) {
				int nTri = idTris[iTri];
				pOrig->GetTri(nTri, oi);
				for(int k = 0; k < 3; k++) {
					const SurfVertex &r = pOrig->VERTS[oi[k]];
					r.UpdateBoundBox(bbMin, bbMax);
				}
			}
			break;
		case SR_VERTS:
			if(!idVerts.Count()) break;
			bbMin = bbMax = pOrig->VERTS[idVerts[0]];
			for(int i = 1; i < idVerts.Count(); i++) {
				const SurfVertex &r = pOrig->VERTS[idVerts[i]];
				r.UpdateBoundBox(bbMin, bbMax);
			}
			break;
		default:
			assert(0);
	}
}

//-----------------------------------------------------------------------------
// SurfRef::GenLOD
//-----------------------------------------------------------------------------
void SurfRef::GenLOD(DWORDS &Tris, int nLod) {
	assert(pOrig);
	Tris.Free();
	assert(nLod >= 0);
	if(!idTris.Count()) return;

	int oi[3], k, i;
	if(nLod == 0) {
		for(i = 0; i < idTris.Count(); i++) {
			pOrig->GetTri(idTris[i], oi);
			for(k = 0; k < 3; k++) {
				Tris.Add(oi[k]);
			}
		}
	} else {
		Complete(SR_VERTS, SR_TRIS);
		assert(idVerts.Count());
		idVerts.CreateHash();

		VIPMLodder Lodder;
		// Adding verts:
		for(i = 0; i < idVerts.Count(); i++) {
			const SurfVertex &r = pOrig->VERTS[idVerts[i]];
			Vector3D n(r.nx, r.ny, r.nz);
			n.normalize();
			Lodder.AddVertex(Vector3D(r.x, r.y, r.z), n);
		}
		// Adding tris:
		int ri[3];
		for(i = 0; i < idTris.Count(); i++) {
			pOrig->GetTri(idTris[i], oi);
			for(k = 0; k < 3; k++) {
				ri[k] = idVerts.IndexOf(oi[k]);
				assert(ri[k] > -1);
			}
			Lodder.AddFace(ri[0], ri[1], ri[2]);
		}
		idVerts.FreeHash();

		Lodder.Process();
		int NCollapses = cMath::Min(int(idVerts.Count() - idVerts.Count() / pow(2, nLod)), Lodder.GetMaxCollapses());
		Lodder.SetNCollapses(NCollapses);

		int NTris = Lodder.GetNFaces();
		for(i = 0; i < NTris; i++) {
			Lodder.GetFace(i, oi[0], oi[1], oi[2]);
			for(k = 0; k < 3; k++) {
				Tris.Add(idVerts[Lodder.GetOldVIndex(oi[k])]);
			}
		}
		Lodder.Reset();
	}
}

//------------------------------------------------------------------------------
// SurfMesh::CalcBoundSphere : void(SurfVertex &, int &) const
//------------------------------------------------------------------------------
void SurfMesh::CalcBoundSphere(SurfVertex &Centre, int &Radius) const {
	SurfVertex bbMin, bbMax;
	CalcBoundBox(bbMin, bbMax);
	Centre.Set((bbMin.x + bbMax.x) / 2, (bbMin.y + bbMax.y) / 2, (bbMin.z + bbMax.z) / 2);
	int dx = bbMax.x - Centre.x;
	int dy = bbMax.y - Centre.y;
	int dz = bbMax.z - Centre.z;
	Radius = sqrt(cMath::Square(dx) + cMath::Square(dy) + cMath::Square(dz));
}

//----------------------------------------------------------------------------------
// SurfRef::CalcBoundSphere : void(SurfVertex &, int &) const
//----------------------------------------------------------------------------------
void SurfRef::CalcBoundSphere(eSurfRef By, SurfVertex &Centre, int &Radius) const {
	assert(pOrig);
	SurfVertex bbMin, bbMax;
	CalcBoundBox(By, bbMin, bbMax);
	Centre.Set((bbMin.x + bbMax.x) / 2, (bbMin.y + bbMax.y) / 2, (bbMin.z + bbMax.z) / 2);
	int dx = bbMax.x - Centre.x;
	int dy = bbMax.y - Centre.y;
	int dz = bbMax.z - Centre.z;
	Radius = sqrt(cMath::Square(dx) + cMath::Square(dy) + cMath::Square(dz));
}

//-----------------------------------------------------------------------------
// SurfRef::Complete : void(DWORD, eSurfRef)
//-----------------------------------------------------------------------------
void SurfRef::Complete(DWORD What, eSurfRef By) {
	assert(pOrig);
	assert((What & By) == 0);
	switch(By) {
		case SR_QUADS:
			if(What & SR_TRIS || What & SR_VERTS) {
				idTris.Free();
				for(int iQ = 0; iQ < idQuads.Count(); iQ++) {
					int nQ = idQuads[iQ];
					DWORDS &q = ((SurfMap*)pOrig)->QUADS[nQ].idTris;
					if(q.Count()) { idTris.Append(q); }
				}
			}
			if(What & SR_VERTS) {
				Complete(SR_VERTS, SR_TRIS);
			}
			break;
		case SR_TRIS:
			if(What & SR_VERTS) {
				idVerts.Free();
				int oi[3];
				for(int iTri = 0; iTri < idTris.Count(); iTri++) {
					int nTri = idTris[iTri];
					pOrig->GetTri(nTri, oi);
					for(int k = 0; k < 3; k++) {
						idVerts.Add(oi[k]);
					}
				}
				idVerts.RemoveDups();
			}
			if(What & SR_QUADS) {
				assert(0);
			}
			break;
		case SR_VERTS:
			assert(0);
			break;
		default:
			assert(0);
	}
}

//-----------------------------------------------------------------------------
// SurfRef.ctor
//-----------------------------------------------------------------------------
SurfRef::SurfRef(const SurfRef &Src) {
	Copy(Src);
}

//-----------------------------------------------------------------------------
// SurfRef.ctor
//-----------------------------------------------------------------------------
SurfRef::SurfRef(SurfMesh *pOrig) {
	this->pOrig = pOrig;
}

//-----------------------------------------------------------------------------
// SurfRef::ToSurfMesh : void(SurfMesh &, eSurfRef = SR_TRIS)
//-----------------------------------------------------------------------------
void SurfRef::ToSurfMesh(SurfMesh &Mesh, eSurfRef By) {
	assert(pOrig);
	Mesh.Free();
	switch(By) {
		case SR_QUADS:
			if(!idQuads.Count()) return;
			Complete(SR_TRIS | SR_VERTS, SR_QUADS);
			if(!idTris.Count()) return;
			break;
		case SR_TRIS:
			if(!idTris.Count()) return;
			Complete(SR_VERTS, SR_TRIS);
			break;
		case SR_VERTS:
			if(!idVerts.Count()) return;
			Complete(SR_TRIS, SR_VERTS);
			break;
		default:
			assert(0);
	}
	// VERTS:
	for(int iVert = 0; iVert < idVerts.Count(); iVert++) {
		int nVert = idVerts[iVert];
		Mesh.VERTS.Add(pOrig->VERTS[nVert]);
	}
	// TRIS:
	int oi[3];
	idVerts.CreateHash();
	for(int iTri = 0; iTri < idTris.Count(); iTri++) {
		int nTri = idTris[iTri];
		pOrig->GetTri(nTri, oi);
		if(oi[0] == oi[1] || oi[0] == oi[2] || oi[1] == oi[2]) continue;
		for(int k = 0; k < 3; k++) {
			int index = idVerts.IndexOf(oi[k]);
			assert(index != -1);
			Mesh.TRIS.Add(index);
		}
	}
	idVerts.FreeHash();
}

//-----------------------------------------------------------------------------
// SurfRef::SpreadTo : void(SurfRef &, int) const
//-----------------------------------------------------------------------------
void SurfRef::SpreadTo(SurfRef &Ref, int idOrigVert, bool UseSurfaceType) {
	Ref.Free();
	Ref.pOrig = pOrig;
	if(!idTris.Count()) return;

	// Orig Vert:
	Ref.idVerts.Add(idOrigVert);
    const SurfVertex &CenterVertex = pOrig->VERTS[idOrigVert];
    cHashIndex vHash;
	int key = vHash.GenerateKey(idOrigVert);
	vHash.Add(key, 0);
	// Spreading:
	int nTri, oi[3], k, c[3], index, j;
	cHashIndex tHash;
	int tKey;
	while(true) {
		bool fAdded = false;
		// Enuming tris:
		for(int iTri = 0; iTri < idTris.Count(); iTri++) {
			nTri = idTris[iTri];
			// Is this tri already added?
			tKey = tHash.GenerateKey(nTri);
			for(index = tHash.First(tKey); index != -1; index = tHash.Next(index)) {
				if(Ref.idTris[index] == nTri) break;
			}
			if(index != -1) continue; // yes, already added.

			pOrig->GetTri(nTri, oi);

            // SurfaceType limitation:
            bool SkipTriByType = UseSurfaceType;
            for(k = 0; k < 3; k++) {
                if(!SkipTriByType) break;
                const SurfVertex &u = pOrig->VERTS[oi[k]];
                SkipTriByType = u.SurfaceType != CenterVertex.SurfaceType;
            }
            if(SkipTriByType) {
                continue;
            }

			for(k = 0; k < 3; k++) {
				c[0] = oi[k], c[1] = oi[(k + 1) % 3], c[2] = oi[(k + 2) % 3];
				key = vHash.GenerateKey(c[0]);
				for(index = vHash.First(key); index != -1; index = vHash.Next(index)) {
					if(Ref.idVerts[index] == c[0]) break;
				}
				if(index != -1) {
					index = Ref.idTris.Add(nTri);
					tHash.Add(tKey, index);
					fAdded = true;
					for(j = 1; j <= 2; j++) {
						key = vHash.GenerateKey(c[j]);
						for(index = vHash.First(key); index != -1; index = vHash.Next(index)) {
							if(Ref.idVerts[index] == c[j]) break;
						}
						if(index == -1) {
							index = Ref.idVerts.Add(c[j]);
							vHash.Add(key, index);
//							fAdded = true;
						}
					}
				}
				if(fAdded) break;
			}
		}
		if(!fAdded) break;
	}
	Ref.idQuads = idQuads;
}

//-----------------------------------------------------------------------------
// SurfRef::GatherEdges
//-----------------------------------------------------------------------------
bool SurfRef::GatherEdges(SurfEdges &Edges, DWORDS *pEdgeTris) {
	Edges.Free();
	if(pEdgeTris) pEdgeTris->Free();
	if(!idTris.Count()) return true;
	int nSize = cMath::Max(cHashIndex::HASH_DEF_SIZE, cMath::CalcPowerOfTwo(idTris.Count()) / 2);
	cHashIndex *pHash = new cHashIndex(nSize, nSize);
	assert(pHash);
	SurfEdge Edge;
	int nTri, oi[3], k, e0, e1, key, index;
	bool fSuccess = true;

	for(int iTri = 0; iTri < idTris.Count(); iTri++) {
		nTri = idTris[iTri];
		pOrig->GetTri(nTri, oi);
		assert(oi[0] != oi[1] && oi[0] != oi[2] && oi[1] != oi[2] && "Tri is degraded to line or point!");
		for(k = 0; k < 3; k++) {
			e0 = oi[k], e1 = oi[(k + 1) % 3];
			Edge.e0 = cMath::Min(e0, e1);
			Edge.e1 = cMath::Max(e0, e1);
			key = pHash->GenerateKey(Edge.e0, Edge.e1);
			for(index = pHash->First(key); index != -1; index = pHash->Next(index)) {
				if(Edges[index].Equals(Edge)) break;
			}
			if(-1 == index) {
				index = Edges.Add(Edge);
				pHash->Add(key, index);
				Edges[index].f0 = iTri;
			} else {
/*				assert(Edges[index].f1 == -1 && "Detected nonmanifold topology (the T shape)!");
				if(Edges[index].f1 != -1) {
					fSuccess = false;
					goto GatherEdges;
				}*/
				Edges[index].f1 = iTri;
			}
			if(pEdgeTris) pEdgeTris->Add(index);
		}
	}
GatherEdges:
	if(!fSuccess) {
		Edges.Free();
		if(pEdgeTris) pEdgeTris->Free();
	}
	delete pHash;
	return fSuccess;
}

//-----------------------------------------------------------------------------
// SurfRef::GatherNodesBoundaryStats
//-----------------------------------------------------------------------------
bool SurfRef::GatherNodesBoundaryStats(SurfNodes &Nodes) {
	Nodes.Free();
	if(!idVerts.Count()) return true;
	SurfEdges Edges;
	if(!GatherEdges(Edges)) return false;
	if(!Edges.Count()) return true;
	Nodes.SetCount(idVerts.Count());
	idVerts.CreateHash();
	// Enuming edges:
	int i0, i1;
	for(int nEdge = 0; nEdge < Edges.Count(); nEdge++) {
		const SurfEdge &Edge = Edges[nEdge];
		if(-1 == Edge.f1) { // this edge is boundary...
			i0 = idVerts.IndexOf(Edge.e0);
			i1 = idVerts.IndexOf(Edge.e1);
			assert(i0 != -1);
			assert(i1 != -1);
			Nodes[i0].f = true;
			Nodes[i1].f = true;
		}
	}
	idVerts.FreeHash();
	return true;
}

//-----------------------------------------------------------------------------
// SurfRef::Relax : void(double, int, bool)
//-----------------------------------------------------------------------------
void SurfRef::Relax(double Amount, int Iterations, bool HoldBoundaryPoints) {
	assert(pOrig);
	if(!idTris.Count()) return;
	Complete(SR_VERTS, SR_TRIS);
	SurfNodes Nodes;
	if(HoldBoundaryPoints) {
		if(!GatherNodesBoundaryStats(Nodes)) return;
	} else {
		Nodes.SetCount(idVerts.Count());
	}
	int k, nTri, oi[3], nNode, iTri;
	cVec3 u[3];
	double l = cMath::Clamp(Amount, -1., 1.);
	idVerts.CreateHash();
	for(int N = 0; N < Iterations; N++) {
		// Clear Nodes:
		for(k = 0; k < Nodes.Count(); k++) {
			Nodes[k].n = 0;
			Nodes[k].u = cVec3::Zero;
		}
		// Enuming tris and cosidering surroundings for each node:
		for(iTri = 0; iTri < idTris.Count(); iTri++) {
			nTri = idTris[iTri];
			pOrig->GetTri(nTri, oi);
			for(k = 0; k < 3; k++) {
				const SurfVertex &v = pOrig->VERTS[oi[k]];
				u[k] = cVec3((float)v.x, (float)v.y, (float)v.z);
			}
			for(k = 0; k < 3; k++) {
				nNode = idVerts.IndexOf(oi[k]);
				assert(nNode != -1);
				SurfNode &Node = Nodes[nNode];
				Node.u += u[(k + 1) % 3] + u[(k + 2) % 3];
				Node.n += 2;
			}
		}
		// Calcing average pos for each node and storing:
		for(nNode = 0; nNode < Nodes.Count(); nNode++) {
			SurfNode &Node = Nodes[nNode];
			if(!Node.n) continue;
			if(HoldBoundaryPoints && Node.f) continue;
			Node.u /= Node.n;
			SurfVertex &r = pOrig->VERTS[idVerts[nNode]];
			cVec3 CurPos(r.x, r.y, r.z);
			cVec3 Pos = cVec3::Lerp(CurPos, Node.u, l);
			r.x = (short)Pos.x;
			r.y = (short)Pos.y;
			r.z = (short)Pos.z;
		}
	}
	idVerts.FreeHash();
}

//-----------------------------------------------------------------------------
// SurfRef::UpdateNormals
//-----------------------------------------------------------------------------
bool CheckValidTriangle(SurfVertex** sv){
	const float maxduv=1.0f;
	if(
		fabs(sv[1]->u-sv[0]->u)>maxduv ||
		fabs(sv[2]->u-sv[0]->u)>maxduv ||
		fabs(sv[2]->u-sv[1]->u)>maxduv ||
		fabs(sv[1]->v-sv[0]->v)>maxduv ||
		fabs(sv[2]->v-sv[0]->v)>maxduv ||
		fabs(sv[2]->v-sv[1]->v)>maxduv 
		) return false;
	return true;
}
void SurfRef::UpdateNormals(bool ReMakeUV) {	
	assert(pOrig);
	if(!idTris.Count()) return;
	Complete(SR_VERTS, SR_TRIS);
	SurfNodes Nodes;
	if(!GatherNodesBoundaryStats(Nodes)) return;
	int nTri, oi[3], k, nNode;
	cVec3 Normal;
	idVerts.CreateHash();
	for(int iTri = 0; iTri < idTris.Count(); iTri++) {
		nTri = idTris[iTri];
		pOrig->GetTri(nTri, oi);
		pOrig->GetTri(nTri, Normal);
		Normal *= Normal.LengthSq();
		for(k = 0; k < 3; k++) {
			nNode = idVerts.IndexOf(oi[k]);
			assert(nNode != -1);
			SurfNode &Node = Nodes[nNode];
			if(Node.f) continue; // vertex is boundary...
			Node.u += Normal;
		}
	}
	idVerts.FreeHash();
	for(nNode = 0; nNode < Nodes.Count(); nNode++) {
		SurfNode &Node = Nodes[nNode];
		if(Node.f) continue; // vertex is boundary...
		cVec3 &u = Node.u;
		u.Normalize();
		cVec3 u1=u;
		u *= SurfVertex::NormalLen;
		SurfVertex &r = pOrig->VERTS[idVerts[nNode]];
		r.nx = u.x;
		r.ny = u.y;
		r.nz = u.z;

		if(ReMakeUV){
			cVec2 uv = SurfMapper::CalcUV(r,cVec3(r.x,r.y,r.z));
			r.u=uv.x;
			r.v=uv.y;
		}
	}
	cVec3 Tangent;
	cVec3 Binormal;
	idVerts.CreateHash();
	for(int iTri = 0; iTri < idTris.Count(); iTri++) {
		nTri = idTris[iTri];
		SurfVertex* v[3];
		pOrig->GetTri(nTri, &v[0]);
		pOrig->GetTri(nTri, Normal, Tangent, Binormal);
		pOrig->GetTri(nTri, oi);

		float lt=1.0/Tangent.Length();
		Tangent*=lt*lt;
		float lb=1.0/Binormal.Length();
		Binormal*=lb*lb;
		float anisotropy=lt/lb;
		if(anisotropy<1)anisotropy=1/anisotropy;
		bool good_anisotropy=anisotropy>minans && anisotropy<maxans && v[0]->nFacture == v[1]->nFacture && v[0]->nFacture==v[2]->nFacture;
		if(good_anisotropy){
			float A2=cVec3::Dot(Binormal,Tangent);
			A2/=lt*lb;
			if(abs(A2)>0.6)good_anisotropy=false;
		}
		Normal *= Normal.LengthSq();		
		if(good_anisotropy){
			for(k = 0; k < 3; k++) {
				nNode = idVerts.IndexOf(oi[k]);
				assert(nNode != -1);
				SurfNode &Node = Nodes[nNode];
				if(Node.f) continue; // vertex is boundary...				
				Node.t+=Tangent;
				Node.b+=Binormal;
				Node.nt++;				
			}
		}
	}	
	idVerts.FreeHash();
	for(nNode = 0; nNode < Nodes.Count(); nNode++) {
		SurfNode &Node = Nodes[nNode];
		if(Node.f) continue; // vertex is boundary...
		cVec3 &u0 = Node.u;
		int id=idVerts[nNode];
		SurfVertex &r = pOrig->VERTS[id];
		cVec3& t=Node.t;
		cVec3& b=Node.b;
		if(Node.nt!=0){
			float L=t.Length();
			float mc=SurfVertex::NormalLen/L;
			r.tx = short(t.x*mc);
			r.ty = short(t.y*mc);
			r.tz = short(t.z*mc);
			r.tscale = L/Node.nt;

			L=b.Length();
			mc=SurfVertex::NormalLen/L;
			r.bx = short(b.x*mc);
			r.by = short(b.y*mc);
			r.bz = short(b.z*mc);
			r.bscale = L/Node.nt;			
		}
	}
}

//-----------------------------------------------------------------------------
// SubdivideArgs::ReadFromSurfEditor
//-----------------------------------------------------------------------------
void SubdivideArgs::ReadFromSurfEditor() {
	Criterion = g_SurfEditor.Subdivision.ADAPTIVE ? C_DOT : C_EDGELEN;
	fAddRefToQuads = true;
	EdgeMaxLen = g_SurfEditor.Subdivision.EDGEMAXLEN;
	Dot = g_SurfEditor.Subdivision.DOT;
	Scale = g_SurfEditor.Subdivision.SCALE;
}

//-----------------------------------------------------------------------------
// SubdivideArgs.ctor
//-----------------------------------------------------------------------------
SubdivideArgs::SubdivideArgs() {
	ReadFromSurfEditor();
}

using namespace comms;

//-------------------------------------------------------------------------------------------------------------------
// SurfRef::SubdivideEdgeMidPoint
//-------------------------------------------------------------------------------------------------------------------
int SurfRef::SubdivideEdgeMidPoint(const SurfVertex &e0, const SurfVertex &e1, const int EdgeMaxLen, SurfVertex &c) {
	int l2 = cMath::Square(e1.x - e0.x) + cMath::Square(e1.y - e0.y) + cMath::Square(e1.z - e0.z);
	if(l2 <= cMath::Square(EdgeMaxLen)) return 0;
	c = e0;
	c.x = (e0.x + e1.x) / 2;
	c.y = (e0.y + e1.y) / 2;
	c.z = (e0.z + e1.z) / 2;
	return 1;
}

//---------------------------------------------------------------------------------------------------------------------------------
// SurfRef::SubdivideEdgeNormal
//---------------------------------------------------------------------------------------------------------------------------------
int SurfRef::SubdivideEdgeNormal(const SurfVertex &e0, const SurfVertex &e1, const double Dot, const double Scale, SurfVertex &c) {
	// Normals in verts:
	dVector N0(e0.nx, e0.ny, e0.nz);
	N0.Normalize();
	if(!cMath::Equals(1.0, N0.Length())) return -1;
	dVector N1(e1.nx, e1.ny, e1.nz);
	N1.Normalize();
	if(!cMath::Equals(1.0, N1.Length())) return -1;
	// Do we need subdiv ?
	if(N0.Dot(N1) >= Dot) return 0;
	dVector A(e0.x, e0.y, e0.z);
	dVector B(e1.x, e1.y, e1.z);
	// Along edge:
	dVector Along = B - A;
	const double l = Along.Length();
	Along.Normalize();
	if(!cMath::Equals(1.0, Along.Length())) return -1;
	// Basis in A:
	dVector bA = N0.Cross(Along);
	bA.Normalize();
	if(!cMath::Equals(1.0, bA.Length())) return -1;
	// Basis in B:
	dVector bB = N1.Cross(Along);
	bB.Normalize();
	if(!cMath::Equals(1.0, bB.Length())) return -1;
	// Tangent in A:
	dVector t0 = bA.Cross(N0);
	assert(cMath::Equals(1.0, t0.Length()));
	// Tangent in B:
	dVector t1 = bB.Cross(N1);
	assert(cMath::Equals(1.0, t1.Length()));

	t0 *= Scale * l, t1 *= Scale * l;
	const double s = 0.5, s2 = cMath::Square(0.5), s3 = cMath::Cube(0.5);
	dVector h = (2 * s3 - 3 * s2 + 1) * A + (- 2 * s3 + 3 * s2) * B + (s3 - 2 * s2 + s) * t0 + (s3 - s2) * t1;

	dVector N = N0 + N1;
	N.Normalize();
	if(!cMath::Equals(1.0, N.Length())) return -1;
	N *= SurfVertex::NormalLen;
	c = e0;
	c.x = (short)h.x;
	c.y = (short)h.y;
	c.z = (short)h.z;
	c.nx = (short)N.x;
	c.ny = (short)N.y;
	c.nz = (short)N.z;
	
	return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// SurfRef::SubdivideEdgeByRace
//-------------------------------------------------------------------------------------------------------------------
int SurfRef::SubdivideEdgeByRace(const SurfVertex &e0, const SurfVertex &e1, SurfVertex &c0, SurfVertex &c1) {
	if(e0.RaceIndex == e1.RaceIndex) return 0;
	
	c0 = e0;
	c1 = e1;

	cVec3 u(e1.x - e0.x, e1.y - e0.y, e1.z - e0.z);
	float l = cVec3::Length(u);
	u /= l;

	const float MaxEdgeLen = 80.0f;
	// e0 ----- c0 -----|----- c1 ------ e1
	//          |___r___|___r___|
	const float r = MaxEdgeLen * (0.5f - 0.33f);


	if(l < MaxEdgeLen) {
		c0.x = (cMath::Lerp(e0.x, e1.x, 0.33f) + 0.5f);
		c0.y = (cMath::Lerp(e0.y, e1.y, 0.33f) + 0.5f);
		c0.z = (cMath::Lerp(e0.z, e1.z, 0.33f) + 0.5f);

		cVec2 uv0 = SurfMapper::ExtrapolateUV(e0,cVec3(c0.x,c0.y,c0.z));
		c0.u = uv0.x;
		c0.v = uv0.y;
		c0.tx = e0.tx;
		c0.ty = e0.ty;
		c0.tz = e0.tz;
		c0.bx = e0.bx;
		c0.by = e0.by;
		c0.bz = e0.bz;
		
		c1.x = (cMath::Lerp(e1.x, e0.x, 0.33f) + 0.5f);
		c1.y = (cMath::Lerp(e1.y, e0.y, 0.33f) + 0.5f);
		c1.z = (cMath::Lerp(e1.z, e0.z, 0.33f) + 0.5f);

		cVec2 uv1 = SurfMapper::ExtrapolateUV(e0,cVec3(c1.x,c1.y,c1.z));
		c1.u = uv1.x;
		c1.v = uv1.y;
		c1.tx = e1.tx;
		c1.ty = e1.ty;
		c1.tz = e1.tz;
		c1.bx = e1.bx;
		c1.by = e1.by;
		c1.bz = e1.bz;
	} else {
		short cx = e0.x / 2 + e1.x / 2;
		short cy = e0.y / 2 + e1.y / 2;
		short cz = e0.z / 2 + e1.z / 2;

		short dx = (r * u.x + cMath::Sign(u.x) * 0.5f);
		short dy = (r * u.y + cMath::Sign(u.y) * 0.5f);
		short dz = (r * u.z + cMath::Sign(u.z) * 0.5f);

		c0.x = cx - dx;
		c0.y = cy - dy;
		c0.z = cz - dz;

		cVec2 uv0 = SurfMapper::ExtrapolateUV(e0,cVec3(c0.x,c0.y,c0.z));
		c0.u = uv0.x;
		c0.v = uv0.y;
		c0.tx = e0.tx;
		c0.ty = e0.ty;
		c0.tz = e0.tz;
		c0.bx = e0.bx;
		c0.by = e0.by;
		c0.bz = e0.bz;

		c1.x = cx + dx;
		c1.y = cy + dy;
		c1.z = cz + dz;

		cVec2 uv1 = SurfMapper::ExtrapolateUV(e0,cVec3(c1.x,c1.y,c1.z));
		c1.u = uv1.x;
		c1.v = uv1.y;
		c1.tx = e1.tx;
		c1.ty = e1.ty;
		c1.tz = e1.tz;
		c1.bx = e1.bx;
		c1.by = e1.by;
		c1.bz = e1.bz;
	}
	
	int l0 = cMath::Max(LAYERSLIST.MinRaceLandIndex, (int)c0.RaceIndex + LAYERSLIST.MinRaceLandIndex - 1);
	int l1 = cMath::Max(LAYERSLIST.MinRaceLandIndex, (int)c1.RaceIndex + LAYERSLIST.MinRaceLandIndex - 1);
	ClonesArray<BoundaryFacture> &bf0 = LAYERSLIST.Layers[l0]->Boundary;
	if(bf0.GetAmount()) { c0.nFacture = bf0[0]->Facture; }
	ClonesArray<BoundaryFacture> &bf1 = LAYERSLIST.Layers[l1]->Boundary;
	if(bf1.GetAmount()) { c1.nFacture = bf1[0]->Facture; }

	// Bump:
	SurfVertex *p0 = &c0, *p1 = &c1;
	if(p0->RaceIndex > p1->RaceIndex) {
		u *= -1.0f;
		cMath::Swap(p0, p1);
	}

	float d = cVec3::Dot(g_UniversalMap.m_Shader.toSun, u);
	int d0 = (int)(d * LAYERSLIST.Layers[l0]->BUMPAMPLITUDE);
	int d1 = (int)(d * LAYERSLIST.Layers[l1]->BUMPAMPLITUDE);

	if(g_SurfEditor.InvertBumpOnByRaceSubdivisions) {
		cMath::Swap(p0, p1);
		cMath::Swap(d0, d1);
	}
	p0->Color = MulDWORD(p0->Color, 256 + d0);
	p1->Color = MulDWORD(p1->Color, 256 - d1);

	return 1;
}

//-----------------------------------------------------------------------------
// SurfRef::SubdivideByRaceWithDupVerts
//-----------------------------------------------------------------------------
void SurfRef::SubdivideByRaceWithDupVerts() {
	assert(pOrig);
	const int NTris = idTris.Count();
	if(!NTris) return;

	int nTri, oi[3], k, nCase, nCur, ch[9]; // max 9 verts in chain
	SurfVertex c0, c1;
	DWORD Flags;
	const int Tris[4][22] = {{ 0,  1,  8,  1,  2,  4,  2,  3,  4,  1,  4,  8,  4,  5,  8,  5,  7,  8,  5,  6,  7, -1 },
							 { 0,  1,  6,  1,  5,  6,  1,  2,  5,  2,  4,  5,  2,  3,  4, -1, -1, -1, -1, -1, -1, -1 },
							 { 0,  1,  2,  0,  2,  6,  6,  2,  3,  6,  3,  5,  5,  3,  4, -1, -1, -1, -1, -1, -1, -1 },
							 { 0,  1,  6,  1,  2,  6,  6,  2,  5,  5,  2,  3,  5,  3,  4, -1, -1, -1, -1, -1, -1, -1 }};
	const int Cases[] = { -1, -1, -1, 1 /* 011b - e0 && e1 */, -1, 3 /* 101b - e0 && e2 */,
		2 /* 110b - e1 && e2 */, 0 /* 111b - e0 && e1 && e2 */ };
	for(int iTri = 0; iTri < NTris; iTri++) {
		nTri = idTris[iTri];
		pOrig->GetTri(nTri, oi);
		// Filling chain:
		nCur = 0;
		Flags = 0;
		for(k = 0; k < 3; k++) {
			ch[nCur] = oi[k], nCur++;
			const SurfVertex &e0 = pOrig->VERTS[oi[k]];
			const SurfVertex &e1 = pOrig->VERTS[oi[(k + 1) % 3]];
			if(SubdivideEdgeByRace(e0, e1, c0, c1) == 1) {
				Flags |= 1 << k;
				ch[nCur] = pOrig->VERTS.Add(c0), nCur++;
				ch[nCur] = pOrig->VERTS.Add(c1), nCur++;
			}
		}
		if(!Flags) continue; // no subdivision
		// How to triangulate?
		nCase = Cases[Flags];
		assert(nCase != -1 && "Impossible triangulation case!");
		// Triangulation:
		idTris[iTri] = pOrig->AddTri(ch[Tris[nCase][0]], ch[Tris[nCase][1]], ch[Tris[nCase][2]]);
		for(k = 3; k < 21; k += 3) {
			if(-1 == Tris[nCase][k]) break;
			idTris.Add(pOrig->AddTri(ch[Tris[nCase][k]], ch[Tris[nCase][k + 1]], ch[Tris[nCase][k + 2]]));
		}
	}
}

//-----------------------------------------------------------------------------
// SurfRef::SubdivideByRace
//-----------------------------------------------------------------------------
void SurfRef::SubdivideByRace() {
	assert(pOrig);
	const int NTris = idTris.Count();
	if(!NTris) return;
	SurfEdges Edges;
	DWORDS EdgeTris;
	SurfVertex c0, c1;
	if(!GatherEdges(Edges, &EdgeTris)) return;
	// Enuming edges and subdividing if necessary:
	for(int nEdge = 0; nEdge < Edges.Count(); nEdge++) {
		SurfEdge &Edge = Edges[nEdge];
		const SurfVertex &e0 = pOrig->VERTS[Edge.e0];
		const SurfVertex &e1 = pOrig->VERTS[Edge.e1];

		if(SubdivideEdgeByRace(e0, e1, c0, c1) == 1) {
			Edge.f0 = pOrig->VERTS.Add(c0);
			Edge.f1 = pOrig->VERTS.Add(c1);
		} else Edge.f0 = -1, Edge.f1 = -1;
	}
	// Enuming tris and reindexing if necessary:
	int ii0, nTri, i, i0, i1, i2, ch[10], nCase; // max 9 verts in chain (ch[9] == ch[0])
	const int Tris[2][22] = {{ 0,  1,  8,  1,  2,  4,  2,  3,  4,  1,  4,  8,  4,  5,  8,  5,  7,  8,  5,  6,  7, -1 },
						     { 0,  1,  6,  1,  5,  6,  1,  2,  5,  2,  4,  5,  2,  3,  4, -1, -1, -1, -1, -1, -1, -1 }};
	for(int iTri = 0; iTri < NTris; iTri++) {
		ii0 = 3 * iTri;
		SurfEdge *p0 = &Edges[EdgeTris[ii0]];
		SurfEdge *p1 = &Edges[EdgeTris[ii0 + 1]];
		SurfEdge *p2 = &Edges[EdgeTris[ii0 + 2]];

		int nSubDivs = (p0->f0 > - 1 ? 1 : 0) + (p1->f0 > -1 ? 1 : 0) + (p2->f0 > -1 ? 1 : 0);
		if(nSubDivs < 2) continue;

		// Unwind edges:
		if(p0->e1 == p2->e0 || p0->e1 == p2->e1) {
			cMath::Swap(p1, p2);
		}

		if(p0->e1 == p1->e0) {
			if(p1->e1 == p2->e0) {
			} else if(p1->e1 == p2->e1) {
				cMath::Swap(p2->e0, p2->e1);
				cMath::Swap(p2->f0, p2->f1);
			} else assert(0);
		} else if(p0->e1 == p1->e1) {
			cMath::Swap(p1->e0, p1->e1);
			cMath::Swap(p1->f0, p1->f1);
			if(p1->e1 == p2->e0) {
			} else if(p1->e1 == p2->e1) {
				cMath::Swap(p2->e0, p2->e1);
				cMath::Swap(p2->f0, p2->f1);
			}
		} else assert(0);

		assert(p0->e1 == p1->e0);
		assert(p1->e1 == p2->e0);
		assert(p2->e1 == p0->e0);

#define Chain(p, s) ch[s] = p->f0, ch[s + 1] = p->f1, ch[s + 2] = p->e1;

		nCase = -1;
		if(3 == nSubDivs) {
			// Chain:
			ch[0] = p0->e0;
			Chain(p0, 1);
			Chain(p1, 4);
			Chain(p2, 7);

			assert(ch[0] == ch[9]);

			nCase = 0;
		}

		if(2 == nSubDivs) {
			if(p0->f0 > -1 && p1->f0 > -1 && p2->f0 == -1) {
			} else if(p0->f0 == -1 && p1->f0 > -1 && p2->f0 > -1) {
				cMath::Swap(p0, p1);
				cMath::Swap(p1, p2);
			} else if(p0->f0 > -1 && p1->f0 == -1 && p2->f0 > -1) {
				cMath::Swap(p0, p2);
				cMath::Swap(p1, p2);
				if(p1->e0 != p0->e1 && p1->e1 != p0->e1) {
					cMath::Swap(p0->e0, p0->e1);
					cMath::Swap(p0->f0, p0->f1);
				}
			} else assert(0);
			
			// Chain:
			ch[0] = p0->e0;
			Chain(p0, 1);
			Chain(p1, 4);

			nCase = 1;
		}

#undef Chain

		// Triangulation:
		if(nCase >= 0) {
			idTris[iTri] = pOrig->AddTri(ch[Tris[nCase][0]], ch[Tris[nCase][1]], ch[Tris[nCase][2]]);
			for(i = 3; i < 21; i += 3) {
				if(-1 == Tris[nCase][i]) break;
				idTris.Add(pOrig->AddTri(ch[Tris[nCase][i]], ch[Tris[nCase][i + 1]], ch[Tris[nCase][i + 2]]));
			}
		}

	}
}

//-----------------------------------------------------------------------------
// SurfRef::Subdivide
//-----------------------------------------------------------------------------
bool SurfRef::Subdivide(const SubdivideArgs &Args) {
	assert(pOrig);
	int NTris = pOrig->NTris();
	if(!idTris.Count()) return false;
	SurfEdges Edges;
	DWORDS EdgeTris;
	if(!GatherEdges(Edges, &EdgeTris)) return false;
	int l, iTri, ii0, k, e;
	SurfVertex c;
	// Enuming edges and subdividing if necessary:
	for(int nEdge = 0; nEdge < Edges.Count(); nEdge++) {
		SurfEdge &Edge = Edges[nEdge];
		if(-1 == Edge.f1) { // Edge is boundary...
			Edge.f0 = -1;
			continue;
		}
		const SurfVertex &r0 = pOrig->VERTS[Edge.e0];
		const SurfVertex &r1 = pOrig->VERTS[Edge.e1];

		int r = SubdivideArgs::C_DOT == Args.Criterion ?
			SubdivideEdgeNormal(r0, r1, Args.Dot, Args.Scale, c) : SubdivideEdgeMidPoint(r0, r1, Args.EdgeMaxLen, c);

		if(1 == r) {
			Edge.f0 = pOrig->VERTS.Add(c);
		} else Edge.f0 = -1;
	}
	// Enuming tris and reindexing if necessary:
	int nTri, i0, i1, i2, t0, t1, t2;
	SurfEdge E0, E1, E2;
	int idNTris = idTris.Count();
	for(iTri = 0; iTri < idNTris; iTri++) {
		ii0 = 3 * iTri;
		E0 = Edges[EdgeTris[ii0]];
		E1 = Edges[EdgeTris[ii0 + 1]];
		E2 = Edges[EdgeTris[ii0 + 2]];
		nTri = idTris[iTri];
		pOrig->GetTri(nTri, i0, i1, i2);
		ii0 = 3 * nTri;
		t0 = t1 = t2 = -1;
		if(E0.f0 > -1 && E1.f0 > -1 && E2.f0 > -1) {
			//      i2       //
			//      /\       //
			//     /  \      //
			// E2 /----\ E1  //
			//   / \  / \    //
			//  /___\/___\   //
			// i0   E0   i1  //
			pOrig->TRIS[ii0 + 1] = E0.f0;
			pOrig->TRIS[ii0 + 2] = E2.f0;
			t0 = pOrig->AddTri(E0.f0, i1, E1.f0);
			t1 = pOrig->AddTri(E0.f0, E1.f0, E2.f0);
			t2 = pOrig->AddTri(E1.f0, i2, E2.f0);
		}
		if(E0.f0 > -1 && -1 == E1.f0 && -1 == E2.f0) {
			//      i2      //
			//     /|\      //
			//    / | \     //
			//   /  |  \    //
			//  /___|___\   //
			// i0   E0  i1  //
			pOrig->TRIS[ii0 + 1] = E0.f0;
			t0 = pOrig->AddTri(E0.f0, i1, i2);
		}
		if(-1 == E0.f0 && E1.f0 > -1 && -1 == E2.f0) {
			//      i2      //
			//     / \      //
			//    /  /\ E1  //
			//   / /   \    //
			//  //______\   //
			// i0       i1  //
			pOrig->TRIS[ii0 + 2] = E1.f0;
			t0 = pOrig->AddTri(i0, E1.f0, i2);
		}
		if(-1 == E0.f0 && -1 == E1.f0 && E2.f0 > -1) {
			//      i2      //
			//     / \      //
			// E2 /\  \     //
			//   /   \ \    //
			//  /______\\   //
			// i0       i1  //
			pOrig->TRIS[ii0 + 2] = E2.f0;
			t0 = pOrig->AddTri(i1, i2, E2.f0);
		}
		if(E0.f0 > -1 && E1.f0 > -1 && -1 == E2.f0) {
			//      i2      //
			//     /|\      //
			//    / | \ E1  //
			//   /  | /\    //
			//  /___|/__\   //
			// i0   E0  i1  //
			pOrig->TRIS[ii0 + 1] = E0.f0;
			t0 = pOrig->AddTri(E0.f0, i1, E1.f0);
			t1 = pOrig->AddTri(E1.f0, i2, E0.f0);
		}
		if(E0.f0 > -1 && -1 == E1.f0 && E2.f0 > -1) {
			//      i2      //
			//     /|\      //
			// E2 / | \     //
			//   /\ |  \    //
			//  /__\|___\   //
			// i0   E0  i1  //
			pOrig->TRIS[ii0] = E0.f0;
			t0 = pOrig->AddTri(E0.f0, i2, E2.f0);
			t1 = pOrig->AddTri(E0.f0, E2.f0, i0);
		}
		if(-1 == E0.f0 && E1.f0 > -1 && E2.f0 > -1) {
			//      i2      //
			//     /_\      //
			// E2 /  /\ E1  //
			//   / /   \    //
			//  //______\   //
			// i0       i1  //
			pOrig->TRIS[ii0 + 2] = E1.f0;
			t0 = pOrig->AddTri(i0, E1.f0, E2.f0);
			t1 = pOrig->AddTri(E1.f0, i2, E2.f0);
		}
		if(t0 > -1) {
			idTris.Add(t0);
			if(Args.fAddRefToQuads) {
				((SurfMap*)pOrig)->QUADS.AddRef(t0);
			}
			if(t1 > -1) {
				idTris.Add(t1);
				if(Args.fAddRefToQuads) {
					((SurfMap*)pOrig)->QUADS.AddRef(t1);
				}
				if(t2 > -1) {
					idTris.Add(t2);
					if(Args.fAddRefToQuads) {
						((SurfMap*)pOrig)->QUADS.AddRef(t2);
					}
				}
			}
		}
	}
	return pOrig->NTris() > NTris;
}

//-----------------------------------------------------------------------------
// SurfRef::Draw
//-----------------------------------------------------------------------------
void SurfRef::Draw(eSurfFx SFX) const {
	SurfSplit ss;
	ss.Split(*this, SFX);
	ss.Draw();
}

//-----------------------------------------------------------------------------
// SurfMap::Free
//-----------------------------------------------------------------------------
void SurfMap::Free() {
	VERTS.Free();
	TRIS.Free();
	QUADS.Free();
}

//-----------------------------------------------------------------------------
// SurfMap::SurfQuads::Compile
//-----------------------------------------------------------------------------
void SurfMap::SurfQuads::Compile() {
	SurfRef Ref;
	GetRefAll(Ref);
	Ref.Complete(SR_TRIS, SR_QUADS);
	SurfMesh Mesh;
	Ref.ToSurfMesh(Mesh);
	pOrig->VERTS.Copy(Mesh.VERTS);
	pOrig->TRIS.Copy(Mesh.TRIS);
	Init();
}

//-----------------------------------------------------------------------------
// SurfMap::SurfQuads::Update
//-----------------------------------------------------------------------------
void SurfMap::SurfQuads::Update(const SurfRef &Ref) {
	assert(pOrig == Ref.pOrig);
	if(!Ref.idQuads.Count()) return;
	for(int iQ = 0; iQ < Ref.idQuads.Count(); iQ++) {
		int nQ = Ref.idQuads[iQ];
		GetAt(nQ).Update();
	}
}

//-----------------------------------------------------------------------------
// SurfMap::SurfQuads::GetRefAll : void(SurfRef &)
//-----------------------------------------------------------------------------
void SurfMap::SurfQuads::GetRefAll(SurfRef &Ref) const {
	Ref.Free();
	Ref.pOrig = pOrig;
	for(int nQ = 0; nQ < Count(); nQ++) {
		Ref.idQuads.Add(nQ);
	}
}

//-----------------------------------------------------------------------------
// SurfMap::SurfQuads::FillMapOfHeights : void() const
//-----------------------------------------------------------------------------
void SurfMap::SurfQuads::FillMapOfHeights() const {
	SurfRef Ref;
	GetRefAll(Ref);
	Ref.Complete(SR_TRIS, SR_QUADS);
	Debug_CheckRef(Ref); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Ref.FillMapOfHeights();
}

//-----------------------------------------------------------------------------
// SurfMap::SurfQuads::SmoothColors : void()
//-----------------------------------------------------------------------------
void SurfMap::SurfQuads::SmoothColors() {
	SurfRef Ref;
	GetRefAll(Ref);
	Ref.Complete(SR_TRIS, SR_QUADS);
	Ref.SmoothColors();
}

//-----------------------------------------------------------------------------
// SurfMap::SurfQuads::RefreshLods
//-----------------------------------------------------------------------------
void SurfMap::SurfQuads::RefreshLods() {
	Vector3D p = ICam->GetPosition();
	Vector3D c;
	float toQuad;
	int Lod, Qx0, Qy0, Qx1, Qy1;
	float LodRanges[] = { 4000 /* 0 */, 6000 /* 1 */, 8000 /* 2 */ };
	for(int nQ = 0; nQ < Count(); nQ++) {
		Quad &q = GetAt(nQ);
		c.set(q.m_Centre.x, q.m_Centre.y, q.m_Centre.z);
		toQuad = p.distance(c);
		Lod = 3;
		for(int i = 0; i <= 2; i++) {
			if(toQuad < LodRanges[i]) {
				Lod = i;
				break;
			}
		}
		if(q.nLod != Lod) {
			q.nLod = Lod;
			CalcQuadRect(nQ, Qx0, Qy0, Qx1, Qy1);
			Rct rc(Qx0, Qy0, Qx1, Qy1);
			ITerra->InvalidateGeometry(&rc);
		}
	}
}

//-------------------------------------------------------------------------------------------
// SurfMap::SurfQuads::CalcQuadRect
//-------------------------------------------------------------------------------------------
void SurfMap::SurfQuads::CalcQuadRect(int nQ, int &Qx0, int &Qy0, int &Qx1, int &Qy1) const {
	Qx0 = Qy0 = Qx1 = Qy1 = 0;
	assert(nQ < Count());
	if(nQ >= Count()) return;
	int iQx = nQ % QuadsNx;
	int iQy = nQ / QuadsNx;
	Qx0 = QuadDim * iQx;
	Qy0 = QuadDim * iQy;
	Qx1 = Qx0 + QuadDim;
	Qy1 = Qy0 + QuadDim;
}

//-----------------------------------------------------------------------------
// SurfMap::SurfQuads::CalcLodOf
//-----------------------------------------------------------------------------
int SurfMap::SurfQuads::CalcLodOf(const SurfRef &Ref) const {
	int Lod = 0;
	if(!Ref.idQuads.Count()) return 0;
	for(int iQ = 0; iQ < Ref.idQuads.Count(); iQ++) {
		int nQ = Ref.idQuads[iQ];
		Lod += GetAt(nQ).nLod;
	}
	Lod /= Ref.idQuads.Count();
	return Lod;
}

//----------------------------------------------------------------------------------------------------
// SurfMap::SurfQuads::CalcRectOf
//----------------------------------------------------------------------------------------------------
void SurfMap::SurfQuads::CalcRectOf(const SurfRef &Ref, int &x0, int &y0, int &x1, int &y1) const {
	x0 = y0 = x1 = y1 = 0;
	if(!Ref.idQuads.Count()) return;
	x0 = cMath::IntMaxValue;
	y0 = cMath::IntMaxValue;
	x1 = cMath::IntMinValue;
	y1 = cMath::IntMinValue;
	int Qx0, Qy0, Qx1, Qy1;
	for(int iQ = 0; iQ < Ref.idQuads.Count(); iQ++) {
		int nQ = Ref.idQuads[iQ];
		CalcQuadRect(nQ, Qx0, Qy0, Qx1, Qy1);
		x0 = cMath::Min(x0, Qx0);
		y0 = cMath::Min(y0, Qy0);
		x1 = cMath::Max(x1, Qx1);
		y1 = cMath::Max(y1, Qy1);
	}
}

short SurfMap::THMapZ(int MapCoordX, int MapCoordY)
{
	int Lx = 32 * VertInLine, Ly = 32 * MaxTH;
	if(MapCoordX >= 0 && MapCoordX < Lx && MapCoordY >= 0 && MapCoordY < Ly)
	{
		int nx = MapCoordX / 32, ny = MapCoordY / 32;
		int z0 = THMap[nx + VertInLine * ny],
			z1 = THMap[(nx + 1) + VertInLine * ny],
			z2 = THMap[nx + VertInLine * (ny + 1)],
			z3 = THMap[(nx + 1) + VertInLine * (ny + 1)];
		return short((z0 + z1 + z2 + z3) / 4);
	}
	return 0;
}

//-------------------------------------------------------------------------------------------
// SurfMap::SurfQuads::GetRefCylinder : void(SurfRef &, const SurfVertex &, int)
//-------------------------------------------------------------------------------------------
void SurfMap::SurfQuads::GetRefCylinder(SurfRef &Ref, const SurfVertex &Center, int Radius) {
	Ref.Free();
	Ref.pOrig = pOrig;
	for(int nQ = 0; nQ < Count(); nQ++) {
		const SurfVertex &q = GetAt(nQ).m_Centre;
		int r2 = cMath::Square(GetAt(nQ).m_Radius + Radius);
		int dx = Center.x - (int)q.x;
		int dy = Center.y - (int)q.y;
		if(cMath::Square(dx) + cMath::Square(dy) <= r2) {
			Ref.idQuads.Add(nQ);
		}
	}
}

//-----------------------------------------------------------------------------
// SurfMap::SurfQuads::CutRef : void(SurfRef &)
//-----------------------------------------------------------------------------
void SurfMap::SurfQuads::CutRef(SurfRef &Ref) {
	if(/*!Ref.idQuads.Count() || */!Ref.idTris.Count()) return;
	for(int nQ = 0; nQ < Count(); nQ++) {
		Quad &q = GetAt(nQ);
		if(!q.idTris.Count()) continue;
		for(int iTri = 0; iTri < Ref.idTris.Count(); iTri++) {
			int nTri = Ref.idTris[iTri];
			int index = q.idTris.IndexOf(nTri);
		}
	}
/*	for(int nQ = 0; nQ < Count(); nQ++) {
		Quad &q = GetAt(nQ);
		if(!q.idTris.Count()) continue;
		for(int iTri = 0; iTri < Ref.idTris.Count(); iTri++) {
			int nTri = Ref.idTris[iTri];
			int index = q.idTris.IndexOf(nTri);
			if(index > 0) {
				q.idTris.RemoveAt(index);
				if(!q.idTris.Count()) break;
			} else iTri++;
		}
	}*/
/*	for(int iQ = 0; iQ < Ref.idQuads.Count(); iQ++) {
		int nQ = Ref.idQuads[iQ];
		Quad &q = GetAt(nQ);
		if(!q.idTris.Count()) continue;
//		q.idTris.BeginHashedSearch();
		for(int iTri = 0; iTri < Ref.idTris.Count(); iTri++) {
			int nTri = Ref.idTris[iTri];
			int index = q.idTris.IndexOf(nTri);
			if(index > 0) {
				q.idTris.RemoveAt(index);
//				q.idTris.EndHashedSearch();
				if(!q.idTris.Count()) break;
//				q.idTris.BeginHashedSearch();
			}
		}
//		q.idTris.EndHashedSearch();
	}
	Ref.idQuads.Free();*/
}

//------------------------------------------------------------------------------------------------------------------------
// SurfMap::QueryRef : void(SurfRef &, QueryRefArgs, DWORD)
//------------------------------------------------------------------------------------------------------------------------
void SurfMap::QueryRef(SurfRef &Ref, QueryRefArgs Args, DWORD What) {
	Ref.Free();
	Ref.pOrig = this;
	if((What & SR_QUADS || What & SR_TRIS || What & SR_VERTS) == 0) return;
	// Rect?
	if(RT_RECT == Args.Type) {
		Args.Center.x = (Args.x0 + Args.x1) / 2;
		Args.Center.y = (Args.y0 + Args.y1) / 2;
		Args.Radius = int(sqrtf(cMath::Square(float(Args.x1 - Args.x0) / 2.0f) +
			cMath::Square(float(Args.y1 - Args.y0) / 2.0f)) + 0.5f);
	}
	// Ref to quads:
	if(RT_LAYER == Args.Type) {
		QUADS.GetRefAll(Ref);
	} else {
		QUADS.GetRefCylinder(Ref, Args.Center, Args.Radius);
	}
	if(!Ref.idQuads.Count()) return;
	if((What & SR_TRIS || What & SR_VERTS) == 0) return;
	SurfRef sr(Ref);
	sr.Complete(SR_VERTS, SR_QUADS);
	if(!sr.idVerts.Count()) return;
	// Adding verts:
	for(int iVert = 0; iVert < sr.idVerts.Count(); iVert++) {
		int nVert = sr.idVerts[iVert];
		const SurfVertex &v = VERTS[nVert];
		bool fSelect = false;
		switch(Args.Type) {
			case RT_CYLINDER:
				fSelect = v.InCylinder(Args.Center, Args.Radius);
				break;
			case RT_SPHERE:
				fSelect = v.InSphere(Args.Center, Args.Radius);
				break;
			case RT_RECT:
				fSelect = v.InRect(Args.x0, Args.y0, Args.x1, Args.y1);
				break;
			case RT_LAYER:
				fSelect = SurfLayerIsSet(Args.idLayer, v.x, v.y);
				break;
			default:
				assert(0);
		}
		if(fSelect) {
			Ref.idVerts.Add(nVert);
		}
	}
	if(!Ref.idVerts.Count()) return;
	if((What & SR_TRIS) == 0) return;
	// Adding / cutting tris which have at least one already added vertex:
	Ref.idVerts.CreateHash();
	int oi[3];
	for(int iQ = 0; iQ < Ref.idQuads.Count(); iQ++) {
		int nQ = Ref.idQuads[iQ];
		Quad &q = QUADS[nQ];
		int NTris = q.idTris.Count();
		for(int iTri = 0; iTri < q.idTris.Count();) {
			int nTri = q.idTris[iTri];
			GetTri(nTri, oi);
			for(int k = 0; k < 3; k++) {
				if(Ref.idVerts.IndexOf(oi[k]) != -1) {
					Ref.idTris.Add(nTri);
					break;
				}
			}
			if(RA_CUT == Args.Action && k < 3) {
				q.idTris.RemoveAt(iTri);
			} else iTri++;
		}
		if(q.idTris.Count() < NTris) {
			q.Update();
		}
	}
	Ref.idVerts.FreeHash();
}

//--------------------------------------------------------------------------------------------
// SurfMap::GetRefCylinder : void(SurfRef &, const SurfVertex &, int, DWORD)
//--------------------------------------------------------------------------------------------
void SurfMap::GetRefCylinder(SurfRef &Ref, const SurfVertex &Center, int Radius, DWORD What) {
	QueryRefArgs Args;
	ZeroMemory(&Args, sizeof(Args));
	Args.Type = RT_CYLINDER;
	Args.Action = RA_GET;
	Args.Center = Center;
	Args.Radius = Radius;
	QueryRef(Ref, Args, What);
}

//------------------------------------------------------------------------------------------
// SurfMap::GetRefSphere : void(SurfRef &, const SurfVertex &, int, DWORD)
//------------------------------------------------------------------------------------------
void SurfMap::GetRefSphere(SurfRef &Ref, const SurfVertex &Center, int Radius, DWORD What) {
	QueryRefArgs Args;
	ZeroMemory(&Args, sizeof(Args));
	Args.Type = RT_SPHERE;
	Args.Action = RA_GET;
	Args.Center = Center;
	Args.Radius = Radius;
	QueryRef(Ref, Args, What);
}

//----------------------------------------------------------------------------------
// SurfMap::GetRefRect : void(SurfRef &, int, int, int, int, DWORD)
//----------------------------------------------------------------------------------
void SurfMap::GetRefRect(SurfRef &Ref, int x0, int y0, int x1, int y1, DWORD What) {
	QueryRefArgs Args;
	ZeroMemory(&Args, sizeof(Args));
	Args.Type = RT_RECT;
	Args.Action = RA_GET;
	Args.x0 = x0, Args.y0 = y0, Args.x1 = x1, Args.y1 = y1;
	QueryRef(Ref, Args, What);
}

//--------------------------------------------------------------------------------------------
// SurfMap::CutRefCylinder : void(SurfRef &, const SurfVertex &, int, DWORD)
//--------------------------------------------------------------------------------------------
void SurfMap::CutRefCylinder(SurfRef &Ref, const SurfVertex &Center, int Radius, DWORD What) {
	QueryRefArgs Args;
	ZeroMemory(&Args, sizeof(Args));
	Args.Type = RT_CYLINDER;
	Args.Action = RA_CUT;
	Args.Center = Center;
	Args.Radius = Radius;
	QueryRef(Ref, Args, What);
}

//-----------------------------------------------------------------------------
// SurfMap::CutRefAlongSurfLayer : void(SurfRef &, int, DWORD)
//-----------------------------------------------------------------------------
void SurfMap::CutRefAlongSurfLayer(SurfRef &Ref, int idLayer, DWORD What) {
	QueryRefArgs Args;
	ZeroMemory(&Args, sizeof(Args));
	Args.Type = RT_LAYER;
	Args.Action = RA_CUT;
	Args.idLayer = idLayer;
	QueryRef(Ref, Args, What);
}

//-----------------------------------------------------------------------------
// SurfMap::GetRefAlongSurfLayer : void(SurfRef &, int, DWORD)
//-----------------------------------------------------------------------------
void SurfMap::GetRefAlongSurfLayer(SurfRef &Ref, int idLayer, DWORD What) {
	QueryRefArgs Args;
	ZeroMemory(&Args, sizeof(Args));
	Args.Type = RT_LAYER;
	Args.Action = RA_GET;
	Args.idLayer = idLayer;
	QueryRef(Ref, Args, What);
}

//-----------------------------------------------------------------------------
// SurfMap::FillMapOfHeights : void() const
//-----------------------------------------------------------------------------
void SurfMap::FillMapOfHeights() {
	QUADS.FillMapOfHeights();
	for(int i = 0; i < MOptions.Archways.NValues; i++) {
		const SurfArchway *pWay = MOptions.Archways[i];
		SurfRef Ref;
		GetRefCylinder(Ref, SurfVertex(pWay->cx, pWay->cy), pWay->r, SR_TRIS);
		Ref.FillMapOfHeights(pWay->h);
	}
}

//---------------------------------------------------------------------------------------------------------
// SurfQuads::CalcCollisionForce : float3(const cMath::float3 &, const float, const float)
//---------------------------------------------------------------------------------------------------------
cVec3 SurfMap::SurfQuads::CalcCollisionForce(const cVec3 &Center, const float Radius, const float k) {
	assert(Radius > 0);
	cVec3 Force(0);
	SurfRef Ref;
	GetRefAroundCenter(Ref, SurfVertex(Center.x, Center.y, Center.z), Radius);
	if(Ref.idQuads.Count()) {
		Ref.Complete(SR_VERTS, SR_QUADS);
		assert(Ref.idVerts.Count());
		for(int iVert = 0; iVert < Ref.idVerts.Count(); iVert++) {
			int nVert = Ref.idVerts[iVert];
			const SurfVertex &u = pOrig->VERTS[nVert];
			cVec3 d(u.x - Center.x, u.y - Center.y, u.z - Center.z);
			float l = d.Length();
			if(l < Radius) {
				cVec3 N = cVec3::Normalize(cVec3(u.nx, u.ny, u.nz));
				if(N.IsNormalized()) {
					float x = Radius - l;
					Force += N * k * x;
				}
			}
		}
	}
	return Force;
}

//-----------------------------------------------------------------------------------------------
// SurfQuads::GetRefAroundCenter : void(SurfRef &, const SurfVertex &, int)
//-----------------------------------------------------------------------------------------------
void SurfMap::SurfQuads::GetRefAroundCenter(SurfRef &Ref, const SurfVertex &Center, int Radius) {
	Ref.Free();
	Ref.pOrig = pOrig;
	for(int nQ = 0; nQ < Count(); nQ++) {
		const SurfVertex &q = GetAt(nQ).m_Centre;
		int r = GetAt(nQ).m_Radius;
		int l = sqrt(cMath::Square(q.x - Center.x) + cMath::Square(q.y - Center.y) + cMath::Square(q.z - Center.z));
		assert(Radius >= 0);
		if(l <= r + Radius) {
			Ref.idQuads.Add(nQ);
		}
	}
}

//------------------------------------------------------------------------------------------------------------------
// SurfQuads::GetRefAlongRay : void(SurfRef &, const dVector &, const dVector &)
//------------------------------------------------------------------------------------------------------------------
void SurfMap::SurfQuads::GetRefAlongRay(SurfRef &Ref, const dVector &Orig, const dVector &Dir) {
	Ref.Free();
	Ref.pOrig = pOrig;
	for(int nQ = 0; nQ < Count(); nQ++) {
		const SurfVertex &q = GetAt(nQ).m_Centre;
		int r = GetAt(nQ).m_Radius;
		dVector c(q.x, q.y, q.z);
		double l = c.DistToLine(Orig, Dir);
		if(l <= r) {
			if((c - Orig).Dot(Dir) >= 0 || c.DistTo(Orig) <= r) {
				Ref.idQuads.Add(nQ);
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// SurfQuads::GetRefRect : void(SurfRef &, int, int, int, int)
//--------------------------------------------------------------------------------------
void SurfMap::SurfQuads::GetRefRect(SurfRef &Ref, int x0, int y0, int x1, int y1) {
	if(!Count()) return;
	Ref.Free();
	Ref.pOrig = pOrig;
	int nQx0 = x0 / QuadDim;
	//cMath::Clamp(nQx0, 0, QuadsNx - 1);
	int nQy0 = y0 / QuadDim;
	//cMath::Clamp(nQy0, 0, QuadsNy - 1);
	int nQx1 = x1 / QuadDim;
	//cMath::Clamp(nQx1, 0, QuadsNx - 1);
	int nQy1 = y1 / QuadDim;
	//cMath::Clamp(nQy1, 0, QuadsNy - 1);
	for(int nQx = nQx0; nQx <= nQx1; nQx++) {
		for(int nQy = nQy0; nQy <= nQy1; nQy++) {
			int n = QuadsNx * nQy + nQx;
			//assert(cMath::IsInRange(n, 0, Count() - 1));
			if(cMath::IsInRange(n, 0, Count() - 1)){
				Ref.idQuads.Add(n);
			}
		}
	}
}

// Quad::Free
void SurfMap::Quad::Free() {
	((SurfRef*)this)->Free();
	for(int i = 0; i < NLODS; i++) {
		LODS[i].Free();
	}
}

//-----------------------------------------------------------------------------
// Quad::Copy
//-----------------------------------------------------------------------------
SurfMap::Quad & SurfMap::Quad::Copy(const Quad &Src) {
	Free();
	((SurfRef*)this)->Copy(Src);
	m_Centre = Src.m_Centre;
	m_Radius = Src.m_Radius;
	for(int i = 0; i < NLODS; i++) {
		LODS[i].Copy(Src.LODS[i]);
	}
	return *this;
}

void Degrade3Lods(DWORDS *pLODS, const SurfMesh &Src) {
	VIPMLodder Lodder;
	// Adding verts:
	Vector3D n;
	for(int nVert = 0; nVert < Src.VERTS.Count(); nVert++) {
		const SurfVertex &r = Src.VERTS[nVert];
		n = Vector3D(r.nx, r.ny, r.nz);
		n.normalize();
		Lodder.AddVertex(Vector3D(r.x, r.y, r.z), n);
	}
	// Adding tris:
	int i0, i1, i2;
	for(int nTri = 0; nTri < Src.NTris(); nTri++) {
		Src.GetTri(nTri, i0, i1, i2);
		Lodder.AddFace(i0, i1, i2);
	}
	Lodder.Process();
	for(int nLod = 1; nLod <= 3; nLod++) {
		int NCollapses = cMath::Min(int(Src.VERTS.Count() - Src.VERTS.Count() / pow(2, nLod)), Lodder.GetMaxCollapses());
		Lodder.SetNCollapses(NCollapses);
		pLODS[nLod - 1].Free();
		
		int NTris = Lodder.GetNFaces();
		for(nTri = 0; nTri < NTris; nTri++) {
			Lodder.GetFace(nTri, i0, i1, i2);
			pLODS[nLod - 1].Add(Lodder.GetOldVIndex(i0));
			pLODS[nLod - 1].Add(Lodder.GetOldVIndex(i1));
			pLODS[nLod - 1].Add(Lodder.GetOldVIndex(i2));
		}
	}
	Lodder.Reset();
}

//-----------------------------------------------------------------------------
// SurfQuads::BeginPassedLODsGen
//-----------------------------------------------------------------------------
int SurfMap::SurfQuads::BeginPassedLODsGen() {
/*	SurfMesh Mesh;
	pOrig->VERTS.BeginHashedSearch();
	for(int nQ = 0; nQ < Count(); nQ++) {
		Quad &q = GetAt(nQ);
		q.LODS[0].Free();
		q.LODS[1].Free();
		q.LODS[2].Free();
		q.Convert(SR_TRIS, Mesh);
		if(!Mesh.TRIS.Count()) continue;
		Degrade3Lods(q.LODS, Mesh);
		for(int l = 0; l < 3; l++) {
			for(int i = 0; i < q.LODS[l].Count(); i++) {
				int oi = pOrig->VERTS.IndexOf(Mesh.VERTS[q.LODS[l][i]]);
				assert(oi != -1);
				q.LODS[l][i] = oi;
			}
		}
	}
	pOrig->VERTS.EndHashedSearch();
	fUseLods = true;
	return 0;*/
	assert(-1 == m_CurPass);
	if(!Count() || !pOrig->VERTS.Count()) return 0;
	pOrig->VERTS.CreateHash();
	m_CurPass = 0;
	return QuadsNy;
}

//-----------------------------------------------------------------------------
// SurfQuads::LODsOnePass
//-----------------------------------------------------------------------------
bool SurfMap::SurfQuads::LODsOnePass() {
	if(-1 == m_CurPass) return false;
	if(m_CurPass < QuadsNy) {
		SurfMesh Mesh;
		int nQ0 = QuadsNx * m_CurPass;
		for(int nQ = nQ0; nQ < nQ0 + QuadsNx; nQ++) {
			Quad &q = GetAt(nQ);
			q.LODS[0].Free();
			q.LODS[1].Free();
			q.LODS[2].Free();
			q.ToSurfMesh(Mesh);
			if(!Mesh.TRIS.Count()) continue;
			Degrade3Lods(q.LODS, Mesh);
			for(int l = 0; l < 3; l++) {
				for(int i = 0; i < q.LODS[l].Count(); i++) {
					int oi = pOrig->VERTS.IndexOf(Mesh.VERTS[q.LODS[l][i]]);
					assert(oi != -1);
					q.LODS[l][i] = oi;
				}
			}
		}
		m_CurPass++;
		return true;
	} else {
		m_CurPass = -1;
		pOrig->VERTS.FreeHash();
		fUseLods = true;
		return false;
	}
}

//-----------------------------------------------------------------------------
// SurfQuads::LODS
//-----------------------------------------------------------------------------
/*void SurfMap::SurfQuads::LODS() {
	SurfMesh Mesh;
	pOrig->VERTS.BeginHashedSearch();
	for(int nQ = 0; nQ < Count(); nQ++) {
		Quad &q = GetAt(nQ);
		q.Convert(SR_TRIS, Mesh);
		if(!Mesh.TRIS.Count()) continue;
		Degrade3Lods(q.LODS, Mesh);
		for(int l = 0; l < 3; l++) {
			for(int i = 0; i < q.LODS[l].Count(); i++) {
				int oi = pOrig->VERTS.IndexOf(Mesh.VERTS[q.LODS[l][i]]);
				assert(oi != -1);
				q.LODS[l][i] = oi;
			}
		}
	}
	pOrig->VERTS.EndHashedSearch();
	fUseLods = true;
}*/

//-----------------------------------------------------------------------------
// Quad::operator =
//-----------------------------------------------------------------------------
SurfMap::Quad & SurfMap::Quad::operator = (const Quad &Src) {
	return Copy(Src);
}

//-----------------------------------------------------------------------------
// Quad::Update : void()
//-----------------------------------------------------------------------------
void SurfMap::Quad::Update() {
	CalcBoundSphere(SR_TRIS, m_Centre, m_Radius);
}

//-----------------------------------------------------------------------------
// SurfQuads::Copy
//-----------------------------------------------------------------------------
SurfMap::SurfQuads & SurfMap::SurfQuads::Copy(const SurfQuads &Src) {
	((cList<Quad>*)this)->Copy(Src);
	pOrig = Src.pOrig;
	QuadsNx = Src.QuadsNx, QuadsNy = Src.QuadsNy;
	return *this;
}

//-----------------------------------------------------------------------------
// SurfQuads::AddRef
//-----------------------------------------------------------------------------
int SurfMap::SurfQuads::AddRef(int nTri) {
	// By Center:
	SurfVertex c;
	pOrig->GetTri(nTri, c);
	int nQx = c.x / QuadDim;
	if(!cMath::IsInRange(nQx, 0, QuadsNx - 1)) return -1;
	int nQy = c.y / QuadDim;
	if(!cMath::IsInRange(nQy, 0, QuadsNy - 1)) return -1;
	int nQ = QuadsNx * nQy + nQx;
	assert(cMath::IsInRange(nQ, 0, Count() - 1));
	GetAt(nQ).idTris.Add(nTri);
	return nQ;
	// By vertex:
/*	SurfVertex t[3];
	pOrig->GetTri(nTri, t);
	int n[3];
	for(int k = 0; k < 3; k++) {
		const SurfVertex &r = t[k];
		int nQx = r.x / Dim;
		int nQy = r.y / Dim;
		if(cMath::IsInRange(nQx, 0, Nx - 1) && cMath::IsInRange(nQy, 0, Ny - 1)) {
		n[k] = Nx * nQy + nQx;
		} else n[k] = -1;
	}
	// Adding at least one ref:
	if(n[0] != -1) Quads[n[0]].Add(nTri);
	if(n[1] != -1 && n[1] != n[0]) Quads[n[1]].Add(nTri);
	if(n[2] != -1 && n[2] != n[1] && n[2] != n[0]) Quads[n[2]].Add(nTri);*/
}

//-----------------------------------------------------------------------------
// SurfQuads::Init
//-----------------------------------------------------------------------------
void SurfMap::SurfQuads::Init() {
	Free();
	fUseLods = false;
	QuadsNx = QuadsNy = 0;
	// Size of map:
	SurfVertex bbMin, bbMax;
	pOrig->CalcBoundBox(bbMin, bbMax);
	int Sx = bbMax.x;
	int Sy = bbMax.y;
	if(Sx > 0 && Sy > 0) {
		// Number of quads:
		QuadsNx = (Sx + 1) / QuadDim, QuadsNy = (Sy + 1) / QuadDim;
		if(QuadDim * QuadsNx <= Sx) QuadsNx++;
		if(QuadDim * QuadsNy <= Sy) QuadsNy++;
		// Lock:
		SetCount(QuadsNx * QuadsNy, Quad(pOrig));
		// Adding ref:
		for(int nTri = 0; nTri < pOrig->NTris(); nTri++) {
			AddRef(nTri);
		}
		// Bound sphere:
		for(int i = 0; i < Count(); i++) {
			GetAt(i).Update();
		}
	}
}

//-----------------------------------------------------------------------------
// SurfMap::Sum
//-----------------------------------------------------------------------------
void SurfMap::Sum(const SurfMesh &Src) {
	int T = Src.NTris();
	if(!T) return;
	VERTS.CreateHash();
	int i[3];
	SurfVertex v[3];
	for(int nTri = 0; nTri < T; nTri++) {
		Src.GetTri(nTri, v);
		for(int k = 0; k < 3; k++) {
			i[k] = VERTS.IndexOf(v[k]);
			if(-1 == i[k]) {
				i[k] = VERTS.AddHashed(v[k]);
			}
		}
		QUADS.AddRef(AddTri(i[0], i[1], i[2]));
	}
	VERTS.FreeHash();
}

//-----------------------------------------------------------------------------
// SurfMesh::ToRawMesh : bool(cRawMesh &) const
//-----------------------------------------------------------------------------
bool SurfMesh::ToRawMesh(cRawMesh &rm) const {
	rm.Clear();
	
	int NTris = TRIS.Count() / 3;
	bool fMultipleBy3 = 3 * NTris == TRIS.Count();
	assert(fMultipleBy3 && "Error in SurfMesh topology!");
	if(!fMultipleBy3) {
		return false;
	}

	DWORDS Tris;
	for(int i = 0; i < TRIS.Count(); i++) {
		int index = TRIS[i];
		bool fInVertsRange = cMath::IsInRange(index, 0, VERTS.Count() - 1);
		assert(fInVertsRange && "Error in SurfMesh topology!");
		if(!fInVertsRange) return false;
		Tris.Add(index);
	}

	for(i = 0; i < Tris.Count(); i += 3) {
		cMath::Swap(Tris[i + 1], Tris[i + 2]);
	}

	for(int index = 0; index < Tris.Count(); index += 3) {
		const DWORD *Tri = &Tris[index];
		rm.GetRaw().Add(cVec3i(3, 0, 0));
		rm.GetRaw().Add(cVec3i(Tri[0], 0, 0));
		rm.GetRaw().Add(cVec3i(Tri[1], 0, 0));
		rm.GetRaw().Add(cVec3i(Tri[2], 0, 0));
	}

	for(i = 0; i < VERTS.Count(); i++) {
		const SurfVertex &r = VERTS[i];
		rm.GetPositions().Add(cVec3((float)r.x, (float)r.z, (float)r.y));
	}
	return true;
}

//-----------------------------------------------------------------------------
// SurfMesh::CopyFromRawMesh : bool(const cRawMesh &)
//-----------------------------------------------------------------------------
bool SurfMesh::CopyFromRawMesh(const cRawMesh &rm) {
	Free();

	if(!rm.IsValid(true)) {
		return false;
	}

	cAssert(rm.IsTriangulated());
	if(!rm.IsTriangulated()) {
		return false;
	}
	
	int NTris = rm.GetPolyCount();

    for(int i = 0; i < rm.GetPositions().Count(); i++) {
		const cVec3 &u = rm.GetPositions()[i];
		SurfVertex sv;
		sv.x=u.x;
        sv.y=u.z;
        sv.z=u.y;
        sv.Color=0xFFFFFFFF;
        sv.SurfaceType=0;
		VERTS.Add(sv);
	}

	DWORDS Tris;
	int index = 0;
	while(index < rm.GetRaw().Count()) {
		const int Deg = rm.GetRaw()[index][0];
		const int idMtl = rm.GetRaw()[index][1];
		for(int k = 0; k < 3; k++) {
			const int i0 = rm.GetRaw()[index + k + 1][0];
			Tris.Add(i0);
			VERTS[i0].SurfaceType = idMtl;
		}
		index += Deg + 1;
	}

	for(i = 0; i < Tris.Count(); i += 3) {
		cMath::Swap(Tris[i + 1], Tris[i + 2]);
	}

	TRIS = Tris;
	
	return true;
}

//-----------------------------------------------------------------------------
// SurfMap::Copy : SurfMap & (const SurfMap &)
//-----------------------------------------------------------------------------
SurfMap & SurfMap::Copy(const SurfMap &Src) {
	((SurfMesh*)this)->Copy(Src);
	QUADS.Copy(Src.QUADS);
	return *this;
}

const char *g_SurfBackUpFileNames[2] = { "SurfMap.BackUp.1st", "SurfMap.BackUp.2nd" };
int g_SurfBackUpFileNameIndex = 0;

//-----------------------------------------------------------------------------
// SurfMap::BackUp
//-----------------------------------------------------------------------------
void SurfMap::BackUp() {
	//UndoMaster::RemoveAllFromDisk();
	SurfaceUndo::DropWholeMap();
	/*
	if(m_pBackUp) { // To Memory
		m_pBackUp->Copy(*this);
	} else { // To File
		const char *pFileName = g_SurfBackUpFileNames[g_SurfBackUpFileNameIndex];
		ResFile r = RRewrite(pFileName);
		if(INVALID_HANDLE_VALUE == r) {
			return;
		}
		void SaveNewSurface(ResFile);
		SaveNewSurface(r);
		RClose(r);
		cStr Msg("Stored to ");
		Msg += pFileName;
		g_Messager.ShowMessage(Msg);
	}
	*/
}

//-----------------------------------------------------------------------------
// SurfMap::Undo
//-----------------------------------------------------------------------------
void SurfMap::Undo() {
	if(m_pBackUp) { // From memory
		SurfMap TMap;
		TMap.Copy(*m_pBackUp);
		m_pBackUp->Copy(*this);
		if(!TMap.TRIS.Count()) return;
		this->Copy(TMap);
	} else { // From file
		// BackUp Cur to 2nd file:
		const char *p2ndFileName = g_SurfBackUpFileNames[g_SurfBackUpFileNameIndex ^ 1];
		ResFile r = RRewrite(p2ndFileName);
		if(INVALID_HANDLE_VALUE == r) {
			return;
		}
		void SaveNewSurface(ResFile);
		SaveNewSurface(r);
		RClose(r);
		// Restore 1st file:
		const char *p1stFileName = g_SurfBackUpFileNames[g_SurfBackUpFileNameIndex];
		r = RReset(p1stFileName);
		if(INVALID_HANDLE_VALUE == r) {
			return;
		}
		DWORD Magic, Size;
		RBlockRead(r, &Magic, 4);
		RBlockRead(r, &Size, 4);
		if('5HSM' == Magic) {
			void LoadNewSurface5(ResFile);
			LoadNewSurface5(r);
		} else {
			MessageBox(g_hWnd, "Unknown magic in BackUp file!", "--- ERROR ---", MB_OK);
		}
		RClose(r);
		// Message:
		cStr Msg("Restored from ");
		Msg += p1stFileName;
		g_Messager.ShowMessage(Msg);
		// Swap 1st file name with 2nd:
		g_SurfBackUpFileNameIndex ^= 1;
	}
}

//-----------------------------------------------------------------------------
// SurfMap::BooleansSum
//-----------------------------------------------------------------------------
void SurfMap::BooleansSum(SurfMesh &Bsm) {
	//if(!FastBool)BackUp();
	SurfRef Ref;
	Bsm.GetRefAll(Ref, SR_TRIS);
	Ref.FillMapOfHeights();
	Sum(Bsm);
	QUADS.Init();
}

//-----------------------------------------------------------------------------
// SurfMap::Booleans
//-----------------------------------------------------------------------------
void SurfMap::Booleans(SurfMesh &B, eBooleansOp Op) {
	B.WeldVertices();

	// "B" operand center and radius:
	SurfVertex Center;
	int Radius;
	B.CalcBoundSphere(Center, Radius);

	// Storing undo info:
	const float UndoMargin = 100;
	const cVec2 luUndo(Center.x - Radius - UndoMargin, Center.y - Radius - UndoMargin);
	const cVec2 rbUndo(Center.x + Radius + UndoMargin, Center.y + Radius + UndoMargin);
	SurfaceUndo::SaveSurfaceRect(luUndo, rbUndo, true, false);

	// Cut ref from surface of operand "A":
	SurfRef CutRef;
	const int CutMargin = 80;
	CutRefCylinder(CutRef, Center, Radius + CutMargin, SR_ALL);

	// Creating operand "A":
	SurfMesh A;
	CutRef.ToSurfMesh(A);
	A.WeldVertices();

	// In difference we should invert tris before operation...
	if(BO_DIFFERENCE == Op) {
		A.InvertTris();
	}

	// Creating pre operands "A" & "B":
	int i;
	const float Amplitude = 0.04f;
	cVec3 s;
	for(i = 0; i < B.VERTS.Count(); i++) {
		s.x = (rand() - 16384) * Amplitude / 16384;
		s.y = (rand() - 16384) * Amplitude / 16384;
		s.z = (rand() - 16384) * Amplitude / 16384;
		B.VERTS[i].Pos() += s;
	}
	
	uni_hash<SurfVertex, tri_DWORD> Attribs;
	for(i = 0; i < A.VERTS.Count(); i++) {
		SurfVertex &u = A.VERTS[i];
		Attribs.add(tri_DWORD(u.x * 256 + 128, u.y * 256 + 128, u.z * 256 + 128), u);
	}
	for(i = 0; i < B.VERTS.Count(); i++) {
		SurfVertex &u = B.VERTS[i];
		Attribs.add(tri_DWORD(u.x * 256 + 128, u.y * 256 + 128, u.z * 256 + 128), u);
	}

	// Creating instances of BasicMesh:
	BasicMesh *a = new BasicMesh;
	BasicMesh *b = new BasicMesh;
	ConvertMesh(A, a);
	ConvertMesh(B, b);

	SurfMesh R;
	
	// Performing booleans operation:
	a->scale(128);
	b->scale(128);
//	b->randshift(5);
	a->invertNormals();
	b->invertNormals();
	BasicMesh *t = new BasicMesh;
	t->boolean(*a, *b);
	t->scale(1.0f / 128.0f);
	t->select_hard_edges(0.25);
	BasicMesh *r = new BasicMesh;
	t->soften_edges(*r, 5);
	r->invertNormals();
	ConvertMesh(*r, &R);

	delete a;
	delete b;
	delete t;
	delete r;

	// If booleans op is failed, use cutted mesh as a result:
	if(R.TRIS.IsEmpty()) {
		R.Copy(A);
	}

	// In difference we should also invert tris after operation!
	if(BO_DIFFERENCE == Op) {
		R.InvertTris();
	}

	// Prepearing result operand "R" for inmap integration:
	
	m_Shader.Shade(R, I_LOCAL);
	SURF.TexturizeSurface(R);    
	for(i = 0; i < R.VERTS.Count(); i++) {
		SurfVertex &u = R.VERTS[i];
		//cVec2 *pTexCoord = Attribs.get(tri_DWORD(u.x * 256 + 128, u.y * 256 + 128, u.z * 256 + 128));
		SurfVertex* sv=Attribs.get(tri_DWORD(u.x * 256 + 128, u.y * 256 + 128, u.z * 256 + 128));
		if(sv != NULL) {
			//u.TexCoord() = sv->TexCoord();
			u=*sv;
			//u.Color = 0xffff0000;
		}else{
            u.u=FLT_MAX;
			u.v=FLT_MAX;
			u.tscale=10000;
			u.bscale=10000;
		}
	}
	for(int q=0;q<3;q++){
		for(int i=0;i<R.TRIS.Count();i+=3){
			int v[3];
			SurfVertex* sv[3];
			R.GetTri(i/3,sv);
			for(int j=0;j<3;j++){
				if(sv[j]->tscale==10000){
					for(int q=0;q<3;q++){
						if(sv[q]->tscale!=10000){
							cVec3 D=sv[j]->Pos();
							int d=cVec3::Distance(sv[q]->Pos(),D);
							if(d<sv[j]->bscale){
								sv[j]->TexCoord()=SurfMapper::ExtrapolateUV(*sv[q],D);
								sv[j]->bscale=d;
							}
						}
					}
				}
			}	
		}
		for(int i=0;i<R.TRIS.Count();i+=3){
			int v[3];
			SurfVertex* sv[3];
			R.GetTri(i/3,sv);
			for(int j=0;j<3;j++){
				if(sv[j]->tscale==10000 && sv[j]->bscale!=10000){
					sv[j]->tscale=9999;                    					
				}
			}	
		}
	}
	
	for(int i=0;i<R.TRIS.Count();i+=3){
		int v[3];
		SurfVertex* sv[3];
		R.GetTri(i/3,sv);
		for(int j=0;j<3;j++){
			if(sv[j]->bscale==10000){
				SurfMapper::CalcUV(*sv[j]);
			}
		}	
	}
	// Summing operand "R":
	Sum(R);
	SurfRef RefC;
	R.GetRefAll(RefC, SR_TRIS);
	RefC.FillMapOfHeights();

	//R.CalcNormals();

	// Updating quads:
	QUADS.Compile();
	QUADS.Update(CutRef);	
}


//	Bsm.WeldVertices();
//	cRawMesh Brm;
//	Bsm.ToRawMesh(Brm);
	// Asm && Arm:
//	SurfVertex Centre;
//	int Radius;
//	Bsm.CalcBoundSphere(Centre, Radius);
//	SurfRef CutRef;
//	SurfaceUndo::SaveSurfaceRect(cVec2(Centre.x-Radius-100,Centre.y-Radius-100),cVec2(Centre.x+Radius+100,Centre.y+Radius+100),true,false);
//	CutRefCylinder(CutRef, Centre, Radius+80, SR_ALL);	
//	SurfMesh Asm;
//	SurfRef Ref(CutRef);	
//	Ref.ToSurfMesh(Asm);
//	Asm.WeldVertices(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	if(BO_DIFFERENCE == Op) Asm.InvertTris();
//	cRawMesh Arm;
//	Asm.ToRawMesh(Arm);
    //preserving the matherial
/*    int N=Bsm.VERTS.Count();
    uni_hash<DWORD,tri_DWORD> MTL;
    for(int i=0;i<N;i++){
        SurfVertex& sv=Bsm.VERTS[i];
        MTL.add_once(tri_DWORD(sv.x*256+128,sv.y*256+128,sv.z*256+128),_DWORD(sv.SurfaceType));
    }

	N = Brm.GetPolyCount();
	int index = 0;
	while(index < Brm.GetRaw().Count()) {
		const int Deg = Brm.GetRaw()[index][0];
		int &idMtl = Brm.GetRaw()[index][1];
		const int i0 = Brm.GetRaw()[index + 1][0];
		const int i1 = Brm.GetRaw()[index + 2][0];
		const int i2 = Brm.GetRaw()[index + 3][0];
		const cVec3 p0 = Brm.GetPositions()[i0];
		const cVec3 p1 = Brm.GetPositions()[i1];
		const cVec3 p2 = Brm.GetPositions()[i2];
        DWORD *M1 = MTL.get(tri_DWORD(p0.x * 256 + 128, p0.z * 256 + 128, p0.y * 256 + 128));
        DWORD *M2 = MTL.get(tri_DWORD(p1.x * 256 + 128, p1.z * 256 + 128, p1.y * 256 + 128));
        DWORD *M3 = MTL.get(tri_DWORD(p2.x * 256 + 128, p2.z * 256 + 128, p2.y * 256 + 128));
        if(M1 && M2 && M2 && ( *M1==*M2 && *M1==*M3) ){
			idMtl = *M1;
        } else {
			idMtl = 0;
        }
		index += Deg + 1;
	}
    MTL.reset();*/

//    cRawMesh Crm;

//    BasicMesh* m1=new BasicMesh;
//    BasicMesh* m2=new BasicMesh;
//    BasicMesh* ds=new BasicMesh;
//	BasicMesh* ds2=new BasicMesh;
    
//    ConvertMesh(Arm,m1);
//	m1->WriteToPly("!arg1.ply");
//    ConvertMesh(Brm,m2);
//	m2->WriteToPly("!arg2.ply");
//    m1->scale(128);
//    m2->scale(128);
//    m2->randshift(5);
//    m1->invertNormals();
//    m2->invertNormals();
//    int n01=getNmtl(m1);
//    int n02=getNmtl(m2);
//    ds->boolean(*m1,*m2);
//    int n03=getNmtl(ds);
	//ds->WriteToPly("!res.ply");
//    ds->scale(1/128.0f);
//    ds->select_hard_edges(0.25);
//	ds->soften_edges(*ds2,5);
//    int n04=getNmtl(ds2);
//    ds2->invertNormals();
//    ConvertMesh(*ds2,&Crm);


//    delete(m1);
  //  delete(m2);
//    delete(ds);	
//    delete(ds2);	

	// Sum:
/*	SurfMesh Csm;
	if(!Crm.GetRaw().Count() || !Crm.GetPositions().Count()) {
		Csm.Copy(Asm);
	} else {
		Csm.CopyFromRawMesh(Crm);
	}*/
//	if(BO_DIFFERENCE == Op) Csm.InvertTris();
//	Csm.CalcNormals();
//	m_Shader.Shade(Csm, I_LOCAL);
//	SURF.TexturizeSurface(Csm);
//	Csm.GetRefAll(Ref, SR_TRIS);
//	Ref.FillMapOfHeights();
//	Sum(Csm);
//	QUADS.Compile();
//	QUADS.Update(CutRef);

//-----------------------------------------------------------------------------
// SurfMap::Create : void(int, int, int, int, bool)
//-----------------------------------------------------------------------------
void SurfMap::Create(int Lx, int Ly, int Dim, int Height, bool Noisy) {
	Free();
	cRawMesh *rm = cRawMesh::GenPlaneHexagonal(Lx, Ly, Dim, Noisy);
	for(int i = 0; i < rm->GetPositions().Count(); i++) {
		cMath::Swap(rm->GetPositions()[i].y, rm->GetPositions()[i].z);
	}

	((SurfMesh*)this)->CopyFromRawMesh(*rm);
	InvertTris();
	for(int i = 0; i < VERTS.Count(); i++) {
		SurfVertex &r = g_UniversalMap.VERTS[i];
		r.Color = 0xff646464;
		r.Luminance = 0;
		r.z = Height;
		r.nx=0;
		r.ny=0;
		r.nz=127;
        r.Rotation = 0;
        r.Scale = 64;
        r.dU = 0;
        r.dV = 0;
		r.nFacture=0;
		r.SurfaceType = 0;
		r.RaceIndex = 0;
		cVec2 uv = SurfMapper::CalcUV(r,cVec3(r.x,r.y,r.z));
		r.u = uv.x;
		r.v = uv.y;
	}
	QUADS.Init();

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/*	int oi[3];
	for(int iQ = 0; iQ < QUADS.Count(); iQ++) {
		const Quad &q = QUADS.GetAt(iQ);
		for(int iTri = 0; iTri < q.idTris.Count(); iTri++) {
			int nTri = q.idTris[iTri];
			GetTri(nTri, oi);
			for(int k = 0; k < 3; k++) {
				SurfVertex &r = VERTS[oi[k]];
				r.z = 10 * iQ;
			}
		}
	}*/ // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	QUADS.FillMapOfHeights();
	CalcNormals();

	void InitLayersSystem();
	InitLayersSystem();

	m_Shader.Shade(*this, I_LOCAL);
}

//-------------------------------------------------------------------------------------------------
// SurfMap::PickTri : int(const SurfVertex &, const SurfVertex &, const SurfVertex &)
//-------------------------------------------------------------------------------------------------
int SurfMap::PickTri(const SurfVertex &t0, const SurfVertex &t1, const SurfVertex &t2) {
	const int x1 = t0.x, x2 = t1.x, x3 = t2.x;
	const int y1 = t0.y, y2 = t1.y, y3 = t2.y;

	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;

	// Constant parts of half - space functions:
	int C1 = Dy12 * x1 - Dx12 * y1;
	int C2 = Dy23 * x2 - Dx23 * y2;
	int C3 = Dy31 * x3 - Dx31 * y3;

	int S1 = cMath::Sign(C1);
	int S2 = cMath::Sign(C2);
	int S3 = cMath::Sign(C3);

	// Edge t0 - t1:
	if(S1 == 0 && ((S2 < 0 && S3 < 0) || (S2 > 0 && S3 > 0))) {
		return (t0.z + t1.z) / 2;
	}
	// Edge t1 - t2:
	if(S2 == 0 && ((S1 < 0 && S3 < 0) || (S1 > 0 && S3 > 0))) {
		return (t1.z + t2.z) / 2;
	}
	// Edge t0 - t2:
	if(S3 == 0 && ((S1 < 0 && S2 < 0) || (S1 > 0 && S2 > 0))) {
		return (t0.z + t2.z) / 2;
	}
	// Inside tri:
	if((S1 < 0 && S2 < 0 && S3 < 0) || (S1 > 0 && S2 > 0 && S3 > 0)) {
		return (t0.z + t1.z + t2.z) / 3;
	}
	return 0;
}

double RayTri(const dVector &RayOrig, const dVector &RayDir, const dVector &t0, const dVector &t1, const dVector &t2) {
	const double Epsilon = 0.000001;

	dVector e1 = t1 - t0;
	dVector e2 = t2 - t0;

	dVector p = RayDir.Cross(e2);

	double d = e1.Dot(p);

	dVector to = RayOrig - t0;
	double id = 1.0 / d;

	dVector q = to.Cross(e1);

	if(d > Epsilon) {
		double u = to.Dot(p);
		if(u < 0.0 || u > d) return cMath::DoubleNaN;

		double v = RayDir.Dot(q);
		if(v < 0.0 || u + v > d) return cMath::DoubleNaN;
	} else if(d < -Epsilon) {
		double u = to.Dot(p);
		if(u > 0.0 || u < d) return cMath::DoubleNaN;

		double v = RayDir.Dot(q);
		if(v > 0.0 || u + v < d) return cMath::DoubleNaN;

	} else return cMath::DoubleNaN;

	return e2.Dot(q) * id;
}

//---------------------------------------------------------------------------------------------------
// SurfRef::PickTri : bool(const dVector &, const dVector &, int &, double &) const
//---------------------------------------------------------------------------------------------------
bool SurfRef::PickTri(const dVector &Orig, const dVector &Dir, int &nPickedTri, double &sMin) const {
	if(!idTris.Count()) return false;

	sMin = cMath::DoubleMaxValue;
	SurfVertex t[3];
	nPickedTri = -1;
	for(int iTri = 0; iTri < idTris.Count(); iTri++) {
		int nTri = idTris[iTri];
		pOrig->GetTri(nTri, t);
		double s = RayTri(Orig, Dir, dVector(t[0].x, t[0].y, t[0].z),
			dVector(t[1].x, t[1].y, t[1].z), dVector(t[2].x, t[2].y, t[2].z));
		if(s != cMath::DoubleNaN && s > 0 && s < sMin) {
			sMin = s;
			nPickedTri = nTri;
		}
	}
	if(-1 == nPickedTri) return false;
}

//-----------------------------------------------------------------------------
// SurfMap::PickHeight : bool(const dVector &, float *)
//-----------------------------------------------------------------------------
bool SurfMap::PickHeight(const dVector &Pos, float *pHeight) {
	*pHeight = 0;

	SurfRef Ref;
	QUADS.GetRefAlongRay(Ref, Pos, - dVector::zAxis);
	if(!Ref.idQuads.Count()) return false;

	Ref.Complete(SR_TRIS | SR_VERTS, SR_QUADS);
	if(!Ref.idTris.Count()) return false;

	double sMin;
	int nPickedTri;
	if(!Ref.PickTri(Pos, - dVector::zAxis, nPickedTri, sMin)) return false;

	if(pHeight) {
		*pHeight = Pos.z - sMin;
	}
	return true;
}

//-----------------------------------------------------------------------------------
// SurfMap::Pick : bool(SurfRef &, const dVector &, const dVector &, double *)
//-----------------------------------------------------------------------------------
bool SurfMap::Pick(SurfRef &Ref, const dVector &Orig, const dVector &Dir, double *s) {
	Ref.Free();
	Ref.pOrig = this;
	
	// Ref to quads:
	QUADS.GetRefAlongRay(Ref, Orig, Dir);
	if(!Ref.idQuads.Count()) return false;

	// Ref to potential tris:
	SurfRef pr(Ref);
	pr.Complete(SR_TRIS | SR_VERTS, SR_QUADS);
	if(!pr.idTris.Count()) return false;

	SurfVertex t[3];
	double sMin = cMath::DoubleMaxValue;
	int nPickedTri = -1;
	for(int iTri = 0; iTri < pr.idTris.Count(); iTri++) {
		int nTri = pr.idTris[iTri];
		GetTri(nTri, t);
		double s = RayTri(Orig, Dir, dVector(t[0].x, t[0].y, t[0].z),
			dVector(t[1].x, t[1].y, t[1].z), dVector(t[2].x, t[2].y, t[2].z));
		if(s != cMath::DoubleNaN && s > 0 && s < sMin) {
			sMin = s;
			nPickedTri = nTri;
		}
	}

	if(-1 == nPickedTri) return false;
	if(s) {
		*s = sMin;
	}

	double dMin = cMath::DoubleMaxValue, d;
	int nPickedVert = -1;
	int oi[3];
	GetTri(nPickedTri, oi);
	for(int k = 0; k < 3; k++) {
		const SurfVertex &p = VERTS[oi[k]];
		dVector u(p.x, p.y, p.z);
		d = u.DistToLine(Orig, Dir);
		if(d < dMin) {
			dMin = d;
			nPickedVert = oi[k];
		}
	}

	assert(nPickedVert != -1);

	Ref.idTris.Add(nPickedTri);
	Ref.idVerts.Add(nPickedVert);

	return true;
}

//-----------------------------------------------------------------------------
// SurfMap::Pick : bool(SurfRef &, int, int)
//-----------------------------------------------------------------------------
bool SurfMap::Pick(SurfRef &Ref, int xScreen, int yScreen, cVec3 *TriIntersectionPoint /*= NULL*/) {
	// GetPickRay -> Orig, Dir:
	dVector Orig, Dir;
	Line3D l = ICam->GetPickRay(xScreen, yScreen);
	Vector3D O = l.Orig(), D = l.Dir();
	Orig.Set(O.x, O.y, O.z);
	Dir.Set(D.x, D.y, D.z);
	double Scale;
	const bool f = Pick(Ref, Orig, Dir, &Scale);
	if(TriIntersectionPoint) {
		const dVector ip = Orig + Scale * Dir;
		TriIntersectionPoint->Set((float)ip.x, (float)ip.y, (float)ip.z);
	}
	return f;
}
float GetAnisotropy(const SurfVertex& from,const SurfVertex& to){
	cVec2 uv2=SurfMapper::ExtrapolateUV(from,to.Pos());
	float d1=cVec2::DistanceSq(from.TexCoord(),uv2);
	float d2=cVec2::DistanceSq(from.TexCoord(),to.TexCoord());
	return d1>d2 ? d1/d2 : d2/d1;
}
bool SurfSplitVertex::TriEquals(const SurfSplitVertex &s) {	
	if( s.oi == oi )return true;
	if( s.nFacture != nFacture )return false;
	float ans;
	if(tscale<s.tscale){
		ans=GetAnisotropy(*this,s);		
	}else{
		ans=GetAnisotropy(s,*this);		
	}
	static float maxans=2;
	return ans<maxans;
	/*
	static float diff=0.5;
	if(tscale<s.tscale){
		cVec3 vt(s.x,s.y,s.z);	
		cVec2 uv=SurfMapper::ExtrapolateUV(*this,vt);
		return s.nFacture == nFacture && abs(s.u-uv.x)<diff && abs(s.v-uv.y)<diff;
	}else{
		cVec3 vt(x,y,z);	
		cVec2 uv=SurfMapper::ExtrapolateUV(*((SurfVertex*)&s),vt);
		return s.nFacture == nFacture && abs(u-uv.x)<diff && abs(v-uv.y)<diff;
	}
	*/
}

} // Surface