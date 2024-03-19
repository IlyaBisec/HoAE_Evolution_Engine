/*****************************************************************
/*  File:   ISkinCache.h                                      
/*  Desc:   Interface to the skinned models geometry cache system
/*    Author: Ruslan Shestopalyuk
/*  Date:   Sep 2004
/*****************************************************************/
#ifndef __ISKINCACHE_H__ 
#define __ISKINCACHE_H__  

/*****************************************************************************/
/*  Class:  ISkinCache
/*  Desc:   Interface for the skin cache
/*****************************************************************************/
class ISkinCache
{
public:
    
    //  initializes skin cache
    virtual bool    Init            () = 0;

    //  flushes render queue
    virtual void    Flush           () = 0;
    //  draws static model with given model matrix
    virtual void    Draw            ( DWORD modelID, const Matrix4D& tm ) = 0;
    //  draws animated model with given model matrix
    virtual void    Draw            ( DWORD modelID, DWORD animID, float anmTime, const Matrix4D& tm ) = 0;
    //  renders debug info
    virtual void    DrawDebugInfo   () = 0;
    
}; // class ISkinCache

extern ISkinCache* ISkCache;

#endif // __ISKINCACHE_H__  