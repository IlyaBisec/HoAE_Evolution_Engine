/*****************************************************************************/
/*    File:    sgShader.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgShader.h"
#include "sgTexture.h"

#ifndef _INLINES
#include "sgShader.inl"
#endif // !_INLINES

IMPLEMENT_CLASS( SurfaceMaterial            );
IMPLEMENT_CLASS( BumpMatrix     );
IMPLEMENT_CLASS( Shader );

/*****************************************************************************/
/*    SurfaceMaterial implementation
/*****************************************************************************/
SurfaceMaterial*            SurfaceMaterial::s_pCurMtl = NULL;

void SurfaceMaterial::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Ambient << m_Diffuse << m_Specular << m_Shininess << m_Transparency;
} // SurfaceMaterial::Serialize

void SurfaceMaterial::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Ambient >> m_Diffuse >> m_Specular >> m_Shininess >> m_Transparency;
} // SurfaceMaterial::Unserialize

void SurfaceMaterial::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "SurfaceMaterial", this );
    pm.f( "Ambient",      m_Ambient,    "color" );
    pm.f( "Diffuse",      m_Diffuse,    "color" );
    pm.f( "Specular",     m_Specular,   "color" );
    pm.f( "Shininess",    m_Shininess           );
    pm.f( "Transparency", m_Transparency        );
} // SurfaceMaterial::Expose

bool SurfaceMaterial::IsEqual( const SNode* node ) const
{
    if (!Parent::IsEqual( node )) return false;
    SurfaceMaterial* pMtl = (SurfaceMaterial*)node;
    return (m_Ambient         == pMtl->m_Ambient      ) &&
           (m_Diffuse         == pMtl->m_Diffuse      ) &&
           (m_Specular        == pMtl->m_Specular     ) &&
           (m_Shininess       == pMtl->m_Shininess    ) &&
           (m_Transparency    == pMtl->m_Transparency );
} // SurfaceMaterial::IsEqual

/*****************************************************************************/
/*    Shader implementation
/*****************************************************************************/
std::stack<int> Shader::s_Overrides;
void Shader::PushOverride( int shID )
{
    s_Overrides.push( shID );  
} // Shader::PushOverride

void Shader::PopOverride()
{
    if (s_Overrides.size() > 0) s_Overrides.pop();  
} // Shader::PopOverride

void Shader::Render()
{
    if (m_Handle == -1) 
    {
        m_Handle = IRS->GetShaderID( GetName() );
        if (m_Handle == -1) m_Handle = 0;
    }
    
    if (s_Overrides.size() > 0)
    {
        IRS->SetShader( s_Overrides.top() );
    }
    else
    {
        IRS->SetShader( m_Handle );
    }

    IRS->SetShaderAutoVars();
} // Shader::Render

void Shader::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
} // Shader::Serialize

void Shader::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
} // Shader::Unserialize

void Shader::Update()
{
    IRS->ReloadShaders();
}

void Shader::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Shader", this );
    pm.p( "Script", &Shader::GetScriptFile, &Shader::SetScriptFile, "#shaderscript" );
} // Shader::Expose

void Shader::SetScriptFile( const char* file )
{
    char drive        [_MAX_DRIVE];
    char directory    [_MAX_DIR  ];
    char filename     [_MAX_PATH ];
    char ext          [_MAX_EXT  ];

    _splitpath( file, drive, directory, filename, ext );

    SetName( filename );
    m_Handle = -1;
} // Shader::SetScriptFile

/*****************************************************************************/
/*    BumpMatrix implementation
/*****************************************************************************/
void BumpMatrix::Render()
{
    IRS->SetBumpTM( m_LocalTM, m_Stage );
    SNode::Render();
} // BumpMatrix::Render

void BumpMatrix::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Stage;
} // BumpMatrix::Serialize

void BumpMatrix::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Stage;
} // BumpMatrix::Unserialize

void BumpMatrix::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "BumpMatrix", this );
    pm.f( "Stage",              m_Stage );
    pm.f( "e00",                m_LocalTM.e00 );
    pm.f( "e01",                m_LocalTM.e01 );
    pm.f( "e10",                m_LocalTM.e10 );
    pm.f( "e11",                m_LocalTM.e11 );
    pm.f( "e11",                m_LocalTM.e11 );
    pm.f( "LuminanceScale",     m_LocalTM.e20 );
    pm.f( "LuminanceOffset",    m_LocalTM.e21 );

} // BumpMatrix::Expose

