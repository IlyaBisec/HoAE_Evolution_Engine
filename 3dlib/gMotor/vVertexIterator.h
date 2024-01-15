/*****************************************************************
/*  File:   vVertexIterator.h                                             
/*    Desc:    Facility for iterating through vertex buffer components
/*  Date:   3 Dec 2004                                        
/*****************************************************************/
#ifndef __VVERTEXITERATOR_H__
#define __VVERTEXITERATOR_H__

#include "IVertexBuffer.h"

struct TexCoord
{
    float u;
    float v;
};

/*****************************************************************************/
/*    Class:    VertexIterator
/*    Desc:    Iterates vertex arrays of any vertex type
/*    Example:
/*                VertexIterator v( pVert, nV, vdecl );
/*                while(v)
/*                {
/*                    v.z() =+ v.x();
/*                    v.diffuse() = 0xFFFFFFFF;
/*                    ++v;
/*                } 
/*****************************************************************************/
class VertexIterator
{
    BYTE*                    m_pData;        //  vertex buffer pointer
    int                     m_NVert;        //  total number of vertices in the buffer
    BYTE*                   m_pVertex;      //  current vertex pointer
    int                     m_CurVertex;    //  current vertex index
    
    int                        m_Stride;       //  vertex stride

    //  component strides
    int                     m_Pos;
    int                     m_RHW;
    int                     m_Normal;
    int                     m_Diffuse;
    int                     m_Specular;
    int                     m_Tex[c_MaxTextureCoordinates];
    int                     m_BlendW[c_MaxBlendWeights];
    int                     m_BlendI[c_MaxBlendWeights];
    int                     m_Tangent;
    int                     m_Binormal;
    
    template <class T> T& component( int stride )
    {
        static T dummy;
        if (stride < 0) return dummy;
        return *((T*)(m_pVertex + stride));
    }

    template <class T> const T& c_component( int stride ) const
    {
        static T dummy;
        if (stride < 0) return dummy;
        return *((T*)(m_pVertex + stride));
    }

    template <class T> T& component( int vID, int stride )
    {
        static T dummy;
        if (stride < 0) return dummy;
        return *((T*)(m_pData + vID*m_Stride + stride));
    }

    template <class T> const T& c_component( int vID, int stride ) const
    {
        static T dummy;
        if (stride < 0) return dummy;
        return *((T*)(m_pData + vID*m_Stride + stride));
    }

public:

                            VertexIterator  () : m_pData(NULL), m_NVert(0), m_Stride(0), m_pVertex(NULL) {}
                            VertexIterator  ( BYTE* vbuf, int nV, const VertexDeclaration& vdecl ) { reset( vbuf, nV, vdecl ); }
    void                    reset           ( BYTE* vbuf, int nV, const VertexDeclaration& vdecl ) 
    { 
        m_pData     = vbuf;
        m_pVertex   = vbuf;
        m_NVert     = nV;
        m_CurVertex = 0;
        set_vdecl( vdecl );
    }
    
    void                    set_vdecl       ( const VertexDeclaration& vdecl )
    {
        m_Pos       = -1;
        m_RHW       = -1;
        m_Normal    = -1;
        m_Diffuse   = -1;
        m_Specular  = -1;
        m_Tangent   = -1;
        m_Binormal  = -1;
        for (int i = 0; i < c_MaxTextureCoordinates; i++)   m_Tex[i]    = -1;
        for (int i = 0; i < c_MaxBlendWeights; i++)         m_BlendW[i] = -1;
        for (int i = 0; i < c_MaxBlendWeights; i++)         m_BlendI[i] = -1;

        m_Stride = vdecl.m_VertexSize;
        for (int i = 0; i < vdecl.m_NElements; i++)
        {
            const VertElement& el = vdecl.m_Element[i];
            switch (el.m_Usage)
            {
            case vcPosition:    m_Pos = el.m_Offset; break;
            case vcPositionRHW:    m_RHW = el.m_Offset + 3*sizeof(Vector3D); break;
            case vcBlend0:        m_BlendW[0] = el.m_Offset; break;
            case vcBlend1:        m_BlendW[1] = el.m_Offset; break;
            case vcBlend2:        m_BlendW[2] = el.m_Offset; break;
            case vcBlend3:        m_BlendW[3] = el.m_Offset; break;
            case vcBlendIdx:    m_BlendI[0] = el.m_Offset; break;
            case vcBlendIdx0:    m_BlendI[0] = el.m_Offset; break;    
            case vcBlendIdx1:    m_BlendI[1] = el.m_Offset; break;    
            case vcBlendIdx2:    m_BlendI[2] = el.m_Offset; break;   
            case vcBlendIdx3:    m_BlendI[3] = el.m_Offset; break;    
            case vcNormal:        m_Normal    = el.m_Offset; break;
            case vcDiffuse:        m_Diffuse   = el.m_Offset; break;
            case vcSpecular:    m_Specular  = el.m_Offset; break;    
            case vcTexCoor0:    m_Tex[0]    = el.m_Offset; break;    
            case vcTexCoor1:    m_Tex[1]    = el.m_Offset; break;    
            case vcTexCoor2:    m_Tex[2]    = el.m_Offset; break;    
            case vcTexCoor3:    m_Tex[3]    = el.m_Offset; break;    
            case vcTexCoor4:    m_Tex[4]    = el.m_Offset; break;    
            case vcTexCoor5:    m_Tex[5]    = el.m_Offset; break;    
            case vcTexCoor6:    m_Tex[6]    = el.m_Offset; break;    
            case vcTexCoor7:    m_Tex[7]    = el.m_Offset; break;    
            case vcBinormal:    m_Binormal  = el.m_Offset; break;    
            case vcTangent:        m_Tangent   = el.m_Offset; break;    
            }
        }
    }

    void                    reset           ()          { m_pVertex = m_pData; m_CurVertex = 0; }
    void                    reset           ( int idx ) { m_pVertex = m_pData + m_Stride*idx; m_CurVertex = idx; }
    VertexIterator&            operator++      ()          { m_pVertex += m_Stride; m_CurVertex++; return *this; }            
    VertexIterator&            operator++      ( int )     { m_pVertex += m_Stride; m_CurVertex++; return *this; }        
    VertexIterator&            operator+=      ( int nV )  { m_pVertex += nV*m_Stride; m_CurVertex += nV; return *this; }            
    VertexIterator&            operator-=      ( int nV )  
    { 
        if (nV > m_CurVertex) nV = m_CurVertex;
        m_pVertex -= nV*m_Stride; 
        m_CurVertex -= nV; 
        return *this; 
    }            

    operator bool   () const    { return (m_CurVertex < m_NVert); }

    const Vector3D&         c_pos           () const    { return c_component<Vector3D>( m_Pos );        }
    const float&            c_rhw           () const    { return c_component<float>   ( m_RHW );        }
    const Vector3D&         c_normal        () const    { return c_component<Vector3D>( m_Normal );     }
    const DWORD&            c_diffuse       () const    { return c_component<DWORD>   ( m_Diffuse );    }
    const DWORD&            c_specular      () const    { return c_component<DWORD>   ( m_Specular );   }
    const TexCoord&         c_uv            ( int idx ) const { return c_component<TexCoord>( m_Tex[idx] );   }
    const Vector3D&         c_tangent       () const    { return c_component<Vector3D>( m_Tangent );    }
    const Vector3D&         c_binormal      () const    { return c_component<Vector3D>( m_Binormal );   }
    const DWORD&            c_blend_idx     ( int idx ) const { return c_component<DWORD>   ( m_BlendI[idx] );}
    const float&            c_blend_w       ( int idx ) const { return c_component<float>   ( m_BlendW[idx] );}

    Vector3D&               pos             ()          { return component<Vector3D>( m_Pos );        }
    float&                  rhw             ()          { return component<float>   ( m_RHW );        }
    Vector3D&               normal          ()          { return component<Vector3D>( m_Normal );     }
    DWORD&                  diffuse         ()          { return component<DWORD>   ( m_Diffuse );    }
    DWORD&                  specular        ()          { return component<DWORD>   ( m_Specular );   }
    TexCoord&               uv              ( int idx ) { return component<TexCoord>( m_Tex[idx] );   }
    Vector3D&               tangent         ()          { return component<Vector3D>( m_Tangent );    }
    Vector3D&               binormal        ()          { return component<Vector3D>( m_Binormal );   }
    DWORD&                  blend_idx       ( int idx ) { return component<DWORD>   ( m_BlendI[idx] );}
    float&                  blend_w         ( int idx ) { return component<float>   ( m_BlendW[idx] );}

    const Vector3D&         c_pos           ( int vID ) const    { return c_component<Vector3D>( vID, m_Pos );        }
    const float&            c_rhw           ( int vID ) const    { return c_component<float>   ( vID, m_RHW );        }
    const Vector3D&         c_normal        ( int vID ) const    { return c_component<Vector3D>( vID, m_Normal );     }
    const DWORD&            c_diffuse       ( int vID ) const    { return c_component<DWORD>   ( vID, m_Diffuse );    }
    const DWORD&            c_specular      ( int vID ) const    { return c_component<DWORD>   ( vID, m_Specular );   }
    const TexCoord&         c_uv            ( int vID, int idx ) const { return c_component<TexCoord>( vID, m_Tex[idx] );   }
    const Vector3D&         c_tangent       ( int vID ) const    { return c_component<Vector3D>( vID, m_Tangent );    }
    const Vector3D&         c_binormal      ( int vID)  const    { return c_component<Vector3D>( vID, m_Binormal );   }
    const DWORD&            c_blend_idx     ( int vID, int idx ) const { return c_component<DWORD>   ( vID, m_BlendI[idx] );}
    const float&            c_blend_w       ( int vID, int idx ) const { return c_component<float>   ( vID, m_BlendW[idx] );}

    Vector3D&               pos             ( int vID )          { return component<Vector3D>( vID, m_Pos );        }
    float&                  rhw             ( int vID )          { return component<float>   ( vID, m_RHW );        }
    Vector3D&               normal          ( int vID )          { return component<Vector3D>( vID, m_Normal );     }
    DWORD&                  diffuse         ( int vID )          { return component<DWORD>   ( vID, m_Diffuse );    }
    DWORD&                  specular        ( int vID )          { return component<DWORD>   ( vID, m_Specular );   }
    TexCoord&               uv              ( int vID, int idx ) { return component<TexCoord>( vID, m_Tex[idx] );   }
    Vector3D&               tangent         ( int vID )          { return component<Vector3D>( vID, m_Tangent );    }
    Vector3D&               binormal        ( int vID)           { return component<Vector3D>( vID, m_Binormal );   }
    DWORD&                  blend_idx       ( int vID, int idx ) { return component<DWORD>   ( vID, m_BlendI[idx] );}
    float&                  blend_w         ( int vID, int idx ) { return component<float>   ( vID, m_BlendW[idx] );}

}; // class VertexIterator

#endif 