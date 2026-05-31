/*****************************************************************
/*  File:   kUtilities.h                                          
/*  Desc:   Helper routines                                      
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   December 2001                                        
/*****************************************************************/
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#pragma    once

/*****************************************************************************/
/*    Enum:    PowerOfTwo
/*    Desc:    Helper for power-of-two values
/*****************************************************************************/
enum PowerOfTwo
{
    pow0        =    1,
    pow1        =    2,
    pow2        =    4,
    pow3        =    8,
    pow4        =    16,
    pow5        =    32,
    pow6        =    64,
    pow7        =    128,
    pow8        =    256,
    pow9        =    512,
    pow10        =    1024,
    pow11        =    2048,
    pow12        =    4096,
    pow13        =    8192,
    pow14        =    16384,
    pow15        =    32768,
    pow16        =    65536,
}; // enum PowerOfTwo

inline bool IsPow2( int val )
{
    return (((val)&(val - 1)) == 0);
} // IsPow2

inline int Pow2( int val )
{
    int res = 1;
    while (val > 0) { res <<= 1; val--; }
    return res;
} // Pow2

inline int GetPower( PowerOfTwo p )
{
    int res = 0;
    int np = (int)p;
    while (np > 1) { np >>= 1; res++; }
    return res;
} // GetPower

/*****************************************************************
/*  Class:  util                                                 *
/*  Desc:   Different helper stuff                               *
/*****************************************************************/
class util{
public:
    util();

    static BYTE            powSidesByte[8];
    static float        powSidesFloat[8];

    static float        byteAngleCos[256];
    static float        byteAngleSin[256];
    static int            rnd[1024];

    static float        PI;    
    static float        DoublePI;    
    static float        PIdiv16;
    static float        PIdiv32;
    static float        PImul2;    
    static float        sin30;

    static WORD            quad8x8Idx[384];

    static void InitValueArrays();

    static _inl void MemcpyRect(    BYTE* dst,            int dstPitch,
                                    const BYTE* src,    int srcPitch, 
                                    int nLines, 
                                    int bytesInLine );

    static _inl void QuadShrink2xW( BYTE* dst,            int dstPitch,
                                    const BYTE* src,    int srcPitch, 
                                    int srcQuadSide );

    static _inl void QuadShrink4xW( BYTE* dst,            int dstPitch,
                                    const BYTE* src,    int srcPitch, 
                                    int srcQuadSide );

private:
    static util    dummy;
}; // class util

//  performs aligned memory allocation of the objects array
template <class T> T* aligned_new( int num, int align )
{
    //  check if align value is power-of-two
    if (((align)&(align - 1)) != 0) return NULL;
    int nBytes = num*sizeof( T );
    //  allocate byte array
    if (align < sizeof( unsigned char* )) align = sizeof( unsigned char* );
    unsigned char* pBegin = new unsigned char[nBytes + align];
    int nLeft = align - reinterpret_cast<int>( pBegin )%align;
    // remember actual adress of the memory block
    *((unsigned char**)(pBegin + nLeft - sizeof( unsigned char* ))) = pBegin;
    //  call placement operator new on array members
    T* pArray = (T*)(pBegin + nLeft);
    pArray = new( pArray ) T[num];
    return pArray;
} // aligned_new

//  performs deletion of aligned array, no destructors are called
template <class T> void aligned_delete_nodestruct( T* ptr )
{
    if (!ptr) return;
    //  retrieve pointer to the actual memory block
    unsigned char* bPtr = (unsigned char*)ptr;
    unsigned char* pBegin = *((unsigned char**)(bPtr - sizeof( unsigned char* )));
    delete []pBegin;
} // aligned_delete_nodestruct

//  performs deletion of aligned array, with destructors called
template <class T> void aligned_delete( T* ptr, int num )
{
    if (!ptr) return;
    //  retrieve pointer to the actual memory block
    unsigned char* bPtr = (unsigned char*)ptr;
    unsigned char* pBegin = *((unsigned char**)(bPtr - sizeof( unsigned char* )));
    for (int i = 0; i < num; i++) ptr[i].~T();
    delete []pBegin;
} // aligned_delete

#define stack_alloc(nBytes,Align) (void *)((reinterpret_cast<int>( _alloca( nBytes + Align ) ) + Align) & ~Align);

/*****************************************************************
/*  Class:  AlignedBuffer                                                 
/*  Desc:   Placeholder class for aligned raw data buffer                              
/*****************************************************************/
template <class T, int Align> class AlignedBuffer
{
    T*          m_pData;
    int         m_Size;

public:
    AlignedBuffer() : m_pData(NULL), m_Size(0) {}
    AlignedBuffer( int size ) : m_pData(NULL), m_Size(0) { Resize( size ); }
    ~AlignedBuffer() { aligned_delete_nodestruct( m_pData ); }
    operator BYTE*() { return m_pData; }
    void Resize( int nElem, bool bKeepContents = false ) 
    { 
        if (m_Size == nElem) return;
        if (bKeepContents && m_pData)
        {
            T* pBuf = aligned_new<T>( nElem, Align );
            int nCopy = m_Size;
            if (nCopy > nElem) nCopy = nElem;
            memcpy( pBuf, m_pData, nCopy );
            aligned_delete_nodestruct( m_pData );
            m_pData = pBuf;
            m_Size = nElem;
            return;
        }
        aligned_delete_nodestruct( m_pData );
        m_pData = aligned_new<T>( nElem, Align );
        m_Size = nElem;
    }

    T& operator[]( int idx ) { return m_pData[idx]; }
    const T& operator[]( int idx ) const { return m_pData[idx]; }

    void Clear()
    {
        aligned_delete_nodestruct( m_pData );
        m_pData = NULL;
        m_Size = 0;
    }

    void Reset( const T& val )
    {
        for (int i = 0; i < m_Size; i++) m_pData[i] = val;
    }

    const T* GetData() const { return m_pData; }
    int GetSize() const { return m_Size; }
}; // class AlignedBuffer

template <class T, int Align> 
InStream& operator >>( InStream& is, AlignedBuffer<T,Align>& ab )
{
    int sz = 0;
    is >> sz;
    ab.Resize( sz );
    is.Read( (BYTE*)ab, sz*sizeof(T) );
    return is;
}

template <class T, int Align> 
OutStream& operator <<( OutStream& os, const AlignedBuffer<T,Align>& ab )
{
    int sz = ab.GetSize();
    os << sz;
    os.Write( ab.GetData(), sz*sizeof(T) );
    return os;
}

bool HaveCPUID  (); 
bool HaveMMX    ();
bool Have3DNow  ();
bool HaveSSE    ();
bool HaveSSE2   (); 

#ifdef _INLINES
#include "kUtilities.inl"
#endif // _INLINES

#endif // __UTILITIES_H__