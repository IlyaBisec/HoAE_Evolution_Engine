#include "stdheader.h"
#include "SurfProcess.h"
#include <richedit.h>
#include "SurfExporter.h"
#include "..\fmm\fmmanager.h"
#include "IResourceManager.h"

Surface::SurfProcess g_SurfProcess;
extern FMManager MManager;
bool LoadHeader(ResFile);
DWORD RGetPos(ResFile, int);
char * GetTrueMapName1(char *);
void CompressFile(char *, char *);

void LoadNewSurface2(ResFile);
void LoadNewSurface3(ResFile);
void LoadNewSurface4(ResFile);
void LoadNewSurface5(ResFile);
void SaveNewSurface(ResFile);

typedef void (*LoadSurfaceFn)(ResFile);

struct LoadSurfaceFnEntry {
	const char *Magic;
	LoadSurfaceFn fn;
};

LoadSurfaceFnEntry g_SurfLoadMap[] = {
	{ "MSH2", LoadNewSurface2 },
	{ "MSH3", LoadNewSurface3 },
	{ "MSH4", LoadNewSurface4 },
	{ "MSH5", LoadNewSurface5 },
	{ NULL, NULL }
};

void LoadSomething(ResFile);
void SimplifySurfMesh(Surface::SurfMesh &);
void SaveSomething(ResFile, char *, char *);

namespace Surface {

typedef void (*SurfProcessFn)(PVOID);

struct SurfProcessMapEntry {
	const char *Name;
	SurfProcessFn fn;
	bool fOutputIsNeeded;
};

void SurfProcessSimplify(PVOID);
void SurfProcessOptimize(PVOID);
void SurfProcessRemoveGenInfo(PVOID);
void SurfProcessExtractGenInfo(PVOID);
void SurfProcessAppendGenInfo(PVOID);
void SurfProcessCopyBlockFromInputToOutput(cStr *Magic);
void SurfProcessCopyMapOptions(PVOID);

SurfProcessMapEntry g_SurfProcessMap[] = {
	{ "Simplify", SurfProcessSimplify, true },
	{ "Optimize", SurfProcessOptimize, true },
	{ "Remove Generator Info", SurfProcessRemoveGenInfo, true },
	{ "Extract Generator Info", SurfProcessExtractGenInfo, false },
	{ "Append Generator Info", SurfProcessAppendGenInfo, true },
	{ "Copy Map Options From INPUT to OUTPUT", SurfProcessCopyMapOptions, true },
	{ NULL, 0 }
};

const cStr SurfProcess::csDef("^B");
const cStr SurfProcess::csFileName("^I");
const cStr SurfProcess::csSize("^M");

//-----------------------------------------------------------------------------
// SurfProcess::FileInfo : int(const cStr &FileName, cStr &Info)
//-----------------------------------------------------------------------------
int SurfProcess::FileInfo(const cStr &FileName, cStr &Info) {
	assert(FileName.Length() < MAX_PATH);
	ResFile r = RReset(FileName.ToCharPtr());
	if(INVALID_HANDLE_VALUE == r) return -1;
	int nSize = RFileSize(r);
	RClose(r);

	float fSize = (float)nSize / 1024.f / 1024.f;
	
	Info << csFileName << FileName.ToCharPtr() << csDef << " (";
	Info << csSize << cStr::FromFloatArray(&fSize, 1) << csDef << " MB)";
	return nSize;
}

//-----------------------------------------------------------------------------
// SurfProcess::Pack : DWORD(const char *pc)
//-----------------------------------------------------------------------------
DWORD SurfProcess::Pack(const char *p) {
	return ((DWORD)(((BYTE)(*p) | ((WORD)((BYTE)(*(p + 1))) << 8)) |
		(((DWORD)(BYTE)(*(p + 2))) << 16) | ((DWORD)((BYTE)(*(p + 3))) << 24)));
}

//------------------------------------------------------------------------------------
// SurfProcess::ProcessStage : bool(eStage, void *, void *, void *)
//------------------------------------------------------------------------------------
bool SurfProcess::ProcessStage(eStage nStage, void *pParam, void *pData, void *pAux) {
	ResFile r;
	cStr Msg, FileName, Temp, *pStr = NULL;
	int nSize, *pi = NULL, i, k;
	DWORD sign, size, dw;
	float fSize;
	bool Flag;
	cList<char> *pCache = NULL;

	switch(nStage) {
		case S_BEGIN:
			g_SurfProcess.EnableLayout(false);
			g_SurfProcess.m_fActive = true;
			return true;
		case S_END:
			g_SurfProcess.Message(SurfProcess::csDef + "------------------------------------------------------------\n\n");
			g_SurfProcess.EnableLayout(true);
			g_SurfProcess.m_fActive = false;
			return true;
		case S_DECOMPRESS: // pParam - file name to decompress
						   // pData - decompressed file name
			assert(pParam && pData);
			FileName = *((cStr *)pParam);
			pStr = (cStr *)pData;
			
			Msg.Free();
			Msg << csDef << "> Decompressing ";
			if(FileInfo(FileName, Msg) < 0) return false;
			Msg << "...\n";
			g_SurfProcess.Message(Msg);

			*pStr = GetTrueMapName1(FileName.ToCharPtr());
			if(strcmp(pStr->ToCharPtr(), FileName.ToCharPtr()) == 0) {
				Msg.Free();
				Msg << csDef << "File is not compressed!\n";
				g_SurfProcess.Message(Msg);

				pStr->Copy("$temp1$.$$$");
				
				IRM->CheckFileAccess(pStr->ToCharPtr());
				CopyFile(FileName.ToCharPtr(), pStr->ToCharPtr(), false);
				SetFileAttributes(pStr->ToCharPtr(), FILE_ATTRIBUTE_NORMAL);
			}
			
			Msg.Free();
			Msg << csDef << "Created ";
			if(FileInfo(*pStr, Msg) < 0) return false;
			Msg << "\n";
			g_SurfProcess.Message(Msg);
			
			return true;
		case S_COMPRESS: // pParam - file to compress
						 // pData - compressed file name
			assert(pParam && pData);
			FileName = *((cStr *)pParam);
			Temp = *((cStr *)pData);
			
			Msg.Free();
			Msg << csDef << "> Compressing ";
			if(FileInfo(FileName, Msg) < 0) return false;
			Msg << "...\n";
			g_SurfProcess.Message(Msg);

			CompressFile(FileName.ToCharPtr(), Temp.ToCharPtr());
			
			Msg.Free();
			Msg << csDef << "Created ";
			if(FileInfo(Temp, Msg) < 0) return false;
			Msg << "\n";
			g_SurfProcess.Message(Msg);

			return true;
		case S_LOAD: // pParam - file to load
					 // pData - cache to load in
			assert(pParam && pData);
            FileName = *((cStr *)pParam);
			pCache = (cList<char> *)pData;
			pCache->Free();

			Msg.Free();
			Msg << csDef << "> Loading ";
			nSize = FileInfo(FileName, Msg);
			if(nSize < 0) return false;
			Msg << "...\n";
			g_SurfProcess.Message(Msg);

			if(nSize) {
				pCache->SetCount(nSize);
				r = RReset(FileName.ToCharPtr());
				fSize = (float)RBlockRead(r, pCache->ToPtr(), nSize) / 1024.f / 1024.f;
				RClose(r);
			} else fSize = 0.0f;

			Msg.Free();
			Msg << csDef << "Loaded " << csSize << cStr::FromFloat(fSize) << csDef << " MB\n";
			g_SurfProcess.Message(Msg);

			return true;
		case S_SAVE: // pParam - file to save in
					 // pData - cache to save from
			assert(pParam && pData);
			FileName = *((cStr *)pParam);
			pCache = (cList<char> *)pData;

			Msg.Free();
			Msg << csDef << "> Saving " << csFileName << FileName << csDef << "...\n";
			g_SurfProcess.Message(Msg);
			
			r = RRewrite(FileName.ToCharPtr());
			if(INVALID_HANDLE_VALUE == r) return false;
			fSize = (float)RBlockWrite(r, pCache->ToPtr(), pCache->Count()) / 1024.f / 1024.f;
			RClose(r);

			Msg.Free();
			Msg << csDef << "Saved " << csSize << cStr::FromFloat(fSize) << csDef << " MB\n";
			g_SurfProcess.Message(Msg);

			return true;
		case S_DELETEFILE: // pParam: file to find
						   // pData: cache to delete in
			assert(pParam && pData);
			int os[2];
			if(ProcessStage(S_FINDFILE, pParam, pData, os)) {
				cStr Msg;
				Msg << SurfProcess::csDef << "> Removing " << SurfProcess::csFileName << *((cStr *)pParam) << SurfProcess::csDef << "...\n";
				g_SurfProcess.Message(Msg);

				pCache = (cList<char> *)pData;
				pCache->RemoveAt(os[0], os[1] + 4);
				return true;
			}
			return false;
		case S_FINDFILE: // pParam: file to find
						 // pData: cache to find in
						 // pAux: [0] - offset to magic, [1] - size
			assert(pParam && pData && pAux);
			FileName = *((cStr *)pParam);
			strupr(FileName.ToCharPtr());
			pCache = (cList<char> *)pData;
			pi = (int *)pAux;
			pi[0] = pi[1] = 0;

			Msg.Free();
			Msg << csDef << "> Searching file " << csFileName << FileName << csDef << "...\n";
			g_SurfProcess.Message(Msg);

			if(pCache->Count() < 12 + 8) return false;
			i = 12;
			do {
				if(i + 8 > pCache->Count()) return false;

				sign = Pack(&pCache->GetAt(i));
				size = Pack(&pCache->GetAt(i + 4));

				if('ELIF' == sign) {
					DWORD nLen;
					char fn[MAX_PATH];
					nLen = Pack(&pCache->GetAt(i + 12));
					assert(nLen < MAX_PATH);
					ZeroMemory(fn, sizeof(fn));
					for(k = 0; k < nLen; k++) {
						fn[k] = pCache->GetAt(i + 16 + k);
					}
					strupr(fn);
					if(strstr(fn, FileName.ToCharPtr())) {
						pi[0] = i, pi[1] = size;

						Msg.Free();
						Msg << csFileName << fn << csDef << " is founded!\n";
						Msg << "Offset: " << csSize << pi[0] << csDef << " (bytes), Size: " << csSize << pi[1] << csDef << " (bytes)\n";
						g_SurfProcess.Message(Msg);
						return true;
					}
				}
				i += size + 4;
			} while(true);
			
			return false;
		case S_FINDSURFBLOCK: // pData: cache to find in
							  // pAux: [0] - offset to magic, [1] - size
			assert(pData && pAux);
			i = 0;
			Flag = false;
			while(g_SurfLoadMap[i].Magic) {
				Flag = ProcessStage(S_FINDBLOCK, &cStr(g_SurfLoadMap[i].Magic), pData, pAux);
				i++;
				if(Flag) {
					break;
				}
			}
			return Flag;

		case S_FINDBLOCK: // pParam: magic word
						  // pData: cache to find in
						  // pAux: [0] - offset to magic, [1] - size
			assert(pParam && pData && pAux);
			Temp = *((cStr *)pParam);
			assert(Temp.Length() == 4);
			dw = Pack(Temp.ToCharPtr());
			pCache = (cList<char> *)pData;
			pi = (int *)pAux;
			pi[0] = pi[1] = 0;

			Msg.Free();
			Msg << csDef << "> Searching block \'" << csFileName << Temp << csDef << "\'...\n";
			g_SurfProcess.Message(Msg);

			if(pCache->Count() < 12 + 8) return false;
			i = 12;
			do {
				if(i + 8 > pCache->Count()) return false;

				sign = Pack(&pCache->GetAt(i));
				size = Pack(&pCache->GetAt(i + 4));

				if(sign == dw) {
					pi[0] = i, pi[1] = size;

					Msg.Free();
					Msg << csDef << "\'" << csFileName << Temp << csDef << "\' block is founded!\n";
					Msg << "Offset: " << csSize << pi[0] << csDef << " (bytes), Size: " << csSize << pi[1] << csDef << " (bytes)\n";
					g_SurfProcess.Message(Msg);

					return true;
				}
				i += size + 4;
			} while(true);
			return false;
		case S_SEEKBLOCK: // pParam: file handler to seek in
						  // pData: magic word
			assert(pParam && pData);
			r = *((ResFile *)pParam);
			Temp = *((cStr *)pData);
			assert(Temp.Length() == 4);
			dw = Pack(Temp.ToCharPtr());

			Msg.Free();
			Msg << csDef << "> Seeking block \'" << csFileName << Temp << csDef << "\'...\n";
			g_SurfProcess.Message(Msg);

			nSize = RFileSize(r);
			if(nSize < 12 + 8) return false;
			i = 12;
			do {
				if(i + 8 > nSize) return false;

				RSeek(r, i);
				RBlockRead(r, &sign, 4);
				RBlockRead(r, &size, 4);

				if(sign == dw) {
					Msg.Free();
					Msg << csDef << "\'" << csFileName << Temp << csDef << "\' block is sought!\n";
					Msg << "Offset: " << csSize << (int)RGetPos(r, 0) << csDef << " (bytes)";
					Msg << ", Size: " << csSize << (int)size << csDef << " (bytes)\n";
					g_SurfProcess.Message(Msg);

					return true;
				}
				i += size + 4;
			} while(true);
			return false;
		case S_SAVEBLOCK: // pParam: file handler to save in
						  // pData: cache to save from
						  // pAux: [0] - offset to magic, [1] - size
			assert(pParam && pData && pAux);
			r = *((ResFile *)pParam);
			pCache = (cList<char> *)pData;
			pi = (int *)pAux;

			Msg.Free();
			Msg << csDef << "Saving block (Offset: " << csSize << pi[0] << csDef << " (bytes), Size: " << csSize;
			Msg << pi[1] << csDef << " (bytes)...\n";
			g_SurfProcess.Message(Msg);

			nSize = RBlockWrite(r, &pCache->GetAt(pi[0]), pi[1]);

			Msg.Free();
			Msg << csSize << nSize << csDef << " (bytes) saved.\n";
			g_SurfProcess.Message(Msg);

			return pi[1] == nSize;
		case S_SEEKFILE: // pParam: file handler to seek in
						 // pData: file to find
			assert(pParam && pData);
			r = *((ResFile *)pParam);
			FileName = *((cStr *)pData);
			strupr(FileName.ToCharPtr());

			Msg.Free();
			Msg << csDef << "> Seeking file " << csFileName << FileName << csDef << "...\n";
			g_SurfProcess.Message(Msg);

			nSize = RFileSize(r);
			if(nSize < 12 + 8) return false;
			i = 12;
			do {
				if(i + 8 > nSize) return false;

				RSeek(r, i);
				RBlockRead(r, &sign, 4);
				RBlockRead(r, &size, 4);

				if('ELIF' == sign) {
					DWORD sz;
					RBlockRead(r, &sz, 4);
					DWORD nLen;
					char fn[MAX_PATH];
					RBlockRead(r, &nLen, 4);
					assert(nLen < MAX_PATH);
					ZeroMemory(fn, sizeof(fn));
					RBlockRead(r, fn, nLen);
					strupr(fn);
					if(strstr(fn, FileName.ToCharPtr())) {
						RSeek(r, i + 8);

						Msg.Free();
						Msg << csFileName << fn << csDef << " is sought!\n";
						Msg << "Offset: " << csSize << (int)RGetPos(r, 0) << csDef << " (bytes)";
						Msg << ", Size: " << csSize << (int)size << csDef << " (bytes)\n";
						g_SurfProcess.Message(Msg);

						return true;
					}
				}
				i += size + 4;
			} while(true);

			return false;
	}
	return false;
}

//-----------------------------------------------------------------------------
// SurfProcessExtractGenInfo : void(PVOID)
//-----------------------------------------------------------------------------
void SurfProcessExtractGenInfo(PVOID) {
	SurfProcess::ProcessStage(SurfProcess::S_BEGIN);

	cStr Temp;
	if(SurfProcess::ProcessStage(SurfProcess::S_DECOMPRESS, &g_SurfProcess.m_InputFileName, &Temp)) {
		ResFile r = RReset(Temp.ToCharPtr());
		if(INVALID_HANDLE_VALUE != r) {
			cStr FileName("SurfaceShape.dat");
			if(SurfProcess::ProcessStage(SurfProcess::S_SEEKFILE, &r, &FileName)) {
				cStr Msg;
				Msg << SurfProcess::csDef << "> Extracting...\n";
				g_SurfProcess.Message(Msg);

				LoadSomething(r);
			}
			RClose(r);
		}
	}

	SurfProcess::ProcessStage(SurfProcess::S_END);
	_endthread();
}

//-----------------------------------------------------------------------------
// SurfProcessCopyBlockFromInputToOutput
//-----------------------------------------------------------------------------
void SurfProcessCopyBlockFromInputToOutput(cStr *Magic) {
	SurfProcess::ProcessStage(SurfProcess::S_BEGIN);

	bool BlockWasCached = false;
	cList<char> Cache, CachedBlock;

	// Caching block from INPUT map:
	cStr TempFileName;
	if(SurfProcess::ProcessStage(SurfProcess::S_DECOMPRESS, &g_SurfProcess.m_InputFileName, &TempFileName)) {
		if(SurfProcess::ProcessStage(SurfProcess::S_LOAD, &TempFileName, &Cache)) {
			int os[2];
			if(SurfProcess::ProcessStage(SurfProcess::S_FINDBLOCK, Magic, &Cache, &os)) {
				cStr Msg;
				Msg << SurfProcess::csDef << "> Caching block...\n";
				g_SurfProcess.Message(Msg);
				CachedBlock.Append(&Cache[os[0]], 4 + os[1]);
				BlockWasCached = true;
			}
		}
	}
	Cache.Free();

	if(BlockWasCached) {
		// Saving block to OUTPUT map:
		if(SurfProcess::ProcessStage(SurfProcess::S_DECOMPRESS, &g_SurfProcess.m_OutputFileName, &TempFileName)) {
			if(SurfProcess::ProcessStage(SurfProcess::S_LOAD, &TempFileName, &Cache)) {
				// Remove Map Options (if exists):
				int os[2];
				cStr Msg;
				if(SurfProcess::ProcessStage(SurfProcess::S_FINDBLOCK, Magic, &Cache, &os)) {
					Msg << SurfProcess::csDef << "> Removing old block...\n";
					g_SurfProcess.Message(Msg);

					Cache.RemoveAt(os[0], os[1] + 4);
				}
				// Appending new block from INPUT:
				Msg.Free();
				Msg << SurfProcess::csDef << "> Appending new block...\n";
				g_SurfProcess.Message(Msg);
				Cache.AppendAt(os[0], CachedBlock);
				// Saving Cache:
				if(SurfProcess::ProcessStage(SurfProcess::S_SAVE, &TempFileName, &Cache)) {
					SurfProcess::ProcessStage(SurfProcess::S_COMPRESS, &TempFileName, &g_SurfProcess.m_OutputFileName);
				}
			}
		}
	}
	
	SurfProcess::ProcessStage(SurfProcess::S_END);
	_endthread();
}

//-----------------------------------------------------------------------------
// SurfProcessCopyMapOptions
//-----------------------------------------------------------------------------
void SurfProcessCopyMapOptions(PVOID) {
	cStr Magic("MOPT");
	SurfProcessCopyBlockFromInputToOutput(&Magic);
}

//-----------------------------------------------------------------------------
// SurfProcessAppendGenInfo : void(PVOID)
//-----------------------------------------------------------------------------
void SurfProcessAppendGenInfo(PVOID) {
	SurfProcess::ProcessStage(SurfProcess::S_BEGIN);

	cStr Temp;
	if(SurfProcess::ProcessStage(SurfProcess::S_DECOMPRESS, &g_SurfProcess.m_InputFileName, &Temp)) {
		cList<char> Cache;
		if(SurfProcess::ProcessStage(SurfProcess::S_LOAD, &Temp, &Cache)) {
			cStr FileName("SurfaceShape.dat");
			SurfProcess::ProcessStage(SurfProcess::S_DELETEFILE, &FileName, &Cache);
			// SurfaceShape.dat we will save after Surface block:
			int os[2];
			
			if(SurfProcess::ProcessStage(SurfProcess::S_FINDSURFBLOCK, NULL, &Cache, os)) {
				cStr Msg;
				Msg << SurfProcess::csDef << "Saving to " << SurfProcess::csFileName << Temp << SurfProcess::csDef << ":\n";
				g_SurfProcess.Message(Msg);

				ResFile r = RRewrite(Temp.ToCharPtr());
				if(r != INVALID_HANDLE_VALUE) {
					int k = os[0] + os[1] + 4;
					int os1[2] = { 0, k };
					int os2[2] = { k, Cache.Count() - k };
					if(SurfProcess::ProcessStage(SurfProcess::S_SAVEBLOCK, &r, &Cache, os1)) {
						DWORD i0 = RGetPos(r, 0);
						Msg.Free();
						Msg << SurfProcess::csDef << "Saving " << SurfProcess::csFileName;
						Msg << "SURFACE\\SurfaceShape.Dat" << SurfProcess::csDef << "...\n";
						g_SurfProcess.Message(Msg);

						SaveSomething(r, "Surface\\SurfaceShape.dat", "");

						i0 = RGetPos(r, 0) - i0;
						Msg.Free();
						Msg << SurfProcess::csSize << (int)i0 << SurfProcess::csDef << " (bytes) saved.\n";
						g_SurfProcess.Message(Msg);

						if(SurfProcess::ProcessStage(SurfProcess::S_SAVEBLOCK, &r, &Cache, os2)) {
							RClose(r);
							SurfProcess::ProcessStage(SurfProcess::S_COMPRESS, &Temp, &g_SurfProcess.m_OutputFileName);
						} else RClose(r);
					} else RClose(r);
				}
			}
		}
	}

	SurfProcess::ProcessStage(SurfProcess::S_END);
	_endthread();
}

//-----------------------------------------------------------------------------
// SurfProcessRemoveGenInfo : void(PVOID)
//-----------------------------------------------------------------------------
void SurfProcessRemoveGenInfo(PVOID) {
	SurfProcess::ProcessStage(SurfProcess::S_BEGIN);

	cStr Temp;
	if(SurfProcess::ProcessStage(SurfProcess::S_DECOMPRESS, &g_SurfProcess.m_InputFileName, &Temp)) {
		cList<char> Cache;
		if(SurfProcess::ProcessStage(SurfProcess::S_LOAD, &Temp, &Cache)) {
			cStr FileName("SurfaceShape.dat");
			SurfProcess::ProcessStage(SurfProcess::S_DELETEFILE, &FileName, &Cache);
			if(SurfProcess::ProcessStage(SurfProcess::S_SAVE, &Temp, &Cache)) {
				SurfProcess::ProcessStage(SurfProcess::S_COMPRESS, &Temp, &g_SurfProcess.m_OutputFileName);
			}
		}
	}

	SurfProcess::ProcessStage(SurfProcess::S_END);
	_endthread();
}

typedef enum { PS_SIMPLIFY, PS_OPTIMIZE } eProcessSurface;
void SurfProcessSurface(eProcessSurface ps) {
	SurfProcess::ProcessStage(SurfProcess::S_BEGIN);

	cStr Temp;
	int nFileSize = 0;
	if(SurfProcess::ProcessStage(SurfProcess::S_DECOMPRESS, &g_SurfProcess.m_InputFileName, &Temp)) {
		ResFile rfi = RReset(Temp.ToCharPtr());
		if(rfi != INVALID_HANDLE_VALUE) {
			g_UniversalMap.Free();
			int sl = 0;
			while(g_SurfLoadMap[sl].Magic) {
				if(SurfProcess::ProcessStage(SurfProcess::S_SEEKBLOCK, &rfi, &cStr(g_SurfLoadMap[sl].Magic))) {
					nFileSize = RFileSize(rfi);
					g_SurfLoadMap[sl].fn(rfi);
					break;
				}
				sl++;
			}
			RClose(rfi);
			if(g_UniversalMap.VERTS.Count() && g_UniversalMap.TRIS.Count()) {
				// g_UniversalMap Info:
				cStr Msg;
				Msg << SurfProcess::csDef << "NVerts: " << SurfProcess::csSize << g_UniversalMap.VERTS.Count();
				Msg << SurfProcess::csDef << ", NTris: " << SurfProcess::csSize << g_UniversalMap.TRIS.Count() << "\n";
				g_SurfProcess.Message(Msg);

				// MManager Info:
				Msg.Free();
				Msg << SurfProcess::csDef << "MManager AllocatedSize: " << SurfProcess::csSize << (int)MManager.HeapBlock.AllocatedSize / 1024 / 1024 << SurfProcess::csDef << " (MB)";
				Msg << ", TotalSize: " << SurfProcess::csSize << (int)MManager.HeapBlock.TotalSize / 1024 / 1024 << SurfProcess::csDef << " (MB)\n";
				g_SurfProcess.Message(Msg);

				// Processing:
				Msg.Free();
				Msg << SurfProcess::csDef << "> ";
				switch(ps) {
					case PS_SIMPLIFY:
						Msg << "Simplification";
						break;
					case PS_OPTIMIZE:
						Msg << "Optimization";
						break;
					default:
						assert(0);
				}
				Msg << "...\n";

				g_SurfProcess.Message(Msg);

				DWORD dwTime = GetTickCount();
				Msg.Free();
				Msg << SurfProcess::csDef;
				switch(ps) {
					case PS_SIMPLIFY:
						SimplifySurfMesh(g_UniversalMap);
						Msg << "Simplified";
						break;
					case PS_OPTIMIZE:
						OptimizeSurfMesh(g_UniversalMap, false);
						Msg << "Optimized";
						break;
					default:
						assert(0);
				}
				dwTime = GetTickCount() - dwTime;
				dwTime /= 1000;
				Msg << " in " << SurfProcess::csSize << (int)dwTime << SurfProcess::csDef << " (sec)\n";
				g_SurfProcess.Message(Msg);

				// g_UniversalMap Info:
				Msg.Free();
				Msg << SurfProcess::csDef << "NVerts: " << SurfProcess::csSize << g_UniversalMap.VERTS.Count();
				Msg << SurfProcess::csDef << ", NTris: " << SurfProcess::csSize << g_UniversalMap.TRIS.Count() << "\n";
				g_SurfProcess.Message(Msg);

				cList<char> Cache;
				if(SurfProcess::ProcessStage(SurfProcess::S_LOAD, &Temp, &Cache)) {
					int os[2];
					if(SurfProcess::ProcessStage(SurfProcess::S_FINDSURFBLOCK, NULL, &Cache, os)) {
						Msg.Free();
						Msg << SurfProcess::csDef << "Saving to " << SurfProcess::csFileName << Temp << SurfProcess::csDef << ":\n";
						g_SurfProcess.Message(Msg);

						ResFile r = RRewrite(Temp.ToCharPtr());
						if(r != INVALID_HANDLE_VALUE) {
							int os1[2] = { 0, os[0] };
							int os2[2] = { os[0] + os[1] + 4, nFileSize - (os[0] + os[1] + 4) };
							if(SurfProcess::ProcessStage(SurfProcess::S_SAVEBLOCK, &r, &Cache, os1)) {
								DWORD i0 = RGetPos(r, 0);
								Msg.Free();
								Msg << SurfProcess::csDef << "Saving Surface...\n";
								g_SurfProcess.Message(Msg);
								
								SaveNewSurface(r);

								i0 = RGetPos(r, 0) - i0;
								Msg.Free();
								Msg << SurfProcess::csSize << (int)i0 << SurfProcess::csDef << " (bytes) saved.\n";
								g_SurfProcess.Message(Msg);

								if(SurfProcess::ProcessStage(SurfProcess::S_SAVEBLOCK, &r, &Cache, os2)) {
									RClose(r);
									SurfProcess::ProcessStage(SurfProcess::S_COMPRESS, &Temp, &g_SurfProcess.m_OutputFileName);
								} else RClose(r);
							} else RClose(r);
						}
					}
				}
			}
		}
	}

	SurfProcess::ProcessStage(SurfProcess::S_END);
}

//-----------------------------------------------------------------------------
// SurfProcessOptimize : void(PVOID)
//-----------------------------------------------------------------------------
void SurfProcessOptimize(PVOID) {
	SurfProcessSurface(PS_OPTIMIZE);

	_endthread();
}

//-----------------------------------------------------------------------------
// SurfProcessSimplify : void(PVOID)
//-----------------------------------------------------------------------------
void SurfProcessSimplify(PVOID) {
	SurfProcessSurface(PS_SIMPLIFY);

	_endthread();
}

//-----------------------------------------------------------------------------
// SurfProcess::EnableLayout : void(bool)
//-----------------------------------------------------------------------------
void SurfProcess::EnableLayout(bool fEnable) {
	EnableWindow(m_hToolCombo, fEnable);
	EnableWindow(GetDlgItem(m_hDlg, IDC_INPUTBUTTON), fEnable);
	EnableWindow(GetDlgItem(m_hDlg, IDC_OUTPUTBUTTON), fEnable);
	EnableWindow(GetDlgItem(m_hDlg, IDC_STARTBUTTON), fEnable);
	EnableWindow(GetDlgItem(m_hDlg, IDC_COPYBUTTON), fEnable);
}

//-----------------------------------------------------------------------------
// SurfProcess::AdjustLayout : void(HWND)
//-----------------------------------------------------------------------------
void SurfProcess::AdjustLayout(HWND hDlg) {
	RECT rc;
	GetClientRect(hDlg, &rc);

	const int d = 10;
	// INPUT:
	int W = rc.right - rc.left - 2 * d;
	int X0 = d, Y0 = d;
	MoveWindow(GetDlgItem(hDlg, IDC_INPUTGROUP), X0, Y0, W, 45, true);
    MoveWindow(m_hInputLabel, X0 + 10, Y0 + 24, W - 15, 16, true);
	MoveWindow(GetDlgItem(hDlg, IDC_INPUTBUTTON), X0 + 10, Y0 + 1, 60, 20, true);
	// OUTPUT:
	Y0 += 36 + d;
	MoveWindow(GetDlgItem(hDlg, IDC_OUTPUTGROUP), X0, Y0, W, 45, true);
	MoveWindow(m_hOutputLabel, X0 + 10, Y0 + 24, W - 15, 16, true);
	MoveWindow(GetDlgItem(hDlg, IDC_OUTPUTBUTTON), X0 + 10, Y0 + 1, 60, 20, true);
	// CopyButton:
	const int cbd = 6;
	MoveWindow(GetDlgItem(hDlg, IDC_COPYBUTTON), X0 - cbd, Y0 + 1, 10 + cbd, 20, true);
	// ToolButton:
	Y0 += 46 + d;
	MoveWindow(GetDlgItem(hDlg, IDC_STARTBUTTON), rc.right - rc.left - 60 - d, Y0, 60, 23, true);
	// ToolCombo:
	RECT rc0;
	int w = rc.right - rc.left - 60 - 3 * d;
	GetWindowRect(m_hToolCombo, &rc0);
	if(rc0.right - rc0.left != w) MoveWindow(m_hToolCombo, X0, Y0, w, 20, true);
	// Console:
	Y0 += 25 + d;
	MoveWindow(m_hConsole, 0, Y0, rc.right - rc.left, rc.bottom - rc.top - Y0, true);
}

//-----------------------------------------------------------------------------
// SurfProcess::InitLayout : void(HWND)
//-----------------------------------------------------------------------------
void SurfProcess::InitLayout(HWND hDlg) {
	const int Lx = 600, Ly = 400;
	SetWindowPos(hDlg, NULL, 0, 0, Lx, Ly, SWP_NOMOVE | SWP_NOOWNERZORDER);
	cWin32::CenterWindow(hDlg, NULL);
	SetWindowText(hDlg, "Surface Processing Tool");

	m_hInputLabel = GetDlgItem(hDlg, IDC_INPUTLABEL);
	m_hOutputLabel = GetDlgItem(hDlg, IDC_OUTPUTLABEL);
	m_hToolCombo = GetDlgItem(hDlg, IDC_TOOLCOMBO);
	for(int i = 0; g_SurfProcessMap[i].Name; i++) {
		SendMessage(m_hToolCombo, CB_ADDSTRING, 0, (LPARAM)g_SurfProcessMap[i].Name);
	}
	SendMessage(m_hToolCombo, CB_SETCURSEL, 0, 0);
	m_hConsole = GetDlgItem(hDlg, IDC_CONSOLE);
}

//-----------------------------------------------------------------------------
// InitDir : void(char *)
//-----------------------------------------------------------------------------
void SurfProcess::InitDir(char *pStr) {
	assert(pStr);
	assert(strlen(pStr) < MAX_PATH);
	if(strlen(pStr) == 0) {
		GetModuleFileName(NULL, pStr, MAX_PATH);
		int i = 0, k = 0;
		while(pStr[i]) {
			if(pStr[i] == '\\') {
				k = i;
			}
			i++;
		}
		pStr[k + 1] = '\0';
		strcat(pStr, "Maps");
	}
}

//--------------------------------------------------------------------------------------
// SurfProcessDlgProc : LRESULT(HWND, UINT, WPARAM, LPARAM)
//--------------------------------------------------------------------------------------
LRESULT CALLBACK SurfProcessDlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam) {
	OPENFILENAME ofn;
	char CurDir[MAX_PATH], Buffer[MAX_PATH];
	static char InputInitDir[MAX_PATH] = "\0", OutputInitDir[MAX_PATH] = "\0";
	int i, k;
	RECT rc, rc0;
	DWORD Color = GetSysColor(COLOR_3DFACE);
	HBRUSH hBr;

	switch(Msg) {
	case WM_INITDIALOG:
		g_SurfProcess.InitLayout(hDlg);
		g_SurfProcess.AdjustLayout(hDlg);
		g_SurfProcess.m_hDlg = hDlg;
		break;
	case WM_ERASEBKGND:
		GetClientRect(hDlg, &rc);
		GetWindowRect(GetDlgItem(hDlg, IDC_CONSOLE), &rc0);
		MapWindowPoints(NULL, hDlg, (LPPOINT)(&rc0), sizeof(RECT) / sizeof(POINT));
		rc.bottom = rc0.top;
		hBr = CreateSolidBrush(Color);
		FillRect((HDC)wParam, &rc, hBr);
		DeleteObject(hBr);
		return TRUE;
	case WM_SIZING:
	case WM_SIZE:
		cWin32::OnSizing(hDlg, wParam, lParam, 400, 300);
		g_SurfProcess.AdjustLayout(hDlg);
		break;
	case WM_COMMAND:
		if(LOWORD(wParam) == IDC_OUTPUTBUTTON || LOWORD(wParam) == IDC_INPUTBUTTON) {
			// Cur Dir:
			CurDir[0] = '\0';
			GetCurrentDirectory(MAX_PATH, CurDir);
			// Initial Dir:
			SurfProcess::InitDir(InputInitDir);
			SurfProcess::InitDir(OutputInitDir);

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = Buffer;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(Buffer);
			ofn.lpstrFilter = "HoAE Map Files (*.m3d)\0*.m3d\0";
		
			// OUTPUT:
			if(LOWORD(wParam) == IDC_OUTPUTBUTTON) {
				ofn.lpstrInitialDir = OutputInitDir;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;// | OFN_ENABLEHOOK;
				
				if(GetSaveFileName(&ofn)) {
					strupr(Buffer);
					SurfExporter::CheckExt3(Buffer, "m3d");
					g_SurfProcess.m_OutputFileName.Copy(Buffer);
					SetWindowText(g_SurfProcess.m_hOutputLabel, Buffer);
					GetCurrentDirectory(MAX_PATH, OutputInitDir);
				}
			}
			// INPUT
			if(LOWORD(wParam) == IDC_INPUTBUTTON) {
				ofn.lpstrInitialDir = InputInitDir;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if(GetOpenFileName(&ofn)) {
					strupr(Buffer);
					g_SurfProcess.m_InputFileName.Copy(Buffer);
					SetWindowText(g_SurfProcess.m_hInputLabel, Buffer);
					GetCurrentDirectory(MAX_PATH, InputInitDir);
				}
			}
			SetCurrentDirectory(CurDir);
		}
		// START
		if(LOWORD(wParam) == IDC_STARTBUTTON) {
			i = SendMessage(GetDlgItem(hDlg, IDC_TOOLCOMBO), CB_GETCURSEL, 0, 0);
			if((g_SurfProcess.m_InputFileName.Length() && !g_SurfProcessMap[i].fOutputIsNeeded) ||
				(g_SurfProcess.m_InputFileName.Length() && g_SurfProcess.m_OutputFileName.Length())) {
					_beginthread(g_SurfProcessMap[i].fn, 0, NULL);
				}
		}
		// CopyButton:
		if(LOWORD(wParam) == IDC_COPYBUTTON) {
			g_SurfProcess.m_OutputFileName = g_SurfProcess.m_InputFileName;
			SetWindowText(g_SurfProcess.m_hOutputLabel, g_SurfProcess.m_OutputFileName.ToCharPtr());
		}
		break;
	case WM_CLOSE:
		if(!g_SurfProcess.m_fActive) EndDialog(hDlg, true);
		break;
	}
	return false;
}

//-----------------------------------------------------------------------------
// SurfProcess::Message : void(const cStr &)
//-----------------------------------------------------------------------------
void SurfProcess::Message(const cStr &Str) {
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.crTextColor = cColor::Red.ToBgra();
	cf.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.yHeight = 20 * 10;
	strcpy(cf.szFaceName, "Courier New");

	cColor Color = cStr::ColorForIndex(0);
	cf.crTextColor = Color.ToBgra() & 0xffffff;
	const char *pStr = Str.ToCharPtr();
	char Buffer[2] = { 0, 0 };
	do {
		if(cStr::IsColor(pStr, Color)) {
			pStr += 2;
			cf.crTextColor = Color.ToBgra() & 0xffffff;
			continue;
		}
		Buffer[0] = *pStr++;
		if(!Buffer[0]) break;
		SendMessage(m_hConsole, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
		SendMessage(m_hConsole, EM_SETSEL, (WPARAM)0xFFFF, (LPARAM)0xFFFF);
		SendMessage(m_hConsole, EM_REPLACESEL, 0, (LPARAM)Buffer);
		SendMessage(m_hConsole, EM_SCROLL, (WPARAM)SB_LINEDOWN, 0);
	} while(true);
}

//-----------------------------------------------------------------------------
// SurfProcess::Show : void(HINSTANCE)
//-----------------------------------------------------------------------------
void SurfProcess::Show(HINSTANCE hInstance) {
	if(!LoadLibrary("RichEd32.dll")) {
		MessageBox(NULL, "LoadLibrary(\"RichEd32.dll\") FAILED!", "--- ERROR ---", 0);
		return;
	}
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_SURF), NULL, (DLGPROC)SurfProcessDlgProc);
}

}