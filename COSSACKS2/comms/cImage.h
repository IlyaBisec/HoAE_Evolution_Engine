#pragma once

//*****************************************************************************
// cImage
//*****************************************************************************

class cImage {
public:
	struct Format {
		static const int None = 0;
		
		// Plain formats:
		static const int R8 = 1;
		static const int I8 = 1;
		static const int Rg8 = 2;
		static const int Ia8 = 2;
		static const int Rgb8 = 3;
		static const int Rgba8 = 4;

		static const int R16 = 5;
		static const int I16 = 5;
		static const int Rg16 = 6;
		static const int Ia16 = 6;
		static const int Rgb16 = 7;
		static const int Rgba16 = 8;

		static const int R16f = 9;
		static const int I16f = 9;
		static const int Rg16f = 10;
		static const int Ia16f = 10;
		static const int Rgb16f = 11;
		static const int Rgba16f = 12;

		static const int R32f = 13;
		static const int I32f = 13;
		static const int Rg32f = 14;
		static const int Ia32f = 14;
		static const int Rgb32f = 15;
		static const int Rgba32f = 16;

		// Packed formats:
		static const int Rgbe8 = 17;
		static const int Rgb565 = 18;
		static const int Rgba4 = 19;

		// Compressed formats:
		static const int Dxt1 = 20;
		static const int Dxt3 = 21;
		static const int Dxt5 = 22;

		static bool IsPlain(const int Format) {
			return Format <= cImage::Format::Rgba32f;
		}

		static bool IsPacked(const int Format) {
			return Format >= cImage::Format::Rgbe8 && Format <= cImage::Format::Rgba4;
		}

		static bool IsCompressed(const int Format) {
			return Format >= cImage::Format::Dxt1 && Format <= cImage::Format::Dxt5;
		}

		static bool IsFloat(const int Format) {
			return Format >= cImage::Format::R16f && Format <= cImage::Format::Rgbe8;
		}

		static int ChannelCount(const int Format) {
			static int Count[] = {
				0, // None
				1, // R8, I8
				2, // Rg8, Ia8
				3, // Rgb8
				4, // Rgba8
				1, // R16, I16
				2, // Rg16, Ia16
				3, // Rgb16
				4, // Rgba16
				1, // R16f, I16f
				2, // Rg16f, Ia16f
				3, // Rgb16f
				4, // Rgba16f
				1, // R32f, I32f
				2, // Rg32f, Ia32f
				3, // Rgb32f
				4, // Rgba32f
				3, // Rgbe8
				3, // Rgb565
				4, // Rgba4
				3, // Dxt1
				4, // Dxt3
				4, // Dxt5
			};
			return Count[Format];
		}

		static int BytesPerChannel(const int Format) { // Only plain formats.
			cAssert(IsPlain(Format));

			if(Format <= cImage::Format::Rgba8) return 1;
			if(Format <= cImage::Format::Rgba16f) return 2;
			return 4;
		}

		static int BytesPerPixel(const int Format) { // Only plain and packed formats.
			cAssert(IsPlain(Format) || IsPacked(Format));
			
			static int Bytes[] = {
				0, // None
				1, // R8, I8
				2, // Rg8, Ia8
				3, // Rgb8
				4, // Rgba8
				2, // R16, I16
				4, // Rg16, Ia16
				6, // Rgb16
				8, // Rgba16
				2, // R16f, I16f
				4, // Rg16f, Ia16f
				6, // Rgb16f
				8, // Rgba16f
				4, // R32f, I32f
				8, // Rg32f, Ia32f
				12, // Rgb32f
				16, // Rgba32f
				4, // Rgbe8
				2, // Rgb565
				2  // Rgba4
			};
			return Bytes[Format];
		}

		static int BytesPerBlock(const int Format) { // Only compressed formats.
			cAssert(IsCompressed(Format));
			return cImage::Format::Dxt1 == Format ? 8 : 16;
		}

		static const char * ToString(const int Format);
		static int ToFormat(const char *String);
	};

	cImage();
	void Copy(const cImage &Src);
	cImage(const cImage &Src);
	unsigned char * Create(const int Format, const int Width, const int Height, const int Depth, const int MipMapCount);
	cImage(const int Format, const int Width, const int Height, const int Depth, const int MipMapCount);
	void Free();
	~cImage();
	
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
	int GetDepth() const { return m_Depth; }
	int GetWidth(const int MipMapLevel) const { return cMath::Max(1, m_Width >> MipMapLevel); }
	int GetHeight(const int MipMapLevel) const { return cMath::Max(1, m_Height >> MipMapLevel); }
	int GetDepth(const int MipMapLevel) const { return cMath::Max(1, m_Depth >> MipMapLevel); }

	int GetFormat() const { return m_Format; }
	
	unsigned char * GetPixels() const { return m_pPixels; }
	unsigned char * GetPixels(const int MipMapLevel) const;
	
	int GetMipMapCount() const { return m_MipMapCount; }
	int GetMipMapCountFromDimensions() const;
	static const int MipMapsAll = 127;
	int GetMipMappedSize(const int MipMapFm = 0, const int MipMapCount = MipMapsAll, const int SrcFormat = Format::None) const;
	int GetPixelCount(const int MipMapFm = 0, const int MipMapCount = MipMapsAll) const;

	bool Is1D() const { return m_Depth == 1 && m_Height == 1; }
	bool Is2D() const { return m_Depth == 1 && m_Height > 1; }
	bool Is3D() const { return m_Depth > 1; }
	bool IsCube() const { return m_Depth == 0; }
	
	bool SwapChannels(const int Ch0, const int Ch1);
	bool ToGrayScale();
	bool ToNormalMap(const bool KeepHeight = false, const float Z = 1.0f, const float MipMapScaleZ = 2.0f);

	struct PixelRgb8 {
		unsigned char r, g, b;
	};
	const PixelRgb8 GetPixelRgb8(const int x, const int y) const;
	const cColor GetPixel(const float s, const float t) const;

	struct PixelRgba8 {
		unsigned char r, g, b, a;
	};
	const PixelRgba8 GetPixelRgba8(const int x, const int y) const;

protected:
	int m_Width;
	int m_Height;
	int m_Depth;
	int m_Format;
	int m_MipMapCount;
	unsigned char *m_pPixels;
};