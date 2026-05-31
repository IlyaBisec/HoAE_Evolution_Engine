/*****************************************************************************/
/*	File:	d3dIndexBuffer.h
/*  Desc:	IndexBuffer interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#ifndef __D3DINDEXBUFFER_H__
#define __D3DINDEXBUFFER_H__
#include "IIndexBuffer.h"

/*****************************************************************************/
/*  Class:  IndexBufferDX9
/*  Desc:   Direct3D9 Index buffer implementation
/*****************************************************************************/
class IndexBufferDX9 : public IIndexBuffer
{
    std::string                 m_Name;             //  index buffer name
    bool                        m_bDynamic;         //  when true, ib is dynamic, else it is static
    int                         m_SizeBytes;        //  size of vertex data, in bytes  
    int                         m_NIndices;         //  number of vertices in the buffer
    bool                        m_bLocked;          //  whether buffer is locked
    int                         m_NFilledIdx;       //  first index which was not filled
    int                         m_IdxStride;        //  index stride, in bytes
    IndexSize                   m_IndexSize;        //  index size type

    IDirect3DIndexBuffer9*      m_pBuffer;          //  d3d index buffer interface

    int                         m_FirstValidStamp;  //  the first valid lock index in the lock sequence
    int                         m_CurrentStamp;     //  current number of locks
	bool						m_Free;

public:
                        IndexBufferDX9          ( const char* name );
    virtual bool        Create                  ( int nBytes, bool bDynamic = true, IndexSize = isWORD ); 
    virtual bool        Bind                    ();

    virtual int         GetSizeBytes            () const { return m_SizeBytes; }
    virtual IndexSize   GetIndexSize            () const { return m_IndexSize; }
    virtual int         GetNIndices             () const { return m_NIndices; }
    virtual const char* GetName                 () const { return m_Name.c_str(); }
    virtual bool        IsLocked                () const { return m_bLocked; }
    virtual bool        IsDynamic               () const { return m_bDynamic; }

    virtual void        DeleteDeviceObjects     ();
    virtual void        InvalidateDeviceObjects ();
    virtual void        RestoreDeviceObjects    ();

    void                CreateQuadBuffer        ();
    virtual BYTE*       Lock                    ( int firstIdx, int numIdx, DWORD& stamp, bool bDiscard = false );
    virtual BYTE*       LockAppend              ( int numIdx, int& offset, DWORD& stamp );
    virtual bool        HasAppendSpace           ( int numIdx );
    virtual bool        IsStampValid            ( DWORD stamp ) { return stamp >= m_FirstValidStamp; }
    void                Unlock                  ();
    virtual void        Purge                   ();
	bool				IsFree					(){return m_Free;}
	void				SetFree					(bool Val){m_Free=Val;}

}; // class IndexBufferDX9

#endif // __D3DINDEXBUFFER_H__