/*****************************************************************/
/*  File:   ISceneEditor.h
/*  Desc:   Interface for scene editor
/*  Date:   Apr 2004                                             
/*****************************************************************/
#ifndef __ISCENEEDITOR_H__
#define __ISCENEEDITOR_H__

/*****************************************************************************/
/*    Enum: ActiveEditorCamera
/*****************************************************************************/
enum ActiveEditorCamera
{
    aecEditor       = 0,
    aecGameOrtho    = 1,
    aecGamePersp    = 2,

    aecLAST         = 3,
}; // enum ActiveEditorCamera

/*****************************************************************/
/*  Class:    ISceneEditor
/*  Desc:    Interface for scene editor
/*****************************************************************/
class ISceneEditor 
{
public:
    //  render all effects
    virtual void                Render              () = 0;
    virtual void                SetActiveCamera     ( ActiveEditorCamera ac ) = 0;
    virtual ActiveEditorCamera  GetActiveCamera     () const = 0;
    virtual void                ShowGrid            ( bool bShow = true ) = 0;
    virtual bool                IsShowGrid          () const = 0;
    virtual IReflected*         GetSelectedNode     () = 0;
    virtual void                SelectNode          ( IReflected* pNode ) = 0;

}; // ISceneEditor

extern ISceneEditor* IScEd;

int        GetEditorFontID();
int        GetEditorGlyphsID();

ENUM( ActiveEditorCamera, "ActiveEditorCamera", 
     en_val( aecEditor,    "Editor"    ) << 
     en_val( aecGameOrtho, "GameOrtho" ) << 
     en_val( aecGamePersp, "GamePersp" ) );

#endif // __ISCENEEDITOR_H__