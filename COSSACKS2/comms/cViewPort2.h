#pragma once

class cViewPort2 {
public:
	cDib24 dib;
	cVec2 At;
	float Scale;
	cMat3 T;

	void CalcT();

	cViewPort2();
	~cViewPort2();

	void Home();
	void Init(int Lx, int Ly);
	
	const cVec2 Pick(int x, int y) const;
	void Pan(int dx, int dy);
	void Zoom(int dx, int dy);
	
	bool Message(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	
	void Fit(const cBounds &B, const int Border = 15);

	void DrawSphere(const cVec2 &c, float Radius, const cColor &Color, const bool TransformRadius);
	void DrawStr(const cStr &Str, const cVec2 &luPos);
	void DrawStrCentered(const cStr &Str, const cVec2 &Center);
	void DrawLine(const cVec2 &l0, const cVec2 &l1, const cColor &Color);
	void DrawRect(const cVec2 &bbMin, const cVec2 &bbMax, const cColor &Color);
	void DrawCircle(const cVec2 &c, const float Radius, const cColor &Color, const bool TransformRadius);
	void DrawRound(const cVec2 &Center, const float Radius, const cColor &Color, const bool TransformRadius);
	void DrawTri(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, const cColor &Color);
	
	void AddCircle(const cVec2 &Center, const float Radius, const cColor &Color, const bool TransformRadius, const int nLayer = 0);
	void AddRound(const cVec2 &Center, const float Radius, const cColor &Color, const bool TransformRadius , const int nLayer = 0);
	void AddSphere(const cVec2 &Center, const float Radius, const cColor &Color, const bool TransformRadius, const int nLayer = 0);
	void AddLine(const cVec2 &A, const cVec2 &B, const cColor &Color, const int nLayer = 0);
	void AddStr(const cStr &Str, const cVec2 &luPos, const int nLayer = 0);
	void AddStrCentered(const cStr &Str, const cVec2 &Center, const int nLayer = 0);
	void AddRect(const cVec2 &bbMin, const cVec2 &bbMax, const cColor &Color, const int nLayer = 0);

	void FreeBatches();
	void Render();

	static const int LayerMaxIndex = 31;
private:
	class BatchShape {
	public:
		virtual void Draw(cViewPort2 &ViewPort) const = 0;
	};

	class BatchRect : public BatchShape {
	protected:
		cColor m_Color;
		cVec2 m_bbMin, m_bbMax;
	public:
		BatchRect(const cVec2 &bbMin, const cVec2 &bbMax, const cColor &Color)
			: m_bbMin(bbMin), m_bbMax(bbMax), m_Color(Color) {}
		virtual void Draw(cViewPort2 &ViewPort) const {
			ViewPort.DrawRect(m_bbMin, m_bbMax, m_Color);
		}
	};
	
	class BatchCircle : public BatchShape {
	protected:
		cColor m_Color;
		cVec2 m_Center;
		float m_Radius;
		bool m_TransformRadius;
	public:
		BatchCircle(const cVec2 &c, const float r, const cColor &Clr, const bool TransformRadius)
			: m_Center(c), m_Radius(r), m_Color(Clr), m_TransformRadius(TransformRadius) {}
		virtual void Draw(cViewPort2 &ViewPort) const {
			ViewPort.DrawCircle(m_Center, m_Radius, m_Color, m_TransformRadius);
		}
	};
	
	class BatchRound : public BatchCircle {
	public:
		BatchRound(const cVec2 &c, const float r, const cColor &Clr, const bool TransformRadius)
			: BatchCircle(c, r, Clr, TransformRadius) {}
		void Draw(cViewPort2 &ViewPort) const {
			ViewPort.DrawRound(m_Center, m_Radius, m_Color, m_TransformRadius);
		}
	};

	class BatchSphere : public BatchCircle {
	public:
		BatchSphere(const cVec2 &c, const float r, const cColor &Clr, const bool TransformRadius)
			: BatchCircle(c, r, Clr, TransformRadius) {}
		void Draw(cViewPort2 &ViewPort) const {
			ViewPort.DrawSphere(m_Center, m_Radius, m_Color, m_TransformRadius);
}
	};
	
	class BatchLine : public BatchShape {
	private:
		cColor m_Color;
		cVec2 m_Start, m_End;
	public:
		BatchLine(const cVec2 &A, const cVec2 &B, const cColor &Clr) : m_Start(A), m_End(B), m_Color(Clr) {}
		void Draw(cViewPort2 &ViewPort) const {
			ViewPort.DrawLine(m_Start, m_End, m_Color);
		}
	};

	class BatchStr : public BatchShape {
	protected:
		cStr m_Str;
		cVec2 m_Pos;
	public:
		BatchStr(const cStr &s, const cVec2 &luPos) : m_Str(s), m_Pos(luPos) {}
		virtual void Draw(cViewPort2 &ViewPort) const {
			ViewPort.DrawStr(m_Str, m_Pos);
		}
	};

	class BatchStrCentered : public BatchStr {
		public:
			BatchStrCentered(const cStr &s, const cVec2 &Center) : BatchStr(s, Center) {}
			void Draw(cViewPort2 &ViewPort) const {
				ViewPort.DrawStrCentered(m_Str, m_Pos);
			}
	};
	
	class Batch : public cList<BatchShape *> {
	public:
		void Draw(cViewPort2 &ViewPort) const;
	};
	Batch m_Layers[LayerMaxIndex + 1];
};