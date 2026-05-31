
#ifndef __DATA_STORAGE_XML__
#define __DATA_STORAGE_XML__

/************************************************************************/
/* Все класы описанные здесь предназначены исключительно для хранения и */
/* записи в *.xml файлы данных необходимых для скриптовой битвы.		*/
/************************************************************************/

// MATH //////////////////////////////////////////////////////////////////
int		GetCircleDif(int F1,int F2,int maxF=256);
double	lvBE_GetVecFi(double x, double y);	//Угол между вектором (x,y) и (1,0); Fi>=0&&Fi<2*Pi
void	FI_2Pi(float& FI);
// logging messages //////////////////////////////////////////////////////
extern	int __cdecl Msg(LPCSTR format, ...);

void	ggg_WarnigMessage(const char* message);

class	be_CFunctionBaseOLD;

class	lvCGraphObject;
class	lvCBlackScreen;
class	lvCAAppearGP;
class	lvCAnimateGP;

class	CSingleMessage;

class	vvPOINT_SET;

extern	ICamera* ICam;

// MissSET ///////////////////////////////////////////////////////////////
class MissZoneBuild : public BaseClass
{
public:
	MissZoneBuild()	{};
	~MissZoneBuild(){};
	int Nat;
	int Zone;
	bool	UseNode;
	int Node;
//	int Type;
	DWORD	GetClassMask()	{ if (UseNode) return 0x00000002; else return 0x00000001; };

	SAVE(MissZoneBuild);
		REG_MEMBER(_int,Nat);
		REG_MEMBER(_bool,UseNode);
		SAVE_SECTION(0x00000001);
			REG_ENUM(_index,Zone,ALL_ZONES_ON_MAP);
		SAVE_SECTION(0x00000002);
			REG_ENUM(_index,Node,ALL_be_CNODES_ON_MAP);
	ENDSAVE;
};
class MissSET : public BaseClass
{
public:
	MissSET()	{	
					ShowHint=false;
					SetDeff_EXP_ALEX();
				};
	~MissSET()	{};

	bool		ShowHint;
	SubSection	EXPERIENCE;
	float GENK;
	float TK;
	float KK;
	float QK;
    void	SetDeff_EXP_ALEX() { GENK=1.f; TK=1.f; KK=1.f; QK=1.f; };
	class SetDeff_ALEX_EX : public BaseFunction
	{
	public:
		void EvaluateFunction(){
			MissSET* pPAR = get_parent<MissSET>();
			if (pPAR!=NULL) {
				pPAR->SetDeff_EXP_ALEX();
			};
		};
		SAVE(SetDeff_ALEX_EX);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}SetDeff_EXPR_ALEX;

	// Zone for disable buld  building
	SubSection					DisableBuildInZones;
	ClonesArray<MissZoneBuild>	DZone;

	SAVE(MissSET);
		REG_MEMBER(_bool,ShowHint);
		REG_AUTO(EXPERIENCE);
		REG_MEMBER(_float,GENK);
		REG_MEMBER(_float,TK);
		REG_MEMBER(_float,KK);
		REG_MEMBER(_float,QK);
		REG_AUTO(SetDeff_EXPR_ALEX);
		REG_AUTO(DisableBuildInZones);
		REG_AUTO(DZone);
	ENDSAVE;
};
extern	MissSET	gMISS_SET;
// lvCStorage ////////////////////////////////////////////////////////////
class lvCStorage : public ReferableBaseClass
{
public:
	lvCStorage() : ReferableBaseClass() {};
	lvCStorage(lvCStorage* pStorage);
	~lvCStorage()	{
		vDeleteDynamicData();
		vCLEAN();
	};
public:
	_str	ClassName;
	DWORD	InfID;
	_str	FileNameXML;

//	_str	NAME;		// Название объекта.
	_str	DESCR;		// Короткое описание.
public:
	virtual			bool	CheckIfObjectIsGlobal()			{ return true; };

	virtual	void	vINIT (char* CLName=NULL, DWORD ID=0, char* FLName=NULL);
	virtual	void	vCLEAN();
	virtual	void	vDeleteDynamicData();
	virtual	void	vSetClassName	(char* name);
	virtual	void	vSetFileNameXML	(char* name);
	virtual	void	vSetClassID		(DWORD	_id);
	virtual	void	vSaveXML();
	virtual	void	vLoadXML();

	virtual	void	vSetObjectName(char* name);
	virtual	void	vSetDescription(char* description);

	virtual	void	vDraw(DWORD mode=1,DWORD _id=0xFFffFFff) {}; 
	virtual	void	vSetXY(int _x,int _y) {};
	virtual	void	vAddX(int dx=1) {};
	virtual	void	vAddY(int dy=1) {};
	virtual	void	vAddXY(int dx=1,int dy=1) {};

	virtual	void	Prepare_lua(){};
public:
	SAVE(lvCStorage);
		REG_LOADSAVE("temp\\","lvCStorage.xml");
		REG_AUTO(Name);
		REG_AUTO(DESCR);
	ENDSAVE;
};

// be_CNODE ///////////////////////////////////////////////////////////////
// lvCNodeActionsVI //////////////////////////////////////////////////////
class lvCNodeActionsVI
{
public:
	virtual	be_CNODE*	vAddNode(int x,int y,char* name=NULL,char* descr=NULL)	= 0;

	virtual	bool		vDelNodeID(DWORD _id)									= 0;
	virtual	bool		vDelNodePT(be_CNODE* pND)								= 0;
	virtual	bool		vDelNode(int _x,int _y,DWORD sqID=0xFFFFFFFF)			= 0;

	virtual	be_CNODE*	vGetNode(DWORD _id)										= 0;
	virtual	be_CNODE*	vGetNode(int _x,int _y,DWORD sqID=0xFFFFFFFF)			= 0;
};
// CNodesMAP_ST //////////////////////////////////////////////////////////
class SingletonlvCNodesMAP : public lvCStorage
{
public:
	SingletonlvCNodesMAP()				{};
	virtual ~SingletonlvCNodesMAP()	{};
	static SingletonlvCNodesMAP* GetObj()			{ return m_pObj; };
	static void SetObj(SingletonlvCNodesMAP* pObj)	{ m_pObj = pObj; };
	static void Delete() { if (m_pObj!=NULL) { delete m_pObj; m_pObj=NULL; } };
	virtual void Accetss() {};
protected:
	static SingletonlvCNodesMAP* m_pObj;
};

inline SingletonlvCNodesMAP* StlvCNodesMAP(){
	if (SingletonlvCNodesMAP::GetObj()==NULL) SingletonlvCNodesMAP::SetObj(new SingletonlvCNodesMAP);
	return SingletonlvCNodesMAP::GetObj();
};

class lvCNodesMAP_ST : public SingletonlvCNodesMAP, public lvCNodeActionsVI
{
public:
	lvCNodesMAP_ST()	{};
	virtual ~lvCNodesMAP_ST(){
		vDeleteDynamicData();
		vCLEAN();
	};
public:
	ClassArray<be_CNODE>	NodesArray;
public:
	virtual	void	vINIT (char* CLName=NULL, DWORD ID=0, char* FLName=NULL);	// lvCStorage
	virtual	void	vCLEAN();													// lvCStorage
	virtual	void	vDeleteDynamicData();										// lvCStorage
	
	// Personal
	virtual	be_CNODE*vAddNode(int x,int y,char* name=NULL,char* descr=NULL);		// lvCNodeActionsVI
	virtual	bool	vDelNodeID(DWORD _id);										// lvCNodeActionsVI
	virtual	bool	vDelNodePT(be_CNODE* pND);									// lvCNodeActionsVI
	virtual	bool	vDelNode(int _x,int _y,DWORD sqID=0xFFFFFFFF);				// lvCNodeActionsVI
	virtual	void	vDraw(DWORD mode=1,DWORD _id=0xFFffFFff);					// lvCStorage

	virtual	be_CNODE*	vGetNode(DWORD _id);									// lvCNodeActionsVI
	virtual	be_CNODE*	vGetNode(const char* name);								// lvCNodeActionsVI
	virtual	be_CNODE*	vGetNode(int _x,int _y,DWORD sqID=0xFFFFFFFF);			// lvCNodeActionsVI

	class CPRESENTATION	fShowPresentation;
public:
	SAVE(lvCNodesMAP_ST);
		REG_CLASS(lvCStorage);
		REG_PARENT(lvCStorage);
		REG_AUTO(NodesArray);
		REG_LOADSAVE("temp\\","lvCNodesMAP_ST.xml");
		REG_AUTO(fShowPresentation);
	ENDSAVE;
};

inline lvCNodesMAP_ST* NodesMap(){
	if (lvCNodesMAP_ST::GetObj()==NULL){ 
		lvCNodesMAP_ST::SetObj(new lvCNodesMAP_ST);
		lvCNodesMAP_ST::GetObj()->vINIT();
	};
	return (lvCNodesMAP_ST*)lvCNodesMAP_ST::GetObj();
};

be_CNODE* __getNodeByName(const char* nodeName);
be_CNODE* __getNodeByID(const int nodeID);
//======================================================================//
//===========================   vGROUPS    =============================//
//======================================================================//
// lvCGroup //////////////////////////////////////////////////////////////
class lvCGroupSmart : public be_CGROUP
{
public:
	
	ClassArray<lvCBaseScript>	BEHAVIOR_SCRIPTS;	// Поведение отряда

	SAVE(lvCGroupSmart);
		REG_PARENT(be_CGROUP);
		REG_BASE(be_CGROUP);
		REG_AUTO(BEHAVIOR_SCRIPTS);
	ENDSAVE;
};
// lvCGroupsMAP_ST ///////////////////////////////////////////////////////
class SingletonlvCGroupsMAP_ST : public lvCStorage
{
public:
	SingletonlvCGroupsMAP_ST()							{};
	virtual ~SingletonlvCGroupsMAP_ST()					{};
	static SingletonlvCGroupsMAP_ST* GetObj()			{ return m_pObj; };
	static void	SetObj(SingletonlvCGroupsMAP_ST* pObj)	{ m_pObj = pObj; };
	static void Delete()								{ if (m_pObj!=NULL) { delete m_pObj; m_pObj=NULL; } };
	virtual void Accetss()								{};
protected:
	static SingletonlvCGroupsMAP_ST* m_pObj;
};

class lvCGroupsMAP_ST : public SingletonlvCGroupsMAP_ST
{
public:
	lvCGroupsMAP_ST();
	virtual ~lvCGroupsMAP_ST();
public:
	ClassArray<be_CGROUP>	GROUPS;
public:
	virtual	void	vINIT (char* CLName=NULL, DWORD ID=0, char* FLName=NULL);	// lvCStorage
	virtual	void	vCLEAN();													// lvCStorage
	virtual	void	vDeleteDynamicData();										// lvCStorage
					
					// Operation on Groups
	void			AddGroup(char* name);
	void			AddGroupSmart(char* name);
	void			DelGroupNM(char* name);
	void			DelGroupID(DWORD id);
	be_CGROUP*		GetGroupNM(const char* name);
	be_CGROUP*		GetGroupID(DWORD id);

	int				GetGroupAmount();
	bool			GetBrigList( LinearArray<int,_int>& _list, int Nat );
	void			GetStructNameList( LinearArray<int,_int>& _list );

	void			DrawGroup();
	void			SetVisible(bool value);

	void			ReSize();

	ClassArray<be_CGROUP>	GRP_donotMove;				// List of donot move groups
	bool			__CheckMove(OneObject* pOB);
	bool			__AddDonMoveGrp(be_CGROUP* pGrp);
	bool			__RemDonMoveGrp(be_CGROUP* pGrp);
	

	void	BeforeSave();
	void	AfterSave();
	void	BeforeLoad();
	void	AfterLoad();

	class CPRESENTATION	fShowPresentation;

public:
	SAVE(lvCGroupsMAP_ST);
		REG_AUTO(GROUPS);
		REG_AUTO(GRP_donotMove);
		REG_LOADSAVE("temp\\","lvCGroupsMAP_ST.xml");
		REG_AUTO(fShowPresentation);
	ENDSAVE;
};
extern	bool	gg_VGrpReSize;
inline lvCGroupsMAP_ST* GroupsMap(){
	if (lvCGroupsMAP_ST::GetObj()==NULL){ 
		lvCGroupsMAP_ST::SetObj(new lvCGroupsMAP_ST);
		lvCGroupsMAP_ST::GetObj()->vINIT();
	};
	return (lvCGroupsMAP_ST*)lvCGroupsMAP_ST::GetObj();
};
be_CGROUP*	__getGrpByName(const char* sss);
be_CGROUP*	__getGrpByID(int sss);
//======================================================================//
//===========================   vVALUES   ==============================//
//======================================================================//
// Base Value ////////////////////////////////////////////////////////////
class	vvBASE : public ReferableBaseClass
{
public:
	vvBASE()							{ InfID=_vvBASE_; Name="";		SetID(); viewType=0x00000001; visible=false; };
	virtual ~vvBASE()					{				  Name.Clear();								  };

	DWORD	InfID;
	//	_str	Name;
	DWORD	id;
	DWORD	viewType;
	bool	visible;

	virtual			bool	CheckIfObjectIsGlobal()			{ return true; }
					DWORD	GetClassMask()					{ return viewType; }

	virtual			char*	GetName()	const;
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			void*	Get();
	virtual			void	Set(void* value);
	virtual			void	SetName(const char* _Name);
	virtual			void	SetID();
	virtual			bool	CheckName(const char* name);

	virtual			void	Draw();
	virtual			void	DrawOnMiniMap(int x,int y,int Lx,int Ly)	  {};
	virtual			bool	IsInShape(int   x,int   y)		 {return false;};
	virtual			bool	IsInShape(float x,float y)		 {return false;};
	virtual			void	SetPoss(int   x,int   y,int   z) {};
	virtual			void	SetPoss(float x,float y,float z) {};

	
	virtual			bool	OnMouseHandling(int mx,int my,
											bool& LeftPressed,bool& RightPressed,
											int MapCoordX,int MapCoordY,
											bool OverMiniMap					) { return false; };	

	SAVE(vvBASE);
		REG_LOADSAVE("temp\\","vvBASE.xml");
		SAVE_SECTION(0x00000001);
			REG_AUTO(Name);
//			SetReadOnlyMode();
			REG_MEMBER(_DWORD,id);
			REG_MEMBER(_bool,visible);
	ENDSAVE;
};

// Triger ////////////////////////////////////////////////////////////////
// Word //////////////////////////////////////////////////////////////////
// Integer ///////////////////////////////////////////////////////////////
// Text //////////////////////////////////////////////////////////////////
class	vvTEXT : public vvBASE
{
public:
			 vvTEXT()	{ InfID = _vvTEXT_; Text=""; TextID=""; oggFile=""; SpeakerID=""; };
	virtual	~vvTEXT()	{};

	_str	TextID;
	_str	oggFile;
	_str	SpeakerID;
	_str	Text;

	virtual			char*	GetName();
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			void*	Get();
	virtual			void	Set(void* value);

					char*	Get_TextID		()	const;
					char*	Get_oggFile		()	const;
					char*	Get_SpeakerID	()	const;
					void	Set_TextID		(const char* text);
					void	Set_oggFile		(const char* text);
					void	Set_SpeakerID	(const char* text);

	SAVE(vvTEXT);
		REG_LOADSAVE("temp\\","vvTEXT.xml");
		REG_PARENT(vvBASE);
		SAVE_SECTION(0x00000001);
			REG_AUTO(TextID);
			//	REG_AUTO(oggFile);
			REG_FILEPATH(oggFile,".ogg");
			REG_AUTO(SpeakerID);
	ENDSAVE;
};
// Picture ///////////////////////////////////////////////////////////////
class vvPICTURE : public vvBASE
{
public:
			 vvPICTURE();
	virtual	~vvPICTURE();

	word				FileID;			// Файл с картинкой (0xFFFF - ничего нет)
	int					SpriteID;		// Индекс картинки	(-1 - ничего нет)
	int					dx,dy;			// 
	int					lx,ly;			// 

	SubSection			Advanced;

	class SET_DEF_POS : public BaseFunction
	{
	public:
		void	EvaluateFunction();
		SAVE(SET_DEF_POS);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}fSET_DEF_POS;

	virtual			char*	GetName();
	virtual const	char*	GetThisElementView(const char* LocalName);

	// Spesial addition
					bool	GetAsStringForMessage(_str& text);
					int		GetSpriteNUM() const;
					int		GetSpriteID() const;
					void	SetSpriteID(const int sid);

	SAVE(vvPICTURE);
		REG_LOADSAVE("temp\\","vvPICTURE.xml");
		REG_PARENT(vvBASE);
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_gpfile,FileID);
			REG_SPRITE(SpriteID,FileID);

			REG_AUTO(Advanced);
				REG_AUTO(fSET_DEF_POS);
				REG_MEMBER(_int,dx);
				REG_MEMBER(_int,dy);
				REG_MEMBER(_int,lx);
				REG_MEMBER(_int,ly);
	ENDSAVE;
};
// Point2D ///////////////////////////////////////////////////////////////
class POINT2D : public BaseClass {
public:
	POINT2D() { x=y=0; newCoord=true; };
	int x;
	int y;
	bool	newCoord;
	SAVE(POINT2D);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
	ENDSAVE;
};
class vvPOINT2D : public vvBASE
{
public:
			 vvPOINT2D()		{ InfID = _vvPOINT2D_; };
	virtual	~vvPOINT2D()	{};

	POINT2D	Value;

	// Spesial for minimap pointing
	ClassArray<lvCCondition>		Cond;
					bool	CondState();

	virtual			void	Draw();
	virtual			char*	GetName();
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			void*	Get();
	virtual			void	Set(void* value);
					void	Set(int x, int y);

					void	SetTR(int  x, int  y);
					void	GetTR(int& x, int& y);
					bool	NewCoord();

					int		GetX()	const;
					int		GetY()	const;
					void	SetX(const int x);
					void	SetY(const int y);

	SAVE(vvPOINT2D);
		REG_LOADSAVE("temp\\","vvPOINT2D.xml");
		REG_PARENT(vvBASE);
		SAVE_SECTION(0x00000001);
			REG_AUTO(Value);
			REG_AUTO(Cond);
	ENDSAVE;
};

// PointSet //////////////////////////////////////////////////////////////
class vvPOINT_SET : public vvBASE
{
public:
			 vvPOINT_SET()	{ InfID=_vvPOINT_SET_; };
	virtual	~vvPOINT_SET()	{};

	ClassArray<vvPOINT2D>	Value;

	virtual			char*	GetName();
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			void*	Get();
	virtual			void	Set(void* value);
	
					bool	TestPoint(int x,int y);
					bool	TestPoint(vvPOINT2D* pPoint);
					void	CleanARR();
					void	AddPoint(int x,int y);

					int			GetNUM			()					const	;
					vvPOINT2D*	Get_vvPOINT2D	(const int id)				;
		
	class SetAgreeGrp : public BaseFunction
	{
	public:
		int GrpID;
		void EvaluateFunction();
		SAVE(SetAgreeGrp);
			REG_PARENT(BaseFunction);
			REG_ENUM(_index,GrpID,ALL_be_CGROUPS_ON_MAP);
		ENDSAVE;
	} fSetAgreeGrp;

	SAVE(vvPOINT_SET);
		REG_LOADSAVE("temp\\","vvPOINT_SET.xml");
		REG_PARENT(vvBASE);
		REG_AUTO(fSetAgreeGrp);
		SAVE_SECTION(0x00000001);
			REG_AUTO(Value);
	ENDSAVE;
};
// vvVector3D ////////////////////////////////////////////////////////////
class vvVector3D : public vvBASE
{
public:
			 vvVector3D()	{ InfID = _vvVector3D_; };
	virtual	~vvVector3D()	{};

	float	x,y,z;
	int		mX,mY;
	
	virtual			char*	GetName();
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			void*	Get();					// return NULL
	virtual			void	Set(void* value);		// get Vector3D*;

	class SetPos : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(SetPos);
			REG_PARENT(BaseFunction);
		ENDSAVE;
		
	} fSetPos;

	class SetDir : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(SetDir);
			REG_PARENT(BaseFunction);
		ENDSAVE;

	} fSetDir;

	class CPosToLookAt : public BaseFunction
	{
	public:
		ClassRef<vvVector3D>	pos;
		ClassRef<vvVector3D>	dir;

		void EvaluateFunction();
		SAVE(CPosToLookAt);
			REG_PARENT(BaseFunction);
			REG_AUTO(pos);
			REG_AUTO(dir);
		ENDSAVE;
	} PosToLookAt;

	SAVE(vvVector3D);
		REG_LOADSAVE("temp\\","vvVector3D.xml");
		REG_PARENT(vvBASE);
		REG_AUTO(fSetPos);
		REG_AUTO(fSetDir);
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_float,x);
			REG_MEMBER(_float,y);
			REG_MEMBER(_float,z);
			REG_MEMBER(_int,mX);
			REG_MEMBER(_int,mY);
			REG_AUTO(PosToLookAt);
	ENDSAVE;
};

// vvDIALOG //////////////////////////////////////////////////////////////
class vvDIALOG : public vvBASE
{
public:
			 vvDIALOG()	{ InfID = _vvDIALOG_; };
	virtual	~vvDIALOG()	{};

	DialogsSystem	Value;
	
	virtual			char*	GetName();
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			void*	Get();					
	virtual			void	Set(void* value);		

	SAVE(vvDIALOG);
		REG_LOADSAVE("temp\\","vvDIALOG.xml");
		REG_PARENT(vvBASE);
		REG_AUTO(Value);
	ENDSAVE;
};
// vvFuzzyRule //////////////////////////////////////////////////////////
class vvFuzzyRule : public vvBASE
{
public:
	vvFuzzyRule();
	vvFuzzyRule(const char* name, float min0, float min100, float max100, float max0);
	~vvFuzzyRule();

	void	Setup(const char* name, float min0, float min100, float max100, float max0);

	float	IsTrueToWhatDegree(float datapoint) const;
	char*	GetName(void) const;

public:
	void Init(void);
	float m_min100; // smallest number considered 100% true
	float m_max100; // biggest number considered 100% true                                         
	float m_min0;   // smallest number still considered within fuzzy rule
	float m_max0;   // biggest number still considered within fuzzy rule

	SAVE(vvFuzzyRule);
		REG_LOADSAVE("temp\\","vvFuzzyRule.xml");
		REG_PARENT(vvBASE);
		REG_MEMBER(_float,m_min0);
		REG_MEMBER(_float,m_min100);
		REG_MEMBER(_float,m_max100);
		REG_MEMBER(_float,m_max0);
	ENDSAVE;
};
float __FuzzyAND(const vvFuzzyRule* rule1, float data1,const vvFuzzyRule* rule2, float data2);
//////////////////////////////////////////////////////////////////////////
// vvMissionLOG //////////////////////////////////////////////////////////
class vvMissionLOG : public vvBASE
{
public:
	vvMissionLOG();
	~vvMissionLOG();

	// vvBASE  // 
	virtual			char*	GetName();
	virtual const	char*	GetThisElementView(const char* LocalName);

	//  QUEST  ///////////////////////////////////////////////////////////////
	//  STATIC //
	int		QuestN;					// Кол-во квестов на карте
	int		QuestTotalExperience;	// Экспириенс за все квесты
	int		KillsTotalExperience;	// Полный экспириенс за убитых
	int		TimeEverage;			// Среднее время прохождения миссии
	int		TimeEverageExperience;	// Экспа за среднее время прохождения мисии
	//  DYNAMIC //
	int		QuestComleteN;			// Кол-во пройденных квестов
	int		QuestComleteExperience;	// Полученный экспириенс
	int		KillsTotalPseudoExperience;		// Сумма всей псевдо экспы зы убитых по всем игрокам
	int		KillsPlayerPseudoExperience;	// Псевдо экспа игрока за убитых
	int		TimePlayer;						// Время за которое игрок прошел миссию
	//////////////////////////////////////////////////////////////////////////

	void	Clear();								// Очистить

	// SET STATIC DATA  //
	void	SetQuestData(int _QuestN, int _QuestTotalExperience);		// Выставить кол-во квестов и мах Experienc
	void	SetKilsData(int _KillsTotalExperience);						// Выставить экспу за убитых
	void	SetTimeData(int _TimeEverage, int _TimeEverageExperience);	// Выставить среднее время и экспу за это время
	// ADD DYNAMIC DATA  //
	void	AddCopmleteQuest(int Experience);							// Выполнен квест, дать экспу
	void	AddKillsCopmlete();											// Записать экспу за убитых
	void	AddTimeCopmlete();											// Записать время прохождения
	// CREATE AND SAVE RESULT //
	void	WriteToLogClass();											// Пересчитать и записать в глобальный класс все данные (gExpMan()->DATA)
	// SPESIAL  //
	char* GetTimeForamt(int msec,_str& Time);
	int	  GetTimeExp(int Tp,int Tt,int Et);

	vvMissionLOG	operator=(vvMissionLOG& MLOG);

	SAVE(vvMissionLOG);
		REG_PARENT(vvBASE);
		REG_MEMBER(_int,QuestN);
		REG_MEMBER(_int,QuestTotalExperience);
		REG_MEMBER(_int,KillsTotalExperience);
		REG_MEMBER(_int,TimeEverage);
		REG_MEMBER(_int,TimeEverageExperience);

		REG_MEMBER(_int,QuestComleteN);
		REG_MEMBER(_int,QuestComleteExperience);
		REG_MEMBER(_int,KillsTotalPseudoExperience);
		REG_MEMBER(_int,KillsPlayerPseudoExperience);
		REG_MEMBER(_int,TimePlayer);
	ENDSAVE;

};
// STORAGE for VVelue ////////////////////////////////////////////////////
class Singleton_vvMAP_ST : public lvCStorage
{
public:
	Singleton_vvMAP_ST()							{};
	virtual ~Singleton_vvMAP_ST()					{};
	static Singleton_vvMAP_ST* GetObj()				{ return m_pObj; };
	static void	SetObj(Singleton_vvMAP_ST* pObj)	{ m_pObj = pObj; };
	static void Delete()							{ if (m_pObj!=NULL) { delete m_pObj; m_pObj=NULL; } };
	virtual void Accetss()							{};
protected:
	static Singleton_vvMAP_ST* m_pObj;
};

class vvMAP_ST : public Singleton_vvMAP_ST
{	
public:
	vvMAP_ST();
   ~vvMAP_ST();

	ClassArray<vvBASE>				V_VALUES;

	ClassArray<be_CVariableBase>	V_NEW_VALUES;
	virtual bool	AskParentForUsingExpansionClass(char* MemberName,char* ClassName);
	
	virtual	void	vINIT (char* CLName=NULL, DWORD ID=0, char* FLName=NULL);	// lvCStorage
	virtual	void	vCLEAN();													// lvCStorage
	virtual	void	vDeleteDynamicData();										// lvCStorage
						
			vvBASE*	GetVValueID(DWORD _id);
			vvBASE*	GetVValueNM(const char* name);
			vvBASE*	GetVValue(DWORD TYPE_id, DWORD _id);	// Get object by type and id
			vvBASE*	GetNearestValue(float x,float y);
			vvBASE*	GetNearestValue(int   x,int   y);
			vvBASE* GetVValueTypeID(DWORD _tid);

			void	SetViewType(DWORD _type);

			void	Sort();						// Sort by type
			void	AddFirstSeparator(char* _Name,ClassArray<vvBASE>* arrX);
			void	AddSepareteValues(ClassArray<vvBASE>* arrX);
			void	DeleteAllSeparators();

			bool	Is_vvBASE(vvBASE* pVV);

			void	Draw();
			void	DrawOnMiniMap(int x,int y,int Lx,int Ly);

			void	VIEW_OBJS(DWORD classID,bool state);
			bool	VIEW_OBJS(DWORD classID);

	virtual	bool	OnMouseHandling(int mx,int my,bool& LeftPressed,bool& RightPressed,int MapCoordX,int MapCoordY,bool OverMiniMap);
		
	class SORT_BY_TYPE : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(SORT_BY_TYPE);
			REG_PARENT(BaseFunction);
		ENDSAVE();
	} fSORT;

	class CPRESENTATION	fShowPresentation;
			
	SAVE(vvMAP_ST);
		REG_LOADSAVE("temp\\","vvMAP_ST.xml");
		REG_AUTO(fShowPresentation);
		REG_AUTO(fSORT);
		REG_AUTO(V_VALUES);
		REG_AUTO(V_NEW_VALUES);
	ENDSAVE;
};

inline vvMAP_ST* vValuesMap(){
	if (vvMAP_ST::GetObj()==NULL){ 
		vvMAP_ST::SetObj(new vvMAP_ST);
		vvMAP_ST::GetObj()->vINIT();

		ReplaceEditor("BEvValueEDIT",dynamic_cast<BaseClass*>(vvMAP_ST::GetObj()));
	};
	return (vvMAP_ST*)vvMAP_ST::GetObj();
};

template<class RET>	RET* __getValByName(const char* val){
	return	dynamic_cast<RET*>(vValuesMap()->GetVValueNM(val));
};

//======================================================================//
//==========================   vMESSAGE   ==============================//
//======================================================================//
template<class T1,class T2>
class Para : public BaseClass
{
public:
	Para()	{};
	Para(T1& first,T2& second) : first_element(first), second_element(second) {};

	T1	first_element;
	T2	second_element;

	SAVE(Para);
	ENDSAVE;
};
class IIPara : public Para<int,int>	{
public:
	IIPara()	{ first_element=second_element=0; };

	SAVE(IIPara);
		REG_ENUM(_index,first_element,ALL_be_CGROUPS_ON_MAP);
		REG_MEMBER(_int,second_element);
	ENDSAVE;
};
class vvMESSGES : public vvBASE
{
public:
			 vvMESSGES();
	virtual	~vvMESSGES()	{};

	ClonesArray<CSingleMessage>	TASKS;
	_str						ActivateAlert;
	_str						StaticAlert;

	ClassArray<CSingleMessage>	TALKS;
	ClassArray<CSingleMessage>	HINTS;

	ClonesArray< Para<int,int> >	AlertState;

	DialogsSystem	MessDS;			bool	MSD_Visible;
	DialogsSystem	AlertDS;
	DialogsSystem	ArrowDS;
    
	bool			ActiveteTilda;	// if true то можно скрыть меню нажатием "~"

	virtual			char*	GetName();
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			void	Draw();
	virtual			void	DrawOnMiniMap(int x,int y,int Lx,int Ly);

					bool	SetMessDS_Visbility(bool val);
					bool	GetMessDS_Visbility();

					bool	GetTaskState(const int Task_MessUID,bool& visible,bool& deleted);

					void	Desk_Load	(char* FilePath=NULL);	// загрузить стандартный ListDesk
					void	Desk_Create	();
					void	Desk_Update	();
					void	Desk_Sort	();
					void	Desk_Draw	();
				ListDesk*	Desk_GetList();
					void	Delete_Enable(bool activate);
					void	Desk_SetHeight();

			DialogsDesk*	Data_GetObj	();
			TextButton*		Data_GetTextBT();
			SimpleDialog*	Data_GetCanselBT();
					void	Data_Visible(bool state);
					void	Data_Update_Position();
					void	Data_SetMessage(int TaskID);
					void	Data_SetMessage(char* TextID);

					void	Alert_Clear();
					void	Alert_AddTASKS();
					void	Alert_Update();
					void	AddArrowOnMiniMap(int _TID,int _x,int _y,bool _visible);

					void	ShowTask(int TaskID);

					bool	SeparVisib;
					void	Separator_Visible(bool state);
					void	Draw_Separator();
			GPPicture*		GetSeparator();
					
					void	AddHintInList(bool add);
					void	RemHintInList(bool rem);
					void	HideHint(DWORD id);

			GPPicture*		GetAnimARROW();
					void	AddArrowOnMap(int _TID,int _x,int _y,bool _visible);
					void	ArrowOnMapVisible(bool _vvv);
			
	virtual			bool	OnMouseHandling(int mx,int my,
											bool& LeftPressed,bool& RightPressed,
											int MapCoordX,int MapCoordY,
											bool OverMiniMap					);

	SAVE(vvMESSGES);
		REG_LOADSAVE("temp\\","vvMESSGES.xml");
		REG_PARENT(vvBASE);
		REG_AUTO(TASKS);
		REG_AUTO(TALKS);
		REG_AUTO(HINTS);
		REG_AUTO(ActivateAlert);
		REG_AUTO(StaticAlert);
		REG_MEMBER(_bool,ActiveteTilda);
	ENDSAVE;
};
extern	bool	CSingleMessagereCreate;
vvMESSGES*	g_TASK_HINT_OBJ();
// vvMISSMGR /////////////////////////////////////////////////////////////
bool	vbRESET_RCLIC(SimpleDialog* SD);
bool	vbSTPS_LCLIC(SimpleDialog* SD);
bool	vbREST_LCLIC(SimpleDialog* SD);
bool	vbNEXT_LCLIC(SimpleDialog* SD);
class vvMISSMGR : public vvBASE
{
public:
	vvMISSMGR();
	~vvMISSMGR();

	virtual			void	Draw();

    _str			DS_FILE;			// DS file name
	DialogsSystem	DS_MENU;			// global DS object
	bool			DS_LOAD();			// load menu
	bool			DS_PREP();			// prepare for work

	bool			DS_DRAW();			// draw DS object

	bool			DS_VISB(bool _st);	// DS visibility

	DialogsDesk*	DS_GetDesck();
	GP_TextButton*	DS_GetElement(char* name);		// Get element from DS by name

	int		DS_X;	// Position from right 
	int		DS_Y;	// Position from top
	void			DS_SETPOS();

	// Start/Pause ///////////////////////////////////////////////////////////////////////
	bool						STPS_PREP();	// Prepare Start/Pause
	
	bool						STPS_ENABLED(bool _st); // Start/Pause active state

	ClassArray<lvCBaseScript>	STPS_SCRIPT;	// script for Start/Pause
	void						STPS_PROC();	// process Start/Pause script

	bool						stps_ANIM;		// Animate button
	bool						STPS_ANIM(bool _st);
	//////////////////////////////////////////////////////////////////////////////////////

	// Restart ///////////////////////////////////////////////////////////////////////////
	bool						REST_PREP();	// Prepare Restart

	bool						REST_ENABLED(bool _st); // Restart active state

	ClassArray<lvCBaseScript>	REST_SCRIPT;	// script for Restart
	void						REST_PROC();	// process Restart script

	bool						rest_ANIM;		// Animate button
	bool						REST_ANIM(bool _st);
	//////////////////////////////////////////////////////////////////////////////////////

	// Next //////////////////////////////////////////////////////////////////////////////
	bool						NEXT_PREP();	// Prepare Next

	bool						NEXT_ENABLED(bool _st); // Next active state

	ClassArray<lvCBaseScript>	NEXT_SCRIPT;	// script for Next
	void						NEXT_PROC();	// process Next script

	bool						next_ANIM;		// Animate button
	bool						NEXT_ANIM(bool _st);
	//////////////////////////////////////////////////////////////////////////////////////

	int							lastATime;		// Last chenge animation time
	int							AnimDelay;		// Delay betveen animate state
	void						markTIME();
	bool						checkDALAY(int curTIME);
	void						ANIMATE_BTN(GP_TextButton* pBTN,bool curST,bool animate);
	void						ANIMATE();		// Animate selected buttons

	class MM_INIT : public BaseFunction
	{
	public:
		bool	SP_EN;
		bool	RS_EN;
		bool	NX_EN;
		void	EvaluateFunction();
		SAVE(MM_INIT);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_bool,SP_EN);
			REG_MEMBER(_bool,RS_EN);
			REG_MEMBER(_bool,NX_EN);
		ENDSAVE;
	}fINIT;

	SAVE(vvMISSMGR);
		REG_LOADSAVE("temp\\","vvMISSMGR.xml");
		REG_PARENT(vvBASE);
		REG_FILEPATH(DS_FILE,".xml");
		REG_MEMBER(_int,DS_X);
		REG_MEMBER(_int,DS_Y);
		REG_MEMBER(_int,AnimDelay);
		REG_MEMBER(_bool,stps_ANIM);
		REG_AUTO(STPS_SCRIPT);
		REG_MEMBER(_bool,rest_ANIM);
		REG_AUTO(REST_SCRIPT);
		REG_MEMBER(_bool,next_ANIM);
		REG_AUTO(NEXT_SCRIPT);
		REG_AUTO(fINIT);
	ENDSAVE;
};
// vvBrigAI //////////////////////////////////////////////////////////////
class AI_BORDER : public BaseClass
{
public:
	enum AI_BR_ORDER_TYPE { 
		ai_STOP=0, 
		ai_MOVE=1 
	}CURRENT_ORDER;		// Order type
	DWORD	CreateTime;	// Creation time (msec)
	DWORD	ImplemTime;	// Implementation time (0 - not implemented) (msec)

	DWORD	GetClassMask();

	// ai_MOVE
	int		dest_x;			// Destination x
	int		dest_y;			// Destination y
	int		dest_Dir;		// Destination Dir

	_str	OrderName;
	_str	CreateTmBC;
	_str	ImplemTmBC;
	void	setRaport();
	void	setTimeInHMSMs(_str& strval,DWORD time);
	class AI_RAPORT : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(AI_RAPORT);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}fUPDATE;

	SAVE(AI_BORDER);
		REG_AUTO(fUPDATE);
		REG_AUTO(OrderName);
		REG_AUTO(CreateTmBC);
		REG_AUTO(ImplemTmBC);
	SAVE_SECTION(0x00000002);	// ai_MOVE
		REG_MEMBER(_int,dest_x);
		REG_MEMBER(_int,dest_y);
		REG_MEMBER(_int,dest_Dir);
	ENDSAVE;
};
class vvBrigAI : public vvBASE
{
public:
	vvBrigAI();
	~vvBrigAI();
public:
	bool	ACTIVE;			// Object is active
	// installation-specific settings ////////////////////////////////////
	SubSection	SETTINGS;
	int			d_Time;				// Delta time for update params
	int			d_x;				// Delta for x to dest x
	int			d_y;				// Delta for y to dest y
	int			d_Dir;				// Delta for Dir to dest Dir
	void		setDefSettings();	// Set settings in defoult
    //////////////////////////////////////////////////////////////////////

	// curent brigade state //////////////////////////////////////////////
	DWORD	destTime;		// destination time for update

	SubSection	GENERAL;
	int						AI_BrigID;		// Controled brigade id
	LinearArray<int,_int>	EN_BrigID_LST;	// Enemy brigade id list
	bool	setAI_BrigID(int _id);	// Set AI controled brigade id
	bool	addEN_BrigID(int _id);	// add EN brigades id

	SubSection	DESTENATION;
	int		dest_EN;		// Destination enemy brigade id from EN_BrigID_LST

	SubSection	STATE;
    bool	_posChanged;		// Destination position is changed
	bool	_posUnachievable;	// Destination position is unachievable
	bool	_posReach;			// Destination position is reach
	//////////////////////////////////////////////////////////////////////

	// order /////////////////////////////////////////////////////////////
	SubSection	LOCAL_ORDER;
	AI_BORDER	CurOrder;	// Current order
	//////////////////////////////////////////////////////////////////////

	void	UpdateState();		// Update state param
	void	CreateOrderList();	// Create new order if need
	void	ImplementOrdes();	// Implament order

	void	PROCESS();			// General process function

	// SAVE SECTION //////////////////////////////////////////////////////
	SAVE(vvBrigAI);
		REG_PARENT(vvBASE);
		REG_MEMBER(_bool,ACTIVE);
		REG_AUTO(SETTINGS);
			REG_MEMBER(_int,d_Time);
			REG_MEMBER(_int,d_x);
			REG_MEMBER(_int,d_y);
			REG_MEMBER(_int,d_Dir);
		REG_AUTO(GENERAL);
			REG_MEMBER(_int,AI_BrigID);
			REG_AUTO(EN_BrigID_LST);
		REG_AUTO(DESTENATION);
			REG_MEMBER(_int,dest_EN);
		REG_AUTO(STATE);
			REG_MEMBER(_bool,_posChanged);
			REG_MEMBER(_bool,_posUnachievable);
			REG_MEMBER(_bool,_posReach);
		REG_AUTO(LOCAL_ORDER);
			REG_CLASS(AI_BORDER);
			REG_AUTO(CurOrder);
	ENDSAVE;
	//////////////////////////////////////////////////////////////////////
};
// OperationMesMgr ///////////////////////////////////////////////////////
class OperationMesMgr : public SimpleMesMgr
{
public:
	OperationMesMgr();
	OperationMesMgr(OperationMesMgr& MM);
	~OperationMesMgr();

	virtual		void		ShowDialog();				// L mouse click

	ClassArray<lvCBaseScript>	SCRIPT;		// Выполнить по нажатию на кнопку.

	SAVE(OperationMesMgr);
		REG_LOADSAVE("temp\\","OperationMesMgr.xml");
		REG_PARENT(SimpleMesMgr);
		REG_BASE(BaseMesMgrST);
		REG_AUTO(SCRIPT);
	ENDSAVE;
};

//======================================================================//
//======================================================================//
// lvCScriptProcVI ///////////////////////////////////////////////////////
class lvCScriptProcVI
{
public:
	virtual	bool	Process(int time)									= 0;
	virtual	bool	Process(int time,int edgeID)						= 0;
	virtual	bool	Process(int time,int squadID,int edgeID)			= 0;
	virtual	int		UpdateEdgeTime(int time, DWORD ND_ID=0xFFFFFFFF)	= 0;
};
// lvCEdge ///////////////////////////////////////////////////////////////
class lvCEdge : public	lvCStorage, public lvCScriptProcVI
{
public:
	lvCEdge()	{selected=false;};
	lvCEdge(lvCEdge* pEdge);
	~lvCEdge()	{
		vDeleteDynamicData();
		vCLEAN();
	};
public:
	int		begID;		// Начало ребра.
	int		endID;		// Конец ребра.
	int		procTime;	// Время выполнения.
	int		startTime;	// Сумма времени выполнения предыдущих.
	ClassPtr<lvCCondition>		SelectCondition;
	ClonesArray<lvCCondForOper>	CondForOper;
	ClassPtr<lvCCondition>		StopCondition;
	DWORD	id;
	bool	selected;
public:
	virtual	void	vINIT (char* CLName=NULL, DWORD ID=0, char* FLName=NULL);
	virtual	void	vCLEAN();
	virtual	void	vDeleteDynamicData();

	// Personal
	virtual	void	vSetID(DWORD _id);
	virtual	void	vSetBegEndNodeID(int _beg,int _end);	
	virtual	void	vSetBegEndNodePT(be_CNODE* _beg,be_CNODE* _end);
	virtual	void	vSetBeg(int _x,int _y);
	virtual	void	vSetEnd(int _x,int _y);
	virtual	void	vSetBeg(int _ndID);
	virtual	void	vSetEnd(int _ndID);
	virtual	void	vSetProcTime(int _pT);
	virtual	void	vSetStartTime(int _sT);
	virtual	void	vSetMODE(DWORD _mode);
	virtual	void	vSetEdgeID(int _beg,int _end,int _pT,int _sT,DWORD _mode,DWORD _id=0,char* name=NULL,char* descr=NULL);
	virtual	void	vSetEdgePT(be_CNODE* _beg,be_CNODE* _end,int _pT,int _sT,DWORD _mode,DWORD _id=0,char* name=NULL,char* descr=NULL);

	virtual	bool	vCheckEdge();

	virtual	void	vDrawDirection(int _x,int _y,int& maxLen,DWORD& pos,bool setLen);
	virtual	void	vDrawProcTime(int _x,int _y,int& maxLen,DWORD& pos,bool setLen);
	virtual	void	vDrawStartTime(int _x,int _y,int& maxLen,DWORD& pos,bool setLen);
	virtual	void	vDarwMode(int _x,int _y,int& maxLen,DWORD& pos,bool setLen);
	virtual	void	vDarwData(int _x, int _y,float _cosF,float _sinF,DWORD mode=1);
	virtual	void	vDraw(DWORD mode=1,DWORD _id=0xFFffFFff);

	virtual	float	vGetEdgeDist(int _x,int _y);
	virtual	float	vGetEdgeDistDirected(int _x,int _y);	// Знак указывает с какой стороны от Edge кликнули мышкой
	virtual	void	GetCopy(lvCStorage** pCopy);

	virtual	void	vSendNodesXYtoOperations();
	virtual	void	vSendSquardID(DWORD sqID);

	// lvCScriptProcVI
	virtual	bool	Process(int time);
	virtual	bool	Process(int time,int edgeID)				{return	true;};
	virtual	bool	Process(int time,int squadID,int edgeID)	{return	true;};
	virtual	int		UpdateEdgeTime(int time, DWORD ND_ID=0xFFFFFFFF);

	virtual	int		Coplite();

	virtual	void	RestartEdge();	// Set all comand ::first=true

public:
	SAVE(lvCEdge);
		REG_LOADSAVE("temp\\","lvCEdge.xml");
		REG_PARENT(lvCStorage);
		REG_MEMBER(_int,begID);
		REG_MEMBER(_int,endID);
		REG_MEMBER(_int,procTime);
		REG_MEMBER(_int,startTime);
		SetReadOnlyMode();
		REG_MEMBER(_DWORD,id);
		REG_AUTO(SelectCondition);
		REG_AUTO(CondForOper);
		REG_AUTO(StopCondition);
	ENDSAVE;
};
// lvCSquardShema ////////////////////////////////////////////////////////
class lvCSquardShema : public lvCStorage, public lvCNodeActionsVI, public lvCScriptProcVI
{
public:
	lvCSquardShema() { Use_vGRP=false; };
	lvCSquardShema(lvCSquardShema* pSquardShema);
	~lvCSquardShema(){
		vDeleteDynamicData();
		vCLEAN();
	};
public:
	_str				SquardName;		// Название группы.
	int					SquardID;		// Номер отряда, если это отряд.
	bool				Use_vGRP;		// Для vGroup
	ClassArray<lvCEdge>	SquardEdges;	// Схема передвижения.
public:
	virtual	void	vINIT (char* CLName=NULL, DWORD ID=0, char* FLName=NULL);	// lvCStorage
	virtual	void	vCLEAN();													// lvCStorage
	virtual	void	vDeleteDynamicData();										// lvCStorage

	// Interface lvCNodeActionsVI
	virtual	be_CNODE*vAddNode(int x,int y,char* name=NULL,char* descr=NULL);		// lvCNodeActionsVI

	virtual	bool	vDelNodeID(DWORD _id);										// lvCNodeActionsVI
	virtual	bool	vDelNodePT(be_CNODE* pND);									// lvCNodeActionsVI
	virtual	bool	vDelNode(int _x,int _y,DWORD sqID=0xFFFFFFFF);				// lvCNodeActionsVI

	virtual	be_CNODE*	vGetNode(DWORD _id);									// lvCNodeActionsVI
	virtual	be_CNODE*	vGetNode(int _x,int _y,DWORD sqID=0xFFFFFFFF);			// lvCNodeActionsVI
		
	// Personal
	virtual	DWORD	vGetFreeEdgeID();
	virtual	bool	vAddEdgeID(int _beg,int _end,int _pT,int _sT,DWORD _mode,char* name=NULL,char* descr=NULL);
	virtual	bool	vAddEdgePT(be_CNODE* _beg,be_CNODE* _end,int _pT,int _sT,DWORD _mode,char* name=NULL,char* descr=NULL);
	virtual	bool	vAddEdge(lvCEdge* pED);
	virtual	bool	vDelEdgeID(DWORD _id);
	virtual	bool	vDelEdgePT(lvCEdge* pED);
	virtual	bool	vDelEdge(int _x,int _y);
	virtual	void	vDraw(DWORD mode=1,DWORD _id=0xFFffFFff);					// lvCStorage
	virtual void	vDrawMyNodes(DWORD mode=1);

	virtual	lvCEdge*	vGetEdgeID(DWORD _id);
	virtual	int			vGetEdgePT(lvCEdge* pED);
	virtual	lvCEdge*	vGetEdge(int _x,int _y);

			DWORD		GetEdgeEndedIn(DWORD end);
			int			GetEdgeBeginIn(DWORD beg);
			void		GetEdgeBeginIn(lvCEdge** pEdges,int N,DWORD beg);
	virtual	DWORD		vGetProbablyFirstNodeID();
	virtual	DWORD		vGetNearestNode();

	virtual	void		GetCopy(lvCStorage** pCopy);
	virtual	void		Select(bool _select);

	virtual	void		vUpdateEdges();
	virtual	void		vRestartEdges();	// Set all first in true

	// lvCScriptProcVI
	virtual	bool	Process(int time);
	virtual	bool	Process(int time,int edgeID)				{return	true;};
	virtual	bool	Process(int time,int squadID,int edgeID)	{return	true;};
	virtual	int		UpdateEdgeTime(int time, DWORD ND_ID=0xFFFFFFFF);

public:
	SAVE(lvCSquardShema);	
		REG_LOADSAVE("temp\\","lvCSquardShema.xml");
		REG_CLASS(lvCStorage);
		REG_PARENT(lvCStorage);
		REG_AUTO(SquardName);
		REG_MEMBER(_int,SquardID);
		REG_MEMBER(_bool,Use_vGRP);
		REG_AUTO(SquardEdges);
	ENDSAVE;
};
// lvCSquardsOnMap ///////////////////////////////////////////////////////
class lvCSquardsOnMap : public	ProcEnumerator
{
public:
	virtual DWORD GetValue(const char* ID);
	virtual char* GetValue(DWORD ID);
	virtual int   GetAmount();
	virtual char* GetIndexedString(int idx);
	virtual DWORD GetIndexedValue (int idx);
};
// lvCZonesOnMap /////////////////////////////////////////////////////////
class lvCZonesOnMap : public	ProcEnumerator
{
public:
	virtual DWORD GetValue(const char* ID);
	virtual char* GetValue(DWORD ID);
	virtual int   GetAmount();
	virtual char* GetIndexedString(int idx);
	virtual DWORD GetIndexedValue (int idx);
};
// lvCBattleShema_ST /////////////////////////////////////////////////////
class SingletonlvCBattleShema : public lvCStorage
{
public:
	SingletonlvCBattleShema()			{};
	virtual ~SingletonlvCBattleShema()	{};
	static SingletonlvCBattleShema* GetObj()			{ return m_pObj; };
	static void SetObj(SingletonlvCBattleShema* pObj)	{ m_pObj = pObj; };
	static void Delete() { if (m_pObj!=NULL) { delete m_pObj; m_pObj=NULL; } };
	virtual void Accetss() {};
protected:
	static SingletonlvCBattleShema* m_pObj;
};

inline SingletonlvCBattleShema* StlvCBattleShema(){
	if (SingletonlvCBattleShema::GetObj()==NULL) SingletonlvCBattleShema::SetObj(new SingletonlvCBattleShema);
	return SingletonlvCBattleShema::GetObj();
};

class lvCBattleShema_ST : public SingletonlvCBattleShema, public lvCNodeActionsVI, public lvCScriptProcVI
{
public:
	lvCBattleShema_ST() {};
	virtual ~lvCBattleShema_ST(){
		vDeleteDynamicData();
		vCLEAN();
	};
public:
	ClassArray<lvCSquardShema>	BattlePlan;
	ClassArray<lvCSquardShema>	vGRP_Plan;
public:
	virtual	void	vINIT (char* CLName=NULL, DWORD ID=0, char* FLName=NULL);
	virtual	void	vCLEAN();
	virtual	void	vDeleteDynamicData();

	virtual ClassArray<lvCSquardShema>*	GetPlan();

	// FOR vGROUP
	virtual	void	AddPlan_vGRP(be_CGROUP* pGRP);
	virtual	void	DelPlan_vGRP(be_CGROUP* pGRP);
	
	virtual	void	vDraw(DWORD mode=1,DWORD _id=0xFFffFFff);
	virtual	void	vDrawMyNodes(DWORD _id,DWORD mode=1);

	virtual	bool	vDelEdge(int _GrpID,int _x,int _y);
	virtual	bool	vAddEdge(int _GrpID,int _begND_ID,int _endND_ID);
	virtual	bool	vAddEdge(int _GrpID,lvCEdge* pED);
	virtual	lvCEdge*	vGetEdge(int _GrpID,int _x,int _y);

	virtual	void	SelectSquardShema(int _GrpID,bool _select);

	// Interface lvCNodeActionsVI
	virtual	be_CNODE*vAddNode(int x,int y,char* name=NULL,char* descr=NULL);		// lvCNodeActionsVI
	virtual	bool	vDelNodeID(DWORD _id);										// lvCNodeActionsVI
	virtual	bool	vDelNodePT(be_CNODE* pND);									// lvCNodeActionsVI
	virtual	bool	vDelNode(int _x,int _y,DWORD sqID=0xFFFFFFFF);				// lvCNodeActionsVI
	virtual	be_CNODE*	vGetNode(DWORD _id);									// lvCNodeActionsVI
	virtual	be_CNODE*	vGetNode(int _x,int _y,DWORD sqID=0xFFFFFFFF);			// lvCNodeActionsVI
	
	virtual	lvCSquardShema*	vGetSqShemaID(int _ID);
	virtual	lvCSquardShema*	vGetSqShemaCH(char* _nameID);
	virtual	void			vUpdateGroups();

	// lvCScriptProcVI
	virtual	bool	Process(int time)							{return	true;};
	virtual	bool	Process(int time,int edgeID)				{return	true;};
	virtual	bool	Process(int time,int squadID,int edgeID)	{return	true;};
	virtual	int		UpdateEdgeTime(int time, DWORD ND_ID=0xFFFFFFFF)	{return	true;};
public:
	SAVE(lvCBattleShema_ST);
		REG_LOADSAVE("temp\\","lvCBattleShema_ST.xml");
		REG_CLASS(lvCStorage);
		REG_PARENT(lvCStorage);
		REG_AUTO(BattlePlan);
		REG_AUTO(vGRP_Plan);
	ENDSAVE;
};

inline lvCBattleShema_ST* BattleShema(){
	if (lvCBattleShema_ST::GetObj()==NULL){ 
		lvCBattleShema_ST::SetObj(new lvCBattleShema_ST);
		lvCBattleShema_ST::GetObj()->vINIT();
	};
	return (lvCBattleShema_ST*)lvCBattleShema_ST::GetObj();
};

// lvCProcSquad //////////////////////////////////////////////////////////
class lvCProcSquad : public BaseClass
{
public:
	_str	SQ_NAME;
	int		SQUARD;
	bool	Active;
	DWORD	NodeID;
	DWORD	EdgeID;
	int		TimeInProc;
public:
	SAVE(lvCProcSquad);
		REG_LOADSAVE("temp\\","lvCProcSquad.xml");
		REG_AUTO(SQ_NAME);
		REG_MEMBER(_int,SQUARD);
		REG_MEMBER(_bool,Active);
		REG_MEMBER(_DWORD,NodeID);
		REG_MEMBER(_DWORD,EdgeID);
		REG_MEMBER(_int,TimeInProc);
	ENDSAVE;
};
// lvCMainScript /////////////////////////////////////////////////////////
class lvCMainScript : public lvCStorage
{
public:
	lvCMainScript() { mastInit=true; };
	virtual ~lvCMainScript(){
		vDeleteDynamicData();
		vCLEAN();
	};
public:
	bool						mastInit;
	ClassArray<lvCBaseScript>	MAIN_INIT;
	ClassArray<lvCBaseScript>	MAIN_SCRIPTS;
	be_CBaseUnaryStatement		MAIN_NEW_SCRIPT;

public:
	virtual	void	vINIT (char* CLName=NULL, DWORD ID=0, char* FLName=NULL);
	virtual	void	vCLEAN();
	virtual	void	vDeleteDynamicData();

	virtual bool	Process(int time);
	virtual	bool	ProcessInit(int time);
	virtual	void	Draw();

	virtual	void	Prepare_lua();

	class CPRESENTATION	fSavePresentation;

public:
	SAVE(lvCMainScript);
		REG_LOADSAVE("temp\\","lvCMainScript.xml");
		REG_CLASS(lvCStorage);
		REG_PARENT(lvCStorage);
		REG_AUTO(fSavePresentation);
//		REG_MEMBER(_bool,mastInit);
//		REG_AUTO(MAIN_INIT);
//		REG_AUTO(MAIN_SCRIPTS);
		REG_AUTO(MAIN_NEW_SCRIPT);
	ENDSAVE;
};
// lvCFilm ///////////////////////////////////////////////////////////////
class lvCFilm : public ReferableBaseClass
{
public:
	lvCFilm()	{ScriptComplit=false; ScriptInProcess=false;};
	~lvCFilm()	{
		vDeleteDynamicData();
		vCLEAN();
	};
	
	DWORD							InfID;
	_str							DESCR;
	ClassArray<lvCCondition>		StartCondition;			// Условия запуска скриптового ролика.
	ClassArray<lvCOperation>		PrepareOperation;		// Операции выполняемые перед роликом.
	ClonesArray<lvCCondForOper>		STEP_CON_OPR;			// Пошаговое выполнение ролика.
	ClassArray<lvCCondition>		StopCondition;			// Условия прекращения скиптового ролика.
	ClassArray<lvCOperation>		DestroyOperation;		// Опереация необходимые для корректного завершения ролика.
	bool							ScriptComplit;			// Скриптовый ролик проигран и больше не обрабатывается.
	bool							ScriptInProcess;

	virtual	bool	CheckIfObjectIsGlobal()			{ return true; }
	virtual	void	vINIT (char* CLName=NULL, DWORD ID=0, char* FLName=NULL);
	virtual	void	vCLEAN();
	virtual	void	vDeleteDynamicData();

	virtual	bool	IsInProcess();
	virtual	bool	Process(int time);
	
	SAVE(lvCFilm);
		REG_LOADSAVE("temp\\","lvCFilm.xml");
		REG_AUTO(Name);
		REG_AUTO(DESCR);
		REG_AUTO(StartCondition);
		REG_AUTO(PrepareOperation);
		REG_AUTO(STEP_CON_OPR);
		REG_AUTO(StopCondition);
		REG_AUTO(DestroyOperation);
		SetReadOnlyMode();
		REG_MEMBER(_bool,ScriptComplit);
		SetReadOnlyMode();
		REG_MEMBER(_bool,ScriptInProcess);
	ENDSAVE;
};
// lvCGraphObject ////////////////////////////////////////////////////////
class lvCGraphObject : public ReferableBaseClass
{
public:
	lvCGraphObject()	{ InfID=_lvCGraphObject_; Visible=false; };
	virtual	~lvCGraphObject()	{};

	DWORD	InfID;
	bool	Visible;

	virtual	void	DRAW()					{};

	virtual	void	StartShow()				{};
	virtual	void	StopShow()				{};
	virtual	void	SetVisible(bool _vs)	{ Visible = _vs;  };
	virtual	bool	GetVisible()			{ return Visible; };

	virtual	bool	isVissible()			{ return false; };
	virtual	bool	isInVisible()			{ return false; };

	virtual	bool	TestSart()				{ return false; };
	virtual	bool	TestStop()				{ return false; };

	virtual	bool	CheckIfObjectIsGlobal() { return true; }

	lvCGraphObject	operator=(lvCGraphObject& OB) { OB.Visible=Visible; return *this; };

	SAVE(lvCGraphObject);
		REG_LOADSAVE("temp\\","lvCGraphObject.xml");
		REG_AUTO(Name);
		REG_MEMBER(_bool,Visible);
	ENDSAVE;
};
// lvCDialogBased ////////////////////////////////////////////////////////
class lvCDialogBased : public lvCGraphObject
{
public:
			 lvCDialogBased();
	virtual	~lvCDialogBased();
	
	_str	sFilePath;		
	_str	SDName;

	DialogsSystem* pDS;
	SimpleDialog* pSD;

	virtual	void	DRAW();
	
			SimpleDialog*	GetSimpleDialog();
	virtual	void			AddToDS(DialogsSystem* _pDS);

public:
	bool	Effects_IsFinished();
    void	Efects_Start();
	
	SAVE(lvCDialogBased);
		REG_LOADSAVE("temp\\","lvCGraphObject.xml");
		REG_PARENT(lvCGraphObject);
		REG_BASE(lvCGraphObject);
		REG_FILEPATH(sFilePath,".xml");
		REG_AUTO(SDName);
	ENDSAVE;	
};
// lvCBlackScreen ////////////////////////////////////////////////////////
class lvCBlackScreen : public lvCGraphObject
{
public:
	lvCBlackScreen()	{ 
		InfID=_lvCBlackScreen_; 
		Visible=false; 
		Alpha=0;  
		AppearTime=3000; 
		DisapperTime=2000; 
		RealTime=0; 
		if (AppearTime!=0)	a_sign=255.f/(float)AppearTime;
		else				a_sign=255.f;
	};
	virtual		~lvCBlackScreen()	{};

	int		Alpha;
	float	a_sign;
	int		AppearTime;
	int		DisapperTime;
	DWORD	RealTime;

	virtual	void	UpDate();
	virtual	void	DRAW();

	virtual	void	StartShow();
	virtual	void	StopShow();
	virtual	void	SetVisible(bool _vs);
	virtual	bool	GetVisible();

	virtual	bool	isVissible();
	virtual	bool	isInVisible();

			void	SetAppearTime(int _time);
			void	SetDisapperTime(int _time);
			int		GetAppearTime();
			int		GetDisapperTime();

	virtual	bool	TestSart();
	virtual	bool	TestStop();

	lvCBlackScreen	operator=(lvCBlackScreen& OB){
		lvCGraphObject* pPR = dynamic_cast<lvCGraphObject*>(&OB);
		(*(dynamic_cast<lvCGraphObject*>(this))) = (*pPR);
		Alpha			=OB.Alpha;
		a_sign			=OB.a_sign;
		AppearTime		=OB.AppearTime;
		DisapperTime	=OB.DisapperTime;
		RealTime		=OB.RealTime;
		return *this;
	};

	class SetVisibleBS : public BaseFunction
	{
	public:
		bool	Visible;
		void EvaluateFunction(){
			lvCBlackScreen* pBS = get_parent<lvCBlackScreen>();
			if (pBS!=NULL) {
				pBS->SetVisible(Visible);
			};
		};
		SAVE(SetVisibleBS);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_bool,Visible);
		ENDSAVE;
	}	fSetVisibleBS;

	SAVE(lvCBlackScreen);
		REG_LOADSAVE("temp\\","lvCBlackScreen.xml");
		REG_PARENT(lvCGraphObject);
		REG_MEMBER(_int,Alpha);
		REG_MEMBER(_float,a_sign);
		REG_MEMBER(_int,AppearTime);
		REG_MEMBER(_int,DisapperTime);
		REG_MEMBER(_int,RealTime);
		REG_AUTO(fSetVisibleBS);
	ENDSAVE;
};
// lvCMoveGP /////////////////////////////////////////////////////////////
class lvCMoveGP : public lvCGraphObject
{
public:
	lvCMoveGP()	{
		InfID = _lvCMoveGP_;

		Nat				= 0;
		Visible			= false;

		BorderType		= 0;
		Stretched		= false;
		BeginPos		= 0;
		EndPos			= 0;
		
		MovePos			= 0.f;
		AppearTime		= 3000;
		DisapperTime	= 2000;
		m_sign			= 0.f;
		RealTime		= 0;

		FileID			= 0xFFFF;
		SpriteID		= 0;

		Width			= 0;
		Height			= 0;
		ExTD			= false;
		pTimeD		= NULL;

		SetMovePos(Visible);
	};
	~lvCMoveGP(){};
public:
	int		Nat;

	int		BorderType;		// К какому краю прилягает.	0-TOP,1-LEFT,2-RIGHT,3-BOTTOM
	bool	Stretched;		// Нужно ли растягивать.
	int		BeginPos;		// Начальная позиция.
	int		EndPos;			// Используется как конечная позиция если Stretched==true.
	
	float	MovePos;		// Текущая позиция при появленни и исчезании
	float	m_sign;			// Величина единичного сдвига и знак
	int		AppearTime;		// Время в милисекундах, появления на экране
	int		DisapperTime;	// Время в милисекундах, исчезания с экрана
	int		RealTime;		// Текущее время
	
	word	FileID;			// Файл с картинкой (0xFFFF-ничего нет)
	int		SpriteID;		// Индекс картинки

	int		Width;			// Ширина спрайта
	int		Height;			// Высота спрайта
	bool	ExTD;			// Использовать внешний таймер.
	int*	pTimeD;			// Внешняя часовая разница.
	int		x;				// Текущая позиция
	int		y;				// Текущая позиция
	
	class SetVisibleBS : public BaseFunction
	{
	public:
		bool	Visible;
		void EvaluateFunction(){
			lvCMoveGP* pBS = get_parent<lvCMoveGP>();
			if (pBS!=NULL) {
				pBS->SetVisible(Visible);
			};
		};
		SAVE(SetVisibleBS);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_bool,Visible);
		ENDSAVE;
	}	fSetVisibleBS;
public:
			void	UpDate();
	virtual	void	DRAW();

	virtual	void	StartShow();
	virtual	void	StopShow();
	virtual	void	SetVisible(bool _vs);
	virtual	bool	GetVisible();

	virtual	bool	isVissible();
	virtual	bool	isInVisible();

			void	SetAppearTime(int _time);
			void	SetDisapperTime(int _time);
			int		GetAppearTime();
			int		GetDisapperTime();

			void	SetSpriteWH();
			void	SetSign(bool appear);
			void	SetMovePos(bool visible);
			void	UseExternalTimeD(int* pTD);
	virtual	bool	TestSart();
	virtual	bool	TestStop();
public:
	SAVE(lvCMoveGP);
		REG_LOADSAVE("temp\\","lvCMoveGP.xml");
		REG_PARENT(lvCGraphObject);
		REG_ENUM(_index,BorderType,BE_BORDER_TYPE);
		REG_MEMBER(_bool,Stretched);
		REG_MEMBER(_int,BeginPos);
		REG_MEMBER(_int,EndPos);

		REG_MEMBER(_float,MovePos);
		REG_MEMBER(_float,m_sign);
		REG_MEMBER(_int,AppearTime);
		REG_MEMBER(_int,DisapperTime);
		REG_MEMBER(_int,RealTime);

		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		
		REG_AUTO(fSetVisibleBS);
	ENDSAVE;
};
// lvCAAppearGP //////////////////////////////////////////////////////////
class lvCAAppearGP : public lvCBlackScreen
{
public:
	lvCAAppearGP()	{
		InfID = _lvCAAppearGP_;
		Visible=false;
		AppearTime=1000; 
		DisapperTime=1000; 
		Flick = false;
		posX = 0;
		posY = 0;
		FileID = 0xFFFF;
		SpriteID = 0;
		Nat = 0;
		dx=dy=0;
		OnCenterSCR=false;
	};
	~lvCAAppearGP()	{};

	int		Nat;
	int		posX;
	int		posY;

	bool	Flick;			// Постоянно мерцать.
	
	word	FileID;			// Файл с картинкой (0xFFFF-ничего нет)
	int		SpriteID;		// Индекс картинки

	int		dx;				// Дополнительное смещение по x
	int		dy;				// Дополнительное смещение по y

	bool	OnCenterSCR;	// Рисовать по центру экрана
	
	virtual	void	UpDate();
	virtual	void	DRAW();
	virtual	void	SetPosXY(int x,int y);
	virtual	void	SetSprite(int id);
	virtual	void	getPictureSize(int& w,int& h);

	lvCAAppearGP	operator=(lvCAAppearGP& OB){
		lvCBlackScreen* pPR = dynamic_cast<lvCBlackScreen*>(&OB);
		(*(dynamic_cast<lvCBlackScreen*>(this))) = (*pPR);
		Nat				=OB.Nat;
		posX			=OB.posX;
		posY			=OB.posY;
		Flick			=OB.Flick;
		FileID			=OB.FileID;
		SpriteID		=OB.SpriteID;
		dx				=OB.dx;
		dy				=OB.dy;
		OnCenterSCR		=OB.OnCenterSCR;
		return *this;
	};

	SAVE(lvCAAppearGP);
		REG_LOADSAVE("temp\\","lvCAAppearGP.xml");
		REG_PARENT(lvCGraphObject);
		// lvCBlackScreen
		REG_MEMBER(_int,Alpha);
		REG_MEMBER(_float,a_sign);
		REG_MEMBER(_int,AppearTime);
		REG_MEMBER(_int,DisapperTime);
		REG_MEMBER(_bool,Flick);
		REG_MEMBER(_int,RealTime);
		REG_AUTO(fSetVisibleBS);
		
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_MEMBER(_int,Nat);

		REG_MEMBER(_int,dx);
		REG_MEMBER(_int,dy);

		REG_MEMBER(_int,posX);
		REG_MEMBER(_int,posY);

		REG_MEMBER(_bool,OnCenterSCR);
	ENDSAVE;
};
// lvCAnimateGP //////////////////////////////////////////////////////////
class lvCAnimateGP : public lvCAAppearGP
{
public:
	lvCAnimateGP();
	~lvCAnimateGP()	{};

	// STATIC DATA 
	bool	Animate;		// Анимировать изображение.
	int		SpriteLastID;	// Последний кадр анимации.
	int		DelayTime;		// Время задержки одного кадра.

	// DYNAMIC DATA
	int		CurSprite;		// Текущий отображемый спрайт
	DWORD	LastUpdeteTime;	// Время последней смены кадра

	// VIRTUAL METHODS
	virtual	void	UpDate(bool stopANIM=false);
	virtual	void	DRAW(bool stopANIM=false);

	lvCAnimateGP	operator=(lvCAnimateGP& OB){
		lvCAAppearGP* pPR = dynamic_cast<lvCAAppearGP*>(&OB);
		(*(dynamic_cast<lvCAAppearGP*>(this))) = (*pPR);
		Animate			=OB.Animate;
		SpriteLastID	=OB.SpriteLastID;
		DelayTime		=OB.DelayTime;
		CurSprite		=OB.CurSprite;
		LastUpdeteTime	=OB.LastUpdeteTime;
		return *this;
	};
 
	// SAVE TO XML AND EDIT
	SAVE(lvCAnimateGP);
		REG_LOADSAVE("temp\\","lvCAnimateGP.xml");
		REG_PARENT(lvCAAppearGP);
		REG_BASE(lvCGraphObject)
			;
		REG_MEMBER(_bool,Animate);
		REG_SPRITE(SpriteLastID,FileID);
		REG_MEMBER(_int,DelayTime);
	ENDSAVE;
};

// lvCDeffFilmMenu ///////////////////////////////////////////////////////
class lvCDeffFilmMenu : public lvCGraphObject
{
public:
	lvCDeffFilmMenu();
	~lvCDeffFilmMenu()	{};
public:
	int					Nat;
	lvCMoveGP			TOP_LINE;
	lvCMoveGP			LEFT_BOX;
	lvCMoveGP			RIGHT_BOX;
	lvCMoveGP			CENTER_BOX;
	lvCAAppearGP		LEFT_MORDA;
	lvCAAppearGP		RIGHT_MORDA;
	lvCAAppearGP		FRAME;
	lvCAnimateGP		CII_FRAME;
    int					Frame_State;// Положение мигающего фрейма 0-нет, 1-слева, 2-справа.
	ClassRef<vvTEXT>	TextStr;	// Текст.
	int					txX,txY;	// Положение относительно страницы
	int					mLx,mLy;	// Размер
	bool				txST;
	int					RealTime;	// Текущее время
	int					lastLx;
	int					lastLy;

	// For CII hero name show style
	ClassRef<vvTEXT>	LeftName;	// Name for left  hero
	ClassRef<vvTEXT>	RightName;	// Name for right hero

	// Aditional parameters
	SubSection		Advanced;
	int L_M_dx;
	int L_M_dy;
	int R_M_dx;
	int R_M_dy;

	int C_B_dy;		// + to y position
	int C_B_dep;	// + to end position

	int L_F_dx;
	int L_F_dy;
	int R_F_dx;
	int R_F_dy;

	bool	DoNotShowLeftMorda;
	bool	DoNotShowRightMorda;

	int TXT_add_x_size;
	int TXT_add_y_size;

	bool CII_HeroName_style;
	int  HNL_x;
	int  HNL_y;
	int  HNR_x;
	int  HNR_y;

	void	SetAlexanderSet();
	void	SetCossacsIISet();

	class SetAdvDeff : public BaseFunction
	{
	public:
		void EvaluateFunction();
		int GameName;
		SAVE(SetAdvDeff);
			REG_PARENT(BaseFunction);
			REG_ENUM(_index,GameName,BE_GAME_NAME);
		ENDSAVE;
	}fSetAdvDeff;


	DialogsSystem		DS_Text;		// Для отображения текста со скролером.
	bool				DS_Load();		// Загрузить стандартный DS "MissionDialogText.DialogsSystem.xml"
	SimpleDialog*		DS_GetUNIT(char* Name);
	bool				DS_Visible(char* Name,bool state);
	bool				DS_Pos(char* Name,int x,int y);
	bool				DS_Size(char* Name,int h,int w);

	bool				DS_SetMessage(char* Message);
	bool				DS_SetMessage(_str& Message);
	bool				DS_SetMaxMessWidth(int MaxWidth);
	
	// Hero names for CII menu style
	bool				DS_SetLHName(char* name);
	bool				DS_SetRHName(char* name);
	bool				DS_PosCenter(char* Name,int xc,int yc);	// set center position

public:
	virtual	void	Draw_TOP_LINE();
    virtual	void	Draw_LEFT_BOX();
	virtual	void	Draw_RIGHT_BOX();
	virtual	void	Draw_CENTER_BOX();
	virtual void	Draw_LEFT_MORDA();
	virtual	void	Draw_RIGHT_MORDA();
	virtual	void	Draw_FRAME();
	virtual	void	Draw_TEXT();
	virtual	void	Draw_HNAME();
	virtual	void	DRAW();

	virtual	void	StartShow_LEFT_MORDA();
	virtual	void	StartShow_RIGHT_MORDA();
	virtual	bool	TestSart_LEFT_MORDA();
	virtual	bool	TestSart_RIGHT_MORDA();
	virtual	void	StartShow();
	virtual	void	StopShow_LEFT_MORDA();
	virtual	void	StopShow_RIGHT_MORDA();
	virtual	bool	TestStop_LEFT_MORDA();
	virtual	bool	TestStop_RIGHT_MORDA();
	virtual	void	StopShow();
	virtual	void	SetVisible_LEFT_MORDA(bool _vs);
	virtual	void	SetVisible_RIGHT_MORDA(bool _vs);
	virtual	void	SetVisible(bool _vs);
	virtual	bool	GetVisible();

	virtual	bool	isVissible_LEFT_MORDA();
	virtual	bool	isVissible_RIGHT_MORDA();
	virtual	bool	isVissible();
	virtual	bool	isInVisible_LEFT_MORDA();
	virtual	bool	isInVisible_RIGHT_MORDA();
	virtual	bool	isInVisible();

	virtual	void	SetLeftMorda(int _id);
	virtual	void	SetRightMorda(int _id);
	virtual	void	SetFarmeState(int _st);
	virtual	void	SetText(vvTEXT* pText);
	virtual	void	ShowText(bool _st);
	virtual	void	PlayText(int _canal);
	virtual	void	SetDeffParam_LEFT_MORDA();
	virtual	void	SetDeffParam_RIGHT_MORDA();
	virtual	void	SetDeffParam();

	class SetDefaul : public BaseFunction
	{
	public:
		void EvaluateFunction(){
			lvCDeffFilmMenu* pBS = get_parent<lvCDeffFilmMenu>();
			if (pBS!=NULL) {
                pBS->SetDeffParam();
			};
		};
		SAVE(SetDefaul);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}	fSetDefaul;
public:
	SAVE(lvCDeffFilmMenu);
		REG_LOADSAVE("temp\\",".xml");
		REG_PARENT(lvCGraphObject);
		REG_MEMBER(_int,Nat);
		REG_AUTO(TOP_LINE);
		REG_AUTO(LEFT_BOX);
		REG_AUTO(RIGHT_BOX);
		REG_AUTO(CENTER_BOX);
		REG_AUTO(LEFT_MORDA);
		REG_AUTO(RIGHT_MORDA);
		REG_AUTO(FRAME);
		REG_AUTO(CII_FRAME);
		REG_MEMBER(_int,Frame_State);
		REG_AUTO(TextStr);
		REG_MEMBER(_int,txX);
		REG_MEMBER(_int,txY);
		REG_MEMBER(_bool,txST);
		REG_AUTO(LeftName);
		REG_AUTO(RightName);
		REG_AUTO(fSetDefaul);

		REG_AUTO(Advanced);
			REG_AUTO(fSetAdvDeff);
			
			REG_MEMBER(_int,L_M_dx);
			REG_MEMBER(_int,L_M_dy);
			REG_MEMBER(_int,R_M_dx);
			REG_MEMBER(_int,R_M_dy);

			REG_MEMBER(_int,C_B_dy);
			REG_MEMBER(_int,C_B_dep);

			REG_MEMBER(_int,L_F_dx);
			REG_MEMBER(_int,L_F_dy);
			REG_MEMBER(_int,R_F_dx);
			REG_MEMBER(_int,R_F_dy);

			REG_MEMBER(_bool,DoNotShowLeftMorda);
			REG_MEMBER(_bool,DoNotShowRightMorda);

			REG_MEMBER(_int,TXT_add_x_size);
			REG_MEMBER(_int,TXT_add_y_size);

			REG_MEMBER(_bool,CII_HeroName_style);
			REG_MEMBER(_int,HNL_x);
			REG_MEMBER(_int,HNL_y);
			REG_MEMBER(_int,HNR_x);
			REG_MEMBER(_int,HNR_y);
	ENDSAVE;
};
// lvCDeffAnimeFilmMenu //////////////////////////////////////////////////
class lvCDeffAnimeFilmMenu : public lvCDeffFilmMenu
{
public:
	lvCDeffAnimeFilmMenu() : lvCDeffFilmMenu()	{ InfID=_lvCDeffAnimeFilmMenu_; };
	~lvCDeffAnimeFilmMenu()						{};
	
	// STATIC DATA 
	ClassRef<lvCAnimateGP>	LEFT_ANIME_MORDA;
	ClassRef<lvCAnimateGP>	RIGHT_ANIME_MORDA;
	
	// VIRTUAL METHODS
	virtual void	Draw_LEFT_MORDA();
	virtual	void	Draw_RIGHT_MORDA();

	virtual	void	StartShow_LEFT_MORDA();
	virtual	void	StartShow_RIGHT_MORDA();

	virtual	bool	TestSart_LEFT_MORDA();
	virtual	bool	TestSart_RIGHT_MORDA();

	virtual	void	StopShow_LEFT_MORDA();
	virtual	void	StopShow_RIGHT_MORDA();

	virtual	bool	TestStop_LEFT_MORDA();
	virtual	bool	TestStop_RIGHT_MORDA();

	virtual	void	SetVisible_LEFT_MORDA(bool _vs);
	virtual	void	SetVisible_RIGHT_MORDA(bool _vs);

	virtual	bool	isVissible_LEFT_MORDA();
	virtual	bool	isVissible_RIGHT_MORDA();

	virtual	bool	isInVisible_LEFT_MORDA();
	virtual	bool	isInVisible_RIGHT_MORDA();

	virtual	void	SetLeftMorda(int _id);
	virtual	void	SetRightMorda(int _id);

	virtual	void	SetDeffParam_LEFT_MORDA();
	virtual	void	SetDeffParam_RIGHT_MORDA();
	
	// SAVE TO XML AND EDIT
	SAVE(lvCDeffAnimeFilmMenu);
		REG_LOADSAVE("temp\\","lvCDeffAnimeFilmMenu.xml");
		REG_PARENT(lvCDeffFilmMenu);
		REG_BASE(lvCGraphObject);
		REG_AUTO(LEFT_ANIME_MORDA);
		REG_AUTO(RIGHT_ANIME_MORDA);
	ENDSAVE;
};

// vvTASKS_CII ///////////////////////////////////////////////////////////
class bbTEXT : public BaseClass
{
public:
	bbTEXT(){};
	virtual int GetExpansionRules() { return 2; };
	SAVE(bbTEXT);
	ENDSAVE;
};
class vvTEXT_EX : public bbTEXT
{
public:
	vvTEXT_EX(){ InfID=_vvTEXT_EX_; };
	~vvTEXT_EX(){};

	DWORD	InfID;

	_str				Name;			// Name for identification
	_str				TextID;			// Text string
	ClassArray<vvBASE>	paramList;		// only ( _be_CINT_ || _vvTEXT_ || _vvPICTURE_ )
	ClassArray<vvBASE>	paramList_EX;	// using in some class like vvTASKS_CII as (x,y) position
	int					id;				// personal id (unique)

	_str				TEXT;					// Text width pameters within
	char*				GetFinishedString();	// Get string with all applying params 

	SAVE(vvTEXT_EX);
		REG_BASE(bbTEXT);
		REG_MEMBER(_int,id);
		REG_AUTO(Name);
		REG_AUTO(TextID);
		REG_AUTO(paramList);
		REG_AUTO(paramList_EX);
	ENDSAVE;
};
bool	vbTASK_LCLIC(SimpleDialog* SD);	// Click on TASK in main menu
bool	vbHINT_LCLIC(SimpleDialog* SD);	// Click on HINT in main menu
bool	vbELSE_LCLIC(SimpleDialog* SD);	// Click on ELSE in main menu
bool	vbTASK_DELET(SimpleDialog* SD);	// Click on delete button in TASK list
bool	vbHINT_DELET(SimpleDialog* SD);	// Click on delete button in HINT list
bool	vbELSE_DELET(SimpleDialog* SD);	// Click on delete button in ELSE list
bool	vbTASK_MOVER(SimpleDialog* SD);	// Mouse over curent TASK
class	bbObjInList : public BaseClass
{
public:
	bbObjInList();
	~bbObjInList();

	_str						Name;		// personal name for object in TASK list
	int							DS_ID;		// id for element in list from DialogSystem element
	LinearArray<OneUS,_OneUS>	UN_BMP_ID;
	ClassArray< lvCAnimateGP >	GP_MMP_ID;	// list animate graph objects on minimap
    
	void	Anim_MMP(bool state);
	void	Draw_MMP(bool stopANIM=false);
	void	PreDelete();
	void	DelUnit(int UID);
	void	DelPoint(int PID);
	void	UpdatePosOnMiniMap();

	SAVE(bbObjInList);
		REG_AUTO(Name);
		REG_MEMBER(_int,DS_ID);
		REG_AUTO(UN_BMP_ID);
		REG_AUTO(GP_MMP_ID);
	ENDSAVE;
};
class vvTASKS_CII : public vvBASE
{
public:
	virtual			void	Draw();		// Function from vvBASE for draw dialogs
	virtual			void	DrawOnMiniMap(int x,int y,int Lx,int Ly);

	vvTASKS_CII();
	~vvTASKS_CII();

	bool			ALREADY_SETIN;
	void			SETIN_OBJECT();

	SubSection		MAIN;
	DialogsSystem	ds_MAIN;			// All menu elements
	_str			ds_FILE;			// file name for DS
	bool			DS_LOAD();			// Load DS from file
	void			DS_DRAW();			// draw DS

	// MAIN MENU //
	SubSection	MAIN_MENU;
	// OBJECT //
	DialogsDesk*	getMM_object();				// Get DD object for main menu
	GP_Button*		getMM_child(char* _name);	// Get GPP object from main menu (TASK,HINT,ELSE)
	// POSITION //
	int		mm_X;	// Main menu x pos from left of screen
	int		mm_Y;	// Main menu y pos from top  of screen
	void	applyMM_pos();	// Set main menu position usin (mm_X,mm_Y)
	// ANIMATION //
	DWORD	anim_diffuse;		// diffuse for animation
	int		task_sate;			// 0-hide, 1-disable, 2-enable, 3-enable+animate, 4-opened
	int		hint_sate;			// 0-hide, 1-disable, 2-enable, 3-enable+animate, 4-opened
	int		else_sate;			// 0-hide, 1-disable, 2-enable, 3-enable+animate, 4-opened
	void	UpdateMM_state();	// update main menu states
	int		mm_anim_delay;		// deley for animation in main menu
	int		mm_last_anim_time;	// time of last update animation
	void	ApplyMM_state();	// apply current state for main menu
	void	ApplyMM_state(GP_Button* pP,int _st);
	void	setMM_LMClick();
	void	setMM_LMClick(GP_Button* pP,vvTASKS_CII* pPARAM,VCall* pFUNC);


	// LIST MENU //
	SubSection	LIST_MENU;
	// POSITION //
	int		dl_X;	// from MAIN MENU by x
	int		dl_Y;	// from MAIN MENU by y
	void	applyLM_pos();	// Set list menu position usin (dl_X,dl_Y)
	void	setLM_ID();		// Set unique id for vvTEXT_EX elements

	// LIST MENU //
	ListDesk*				getLM_child(char* _name);	// Get GPP object from list menu (TASK,HINT,ELSE)
	void					ApplyLM_state();			// apply current state for list menu
	int						LM_OPEN_ID;		// 0-HIDE ALL,1-TASK,2-HINT,3-ELSE
	// TAKS //
	SubSection	TASKS;
	ClassArray< bbTEXT >	TASK;			// List of task messages (data storage)
	ListDesk*				getLM_TASK();	// Get ListDesck accoding to TASK
	bool					setLM_TASK_VIS(bool _st);	// Set TASK LIST visibility
	void					setLM_TASK_POS();			// Set list menu for TASK position
	ClassArray< bbObjInList >	LM_TASK_OBJ;			// Objects in TASK list
	void					TAKS_START_ANIM(int Task_ID);
	void					TASK_POINT_DRAW();
	bool					addLT_TASK(vvTEXT_EX* pT,char* name=NULL,int pos=0);	// Add element from TASK in list menu for TASK
	bool					addLT_TASK(const char* TName,int pos=0,bool Dublicate=true);
	void					delLT_TASK(int Task_ID);	// Delete task from menu list by it id in LM_TAKS
	void					delLT_TASK(const char* Task_Name);// Delete task from menu list by it id in LM_TAKS
	void					setLT_TASK_COMPLITE(const char* Task_Name);
	int						PointOnBigMap;				// Unit type on bigmap as destination point
	lvCAnimateGP			PointOnMiniMap;				// Graph object on minimap as destination point
	void					TASK_RESTORE();
	void					CHECK_POINT_COND();			// Check and dell if condition for point donot true
	// HINT //
	SubSection	HINTS;
	ClassArray< bbTEXT >	HINT;		// List of hint messages (data storage)
	ListDesk*				getLM_HINT();	// Get ListDesck accoding to HINT
	bool					setLM_HINT_VIS(bool _st);	// Set HINT LIST visibility
	void					setLM_HINT_POS();			// Set list menu for HINT position
	ClassArray< bbObjInList >	LM_HINT_OBJ;			// Objects in HINT list
	bool					addLT_HINT(vvTEXT_EX* pH,char* name=NULL,int pos=0);	// Add element from HINT in list menu for HINT
	bool					addLT_HINT(const char* HName,int pos=0,bool Dublicate=true);
	void					delLT_HINT(int Hint_ID);	// Delete hint from menu list by it id in LM_HINT
	void					delLT_HINT(const char* Hint_Name);// Delete hint from menu list by it id in LM_HINT
	void					HINT_RESTORE();
	// ELSE //
	SubSection	ELSES;
	ClassArray< bbTEXT >	ELSE;		// List of else messages (data storage)
	ListDesk*				getLM_ELSE();	// Get ListDesck accoding to TASK
	bool					setLM_ELSE_VIS(bool _st);	// Set ELSE LIST visibility
	void					setLM_ELSE_POS();			// Set list menu for ELSE position
	ClassArray< bbObjInList >	LM_ELSE_OBJ;			// Objects in ELSE list
	bool					addLT_ELSE(vvTEXT_EX* pE,char* name=NULL,int pos=0);	// Add element from ELSE in list menu for ELSE
	bool					addLT_ELSE(const char* EName,int pos=0,bool Dublicate=true);
	void					delLT_ELSE(int Else_ID);	// Delete else from menu list by it id in LM_ELSE
	void					delLT_ELSE(const char* Else_Name);// Delete else from menu list by it id in LM_ELSE
	void					ELSE_RESTORE();

	SubSection				DONOT_TOUCH;

	// for lua bind
	bool					addLT_TASK_lua(const char* TName,int pos,bool Dublicate);
	bool					addLT_HINT_lua(const char* HName,int pos,bool Dublicate);
	bool					addLT_ELSE_lua(const char* EName,int pos,bool Dublicate);
	void					delLT_TASK_lua(const char* Task_Name);
	void					delLT_HINT_lua(const char* Hint_Name);
	void					delLT_ELSE_lua(const char* Else_Name);

	// ADDON
	int						Assign(int el,LinearArray<int,_int>&   Arr);
	int						Assign(int el,ClassArray<bbObjInList>& Arr);
	bbTEXT*					getByNAME(const char* name,ClassArray<bbTEXT>& Arr);

	bool					NeedRestore;
	void					Restore();

	void					vv_TASK_LCLIC();
	void					vv_HINT_LCLIC();
	void					vv_ELSE_LCLIC();

	// SAVE/LOAD

	// IN GAME FUNCTIONS //
	class FCN00 : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(FCN00);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}fMM_LOAD;
	class FCN01 : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(FCN01);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}fMM_POS;
	class FCN02 : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(FCN02);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}fLM_POS;
	class FCN03 : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(FCN03);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}fMM_CLICK;
	class FCN04 : public BaseFunction
	{
	public:
		void EvaluateFunction();
		SAVE(FCN04);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}fLL_SET_ID;
	class FCN05 : public BaseFunction
	{
	public:
		void EvaluateFunction();
		_ClassIndex  TASK;
		SAVE(FCN05);
		REG_PARENT(BaseFunction);
		REG_AUTO2(TASK,"../TASK Name");
		ENDSAVE;
	}fLM_TASK_ADD;
	class FCN06 : public BaseFunction
	{
	public:
		void EvaluateFunction();
		_ClassIndex  HINT;
		SAVE(FCN06);
		REG_PARENT(BaseFunction);
		REG_AUTO2(HINT,"../HINT Name");
		ENDSAVE;
	}fLM_HINT_ADD;
	class FCN07 : public BaseFunction
	{
	public:
		void EvaluateFunction();
		_ClassIndex  ELSE;
		SAVE(FCN07);
		REG_PARENT(BaseFunction);
		REG_AUTO2(ELSE,"../ELSE Name");
		ENDSAVE;
	}fLM_ELSE_ADD;

	SAVE(vvTASKS_CII);
	REG_LOADSAVE("temp\\","vvTASKS_CII.xml");
	REG_PARENT(vvBASE);

	REG_AUTO(MAIN);
		REG_FILEPATH(ds_FILE,".xml");
		REG_AUTO(fMM_LOAD);
		REG_AUTO(fMM_CLICK);

	REG_AUTO(MAIN_MENU);
		REG_MEMBER(_int,mm_X);
		REG_MEMBER(_int,mm_Y);
		REG_AUTO(fMM_POS);
		REG_MEMBER(_DWORD,anim_diffuse);
		REG_MEMBER(_int,task_sate);
		REG_MEMBER(_int,hint_sate);
		REG_MEMBER(_int,else_sate);
		REG_MEMBER(_int,mm_anim_delay);
		REG_MEMBER(_int,mm_last_anim_time);

	REG_AUTO(LIST_MENU);
		REG_MEMBER(_int,LM_OPEN_ID);
		REG_MEMBER(_int,dl_X);
		REG_MEMBER(_int,dl_Y);
		REG_AUTO(fLM_POS);
		REG_AUTO(fLL_SET_ID);

	REG_AUTO(TASKS);
		REG_MEMBER(_UnitType,PointOnBigMap);
		REG_AUTO(PointOnMiniMap);
		REG_AUTO(TASK);
		REG_AUTO(fLM_TASK_ADD);

	REG_AUTO(HINTS);
		REG_AUTO(HINT);
		REG_AUTO(fLM_HINT_ADD);

	REG_AUTO(ELSES);
		REG_AUTO(ELSE);
		REG_AUTO(fLM_ELSE_ADD);

	REG_AUTO(DONOT_TOUCH);
		REG_AUTO(ds_MAIN);
		REG_AUTO(LM_TASK_OBJ);
		REG_AUTO(LM_HINT_OBJ);
		REG_AUTO(LM_ELSE_OBJ);

	ENDSAVE;
};
// lvCTeraforming ////////////////////////////////////////////////////////
class lvCTeraforming : public vvBASE
{
public:
	lvCTeraforming()	{	InfID=_lvCTeraforming_; 
							PosXYZ.z=0.f; 
							r=50; R=100; h=100; H=50; 
							visible=false; 
							TOP=Cross=BOTTOM=true;
						};
	~lvCTeraforming()	{};

	vvVector3D	PosXYZ;	// Position in space, PosXYZ.z == 0.f;
	float		r,h;	// INside  radius and height from current PosXY
	float		R,H;	// Outside radius and height from current PosXY

	bool		visible;
	bool		TOP;
	bool		Cross;
	bool		BOTTOM;

	virtual			char*	GetName();
	virtual const	char*	GetThisElementView(const char* LocalName);
	
	virtual			void	Add_X(float dx,int MaxStep=0);		// Correct x - addition 
	virtual			void	Add_Y(float dy,int MaxStep=0);		// Correct y - addition 
	virtual			void	Add_Z(float dz,int MaxStep=0);		// Correct z - addition 
	virtual			void	Add_r(float dr,int MaxStep=0);		// Correct r - addition 
	virtual			void	Add_h(float dh,int MaxStep=0);		// Correct h - addition 
	virtual			void	Add_R(float dR,int MaxStep=0);		// Correct R - addition 
	virtual			void	Add_H(float dH,int MaxStep=0);		// Correct H - addition 

	virtual			bool	IsInShape(int   x,int   y);
	virtual			bool	IsInShape(float x,float y);

	virtual			void	SetPoss(int   x,int   y,int   z);
	virtual			void	SetPoss(float x,float y,float z);

	virtual			void	Set_r(float Val);
	virtual			void	Set_R(float Val);
	virtual			void	Set_h(float Val);
	virtual			void	Set_H(float Val);

    virtual			void	ValidateRH();		// validate r width R
	virtual			void	Draw();				// draw on map

	virtual			void	Apply();			// Apply current terraforming
	
	class ApplyTerra : public BaseFunction
	{
	public:
		void EvaluateFunction(){
			lvCTeraforming* pMyClass = get_parent<lvCTeraforming>();
			if (pMyClass!=NULL) {
				pMyClass->Apply();
			};
		};

		SAVE(ApplyTerra);
			REG_PARENT(BaseFunction);
		ENDSAVE;

	} fApplyTerra;

	SAVE(lvCTeraforming);
		REG_LOADSAVE("temp\\","lvCTeraforming.xml");
		REG_PARENT(vvBASE);
		SAVE_SECTION(0x00000001);
			REG_AUTO(Name);
			REG_AUTO(PosXYZ);
			REG_MEMBER(_float,r);
			REG_MEMBER(_float,h);
			REG_MEMBER(_float,R);
			REG_MEMBER(_float,H);
			REG_MEMBER(_bool,visible);
			REG_MEMBER(_bool,TOP);
			REG_MEMBER(_bool,Cross);
			REG_MEMBER(_bool,BOTTOM);
			REG_AUTO(fApplyTerra);
	ENDSAVE;
};

// lvCScriptHandler_ST ///////////////////////////////////////////////////
class CFileList : public BaseClass
{
public:
	CFileList(){};
	_str	FileName;
	_str	AddFnName;
	SAVE(CFileList);
		REG_FILEPATH(FileName,".script,.lua,.luac");
		REG_AUTO(AddFnName);
	ENDSAVE;
};
class CLUA_COORDINATOR_FOR_SH : public BaseClass
{
public:
	CLUA_COORDINATOR_FOR_SH() { useLua=false; useLuaDEBUGER=false; callLuaFList=false; };
	bool						useLua;
	bool						useLuaDEBUGER;
	ClonesArray<CFileList>		LUA_FileList;
	ClonesArray<_str>			LUA_CALL_LIST;	// function FuncName [ ... ] end; list of FuncName
	bool						callLuaFList;
	SAVE(CLUA_COORDINATOR_FOR_SH);
		REG_MEMBER(_bool,useLua);
		REG_MEMBER(_bool,useLuaDEBUGER);
		REG_MEMBER(_bool,callLuaFList);
		REG_AUTO(LUA_FileList);
		REG_AUTO(LUA_CALL_LIST);
	ENDSAVE;
};
class SingletonlvCScriptHandler : public lvCStorage
{
	public:
		SingletonlvCScriptHandler()			{};
		virtual ~SingletonlvCScriptHandler()	{};
		static SingletonlvCScriptHandler* GetObj()			{ return m_pObj; };
		static void SetObj(SingletonlvCScriptHandler* pObj)	{ m_pObj = pObj; };
		static void Delete() { 
			if (m_pObj!=NULL) { 
				delete m_pObj; 
				m_pObj=NULL; 
			}; 
		};
		virtual void Accetss() {};
	protected:
		static SingletonlvCScriptHandler* m_pObj;
};

inline SingletonlvCScriptHandler* StlvCScriptHandler(){
	if (SingletonlvCScriptHandler::GetObj()==NULL) SingletonlvCScriptHandler::SetObj(new SingletonlvCScriptHandler);
	return SingletonlvCScriptHandler::GetObj();
};

class lvCScriptHandler_ST : public SingletonlvCScriptHandler
{
public:
	lvCScriptHandler_ST()	{ Time=0; LastTime=0; LUAC.useLua=false; LUAC.useLuaDEBUGER=false; LUAC.callLuaFList=false; };
	virtual ~lvCScriptHandler_ST();
public:
	DWORD						Time;					// Update in load function (set 0)
	DWORD						LastTime;				// Update in load function (set 0)
	ClassArray<lvCProcSquad>	SQUARDS_SCRIPTS;		// For additional save game in mission
	ClassArray<lvCProcSquad>	vGROUPS_SCRIPTS;		// For additional save game in mission
	lvCMainScript				MAIN_SCRIPT;			// Save in map "editor"
	ClassArray<lvCFilm>			SCRIPT_FILMS;			// Save in map "editor"
	ClassArray<lvCGraphObject>	SCRIPT_GRAPH;			// Save in map "editor"

	// Modern Graph
	DialogsSystem				DS;

	SubSection					_LUA_;
#ifdef  __LUA__
	class	LUA_MGR				LUA_MISS;
#endif//__LUA__
	CLUA_COORDINATOR_FOR_SH		LUAC;

	bool						LUA_Create();
	bool						LUA_Close();
	bool						LUA_LoadFile(const char* FName);
    void						LUA_CallFList();

	bool						LUA_SAFE_OPEN_MISS();
	void						LUA_SAFE_CLOSE_MISS();

	int							GET_GC_COUNT();
	int							GET_GC_THRESHOLD();
	void						SET_GC_THRESHOLD(int newthreshold);

	class	LUA_CREATE		: public BaseFunction{
	public:
		void EvaluateFunction();
		SAVE(LUA_CREATE);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}		luaCreate;
	class	LUA_CLOSE		: public BaseFunction{
	public:
		void EvaluateFunction();
		SAVE(LUA_CLOSE);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}		luaClose;
	class	LUA_LOAD_FILE	: public BaseFunction{
	public:
		_str FName;
		_str AddFnName;
		void EvaluateFunction();
		SAVE(LUA_LOAD_FILE);
		REG_PARENT(BaseFunction);
			REG_FILEPATH(FName,".lua,.luac,.script");
			REG_AUTO(AddFnName);
		ENDSAVE;
	}	luaLoadFile;

public:
	virtual bool	PROCESS(bool onlySwitched=false);
	virtual	bool	PROCESS(int squadID);
	virtual bool	PROCESS_MAIN(bool inProc=false);
	virtual	void	DRAW();								//
	virtual	void	TIMER(bool	inProc);

public:
	virtual	bool	Create();
	virtual	lvCProcSquad*	vGetSquadScriptID(int squadID);
	virtual	void	AddNewFilm(char* scrName,char* scrDescr);
	virtual	void	DeleteFilm(char* scrName);

	class CPRESENTATION	fShowPresentation;

	lvCGraphObject*	getGraphByName(const char* val);

public:
	SAVE(lvCScriptHandler_ST);
		//REG_LOADSAVE("temp\\","lvCScriptHandler_ST.xml");
		SetReadOnlyMode();
		REG_MEMBER(_DWORD,Time);
		SetReadOnlyMode();
		REG_MEMBER(_DWORD,LastTime);
		SetReadOnlyMode();
		REG_AUTO(SQUARDS_SCRIPTS);
		SetReadOnlyMode();
		REG_AUTO(vGROUPS_SCRIPTS);
		REG_AUTO(MAIN_SCRIPT);
		REG_AUTO(SCRIPT_FILMS);
		REG_AUTO(SCRIPT_GRAPH);
		//REG_AUTO(fShowPresentation);

		REG_AUTO(_LUA_);
		REG_AUTO(LUAC);
		REG_AUTO(luaCreate);
		REG_AUTO(luaClose);
		REG_AUTO(luaLoadFile);
	ENDSAVE;
};

inline lvCScriptHandler_ST* BattleHandler(){
	if (lvCScriptHandler_ST::GetObj()==NULL){ 
		lvCScriptHandler_ST::SetObj(new lvCScriptHandler_ST);

		lvCScriptHandler_ST::GetObj()->vINIT();
		((lvCScriptHandler_ST*)lvCScriptHandler_ST::GetObj())->MAIN_SCRIPT.vINIT();

		BaseClass* pMainScript = dynamic_cast<BaseClass*>( &(((lvCScriptHandler_ST*)lvCScriptHandler_ST::GetObj())->MAIN_SCRIPT) );
		ReplaceEditor("MainBEScript",pMainScript);
		ReplaceEditor("MainBEScriptEX",pMainScript);
		
		((lvCScriptHandler_ST*)lvCScriptHandler_ST::GetObj())->SCRIPT_FILMS.Clear();
		((lvCScriptHandler_ST*)lvCScriptHandler_ST::GetObj())->SCRIPT_GRAPH.Clear();

		ReplaceEditor("BattleHendler",dynamic_cast<BaseClass*>(lvCScriptHandler_ST::GetObj()));
	};
	return (lvCScriptHandler_ST*)lvCScriptHandler_ST::GetObj();
};

lvCGraphObject*	__getGraphByName(const char* val);
// lvCScriptMapSaver /////////////////////////////////////////////////////
class lvCBattleMapSaver : public MapXmlSaver
{
public:
	virtual bool GetXmlToSaveInMap(xmlQuote& xml);
	virtual void SetXmlFromMap(xmlQuote& xml);
	virtual DWORD GetSaverID()						{return 'BE'+'EDIT';};

	static	bool ResetAllClass();
	static	bool SaveToXML(xmlQuote& xml);
	static	bool LoadFromXML(xmlQuote& xml);

			bool SaveToXML	(char* FileName);
			bool LoadFromXML(char* FileName);

	static	bool LoadSubQuote(xmlQuote& xml, char* sqName, void* pObject, ErrorPager* pERR=NULL, void* Extra=NULL);
public:
	SAVE(lvCBattleMapSaver);
		REG_PARENT(MapXmlSaver);
	ENDSAVE;
};
void	RegisterBattleEditorSaver();
//////////////////////////////////////////////////////////////////////////

// Class Register ////////////////////////////////////////////////////////
inline	void	REG_BE_DataStorageXML_class();
//////////////////////////////////////////////////////////////////////////

#endif//__DATA_STORAGE_XML__














