/*****************************************************************************/
/*	File:	d3dVertexBuffer.h
/*  Desc:	VertexBuffer interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#ifndef __D3DVERTEXBUFFER_H__
#define __D3DVERTEXBUFFER_H__
#include "IVertexBuffer.h"

/*****************************************************************************/
/*  Class:  VertexBufferDX9
/*  Desc:   Direct3D9 vertex buffer implementation
/*****************************************************************************/
class VertexBufferDX9 : public IVertexBuffer
{
    std::string             m_Name;             //  vertex buffer name 
    bool                    m_bDynamic;         //  when true, vb is dynamic, else it is static
    IDirect3DDevice9*       m_pDevice;          //  device interface
    IDirect3DVertexBuffer9* m_pBuffer;          //  d3d vertex buffer interface
    int                     m_SizeBytes;        //  size of vertex data, in bytes  
    int                     m_NVert;            //  number of vertices in the buffer
    int                     m_NFilledVert;      //  first vertex which was not filled
    bool                    m_bLocked;          //  whether buffer is locked

    VertexDeclaration       m_VDecl;            //  buffer vertex declaration
    DWORD                   m_FVF;              //  FVF code 
    int                     m_VStride;          //  vertex stride
    int                     m_VType;            //  vertex type index
    int                     m_FirstValidStamp;  //  the first valid lock index in the lock sequence
    int                     m_CurrentStamp;     //  current number of locks
    bool                    m_bManaged;
	bool					m_Free;
    
public:
                            VertexBufferDX9         ( const char* name, IDirect3DDevice9* pDevice = NULL );

    virtual bool            Bind                    ( int stream = 0, int frequency = 1 );
    virtual bool            Create                  ( int nBytes, bool bDynamic = true, 
                                                        const VertexDeclaration* pVDecl = NULL ); 

    virtual void            SetVertexDecl           ( const VertexDeclaration& vdecl );
    virtual const VertexDeclaration& GetVDecl       () const { return m_VDecl; }
    virtual void            SetVertexSize           ( int size );

    virtual int             GetSizeBytes            () const { return m_SizeBytes; }
    virtual int             GetNVert                () const { return m_NVert; }
    virtual int             GetNFilledVert          () const { return m_NFilledVert; }
    virtual const char*     GetName                 () const { return m_Name.c_str(); }
    virtual int             GetVType                () const { return m_VType; }
    virtual void            SetVType                ( int vType ) { m_VType = vType; }


    virtual bool            IsLocked                () const { return m_bLocked; }
    virtual bool            IsDynamic               () const { return m_bDynamic; }

    virtual void            DeleteDeviceObjects     ();
    virtual void            InvalidateDeviceObjects ();
    virtual void            RestoreDeviceObjects    ();

    virtual BYTE*           Lock                    ( int firstV, int numV, DWORD& stamp, bool bDiscard = false );
    virtual BYTE*           LockAppend              ( int numV, int& offset, DWORD& stamp );
    virtual bool            HasAppendSpace           ( int numV );
    
    virtual void            Unlock                  ();
    virtual bool            IsStampValid            ( DWORD stamp ) { return (stamp >= m_FirstValidStamp); }

    void                    SetStride               ( int stride ) { m_VStride = stride; }
    virtual void            Purge                   ();
	bool					IsFree					(){return m_Free;}
	void					SetFree					(bool Val){m_Free=Val;}

}; // class VertexBufferDX9

#endif // __D3DVERTEXBUFFER_H__