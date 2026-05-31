#ifndef __cFileSpec__
#define __cFileSpec__

#include "cStr.h"

namespace comms {

class cFileSpec {
public:
	typedef enum { FS_EMPTY, FS_APP, FS_APPDIR, FS_WINDIR, FS_SYSDIR,
		FS_TMPDIR, FS_DESKTOP, FS_FAVOURITES, FS_MEDIA, FS_CURRDIR, FS_TEMPNAME } eFileSpec;

	cFileSpec(eFileSpec eSpec = FS_EMPTY);
	cFileSpec(eFileSpec eSpec, const cStr &FileName);
	cFileSpec(const cStr &Spec, const cStr &FileName);
	cFileSpec(const cStr &FileName);
	
	bool Exists() const;
	void MakeWriteAble() const;
	void MakeReadOnly() const;
	
	cStr & Drive() { return m_csDrive; }
	cStr & Path() { return m_csPath; }
	cStr & FileName() { return m_csFileName; }
	cStr & Extension() { return m_csExtension; }

	const cStr GetFullPathNoExtension() const;
	const cStr GetFolder() const;
	const cStr GetFullSpec() const;
	const cStr GetFileName() const;
	
	void SetFullSpec(const cStr &Spec);
	void SetFullSpec(eFileSpec Spec = FS_EMPTY);
	void SetFileName(const cStr &Spec);
	void Initialize(eFileSpec Spec);
private:
	static void AddBackSlash(char *pStr);
	void GetShellFolder(int idFolder);
	cStr m_csDrive, m_csPath, m_csFileName, m_csExtension;
};

} // comms

#endif // __cFileSpec__