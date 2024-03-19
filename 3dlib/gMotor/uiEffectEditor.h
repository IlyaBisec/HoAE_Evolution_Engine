/*****************************************************************************/
/*    File:    uiEffectEditor.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-23-2003
/*****************************************************************************/
#ifndef __UIEFFECTEDITOR_H__
#define __UIEFFECTEDITOR_H__

#include "ISceneEditor.h"

#include "uiWindow.h"
#include "uiPalette.h"
#include "uiMayaController.h"
#include "uiObjectInspector.h"
#include "uiNodeTree.h"
#include "uiManipulator.h"

#include "kFilePath.h"
#include "kTimer.h"

#include "sgDummy.h"

class PEmitter;
class PEffect;

/*****************************************************************************/
/*    Class:    EffectEditor    
/*    Desc:    Editor of the particle effects
/*****************************************************************************/
class EffectTreeRoot : public Group
{
public:

    virtual void            Expose      ( PropertyMap& pm );
    
    DECLARE_CLASS(EffectTreeRoot)
}; // class EffectTreeRoot

enum EffectFireMode
{
    fmParabolic         = 1,
    fmGroundMove        = 2
}; // enum EffectFireMode

ENUM( EffectFireMode, "EffectFireMode", en_val( fmParabolic, "Parabolic" ) << en_val( fmGroundMove, "GroundMove" ) );
/*****************************************************************************/
/*    Class:    EffectEditor    
/*    Desc:    Editor of the particle effects
/*****************************************************************************/
class EffectEditor : public Window
{
public:
                            EffectEditor    ();
    virtual void            Expose          ( PropertyMap& pm );
    virtual void            Render          ();

    virtual void            OnInit          ();

    virtual bool            OnChar          ( DWORD charCode, DWORD flags );
    virtual bool            OnKeyDown       ( DWORD keyCode, DWORD flags );

    void                    SetShowBackdrop ( bool val );
    bool                    IsShowBackdrop  () const;
    void                    Reset           ();
    void                    Load            ();
    void                    Fire            ();
    void                    Pause           ();
    void                    Save            ();
    void                    SaveAs          ();
    void                    DumpSequence    ();
    void                    KillAutoUpdate  ();
    void                    StopEffect      ();
    void                    PlayEffect      ();
    void                    SetEffectFile   ( const char* fname );
    float                   GetTotalTime    () const;
    float                   GetCurTime      () const;
    float                   GetTimeRatio    () const;
    void                    SetTimeRatio    ( float t );
    
    float                   GetPlayRate     () const;
    void                    SetPlayRate     ( float rate );

    int                     GetParticleArea () const;
    DWORD                   GetDangerColor  () const;
    void                    ResetCameras    ();

    float                   GetDirAngle     () const { return m_DirectionAngle; }
    void                    SetDirAngle     ( float ang ); 

    ActiveEditorCamera      GetCameraMode   () const { return IScEd->GetActiveCamera(); }
    void                    SetCameraMode   ( ActiveEditorCamera val ){ IScEd->SetActiveCamera( val ); }
    bool                    ShowGrid        () const { return m_bShowGrid; }
    void                    ShowGrid        ( bool val = true ){ m_bShowGrid = val; }

	bool InvertMouse() const { return m_bInvertMouse; }
	void InvertMouse(bool f = true) {
		m_bInvertMouse = f;
		m_CameraController.Invert(m_bInvertMouse);
	}

	static void GenQuad(BaseMesh &bm, int L, int idTex, int idSh);

    const char*             GetEffectFile   () const { return m_EffectFile.c_str(); }

    void                    SetEffectFireMode( EffectFireMode fm );
    EffectFireMode          GetEffectFireMode() const { return m_FireMode; }

    void                    SaveEffectTextures( const char* path );

    int                     GetNEmitters    () const;
    int                     GetNParticles   () const;
    bool                    IsEffectLoaded  () const { return (m_EffectRoot.NumChildren() > 0); }

    DECLARE_CLASS(EffectEditor);

protected:

    void                    DumpSetup       ();
    void                    LoadSubtree     ();
    void                    SaveSubtree     ();

    std::string             m_EffectFile;
    bool                    m_bLowPersp;

    SNode*                  m_pEffect;
    int                     m_InstanceID;    //    id of the currently playing instance

    float                   m_AlphaFactor;
    float                   m_IntensityFactor;
    bool                    m_bShowBackdrop;
    bool                    m_bShowGrid;
	bool					m_bInvertMouse;

	int						m_nRace;

    float                   m_PlayRate;
    float                   m_ShotTime;     
    float                   m_ShotVelocity;
    float                   m_ShotAngle;
    float                   m_ShotHeight;
    float                   m_ShotGravity;
    bool                    m_bSaveTextures;

    int                     m_DumpSide;
    float                   m_DumpFPS;
    bool                    m_bDumpSetupMode;


    EffectFireMode          m_FireMode;

    float                   m_DirectionAngle;

    Matrix4D                m_Transform;
    Matrix4D                m_TargetTransform;
    float                   m_CurShotTime;
    DWORD                   m_StartShotTime;
    float                   m_bShot;
    bool                    m_bLockFPS;
    
    NodeTree                m_Palette;
    SNode*                  m_PaletteNodes;

    Window                  m_ToolBar;

    PerspCamera             m_LowPerspCamera;
    PerspCamera             m_HighPerspCamera;

    MayaController          m_CameraController;
    NodeTree                m_EffectTree;
    ObjectInspector         m_Inspector;
    EffectTreeRoot          m_EffectRoot;
    
	uiMoveTool				m_MoveTool;
	uiMoveTool				m_AuxMoveTool;

    ScaleTool               m_Scaler;

    FilePath                m_LastPath;

    Timer                   m_ShotTimer;

    void                    SetupCameras();
    SNode*                  CreatePalette();
}; // class EffectEditor


#endif // __UIEFFECTEDITOR_H__