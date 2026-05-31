#ifndef __Splitter__
#define __Splitter__

#include "comms\comms.h"

namespace Surface {

#define VERTEX VertexTS
#define vfVERTEX vfTS

class SurfRef;
class SurfVertex;

class Splitter {
public:
	~Splitter();

	void Free();
	void Split(const SurfRef &Ref, eSurfFx SFX = SFX_DEFAULT);
	void Draw();
	enum ModifyFlags {
		MF_OFFSET = 1 << 0,
		MF_AND = 1 << 1,
		MF_OR = 1 << 2,
		MF_SEA = 1 << 3,
		MF_TEXTURE = 1 << 4
	};
	void Modify(const SurfVertex &Offset, DWORD DiffuseAndFlag, DWORD DiffuseOrFlag, int idTexture, DWORD Flags);
	static void Modify(BaseMesh &bm, const SurfVertex &Offset, DWORD DiffuseAndFlag, DWORD DiffuseOrFlag, int idTexture, DWORD Flags);
//private:
	cList<BaseMesh *> m_Cache;
};

class reIndexingHashEntry {
public:
	int oi, MappingCode;
	inline reIndexingHashEntry() {
		oi = -1;
		MappingCode = -1;
	}
	inline bool Equals(const reIndexingHashEntry &E) const {
		return E.oi == oi && E.MappingCode == MappingCode;
	}
};

} // Surface

#endif // __Splitter__