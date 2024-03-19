#pragma once

class cImage;

class cDib24 {
public:
	cDib24() {
		m_pBits = NULL;
		Free();
	}
	cDib24(LONG Lx, LONG Ly) {
		m_pBits = NULL;
		Init(Lx, Ly);
	}
	~cDib24() {
		Free();
	}
	void Free() {
		ZeroMemory(&m_bmi, sizeof(BITMAPINFO));
		if(m_pBits) {
			delete[] m_pBits;
			m_pBits = NULL;
		}
	}
	void Init(LONG Lx, LONG Ly) {
		Free();
		if(Lx <= 0 || Ly <= 0) return;

		// m_nBytesPerScanLine:
		m_nBytesPerScanLine = 3 * Lx;
		int mod = m_nBytesPerScanLine % 4;
		if(mod) m_nBytesPerScanLine += 4 - mod;
		// m_bmi:
		m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_bmi.bmiHeader.biWidth = Lx;
		m_bmi.bmiHeader.biHeight = Ly;
		m_bmi.bmiHeader.biPlanes = 1;
		m_bmi.bmiHeader.biBitCount = 24;
		m_bmi.bmiHeader.biCompression = BI_RGB;
		m_bmi.bmiHeader.biSizeImage = m_nBytesPerScanLine * Ly;
		// m_pBits:
		m_pBits = new BYTE[m_bmi.bmiHeader.biSizeImage];
		cAssert(m_pBits);
	}

	struct Pixel {
		BYTE r, g, b;

		Pixel();
		Pixel(BYTE R, BYTE G, BYTE B);
		DWORD ToBgr() const;
		DWORD ToRgb() const;
		cColor ToColor() const;
		static Pixel FromBgr(DWORD);
		static Pixel FromRgb(DWORD);
		static Pixel FromColor(const cColor &);
	};


	void SetPixel(int x, int y, const cDib24::Pixel &p);
	cDib24::Pixel GetPixel(int x, int y) const;
	cDib24::Pixel GetPixel(const float s, const float t) const;
	
	void SetPixel(int x, int y, const cDib24::Pixel &p, float z, float *pZBuffer);
	void DrawLineHorFlat(int x0, int x1, int y, float z0, float z1, const cDib24::Pixel &p, float *pZBuffer);
	void DrawPolygonFlat(const cVec3 *pVerts, const int Num, const cDib24::Pixel &p, float *pZBuffer);

	static const cColor CalcColor(const cMaterial &Mt, const float Dot);
	void DrawLineHorGouraud(int x0, int x1, int y, float z0, float z1, float d0, float d1, const cMaterial &Mt, float *pZBuffer);
	void DrawPolygonGouraud(const cVec3 *pVerts, const float *pDots, const int Num, const cMaterial &Mt, float *pZBuffer);
	
	cDib24::Pixel GetBlot(int x, int y, int nSize) const;
	void Tri(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, const cDib24::Pixel &p);
	void Quad(const cVec2 &ul, const float d, const cDib24::Pixel &p);
	void BlockTri(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, const cDib24::Pixel &p);
	void ZTri(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2, const cDib24::Pixel &p, int *pZBuffer);
	void Line(int x0, int y0, int x1, int y1, const cDib24::Pixel &p);
	void DrawLine(const cVec2 &p0, const cVec2 &p1, const cDib24::Pixel &p);
	void Char(int c, int x, int y, COLORREF Color);
	int GetStrWidth(const cStr &Str) const;
	int GetStrHeight() const;
	void Str(const cStr &Str, int x, int y);
	void StrCenter(const cStr &Str, int cx, int cy);
	void Circle(int cx, int cy, int R, const cColor &C);
	void Round(int xCenter, int yCenter, int Radius, const cColor &Color);
	void Sphere(int cx, int cy, int R, const cColor &C);
	void Clear(BYTE GreyLevel);
	void Clear(const cDib24::Pixel &p);
	void ToDC(HDC hDC) const {
		SetDIBitsToDevice(hDC, 0, 0, m_bmi.bmiHeader.biWidth, m_bmi.bmiHeader.biHeight,
			0, 0, 0, m_bmi.bmiHeader.biHeight, m_pBits, &m_bmi, DIB_RGB_COLORS);
	}
	void ConvertFrom32To24();
	bool LoadBMP(const char *pFileName);
	bool SaveBMP(const char *pFileName);
	void Scale(int s);
	
	int GetWidth() const;
	int GetHeight() const;
	
	cDib24 & Copy(const cDib24 &Src);
	cDib24 & Paste(int WhereX, int WhereY, const cDib24 &Src);

	void Sharp(const float Sharpness);
private:
	BITMAPINFO m_bmi;
	DWORD m_nBytesPerScanLine;
	BYTE *m_pBits;
};

// cDib24::Pixel.ctor : ()
inline cDib24::Pixel::Pixel() {
}

// cDib24::Pixel.ctor : (BYTE, BYTE, BYTE)
inline cDib24::Pixel::Pixel(BYTE R, BYTE G, BYTE B)
: r(R), g(G), b(B) {}

// cDib24::Pixel::ToBgr : DWORD() const
inline DWORD cDib24::Pixel::ToBgr() const {
	return b | (WORD)g << 8 | (DWORD)r << 16;
}

// cDib24::Pixel::ToRgb : DWORD() const
inline DWORD cDib24::Pixel::ToRgb() const {
	return r | (WORD)g << 8 | (DWORD)b << 16;
}

// cDib24::Pixel::ToColor : cColor() const
inline cColor cDib24::Pixel::ToColor() const {
	return cColor::FromRgba(r, g, b);
}

// cDib24::Pixel::FromBgr : cDib24::Pixel(DWORD)
inline cDib24::Pixel cDib24::Pixel::FromBgr(DWORD Bgr) {
	return Pixel((BYTE)(Bgr >> 16), (BYTE)(Bgr >> 8), (BYTE)(Bgr));
}

// cDib24::Pixel::FromRgb : cDib24::Pixel(DWORD)
inline cDib24::Pixel cDib24::Pixel::FromRgb(DWORD Rgb) {
	return Pixel((BYTE)Rgb, (BYTE)(Rgb >> 8), (BYTE)(Rgb >> 16));
}

// cDib24::Pixel::FromColor : cDib24::Pixel(const cColor &)
inline cDib24::Pixel cDib24::Pixel::FromColor(const cColor &Color) {
	return Pixel((BYTE)cColor::Pack(Color.r), (BYTE)cColor::Pack(Color.g), (BYTE)cColor::Pack(Color.b));
}

// cDib24::GetWidth : int() const
inline int cDib24::GetWidth() const {
	return m_bmi.bmiHeader.biWidth;
}

// cDib24::GetHeight : int() const
inline int cDib24::GetHeight() const {
	return m_bmi.bmiHeader.biHeight;
}

// cDib24::SetPixel
inline void cDib24::SetPixel(int x, int y, const cDib24::Pixel &p) {
	if(x < 0 || x >= m_bmi.bmiHeader.biWidth || y < 0 || y >= m_bmi.bmiHeader.biHeight) return;
	long index = m_nBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 3 * x;
	m_pBits[index] = p.b;
	m_pBits[index + 1] = p.g;
	m_pBits[index + 2] = p.r;
}

inline void cDib24::SetPixel(int x, int y, const cDib24::Pixel &p, float z, float *pZBuffer) {
	if(x < 0 || x >= m_bmi.bmiHeader.biWidth || y < 0 || y >= m_bmi.bmiHeader.biHeight) {
		return;
	}
	long zindex = (long)y * m_bmi.bmiHeader.biWidth + (long)x;
	if(z <= pZBuffer[zindex]) {
		pZBuffer[zindex] = z;
		long index = m_nBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 3 * x;
		m_pBits[index] = p.b;
		m_pBits[index + 1] = p.g;
		m_pBits[index + 2] = p.r;
	}
}

//-----------------------------------------------------------------------------
// cDib24::GetPixel : DWORD(int, int) const
//-----------------------------------------------------------------------------
inline cDib24::Pixel cDib24::GetPixel(int x, int y) const {
	Pixel p;
	if(x < 0 || x >= m_bmi.bmiHeader.biWidth || y < 0 || y >= m_bmi.bmiHeader.biHeight) {
		p.r = p.g = p.b = 0;
		return p;
	}
	long index = m_nBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 3 * x;
	p.b = m_pBits[index];
	p.g = m_pBits[index + 1];
	p.r = m_pBits[index + 2];
	return p;
}

//-----------------------------------------------------------------------------
// cDib24::GetBlot : DWORD(int, int, int) const
//-----------------------------------------------------------------------------
inline cDib24::Pixel cDib24::GetBlot(int x, int y, int nSize) const {
	cColor Clr(cColor::Black);
	int X, Y, n = 0;
	for(int dy = 0; dy < nSize; dy++) {
		for(int dx = 0; dx < nSize; dx++) {
			X = x + dx;
			Y = y + dy;
			if(X < GetWidth() && Y < GetHeight()) {
				Clr += GetPixel(X, Y).ToColor();//cColor::FromArgb(GetPixel(X, Y));
				n++;
			}
		}
	}
	if(n) {
		Clr /= (float)n;
	}
	return cDib24::Pixel::FromColor(Clr);
}



// cDib24::Clear
inline void cDib24::Clear(BYTE GreyLevel) {
	memset(m_pBits, GreyLevel, m_bmi.bmiHeader.biSizeImage);
}