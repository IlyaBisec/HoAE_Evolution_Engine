#ifndef __SelExt__
#define __SelExt__

#include "..\GameExtension.h"
#include "Surface.h"
#include "ITerrain.h"
#include "..\UndoMaster.h"

extern bool g_fWheelIsBlocked;

namespace Surface {

typedef enum { ST_INSPHEREWITHSPREAD, ST_INCYLINDER } eSelType;
extern eSelType g_SelType;

struct SelExtArgs {
	cList<cVec3>    PointsArray;
	cList<cVec3>    NormalsArray;
	cSplineBezier3	Points;
	cSplineTCB<cVec3> Normals;
	float			SplineLength;
	cVec2			duv;
	void			CreateSpline();
    void			AddPoint(cVec3 n,cVec3 p);
	void			DelLast();
	void			ClearAllPoints();
	void			DrawSpline();
	cVec2			TransformPointBySpline(cVec3 p);
	bool			IsSplined();

	int				CurrFacture;
	bool			PreserveUV;
	SurfVertex  	Center;
	cVec3			IntersectionPoint;
	int				Radius;
	cMat3			uvTransform;
	cVec2			GetUV(cVec3 n,cVec3 pos,int nFacture);
	void			ToDefault();
	void        	ApplyScale(float s);
	void			ApplyRotate(float a);
	void			ApplyShift(float du,float dv);
	//old stuff		
    float			Angle;
    float			Scale;
    float			dU;
    float			dV;
    bool			LimitInSurfaceType;
	SurfRef			Ref;
};

class SelExt : public GameExtension {
public:
	enum InvalFlags {
		IF_CACHE = 1 << 0,
		IF_GEOMETRY = 1 << 1 | IF_CACHE
	};

	SelExt();
	virtual bool CheckActivityOfEditor();
	virtual void ClearActivityOfEditor();
	virtual void ActivateEditor();
	virtual bool OnMouseWheel(int);
	virtual bool OnMouseHandling(int, int, bool &, bool &, int, int, bool);
	virtual void OnDrawOnMapOverAll();
	virtual void DrawEditorInterface();

	virtual void OnActivate() {}
	virtual void OnClear() {}
	virtual DWORD OnLMB(SelExtArgs &Args) { return 0; }
	virtual DWORD OnRMB(SelExtArgs &Args) { return 0; }
	virtual void OnDraw(SelExtArgs &Args) {}
	virtual void OnDrawInterface() {}
	virtual bool OnWheel(int) { return false; }
	
	static void GenRect(BaseMesh &bm, const cVec2 &lu, const cVec2 &wh, const cColor &Color, const int idTex, const int idSh);
	static void GenCubes(BaseMesh &bm, const cList<cVec3> &Centers, const cColor &Color, const float d, const int idTex, const int idSh);
	static void GenArrows(BaseMesh &bm, const cList<cVec3> &Centers, const cList<cVec3> &Dirs, const cColor &Color, const float Radius,
		const float Length, const int idTex, const int idSh);
	static void DrawWindow(int x0, int y0, int x1, int y1, DWORD argbColor, int Border);
	static void DrawColoredSwatch(SurfMesh &Mesh);
	static void SplitSwatch(SurfMesh &Mesh, SurfSplit &ssStage1, SurfSplit &ssStage2);
	static void DrawSwatch(SurfMesh &Mesh);
	static void DrawGrid(const SurfRef &Ref, DWORD argbColor);
	static eMappingType QueryMappingTypeKeys();
	static bool IsSelected(const SurfVertex &vtx, const SurfVertex &Center, int Radius);

	static const int RadiusDelta = 24;
	static const int RadiusMin = 16;
	static const int RadiusInitial = 16 + 4 * RadiusDelta;
	static const int dBorder = 3;
	static const int IconTopOffset = 60;
	static const int IconRightOffset = 10;
	static const int IconLxL = 91;
    SelExtArgs m_Args;
private:
	bool m_fActive;
	SurfMap *m_pUniversalMap;
	int *m_pSelRadius;
	
};

class SurfSelCtrl {
public:
	typedef bool (*CallBack)(int index, cStr &Name, DWORD &Color);
	void Init(CallBack fn, const int X0 = -1, const int Y0 = -1, const int W = 300, const int H = 36);
	void Draw(int nActive, int nHot) const;
	int GetCurSel() const;
private:
	struct Sample {
		int x0, y0, x1, y1;
		DWORD Color;
		cStr Name;
		bool IsPicked(int xMouse, int yMouse) const { return InRect(x0, y0, x1, y1, xMouse, yMouse); }
		void Draw() const;
	};
	cList<Sample> m_Samples;
};

class SurfListCtrl {
public:
	typedef bool (*CallBack)(int index, cStr &Name, DWORD &Color);
	void Init(CallBack fn, const int nSelected = -1, const DWORD Key = VK_SPACE,
		const int X0 = -1, const int Y0 = -1, const int W = 300, const int H = 36);

	void OnDrawInterface();

	int CurSel() const { return m_nSelected; }
	bool Active() const { return m_fActive; }
private:
	bool m_fActive;
	int m_nSelected;
	DWORD m_Key;

	struct Sample {
		int x0, y0, x1, y1;
		DWORD Color;
		cStr Name;
		bool IsPicked(int xMouse, int yMouse) const { return InRect(x0, y0, x1, y1, xMouse, yMouse); }
		void Draw() const;
	};
	cList<Sample> m_Samples;
	void Draw(int nActive, int nHot) const;
	int GetCurSel() const;
};

class SurfSwatchCtrl {
public:
	typedef bool (*CallBack)(int index, int &idTex);
	void Init(CallBack fn, const int nSelected = -1, const DWORD Key = VK_SPACE,
		const bool fShowSelectedIcon = true, const bool fAllowSelectNothing = false);
	
	void OnDrawInterface();

	int CurSel() const { return m_nSelected; }
	void CurSel(int nSelectd);
	bool Active() const { return m_fActive; }
private:
	bool m_fActive, m_fShowSelectedIcon, m_fAllowSelectNothing;
	int m_nSelected;
	DWORD m_Key;

	struct Sample {
		BaseMesh bmSample, bmInfoBase;
		int x0, y0, x1, y1;
		struct {
			cStr WxH;
			cStr Title;
		} Info;
		bool IsPicked(int xMouse, int yMouse) const { return InRect(x0, y0, x1, y1, xMouse, yMouse); }
		void Draw();
		void Gen(int x, int y, int L, int idTex);
	};
	struct Swatch {
		cList<Sample> Samples;
		Sample ActiveSample;
		void Draw(int nSelected, int nHot);
		void Gen(ints idsTex);
		int GetCurSel() const;
	} m_Swatch;
	void DrawActiveSample();
};

} // Surface

extern bool g_fSpaceIsBlocked;

#endif // __SelExt__