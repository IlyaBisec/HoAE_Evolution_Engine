#include "comms.h"

void cDib24::Clear(const cDib24::Pixel &p) {
	BYTE *Ptr = m_pBits;
	for(int y = 0; y < m_bmi.bmiHeader.biHeight; y++) {
		for(int x = 0; x < m_bmi.bmiHeader.biWidth; x++) {
			*Ptr++ = p.b;
			*Ptr++ = p.g;
			*Ptr++ = p.r;
		}
		Ptr += m_nBytesPerScanLine - 3 * m_bmi.bmiHeader.biWidth;
	}
}

//-----------------------------------------------------------------------------
// cDib24::GetPixel : cDib24::Pixel(const float, const float) const
//-----------------------------------------------------------------------------
cDib24::Pixel cDib24::GetPixel(const float s, const float t) const {
	float x = (float)GetWidth() * s - 0.5f;
	float y = (float)GetHeight() * t - 0.5f;
	int x0 = cMath::Clamp((int)cMath::Floor(x), 0, GetWidth() - 1);
	int y0 = cMath::Clamp((int)cMath::Floor(y), 0, GetHeight() - 1);
	int x1 = cMath::Clamp((int)cMath::Ceil(x), 0, GetWidth() - 1);
	int y1 = cMath::Clamp((int)cMath::Ceil(y), 0, GetHeight() - 1);
	Pixel p00 = GetPixel(x0, y0);
	Pixel p10 = GetPixel(x1, y0);
	Pixel p01 = GetPixel(x0, y1);
	Pixel p11 = GetPixel(x1, y1);
	float k00 = (1.0f - (x - x0)) * (1.0f - (y - y0));
	float k10 = (x - x0) * (1.0f - (y - y0));
	float k01 = (1.0f - (x - x0)) * (y - y0);
	float k11 = (x - x0) * (y - y0);
	Pixel p;
	p.r = (unsigned char)((float)p00.r * k00 + (float)p01.r * k01 + (float)p10.r * k10 + (float)p11.r * k11);
	p.g = (unsigned char)((float)p00.g * k00 + (float)p01.g * k01 + (float)p10.g * k10 + (float)p11.g * k11);
	p.b = (unsigned char)((float)p00.b * k00 + (float)p01.b * k01 + (float)p10.b * k10 + (float)p11.b * k11);
	return p;
}

void cDib24::Circle(int cx, int cy, int R, const cColor &C) {
	long r2 = (long)R * (long)R;
	long dst = 4 * r2;
	int dxt = int((double)R / 1.414213562373);
	long t = 0;
	long s = - 4 * r2 * (long)R;
	long e = (- s / 2) - 3 * r2;
	long ca = - 6 * r2;
	long cd = - 10 * r2;
	int x = 0;
	int y = R;
	cDib24::Pixel Px = cDib24::Pixel::FromColor(C);
	SetPixel(cx, cy + R, Px);
	SetPixel(cx, cy - R, Px);
	SetPixel(cx + R, cy, Px);
	SetPixel(cx - R, cy, Px);
	for(long indx = 1; indx <= dxt; indx++) {
		x++;
		if(e >= 0) e += t + ca;
		else {
			y--;
			e += t - s + cd;
			s += dst;
		}
		t -= dst;
		SetPixel(cx + x, cy + y, Px);
		SetPixel(cx + y, cy + x, Px);
		SetPixel(cx + y, cy - x, Px);
		SetPixel(cx + x, cy - y, Px);
		SetPixel(cx - x, cy - y, Px);
		SetPixel(cx - y, cy - x, Px);
		SetPixel(cx - y, cy + x, Px);
		SetPixel(cx - x, cy + y, Px);
	}
}

//-----------------------------------------------------------------------------
// cDib24::Round : void(int, int, int, const cColor &)
//-----------------------------------------------------------------------------
void cDib24::Round(int xCenter, int yCenter, int Radius, const cColor &Color) {
	int R2 = cMath::Square(Radius);
	Pixel Px = Pixel::FromColor(Color);
	for(int x = 0; x <= Radius; x++) {
		int r2 = cMath::Square(x);
		int dr2 = 0;
		for(int y = 0; y <= x; y++) {
			r2 += dr2;
			dr2 += 2;
			if(r2 >= R2) break;
			
			SetPixel(xCenter + x, yCenter + y, Px);
			SetPixel(xCenter - x, yCenter - y, Px);
			SetPixel(xCenter + x, yCenter - y, Px);
			SetPixel(xCenter - x, yCenter + y, Px);
			SetPixel(xCenter + y, yCenter + x, Px);
			SetPixel(xCenter - y, yCenter - x, Px);
			SetPixel(xCenter - y, yCenter + x, Px);
			SetPixel(xCenter + y, yCenter - x, Px);
		}
	}
}

void cDib24::Sphere(int cx, int cy, int R, const cColor &Clr) {
	int R2 = R * R;
	for(int x = 0; x <= R; x++) {
		int r2 = x * x;
		int dr2 = 0;
		for(int y = 0; y <= x; y++) {
			r2 += dr2;
			dr2 += 2;
			if(r2 >= R2) break;
			float k = 1.0f - float((r2 << 8) / R2) / 512.0f;
			
			cColor C = k * Clr;
			cDib24::Pixel p = cDib24::Pixel::FromColor(C);
			SetPixel(cx + x, cy + y, p);
			SetPixel(cx - x, cy - y, p);
			SetPixel(cx + x, cy - y, p);
			SetPixel(cx - x, cy + y, p);
			SetPixel(cx + y, cy + x, p);
			SetPixel(cx - y, cy - x, p);
			SetPixel(cx - y, cy + x, p);
			SetPixel(cx + y, cy - x, p);
		}
	}
}

// cDib24::Quad
void cDib24::Quad(const cVec2 &ul, const float d, const cDib24::Pixel &p) {
	Tri(ul, ul + cVec2(1.0f, 0.0f) * d, ul + cVec2(1.0f, 1.0f) * d, p);
	Tri(ul, ul + cVec2(1.0f, 1.0f) * d, ul + cVec2(0.0f, 1.0f) * d, p);
}

// cDib24::Tri
void cDib24::Tri(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, const cDib24::Pixel &p) {
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
	int xMin = (cMath::Min(x1, x2, x3) + 0xF) >> 4;
	int xMax = (cMath::Max(x1, x2, x3) + 0xF) >> 4;
	int yMin = (cMath::Min(y1, y2, y3) + 0xF) >> 4;
	int yMax = (cMath::Max(y1, y2, y3) + 0xF) >> 4;

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
				SetPixel(x, y, p);
			}
			Cx1 -= fDy12;
			Cx2 -= fDy23;
			Cx3 -= fDy31;
		}
		Cy1 += fDx12;
		Cy2 += fDx23;
		Cy3 += fDx31;
	}
} // cDib24::Tri

// cDib24::BlockTri
void cDib24::BlockTri(const cVec2 &t0, const cVec2 &t1, const cVec2 &t2, const cDib24::Pixel &p) {
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
	int xMin = (cMath::Min(x1, x2, x3) + 0xF) >> 4;
	int xMax = (cMath::Max(x1, x2, x3) + 0xF) >> 4;
	int yMin = (cMath::Min(y1, y2, y3) + 0xF) >> 4;
	int yMax = (cMath::Max(y1, y2, y3) + 0xF) >> 4;

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
						SetPixel(ix, iy, p);
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
							SetPixel(ix, iy, p);
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
} // cDib24::BlockTri

// cDib24::ZTri
void cDib24::ZTri(const cVec3 &t0, const cVec3 &t1, const cVec3 &t2, const cDib24::Pixel &p, int *pZBuffer) {
	int x0 = (int)t0.x, y0 = (int)t0.y, z0 = (int)t0.z;
	int x1 = (int)t1.x, y1 = (int)t1.y, z1 = (int)t1.z;
	int x2 = (int)t2.x, y2 = (int)t2.y, z2 = (int)t2.z;

	// Sorting ascending along y axis:
	if(y0 > y1) { cMath::Swap(x0, x1); cMath::Swap(y0, y1); cMath::Swap(z0, z1); }
	if(y0 > y2) { cMath::Swap(x0, x2); cMath::Swap(y0, y2); cMath::Swap(z0, z2); }
	if(y1 > y2) { cMath::Swap(x1, x2); cMath::Swap(y1, y2); cMath::Swap(z1, z2); }

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
			cMath::Swap(_x1, _x2);
			cMath::Swap(_z1, _z2);
		}

		// Rasterizing horizontal line (_x1 - _x2):
		for(int x = _x1; x < _x2; x++) {
			int z = _z1 + (x - _x1) * (_z2 - _z1) / (_x2 - _x1);
			if(x >= 0 && x < m_bmi.bmiHeader.biWidth && y >= 0 && y < m_bmi.bmiHeader.biHeight) {
				int index = y * m_bmi.bmiHeader.biWidth + x;
				if(pZBuffer[index] < z) {
					SetPixel(x, y, p);
					pZBuffer[index] = z;
				}
			}
		}
	}
} // cDib24::ZTri

void cDib24::DrawLine(const cVec2 &p0, const cVec2 &p1, const cDib24::Pixel &p) {
	Line((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, p);
}

void cDib24::Line(int x0, int y0, int x1, int y1, const cDib24::Pixel &p) {
	const int dx = abs(x1 - x0), dy = abs(y1 - y0);

	const int sx = x1 >= x0 ? 1 : -1;
	const int sy = y1 >= y0 ? 1 : -1;

	if(dy <= dx) {
		int d1 = dy << 1;
		int d = d1 - dx;
		int d2 = (dy - dx) << 1;
		SetPixel(x0, y0, p);
		for(int x = x0 + sx, y = y0, i = 1; i <= dx; i++, x += sx) {
			if(d > 0) {
				d += d2;
				y += sy;
			} else d += d1;
			SetPixel(x, y, p);
		}
	} else {
		int d1 = dx << 1;
		int d = d1 - dy;
		int d2 = (dx - dy) << 1;
		SetPixel(x0, y0, p);
		for(int x = x0, y = y0 + sy, i = 1; i <= dy; i++, y += sy) {
			if(d > 0) {
				d += d2;
				x += sx;
			} else d += d1;
			SetPixel(x, y, p);
		}
	}
}

namespace comms {

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

} // comms

int cDib24::GetStrWidth(const cStr &Str) const {
	return 8 * Str.LengthWithoutColors();
}

int cDib24::GetStrHeight() const {
	return 16;
}

// cDib24::Char
void cDib24::Char(int c, int x, int y, COLORREF Color) {
	if(c < 32 || c > 126) return;
	if(!cMath::IsInRange(x, 0, int(m_bmi.bmiHeader.biWidth) - 8) ||
		!cMath::IsInRange(y, 0, int(m_bmi.bmiHeader.biHeight) - 16)) return;

	BYTE r = GetRValue(Color), g = GetGValue(Color), b = GetBValue(Color);
	BYTE *pRow = comms::Chars8x16[c - 32] + 15;
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
		pRow--;
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
} // cDib24::Char

// cDib24::Str
void cDib24::Str(const cStr &Str, int x, int y) {
	cColor Color = cStr::ColorForIndex(0);
	int xCur = x;
	const char *pStr = Str.ToCharPtr();
	do {
		if(cStr::IsColor(pStr, Color)) {
			pStr += 2;
			continue;
		}
		int c = *pStr++;
		if(!c) break;
		Char(c, xCur, y, Color.ToRgba());
		xCur += 8;
	} while(true);
} // cDib24::Str

// cDib24::StrCenter
void cDib24::StrCenter(const cStr &Str, int cx, int cy) {
	int Lx = 8 * Str.LengthWithoutColors(), Ly = 16;
	cDib24::Str(Str, cx - Lx / 2, cy - Ly / 2);
} // cDib24::StrCenter

//-----------------------------------------------------------------------------
// cDib24::SaveBMP : bool(const char *)
//-----------------------------------------------------------------------------
bool cDib24::SaveBMP(const char *pFileName) {
	cAssert(m_pBits);
	
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

	return true;
}

//-----------------------------------------------------------------------------
// ConvertFrom32To24
//-----------------------------------------------------------------------------
void cDib24::ConvertFrom32To24() {
	if(m_bmi.bmiHeader.biBitCount != 32) return;

	m_bmi.bmiHeader.biBitCount = 24;
	int nOldBytesPerScanLine = m_nBytesPerScanLine;
	m_nBytesPerScanLine = 3 * m_bmi.bmiHeader.biWidth;
	int mod = m_nBytesPerScanLine % 4;
	if(mod) m_nBytesPerScanLine += 4 - mod;
	m_bmi.bmiHeader.biSizeImage = m_nBytesPerScanLine * m_bmi.bmiHeader.biHeight;
	
	BYTE *p32Bits = m_pBits;
	
	m_pBits = new BYTE[m_bmi.bmiHeader.biSizeImage];
	cAssert(m_pBits);
	
	COLORREF Clr;
	for(int y = 0; y < m_bmi.bmiHeader.biHeight; y++) {
		for(int x = 0; x < m_bmi.bmiHeader.biWidth; x++) {
			long index = nOldBytesPerScanLine * (m_bmi.bmiHeader.biHeight - y - 1) + 4 * x;
			Clr = RGB(p32Bits[index + 2], p32Bits[index + 1], p32Bits[index]);
			SetPixel(x, y, cDib24::Pixel::FromBgr(Clr));
		}
	}
	delete[] p32Bits;
	p32Bits = NULL;
}

//-----------------------------------------------------------------------------
// cDib24::LoadBMP : bool(const char *)
//-----------------------------------------------------------------------------
bool cDib24::LoadBMP(const char *pFileName) {
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
					cAssert(m_pBits);
					
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
// cDib24::Copy : cDib24 & (const cDib24 &)
//-----------------------------------------------------------------------------
cDib24 & cDib24::Copy(const cDib24 &Src) {
	Free();
	CopyMemory(&m_bmi, &Src.m_bmi, sizeof(m_bmi));
	m_nBytesPerScanLine = Src.m_nBytesPerScanLine;
	if(Src.m_pBits) {
		m_pBits = new BYTE[Src.m_bmi.bmiHeader.biSizeImage];
		cAssert(m_pBits);
		CopyMemory(m_pBits, Src.m_pBits, Src.m_bmi.bmiHeader.biSizeImage);
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cDib24::Scale
//-----------------------------------------------------------------------------
void cDib24::Scale(int s) {
	cDib24 dib(GetWidth() / s, GetHeight() / s);
	for(int y = 0; y < dib.GetHeight(); y++) {
		for(int x = 0; x < dib.GetWidth(); x++) {
			cDib24::Pixel C = GetBlot(s * x, s * y, s);
			dib.SetPixel(x, y, C);
		}
	}
	Copy(dib);
}

//-----------------------------------------------------------------------------
// cDib24::Paste
//-----------------------------------------------------------------------------
cDib24 & cDib24::Paste(int WhereX, int WhereY, const cDib24 &Src) {
	for(int y = WhereY; y < cMath::Min(GetHeight(), WhereY + Src.GetHeight()); y++) {
		for(int x = WhereX; x < cMath::Min(GetWidth(), WhereX + Src.GetWidth()); x++) {
			SetPixel(x, y, Src.GetPixel(x - WhereX, y - WhereY));
		}
	}
	return *this;
}

//-----------------------------------------------------------------------------
// cDib24::Sharp : void(const float)
//-----------------------------------------------------------------------------
void cDib24::Sharp(const float Sharpness) {
	cDib24 Buffer;
	Buffer.Copy(*this);
	for(int y = 1; y < GetHeight(); y++) {
		for(int x = 1; x < GetWidth(); x++) {
			cColor c = GetPixel(x, y).ToColor();
			cColor c00 = GetPixel(x - 1, y - 1).ToColor();
			cColor c01 = GetPixel(x + 1, y - 1).ToColor();
			cColor c10 = GetPixel(x - 1, y + 1).ToColor();
			cColor c11 = GetPixel(x + 1, y + 1).ToColor();
			cColor cc = (c00 + c01 + c10 + c11) / 4.0f;
			cColor r = (c - cc * Sharpness) / (1.0f - Sharpness);
			Buffer.SetPixel(x, y, cDib24::Pixel::FromColor(r));
		}
	}
	Copy(Buffer);
}

void cDib24::DrawLineHorFlat(int x0, int x1, int y, float z0, float z1, const cDib24::Pixel &p, float *pZBuffer) {
	if(y < 0 || y >= m_bmi.bmiHeader.biHeight) {
		return;
	}
	if(x0 == x1) {
		SetPixel(x0, y, p, z0, pZBuffer);
		return;
	}
	float k = (z1 - z0) / (float)(x1 - x0);
	if(x0 < x1) {
		for(int x = x0 + 1; x < x1; x++) {
			float z = z0 + (float)(x - x0) * k;
			SetPixel(x, y, p, z, pZBuffer);
		}
	}
	if(x0 > x1) {
		for(int x = x1 + 1; x < x0; x++) {
			float z = z0 + (float)(x - x0) * k;
			SetPixel(x, y, p, z, pZBuffer);
		}
	}
	SetPixel(x0, y, p, z0, pZBuffer);
	SetPixel(x1, y, p, z1, pZBuffer);
}

void cDib24::DrawPolygonFlat(const cVec3 *pVerts, const int Num, const cDib24::Pixel &p, float *pZBuffer) {
	int yMin = (int)pVerts[0].y;
	int yMax = (int)pVerts[0].y;

	for(int i = 1; i < Num; i++) {
		yMin = cMath::Min(yMin, (int)pVerts[i].y);
		yMax = cMath::Max(yMax, (int)pVerts[i].y);
	}
	float x[4], z[4];
	for(int y = yMin; y <= yMax; y++) {
		int nhor = 0;
		for(i = 0; i < Num; i++) {
			int st = i;
			int en = i + 1;
			if(en >= Num) {
				en = 0;
			}
			float x1 = pVerts[st].x;
			float y1 = pVerts[st].y;
			float z1 = pVerts[st].z;
			float x2 = pVerts[en].x;
			float y2 = pVerts[en].y;
			float z2 = pVerts[en].z;
			if(y >= y1 && y < y2 || y <= y1 && y > y2) {
				if(y1 != y2) {
					x[nhor] = x1 + (x2 - x1) * (float)(y - y1) / (float)(y2  - y1);
					z[nhor] = z1 + (z2 - z1) * (float)(y - y1) / (float)(y2 - y1);
					nhor++;
				}
			}
			if(nhor == 2) {
				DrawLineHorFlat((int)x[0], (int)x[1], y, z[0], z[1], p, pZBuffer);
			}
		}
	}
}

const cColor cDib24::CalcColor(const cMaterial &Mt, const float Dot) {
	return cColor::Clamp01(Mt.Ambient + Dot * Mt.Diffuse + Mt.Specular * cMath::Pow(Dot, Mt.Shininess));
}

//------------------------------------------------------------------------------------------------------------------------------------
// cDib24::DrawLineHorGouraud
//------------------------------------------------------------------------------------------------------------------------------------
void cDib24::DrawLineHorGouraud(int x0, int x1, int y, float z0, float z1, float d0, float d1, const cMaterial &Mt, float *pZBuffer) {
	if(y < 0 || y >= m_bmi.bmiHeader.biHeight) {
		return;
	}
	if(x0 == x1) {
		SetPixel(x0, y, cDib24::Pixel::FromColor(CalcColor(Mt, d0)), z0, pZBuffer);
		return;
	}
	float kz = (z1 - z0) / (float)(x1 - x0);
	float kd = (d1 - d0) / (float)(x1 - x0);
	if(x0 < x1) {
		for(int x = x0; x < x1; x++) {
			float z = z0 + (float)(x - x0) * kz;
			float d = d0 + (float)(x - x0) * kd;
			SetPixel(x, y, cDib24::Pixel::FromColor(CalcColor(Mt, d)), z, pZBuffer);
		}
	}
	if(x0 > x1) {
		for(int x = x1; x < x0; x++) {
			float z = z0 + (float)(x - x0) * kz;
			float d = d0 + (float)(x - x0) * kd;
			SetPixel(x, y, cDib24::Pixel::FromColor(CalcColor(Mt, d)), z, pZBuffer);
		}
	}
} // cDib24::DrawLineHorGouraud

//-----------------------------------------------------------------------------------------------------------------------------
// cDib24::DrawPolygonGouraud
//-----------------------------------------------------------------------------------------------------------------------------
void cDib24::DrawPolygonGouraud(const cVec3 *pVerts, const float *pDots, const int Num, const cMaterial &Mt, float *pZBuffer) {
	int yMin = (int)pVerts[0].y;
	int yMax = (int)pVerts[0].y;

	for(int i = 1; i < Num; i++) {
		yMin = cMath::Min(yMin, (int)pVerts[i].y);
		yMax = cMath::Max(yMax, (int)pVerts[i].y);
	}

	float x[4], z[4], d[4];
	for(int y = yMin; y <= yMax; y++) {
		int nhor = 0;
		for(i = 0; i < Num; i++) {
			int st = i;
			int en = i + 1;
			if(en >= Num) {
				en = 0;
			}
			float y0 = pVerts[st].y;
			float y1 = pVerts[en].y;
			if(y >= y0 && y < y1 || y <= y0 && y > y1) {
				if(y0 != y1) {
					float x0 = pVerts[st].x;
					float z0 = pVerts[st].z;
					float d0 = pDots[st];
					float x1 = pVerts[en].x;
					float z1 = pVerts[en].z;
					float d1 = pDots[en];
					x[nhor] = x0 + (x1 - x0) * ((float)y - y0) / (y1 - y0);
					z[nhor] = z0 + (z1 - z0) * ((float)y - y0) / (y1 - y0);
					d[nhor] = d0 + (d1 - d0) * ((float)y - y0) / (y1 - y0);
					nhor++;
				}
			}
			if(nhor == 2) {
				DrawLineHorGouraud((int)x[0], (int)x[1], y, z[0], z[1], d[0], d[1], Mt, pZBuffer);
			}
		}
	}
} // cDib24::DrawPolygonGouraud