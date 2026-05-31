#include "comms.h"
#include "Share.h"

//*****************************************************************************
// cFolder
//*****************************************************************************

//-----------------------------------------------------------------------------
// cFolder::LoadFile
//-----------------------------------------------------------------------------
cData * cFolder::LoadFile(const char *FileName) const {
	cStr FilePathName(m_Folder);
	FilePathName.AppendPath(FileName);
	
	FilePathName.SlashesToBackSlashes();
	
	int fd = _sopen(FilePathName.ToCharPtr(), O_RDONLY | O_BINARY, _SH_DENYWR, _S_IREAD);
	if(-1 == fd) {
		return NULL;
	}

	long l = _filelength(fd);
	if(-1 == l) {
		_close(fd);
		return NULL;
	}

	byte *pBits = new byte[l];
	if(NULL == pBits) {
		_close(fd);
		return NULL;
	}

	int Size = _read(fd, pBits, l);
	_close(fd);

	return new cData(cData::SetCtor, pBits, Size, FilePathName);
} // cFolder::LoadFile

//-----------------------------------------------------------------------------
// cFolder::SaveFile
//-----------------------------------------------------------------------------
bool cFolder::SaveFile(const cData &File, const char *FileName) const {
	cStr FilePathName(m_Folder);
	FilePathName.AppendPath(FileName);
	FilePathName.SlashesToBackSlashes();
	if(FilePathName.GetFilePath().Length() < 1) { // No path. Cannot save.
		return false;
	}

	int fd = _sopen(FilePathName.ToCharPtr(), O_WRONLY | O_BINARY | O_TRUNC | O_CREAT, _SH_DENYNO, S_IWRITE);
	if(-1 == fd) {
		return NULL;
	}
	const int Size = _write(fd, File.GetData(), File.GetSize());
	_close(fd);
	return Size == File.GetSize();
} // cFolder::SaveFile

//*****************************************************************************
// cIO
//*****************************************************************************

cList<cFileSystem *> cIO::m_FileSystems;
cList<cIO::ImageCodecInfo> cIO::m_ImageCodecs;
cList<cIO::MeshCodecInfo> cIO::m_MeshCodecs;

//-----------------------------------------------------------------------------
// cIO::AddFolder
//-----------------------------------------------------------------------------
void cIO::AddFolder(const char *Folder) {
	Init();
	
	m_FileSystems.Add(new cFolder(Folder));
	cLog::Message("cIO::AddFolder(): Added folder source \"%s\"", Folder);
}

//-----------------------------------------------------------------------------
// cIO::LoadFile
//-----------------------------------------------------------------------------
cData * cIO::LoadFile(const char *FileName) {
	Init();

	cData *Data;
	for(cList<cFileSystem *>::Iterator It = m_FileSystems.GetIterator(); !It.End(); ++It) {
		if((Data = (*It)->LoadFile(FileName)) != NULL) {
			return Data;
		}
	}
	return NULL;
} // cIO::LoadFile

//-----------------------------------------------------------------------------
// cIO::SaveFile
//-----------------------------------------------------------------------------
bool cIO::SaveFile(const cData &File, const char *FileName /*= NULL*/) {
	Init();

	// Selecting File Name:
	const char *p = FileName != NULL ? FileName : File.GetFileName().ToCharPtr();
	if(cStr::Length(p) < 1) {
		return false;
	}

	bool IsSaved = false;
	for(cList<cFileSystem *>::Iterator It = m_FileSystems.GetIterator(); !It.End(); ++It) {
		IsSaved = (*It)->SaveFile(File, p);
		if(IsSaved) {
			break;
		}
	}
	return IsSaved;
} // cIO::SaveFile

//-----------------------------------------------------------------------------
// cIO::AddCodec
//-----------------------------------------------------------------------------
bool cIO::AddCodec(const char *FileExtension, cImageCodec *Codec) {
	Init();

	for(int i = 0; i < m_ImageCodecs.Count(); i++) {
		if(cStr::EqualsNoCase(FileExtension, m_ImageCodecs[i].FileExtension)) {
			return false;
		}
	}

	ImageCodecInfo Info;
	Info.FileExtension = FileExtension;
	Info.Codec = Codec;
	m_ImageCodecs.Add(Info);

	cLog::Message("cIO::AddCodec(): Added \"%s\" image codec", Info.FileExtension);

	return true;
} // cIO::AddCodec

//-----------------------------------------------------------------------------
// cIO::AddCodec
//-----------------------------------------------------------------------------
bool cIO::AddCodec(const char *FileExtension, cMeshCodec *Codec) {
	Init();

	for(cList<MeshCodecInfo>::Iterator It = m_MeshCodecs.GetIterator(); !It.End(); ++It) {
		if(cStr::EqualsNoCase(FileExtension, It->FileExtension)) {
			return false;
		}
	}

	MeshCodecInfo Info;
	Info.FileExtension = FileExtension;
	Info.Codec = Codec;
	m_MeshCodecs.Add(Info);

	cLog::Message("cIO::AddCodec(): Added \"%s\" mesh codec.", Info.FileExtension);
	return true;
} // cIO::AddCodec

//-----------------------------------------------------------------------------
// cIO::Init
//-----------------------------------------------------------------------------
void cIO::Init() {
	static bool Inited = false;
	if(Inited) {
		return;
	}
	Inited = true;

	// Default file systems:
	AddFolder(""); // Current folder.
	// Program file folder.
	cStr Folder(cStr::SetLengthCtor, MAX_PATH);
	GetModuleFileName(NULL, Folder.ToCharPtr(), Folder.Length());
	Folder.CalcLength();
	Folder.StripFileName();
	AddFolder(Folder);

	// Image Decoders:
	AddCodec("Bmp", new cImageBmp);
//	AddImageDecoder("Dds", new cImageDds);
//	AddImageDecoder("Tga", new cImageTga);

	// Mesh codecs:
	AddCodec("Obj", new cMeshObj);
} // cIO::Init

//-----------------------------------------------------------------------------
// cIO::LoadImage
//-----------------------------------------------------------------------------
cImage * cIO::LoadImage(const char *FileName) {
	Init();
	
	if(FileName == NULL) {
		cLog::Warning("cIO::LoadImage(): File name is NULL");
		return NULL;
	}
	const cStr FileExtension = cStr(FileName).GetFileExtension();
	if(!FileExtension.Length()) {
		cLog::Warning("cIO::LoadImage(): File name \"%s\" has no extension", FileName);
		return NULL;
	}

	// Load file:
	cData *Src = LoadFile(FileName);
	if(!Src) {
		cLog::Warning("cIO::LoadImage(): Can't open file \"%s\"", FileName);
		return NULL;
	}
	if(Src->GetSize() < 1) {
		delete Src;
		cLog::Warning("cIO::LoadImage(): File \"%s\" is empty", FileName);
		return NULL;
	}
	cLog::Message("cIO::LoadImage(): Loaded image file \"%s\"", FileName);
	
	// Looking for codec:
	cImageCodec *Codec = NULL;
	for(int i = 0; i < m_ImageCodecs.Count(); i++) {
		if(cStr::EqualsNoCase(m_ImageCodecs[i].FileExtension, FileExtension)) {
			Codec = m_ImageCodecs[i].Codec;
			break;
		}
	}
	if(!Codec) {
		delete Src;
		cLog::Warning("cIO::LoadImage(): No codec for image file \"%s\"", FileName);
		return NULL;
	}
	
	// Decoding:
	cImage *Image = Codec->Decode(Src);
	delete Src;
	return Image;
} // cIO::LoadImage

//-----------------------------------------------------------------------------
// cIO::LoadMesh
//-----------------------------------------------------------------------------
cRawMesh * cIO::LoadMesh(const char *FileName) {
	Init();

	if(NULL == FileName) {
		cLog::Warning("cIO::LoadMesh(): File name is NULL.");
		return NULL;
	}
	const cStr FileExtension = cStr(FileName).GetFileExtension();
	if(!FileExtension.Length()) {
		cLog::Warning("cIO::LoadMesh(): File name \"%s\" has no extension.", FileName);
		return NULL;
	}

	// Load file:
	cData *Src = LoadFile(FileName);
	if(!Src) {
		cLog::Warning("cIO::LoadMesh(): Can't open file \"%s\".", FileName);
		return NULL;
	}
	if(Src->GetSize() < 1) {
		delete Src;
		cLog::Warning("cIO::LoadMesh(): File \"%s\" is empty.", FileName);
		return NULL;
	}
	cLog::Message("cIO::LoadMesh(): Loaded mesh file \"%s\".", FileName);

	// Looking for codec:
	cMeshCodec *Codec = NULL;
	for(cList<MeshCodecInfo>::Iterator It = m_MeshCodecs.GetIterator(); !It.End(); ++It) {
		if(cStr::EqualsNoCase(It->FileExtension, FileExtension)) {
			Codec = It->Codec;
			break;
		}
	}
	if(!Codec) {
		delete Src;
		cLog::Warning("cIO::LoadMesh(): No codec for mesh file \"%s\".", FileName);
		return NULL;
	}

	// Decoding:
	cRawMesh *Mesh = Codec->Decode(*Src);
	delete Src;
	return Mesh;
} // cIO::LoadMesh

//-----------------------------------------------------------------------------
// cIO::SaveMesh
//-----------------------------------------------------------------------------
bool cIO::SaveMesh(const cRawMesh &Mesh, const char *FileName) {
	Init();

	if(NULL == FileName) {
		cLog::Warning("cIO::SaveMesh(): File name is NULL.");
		return false;
   	}
	const cStr FileExtension = cStr(FileName).GetFileExtension();
	if(!FileExtension.Length()) {
		cLog::Warning("cIO::SaveMesh(): File name \"%s\" has no extension.", FileName);
		return false;
	}

	// Looking for codec:
	cMeshCodec *Codec = NULL;
	for(cList<MeshCodecInfo>::Iterator It = m_MeshCodecs.GetIterator(); !It.End(); ++It) {
		if(cStr::EqualsNoCase(It->FileExtension, FileExtension)) {
			Codec = It->Codec;
			break;
		}
	}
	if(!Codec) {
		cLog::Warning("cIO::SaveMesh(): No codec for mesh file \"%s\".", FileName);
		return false;
	}

	// Encoding:
	cData File;
	Codec->Encode(Mesh, &File);
	return SaveFile(File, FileName);
} // cIO::SaveMesh