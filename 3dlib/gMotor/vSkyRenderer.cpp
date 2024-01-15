/*****************************************************************************/
/*    File:    vSkyRenderer.cpp
/*    Desc:    Skinned mesh
/*    Author:  Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "IMediaManager.h"
#include "ISkyRenderer.h"
#include "kContext.h"

/*****************************************************************************/
/*  Class:  SkyRenderer
/*  Desc:   
/*****************************************************************************/
class SkyRenderer : public IReflected, public ISkyRenderer
{
public:
    virtual void        Expose( PropertyMap& pm );
    virtual void        Render();
}; // class SkyRenderer

SkyRenderer g_SkyRenderer;
ISkyRenderer* ISky = &g_SkyRenderer;

void SkyRenderer::Expose( PropertyMap& pm )
{

}

void SkyRenderer::Render()
{
    static int mID = IMM->GetModelID( "models\\sky\\skybox.c2m" );
    IRS->ResetWorldTM();
    IRS->SetShaderAutoVars();    
    IMM->StartModel( mID, Matrix4D::identity, mID );
    IMM->DrawModel();
} // SkyRenderer::Render
