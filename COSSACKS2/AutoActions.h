class cva_GS_Player:public vui_Action{
public:
    virtual void SetFrameState(SimpleDialog* SD);
    SAVE(cva_GS_Player)
    REG_PARENT(vui_Action);
    ENDSAVE
};
class cva_GS_Resource:public vui_Action{
public:
    virtual void SetFrameState(SimpleDialog* SD);
    SAVE(cva_GS_Resource)
    REG_PARENT(vui_Action);
    ENDSAVE
};
class cva_GS_FormationDesk:public vui_Action{
public:
    virtual void SetFrameState(SimpleDialog* SD);
    SAVE(cva_GS_FormationDesk)
    REG_PARENT(vui_Action);
    ENDSAVE
};
class cva_GS_UnitsDesk:public vui_Action{
public:
    virtual void SetFrameState(SimpleDialog* SD);
	bool Building;
	SAVE(cva_GS_UnitsDesk){		
		REG_PARENT(vui_Action);
		REG_MEMBER(_bool,Building);
	}ENDSAVE;
};
class cva_GS_Formation:public vui_Action{
public:
    virtual void SetFrameState(SimpleDialog* SD);
    SAVE(cva_GS_Formation)
    REG_PARENT(vui_Action);
    ENDSAVE
};
class cva_GS_Unit:public vui_Action{
public:
    virtual void SetFrameState(SimpleDialog* SD);
    SAVE(cva_GS_Unit)
    REG_PARENT(vui_Action);
    ENDSAVE
};
class cva_GS_PlayerName:public vui_Action{
public:
    virtual void SetFrameState(SimpleDialog* SD);
    SAVE(cva_GS_PlayerName)
    REG_PARENT(vui_Action);
    ENDSAVE
};
class cva_GS_PlayerRace:public vui_Action{
public:
    virtual void SetFrameState(SimpleDialog* SD);
    SAVE(cva_GS_PlayerRace)
    REG_PARENT(vui_Action);
    ENDSAVE
};
class cva_GS_Desk:public vui_Action{
public:
    virtual void SetFrameState(SimpleDialog* SD);
    SAVE(cva_GS_Desk)
    REG_PARENT(vui_Action);
    ENDSAVE
};
class cva_OnMove:public vui_Action{
public:
	cva_OnMove() : vui_Action() { m_pOBJ=NULL; Move=false; };

    virtual bool isLeftClick(){ return true; };
    virtual bool LeftClick(SimpleDialog* SD);
    virtual void OnDraw(SimpleDialog* SD);
	virtual void SetFrameState(SimpleDialog* SD);

	bool	UseName;
	DWORD	GetClassMask(){
				if ( UseName )	return 0x00000001;	// Use name for identify object
								return 0x00000002;	// Use id for identify object
			};
	DWORD	m_id;		// Object id for move
	_str	m_name;		// Object name for move

protected:
	SimpleDialog*	m_pOBJ;							// Moveble object
	bool			FindeObject(SimpleDialog* SD);	// finde object by NAME or ID ( use for LeftClick )
	int				mx0,my0;						// Old mouse position
	void			ShiftDialog(ParentFrame* pPF,int dx,int dy);
	bool			Move;

public:
    SAVE(cva_OnMove)
		REG_PARENT(vui_Action);
		REG_MEMBER(_bool,UseName);
		SAVE_SECTION(0x00000002);
			REG_MEMBER(_int,m_id);
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_str,m_name);
    ENDSAVE
};

class cva_CursorGP:public vui_Action{
public:
	virtual void OnDraw(SimpleDialog* SD);
	
	word				FileID;
	int					SpriteID;

	SAVE(cva_CursorGP)
		REG_PARENT(vui_Action);
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
	ENDSAVE
};
