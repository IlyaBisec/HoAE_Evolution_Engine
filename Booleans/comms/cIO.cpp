#include "cIO.h"

namespace comms {

//-----------------------------------------------------------------------------------------
// Load
//-----------------------------------------------------------------------------------------
bool cIO::Load(const char *FileName, cRawMesh *pMesh, eFileType Type, cStr *pLog) {
	assert(FileName);
	FILE *pFile;
	if(NULL == (pFile = fopen(FileName, "r"))) {
		if(pLog) {
			if(pLog->Length()) pLog->Append("\n");
			pLog->Append("Could not open file for read \"");
			pLog->Append(FileName);
			pLog->Append("\".");
		}
		return false;
	}

	assert(pMesh);
	pMesh->Free();

	bool fSuccess = false;
	switch(Type) {
		case GTS:
			fSuccess = LoadGTS(pFile, pMesh);
			break;
		case X:
			fSuccess = LoadX(pFile, pMesh);
			break;
		case Obj:
			fSuccess = LoadObj(pFile, pMesh, pLog);
			break;
		default:
			assert(0);
	}

	if(!fSuccess) pMesh->Free();
	if(fclose(pFile)) {
		if(pLog) {
			if(pLog->Length()) pLog->Append("\n");
			pLog->Append("Could not close file \"");
			pLog->Append(FileName);
			pLog->Append("\".");
		}
		return false;
	}
	return fSuccess;
}

//----------------------------------------------------------------------------------------------
// Save
//----------------------------------------------------------------------------------------------
bool cIO::Save(const char *FileName, const cRawMesh &Mesh, eFileType Type, cStr *pLog) {
	assert(FileName);
	FILE *pFile;
	if(NULL == (pFile = fopen(FileName, "w"))) {
		if(pLog) {
			if(pLog->Length()) pLog->Append("\n");
			pLog->Copy("Could not open file for write \"");
			pLog->Append(FileName);
			pLog->Append("\".");
		}
		return false;
	}

	bool fSuccess = false;
	switch(Type) {
		case X:
			fSuccess = SaveX(pFile, Mesh);
			break;
		case Obj:
			fSuccess = SaveObj(pFile, Mesh);
			break;
		default:
			assert(0);
	}

	if(fclose(pFile)) {
		if(pLog) {
			if(pLog->Length()) pLog->Append("\n");
			pLog->Append("Could not close file \"");
			pLog->Append(FileName);
			pLog->Append("\".");
		}
		return false;
	}
	return fSuccess;
}

//-----------------------------------------------------------------------------
// LoadGTS
//-----------------------------------------------------------------------------
bool cIO::LoadGTS(FILE *pFile, cRawMesh *pMesh) {
	// Info:
	int nVerts, nEdges, nTris;
	if(3 != fscanf(pFile, "%d%d%d", &nVerts, &nEdges, &nTris)) return false;
	// -> Verts:
	float x, y, z;
	for(int i = 0; i < nVerts; i++) {
		if(3 != fscanf(pFile, "%f%f%f", &x, &y, &z)) return false;
		pMesh->m_Verts.Add(cVector(x, y, z));
	}
	// -> Edges:
	int i0, i1;
	for(i = 0; i < nEdges; i++) {
		if(2 != fscanf(pFile, "%d%d", &i0, &i1)) return false;
		pMesh->m_Edges.Add(i0 - 1);
		pMesh->m_Edges.Add(i1 - 1);
	}
	// -> Tris:
	int i2;
	for(i = 0; i < nTris; i++) {
		if(3 != fscanf(pFile, "%d%d%d", &i0, &i1, &i2)) return false;
		pMesh->m_Tris.Add(i0 - 1);
		pMesh->m_Tris.Add(i1 - 1);
		pMesh->m_Tris.Add(i2 - 1);
	}
	pMesh->m_Type = RMTTEdgesTris;
	return true;
}

//-----------------------------------------------------------------------------
// LoadX
//-----------------------------------------------------------------------------
bool cIO::LoadX(FILE *pFile, cRawMesh *pMesh) {
	// Magic:
	char Buffer[128];
	if(1 != fread(Buffer, 16, 1, pFile)) return false;
	if(memcmp("xof 0303txt 0032", Buffer, 16)) return false;

	// Looking for "template Mesh":
	while(true) {
		if(NULL == fgets(Buffer, sizeof(Buffer), pFile)) return false;
		if(0 == strcmp(Buffer, "Mesh {\n")) {
			/*
			template Mesh {
			DWORD nVertices;
			array Vector vertices[nVertices];
			DWORD nFaces;
			array MeshFace faces[nFaces];
			[...]
			}
			*/
			// DWORD nVertices;
			int nVerts;
			if(1 != fscanf(pFile, "%d;", &nVerts)) return false;
			/*
			template Vector {
			FLOAT x;
			FLOAT y;
			FLOAT z;
			}
			*/
			// array Vector vertices[nVertices];
			float x, y, z;
			for(int i = 0; i < nVerts; i++) {
				if(i) {
					if(0 != fscanf(pFile, ",")) return false;
				}
				if(3 != fscanf(pFile, "%f;%f;%f;", &x, &y, &z)) return false;
				pMesh->m_Verts.Add(cVector(x, y, z));
			}
			if(0 != fscanf(pFile, ";")) return false;
			// DWORD nFaces;
			int nTris;
			if(1 != fscanf(pFile, "%d;", &nTris)) return false;
			/*
			template MeshFace {
			DWORD nFaceVertexIndices;
			array DWORD faceVertexIndices[nFaceVertexIndices];
			}
			*/
			// array MeshFace faces[nFaces];
			int n, i0, i1, i2;
			for(i = 0; i < nTris; i++) {
				if(i) {
					if(0 != fscanf(pFile, ",")) return false;
				}
				if(4 != fscanf(pFile, "%d;%d,%d,%d;", &n, &i0, &i1, &i2)) return false;
				if(3 != n) return false;
				pMesh->m_Tris.Add(i0);
				pMesh->m_Tris.Add(i1);
				pMesh->m_Tris.Add(i2);
			}
			if(0 != fscanf(pFile, ";")) return false;
			break;
		}
	}
	pMesh->m_Type = RMTTTris;
	return true;
}

//-----------------------------------------------------------------------------
// SaveX
//-----------------------------------------------------------------------------
bool cIO::SaveX(FILE *pFile, const cRawMesh &Mesh) {
	// Header:
	fputs("xof 0303txt 0032\nMesh {\n", pFile);
	// nVerts:
	int nVerts = Mesh.m_Verts.Count();
	fprintf(pFile, " %d;\n", nVerts);
	// Verts:
	for(int i = 0; i < nVerts; i++) {
		if(i) fputs(",\n", pFile);
		const cVector &u = Mesh.m_Verts[i];
		fprintf(pFile, " %f;%f;%f;", u.x, u.y, u.z);
	}
	fputs(";\n", pFile);
	// nTris:
	int nTris = Mesh.m_Tris.Count() / 3;
	fprintf(pFile, " %d;\n", nTris);
	// Tris:
	int i0, i1, i2;
	for(int nTri = 0; nTri < nTris; nTri++) {
		Mesh.GetTri(nTri, i0, i1, i2);
		if(nTri) fputs(",\n", pFile);
		fprintf(pFile, " 3;%d,%d,%d;", i0, i1, i2);
	}
	fputs(";\n", pFile);
	// End:
	fputs("}", pFile);
	return true;
}

const int ioBufferSize = 4096;
char ioBuffer[ioBufferSize], ioBackBuffer[ioBufferSize];

//-----------------------------------------------------------------------------
// ObjLine
//-----------------------------------------------------------------------------
int cIO::ObjLine(FILE *pFile) {
	char *Str = fgets(ioBuffer, ioBufferSize, pFile);
	if(NULL == Str) return -1; // EOF

	ioBuffer[ioBufferSize - 2] = ' ';
	ioBuffer[ioBufferSize - 1] = '\0';

	// Convert '\t' and '\n' into ' ':
	for(char *c = Str; *c != '\0'; c++) {
		if('\t' == *c) {
			*c = ' ';
		} else if('\n' == *c) {
			*c = ' ';
			break;
		}
	}

	// BackBuffer:
	strcpy(ioBackBuffer, ioBuffer);
	ioBackBuffer[strlen(ioBackBuffer) - 1] = '\0';

	// Is this comment line (first non-space is '#')?
	for(c = Str; *c != '\0'; c++) {
		if('#' == *c) {
			return 0; // comment
		} else if(*c != ' ') {
			break;
		}
	}
	
	// Cut off trailing comments:
	while(*c != '\0') {
		if('#' == *c) {
			*c++ = ' ';
			*c = '\0';
			break;
		}
		c++;
	}
	return 1;
}

//-----------------------------------------------------------------------------
// ObjWords
//-----------------------------------------------------------------------------
void cIO::ObjWords(cList<char *> *pWords){
	// Find the words in the line:
	char *c = ioBuffer;
	while(*c != '\0') {
		// Jump over leading spaces:
		while(' ' == *c) c++;
		// Break if we reach the end:
		if('\0' == *c) break;
		// Save pointer to beginning of word:
		pWords->Add(c);
		// Jump over non-spaces:
		while(*c != ' ') c++;
		// Place a null char here to mark the end of the word:
		*c++ = '\0';
	}
}

//------------------------------------------------------------------------------------
// ObjIndices
//------------------------------------------------------------------------------------
void cIO::ObjIndices(char *Word, int *pVertIndex, int *pTexIndex, int *pNormIndex) {
	char *None = " ";
	char *Tex = None;
	char *Norm = None;
	// Replace '/' with '\0' and setup Tex and Norm:
	for(char *c = Word; *c != '\0'; c++) {
		if('/' == *c) {
			if(None == Tex) Tex = c + 1;
			else Norm = c + 1;
			*c = '\0';
		}
	}
	*pVertIndex = atoi(Word);
	*pTexIndex = atoi(Tex);
	*pNormIndex = atoi(Norm);
}

//-----------------------------------------------------------------------------
// ObjFace
//-----------------------------------------------------------------------------
bool cIO::ObjFace(cList<char*> *pWords, cRawMesh *pMesh, cStr *pLog) {
	int VertIndex, TexIndex, NormIndex;
	pMesh->m_Raw.Add(pWords->Count());
	for(int i = 0; i < pWords->Count(); i++) {
		ObjIndices(pWords->GetAt(i), &VertIndex, &TexIndex, &NormIndex);
		if(VertIndex > 0) pMesh->m_Raw.Add(VertIndex - 1);
		else if(VertIndex < 0) pMesh->m_Raw.Add(pMesh->m_Verts.Count() + VertIndex);
		else {
			if(pLog) {
				if(pLog->Length()) pLog->Append("\n");
				pLog->Append("Zero indices not allowed: \"");
				pLog->Append(ioBackBuffer);
				pLog->Append("\".");
			}
			return false;
		}
		if(TexIndex != 0) {}
		if(NormIndex != 0) {}
	}
	return true;
}

//-----------------------------------------------------------------------------
// LoadObj
//-----------------------------------------------------------------------------
bool cIO::LoadObj(FILE *pFile, cRawMesh *pMesh, cStr *pLog) {
	while(true) {
		int r = ObjLine(pFile);
		if(-1 == r) break;
		if(0 == r) continue;

		cList<char *> Words;
		ObjWords(&Words);
		if(Words.Count() == 0) continue;

		if(cStr::Equals(Words[0], "v")) {
			if(Words.Count() < 4) {
				if(pLog) {
					if(pLog->Length()) pLog->Append("\n");
					pLog->Append("Too few coordinates: \"");
					pLog->Append(ioBackBuffer);
					pLog->Append("\".");
				}
				return false;
			}
			cVector u(atof(Words[1]), atof(Words[2]), atof(Words[3]));
			if(Words.Count() == 5) u /= atof(Words[4]);
			pMesh->m_Verts.Add(u);
		} else if(cStr::Equals(Words[0], "f")) {
			Words.RemoveAt(0);
			if(!ObjFace(&Words, pMesh, pLog)) return false;
		}
	}
	pMesh->m_Type = RMTTRaw;
	return true;
}

//-----------------------------------------------------------------------------
// SaveObj
//-----------------------------------------------------------------------------
bool cIO::SaveObj(FILE *pFile, const cRawMesh &Mesh) {
	fputs("# Wavefront OBJ file\n", pFile);
	// Verts:
	for(int i = 0; i < Mesh.m_Verts.Count(); i++) {
		const cVector &u = Mesh.m_Verts[i];
		fprintf(pFile, "\nv %f %f %f", u.x, u.y, u.z);
	}
	fprintf(pFile, "\n# vertices: %d\n", Mesh.m_Verts.Count());
	// Topology:
	switch(Mesh.m_Type) {
		case RMTTTris:
			int NTris;
			NTris = Mesh.m_Tris.Count() / 3;
			// Tris:
			int i0, i1, i2;
			for(int nTri = 0; nTri < NTris; nTri++) {
				Mesh.GetTri(nTri, i0, i1, i2);
				fprintf(pFile, "\nf %d %d %d", i0 + 1, i1 + 1, i2 + 1);
			}
			fprintf(pFile, "\n# tris: %d", NTris);
			break;
		default:
			assert(0);
	}
	return true;
}

} // comms