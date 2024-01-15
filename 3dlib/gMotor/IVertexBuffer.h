/*****************************************************************
/*  File:   IVertexBuffer.h                                      
/*  Desc:   Interface to the vertex buffers
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Oct 2004                                             
/*****************************************************************/
#ifndef __IVERTEXBUFFER_H__ 
#define __IVERTEXBUFFER_H__

//  maximal possible number of texture coordinates in the mesh vertex
const int c_MaxTextureCoordinates   = 8;  
const int c_MaxBlendWeights         = 4;

/*****************************************************************/
/*    Enum:    VertCompUsage
/*    Desc:    Describes meaning of the vertex component value
/*    Remark:    Values are ordered by priority, as they are in the 
/*                actual vertex layout
/*****************************************************************/
enum VertCompUsage
{
    vcUnknown        = 0x0,

    vcPosition       = 0x00000001,     //  vertex position    
    vcPositionRHW    = 0x00000002,     //  vertex position with reciprocal homogeneous w component
   
    vcBlend0         = 0x00000004,     //     blending weights
    vcBlend1         = 0x00000008,
    vcBlend2         = 0x00000010,
    vcBlend3         = 0x00000020,
    vcBlendIdx       = 0x00000040,     //     blending indices packed into DWORD value        
    vcBlendIdx0      = 0x00000080,     //     blending indices packed into DWORD value        
    vcBlendIdx1      = 0x00000100,     //     blending indices packed into DWORD value        
    vcBlendIdx2      = 0x00000200,     //     blending indices packed into DWORD value        
    vcBlendIdx3      = 0x00000400,     //     blending indices packed into DWORD value    
    
    vcNormal         = 0x00000800,     //  normal at the vertex 
    vcDiffuse        = 0x00001000,     //     diffuse color component 
    vcSpecular       = 0x00002000,    //  specular color component 
    
    vcTexCoor0       = 0x00004000,    //  texture coordinates
    vcTexCoor1       = 0x00008000,
    vcTexCoor2       = 0x00010000,
    vcTexCoor3       = 0x00020000,
    vcTexCoor4       = 0x00040000,
    vcTexCoor5       = 0x00080000,
    vcTexCoor6       = 0x00100000,
    vcTexCoor7       = 0x00200000,
    
    vcBinormal       = 0x00400000,     //  binormal (oY axis)
    vcTangent        = 0x00800000,     //     tangent  (oX axis)

	vcColor2		 = 0x01000000,
	vcColor3		 = 0x02000000,

    
    vcCustom         = 0x80000000,    //  something defined by user

}; // enum VertCompUsage

/*****************************************************************/
/*    Enum:    VertCompType
/*    Desc:    Describes type of the vertex component value
/*****************************************************************/
enum VertCompType
{
    ctUnknown           = 0,
    ctFloat1            = 1,
    ctFloat2            = 2,
    ctFloat3            = 3,
    ctFloat4            = 4,
    ctColor             = 5,
    ctShort2            = 6,
    ctShort4            = 7,
    ctUByte4            = 8,
}; // enum VertCompType 

inline int GetVertCompSize( VertCompType type )
{
    switch (type)
    {
    case ctFloat1:  return 4;
    case ctFloat2:  return 8;
    case ctFloat3:  return 12;
    case ctFloat4:  return 16;
    case ctColor:   return 4;    
    case ctShort2:  return 4;
    case ctShort4:  return 8;
    case ctUByte4:  return 4;
    }
    return 0;
} // GetVertCompSize

inline VertCompType GetDefaultCompType( VertCompUsage usage )
{
    switch (usage)
    {
        case vcPosition:        return ctFloat3;
        case vcPositionRHW:     return ctFloat4;
        case vcBlend0:          return ctFloat1;
        case vcBlend1:          return ctFloat1;
        case vcBlend2:          return ctFloat1;
        case vcBlend3:          return ctFloat1;
        case vcBlendIdx:        return ctUByte4;
        case vcBlendIdx0:       return ctUByte4;
        case vcBlendIdx1:       return ctUByte4;
        case vcBlendIdx2:       return ctUByte4;
        case vcBlendIdx3:       return ctUByte4;
        case vcNormal:          return ctFloat3;
        case vcDiffuse:         return ctColor;
        case vcSpecular:        return ctColor;
		case vcColor2:			return ctColor;
		case vcColor3:			return ctColor;
        case vcTexCoor0:        return ctFloat2;
        case vcTexCoor1:        return ctFloat2;
        case vcTexCoor2:        return ctFloat2;
        case vcTexCoor3:        return ctFloat2;
        case vcTexCoor4:        return ctFloat2;
        case vcTexCoor5:        return ctFloat2;
        case vcTexCoor6:        return ctFloat2;
        case vcTexCoor7:        return ctFloat2;
        case vcBinormal:        return ctFloat3;
        case vcTangent:         return ctFloat3;
        case vcCustom:          return ctFloat3;
    }
    return ctUnknown;
} // GetDefaultCompType

/*****************************************************************/
/*    Struct:    VertElement
/*    Desc:    Describes single element of the vertex stream layout
/*****************************************************************/
struct VertElement
{
    int                     m_Stream;       //  index of the stream this element is in
    int                     m_Offset;       //    offset from the vertex data begin, in bytes
    VertCompUsage           m_Usage;        //  element semantic
    VertCompType            m_Type;         //    element data type
    
    VertElement() : m_Stream(0), m_Offset(0), m_Usage(vcUnknown), m_Type(ctUnknown){}
    VertElement( VertCompUsage usage, VertCompType type, int stream = 0 )
    {
        m_Usage     = usage;
        m_Offset    = 0;
        m_Type      = type;
        m_Stream    = stream;
    }
}; // struct VertElement

const int c_MaxVertDeclElements = 16;
/*****************************************************************/
/*    Struct:    VertexDeclaration
/*    Desc:    Declaration of the vertex stream data mapping
/*****************************************************************/
struct VertexDeclaration
{
    VertElement             m_Element[c_MaxVertDeclElements];   //  array of the vertex elements
    int                     m_NElements;                        //  number of vertex elements
    int                     m_VertexSize;                       //  size of the vertex, in bytes
    int                     m_TypeID;                           //  id in the render system vtype registry

    VertexDeclaration() : m_NElements(0), m_VertexSize(0), m_TypeID(-1) {}
    VertexDeclaration& operator <<( const VertElement& el ) 
    {
        assert( m_NElements < c_MaxVertDeclElements );
        m_Element[m_NElements].m_Usage  = el.m_Usage;
        m_Element[m_NElements].m_Type   = el.m_Type;
        m_Element[m_NElements].m_Stream = el.m_Stream;
        m_Element[m_NElements].m_Offset = m_VertexSize;
        m_VertexSize += GetVertCompSize( el.m_Type );
        m_NElements++;
        return *this;
    } // operator <<

    VertexDeclaration& operator <<( VertCompUsage usage ) 
    {
        return operator <<( VertElement( usage, GetDefaultCompType( usage ), 0 ) );
    } // operator <<

    bool operator ==( const VertexDeclaration& v ) const
    {
        if (m_NElements != v.m_NElements || m_VertexSize != v.m_VertexSize) return false;
        for (int i = 0; i < m_NElements; i++) 
        {
            if (m_Element[i].m_Type   != v.m_Element[i].m_Type  || 
                m_Element[i].m_Usage  != v.m_Element[i].m_Usage ||
                m_Element[i].m_Stream != v.m_Element[i].m_Stream) return false;
        }
        return true;
    } // operator ==

}; // struct VertexDeclaration

/*****************************************************************/
/*    Class:    IVertexBuffer
/*    Desc:    Interface to the array of geometry vertices
/*****************************************************************/
class IVertexBuffer
{
public:
    
    virtual bool        Bind                    ( int stream = 0, int frequency = 1 ) = 0;
    virtual bool        Create                  ( int nBytes, bool bDynamic = true, const VertexDeclaration* pVDecl = NULL ) = 0; 
    virtual void        SetVertexDecl           ( const VertexDeclaration& vdecl ) = 0;
    virtual const VertexDeclaration& GetVDecl   () const = 0;
    virtual int         GetVType                () const = 0;
    virtual void        SetVertexSize           ( int size ) = 0;
    virtual void        SetVType                ( int vType ) = 0;

    virtual int         GetSizeBytes            () const = 0;
    virtual int         GetNVert                () const = 0;
    virtual int         GetNFilledVert          () const = 0;
    virtual const char* GetName                 () const = 0;

    virtual bool        IsLocked                () const = 0;
    virtual bool        IsDynamic               () const = 0;

    virtual void        DeleteDeviceObjects     () = 0;
    virtual void        InvalidateDeviceObjects () = 0;
    virtual void        RestoreDeviceObjects    () = 0;
    
    virtual BYTE*       Lock                    ( int firstV, int numV, DWORD& stamp, bool bDiscard = false ) = 0;
    virtual BYTE*       LockAppend              ( int numV, int& offset, DWORD& stamp ) = 0;
    virtual bool        HasAppendSpace          ( int numV ) = 0;
    virtual bool        IsStampValid            ( DWORD stamp ) = 0;

    virtual void        Unlock                  () = 0;
    virtual void        Purge                   () = 0;

}; // class IVertexBuffer

#endif // __IVERTEXBUFFER_H__ 