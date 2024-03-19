/*****************************************************************
/*  File:   IIndexBuffer.h                                      
/*  Desc:   Interface to the index buffers
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Oct 2004                                             
/*****************************************************************/
#ifndef __IINDEXBUFFER_H__ 
#define __IINDEXBUFFER_H__

enum IndexSize
{
    isWORD  = 0,
    isDWORD = 1
}; // enum IndexSize

/*****************************************************************/
/*    Class:    IIndexBuffer
/*    Desc:    Interface to the index arrays 
/*****************************************************************/
class IIndexBuffer
{
public:    
    virtual bool        Create                  ( int nBytes, bool bDynamic = true, IndexSize = isWORD ) = 0; 
    virtual bool        Bind                    () = 0;

    virtual int         GetSizeBytes            () const = 0;
    virtual int         GetNIndices             () const = 0;
    virtual bool        IsLocked                () const = 0;
    virtual bool        IsDynamic               () const = 0;
    virtual IndexSize   GetIndexSize            () const = 0;
    virtual const char* GetName                 () const = 0;

    virtual BYTE*       Lock                    ( int firstIdx, int numIdx, DWORD& stamp, bool bDiscard = false ) = 0;
    virtual BYTE*       LockAppend              ( int numIdx, int& offset, DWORD& stamp ) = 0;
    virtual bool        HasAppendSpace          ( int numIdx ) = 0;

    virtual bool        IsStampValid            ( DWORD stamp ) = 0;
    virtual void        Unlock                  () = 0;
    virtual void        DeleteDeviceObjects     () = 0;
    virtual void        InvalidateDeviceObjects () = 0;
    virtual void        RestoreDeviceObjects    () = 0;
    virtual void        Purge                   () = 0;
}; // class IIndexBuffer

#endif // __IINDEXBUFFER_H__ 