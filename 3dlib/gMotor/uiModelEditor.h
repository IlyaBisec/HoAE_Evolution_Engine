/*****************************************************************************/
/*    File:    uiModelEditor.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-31-2003
/*****************************************************************************/
#ifndef __UIMODELEDITOR_H__
#define __UIMODELEDITOR_H__

#include "ISceneEditor.h"
#include "vCamera.h"

#include "uiNodeTree.h"
#include "uiMayaController.h"
#include "uiObjectInspector.h"
#include "kFilePath.h"
#include "kTimer.h"
#include "uiManipulator.h"
#include "sgShader.h"
#include "sgDummy.h"
#include "sgGeometry.h"
#include "uiTransformTools.h"

class NodeTree;
/*****************************************************************************/
/*    Class:    ModelEditor
/*  Desc:    Core Kangaroo functionality
/***********************************************>******************************/
class ModelEditor : public FrameWindow, public ISceneEditor
{
public:
                        ModelEditor            ();
    virtual                ~ModelEditor        ();

    virtual bool         OnMouseLBDown        ( int mX, int mY );
    virtual void        Render                ();
    virtual void        Expose                ( PropertyMap& pm );
    virtual bool        OnKeyDown            ( DWORD keyCode, DWORD flags );    
    virtual bool        OnChar                ( DWORD keyCode, DWORD flags );    
    virtual void        OnInit              ();
    void                ShowCameras            ( bool val = true );
    bool                GetShowCameras        () const { return m_bShowCameras; }
    void                LoadSubtree         ();

    void                SetActiveCamera     ( ActiveEditorCamera ac )    { m_ActiveCamera = ac; }
    ActiveEditorCamera    GetActiveCamera        () const                    { return m_ActiveCamera; }
    void                ShowGrid            ( bool bShow = true )        { m_bShowGrid = bShow; }
    bool                IsShowGrid            () const                    { return m_bShowGrid; }
    void                ShowBackdrop        ( bool bShow = true )        { m_bShowBackdrop = bShow; }
    bool                IsShowBackdrop        () const                    { return m_bShowBackdrop; }


    virtual IReflected*    GetSelectedNode        ();
    virtual void        SelectNode          ( IReflected* pNode );

    void                SetModel            ( int mdlID );   

    bool                IsModelLoaded       () const;
    bool                IsAnimLoaded        () const;

    DECLARE_CLASS(ModelEditor);

public:
    float               GetCurAnimTimeRatio () const;
    void                SetCurAnimTimeRatio ( float t );

    void                 DeleteNode            ();
    void                ToggleInspector        ();
    void                 InsertNode            ();
    void                 ToggleHideModel        ();
    void                 ToggleStatistics    ();
    void                ToggleDrawTerrain   ();
    void                 ApplySelectionTool    ();
    void                 ApplyTranslateTool    ();
    void                 ApplyRotateTool        ();
    void                 ApplyScaleTool        ();
    void                 ZoomSelection        ();
    void                 ZoomExtents            ();
    void                Play                ();
    void                Pause               ();
    void                Stop                ();


    void                InstantiateInput    ();
    void                ResetModel            ();
    void                LoadModel            ();
    void                SaveModel           ();
    void                LoadAnimation        (int Index);
	void                LoadAnimation        ();
    void                ResetCameras        ();
	void				ConvertSkin2GPU();

    void                SaveEffects         ();
    void                LoadEffects         ();

    void                CopyNode            ();
    void                PasteNode            ();
    void                CutNode                ();
    void                MoveUpNode            ();
    void                MoveDownNode        ();

    void                ToggleLeftPane        ();
    void                ToggleRightPane        ();
    void                ToggleInvisible        ();
    void                ToggleMeasure       ();

    void                DrawSkeleton        ();
    void                DrawSlots            ();
    void                DrawLights            ();
    void                DrawTransforms        ();
    void                DrawLocators        ();
    void                DrawEffectEmitters    ();
    void                DrawCameras            ();
    void                DrawBoundBoxes        ();
    void                DrawBoundSpheres    ();
    void                DrawGrid            ();
    void                DrawNormals         ();
    void                DrawModelStats      ();

    void                CreateVIPM            ();

    void                PlayCinematics      ();
    void                ResetCinematics     ();
    void                CycleActiveCamera   ();
    void                UpdateAllModels     ();

    bool                    m_bShowCameras;
    bool                    m_bShowLights;
    bool                    m_bShowLocators;
    bool                    m_bShowBones;
    bool                    m_bShowTransformNodes;
    bool                    m_bShowNormals;
    bool                    m_bShowModelStats;
    bool                    m_bShowBackdrop;

    bool                    m_bDrawTerrain;
    bool                    m_bDrawModel;

    bool                    m_bPlayAnim;
    bool                    m_bPauseAnim;

    float                    m_Size;
    float                    m_HandleSide;
    float                   m_AnimSpeed;

    DWORD                    m_FrustumColor;
    DWORD                    m_FrustumLinesColor;

    DWORD                    m_AABBFillColor;
    DWORD                    m_AABBLinesColor;

    SNode*                    m_pSelectedNode;
    SNode*                    m_pRootNode;

    NodeTree                m_TreeL;        //  left tree pane
    NodeTree                m_TreeR;        //  right tree pane
    NodeTree                m_PalTree;        //  current editor's node palette tree
    MayaController          m_MayaController;
    ObjectInspector            m_Inspector;

    int                     m_ModelID;
    int                     m_AnimID;
	int                     m_AnimID1;
	int                     m_AnimID2;
    bool                    m_bDrawMeasure;

    Timer                   m_AnimTimer;
    float                   m_ModelTransparency;
    DWORD                   m_ModelColor;

    OrthoCamera                m_GameOrthoCamera;
    PerspCamera                m_GamePerspCamera;
    PerspCamera                m_EditorCamera;

    TranslateTool              m_Measure;
    SelectionTool              m_Selection;

    ActiveEditorCamera        m_ActiveCamera;
    bool                    m_bInited;
    bool                    m_bShowGrid;
    DWORD                    m_GridColor;
    DWORD                    m_GridSubColor;
    float                    m_GridSide;
    int                        m_NGridCells;

    Matrix4D                m_MeasureTM;

    Window                  m_Toolbar;
    FilePath                m_LastPath;

}; // class ModelEditor

ISceneEditor*            GetModelEditor();

#endif // __UIMODELEDITOR_H__