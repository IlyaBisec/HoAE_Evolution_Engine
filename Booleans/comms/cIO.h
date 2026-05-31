#ifndef __cIO__
#define __cIO__

#include "cRawMesh.h"

namespace comms {

class cIO {
public:
	typedef enum { GTS, X, Obj } eFileType;

	static bool Load(const char *, cRawMesh *, eFileType, cStr * = NULL);
	static bool Save(const char *, const cRawMesh &, eFileType, cStr * = NULL);
private:
	static bool LoadGTS(FILE *, cRawMesh *);

	static bool LoadX(FILE *, cRawMesh *);
	static bool SaveX(FILE *, const cRawMesh &);

	// The .obj file tokens:
	// # some text
	//		Line is a comment until the end of the line.
	// v float float float
	//		A single vertex's geometric position in space. The first vertex listed in the file
	//		has index 1, and subsequent vertices are numbered sequentially.
	// vn float float float
	//		A normal. The first normal in the file has index 1,
	//		and subsequent normals are numbered sequentially.
	// vt float float
	//		A texture coordinate. The first texture coordinate in the file has index 1,
	//		and subsequent textures are numbered sequentially.
	// f int int int ...
	//		or
	// f int/int int/int int/int ...
	//		or
	// f int/int/int int/int/int int/int/int ...
	//		A polygonal face. The numbers are indexes into the arrays of vertex positions,
	//		texture coordinates, and normals respectively. A number may be ommited if,
	//		for example, texture coordinates are not being defined in the model.
	//		There is no maximum number of vertices that a single polygon may contain.
	//		The .obj file specification says that each face must be flat and convex.
	static bool LoadObj(FILE *, cRawMesh *, cStr *);
	static int ObjLine(FILE *);
	static void ObjWords(cList<char*> *);
	static void ObjIndices(char *, int *, int *, int *);
	static bool ObjFace(cList<char*> *, cRawMesh *, cStr *);
	static bool SaveObj(FILE *, const cRawMesh &);
};


} // comms

#endif // __cIO__