/*****************************************************************************/
/*	File:	sgVSPS.h
/*  Desc:	GPU vertex/pixel shader nodes
/*	Author:	Ruslan Shestopalyuk
/*	Date:	01.12.2003
/*****************************************************************************/
#ifndef __SGVSPS_H__
#define __SGVSPS_H__

namespace sg{
/*****************************************************************************/
/*	Class:	VertexShader
/*	Desc:	Vertex shader node
/*****************************************************************************/
class VertexShader : public Node
{
	VertexFormat				m_VF;
	int				            m_ShaderID;
	std::string					m_ScriptName;

public:
								VertexShader	();
	virtual						~VertexShader	();

	virtual void				Render			();
	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is );
	virtual void				Expose			( PropertyMap& pm );
	virtual void				Reload			();

	void						SetScriptName 	( const char* name );
	const char*					GetScriptName	() const { return m_ScriptName.c_str(); }

	void						SetVertexFormat	( VertexFormat vf );
	VertexFormat				GetVertexFormat	() const { return m_VF; }
	bool						IsLoaded		() const { return false; }

	void						DestroyShader	();

	NODE(VertexShader,Node,VSHA);
}; // VertexShader

/*****************************************************************************/
/*	Class:	VSConstant
/*	Desc:	Vertex shader constant
/*****************************************************************************/
class VSConstant : public Node
{
public:
					VSConstant();

	virtual void	Serialize	( OutStream& os ) const;
	virtual void	Unserialize	( InStream& is );
	virtual void	Expose		( PropertyMap& pm );	

	int				GetNRegisters() const { return m_NRegisters; }

	NODE(VSConstant,Node,VSCO);

protected:
	int			m_FirstRegister;
	int			m_NRegisters;
}; // VSConstant

/*****************************************************************************/
/*	Class:	VSVectorConstant
/*	Desc:	Vertex shader vector constant
/*****************************************************************************/
class VSVectorConstant : public VSConstant
{
public:
	/*****************************************************************************/
	/*	Enum:	PredefinedConstantType
	/*  Desc:	Types of stock constants
	/*****************************************************************************/
	enum PredefinedConstantType
	{
		pctUnknown		= 0,
		pctCustom		= 1,
		pctTime			= 2
	}; // enum PredefinedConstantType
					
					VSVectorConstant();
	virtual void	Render			();
	virtual void	Serialize		( OutStream& os ) const;
	virtual void	Unserialize		( InStream& is );
	virtual void	Expose			( PropertyMap& pm );	

	NODE(VSVectorConstant,VSConstant,VSVC);

private:
	Vector4D					m_Vector;
	PredefinedConstantType		m_CType[4];
	bool						m_bKeepNormalized;
}; // class VSVectorConstant

}; // namespace sg
ENUM( sg::VSVectorConstant::PredefinedConstantType, "PredefinedConstantType",
		en_val( sg::VSVectorConstant::pctCustom,	"Custom" ) << 
		en_val( sg::VSVectorConstant::pctTime,		"Time,sec" ) ); 
namespace sg{

/*****************************************************************************/
/*	Class:	VSMatrixConstant
/*	Desc:	Vertex shader matrix constant
/*****************************************************************************/
class VSMatrixConstant : public VSConstant
{
public:
	
	/*****************************************************************************/
	/*	Enum:	PredefinedMatrixType
	/*  Desc:	Types of stock matrix transforms
	/*****************************************************************************/
	enum PredefinedMatrixType
	{
		pmtUnknown		= 0,
		pmtWorldTM		= 1,
		pmtViewProjTM	= 2,
		pmtViewTM		= 3,
		pmtProjTM		= 4,
		pmtDirLight		= 5
	}; // enum PredefinedMatrixType

					VSMatrixConstant();
	virtual void	Render			();
	virtual void	Serialize		( OutStream& os ) const;
	virtual void	Unserialize		( InStream& is );
	virtual void	Expose			( PropertyMap& pm );	

	NODE(VSMatrixConstant,VSConstant,VSMC);

private:
	PredefinedMatrixType	m_MType;
	bool					m_bFullMatrix;
}; // class VSMatrixConstant

}; // namespace sg
ENUM( sg::VSMatrixConstant::PredefinedMatrixType, "PredefinedMatrixType",
		en_val( sg::VSMatrixConstant::pmtWorldTM,		"WorldTransform"	) << 
		en_val( sg::VSMatrixConstant::pmtViewProjTM,	"ViewProjTransform" ) << 
		en_val( sg::VSMatrixConstant::pmtViewTM,		"ViewTransform"		) << 
		en_val( sg::VSMatrixConstant::pmtProjTM,		"ProjTransform"		) <<
		en_val( sg::VSMatrixConstant::pmtDirLight,		"DirLight"			) ); 
namespace sg{

/*****************************************************************************/
/*	Class:	VSConstBlock
/*	Desc:	Set of the vertex shader constants
/*****************************************************************************/
class VSConstBlock : public Node
{
public:
	virtual void				Render();
	virtual void				Serialize( OutStream& os ) const;
	virtual void				Unserialize( InStream& is );
	virtual void				Expose		( PropertyMap& pm );	

	NODE(VSConstBlock,Node,VSCB);
}; // VSConstBlock

/*****************************************************************************/
/*	Class:	VertexShaderManager
/*	Desc:	Manages vertex shaders
/*****************************************************************************/
class VertexShaderManager : public Node, public PSingleton<VertexShaderManager>
{
public:
								VertexShaderManager();
	virtual void				Expose		( PropertyMap& pm );	
	DWORD						GetShaderID ( const char* fname );
	
	NODE(VertexShaderManager,Node,VSMG);
}; // VertexShaderManager

/*****************************************************************************/
/*	Class:	PixelShader
/*	Desc:	Pixel shader node
/*****************************************************************************/
class PixelShader : public AssetNode
{
	DWORD						m_Handle;	//  device handle
public:
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is );
	virtual void				Expose		( PropertyMap& pm );	

	NODE(PixelShader,Node,PSHA);
}; // PixelShader

/*****************************************************************************/
/*	Class:	PSConstBlock
/*	Desc:	Set of the pixel shader constants
/*****************************************************************************/
class PSConstBlock : public AssetNode
{
public:
	virtual void				Render		();
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is );
	virtual void				Expose		( PropertyMap& pm );	

	NODE(PSConstBlock,Node,PSCB);
}; // PSConstBlock

/*****************************************************************************/
/*	Class:	PixelShaderManager
/*	Desc:	Manages pixel shaders
/*****************************************************************************/
class PixelShaderManager : public Node, public PSingleton<PixelShaderManager>
{
public:
								PixelShaderManager	();
	virtual void				Expose				( PropertyMap& pm );	


	NODE(PixelShaderManager,Node,PSMG);
}; // PixelShaderManager

}; // namespace sg

#endif // __SGVSPS_H__