/*****************************************************************************/
/*    File:    IInput.h
/*    Desc:    Input access interface
/*    Author:    Ruslan Shestopalyuk
/*    Date:    29.10.2003
/*****************************************************************************/
#ifndef __IINPUT_H__
#define __IINPUT_H__

enum VirtualKey
{
    vkNone          = 0,
    vkControl       = 1,
    vkAlt           = 2,
    vkShift         = 3,
    vkTab           = 4,
    vkEnter         = 5,
    vkBackspace     = 6,
    vkUp            = 7,
    vkDown          = 8,
    vkRight         = 9,
    vkLeft          = 10,
    vkPgUp          = 11,
    vkPgDown        = 12,
    vkHome          = 13,
    vkEnd           = 14,
    vkPause         = 15,
    vkInsert        = 16,
    vkDelete        = 17,
    vkCapsLock      = 18,
    vkNumLock       = 19,
    vkScrollLock    = 20,
    vkF1            = 21,
    vkF2            = 22,
    vkF3            = 23,
    vkF4            = 24,
    vkF5            = 25,
    vkF6            = 26,
    vkF7            = 27,
    vkF8            = 28,
    vkF9            = 29,
    vkF10           = 30,
    vkF11           = 31,
    vkF12           = 32,
}; // enum VirtualKey

enum MouseButtonEvent
{
    mbeUnknown  = 0,
    mbeDown     = 1,
    mbeUp       = 2,
    mbeDbl      = 3,
}; // enum MouseButtonEvent

enum MouseButton
{
    mbNone      = 0x00,
    mbLeft      = 0x01,
    mbMiddle    = 0x02,
    mbRight     = 0x04,
}; // enum MouseButton

/*****************************************************************************/
/*    Class:    IInputDispatcher
/*    Desc:    Interface for object which has user input response
/*****************************************************************************/
class IInputDispatcher
{
public:
    virtual bool     MouseWheel       ( int delta )                     { return false; }
    virtual bool     MouseMove        ( int mX, int mY, DWORD keys )    { return false; }
    virtual bool     MouseLBDown      ( int mX, int mY )                { return false; }
    virtual bool     MouseMBDown      ( int mX, int mY )                { return false; }
    virtual bool     MouseRBDown      ( int mX, int mY )                { return false; }
    virtual bool     MouseLBUp        ( int mX, int mY )                { return false; }
    virtual bool     MouseMBUp        ( int mX, int mY )                { return false; }
    virtual bool     MouseRBUp        ( int mX, int mY )                { return false; }
    virtual bool     MouseLBDbl       ( int mX, int mY )                { return false; }
    virtual bool     MouseRBDbl       ( int mX, int mY )                { return false; }
    virtual bool     MouseMBDbl       ( int mX, int mY )                { return false; }
    virtual bool     KeyDown          ( DWORD keyCode, DWORD flags )    { return false; }
    virtual bool     Char             ( DWORD charCode, DWORD flags )   { return false; }
    virtual bool     KeyUp            ( DWORD keyCode, DWORD flags )    { return false; }

}; // class IInputDispatcher

//  top-level input manager
extern IInputDispatcher*    IInput;

#endif // __IINPUT_H__