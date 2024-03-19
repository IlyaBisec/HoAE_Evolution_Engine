class SetTextureFactor:public SNode{	
	DWORD	Const;
public:
	DWORD						GetColor(){return Const;}
	void						SetColor(DWORD Color){Const=Color;}
	virtual void                Render          ();
    void                        Expose          ( PropertyMap& pm );
    virtual void                Serialize       ( OutStream& os ) const;
    virtual void                Unserialize     ( InStream& is );

	DECLARE_SCLASS(SetTextureFactor,SNode,STTF);
};
class SetColorConst:public SNode{
DWORD	Const;
public:
	DWORD						GetColor(){return Const;}
	void						SetColor(DWORD Color){Const=Color;}
	virtual void                Render          ();
    void                        Expose          ( PropertyMap& pm );
    virtual void                Serialize       ( OutStream& os ) const;
    virtual void                Unserialize     ( InStream& is );

	DECLARE_SCLASS(SetColorConst,SNode,STCC);
};