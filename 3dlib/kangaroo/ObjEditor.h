class ObjEditor:public ModelEditor{
public:	

	DECLARE_CLASS(ObjEditor);

	PolyMesh* Mesh;

	ObjEditor();
	virtual void	OnInit();	
	virtual void	Expose( PropertyMap& pm );

	void			SaveMesh();
	void			LoadMesh();
	void			LoadObj();
	void			SaveNormalMap();
	void			SaveTexture();

	float   		m_PenDepth;
	float   		m_SpecularLevel;
	float   		m_PenRadius;

	bool    		m_LockSpecular;
	bool    		m_LockColor;

	int				m_ColorPaintTexture;
	int				m_ZPaintTexture;
};
extern ObjEditor* m_OED;