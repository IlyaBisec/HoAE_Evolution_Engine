#ifndef __cDIB__
#define __cDIB__

#include "cUtils.h"
#include "cVector.h"
#include "cStr.h"

namespace comms {

extern BYTE Chars8x16[95][16];

class cDIB {
public:
	cDIB() {
		m_pBits = NULL;
		Free();
	}
	cDIB(LONG Lx, LONG Ly) {
		m_pBits = NULL;
		Init(Lx, Ly);
	}
	~cDIB() {
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
		assert(m_pBits);
	}
	void SetPixel(int x, int y, COLORREF Color);
	void SetPixel(int x, int y, const cColor &Color);
	DWORD GetPixel(int x, int y) const;
	DWORD GetBlot(int x, int y, int nSize) const;
	void Tri(const cVector &t0, const cVector &t1, const cVector &t2, COLORREF Color);
	void Quad(const cVector &ul, int d, COLORREF Clr);
	void BlockTri(const cVector &t0, const cVector &t1, const cVector &t2, COLORREF Color);
	void ZTri(const cVector &t0, const cVector &t1, const cVector &t2, COLORREF Color, int *pZBuffer);
	void Line(const cVector &l0, const cVector &l1, COLORREF Color);
	void BoldLine(const cVector &l0, const cVector &l1, COLORREF Color);
	void Char(int c, int x, int y, COLORREF Color);
	void Str(const cStr &Str, int x, int y);
	void StrCenter(const cStr &Str, int cx, int cy);
	void StrCenter(const cStr &Str, const cVector &Center);
	void Clear(BYTE GreyLevel);
	void toDC(HDC hDC) const {
		SetDIBitsToDevice(hDC, 0, 0, m_bmi.bmiHeader.biWidth, m_bmi.bmiHeader.biHeight,
			0, 0, 0, m_bmi.bmiHeader.biHeight, m_pBits, &m_bmi, DIB_RGB_COLORS);
	}
	void ConvertFrom32To24();
	bool LoadBMP(const char *pFileName);
	bool SaveBMP(const char *pFileName);
	void Scale(int s);
	
	int Lx() const;
	int Ly() const;

	cDIB & Copy(const cDIB &Src);
	cDIB & Paste(int WhereX, int WhereY, const cDIB &Src);
private:
	BITMAPINFO m_bmi;
	DWORD m_nBytesPerScanLine;
	BYTE *m_pBits;
};

// cDIB::Lx : int() const
inline int cDIB::Lx() const {
	return m_bmi.bmiHeader.biWidth;
}

// cDIB::Ly : int() const
inline int cDIB::Ly() const {
	return m_bmi.bmiHeader.biHeight;
}

// cDIB::SetPixel
inline void cDIB::SetPixel(int x, int y, COLORREF Color) {
	if(x < 0 || x >= m_bmi.bmiHeader.biWidth || y < 0 || y >= m_bmi.bmiHeader.biHeight) return;
	long index = m_nBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 3 * x;
	m_pBits[index] = GetBValue(Color);
	m_pBits[index + 1] = GetGValue(Color);
	m_pBits[index + 2] = GetRValue(Color);
}

// cDIB::SetPixel : void(int, int, const cColor &)
inline void cDIB::SetPixel(int x, int y, const cColor &Color) {
	if(x < 0 || x >= m_bmi.bmiHeader.biWidth || y < 0 || y >= m_bmi.bmiHeader.biHeight) return;
	long index = m_nBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 3 * x;
	m_pBits[index] = (BYTE)cColor::Pack(Color.b);
	m_pBits[index + 1] = (BYTE)cColor::Pack(Color.g);
	m_pBits[index + 2] = (BYTE)cColor::Pack(Color.r);
}

//-----------------------------------------------------------------------------
// cDIB::GetPixel : DWORD(int, int) const
//-----------------------------------------------------------------------------
inline DWORD cDIB::GetPixel(int x, int y) const {
	if(x < 0 || x >= m_bmi.bmiHeader.biWidth || y < 0 || y >= m_bmi.bmiHeader.biHeight) return 0;
	long index = m_nBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 3 * x;
	BYTE b = m_pBits[index];
	BYTE g = m_pBits[index + 1];
	BYTE r = m_pBits[index + 2];
	return RGB(r, g, b);
}

//-----------------------------------------------------------------------------
// cDIB::GetBlot : DWORD(int, int, int) const
//-----------------------------------------------------------------------------
inline DWORD cDIB::GetBlot(int x, int y, int nSize) const {
	cColor Clr(cColor::Black);
	int X, Y, n = 0;
	for(int dy = 0; dy < nSize; dy++) {
		for(int dx = 0; dx < nSize; dx++) {
			X = x + dx;
			Y = y + dy;
			if(X < Lx() && Y < Ly()) {
				Clr += cColor::FromArgb(GetPixel(X, Y));
				n++;
			}
		}
	}
	if(n) {
		Clr /= (float)n;
	}
	return Clr.ToArgb();
}



// cDIB::Clear
inline void cDIB::Clear(BYTE GreyLevel) {
	memset(m_pBits, GreyLevel, m_bmi.bmiHeader.biSizeImage);
}

} // comms

#endif // __cDIB__