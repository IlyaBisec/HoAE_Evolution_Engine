#include "comms.h"

//-----------------------------------------------------------------------------
// cImage::GetPixelRgb8
//-----------------------------------------------------------------------------
const cImage::PixelRgb8 cImage::GetPixelRgb8(const int x, const int y) const {
	PixelRgb8 p;
	if(m_Format != Format::Rgb8 || m_Width == 0 || m_Height == 0) {
		p.r = p.g = p.b = 0;
		return p;
	}
	int i = x % m_Width;
	int j = y % m_Height;
	if(i < 0) {
		i += m_Width;
	}
	if(j < 0) {
		j += m_Height;
	}
	const int index = 3 * (m_Width * y + x);
	const unsigned char *Src = &m_pPixels[index];
	p.r = *Src++;
	p.g = *Src++;
	p.b = *Src++;
	return p;
} // cImage::GetPixelRgb8

const cColor cImage::GetPixel(const float s, const float t) const {
	float x = (float)GetWidth() * s - 0.5f;
	float y = (float)GetHeight() * t - 0.5f;
	int x0 = (int)cMath::Floor(x);
	int y0 = (int)cMath::Floor(y);
	int x1 = (int)cMath::Ceil(x);
	int y1 = (int)cMath::Ceil(y);
	PixelRgb8 p00 = GetPixelRgb8(x0, y0);
	PixelRgb8 p10 = GetPixelRgb8(x1, y0);
	PixelRgb8 p01 = GetPixelRgb8(x0, y1);
	PixelRgb8 p11 = GetPixelRgb8(x1, y1);
	float k00 = (1.0f - (x - x0)) * (1.0f - (y - y0));
	float k10 = (x - x0) * (1.0f - (y - y0));
	float k01 = (1.0f - (x - x0)) * (y - y0);
	float k11 = (x - x0) * (y - y0);
	PixelRgb8 p;
	p.r = (unsigned char)((float)p00.r * k00 + (float)p01.r * k01 + (float)p10.r * k10 + (float)p11.r * k11);
	p.g = (unsigned char)((float)p00.g * k00 + (float)p01.g * k01 + (float)p10.g * k10 + (float)p11.g * k11);
	p.b = (unsigned char)((float)p00.b * k00 + (float)p01.b * k01 + (float)p10.b * k10 + (float)p11.b * k11);
//	return p;
	return cColor::Black;
} // cImage::GetPixelRgb8

//*****************************************************************************
// FormatString
//*****************************************************************************

struct FormatString {
	int Format;
	char *String;
};

static const FormatString FormatStrings[] = {
	cImage::Format::None, "None",

	cImage::Format::R8, "R8",
	cImage::Format::Rg8, "Rg8",
	cImage::Format::Rgb8, "Rgb8",
	cImage::Format::Rgba8, "Rgba8",

	cImage::Format::R16, "R16",
	cImage::Format::Rg16, "Rg16",
	cImage::Format::Rgb16, "Rgb16",
	cImage::Format::Rgba16, "Rgba16",

	cImage::Format::R16f, "R16f",
	cImage::Format::Rg16f, "Rg16f",
	cImage::Format::Rgb16f, "Rgb16f",
	cImage::Format::Rgba16f, "Rgba16f",

	cImage::Format::R32f, "R32f",
	cImage::Format::Rg32f, "Rg32f",
	cImage::Format::Rgb32f, "Rgb32f",
	cImage::Format::Rgba32f, "Rgba32f",

	cImage::Format::Rgbe8, "Rgbe8",
	cImage::Format::Rgb565, "Rgb565",
	cImage::Format::Rgba4, "Rgba4",

	cImage::Format::Dxt1, "Dxt1",
	cImage::Format::Dxt3, "Dxt3",
	cImage::Format::Dxt5, "Dxt5"
};

//-----------------------------------------------------------------------------
// cImage::Format::ToString
//-----------------------------------------------------------------------------
const char * cImage::Format::ToString(const int Format) {
	const int Count = sizeof(FormatStrings) / sizeof(FormatString);
	for(int i = 0; i < Count; i++) {
		if(Format == FormatStrings[i].Format) {
			return FormatStrings[i].String;
		}
	}
	return NULL;
} // cImage::Format::ToString

//-----------------------------------------------------------------------------
// cImage::Format::ToFormat
//-----------------------------------------------------------------------------
int cImage::Format::ToFormat(const char *String) {
	const int Count = sizeof(FormatStrings) / sizeof(FormatString);
	for(int i = 0; i < Count; i++) {
		if(cStr::EqualsNoCase(String, FormatStrings[i].String)) {
			return FormatStrings[i].Format;
		}
	}
	return None;
} // cImage::Format::ToFormat


//-----------------------------------------------------------------------------
// cImage.ctor
//-----------------------------------------------------------------------------
cImage::cImage() {
	m_Width = 0;
	m_Height = 0;
	m_Depth = 0;
	m_Format = Format::None;
	m_MipMapCount = 0;
	m_pPixels = NULL;
} // cImage.ctor

//-----------------------------------------------------------------------------
// cImage::Copy
//-----------------------------------------------------------------------------
void cImage::Copy(const cImage &Src) {
	Free();
	
	m_Width = Src.m_Width;
	m_Height = Src.m_Height;
	m_Depth = Src.m_Depth;
	m_Format = Src.m_Format;
	m_MipMapCount = Src.m_MipMapCount;
	const int Size = GetMipMappedSize(0, m_MipMapCount);
	m_pPixels = new unsigned char[Size];
	CopyMemory(m_pPixels, Src.m_pPixels, Size);
} // cImage::Copy

//-----------------------------------------------------------------------------
// cImage.ctor
//-----------------------------------------------------------------------------
cImage::cImage(const cImage &Src) {
	m_pPixels = NULL;
	Copy(Src);
} // cImage.ctor

//-----------------------------------------------------------------------------
// cImage::Free
//-----------------------------------------------------------------------------
void cImage::Free() {
	if(m_pPixels) {
		delete m_pPixels;
		m_pPixels = NULL;
	}
	m_Width = 0;
	m_Height = 0;
	m_Depth = 0;
	m_MipMapCount = 0;
	m_Format = Format::None;
} // cImage::Free

//-----------------------------------------------------------------------------
// cImage.dtor
//-----------------------------------------------------------------------------
cImage::~cImage() {
	Free();
} // cImage.dtor

//---------------------------------------------------------------------------------------------------------------------------
// cImage::Create
//---------------------------------------------------------------------------------------------------------------------------
unsigned char * cImage::Create(const int Format, const int Width, const int Height, const int Depth, const int MipMapCount) {
	Free();

	m_Format = Format;
	m_Width = Width;
	m_Height = Height;
	m_Depth = Depth;
	m_MipMapCount = MipMapCount;
	m_pPixels = new unsigned char[GetMipMappedSize(0, m_MipMapCount)];

	return m_pPixels;
} // cImage::Create

//-----------------------------------------------------------------------------------------------------------
// cImage.ctor
//-----------------------------------------------------------------------------------------------------------
cImage::cImage(const int Format, const int Width, const int Height, const int Depth, const int MipMapCount) {
	m_pPixels = NULL; // ?????????????????????????????????????????????????????????????????????????????????????
	Create(Format, Width, Height, Depth, MipMapCount);
} // cImage.ctor

//-----------------------------------------------------------------------------
// cImage::GetPixels
//-----------------------------------------------------------------------------
unsigned char * cImage::GetPixels(const int MipMapLevel) const {
	return MipMapLevel < m_MipMapCount ? m_pPixels + GetMipMappedSize(0, MipMapLevel) : NULL;
} // cImage::GetPixels

//-----------------------------------------------------------------------------
// cImage::GetMipMapCountFromDimensions
//-----------------------------------------------------------------------------
int cImage::GetMipMapCountFromDimensions() const {
	int Dim = cMath::Max(m_Width, m_Height);
	int Count = 0;
	while(Dim > 0) {
		Dim >>= 1;
		Count++;
	}
	return Count;
} // cImage::GetMipMapCountFromDimensions

//----------------------------------------------------------------------------------------------------------------------------------------------
// cImage::GetMipMappedSize
//----------------------------------------------------------------------------------------------------------------------------------------------
int cImage::GetMipMappedSize(const int MipMapFm /*= 0*/, const int MipMapCount /*= MipMapsAll*/, const int SrcFormat /*= Format::None*/) const {
	int Width = GetWidth(MipMapFm);
	int Height = GetHeight(MipMapFm);
	int Depth = GetDepth(MipMapFm);
	const int Format = SrcFormat == Format::None ? m_Format : SrcFormat;
	int Size = 0;

	int Level = MipMapCount;
	while(Level) {
		if(cImage::Format::IsCompressed(Format)) {
			Size += ((Width + 3) >> 2) * ((Height + 3) >> 2);
		} else {
			Size += Width * Height * Depth;
		}
		Width /= 2;
		Height /= 2;
		Depth /= 2;
		if(Width + Height + Depth == 0) {
			break;
		}
		Width = cMath::Max(Width, 1);
		Height = cMath::Max(Height, 1);
		Depth = cMath::Max(Depth, 1);
		Level--;
	}
	if(cImage::Format::IsCompressed(Format)) {
		Size *= cImage::Format::BytesPerBlock(Format);
	} else {
		Size *= cImage::Format::BytesPerPixel(Format);
	}
	return Depth == 0 ? 6 * Size : Size;
} // cImage::GetMipMappedSize

//---------------------------------------------------------------------------------------------------
// cImage::GetPixelCount
//---------------------------------------------------------------------------------------------------
int cImage::GetPixelCount(const int MipMapFm /*= 0*/, const int MipMapCount /*= MipMapsAll*/) const {
	int Width = GetWidth(MipMapFm);
	int Height = GetHeight(MipMapFm);
	int Depth = GetDepth(MipMapFm);
	int Size = 0;

	int Level = MipMapCount;
	while(Level) {
		Size += Width * Height * Depth;
		Width /= 2;
		Height /= 2;
		Depth /= 2;
		if(Width + Height + Depth == 0) {
			break;
		}
		Width = cMath::Max(Width, 1);
		Height = cMath::Max(Height, 1);
		Depth = cMath::Max(Depth, 1);
		Level--;
	}
	
	return 0 == Depth ? 6 * Size : Size;
} // cImage::GetPixelCount

//-----------------------------------------------------------------------------
// cImage::SwapChannels
//-----------------------------------------------------------------------------
bool cImage::SwapChannels(const int Ch0, const int Ch1) {
	if(!Format::IsPlain(m_Format)) {
		return false;
	}

	const int NPixels = GetPixelCount(0, m_MipMapCount);
	const int NChannels = Format::ChannelCount(m_Format);

	if(m_Format <= Format::Rgba8) {
		cMath::SwapChannels((unsigned char *)m_pPixels, NPixels, NChannels, Ch0, Ch1);
	} else if(m_Format <= Format::Rgba16f) {
		cMath::SwapChannels((unsigned short *)m_pPixels, NPixels, NChannels, Ch0, Ch1);
	} else {
		cMath::SwapChannels((float *)m_pPixels, NPixels, NChannels, Ch0, Ch1);
	}
	return true;
} // cImage::SwapChannels

//-----------------------------------------------------------------------------
// cImage::ToGrayScale
//-----------------------------------------------------------------------------
bool cImage::ToGrayScale() {
	const int NChannels = Format::ChannelCount(m_Format);
	if(!Format::IsPlain(m_Format) || Format::IsFloat(m_Format) || NChannels < 3) {
		return false;
	}
	const int NPixels = GetPixelCount(0, m_MipMapCount);
	unsigned char *GrayPixels = new unsigned char[NPixels];
	unsigned char *Dest = GrayPixels;

	const unsigned char *Src = m_pPixels;
	for(int i = 0; i < NPixels; i++) {
		*Dest = (77 * Src[0] + 151 * Src[1] + 28 * Src[2] + 128) >> 8;
		Dest++;
		Src += NChannels;
	}
	m_Format = Format::I8;
	delete m_pPixels;
	m_pPixels = GrayPixels;

	return true;
} // cImage::ToGrayScale

//--------------------------------------------------------------------------------------------------------------------------
// cImage::ToNormalMap
//--------------------------------------------------------------------------------------------------------------------------
bool cImage::ToNormalMap(const bool KeepHeight /*= false*/, const float Z /*= 1.0f*/, const float MipMapScaleZ /*= 2.0f*/) {
	if(Format::Rgb8 == m_Format || Format::Rgba8 == m_Format) {
		ToGrayScale();
	}
	if(m_Format != Format::I8) {
		return false;
	}

	static const float SobelX[5][5] = {
		1,  2, 0,  -2, -1,
		4,  8, 0,  -8, -4,
		6, 12, 0, -12, -6,
		4,  8, 0,  -8, -4,
		1,  2, 0,  -2, -1
	};
	static const float SobelY[5][5] = {
		 1,  4,   6,  4,  1,
		 2,  8,  12,  8,  2,
		 0,  0,   0,  0,  0,
		-2, -8, -12, -8, -2,
		-1, -4,  -6, -4, -1
	};

	float sZ = (48 * 255) * 128.0f / cMath::Max(m_Width, m_Height);
	unsigned char *NormalMapPixels = new unsigned char[(KeepHeight ? 4 : 3) * GetMipMappedSize(0, m_MipMapCount)];
	unsigned char *Dest = NormalMapPixels;

	for(int Level = 0; Level < m_MipMapCount; Level++) {
		unsigned char *Src = GetPixels(Level);

		const int Width = GetWidth(Level);
		const int Height = GetHeight(Level);

		for(int y = 0; y < Height; y++) {
			for(int x = 0; x < Width; x++) {
				// Sobel Filter:
				float sX = 0.0f, sY = 0.0f;
				for(int dy = 0; dy < 5; dy++) {
					int fy = (y + dy - 2) & (Height - 1);
					for(int dx = 0; dx < 5; dx++) {
						int fx = (x + dx - 2) & (Width - 1);
						sX += SobelX[dy][dx] * Src[fy * Width + fx];
						sY += SobelY[dy][dx] * Src[fy * Width + fx];
					}
				}
				float InvLen = 1.0f / cMath::Sqrt(sX * sX + sY * sY + sZ * sZ);

				Dest[0] = (unsigned char)(127.5f * (sX * InvLen + 1.0f));
				Dest[1] = (unsigned char)(127.5f * (sY * InvLen + 1.0f));
				Dest[2] = (unsigned char)(127.5f * (sZ * InvLen + 1.0f));
				Dest += 3;
				if(KeepHeight) {
					*Dest++ = Src[y * Width + x];
				}
			}
		}
		sZ *= MipMapScaleZ;
	}

	m_Format = KeepHeight ? Format::Rgba8 : Format::Rgb8;
	delete m_pPixels;
	m_pPixels = NormalMapPixels;
	return true;
} // cImage::ToNormalMap

/*
//----------------------------------------------------------------------------------------------------------------
// CompressedTexture.ctor
//----------------------------------------------------------------------------------------------------------------
CompressedTexture::CompressedTexture(int Width, int Height, int NumComponents, int Format, int Levels, int Size) {
	m_Width = Width;
	m_Height = Height;
	m_NumComponents = NumComponents;
	m_Format = Format;
	m_pData = new byte[Size];
	m_Levels = Levels;
	m_IsCompressed = true;
} // CompressedTexture.ctor

//-----------------------------------------------------------------------------
// CompressedTexture::Upload
//-----------------------------------------------------------------------------
bool CompressedTexture::Upload(const int Target, const bool MipMap) {
	int l = m_Levels;
	if(!MipMap) {
		l = 1;
	}
	int BlockSize;
	if(m_Format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || m_Format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) {
		BlockSize = 8;
	} else {
		BlockSize = 16;
	}
	if(MipMap && m_Levels < 1) {
		glTexParameteri(Target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	}
	int w = m_Width;
	int h = m_Height;
	int Offset = 0;
	for(int i = 0; i < l; i++) {
		if(0 == w) {
			w = 1;
		}
		if(0 == h) {
			h = 1;
		}
		int Size = ((w + 3) / 4) * ((h + 3) / 4) * BlockSize;
		glCompressedTexImage2DARB(Target, i, m_Format, w, h, 0, Size, m_pData + Offset);
		Offset += Size;
		w /= 2;
		h /= 2;
	}
	return true;
} // CompressedTexture::Upload

void DecodeColorBlock(byte *Dst, int Width, int Height, int xOffset, int yOffset, int Format, int Red, int Blue, byte *Src) {
	byte Colors[4][3];
	word c0 = *(word *)Src;
	word c1 = *(word *)(Src + 2);

	Colors[0][0] = ((c0 >> 11) & 0x1f) << 3;
	Colors[0][1] = ((c0 >> 5) & 0x3f) << 2;
	Colors[0][2] = (c0 & 0x1f) << 3;

	Colors[1][0] = ((c1 >> 11) & 0x1f) << 3;
	Colors[1][1] = ((c1 >> 5) & 0x3f) << 2;
	Colors[1][2] = (c1 & 0x1f) << 3;

	if(c0 > c1 || GL_COMPRESSED_RGBA_S3TC_DXT5_EXT == Format) {
		for(int i = 0; i < 3; i++) {
			Colors[2][i] = (2 * Colors[0][i] + Colors[1][i] + 1) / 3;
			Colors[3][i] = (Colors[0][i] + 2 * Colors[1][i] + 1) / 3;
		}
	} else {
		for(int i = 0; i < 3; i++) {
			Colors[2][i] = (Colors[0][i] + Colors[1][i] + 1) >> 1;
			Colors[3][i] = 0;
		}
	}

	Src += 4;
	for(int y = 0; y < Height; y++) {
		byte *Ptr = Dst + yOffset * y;
		dword Indexes = Src[y];
		for(int x = 0; x < Width; x++) {
			byte index = (byte)(Indexes & 0x3);
			Ptr[Red] = Colors[index][0];
			Ptr[1] = Colors[index][1];
			Ptr[Blue] = Colors[index][2];
			Indexes >>= 2;

			Ptr += xOffset;
		}
	}
}

cImage * CompressedTexture::Decompress() const {
	cImage *pTex = new cImage(m_Width, m_Height, m_NumComponents);
	int Sx = m_Width < 4 ? m_Width : 4;
	int Sy = m_Height < 4 ? m_Height : 4;

	byte *Src = m_pData;
	for(int y = 0; y < m_Height; y += 4) {
		for(int x = 0; x < m_Width; x += 4) {
			byte *Dst = pTex->GetData() + (y * m_Width + x) * m_NumComponents;
			if(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT == m_Format) {
//				DecodeDXT3AlphaBlock(Dst + 3, Sx, Sy, m_NumComponents, m_Width * m_NumComponents, Src);
				Src += 8;
			} else if(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT == m_Format) {
//				DecodeDXT5AlphaBlock(Dst + 3, Sx, Sy, m_NumComponents, m_Width * m_NumComponents, Src);
				Src += 8;
			}
			DecodeColorBlock(Dst, Sx, Sy, m_NumComponents, m_Width * m_NumComponents, m_Format, 0, 2, Src);
			Src += 8;
		}
	}
	return pTex;
}*/