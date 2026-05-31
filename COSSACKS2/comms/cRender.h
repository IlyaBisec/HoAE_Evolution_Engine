#pragma once

class cMaterial;

//*****************************************************************************
// cRender
//*****************************************************************************
class cRender {
private:
	static cRender *Instance;
public:
	static void SetInstance(cRender *RenderInstance) { Instance = RenderInstance; }
	static cRender * GetInstance() { return Instance; }
	
	virtual bool Init(const void *hWnd) = 0;
	virtual void Free() = 0;

	virtual void SetViewer(const cViewer *) = 0;
	virtual const cViewer * GetViewer() const = 0;

	struct Filter {
		static const int Nearest = 0;
		static const int Linear = 1;
		static const int Bilinear = 2;
		static const int Trilinear = 3;
		static const int BilinearAniso = 4;
		static const int TrilinearAniso = 5;

		static bool HasMipMaps(const int Filter) { return Filter >= cRender::Filter::Bilinear; }
		static bool HasAniso(const int Filter) { return Filter >= cRender::Filter::BilinearAniso; }
	};

	enum TextureFlags {
		CLAMP_S = 0x1, CLAMP_T = 0x2, CLAMP_R = 0x4,
		CLAMP_ST = CLAMP_S | CLAMP_T, CLAMP_STR = CLAMP_S | CLAMP_T | CLAMP_R,
		NORMALMAP = 0x40, KEEPHEIGHT = 0x80, NORMALHEIGHTMAP = NORMALMAP | KEEPHEIGHT, CUBEMAP = 0x100
	};
	

	
	static const int idNone = -1;
	typedef int idTexture;
	virtual idTexture AddTexture(const char *FileName, const bool MipMap) = 0;
	
	struct Blend {
		static const int None = 0;
		static const int Zero = 1;					// (0, 0, 0, 0)
		static const int One = 2;					// (1, 1, 1, 1)
		static const int SourceColor = 3;			// (Rs, Gs, Bs, As)
		static const int InvSourceColor = 4;		// (1 - Rs, 1 - Gs, 1 - Bs, 1 - As)
		static const int SourceAlpha = 5;			// (As, As, As, As)
		static const int InvSourceAlpha = 6;		// (1 - As, 1 - As, 1 - As, 1 - As)
		static const int DestinationAlpha = 7;		// (Ad, Ad, Ad, Ad)
		static const int InvDestinationAlpha = 8;	// (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad)
		static const int DestinationColor = 9;		// (Rd, Gd, Bd, Ad)
		static const int InvDestinationColor = 10;	// (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad)
		static const int SourceAlphaSat = 11;		// (f, f, f, 1); f = Min(As, 1 - Ad)
	};
	virtual void SetBlendFunc(const int SourceBlend, const int DestinationBlend) = 0;
	
	struct Compare {
		static const int Never = 1;
		static const int Less = 2;
		static const int Equal = 3;
		static const int LessEqual = 4;
		static const int Greater = 5;
		static const int NotEqual = 6;
		static const int GreaterEqual = 7;
		static const int Always = 8;
	};
	virtual void SetDepthFunc(const int Compare) = 0;
	
	struct Cull {
		static const int None = 1;
		static const int Clockwise = 2;
		static const int CounterClockwise = 3;
	};
	virtual void SetCullMode(const int Cull) = 0;
	
	struct ClearFlags {
		static const int Target = 1;
		static const int ZBuffer = 2;
		static const int Stencil = 4;
	};
	virtual void Clear(const DWORD Flags, const cColor &Color = cColor(0.63f, 0.63f, 0.63f, 1.0f), const float zDepth = 1.0f, const int Stencil = 0) = 0;
	virtual void Present() = 0;

	struct TransformType {
		static const int View = 1;
		static const int Projection = 2;
	};
	virtual void SetTransform(const int TransformType, const cMat4 &) = 0;
	virtual void SetViewPort(const cRectI &) = 0;
	
	//*************************************************************************
	// Topology
	//*************************************************************************
	struct Topology {
		static const int Undefined = 0, PointList = 1, LineList = 2, LineStrip = 3,
			TriangleList = 4, TriangleStrip = 5, TriangleFan = 6;

		static bool IsValid(const int Topology) {
			return Topology >= PointList && Topology <= TriangleFan;
		}

		//-------------------------------------------------------------------------
		// NIndices
		//-------------------------------------------------------------------------
		static const int NIndices(const int Topology, const int NPrims) {
			switch(Topology) {
				case PointList: return NPrims;
				case LineList: return 2 * NPrims;
				case LineStrip: return NPrims + 1;
				case TriangleList: return 3 * NPrims;
				case TriangleStrip:  return NPrims + 2;
				case TriangleFan: return NPrims + 2;
				default: return 0;
			}
		}
		//-------------------------------------------------------------------------
		// NPrims
		//-------------------------------------------------------------------------
		static int NPrims(const int Topology, const int NIndices) {
			switch(Topology) {
				case PointList: return NIndices;
				case LineList: return NIndices / 2;
				case LineStrip: return NIndices - 1;
				case TriangleList: return NIndices / 3;
				case TriangleStrip: return NIndices - 2;
				case TriangleFan: return NIndices - 2;
				default: return 0;
			}
		}
	};
	
	virtual void DrawPrimitives(const int Topology, const cVertex::Format &VertexFormat, const void *Verts, const int NVerts, const int *Indices = NULL, const int NIndices = 0, const cMaterial *Material = NULL) = 0;
	virtual void DrawLine(const cVec3 &Fm, const cVec3 &To, const cColor &Color, const float Width = 1.0f) = 0;
	
	void DrawCircle(const cVec3 &Center, const float Radius, const cVec3 &Axis, const cColor &Color, const int SubDivs = 12, const float Width = 1.0f);
	void DrawBillboardCircle(const cVec3 &Center, const float Radius, const cColor &Color, const int SubDivs = 12, const float Width = 1.0f) {
		DrawCircle(Center, Radius, GetViewer()->GetForward(), Color, SubDivs, Width);
	}
	void DrawBillboardQuad(const cVec3 &Center, const float Side, const cColor &Color);
	void DrawFacingCircle(const cVec3 &Center, const float Radius, const cVec3 &Axis, const cColor &Color, const int SubDivs = 12, const float Width = 1.0f);
	void DrawCone(const cVec3 &Apex, const cVec3 &Axis, const float Radius, const float Length, const cColor &Color, const int SubDivs = 12);
	void DrawSector(const cVec3 &Center, const float Radius, const cVec3 &Fm, const cVec3 &To, const cColor &Color, const float dAngle = 5.0f);
	void DrawCube(const cVec3 &Center, const float Side, const cColor &Color, const cQuat &q = cQuat::Identity);
	void DrawStrCentered(const cStr &Str, const cVec2 &Center);
	void DrawStr(const cStr &Str, const cVec2 &luPos);
	void DrawStrCentered(const cStr &Str, const cVec3 &Center);
	void DrawStr(const cStr &Str, const cVec3 &luPos);

protected:
	int m_MaxTexUnits;
	idTexture *m_CurrentTextures;
	idTexture *m_SelectedTextures;
	float *m_TextureLod;
};