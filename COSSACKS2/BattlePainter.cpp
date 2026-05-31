#include "stdheader.h"
#include "BE_HEADERS.h"
#include "ITerrain.h"
#include "GameInterface.h"

bool	BE_SetCamera = false;
bool	BE_UseCamera(){
	//bool	retVal = BE_SetCamera;
	//BE_SetCamera = false;
	//return retVal;
	return BE_SetCamera;
};
void	BE_NewSetCamera(){
	BE_SetCamera = true;
};
void	BE_DelCamera(){
	BE_SetCamera = false;
};

// SCREEN DRAW ///////////////////////////////////////////////////////////
bool	IsOnScreen(int wX,int wY,int wZ,int dx/*=0*/,int dy/*=0*/){
	Vector4D	pos((float)wX,(float)wY,(float)wZ,1);
	WorldToScreenSpace(pos);
	int sX = (int)(pos.x);
	int sY = (int)(pos.y);
	bool onScreen = false;
	if ( (-dx<=sX) && (sX<RealLx+dx) && (-dy<=sY) && (sY<RealLy+dy) ) {
		onScreen=true;
	};
	return onScreen;
};
//////////////////////////////////////////////////////////////////////////

// HELP DRAW FUNCTIONS ///////////////////////////////////////////////////
void	lvDrawArrow(int x,int y,int x1,int y1,int dx/* =100 */,int dy/* =150 */,DWORD color/* =0xFF00ff00 */){
	float	mX	= (float)x;
	float	mY	= (float)y;
	float	mX1 = (float)x1;
	float	mY1 = (float)y1;
	float	mDX	= (float)dx;
	float	mDY	= (float)dy;

	float	DDD = (float)sqrt(mDX*mDX+mDY*mDY)*0.8f;
	float	SSS = (float)sqrt((mX-mX1)*(mX-mX1)+(mY-mY1)*(mY-mY1));

	if (-2.f*DDD-70.f<=SSS&&SSS<=2.f*DDD+70.f)	return;

	float	cosF = (mX1-mX)/SSS;
	float	sinF = (mY1-mY)/SSS;

	float	X0 = mX+DDD*cosF;
	float	Y0 = mY+DDD*sinF;
	float	X1 = mX1-DDD*cosF;
	float	Y1 = mY1-DDD*sinF;

	Vector3D	p0(X0,Y0,(float)GetHeight((int)X0,(int)Y0));
	Vector3D	p1(X1,Y1,(float)GetHeight((int)X1,(int)Y1));

	float	X2 = X1-50.0f*cosF;
	float	Y2 = Y1-50.0f*sinF;

	float	X3 = X2-15.0f*sinF;
	float	Y3 = Y2+15.0f*cosF;
	float	X4 = X2+15.0f*sinF;
	float	Y4 = Y2-15.0f*cosF;

	float	X5 = X2-6.0f*sinF;
	float	Y5 = Y2+6.0f*cosF;
	float	X6 = X2+6.0f*sinF;
	float	Y6 = Y2-6.0f*cosF;

	float	X7 = X0-6.0f*sinF;
	float	Y7 = Y0+6.0f*cosF;
	float	X8 = X0+6.0f*sinF;
	float	Y8 = Y0-6.0f*cosF;

	Vector3D	p3(X3,Y3,(float)GetHeight((int)X3,(int)Y3));
	Vector3D	p4(X4,Y4,(float)GetHeight((int)X4,(int)Y4));

	Vector3D	p5(X5,Y5,(float)GetHeight((int)X5,(int)Y5));
	Vector3D	p6(X6,Y6,(float)GetHeight((int)X6,(int)Y6));

	Vector3D	p7(X7,Y7,(float)GetHeight((int)X7,(int)Y7));
	Vector3D	p8(X8,Y8,(float)GetHeight((int)X8,(int)Y8));

	GPS.DrawLine(p3,p1,color);
	GPS.DrawLine(p4,p1,color);

	GPS.DrawLine(p3,p5,color);
	GPS.DrawLine(p4,p6,color);

	GPS.DrawLine(p5,p7,color);
	GPS.DrawLine(p6,p8,color);

	GPS.DrawLine(p7,p8,color);
};

void	lvDrawArrow(be_CNODE* nd1,be_CNODE* nd2,int dx/* =100 */,int dy/* =150 */,DWORD color/* =0xFF00ff00 */){
	if (nd1!=NULL&&nd2!=NULL) {
		lvDrawArrow(nd1->x,nd1->y,nd2->x,nd2->y,dx,dy,color);
	};
};

void	DrawLineWidhTerraCross(Vector3D& p0,Vector3D& p1,DWORD Color,DWORD crossColor,bool TOP, bool BOTTOM,Vector3D& _crossPos){
	_crossPos.set(0.f,0.f,0.f);
	Vector3D crossPos;
    Vector3D dir;
	if (p0.z>p1.z) {
		float NORMA = sqrt((p0.x-p1.x)*(p0.x-p1.x)+(p0.y-p1.y)*(p0.y-p1.y)+(p0.z-p1.z)*(p0.z-p1.z));
		dir.set((p1.x-p0.x)/NORMA,(p1.y-p0.y)/NORMA,(p1.z-p0.z)/NORMA);
		if (ITerra->Pick(p0,dir,crossPos)&&crossPos.z!=p0.z){
			if (crossPos.z>p1.z) {
				if (TOP)	GPS.DrawLine(p0,crossPos,Color);
				if (BOTTOM) GPS.DrawLine(crossPos,p1,crossColor);
				_crossPos.set(crossPos.x,crossPos.y,crossPos.z);
				return;
			}else{
				if (TOP) GPS.DrawLine(p0,p1,Color);
				return;
			};
		}else{
			if (BOTTOM) GPS.DrawLine(p0,p1,crossColor);
			return;
		};
	}else if (p0.z<p1.z) {
		float NORMA = sqrt((p0.x-p1.x)*(p0.x-p1.x)+(p0.y-p1.y)*(p0.y-p1.y)+(p0.z-p1.z)*(p0.z-p1.z));
		dir.set((p0.x-p1.x)/NORMA,(p0.y-p1.y)/NORMA,(p0.z-p1.z)/NORMA);
		if (ITerra->Pick(p1,dir,crossPos)&&crossPos.z!=p1.z){
			if (crossPos.z>p0.z) {
				if (TOP)	GPS.DrawLine(p1,crossPos,Color);
				if (BOTTOM) GPS.DrawLine(crossPos,p0,crossColor);
				_crossPos.set(crossPos.x,crossPos.y,crossPos.z);
				return;
			}else{
				if (TOP) GPS.DrawLine(p1,p0,Color);
				return;
			};
		}else{
			if (BOTTOM) GPS.DrawLine(p1,p0,crossColor);
		};
	}else if (p0.z=p1.z) {
		if (p0.z<ITerra->GetH(p0.x,p0.y)) {
			if (BOTTOM) GPS.DrawLine(p0,p1,crossColor);
			return;
		}else{
			if (TOP) GPS.DrawLine(p0,p1,Color);
			return;
		};
	};
};
//////////////////////////////////////////////////////////////////////////
extern	DialogsSystem	DLG00,DLG01,DLG02;
extern	ClassEditor		EDT00,EDT01,EDT02;
//////////////////////////////////////////////////////////////////////////

// Active object /////////////////////////////////////////////////////////
be_CNODE* ActivObject = NULL;	// ��� �� ���������� ����.
lvCEdge*	pActivEdge	= NULL; //
//////////////////////////////////////////////////////////////////////////

// BattlePainter /////////////////////////////////////////////////////////
int		DLG_X0 = 745,		DLG01_X0 = 20;		
int		DLG_Y0 = 170,		DLG01_Y0 = 70;
int		DLG_W  = 255,		DLG01_W  = 700;
int		DLG_H  = 230,		DLG01_H  = 500;

extern	float fMapX;
extern	float fMapY;
extern	int	smaplx;
extern	int	smaply;

extern	int LastMx;
extern	int LastMy;
extern bool LockMouse;
extern	void	UnPress();

// lvCShapes /////////////////////////////////////////////////////////////
void	lvCShapes::SetNewR(float _R){
	if (_R!=R) {
		R = _R;

		assert(regular_polygon_4D.GetAmount());
		float dF = (2.f*3.1415) / regular_polygon_4D.GetAmount();
		// 4D
		for (int n=0; n<regular_polygon_4D.GetAmount(); n++){
			GetNextXY(regular_polygon_4D[n]->x,regular_polygon_4D[n]->y,dF,n);
		};
		// 6D
		assert(regular_polygon_6D.GetAmount());
			  dF = (2.f*3.1415) / regular_polygon_6D.GetAmount();
		for (    n=0; n<regular_polygon_6D.GetAmount(); n++){
			GetNextXY(regular_polygon_6D[n]->x,regular_polygon_6D[n]->y,dF,n);
		};
	};
};

void	lvCShapes::CreateEdge(){
	lvCPoint3D* pP3D = NULL;
	for (int n=0; n<4; n++){
		pP3D = new lvCPoint3D;
		regular_polygon_4D.Add(pP3D);
		pP3D=NULL;
	};
	for (    n=0; n<6; n++){
		pP3D = new lvCPoint3D;
		regular_polygon_6D.Add(pP3D);
		pP3D=NULL;
	};
};

void	lvCShapes::GetNextXY(float& nX,float& nY,float dF,int n){
	nX = R * cosf(dF*n);
	nY = R * sinf(dF*n);
};
void	lvCShapes::SetAngel_XY(int nA,float Fi){
	if (nA==4&&regular_polygon_4D.GetAmount()!=0) {
		for (int i=0; i<regular_polygon_4D.GetAmount(); i++){
			float	x = regular_polygon_4D[i]->x;
			float	y = regular_polygon_4D[i]->y;
			regular_polygon_4D[i]->x = x*cos(Fi)-y*sin(Fi);
			regular_polygon_4D[i]->y = x*sin(Fi)+y*cos(Fi);
		};
	}else if (nA==6&&regular_polygon_6D.GetAmount()!=0) {
		for (int i=0; i<regular_polygon_6D.GetAmount(); i++){
			float	x = regular_polygon_6D[i]->x;
			float	y = regular_polygon_6D[i]->y;
			regular_polygon_6D[i]->x = x*cos(Fi)-y*sin(Fi);
			regular_polygon_6D[i]->y = x*sin(Fi)+y*cos(Fi);
		};
	}
};
// CAMERA ////////////////////////////////////////////////////////////////
void MapPosToMapXMapY(int x, int y, float *pMapX, float *pMapY);
void	lvCRotateCamera::Set(Vector3D* pPOS0,int _TimeD){
	Vector3D	DefPOS(fMapX*32,fMapY*32);
	Set(&DefPOS,NULL,pPOS0,NULL,_TimeD);
	MoveType	= beTO_POS;
};
void	lvCRotateCamera::Set(float x,float y,int _TimeD){
	Vector3D	DefPOS(fMapX,fMapY);
//	GameInterface::SetPositionOnMap(x,y);
	float destMapX,destMapY;
	MapPosToMapXMapY(x,y,&destMapX,&destMapY);
	Vector3D	__Pos(destMapX,destMapY);
//	fMapX = DefPOS.x;
//	fMapY = DefPOS.y;
	Set(&DefPOS,NULL,&__Pos,NULL,_TimeD);
	MoveType = beTO_POS;
};
void	lvCRotateCamera::Set(Vector3D* pPOS0,Vector3D* pDIR0,int _TimeD){
    Set(&(ICam->GetLookAt()),&(ICam->GetDir()),pPOS0,pDIR0,_TimeD);
	MoveType	= beTO_POSDIR;
};

void	lvCRotateCamera::Set(Vector3D* pPOS0,Vector3D* pPOS1,int _TimeD,int type){
	float destMapX;
	float destMapY;
//	GameInterface::SetPositionOnMap(pPOS0->x,pPOS0->y);
	MapPosToMapXMapY(pPOS0->x,pPOS0->y,&destMapX,&destMapY);
	Vector3D	__Pos00(destMapX,destMapY);
//	GameInterface::SetPositionOnMap(pPOS1->x,pPOS1->y);
	MapPosToMapXMapY(pPOS1->x,pPOS1->y,&destMapX,&destMapY);
	Vector3D	__Pos01(destMapX,destMapY);
//	fMapX = cur_fMapX;
//	fMapY = cur_fMapY;
	Set(&__Pos00,NULL,&__Pos01,NULL,_TimeD);
	MoveType	= beTO_POSPOS;
};

void	lvCRotateCamera::Set(Vector3D* pPOS0,Vector3D* pDIR0,Vector3D* pPOS1,Vector3D* pDIR1,int _TimeD){
	MoveType	= beTO_POSDIR_POSDIR;
	LastTime	= 0;
	TimeD		= _TimeD;
	if (pPOS0!=NULL){
		POS0.x = pPOS0->x;
		POS0.y = pPOS0->y;
		POS0.z = pPOS0->z;
	};
	if (pDIR0!=NULL){
		DIR0.x = pDIR0->x;
		DIR0.y = pDIR0->y;
		DIR0.z = pDIR0->z;
	};
	if (pPOS1!=NULL){
		POS1.x = pPOS1->x;
		POS1.y = pPOS1->y;
		POS1.z = pPOS1->z;
	};
	if (pDIR1!=NULL){
		DIR1.x = pDIR1->x;
		DIR1.y = pDIR1->y;
		DIR1.z = pDIR1->z;
	};
	LastTime = GetTickCount();
};

void	lvCRotateCamera::Set(bool _useMapXY){
	useMapXY = _useMapXY;
};

void	lvCRotateCamera::Set(_str_ref& _vGrpID){
	MoveType = beTO_VGROUP;
	vGrpID	 = _vGrpID;
};

void	lvCRotateCamera::Draw(){
	if (MoveType==beTO_FREE)	return;

	void SetCameraPos(float x,float y);

	// Move to vGroupe
	if (MoveType==beTO_VGROUP) {
		BE_DelCamera();
		be_CGROUP* pvGrp = (be_CGROUP*)(vGrpID.Get());
		//if (pvGrp!=NULL||pvGrp->GetTotalAmount()!=0) {
		if( pvGrp && pvGrp->GetTotalAmount() ){
//			float cx=0.f,cy=0.f;
			int	  gx=0,  gy=0;
			pvGrp->GetGroupCenter(gx,gy);
//			cx = ((float)gx/32.f)-(float)smaplx/2.f;
//			cy = ((float)gy/32.f)-(float)smaply/2.f;
//			if (abs(cx-fMapX)>=1.f/32.f || abs(cy-fMapY)>=1.f/32.f) {
//				SetCameraPos(cx,cy);
//			};
			MapPosToMapXMapY(gx,gy,&fMapX,&fMapY);
		}else{
			MoveType = beTO_FREE;
		};
		return;
	};

	float Delta = 0.f;
	if (TimeD==0)	Delta = 1.f;
	else			Delta = (float)(GetTickCount()-LastTime)/(float)TimeD;
	if (Delta>=1.f)	Delta = 1.f;

	if (MoveType==beTO_POS||MoveType==beTO_POSPOS){
//		assert(false);
        if (!useMapXY){	/*ICam->Interpolate(Delta,POS0,POS1);*/ }
		else{
			fMapX = POS0.x + ( POS1.x - POS0.x ) * Delta;
			fMapY = POS0.y + ( POS1.y - POS0.y ) * Delta;
		};
	};
	if (MoveType==beTO_POSDIR||MoveType==beTO_POSDIR_POSDIR){
		assert(false);//ICam->Interpolate(Delta,POS0,DIR0,POS1,DIR1);
	};

	if (!useMapXY) BE_NewSetCamera();
	if (Delta==1.f)	MoveType=beTO_FREE;
};

void	lvCRotateCamera::Stop(){
	MoveType = beTO_FREE;
	BE_DelCamera();
};

// CameraPoss ////////////////////////////////////////////////////////////
const	char*	VectorPara::GetThisElementView(const char* LocalName){
	Descr = Name.pchar();
    Descr += "(";
	if (Pos.Get()!=NULL)	Descr += Pos.Get()->GetName();
	else					Descr += "NoPos";
	Descr += ",";
	if (Dir.Get()!=NULL)	Descr += Dir.Get()->GetName();
	else					Descr += "NoDir";
	Descr += ")";
	return Descr.pchar();
};

bool	SolveCircleLine(float vX,float vY,float X0,float Y0,float R,float Fi,float& x1,float& y1,float& x2,float& y2){
	float a = vX*cos(Fi)-vY*sin(Fi);
	float b = vX*sin(Fi)+vY*cos(Fi);
	
	if (a==0&&b==0)	return false;

	if (a==0.f){
		y1=y2=Y0;
		x1=X0+abs(R);
		x2=X0-abs(R);
		return true;
	};

	float NORMA = sqrt(a*a+b*b);

	x1=X0-b*(abs(R)/NORMA);
	x2=X0+b*(abs(R)/NORMA);

	y1=Y0+a*(abs(R)/NORMA);
	y2=Y0-a*(abs(R)/NORMA);

	return true;
};

void			VectorPara::Draw(){
	if (!Visible)	return;
	bool	cP = (Pos.Get()!=NULL);
	float	cX = 0.f;
	float	cY = 0.f;
	float	cZ = 0.f;
	bool	dP = (Dir.Get()!=NULL);
	float	dX = 0.f;
	float	dY = 0.f;
	float	dZ = 0.f;
	if (!cP||!dP)	return;
    if (cP) {
		cX = Pos.Get()->x;
		cY = Pos.Get()->y;
		cZ = Pos.Get()->z;
	};
	if (dP) {
		dX = Dir.Get()->x;
		dY = Dir.Get()->y;
		dZ = Dir.Get()->z;
	};
	Vector3D	cPOS(cX,cY,cZ);
	Vector3D	cDIR(dX,dY,dZ);

	DWORD		color = 0xFFff0000;
	DWORD		color1= 0xFF0000ff;

	float		nR = 50.f;
	Vector3D	pNn[6];
	for (int i=0; i<6; i++){
		pNn[i].set(	Shapes_MAP(nR)->regular_polygon_6D[i]->x+cX,
					Shapes_MAP(nR)->regular_polygon_6D[i]->y+cY,
					(float)GetHeight(	(int)Shapes_MAP(nR)->regular_polygon_6D[i]->x+cX,
										(int)Shapes_MAP(nR)->regular_polygon_6D[i]->y+cY	)
					);
	};	
	GPS.DrawLine(pNn[0],pNn[1],color1);
	GPS.DrawLine(pNn[1],pNn[2],color1);
	GPS.DrawLine(pNn[2],pNn[3],color1);
	GPS.DrawLine(pNn[3],pNn[4],color1);
	GPS.DrawLine(pNn[4],pNn[5],color1);
	GPS.DrawLine(pNn[5],pNn[0],color1);

	Vector3D	bottom(cX,cY,(float)GetHeight((int)cX,(int)cY));
	Vector3D	top(cX,cY,cZ);
    GPS.DrawLine(bottom,top,color1);

	Vector3D	prCENTER;
	Plane::xOy.intersect(Line3D(cPOS,cDIR),prCENTER);
	GPS.DrawLine(cPOS,prCENTER,color1);
	GPS.DrawLine(prCENTER,bottom,color1);

	float	R=1000.f;
	float	x1,y1,x2,y2;
	Vector3D	LendPos1,LendPos2,LendPos3,LendPos4;
	bool		pr1,pr2;
	pr1 = SolveCircleLine(dX,dY,prCENTER.x,prCENTER.y,R,3.1415f/4.f,x1,y1,x2,y2);
	if (pr1){
		LendPos1.set(x1,y1,(float)GetHeight((int)x1,(int)y1));
		LendPos3.set(x2,y2,(float)GetHeight((int)x2,(int)y2));
		GPS.DrawLine(top,LendPos1,color);
		GPS.DrawLine(top,LendPos3,color);
	};
	pr2 = SolveCircleLine(dX,dY,prCENTER.x,prCENTER.y,R,-3.1415f/4.f,x1,y1,x2,y2);
	if (pr2){
		LendPos2.set(x1,y1,(float)GetHeight((int)x1,(int)y1));
		LendPos4.set(x2,y2,(float)GetHeight((int)x2,(int)y2));
		GPS.DrawLine(top,LendPos2,color);
		GPS.DrawLine(top,LendPos4,color);
	};
	if (pr1&&pr2){
		GPS.DrawLine(LendPos1,LendPos2,color);
		GPS.DrawLine(LendPos2,LendPos3,color);
		GPS.DrawLine(LendPos3,LendPos4,color);
		GPS.DrawLine(LendPos4,LendPos1,color);
	};
};

void	CameraPoss::Draw(){
	for (int i=0; i<CamerasPoss.GetAmount(); i++){
		CamerasPoss[i]->Draw();
	};
};
// lvCDialogBased ////////////////////////////////////////////////////////
				lvCDialogBased::lvCDialogBased(){ 
	InfID=_lvCDialogBased_; 
	Visible=false; 

	sFilePath="NoPath";
	SDName="NoName";

	pDS=NULL;
	pSD=NULL;
};
				lvCDialogBased::~lvCDialogBased(){
	if (pDS!=NULL){
		int N=pDS->DSS.GetAmount();
		while (N--) {
			if (pDS->DSS[N]==pSD){
				pDS->DSS[N]=NULL;
				pDS->DSS.DelElement(N);
			};
		};
	};
	if (pSD!=NULL){
		delete pSD;
		pSD=NULL;
	};
};
void			lvCDialogBased::DRAW(){
	if (Visible==true){
		if (Effects_IsFinished()==true){
			Efects_Start();
		};
	};
};
SimpleDialog*	lvCDialogBased::GetSimpleDialog(){
	if (pSD==NULL){
		// load graphic storage
		ClassArray<SimpleDialog>	lSD_Arr;
		xmlQuote	xmlData( "DS_STORAGE" ); 
		xmlData.ReadFromFile( sFilePath.pchar() );
		ErrorPager Err;
		lSD_Arr.Load( xmlData, &lSD_Arr, &Err );

		// fine object by name
		int N = lSD_Arr.GetAmount();
		while (N--) {
			if (strcmp(lSD_Arr[N]->Name.pchar(),SDName.pchar())==0){
				pSD = lSD_Arr[N];
				lSD_Arr[N]=NULL;
			};
		};
	};
	return pSD;
};
void			lvCDialogBased::AddToDS(DialogsSystem* _pDS){
	if (_pDS!=NULL){
		if (GetSimpleDialog()!=NULL){
			pDS=_pDS;
			pDS->AddDialog(pSD);
		};
	};
};
bool			lvCDialogBased::Effects_IsFinished(){
	bool	ret = true;
	vui_BasicEffect* pEfect=NULL;
	if (pSD!=NULL) {
		for (int i=0; i<pSD->v_Actions.GetAmount(); i++){
			pEfect = dynamic_cast<vui_BasicEffect*>(pSD->v_Actions[i]);
			if (pEfect!=NULL){
				ret = ret && pEfect->isFinished();
				pEfect=NULL;
			};
		};
	};
	return ret;
};
void			lvCDialogBased::Efects_Start(){
	vui_BasicEffect* pEfect=NULL;
	if (pSD!=NULL) {
		for (int i=0; i<pSD->v_Actions.GetAmount(); i++){
			pEfect = dynamic_cast<vui_BasicEffect*>(pSD->v_Actions[i]);
			if (pEfect!=NULL){
				pEfect->StartEffect();
				pEfect=NULL;
			};
		};
	};
};
// lvCBlackScreen ////////////////////////////////////////////////////////
void	lvCBlackScreen::UpDate(){
	if ( (Visible==true && Alpha==255) || (Visible==false&&Alpha==0) ){
		RealTime = 0;
		return;
	}else{
		Alpha += (int)(( (float)( (int)(GetTickCount()-RealTime) ) ) * a_sign);
		if (Alpha>255)	Alpha = 255;
		if (Alpha<0)	Alpha = 0;
		RealTime = GetTickCount();
	};
};

void	lvCBlackScreen::DRAW(){
	UpDate();
	if (Alpha!=0) {
		GPS.DrawFillRect(0.f,0.f,(float)RealLx,(float)RealLy,Alpha*0x01000000);
	};
};

void	lvCBlackScreen::StartShow(){
	if ( TestSart() )	return;
	RealTime = GetTickCount();
	if (AppearTime!=0)	a_sign = 255.f/(float)AppearTime;
	else				a_sign = 255.f;
	Visible = true;
};

void	lvCBlackScreen::StopShow(){
	if ( TestStop() )	return;
	RealTime = GetTickCount();
	if (DisapperTime!=0)	a_sign = -255.f/(float)DisapperTime;
	else					a_sign = -255.f;
	Visible = false;
};

void	lvCBlackScreen::SetVisible(bool _vs){
	Visible	= _vs;
	RealTime = GetTickCount();
	if (_vs) {
		Alpha	= 255;
		if (DisapperTime!=0)	a_sign = -255.f/(float)DisapperTime;
		else					a_sign = -255.f;
	}else{
		Alpha	= 0;
		if (AppearTime!=0)	a_sign = 255.f/(float)AppearTime;
		else				a_sign = 255.f;
	};	
};

bool	lvCBlackScreen::GetVisible(){
	return (Visible==true && Alpha==0xFF);
};

bool	lvCBlackScreen::isVissible(){
	return (Visible==true && Alpha==0xFF);
};

bool	lvCBlackScreen::isInVisible(){
	return (Visible==false && Alpha==0x00);
};

void	lvCBlackScreen::SetAppearTime(int _time){
	AppearTime = _time;
};

void	lvCBlackScreen::SetDisapperTime(int _time){
	DisapperTime = _time;
};

int		lvCBlackScreen::GetAppearTime(){
	return	AppearTime;
};

int		lvCBlackScreen::GetDisapperTime(){
	return	DisapperTime;
};

bool	lvCBlackScreen::TestSart(){
	return	(Visible==true && a_sign>0);
};

bool	lvCBlackScreen::TestStop(){
	return	(Visible==false && a_sign<0);
};
// lvCMoveGP /////////////////////////////////////////////////////////////
void	lvCMoveGP::UpDate(){
    bool	Already  = Visible==true && ( ( (BorderType==0||BorderType==1) && m_sign>0 ) || ( (BorderType==2||BorderType==3) && m_sign<0 ) );
	Already	= Already && (Visible==false && ( ( (BorderType==0||BorderType==1) && m_sign<0 ) || ( (BorderType==2||BorderType==3) && m_sign>0 ) ));

	if (Already) {
		RealTime = 0;
		return;
	}else{
		SetSpriteWH();

		if (ExTD)	MovePos += (float)(*pTimeD)*m_sign;
		else		MovePos += (float)(GetTickCount()-RealTime)*m_sign;
		RealTime = GetTickCount();

		switch(BorderType) {
		case 0:		// TOP 
			if (MovePos>(float)(Height))	MovePos=(float)(Height);
			if (MovePos<0.f)				MovePos=0.f;
			break;
		case 1:		// LEFT
			if (MovePos>(float)(Width))	MovePos=(float)(Width);
			if (MovePos<0.f)				MovePos=0.f;
			break;
		case 2:		// RIGHT
			if (MovePos<(float)(RealLx-Width))	MovePos=(float)(RealLx-Width);
			if (MovePos>(float)(RealLx))		MovePos=(float)(RealLx);
			break;
		case 3:		// BOTTOM
			if (MovePos<(float)(RealLy-Height))	MovePos=(float)(RealLy-Height);
			if (MovePos>(float)(RealLy))		MovePos=(float)(RealLy);
			break;
		}
		
	};
};

void	lvCMoveGP::DRAW(){
	UpDate();
	switch(BorderType) {
	case 0:		// TOP
		if (MovePos<=-(float)Height)	return;
		break;
	case 1:		// LEFT
		if (MovePos<=-(float)Width)	return;
		break;
	case 2:		// RIGHT
		if (MovePos>=(float)RealLx)	return;
		break;
	case 3:		// BOTTOM
		if (MovePos>=(float)RealLy)	return;
		break;
	}
	
	// Now we mast draw GP
	int LastPos = 0;
	if (BorderType==0||BorderType==3){
		LastPos = Width;
		if (BorderType==0){
			x = BeginPos;
			y = MovePos-(float)Height;
		};
		if (BorderType==3){
			x = BeginPos;
			y = (int)MovePos;
		};
		GPS.ShowGP(x,y,FileID,SpriteID,Nat);
		if (Stretched){
			while (LastPos+BeginPos<EndPos) {
				if (BorderType==0)	GPS.ShowGP(BeginPos+LastPos,(int)(MovePos-(float)Height),FileID,SpriteID,Nat);
				if (BorderType==3)	GPS.ShowGP(BeginPos+LastPos,(int)MovePos,FileID,SpriteID,Nat);
				LastPos += Width;
			};
		};
	};
	if (BorderType==1||BorderType==2){
		LastPos = Height;
		if (BorderType==1){
			x = (int)(MovePos-(float)Width);
			y = BeginPos;
		};
		if (BorderType==2) GPS.ShowGP((int)MovePos,BeginPos,FileID,SpriteID,Nat);
		GPS.ShowGP(x,y,FileID,SpriteID,Nat);
		if (Stretched){
			while (LastPos<EndPos) {
				if (BorderType==1) GPS.ShowGP((int)(MovePos-(float)Width),BeginPos+LastPos,FileID,SpriteID,Nat);
				if (BorderType==2) GPS.ShowGP((int)MovePos,BeginPos+LastPos,FileID,SpriteID,Nat);
				LastPos += Height;
			};
		};
	};
};

void	lvCMoveGP::StartShow(){
	if ( TestSart() )	return;
	SetSpriteWH();
	RealTime = GetTickCount();
	if (ExTD)	RealTime = 0;
	SetSign(true);
	Visible = true;
};

void	lvCMoveGP::StopShow(){
	if ( TestStop() )	return;
	SetSpriteWH();
	RealTime = GetTickCount();
	if (ExTD)	RealTime = 0;
	SetSign(false);
	Visible = false;
};

void	lvCMoveGP::SetVisible(bool _vs){
	Visible	= _vs;
	RealTime = GetTickCount();
	if (ExTD)	RealTime = 0;
	SetSpriteWH();
	SetMovePos(_vs);
	SetSign(_vs);
};

bool	lvCMoveGP::GetVisible(){
	return isVissible();
};

bool	lvCMoveGP::isVissible(){
	bool	retVAL = ( Visible==true );
	switch(BorderType) {
	case 0:		// TOP 
		retVAL = retVAL && ( MovePos >= (float)0.f );
		break;
	case 1:		// LEFT
		retVAL = retVAL && ( MovePos >= (float)0.f );
		break;
	case 2:		// RIGHT
		retVAL = retVAL && ( MovePos <= (float)(RealLx - Width) );
		break;
	case 3:		// BOTTOM
		retVAL = retVAL && ( MovePos <= (float)(RealLy - Height) );
		break;
	}
	return retVAL;
};

bool	lvCMoveGP::isInVisible(){
	
	bool	retVAL = ( Visible==false );
	switch(BorderType) {
	case 0:		// TOP 
		retVAL = retVAL && ( MovePos <= 0.f );
		break;
	case 1:		// LEFT
		retVAL = retVAL && ( MovePos <= 0.f );
		break;
	case 2:		// RIGHT
		retVAL = retVAL && ( MovePos >= (float)RealLx );
		break;
	case 3:		// BOTTOM
		retVAL = retVAL && ( MovePos >= (float)RealLy );
		break;
	}
	return retVAL;
};

void	lvCMoveGP::SetAppearTime(int _time){
	AppearTime = _time;
};

void	lvCMoveGP::SetDisapperTime(int _time){
	DisapperTime = _time;
};

int		lvCMoveGP::GetAppearTime(){
	return	AppearTime;
};

int		lvCMoveGP::GetDisapperTime(){
	return	DisapperTime;
};

void	lvCMoveGP::SetSpriteWH(){
	Width  = 0;
	Height = 0;
	if (FileID!=0xFFFF){
		if (0<=SpriteID&&GPS.GPNFrames(FileID))
		Width	= GPS.GetGPWidth(FileID,SpriteID);
		Height	= GPS.GetGPHeight(FileID,SpriteID);
	};
};

void	lvCMoveGP::SetSign(bool appear){
	if (appear){	// ���� ����
		switch(BorderType) {
		case 0:			// TOP 
			if (AppearTime!=0)		m_sign=(float)Height/(float)AppearTime;
			else					m_sign=(float)Height;
			break;
		case 1:			// LEFT
			if (AppearTime!=0)		m_sign=(float)Width/(float)AppearTime;
			else					m_sign=(float)Width;
			break;
		case 2:			// RIGHT
			if (AppearTime!=0)		m_sign=-(float)Width/(float)AppearTime;
			else					m_sign=-(float)Width;
			break;
		case 3:			// BOTTOM
			if (AppearTime!=0)		m_sign=-(float)Height/(float)AppearTime;
			else					m_sign=-(float)Height;
			break;
		};
	}else{			// ���� ����
		switch(BorderType) {
		case 0:			// TOP 
			if (DisapperTime!=0)	m_sign=-(float)Height/(float)DisapperTime;
			else					m_sign=-(float)Height;
			break;
		case 1:			// LEFT
			if (DisapperTime!=0)	m_sign=-(float)Width/(float)DisapperTime;
			else					m_sign=-(float)Width;
			break;
		case 2:			// RIGHT
			if (DisapperTime!=0)	m_sign=(float)Width/(float)DisapperTime;
			else					m_sign=(float)Width;
			break;
		case 3:			// BOTTOM
			if (DisapperTime!=0)	m_sign=(float)Height/(float)DisapperTime;
			else					m_sign=(float)Height;
			break;
		};
	};

	// Rounr m_singn to 0.001
	int temp_sign = (int)(m_sign*1000.f);
	m_sign = (float)temp_sign/1000.f;
};

void	lvCMoveGP::SetMovePos(bool visible){
	MovePos = 0.f;
	switch(BorderType) {
	case 0:			// TOP 
		if (visible==true)  MovePos	= (float)Height;
		if (visible==false) MovePos	= 0.f;
		break;
	case 1:			// LEFT
		if (visible==true)	MovePos = (float)Width;
		if (visible==false)	MovePos = 0.f;
		break;
	case 2:			// RIGHT
		if (visible==true)	MovePos	= (float)(RealLx-Width);
		if (visible==false)	MovePos	= (float)RealLx;
		break;
	case 3:			// BOTTOM
		if (visible==true)	MovePos = (float)(RealLy-Height);
		if (visible==false)	MovePos = (float)RealLy;
		break;
	};
};

void	lvCMoveGP::UseExternalTimeD(int* pTD){
	if (pTD!=NULL){
		ExTD = true;
		pTimeD = pTD;
	};
};

bool	lvCMoveGP::TestSart(){
	return	( Visible==true && 
				( ( (BorderType==0||BorderType==1) && m_sign>0 ) || 
				  ( (BorderType==2||BorderType==3) && m_sign<0 ) 
				 ) 
			);
};

bool	lvCMoveGP::TestStop(){
	return	( Visible==false && 
				( ( (BorderType==0||BorderType==1) && m_sign<0 ) ||
				  ( (BorderType==2||BorderType==3) && m_sign>0 ) 
				 ) 
			);
};
// lvCAAppearGP //////////////////////////////////////////////////////////
void	lvCAAppearGP::UpDate(){
	if (Flick){
		if ( (Visible==true && Alpha==0xFF) || (Visible==false&&Alpha==0x00) ){
			if (Visible==true && Alpha==0xFF) {
				StopShow();
			}else if (Visible==false&&Alpha==0x00) {
				StartShow();
			};
		}else{
			Alpha += (float)(GetTickCount()-RealTime)*a_sign;
			if (Alpha>255)	Alpha = 255;
			if (Alpha<0)	Alpha = 0;
			RealTime = GetTickCount();
		};
	}else{
		lvCBlackScreen::UpDate();
	};
	if (OnCenterSCR){
		int aH = GPS.GetGPHeight(FileID,SpriteID);
		int aW = GPS.GetGPWidth(FileID,SpriteID);
		posX = abs(RealLx-GPS.GetGPWidth(FileID,SpriteID))/2;
		posY = abs(RealLy-GPS.GetGPHeight(FileID,SpriteID))/2;
	};
};

void	lvCAAppearGP::DRAW(){
	UpDate();
	if (Alpha!=0) {
		DWORD curDiffuse = GPS.GetCurrentDiffuse();
		GPS.SetCurrentDiffuse(Alpha*0x01000000+0x00FFFFFF);
		GPS.ShowGP(posX+dx,posY+dy,FileID,SpriteID,Nat);
		GPS.SetCurrentDiffuse(curDiffuse);
	};
};

void	lvCAAppearGP::SetPosXY(int x,int y){
	posX = x;
	posY = y;
};

void	lvCAAppearGP::SetSprite(int id){
	SpriteID = id;
};
void	lvCAAppearGP::getPictureSize(int& w,int& h){
	w = GPS.GetGPWidth(FileID,SpriteID);
	h = GPS.GetGPHeight(FileID,SpriteID);
};
// lvCAnimateGP //////////////////////////////////////////////////////////
lvCAnimateGP::lvCAnimateGP() : lvCAAppearGP() {
	InfID = _lvCAnimateGP_;

	Animate			= false;
	SpriteLastID	= SpriteID;
	DelayTime		= 0;

	CurSprite		= SpriteID;
	LastUpdeteTime	= 0;
};

void	lvCAnimateGP::UpDate(bool stopANIM/*=false*/){
	lvCAAppearGP::UpDate();
	if (Animate&&(SpriteLastID-SpriteID)>0) {
		DWORD deltaT = GetTickCount()-LastUpdeteTime;
        if (deltaT>=DelayTime) {
			int NC = 1;
			if (DelayTime!=0)	NC = deltaT/(DWORD)DelayTime;
			CurSprite = (CurSprite+NC)%(SpriteLastID-SpriteID);
			if (stopANIM&&CurSprite==SpriteID)	Animate=false;
			LastUpdeteTime = GetTickCount();
		};
	};
};

void	lvCAnimateGP::DRAW(bool stopANIM/*=false*/){
	UpDate(stopANIM);
	if (Alpha!=0) {
		DWORD curDiffuse = GPS.GetCurrentDiffuse();
		GPS.SetCurrentDiffuse(Alpha*0x01000000+0x00FFFFFF);
		GPS.ShowGP(posX+dx,posY+dy,FileID,CurSprite,Nat);
		GPS.SetCurrentDiffuse(curDiffuse);
	};
};

// lvCDeffFilmMenu ///////////////////////////////////////////////////////
lvCDeffFilmMenu::lvCDeffFilmMenu(){
	InfID	= _lvCDeffFilmMenu_;
	Visible = false;
	Nat		= 0;
	RealTime= 0;
	Frame_State = 0;

	lastLx = RealLx;
	lastLy = RealLy;

	//assert(DS_Load());
	DS_Load();

	SetAlexanderSet();
};
void	lvCDeffFilmMenu::Draw_TOP_LINE(){
	GPS.FlushBatches();
	TOP_LINE.DRAW();
};
void	lvCDeffFilmMenu::Draw_LEFT_BOX(){
	GPS.FlushBatches();
	LEFT_BOX.DRAW();
};
void	lvCDeffFilmMenu::Draw_RIGHT_BOX(){
	GPS.FlushBatches();
	RIGHT_BOX.DRAW();
};
void	lvCDeffFilmMenu::Draw_CENTER_BOX(){
	GPS.FlushBatches();

	CENTER_BOX.DRAW();
};
void	lvCDeffFilmMenu::Draw_LEFT_MORDA(){
	LEFT_MORDA.SetPosXY(LEFT_BOX.x,LEFT_BOX.y);
	GPS.FlushBatches();
	LEFT_MORDA.DRAW();
};
void	lvCDeffFilmMenu::Draw_RIGHT_MORDA(){
	RIGHT_MORDA.SetPosXY(RIGHT_BOX.x,RIGHT_BOX.y);
	GPS.FlushBatches();
	RIGHT_MORDA.DRAW();
};
void	lvCDeffFilmMenu::Draw_FRAME(){
	// Alexandr frame
	if (Frame_State==1){
		FRAME.dx = L_F_dx/*16*//*15*/;
		FRAME.dy = L_F_dy/*19*//*14*/;
		FRAME.SetPosXY(LEFT_BOX.x,LEFT_BOX.y);
	}else if (Frame_State==2){
		FRAME.SetPosXY(RIGHT_BOX.x,RIGHT_BOX.y);
		FRAME.dx = R_F_dx/*594*//*42*/;
		FRAME.dy = R_F_dy/*19*//*14*/;
	};
	if (Frame_State!=0){
		GPS.FlushBatches();
		FRAME.DRAW();
	};

	// CosscsII frame
	if (Frame_State==1){
		CII_FRAME.dx = L_F_dx;
		CII_FRAME.dy = L_F_dy;
		CII_FRAME.SetPosXY(LEFT_BOX.x,LEFT_BOX.y);
	}else if (Frame_State==2){
		CII_FRAME.dx = R_F_dx;
		CII_FRAME.dy = R_F_dy;
		CII_FRAME.SetPosXY(RIGHT_BOX.x,RIGHT_BOX.y);
	};
	if (Frame_State!=0){
		GPS.FlushBatches();
		CII_FRAME.DRAW();
	};
};
void	lvCDeffFilmMenu::Draw_TEXT(){
	vvTEXT* pText = TextStr.Get();
	if (pText!=NULL){
		DS_Visible("TextDialog",GetVisible());
		DS_Visible("SomeText",  GetVisible());

		// Set position for panel
		mLx = RIGHT_BOX.x-LEFT_BOX.Width-2*txX+550;
		mLy = CENTER_BOX.Height-txY-35;
		DS_Pos("TextDialog",CENTER_BOX.x+txX,CENTER_BOX.y+txY);
		DS_Size("TextDialog",mLy-2+TXT_add_y_size,mLx+TXT_add_x_size);
		DS_SetMaxMessWidth(mLx-25);

		_str TextInPanel;
		if (pText->SpeakerID.pchar()!=NULL){
			TextInPanel=GetTextByID(pText->SpeakerID.pchar());
			TextInPanel+="\\";
		};
		TextInPanel+="{C 0xFF000000}";
		TextInPanel+=reinterpret_cast<char*>(pText->Get());
		/*
		DrawMultilineText(	CENTER_BOX.x+txX,CENTER_BOX.y+txY,
						//	reinterpret_cast<char*>(pText->Get()),
							TextInPanel.pchar(),
							&BlackFont,
							mLx,mLy,
							0,txST
						  );*/
		DS_SetMessage(TextInPanel.pchar());

		DS_Text.ProcessDialogs();

	}else{
		DS_Visible("TextDialog",false);
		DS_Visible("SomeText",  false);
	};
};
void	lvCDeffFilmMenu::Draw_HNAME(){
	if (CII_HeroName_style==false){
		DS_Visible("HLName",false);
		DS_Visible("HRName",false);
		return;
	};

	vvTEXT* pLHN = LeftName.Get();
	vvTEXT*	pRHN = RightName.Get();
	if ((pLHN!=NULL||pRHN!=NULL)&&CII_HeroName_style) {
		DS_Visible("HLName",GetVisible());
		DS_Visible("HRName",GetVisible());

		if (pLHN&&pLHN->SpeakerID.pchar()!=NULL){
			DS_SetLHName(GetTextByID(pLHN->SpeakerID.pchar()));
		};
		
		if (pRHN&&pRHN->SpeakerID.pchar()!=NULL){
			DS_SetRHName(GetTextByID(pRHN->SpeakerID.pchar()));
		};

		// Set position fro hero names
		DS_PosCenter("HLName",LEFT_BOX.x+HNL_x,LEFT_BOX.y+HNL_y);
		DS_PosCenter("HRName",RIGHT_BOX.x+HNR_x,RIGHT_BOX.y+HNR_y);

	};
		if (pLHN==NULL||DoNotShowLeftMorda==true)	DS_Visible("HLName",false);
		if (pRHN==NULL||DoNotShowRightMorda==true)	DS_Visible("HRName",false);
};
void	lvCDeffFilmMenu::DRAW(){
	RealTime = GetTickCount()-RealTime;
	
	TOP_LINE.EndPos			= RealLx;
	CENTER_BOX.EndPos		= RealLx+C_B_dep/*-765*/;

	Draw_TOP_LINE();
	Draw_CENTER_BOX();
	Draw_LEFT_BOX();
	Draw_RIGHT_BOX();

	if (DoNotShowLeftMorda==false)	Draw_LEFT_MORDA();

	if (DoNotShowRightMorda==false)	Draw_RIGHT_MORDA();

	Draw_FRAME();

	Draw_HNAME();
	Draw_TEXT();

	RealTime = GetTickCount();
};	

void	lvCDeffFilmMenu::StartShow_LEFT_MORDA(){
	LEFT_MORDA.StartShow();
};
void	lvCDeffFilmMenu::StartShow_RIGHT_MORDA(){
	RIGHT_MORDA.StartShow();
};
bool	lvCDeffFilmMenu::TestSart_LEFT_MORDA(){
	return LEFT_MORDA.TestSart();
};
bool	lvCDeffFilmMenu::TestSart_RIGHT_MORDA(){
	return RIGHT_MORDA.TestSart();
};
void	lvCDeffFilmMenu::StartShow(){
	if (TOP_LINE.TestSart()&&
		LEFT_BOX.TestSart()&&
		RIGHT_BOX.TestSart()&&
		CENTER_BOX.TestSart()&&
		TestSart_LEFT_MORDA()&&
		TestSart_RIGHT_MORDA())	return;

	Visible = false;
	RealTime = GetTickCount();
	SetDeffParam();

	TOP_LINE.UseExternalTimeD(&RealTime);
	LEFT_BOX.UseExternalTimeD(&RealTime);
	RIGHT_BOX.UseExternalTimeD(&RealTime);
	CENTER_BOX.UseExternalTimeD(&RealTime);

	TOP_LINE.StartShow();
	LEFT_BOX.StartShow();
	RIGHT_BOX.StartShow();
	CENTER_BOX.StartShow();

	StartShow_LEFT_MORDA();
	StartShow_RIGHT_MORDA();
	Visible = true;

	DS_Visible("TextDialog",true);
	DS_Visible("SomeText",  true);
};

void	lvCDeffFilmMenu::StopShow_LEFT_MORDA(){
	LEFT_MORDA.StopShow();
};
void	lvCDeffFilmMenu::StopShow_RIGHT_MORDA(){
	RIGHT_MORDA.StopShow();
};
bool	lvCDeffFilmMenu::TestStop_LEFT_MORDA(){
	return LEFT_MORDA.TestStop();
};
bool	lvCDeffFilmMenu::TestStop_RIGHT_MORDA(){
	return RIGHT_MORDA.TestStop();
};
void	lvCDeffFilmMenu::StopShow(){
	if (TOP_LINE.TestStop()&&
		LEFT_BOX.TestStop()&&
		RIGHT_BOX.TestStop()&&
		CENTER_BOX.TestStop()&&
		TestStop_LEFT_MORDA()&&
		TestStop_RIGHT_MORDA())	return;

	Frame_State = 0;

	Visible = true;
	RealTime = GetTickCount();
	SetDeffParam();

	TOP_LINE.UseExternalTimeD(&RealTime);
	LEFT_BOX.UseExternalTimeD(&RealTime);
	RIGHT_BOX.UseExternalTimeD(&RealTime);
	CENTER_BOX.UseExternalTimeD(&RealTime);

	TOP_LINE.StopShow();
	LEFT_BOX.StopShow();
	RIGHT_BOX.StopShow();
	CENTER_BOX.StopShow();

	StopShow_LEFT_MORDA();
	StopShow_RIGHT_MORDA();
	Visible = false;

	DS_Visible("TextDialog",false);
	DS_Visible("SomeText",  false);
};

void	lvCDeffFilmMenu::SetVisible_LEFT_MORDA(bool _vs){
	LEFT_MORDA.SetVisible(_vs);
};
void	lvCDeffFilmMenu::SetVisible_RIGHT_MORDA(bool _vs){
	RIGHT_MORDA.SetVisible(_vs);
};
void	lvCDeffFilmMenu::SetVisible(bool _vs){
	RealTime = GetTickCount();
	SetDeffParam();

	TOP_LINE.UseExternalTimeD(&RealTime);
	LEFT_BOX.UseExternalTimeD(&RealTime);
	RIGHT_BOX.UseExternalTimeD(&RealTime);
	CENTER_BOX.UseExternalTimeD(&RealTime);

	TOP_LINE.SetVisible(_vs);
	LEFT_BOX.SetVisible(_vs);
	RIGHT_BOX.SetVisible(_vs);
	CENTER_BOX.SetVisible(_vs);
	SetVisible_LEFT_MORDA(_vs);
	SetVisible_RIGHT_MORDA(_vs);
};

bool	lvCDeffFilmMenu::GetVisible(){
	return isVissible();
};

bool	lvCDeffFilmMenu::isVissible_LEFT_MORDA(){
	if (DoNotShowLeftMorda)		return true;
	return LEFT_MORDA.isVissible();
};
bool	lvCDeffFilmMenu::isVissible_RIGHT_MORDA(){
	if (DoNotShowRightMorda)	return true;
	return RIGHT_MORDA.isVissible();
};
bool	lvCDeffFilmMenu::isVissible(){
    return	TOP_LINE.isVissible()&&
			LEFT_BOX.isVissible()&&
			RIGHT_BOX.isVissible()&&
			CENTER_BOX.isVissible()&&
			isVissible_LEFT_MORDA()&&
			isVissible_RIGHT_MORDA();
};

bool	lvCDeffFilmMenu::isInVisible_LEFT_MORDA(){
	return LEFT_MORDA.isInVisible();
};
bool	lvCDeffFilmMenu::isInVisible_RIGHT_MORDA(){
	return RIGHT_MORDA.isInVisible();
};
bool	lvCDeffFilmMenu::isInVisible(){
	return	TOP_LINE.isInVisible()&&
			LEFT_BOX.isInVisible()&&
			RIGHT_BOX.isInVisible()&&
			CENTER_BOX.isInVisible()&&
			isInVisible_LEFT_MORDA()&&
			isInVisible_RIGHT_MORDA();
};

void	lvCDeffFilmMenu::SetLeftMorda(int _id){
	LEFT_MORDA.SetSprite(_id);
};

void	lvCDeffFilmMenu::SetRightMorda(int _id){
	RIGHT_MORDA.SetSprite(_id);
};

void	lvCDeffFilmMenu::SetFarmeState(int _st){
	Frame_State = _st;
	if (CII_HeroName_style){
		if (Frame_State==1||Frame_State==2)
			CII_FRAME.SetVisible(true);
		else 
			CII_FRAME.SetVisible(false);
	};
};

void	lvCDeffFilmMenu::SetText(vvTEXT* pText){
	if (pText!=NULL){
		TextStr.Set(pText);
	};
};

void	lvCDeffFilmMenu::ShowText(bool _st){
	txST = _st;
};

void	lvCDeffFilmMenu::PlayText(int _canal){
	vvTEXT* pTetx = TextStr.Get();
	if (pTetx!=NULL&&CheckFilePath(pTetx->oggFile.pchar())){
		ov_Play(pTetx->oggFile.pchar(),(DWORD)_canal);
	};
};

void	lvCDeffFilmMenu::SetDeffParam_LEFT_MORDA(){
	LEFT_MORDA.SetAppearTime(1000);
	LEFT_MORDA.SetDisapperTime(1000);
	LEFT_MORDA.dx = L_M_dx/*25*//*24*/;
	LEFT_MORDA.dy = L_M_dy/*27*//*23*/;
	LEFT_MORDA.SetVisible(Visible);
};
void	lvCDeffFilmMenu::SetDeffParam_RIGHT_MORDA(){
	RIGHT_MORDA.SetAppearTime(1000);
	RIGHT_MORDA.SetDisapperTime(1000);
	RIGHT_MORDA.dx = R_M_dx/*603*//*51*/;
	RIGHT_MORDA.dy = R_M_dy/*27*//*23*/;
	RIGHT_MORDA.SetVisible(Visible);
};
void	lvCDeffFilmMenu::SetDeffParam(){
		
	TOP_LINE.SetSpriteWH();
	TOP_LINE.BorderType		= 0;
	TOP_LINE.Stretched		= true;
	TOP_LINE.BeginPos		= 0;
	TOP_LINE.EndPos			= RealLx;
	TOP_LINE.SetVisible(Visible);
	

	LEFT_BOX.SetSpriteWH();
	LEFT_BOX.BorderType		= 3;
	LEFT_BOX.Stretched		= false;
	LEFT_BOX.BeginPos		= 0;
	LEFT_BOX.SetVisible(Visible);
		
	CENTER_BOX.SetSpriteWH();
	CENTER_BOX.BorderType	= 3;
	CENTER_BOX.Stretched	= true;
	CENTER_BOX.BeginPos		= LEFT_BOX.Width;
	CENTER_BOX.EndPos		= RealLx+C_B_dep/*-765*/;
	CENTER_BOX.SetVisible(Visible);

	RIGHT_BOX.SetSpriteWH();
	RIGHT_BOX.BorderType	= 3;
	RIGHT_BOX.Stretched		= false;
	RIGHT_BOX.BeginPos		= RealLx-RIGHT_BOX.Width;
	RIGHT_BOX.SetVisible(Visible);
	
	SetDeffParam_LEFT_MORDA();

	SetDeffParam_RIGHT_MORDA();
	
	if (CII_HeroName_style==false){
		txX = 15;
		txY = 80;
	};

	DS_Visible("TextDialog",false);
	DS_Visible("SomeText",  false);
};
bool	lvCDeffFilmMenu::DS_Load(){
	xmlQuote	DS_XML("MissDialog");
	if (DS_XML.ReadFromFile("Dialogs\\MissionDialogText.DialogsSystem.xml")) {
		ErrorPager	Err;
		DS_Text.Load(DS_XML,&DS_Text,&Err);
		return true;
	};
	return false;
};
SimpleDialog*		lvCDeffFilmMenu::DS_GetUNIT(char* Name){
	SimpleDialog* retSD=NULL;
	if (Name!=NULL){
		retSD = DS_Text.Find(Name);
	};
	return retSD;
};
bool	lvCDeffFilmMenu::DS_Visible(char* Name,bool state){
	SimpleDialog* pSD=DS_GetUNIT(Name);
	if (pSD!=NULL) {
		pSD->Visible=state;
	};
	return (pSD!=NULL);
};
bool	lvCDeffFilmMenu::DS_Pos(char* Name,int x,int y){
	SimpleDialog* pSD=DS_GetUNIT(Name);
	if (pSD!=NULL) {
		pSD->Setx(x);
		pSD->Sety(y);
	};
	return (pSD!=NULL);
};
bool	lvCDeffFilmMenu::DS_Size(char* Name,int h,int w){
	SimpleDialog* pSD=DS_GetUNIT(Name);
	if (pSD!=NULL) {
		pSD->SetHeight(h);
		pSD->SetWidth(w);
	};
	return (pSD!=NULL);
};
bool	lvCDeffFilmMenu::DS_SetMessage(char* Message){
	bool retST=false;
	if (Message!=NULL) {
		TextButton* pTB = dynamic_cast<TextButton*>( DS_GetUNIT("SomeText") );
		if (pTB!=NULL) {
			pTB->SetMessage( Message );
			retST=true;
		};
	};
	return retST;
};
bool	lvCDeffFilmMenu::DS_SetMessage(_str& Message){
	return DS_SetMessage(Message.pchar());
};
bool	lvCDeffFilmMenu::DS_SetMaxMessWidth(int MaxWidth){
	bool retST=false;
	TextButton* pTB = dynamic_cast<TextButton*>( DS_GetUNIT("SomeText") );
	if (pTB!=NULL) {
		pTB->MaxWidth=MaxWidth;
		retST=true;
	};
	return retST;
};
bool	lvCDeffFilmMenu::DS_SetLHName(char* name){
	bool retST=false;
	if (name!=NULL) {
		TextButton* pTB = dynamic_cast<TextButton*>( DS_GetUNIT("HLName") );
		if (pTB!=NULL) {
			pTB->SetMessage( name );
			retST=true;
		};
	};
	return retST;
};
bool	lvCDeffFilmMenu::DS_SetRHName(char* name){
	bool retST=false;
	if (name!=NULL) {
		TextButton* pTB = dynamic_cast<TextButton*>( DS_GetUNIT("HRName") );
		if (pTB!=NULL) {
			pTB->SetMessage( name );
			retST=true;
		};
	};
	return retST;
};
bool	lvCDeffFilmMenu::DS_PosCenter(char* Name,int xc,int yc){
	SimpleDialog* pSD=DS_GetUNIT(Name);
	if (pSD!=NULL) {
		int Hd2 = pSD->GetHeight()/2;
		int Wd2 = pSD->GetWidth()/2;
		pSD->Setx(xc-Wd2);
		pSD->Sety(yc-Hd2);
	};
	return (pSD!=NULL);
};
void	lvCDeffFilmMenu::SetAlexanderSet(){
	L_M_dx	=25;
	L_M_dy	=27;
	R_M_dx	=603;
	R_M_dy	=27;

	C_B_dy	=0;
	C_B_dep =-765;

	L_F_dx	=16;
	L_F_dy	=19;
	R_F_dx	=594;
	R_F_dy	=19;

	DoNotShowLeftMorda	= false;
	DoNotShowRightMorda	= false;

	TXT_add_x_size	= 0;
	TXT_add_y_size	= 0;	

	CII_HeroName_style = false;

	HNL_x	= 0;
	HNL_y	= 0;
	HNR_x	= 0;
	HNR_y	= 0;
	
	txX = 15;
	txY = 80;
};
void	lvCDeffFilmMenu::SetCossacsIISet(){
	L_M_dx	=21;
	L_M_dy	=37;
	R_M_dx	=570;
	R_M_dy	=37;

	C_B_dy	=-1;
	C_B_dep =-750;

	L_F_dx	=24;
	L_F_dy	=12;
	R_F_dx	=574;
	R_F_dy	=12;

	DoNotShowLeftMorda	= false;
	DoNotShowRightMorda	= false;

	TXT_add_x_size	= 0;
	TXT_add_y_size	= 0;

	CII_HeroName_style = true;

	HNL_x	= 103;
	HNL_y	= 20;
	HNR_x	= 655;
	HNR_y	= 20;
};
void	lvCDeffFilmMenu::SetAdvDeff::EvaluateFunction(){
	lvCDeffFilmMenu* pPR = get_parent<lvCDeffFilmMenu>();
	if (pPR!=NULL) {
		switch(GameName) {
		case 0:	// Alexander
			pPR->SetAlexanderSet();
			break;
		case 1:	// CossacsII
			pPR->SetCossacsIISet();
			break;
		};
	};
};
// lvCDeffAnimeFilmMenu //////////////////////////////////////////////////
void	lvCDeffAnimeFilmMenu::Draw_LEFT_MORDA(){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		LEFT_ANIME_MORDA.Get()->SetPosXY(LEFT_BOX.x,LEFT_BOX.y);
		GPS.FlushBatches();
		LEFT_ANIME_MORDA.Get()->DRAW();
	};
};
void	lvCDeffAnimeFilmMenu::Draw_RIGHT_MORDA(){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		RIGHT_ANIME_MORDA.Get()->SetPosXY(RIGHT_BOX.x,RIGHT_BOX.y);
		GPS.FlushBatches();
		RIGHT_ANIME_MORDA.Get()->DRAW();
	};
};
void	lvCDeffAnimeFilmMenu::StartShow_LEFT_MORDA(){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		LEFT_ANIME_MORDA.Get()->StartShow();
	};
};
void	lvCDeffAnimeFilmMenu::StartShow_RIGHT_MORDA(){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		RIGHT_ANIME_MORDA.Get()->StartShow();
	};
};
bool	lvCDeffAnimeFilmMenu::TestSart_LEFT_MORDA(){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		return LEFT_ANIME_MORDA.Get()->TestSart();
	};
	return false;
};
bool	lvCDeffAnimeFilmMenu::TestSart_RIGHT_MORDA(){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		return RIGHT_ANIME_MORDA.Get()->TestSart();
	};
	return false;
};
void	lvCDeffAnimeFilmMenu::StopShow_LEFT_MORDA(){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		LEFT_ANIME_MORDA.Get()->StopShow();
	};
};
void	lvCDeffAnimeFilmMenu::StopShow_RIGHT_MORDA(){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		RIGHT_ANIME_MORDA.Get()->StopShow();
	};
};
bool	lvCDeffAnimeFilmMenu::TestStop_LEFT_MORDA(){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		return LEFT_ANIME_MORDA.Get()->TestStop();
	};
	return false;
};
bool	lvCDeffAnimeFilmMenu::TestStop_RIGHT_MORDA(){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		return RIGHT_ANIME_MORDA.Get()->TestStop();
	};
	return false;
};
void	lvCDeffAnimeFilmMenu::SetVisible_LEFT_MORDA(bool _vs){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		LEFT_ANIME_MORDA.Get()->SetVisible(_vs);
	};
};
void	lvCDeffAnimeFilmMenu::SetVisible_RIGHT_MORDA(bool _vs){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		RIGHT_ANIME_MORDA.Get()->SetVisible(_vs);
	};
};
bool	lvCDeffAnimeFilmMenu::isVissible_LEFT_MORDA(){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		return LEFT_ANIME_MORDA.Get()->isVissible();
	};
	return false;
};
bool	lvCDeffAnimeFilmMenu::isVissible_RIGHT_MORDA(){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		return RIGHT_ANIME_MORDA.Get()->isVissible();
	};
	return false;
};
bool	lvCDeffAnimeFilmMenu::isInVisible_LEFT_MORDA(){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		return LEFT_ANIME_MORDA.Get()->isInVisible();
	};
	return false;
};
bool	lvCDeffAnimeFilmMenu::isInVisible_RIGHT_MORDA(){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		return RIGHT_ANIME_MORDA.Get()->isInVisible();
	};
	return false;
};
void	lvCDeffAnimeFilmMenu::SetLeftMorda(int _id){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		LEFT_ANIME_MORDA.Get()->SetSprite(_id);
	};
};
void	lvCDeffAnimeFilmMenu::SetRightMorda(int _id){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		RIGHT_ANIME_MORDA.Get()->SetSprite(_id);
	};
};
void	lvCDeffAnimeFilmMenu::SetDeffParam_LEFT_MORDA(){
	if (LEFT_ANIME_MORDA.Get()!=NULL){
		LEFT_ANIME_MORDA.Get()->SetAppearTime(1000);
		LEFT_ANIME_MORDA.Get()->SetDisapperTime(1000);
		LEFT_ANIME_MORDA.Get()->dx = 24;
		LEFT_ANIME_MORDA.Get()->dy = 23;
		LEFT_ANIME_MORDA.Get()->SetVisible(Visible);
	};
};
void	lvCDeffAnimeFilmMenu::SetDeffParam_RIGHT_MORDA(){
	if (RIGHT_ANIME_MORDA.Get()!=NULL){
		RIGHT_ANIME_MORDA.Get()->SetAppearTime(1000);
		RIGHT_ANIME_MORDA.Get()->SetDisapperTime(1000);
		RIGHT_ANIME_MORDA.Get()->dx = 24;
		RIGHT_ANIME_MORDA.Get()->dy = 23;
		RIGHT_ANIME_MORDA.Get()->SetVisible(Visible);
	};
};

// lvCTeraforming ////////////////////////////////////////////////////////
extern	_str		g_vvElementView;
char*			lvCTeraforming::GetName(){
	g_vvElementView  = "";
	g_vvElementView += "(TERA)";
	g_vvElementView += Name.pchar();	
	return	g_vvElementView.pchar();
};
const	char*	lvCTeraforming::GetThisElementView(const char* LocalName){
	return GetName();
};
void			lvCTeraforming::Add_X(float dx,int MaxStep){
	if (MaxStep==0||dx<=(float)MaxStep){
		PosXYZ.x += dx;
		return;
	}else{
		// dx>MaxStep
		float Ndx = (int)dx;
		float Ddx = dx-Ndx;
		PosXYZ.x += (int)Ndx%MaxStep + Ddx;
	};
};
void			lvCTeraforming::Add_Y(float dy,int MaxStep){
	if (MaxStep==0||dy<=(float)MaxStep){
		PosXYZ.y += dy;
		return;
	}else{
		// dx>MaxStep
		float Ndy = (int)dy;
		float Ddy = dy-Ndy;
		PosXYZ.y += (int)Ndy%MaxStep + Ddy;
	};
};
void			lvCTeraforming::Add_Z(float dz,int MaxStep){
	PosXYZ.z += dz;
};
void			lvCTeraforming::Add_r(float dr,int MaxStep){
	if (MaxStep==0||dr<=(float)MaxStep){
		r += dr;
		return;
	}else{
		// dx>MaxStep
		float Ndr = (int)dr;
		float Ddr = dr-Ndr;
		r += (int)Ndr%MaxStep + Ddr;
	};
	ValidateRH();
};
void			lvCTeraforming::Add_h(float dh,int MaxStep){
	if (MaxStep==0||dh<=(float)MaxStep){
		h += dh;
		return;
	}else{
		// dx>MaxStep
		float Ndh = (int)dh;
		float Ddh = dh-Ndh;
		h += (int)Ndh%MaxStep + Ddh;
	};
	ValidateRH();
};
void			lvCTeraforming::Add_R(float dR,int MaxStep){
	if (MaxStep==0||dR<=(float)MaxStep){
		R += dR;
		return;
	}else{
		// dx>MaxStep
		float NdR = (int)dR;
		float DdR = dR-NdR;
		R += (int)NdR%MaxStep + DdR;
	};
	ValidateRH();
};
void			lvCTeraforming::Add_H(float dH,int MaxStep){
	if (MaxStep==0||dH<=(float)MaxStep){
		H += dH;
		return;
	}else{
		// dx>MaxStep
		float NdH = (int)dH;
		float DdH = dH-NdH;
		H += (int)NdH%MaxStep + DdH;
	};
	ValidateRH();
};
void			lvCTeraforming::ValidateRH(){
	if (r<10.f) r=10.f;
	if (R<15.f) R=15.f;
	while (r>=R) {
		r -= 1.f;
	};
	while (h<H+10.f) {
		h += 1.f;
	};
};
void			lvCTeraforming::Draw(){
	ValidateRH();
	if (visible==false||r==0||R==0)	return;
	DWORD	color		= 0xFF0000ff;
	DWORD	colorCross	= 0x66ff0000;
	DWORD	colorTERRA	= 0xFF00ff00;
	Vector3D	pRH_Pos[6];
	Vector3D	prh_Pos[6];
	Shapes_MAP(R)->SetAngel_XY(6,(2*3.1415/6000)*(float)(GetTickCount()%6000));
	for (int i=0; i<6; i++){
		pRH_Pos[i] = SkewPt(	Shapes_MAP(R)->regular_polygon_6D[i]->x+PosXYZ.x,
								Shapes_MAP(R)->regular_polygon_6D[i]->y+PosXYZ.y,
								H
						   );
	};
	Shapes_MAP(r)->SetAngel_XY(6,(2*3.1415/6000)*(float)(GetTickCount()%6000));
	for (int i=0; i<6; i++){
		prh_Pos[i] = SkewPt(	Shapes_MAP(r)->regular_polygon_6D[i]->x+PosXYZ.x,
								Shapes_MAP(r)->regular_polygon_6D[i]->y+PosXYZ.y,
								h
		  				   );
	};	
	Vector3D	Temp;
	Vector3D	pCROSS_TERA[6];
	for (int i=0; i<6; i++){
		DrawLineWidhTerraCross(pRH_Pos[i],pRH_Pos[(i+1)%6],color,colorCross,TOP,BOTTOM,Temp);
		DrawLineWidhTerraCross(prh_Pos[i],prh_Pos[(i+1)%6],color,colorCross,TOP,BOTTOM,Temp);
		DrawLineWidhTerraCross(pRH_Pos[i],prh_Pos[i],	   color,colorCross,TOP,BOTTOM,pCROSS_TERA[i]);
	};
	if (Cross==true) {
		for (int i=0; i<6; i++){
			if (pCROSS_TERA[i].x!=0.f&&pCROSS_TERA[i].y!=0.f&&pCROSS_TERA[(i+1)%6].x!=0.f&&pCROSS_TERA[(i+1)%6].y!=0.f)
				GPS.DrawLine(pCROSS_TERA[i],pCROSS_TERA[(i+1)%6],colorTERRA);
		};
	};
};
void			lvCTeraforming::Apply(){
	void SetHiInRound(int x,int y,int r1,int H1,int r2,int H2);
	SetHiInRound((int)PosXYZ.x,(int)PosXYZ.y,(int)r,(int)h,(int)R,(int)H);
};
bool			lvCTeraforming::IsInShape(int   x,int   y){
	return IsInShape((float)x,(float)y);
};
bool			lvCTeraforming::IsInShape(float x,float y){
	if (Norma((int)(PosXYZ.x-x),(int)(PosXYZ.y-y))<=(int)R) return true;
	return false;
};
void			lvCTeraforming::SetPoss(int   x,int   y,int   z){
	SetPoss((float)x,(float)y,(float)z);
};
void			lvCTeraforming::SetPoss(float x,float y,float z){
	PosXYZ.x = x;
	PosXYZ.y = y;
//	PosXYZ.z = z;
};
void			lvCTeraforming::Set_r(float Val){
	r += Val;
};
void			lvCTeraforming::Set_R(float Val){
	R += Val;
};
void			lvCTeraforming::Set_h(float Val){
	h += Val;
};
void			lvCTeraforming::Set_H(float Val){
	H += Val;
};

// ggCPlot ///////////////////////////////////////////////////////////////
bool move_plot_LMClick(SimpleDialog* SD){
	ggCDialog* pPAR = getGGPAR<ggCDialog>(SD);
	if (pPAR){
		pPAR->startMove();
	};
	return (pPAR!=NULL);
};
bool move_plot_MOwer(SimpleDialog* SD){
	if (SD){
		ggCDialog* pPAR = getGGPAR<ggCDialog>(SD);
		if ( Lpressed ){
			if (pPAR!=NULL) {
				pPAR->setOffsetXY(mouseX,mouseY);
			};
		}else{
			pPAR->stopMove();
		};
		return (pPAR!=NULL);
	};
	return false;
};
ggCDialog::ggCDialog(){
	pDIALOG		= NULL;
	pPANEL		= NULL;
	pSHEET		= NULL;
	pMOVE		= NULL;
};
ggCDialog::~ggCDialog(){};
bool			ggCDialog::load_DIALOG(const char* FileName){
	bool state = delete_DIALOG();
	if ( state && FName.pchar()!=NULL) {
		xmlQuote	xmlDATA( "DIALOG" );
		if (xmlDATA.ReadFromFile(FName.pchar())) {
			ErrorPager Err;
			pDIALOG = new DialogsSystem;
			pDIALOG->reset_class(pDIALOG);
			state = pDIALOG->Load( xmlDATA, pDIALOG, &Err );
		};	
		if (!state) delete_DIALOG();
	};
	return state;
};
bool			ggCDialog::delete_DIALOG(){
	if ( pDIALOG!=NULL ){
		delete pDIALOG;
		pDIALOG=NULL;
	};
	if ( pDIALOG==NULL ){
		setPANEL();
		setSHEET();
		setMOVE	();
	};
	return true;
};
// global on PLOT
void			ggCDialog::INIT(const bool LOAD/*=true*/){
	if ( LOAD )	load_DIALOG(FName.pchar());
	if ( pDIALOG ){
		initPANEL(NULL);
		initSHEET(NULL);
		initMOVE (NULL);
	};
};
bool			ggCDialog::DRAW(){return true;};
bool			ggCDialog::setXY_DIALOG(const int  x,const int  y){return true;};
bool			ggCDialog::getXY_DIALOG(	  int& x,      int& y){return true;};
bool			ggCDialog::setHW_DIALOG(const int  w,const int  h){return true;};
bool			ggCDialog::getHW_DIALOG(	  int& w,      int& h){return true;};
// for move plot on screen 
void			ggCDialog::startMove(){
	getXY_DIALOG(dlX,dlY);
	msX = mouseX;
	msY = mouseY;
};
void			ggCDialog::setOffsetXY(const int nmX,const int nmY){
	int dx = nmX-msX;
	int dy = nmY-msY;
	setXY_DIALOG(dlX+dx,dlY+dy);
};
void			ggCDialog::stopMove(){
	msY=msX=0xFFFF;
	dlY=dlX=0xFFFF;
};
// protected:
void			ggCDialog::setPANEL (DialogsDesk*	ptr/*=NULL*/){
	if (pDIALOG==NULL)	pPANEL = NULL;
	else if (ptr==NULL){
		
	}else{
		pPANEL = ptr;
	};
};
void			ggCDialog::initPANEL(DialogsDesk*	ptr/*=NULL*/){};
void			ggCDialog::setSHEET (Canvas*		ptr/*=NULL*/){
	if (pDIALOG==NULL)	pSHEET = NULL;
	else if (ptr==NULL){

	}else{
		pSHEET = ptr;
	};
};
void			ggCDialog::initSHEET(Canvas*		ptr/*=NULL*/){};
void			ggCDialog::setMOVE  (GPPicture*		ptr/*=NULL*/){
	if (pDIALOG==NULL)	pMOVE = NULL;
	else if (ptr==NULL){

	}else{
		pMOVE = ptr;
	};
};
void			ggCDialog::initMOVE (GPPicture*		ptr/*=NULL*/){};
//////////////////////////////////////////////////////////////////////////

// for skirmish //////////////////////////////////////////////////////////
CSkirmishTASK	g_SkTASK;
CEXPORT bool	g_addTask(const byte NI,const int x,const int y,const char* name){
	return	g_SkTASK.addTASK(NI,x,y,name);
}; // position on map in pixel
CEXPORT int		g_delTask(const char* name /* = NULL */){
	return	g_SkTASK.delTASK(name);
};
CSkirmishTASK::CSkirmishTASK(){
	ready=false;
//	Check();
};
CSkirmishTASK::~CSkirmishTASK(){
	Clear();
};
bool	CSkirmishTASK::LoadTemplate(char* FName/*=NULL*/){
	if (FName==NULL) return false;
	return	this->ReadFromFile(FName);
};
void	CSkirmishTASK::Check(){
	if (!ready)	ready=LoadTemplate("Dialogs\\bmFLAG.SkTASK_TL.xml");
};
void	CSkirmishTASK::Clear(){
	int N = TASK_arr.GetAmount();
	while (N--) {
		TASK_arr[N]->PreDelete();
		TASK_arr.DelElement(N);
	};
};
bool	CSkirmishTASK::DrawOnMiniMap(){
	if ( !ready ) return true;
	int N=TASK_arr.GetAmount();
	while (N--) {
		if (TASK_arr[N]!=NULL){
			TASK_arr[N]->UpdatePosOnMiniMap();
			TASK_arr[N]->Draw_MMP();
		};
	};
	return true;
};
bool	CSkirmishTASK::addTASK(const byte NI,const int x,const int y,const char* name){
	Check();
	if (NI<0||NI>7) return false;
	//////////////////////////////////////////////////////////////////////////
	bbObjInList* pTL=new bbObjInList();
	if (pTL!=NULL&&name!=NULL&&name[0]!=0)		pTL->Name=name;
	else{
		if (pTL!=NULL)	{ delete pTL; pTL=NULL; };
		return false;
	};
		
	int xxx,yyy;
	int mxxx,myyy;
	int h,w;

	xxx=x<<4;
	yyy=y<<4;

	int CreateNewTerrMons2(byte NI,int x,int y,word Type);
	int ID=CreateNewTerrMons2(NI,xxx,yyy,PointOnBigMap);
	OneObject* pOOB = Group[ID];
	if(ID!=-1&& pOOB!=NULL && (!pOOB->Sdoxlo||pOOB->Hidden)){
		OneUS* pUS = new OneUS();
		pUS->ID = pOOB->Index;
		pUS->SN = pOOB->Serial;
		pTL->UN_BMP_ID.Add(*pUS);
	};

	// Set point on miniMAP
	mxxx = GetXOnMiniMap(xxx>>4,yyy>>4);
	myyy = GetYOnMiniMap(xxx>>4,yyy>>4);
	lvCAnimateGP* pMMP = new lvCAnimateGP();
	*pMMP = PointOnMiniMap;
	h=0;
	w=0;
	pMMP->getPictureSize(w,h);
	pMMP->SetPosXY(mxxx-w/2,myyy-h/2);
	pMMP->SetVisible(true);
	pTL->GP_MMP_ID.Add(pMMP);
	//////////////////////////////////////////////////////////////////////////

	TASK_arr.Add(pTL);

	return true;
};	
int 	CSkirmishTASK::delTASK(const char* name){
    int delN=0;
	if (name!=NULL){
		int N = TASK_arr.GetAmount();
		while (N--) {
			if ( !strcmp( TASK_arr[N]->Name.pchar(), name ) ){
				TASK_arr[N]->PreDelete();
				TASK_arr.DelElement(N);
				delN++;
			};
		};
	}else{
		delN = TASK_arr.GetAmount();
		Clear();
	};
	return delN;
};
//////////////////////////////////////////////////////////////////////////


BattlePainter(){
	if (DriveMode()->NodeView==1/* VIEW ALL */||DriveMode()->NodeView==2/* FOR GROUP */) {
		
		if (DriveMode()->NodeView==1){	
			NodesMap()->vDraw( DriveMode()->NodeMode() ); 
		}
		
		if (DriveMode()->NodeView==2)	BattleShema()->vDrawMyNodes( DriveMode()->GetSquardID(), DriveMode()->NodeMode() );
		if (DriveMode()->NodeAct==2/* MOVE */ && ActivObject!= NULL) {
			ActivObject->vSetXY(LastMx,LastMy);
		};
	};
	if (DriveMode()->EdgeView==1/* VIEW */) {
		BattleShema()->vDraw( DriveMode()->EdgeMode() , DriveMode()->GetSquardID() );

		switch(DriveMode()->EdgeAct) {
		case 0:			// "ADD"
			if (pActivEdge!=NULL) {
				pActivEdge->vDraw();
			};
			break;
		case 1:			// "DELETE"
			break;
		case 2:			// "EDIT"
			break;
		default:
			break;
		};
	};
};

void	SET_BE_DrawOnMapCallback(){
	/*static*/	bool	mastSET = true;
	if (mastSET) {
		mastSET=false;
		SetDrawOnMapCallback(BattlePainter);
	}
};
//////////////////////////////////////////////////////////////////////////















