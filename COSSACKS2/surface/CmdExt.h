#ifndef __CmdExt__
#define __CmdExt__

#include "..\GameExtension.h"
#include "..\surface\Surface.h"

namespace Surface {

class CmdExt : public GameExtension {
public:
	CmdExt() { assert(0); }
	typedef bool (*CallBack)(cStr &StageLabel);
	CmdExt(CallBack fn);

	bool CheckActivityOfEditor();
	void ClearActivityOfEditor();
	void ActivateEditor();
	void DrawEditorInterface();

	typedef enum { A_CENTER, A_CORNER, A_WORLD, A_SCREEN } eAlign;
	struct MessageBoxArgs {
		cVec3 World;
		cVec2 Screen;
		eAlign Align;
		DWORD FaceColor;
		int SliderPos, SliderLen;
		void Defaults() {
			Align = A_CENTER;
			FaceColor = cColor::Blue.ToBgra();
			SliderPos = SliderLen = -1;
			World.SetZero();
			Screen.SetZero();
		}
		MessageBoxArgs() { Defaults(); }
	};
	static void MessageBox(cStr &Msg, const MessageBoxArgs &Args = MessageBoxArgs());
	static void DrawMessageBox(cStr &Msg, const MessageBoxArgs &Args = MessageBoxArgs());
	
	struct ISliderArgs {
		float CurPos;
		int MinValue, MaxValue, DefValue;
		cColor Color0, Color1;
		RLCFont *pFont;
		int x0, y0, Lx, Ly;
		bool fPercentage;
		void Defaults() {
			Color0 = cColor::Blue / 3;
			Color1 = cColor::Blue;
			pFont = &SmallWhiteFont;
			fPercentage = false;
		}
		ISliderArgs() {
			Defaults();
		}
		int GetValue() const;
		void SetValue(int n);
	};
	static void DrawSlider(const ISliderArgs &Args);
private:
	CallBack m_pCallBack;
	typedef enum { S_INIT, S_PROCESS, S_STATS } eStage;
	eStage m_Stage;
	bool m_fActive;

	static const int dBorder = 3;
	static const int dStrSlider = 6;
	static const int dCornerOffset = 20;
	static const int tShowStats = 6; // seconds
	static const int hSlider = 28;
};

} // Surface

#endif // __Refresh__