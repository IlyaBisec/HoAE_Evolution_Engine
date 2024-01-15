#ifndef __Surface__
#define __Surface__

#define SPLITMESH_MAXNFACTURES	160
#include "..\stdheader.h"
#include "..\comms\comms.h"
#include "Shader.h"
#include "MCubes.h"
#include "bitset.h"

void BindManipulators(cVec3 *pScale, cQuat *pRotate, cVec3 *pTranslate);

bool ActivateSurfTool(const char *);
bool IsActiveSurfTool(const char *);
void ClearSurfTool();
void ShowSurfEditor();
void ShowSurfGenerator();
void ResetGroundCache();
void ShowPen(int, int, int, DWORD);
bool UsesBump(int);
int GetFactureBump(int, float &, float &);
int GetFacture(int);
const char * GetFactureName(int);
int GetNFactures();
void GetUVInfo(int id, float &du, float &dv, float &su, float &sv);

extern HWND g_hWnd;
extern bool g_LockArrows;
extern bool g_fZXCKeysAreBlocked;

class SurfCamera : public BaseFunction {
public:
	float xAt, yAt, zAt, Azim, Elev, toEye, toEyeEx;

	SurfCamera();
	SurfCamera(const dVector &At, double Azim, double Elev, double toEye, double toEyeEx);

	class SurfCameraRemove : public BaseFunction {
	public:
		void EvaluateFunction();
		SAVE(SurfCameraRemove);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} Remove;
	
	void EvaluateFunction();
	bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
	bool CheckIfElementInvisible(int, void *, DWORD);

	SAVE(SurfCamera);
	REG_PARENT(BaseFunction);
	REG_AUTO(Remove);
	REG_MEMBER(_float, xAt);
	REG_MEMBER(_float, yAt);
	REG_MEMBER(_float, zAt);
	REG_MEMBER(_float, Azim);
	REG_MEMBER(_float, Elev);
	REG_MEMBER(_float, toEye);
	REG_MEMBER(_float, toEyeEx);
	ENDSAVE;
};

namespace Surface {

class SurfEditor : public BaseClass {
public:
	Delimiter D;
	// TOOLS
	SubSection Tools;

	class SurfaceTool : public BaseFunction {
	public:
		void SetName(const char *pStr);
		void EvaluateFunction();
		SAVE(SurfaceTool);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	private:
		cStr m_ToolName;
	} Colorize, Facturize, Deform, Archway, Scissors, LocalExporter;

	class SurfSmooth : public SurfaceTool {
	public:
		// Args:
		float AMOUNT;
		_str DSPMAP;
		float MAPSCALE;
		float MAPAMOUNT;
		// Params:
		float Amount, pAmount;
		int DspMap, pDspMap;
		float MapScale, pMapScale;
		float MapAmount, pMapAmount;
		Delimiter D;

		void Defaults();
		void Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool CheckIfElementInvisible(int, void *, DWORD);

		SAVE(SurfSmooth);
		REG_PARENT(BaseFunction);
		REG_MEMBER(_float01, Amount);
		REG_AUTO(D);
		REG_ENUM(_index, DspMap, DISPLACEMENTMAPS);
		REG_MEMBER(_float01, MapScale);
		REG_MEMBER(_float01, MapAmount);
		REG_MEMBER(_float01, MAPSCALE);
		REG_MEMBER(_float01, AMOUNT);
		REG_AUTO(DSPMAP);
		REG_MEMBER(_float01, MAPAMOUNT);
		ENDSAVE;
	} Smooth;

	//*************************************************************************
	// ChamferArgs
	//*************************************************************************
	class ChamferArgs : public SurfaceTool {
	public:
		float Threshold;
		float Width;

		float GetThreshold() const { return Threshold; }
		float GetWidth() const { return Width; }

#define SurfEditorChamferArgs_DefThreshold	0.25f
#define SurfEditorChamferArgs_DefWidth		5

		void Defaults() {
			Threshold = SurfEditorChamferArgs_DefThreshold;
			Width = SurfEditorChamferArgs_DefWidth;
		}

		__constructor(ChamferArgs) {
			Defaults();
		}

		SAVE(ChamferArgs);
			REG_PARENT(BaseFunction);

			REG_FSLIDER(Threshold, 0.0f, 1.0f, SurfEditorChamferArgs_DefThreshold);
			REG_FSLIDER(Width, 0.1f, 10.0f, SurfEditorChamferArgs_DefWidth);
		ENDSAVE;
	} Chamfer;

	class SurfRacist : public SurfaceTool {
	public:
		// Args:
		int RACE;
		// Params:
		int Race, pRace;

		void Defaults();
		bool Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool CheckIfElementInvisible(int, void *, DWORD);

		SAVE(SurfRacist);
		REG_PARENT(BaseFunction);
		REG_ENUM(_index, Race, RACES);
		REG_MEMBER(_int, pRace);
		REG_MEMBER(_int, RACE);
		ENDSAVE;
	} Racist;

	class SurfLayer : public SurfaceTool {
	public:
		// Args:
		int LAYER;
		// Params:
		int Layer, pLayer;

		void Defaults();
		bool Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool CheckIfElementInvisible(int, void *, DWORD);

		int SelLayerIndex() const;
		const char * SelLayerName() const;
		DWORD SelLayerColor() const;

		SAVE(SurfLayer);
		REG_PARENT(BaseFunction);
		REG_ENUM(_index, Layer, SURFLAYERS);
		REG_MEMBER(_int, pLayer);
		REG_MEMBER(_int, LAYER);
		ENDSAVE;
	} Layer;

	class SurfTypify : public SurfaceTool {
	public:
		// Args:
		int TYPE;
		// Params:
		int Type, pType;
		
		bool Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool CheckIfElementInvisible(int, void *, DWORD);

		SAVE(SurfTypify);
		REG_PARENT(BaseFunction);
		REG_ENUM(_index, Type, SURFTYPES);
		REG_MEMBER(_int, pType);
		REG_MEMBER(_int, TYPE);
		ENDSAVE;
	} Typify;

	class SurfLocalGenerator : public SurfaceTool {
	public:
		static const int DefPasses = 10;
		void Defaults();
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		int Passes;
		SAVE(SurfLocalGenerator);
		REG_PARENT(BaseFunction);
		REG_ISLIDER(Passes, 1, 10, DefPasses);
		ENDSAVE;
	} LocalGenerator;

	// COMMANDS
	SubSection Commands;

	// CmdRefresh:
	class CmdRefresh : public BaseFunction {
	public:
		int Illumination;
		bool LODs;

		void Defaults();
		void EvaluateFunction();
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool OnChangeMember(BaseClass *, void *, void *, const char *);

		SAVE(CmdRefresh);
		REG_PARENT(BaseFunction);
		REG_ENUM(_index, Illumination, SURFACE_ILLUMINATIONTYPE_ENUM);
		REG_MEMBER(_bool, LODs);
		ENDSAVE;
	} Refresh;

	class CmdSimplify : public BaseFunction {
	public:
		void EvaluateFunction();

		SAVE(CmdSimplify);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} Simplify;

	class CmdFillGenerator : public BaseFunction {
	public:
		void EvaluateFunction();

		SAVE(CmdFillGenerator);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} FillGenerator;

	class CmdCutRect : public BaseFunction {
	public:
		bool Update();
		void Defaults();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool CheckIfElementInvisible(int, void *, DWORD);
		void EvaluateFunction();
		// Args:
		int X0, Y0, X1, Y1;
		// Params:
		float x0, px0, y0, py0, x1, px1, y1, py1;

		SAVE(CmdCutRect);
		REG_PARENT(BaseFunction);
		// Visible:
		REG_MEMBER(_float01, x0);
		REG_MEMBER(_float01, y0);
		REG_MEMBER(_float01, x1);
		REG_MEMBER(_float01, y1);
		// Invisible:
		REG_MEMBER(_float01, px0);
		REG_MEMBER(_float01, py0);
		REG_MEMBER(_float01, px1);
		REG_MEMBER(_float01, py1);
		REG_MEMBER(_int, X0);
		REG_MEMBER(_int, Y0);
		REG_MEMBER(_int, X1);
		REG_MEMBER(_int, Y1);
		ENDSAVE;
	} CutRect;

	// CmdCreateNewMap:
	class CmdCreateNewMap : public BaseFunction {
	public:
		static const int MaxLx = 16384;
		static const int MaxLy = 16384;
		static const int MinCell = 16;
		static const int MaxCell = 144;
		static const int MinBaseHeight = -50;
		static const int MaxBaseHeight = 150;

		void Defaults();
		void Update();
		void EvaluateFunction();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool CheckIfElementInvisible(int, void *, DWORD);
		// Args:
		int Lx, Ly, D, H;
		bool N;
		// Params:
		float xSize, ySize, Cell, BaseHeight, pxSize, pySize, pCell, pBaseHeight;
		bool Noisy, pNoisy;

		SAVE(CmdCreateNewMap);
		REG_PARENT(BaseFunction);
		REG_MEMBER(_float01, xSize);
		REG_MEMBER(_float01, ySize);
		REG_MEMBER(_float01, Cell);
		REG_MEMBER(_float01, BaseHeight);
		REG_MEMBER(_bool, Noisy);
		REG_MEMBER(_int, Lx);
		REG_MEMBER(_int, Ly);
		REG_MEMBER(_int, D);
		REG_MEMBER(_int, H);
		REG_MEMBER(_bool, N);
		ENDSAVE;
	} CreateNewMap;

	// CmdUndo:
	class CmdUndo : public BaseFunction {
	public:
		void EvaluateFunction();
		SAVE(CmdUndo);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} Undo;

	// CmdInvertTris:
	class CmdInvertTris : public BaseFunction {
	public:
		void EvaluateFunction();
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);

		SAVE(CmdInvertTris);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} InvertTris;

	// CmdExport:
	class CmdExport : public BaseFunction {
	public:
		void EvaluateFunction();

		SAVE(CmdExport);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} Export;


	// CmdShift:
	class CmdShift : public BaseFunction {
	public:
//		static const int MinD = - 100;
//		static const int MaxD = 100;

		void Defaults();
//		void Update();
		void EvaluateFunction();
//		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
//		bool CheckIfElementInvisible(int, void *, DWORD);
		// Args:
//		int D;
		// Params:
//		float Delta, pDelta;
		int Delta;

		SAVE(CmdShift);
		REG_PARENT(BaseFunction);
		REG_ISLIDER(Delta, - 100, 100, 0);
//		REG_MEMBER(_float01, Delta);
//		REG_MEMBER(_int, D);
		ENDSAVE;
	} Shift;

	// CmdSaveCamera:
	class CmdSaveCamera : public BaseFunction {
	public:
		ClassArray<SurfCamera> Cameras;
		void EvaluateFunction();
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);

		SAVE(CmdSaveCamera);
		REG_PARENT(BaseFunction);
		REG_AUTO(Cameras);
		ENDSAVE;
	} SaveCamera;

	// OPTIONS
	SubSection Options;
	bool Wireframe;
	bool prevFreeCamera, FreeCamera;
	bool ForceZNear;
	bool ShowMapOfHeights;
	bool ShowNormalInTris;
	bool ShowNormalInVerts;
	bool HideUI, pHideUI;
	bool ShowSurface;
	bool LockSurface;
	bool SubdivideByRace;
	bool InvertBumpOnByRaceSubdivisions;
	bool BooleansSum;

#define SurfEditor_DefRenderAllMapZoomOut 6
	int RenderAllMapZoomOut;

	// SubdivisionOptions:
	class SubdivisionOptions : public BaseFunction {
	public:
		static const int EdgeMaxLenLo = 16;
		static const int EdgeMaxLenHi = 200;
		static const int ScaleMultiplier = 10;
		// Args:
		int EDGEMAXLEN;
		float DOT, SCALE;
		bool ADAPTIVE;
		// Params:
		float Edge, pEdge;
		float Dot, pDot;
		float Scale, pScale;
		bool Adaptive, pAdaptive;

		void Defaults();
		void Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool CheckIfElementInvisible(int, void *, DWORD);
		void EvaluateFunction();

		SAVE(SubdivisionOptions);
		REG_PARENT(BaseFunction);
		REG_MEMBER(_bool, Adaptive);
		REG_MEMBER(_float01, Edge);
		REG_MEMBER(_float01, Dot);
		REG_MEMBER(_float01, Scale);
		REG_MEMBER(_int, EDGEMAXLEN);
		REG_MEMBER(_bool, ADAPTIVE);
		REG_MEMBER(_float, DOT);
		REG_MEMBER(_float, SCALE);
		ENDSAVE;
	} Subdivision;

	// ShaderOptions:
	class ShaderOptions : public BaseFunction {
	public:
		class ShaderReset : public BaseFunction {
		public:
			void EvaluateFunction();
			SAVE(ShaderReset);
			REG_PARENT(BaseFunction);
			ENDSAVE;
		} Reset;

		void Defaults();
		void Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool CheckIfElementInvisible(int, void *, DWORD);

		// Args:
		int aAzim, aElev, aVar, aQtyOfLights, aSeed, aZBufferSide;
		float aAmbientIllumination, aPhoton;
		// Params:
		float Azimuth, pAzimuth, Elevation, pElevation, Var, pVar, AmbientIllumination, pAmbientIllumination,
			Photon, pPhoton, QtyOfLights, pQtyOfLights, Seed, pSeed, ZBufferSide, pZBufferSide;

		SAVE(ShaderOptions);
		REG_PARENT(BaseFunction);
		// RegParams:
		REG_MEMBER(_float01, Azimuth);
		REG_MEMBER(_float01, Elevation);
		REG_MEMBER(_float01, Var);
		REG_MEMBER(_float01, AmbientIllumination);
		REG_MEMBER(_float01, Photon);
		REG_MEMBER(_float01, QtyOfLights);
		REG_MEMBER(_float01, Seed);
		REG_MEMBER(_float01, ZBufferSide);
		REG_AUTO(Reset);
		// RegArgs:
		REG_MEMBER(_int, aAzim);
		REG_MEMBER(_int, aElev);
		REG_MEMBER(_int, aVar);
		REG_MEMBER(_float, aAmbientIllumination);
		REG_MEMBER(_float, aPhoton);
		REG_MEMBER(_int, aQtyOfLights);
		REG_MEMBER(_int, aSeed);
		REG_MEMBER(_int, aZBufferSide);

		ENDSAVE;
	} Shader;

	// LODSOptions:
	class LODSOptions : public BaseFunction {
	public:
		// Args:
		float LodBias;
		// Params:
		float lb, plb;

		void Defaults();
		void Update();
		bool OnChangeMember(BaseClass *, void *, void *, const char *);
		bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
		bool CheckIfElementInvisible(int, void *, DWORD);

		SAVE(LODSOptions);
		REG_PARENT(BaseFunction);
		// RegParams:
		REG_MEMBER(_float01, lb);
		// RegArgs:
		REG_MEMBER(_float, LodBias);
		ENDSAVE;
	} LODS;

	void Load();
	void Update();
	void Defaults() {
		CutRect.Defaults();
		Racist.Defaults();
		Layer.Defaults();
		Smooth.Defaults();
		Chamfer.Defaults();
		LODS.Defaults();
		Colorize.SetName("Colorize");
		Facturize.SetName("Facturize");
		Deform.SetName("Deform");
		Scissors.SetName("Scissors");
		Smooth.SetName("Smooth");
		Chamfer.SetName("Chamfer");
		Archway.SetName("Archway");
		Racist.SetName("Racist");
		Layer.SetName("Layer");
		Typify.SetName("Typify");
		LocalGenerator.SetName("LocalGenerator");
		LocalGenerator.Defaults();
		LocalExporter.SetName("LocalExporter");

		m_fActive = false;
		// Options:
		Wireframe = false;
		prevFreeCamera = FreeCamera = false;
		ForceZNear = false;
		ShowMapOfHeights = false;
		ShowNormalInTris = false;
		ShowNormalInVerts = false;
		pHideUI = HideUI = false;
		ShowSurface = true;
		LockSurface = false;
		BooleansSum = false;
		SubdivideByRace = true;
		InvertBumpOnByRaceSubdivisions = false;

		Refresh.Defaults();
		CreateNewMap.Defaults();
		Subdivision.Defaults();
		Shift.Defaults();
		Shader.Defaults();

		RenderAllMapZoomOut = SurfEditor_DefRenderAllMapZoomOut;
	}

	// TESTS
	SubSection Tests;
	// CmdWindowsList:
	class CmdTestHtml : public BaseFunction {
	public:
		void EvaluateFunction();

		SAVE(CmdTestHtml);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} TestHtml;
	// CmdSite:
	class CmdSite : public BaseFunction {
	public:
		void EvaluateFunction();

		SAVE(CmdSite);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	} Site;

//	ClonesArray<CameraTrack> CameraTracks; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	SAVE(SurfEditor);
	// TOOLS
	REG_AUTO(Tools);
	REG_AUTO(Colorize);
	REG_AUTO(Facturize);
	REG_AUTO(Deform);
	REG_AUTO(Smooth);
	REG_AUTO(Chamfer);
	REG_AUTO(Archway);
	REG_AUTO(Racist);
	REG_AUTO(Layer);
	REG_AUTO(Typify);
	REG_AUTO(Scissors);
	REG_AUTO(LocalGenerator);
	REG_AUTO(LocalExporter);
	// COMMANDS
	REG_AUTO(Commands);
	REG_AUTO(CreateNewMap);
	REG_AUTO(InvertTris);
	REG_AUTO(Undo);
	REG_AUTO(Refresh);
	REG_AUTO(Simplify);
	REG_AUTO(Shift);
	REG_AUTO(SaveCamera);
	REG_AUTO(Export);
	REG_AUTO(FillGenerator);
	REG_AUTO(CutRect);
	// OPTIONS
	REG_AUTO(Options);
	REG_MEMBER(_bool, FreeCamera);
	REG_MEMBER(_bool, ForceZNear);
	REG_AUTO(D);
	REG_MEMBER(_bool, Wireframe);
	REG_AUTO(D);
	REG_MEMBER(_bool, ShowMapOfHeights);
	REG_MEMBER(_bool, ShowNormalInTris);
	REG_MEMBER(_bool, ShowNormalInVerts);
	REG_AUTO(D);
	REG_MEMBER(_bool, HideUI);
	REG_AUTO(D);
	REG_AUTO(Subdivision);
	REG_AUTO(Shader);
	REG_AUTO(D);
	REG_MEMBER(_bool, ShowSurface);
	REG_MEMBER(_bool, LockSurface);
	REG_MEMBER(_bool, SubdivideByRace);
	REG_MEMBER(_bool, InvertBumpOnByRaceSubdivisions);
	REG_AUTO(D);
	REG_AUTO(LODS);
	REG_MEMBER(_bool, BooleansSum);
	REG_AUTO(D);
	REG_ISLIDER(RenderAllMapZoomOut, 2, 10, SurfEditor_DefRenderAllMapZoomOut);

	// TESTS
	REG_AUTO(Tests);
	REG_AUTO(TestHtml);
	REG_AUTO(Site);
//	REG_AUTO(CameraTracks); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	ENDSAVE;
	bool OnChangeMember(BaseClass *, void *, void *, const char *);
	bool m_fActive;
};

struct uv_pair {
	float u;
	float v;
	DWORD specular;
};

class SurfMapper;

typedef enum { MT_VERTICAL = 1 << 0, MT_HORIZONTAL = 1 << 1, MT_CUBIC = MT_VERTICAL | MT_HORIZONTAL } eMappingType;

struct SurfVertex {
	SurfVertex();
	SurfVertex(short, short, short = 0);
	SurfVertex(short, short, short, DWORD C);
	SurfVertex(short, short, short, DWORD C, byte L);
	SurfVertex(short, short, short, DWORD C, byte L, byte F);

	SurfVertex & Set(short, short, short);
	SurfVertex & Set(short, short, short, DWORD);
	SurfVertex & Set(short, short, short, DWORD C, byte L);
	
	cVec3 GetN() const { return cVec3(nx,ny,nz); }
	cVec3 GetT() const { return cVec3(nx,ny,nz); }
	cVec3 GetB() const { return cVec3(nx,ny,nz); }	

	cVec3& Pos() { return *(cVec3*)(&x); }
	const cVec3& Pos() const { return *(cVec3*)(&x); }
	cVec2& TexCoord() { return *(cVec2*)(&u); }
	const cVec2& TexCoord() const { return *(cVec2*)(&u); }

	bool Equals(const SurfVertex &) const;
	bool operator == (const SurfVertex &) const;

	DWORD CalcDiffuseColor() const;
	DWORD CalcSpecularColor() const;
	eMappingType MappingType() const;

    /*
	short x;
	short y;
	short z;
    */
	float x;
	float y;
	float z;

	DWORD Color;
    byte  Rotation;
    byte  Scale;
    byte  dU;
    byte  dV;
    byte  nFacture;
	byte  Luminance;
	char  nx;
	char  ny;
	char  nz;
	unsigned SurfaceType:4;
	unsigned RaceIndex:4;
	
	float u;
	float v;
		
	//temp fields

	char tx;
	char ty;
	char tz;
	short tscale;

	char bx;
	char by;
	char bz;
	short bscale;

	static SurfMapper GetMapper(int nFacture);
	static int GetFactureID(int nFacture);
	static int GetFactureID2(int nFacture);
	static int GetFx(int nFacture);
	static const SurfVertex Origin;
	static const SurfVertex MinValue;
	static const SurfVertex MaxValue;
	static const char NormalLen = 127;
	void UpdateBoundBox(SurfVertex &bbMin, SurfVertex &bbMax) const;
	bool InCylinder(const SurfVertex &Center, int Radius) const;
	bool InSphere(const SurfVertex &Center, int Radius) const;
	bool InRect(int x0, int y0, int x1, int y1) const;

	cVec3 ToVector3() const;
};

// SurfVertex()
inline SurfVertex::SurfVertex()
: x(0), y(0), z(0), nFacture(0), Rotation(0), Scale(64), dU(0), dV(0), Color(0xFFFFFFFF) {}

// SurfVertex(short, short, short)
inline SurfVertex::SurfVertex(short a, short b, short c)
: x(a), y(b), z(c), nFacture(0), Rotation(0), Scale(64), dU(0), dV(0), Color(0xFFFFFFFF) {}

// SurfVertex(short, short, short, DWORD)
inline SurfVertex::SurfVertex(short a, short b, short c, DWORD C)
: x(a), y(b), z(c), nFacture(0), Rotation(0), Scale(64), dU(0), dV(0), Color(C) {}

// SurfVertex(short, short, short, DWORD, byte)
inline SurfVertex::SurfVertex(short a, short b, short c, DWORD C, byte L)
: x(a), y(b), z(c), nFacture(0), Rotation(0), Scale(64), dU(0), dV(0), Color(C), Luminance(L) {}

// SurfVertex(short, short, short, DWORD, byte, byte)
inline SurfVertex::SurfVertex(short a, short b, short c, DWORD C, byte L, byte F)
: x(a), y(b), z(c), nFacture(F), Rotation(0), Scale(64), dU(0), dV(0), Color(C), Luminance(L) {}

// SurfVertex::Set(short, short, short)
inline SurfVertex & SurfVertex::Set(short a, short b, short c = 0) {
	x = a, y = b, z = c; return *this;
} // SurfVertex::Set(short, short, short)

// SurfVertex::Set(short, short, short, DWORD)
inline SurfVertex & SurfVertex::Set(short a, short b, short c, DWORD C) {
	x = a, y = b, z = c;
	Color = C;
	return *this;
} // SurfVertex::Set(short, short, short, DWORD)

// SurfVertex::Set(short, short, short, DWORD, byte)
inline SurfVertex & SurfVertex::Set(short a, short b, short c, DWORD C, byte L) {
	x = a, y = b, z = c;
	Color = C, Luminance = L;
	return *this;
} // SurfVertex::Set(short, short, short, DWORD, byte)

// SurfVertex::Equals
inline bool SurfVertex::Equals(const SurfVertex &v) const {
	return v.x == x && v.y == y && v.z == z;
}

// SurfVertex::operator ==
inline bool SurfVertex::operator == (const SurfVertex &v) const {
	return Equals(v);
}

class SurfVertices : public cList<SurfVertex> {
public:
	void CalcBoundBox(SurfVertex &bbMin, SurfVertex &bbMax) const;
	void Typify(int idType);
};

//-----------------------------------------------------------------------------
// SurfVertex::InCylinder : bool(const SurfVertex &, int) const
//-----------------------------------------------------------------------------
inline bool SurfVertex::InCylinder(const SurfVertex &Center, int Radius) const {
	int dx = x - Center.x;
	int dy = y - Center.y;
	return cMath::Square(dx) + cMath::Square(dy) <= cMath::Square(Radius);
}

//-----------------------------------------------------------------------------
// SurfVertex::InSphere : bool(const SurfVertex &, int) const
//-----------------------------------------------------------------------------
inline bool SurfVertex::InSphere(const SurfVertex &Center, int Radius) const {
	int dx = x - Center.x;
	int dy = y - Center.y;
	int dz = z - Center.z;
	return cMath::Square(dx) + cMath::Square(dy) + cMath::Square(dz) <= cMath::Square(Radius);
}

//-----------------------------------------------------------------------------
// SurfVertex::InRect : bool(int, int, int, int) const
//-----------------------------------------------------------------------------
inline bool SurfVertex::InRect(int x0, int y0, int x1, int y1) const {
	// *********     * - border is included
	// *       *
	// *       *
	// *********
	return x >= x0 && x <= x1 && y >= y0 && y <= y1;
}

// SurfVertex::ToVector3 : cVec3() const
inline cVec3 SurfVertex::ToVector3() const {
	return cVec3(x, y, z);
}
struct SurfSplitVertex;
class SurfMapper {
public:
	SurfMapper() {}
	SurfMapper(int nFacture);
	static DWORD CalcMappingCode(const SurfVertex &);
	static DWORD CalcMappingCode(const dVector &Normal);
	uv_pair  CalcMappingUV(const SurfVertex &, DWORD) const;
	uv_pair  CalcMappingUV(const float, const float, const float, DWORD, Vector3D,const SurfSplitVertex&) const;	

	static cVec2 CalcUV(SurfVertex& v,cVec3 pos);
	static void  CalcUV(SurfVertex& v);
	static cVec2 ExtrapolateUV(const SurfVertex& v,cVec3 pos);

	float uScale, vScale;
	int fWidth, fHeight;

	static const int k = 1; 
	// | --- 1 --- |
	// *************---
	// *           * |
	// *           * |
	// *           * k
	// *           * |
	// *           * |
	// *************---

	enum MappingCodes {
		MC_BACK = 0,
		MC_LEFT = 1,
		MC_FRONT = 2,
		MC_RIGHT = 3,
		MC_TOP = 4,
		MC_BOTTOM = 5
	};
};
struct SurfSplitVertex : SurfVertex {
    int oi;
    bool Alpha;
    DWORD MappingCode;
	//SurfSplitVertex* s_orig;
	DWORD src_vindex;
	float newU;
	float newV;
	float DistToSrc;
    bool TriEquals(const SurfSplitVertex &s);
    inline bool Equals(const SurfSplitVertex &s) {
        return s.x == x && s.y == y && s.z == z && s.MappingCode == MappingCode && s.Alpha == Alpha;
    }
};
inline int SurfVertex::GetFactureID(int nFacture) {	
	return GetFacture(nFacture);
}

inline int SurfVertex::GetFactureID2(int nFacture) {
	float x;
	static int nm=IRS->GetTextureID("empty_nm.dds");
	if(UsesBump(nFacture)) return GetFactureBump(nFacture, x, x);
	return nm;
}

class SurfMesh;

class BaseSurfaceFunction {
public:
	virtual void ModifyVertex(SurfVertex &) = 0;
	virtual bool ShiftVertex(SurfVertex &) = 0;
};

typedef enum { SR_QUADS = 1 << 0, SR_VERTS = 1 << 1, SR_TRIS = 1 << 2, SR_ALL = SR_QUADS | SR_VERTS | SR_TRIS } eSurfRef;

struct SurfNode {
	int n;
	cVec3 u;
	cVec3 t;
	cVec3 b;
	int nt;
	bool f;	
	SurfNode() : n(0), f(false), t(0,0,0), b(0,0,0), nt(0) {}
};
typedef cList<SurfNode> SurfNodes;

typedef enum { SFX_DEFAULT, SFX_WIRE, SFX_BUMP, SFX_GRIDSTAGE1, SFX_GRIDSTAGE2, SFX_AUX,
	SFX_SAMPLES, SFX_SWATCHSTAGE1, SFX_SWATCHSTAGE2, SFX_BOOLEANS } eSurfFx;
int SurfFx(eSurfFx);

struct SubdivideArgs {
	typedef enum { C_EDGELEN, C_DOT } eCriterion;
	SubdivideArgs();
	void ReadFromSurfEditor();
	eCriterion Criterion;
	bool fAddRefToQuads;
	int EdgeMaxLen; // Used when Criterion == C_EDGELEN
	float Dot, Scale; // Used when Criterion == C_DOT
};

struct SurfEdge {
	int e0, e1, f0, f1;
	SurfEdge() : e0(-1), e1(-1), f0(-1), f1(-1) {}
	bool Equals(const SurfEdge &Edge) const {
		return Edge.e0 == e0 && Edge.e1 == e1;
	}
};
typedef cList<SurfEdge> SurfEdges;


class SurfRef {
public:
	DWORDS idQuads, idVerts, idTris;
	SurfMesh *pOrig;

	SurfRef() {}
	SurfRef & Copy(const SurfRef &Src);
	SurfRef(const SurfRef &Src);	
	SurfRef(SurfMesh *pOrig);
	void Free();
	~SurfRef() { Free(); }

	void CalcBoundBox(eSurfRef By, SurfVertex &bbMin, SurfVertex &bbMax) const;
	void CalcBoundSphere(eSurfRef By, SurfVertex &Centre, int &Radius) const;
	void Complete(DWORD What, eSurfRef By);
	void ToSurfMesh(SurfMesh &Mesh, eSurfRef By = SR_TRIS);
	void FillMapOfHeights(int MaxHeight = cMath::IntMaxValue) const; // by idTris
	void SmoothColors(); // by idTris
	void SpreadTo(SurfRef &Ref, int idOrigVert, bool UseSurfaceType);// const; // by idTris
	void Relax(double Amount = 0.3, int Iterations = 1, bool HoldBoundaryPoints = true); // by idTris
	void UpdateNormals(bool RemakeUV=true); // by idTris	
	bool Subdivide(const SubdivideArgs &Args = SubdivideArgs()); // by idTris
	void SubdivideByRace();
	void SubdivideByRaceWithDupVerts();
	void Draw(eSurfFx SFX = SFX_DEFAULT) const; // by idTris
	void RemoveUnreferredTris(); // by idTris

	bool GatherNodesBoundaryStats(SurfNodes &Nodes); // by idVerts
	void GenLOD(DWORDS &Tris, int nLod); // by idTris, fills idVerts
	/* private */ bool GatherEdges(SurfEdges &Edges, DWORDS *pEdgeTris = NULL); // by idTris
	bool PickTri(const dVector &Orig, const dVector &Dir, int &nPickedTri, double &sMin) const; // by idTris
private:
	int SubdivideEdgeMidPoint(const SurfVertex &e0, const SurfVertex &e1, const int EdgeMaxLen, SurfVertex &c);
	int SubdivideEdgeNormal(const SurfVertex &e0, const SurfVertex &e1, const double Dot, const double Scale, SurfVertex &c);
//	int SubdivideEdgeByRace(const SurfVertex &e0, const SurfVertex &e1, SurfVertex &c);
	static int SubdivideEdgeByRace(const SurfVertex &e0, const SurfVertex &e1, SurfVertex &c0, SurfVertex &c1);
	static int CalcTHMapIndex(int x, int y);
	static void Tri2THMap(const SurfVertex &, const SurfVertex &, const SurfVertex &, int MaxHeight);
};
class SurfMesh {
public:
	SurfVertices	VERTS;
	DWORDS			TRIS;
	UnlimitedBitset BrokenTRIS;

	SurfMesh() {}
	SurfMesh & Copy(const SurfMesh &);
	SurfMesh(const SurfMesh &);
	void Free();
	~SurfMesh() { Free(); }

	void Relax(double Amount = 0.5, int Iterations = 2, float Threshold = 0.5, bool HoldBoundaryVerts = false);
	bool Subdivide(const SubdivideArgs &Args = SubdivideArgs());
	void WeldVertices();
	void GetRefAll(SurfRef &Ref, DWORD What);
	void Draw(eSurfFx SFX = (eSurfFx)-1);
	void Sum(const SurfMesh &);
	void InvertTris();
	void GenPlane(int xLen, int yLen, int Cell);
//	void GenHexagonalPlane(int xLen, int yLen, int Cell, bool Noisy);
	void GenRound(const SurfVertex &Centre, int Radius, int dAngle);
	void CalcNormals();

	int NVerts() const;
	int NTris() const;

	void GetTri(int nTri, int *i3) const;
	void GetTri(int nTri, SurfVertex *t3) const;
	void GetTri(int nTri, int &i0, int &i1, int &i2) const;
	void GetTri(int nTri, SurfVertex &t0, SurfVertex &t1, SurfVertex &t2) const;
	void GetTri(int nTri, SurfVertex &Center) const;
	void GetTri(int nTri, dVector &Normal) const;
	void GetTri(int nTri, cVec3 &Normal) const;
	void GetTri(int nTri, cVec3 &Normal, cVec3 &Tangent, cVec3 &Binormal) const;
	void GetTri(int nTri, SurfVertex **p3);

	int AddTri(int i0, int i1, int i2);

	void CalcBoundBox(SurfVertex &bbMin, SurfVertex &bbMax) const;
	bool GatherVertsBoundaryStats(cBitMap &BoundMap);
	bool GatherEdges(SurfEdges &Edges, DWORDS *pEdgeTris = NULL);

	void CalcBoundSphere(SurfVertex &Centre, int &Radius) const;
	void ApplyMCubes(int CellSize, int Nx, int Ny, BaseSurfaceFunction *Func);
	void ApproximateGenerator();

	void SetFacture(int nFacture);
	bool ToRawMesh(cRawMesh &rm) const;
	bool CopyFromRawMesh(const cRawMesh &rm);
};

extern SurfEditor g_SurfEditor;

//-----------------------------------------------------------------------------
// SurfVertex::GetFx : int(int)
//-----------------------------------------------------------------------------
inline int SurfVertex::GetFx(int nFacture) {
	return UsesBump(nFacture) ? SurfFx(SFX_BUMP) :
	(g_SurfEditor.Wireframe ? SurfFx(SFX_WIRE) : SurfFx(SFX_DEFAULT));
}

//-----------------------------------------------------------------------------
// NVerts : int() const
//-----------------------------------------------------------------------------
inline int SurfMesh::NVerts() const {
	return VERTS.Count();
}

//-----------------------------------------------------------------------------
// NTris : int() const
//-----------------------------------------------------------------------------
inline int SurfMesh::NTris() const {
	return TRIS.Count() / 3;
}

//-----------------------------------------------------------------------------
// GetTri: void(int, int *) const
//-----------------------------------------------------------------------------
inline void SurfMesh::GetTri(int nTri, int *i3) const {
	int ii0 = 3 * nTri;
	for(int k = 0; k < 3; k++) {
		i3[k] = TRIS[ii0 + k];
	}
}

//-----------------------------------------------------------------------------
// GetTri : void(int, SurfVertex **)
//-----------------------------------------------------------------------------
inline void SurfMesh::GetTri(int nTri, SurfVertex **p3) {
	int i[3];
	GetTri(nTri, i);
	for(int k = 0; k < 3; k++) {
		p3[k] = &VERTS[i[k]];
	}
}

//-----------------------------------------------------------------------------
// GetTri : void(int, SurfVertex *) const
//-----------------------------------------------------------------------------
inline void SurfMesh::GetTri(int nTri, SurfVertex *t3) const {
	int i[3];
	GetTri(nTri, i);
	for(int k = 0; k < 3; k++) {
		t3[k] = VERTS[i[k]];
	}
}

//-----------------------------------------------------------------------------
// GetTri : void(int, SurfVertex &) const
//-----------------------------------------------------------------------------
inline void SurfMesh::GetTri(int nTri, SurfVertex &Center) const {
	SurfVertex t0, t1, t2;
	GetTri(nTri, t0, t1, t2);
	int cx = (t0.x + t1.x + t2.x) / 3;
	int cy = (t0.y + t1.y + t2.y) / 3;
	int cz = (t0.z + t1.z + t2.z) / 3;
	Center.Set(cx, cy, cz);
}

//-----------------------------------------------------------------------------
// GetTri : void(int, int &, int &, int &) const
//-----------------------------------------------------------------------------
inline void SurfMesh::GetTri(int nTri, int &i0, int &i1, int &i2) const {
	int ii0 = 3 * nTri;
	i0 = TRIS[ii0], i1 = TRIS[ii0 + 1], i2 = TRIS[ii0 + 2];
}

//--------------------------------------------------------------------------------------------
// GetTri : void(int, SurfVertex &, SurfVertex &, SurfVertex &) const
//--------------------------------------------------------------------------------------------
inline void SurfMesh::GetTri(int nTri, SurfVertex &t0, SurfVertex &t1, SurfVertex &t2) const {
	int i0, i1, i2;
	GetTri(nTri, i0, i1, i2);
	t0 = VERTS[i0], t1 = VERTS[i1], t2 = VERTS[i2];
}

//-----------------------------------------------------------------------------
// AddTri : int(int, int, int)
//-----------------------------------------------------------------------------
inline int SurfMesh::AddTri(int i0, int i1, int i2) {
	TRIS.Add(i0);
	TRIS.Add(i1);
	TRIS.Add(i2);
	return NTris() - 1;
}

typedef enum { BO_UNION, BO_DIFFERENCE } eBooleansOp;

class SurfMap : public SurfMesh {
public:
	SurfMap *m_pBackUp;
	SurfShader m_Shader;

	class Quad : public SurfRef {
	public:
		Quad() {
			nLod = 0;
		}
		~Quad() { Free(); }
		Quad(SurfMesh *pOrig) { this->pOrig = pOrig; }
		void Update();
		SurfVertex m_Centre;
		int m_Radius;
		void Free();
		Quad & Copy(const Quad &Src);
		Quad & operator = (const Quad &);
		static const int NLODS = 3;
		DWORDS LODS[NLODS];
		int nLod;
	};

	class SurfQuads : public cList<Quad> {
		friend SurfMap;
	public:
		SurfQuads() {
			m_CurPass = -1;
		}
		void Init();
		int AddRef(int nTri);
		void CutRef(SurfRef &); // by idTris, looks only in idQuads (frees idQuads since it is no longer valid)
		void GetRefRect(SurfRef &Ref, int x0, int y0, int x1, int y1);
		void GetRefCylinder(SurfRef &Ref, const SurfVertex &Center, int Radius);
		void GetRefAlongRay(SurfRef &Ref, const dVector &Orig, const dVector &Dir);
		void GetRefAroundCenter(SurfRef &Ref, const SurfVertex &Center, int Radius = 0);
		cVec3 CalcCollisionForce(const cVec3 &Center, const float Radius, const float k);
		void CalcRectOf(const SurfRef &Ref, int &x0, int &y0, int &x1, int &y1) const;
		int CalcLodOf(const SurfRef &Ref) const; // by idQuads
		void Update(const SurfRef &Ref); // by idQuads
		void GetRefAll(SurfRef &Ref) const;
		void FillMapOfHeights() const;
		void SmoothColors();
		void Compile();
		SurfQuads & Copy(const SurfQuads &Src);
		bool fUseLods;
		void RefreshLods();
		int BeginPassedLODsGen();
		bool LODsOnePass();
	private:
		void CalcQuadRect(int nQ, int &Qx0, int &Qy0, int &Qx1, int &Qy1) const;
		SurfMesh *pOrig;
		int QuadsNx, QuadsNy, m_CurPass;
		static const int QuadDim = 256;
	} QUADS;

	SurfMap() {

		QUADS.pOrig = this;
	}

	SurfMap(const SurfShader &Shader, SurfMap *pBackUp = NULL)
	{
		QUADS.pOrig = this;
		QUADS.fUseLods = false;
		m_Shader = Shader;
		m_pBackUp = pBackUp;
	}

	static short THMapZ(int MapCoordX, int MapCoordY);
	void Create(int Lx = 16384, int Ly = 16384, int Dim = 80, int Height = 50, bool Noisy = true);
	void CutRect(int x0, int y0, int x1, int y1);
	void BackUp();
	void Undo();

	SurfMap & Copy(const SurfMap &);
	void Booleans(SurfMesh &, eBooleansOp);
	void BooleansSum(SurfMesh &);
	void Sum(const SurfMesh &);
	void Free();
	~SurfMap() { Free(); }
	void GetRefCylinder(SurfRef &Ref, const SurfVertex &Center, int Radius, DWORD What = SR_ALL);
	void GetRefSphere(SurfRef &Ref, const SurfVertex &Center, int Radius, DWORD What = SR_ALL);
	void CutRefCylinder(SurfRef &Ref, const SurfVertex &Center, int Radius, DWORD What = SR_ALL);
	void CutRefAlongSurfLayer(SurfRef &Ref, int idLayer, DWORD What = SR_ALL);
	void GetRefAlongSurfLayer(SurfRef &Ref, int idLayer, DWORD What = SR_ALL);
	void GetRefRect(SurfRef &Ref, int x0, int y0, int x1, int y1, DWORD What = SR_ALL);
	void FillMapOfHeights();

	bool Pick(SurfRef &Ref, const dVector &Orig, const dVector &Dir, double *s = NULL);
	bool Pick(SurfRef &Ref, int xScreen, int yScreen, cVec3 *TriIntersectionPoint = NULL);
	bool PickHeight(const dVector &Pos, float *pHeight);
private:
	typedef enum { RA_GET, RA_CUT } eRefAction;
	typedef enum { RT_CYLINDER, RT_SPHERE, RT_RECT, RT_LAYER } eRefType;
	struct QueryRefArgs {
		eRefType Type;
		eRefAction Action;
		SurfVertex Center;
		int Radius, x0, y0, x1, y1;
		int idLayer;
	};
	void QueryRef(SurfRef &Ref, QueryRefArgs Args, DWORD What);
	static int PickTri(const SurfVertex &, const SurfVertex &, const SurfVertex &);
};

// InRect
inline bool InRect(int x0, int y0, int x1, int y1, int xMouse, int yMouse) {
	// *********     * - included
	// *       .     . - excluded
	// *       .
	// *........
	return xMouse >= x0 && xMouse < x1 && yMouse >= y0 && yMouse < y1;
}

} // Surface

class SurfArchway : public BaseClass {
public:
	int cx, cy, h, r;

	SurfArchway();
	SurfArchway(int cx, int cy, int h, int r);

	bool Equals(const SurfArchway &) const;

	bool GetClassPresentationMask(_str &, void *, void *, const char *, DWORD);
	bool CheckIfElementInvisible(int, void *, DWORD);

	SAVE(SurfArchway);
	REG_MEMBER(_int, cx);
	REG_MEMBER(_int, cy);
	REG_MEMBER(_int, h);
	REG_MEMBER(_int, r);
	ENDSAVE;
};

#include "SurfSplit.h"

using Surface::g_SurfEditor;
extern Surface::SurfMap g_UniversalMap;
void Degrade(Surface::SurfMesh &Dst, const Surface::SurfMesh &Src, int LOD);
void SimplifySurface(Surface::SurfMesh &, float = 1.0f);
void OptimizeSurfMesh(Surface::SurfMesh &, bool = false);

#endif // __Surface__