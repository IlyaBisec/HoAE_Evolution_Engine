#ifndef	__MISS__GRAPH__OBJECTS__
#define	__MISS__GRAPH__OBJECTS__

// Cloud dialog
class be_CCloud : public ReferableBaseClass
{
public:
	be_CCloud(){ ds_Init=true; UnitID=-1; UnitUniqID=0xFFFFFFFF; Fi=160; addD=25; };
	~be_CCloud(){};

	// Graphics storage ///////////////////////
	DialogsSystem		ds_MAIN;		// Main DS object
	// Iternal use for initialize DS
	_str				ds_FileName;	
	void				SetDefDSFile();
	bool				ds_Init;
	void				InitDS();		// Safe init function
	// Use for manage object in DS
	int					getCloudTypeNUM();					// return N of DialogDesks in DS
	TextButton*			getTextOBJ(int cloudTypeLID,bool st=false);		// return text object use cloud type local ID
	GPPicture*			getPictureOBJ(int cloudTypeLID);	// return picture object use cloud type local ID
	int					getSubCloudNUM(int cloudTypeLID);	// return number of cloud use cloud type local ID
	GPPicture*			getCloudOBJ(int cloudTypeLID,int i);// return i-th cloud object use cloud type local ID
	int					getProperCloudType(char* _text);	// return id of cloud type for cur text is propere

	void				setRandCloud(int cloudTypeLID);
	void				setText(int cloudTypeLID,char* _text);
	void				setProperPictureOBJ(int cloudTypeLID,char* EmotionName);

	bool				isTextTooLarge(int cloudTypeLID,char* _text);
	int					getLineCount();

	void				hideAll();							// Hide DialogDesk use index
	void				vievCloudType(int cloudTypeLID);	// View DialogDesk use index
	void				centreTextY(int cloudTypeLID);		// Centre text by oY exe
	void				centreTextX(int cloudTypeLID);		// Centre text by ox exe only for smallest
	void				correctForDrawOnlyOnScreenRect(int& _sX,int& _sY,int& _Fi);
	///////////////////////////////////////////
	// Position
	int		startX,startY;		// Position of start draw point
	int		addD,Fi;		
	void	getStartXY(int& _x,int& _y);		// Get start draw position
	void	get_addD_Fi(int& _addD,int& _Fi);	// Get center position for big cloud
	int		getScreenDX();
	int		getScreenDY();
	// Draw
	int		CloudTypeID;
	void	Draw();
	// For Test
	int		UnitID;
	DWORD	UnitUniqID;
	ClonesArray<be_CGROUP>	pSPEAKERS;			// Array of speakers groups
	bool	selectSPEAKER(be_CGROUP* pSP_GR,be_CGROUP* pGR_INIT);	// Set UID in UnitID from pSP_GR - nearest for other GR
	bool	getCenterOfSPEAKERS(int& x,int& y,be_CGROUP* pGR=NULL);
	bool	getCurrentSpeakerPosition(int& x,int& y);
	// User cancel dialog
	static	bool	NeedCancel;
	static	bool	NeedCenter;
	static	bool	SetCanceled(SimpleDialog* pSD);
	static	bool	SetCentered(SimpleDialog* pSD);
	void	SetOnUserClickCallBack();	// Only for big cloud
private:
	float	getRADIAN(int angle);
	void	getCrossdXdY(int& dx,int& dy,int fi,int w,int h);
	bool	getUnitTopXY(int& _x,int& _y);
public:
	SAVE(be_CCloud);
		REG_AUTO(Name);
		REG_FILEPATH(ds_FileName,"DialogsSystem.xml");

		REG_MEMBER(_int,startX);
		REG_MEMBER(_int,startY);

		REG_MEMBER(_int,addD);
		REG_MEMBER(_int,Fi);

		REG_MEMBER(_int,UnitID);
	ENDSAVE;
};

extern be_CCloud	g_beCloudTest;

class	be_CSTR_DIALOG;
class	be_CGROUP;
void	be_gShowCloudDialog(be_CSTR_DIALOG* pSTR,be_CGROUP* pGRP,be_CGROUP* pGR_INIT);
void	be_gShowCloudDialog(be_CSTR_DIALOG* pSTR,be_CGROUP* pGR_INIT);
int		be_gGetLineCountInCloudDialog(be_CSTR_DIALOG* pSTR);
bool	be_gGetCloudDilogCancel();
void	be_gSetCloudDilogCancel(bool state);
bool	be_gGetCloudDilogCenter();
void	be_gSetCloudDilogCenter(bool state);
void	be_gClearCloudDilogSPEAKERS();
void	be_gAddCloudDilogSPEAKERS(be_CGROUP* pSP);
bool	be_getCenterOfSPEAKERS(int& x,int& y,be_CGROUP* pGR=NULL);
bool	be_getCurrentSpeakerPosition(int& x,int& y);

// Camera
void	be_gDRAW_CAMERA(Vector3D& pos, Vector3D& dir, Vector3D& lookAt);

// Info Dialog
//class CInfoDialog : public ReferableBaseClass
//{
//public:
//	CInfoDialog();
//
//	void	SetMessage( char* _message, int _pos ); // 0 <= _pos <= 4
//	
//	_str	Message0;
//	_str	Message1;
//	_str	Message2;
//	_str	Message3;
//	_str	Message4;
//
//	void	Init();
//	void	Update();				// Update messages and positions
//	void	Render();				// Render object
//
//	bool	InitDS;
//	bool	Visible;
//	bool	Empty;
//	bool	HideAnyWay;
//protected:
//	DialogsSystem		ds_MAIN;				// Main DS object
//	bool				LoadDefDS();
//	bool				SetMessageInDS( char* _message, int _pos );
//	_str&				operator[](int _ixd);	// Get current message
//	int					getMaxWidth();
//	int					getMaxHeight();
//public:
//	SAVE(CInfoDialog);
//		REG_AUTO(Message0);
//		REG_AUTO(Message1);
//		REG_AUTO(Message2);
//		REG_AUTO(Message3);
//		REG_AUTO(Message4);
//		REG_MEMBER(_bool,Visible);
//	ENDSAVE;
//};
//
//extern	CInfoDialog		g_beInfoDialog;

class	CTaskInfo	: public BaseClass
{
public:
	_str	m_Title;
	_str	m_Descr;
	_str	m_Task;

	_str	m_Name;

	SAVE( CTaskInfo );
		REG_AUTO( m_Name  );
		REG_AUTO( m_Title );
		REG_AUTO( m_Descr );
		REG_AUTO( m_Task  );
	ENDSAVE;
};

class	CInfoDialog : public ReferableBaseClass
{
public:
	void			init			(char* _name=NULL);
					CInfoDialog		();
					CInfoDialog		(char* _name);
	void			clear			();
					~CInfoDialog	();

	virtual bool	CheckIfObjectIsGlobal() { return true; };

	bool			Process			(
										int mx,
										int my,
										bool& LeftPressed,
										bool& RightPressed,
										int MapCoordX,
										int MapCoordY,
										bool OverMiniMap
									);
	bool			OnRender		();
	void			OnInit			(char* fileName);
	bool			loadDS			(char* fileName);
	DialogsSystem	_mainDS;
	bool			_init;
	bool			_error;

	bool							Visible;

	ClonesArray< CTaskInfo >		m_Tasks;
	void							AddTask(_str& l_name,_str& l_title,_str& l_descr,_str& l_task);
	void							DelTask(_str& l_name);

	SAVE( CInfoDialog );
		REG_BASE(ReferableBaseClass);
		REG_AUTO(Name);

		REG_MEMBER(_bool,Visible);
		REG_AUTO( m_Tasks );
	ENDSAVE;
}; // class CInfoDialog

extern	CInfoDialog		g_beInfoDialog;

void	be_REG_CLASS_MissGraph();

#endif//__MISS__GRAPH__OBJECTS__


















