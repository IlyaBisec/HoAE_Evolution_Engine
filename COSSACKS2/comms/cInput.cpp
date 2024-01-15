#include "comms.h"

// ToDo:
// ToCode() can't find some characters (for example "q")

cInput *cInput::Instance = NULL;
cInput::KeyboardState cInput::m_KeyboardState;
cInput::MouseState cInput::m_MouseState;
cList<cInput::Event *> cInput::m_Events;
cList<cStr> cInput::m_Strs;

//-----------------------------------------------------------------------------
// cInput::GetEvent
//-----------------------------------------------------------------------------
cInput::Event * cInput::GetEvent() {
	Event *pEvent = NULL;
	if(m_Events.Count()) {
		pEvent = m_Events[0];
		m_Events.RemoveAt(0);
	}
	return pEvent;
} // cInput::GetEvent

//-----------------------------------------------------------------------------
// cInput::AddMouseWheelEvent
//-----------------------------------------------------------------------------
void cInput::AddMouseWheelEvent(const int Delta) {
	cInput::Event *pEvent = new cInput::Event;
	pEvent->Type = cInput::Event::TYPE_BUTTON;
	pEvent->IsPressed = true;
	pEvent->IsMouseButton = true;
	pEvent->DeltaWheel = Delta;
	pEvent->Code = pEvent->DeltaWheel > 0 ? cInput::WheelUp : cInput::WheelDown;
	m_Events.Add(pEvent);
} // cInput::AddMouseWheelEvent

//-----------------------------------------------------------------------------
// cInput::IsDown
//-----------------------------------------------------------------------------
bool cInput::IsDown(const int Code) {
	if(Code >= Esc && Code <= Z) {
		return m_KeyboardState.IsDown[Code + 1];
	}
	if(Code >= LeftButton && Code <= MiddleButton) {
		return m_MouseState.Buttons[Code - LeftButton];
	}
	return false;
} // cInput::IsDown

//-----------------------------------------------------------------------------
// cInput::IsDown
//-----------------------------------------------------------------------------
bool cInput::IsDown(const char *Str) {
	return IsDown(ToCode(Str));
} // cInput::IsDown

//-----------------------------------------------------------------------------
// cInput::Acquire
//-----------------------------------------------------------------------------
bool cInput::Acquire() {
	cAssertM(Instance != NULL, "Acquire() - Instance of \"Input Manager\" is NULL");
	if(!Instance) {
		return false;
	}
	
	KeyboardState CurKeyboardState;
	if(!Instance->AcquireKeyboard(CurKeyboardState)) {
		CurKeyboardState = m_KeyboardState;
	}

	MouseState CurMouseState;
	if(!Instance->AcquireMouse(CurMouseState)) {
		CurMouseState = m_MouseState;
	}

	Event *pEvent = NULL;

	// First go mouse move events.
	// Reason: if some widget starts capturing mouse input on "button down" event,
	// it hasn't to receive previous "mouse move event".
	const int DeltaX = CurMouseState.PosX - m_MouseState.PosX;
	const int DeltaY = CurMouseState.PosY - m_MouseState.PosY;
	if(DeltaX != 0 || DeltaY != 0) {
		pEvent = new Event;
		pEvent->Type = Event::TYPE_MOUSEMOVE;
		pEvent->DeltaX = DeltaX;
		pEvent->DeltaY = DeltaY;
		m_Events.Add(pEvent);
		pEvent = NULL;
	}
	
	// Keyboard keys:
	for(int Code = Esc; Code <= Z; Code++) {
		const bool WasDown = m_KeyboardState.IsDown[Code + 1];
		const bool IsDown = CurKeyboardState.IsDown[Code + 1];
		if(WasDown != IsDown) {
			pEvent = new Event;
			pEvent->Type = Event::TYPE_BUTTON;
			pEvent->Code = Code;
			pEvent->IsPressed = IsDown;
			m_Events.Add(pEvent);
			pEvent = NULL;
		}
	}

	// Mouse buttons:
	for(int nButton = 0; nButton < 3; nButton++) {
		if(m_MouseState.Buttons[nButton] != CurMouseState.Buttons[nButton]) {
			pEvent = new Event;
			pEvent->Type = Event::TYPE_BUTTON;
			pEvent->Code = LeftButton + nButton;
			pEvent->IsPressed = CurMouseState.Buttons[nButton];
			pEvent->IsMouseButton = true;
			m_Events.Add(pEvent);
			pEvent = NULL;
		}
	}

	m_KeyboardState = CurKeyboardState;
	m_MouseState = CurMouseState;

	return true;
} // cInput::Acquire

//-----------------------------------------------------------------------------
// cInput::ToCode
//------------------------------------------------------------------------------
int cInput::ToCode(const char *Str) {
	return m_Strs.IndexOfStr(Str);
} // cInput::ToStr

//-----------------------------------------------------------------------------
// cInput::ToStr
//-----------------------------------------------------------------------------
const cStr cInput::ToStr(const int Code) {
	if(m_Strs.Count() && Code >= Esc && Code <= WheelDown) {
		return m_Strs[Code];
	}
	return cStr("");
} // cInput::ToStr

//-----------------------------------------------------------------------------
// cInput.ctor
//-----------------------------------------------------------------------------
cInput::cInput() {
	ZeroMemory(&m_KeyboardState, sizeof(m_KeyboardState));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));

	if(m_Strs.Count()) {
		return; // Table is already inited by ctor of previous input manager instance.
	}
	m_Strs.Add("Esc");
	m_Strs.Add("F1");
	m_Strs.Add("F2");
	m_Strs.Add("F3");
	m_Strs.Add("F4");
	m_Strs.Add("F5");
	m_Strs.Add("F6");
	m_Strs.Add("F7");
	m_Strs.Add("F8");
	m_Strs.Add("F9");
	m_Strs.Add("F10");
	m_Strs.Add("F11");
	m_Strs.Add("F12");
	m_Strs.Add("0");
	m_Strs.Add("1");
	m_Strs.Add("2");
	m_Strs.Add("3");
	m_Strs.Add("4");
	m_Strs.Add("5");
	m_Strs.Add("6");
	m_Strs.Add("7");
	m_Strs.Add("8");
	m_Strs.Add("9");
	m_Strs.Add("-"); // Minus
	m_Strs.Add("="); // Equals
	m_Strs.Add("BackSpace");
	m_Strs.Add("Tab");
	m_Strs.Add("CapsLock");
	m_Strs.Add("PrintScreen");
	m_Strs.Add("ScrollLock");
	m_Strs.Add("Pause");
	m_Strs.Add("Insert");
	m_Strs.Add("Delete");
	m_Strs.Add("Home");
	m_Strs.Add("End");
	m_Strs.Add("PageUp");
	m_Strs.Add("PageDown");
	m_Strs.Add("Up");
	m_Strs.Add("Down");
	m_Strs.Add("Left");
	m_Strs.Add("Right");
	m_Strs.Add("\\"); // BackSlash
	m_Strs.Add("Enter");
	m_Strs.Add("["); // LeftBracket
	m_Strs.Add("]"); // RightBracket
	m_Strs.Add(";"); // SemiColon
	m_Strs.Add("\'"); // SingleQuote
	m_Strs.Add(","); // Comma
	m_Strs.Add("."); // Period
	m_Strs.Add("/"); // Slash
	m_Strs.Add("LeftShift");
	m_Strs.Add("LeftControl");
	m_Strs.Add("LeftAlt");
	m_Strs.Add("RightShift");
	m_Strs.Add("RightControl");
	m_Strs.Add("RightAlt");
	// After Left and Right types:
	m_Strs.Add("Shift");
	m_Strs.Add("Control");
	m_Strs.Add("Alt");
	m_Strs.Add("Space");
	m_Strs.Add("~"); // Tilda
	m_Strs.Add("A");
	m_Strs.Add("B");
	m_Strs.Add("C");
	m_Strs.Add("D");
	m_Strs.Add("E");
	m_Strs.Add("F");
	m_Strs.Add("G");
	m_Strs.Add("H");
	m_Strs.Add("I");
	m_Strs.Add("J");
	m_Strs.Add("K");
	m_Strs.Add("L");
	m_Strs.Add("M");
	m_Strs.Add("N");
	m_Strs.Add("O");
	m_Strs.Add("P");
	m_Strs.Add("Q");
	m_Strs.Add("R");
	m_Strs.Add("S");
	m_Strs.Add("T");
	m_Strs.Add("U");
	m_Strs.Add("V");
	m_Strs.Add("W");
	m_Strs.Add("X");
	m_Strs.Add("Y");
	m_Strs.Add("Z");
	m_Strs.Add("LeftButton");
	m_Strs.Add("RightButton");
	m_Strs.Add("MiddleButton");
	m_Strs.Add("WheelUp");
	m_Strs.Add("WheelDown");
	
	m_Strs.CreateHashStr(true);
}; // cInput.ctor