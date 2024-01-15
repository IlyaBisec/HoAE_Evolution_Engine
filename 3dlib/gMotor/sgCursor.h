/*****************************************************************************/
/*    File:    sgCursor.h
/*    Desc:    Mouse cursor node
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-07-2003
/*****************************************************************************/
#ifndef __SGCURSOR_H__
#define  __SGCURSOR_H__
#include "IInput.h"


/*****************************************************************************/
/*    Class:    Cursor
/*    Desc:    Base class for the mouse cursor representations
/*****************************************************************************/
class Cursor : public HudNode, public IInputDispatcher
{
public:
                        Cursor();
    virtual                ~Cursor();

    virtual void        Update                ( int mX, int mY ) {}
    int                    GetCursorID            ( const char* name ) const;
    bool                SetCurrentCursor    ( const char* name );
    void                SetHotSpot            ( int x, int y ) { m_HotSpotX = x; m_HotSpotY = y; }
    
    virtual bool         OnMouseMove            ( int mX, int mY, DWORD keys ); 
    virtual void        Expose                ( PropertyMap& pm );
    virtual void        Serialize            ( OutStream& os ) const;
    virtual void        Unserialize            ( InStream& os );
    
    DECLARE_SCLASS(Cursor,HudNode,CURS);

protected:
    int                    m_PosX;
    int                    m_PosY;

    int                    m_HotSpotX;        //  horz shift of the main cursor point
    int                    m_HotSpotY;        //  vert shift of the main cursor point
}; // class Cursor

const HCURSOR c_BadCursorHandle = NULL;  
/*****************************************************************************/
/*    Class:    SystemCursor
/*    Desc:    Windoze cursor 
/*****************************************************************************/
class SystemCursor : public Cursor
{
public:
                        SystemCursor();
    virtual             ~SystemCursor();

    virtual void        Activate    ();
    virtual void        Update        ( int mX, int mY );

    void                Expose        ( PropertyMap& pm );
    virtual void        Serialize    ( OutStream& os ) const;
    virtual void        Unserialize    ( InStream& os );

    _inl const char*    GetFileName    () const { return m_FileName.c_str(); }
    void                SetFileName ( const char* val );

    DECLARE_SCLASS(SystemCursor,Cursor,SCUR);

protected:

    HCURSOR                m_hCursor;
    std::string            m_FileName;
}; // class SystemCursor

/*****************************************************************************/
/*    Class:    TexturedCursor
/*    Desc:    Cursor which uses image from some texture
/*****************************************************************************/
class TexturedCursor : public Cursor
{
public:
                        TexturedCursor();

    virtual void        Serialize    ( OutStream& os ) const;
    virtual void        Unserialize    ( InStream& os );
    void                Expose        ( PropertyMap& pm );

    _inl const char*    GetTextureName    () const                { return m_TexName.c_str(); }
    _inl void            SetTextureName    ( const char* name )    { m_TexName = name; }
    _inl void            SetTexRect        ( const Rct& rct )        { m_TexRct = rct; }


    DECLARE_SCLASS(TexturedCursor, Cursor, TCUR);

protected:
    std::string            m_TexName;    //  name of the cursor texture
    Rct                    m_TexRct;    //  position/dimensions on the texture
    int                    m_TexID;    //  texture identifier
}; // class TexturedCursor

/*****************************************************************************/
/*    Class:    FramerateCursor
/*    Desc:    Standard dumb cursor, which is drawn into the back buffer and
/*                reacts with the framerate frequency
/*****************************************************************************/
class FramerateCursor : public TexturedCursor
{
public:

    virtual void        Update        ( int mX, int mY );
    virtual void        Activate    ();
    virtual void        Render        ();

    DECLARE_SCLASS(FramerateCursor,TexturedCursor,FCUR);

protected:
    static BaseMesh            s_DrawMesh;

    Rct                        m_UV;

}; // class FramerateCursor

/*****************************************************************************/
/*    Class:    HardwareCursor
/*    Desc:    Uses buggy 'hardware cursor' 
/*****************************************************************************/
class HardwareCursor : public TexturedCursor
{
public:
    virtual void        Update        ( int mX, int mY );
    virtual void        Activate    ();
    
    DECLARE_SCLASS(HardwareCursor,TexturedCursor,HCUR);

protected:
    
}; // class HardwareCursor



#endif // __SGCURSOR_H__