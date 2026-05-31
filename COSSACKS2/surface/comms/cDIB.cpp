#include "cDIB.h"

namespace comms {

// cDIB::Quad
void cDIB::Quad(const cVector &ul, int d, COLORREF Clr) {
	Tri(ul, ul + cVector(1, 0, 0) * d, ul + cVector(1, 1, 0) * d, Clr);
	Tri(ul, ul + cVector(1, 1, 0) * d, ul + cVector(0, 1, 0) * d, Clr);
}

// cDIB::Tri
void cDIB::Tri(const cVector &t0, const cVector &t1, const cVector &t2, COLORREF Color) {
	// 28.4
	const int x1 = int(16. * t0.x), x2 = int(16. * t1.x), x3 = int(16. * t2.x);
	const int y1 = int(16. * t0.y), y2 = int(16. * t1.y), y3 = int(16. * t2.y);

	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;
	// int fixed - point:
	const int fDx12 = Dx12 << 4, fDx23 = Dx23 << 4, fDx31 = Dx31 << 4;
	const int fDy12 = Dy12 << 4, fDy23 = Dy23 << 4, fDy31 = Dy31 << 4;

	// Bounding rect:
	int xMin = (comms::Min(x1, x2, x3) + 0xF) >> 4;
	int xMax = (comms::Max(x1, x2, x3) + 0xF) >> 4;
	int yMin = (comms::Min(y1, y2, y3) + 0xF) >> 4;
	int yMax = (comms::Max(y1, y2, y3) + 0xF) >> 4;

	// Constant parts of half - space functions:
	int C1 = Dy12 * x1 - Dx12 * y1;
	int C2 = Dy23 * x2 - Dx23 * y2;
	int C3 = Dy31 * x3 - Dx31 * y3;

	// Fill convention:
	if(Dy12 < 0 || (Dy12 == 0 && Dx12 > 0)) C1++;
	if(Dy23 < 0 || (Dy23 == 0 && Dx23 > 0)) C2++;
	if(Dy31 < 0 || (Dy31 == 0 && Dx31 > 0)) C3++;

	// Starting values of the half - space functions at the top of the bounding rectangle:
	int Cy1 = C1 + Dx12 * (yMin << 4) - Dy12 * (xMin << 4);
	int Cy2 = C2 + Dx23 * (yMin << 4) - Dy23 * (xMin << 4);
	int Cy3 = C3 + Dx31 * (yMin << 4) - Dy31 * (xMin << 4);

	// Scanning:
	for(int y = yMin; y < yMax; y++) {
		// Starting values for horizontal scan:
		int Cx1 = Cy1, Cx2 = Cy2, Cx3 = Cy3;
		for(int x = xMin; x < xMax; x++) {
			if(Cx1 > 0 && Cx2 > 0 && Cx3 > 0) {
				SetPixel(x, y, Color);
			}
			Cx1 -= fDy12;
			Cx2 -= fDy23;
			Cx3 -= fDy31;
		}
		Cy1 += fDx12;
		Cy2 += fDx23;
		Cy3 += fDx31;
	}
} // cDIB::Tri

// cDIB::BlockTri
void cDIB::BlockTri(const cVector &t0, const cVector &t1, const cVector &t2, COLORREF Color) {
	// 28.4
	const int x1 = int(16. * t0.x), x2 = int(16. * t1.x), x3 = int(16. * t2.x);
	const int y1 = int(16. * t0.y), y2 = int(16. * t1.y), y3 = int(16. * t2.y);

	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;
	// int fixed - point:
	const int fDx12 = Dx12 << 4, fDx23 = Dx23 << 4, fDx31 = Dx31 << 4;
	const int fDy12 = Dy12 << 4, fDy23 = Dy23 << 4, fDy31 = Dy31 << 4;

	// Bounding rect:
	int xMin = (comms::Min(x1, x2, x3) + 0xF) >> 4;
	int xMax = (comms::Max(x1, x2, x3) + 0xF) >> 4;
	int yMin = (comms::Min(y1, y2, y3) + 0xF) >> 4;
	int yMax = (comms::Max(y1, y2, y3) + 0xF) >> 4;

	// Block size (must be power of 2):
	const int q = 8; // 8x8

	// Start in corner of block:
	xMin &= ~(q - 1);
	yMin &= ~(q - 1);

	// Constant parts of half - space functions:
	int C1 = Dy12 * x1 - Dx12 * y1;
	int C2 = Dy23 * x2 - Dx23 * y2;
	int C3 = Dy31 * x3 - Dx31 * y3;

	// Fill convention:
	if(Dy12 < 0 || (Dy12 == 0 && Dx12 > 0)) C1++;
	if(Dy23 < 0 || (Dy23 == 0 && Dx23 > 0)) C2++;
	if(Dy31 < 0 || (Dy31 == 0 && Dx31 > 0)) C3++;

	// Loop through blocks:
	for(int y = yMin; y < yMax; y += q) {
		for(int x = xMin; x < xMax; x += q) {
			// Corners of block:
			int x0 = x << 4, x1 = (x + q - 1) << 4;
			int y0 = y << 4, y1 = (y + q - 1) << 4;
			// Evaluate half - space functions:
			bool a00 = C1 + Dx12 * y0 - Dy12 * x0 > 0;
			bool a10 = C1 + Dx12 * y0 - Dy12 * x1 > 0;
			bool a01 = C1 + Dx12 * y1 - Dy12 * x0 > 0;
			bool a11 = C1 + Dx12 * y1 - Dy12 * x1 > 0;
			int a = (a00 << 0) | (a10 << 1) | (a01 << 2) | (a11 << 3);

			bool b00 = C2 + Dx23 * y0 - Dy23 * x0 > 0;
			bool b10 = C2 + Dx23 * y0 - Dy23 * x1 > 0;
			bool b01 = C2 + Dx23 * y1 - Dy23 * x0 > 0;
			bool b11 = C2 + Dx23 * y1 - Dy23 * x1 > 0;
			int b = (b00 << 0) | (b10 << 1) | (b01 << 2) | (b11 << 3);

			bool c00 = C3 + Dx31 * y0 - Dy31 * x0 > 0;
			bool c10 = C3 + Dx31 * y0 - Dy31 * x1 > 0;
			bool c01 = C3 + Dx31 * y1 - Dy31 * x0 > 0;
			bool c11 = C3 + Dx31 * y1 - Dy31 * x1 > 0;
			int c = (c00 << 0) | (c10 << 1) | (c01 << 2) | (c11 << 3);

			// Skip this block if it's outside at least one edge:
			if(a == 0x0 || b == 0x0 || c == 0x0) continue;

			if(a == 0xF && b == 0xF && c == 0xF) { // block is totally covered
				for(int iy = y; iy < y + q; iy++)
					for(int ix = x; ix < x + q; ix++) {
						SetPixel(ix, iy, Color);
					}
			} else { // block is partically covered
				// Starting values of the half - space functions at the top of the bounding rectangle:
				int Cy1 = C1 + Dx12 * y0 - Dy12 * x0;
				int Cy2 = C2 + Dx23 * y0 - Dy23 * x0;
				int Cy3 = C3 + Dx31 * y0 - Dy31 * x0;

				// Scanning:
				for(int iy = y; iy < y + q; iy++) {
					// Starting values for horizontal scan:
					int Cx1 = Cy1, Cx2 = Cy2, Cx3 = Cy3;
					for(int ix = x; ix < x + q; ix++) {
						if(Cx1 > 0 && Cx2 > 0 && Cx3 > 0) {
							SetPixel(ix, iy, Color);
						}
						Cx1 -= fDy12;
						Cx2 -= fDy23;
						Cx3 -= fDy31;
					}
					Cy1 += fDx12;
					Cy2 += fDx23;
					Cy3 += fDx31;
				}
			}
		}
	}
} // cDIB::BlockTri

// cDIB::ZTri
void cDIB::ZTri(const cVector &t0, const cVector &t1, const cVector &t2, COLORREF Color, int *pZBuffer) {
	int x0 = (int)t0.x, y0 = (int)t0.y, z0 = (int)t0.z;
	int x1 = (int)t1.x, y1 = (int)t1.y, z1 = (int)t1.z;
	int x2 = (int)t2.x, y2 = (int)t2.y, z2 = (int)t2.z;

	// Sorting ascending along y axis:
	if(y0 > y1) { Swap(x0, x1); Swap(y0, y1); Swap(z0, z1); }
	if(y0 > y2) { Swap(x0, x2); Swap(y0, y2); Swap(z0, z2); }
	if(y1 > y2) { Swap(x1, x2); Swap(y1, y2); Swap(z1, z2); }

	if(y2 - y1 == 0) return;

	for(int y = y0; y <= y2; y++) {
		// Crossing with longest section ((x0, y0) --- (x2, y2)):
		int _x1 = x0 + (y - y0) * (x2 - x0) / (y2 - y0);
		int _z1 = z0 + (y - y0) * (z2 - z0) / (y2 - y0);

		// Crossing with short section ((x0, y0) --- (x1, y1) or (x1, y1) --- (x2, y2)):
		int _x2, _z2;
		if(y < y1) {
			_x2 = x0 + (y - y0) * (x1 - x0) / (y1 - y0);
			_z2 = z0 + (y - y0) * (z1 - z0) / (y1 - y0);
		} else {
			if(y2 == y1) {
				_x2 = x1;
				_z2 = z1;
			}
			else {
				_x2 = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
				_z2 = z1 + (y - y1) * (z2 - z1) / (y2 - y1);
			}
		}

		if(_x1 > _x2) {
			Swap(_x1, _x2);
			Swap(_z1, _z2);
		}

		// Rasterizing horizontal line (_x1 - _x2):
		for(int x = _x1; x < _x2; x++) {
			int z = _z1 + (x - _x1) * (_z2 - _z1) / (_x2 - _x1);
//			SetPixel(x, y, Color);
//			if(x >= 0 && x < SSCache::cZBufferSide && y >= 0 && y < SSCache::cZBufferSide) {
			if(x >= 0 && x < m_bmi.bmiHeader.biWidth && y >= 0 && y < m_bmi.bmiHeader.biHeight) {
				int index = y * m_bmi.bmiHeader.biWidth + x;
				if(pZBuffer[index] < z) {
					SetPixel(x, y, Color);
					pZBuffer[index] = z;
				}
			}
//				if(C.pZBuffer[y * SSCache::cZBufferSide + x] < z) {
//					C.pZBuffer[y * SSCache::cZBufferSide + x] = z;
//					dib.SetPixel(x, y, comms::cWhite); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//				}
//			}
		}
	}
} // cDIB::ZTri

// cDIB::Line
void cDIB::Line(const cVector &l0, const cVector &l1, COLORREF Color) {
	const int x0 = int(l0.x), x1 = int(l1.x);
	const int y0 = int(l0.y), y1 = int(l1.y);

	const int dx = abs(x1 - x0), dy = abs(y1 - y0);

	const int sx = x1 >= x0 ? 1 : -1;
	const int sy = y1 >= y0 ? 1 : -1;

	if(dy <= dx) {
		int d1 = dy << 1;
		int d = d1 - dx;
		int d2 = (dy - dx) << 1;
		SetPixel(x0, y0, Color);
		for(int x = x0 + sx, y = y0, i = 1; i <= dx; i++, x += sx) {
			if(d > 0) {
				d += d2;
				y += sy;
			} else d += d1;
			SetPixel(x, y, Color);
		}
	} else {
		int d1 = dx << 1;
		int d = d1 - dy;
		int d2 = (dx - dy) << 1;
		SetPixel(x0, y0, Color);
		for(int x = x0, y = y0 + sy, i = 1; i <= dy; i++, y += sy) {
			if(d > 0) {
				d += d2;
				x += sx;
			} else d += d1;
			SetPixel(x, y, Color);
		}
	}
} // cDIB::Line

// cDIB::BoldLine
void cDIB::BoldLine(const cVector &l0, const cVector &l1, COLORREF Color) {
	const int x0 = int(l0.x), x1 = int(l1.x);
	const int y0 = int(l0.y), y1 = int(l1.y);

	const int dx = abs(x1 - x0), dy = abs(y1 - y0);

	const int sx = x1 >= x0 ? 1 : -1;
	const int sy = y1 >= y0 ? 1 : -1;

	if(dy <= dx) {
		int d1 = dy << 1;
		int d = d1 - dx;
		int d2 = (dy - dx) << 1;
		SetPixel(x0 - 1, y0 - 1, Color);
		SetPixel(x0, y0 - 1, Color);
		SetPixel(x0 - 1, y0, Color);
		SetPixel(x0, y0, Color);
		for(int x = x0 + sx, y = y0, i = 1; i <= dx; i++, x += sx) {
			if(d > 0) {
				d += d2;
				y += sy;
			} else d += d1;
			SetPixel(x - 1, y - 1, Color);
			SetPixel(x, y - 1, Color);
			SetPixel(x - 1, y, Color);
			SetPixel(x, y, Color);
		}
	} else {
		int d1 = dx << 1;
		int d = d1 - dy;
		int d2 = (dx - dy) << 1;
		SetPixel(x0 - 1, y0 - 1, Color);
		SetPixel(x0, y0 - 1, Color);
		SetPixel(x0 - 1, y0, Color);
		SetPixel(x0, y0, Color);
		for(int x = x0, y = y0 + sy, i = 1; i <= dy; i++, y += sy) {
			if(d > 0) {
				d += d2;
				x += sx;
			} else d += d1;
			SetPixel(x - 1, y - 1, Color);
			SetPixel(x, y - 1, Color);
			SetPixel(x - 1, y, Color);
			SetPixel(x, y, Color);
		}
	}
} // cDIB::BoldLine

BYTE Chars8x16[95][16] = {
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, //   //
	{   0,   0,   0,   0,  24,  24,   0,  24,  24,  24,  60,  60,  60,  24,   0,   0}, // ! //
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  36, 102, 102, 102,   0}, // " //
	{   0,   0,   0,   0,  54,  54, 127,  54,  54,  54, 127,  54,  54,   0,   0,   0}, // # //
	{   0,   0,   0,  24,  24,  62,  99,  97,  96,  62,   3,  67,  99,  62,  24,  24}, // $ //
	{   0,   0,   0,   0,  97,  99,   6,  12,  24,  48,  99,  67,   0,   0,   0,   0}, // % //
	{   0,   0,   0,   0, 110,  51,  51,  51,  59, 110,  28,  54,  54,  28,   0,   0}, // & //
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6,  12,  12,  12,   0}, // ' //
	{   0,   0,   0,   0,  48,  24,  12,  12,  12,  12,  12,  12,  24,  48,   0,   0}, // ( //
	{   0,   0,   0,   0,  12,  24,  48,  48,  48,  48,  48,  48,  24,  12,   0,   0}, // ) //
	{   0,   0,   0,   0,   0,   0, 102,  60, 255,  60, 102,   0,   0,   0,   0,   0}, // * //
	{   0,   0,   0,   0,   0,   0,  24,  24, 126,  24,  24,   0,   0,   0,   0,   0}, // + //
	{   0,   0,   0,  12,  24,  24,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0}, // , //
	{   0,   0,   0,   0,   0,   0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0}, // - //
	{   0,   0,   0,   0,  24,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, // . //
	{   0,   0,   0,   0,   1,   3,   6,  12,  24,  48,  96,  64,   0,   0,   0,   0}, // / //
	{   0,   0,   0,   0,  62,  99,  99, 103, 107, 107, 115,  99,  99,  62,   0,   0}, // 0 //
	{   0,   0,   0,   0, 126,  24,  24,  24,  24,  24,  24,  30,  28,  24,   0,   0}, // 1 //
	{   0,   0,   0,   0, 127,  99,   3,   6,  12,  24,  48,  96,  99,  62,   0,   0}, // 2 //
	{   0,   0,   0,   0,  62,  99,  96,  96,  96,  60,  96,  96,  99,  62,   0,   0}, // 3 //
	{   0,   0,   0,   0, 120,  48,  48,  48, 127,  51,  54,  60,  56,  48,   0,   0}, // 4 //
	{   0,   0,   0,   0,  62,  99,  96,  96, 112,  63,   3,   3,   3, 127,   0,   0}, // 5 //
	{   0,   0,   0,   0,  62,  99,  99,  99,  99,  63,   3,   3,   6,  28,   0,   0}, // 6 //
	{   0,   0,   0,   0,  12,  12,  12,  12,  24,  48,  96,  96,  99, 127,   0,   0}, // 7 //
	{   0,   0,   0,   0,  62,  99,  99,  99,  99,  62,  99,  99,  99,  62,   0,   0}, // 8 //
	{   0,   0,   0,   0,  30,  48,  96,  96,  96, 126,  99,  99,  99,  62,   0,   0}, // 9 //
	{   0,   0,   0,   0,   0,  24,  24,   0,   0,   0,  24,  24,   0,   0,   0,   0}, // : //
	{   0,   0,   0,   0,  12,  24,  24,   0,   0,   0,  24,  24,   0,   0,   0,   0}, // ; //
	{   0,   0,   0,   0,  96,  48,  24,  12,   6,  12,  24,  48,  96,   0,   0,   0}, // < //
	{   0,   0,   0,   0,   0,   0, 127,   0,   0, 127,   0,   0,   0,   0,   0,   0}, // = //
	{   0,   0,   0,   0,   6,  12,  24,  48,  96,  48,  24,  12,   6,   0,   0,   0}, // > //
	{   0,   0,   0,   0,  24,  24,   0,  24,  24,  24,  48,  99,  99,  62,   0,   0}, // ? //
	{   0,   0,   0,   0,  62,   3,  59, 123, 123, 123,  99,  99,  62,   0,   0,   0}, // @ //
	{   0,   0,   0,   0,  99,  99,  99,  99, 127,  99,  99,  54,  28,   8,   0,   0}, // A //
	{   0,   0,   0,   0,  63, 102, 102, 102, 102,  62, 102, 102, 102,  63,   0,   0}, // B //
	{   0,   0,   0,   0,  60, 102,  67,   3,   3,   3,   3,  67, 102,  60,   0,   0}, // C //
	{   0,   0,   0,   0,  31,  54, 102, 102, 102, 102, 102, 102,  54,  31,   0,   0}, // D //
	{   0,   0,   0,   0, 127, 102,  70,   6,  22,  30,  22,  70, 102, 127,   0,   0}, // E //
	{   0,   0,   0,   0,  15,   6,   6,   6,  22,  30,  22,  70, 102, 127,   0,   0}, // F //
	{   0,   0,   0,   0,  92, 102,  99,  99, 123,   3,   3,  67, 102,  60,   0,   0}, // G //
	{   0,   0,   0,   0,  99,  99,  99,  99,  99, 127,  99,  99,  99,  99,   0,   0}, // H //
	{   0,   0,   0,   0,  60,  24,  24,  24,  24,  24,  24,  24,  24,  60,   0,   0}, // I //
	{   0,   0,   0,   0,  30,  51,  51,  51,  48,  48,  48,  48,  48, 120,   0,   0}, // J //
	{   0,   0,   0,   0, 103, 102, 102,  54,  30,  30,  54,  54, 102, 103,   0,   0}, // K //
	{   0,   0,   0,   0, 127, 102,  70,   6,   6,   6,   6,   6,   6,  15,   0,   0}, // L //
	{   0,   0,   0,   0,  99,  99,  99,  99,  99, 107, 127, 127, 119,  99,   0,   0}, // M //
	{   0,   0,   0,   0,  99,  99,  99,  99, 115, 123, 127, 111, 103,  99,   0,   0}, // N //
	{   0,   0,   0,   0,  28,  54,  99,  99,  99,  99,  99,  99,  54,  28,   0,   0}, // O //
	{   0,   0,   0,   0,  15,   6,   6,   6,   6,  62, 102, 102, 102,  63,   0,   0}, // P //
	{   0,   0, 112,  48,  62, 123, 107,  99,  99,  99,  99,  99,  99,  62,   0,   0}, // Q //
	{   0,   0,   0,   0, 103, 102, 102, 102,  54,  62, 102, 102, 102,  63,   0,   0}, // R //
	{   0,   0,   0,   0,  62,  99,  99,  96,  48,  28,   6,  99,  99,  62,   0,   0}, // S //
	{   0,   0,   0,   0,  60,  24,  24,  24,  24,  24,  24,  90, 126, 126,   0,   0}, // T //
	{   0,   0,   0,   0,  62,  99,  99,  99,  99,  99,  99,  99,  99,  99,   0,   0}, // U //
	{   0,   0,   0,   0,   8,  28,  54,  99,  99,  99,  99,  99,  99,  99,   0,   0}, // V //
	{   0,   0,   0,   0,  54,  54, 127, 107, 107,  99,  99,  99,  99,  99,   0,   0}, // W //
	{   0,   0,   0,   0,  99,  99,  54,  54,  28,  28,  54,  54,  99,  99,   0,   0}, // X //
	{   0,   0,   0,   0,  60,  24,  24,  24,  24,  60, 102, 102, 102, 102,   0,   0}, // Y //
	{   0,   0,   0,   0, 127,  99,  67,   6,  12,  24,  48,  97,  99, 127,   0,   0}, // Z //
	{   0,   0,   0,   0,  60,  12,  12,  12,  12,  12,  12,  12,  12,  60,   0,   0}, // [ //
	{   0,   0,   0,   0,  64,  96, 112,  56,  28,  14,   7,   3,   1,   0,   0,   0}, // \ //
	{   0,   0,   0,   0,  60,  48,  48,  48,  48,  48,  48,  48,  48,  60,   0,   0}, // ] //
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99,  54,  28,   8}, // ^ //
	{   0,   0, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, // _ //
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  24,  12,  12}, // ` //
	{   0,   0,   0,   0, 110,  51,  51,  51,  62,  48,  30,   0,   0,   0,   0,   0}, // a //
	{   0,   0,   0,   0,  59, 102, 102, 102, 102,  54,  30,   6,   6,   7,   0,   0}, // b //
	{   0,   0,   0,   0,  62,  99,   3,   3,   3,  99,  62,   0,   0,   0,   0,   0}, // c //
	{   0,   0,   0,   0, 110,  51,  51,  51,  51,  54,  60,  48,  48,  56,   0,   0}, // d //
	{   0,   0,   0,   0,  62,  99,   3,   3, 127,  99,  62,   0,   0,   0,   0,   0}, // e //
	{   0,   0,   0,   0,  15,   6,   6,   6,   6,  15,   6,  38,  54,  28,   0,   0}, // f //
	{   0,  30,  51,  48,  62,  51,  51,  51,  51,  51, 110,   0,   0,   0,   0,   0}, // g //
	{   0,   0,   0,   0, 103, 102, 102, 102, 102, 110,  54,   6,   6,   7,   0,   0}, // h //
	{   0,   0,   0,   0,  60,  24,  24,  24,  24,  24,  28,   0,  24,  24,   0,   0}, // i //
	{   0,  60, 102, 102,  96,  96,  96,  96,  96,  96, 112,   0,  96,  96,   0,   0}, // j //
	{   0,   0,   0,   0, 103, 102,  54,  30,  30,  54, 102,   6,   6,   7,   0,   0}, // k //
	{   0,   0,   0,   0,  60,  24,  24,  24,  24,  24,  24,  24,  24,  28,   0,   0}, // l //
	{   0,   0,   0,   0, 107, 107, 107, 107, 107, 127,  55,   0,   0,   0,   0,   0}, // m //
	{   0,   0,   0,   0, 102, 102, 102, 102, 102, 102,  59,   0,   0,   0,   0,   0}, // n //
	{   0,   0,   0,   0,  62,  99,  99,  99,  99,  99,  62,   0,   0,   0,   0,   0}, // o //
	{   0,  15,   6,   6,  62, 102, 102, 102, 102, 102,  59,   0,   0,   0,   0,   0}, // p //
	{   0, 120,  48,  48,  62,  51,  51,  51,  51,  51, 110,   0,   0,   0,   0,   0}, // q //
	{   0,   0,   0,   0,  15,   6,   6,   6,  70, 110,  59,   0,   0,   0,   0,   0}, // r //
	{   0,   0,   0,   0,  62,  99,  48,  28,   6,  99,  62,   0,   0,   0,   0,   0}, // s //
	{   0,   0,   0,   0,  56, 108,  12,  12,  12,  12,  63,  12,  12,   8,   0,   0}, // t //
	{   0,   0,   0,   0, 110,  51,  51,  51,  51,  51,  51,   0,   0,   0,   0,   0}, // u //
	{   0,   0,   0,   0,  24,  60, 102, 102, 102, 102, 102,   0,   0,   0,   0,   0}, // v //
	{   0,   0,   0,   0,  54, 127, 107, 107,  99,  99,  99,   0,   0,   0,   0,   0}, // w //
	{   0,   0,   0,   0,  99,  54,  28,  28,  28,  54,  99,   0,   0,   0,   0,   0}, // x //
	{   0,  31,  48,  96, 126,  99,  99,  99,  99,  99,  99,   0,   0,   0,   0,   0}, // y //
	{   0,   0,   0,   0, 127,  99,   6,  12,  24,  51, 127,   0,   0,   0,   0,   0}, // z //
	{   0,   0,   0,   0, 112,  24,  24,  24,  24,  14,  24,  24,  24, 112,   0,   0}, // { //
	{   0,   0,   0,   0,  24,  24,  24,  24,  24,   0,  24,  24,  24,  24,   0,   0}, // | //
	{   0,   0,   0,   0,  14,  24,  24,  24,  24, 112,  24,  24,  24,  14,   0,   0}, // } //
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  59, 110,   0,   0}, // ~ //
};

// cDIB::Char
void cDIB::Char(int c, int x, int y, COLORREF Color) {
	if(c < 32 || c > 126) return;
	if(!RangeCheck(x, 0, int(m_bmi.bmiHeader.biWidth) - 8) ||
		!RangeCheck(y, 0, int(m_bmi.bmiHeader.biHeight) - 16)) return;

	BYTE r = GetRValue(Color), g = GetGValue(Color), b = GetBValue(Color);
	BYTE *pRow = Chars8x16[c - 32];
	long index = m_nBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 3 * x;
	BYTE *pBitsRow = &m_pBits[index];
	for(int iy = 0; iy < 16; iy++) {
		BYTE *pCurPixel = pBitsRow;
		for(int ix = 0; ix < 8; ix++) {
			if(*pRow & (1 << ix)) {
				pCurPixel[0] = b, pCurPixel[1] = g, pCurPixel[2] = r;
			}
			pCurPixel += 3;
		}
		pRow++;
		pBitsRow -= m_nBytesPerScanLine;
	}

/*	if(c < 32 || c > 126) return;
	if(!RangeCheck(x, -7, int(m_bmi.bmiHeader.biWidth) - 1) ||
		!RangeCheck(y, -15, int(m_bmi.bmiHeader.biHeight) - 1)) return;
	const int sx = x >= 0 ? 0 : - x;
	const int sy = y >= 0 ? 0 : - y;

	const int dx = Min(int(m_bmi.bmiHeader.biWidth) - x, 8);
	const int dy = Min(int(m_bmi.bmiHeader.biHeight) - y, 16);

	BYTE r = GetRValue(Color), g = GetGValue(Color), b = GetBValue(Color);
	BYTE *pRow = &Chars8x16[c - 32][sy];
	long index = m_nBytesPerScanLine * (m_bmi.bmiHeader.biHeight - (y + sy) - 1) + 3 * (x + sx);
	BYTE *pBitsRow = &m_pBits[index];
	for(int iy = sy; iy < dy; iy++) {
		BYTE *pCurPixel = pBitsRow;
		for(int ix = sx; ix < dx; ix++) {
			if(*pRow & (1 << ix)) {
				pCurPixel[0] = b, pCurPixel[1] = g, pCurPixel[2] = r;
			}
			pCurPixel += 3;
		}
		pRow++;
		pBitsRow -= m_nBytesPerScanLine;
	}*/
} // cDIB::Char

// cDIB::Str
void cDIB::Str(const cStr &Str, int x, int y) {
	cColor Color = cStr::ColorForIndex(0);
	int xCur = x;
	const char *pStr = Str.ToChar();
	do {
		if(cStr::IsColor(pStr, Color)) {
			pStr += 2;
			continue;
		}
		int c = *pStr++;
		if(!c) break;
		Char(c, xCur, y, Color.ToAbgr());
		xCur += 8;
	} while(true);
} // cDIB::Str

// cDIB::StrCenter
void cDIB::StrCenter(const cStr &Str, int cx, int cy) {
	int Lx = 8 * Str.LengthWithoutColors(), Ly = 16;
	cDIB::Str(Str, cx - Lx / 2, cy - Ly / 2);
} // cDIB::StrCenter

// cDIB::StrCenter
void cDIB::StrCenter(const cStr &Str, const cVector &Center) {
	StrCenter(Str, (int)Center.x, (int)Center.y);
} // cDIB::StrCenter

//-----------------------------------------------------------------------------
// cDIB::SaveBMP : bool(const char *)
//-----------------------------------------------------------------------------
bool cDIB::SaveBMP(const char *pFileName) {
	assert(m_pBits);
	
	HANDLE hf = CreateFile(pFileName, GENERIC_READ | GENERIC_WRITE,
		(DWORD)0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if(INVALID_HANDLE_VALUE == hf) {
		return false;
	}
	
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

//-----------------------------------------------------------------------------
// ConvertFrom32To24
//-----------------------------------------------------------------------------
void cDIB::ConvertFrom32To24() {
	if(m_bmi.bmiHeader.biBitCount != 32) return;

	m_bmi.bmiHeader.biBitCount = 24;
	int nOldBytesPerScanLine = m_nBytesPerScanLine;
	m_nBytesPerScanLine = 3 * m_bmi.bmiHeader.biWidth;
	int mod = m_nBytesPerScanLine % 4;
	if(mod) m_nBytesPerScanLine += 4 - mod;
	m_bmi.bmiHeader.biSizeImage = m_nBytesPerScanLine * m_bmi.bmiHeader.biHeight;
	
	BYTE *p32Bits = m_pBits;
	
	m_pBits = new BYTE[m_bmi.bmiHeader.biSizeImage];
	assert(m_pBits);
	
	COLORREF Clr;
	for(int y = 0; y < m_bmi.bmiHeader.biHeight; y++) {
		for(int x = 0; x < m_bmi.bmiHeader.biWidth; x++) {
			long index = nOldBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 4 * x;
			Clr = RGB(p32Bits[index + 2], p32Bits[index + 1], p32Bits[index]);
			SetPixel(x, y, Clr);
		}
	}
	delete[] p32Bits;
	p32Bits = NULL;
}

//-----------------------------------------------------------------------------
// cDIB::LoadBMP : bool(const char *)
//-----------------------------------------------------------------------------
bool cDIB::LoadBMP(const char *pFileName) {
	Free();

	HANDLE hf = CreateFile(pFileName, GENERIC_READ, (DWORD)0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	
	if(INVALID_HANDLE_VALUE == hf) {
		return false;
	}

	bool fSuccess = false;

	BITMAPFILEHEADER hdr;
	ZeroMemory(&hdr, sizeof(BITMAPFILEHEADER));
	DWORD nBytesRead;
	ReadFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&nBytesRead, NULL);
	if(nBytesRead == sizeof(BITMAPFILEHEADER)) {
		if(0x4d42 == hdr.bfType) {
			ReadFile(hf, (LPVOID)&m_bmi.bmiHeader, sizeof(BITMAPINFOHEADER), (LPDWORD)&nBytesRead, NULL);
			if(nBytesRead == sizeof(BITMAPINFOHEADER)) {
				// Only 24 or 32 bits per pixel!
				if(m_bmi.bmiHeader.biBitCount == 24 || m_bmi.bmiHeader.biBitCount == 32) {
					m_nBytesPerScanLine = m_bmi.bmiHeader.biBitCount / 8 * m_bmi.bmiHeader.biWidth;
					int mod = m_nBytesPerScanLine % 4;
					if(mod) m_nBytesPerScanLine += 4 - mod;
					m_bmi.bmiHeader.biSizeImage = m_nBytesPerScanLine * m_bmi.bmiHeader.biHeight;

					m_pBits = new BYTE[m_bmi.bmiHeader.biSizeImage];
					assert(m_pBits);
					
					ReadFile(hf, (LPSTR)m_pBits, m_bmi.bmiHeader.biSizeImage, (LPDWORD)&nBytesRead, NULL);
					if(m_bmi.bmiHeader.biSizeImage == nBytesRead) {
						// We don't need alpha:
						if(m_bmi.bmiHeader.biBitCount == 32) {
							ConvertFrom32To24();
						}
						fSuccess = true;
					}
				}
			}
		}
	}
	CloseHandle(hf);

	if(!fSuccess) {
		Free();
	}

	return fSuccess;
}

//-----------------------------------------------------------------------------
// cDIB::Copy : cDIB & (const cDIB &)
//-----------------------------------------------------------------------------
cDIB & cDIB::Copy(const cDIB &Src) {
	Free();
	CopyMemory(&m_bmi, &Src.m_bmi, sizeof(m_bmi));
	m_nBytesPerScanLine = Src.m_nBytesPerScanLine;
	if(Src.m_pBits) {
		m_pBits = new BYTE[Src.m_bmi.bmiHeader.biSizeImage];
		assert(m_pBits);
		CopyMemory(m_pBits, Src.m_pBits, Src.m_bmi.bmiHeader.biSizeImage);
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cDIB::Scale
//-----------------------------------------------------------------------------
void cDIB::Scale(int s) {
	cDIB dib(Lx() / s, Ly() / s);
	for(int y = 0; y < dib.Ly(); y++) {
		for(int x = 0; x < dib.Lx(); x++) {
			DWORD C = GetBlot(s * x, s * y, s);
			dib.SetPixel(x, y, C);
		}
	}
	Copy(dib);
}

//-----------------------------------------------------------------------------
// cDIB::Paste
//-----------------------------------------------------------------------------
cDIB & cDIB::Paste(int WhereX, int WhereY, const cDIB &Src) {
	for(int y = WhereY; y < Min(Ly(), WhereY + Src.Ly()); y++) {
		for(int x = WhereX; x < Min(Lx(), WhereX + Src.Lx()); x++) {
			SetPixel(x, y, Src.GetPixel(x - WhereX, y - WhereY));
		}
	}
	return *this;
}

} // comms