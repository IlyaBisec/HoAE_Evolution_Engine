#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgTexture.h"
#include "kFilePath.h"

#ifndef _INLINES
#include "sgTexture.inl"
#endif // _INLINES

IMPLEMENT_CLASS( RenderTarget   );
IMPLEMENT_CLASS( Texture        );
IMPLEMENT_CLASS( TextureManager );

/*****************************************************************************/
/*    RenderTarget implementation
/*****************************************************************************/
RenderTarget::RenderTarget() : m_pTexture(NULL), m_pDepthBuffer(NULL)
{
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Render()
{ 
    if (!m_pTexture) 
    {
        SNode* pNode = GetInput( 0 );
        if (pNode && pNode->IsA<Texture>() )
        {
            m_pTexture = (Texture*)pNode;
        }
    }

    DWORD depthID = m_pDepthBuffer ? m_pDepthBuffer->GetTexID() : -1;

    if (m_pTexture) IRS->PushRenderTarget( m_pTexture->GetTexID(), depthID );
    
    Rct curVP = IRS->GetViewPort();
    IRS->SetViewPort( m_ViewPort );

    if (m_bClear) 
    {
        if (m_pDepthBuffer && m_pDepthBuffer->GetTexID() != -1) 
        {
            IRS->ClearDevice( m_ClearColor, true, true, false );
        }
        else
        {
            IRS->ClearDevice( m_ClearColor );
        }
    }
    SNode::Render();
    if (m_pTexture) IRS->PopRenderTarget();
    IRS->SetViewPort( curVP );

} // RenderTarget::Render

void RenderTarget::SetViewPort( float x, float y, float w, float h )
{
    m_ViewPort.Set( x, y, w, h );
}

void RenderTarget::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "RenderTarget", this );
    pm.f( "Clear",        m_bClear );
    pm.f( "ClearColor", m_ClearColor, "color" );
} // RenderTarget::Expose

void RenderTarget::Serialize( OutStream& os ) const
{
    Parent::Serialize( os ); 
    os << m_ClearColor << m_bClear;
}

void RenderTarget::Unserialize( InStream&  is )
{
    Parent::Unserialize( is ); 
    is >> m_ClearColor >> m_bClear;
}

void RenderTarget::SetTarget(Texture* pTex, Texture* pDepth )
{ 
    m_pTexture        = pTex; 
    m_pDepthBuffer    = pDepth;
    m_ViewPort.Set( 0.0f, 0.0f, pTex->GetWidth(), pTex->GetHeight() );
}

/*****************************************************************************/
/*    Texture implementation
/*****************************************************************************/
Texture* Texture::s_pCurTexture[Texture::c_MaxStages];
bool     Texture::s_bFrozen = false;
std::stack<int> Texture::s_Overrides[c_MaxStages];

void Texture::PushOverride( int stage, int texID )
{
   s_Overrides[stage].push( texID );  
} // Texture::PushOverride

void Texture::PopOverride( int stage )
{
    if (s_Overrides[stage].size() > 0) s_Overrides[stage].pop();  
} // Texture::PopOverride


void Texture::Render()
{
    if (s_bFrozen) return;

    if (m_TexID == -1)
    {
        m_TexID = IRS->GetTextureID( GetName() );
        if (m_TexID == -1) m_TexID = 0;
    }

    if (s_Overrides[m_Stage].size() > 0)
    {
        IRS->SetTexture( s_Overrides[m_Stage].top(), m_Stage );
    }
    else
    {
        IRS->SetTexture( m_TexID, m_Stage, true );
    }
    
} // Texture::Render

std::string Texture::GetFileName() const
{
    FilePath path( GetName() );
    return std::string( path.GetFileWithExt() );
} // Texture::GetFileName

void Texture::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Stage;
    BYTE buf[28];
    os.Write( buf, sizeof( buf ) );
} // Texture::Serialize

void Texture::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Stage;
    BYTE buf[28];
    is.Read( buf, sizeof( buf ) );
    CreateTexture();

} // Texture::Unserialize

void Texture::VisitAttributes()
{
    s_pCurTexture[m_Stage] = this;
}

bool Texture::CreateTexture()
{
    if (!IRS) return false;
    if (m_TexID != -1)
    {
        IRS->DeleteTexture( m_TexID );
        m_TexID = -1;
    }
    //m_TexID = IRS->CreateTexture( GetName() );
    return false;
} // Texture::CreateTexture

void Texture::CreateMipLevels()
{
    IRS->CreateMipLevels( m_TexID );
} // Texture::CreateMipLevels

BYTE* Texture::LockBits( int& pitch, int level )
{
    return IRS->LockTexBits( m_TexID, pitch, level );
} // Texture::LockBits

void Texture::UnlockBits( int level )
{
    IRS->UnlockTexBits( m_TexID, level );
} // Texture::UnlockBits

bool Texture::IsEqual( const SNode* node ) const
{
    if (!node->IsA<Texture>()) return false;
    if (stricmp( GetName(), node->GetName() )) return false;
    if (m_Stage != ((Texture*)node)->GetStage()) return false;
    return true;
} // Texture::IsEqual

bool Texture::Save( const char* fname )
{
    if (m_TexID == -1) return false;
    IRS->SaveTexture( m_TexID, fname );
    return true;
} // Texture::Save

void Texture::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Texture", this );
    pm.p( "TexID", &Texture::GetTexID, &Texture::SetTexID                  );
    pm.p( "Stage", &Texture::GetStage, &Texture::SetStage                  );
    pm.p( "Width", &Texture::GetWidth                            );
    pm.p( "Height", &Texture::GetHeight                           );
    pm.p( "ColorFormat", &Texture::GetColorFormat                      );
    pm.p( "DepthStencil", &Texture::GetDepthStencilFormat               );
    pm.p( "Procedural", &Texture::IsProcedural                        );
    pm.p( "RenderTarget", &Texture::IsRT                                );
    pm.p( "MemoryPool", &Texture::GetMemoryPool                       );
    pm.p( "Usage", &Texture::GetUsage                            );
    pm.p( "NMips", &Texture::GetNMips                            );
    pm.p( "Pixels", &Texture::GetTexID, &Texture::SetTexID, "texture"       );
    pm.p( "File", &Texture::GetTextureFile, &Texture::SetTextureFile, "#texture" );
} // Texture::Expose

void Texture::SetTextureFile( const char* file )
{
    SetName( file );
    m_TexID = -1;
} // Shader::SetScriptFile


