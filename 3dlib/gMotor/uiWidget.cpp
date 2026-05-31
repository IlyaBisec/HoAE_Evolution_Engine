/*****************************************************************************/
/*    File:    uiWidget.cpp
/*    Desc:    Brand-new ui
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiMainFrame.h"
#include "kFilePath.h"
#include "kXMLParser.h"
#include "kHose.h"

//  Here comes horrible thing... yet we have to live with that, as soon as 
//  linker "optimizes" out the whole .obj's from the static library, 
//  with the widget class registration
//  Note, however, that there is no need to include here headers with linked classes 
//  definitions.

void LinkWidgets()
{
LINK_CLASS( Window          );
LINK_CLASS( FrameWindow     );
LINK_CLASS( MainFrame       );
LINK_CLASS( Label           );
LINK_CLASS( Slider          );
LINK_CLASS( EditBox         );
LINK_CLASS( ComboWidget     );
LINK_CLASS( CheckButton     );
LINK_CLASS( ListWidget      );
LINK_CLASS( ScrollBox       );
LINK_CLASS( Button          );
LINK_CLASS( RadioButton     );
LINK_CLASS( RadioGroup      );
LINK_CLASS( Ratchet         );
LINK_CLASS( IntEditBox      );
LINK_CLASS( FloatEditBox    );
LINK_CLASS( Progress        );
LINK_CLASS( MayaController  );
LINK_CLASS( RTSController   );
LINK_CLASS( NodeTree        );
LINK_CLASS( ObjectInspector );
LINK_CLASS( BoolPropEditor  );
LINK_CLASS( MethodButton    );
LINK_CLASS( ExtendedEdit    );
LINK_CLASS( ResourceBrowser );
LINK_CLASS( ColorPicker     );
LINK_CLASS( ColorBox        );
LINK_CLASS( ModelPalette    );
LINK_CLASS( SpritePalette   );
LINK_CLASS( FileBrowser     );
LINK_CLASS( PaletteSample   );
LINK_CLASS( Kangaroo        );

LINK_CLASS( EffectEditor     );
LINK_CLASS( ModelEditor      );
LINK_CLASS( SceneEditor      );
LINK_CLASS( SystemExplorer   );
LINK_CLASS( WidgetEditor     );
LINK_CLASS( AlgorithmDebugger);
}

/*****************************************************************************/
/*    Widget implementation
/*****************************************************************************/
Widget*         Widget::s_pFocused      = NULL;
float           Widget::s_ScalingFactor = 1.0f;
int             Widget::s_MouseX        = 0;    
int             Widget::s_PrevMouseX    = 0;
int             Widget::s_MouseY        = 0;    
int             Widget::s_PrevMouseY    = 0;
MouseButton     Widget::s_DragButton    = mbNone;

Widget::Widget()
{
    m_Pivot         = Vector3D::null;    
    m_Flags         = 0;    
    m_pParent       = NULL;    
    m_HAligh        = haNone;
    m_VAligh        = vaNone;
    m_Extents       = Rct::null;
    m_TextColor     = 0xFF000000;
    m_DisTextColor  = 0xFFCCCCCF;
    m_BgColor       = 0xAAD6D3CE;

    //  some default widget flags
    SetVisible      ();
    SetEnabled      ();
    SetTrackOutside ();
    SetTopOnFocus   ();
    SetFocusable    ();
    SetDraggable    ( false );
    SetClippedToExt ( false );
} // Widget::Widget

Widget::~Widget()
{
    HoseDepot::instance().UnregisterObject( this );
    if (s_pFocused == this) s_pFocused = NULL;
}

void Widget::Init()
{
    OnInit();
    ApplyAlignment();

    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        ch->Init();
    }
} // Widget::Init

Rct Widget::GetScreenExtents() const
{
    Rct ext = m_Extents;
    if (m_pParent)
    {
        Rct pext = m_pParent->GetScreenExtents();
        ext.x += pext.x;
        ext.y += pext.y;
    }
    return ext;
} // Widget::GetScreenExtents

bool Widget::AddChild( IReflected* pChild )
{
    Widget* pWidget = dynamic_cast<Widget*>( pChild );
    AddChild( pWidget );
    return true;
} // Widget::AddChild

void Widget::AddChild( Widget* pChild )
{
    if (!pChild) return;
    pChild->m_pParent = this;
    for (int i = 0; i < m_Children.size(); i++)
    {
        if (m_Children[i] == pChild) return;
    }
    m_Children.push_back( pChild );
} // Widget::AddChild 

void Widget::AddChild ( Widget& child )
{
    for (int i = 0; i < m_Children.size(); i++)
    {
        if (m_Children[i] == &child) return;
    }
    child.m_pParent = this;
    m_Children.push_back( &child );
} // Widget::AddChild 

bool Widget::DelChild( int idx )
{
    return false;
} // Widget::DelChild

const char* Widget::GenerateName( const char* cName )
{
    return "";
} // Widget::GenerateName

void Widget::Expose( PropertyMap& pm )
{
    pm.start( "Widget", this );
    pm.p( "Class", &Widget::ClassName               );
    pm.p( "Name", &Widget::GetName, &Widget::SetName        );
    pm.p( "Text", &Widget::GetText, &Widget::SetText        );
    pm.p( "Script", &Widget::GetScriptName           );
    pm.f( "TextColor",          m_TextColor,    "color" );
    pm.f( "DisabledTextColor",  m_DisTextColor, "color" );
    pm.f( "BgColor",            m_BgColor,      "color" );
    pm.p( "RelativeUnits", &Widget::IsRelativeUnits );
    pm.p( "Visible", &Widget::IsVisible, &Widget::SetVisible      );
    pm.p( "Draggable", &Widget::IsDraggable, &Widget::SetDraggable    );
    pm.p( "Enabled", &Widget::IsEnabled, &Widget::SetEnabled      );
    pm.p( "TrackOutside", &Widget::IsTrackOutside, &Widget::SetTrackOutside );
    pm.p( "ClippedToExt", &Widget::IsClippedToExt, &Widget::SetClippedToExt );
    pm.p( "KeptAligned", &Widget::IsKeptAligned, &Widget::SetKeptAligned  );
    pm.p( "LockWidth", &Widget::IsLockWidth, &Widget::SetLockWidth    );
    pm.p( "LockHeight", &Widget::IsLockHeight, &Widget::SetLockHeight   );
    pm.p( "Ext", &Widget::GetExtents, &Widget::SetExtents      );
    
    pm.f( "Left",               m_Extents.x     );
    pm.f( "Top",                m_Extents.y     );
    pm.f( "Width",              m_Extents.w     );
    pm.f( "Height",             m_Extents.h     );

    pm.f( "HAlign",             m_HAligh );
    pm.f( "VAlign",             m_VAligh );

    pm.m( "ToggleVisible", &Widget::ToggleVisible   );
    pm.m( "ToggleEnabled", &Widget::ToggleEnabled   );

    pm.m( "LoadFromScript", &Widget::LoadScript      );
    pm.m( "SaveToScript", &Widget::SaveScript      );

    pm.s( "mouse_wheel",        sigMouseWheel   );    
    pm.s( "mouse_move",         sigMouseMove    );    
    pm.s( "mouse_lb_down",      sigMouseLBDown  );    
    pm.s( "mouse_mb_down",      sigMouseMBDown  );    
    pm.s( "mouse_rb_down",      sigMouseRBDown  );    
    pm.s( "mouse_lb_up",        sigMouseLBUp    );    
    pm.s( "mouse_mb_up",        sigMouseMBUp    );    
    pm.s( "mouse_rb_up",        sigMouseRBUp    );    
    pm.s( "mouse_lb_dbl",       sigMouseLBDbl   );  
    pm.s( "mouse_rb_dbl",       sigMouseRBDbl   );  
    pm.s( "mouse_mb_dbl",       sigMouseMBDbl   );  
    pm.s( "key_down",           sigKeyDown      );        
    pm.s( "char",               sigChar         );        
    pm.s( "key_up",             sigKeyUp        );        

} // Widget::Expose

void Widget::AlignToParent( bool bAlign ) 
{ 
    if (bAlign) 
    {
        SetHAligh( haParent );
        SetVAligh( vaParent );
    }
    else
    {
        SetHAligh( haNone );
        SetVAligh( vaNone );
    }
} // Widget::AlignToParent

void Widget::AlignToViewport( bool bAlign )
{
    if (bAlign) 
    {
        SetHAligh( haViewPort );
        SetVAligh( vaViewPort );
    }
    else
    {
        SetHAligh( haNone );
        SetVAligh( vaNone );
    }
} // Widget::AlignToViewport

void Widget::ApplyAlignment()
{
    if (m_HAligh == haNone && m_VAligh == vaNone) return;
    Rct pext = m_pParent ? m_pParent->GetExtents() : Rct( 0.0f, 0.0f, 1024.0f, 768.0f );
    switch (m_HAligh)
    {
    case haLeft:
        m_Extents.x = 0;
        break;
    case haRight:
        m_Extents.x = pext.w - m_Extents.w;
        break;
    case haCenter:
        m_Extents.x = (pext.w - m_Extents.w)*0.5f;
        break;
    case haParent:
        m_Extents.x = 0;
        m_Extents.w = pext.w;
        break;
    case haViewPort:
        if (!IRS) break;
        Rct vp = IRS->GetViewPort();
        m_Extents.x = vp.x;
        m_Extents.w = vp.w;
        break;
    }

    switch (m_VAligh)
    {
    case vaTop:
        m_Extents.y = 0;
        break;
    case vaBottom:
        m_Extents.y = pext.h - m_Extents.h;
        break;
    case vaCenter:
        m_Extents.y = (pext.h - m_Extents.h)*0.5f;
        break;
    case vaParent:
        m_Extents.y = 0;
        m_Extents.h = pext.h;
        break;
    case vaViewPort:
        if (!IRS) break;
        Rct vp = IRS->GetViewPort();
        m_Extents.y = vp.y;
        m_Extents.h = vp.h;
        break;
    }
} // Widget::ApplyAlignment

void Widget::Draw()
{
    if (IsKeptAligned()) ApplyAlignment();

    Rct ext     = GetExtents();
    Rct scrExt  = GetScreenExtents();
    
    Rct vp;      
    if (IsClippedToExt())
    {
        vp = IRS->GetViewPort();
        IRS->SetViewPort( scrExt );
    }
    
    HoseDepot::instance().Update( this );
    Render();

    //  propagate to children
    int nCh = m_Children.size();
    Widget* pFocus = NULL;
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (ch->IsKeptAligned()) ch->ApplyAlignment();
        
        //  clip if outside extents
        Rct chExt = ch->GetExtents();
        if (IsClippedToExt() &&
            (   chExt.x           > ext.w || 
                chExt.y           > ext.h || 
                chExt.GetRight()  < 0     || 
                chExt.GetBottom() < 0)) 
        {
            continue; 
        }
        if (ch->HasFocus() && ch->TopOnFocus()) 
        { 
            //assert( !pFocus );
            pFocus = ch; 
            continue; 
        }
        if (!ch->IsVisible()) continue;
        ch->Draw();
    }
    //  focused child widget is drawn last 
    if (pFocus && pFocus->IsVisible()) 
    {
        pFocus->Draw();
    }

    if (IsClippedToExt()) IRS->SetViewPort( vp );
} // Widget::Draw

void Widget::Render()
{
} // Widget::Render

void Widget::Serialize( OutStream& os ) const
{
} // Widget::Serialize

void Widget::Unserialize( InStream& is )
{
} // Widget::Unserialize

Widget* Widget::FindChildByName( const char* name ) const
{
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        if (!strcmp( name, m_Children[i]->GetName() )) return m_Children[i];
    }
    return NULL;
} // Widget::FindChildByName

bool Widget::MouseWheel( int delta )
{ 
    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        if (ch->MouseWheel( delta )) return true;
    }
    if (OnMouseWheel( delta )) return true;;
    //  invoke signal
    sigMouseWheel();
    return false; 
} // Widget::MouseWheel

bool Widget::MouseMove( int mX, int mY, DWORD keys )    
{ 
    if (IsDragged()) 
    {
        //if (!s_bDragging) SetDragged( false );
        float dx = s_MouseX - s_PrevMouseX; 
        float dy = s_MouseY - s_PrevMouseY;
        OnDrag( dx, dy );
    }

    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        int cMx = mX - ch->m_Extents.x;
        int cMy = mY - ch->m_Extents.y;
        if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) continue;
        if (ch->MouseMove( cMx, cMy, keys )) return true;
    }

    if (OnMouseMove( mX, mY, keys )) 
    {
        return true;
    }
    //  invoke signal
    sigMouseMove();
    
    return false; 
} // Widget::MouseMove

bool Widget::MouseLBDown( int mX, int mY )                
{ 
    //  propagate to children
    int nCh = m_Children.size();
    
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        int cMx = mX - ch->m_Extents.x;
        int cMy = mY - ch->m_Extents.y;
        if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) 
        {
            continue;
        }
        if (ch->MouseLBDown( cMx, cMy )) 
        {
            return true;
        }
    }
    
    if (OnMouseLBDown( mX, mY )) return true;
    //  invoke signal
    sigMouseLBDown();

    return false; 
} // Widget::MouseLBDown

bool Widget::MouseMBDown( int mX, int mY )                
{ 
    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        int cMx = mX - ch->m_Extents.x;
        int cMy = mY - ch->m_Extents.y;
        if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) continue;
        if (ch->MouseMBDown( cMx, cMy )) return true;
    }
    if (OnMouseMBDown( mX, mY )) return true;
    //  invoke signal
    sigMouseMBDown();
    return false; 
} // Widget::MouseMBDown

bool Widget::MouseRBDown( int mX, int mY )                
{ 
    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
         int cMx = mX - ch->m_Extents.x;
         int cMy = mY - ch->m_Extents.y;
         if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) continue;
         if (ch->MouseRBDown( cMx, cMy )) return true;
    }
    if (OnMouseRBDown( mX, mY )) return true;
    //  invoke signal
    sigMouseRBDown();

    if (OnStartDrag( mX, mY )) SetDragged( true );
    return false; 
} // Widget::MouseRBDown

bool Widget::MouseLBUp( int mX, int mY )                
{ 
    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
         int cMx = mX - ch->m_Extents.x;
         int cMy = mY - ch->m_Extents.y;
         if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) continue;
         if (ch->MouseLBUp( cMx, cMy )) return true;
    }
    if (OnMouseLBUp( mX, mY )) return true;
    //  invoke signal
    sigMouseLBUp();
    return false; 
} // Widget::MouseLBUp

bool Widget::MouseMBUp( int mX, int mY )                
{ 
    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        int cMx = mX - ch->m_Extents.x;
        int cMy = mY - ch->m_Extents.y;
        if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) continue;
        if (ch->MouseMBUp( cMx, cMy )) return true;
    }
    
    if (OnMouseMBUp( mX, mY )) return true;
    //  invoke signal
    sigMouseMBUp();
    return false; 
} // Widget::MouseMBUp

bool Widget::MouseRBUp( int mX, int mY )                
{ 
    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        int cMx = mX - ch->m_Extents.x;
        int cMy = mY - ch->m_Extents.y;
        if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) continue;
        if (ch->MouseRBUp( cMx, cMy )) return true;
    }
    if (OnMouseRBUp( mX, mY )) return true;
    //  invoke signal
    sigMouseRBUp();

    if (IsDragged())
    {
        //  find drop target
        MainFrame::instance().OnDrop( mX, mY, OnEndDrag() );
    }
    return false; 
} // Widget::MouseRBUp

bool Widget::MouseLBDbl( int mX, int mY )                
{ 
    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        int cMx = mX - ch->m_Extents.x;
        int cMy = mY - ch->m_Extents.y;
        if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) continue;
        if (ch->MouseLBDbl( cMx, cMy )) return true;
    }
    if (OnMouseLBDbl( mX, mY )) return true;
    //  invoke signal
    sigMouseLBDbl();
    return false; 
} // Widget::MouseLBDbl

bool Widget::MouseRBDbl( int mX, int mY )                
{ 
    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        int cMx = mX - ch->m_Extents.x;
        int cMy = mY - ch->m_Extents.y;
        if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) continue;
        if (ch->MouseRBDbl( cMx, cMy )) return true;
    }
    if (OnMouseRBDbl( mX, mY )) return true;
    //  invoke signal
    sigMouseRBDbl();
    return false; 
} // Widget::MouseRBDbl

bool Widget::MouseMBDbl( int mX, int mY )                
{ 
    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
         int cMx = mX - ch->m_Extents.x;
         int cMy = mY - ch->m_Extents.y;
         if (!ch->IsTrackOutside() && !ch->PtIn( mX, mY )) continue;
         if (ch->MouseMBDbl( cMx, cMy )) return true;
    }
    if (OnMouseMBDbl( mX, mY )) return true;
    //  invoke signal
    sigMouseMBDbl();
    return false; 
} // Widget::MouseMBDbl

bool Widget::KeyDown( DWORD keyCode, DWORD flags )    
{ 
    if (OnKeyDown( keyCode, flags )) return true;

    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        if (!ch->HasFocus() && ch->KeyDown( keyCode, flags )) return true;
    }
    //  invoke signal
    sigKeyDown();
    return false; 
} // Widget::KeyDown

bool Widget::Char( DWORD charCode, DWORD flags )    
{ 
    if (OnChar( charCode, flags )) return true;

    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        if (!ch->HasFocus() && ch->Char( charCode, flags )) return true;
    }
    //  invoke signal
    sigChar();
    return false; 
} // Widget::Char

bool Widget::KeyUp( DWORD keyCode, DWORD flags )    
{ 
    if (OnKeyUp( keyCode, flags )) return true;

    //  propagate to children
    int nCh = m_Children.size();
    for (int i = 0; i < nCh; i++)
    {
        Widget* ch = m_Children[i];
        if (!ch->IsVisible()) continue;
        if (!ch->IsEnabled()) continue;
        if (!ch->HasFocus() && ch->KeyUp( keyCode, flags )) return true;
    }
    //  invoke signal
    sigKeyUp();
    return false; 
} // Widget::KeyUp

bool Widget::Reload()
{
    const char* resName = IRM->GetName( this );
    return (ObjectFactory::instance().CreateFromScript( resName, this ) != NULL);
} // Widget::Reload

void Widget::SaveScript()
{
    const char* resName = IRM->GetName( this );
    (ObjectFactory::instance().SaveToScript( resName, this ) != NULL);
}

void Widget::LoadScript()
{
    const char* resName = IRM->GetName( this );
    (ObjectFactory::instance().CreateFromScript( resName, this ) != NULL);
}

void Widget::ClearChildren()
{
    m_Children.clear();
} // Widget::ClearChildren

bool Widget::OnDrag( float dx, float dy )
{
    m_Extents.x += dx;
    m_Extents.y += dy;
    return false;
} // Widget::OnDrag

bool Widget::CreateFromScript( const char* fname ) 
{ 
    FilePath path( fname );
    path.SetExt( "ui" );
    bool res = (ObjectFactory::instance().CreateFromScript( path, this ) != NULL);
    if (res) Init();
    return res;
} // Widget::CreateFromScript

void Widget::SetExtents( float x, float y, float w, float h )
{ 
    if (!IsLockWidth())  m_Extents.w = w;
    if (!IsLockHeight()) m_Extents.h = h;
    m_Extents.x = x;
    m_Extents.y = y;
    OnExtents(); 
} // Widget::SetExtents

bool Widget::PtIn( float x, float y ) const 
{ 
    return (x >= 0.0f && y >= 0.0f && x <= m_Extents.w && y <= m_Extents.h);
}

void Widget::SetFocus( bool bVal ) 
{ 
    if (HasFocus() == bVal) return;
    if (s_pFocused) 
    {
        SetFlag( s_pFocused->m_Flags, wfHasFocus, false );
        s_pFocused->OnUnfocus();
        s_pFocused->sigUnfocus();
    }
    if (bVal) 
    {
        OnFocus(); s_pFocused = this;
        sigFocus();
    }
    SetFlag( m_Flags, wfHasFocus, bVal );
} // Widget::SetFocus


