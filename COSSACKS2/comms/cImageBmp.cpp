#include "comms.h"

#undef BI_RGB
#undef BI_RLE8
#undef BI_RLE4

#define BI_RGB	0
#define BI_RLE8	1
#define BI_RLE4	2

#pragma pack (push, 1)

struct BmpFileHeader {
	WORD	Type;					// Type of file (must be BM).
	DWORD	Size;					// Size of file in bytes.
	WORD	Reserved1, Reserved2;	// Reserved (must be zero).
	DWORD	OffsetToBits;			// Offset in bytes from this header to actual data.
};

struct BmpInfoHeader {
	DWORD	Size;			// Size of this header.
	LONG	Width;			// Width of the bitmap in pixels.
	LONG	Height;			// Height of the bitmap in pixels.
	WORD	Planes;			// Number of planes.
	WORD	BitCount;		// Bits per pixel.
	DWORD	Compression;	// Compression type (BI_RGB - no compression).
	DWORD	SizeImage;		// Size of image in bytes.
	LONG	xPelsPerMeter;	// Hor resolution.
	LONG	yPelsPerMeter;	// Vert resolution.
	DWORD	ClrUsed;		// Number of color indexes.
	DWORD	ClrImportant;	// Number of important colors.
};

struct RgbQuad {
	BYTE	Blue;
	BYTE	Green;
	BYTE	Red;
	BYTE	Reserved;
};

#pragma pack (pop)

//-----------------------------------------------------------------------------
// cImageBmp::Decode
//-----------------------------------------------------------------------------
cImage * cImageBmp::Decode(const cData *Src) {
#pragma pack (push, 1)
	struct BmpHeader {
		word	Magic;
		byte	Junk[16];
		dword	Width;
		dword	Height;
		word	Junk2;
		word	Bpp;
		word	Compression;
		byte	Junk3[22];
	};
#pragma pack (pop)

	BmpHeader Hdr;
	Src->Read(&Hdr, sizeof(BmpHeader));
	if(Hdr.Magic != ('B' | 'M' << 8)) {
		return NULL;
	}
	unsigned char /*Palette[1024], *Buffer, */*Dest;
	int i;
	if(24 == Hdr.Bpp || 32 == Hdr.Bpp) {
		const int NChannels = Hdr.Bpp == 24 ? 3 : 4;
		const int Format = Hdr.Bpp == 24 ? cImage::Format::Rgb8 : cImage::Format::Rgba8;
		cImage *Image = new cImage(Format, Hdr.Width, Hdr.Height, 1, 1);
		for(i = Hdr.Height - 1; i >= 0; i--) {
			Dest = Image->GetPixels() + i * Hdr.Width * NChannels;
			Src->Read(Dest, Hdr.Width * NChannels);
			cMath::SwapChannels(Dest, Hdr.Width, NChannels, 0, 2);
		}
		return Image;
	}

	cLog::Warning("cImageBmp::Decode(): Can't decode file \"%s\"", Src->GetFileName());
	return NULL;

/*	if(Src->GetPtrCur() == NULL) {
		return NULL;
	}
	
	const BmpFileHeader *FileHdr = (const BmpFileHeader *)Src->GetPtrCur();
	const BmpInfoHeader *InfoHdr = (const BmpInfoHeader *)Src->GetPtrAbs(sizeof(BmpFileHeader));
	
	bool Success = FileHdr->Type == ('B' | 'M' << 8) && InfoHdr->Size == sizeof(BmpInfoHeader) && InfoHdr->Planes == 1 &&
		(InfoHdr->BitCount == 1 || InfoHdr->BitCount == 2 || InfoHdr->BitCount == 4 ||
		InfoHdr->BitCount == 8 || InfoHdr->BitCount == 24 || InfoHdr->BitCount == 32) &&
		(InfoHdr->Compression == BI_RGB || InfoHdr->Compression == BI_RLE4 || InfoHdr->Compression == BI_RLE8);

	if(!Success) {
		cLog::Warning("cImageCodecBmp::Decode(): Can't decode \"%s\"", Src->GetFileName());
		return NULL;
	}

	const int NColors = 1 << InfoHdr->BitCount;
	const int Width = InfoHdr->Width;
	const int Height = InfoHdr->Height;
	const int NChannels = InfoHdr->BitCount == 32 ? 4 : 3;
	
	const RgbQuad *SrcPal = (const RgbQuad *)(InfoHdr + 1);
	DWORD Palette[256];
	
	if(InfoHdr->BitCount <= 8) {
		for(int i = 0; i < NumColors; i++) {
			Palette[i] = cMath::Rgba(SrcPal[i].Red, SrcPal[i].Green, SrcPal[i].Blue);
		}
	}

	cImage *Tex = new cImage(Width, Height, NumComponents);
	if(NULL == Tex) {
		return NULL;
	}
	
	DWORD *Buffer = new DWORD[Width];
	Src->SeekAbs(FileHdr->OffsetToBits);

	if(InfoHdr->Compression == BI_RGB) {
		if(InfoHdr->BitCount == 1) { // Mono
			Success = LoadMono(Src, Tex, Buffer);
		} else if(InfoHdr->BitCount == 4) { // 16 colors uncompressed
			Success = LoadRgb4(Src, Tex, Buffer, Palette);
		} else if(InfoHdr->BitCount == 8) { // 256 colors uncompressed
			Success = LoadRgb8(Src, Tex, Buffer, Palette);
		} else if(InfoHdr->BitCount == 24) { // True color
			Success = LoadRgb24(Src, Tex, Buffer);
		} else if(InfoHdr->BitCount == 32) { // True color with alpha - channel
			Success = LoadRgb32(Src, Tex, Buffer);
		}
	} else if(InfoHdr->Compression == BI_RLE4) { // 16 colors RLE compressed
		Success = LoadRLE4(Src, Tex, Buffer, Palette);
	} else if(InfoHdr->Compression == BI_RLE8) { // 256 colors RLE compressed
		Success = LoadRLE8(Src, Tex, Buffer, Palette);
	} else {
		Success = false;
	}

	delete Buffer;

	if(!Success) {
		delete Tex;
		return NULL;
	}
	return Tex;*/
}
/*
//-----------------------------------------------------------------------------
// cImageBmp::LoadMono
//-----------------------------------------------------------------------------
bool cImageBmp::LoadMono(const cData *Src, cImage *Tex, dword *Buffer) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();
	
	int BytesPerLine = (Width + 7) / 8;
	const int Delta = BytesPerLine & 3; // Alignment to DWORD
	if(Delta != 0) {
		BytesPerLine += 4 - Delta;
	}

	byte *Line = new byte[BytesPerLine];
	
	for(int y = Height - 1; y >= 0; y--) {
		if(Src->GetBytes(Line, BytesPerLine) != BytesPerLine) {
			delete Line;
			return false;
		}

		ZeroMemory(Buffer, sizeof(dword) * Width);

		for(int x = 0; x < Width; x++) {
			if(Line[x >> 3] & (0x80 >> (x & 7))) {
				Buffer[x] = cMath::Rgba(255, 255, 255);
			} else {
				Buffer[x] = cMath::Rgba(0, 0, 0);
			}
		}

		Tex->PutLine(y, Buffer);
	}

	delete Line;
	return true;
}

//--------------------------------------------------------------------------------------
// cImageBmp::LoadRgb4
//--------------------------------------------------------------------------------------
bool cImageBmp::LoadRgb4(const cData *Src, cImage *Tex, dword *Buffer, const dword *Palette) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();
	int Index;

	for(int y = Height - 1; y >= 0; y--) {
		ZeroMemory(Buffer, sizeof(dword) * Width);
		for(int x = 0; x < Width; x++) {
			if((x & 1) == 0) {
				if((Index = Src->GetByte()) == -1) {
					return false;
				}
				Buffer[x] = Palette[Index >> 4];
			} else {
				Buffer[x] = Palette[Index & 0x0f];
			}
		}

		// DWORD alignment
		for(int Count = (Width + 1) / 2; Count % 4; Count++) {
			if(Src->GetByte() == -1) {
				return false;
			}
		}

		Tex->PutLine(Height - 1 - y, Buffer);
	}
	return false;
}

//--------------------------------------------------------------------------------------
// cImageBmp::LoadRgb8
//--------------------------------------------------------------------------------------
bool cImageBmp::LoadRgb8(const cData *Src, cImage *Tex, dword *Buffer, const dword *Palette) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();
	int Count, Index;

	for(int y = Height - 1; y >= 0; y--) {
		ZeroMemory(Buffer, sizeof(dword) * Width);
		for(int x = Count = 0; x < Width; x++, Count++) {
			if((Index = Src->GetByte()) == -1) {
				return false;
			}
			Buffer[x] = Palette[Index];
		}

		// DWORD alignment
		for(; Count % 4; Count++) {
			if(Src->GetByte() == -1) {
				return false;
			}
		}

		Tex->PutLine(Height - 1 - y, Buffer);
	}
	return true;
}

//-----------------------------------------------------------------------------
// cImageBmp::LoadRgb24
//-----------------------------------------------------------------------------
bool cImageBmp::LoadRgb24(const cData *Src, cImage *Tex, dword *Buffer) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();
	int Count, Red, Green, Blue;

	for(int y = Height - 1; y >= 0; y--) {
		ZeroMemory(Buffer, sizeof(dword) * Width);
		for(int x = Count = 0; x < Width; x++, Count += 3) {
			if((Blue = Src->GetByte()) == -1) {
				return false;
			}
			if((Green = Src->GetByte()) == -1) {
				return false;
			}
			if((Red = Src->GetByte()) == -1) {
				return false;
			}
			Buffer[x] = cMath::Rgba(Red, Green, Blue);
		}

		// DWORD alignment
		for(; Count % 4; Count++) {
			if(Src->GetByte() == -1) {
				return false;
			}
		}

		Tex->PutLine(Height - 1 - y, Buffer);
	}
	return true;
}

//-----------------------------------------------------------------------------
// cImageBmp::LoadRgb32
//-----------------------------------------------------------------------------
bool cImageBmp::LoadRgb32(const cData *Src, cImage *Tex, dword *Buffer) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();
	int Red, Green, Blue, Alpha;

	for(int y = Height - 1; y >= 0; y--) {
		ZeroMemory(Buffer, sizeof(dword) * Width);
		for(int x = 0; x < Width; x++) {
			if((Blue = Src->GetByte()) == -1) {
				return false;
			}
			if((Green = Src->GetByte()) == -1) {
				return false;
			}
			if((Red = Src->GetByte()) == -1) {
				return false;
			}
			if((Alpha = Src->GetByte()) == -1) {
				return false;
			}
			Buffer[x] = cMath::Rgba(Red, Green, Blue, Alpha);
		}
		Tex->PutLine(Height - 1 - y, Buffer);
	}
	return true;
}

//--------------------------------------------------------------------------------------
// cImageBmp::LoadRLE4
//--------------------------------------------------------------------------------------
bool cImageBmp::LoadRLE4(const cData *Src, cImage *Tex, dword *Buffer, const dword *Palette) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();
	int y = Height - 1;
	int x = 0;
	int Count, Index;

	ZeroMemory(Buffer, sizeof(dword) * Width);

	while(true) {
		if((Count = Src->GetByte()) == -1) {
			return false;
		}
		if(Count == 0) {
			if((Count = Src->GetByte()) == -1) {
				return false;
			}
			if(Count == 0) { // End of line.
				Tex->PutLine(Height - 1 - y, Buffer);
				ZeroMemory(Buffer, sizeof(dword) * Width);
				y--;
				x = 0;
			} else if(Count == 1) { // End of RLE buffer.
				break;
			} else if(Count == 2) { // Delta record.
				Tex->PutLine(Height - 1 - y, Buffer);
				ZeroMemory(Buffer, sizeof(dword) * Width);
				y -= Src->GetByte();
				x += Src->GetByte();
			} else { // Start of unencoded block.
				for(int i = 0; i < Count; i += 2, x += 2) {
					if((Index = Src->GetByte()) == -1) {
						return false;
					}
					Buffer[x] = Palette[Index >> 4];
					Buffer[x + 1] = Palette[Index & 0x0f];
				}

				if((Count / 2) & 1) {
					if(Src->GetByte() == -1) {
						return false;
					}
				}
			}
		} else { // RLE decoded record
			if((Index = Src->GetByte()) == -1) {
				return false;
			}

			for(int i = 0; i < Count; i++, x++) {
				if(i & 1) {
					Buffer[x] = Palette[Index & 0x0f];
				} else {
					Buffer[x] = Palette[Index >> 4];
				}
			}
		}
	}
	return true;
}

//--------------------------------------------------------------------------------------
// cImageBmp::LoadRLE8
//--------------------------------------------------------------------------------------
bool cImageBmp::LoadRLE8(const cData *Src, cImage *Tex, DWORD *Buffer, const DWORD *Palette) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();
	int y = Height - 1;
	int x = 0;
	int Count, Index;

	ZeroMemory(Buffer, sizeof(dword) * Width);

	while(true) {
		if((Count = Src->GetByte()) == -1) {
			return false;
		}
		if(Count == 0) {
			if((Count = Src->GetByte()) == -1) {
				return false;
			}
			if(Count == 0) { // End of line
				Tex->PutLine(Height - 1 - y, Buffer);
				ZeroMemory(Buffer, sizeof(dword) * Width);
				y--;
				x = 0;
			} else if(Count == 1) { // End of RLE buffer
				break;
			} else if(Count == 2) { // Delta record
				Tex->PutLine(Height - 1 - y, Buffer);
				ZeroMemory(Buffer, sizeof(dword) * Width);
				y -= Src->GetByte();
				x += Src->GetByte();
			} else { // Start of unencoded block
				for(int i = 0; i < Count; i++, x++) {
					if((Index = Src->GetByte()) == -1) {
						return false;
					}
					Buffer[x] = Palette[Index];
				}
				if(Count & 1) {
					if(Src->GetByte() == -1) {
						return false;
					}
				}
			}
		} else { // RLE decoded record
			if((Index = Src->GetByte()) == -1) {
				return false;
			}
			for(int i = 0; i < Count; i++, x++) {
				Buffer[x] = Palette[Index];
			}
		}
	}
	return true;
}*/