/*****************************************************************************/
/*	File:	d3dShaderFX.h
/*  Desc:	ShaderFX interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#ifndef __D3DSHADERFX_H__
#define __D3DSHADERFX_H__
#include "IShader.h"
#include "IResourceManager.h"
#include "kTimer.h"

///#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 
 
/*****************************************************************************/
/*  Union: ShaderVarValue
/*  Desc: 
/*****************************************************************************/
union ShaderVarValue 
{
    int                 nVal;       
    bool                bVal;       
    float               fVal;       
    float               vVal[4];    
    float               mVal[16];   
}; // union ShaderVarValue

/*****************************************************************************/
/*  Struct: ShaderFXVariable
/*  Desc:   
/*****************************************************************************/
struct ShaderFXVariable
{
    std::string         m_Name;
    ShaderVarType       m_Type;
    ShaderVarValue      m_Value;
    D3DXHANDLE          m_Handle;
    bool                m_bAuto;
}; // struct ShaderFXVar

struct ShaderFXTechnique
{
    std::string         m_Name;
    D3DXHANDLE          m_Handle;
    int                 m_NPasses;
    int                 m_NAnnotations;
}; // struct ShaderFXTechnique

struct ShaderFXAutoVar
{
    int                 m_Idx;
    AutoShaderVariable  m_Type;
}; // struct ShaderFXAutoVar

class ShaderFX;
/*****************************************************************************/
/*  Class:  FXIncluder
/*  Desc:   Used for providing shader file including to effect compiler
/*****************************************************************************/
class FXIncluder : public ID3DXInclude
{
    DWORD       m_ResID;
    BYTE*       m_pData;
    ShaderFX*   m_pCurShader;

public:
    FXIncluder() : m_ResID(0xFFFFFFFF), m_pData(NULL), m_pCurShader(NULL){}
    virtual HRESULT __stdcall Open( D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, 
                                    LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes );
    virtual HRESULT __stdcall Close( LPCVOID pData );

    void SetCurShader( ShaderFX* pShader );
}; // class FXIncluder

/*****************************************************************************/
/*  Class:  ShaderFX
/*  Desc:   Direct3D9 .fx shader class 
/*****************************************************************************/
class ShaderFX : public IShader, public IResource
{
    ID3DXEffect*                    m_pEffect;          //  pointer to the d3dx effect interface
    std::vector<ShaderFXVariable>   m_Variables;        //  variables list
    std::vector<ShaderFXAutoVar>    m_AutoVars;         //  automatic variables references
    std::vector<ShaderFXTechnique>  m_Techniques;       //  techniques list

    std::string                     m_Name;             //  effect name
    std::string                     m_FileName;         //  effect file name
    int                             m_ID;               //  effect ID in the shader manager
    int                             m_ActiveTech;       //  currently aclive technique
    static Timer                    s_Timer;

public:
                                ShaderFX        ( IDirect3DDevice9* pDevice );
                                ShaderFX        ();
                                ~ShaderFX       ();

    virtual const char*         GetName         () const { return m_Name.c_str(); }
    virtual int                 GetID           () const { return m_ID; }
    virtual void                SetID           ( int id ) { m_ID = id; }

    virtual int                 GetNTech        () const;
    virtual bool                SetActiveTech   ( int techID ) { m_ActiveTech = techID; return false; }
    virtual const char*         GetTechName     ( int techID ) const;
    virtual bool                IsTechValid     ( int techID ) const;

    virtual int                 GetNPasses      ( int techID = 0 ) const;
    virtual int                 GetNShaderVars  () const { return m_Variables.size(); }
    virtual const char*         GetVariableName ( int cID ) const;
    virtual int                 GetShaderVarID  ( const char* cName );

    virtual bool                SetShaderVar    ( int cID, bool val );
    virtual bool                SetShaderVar    ( int cID, float val );
    virtual bool                SetShaderVar    ( int cID, int val );
    virtual bool                SetShaderVar    ( int cID, const Matrix4D& val );
    virtual bool                SetShaderVar    ( int cID, const Vector4D& val );
    virtual bool                SetShaderVar    ( int cID, const Vector3D& val );
	virtual bool				SetShaderVar	( int cID, const Vector4D* val, int count );

    virtual bool                GetShaderVar    ( int cID, bool& val );
    virtual bool                GetShaderVar    ( int cID, float& val );
    virtual bool                GetShaderVar    ( int cID, int& val );
    virtual bool                GetShaderVar    ( int cID, Matrix4D& val );
    virtual bool                GetShaderVar    ( int cID, Vector4D& val );
    virtual bool                GetShaderVar    ( int cID, Vector3D& val );

    virtual void                SetAutoVars     ();
    virtual bool                IsAutoVar       ( int cID );

    virtual bool                Load            ( const char* fName );
    
    virtual bool                Begin           ();
    virtual bool                BeginPass       ( int passID );
    virtual bool                EndPass         ();
    virtual bool                End             ();
    virtual bool                Reload          ();
    virtual void                SetName         ( const char* name ) { m_Name = name; }

    virtual void                DeleteDeviceObjects     ();
    virtual void                InvalidateDeviceObjects ();
    virtual void                RestoreDeviceObjects    ();

protected:
    void                        EnumerateVariables      ();
    void                        EnumerateTechniques     ();
    virtual bool                LoadFromFile            ( const char* fName );
    virtual bool                LoadFromMemory          ( const BYTE* pBuf, int bufSize );
    ShaderVarValue              GetVariableValue        ( D3DXHANDLE h, ShaderVarType type );

    static FXIncluder           s_FXIncluder;
}; // class ShaderFX

/*****************************************************************/
/*	Class:	ShaderError
/*	Desc:	"No shader" stub
/*****************************************************************/
class ShaderError : public IShader, public IResource
{
    std::string                     m_Name;
    int                             m_ID;

public:
    ShaderError() : m_ID(-1) {}
    virtual const char*             GetName         () const { return m_Name.c_str(); }
    virtual void                    SetName         ( const char* name ) { m_Name = name; }

    virtual int                     GetID           () const { return m_ID; }
    virtual void                    SetID           ( int id ) { m_ID = id; }

    virtual int                     GetNTech        () const { return 0; }
    virtual bool                    IsTechValid     ( int techID ) const { return false; }
    virtual const char*             GetTechName     ( int techID ) const { return ""; }
    virtual bool                    SetActiveTech   ( int techID ) { return false; }

    virtual int                     GetNPasses      ( int techID = 0 ) const { return 0; }
    virtual int                     GetNShaderVars  () const { return 0; }
    virtual const char*             GetVariableName ( int cID ) const { return ""; }
    virtual int                     GetShaderVarID  ( const char* cName ) { return 0; }

    virtual bool                    SetShaderVar    ( int cID, bool val ) { return false; }
    virtual bool                    SetShaderVar    ( int cID, float val ) { return false; }
    virtual bool                    SetShaderVar    ( int cID, int val ) { return false; }
    virtual bool                    SetShaderVar    ( int cID, const Matrix4D& val ) { return false; }
    virtual bool                    SetShaderVar    ( int cID, const Vector4D& val ) { return false; }
    virtual bool                    SetShaderVar    ( int cID, const Vector3D& val ) { return false; }
    virtual void                    SetAutoVars     () {}

    virtual bool                    LoadFromFile    ( const char* fName ) { return false; }
    virtual bool                    LoadFromMemory  ( const BYTE* pBuf, int bufSize ){ return false; }

    virtual bool                    Begin           () { return false; }
    virtual bool                    BeginPass       ( int passID ) { return false; }
    virtual bool                    EndPass         () { return false; }
    virtual bool                    End             () { return false; }

    virtual bool                    Reload          () { return false; }

    virtual void                    DeleteDeviceObjects     () {}
    virtual void                    InvalidateDeviceObjects () {}
    virtual void                    RestoreDeviceObjects    () {}
}; // class ShaderError

inline ShaderVarType GetShaderVarType( D3DXPARAMETER_CLASS cl, D3DXPARAMETER_TYPE vt )
{
    if (cl == D3DXPC_SCALAR)
    {
        if (vt == D3DXPT_BOOL   ) return svtBool;   
        if (vt == D3DXPT_FLOAT  ) return svtFloat;     
        if (vt == D3DXPT_INT    ) return svtInt;    
    }
    if (cl == D3DXPC_VECTOR) return svtVector; 
    if (cl == D3DXPC_MATRIX_COLUMNS) return svtMatrix; 
    if (cl == D3DXPC_MATRIX_ROWS) return svtMatrix; 
    return svtUnknown;
} // GetShaderVarType

#endif // __D3DShaderFX_H__