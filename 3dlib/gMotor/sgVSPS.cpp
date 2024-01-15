/*****************************************************************************/
/*	File:	sgVSPS.cpp
/*  Desc:	GPU vertex/pixel shader nodes
/*	Author:	Ruslan Shestopalyuk
/*	Date:	01.12.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgAssetNode.h"
#include "kIOHelpers.h"
#include "kEnumTraits.h"
#include "sgMovable.h"
#include "sgCamera.h"
#include "sgLight.h"
#include "sgVSPS.h"


_inl OutStream&	operator<<( OutStream& os, VertexFormat vf )
{
	WORD bvf = vf;
	os << bvf;
	return os;
}

_inl InStream&	operator>>( InStream& is, VertexFormat& vf )
{
	WORD bvf = 0;
	is >> bvf;
	vf = (VertexFormat)bvf;
	return is;
}

BEGIN_NAMESPACE( sg )
/*****************************************************************************/
/*	VertexShader implementation
/*****************************************************************************/
VertexShader::VertexShader() : m_VF(vfUnknown)//, m_iVShader(NULL)
{

}

void VertexShader::DestroyShader()
{
	//if (m_iVShader) m_iVShader->Reset();
	//delete m_iVShader;
	//m_iVShader = NULL;
}

VertexShader::~VertexShader()
{
	// FIXME - wrong thing to do after D3D device is destroyed...
	//DestroyShader();
}

void VertexShader::SetScriptName( const char* name )
{
	m_ScriptName = name;
	//m_iVShader->Reset();
	//m_iVShader->Create( name );
}

void VertexShader::SetVertexFormat( VertexFormat vf )
{
	//if (!m_iVShader) m_iVShader = IRS->CreateVertexShader();
	m_VF = vf;
	//if (m_iVShader) m_iVShader->SetVertexFormat( vf );
}

void VertexShader::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_ScriptName << m_VF;
} // VertexShader::Serialize

void VertexShader::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_ScriptName >> m_VF;
} // VertexShader::Unserialize

void VertexShader::Render()
{
	//if (!m_iVShader)
	{
		//m_iVShader = IRS->CreateVertexShader();
		//m_iVShader->SetVertexFormat( m_VF );
		//m_iVShader->Create( m_ScriptName.c_str() );
	}

	//if (!m_iVShader) return;
	//m_iVShader->Apply();
} // VertexShader::Render

void VertexShader::Reload()
{
	DestroyShader();
	//m_iVShader = IRS->CreateVertexShader();
	//m_iVShader->SetVertexFormat( m_VF );
	//m_iVShader->Create( m_ScriptName.c_str() );
} // VertexShader::Reload

void VertexShader::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "VertexShader", this );
	pm.p( "VertexFormat", GetVertexFormat, SetVertexFormat );
	pm.p( "Script", GetScriptName, SetScriptName, "file" );
	pm.p( "Loaded", IsLoaded );
	pm.m( "Reload", Reload );
} // VertexShader::Expose

/*****************************************************************************/
/*	VSConstBlock implementation
/*****************************************************************************/
void VSConstBlock::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // VSConstBlock::Serialize

void VSConstBlock::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
} // VSConstBlock::Unserialize

void VSConstBlock::Render()
{
}

void VSConstBlock::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "VSConstBlock", this );
}

/*****************************************************************************/
/*	VSConstant implementation
/*****************************************************************************/
VSConstant::VSConstant() : m_FirstRegister(0), m_NRegisters(0) 
{
}

void VSConstant::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_FirstRegister << m_NRegisters;
}

void VSConstant::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_FirstRegister >> m_NRegisters;
}

void VSConstant::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "VSConstant", this );
	pm.f( "RegisterIndex", m_FirstRegister );
	pm.p( "NumRegisters", GetNRegisters );
}

/*****************************************************************************/
/*	VSVectorConstant implementation
/*****************************************************************************/
VSVectorConstant::VSVectorConstant()
{
	m_Vector = Vector4D::null;
	m_CType[0] = m_CType[1] = m_CType[2] = m_CType[3] = pctCustom;
	m_bKeepNormalized = false;
}

void VSVectorConstant::Render()
{
	Vector4D v( m_Vector );
	
	for (int i = 0; i < 4; i++)
	{
		switch (m_CType[i])
		{
			case pctTime: v.x = float( GetTickCount() )*0.001f; break;
		}
	}

	//IRS->SetVSConstant( m_FirstRegister, v );
} // VSVectorConstant::Render

void VSVectorConstant::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Vector << Enum2Byte( m_CType );
}

void VSVectorConstant::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_Vector >> Enum2Byte( m_CType );
} // VSVectorConstant::Unserialize

void VSVectorConstant::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "VSVectorConstant", this );
	pm.f( "KeepNormalized",	m_bKeepNormalized );
	pm.f( "v.x Type", 		m_CType[0] );
	pm.f( "v.y Type", 		m_CType[1] );
	pm.f( "v.z Type", 		m_CType[2] );
	pm.f( "v.w Type", 		m_CType[3] );	
	pm.f( "v.x", 				m_Vector.x );
	pm.f( "v.y", 				m_Vector.y );
	pm.f( "v.z", 				m_Vector.z );
	pm.f( "v.w", 				m_Vector.w );
} // VSVectorConstant::Expose

/*****************************************************************************/
/*	VSMatrixConstant implementation
/*****************************************************************************/
VSMatrixConstant::VSMatrixConstant() : m_bFullMatrix(true), m_MType(pmtWorldTM)
{
}

void VSMatrixConstant::Render()
{
	Matrix4D m = Matrix4D::identity;
	switch (m_MType)
	{
	case pmtWorldTM:
		{
			m = TransformNode::TMStackTop();
		};break;
	case pmtViewProjTM:	
		{
			m = BaseCamera::GetActiveCamera()->GetViewM();
			m *= BaseCamera::GetActiveCamera()->GetProjM();
		};break;

	case pmtViewTM:	
		{
			m = BaseCamera::GetActiveCamera()->GetViewM();
		};break;

	case pmtProjTM:		
		{
			m = BaseCamera::GetActiveCamera()->GetProjM();
		};break;
	case pmtDirLight:		
		{
			m = BaseCamera::GetActiveCamera()->GetProjM();
		};break;
	}
	//IRS->SetVSConstant( m_FirstRegister, m, m_bFullMatrix );
} // VSMatrixConstant::Render

void VSMatrixConstant::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << Enum2Byte( m_MType );
}

void VSMatrixConstant::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> Enum2Byte( m_MType );
}

void VSMatrixConstant::Expose( PropertyMap& pm )
{	
	pm.start<Parent>( "VSMatrixConstant", this );
	pm.f( "MatrixType", m_MType );
}

/*****************************************************************************/
/*	VertexShaderManager implementation
/*****************************************************************************/
VertexShaderManager::VertexShaderManager() 
{ 
	SetName( "VertexShaderManager" ); 
}

void VertexShaderManager::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "VertexShaderManager", this );
}

DWORD VertexShaderManager::GetShaderID( const char* fname )
{
	VertexShader* pShader = NodePool::GetNodeByName<VertexShader>( fname );
	if (pShader) return pShader->GetID();
	pShader = AddChild<VertexShader>( fname );
	return pShader->GetID();
} // VertexShaderManager::GetShaderID

/*****************************************************************************/
/*	PixelShader implementation
/*****************************************************************************/
void PixelShader::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // PixelShader::Serialize

void PixelShader::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
} // PixelShader::Unserialize

void PixelShader::Render()
{
}

void PixelShader::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PixelShader", this );
}

/*****************************************************************************/
/*	PSConstBlock implementation
/*****************************************************************************/
void PSConstBlock::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // PSConstBlock::Serialize

void PSConstBlock::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
} // PSConstBlock::Unserialize

void PSConstBlock::Render()
{
}

void PSConstBlock::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PSConstBlock", this );
}

/*****************************************************************************/
/*	PixelShaderManager implementation
/*****************************************************************************/
PixelShaderManager::PixelShaderManager() 
{ 
	SetName( "PixelShaderManager" ); 
}

void PixelShaderManager::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PixelShaderManager", this );
}

END_NAMESPACE( sg )