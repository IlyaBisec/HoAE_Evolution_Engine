/*****************************************************************************/
/*	File:	vMaterial.cpp
/*	Desc:	Mesh interface implementation
/*	Author:	Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "IShader.h"
#include "vMaterial.h"

/*****************************************************************************/
/*  Material implementation  
/*****************************************************************************/
Material::Material()
{
    m_ScriptName    = "vertex_lit";
    m_ShaderID      = -1;
    m_NPasses       = 1;
    m_bEnabled      = true;
    m_ID            = -1;
    for (int i = 0; i < c_MaxTextureStages; i++) m_TexID[i] = -1;
} // Material::Material

struct ShaderFValFunctor
{
    IShader*        m_pShader;
    int             m_ValID;

    typedef float   PropertyType;

    ShaderFValFunctor( IShader* pShader, int valID ) : m_pShader(pShader), m_ValID(valID){}
    bool           Get( float& val ) const
    {
        return m_pShader->GetShaderVar( m_ValID, val );
    }
    bool            Set( float val )
    {
        return m_pShader->SetShaderVar( m_ValID, val );
    }

}; // struct ShaderFValFunctor

void Material::Expose( PropertyMap& pm )
{
    pm.start( "Material", this );
    pm.p( "NodeType",   ClassName );
    pm.f( "Enabled",    m_bEnabled );
    pm.p( "Script",     GetScriptName, SetScriptName, "#shaderscript" );
    pm.p( "Texture[0]", GetTex0, SetTex0, "#texture" );
    pm.p( "Texture[1]", GetTex1, SetTex1, "#texture" );
    pm.p( "Texture[2]", GetTex2, SetTex2, "#texture" );
    pm.p( "Texture[3]", GetTex3, SetTex3, "#texture" );

    //  expose shader script parameters
    if (m_ShaderID == -1) m_ShaderID = IRS->GetShaderID( GetScriptName() );
    if (m_ShaderID == -1) return;

    IShader* pShader = IRS->GetShader( m_ShaderID );
    if (!pShader) return;
    int nC = pShader->GetNShaderVars();
    for (int i = 0; i < nC; i++)
    {
        float fval = 0.0f;
        if (pShader->IsAutoVar( i ) || !pShader->GetShaderVar( i, fval )) continue;
        pm.r( pShader->GetVariableName( i ), ShaderFValFunctor( pShader, i ) );
    }
} // Material::Expose

void Material::SetScriptName( const char* name ) 
{ 
    m_ScriptName = name; 
    m_ShaderID = IRS->GetShaderID ( m_ScriptName.c_str() );
} // Material::SetScriptName

void Material::SetTex( int stage, const char* name )
{
    m_Texture[stage] = name;
    m_TexID[stage] = IRS->GetTextureID( name );
} // Material::SetTex

void Material::Validate()
{
    for (int i = 0; i < c_MaxTextureStages; i++)
    {
        m_TexID[i] = IRS->GetTextureID( m_Texture[i].c_str() );
    }
    m_ShaderID = IRS->GetShaderID ( m_ScriptName.c_str() );
} // Material::Validate

bool Material::operator ==( const Material& sh ) const
{
    if (m_ScriptName != sh.m_ScriptName) return false;
    for (int i = 0; i < c_MaxTextureStages; i++)
    {
        if (m_Texture[i] != sh.m_Texture[i]) return false;
    }
    return true;
} // Material::operator

void Material::Render( int pass )
{
    if (!m_bEnabled) return;
    IRS->SetShader( m_ShaderID, pass );
    IRS->SetShaderAutoVars();
    if (m_TexID[0] >= 0) IRS->SetTexture( m_TexID[0], 0 );
    if (m_TexID[1] >= 0) IRS->SetTexture( m_TexID[1], 1 );
    if (m_TexID[2] >= 0) IRS->SetTexture( m_TexID[2], 2 );
    if (m_TexID[3] >= 0) IRS->SetTexture( m_TexID[3], 3 );
} // Material::Render



