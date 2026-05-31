/*****************************************************************/
/*  File:   rsShaderDX.h
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __RSSHADERDX_H__
#define __RSSHADERDX_H__

#include "kHash.hpp"

const int c_MaxShaderTextureStages	= 2;
const int c_MaxLights				= 8;

/*****************************************************************************/
/*	Class:	ShaderState
/*****************************************************************************/
class ShaderState
{
	struct Vec4
	{
		float x;
		float y;
		float z;
		float w;
	}; // struct Vec4

	DWORD			tableIdx;
	BYTE			stage;
	DWORD			value;
public:
	
	ShaderState() : value(0), tableIdx(0), stage(0) {} 
	~ShaderState();

	void			setValue( DWORD val, BYTE stageIdx = 0xFF ) 
					{ 
						value	= val; 
						stage	= stageIdx;
					}
	bool			less( const ShaderState& state );
	bool			equal( const ShaderState& state );
	bool			equalValue( const ShaderState& state );
	void			dump( FILE* fp );
	void			copy( const ShaderState& st );
	bool			apply( DXDevice* pDevice );
	void			setValueFromDataPtr( void* data );
	void			setValueToDataPtr( void* data, int stride = 0 );

protected:
	void			order();

	void			getFieldValue( void* where, int bufSize );

	static			DXMaterial		curMtl;
	static			DXLight			curLight[c_MaxLights];
	static			bool			lchanged[c_MaxLights];
	static			bool			mchanged;


private:
	friend class Shader;

	static int cmpFunc( const void* pState1, const void* pState2 );
}; // class ShaderState

const int c_MaxShaderIDLen = 128;
const int c_MaxStates		= 512; 
#define T_DWORD_HEX (void*)0x00000001
#define T_FLOAT		(void*)0x00000002
#define T_VECTOR	(void*)0x00000003

enum ShaderStateType
{
	sstUnknown			= 0,
	sstRenderState		= 1,
	sstTexStageState	= 2,
	sstMaterialField	= 3,
	sstLightField		= 4,
	sstLightEnable		= 5
}; // enum ShaderStateType

/*****************************************************************************/
/*	Class:	ShaderStateDesc
/*	Desc:	Single shader state attributes
/*****************************************************************************/
class ShaderStateDesc
{
	char			name[c_MaxShaderIDLen];
	void*			valType;
	ShaderStateType stateType;
	int				maxValues;
	DWORD			stateDevID;

public:
	ShaderStateDesc( const char* sname, void* type );
	ShaderStateDesc();


	void		setName( const char* sname );
	const char* getName() const { return name; }
	void		setValType( void* type );
	void		setStateType( ShaderStateType type );
	bool		getValStr( DWORD val, char* str, int maxlen );
	bool		hasName( const char* sname ) { return !strcmp( name, sname ); }

private:
	friend class Shader;
	friend class ShaderState;

}; // class ShaderStateDesc

class ShaderCache;

#define NO_STATE_BLOCK 0xFFFFFFFF

#define STRIDE(A,B)		((BYTE*)(&(A.B)) - (BYTE*)(&(A)))

/*****************************************************************************/
/*	Class:	Shader
/*****************************************************************************/
class Shader
{
private:
	double					execTime;			//  time of execution, in mks
	
	ShaderState				state[c_MaxStates];
	
	ShaderState*			stateOrder[c_MaxStates];
	bool					correctOrder;
	int						nStates;

	DXEffect*				iEffect;

	static ShaderStateDesc	stateDesc[c_MaxStates];
	static DWORD			nStateDesc;

protected:
	char					name[c_MaxShaderIDLen];
	int						nameLen;
	int						baseShader;
	int						id;

	DWORD					stateBlockID;
	DWORD					vshID;

public:
							Shader		();
							Shader		( const char* shName, int baseID );
	void					Sub			( Shader& a, Shader& b );
	void					Supp		( Shader& s );

	_inl bool				Apply		( DXDevice* pDevice, ShaderCache* shaderCache = 0 );

	void					Reset		();
	void					Dump		( FILE* fp, ShaderCache* cache = 0 );
	bool					Load		( const char* rootDirectory, DXDevice* pDevice );
	void					SetID		( int shId ) { id = shId; }

	bool					CreateFromCurrentDeviceState( DXDevice* pDevice );
	void					SetVertexShaderID			( int id ) { vshID = id; }

	void					InvalidateDeviceObjects		( DXDevice* pDevice );
	void					RestoreDeviceObjects		( DXDevice* pDevice );

							
protected:					
	static void				initStatesTable();
	static void				addRenderStateDesc( const char* stname, D3DRENDERSTATETYPE state, void* valType );
	static void				addTexStageStateDesc( int idx, const char* stname, 
													D3DTEXTURESTAGESTATETYPE state, void* valType );
	static void				addMaterialFieldDesc( const char* stname, int fieldStride, void* valType );
	static void				addLightFieldDesc( const char* stname, int lightIdx, 
												int fieldStride, void* valType );
	void					addState( const ShaderState& st );
	bool					getRenderState( DXDevice* dev, const char* stName );
	bool					getTextureStageState(	DXDevice* dev, int stage, 
													const char* stName );
	bool					getLightField	( const char* fieldName, int lightIdx, void* dataPtr );
	bool					getMaterialField( const char* fieldName, void* dataPtr );

	ShaderState*			getCreateState( const char* stateName );

	void					order();

private:
	friend class			ShaderState;
	friend class			ShaderCache;
	friend class			ShaderStateDescr;

	static const char*		getStateName( const ShaderState& state );
	static bool				getValStr( const ShaderState& state, char* str, int maxlen );
	static int				getStateDescrTableIdx( const char* state );

	static double			perfCounterRes;					

	static const char		errstr[];	
}; // class Shader

/*****************************************************************************/
/*	Class:	HashedShader
/*	Desc:	Shader as hash table element
/*****************************************************************************/
class HashedShader : public Shader
{
public:
	HashedShader() : Shader() {}
	HashedShader( const char* shName, int baseID ) : Shader( shName, baseID ) {}

	//  we mix shader name and base shader in hash function
	unsigned int hash() const
	{
		unsigned char* s = (unsigned char*) &name;
		unsigned int h = 0;
		for (int i = 0; i < nameLen; i++)
		{
			h = (h * 729 + (*s) * 37) ^ (h >> 1);
			s++;
		}

		int nB = sizeof( baseShader );
		s = (unsigned char*) &baseShader;
		for (i = 0; i < nB; i++)
		{
			h = (h * 729 + (*s) * 37) ^ (h >> 1);
			s++;
		}

		return h;
	} // hash

	bool equal( const HashedShader& el )
	{
		return (!strcmp( name, el.name ) && baseShader == el.baseShader);
	} // equal

	void copy( const HashedShader& el )
	{
		memcpy( this, &el, sizeof( HashedShader ) );
	} // copy

}; // class HashedShader

const int c_MaxZeroLevelShaders			= 128;
const int c_ShaderCacheHashTableSize	= 2048;
const int c_ShaderCacheHashPoolSize		= 512;
/*****************************************************************************/
/*	Class:	ShaderCache
/*	Desc:	Performs caching of shader render states
/*****************************************************************************/
class ShaderCache
{
	DXDevice*		pDevice;
public:
	ShaderCache();
	~ShaderCache();

	bool			Init( DXDevice* dev );
	void			Shut();

	_inl bool		SetShader			( int ID );
	
	int				GetShaderID			( const char* name );
	void			Reset				();
	void			ReloadShaders		();

	bool			InvalidateDeviceObjects();
	bool			RestoreDeviceObjects();
	void			SetDevice( DXDevice* pDev ) { pDevice = pDev; }

	void			Dump( FILE* fp );
	void			DumpShader( int shaderID, FILE* fp );

protected:
	int				getShaderID( const char* name, int base );
	HashedShader&	getShader( int idx );

	void			SetInitialState();

private:
	int					currentShader;

	Hash<	HashedShader, 
			c_ShaderCacheHashTableSize, 
			c_ShaderCacheHashPoolSize>	hash;
	
	char				root[_MAX_PATH];
	HashedShader*		table[c_MaxZeroLevelShaders][c_MaxZeroLevelShaders];

	static int			cmpShadersByExecTime( const void *s1, const void *s2 );
	friend				class Shader;
}; // class ShaderCache

#ifdef _INLINES
#include "rsShaderDX.inl"
#endif // _INLINES

#endif // __RSSHADERDX_H__