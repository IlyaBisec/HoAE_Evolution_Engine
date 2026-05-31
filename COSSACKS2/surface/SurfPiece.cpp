#include "stdheader.h"
#include "SurfPiece.h"

namespace Surface {

//-----------------------------------------------------------------------------
// SurfPiece::SaveBuffer : bool(const char *, cList<char> &)
//-----------------------------------------------------------------------------
bool SurfPiece::SaveBuffer(const char *pFileName, cList<char> &Buffer) {
	ResFile rf = RRewrite(pFileName);
	if(rf != INVALID_HANDLE_VALUE) {
		if(Buffer.Count()) { RBlockWrite(rf, Buffer.ToPtr(), Buffer.Count()); }
		RClose(rf);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// SurfPiece::LoadBuffer : bool(const char *, cList<char> &)
//-----------------------------------------------------------------------------
bool SurfPiece::LoadBuffer(const char *pFileName, cList<char> &Buffer) {
	ResFile rf = RReset(pFileName);
	if(rf != INVALID_HANDLE_VALUE) {
		int Size = RFileSize(rf);
		Buffer.SetCount(Size);
		RBlockRead(rf, Buffer.ToPtr(), Size);
		RClose(rf);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------------------------
// SurfPiece::Tri2Buffer : void(const SurfVertex *, cList<char> &, int, int, int)
//------------------------------------------------------------------------------------------------
void SurfPiece::Tri2Buffer(const SurfVertex *t3, cList<char> &Buffer, int W, int H, int HdrSize) {
	const int x1 = t3[0].x, x2 = t3[1].x, x3 = t3[2].x;
	const int y1 = t3[0].y, y2 = t3[1].y, y3 = t3[2].y;

	// Block size:
	const int qx = 1, qy = 1;

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
				if(x >= 0 && x < W && y >= 0 && y < H) {
					dVector t0(t3[0].x, t3[0].y, t3[0].z);
					dVector t1(t3[1].x, t3[1].y, t3[1].z);
					dVector t2(t3[2].x, t3[2].y, t3[2].z);
					dVector N = (t1 - t0).Cross(t2 - t0);
					if(!cMath::IsZero(N.z)) {
						N.Normalize();
						double hx = x - x1, hy = y - y1;
						int z = - (N.x * hx + N.y * hy) / N.z + int(t3[0].z);
						Buffer[HdrSize + x + W * y] = z;
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

//-----------------------------------------------------------------------------
// SurfPiece::Save
//-----------------------------------------------------------------------------
void SurfPiece::Save(const char *pFileName, int x0, int y0, int x1, int y1) {
	int W = x1 - x0, H = y1 - y0;
	if(!W || !H) return;

	SurfRef Ref;
	g_UniversalMap.GetRefRect(Ref, x0, y0, x1, y1, SR_TRIS);
	if(!Ref.idTris.Count()) return;

	// Hdr:
	int Hdr[] = {'TRAP', 0, 0, W, H };
	cList<char> Buffer;
	Buffer.Append((char *)&Hdr, sizeof(Hdr));

	Buffer.Add(0, W * H);
	cDib24 dib(W, H);
	SurfVertex t[3];
	for(int iTri = 0; iTri < Ref.idTris.Count(); iTri++) {
		Ref.pOrig->GetTri(Ref.idTris[iTri], t);
		cMath::Swap(t[1], t[2]);
		for(int k = 0; k < 3; k++) {
			t[k].x -= x0;
			t[k].y -= y0;
		}
		Tri2Buffer(t, Buffer, W, H, 12);
	}

	dib.SaveBMP("c:\\1.bmp");

	SaveBuffer(pFileName, Buffer);
}

//-----------------------------------------------------------------------------
// SurfPiece::Load
//-----------------------------------------------------------------------------
void SurfPiece::Load(const char *pFileName, int x, int y) {
	cStr Info; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Info << pFileName << "\n@ " << x << ", " << y; // <<<<<<<<<<<<<<<<<<<<<<<
	MessageBox(g_hWnd, Info.ToCharPtr(), "LOAD:", 0); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	cList<char> Buffer;
	if(!LoadBuffer(pFileName, Buffer)) return;
}

//-----------------------------------------------------------------------------
// SurfPiece::Preview
//-----------------------------------------------------------------------------
void SurfPiece::Preview(const char *pFileName, int x, int y) {
	cStr Info; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Info << "PREVIEW: @ " << x << ", " << y; // <<<<<<<<<<<<<<<<<<<<<<<<<<
	ShowString(200, 200, Info.ToCharPtr(), &BigWhiteFont); // <<<<<<<<<<<<<<<<<<<<<<<<

	cList<char> Buffer;
	if(!LoadBuffer(pFileName, Buffer)) return;
}

} // Surface