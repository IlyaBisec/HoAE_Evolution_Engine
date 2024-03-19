/*****************************************************************************/
/*    File:    uiListBox.h
/*    Desc:    ListWidget control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UILISTBOX_H__
#define __UILISTBOX_H__

#include "uiScrollBox.h"
#include "uiLabel.h"

/*****************************************************************************/
/*    Class:    ListWidget
/*    Desc:    General listbox control
/*****************************************************************************/    
class ListWidget : public ScrollBox
{
    bool                        m_bAutoHeight;

    DWORD                       m_SelTextColor;
    DWORD                       m_SelBgColor;
    int                         m_SelectedItem;

public:
    Signal                      sigSelChanged;

public:
                                ListWidget      ();
    virtual void                Render          ();
    virtual void                Expose          ( PropertyMap& pm );
    void                        AddItem         ( Widget* pItem );
    void                        AddItem         ( const char* item );
    void                        SetAutoHeight   ( bool bVal ) { m_bAutoHeight = bVal; }
    virtual bool                OnMouseMove     ( int mX, int mY, DWORD keys );
    virtual bool                OnMouseLBDown   ( int mX, int mY );

    void                        SelectItem      ( int idx ) { m_SelectedItem = idx; }

    DECLARE_CLASS(ListWidget);

protected:
    int                         PickItem        ( int mX, int mY );

}; // class ListWidget

#endif // __UILISTBOX_H__