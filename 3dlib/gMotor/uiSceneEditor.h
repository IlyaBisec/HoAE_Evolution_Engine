/*****************************************************************************/
/*    File:    uiSceneEditor.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-31-2003
/*****************************************************************************/
#ifndef __UISCENEEDITOR_H__
#define __UISCENEEDITOR_H__

#include "uiModelView.h"
#include "uiListBox.h"
#include "uiRTSController.h"

/*****************************************************************************/
/*    Class:    ModelPalette
/*    Desc:    Control which contains preview of the set of scene objects
/*****************************************************************************/
class ModelPalette : public ListWidget
{
public:
                    ModelPalette        ();
    virtual void    Serialize            ( OutStream& os ) const;
    virtual void    Unserialize            ( InStream& is );
    virtual void    Expose                ( PropertyMap& pm );
    virtual void    Render              ();

    void            AddModel            ( const char* mdlName );

    DECLARE_CLASS(ModelPalette);
}; // class ModelPalette

/*****************************************************************************/
/*    Class:    SpritePalette
/*    Desc:    Control which contains preview of the set of scene objects
/*****************************************************************************/
class SpritePalette : public Window
{
    std::string         m_Package;
    int                 m_PackID;
    int                 m_CellSide;
    int                 m_HCells;
    int                 m_VCells;

    std::vector<int>    m_Selected;

    int                 PickCell( int mX, int mY );

public:
                        SpritePalette       ();
    virtual void        Expose                ( PropertyMap& pm );
    virtual void        Render              ();

    void                SetPackage          ( const char* packName ) { m_Package = packName; }
    const char*         GetPackage          () const { return m_Package.c_str(); }
    virtual bool         OnMouseLBDown        ( int mX, int mY );
    virtual bool         OnMouseMove            ( int mX, int mY, DWORD keys );

    void                ClearSelection      () { m_Selected.clear(); }
    void                AddSelected         ( int id ) 
    {
        for (int i = 0; i < m_Selected.size(); i++)
        {
            if (m_Selected[i] == id) return;
        }
        m_Selected.push_back( id );
    }

    DECLARE_CLASS(SpritePalette);
}; // class SpritePalette

/*****************************************************************************/
/*    Class:    SceneEditor
/*    Desc:    
/*****************************************************************************/
class SceneEditor : public Widget
{
    ModelPalette    m_ModelPalette;
    SpritePalette   m_SpritePalette;

    RTSController   m_RTSController;

    bool            m_bDrawTerrain;
    bool            m_bDrawSkybox;
    
    int             m_Mx, m_My;

    float           m_BrushRadius; 
    DWORD           m_BrushColor;

    void            DrawBrush();

public:
                    SceneEditor         ();
    virtual void    Serialize            ( OutStream& os ) const;
    virtual void    Unserialize            ( InStream& is );
    virtual void    Expose                ( PropertyMap& pm );
    virtual void    OnInit              ();
    virtual void    Render              ();
    virtual bool    OnKeyDown           ( DWORD keyCode, DWORD flags );
    virtual bool    OnMouseMove         ( int mX, int mY, DWORD keys );

    void            UpdateModelPalette  ( const char* dir );

    DECLARE_CLASS(SceneEditor);
}; // class SceneEditor

#endif // __UISCENEEDITOR_H__