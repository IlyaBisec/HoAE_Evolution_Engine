/*****************************************************************************/
/*    File:    uiObjectInspector.h
/*    Desc:    Control used for visual editing scene node properties
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-13-2003
/*****************************************************************************/
#ifndef __UIOBJECTINSPECTOR_H__
#define __UIOBJECTINSPECTOR_H__

#include "kEnumTraits.h"

#include "uiWidget.h"
#include "uiWindow.h"

class ClassMember;
/*****************************************************************************/
/*    Class:    InspectorItem
/*    Desc:    Visual aspect of the single object inspector item
/*****************************************************************************/
class InspectorItem
{
protected:
                                InspectorItem();
    
    Rct                            m_Extents;
    std::string                    m_Caption;
    int                            m_IndexInMap;
    DWORD                        m_FgColor;
    DWORD                        m_BgColor;
    bool                        m_bSelected;
    ClassMember*                m_pMember;
    Widget*                        m_pEditor;
    
    friend class                ObjectInspector;
}; // class InspectorItem

/*****************************************************************************/
/*    Class:    ObjectInspector
/*    Desc:    Control used for editing object property map
/*****************************************************************************/
class ObjectInspector : public Window
{
    PropertyMap                    m_Map;        //  pointer to the property map being shown
    std::vector<InspectorItem>    m_Items;    //  list of current items
    IReflected*                    m_pExposed; //  scene node we are being inspecting

    //  metric settings
    int                            m_DefItemHeight;
    int                            m_MaxColWidth;
    int                            m_MinColWidth;

    //  color settings
    DWORD                        m_SelBgColor;
    DWORD                        m_SelFgColor;
    DWORD                        m_BgColor;
    DWORD                        m_FgColor;
    DWORD                        m_DisabledFgColor;
    DWORD                        m_BgAltColor;
    DWORD                        m_LinesColor1;
    DWORD                        m_LinesColor2;
    DWORD                        m_SectionFgColor;
    DWORD                        m_SectionBgColor;
    
    bool                        m_bShowParent;  //  when false show leaf class properties only

public:
                    ObjectInspector        ();
                    ~ObjectInspector    ();

    void            Bind                ( IReflected* pExposed );

    virtual void    Render                ();
    virtual void    Expose                ( PropertyMap& pm );

    virtual bool     OnMouseLBDown        ( int mX, int mY );
    virtual bool     OnKeyDown            ( DWORD keyCode, DWORD flags );
    virtual void    OnInit              ();

    int                GetSelectIdx        () const;
    void            SetSelectIdx        ( int idx );
    int                GetNItems            () const { return m_Items.size(); }
    void            GetProperties       ();

    DECLARE_CLASS(ObjectInspector);

protected:
    void            UpdateItems            ();
    void            ClearItems          ();

    InspectorItem*     GetItemByPt            ( float pX, float pY );
    int                GetSelectedItem        ();
    void            ClearSelection        ();
    float            GetMaxCaptionWidth    ();
    float            GetLeftColWidth        ();
    float            GetRightColWidth    ();

private:
    const char*     GetEditorWidgetType ( const char* propType ) const;

}; // class ObjectInspector

#endif // __UIOBJECTINSPECTOR_H__