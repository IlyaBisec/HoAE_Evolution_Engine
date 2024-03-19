#include "comms.h"

//-----------------------------------------------------------------------------
// cMeshObj::Decode
//-----------------------------------------------------------------------------
cRawMesh * cMeshObj::Decode(const cData &Src) {
	if(Src.IsEof()) {
		return NULL;
	}
	cRawMesh *pMesh = new cRawMesh;
	pMesh->SetName(Src.GetFileName());
	
	if(!DecodeObj(Src, *pMesh)) {
		delete pMesh;
		pMesh = NULL;
	}
	return pMesh;
} // cMeshObj::Decode

//-----------------------------------------------------------------------------
// cMeshObj::Encode
//-----------------------------------------------------------------------------
void cMeshObj::Encode(const cRawMesh &Mesh, cData *To) {
	cAssert(To);
	if(!To) {
		return;
	}
	To->WriteStr("# Wavefront Obj file\r\n");
	To->WriteStr(cStr::Format("# %s\r\n", Mesh.GetName()));

	// Positions:
	for(cList<cVec3>::Iterator It = Mesh.GetPositions().GetIterator(); !It.End(); ++It) {
		To->WriteStr(cStr::Format("\r\nv %f %f %f", It->x, It->y, It->z));
	}
	To->WriteStr(cStr::Format("\r\n# %d positions\r\n", Mesh.GetPositions().Count()));

	// TexCoords:
	for(cList<cVec2>::Iterator It = Mesh.GetTexCoords().GetIterator(); !It.End(); ++It) {
		To->WriteStr(cStr::Format("\r\nvt %f %f", It->x, It->y));
	}
	To->WriteStr(cStr::Format("\r\n# %d texture coordinates\r\n", Mesh.GetTexCoords().Count()));

	// Normals:
	for(cList<cVec3>::Iterator It = Mesh.GetNormals().GetIterator(); !It.End(); ++It) {
		To->WriteStr(cStr::Format("\r\nvn %f %f %f", It->x, It->y, It->z));
	}
	To->WriteStr(cStr::Format("\r\n# %d normals\r\n", Mesh.GetNormals().Count()));

	// Raw:
	int index = 0;
	cStr Face;
	int NFaces = 0;
	const bool NoTexCoords = Mesh.GetTexCoords().Count() < 1;
	const bool NoNormals = Mesh.GetNormals().Count() < 1;
	while(index < Mesh.GetRaw().Count()) {
		const cVec3i &Info = Mesh.GetRaw()[index++];
		int nDeg = Info[0];
		const int idMtl = Info[1];
		Face = "\r\nf";
		while(nDeg--) {
			const cVec3i i0 = Mesh.GetRaw()[index++];
			if(NoTexCoords && NoNormals) {
				Face << cStr::Format(" %d", i0[0] + 1); // Only position.
			} else if(NoTexCoords && !NoNormals) {
				Face << cStr::Format(" %d//%d", i0[0] + 1, i0[2] + 1); // Position && Normal.
			} else if(!NoTexCoords && NoNormals) {
				Face << cStr::Format(" %d/%d", i0[0] + 1, i0[1] + 1); // Position && TexCoord.
			} else {
				Face << cStr::Format(" %d/%d/%d", i0[0] + 1, i0[1] + 1, i0[2] + 1); // Position, TexCoord, and Normal.
			}
		}
		To->WriteStr(Face);
		NFaces++;
	}
	To->WriteStr(cStr::Format("\r\n# %d faces\r\n", NFaces));
} // cMeshObj::Encode

//-----------------------------------------------------------------------------
// cMeshObj::DecodeObj
//-----------------------------------------------------------------------------
bool cMeshObj::DecodeObj(const cData &Src, cRawMesh &Mesh) {
	const cMaterial *DefMtls[] = {
		&cMaterial::Brass,
		&cMaterial::Chrome,
		&cMaterial::Emerald,
		&cMaterial::Copper,
		&cMaterial::Pewter,
		&cMaterial::Jade,
		&cMaterial::Pearl
	};
	// Default material:
	Mesh.GetMaterials().Add(cMaterial::Brass);
	Mesh.GetMaterials().GetBack().Name = "default";
	int idCurMtl = 0;

	cStr MaterialFileName;
	while(true) {
		const int r = LoadLine(Src);
		if(-1 == r) { // EOF
			break;
		}
		if(0 == r) { // Comment or empty string.
			continue;
		}

		if(cStr::Equals(m_Tokens[0], "mtllib")) {
			MaterialFileName = m_Tokens[1];
		} else if(cStr::Equals(m_Tokens[0], "v")) { // Position:
			if(m_Tokens.Count() < 4) {
				cLog::Warning("cMeshObj::DecodeObj(): Too few position coordinates: \"%s\".", m_Buffer);
				return false;
			}
			cVec3 Position(cStr::ToFloat(m_Tokens[1]), cStr::ToFloat(m_Tokens[2]), cStr::ToFloat(m_Tokens[3]));
			Mesh.GetPositions().Add(Position);
		} else if(cStr::Equals(m_Tokens[0], "vt")) { // TexCoord:
			if(m_Tokens.Count() < 3) {
				cLog::Warning("cMeshObj::DecodeObj(): Too few texture coordinates: \"%s\".", m_Buffer);
				return false;
			}
			cVec2 TexCoord(cStr::ToFloat(m_Tokens[1]), cStr::ToFloat(m_Tokens[2]));
			Mesh.GetTexCoords().Add(TexCoord);
		} else if(cStr::Equals(m_Tokens[0], "vn")) { // Normal:
			if(m_Tokens.Count() < 4) {
				cLog::Warning("cMeshObj::DecodeObj(): Too few normal coordinates: \"%s\".", m_Buffer);
				return false;
			}
			cVec3 Normal(cStr::ToFloat(m_Tokens[1]), cStr::ToFloat(m_Tokens[2]), cStr::ToFloat(m_Tokens[3]));
			Mesh.GetNormals().Add(Normal);
		} else if(cStr::Equals(m_Tokens[0], "f")) { // Face:
			if(m_Tokens.Count() < 4) {
				cLog::Warning("cMeshObj::DecodeObj(): Too few face indices: \"%s\".", m_Buffer);
				return false;
			}
			if(!AddFace(Mesh, idCurMtl)) {
				return false;
			}
		} else if(cStr::Equals(m_Tokens[0], "usemtl")) { // Material:
			if(m_Tokens.Count() > 1) { // There is a name token.
				// If raw is empty, usemtl was listed before any faces.
				// So previous default material is not needed at all.
				if(Mesh.GetRaw().IsEmpty()) {
					Mesh.GetMaterials().Clear();
					Mesh.GetMaterials().Add(cMaterial::Brass);
					Mesh.GetMaterials().GetBack().Name = m_Tokens[1];
					idCurMtl = 0;
				} else { // There are faces, that surely use previous materials.
					bool IsFound = false;
					for(int idMtl = 0; idMtl < Mesh.GetMaterials().Count(); idMtl++) {
						if(cStr::Equals(Mesh.GetMaterials()[idMtl].Name, m_Tokens[1])) {
							IsFound = true;
							idCurMtl = idMtl;
							break;
						}
					}
					if(!IsFound) {
						const N = sizeof(DefMtls) / sizeof(DefMtls[0]);
						idCurMtl = Mesh.GetMaterials().Count();
						Mesh.GetMaterials().Add(*DefMtls[idCurMtl % N]);
						Mesh.GetMaterials().GetBack().Name = m_Tokens[1];
					}
				}
			}
		}
	}
	return true;
} // cMeshObj::DecodeObj

//-----------------------------------------------------------------------------
// cMeshObj::LoadLine
//-----------------------------------------------------------------------------
int cMeshObj::LoadLine(const cData &Src) {
	if(!Src.ReadLine(m_Buffer)) {
		return -1; // EOF
	}
	// Parsing:
	cStr Str(m_Buffer);
	// Cut off comments:
	const int iSharp = Str.Find('#');
	if(iSharp > -1) {
		Str[iSharp] = '\0';
		Str.CapLength(iSharp);
	}
	Str.Split(m_Tokens, " \t");

	if(m_Tokens.Count() < 1) { // Comment or empty string.
		return 0;
	}

	return 1;
} // cMeshObj::LoadLine

//-----------------------------------------------------------------------------
// cMeshObj::GetIndices
//-----------------------------------------------------------------------------
const cVec3i cMeshObj::GetIndices(const cStr &Token) {
	cStr Str(Token);
	char *None = " ";
	char *TexCoord = None;
	char *Normal = None;
	// Replace '/' with '\0' and setup TexCoord and Normal:
	for(char *c = Str.ToCharPtr(); *c != '\0'; c++) {
		if('/' == *c) {
			if(None == TexCoord) {
				TexCoord = c + 1;
			} else {
				Normal = c + 1;
			}
			*c = '\0';
		}
	}
	return cVec3i(cStr::ToInt(Str), cStr::ToInt(TexCoord), cStr::ToInt(Normal));
} // cMeshObj::GetIndices

//-------------------------------------------------------------------------------
// cMeshObj::AddFace
//-------------------------------------------------------------------------------
bool cMeshObj::AddFace(cRawMesh &Mesh, const int idMtl) {
	// Face Info:
	Mesh.GetRaw().Add(cVec3i(m_Tokens.Count() - 1, idMtl, 0));
	// Indices:
	for(int i = 1; i < m_Tokens.Count(); i++) {
		cVec3i Index = GetIndices(m_Tokens[i]);
		// Position index:
		if(Index[0] > 0) {
			Index[0]--;
		} else if(Index[0] < 0) {
			Index[0] = Mesh.GetPositions().Count() + Index[0];
		} else {
			cLog::Warning("cMeshObj: Zero indices not allowed: \"%s\".", m_Buffer);
			return false;
		}
		// TexCoord index:
		if(Index[1] > 0) {
			Index[1]--;
		} else if(Index[1] < 0) {
			Index[1] = Mesh.GetTexCoords().Count() + Index[1];
		} else {
			// Zero index, i.e. no tex coord.
		}
		// Normal index:
		if(Index[2] > 0) {
			Index[2]--;
		} else if(Index[2] < 0) {
			Index[2] = Mesh.GetNormals().Count() + Index[2];
		} else {
			// Zero index, i.e. no normal.
		}
		Mesh.GetRaw().Add(Index);
	}
	return true;
} // cMeshObj::AddFace