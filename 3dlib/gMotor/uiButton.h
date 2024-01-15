/*****************************************************************************/
/*    File:    uiButton.h
/*    Desc:    Button control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

/*****************************************************************************/
/*    Class:    Button
/*    Desc:    Push button control
/*****************************************************************************/    
class Button : public Widget
{
protected:
    bool            m_bDown;         //  button is down
    
    std::string     m_SpritePack;
    int             m_SpriteID;
    int             m_SpritePackID;
    bool            m_bDrawBorder;

public:
    Signal          sigPressed;      //  signals that button was pressed

public:

                    Button          ();
    virtual void    Expose          ( PropertyMap& pm );
    virtual void    Render            ();
    virtual bool     OnMouseLBDown   ( int mX, int mY );
    virtual bool     OnMouseLBUp        ( int mX, int mY );
    void            SetOnPress      ();
    bool            IsDown          () const { return m_bDown; }
    void            Press           () { m_bDown = true; }

    const char*     GetSpritePack   () const { return m_SpritePack.c_str(); }
    void            SetSpritePack   ( const char* name ) { m_SpritePack = name; }
    void            SetSpriteID     ( int id ) { m_SpriteID = id; }
    int             GetSpriteID     () const { return m_SpriteID; }

    DECLARE_CLASS(Button);
}; // class Button

/*****************************************************************************/
/*    Class:    RadioButton
/*    Desc:    Radio button control
/*****************************************************************************/    
class RadioButton : public Button
{
public:

                    RadioButton     ();
    virtual void    Expose          ( PropertyMap& pm );
    virtual void    Render            ();
    virtual bool     OnMouseLBDown   ( int mX, int mY );
    virtual bool     OnMouseLBUp        ( int mX, int mY );
    void            Unpress         () { m_bDown = false; }


    DECLARE_CLASS(RadioButton);
}; // class RadioButton

/*****************************************************************************/
/*    Class:    RadioGroup
/*    Desc:    Grouping radio button control
/*****************************************************************************/    
class RadioGroup : public Widget
{
    bool            m_bAllowNonePresed;

public:

                    RadioGroup      ();
    virtual void    Expose          ( PropertyMap& pm );
    virtual void    Render            ();
    virtual bool    AddChild        ( IReflected* pChild );
    void            OnButtonPressed ( RadioButton* pButton );

    DECLARE_CLASS(RadioGroup);
}; // class RadioGroup

#endif // __UIBUTTON_H__