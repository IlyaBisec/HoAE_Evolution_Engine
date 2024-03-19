/*****************************************************************************/
/*    File:    uiNoisePanel.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __UINOISEPANEL_H__
#define __UINOISEPANEL_H__

/*****************************************************************************/
/*    Class:    NoisePanel
/*    Desc:    
/*****************************************************************************/
class NoisePanel : public Widget
{
    int                 m_TexSide;
    int                 m_TexID;

public:
                        NoisePanel      ();
    virtual void        Render            ();
    virtual void        Expose            ( PropertyMap& pm );
    void                Update          ();

}; // class NoisePanel

#endif // __UINOISEPANEL_H__