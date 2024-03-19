/*****************************************************************************/
/*	File:	vMaterial.h
/*	Desc:	Geometry shader, instanced with concrete parameter values
/*	Author:	Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __VSHADERINSTANCE_H__
#define __VSHADERINSTANCE_H__

#include "IReflected.h"


class ModelObject;
/*****************************************************************************/
/*  Class:  ModelShader
/*  Desc:   
/*****************************************************************************/
class ModelShader : public IReflected
{
    int                     m_ID;       
    std::string             m_ScriptName;           //  name of the shader script
    ModelObject*            m_pModel;       
    std::string             m_Texture   [c_MaxTextureStages];

    int                     m_ShaderID;             
    int                     m_TexID     [c_MaxTextureStages];
    int                     m_NPasses;
    bool                    m_bEnabled;

public: 
                            ModelShader            ();
    virtual void		    Expose              ( PropertyMap& pm );
    
    const char*             GetScriptName       () const { return m_ScriptName.c_str(); }

    const char*             GetTex0             () const { return m_Texture[0].c_str(); }
    const char*             GetTex1             () const { return m_Texture[1].c_str(); }
    const char*             GetTex2             () const { return m_Texture[2].c_str(); }
    const char*             GetTex3             () const { return m_Texture[3].c_str(); }

    void                    SetScriptName       ( const char* name );
    void                    SetTex0             ( const char* name ) { SetTex( 0, name ); }
    void                    SetTex1             ( const char* name ) { SetTex( 1, name ); }
    void                    SetTex2             ( const char* name ) { SetTex( 2, name ); }
    void                    SetTex3             ( const char* name ) { SetTex( 3, name ); }
    void                    SetTex              ( int stage, const char* name );

    int                     GetShaderID         () const { return m_ShaderID; }
    int                     GetTexID            ( int stage ) { return m_TexID[stage]; }
    int                     GetNPasses          () const { return m_NPasses; }
    bool                    IsTransparent       () const { return false; }
    void                    Render              ( int pass = 0 );
    void                    SetModel            ( ModelObject* pModel )       { m_pModel = pModel; }
    void                    Validate            ();

    int                     GetID               () const { return m_ID; }
    void                    SetID               ( int id ) { m_ID = id; }

    //  IReflected interface
    virtual IReflected*     Parent              () const                { return (IReflected*)m_pModel; }
    virtual int             NumChildren         () const                { return 0; }
    virtual IReflected*     Child               ( int idx ) const       { return NULL; }
    virtual const char*     GetName             () const                { return m_ScriptName.c_str(); }
    virtual bool            AddChild            ( IReflected* pChild )  { return false; }
    virtual bool            DelChild            ( int idx )             { return false; }
    bool                    operator ==         ( const ModelShader& sh ) const;

    friend inline InStream& operator >>( InStream& is, ModelShader& val );
    friend inline OutStream& operator <<( OutStream& os, const ModelShader& val );

    DECLARE_CLASS(ModelShader);
}; // class ModelShader

inline InStream& operator >>( InStream& is, ModelShader& val )
{
    is >> val.m_ScriptName;
    for (int i = 0; i < c_MaxTextureStages; i++) 
    { 
        is >> val.m_Texture[i]; 
    }
    is >> val.m_bEnabled;
    return is;
} 

inline OutStream& operator <<( OutStream& os, const ModelShader& val )
{
    os << val.m_ScriptName;
    for (int i = 0; i < c_MaxTextureStages; i++) os << val.m_Texture[i];
    os << val.m_bEnabled;
    return os;
}

/*****************************************************************************/
/*  Class:  ShaderTable
/*  Desc:   
/*****************************************************************************/
class ShaderTable : public IReflected, public std::vector<ModelShader>
{
public:
    
    virtual IReflected*     Child               ( int idx ) const 
    { 
        if (idx < 0 || idx >= size()) return NULL;
        return (IReflected*)&at(idx); 
    }
    virtual IReflected*     Parent              () const 
    { 
        if (size() == 0) return NULL; 
        return NULL; 
    }
    virtual void		    Expose              ( PropertyMap& pm )
    {
        pm.start( "ShaderTable", this );
        pm.p( "NodeType", ClassName );
        pm.p( "NumEntries", GetNEntries );
        pm.m( "NewMaterial", NewMaterial );
    }

    virtual int             NumChildren         () const { return size(); }
    virtual const char*     GetName             () const                { return "Shaders"; }
    int                     GetNEntries         () const                { return size(); }
    void                    NewMaterial         ()                      { push_back( ModelShader() ); }
    virtual bool            AddChild            ( IReflected* pChild )  { return false; }
    virtual bool            DelChild            ( int idx )             { return false; }

    DECLARE_CLASS(ShaderTable);
}; // class ShaderTable

#endif // __VSHADERINSTANCE_H__

