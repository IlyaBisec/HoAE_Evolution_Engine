/*****************************************************************************/
/*    File:    uiWidgetEditor.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-23-2003
/*****************************************************************************/
#ifndef __UIWIDGETEDITOR_H__
#define __UIWIDGETEDITOR_H__

#include "uiObjectInspector.h"
#include "uiNodeTree.h"
#include "uiFrameWindow.h"

/*****************************************************************************/
/*    Class:    WidgetEditor    
/*    Desc:    Editor of the user interface elements
/*****************************************************************************/
class WidgetEditor : public Window
{
    NodeTree                m_WidgetTree;
    ObjectInspector         m_Inspector;

public:
                            WidgetEditor    ();
    virtual void            Expose            ( PropertyMap& pm );
    virtual void            Render            ();
    virtual void            OnInit          ();
    
    DECLARE_CLASS(WidgetEditor);
protected:
}; // class WidgetEditor

#endif // __UIWIDGETEDITOR_H__