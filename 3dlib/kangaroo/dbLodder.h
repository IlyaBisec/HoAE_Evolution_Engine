/*****************************************************************
/*  File:   dbLodder.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBLODDER_H__
#define __DBLODDER_H__


/*****************************************************************/
/*	Class:	VIPMDebugger
/*	Desc:	VIPM Lodder debugger
/*****************************************************************/
class VIPMDebugger : public Dialog
{
    int                     m_NIterations;
    bool                    m_bDrawDebugInfo;
    std::string             m_FileName;

protected:
    const char*             GetFileName() const { return m_FileName.c_str(); }
    void                    SetFileName( const char* fname ) { m_FileName = fname; }

public:
							VIPMDebugger		();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual bool 			OnMouseLBDown	( int mX, int mY );
	virtual bool 			OnMouseMove			( int mX, int mY, DWORD keys );
	virtual bool 			OnMouseRBUp	( int mX, int mY );
    virtual bool 			OnMouseLBUp	( int mX, int mY );

	DECLARE_SCLASS( VIPMDebugger, Dialog, VIPD );
}; // class VIPMDebugger

/*****************************************************************/
/*	Class:	LODDebugger
/*	Desc:	LOD Manager debugger
/*****************************************************************/
class LODDebugger : public Dialog
{
 
public:
                            LODDebugger		    ();
    virtual void			Render				();
    virtual void			Expose				( PropertyMap& pm );
    virtual bool 			OnMouseLBDown	( int mX, int mY );
    virtual bool 			OnMouseMove			( int mX, int mY, DWORD keys );
    virtual bool 			OnMouseRBUp	( int mX, int mY );
    virtual bool 			OnMouseLBUp	( int mX, int mY );

    DECLARE_SCLASS( LODDebugger, Dialog, LIPD );
}; // class LODDebugger



#endif // __DBLODDER_H__
