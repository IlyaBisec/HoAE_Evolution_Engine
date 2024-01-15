/***********************************************************************************/
/*  File:   sgAnimation.h
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __SGANIMATION_H__
#define __SGANIMATION_H__

#include "sgController.h"

typedef std::stack<float> float_stack;
/*****************************************************************************/
/*    Class:    Animation
/*    Desc:    Base animation class
/*****************************************************************************/
class Animation : public Controller
{
protected:

    float                   m_AnimationTime;
    float                   m_StartTime;
    float                   m_CurrentTime;
    
    static bool             s_bFrozen;
    static bool             s_bAnimateInvisible;
    static float_stack      s_CurAnimTime;
    static float_stack      s_Weight;
    static float_stack      s_CurAnimTimeDelta;
    static DWORD            s_PrevTime;

    SNode*                  m_pOperated;
    SNode*                  m_pOperatedParent;

public:
                            Animation           ();
    float                   GetAnimationTime    () const { return m_AnimationTime; }
    virtual void            SetAnimationTime    ( float val ) { m_AnimationTime = val; }

    float                   GetMaxTime          () const { return m_StartTime + m_AnimationTime; }

    float                   GetStartTime        () const { return m_StartTime; }
    virtual void            SetStartTime        ( float val ) { m_StartTime = val; }

    void                    SetCurrentTime      ( float val ) { m_CurrentTime = val; }
    float                   GetCurrentTime      () const { return m_CurrentTime; }

    void                    Play                ();  
    bool                    IsPlaying           () const;  
    void                    Pause               ();  
    void                    Stop                ();  
    void                    Loop                ( bool bLoop = true );  

    void                    SetOperated         ( SNode* pNode ) { m_pOperated = pNode; }
    SNode*                  GetOperated         () const { return m_pOperated; }

    void                    SetOperatedParent   ( SNode* pNode ) { m_pOperatedParent = pNode; }
    SNode*                  GetOperatedParent   () const { return m_pOperatedParent; }


    virtual void            Serialize           ( OutStream& os ) const;
    virtual void            Unserialize         ( InStream& is );
    virtual void            Expose              ( PropertyMap& pm );
    virtual void            Render              ();
    virtual void            BindNode            ( SNode* pNode );

    static float            CurTime             () { return s_CurAnimTime.empty() ? 0.0f : s_CurAnimTime.top(); }
    static void             PushTime            ( float anmTime ) { s_CurAnimTime.push( anmTime ); }
    static void             PopTime             () { if (!s_CurAnimTime.empty()) s_CurAnimTime.pop(); }

    static float            CurWeight           () { return s_Weight.empty() ? 0.0f : s_Weight.top(); }
    static void             PushWeight          ( float w ) { s_Weight.push( w ); }
    static void             PopWeight           () { if (!s_Weight.empty()) s_Weight.pop(); }

    static float            CurTimeDelta        () { return s_CurAnimTimeDelta.empty() ? 0.0f : s_CurAnimTimeDelta.top(); }
    static void             PushTimeDelta       ( float delta ) { s_CurAnimTimeDelta.push( delta ); }
    static void             PopTimeDelta        () { if (!s_CurAnimTimeDelta.empty()) s_CurAnimTimeDelta.pop(); }
    
    static void             SetupTimeDelta      ();

    static void             Freeze              () { s_bFrozen = true; }
    static void             Unfreeze            () { s_bFrozen = false; }
    static void             AnimateInvisible    ( bool anim = true ) { s_bAnimateInvisible = anim; }
    
    DECLARE_SCLASS(Animation,Controller,ANIM);

protected:
    bool                    m_bLooped;
    bool                    m_bPlayed;
    bool                    m_bPaused;

}; // class Animation

#endif //__SGANIMATION_H__