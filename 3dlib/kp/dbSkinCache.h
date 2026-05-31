/*****************************************************************
/*  File:   dbSkinCache.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBSKINCACHE_H__
#define __DBSKINCACHE_H__


/*****************************************************************/
/*	Class:	SkinCacheDebugger
/*	Desc:	Skin cache debugger
/*****************************************************************/
class SkinCacheDebugger : public Dialog
{
    bool                    m_bDrawModels;
    bool                    m_bDrawSkinCache;
    bool                    m_bApplyLOD;
    bool                    m_bDrawDebugInfo;
    bool                    m_bDrawScene;
    bool                    m_bAnimate;
    int                     m_DebugSurface;
    std::string             m_ModelName;
    std::string             m_AnimName;
    int                     m_NX;
    int                     m_NY;
    float                   m_Scale;
    float                   m_Width;

    DWORD                   m_ModelID;
    DWORD                   m_AnimID;
    
    float                   m_LODTreshold;

    DWORD                   m_SceneID;

public:
							SkinCacheDebugger	();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );

    const char*             GetModelName        () const { return m_ModelName.c_str(); }
    const char*             GetAnimName         () const { return m_AnimName.c_str(); }

    void                    SetModelName        ( const char* name );
    void                    SetAnimName         ( const char* name );
    void                    Dump                ();


	DECLARE_SCLASS( SkinCacheDebugger, Dialog, SCDB );
}; // class SkinCacheDebugger



#endif // __DBSKINCACHE_H__
