/*****************************************************************
/*  File:   dbImpostor.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBIMPOSTOR_H__
#define __DBIMPOSTOR_H__


/*****************************************************************/
/*	Class:	ImpostorDebugger
/*	Desc:	Impostor debugger
/*****************************************************************/
class ImpostorDebugger : public Dialog
{
    bool                    m_bDrawModels;
    bool                    m_bDrawImpostors;
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
							ImpostorDebugger	();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );

    const char*             GetModelName        () const { return m_ModelName.c_str(); }
    const char*             GetAnimName         () const { return m_AnimName.c_str(); }

    void                    SetModelName        ( const char* name );
    void                    SetAnimName         ( const char* name );
    void                    Dump                ();


	DECLARE_SCLASS( ImpostorDebugger, Dialog, IMDB );
}; // class ImpostorDebugger



#endif // __DBIMPOSTOR_H__
