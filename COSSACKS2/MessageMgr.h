
#ifndef __MessageMgr__
#define __MessageMgr__

class BaseMesMgrST;
class SimpleMesMgr;
class C2CampMesMgr;

void	RegClassBMST();

CEXPORT	void	AddMessageMM(BaseMesMgrST* pMessage,char* ParentName=NULL);
//======================================================================//
//							 BaseMesMgr									//
//======================================================================//
class BaseMesMgrST : public BaseClass
{
public:
	BaseMesMgrST();
	BaseMesMgrST(BaseMesMgrST& MM);	// copy constructor
	virtual ~BaseMesMgrST();

	// General
	virtual		void			AddMessage(BaseMesMgrST* pMessage);
	virtual		bool			AddMessage(BaseMesMgrST* pMessage,char* ParenName);
	virtual		void			SetDelete();				// пометить как удаленный
	virtual		void			ClearDeleted();				// удалить помеченные как удаленные

	virtual		void			SetObjectName(char* Name);

	virtual		bool			HANDLE(	int mx,int my,
										bool& LeftPressed,
										bool& RightPressed,
										int MapCoordX,int MapCoordY,
										bool OverMiniMap);
	virtual		void			DRAW();

	virtual		void			ShowAllMessages();
	virtual		void			HideAllMessages();
	virtual		void			ChangeMessVisibleState();
	virtual		void			SetActive(bool _st);
	virtual		void			SetVisible(bool _st);
	virtual		bool			GetActive();
	virtual		bool			GetVisible();
	virtual		bool			GetChildActive();//grey
	virtual		bool			GetChildVisible();
	virtual		int				GetNMess();

	// Mast be redefinate for derived class
	virtual		void			Icon_Draw();
	virtual		bool			Icon_MouseOver(int mx,int my);
	virtual		bool			Icon_OnMouseLClick(int mx,int my,bool& LeftPressed);
	virtual		bool			Icon_OnMouseRClick(int mx,int my,bool& RightPressed);
	virtual		void			ShowDialog();				// L mouse click
	virtual		void			ProcessMessage();			// mouse over
	virtual		void			ArrangeIcons();
	
	virtual		void			Icon_Sort();
	virtual		void			Icon_SetPos(int  _x,int  _y);
	virtual		void			Icon_GetPos(int& _x,int& _y);
	virtual		void			Icon_GetSize(int& h,int& w);

	// Spesial
	virtual		void			DeleteData();
	virtual		void			InitData();
	virtual		BaseMesMgrST*	FindeObject(char* ObjectName);
	virtual		bool			CompareName(char* ObjectName);

	// Data
	DWORD						InfID;
	_str						Object_Name;		
	ClassArray<BaseMesMgrST>	Messages;			// array of messages
	bool						Deleted;			// пометить как удаленное
	bool						visible;		
	bool						active;
	int							AlignType;
	int							dX;
	int							dY;
	int							x;
	int							y;
	
	// BaseClass save
	SAVE(BaseMesMgrST);
		REG_LOADSAVE("Dialogs\\","*.MessBase.xml");
		REG_AUTO(Object_Name);
		REG_AUTO(Messages);
		REG_MEMBER(_bool,Deleted);
		REG_MEMBER(_bool,visible);
		REG_MEMBER(_bool,active);
		REG_ENUM(_index,AlignType,HintAligningX);
		REG_MEMBER(_int,dX);
		REG_MEMBER(_int,dY);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
	ENDSAVE;
};
//////////////////////////////////////////////////////////////////////////

class SimpleMesMgr : public BaseMesMgrST
{
public:
	SimpleMesMgr();
	SimpleMesMgr(SimpleMesMgr& MM);
	virtual	~SimpleMesMgr();

	// Draw
	virtual		void		Icon_Draw();
	virtual		void		Icon_Lines();
	// System messages
	virtual		bool		Icon_MouseOver(int mx,int my);
	// data
	virtual		void		Icon_GetSize(int& h,int& w);
	// Static data
	word		FileID;
	int			SpriteID;
	DWORD		Colour;
	
	SAVE(SimpleMesMgr);
		REG_PARENT(BaseMesMgrST);
		REG_BASE(BaseMesMgrST);

		REG_LOADSAVE("Dialogs\\","*.MessSimple.xml");

		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_MEMBER(_color,Colour);
	ENDSAVE;
};
//////////////////////////////////////////////////////////////////////////

class C2CampMesMgr : public SimpleMesMgr
{
public:
	C2CampMesMgr();
	C2CampMesMgr(C2CampMesMgr& MM);
	virtual	~C2CampMesMgr();

	virtual void	Init();

	// General
	virtual		bool			AddMessage(BaseMesMgrST* pMessage,char* ParenName);
	virtual		void			SetDelete();				// пометить как удаленный
	virtual		void			ClearDeleted();				// удалить помеченные как удаленные
	
	// Draw
	virtual		void			Icon_Draw();
	virtual		bool			Icon_OnMouseRClick(int mx,int my,bool& RightPressed);

	// process
	virtual		void			ShowDialog();			// L mouse click
	virtual		void			ProcessMessage();		// mouse over
	virtual		void			ProcessMessActions();	// L mouse click ACTIONs

	virtual		void			Icon_Sort();			// sort messages!

	// modificators
	virtual		void			ChangeIcon();

	// Static data
	DWORD		Time_Pulse;	
	int			TypeObj;	// 0-Base, 1-Page, 2-Message
	int			SpriteID0; //very new		// blink   sprite ID
	int			SpriteID1; //very new		// opened  sprite ID
	int			SpriteID2; //very new		// mouse over sprite ID
		
	// dynamic data
	bool		NewMess;	// have new message
	int			CurSprite;
	DWORD		Time_LastUpdate;
	
	// message param
	int			ID;			// message ID
	_str		PageName;	// mess page name//last add
	_str		text;		// message text
	int			param;		// param
	int			state;		// additional mess params 
	int			zoneX;		// pulse/show zone x
	int			zoneY;		// pulse/show zone y

	SAVE(C2CampMesMgr);
		REG_PARENT(SimpleMesMgr);
		REG_BASE(BaseMesMgrST);

		REG_LOADSAVE("Dialogs\\","*.MessCampCII.xml");

		REG_MEMBER(_int,TypeObj);
		REG_MEMBER(_int,Time_Pulse);
		//REG_SPRITE(SpriteID1,FileID); //very new
		//REG_SPRITE(SpriteID2,FileID);
		REG_MEMBER(_int,SpriteID0); //very new
		REG_MEMBER(_int,SpriteID1); //very new
		REG_MEMBER(_int,SpriteID2); //very new
		REG_AUTO( text );
		REG_MEMBER(_int,param);
	ENDSAVE;
};

//////////////////////////////////////////////////////////////////////////

#endif//__MessageMgr__


















