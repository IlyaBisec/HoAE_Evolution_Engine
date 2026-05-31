/*****************************************************************
/*  File:   IImpostorCache.h                                      
/*  Desc:   Interface to the impostor cache system
/*    Author: Ruslan Shestopalyuk
/*  Date:   Sep 2004
/*****************************************************************/
#ifndef __IIMPOSTORCACHE_H__ 
#define __IIMPOSTORCACHE_H__ 

/*****************************************************************************/
/*  Class:  IImpostorCache
/*  Desc:   Interface for the impostor cache
/*****************************************************************************/
class IImpostorCache
{
public:
    
    //  initializes impostor cache
    virtual bool    Init            () = 0;

    //  flushes render queue
    virtual void    Flush           () = 0;
    //  draws static model with given model matrix
    virtual void    Draw            ( DWORD modelID, const Matrix4D& tm ) = 0;
    //  draws animated model with given model matrix
    virtual void    Draw            ( DWORD modelID, DWORD animID, float anmTime, const Matrix4D& tm ) = 0;
    //  renders debug info
    virtual void    DrawDebugInfo   ( int dbgSurf = -1 ) = 0;
    virtual void    DumpSurfaces    () = 0;
    
}; // class IImpostorCache

extern IImpostorCache* IImpCache;

#endif // __IIMPOSTORCACHE_H__ 