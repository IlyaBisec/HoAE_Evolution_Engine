#pragma once

//*****************************************************************************
// cFileSystem
//*****************************************************************************
class cFileSystem {
public:
	cFileSystem() {}
	virtual ~cFileSystem() {}
	virtual cData * LoadFile(const char *FileName) const = 0;
	virtual bool SaveFile(const cData &File, const char *FileName) const = 0;
};

//*****************************************************************************
// cFolder
//*****************************************************************************
class cFolder : public cFileSystem {
private:
	cStr m_Folder;
public:
	cFolder(const char *Folder) : cFileSystem(), m_Folder(Folder) {}
	virtual cData * LoadFile(const char *FileName) const;
	virtual bool SaveFile(const cData &File, const char *FileName) const;
};

class cImage; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
class cImageBmp;
class cImageDds;
class cImageTga;

//*****************************************************************************
// cImageCodec
//*****************************************************************************
class cImageCodec {
public:
	cImageCodec() {}
	virtual ~cImageCodec() {}
	virtual cImage * Decode(const cData *Src) = 0;
};

//*****************************************************************************
// cMeshCodec
//*****************************************************************************
class cMeshCodec {
public:
	cMeshCodec() {}
	virtual ~cMeshCodec() {}
	virtual cRawMesh * Decode(const cData &Fm) = 0;
	virtual void Encode(const cRawMesh &Mesh, cData *To) = 0;
};

//*****************************************************************************
// cIO
//*****************************************************************************
class cIO {
public:
	static void Init();
	static void AddFolder(const char *Folder);
	static cData * LoadFile(const char *FileName);
	static bool SaveFile(const cData &File, const char *FileName = NULL); // If "FileName" is NULL, data filename will be used.

	static bool AddCodec(const char *FileExtension, cImageCodec *Codec);
	static cImage * LoadImage(const char *FileName);

	struct ImageCodecInfo {
		cStr FileExtension;
		cImageCodec *Codec;
	};
	static cList<ImageCodecInfo> & GetImageCodecs() { return m_ImageCodecs; }

	static bool AddCodec(const char *FileExtension, cMeshCodec *Codec);
	static cRawMesh * LoadMesh(const char *FileName);
	static bool SaveMesh(const cRawMesh &Mesh, const char *FileName);

	struct MeshCodecInfo {
		cStr FileExtension;
		cMeshCodec *Codec;
	};
	static cList<MeshCodecInfo> & GetMeshCodecs() { return m_MeshCodecs; }
private:
	static cList<cFileSystem *> m_FileSystems;
	static cList<ImageCodecInfo> m_ImageCodecs;
	static cList<MeshCodecInfo> m_MeshCodecs;
};