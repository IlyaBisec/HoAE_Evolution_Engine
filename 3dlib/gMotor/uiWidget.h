/*****************************************************************************/
/*    File:    uiWidget.h
/*    Desc:    Brand-new ui
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#ifndef __UIWIDGET_H__
#define __UIWIDGET_H__
#include "IInput.h"
#include "IReflected.h"
#include "IResourceManager.h"
#include <map>

/*****************************************************************************/    
/*    Enum:    WidgetHAlignment
/*****************************************************************************/    
enum WidgetHAlignment
{
    haNone              = 0,
    haLeft              = 1,
    haCenter            = 2,
    haRight             = 3,
    haParent            = 4,
    haViewPort          = 5
}; // enum WidgetAlignment

/*****************************************************************************/    
/*    Enum:    WidgetVAlignment
/*****************************************************************************/    
enum WidgetVAlignment
{
    vaNone              = 0,
    vaTop               = 1,
    vaCenter            = 2,
    vaBottom            = 3,
    vaParent            = 4,
    vaViewPort          = 5
}; // enum WidgetVAlignment

/*****************************************************************************/    
/*    Enum:    WidgetFlags
/*****************************************************************************/    
enum WidgetFlags
{
    wfRelativeUnits     = 0x00000001,          //  control has relative extents
    wfVisible            = 0x00000002,         //  control is visible
    wfEnabled            = 0x00000004,         //  control is enabled
    wfTrackOutside      = 0x00000008,         //  whether to track events outside control extents
    wfDraggable         = 0x00000010,         //  widget responds to the dragging
    wfDragged           = 0x00000020,         //  widget is dragged right now
    wfHasFocus          = 0x00000040,         //  widget has focus
    wfTopOnFocus        = 0x00000080,         //  when focused widget is drawn last
    wfFocusable         = 0x00000100,         //  widget can gain focus 
    wfClippedToExt      = 0x00000200,         //  drawn contents are clipped to the widget extents
    wfKeptAligned       = 0x00000400,         //  whether control permanently keeps its aligning to parent
    wfLockWidth         = 0x00000800,           
    wfLockHeight        = 0x00001000,
}; // enum WidgetFlags

ENUM( WidgetHAlignment, "WidgetHAlignment", 
     en_val( haNone,        "None"        ) <<
     en_val( haLeft,        "Left"        ) << 
     en_val( haCenter,        "Center"    ) <<
     en_val( haRight,        "Right"        ) <<
     en_val( haParent,        "Parent"    ) << 
     en_val( haViewPort,    "ViewPort"    ) );

ENUM( WidgetVAlignment, "WidgetVAlignment", 
     en_val( haNone,        "None"        ) <<
     en_val( vaTop,            "Top"        ) << 
     en_val( haCenter,        "Center"    ) <<
     en_val( vaBottom,        "Bottom"    ) <<
     en_val( haParent,        "Parent"    ) << 
     en_val( haViewPort,    "ViewPort"    ) );

/*****************************************************************************/
/*    Class:    Widget
/*    Desc:    General control class
/*****************************************************************************/    
class Widget :  public   IReflected,
                public   IInputDispatcher,  
                public   IResource
{
public:
    Signal sigMouseWheel;    
    Signal                  sigMouseMove;        
    Signal                  sigMouseLBDown;    
    Signal                  sigMouseMBDown;    
    Signal                  sigMouseRBDown;    
    Signal                  sigMouseLBUp;        
    Signal                  sigMouseMBUp;        
    Signal                  sigMouseRBUp;        
    Signal                  sigMouseLBDbl;    
    Signal                  sigMouseRBDbl;    
    Signal                  sigMouseMBDbl;    
    Signal                  sigKeyDown;        
    Signal                  sigChar;        
    Signal                  sigKeyUp;    
    Signal                  sigTextChanged;
    Signal                  sigFocus;
    Signal                  sigUnfocus;

protected:
    
    std::string             m_Text;             //  control text (caption, edited text, label text etc.)
    std::string             m_Name;             //  control identifier
    Rct                        m_Extents;            //  extents of control, local to parent
    Vector3D                m_Pivot;            //  pivot point, local to parent
    DWORD                    m_Flags;            //  bunch of flags
    DWORD                   m_TextColor;        //  color with which text is drawn
    DWORD                   m_DisTextColor;     //  text color when disabled
    DWORD                   m_BgColor;          //  color of the background
    WidgetHAlignment        m_HAligh;           //  horizontal alignment to parent
    WidgetVAlignment        m_VAligh;           //  vertical alignment to parent
    
    Widget*                    m_pParent;            //  parent widget
    std::vector<Widget*>    m_Children;            //  list of child controls
    int                     m_ID;               //  object id

    static Widget*          s_pFocused;         //  leaf of the current focus chain
    static float            s_ScalingFactor;    //  screen resolution scaling ratio  

    static int              s_MouseX;           //  current mouse X, in viewport coordinate system        
    static int              s_PrevMouseX;       //  previous mouse X, in viewport coordinate system
    static int              s_MouseY;           //  current mouse Y, in viewport coordinate system        
    static int              s_PrevMouseY;       //  previous mouse Y, in viewport coordinate system
    static MouseButton      s_DragButton;       //  true when currently dragging

public:
                            Widget          ();
    virtual                 ~Widget         ();
    virtual void            OnInit          () {}

    void                    Init            ();

    const char*             GetName         () const { return m_Name.c_str(); }
    void                    SetName         ( const char* name ) { m_Name = name; }

    int                     GetID           () const { return m_ID; }

    Rct                     GetScreenExtents() const;
    Rct                     GetExtents      () const { return m_Extents; }
    void                    SetExtents      ( Rct ext ) { SetExtents( ext.x, ext.y, ext.w, ext.h ); }
    void                    SetExtents      ( float x, float y, float w, float h );
    void                    SetWidth        ( float w ) { m_Extents.w = w; }
    void                    SetHeight       ( float h ) { m_Extents.h = h; }

    const char*             GetText         () const { return m_Text.c_str(); }
    void                    SetText         ( const char* text ) { m_Text = text; sigTextChanged(); }
    const char*             GetScriptName   () const { return IRM->GetName( (IResource*)this ); }
    DWORD                   GetTextColor    () const { return m_TextColor; }
    void                    SetTextColor    ( DWORD clr ) { m_TextColor = clr; }

    DWORD                   GetBgColor      () const { return m_BgColor; }
    void                    SetBgColor      ( DWORD clr ) { m_BgColor = clr; }

    void                    ToggleVisible   () { SetVisible( !IsVisible() ); } 
    void                    ToggleEnabled   () { SetEnabled( !IsEnabled() ); } 

    void                    SetVisible      ( bool bVal = true ) { SetFlag( m_Flags, wfVisible, bVal ); }
    void                    SetEnabled      ( bool bVal = true ) { SetFlag( m_Flags, wfEnabled, bVal ); }
    void                    SetRelativeUnits( bool bVal = true ) { SetFlag( m_Flags, wfRelativeUnits, bVal ); }
    void                    SetTrackOutside ( bool bVal = true ) { SetFlag( m_Flags, wfTrackOutside, bVal ); }
    void                    SetDraggable    ( bool bVal = true ) { SetFlag( m_Flags, wfDraggable, bVal ); }
    void                    SetDragged      ( bool bVal = true ) { SetFlag( m_Flags, wfDragged, bVal ); }
    void                    SetClippedToExt ( bool bVal = true ) { SetFlag( m_Flags, wfClippedToExt, bVal ); }
    void                    SetKeptAligned  ( bool bVal = true ) { SetFlag( m_Flags, wfKeptAligned, bVal ); }
    void                    SetFocusable    ( bool bVal = true ) { SetFlag( m_Flags, wfFocusable, bVal ); }
    void                    SetLockWidth    ( bool bVal = true ) { SetFlag( m_Flags, wfLockWidth, bVal ); }
    void                    SetLockHeight   ( bool bVal = true ) { SetFlag( m_Flags, wfLockHeight, bVal ); }
    void                    SetFocus        ( bool bVal = true ); 

    bool                    IsVisible       () const { return ((m_Flags & wfVisible) != 0); }
    bool                    IsEnabled       () const { return ((m_Flags & wfEnabled) != 0); }
    bool                    IsRelativeUnits () const { return ((m_Flags & wfRelativeUnits) != 0); }
    bool                    IsTrackOutside  () const { return ((m_Flags & wfTrackOutside) != 0); }
    bool                    IsDraggable     () const { return ((m_Flags & wfDraggable) != 0); }
    bool                    IsDragged       () const { return ((m_Flags & wfDragged) != 0); }
    bool                    IsClippedToExt  () const { return ((m_Flags & wfClippedToExt) != 0); }
    bool                    IsKeptAligned   () const { return ((m_Flags & wfKeptAligned) != 0); }
    bool                    IsFocusable     () const { return ((m_Flags & wfFocusable) != 0); }
    bool                    IsLockWidth     () const { return ((m_Flags & wfLockWidth) != 0); }
    bool                    IsLockHeight    () const { return ((m_Flags & wfLockHeight) != 0); }
    bool                    HasFocus        () const { return ((m_Flags & wfHasFocus) != 0); }
    
    bool                    TopOnFocus      () const { return ((m_Flags & wfTopOnFocus) != 0); }
    void                    SetTopOnFocus   ( bool bVal = true ) { SetFlag( m_Flags, wfTopOnFocus, bVal ); }

    virtual bool            PtIn            ( float x, float y ) const; 
    void                    AddChild        ( Widget* pChild );
    void                    AddChild        ( Widget& child );
    void                    ClearChildren   ();

    IReflected*             Parent          () const            { return m_pParent; }
    int                     NumChildren     () const            { return m_Children.size(); }
    IReflected*             Child           ( int idx ) const   
    { 
        if (idx < 0 || idx >= m_Children.size()) return NULL;
        return m_Children[idx]; 
    }
    virtual bool            AddChild        ( IReflected* pChild );
    virtual bool            DelChild        ( int idx );

    Widget*                 FindChildByName ( const char* name ) const;
    Widget*                 GetChild        ( int idx ) const   { return m_Children[idx]; }

    virtual const char*     ClassName       () const            { return "Widget"; }

    void                    Draw            ();
    void                    SaveScript      ();
    void                    LoadScript      ();

    bool                    CreateFromScript( const char* fname );
    virtual bool            Reload          ();

    virtual void            Render            ();
    virtual void            Connect         (){}
    virtual void            Expose            ( PropertyMap& pm );
    virtual void            Serialize        ( OutStream& os ) const;
    virtual void            Unserialize        ( InStream& is );

    virtual bool            OnDrag          ( float dx, float dy );
    const char*             GenerateName    ( const char* cName );

    void                    SetHAligh       ( WidgetHAlignment a ) { m_HAligh = a; }
    void                    SetVAligh       ( WidgetVAlignment a ) { m_VAligh = a; }
    void                    AlignToParent   ( bool bAlign = true ); 
    void                    AlignToViewport ( bool bAlign = true ); 
    
private:
    virtual bool             MouseWheel        ( int delta );                    
    virtual bool             MouseMove        ( int mX, int mY, DWORD keys );    
    virtual bool             MouseLBDown        ( int mX, int mY );                
    virtual bool             MouseMBDown        ( int mX, int mY );                
    virtual bool             MouseRBDown        ( int mX, int mY );                
    virtual bool             MouseLBUp        ( int mX, int mY );                
    virtual bool             MouseMBUp        ( int mX, int mY );                
    virtual bool             MouseRBUp        ( int mX, int mY );                
    virtual bool             MouseLBDbl      ( int mX, int mY );                
    virtual bool             MouseRBDbl      ( int mX, int mY );                
    virtual bool             MouseMBDbl      ( int mX, int mY );        

    virtual bool            KeyDown            ( DWORD keyCode, DWORD flags );    
    virtual bool            Char            ( DWORD charCode, DWORD flags );    
    virtual bool            KeyUp            ( DWORD keyCode, DWORD flags );    
    
    friend class            MainFrame;

protected:
    virtual bool             OnMouseWheel    ( int delta )                    { return false; }
    virtual bool             OnMouseMove        ( int mX, int mY, DWORD keys )    { return false; }
    virtual bool             OnMouseLBDown    ( int mX, int mY )                { return false; }
    virtual bool             OnMouseMBDown    ( int mX, int mY )                { return false; }
    virtual bool             OnMouseRBDown    ( int mX, int mY )                { return false; }
    virtual bool             OnMouseLBUp        ( int mX, int mY )                { return false; }
    virtual bool             OnMouseMBUp        ( int mX, int mY )                { return false; }
    virtual bool             OnMouseRBUp        ( int mX, int mY )                { return false; }
    virtual bool             OnMouseLBDbl    ( int mX, int mY )                { return false; }
    virtual bool             OnMouseRBDbl    ( int mX, int mY )                { return false; }
    virtual bool             OnMouseMBDbl    ( int mX, int mY )                { return false; }
    virtual bool            OnKeyDown        ( DWORD keyCode, DWORD flags )    { return false; }
    virtual bool            OnChar            ( DWORD charCode, DWORD flags )    { return false; }
    virtual bool            OnKeyUp            ( DWORD keyCode, DWORD flags )    { return false; }
    virtual bool            OnFocus         ()                              { return false; } 
    virtual bool            OnUnfocus       ()                              { return false; } 
    virtual bool            OnExtents        ()                             { return false; } 
   
public:
    virtual bool            OnStartDrag     ( int mX, int mY )              { return false; } 
    virtual IReflected*     OnEndDrag       ()                              { return NULL; } 
    virtual void            OnDrop          ( int mX, int mY, IReflected* pDropped )        {} 

    void                    ApplyAlignment  ();
}; // class Widget

#endif // __UIWIDGET_H__