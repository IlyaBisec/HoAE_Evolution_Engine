#ifndef __cDIB__
#define __cDIB__

#include "cUtils.h"
#include "cVector.h"
#include "cStr.h"

namespace comms {

class cDIB {
public:
	cDIB() {
		Init(1, 1);
	}
	cDIB(LONG Lx, LONG Ly) {
		Init(Lx, Ly);
	}
	~cDIB() {
		Free();
	}
	void Free() {
		ZeroMemory(&m_bmi, sizeof(BITMAPINFO));
		if(m_pBits) delete[] m_pBits;
	}
	void Init(LONG Lx, LONG Ly) {
		assert(Lx > 0);
		assert(Ly > 0);
		Free();

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
	void Tri(const cVector &t0, const cVector &t1, const cVector &t2, COLORREF Color);
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
	void toFile(LPTSTR lpFileName) {
		assert(m_pBits);

		HANDLE hf = CreateFile(lpFileName, GENERIC_READ | GENERIC_WRITE,
			(DWORD)0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
		assert(hf != INVALID_HANDLE_VALUE);

		BITMAPFILEHEADER hdr;
		ZeroMemory(&hdr, sizeof(BITMAPFILEHEADER));
		hdr.bfType = 0x4d42;
		hdr.bfSize = sizeof(BITMAPFILEHEADER) + m_bmi.bmiHeader.biSize + m_bmi.bmiHeader.biSizeImage;
		hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + m_bmi.bmiHeader.biSize;

		DWORD nBytesWritten;
		WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&nBytesWritten, NULL);
		WriteFile(hf, (LPVOID)&m_bmi.bmiHeader, sizeof(BITMAPINFOHEADER), (LPDWORD)&nBytesWritten, NULL);
		WriteFile(hf, (LPSTR)m_pBits, m_bmi.bmiHeader.biSizeImage, (LPDWORD)&nBytesWritten, NULL);

		CloseHandle(hf);
	}
private:
	int m_nBytesPerPixel;
	BITMAPINFO m_bmi;
	DWORD m_nBytesPerScanLine;
	BYTE *m_pBits;
};

// cDIB::SetPixel
inline void cDIB::SetPixel(int x, int y, COLORREF Color) {
	if(x < 0 || x >= m_bmi.bmiHeader.biWidth || y < 0 || y >= m_bmi.bmiHeader.biHeight) return;
	long index = m_nBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 3 * x;
	m_pBits[index] = GetBValue(Color);
	m_pBits[index + 1] = GetGValue(Color);
	m_pBits[index + 2] = GetRValue(Color);
}

// cDIB::Clear
inline void cDIB::Clear(BYTE GreyLevel) {
	memset(m_pBits, GreyLevel, m_bmi.bmiHeader.biSizeImage);
}

} // comms

#endif // __cDIB__