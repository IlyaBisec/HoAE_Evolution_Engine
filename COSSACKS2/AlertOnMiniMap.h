
#ifndef __AlertOnMiniMap__
#define __AlertOnMiniMap__

class AlertEditor;

#define	 AL_NULL	0
#define	 AL_VIEW	1
#define	 AL_HIDE	2
#define	 AL_DELETE	3
typedef	 AL_FUNC(void* param);

bool	g_AddAlert(char* Name,int x,int y,AL_FUNC* pAL_FUNC=NULL,void* param=NULL,char* MyFullName=NULL);
bool	g_AddAlert(_str  Name,int x,int y,AL_FUNC* pAL_FUNC=NULL,void* param=NULL,char* MyFullName=NULL);
bool	g_SetNewAlertCoord(char* FullName,int x,int y);
void	g_UpdateAlertXY();
void	g_DeleteAllAlert();

// BaseShape /////////////////////////////////////////////////////////////
class Param_BS : public BaseClass{
public:
	Param_BS();

	virtual	Param_BS&	operator=(Param_BS& other);

	SAVE(Param_BS);
	ENDSAVE;
};
class BaseShape : public BaseClass
{
public:
	BaseShape();

	virtual	void	SetParam	(Param_BS* param);
	virtual	void	GetParam	(Param_BS* param);

	virtual	void	SetPos		(float x,float y);
	virtual	void	SetSacale	(float scale);
	virtual	void	SetTurne	(float angle);	
	virtual	void	SetColour	(DWORD color);
	virtual	void	SetAColour	(DWORD color);
	virtual	void	SetAlpha	(DWORD alpha);
	virtual	void	SetVisible	(bool  state);
	virtual	void	SetFlip		(float flip );

	virtual	void	AddPos		(float x,float y);
	virtual	void	AddSacale	(float scale);
	virtual	void	AddTurne	(float angle);	
	virtual	void	AddColour	(DWORD color);
	virtual	void	AddAlpha	(DWORD alpha);
	virtual	void	AddFlip		(float flip );

	virtual	void	GetPos		(float& x,float& y);
	virtual	void	GetPosXY	(float& x,float& y);
	virtual	void	GetSacale	(float& scale);
	virtual	void	GetTurne	(float& angle);	
	virtual	void	GetColour	(DWORD& color);
	virtual	void	GetAColour	(DWORD color);
	virtual	void	GetAlpha	(DWORD& alpha);
	virtual	void	GetVisible	(bool&  state);
	virtual	void	GetFlip		(float& flip );
	virtual char*	GetName		();

	virtual	void	UpdateXY	();	// chenge scroll limit

	virtual	bool	CopyIn		(BaseShape** ppShape);

	virtual	BaseShape&	operator=(BaseShape& other);

	virtual	void	Draw		();
	virtual void	ReStart		();
	virtual	void	DefToCur	();

	_str	NAME;

	// deff data
	float	d_x,d_y;
	float	d_X,d_Y;
	float	d_scale;
	float	d_angle;
	DWORD	d_colour;
	bool	d_visible;
	float	d_flip;

	// dynamic data
	float	m_x,m_y;
	float	m_X,m_Y;
	float	m_scale;
	float	m_angle;
	DWORD	m_colour;
    bool	m_visible;
	float	m_flip;

	SAVE(BaseShape);
		REG_AUTO(NAME);
	ENDSAVE;
};

// LineBasedShape ////////////////////////////////////////////////////////
class Param_LBS : public Param_BS
{
public:
	Param_LBS();

	float	a;
	float	b;
	int		n;
	
	virtual	Param_LBS&	operator=(Param_LBS& other);

	SAVE(Param_LBS);
		REG_PARENT(Param_BS);
		REG_BASE(Param_BS);
		REG_MEMBER(_float,a);
		REG_MEMBER(_float,b);
		REG_MEMBER(_int,n);
	ENDSAVE;
};
class LineBasedShape : public BaseShape
{
public:
	LineBasedShape();

	virtual	void	SetParam	(Param_BS* param);
	virtual	void	GetParam	(Param_BS* param);

	virtual	bool	CopyIn		(BaseShape** ppShape);

	virtual	LineBasedShape&	operator=(LineBasedShape& other);

	virtual	void	Draw		();

	// Base static data
	Param_LBS		Params;
	SubSection		AdvancedParam;

	SAVE(LineBasedShape);
		REG_PARENT(BaseShape);
		REG_BASE(BaseShape);
		REG_AUTO(Params);
		REG_AUTO(AdvancedParam);
		REG_MEMBER(_float,m_X);
		REG_MEMBER(_float,m_Y);
		REG_MEMBER(_float,m_scale);
		REG_MEMBER(_float,m_angle);
		REG_MEMBER(_color,m_colour);
		REG_MEMBER(_bool,m_visible);
		REG_MEMBER(_float,m_flip);
	ENDSAVE;
};

// GPBasedShape //////////////////////////////////////////////////////////
class GPBasedShape : BaseShape
{
public:
	
};
// AlertCOM //////////////////////////////////////////////////////////////
class AlertCOM : public BaseClass
{
public:
	AlertCOM();

	_str	Shape_Name;

	int		Time_Start;
	int		Time_Stop;
	int		COM_Type;
	
	bool	bParam;
	int		iParam;
	float	fParam;
    int		iColorID;	// 0-red,1-green,2-blue

	float	fFI;
	float	speed;
	float	speedR;
	float	speedG;
	float	speedB;
	bool	del;
	int		Time_Total;

	void	ApplyParam(int dT,BaseShape* pShape);
	bool	DeleteMe();
	char*	GetShapeName();
	void	ReStart();
	void	SetStatus(bool status);

	AlertCOM&	operator=(AlertCOM& other);

	DWORD	GetClassMask();

	SAVE(AlertCOM);
		REG_AUTO(Shape_Name);
		REG_MEMBER(_int,Time_Start);
		REG_MEMBER(_int,Time_Stop);
		REG_ENUM(_index,COM_Type,AlertTypeCOM);
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_bool,bParam);
		SAVE_SECTION(0x00000002);
			REG_MEMBER(_int,iParam);
		SAVE_SECTION(0x00000004);
			REG_MEMBER(_float,fParam);
	ENDSAVE;
};

// AlertOBJ //////////////////////////////////////////////////////////////
class AlertOBJ : public BaseClass
{
public:
	AlertOBJ();
	~AlertOBJ();
	
	_str	NAME;
	virtual		const char* GetThisElementView(const char* LocalName);

	int		Time_Start;

	_str	FullName;

	AL_FUNC*	Func;
	void*		param;

	ClassArray<BaseShape>	Shapes;
	ClonesArray<AlertCOM>	COM;

	void		SetPos(int x,int y);
	void		SetStartTime();
	void		SetFunction(AL_FUNC* pAL_FUNC=NULL,void* _param=NULL);
	BaseShape*	GetShape(char* Name);
    	
	bool		AllCOM_Complete();
	void		SetCOM_Staus(bool complite=false);

	void		Draw();
	void		Update();
	void		UpdateXY();
	bool		DeleteMe();

	bool		CopyIn(AlertOBJ** ppAlert);

	SAVE(AlertOBJ);
       	REG_LOADSAVE("Dialogs\\Alert\\","*.Alert.xml");
		REG_AUTO(NAME);
		REG_AUTO(Shapes);
		REG_AUTO(COM);
	ENDSAVE;
};
// AlertStore ////////////////////////////////////////////////////////////
class AlertArray : public BaseClass
{
public:
	ClonesArray<AlertOBJ>	AlertTemplates;

	AlertOBJ*	GetNewAlert(char* name);

	bool		DelAlert(char* name);

	SAVE(AlertArray);
		REG_LOADSAVE("Dialogs\\Alert\\","*.AlertStore.xml");
		REG_AUTO(AlertTemplates);
	ENDSAVE;
};
class AlertStore : public BaseClass
{
public:
	AlertStore();

	_str			AlertStorePath;
	AlertArray		ALERTS;
	
	void	LoadAlertStore();

	AlertOBJ* GetAlert(char* Name);

	SAVE(AlertStore);
		REG_FILEPATH(AlertStorePath,".xml");
		REG_AUTO(ALERTS);
	ENDSAVE;
};
extern AlertStore	AlertSTORE;
// AlertProc /////////////////////////////////////////////////////////////
class AlertProc : public BaseClass
{
public:

	ClassArray<AlertOBJ>	ALERTS;

	bool	AddAlert(AlertOBJ* pAlert);
	bool	AddAlert(char* Name,int x,int y,AL_FUNC* pAL_FUNC=NULL,void* _param=NULL,char* MyFullName=NULL);

	bool	SetNewAlertCoord(char* _FullName,int x,int y);
	
	void	Draw();
	void	Update();
	void	UpdateXY();
	void	Free();
};
extern	AlertProc	AlertPROC;
// AlertEditor ///////////////////////////////////////////////////////////
class AlertEditor : public BaseClass
{
public:
	AlertEditor();

	class Add_Alert : public BaseFunction
	{
	public:
		Add_Alert(){ x=y=5000; AlertName="NoName"; };
		int x,y;
		_str AlertName;
		void EvaluateFunction();
		SAVE(Add_Alert);
			REG_PARENT(BaseFunction);
			REG_AUTO(AlertName);
            REG_MEMBER(_int,x);
			REG_MEMBER(_int,y);
		ENDSAVE;
	} AddAlert;

	class ViewAlertStore : public BaseFunction
	{
	public:
		void EvaluateFunction();
        SAVE(ViewAlertStore);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}	AlretStore;

	SAVE(AlertEditor);
		REG_AUTO(AlretStore);
		REG_AUTO(AddAlert);
	ENDSAVE;
};

extern	AlertEditor gAlertEditor;

// Alert_HANDLER /////////////////////////////////////////////////////////
class Alert_HANDLER : public GameExtension
{
public:
	virtual void OnClassRegistration();
	virtual void OnEditorStart();
	virtual void OnGameStart();
	virtual bool OnMapUnLoading();
	virtual void ProcessingGame();
	virtual void OnDrawOnMiniMap(int x,int y,int Lx,int Ly);
};

void	Alert_InstallExtension();
// ClassRegisteration ////////////////////////////////////////////////////
void	RegAlertClass();

// ClassEditor ///////////////////////////////////////////////////////////
void	EditAlertClass(DWORD _rce_, DWORD _DILOG_EDITOR_);

//////////////////////////////////////////////////////////////////////////

#endif//__AlertOnMiniMap__








