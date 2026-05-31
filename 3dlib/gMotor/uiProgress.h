/*****************************************************************************/
/*    File:    uiProgress.h
/*    Desc:    Progress control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIPROGRESS_H__
#define __UIPROGRESS_H__

/*****************************************************************************/
/*    Class:    Progress
/*    Desc:    Progress control
/*****************************************************************************/    
class Progress : public Widget
{
    float           m_Progress;
    int             m_TickWidth;
    int             m_TickSpacing;
    DWORD           m_BarColor;
    bool            m_bDrawText;

public:
                    Progress    ();
    virtual void    Expose      ( PropertyMap& pm );
    virtual void    Render        ();

    float           GetProgress () const { return m_Progress; }
    void            SetProgress ( float pos ) { if (pos <= 1.0f || pos >= 0.0f) m_Progress = pos; }

    DECLARE_CLASS(Progress);
}; // class Progress

#endif // __UIPROGRESS_H__