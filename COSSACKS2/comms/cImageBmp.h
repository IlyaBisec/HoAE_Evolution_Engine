#pragma once

class cImageBmp : public cImageCodec {
public:
	cImageBmp() : cImageCodec() {}
	virtual cImage * Decode(const cData *Src);
private:
//	bool LoadMono(const cData *Src, cImage *Image, dword *Buffer);
//	bool LoadRgb4(const cData *Src, cImage *Image, dword *Buffer, const dword *Palette);
//	bool LoadRgb8(const cData *Src, cImage *Image, dword *Buffer, const dword *Palette);
//	bool LoadRgb24(const cData *Src, cImage *Image, dword *Buffer);
//	bool LoadRgb32(const cData *Src, cImage *Image, dword *Buffer);
//	bool LoadRLE4(const cData *Src, cImage *Image, dword *Buffer, const dword *Palette);
//	bool LoadRLE8(const cData *Src, cImage *Image, dword *Buffer, const dword *Palette);
};