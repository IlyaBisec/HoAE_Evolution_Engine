#include "comms.h"

#pragma pack (push, 1)

struct DdsPixelFormat {
	DWORD dwSize;
	DWORD dwFlags;
    DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwRGBAlphaBitMask;
};

struct DdsHeader {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwHeight;
	DWORD dwWidth;
	DWORD dwPitchOrLinearSize;
	DWORD dwDepth;
	DWORD dwMipMapCount;
	DWORD dwReserved1[11];
	DdsPixelFormat ddspf;
	DWORD dwCaps1;
	DWORD dwCaps2;
	DWORD dwReserved2[3];
};

#pragma pack (pop)

enum DdsFlags {
	// Surface header:
	DDS_CAPS		= 0x00000001,
	DDS_HEIGHT		= 0x00000002,
	DDS_WIDTH		= 0x00000004,
	DDS_PITCH		= 0x00000008,
	DDS_PIXELFORMAT = 0x00001000,
	DDS_MIPMAPCOUNT = 0x00020000,
	DDS_LINEARSIZE	= 0x00080000,
	DDS_DEPTH		= 0x00800000,

	// Pixel format:
	DDS_ALPHA_PIXELS	= 0x00000001,
	DDS_ALPHA			= 0x00000002,
	DDS_FOURCC			= 0x00000004,
	DDS_RGB				= 0x00000040,
	DDS_RGBA			= 0x00000041,

	// Complex caps:
	DDS_COMPLEX			= 0x00000008,
	DDS_TEXTURE			= 0x00001000,
	DDS_MIPMAP			= 0x00400000,

	// Cubemaps:
	DDS_CUBEMAP				= 0x00000200,
	DDS_CUBEMAP_POSITIVEX	= 0x00000400,
	DDS_CUBEMAP_NEGATIVEX	= 0x00000800,
	DDS_CUBEMAP_POSITIVEY	= 0x00001000,
	DDS_CUBEMAP_NEGATIVEY	= 0x00002000,
	DDS_CUBEMAP_POSITIVEZ	= 0x00008000,
	DDS_CUBEMAP_NEGATIVEZ	= 0x00200000
};

const DWORD FOURCC_DXT1 = 'D' | 'X' << 8 | 'T' << 16 | '1' << 24;
const DWORD FOURCC_DXT3 = 'D' | 'X' << 8 | 'T' << 16 | '3' << 24;
const DWORD FOURCC_DXT5 = 'D' | 'X' << 8 | 'T' << 16 | '5' << 24;
/*
cImage * cImageDds::Decode(const cData *Src) {
	// Magic:
	Src->SeekAbs(0);
	char Magic[4];
	Src->GetBytes(Magic, 4);
	if(!cStr::Equals("DDS ", Magic, 4)) {
		return NULL;
	}

	// Dds header:
	DdsHeader ddsh;
	Src->GetBytes(&ddsh, sizeof(ddsh));
	int NumComponents = 0;
	if(ddsh.ddspf.dwFlags & DDS_ALPHA_PIXELS) {
		NumComponents = 4; // ARGB
	} else {
		NumComponents = 3; // RGB
	}
	if((ddsh.ddspf.dwFlags & DDS_FOURCC) == 0) {
		return LoadUncompressed(Src, ddsh, NumComponents);
	}

	return LoadCompressed(Src, ddsh, NumComponents);
}

//------------------------------------------------------------------------------------------
// cImageDds::LoadUncompressed
//------------------------------------------------------------------------------------------
cImage * cImageDds::LoadUncompressed(const cData *Src, const DdsHeader &ddsh, int NumComponents) {
	int Width = (int)ddsh.dwWidth;
	int Height = (int)ddsh.dwHeight;

	int BytesPerLine = Width * NumComponents;
	// Alignment to DWORD:
	int Delta = BytesPerLine & 3;
	if(Delta != 0) {
		BytesPerLine += 4 - Delta;
	}

	byte *Line = new byte[BytesPerLine];
	cImage *pTex = new cImage(Width, Height, NumComponents);

	for(int y = 0; y < Height; y++) {
		Src->GetBytes(Line, BytesPerLine);

		// Rearrange components:
		byte *Dest = pTex->GetData() + y * Width * NumComponents;
		byte *Src = Line;
		for(int x = 0; x < Width; x++) {
			Dest[0] = Src[2]; // Red
			Dest[1] = Src[1]; // Green
			Dest[2] = Src[0]; // Blue
			if(NumComponents == 4) {
				Dest[3] = Src[3]; // Alpha
			}
			Dest += NumComponents;
			Src += NumComponents;
		}
	}

	// MipMap:
	for(int m = 1; m < (int)ddsh.dwMipMapCount; m++) {
		Width = cMath::Max(1, Width / 2);
		Height = cMath::Max(1, Height / 2);
		BytesPerLine = Width * NumComponents;

		// DWORD alignment:
		int Delta = BytesPerLine & 3;
		if(Delta != 0) {
			BytesPerLine += 4 - Delta;
		}

		Src->SeekCur(BytesPerLine * Height);
	}
	delete Line;
	return pTex;
} // cImageDds::LoadUncompressed

//----------------------------------------------------------------------------------------
// cImageDds::LoadCompressed
//----------------------------------------------------------------------------------------
cImage * cImageDds::LoadCompressed(const cData *Src, const DdsHeader &ddsh, int NumComponents) {
	// DXT1, DXT3, DXT5
	int NumBlocks = ((ddsh.dwWidth + 3) / 4) * ((ddsh.dwHeight + 3) / 4); // Number of 4 x 4 texel blocks.
	int BlocksSize, Format, Factor;
	
	switch(ddsh.ddspf.dwFourCC) {
		case FOURCC_DXT1: // DXT1's compression ratio is 8:1
			if(NumComponents == 4) {
				Format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			} else {
				Format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			}
			Factor = 2;
			BlocksSize = NumBlocks * 8; // 8 bytes per block.
			break;
		case FOURCC_DXT3: // DXT3's compression ratio is 4:1
			Format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			Factor = 4;
			BlocksSize = NumBlocks * 16; // 16 bytes per block.
			break;
		case FOURCC_DXT5: // DXT5's compression ratio is 4:1
			Format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			Factor = 4;
			BlocksSize = NumBlocks * 16; // 16 bytes per block.
			break;
		default:
			return NULL;
	}

	if(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT == Format || GL_COMPRESSED_RGBA_S3TC_DXT5_EXT == Format) {
		NumComponents = 4;
	} else {
		NumComponents = 3;
	}

	int BufferSize;
	if(ddsh.dwPitchOrLinearSize != 0) {
		if(ddsh.dwMipMapCount > 1) {
			BufferSize = ddsh.dwPitchOrLinearSize * Factor;
		} else {
			BufferSize = ddsh.dwPitchOrLinearSize;
		}
	} else {
		BufferSize = BlocksSize;
	}

	int MipMapsCount = ddsh.dwMipMapCount;
	if(MipMapsCount < 1) {
		MipMapsCount = 1;
	}
	CompressedTexture *pTex = new CompressedTexture(ddsh.dwWidth, ddsh.dwHeight, NumComponents, Format, MipMapsCount, BufferSize);
	Src->GetBytes(pTex->GetData(), BufferSize);
	return pTex;
} // cImageDds::LoadCompressed*/