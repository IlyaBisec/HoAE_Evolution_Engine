/*****************************************************************************/
/*    File:    uiPropEditors.h
/*    Desc:    Property editors for the mostly used value types
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-13-2003
/*****************************************************************************/
#ifndef __UIPROPEDITORS_H__
#define __UIPROPEDITORS_H__

/*****************************************************************************/
/*    Class:    BoolPropEditor
/*    Desc:    Edits boolean object properties
/*****************************************************************************/
class BoolPropEditor : public CheckButton
{
    void                OnChangeText();
    void                ChangeText();
public:
    BoolPropEditor();
    virtual void        Render();

    DECLARE_CLASS(BoolPropEditor);
}; // class BoolPropEditor

/*****************************************************************************/
/*    Class:    MethodButton
/*    Desc:    Object inspector "activate method" button
/*****************************************************************************/
class MethodButton : public Button
{
public:
    virtual void        Render();

    DECLARE_CLASS(MethodButton);
}; // class MethodButton

#endif // __UIPROPEDITORS_H__