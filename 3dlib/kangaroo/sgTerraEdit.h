/*****************************************************************************/
/*	File:	sgTerraEdit.h
/*	Desc:	Terrain editing tools
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
#ifndef __SGTERRAEDIT_H__
#define __SGTERRAEDIT_H__



/*****************************************************************************/
/*	Enum:	BrushForm
/*	Desc:	Describes form of the terrain painting brush
/*****************************************************************************/
enum BrushForm
{
	bfNone	    = 0,
	bfSquare	= 1,
	bfRound		= 2
}; // enum BrushForm

/*****************************************************************************/
/*	Enum:	TerrainEditMode
/*	Desc:	Mode of terrain editor
/*****************************************************************************/
enum TerrainEditMode
{
	teUnknown	= 0,
	teHeight	= 1,
	teTexture	= 2,
	teFacture	= 3,
	teTrees		= 4,
	teObjects	= 5
}; // enum EditMode



ENUM( BrushForm, "BrushForm", 
	 en_val( bfSquare,	"Square"  ) <<
	 en_val( bfRound,	"Round"	) );

ENUM( TerrainEditMode, "TerrainEditMode", 
	 en_val( teUnknown	,	"Unknown"	) <<
	 en_val( teHeight	,	"Height"	) <<
	 en_val( teTexture	,	"Texture"	) <<
	 en_val( teFacture	,	"Facture"	) <<
	 en_val( teTrees	,	"Trees"		) <<
	 en_val( teObjects	,	"Objects"	) );



struct TerraObjectInstance
{
	Matrix4D	tm;
	int			id;
}; // struct TerraObjectInstance

/*****************************************************************************/
/*	Class:	TerrainEditor
/*	Desc:	Generates terrain for testing purposes
/*****************************************************************************/
class TerrainEditor : public Dialog
{
public:
						TerrainEditor		();

	void				DisturbPerlin		();
	void				ResetGround			();
	void				FlattenValleys		();
	void				AccentSlopes		();

	virtual void		Render				();
	virtual void		Expose				( PropertyMap& pm );
	virtual void		Serialize			( OutStream& os ) const;
	virtual void		Unserialize			( InStream& is );
	
	virtual bool 		OnMouseLBDown	( int mX, int mY );
	virtual bool 		OnMouseRBDown	( int mX, int mY );
	virtual bool 		OnMouseRBUp	( int mX, int mY );
	virtual bool 		OnMouseMove			( int mX, int mY, DWORD keys );
	virtual bool 		OnMouseLBUp	( int mX, int mY );
	virtual bool 		OnMouseWheel		( int delta );

	DECLARE_SCLASS(TerrainEditor,Dialog,TERG);

protected:
	float				m_PerlinShiftX, m_PerlinShiftY;
	float				m_PerlinScaleX, m_PerlinScaleY;

	float				m_HeightShift;
	float				m_HeightScale;
	float				m_ZeroGroundLevel;

	TerrainEditMode		m_EditMode;

	float				m_BrushRadius;
	DWORD				m_BrushColor;
	BrushForm			m_BrushForm;
	float				m_BrushDecay;
	float				m_BrushRotation;
	Vector3D			m_BrushPosition;
	bool				m_bDrawBrush;
	float				m_HeightDelta;

	Group*				m_pObjectTypes;

	std::vector<TerraObjectInstance>	m_Objects;

	void				DrawBrush			();

}; // class TerrainEditor



#endif // __SGTERRAEDIT_H__
