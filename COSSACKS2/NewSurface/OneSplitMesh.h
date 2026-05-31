#ifndef __OneSplitMesh__
#define __OneSplitMesh__

#include "comms\comms.h"

#define VERTEX VertexTS
#define vfVERTEX vfTS

class VertexBasedMeshRef;
class OneMapVertex;
class OneSplitMesh {
private:
	cList<BaseMesh *> m_Cache;
public:
	~OneSplitMesh();

	void Free();
	bool Split(const VertexBasedMeshRef &Ref, int nStage = 0);
	void Draw();
	enum ModifyFlags {
		MF_OFFSET = 1 << 0,
		MF_AND = 1 << 1,
		MF_OR = 1 << 2,
		MF_SEA = 1 << 3,
		MF_FACTURE = 1 << 4
	};
	void Modify(const OneMapVertex &Offset, DWORD DiffuseAndFlag, DWORD DiffuseOrFlag, int nFacture, DWORD Flags);
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

#endif // __OneSplitMesh__