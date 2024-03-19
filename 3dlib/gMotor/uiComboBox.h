/*****************************************************************************/
/*    File:    uiComboBox.h
/*    Desc:    ComboWidget control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UICOMBOBOX_H__
#define __UICOMBOBOX_H__

#include "uiListBox.h"

/*****************************************************************************/
/*    Class:    ComboWidget
/*    Desc:    Combobox control
/*****************************************************************************/    
class ComboWidget : public Widget
{
    Button          m_Thumb;        //  combobox thumb button
    ListWidget      m_DropList;     //  droplist
    Label           m_Selection;    //  currently selected item edit box

    bool            m_bDropActive;
    float           m_ThumbWidth;

public:
                    ComboWidget     ();
    virtual void    Render          ();
    virtual void    Expose          ( PropertyMap& pm );
    void            AddItem         ( const char* item );
    void            OnDropDown      ();
    void            HideDropList    (); 
    void            OnSelectItem    ();
    virtual bool    OnMouseLBDown   ( int mX, int mY );

    DECLARE_CLASS(ComboWidget);
}; // class ComboWidget

#endif // __UICOMBOBOX_H__