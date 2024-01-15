#pragma once

//*****************************************************************************
// cInput
//*****************************************************************************

class cInput {
protected:
	struct KeyboardState {
		bool IsDown[256];
		KeyboardState() {
			ZeroMemory(IsDown, sizeof(IsDown));
		}
	};
	struct MouseState {
		int PosX, PosY, Wheel;
		bool Buttons[3]; // Left, Right, Middle
		MouseState() {
			PosX = PosY = Wheel = 0;
			Buttons[0] = Buttons[1] = Buttons[2] = false;
		}
	};
	
	virtual bool AcquireKeyboard(KeyboardState &) = 0;
	virtual bool AcquireMouse(MouseState &) = 0;

public:
	static void SetInstance(cInput *InputInstance) { Instance = InputInstance; }
	static cInput * GetInstance() { return Instance; }
	
	cInput();
	virtual bool Init(const void *hWnd) = 0;
	virtual void Free() = 0;
	
	static bool Acquire();
	
	struct Event {
		enum {
			TYPE_NONE,
			TYPE_BUTTON,
			TYPE_CHAR,
			TYPE_MOUSEMOVE
		};
		
		int Type;
		// TYPE_BUTTON:
		int Code;
		bool IsPressed; // Always true for WheelUp and WheelDown.
		bool IsMouseButton; // true for LeftButton, RightButton, MiddleButton, WheelUp and WheelDown.
		int DeltaWheel; // Filled for WheelUp and WheelDown.
		// TYPE_CHAR:
		int Char;
		// TYPE_MOUSEMOVE:
		int DeltaX, DeltaY;
		
		Event() {
			Type = TYPE_NONE;
			Code = -1;
			IsPressed = false;
			IsMouseButton = false;
			DeltaWheel = 0;
			Char = -1;
			DeltaX = DeltaY = 0;
		}

	};
	static cInput::Event * GetEvent();
	static void AddMouseWheelEvent(const int Delta);
	
	enum Codes {
		Esc, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Zero,	// "0"
		One,	// "1"
		Two,	// "2"
		Three,	// "3"
		Four,	// "4"
		Five,	// "5"
		Six,	// "6"
		Seven,	// "7"
		Eight,	// "8"
		Nine,	// "9"
		Minus,	// "-"
		Equals,	// "="
		BackSpace, Tab, CapsLock, PrintScreen, ScrollLock, Pause,
		Insert, Delete, Home, End, PageUp, PageDown,
		Up, Down, Left, Right,
		BackSlash, // "\\"
		Enter,
		LeftBracket,	// "["
		RightBracket,	// "]"
		SemiColon,		// ";"
		SingleQuote,	// "\'"
		Comma,			// ","
		Period,			// "."
		Slash,			// "/"
		LeftShift, LeftControl, LeftAlt, RightShift, RightControl, RightAlt,
		Shift, Control, Alt, Space,
		Tilda,	// "~"
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		LeftButton, RightButton, MiddleButton,
		WheelUp, WheelDown
	};
	
	static int ToCode(const char *Str);
	static const cStr ToStr(const int Code);

	static bool IsDown(const int Code);
	static bool IsDown(const char *Str);

	static int GetMouseX() { return m_MouseState.PosX; }
	static int GetMouseY() { return m_MouseState.PosY; }

private:
	static cInput *Instance;
	
	static KeyboardState m_KeyboardState;
	static MouseState m_MouseState;
	static cList<Event *> m_Events;
	static cList<cStr> m_Strs;
};