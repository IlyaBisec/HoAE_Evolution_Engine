/*****************************************************************************/
/*    File:    uiSceneEditor.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "ICamera.h"
#include "uiWidget.h"
#include "uiSceneEditor.h"

#include "ISkyRenderer.h"
#include "ITerrain.h"
#include "IWater.h"
#include "kDirIterator.h"
#include "IMediaManager.h"

/*****************************************************************************/
/*    ModelPalette implementation
/*****************************************************************************/
IMPLEMENT_CLASS(ModelPalette);
ModelPalette::ModelPalette()
{
    SetDraggable();
} // ModelPalette::ModelPalette

void ModelPalette::AddModel( const char* mdlName )
{
    ModelView* pItem = new ModelView();
    AddItem( pItem );
    pItem->SetModel( mdlName );
} // ModelPalette::AddModel

void ModelPalette::Serialize( OutStream& os ) const
{
} // ModelPalette::Serialize

void ModelPalette::Unserialize( InStream& is )
{
} // ModelPalette::Unserialize

void ModelPalette::Expose( PropertyMap& pm )
{

} // ModelPalette::Expose

void ModelPalette::Render()
{
    ListWidget::Render();
} // ModelPalette::Render

/*****************************************************************************/
/*    SpritePalette implementation
/*****************************************************************************/
IMPLEMENT_CLASS(SpritePalette);
SpritePalette::SpritePalette()
{
    SetDraggable();
    m_PackID    = -1;
    m_CellSide  = -1;
    m_HCells    = 0;
    m_VCells    = 0;
} // SpritePalette::SpritePalette

void SpritePalette::Expose( PropertyMap& pm )
{
    pm.start<Window>( "SpritePalette", this );
    pm.p( "SpritePackage", &SpritePalette::GetPackage, &SpritePalette::SetPackage );
} // SpritePalette::Expose

void SpritePalette::Render()
{
    if (m_PackID == -1)
    {
        m_PackID = ISM->GetPackageID( m_Package.c_str() );
    }
    int nSp = ISM->GetNFrames( m_PackID );
    if (nSp == 0 || nSp == 0x7FFFFFFF) return;
    Rct ext = GetScreenExtents();

    int cellW = ext.w/m_CellSide;
    int cellH = nSp/cellW + 1;
    if (m_CellSide == -1)
    {
        m_CellSide = sqrtf( ext.w*ext.h/(float( nSp )) );   
        cellW = ext.w/m_CellSide;
        cellH = nSp/cellW + 1;
        while (cellW*cellH < nSp)
        {
            m_CellSide--;
            cellW = ext.w/m_CellSide;
            cellH = nSp/cellW + 1;
        }
    }
    m_HCells = cellW;
    m_VCells = cellH;

    SetExtents( GetExtents().x, GetExtents().y, cellW*m_CellSide + 2, cellH*m_CellSide + 2 );

    ext = GetScreenExtents();
    rsInnerPanel( ext, 0, 0x33FFFFFF );

    int cX = ext.x;
    int cY = ext.y;
    ISM->SetCurrentDiffuse( 0xBBFFFFFF );
    for (int i = 0; i < nSp; i++)
    {
        Rct rc( cX, cY, m_CellSide + 1, m_CellSide + 1 );
        rsFrame( rc, 0.0f, 0x44FFFFFF );
        Rct frame;
        ISM->GetBoundFrame( m_PackID, i, frame );
        
        float scale = m_CellSide/tmax( frame.w, frame.h );
        Matrix4D tm = Matrix4D::identity;
        tm.e30 = cX - frame.x*scale;
        tm.e31 = cY - frame.y*scale;
        tm.e00 = scale;
        tm.e11 = scale;
        tm.e22 = scale;
        ISM->DrawSprite( m_PackID, i, tm );
        cX += m_CellSide;
        if (cX + m_CellSide >= ext.GetRight())
        {
            cX = ext.x;
            cY += m_CellSide;
        }
    }

    //  render selection
    for (int i = 0; i < m_Selected.size(); i++)
    {
        int idx = m_Selected[i];
        int cX = GetScreenExtents().x + (idx%m_HCells)*m_CellSide;
        int cY = GetScreenExtents().y + (idx/m_HCells)*m_CellSide;
        Rct cExt( cX, cY, m_CellSide, m_CellSide );
        rsRect( cExt, 0.0f, 0x44FFFF00 );
    }

    rsFlush();
    ISM->Flush();
} // SpritePalette::Render

int SpritePalette::PickCell( int mX, int mY )
{
    if (!PtIn( mX, mY)) return -1;
    int i = mX/m_CellSide;
    int j = mY/m_CellSide;
    if (i < 0 || i >= m_HCells) return -1;
    if (j < 0 || j >= m_VCells) return -1;
    return i + j*m_HCells;
} // SpritePalette::PickCell

bool SpritePalette::OnMouseLBDown( int mX, int mY )
{
    int idx = PickCell( mX, mY );
    if (idx == -1) return false;
    if (GetKeyState( VK_CONTROL ) >= 0) ClearSelection();
    AddSelected( idx );
    return false;
} // SpritePalette::OnMouseLBDown

bool SpritePalette::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (GetKeyState( VK_CONTROL ) < 0 && GetKeyState( VK_LBUTTON ) < 0 )
    {
        int idx = PickCell( mX, mY );
        if (idx == -1) return false;
        AddSelected( idx );
    }
    return false;
} // SpritePalette::OnMouseMove

/*****************************************************************************/
/*    SceneEditor implementation
/*****************************************************************************/
IMPLEMENT_CLASS(SceneEditor);
SceneEditor::SceneEditor()
{
    AlignToParent();
    AddChild( m_ModelPalette );
    m_ModelPalette.SetExtents( 0, 0, 100, 600 );
    m_ModelPalette.SetVisible( false );

    AddChild( m_SpritePalette );

    m_bDrawTerrain = true;
    m_bDrawSkybox  = true;

    AddChild( m_RTSController   );
    m_RTSController.AlignToParent();
    SetName         ( "SceneEditor" );
    SetDraggable    ( false );
    SetKeptAligned  ( true );

    m_BrushRadius   = 1000.0f; 
    m_BrushColor    = 0x550000FF;
} // SceneEditor::SceneEditor

void SceneEditor::OnInit()
{
    m_SpritePalette.CreateFromScript( "scene_editor_spritepal" );
} // SceneEditor::OnInit

void SceneEditor::Serialize( OutStream& os ) const
{
} // SceneEditor::Serialize

void SceneEditor::Unserialize( InStream& is )
{
} // SceneEditor::Unserialize

void SceneEditor::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "SceneEditor", this );
    pm.f( "DrawTerrain", m_bDrawTerrain );
    pm.f( "DrawSkybox", m_bDrawSkybox );
} // SceneEditor::Expose

bool SceneEditor::OnMouseMove( int mX, int mY, DWORD keys )
{
    m_Mx = mX;
    m_My = mY;
    return false;
} // SceneEditor::OnMouseMove

void SceneEditor::Render()
{
    ICamera* pCam = GetCamera();
    if (pCam) pCam->Render();
    if (m_bDrawSkybox) 
    {
        ISky->Render();
        IRS->Flush();
        
    }
    if (m_bDrawTerrain) ITerra->Render();
    
    DrawBrush();

} // SceneEditor::Render

void SceneEditor::DrawBrush()
{
    ICamera* pCam = GetCamera();
    if (!pCam) return;
    
    Vector3D pt;
    ITerra->Pick( m_Mx, m_My, pt );
    static int s_BrushTexID = IRS->GetTextureID( "Models\\kangaroo\\brush.tga" );
    ITerra->DrawPatch( pt.x, pt.y, m_BrushRadius, m_BrushRadius, 0.0f, Rct::unit, 0.5f, 0.5f,
                        s_BrushTexID, m_BrushColor, true );
    ITerra->FlushPatches();
} // SceneEditor::DrawBrush

bool SceneEditor::OnKeyDown( DWORD keyCode, DWORD flags )
{
    if (keyCode == 'T') m_bDrawTerrain = !m_bDrawTerrain;
    if (keyCode == 'Q' && GetKeyState( VK_CONTROL ) < 0) IRM->UpdateResources();
    return false;
} // SceneEditor::OnKeyDown

void SceneEditor::UpdateModelPalette( const char* dir )
{
    DirIterator it( dir );
    while (it)
    {
        if (it.IsDirectory()) UpdateModelPalette( it.GetFullFilePath() );
        else if (!stricmp( it.GetFileExt(), "c2m" ))
        {
            m_ModelPalette.AddModel( it.GetFileName() );
        }
        ++it;
    }
} // SceneEditor::UpdateModelPalette



