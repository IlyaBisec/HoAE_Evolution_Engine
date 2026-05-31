/*****************************************************************************/
/*    File:    uiRampEdit.h
/*    Desc:    Color/alpha ramp editors
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-12-2003
/*****************************************************************************/
#ifndef __UIRAMPEDIT_H__
#define __UIRAMPEDIT_H__

/*****************************************************************************/
/*    Class:    ColorRampEdit 
/*    Desc:    Editor of the color ramp values
/*****************************************************************************/
class ColorRampEdit : public Widget
{
    ColorRamp        m_Ramp;
    int                m_SelectedKey;
    bool            m_bDragKey;

public:
                    ColorRampEdit        ();
    virtual    void    Render                ();

    virtual bool     OnMouseMove            ( int mX, int mY, DWORD keys );
    virtual bool     OnMouseLBDown        ( int mX, int mY );        
    virtual bool    OnMouseLBUp            ( int mX, int mY );
    virtual bool     OnMouseRBDown        ( int mX, int mY );            
    virtual bool    OnKeyDown            ( DWORD keyCode, DWORD flags );
    virtual bool     OnMouseLBDbl        ( int mX, int mY );

    DECLARE_SCLASS(ColorRampEdit,Widget,CORE);

protected:
    bool            AskColor            ( DWORD& col );
    int                GetKey                ( int mX, int mY );
    float            GetTimeInPoint        ( int mX, int mY );
    void            UpdateFromText      ();
    void            UpdateText          ();
}; // class ColorRampEdit

/*****************************************************************************/
/*    Class:    AlphaRampEdit 
/*    Desc:    Editor of the color ramp values
/*****************************************************************************/
class AlphaRampEdit : public Widget
{
    AlphaRamp        m_Ramp;
    int                m_SelectedKey;
    bool            m_bDragKey;

    WeightEdit        m_WeightEdit;

public:
                    AlphaRampEdit        ();
    virtual    void    Render                ();

    virtual bool     OnMouseMove            ( int mX, int mY, DWORD keys );
    virtual bool     OnMouseLBDown        ( int mX, int mY );        
    virtual bool    OnMouseLBUp            ( int mX, int mY );
    virtual bool     OnMouseRBDown        ( int mX, int mY );            
    virtual bool    OnKeyDown            ( DWORD keyCode, DWORD flags );
    virtual bool     OnMouseLBDbl        ( int mX, int mY );


    DECLARE_SCLASS(AlphaRampEdit,Widget,ALRE);

protected:
    void            AskWeight            ( float& w, int mX, int mY );
    int                GetKey                ( int mX, int mY );
    float            GetTimeInPoint        ( int mX, int mY );
    void            UpdateFromText      ();
    void            UpdateText          ();

}; // class AlphaRampEdit



#endif // __UIRAMPEDIT_H__