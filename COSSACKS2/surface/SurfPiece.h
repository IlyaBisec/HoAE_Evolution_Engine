#ifndef __SurfPiece__
#define __SurfPiece__

#include "SelExt.h"

namespace Surface {

class SurfPiece {
public:
	static void Save(const char *pFileName, int x0, int y0, int x1, int y1);
	static void Load(const char *pFileName, int x, int y);
	static void Preview(const char *pFileName, int x, int y);
private:
	static bool SaveBuffer(const char *pFileName, cList<char> &Buffer);
	static bool LoadBuffer(const char *pFileName, cList<char> &Buffer);
	static void Tri2Buffer(const SurfVertex *t3, cList<char> &Buffer, int W, int H, int HdrSize);
};

} // Surface

#endif // __SurfPiece__