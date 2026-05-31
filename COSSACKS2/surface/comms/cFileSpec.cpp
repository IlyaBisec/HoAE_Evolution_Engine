#include <io.h>
#include <sys/stat.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#include "cFileSpec.h"

namespace comms {

//-----------------------------------------------------------------------------
// cFileSpec.ctor : (eFileSpec)
//-----------------------------------------------------------------------------
cFileSpec::cFileSpec(eFileSpec eSpec) { Initialize(eSpec); }


//-----------------------------------------------------------------------------
// cFileSpec.ctor : (eFileSpec, const cStr &)
//-----------------------------------------------------------------------------
cFileSpec::cFileSpec(eFileSpec eSpec, const cStr &FileName) {
	Initialize(eSpec);
	SetFileName(FileName);
}

//-----------------------------------------------------------------------------
// .ctor : (const cStr &, const cStr &)
//-----------------------------------------------------------------------------
cFileSpec::cFileSpec(const cStr &Spec, const cStr &FileName) {
	SetFullSpec(Spec);
	SetFileName(FileName);
}

//-----------------------------------------------------------------------------
// .ctor : (const cStr &)
//-----------------------------------------------------------------------------
cFileSpec::cFileSpec(const cStr &FileName) { SetFullSpec(FileName); }

//-----------------------------------------------------------------------------
// cFileSpec::Exists : bool() const
//-----------------------------------------------------------------------------
bool cFileSpec::Exists() const {
	return _access(GetFullSpec(), 0) == 0;
}

//-----------------------------------------------------------------------------
// cFileSpec::MakeWriteAble : void() const
//-----------------------------------------------------------------------------
void cFileSpec::MakeWriteAble() const {
	if(Exists()) _chmod(GetFullSpec(), _S_IWRITE | _S_IREAD);
}

//-----------------------------------------------------------------------------
// cFileSpec::MakeReadOnly : void() const
//-----------------------------------------------------------------------------
void cFileSpec::MakeReadOnly() const {
	if(Exists()) _chmod(GetFullSpec(), _S_IREAD);
}

//-----------------------------------------------------------------------------
// cFileSpec::GetFullPathNoExtension : const cStr () const
//-----------------------------------------------------------------------------
const cStr cFileSpec::GetFullPathNoExtension() const {
	return m_csDrive + m_csPath + m_csFileName;
}

//-----------------------------------------------------------------------------
// cFileSpec::GetFolder : const cStr () const
//-----------------------------------------------------------------------------
const cStr cFileSpec::GetFolder() const {
	return m_csDrive + m_csPath;
}

//-----------------------------------------------------------------------------
// cFileSpec::GetFullSpec : const cStr () const
//-----------------------------------------------------------------------------
const cStr cFileSpec::GetFullSpec() const {
	return m_csDrive + m_csPath + m_csFileName + m_csExtension;
}

//-----------------------------------------------------------------------------
// cFileSpec::GEtFileName : const cStr () const
//-----------------------------------------------------------------------------
const cStr cFileSpec::GetFileName() const {
	return m_csFileName + m_csExtension;
}

//-----------------------------------------------------------------------------
// cFileSpec::SetFullSpec : void(const cStr &)
//-----------------------------------------------------------------------------
void cFileSpec::SetFullSpec(const cStr &Spec) {
	char Drive[_MAX_DRIVE], Path[_MAX_DRIVE], FileName[_MAX_FNAME], Extension[_MAX_EXT];
	_splitpath(Spec.ToChar(), Drive, Path, FileName, Extension);
	m_csDrive = Drive;
	m_csPath = Path;
	m_csFileName = FileName;
	m_csExtension = Extension;
}

//-----------------------------------------------------------------------------
// cFileSpec::SetFullSpec : void(eFileSpec)
//-----------------------------------------------------------------------------
void cFileSpec::SetFullSpec(eFileSpec eSpec) {
	Initialize(eSpec);
}

//-----------------------------------------------------------------------------
// cFileSpec::SetFileName : void(const cStr &)
//-----------------------------------------------------------------------------
void cFileSpec::SetFileName(const cStr &Spec) {
	char FileName[_MAX_FNAME], Extension[_MAX_EXT];

	_splitpath(Spec.ToChar(), NULL, NULL, FileName, Extension);
	m_csFileName = FileName;
	m_csExtension = Extension;
}

//-----------------------------------------------------------------------------
// cFileSpec::AddBackSlash : void(char *pStr)
//-----------------------------------------------------------------------------
void cFileSpec::AddBackSlash(char *pStr) {
	int l = strlen(pStr);
	if(l > 0) {
		if(pStr[l - 1] != '\\') {
			strcat(pStr, "\\");
		}
	} else {
		strcat(pStr, "\\");
	}
}

//-----------------------------------------------------------------------------
// cFileSpec::Initialize : void(eFileSpec)
//-----------------------------------------------------------------------------
void cFileSpec::Initialize(eFileSpec eSpec) {
	char *pStr, Buffer[MAX_PATH];
	switch(eSpec) {
		case FS_EMPTY:
			break;
		case FS_APP:
			GetModuleFileName(NULL, Buffer, MAX_PATH);
			SetFullSpec(cStr(Buffer));
			break;
		case FS_APPDIR:
			GetModuleFileName(NULL, Buffer, MAX_PATH);
			SetFullSpec(cStr(Buffer));
			m_csExtension.Free();
			m_csFileName.Free();
			break;
		case FS_WINDIR:
			GetWindowsDirectory(Buffer, MAX_PATH);
			AddBackSlash(Buffer);
			SetFullSpec(cStr(Buffer));
			break;
		case FS_SYSDIR:
			GetSystemDirectory(Buffer, MAX_PATH);
			AddBackSlash(Buffer);
			SetFullSpec(cStr(Buffer));
			break;
		case FS_TMPDIR:
			GetTempPath(MAX_PATH, Buffer);
			SetFullSpec(cStr(Buffer));
			break;
		case FS_DESKTOP:
			GetShellFolder(CSIDL_DESKTOP);
			break;
		case FS_FAVOURITES:
			GetShellFolder(CSIDL_FAVORITES);
			break;
		case FS_TEMPNAME:
            GetTempPath(MAX_PATH, Buffer);
			strcpy(Buffer, pStr = tempnam(Buffer, "~"));
			SetFullSpec(cStr(Buffer));
			assert(pStr);
			free(pStr);
			break;
		case FS_MEDIA:
			GetWindowsDirectory(Buffer, MAX_PATH);
			AddBackSlash(Buffer);
			strcat(Buffer, "media\\");
			SetFullSpec(cStr(Buffer));
			break;
		case FS_CURRDIR:
			GetCurrentDirectory(MAX_PATH, Buffer);
			AddBackSlash(Buffer);
			SetFullSpec(cStr(Buffer));
			break;
		default:
			assert(0 && "Unknown spec!");
	}
}

//-----------------------------------------------------------------------------
// cFileSpec::GetShellFolder : void(int)
//-----------------------------------------------------------------------------
void cFileSpec::GetShellFolder(int nFolder) {
	LPITEMIDLIST pItemList;
	LPMALLOC	 pMalloc;
	
	SHGetMalloc(&pMalloc);
	if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, nFolder, &pItemList))) {
		char Buffer[MAX_PATH];
		if(SHGetPathFromIDList(pItemList, Buffer)) SetFullSpec(cStr(Buffer));
		pMalloc->Free(pItemList);
	}
}

} // comms