/*****************************************************************************/
/*    File:    sgShader.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGSHADER_H__
#define __SGSHADER_H__

#include "sgNode.h"
#include "sgTransformNode.h"

#include <stack>

/*****************************************************************************/
/*    Class:    SurfaceMaterial
/*    Desc:     Object material properties
/*****************************************************************************/
class SurfaceMaterial : public SNode
{
    DWORD                       m_Ambient;
    DWORD                       m_Diffuse;
    DWORD                       m_Specular;
    float                       m_Shininess;
    BYTE                        m_Transparency;

    static SurfaceMaterial*     s_pCurMtl;

public:
    _inl                        SurfaceMaterial ();
    _inl virtual void           Render          ();
    _inl void                   SetDiffuse      ( DWORD _diffuse    );
    _inl void                   SetSpecular     ( DWORD _specular    );
    _inl void                   SetAmbient      ( DWORD _ambient    );
    _inl void                   SetShininess    ( float _shininess  );
    _inl void                   SetTransparency ( BYTE _transparency);

    _inl DWORD                  GetDiffuse      () const;
    _inl DWORD                  GetSpecular     () const;
    _inl DWORD                  GetAmbient      () const;
    _inl float                  GetShininess    () const;
    _inl BYTE                   GetTransparency () const;

    static _inl SurfaceMaterial* GetCurMaterial () { return s_pCurMtl; }

    virtual void                Serialize       ( OutStream& os ) const;
    virtual void                Unserialize     ( InStream& is  );
    virtual void                VisitAttributes (){ s_pCurMtl = this; }
    virtual void                Expose          ( PropertyMap& pm );    
    virtual bool                IsEqual         ( const SNode* node ) const;

    DECLARE_SCLASS(SurfaceMaterial,SNode,MATL);
}; // SurfaceMaterial

/*****************************************************************************/
/*    Class:    BumpMatrix
/*    Desc:    Bump-mapping matrix node
/*****************************************************************************/
class BumpMatrix : public Transform2D
{
    int                 m_Stage;

public:
                        BumpMatrix       () : m_Stage(0) { m_LocalTM.setIdentity(); }
    virtual void        Render           ();
    virtual void        Serialize        ( OutStream& os ) const;
    virtual void        Unserialize      ( InStream& is    );
    virtual void        Expose           ( PropertyMap& pm );


    DECLARE_SCLASS(BumpMatrix,Transform2D,BMTR);
}; // class BumpMatrix

/*****************************************************************************/
/*    Class:    Shader
/*    Desc:    Set of device render states and texture stage states
/*****************************************************************************/
class Shader : public SNode
{
    int                         m_Handle;
    static std::stack<int>      s_Overrides;

public:
                                Shader          () : m_Handle( -1 ) {}
                                Shader          ( const char* name ) : m_Handle( -1 ) { SetName( name ); }
    
    const char*                 GetScriptFile   () const { return GetName(); }
    void                        SetScriptFile   ( const char* file );
    void                        Update          ();
    virtual void                Render          ();
    void                        Expose          ( PropertyMap& pm );

    virtual void                Serialize       ( OutStream& os ) const;
    virtual void                Unserialize     ( InStream& is );

    static void                 PushOverride    ( int shID );
    static void                 PopOverride     ();

    DECLARE_SCLASS(Shader,SNode,DSST);
}; // Shader



#ifdef _INLINES
#include "sgShader.inl"
#endif // _INLINES

#endif // __SGSHADER_H__