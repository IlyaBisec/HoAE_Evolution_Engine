/*****************************************************************/
/*  File:   IEffectManager.h
/*  Desc:   Interface for particle effects 
/*  Date:   Apr 2004                                             
/*****************************************************************/
#ifndef __IEFFECTMANAGER_H__
#define __IEFFECTMANAGER_H__

/*****************************************************************/
/*  Class:    IEffectManager
/*  Desc:    Interface for particle effects 
/*****************************************************************/
class IEffectManager 
{
public:
    //  render all effects, do not call it please explicitly
    virtual void            Render              () = 0;

    //  renders all bottom layer effects
    virtual void            PreRender           () = 0;
    //  evaluates all particle systems, but douesn't render anything
    virtual void            Evaluate            ( float dt = 0.0f ) = 0;
    //  post evaluation process - to finalize render sequence. It is required because you can want to swap order of PreRender and Post Render
    //so, valid claa sequences are:
    //1)...adding effects...,Evaluate,PreRender,PostRender,PostEvaluate
    //2)PreRender,PostEvaluate,...adding effects...,Evaluate,PostRender
    virtual void            PostEvaluate         () = 0;
    //  renders all top layer effects
    virtual void            PostRender          () = 0;

    //  pause all effect rendering
    virtual void            Pause               ( bool bPause = true ) = 0;

    //  creates instance of the given effect type in the current context
    virtual DWORD           InstanceEffect      ( DWORD modelID ) = 0;

	//  creates instance of the given effect type in the current context
    virtual DWORD           InstanceEffect      ( DWORD modelID,Matrix4D& TM ) = 0;

    //  rewinds effect to given time
    virtual void            RewindEffect        ( DWORD hInst, float amount, float step = 0.1f, bool bRewChildren = true ) = 0;
    
    //  sets transform of effect's instance
    virtual void            UpdateInstance      ( DWORD hInst, const Matrix4D& tm = Matrix4D::identity ) = 0;
   
    //  returns true if emitter instance is valid and present in the system 
    virtual bool            IsValid             ( DWORD hInst ) const = 0;
    
    //  destroys instance of the effect
    virtual void            DestroyInstance     ( DWORD hInst ) = 0;
    //  destroys instance of the effect in the current context
    virtual void            DestroyInstance     () = 0;

    //  current time of the playing emitter instance 
    virtual float           GetCurTime          ( DWORD hInst ) const = 0;
   
    //  sets global alpha value multiplier for given effect instance
    virtual void            SetAlphaFactor      ( DWORD hInst, float factor ) = 0;
    
    //  sets global emit intensity multiplier for given effect instance
    virtual void            SetIntensity        ( DWORD hInst, float intensity ) = 0;

    //Set/Get national color
    virtual void            SetCurrentNatColor  ( DWORD Color ) = 0;
    virtual DWORD           GetCurrentNatColor  () = 0;
    //  current time of the current context instance 
    virtual float           GetCurTime          () const = 0;
    //  sets global alpha value multiplier for current context effect instance
    virtual void            SetAlphaFactor      ( float factor ) = 0;
    //  sets global emit intensity multiplier for current context effect instance
    virtual void            SetIntensity        ( float intensity ) = 0;
    //  sets current target for the effect
    virtual void            SetTarget           ( const Vector3D& pos ) = 0;
    //  sets transform of effect's instance in the current context
    virtual void            UpdateInstance      ( const Matrix4D& tm = Matrix4D::identity ) = 0;
    //  enables/disables effect's instance auto-update mode
     virtual void           EnableAutoUpdate    (   bool bEnable = true, 
                                                    float timeout = 10.0f,
                                                    float fadeTime = 5.0f ) = 0;
     virtual void           EnableSelection     ( bool State ) = 0;
     virtual bool           GetSelectionState   () = 0;

    //  destroys all currently active emitters
    virtual void            Reset               () = 0;

    virtual int             GetNParticles       () const = 0;
    virtual int             GetNEmitterInst     () const = 0;
    virtual int             GetParticleArea     () const = 0;

    virtual float           GetPlayRate         () const = 0;
    virtual void            SetPlayRate         ( float rate ) = 0;
    virtual bool            IsPaused            () const = 0;

    //  effect set manipulation
    virtual int             GetEffectSetID      ( const char* fileName ) = 0;
    virtual int             GetNEffects         ( int setID ) = 0;
    virtual bool            BindEffectSet       ( int setID, DWORD modelID ) = 0;   
    virtual int             FindEffectByName    ( int setID, const char* effName ) = 0;

    virtual bool            UpdateInstance      ( int setID, int effID, const Matrix4D& tm = Matrix4D::identity ) = 0;
    virtual void            SetAlphaFactor      ( int setID, int effID, float alpha ) = 0;
    virtual void            SetIntensity        ( int setID, int effID, float intensity ) = 0;
    virtual float           GetTotalTime        ( DWORD hInst ) const = 0;
    virtual void            SetCurrentSubset    ( DWORD Subset ) = 0;
    virtual DWORD           GetCurrentSubset    () = 0;

}; // IEffectManager

/*****************************************************************/
/*  Class:    IEffect
/*  Desc:    Interface for effect instance
/*****************************************************************/
class IEffect 
{
public:
    virtual    DWORD         GetEffectID        () const = 0;
    virtual    const char*   GetEffectName      () const = 0;
    virtual    void          Render             () = 0;
    virtual    const char*   GetEffectFile      () const = 0;
    virtual    void          SetEffectFile      ( const char* fname ) = 0;
    virtual    int           GetPriority        () const = 0;
    virtual    void          SetPriority        ( int val ) = 0;
    virtual    float         GetAlpha           () const = 0;
    virtual    void          SetAlpha           ( float val ) = 0;

}; // class IEffect

extern IEffectManager* IEffMgr;

#endif // __IEFFECTMANAGER_H__