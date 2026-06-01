/*****************************************************************************/
/*    File:    sgSprite.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    09-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "kCache.h"
#include "kHash.hpp"
#include "kResource.h"
#include "kIOHelpers.h"

#include "mMath2D.h"
#include "mAlgo.h"

#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgController.h"

#include "sgDummy.h"
#include "sgGeometry.h"
#include "sgSprite.h"

#ifndef _INLINES
#include "sgSprite.inl"
#endif // _INLINES

IMPLEMENT_CLASS( ScreenSprite );
IMPLEMENT_CLASS( WorldSprite );

/*****************************************************************************/
/*    ScreenSprite implementation
/*****************************************************************************/
void ScreenSprite::Render()
{
    if (m_Handle == c_BadHandle)
    {
        Precache();
    }
    
    if (m_Handle != c_BadHandle)
    {
        ISM->SetScale( GetScale() );
        ISM->SetCurrentDiffuse( m_Color );
        ISM->DrawSprite( m_Handle, m_FrameIdx, pos, false );
    }
} // ScreenSprite::Render

bool ScreenSprite::Precache()
{
    if(!GetPackageName()) return false;
    m_Handle = ISM->GetPackageID( GetPackageName() );
    bool allOk =  ISM->LoadPackage( m_Handle );
    return allOk;
} // ScreenSprite::Precache

void ScreenSprite::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_FrameIdx << m_FileName << m_Color;
} // ScreenSprite::Serialize

void ScreenSprite::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_FrameIdx >> m_FileName >> m_Color;
    m_Handle = c_BadHandle;
} // ScreenSprite::Unserialize

void ScreenSprite::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "ScreenSprite", this );
    pm.p( "GPName", &ScreenSprite::GetPackageName, &ScreenSprite::SetPackageName, "file" );
    pm.f( "FrameIndex",        m_FrameIdx            );
    pm.f( "Color",            m_Color, "color"    );
    pm.p( "Handle", &ScreenSprite::GetGPHandle            );
    pm.p( "FrameWidth", &ScreenSprite::GetFrameWidth        );
    pm.p( "FrameHeight", &ScreenSprite::GetFrameHeight        );
} // ScreenSprite::Expose

Rct    ScreenSprite::GetBounds()
{
    if (m_Handle == c_BadHandle) Precache();
    Rct rct;
    ISM->GetBoundFrame( m_Handle, m_FrameIdx, rct );
    return rct;
} // ScreenSprite::GetBounds

int    ScreenSprite::GetFrameWidth() const
{
    return ISM->GetFrameWidth( m_Handle, m_FrameIdx );
} // ScreenSprite::GetFrameWidth

int    ScreenSprite::GetFrameHeight() const
{
    return ISM->GetFrameHeight( m_Handle, m_FrameIdx );
} // ScreenSprite::GetFrameHeight

void ScreenSprite::SetPackageName( const char* name )
{ 
    if (!name) return;
    char buf[_MAX_PATH];
    strcpy( buf, name );
    //ToRelativePath( buf, _MAX_PATH );

    char* cExt = strrchr( buf, '.' );
    if (cExt) *cExt = 0;
    m_FileName    = buf;
    m_Handle    = c_BadHandle;
} // ScreenSprite::SetPackageName

/*****************************************************************************/
/*    WorldSprite implementation
/*****************************************************************************/
WorldSprite::WorldSprite()
{
    m_Handle            = -1;
    m_RealFrame        = 0;
    m_AnimFrame        = 0;
    m_NDirections    = 16;
    m_bHalfCircle    = false;
    m_bBillboard    = false;
    m_Pivot         = Vector3D::null;
    m_bAnimate      = false;
    m_Scale            = 1.0f;
    m_Direction        = 0;
    m_Color            = 0xFFFFFFFF;
    m_ShaderID        = -1;

    m_MaxH            = 0.0f;
    m_MaxHalfW        = 0.0f;

    m_PlayerColor    = 0xFFFFFFFF;
    m_LOD            = 0;

    SetTransform    ( Matrix4D::identity );
    SetInitialTM    ( Matrix4D::identity );
} // WorldSprite::WorldSprite

void WorldSprite::Render()
{
    s_TMStack.Push( m_LocalTM );
    m_WorldTM = s_TMStack.Top();
    
    if (m_Handle == c_BadHandle)
    {
        Precache();
    }

    if (m_Handle != c_BadHandle)
    {
        Matrix4D tmatr = m_bBillboard ? GetImpostorMatrix() : m_WorldTM;
        if (!m_bBillboard)
        {
            Matrix4D m = tmatr;
            if (m_Scale != 1.0f)
            {
                Matrix4D sc;
                sc.scaling( m_Scale );
                m.mulLeft( sc );
            }

            Matrix4D pivTM;
            pivTM.translation( -m_Pivot.x, -m_Pivot.y, -m_Pivot.z );
            m.mulLeft( pivTM );
            
            Matrix4D rot;
            rot.rotation( Vector3D::oX, -c_HalfPI + c_PId6 );

            const Matrix4D c_SkewTM( 1.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f, 0.0f,
                                     0.0f, -0.5f, c_CosPId6, 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f );
            m.mulLeft( rot );
            //Matrix4D is; is.inverse( c_SkewTM );
            //m *= rot;

            tmatr = m;
            if (DoDrawGizmo())
            {
                AABoundBox aabb; 
                ISM->GetAABB( m_Handle, m_RealFrame, aabb, m_PlayerColor );
                
                IRS->SetWorldTM( tmatr );
                DrawAABB( aabb, 0, 0xFFFF0000 );
                rsFlushLines3D();
            }

        }
        
        m_RealFrame = m_AnimFrame * m_NDirections + abs( m_Direction );

        ISM->SetCurrentDiffuse    ( m_Color );
        ISM->SetLOD                ( m_LOD );
        IRS->SetWorldTM( Matrix4D::identity );
        if (m_ShaderID != -1) ISM->SetCurrentShader( m_ShaderID );
        
        if (ISM->HasColorData( m_Handle ))
        {
            IRS->SetTextureFactor( m_Color );
            ISM->DrawNWSprite( m_Handle, m_RealFrame, tmatr, m_PlayerColor );
            ISM->Flush();
        }
        else
        {
            ISM->DrawWSprite( m_Handle, m_RealFrame, tmatr, m_PlayerColor );
        }

        if (m_bAnimate) SetAnimFrame( GetTickCount() / 50 );
    }

    s_TMStack.Pop();
} // WorldSprite::Render

void WorldSprite::SetShaderID( const char* name )
{
    char drive        [_MAX_DRIVE];
    char directory    [_MAX_DIR  ];
    char filename    [_MAX_PATH ];
    char ext        [_MAX_EXT  ];

    _splitpath( name, drive, directory, filename, ext );
    m_ShaderID = IRS->GetShaderID( filename );
} // WorldSprite::SetShaderID

void WorldSprite::Serialize    ( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_RealFrame << m_AnimFrame << m_NDirections << 
        m_bHalfCircle << m_bBillboard 
        << m_GPName << m_Scale << m_Pivot << m_bAnimate;
}

void WorldSprite::Unserialize    ( InStream&     is )
{
    Parent::Unserialize( is );
    is >> m_RealFrame >> m_AnimFrame >> m_NDirections >> m_bHalfCircle >> m_bBillboard >>
        m_GPName >> m_Scale >> m_Pivot >> m_bAnimate;
}

void WorldSprite::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "WorldSprite", this );
    pm.p( "GPName", &WorldSprite::GetPackageName, &WorldSprite::SetPackageName, "file" );
    pm.f( "GPHandle",        m_Handle                    );
    pm.f( "RealFrame",        m_RealFrame                    );
    pm.p( "AnimFrame", &WorldSprite::GetAnimFrame, &WorldSprite::SetAnimFrame    );
    pm.p( "FrameWidth", &WorldSprite::GetFrameWidth                );
    pm.p( "FrameHeight", &WorldSprite::GetFrameHeight                );
    pm.f( "NDir",            m_NDirections                );
    pm.f( "HalfCircle",        m_bHalfCircle                );
    pm.f( "Billboard",        m_bBillboard                );
    pm.f( "Animate",        m_bAnimate                    );
    pm.f( "PivotX",            m_Pivot.x                    );
    pm.f( "PivotY",            m_Pivot.y                    );
    pm.f( "PivotZ",            m_Pivot.z                    );
    pm.f( "Scale",            m_Scale                        );
    pm.f( "Dir",            m_Direction                    );
    pm.f( "Color",            m_Color, "color"            );
    pm.f( "LOD",            m_LOD                        );
    pm.f( "PlayerColor",    m_PlayerColor, "color"        );
    pm.f( "MaxH",            m_MaxH,        NULL, true        );
    pm.f( "MaxHalfW",        m_MaxHalfW, NULL, true        );
    pm.p( "Shader", &WorldSprite::GetShaderID, &WorldSprite::SetShaderID, "#shaderscript" );
} // WorldSprite::Expose

int    WorldSprite::GetFrameWidth() const
{
    return ISM->GetFrameWidth( m_Handle, m_RealFrame );
} // WorldSprite::GetFrameWidth

int    WorldSprite::GetFrameHeight() const
{
    return ISM->GetFrameHeight( m_Handle, m_RealFrame );
} // WorldSprite::GetFrameHeight

bool WorldSprite::Precache()
{
    if(!GetPackageName()) return false;
    m_Handle = ISM->GetPackageID( GetPackageName() );
    bool allOk =  ISM->LoadPackage( m_Handle );
    Rct rct;
    ISM->GetBoundFrame( m_Handle, m_RealFrame, rct );
    m_MaxHalfW = rct.w * 0.5;
    m_MaxH = rct.h;
    return allOk;
} // WorldSprite::Precache

int    WorldSprite::GetNFrames()
{
    if (m_Handle == c_BadHandle)
    {
        Precache();
    }
    return ISM->GetNFrames( m_Handle );
} // WorldSprite::GetNFrames

void WorldSprite::SetPackageName( const char* _gpName )
{ 
    if (!_gpName) return;
    char buf[_MAX_PATH];
    strcpy( buf, _gpName );
    //ToRelativePath( buf, _MAX_PATH );

    char* cExt = strrchr( buf, '.' );
    if (cExt) *cExt = 0;
    m_Handle = c_BadHandle;
    m_GPName = buf;
} // ScreenSprite::SetPackageName





