/*****************************************************************************/
/*	File:	edWaypointEditor.h
/*	Desc:	Ragdoll controller
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-22-2003
/*****************************************************************************/
#ifndef __EDWAYPOINTEDITOR_H__
#define __EDWAYPOINTEDITOR_H__



/*****************************************************************************/
/*	Class:	WaypointEditor
/*	Desc:	Performs creating of rails that units are walking on
/*****************************************************************************/
class WaypointEditor : public Dialog
{
	SNode*				m_pModel;

	Vector3D			m_CurBeg;
	Vector3D			m_CurEnd;
	float				m_CurWidth;	
	bool				m_bDragging;
	bool				m_bCreating;
	float				m_Bias;
	float				m_WeldRadius;

	DWORD				m_CoreColor;	
	DWORD				m_Color;		
	int					m_CurSeg;

	int					m_CurMX, m_CurMY;

	std::string			m_SystemName;

public:
					WaypointEditor			();

	virtual bool 	OnMouseLBDown		( int mX, int mY );
	virtual bool 	OnMouseMove				( int mX, int mY, DWORD keys );
	virtual bool 	OnMouseLBUp		( int mX, int mY );

	virtual bool	OnMouseMBDown		( int mX, int mY );
	virtual bool	OnMouseMBUp		( int mX, int mY );
	virtual bool	OnKeyDown				( DWORD keyCode, DWORD flags );
	virtual void	Expose					( PropertyMap& pm );
	virtual void	Render					();

	void			SetColors				( DWORD segClr, DWORD coreClr )
	{
		m_Color = segClr;
		m_CoreColor = coreClr;
	}
	void			SetSystemName			( const char* name ) { m_SystemName = name; }

	DECLARE_SCLASS(WaypointEditor,Dialog,WYED);

protected:
	SNode*			FindModel				();
	void			AddSegment				(	const Vector3D& beg, 
												const Vector3D& end, 
												float width );
	bool			MouseToWorld			( int mX, int mY, Vector3D& pt );
	float			GetCurWidth				();
	void			Weld					( Vector3D& pt );

}; // class WaypointEditor



#endif // __EDWAYPOINTEDITOR_H__
