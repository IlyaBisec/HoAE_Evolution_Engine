#include "comms.h"

#pragma pack (push, 1)

struct TgaHeader {
	byte	idLength;
	byte	ColormapType;
	byte	ImageType;
	word	ColormapIndex;
	word	ColormapLength;
	byte	ColormapEntrySize;
	word	xOrigin;
	word	yOrigin;
	word	Width;
	word	Height;
	byte	PixelSize;
	byte	ImageDesc;
};

#pragma pack (pop)
/*
//-----------------------------------------------------------------------------
// cImageTga::Load
//-----------------------------------------------------------------------------
cImage * cImageTga::Decode(const cData *Src) {
	if(Src->GetPtrCur() == NULL) {
		return NULL;
	}
	const TgaHeader *Hdr = (const TgaHeader *)Src->GetPtrCur();

	bool KnownType = Hdr->ImageType == 0 || Hdr->ImageType == 1 || Hdr->ImageType == 2 || Hdr->ImageType == 3 ||
		Hdr->ImageType == 9 || Hdr->ImageType == 10 || Hdr->ImageType == 11;
	if(!KnownType) {
		return NULL;
	}

	bool PixelSizeOk = Hdr->PixelSize == 32 || Hdr->PixelSize == 24 || Hdr->PixelSize == 16 ||
		Hdr->PixelSize == 15 || Hdr->PixelSize == 8;
	if(!PixelSizeOk) {
		return NULL;
	}

	const int Width = Hdr->Width;
	const int Height = Hdr->Height;
	const int NumComponents = Hdr->PixelSize == 32 || (Hdr->PixelSize == 8 && Hdr->ColormapEntrySize == 32) ? 4 : 3;
	int Offset = sizeof(TgaHeader);
	dword Palette[256];

	if(Hdr->idLength != 0) {
		Offset += Hdr->idLength;
	}

	byte *Bits = (byte *)Src->GetPtrCur() + Offset;
	Src->SeekAbs(Offset);

	cImage *Tex = new cImage(Width, Height, NumComponents);
	dword *Buffer = new dword[Width];

	// Filling palette for color - mapped images:
	if(Hdr->ImageType == 1 || Hdr->ImageType == 9) {
		if(Hdr->ColormapEntrySize == 15 || Hdr->ColormapEntrySize == 16) { // 16(15) bpp palette
			byte *Ptr = (byte *)Bits;
			for(int i = 0; i < Hdr->ColormapLength; i++) {
				int a = *Ptr++;
				int b = *Ptr++;
				Palette[i] = cMath::Rgba(a & 0x1f, ((a & 0xe0) >> 5) | ((b & 0x03) << 3), (b & 0x7c) >> 2);
			}
			Bits += 2 * Hdr->ColormapLength;
			Src->SeekCur(2 * Hdr->ColormapLength);
		} else if(Hdr->ColormapEntrySize == 24) { // 24 bpp palette
			byte *Ptr = (byte *)Bits;
			for(int i = 0; i < Hdr->ColormapLength; i++, Ptr += 3) {
				Palette[i] = cMath::Rgba(Ptr[2], Ptr[1], Ptr[0]);
			}
			Bits += 3 * Hdr->ColormapLength;
			Src->SeekCur(3  * Hdr->ColormapLength);
		} else if(Hdr->ColormapEntrySize == 32) { // 32 bpp palette
			CopyMemory(Palette, Bits, sizeof(dword) * Hdr->ColormapLength);
			Bits += 4 * Hdr->ColormapLength;
			Src->SeekCur(4 * Hdr->ColormapLength);
		}
	} else if(Hdr->ColormapEntrySize == 0 && Hdr->PixelSize == 8) { // 8 bpp grey - scale w/o palette
		for(int i = 0; i < 256; i++) {
			Palette[i] = cMath::Rgba(i, i, i, i);
		}
	}

	bool VertInvert = (Hdr->ImageDesc & 0x20) != 0;
	bool Success = false;
	if(Hdr->ImageType >= 1 && Hdr->ImageType <= 3) { // Unencoded image
		if(Hdr->PixelSize == 8) {
			Success = LoadRgb8(Src, Tex, Buffer, Palette, VertInvert);
		} else if(Hdr->PixelSize == 16 || Hdr->PixelSize == 15) {
			Success = LoadRgb15(Src, Tex, Buffer, VertInvert);
		} else if(Hdr->PixelSize == 24) {
			Success = LoadRgb24(Src, Tex, Buffer, VertInvert);
		} else {
			Success = LoadRgb32(Src, Tex, Buffer, VertInvert);
		}
	} else if(Hdr->ImageType >= 9 && Hdr->ImageType <= 11) { // Rle image
		if(Hdr->PixelSize == 8) {
			Success = LoadRle8(Src, Tex, Buffer, Palette, VertInvert);
		} else if(Hdr->PixelSize == 24) {
			Success = LoadRle24(Src, Tex, Buffer, VertInvert);
		} else {
			Success = LoadRle32(Src, Tex, Buffer, VertInvert);
		}
	}

	delete Buffer;
	if(!Success) {
		delete Tex;
		return NULL;
	}
	return Tex;
} // cImageTga::Load

//-------------------------------------------------------------------------------------------------------
// cImageTga::LoadRgb8
//-------------------------------------------------------------------------------------------------------
bool cImageTga::LoadRgb8(const cData *Src, cImage *Tex, dword *Buffer, const dword *Palette, bool VertInvert) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();

	for(int y = 0; y < Height; y++) {
		ZeroMemory(Buffer, sizeof(dword) * Width);
		for(int x = 0; x < Width; x++) {
			int index = Src->GetByte();
			if(index == -1) {
				return false;
			}
			Buffer[x] = Palette[index];
		}
		Tex->PutLine(VertInvert ? Height - 1 - y : y, Buffer);
	}
	return true;
} // cImageTga::LoadRgb8

//----------------------------------------------------------------------------------
// cImageTga::LoadRgb15
//----------------------------------------------------------------------------------
bool cImageTga::LoadRgb15(const cData *Src, cImage *Tex, dword *Buffer, bool VertInvert) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();

	for(int y = 0; y < Height; y++) {
		ZeroMemory(Buffer, sizeof(dword) * Width);
		for(int x = 0; x < Width; x++) {
			const int a = Src->GetByte();
			const int b = Src->GetByte();
			if(a == -1 || b == -1) {
				return false;
			}
			Buffer[x] = cMath::Rgba(a & 0x1f, ((a & 0xe0) >> 5) | ((b & 0x03) << 3), (b & 0x7c) >> 2);
		}
		Tex->PutLine(VertInvert ? Height - 1 - y : y, Buffer);
	}
	return true;
} // cImageTga::LoadRgb15

//----------------------------------------------------------------------------------
// cImageTga::LoadRgb24
//----------------------------------------------------------------------------------
bool cImageTga::LoadRgb24(const cData *Src, cImage *Tex, dword *Buffer, bool VertInvert) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();

	for(int y = 0; y < Height; y++) {
		ZeroMemory(Buffer, sizeof(dword) * Width);
		for(int x = 0; x < Width; x++) {
			int Blue = Src->GetByte();
			int Green = Src->GetByte();
			int Red = Src->GetByte();
			if(Red == -1 || Green == -1 || Blue == -1) {
				return false;
			}
			Buffer[x] = cMath::Rgba(Red, Green, Blue);
		}
		Tex->PutLine(VertInvert ? Height - 1 - y : y, Buffer);
	}
	return true;
} // cImageTga::LoadRgb24

//----------------------------------------------------------------------------------
// cImageTga::LoadRgb32
//----------------------------------------------------------------------------------
bool cImageTga::LoadRgb32(const cData *Src, cImage *Tex, dword *Buffer, bool VertInvert) {
	const int Width = Tex->GetWidth();
	const int Height = Tex->GetHeight();

	for(int y = 0; y < Height; y++) {
		ZeroMemory(Buffer, sizeof(dword) * Width);
		for(int x = 0; x < Width; x++) {
			int Blue = Src->GetByte();
			int Green = Src->GetByte();
			int Red = Src->GetByte();
			int Alpha = Src->GetByte();
			if(Red == -1 || Green == -1 || Blue == -1 || Alpha == -1) {
				return false;
			}
			Buffer[x] = cMath::Rgba(Red, Green, Blue, Alpha);
		}
		Tex->PutLine(VertInvert ? Height - 1 - y : y, Buffer);
	}
	return true;
} // cImageTga::LoadRgb32

bool cImageTga::LoadRle8(const cData *Src, cImage *Tex, dword *Buffer, const dword *Palette, bool VertInvert) {
	cLog::Error("cImageTga::LoadRle8");
	return false;
}

bool cImageTga::LoadRle15(const cData *Src, cImage *Tex, dword *Buffer, bool VertInvert) {
	cLog::Error("cImageTga::LoadRle15");
	return false;
}

bool cImageTga::LoadRle24(const cData *Src, cImage *Tex, dword *Buffer, bool VertInvert) {
	cLog::Error("cImageTga::LoadRle24");
	return false;
}

bool cImageTga::LoadRle32(const cData *Src, cImage *Tex, dword *Buffer, bool VertInvert) {
	cLog::Error("cImageTga::LoadRle32");
	return false;
}

*/
/*

bool    cImageTga :: loadRLE8 ( Data * data, cImage * tex, dword * buf, dword palette [], bool vertInvert )
{
    int width  = tex -> getWidth  ();
    int height = tex -> getHeight ();
    int val;
    int packetHeader, packetSize;

    memset ( buf, '\0', width * sizeof ( dword ) );

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; )
        {
            packetHeader = data -> getByte ();
            packetSize   = (packetHeader & 0x7F) + 1;

            if ( packetHeader == -1 )
                return false;

            if ( packetHeader & 0x80 )              // run-length packet
            {
                val = data -> getByte ();

                if ( val == -1 )
                    return false;

                for ( int c = 0; c < packetSize; c++ )
                {
                    buf [x++] = palette [val];

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }
            else                                    // non run-length packet
            {
                for ( int i = 0; i < packetSize; i++ )
                {
                    val = data -> getByte ();

                    if ( val == -1 )
                        return false;

                    buf [x++] = palette [val];

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }

        }
    }

    return true;
}

bool    cImageTga :: loadRLE15 ( Data * data, cImage * tex, dword * buf, bool vertInvert )
{
    int     width  = tex -> getWidth  ();
    int     height = tex -> getHeight ();
    int     a, b;
    int     packetHeader, packetSize;
    dword   color;

    memset ( buf, '\0', width * sizeof ( dword ) );

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; )
        {
            packetHeader = data -> getByte ();
            packetSize   = (packetHeader & 0x7F) + 1;

            if ( packetHeader == -1 )
                return false;

            if ( packetHeader & 0x80 )              // run-length packet
            {
                a = data -> getByte ();
                b = data -> getByte ();

                if ( a == -1 || b == -1 )
                    return false;

                color = makeRgba ( a & 0x1F, ((b & 0x03) << 3) + ((a & 0xE0) >> 5), (b & 0x7C) >> 2 );

                for ( int c = 0; c < packetSize; c++ )
                {
                    buf [x++] = color;

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }
            else                                    // non run-length packet
            {
                for ( int i = 0; i < packetSize; i++ )
                {
                    a = data -> getByte ();
                    b = data -> getByte ();

                    if ( a == -1 || b == -1 )
                        return false;

                    buf [x++] = makeRgba ( a & 0x1F, ((b & 0x03) << 3) + ((a & 0xE0) >> 5), (b & 0x7C) >> 2 );

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }

        }
    }

    return true;
}

bool    cImageTga :: loadRLE24 ( Data * data, cImage * tex, dword * buf, bool vertInvert )
{
    int     width  = tex -> getWidth  ();
    int     height = tex -> getHeight ();
    int     packetHeader, packetSize;
    int     red, green, blue;
    dword   color;

    memset ( buf, '\0', width * sizeof ( dword ) );

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; )
        {
            packetHeader = data -> getByte ();
            packetSize   = (packetHeader & 0x7F) + 1;

            if ( packetHeader == -1 )
                return false;

            if ( packetHeader & 0x80 )              // run-length packet
            {
                blue  = data -> getByte ();
                green = data -> getByte ();
                red   = data -> getByte ();

                if ( red == -1 || green == -1 || blue == -1 )
                    return false;

                color = makeRgba ( red, green, blue );

                for ( int c = 0; c < packetSize; c++ )
                {
                    buf [x++] = color;

                    if ( x >= width )               // wrapped on next scan line
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }
            else                                    // non run-length packet
            {
                for ( int i = 0; i < packetSize; i++ )
                {
                    blue  = data -> getByte ();
                    green = data -> getByte ();
                    red   = data -> getByte ();

                    if ( red == -1 || green == -1 || blue == -1 )
                        return false;

                    buf [x++] = makeRgba ( red, green, blue );

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }

        }
    }

    return true;
}

bool    cImageTga :: loadRLE32 ( Data * data, cImage * tex, dword * buf, bool vertInvert )
{
    int     width  = tex -> getWidth  ();
    int     height = tex -> getHeight ();
    int     packetHeader, packetSize;
    int     red, green, blue, alpha;
    dword   color;

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; )
        {
            packetHeader = data -> getByte ();
            packetSize   = (packetHeader & 0x7F) + 1;

            if ( packetHeader == -1 )
                return false;

            if ( packetHeader & 0x80 )              // run-length packet
            {
                blue  = data -> getByte ();
                green = data -> getByte ();
                red   = data -> getByte ();
                alpha = data -> getByte ();

                if ( red == -1 || green == -1 || blue == -1 || alpha == -1 )
                    return false;

                color = makeRgba ( red, green, blue, alpha );

                for ( int c = 0; c < packetSize; c++ )
                {
                    buf [x++] = color;

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }
            else                                    // non run-length packet
            {
                for ( int i = 0; i < packetSize; i++ )
                {
                    blue  = data -> getByte ();
                    green = data -> getByte ();
                    red   = data -> getByte ();
                    alpha = data -> getByte ();

                    if ( red == -1 || green == -1 || blue == -1 || alpha == -1 )
                        return false;

                    buf [x++] = makeRgba ( red, green, blue, alpha );

                    if ( x >= width )
                    {
                        tex -> putLine ( vertInvert ? height - 1 - y : y, buf );

                        memset ( buf, '\0', width * sizeof ( dword ) );

                        x = 0;

                        if ( ++y >= height )
                            return true;
                    }
                }
            }

        }
    }

    return true;
}
*/