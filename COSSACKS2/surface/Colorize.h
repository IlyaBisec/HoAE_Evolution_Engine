#ifndef __Colorize__
#define __Colorize__

#include "SelExt.h"

namespace Surface {

class Colorize : public SelExt {
public:
	DWORD OnLMB(SelExtArgs &);
	DWORD OnRMB(SelExtArgs &);
	void OnDraw(SelExtArgs &);
	void OnDrawInterface();
    void OnActivate();
private:
	static void Process(SurfRef &Ref, DWORD Color, const SurfVertex &Centre, int Radius);
	static void ColorizeOneVertex(const SurfVertex &Center, int Radius, DWORD argbColor, SurfVertex &vtx);
	static UINT_PTR CALLBACK CCHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	static void ShowChooseColorDialog(int xCursor, int yCursor, DWORD &argbColor);
	static const int rFadeOffPortion = 3; // rFadeOff = Radius / rFadeOffPortion;
};

// argb2rgb
inline DWORD argb2rgb(DWORD argb) {
	int b = argb & 0xff;
	argb >>= 8;
	int g = argb & 0xff;
	argb >>= 8;
	int r = argb & 0xff;
	return RGB(r, g, b);
}

// rgb2argb
inline void rgb2argb(DWORD rgb, DWORD &argb) {
	argb = (0xff << 24) + (GetRValue(rgb) << 16) + (GetGValue(rgb) << 8) + GetBValue(rgb);
}

// argb2rgb(DWORD, int &, int &, int &)
inline void argb2rgb(DWORD argb, int &r, int &g, int &b) {
	b = argb & 0xFF;
	argb >>= 8;
	g = argb & 0xFF;
	argb >>= 8;
	r = argb & 0xFF;
}

// rgb2argb(int, int, int, DWORD &)
inline void rgb2argb(int r, int g, int b, DWORD &argb) {
	argb = (0xFF << 24) + (r << 16) + (g << 8) + b;
}

} // Surface

#endif // __Colorize__