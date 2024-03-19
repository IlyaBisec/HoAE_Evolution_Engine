/*****************************************************************************/
/*    File:    uiExtendedEdit.h
/*    Desc:    ExtendedEdit control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIEXTENDEDEDIT_H__
#define __UIEXTENDEDEDIT_H__

/*****************************************************************************/
/*    Class:    ExtendedEdit
/*    Desc:    
/*****************************************************************************/
class ExtendedEdit : public Widget
{
    Button              m_btnBrowse;
    EditBox             m_edPath;

    std::string         m_ExType;

public:
                        ExtendedEdit    ();
    virtual void        Render          ();
    void                SetExtendedType ( const char* type ) { m_ExType = type; }
    virtual bool        OnExtents       ();

    void                Browse          ();
    void                Box2Text        ();
    void                Text2Box        () { m_edPath.SetText( GetText() ); } 

    DECLARE_CLASS(ExtendedEdit);
}; // class ExtendedEdit

#endif // __UIEXTENDEDEDIT_H__