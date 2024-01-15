
#ifndef __BATTLE_PAINTER__
#define __BATTLE_PAINTER__

// SCREEN DRAW ///////////////////////////////////////////////////////////
bool	IsOnScreen(int wX,int wY,int wZ,int dx=0,int dy=0);
//////////////////////////////////////////////////////////////////////////

// HELP DRAW FUNCTIONS ///////////////////////////////////////////////////
void	lvDrawArrow(int x,int y,int x1,int y1,int dx=100,int dy=150,DWORD color=0xFF00ff00);
void	lvDrawArrow(be_CNODE* nd1,be_CNODE* nd2,int dx=100,int dy=150,DWORD color=0xFF00ff00);
//////////////////////////////////////////////////////////////////////////

// BattlePainter /////////////////////////////////////////////////////////
typedef int tpDrawOnMapCallback();
CEXPORT	tpDrawOnMapCallback* SetDrawOnMapCallback(tpDrawOnMapCallback* dc);

// lvCPoint3D ////////////////////////////////////////////////////////////
class lvCPoint3D : public BaseClass
{
public:
	lvCPoint3D()	{x=y=z=0.f;};
	~lvCPoint3D()	{};
	float x,y,z;
	void	SetPoint(int   _x,int   _y,int   _z=0){
		SetPoint((float)_x,(float)_y,(float)_z);
	};
	void	SetPoint(float _x,float _y,float _z=0.f){
		x = _x;
		y = _y;
		z = _z;
	};
};

// lvCShapes /////////////////////////////////////////////////////////////
class SingletonlvCShapes : public BaseClass
{
public:
	SingletonlvCShapes()							{};
	virtual ~SingletonlvCShapes()					{};
	static SingletonlvCShapes* GetObj()				{ return m_pObj; };
	static void	SetObj(SingletonlvCShapes* pObj)	{ m_pObj = pObj; };
	static void Delete()							{ if (m_pObj!=NULL) { delete m_pObj; m_pObj=NULL; } };
	virtual void Accetss()							{};
protected:
	static SingletonlvCShapes* m_pObj;
};

class lvCShapes : public SingletonlvCShapes
{
public:
	lvCShapes()			{ CreateEdge(); SetNewR(50.f); };
	lvCShapes(float R)	{ CreateEdge(); SetNewR( R );  };
	lvCShapes(int R)	{ CreateEdge(); SetNewR( R );  };
	virtual ~lvCShapes()		{ regular_polygon_4D.Clear(); regular_polygon_6D.Clear(); };
public:
	void	SetNewR(float _R);
	void	SetNewR(int	  _R)	{ SetNewR((float)_R); };
	void	SetAngel_XY(int nA,float Fi);
public:
	ClassArray<lvCPoint3D>	regular_polygon_4D;
	ClassArray<lvCPoint3D>	regular_polygon_6D;
private:
	float					R;
	void	CreateEdge();
	void	GetNextXY(float& nX,float& nY,float dF,int n);
};

inline lvCShapes* Shapes_MAP(float R=50.f){
	if (lvCShapes::GetObj()==NULL){ 
		lvCShapes::SetObj(new lvCShapes(R));
	};
	((lvCShapes*)lvCShapes::GetObj())->SetNewR(R);
	return (lvCShapes*)lvCShapes::GetObj();
};
inline lvCShapes* Shapes_MAP(int  R=50){
	if (lvCShapes::GetObj()==NULL){ 
		lvCShapes::SetObj(new lvCShapes(R));
	};
	((lvCShapes*)lvCShapes::GetObj())->SetNewR(R);
	return (lvCShapes*)lvCShapes::GetObj();
};
// CAMERA ////////////////////////////////////////////////////////////////
class lvCRotateCamera 
{
public:
			 lvCRotateCamera()	{ MoveType=beTO_FREE; useMapXY=false; };
	virtual	~lvCRotateCamera()	{};

	static	lvCRotateCamera* GetObj()						{ return m_pObj; };
	static	void			 SetObj(lvCRotateCamera* pObj)	{ m_pObj = pObj; };
	static	void			 Delete()						{ if (m_pObj!=NULL) { delete m_pObj; m_pObj=NULL; } };
public:
	static	lvCRotateCamera*	m_pObj;
	
	Vector3D	POS0;
	Vector3D	POS1;
	Vector3D	DIR0;
	Vector3D	DIR1;

	_str_ref	vGrpID;		//
    	
	bool		useMapXY;

	int			TimeD;		//
	int			LastTime;	//
	enum		enMoveType {
		beTO_FREE=0, beTO_POS=1, beTO_POSDIR=2, beTO_POSPOS=3, beTO_POSDIR_POSDIR=4, beTO_VGROUP=5
	}			MoveType;

	void		Set(Vector3D* pPOS0,int _TimeD);
	void		Set(float x,float y,int _TimeD);
	void		Set(Vector3D* pPOS0,Vector3D* pDIR0,int _TimeD);
	void		Set(Vector3D* pPOS0,Vector3D* pPOS1,int _TimeD,int type);
	void		Set(Vector3D* pPOS0,Vector3D* pDIR0,Vector3D* pPOS1,Vector3D* pDIR1,int _TimeD);
	void		Set(bool _useMapXY);
	void		Set(_str_ref& _vGrpID);

	void		Draw();
	void		Stop();
};

__forceinline	lvCRotateCamera* CameraDriver(){
	if (lvCRotateCamera::GetObj()==NULL){
		lvCRotateCamera::SetObj(new lvCRotateCamera());
	};
	return	lvCRotateCamera::GetObj();
};

// CameraPoss ////////////////////////////////////////////////////////////
class VectorPara : public BaseClass{
public:
	VectorPara()	{ Name="NoName"; Visible=false; };

	ClassRef<vvVector3D>	Pos;
	ClassRef<vvVector3D>	Dir;

	_str					Descr;
	_str					Name;

	bool					Visible;
	
	virtual const	char*	GetThisElementView(const char* LocalName);
					void	Draw();

	SAVE(VectorPara);
		REG_AUTO(Pos);
		REG_AUTO(Dir);
		REG_AUTO(Name);
		REG_MEMBER(_bool,Visible);
	ENDSAVE;
};

class CameraPoss : public BaseClass
{
public:
	//--------------------------------------------------------------------
			 CameraPoss()	{};
	virtual ~CameraPoss()	{};

	//--------------------------------------------------------------------
	static	CameraPoss*		m_pObj;

	static	CameraPoss* GetObj()					{ return m_pObj; };
	static	void		SetObj(CameraPoss* pObj)	{ m_pObj = pObj; };
	static	void		Delete()					{ if (m_pObj!=NULL) { delete m_pObj; m_pObj=NULL; } };

	//--------------------------------------------------------------------
	ClonesArray<VectorPara>	CamerasPoss;

	void	Draw();
	//--------------------------------------------------------------------
	SAVE(CameraPoss);
		REG_AUTO(CamerasPoss);
	ENDSAVE;
	//--------------------------------------------------------------------
};

__forceinline	CameraPoss* CameraPositons(){
	if (CameraPoss::GetObj()==NULL){
		CameraPoss::SetObj(new CameraPoss());
	};
	return	CameraPoss::GetObj();
};

// ggCPlot ///////////////////////////////////////////////////////////////
// using for draw lines by point set
template<class RET>
RET* getGGPAR(BaseClass* pBC){
	RET* pGG=NULL;
	BaseClass* pPR=pBC;
	
	while ( pPR!=NULL && pGG==NULL ) {
		pPR = pPR->GetParent();
		pGG = dynamic_cast< RET* >( pPR );
	};

	return pGG;
};
bool move_plot_LMClick(SimpleDialog* SD);
bool move_plot_MOwer(SimpleDialog* SD);
class ggCDialog : public BaseClass
{
public:
	ggCDialog();
	~ggCDialog();

	_str			FName;										// *.xml file for DIALOG
	DialogsSystem*	pDIALOG;
	bool			load_DIALOG(const char* FileName=NULL);	// load from file
	bool			delete_DIALOG();

	// global on PLOT
	void			INIT(const bool LOAD=true);					// init object if DIALOG loaded
	bool			DRAW();										// draw DIALOG 
	bool			setXY_DIALOG(const int  x,const int  y);	// set DIALOG position on screen
	bool			getXY_DIALOG(	   int& x,		int& y);	// get DIALOG position on screen
	bool			setHW_DIALOG(const int  w,const int  h);	// set DIALOG size : width and height
	bool			getHW_DIALOG(	   int& w,		int& h);	// get DIALOG size : width and height

	// for move plot on screen 
	int				msX,msY;
	int				dlX,dlY;
	void			startMove();
	void			setOffsetXY(const int nmX,const int nmY);
	void			stopMove();

protected:
	DialogsDesk*	pPANEL;	// from loaded DIALOG ( main dialog panel	)
	void			setPANEL (DialogsDesk*	ptr=NULL);
	void			initPANEL(DialogsDesk*	ptr=NULL);
	Canvas*			pSHEET;	// from loaded DIALOG ( sheet for draw plot	)
	void			setSHEET (Canvas*		ptr=NULL);
	void			initSHEET(Canvas*		ptr=NULL);
	GPPicture*		pMOVE;	// from loaded DIALOG ( move picture		)	
	void			setMOVE (GPPicture*		ptr=NULL);
	void			initMOVE(GPPicture*		ptr=NULL);
public:
	SAVE(ggCDialog);
		REG_FILEPATH(FName,".DialogsSystem.xml");
	ENDSAVE;
};
// for skirmish //////////////////////////////////////////////////////////
class CSkirmishTASK : public BaseClass
{
public:
	CSkirmishTASK();	// create and load
	~CSkirmishTASK();	// delete all
		
	int						PointOnBigMap;				// Unit type on bigmap as destination point
	lvCAnimateGP			PointOnMiniMap;				// Graph object on minimap as destination point
	
	ClonesArray< bbObjInList >	TASK_arr;

	bool	LoadTemplate(char* FName=NULL);
	void	Check();
	void	Clear();
	bool	DrawOnMiniMap();
	bool	addTASK(const byte NI,const int x,const int y,const char* name);
	int		delTASK(const char* name);

	SAVE(CSkirmishTASK);
		REG_LOADSAVE("Dialogs\\",".SkTASK_TL.xml");
		REG_MEMBER(_UnitType,PointOnBigMap);
		REG_AUTO(PointOnMiniMap);
	ENDSAVE;

protected:
	bool	ready;
};
extern CSkirmishTASK	g_SkTASK;
//////////////////////////////////////////////////////////////////////////
int BattlePainter();
void	SET_BE_DrawOnMapCallback();
//////////////////////////////////////////////////////////////////////////

#endif//__BATTLE_PAINTER__




















































