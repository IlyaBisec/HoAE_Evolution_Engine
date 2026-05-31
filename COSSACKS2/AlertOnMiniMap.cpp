#include "stdheader.h"

#include "TestEditor.h"
#include "GameExtension.h"
#include "ClassEditorsRegistration.h"

#include "AlertOnMiniMap.h"

//extern int ItemChoose;
CEXPORT int GetXOnMiniMap(int x,int y);
CEXPORT int GetYOnMiniMap(int x,int y);

// Global Bjects /////////////////////////////////////////////////////////
AlertEditor		gAlertEditor;
AlertStore		AlertSTORE;
AlertProc		AlertPROC;

bool	g_AddAlert(char* Name,int x,int y,AL_FUNC* pAL_FUNC/*=NULL*/,void* param/*=NULL*/,char* MyFullName/*=NULL*/){
	return AlertPROC.AddAlert(Name,x,y,pAL_FUNC,param,MyFullName);
};
bool	g_AddAlert(_str  Name,int x,int y,AL_FUNC* pAL_FUNC/*=NULL*/,void* param/*=NULL*/,char* MyFullName/*=NULL*/){
	return g_AddAlert(Name.pchar(),x,y,pAL_FUNC,param,MyFullName);
};
bool	g_SetNewAlertCoord(char* FullName,int x,int y){
	return AlertPROC.SetNewAlertCoord(FullName,x,y);
};
void	g_UpdateAlertXY(){
	AlertPROC.UpdateXY();
};

void	g_DeleteAllAlert(){
	AlertPROC.Free();
};
// BaseShape /////////////////////////////////////////////////////////////
			Param_BS::Param_BS(){
	// ...
};
Param_BS&	Param_BS::operator=(Param_BS& other){
	return *this;
};
			BaseShape::BaseShape(){
	m_x			= 0.f;
	m_y			= 0.f;
	m_X			= 0.f;
	m_Y			= 0.f;
	m_scale		= 1.f;
	m_angle		= 0.f;
	m_colour	= 0xFFffffff;
	m_visible	= false;
	m_flip		= 0.f;

	NAME		= "BaseShape";
};
void		BaseShape::SetParam(Param_BS* param){
	// ...
};
void		BaseShape::GetParam(Param_BS* param){
	// ...
};
void		BaseShape::SetPos(float x,float y){
	d_X = x;
	d_Y = y;
	d_x = (float)( GetXOnMiniMap( (int)d_X, (int)d_Y ) );
	d_y = (float)( GetYOnMiniMap( (int)d_X, (int)d_Y ) );
	
	m_X = d_x;
	m_Y = d_y;
	m_x = d_X;
	m_y = d_Y;
};
void		BaseShape::SetSacale(float scale){
	if ( 0.f<=scale && scale<=1.f ) m_scale = scale;
};
void		BaseShape::SetTurne	(float angle){
	m_angle = angle;
};
void		BaseShape::SetColour(DWORD color){
	m_colour = ( m_colour & 0xFF000000 ) + ( color & 0x00FFFFFF );
};
void		BaseShape::SetAlpha	(DWORD alpha){
	m_colour = ( m_colour & 0x00FFFFFF ) + ( alpha & 0x000000FF ) * 0x1000000;
};
void		BaseShape::SetAColour	(DWORD color){
	m_colour = color;
};
void		BaseShape::SetVisible(bool  state){
	m_visible = state;
};
void		BaseShape::SetFlip(float flip){
	bool setONE = (flip>0.f);
	m_flip = flip - (float)((int)flip);
	if (setONE&&flip==0.f)	m_flip = 1.f;
};
void		BaseShape::AddPos(float x,float y){
	m_X += x;
	m_Y += y;
	m_x = (float)( GetXOnMiniMap( (int)m_X, (int)m_Y ) );
	m_y = (float)( GetYOnMiniMap( (int)m_X, (int)m_Y ) );
};
void		BaseShape::AddSacale(float scale){
	m_scale += scale;
	if (m_scale<0||m_scale>1)	m_scale -= scale;
};
void		BaseShape::AddTurne(float angle){
	m_angle += angle;
};
void		BaseShape::AddColour(DWORD color){
	DWORD	RED = ((m_colour & 0x00ff0000) + (color & 0x00FF0000)) & 0x00FF0000;
	DWORD	GRN = ((m_colour & 0x0000ff00) + (color & 0x0000FF00)) & 0x0000FF00;
	DWORD	BLU = ((m_colour & 0x000000ff) + (color & 0x000000FF)) & 0x000000FF;

	m_colour = (m_colour&0xFF000000) + RED + GRN + BLU;
};
void		BaseShape::AddAlpha(DWORD alpha){
	DWORD tempAlpha = ( ( m_colour & 0xFF000000 ) + ( alpha & 0x000000FF ) * 0x1000000 ) & 0xFF000000;
	SetAlpha(tempAlpha/0x1000000);
};
void		BaseShape::AddFlip(float flip ){
	float tFlip = m_flip + flip;
	SetFlip(tFlip);
};
void		BaseShape::GetPos(float& x,float& y){
	x = m_x;
	y = m_y;
};
void		BaseShape::GetPosXY(float& x,float& y){
	x = m_X;
	y = m_Y;
};
void		BaseShape::GetSacale(float& scale){
	scale = m_scale;
};
void		BaseShape::GetTurne(float& angle){
	angle = m_angle;
};
void		BaseShape::GetColour(DWORD& color){
	color = m_colour & 0x00FFFFFF;
};
void		BaseShape::GetAlpha(DWORD& alpha){
	alpha = (m_colour & 0xFF000000) / 0x1000000;
};
void		BaseShape::GetAColour	(DWORD color){
	color = m_colour;
};
void		BaseShape::GetVisible(bool&  state){
	state = m_visible;
};
void		BaseShape::GetFlip(float& flip){
	flip = m_flip;
};
char*		BaseShape::GetName(){
	return NAME.pchar();
};
void		BaseShape::UpdateXY(){
	d_x = GetXOnMiniMap(d_Y,d_Y);
	d_y = GetYOnMiniMap(d_Y,d_Y);
	DefToCur();
};
bool		BaseShape::CopyIn(BaseShape** ppShape){
	BaseShape* pBaseShape = new BaseShape();
	*(pBaseShape) = *(this);
	*ppShape = pBaseShape;
	return true;
};
BaseShape&	BaseShape::operator=(BaseShape& other){
	NAME		= other.NAME.pchar();

	// dynamic data
	d_x			= other.m_x;
	d_y			= other.m_y;
	d_X			= other.m_X;
	d_Y			= other.m_Y;
	d_scale		= other.m_scale;
	d_angle		= other.m_angle;
	d_colour	= other.m_colour;
	d_visible	= other.m_visible;

	DefToCur();

	return *this;
};
void		BaseShape::Draw(){
	// ...
};

void		BaseShape::ReStart(){
	DefToCur();
};
void		BaseShape::DefToCur(){
	m_x			= d_x;
	m_y			= d_y;
	m_X			= d_X;
	m_Y			= d_Y;
	m_scale		= d_scale;
	m_angle		= d_angle;
	m_colour	= d_colour;
	m_visible	= d_visible;
	m_flip		= d_flip;
};
// LineBasedShape ////////////////////////////////////////////////////////
			Param_LBS::Param_LBS() : Param_BS() {
	a	= 0.f;
	b	= 0.f;
	n	= 0.f;
	};
Param_LBS&	Param_LBS::operator=(Param_LBS& other){
	a	= other.a;
	b	= other.b;
	n	= other.n;
	return *this;
};
				LineBasedShape::LineBasedShape() : BaseShape() {
	
};
void			LineBasedShape::SetParam(Param_BS* param){
	Param_LBS* pParam = dynamic_cast<Param_LBS*>(param);
	if (pParam!=NULL) {
		Params = (*pParam);
	};
};
bool			LineBasedShape::CopyIn(BaseShape** ppShape){
	LineBasedShape* pLBS = new LineBasedShape();
	*(pLBS) = *(this);
	BaseShape* pBS = dynamic_cast<BaseShape*>(pLBS);
	*ppShape = pBS;
	return true;
};
LineBasedShape&	LineBasedShape::operator=(LineBasedShape& other){
	BaseShape* pBS = dynamic_cast<BaseShape*>(&other);
	BaseShape::operator =( *pBS );
	Params = other.Params;

	return *this;
};
void			LineBasedShape::GetParam(Param_BS* param){
	Param_LBS* pParam = dynamic_cast<Param_LBS*>(param);
	if (pParam!=NULL) {
		*(pParam) = Params;
	};
};
void			LineBasedShape::Draw(){
	if (m_visible==false)	return;

	int NNN = Params.n;
	// shape type
	float dFi = 0.f;	// step in angle
	if (Params.n==0){
		dFi = 2.f/Params.a;
		NNN = (2.f*3.1415*Params.a)/2.f;
	}else{
		dFi = (2.f*3.1415f)/Params.n;
	};

	m_x = GetXOnMiniMap(d_X,d_Y);
	m_y = GetYOnMiniMap(d_X,d_Y);

	// draw
	float angleR = 0.f;
	float x1,y1;
	float x0= ( Params.a*m_scale ) * cos(angleR+m_angle) + m_x;
	float y0= ( Params.b*m_scale ) * sin(angleR+m_angle) + m_y;
	for (int i=1; i<=NNN; i++) {
		angleR += dFi;
		if (angleR>=2.f*3.1415f) angleR=2.f*3.1415f;
		x1 = ( Params.a*m_scale ) * cos(angleR+m_angle) + m_x;
		y1 = ( Params.b*m_scale ) * sin(angleR+m_angle) + m_y;

		if (0.f<=m_flip&&m_flip<0.5f) 
			GPS.DrawLine(x0,y0,x1,y1,m_colour);

		x0 = x1;
		y0 = y1;
	};
};

// AlertCOM //////////////////////////////////////////////////////////////
			AlertCOM::AlertCOM(){
	COM_Type = 0;
	speed = 0.f;
	del	  = false;
	Shape_Name = "NoName";
	speedR = 0.f;
	speedG = 0.f;
	speedB = 0.f;
};
void		AlertCOM::ApplyParam(int dT,BaseShape* pShape){
	if (Time_Total>=Time_Stop)	{ del=true; return; };

	if (pShape==NULL)	return;

	if (speed==0.f){
		int DT = Time_Stop - Time_Start;
		switch(COM_Type) {
		case 0:	// Scale
			if (DT!=0)	speed = fParam/(float)DT;
			else		pShape->SetSacale(fParam);
			break;
		case 1:	// Turn
			if (DT!=0)	speed = ((fParam/360.f)*(2.f*3.1415f))/(float)DT;
			else		pShape->SetTurne(fParam);
			break;
		case 2:	// Flip
			if (DT!=0)	speed = (float)iParam/(float)DT;
			else		pShape->SetFlip(iParam);
			break;
		case 3:	// Appear
			if (DT!=0)	speed = (float)iParam/(float)DT;
			else		pShape->SetAlpha(iParam);
			break;
		case 4:	// Colour
			if (DT!=0){
				speedR = (float)((iParam&0x00FF0000)/0x10000)/(float)DT;
				speedG = (float)((iParam&0x0000FF00)/0x100)/(float)DT;
				speedB = (float)((iParam&0x000000FF)/0x1)/(float)DT;
			}else{
				pShape->SetColour(iParam);
			};
			break;
		};
	};
	
	if (Time_Total>=Time_Start){
		switch(COM_Type) {
		case 0:	// Scale
			pShape->AddSacale(speed*dT);
			break;
		case 1:	// Turn
			pShape->AddTurne(speed*dT);
			break;
		case 2:	// Flip
			pShape->AddFlip(speed*dT);
			break;
		case 3:	// Appear
			pShape->AddAlpha((DWORD)(speed*dT));
			break;
		case 4:	// Colour
			pShape->AddColour(	((DWORD)(speedR*dT)&0xFF)*0x10000 +
								((DWORD)(speedG*dT)&0xFF)*0x100	  +
								((DWORD)(speedB*dT)&0xFF)*0x1        );
			break;
		};
	};

	Time_Total+=dT;
};
bool		AlertCOM::DeleteMe(){
	return del;
};
char*		AlertCOM::GetShapeName(){
	return Shape_Name.pchar();
};
void		AlertCOM::ReStart(){
	Time_Total = 0;
	speed = 0.f;
	del = false;
	speedR = 0.f;
	speedG = 0.f;
	speedB = 0.f;
};
void		AlertCOM::SetStatus(bool status){
	del = status;
};
DWORD		AlertCOM::GetClassMask(){
	DWORD ret = 0x00000000;
	switch(COM_Type) {
	case 0:	//	Scale
		ret = 0x00000004;
		break;
	case 1:	//	Turn
		ret = 0x00000004;
		break;
	case 2:	//	Flip
		ret = 0x00000002;
		break;
	case 3:	//	Appear
		ret = 0x00000002;
		break;
	case 4: //	Colour
		ret = 0x00000002;
	default:
		assert(1);
	};
	return ret;
};
AlertCOM&	AlertCOM::operator=(AlertCOM& other){
	Shape_Name	= other.Shape_Name.pchar();
	Time_Start	= other.Time_Start;
	Time_Stop	= other.Time_Stop;
	COM_Type	= other.COM_Type;
	bParam		= other.bParam;
	iParam		= other.iParam;
	fParam		= other.fParam;
	speed		= other.speed;
	del			= other.del;
	Time_Total	= other.Time_Total;

	return *this;
};
// AlertOBJ //////////////////////////////////////////////////////////////
			AlertOBJ::AlertOBJ(){
	NAME = "NoName";
	Time_Start = 0;
	Func=NULL;
	param=NULL;
	FullName = "NoFullName";
};
			AlertOBJ::~AlertOBJ(){
	return;
};
const char* AlertOBJ::GetThisElementView(const char* LocalName){
	return NAME.pchar();
};
void		AlertOBJ::SetPos(int x,int y){
	int N = Shapes.GetAmount();
	while (N--) {
		Shapes[N]->SetPos(x,y);
	};
};
void		AlertOBJ::SetStartTime(){
	Time_Start = GetTickCount();
	int N = COM.GetAmount();
	while (N--) {
		COM[N]->ReStart();
	};
	N = Shapes.GetAmount();
	while (N--) {
		Shapes[N]->ReStart();
	};
};
void		AlertOBJ::SetFunction(AL_FUNC* pAL_FUNC/*=NULL*/,void* _param/*=NULL*/){
	// Set activation function
	Func = pAL_FUNC;
	param= _param;
};
void		AlertOBJ::Draw(){
	if (Func!=NULL)	{
		if ( Func(param)==AL_HIDE && AllCOM_Complete() )	return;
		if ( Func(param)==AL_NULL )							return;
	};

	int N = Shapes.GetAmount();
	while (N--) {
		Shapes[N]->Draw();
	};
};
void		AlertOBJ::Update(){
	int time = (GetTickCount()-Time_Start)/*%40*/;  // OPENTEST
	Time_Start = GetTickCount();

	int N = COM.GetAmount();
	BaseShape* pShape = NULL;
	bool	COM_Complite = true;
	while (N--) {
		pShape = GetShape(COM[N]->GetShapeName());
		if (pShape!=NULL) {
			COM[N]->ApplyParam(time,pShape);
		};
		if (Func==NULL&&COM[N]->DeleteMe())	COM.DelElement(N);
		if (Func!=NULL) COM_Complite = COM_Complite && ( COM[N]->DeleteMe() );
	};

	// Restart if func!=NUUL
	if (Func!=NULL&&COM_Complite) {
		if (Func(param)!=AL_DELETE&&Func(param)==AL_VIEW) {
			SetStartTime();
		};
	};
};
BaseShape*	AlertOBJ::GetShape(char* Name){
	if (Name==NULL)	return NULL;
	int N = Shapes.GetAmount();
	BaseShape* pShape=NULL;
	while (pShape==NULL&&N--) {
		if (strcmp(Shapes[N]->GetName(),Name)==0) {
			pShape = Shapes[N];
		};
	};
	return	pShape;
};
bool		AlertOBJ::AllCOM_Complete(){
	bool COM_Complite = true;
	int N = COM.GetAmount();
	while (N--) {
		COM_Complite = COM_Complite && ( COM[N]->DeleteMe() );
	};
	return COM_Complite;
};
void		AlertOBJ::SetCOM_Staus(bool complite/*=false*/){
	int N = COM.GetAmount();
	while (N--) {
		COM[N]->SetStatus(complite);
	};
};
void		AlertOBJ::UpdateXY(){
	SetCOM_Staus(false);	
	int N = Shapes.GetAmount();
	while (N--) {
		Shapes[N]->UpdateXY();
	};
};
bool		AlertOBJ::DeleteMe(){
	bool ret = COM.GetAmount()==0;
	if (Func!=NULL) {
		ret = (Func(param)==AL_DELETE && AllCOM_Complete());
	};
    return ret;
};
bool		AlertOBJ::CopyIn(AlertOBJ** ppAlert){
	if (ppAlert==NULL)	return false;
	AlertOBJ* pAlert = new AlertOBJ();
	pAlert->NAME		= NAME.pchar();
	pAlert->Time_Start	= Time_Start;

	BaseShape* pShape = NULL;
	for (int i=0; i<Shapes.GetAmount(); i++){
		Shapes[i]->CopyIn( &pShape );
		pAlert->Shapes.Add(pShape);
		pShape = NULL;
	};

	AlertCOM* pCOM = NULL;
	for (int i=0; i<COM.GetAmount(); i++){
		pCOM = new AlertCOM();
		*(pCOM) = *(COM[i]);
		pAlert->COM.Add(pCOM);
		pCOM = NULL;
	};

	*ppAlert = pAlert;
	return true;
};
// AlertStore ////////////////////////////////////////////////////////////
AlertOBJ*	AlertArray::GetNewAlert(char* name){
	if (name==NULL)	return NULL;
	// Finde in store template
	AlertOBJ* pTemplate = NULL;
	AlertOBJ* pNewAlert = NULL;
	int N = AlertTemplates.GetAmount();
	while ( pTemplate==NULL && N-- ) {
		if ( strcmp(AlertTemplates[N]->NAME.pchar(),name)==0 ) {
			pTemplate = AlertTemplates[N];
		};
	};
	if (pTemplate!=NULL) {
		pTemplate->CopyIn( &pNewAlert );
	};
	return pNewAlert;
};
bool		AlertArray::DelAlert(char* name){
	if (name==NULL)	return false;
	bool	del = false;
	int N = AlertTemplates.GetAmount();
	while (!del&&N--) {
		if (strcmp(name,AlertTemplates[N]->NAME.pchar())==0){
			del = true;
			AlertTemplates.DelElement(N);
		};
	};
	return del;
};

			AlertStore::AlertStore(){
	AlertStorePath = "Dialogs\\Alert\\Base.AlertStore.xml";
};
AlertOBJ*	AlertStore::GetAlert(char* Name){
	return	ALERTS.GetNewAlert(Name);
};
void		AlertStore::LoadAlertStore(){
	xmlQuote	xmlData( "AlertStore" ); 
	xmlData.ReadFromFile( AlertStorePath.pchar() );
	ErrorPager Err;
	ALERTS.Load( xmlData, &ALERTS, &Err );
};
// AlertProc /////////////////////////////////////////////////////////////
bool	AlertProc::AddAlert(AlertOBJ* pAlert){
	if (pAlert!=NULL){
		pAlert->SetStartTime();
		if (pAlert->Func!=NULL && pAlert->Func(pAlert->param)!=AL_VIEW)	pAlert->SetCOM_Staus(true);
		ALERTS.Add(pAlert);
		return true;
	};
	return false;
};
bool	AlertProc::AddAlert(char* Name,int x,int y,AL_FUNC* pAL_FUNC/*=NULL*/,void* _param/*=NULL*/,char* MyFullName/*=NULL*/){
	// Get New Alert Object
	if (AlertSTORE.ALERTS.AlertTemplates.GetAmount()==0){
		AlertSTORE.LoadAlertStore();	
	};

	AlertOBJ* pAlert = AlertSTORE.GetAlert(Name);
	if (pAlert!=NULL){
		if (MyFullName!=NULL) pAlert->FullName = MyFullName;
		pAlert->SetPos(x,y);
		pAlert->SetFunction(pAL_FUNC,_param);
	};
	return AddAlert(pAlert);
};
bool	AlertProc::SetNewAlertCoord(char* _FullName,int x,int y){
	if (_FullName==NULL)	return false;
	bool	update=false;
	int N=ALERTS.GetAmount();
	while (N--) {
		if (strcmp(ALERTS[N]->FullName.pchar(),_FullName)==0){
			ALERTS[N]->SetPos(x,y);
			update=true;
		};
	};
	return update;
};
void	AlertProc::Draw(){
	int N = ALERTS.GetAmount();
	while (N--) {
		ALERTS[N]->Draw();
	};
};
void	AlertProc::Update(){
	int N = ALERTS.GetAmount();
	while (N--) {
		ALERTS[N]->Update();
		if (ALERTS[N]->DeleteMe())	ALERTS.DelElement(N);
	};
};
void	AlertProc::UpdateXY(){
	int N = ALERTS.GetAmount();
	while (N--) {
		ALERTS[N]->UpdateXY();
	};
};
void	AlertProc::Free(){
	ALERTS.Clear();
};
// AlertEditor ///////////////////////////////////////////////////////////
		AlertEditor::AlertEditor(){
	
};

void	AlertEditor::Add_Alert::EvaluateFunction(){
	g_AddAlert(AlertName.pchar(),x,y);
};
void	AlertEditor::ViewAlertStore::EvaluateFunction(){
	int lastItemChoose = ItemChoose;
	ItemChoose=-1;
//	AlertSTORE.SetViewMode();
	ProcessEditor("Alert Storage");
	ItemChoose = lastItemChoose;
//	AlertSTORE.SetSaveMode();
};
// ClassRegisteration ////////////////////////////////////////////////////
void	RegAlertClasses(){
	REG_CLASS(AlertEditor);
	REG_CLASS(Param_BS);
	REG_CLASS(BaseShape);
	REG_CLASS(Param_LBS);
	REG_CLASS(LineBasedShape);
//	REG_CLASS(GPBasedShape);
	REG_CLASS(AlertCOM);
	REG_CLASS(AlertArray);
	REG_CLASS(AlertStore);
	REG_CLASS(AlertOBJ);
};

// Alert_HANDLER /////////////////////////////////////////////////////////
void Alert_HANDLER::OnEditorStart(){
	AlertSTORE.LoadAlertStore();
};

void Alert_HANDLER::OnClassRegistration(){
	RegAlertClasses();
};
void Alert_HANDLER::OnGameStart(){
	AlertSTORE.LoadAlertStore();
};
bool Alert_HANDLER::OnMapUnLoading(){
	AlertPROC.Free();
	return false;
};
void Alert_HANDLER::ProcessingGame(){
	AlertPROC.Update();
};
void Alert_HANDLER::OnDrawOnMiniMap(int x,int y,int Lx,int Ly){
	AlertPROC.Draw();
};
void	Alert_InstallExtension(){
	InstallExtension(new Alert_HANDLER, "Start Alert Handler");
};

// ClassEditor ///////////////////////////////////////////////////////////
void	EditAlertClass(DWORD _rce_, DWORD _DILOG_EDITOR_){
	AddStdEditor("Alert Editor",dynamic_cast<BaseClass*>(&gAlertEditor),"",_DILOG_EDITOR_);
	AddStdEditor("Alert Storage",dynamic_cast<BaseClass*>(&AlertSTORE),"",_rce_);
};

//////////////////////////////////////////////////////////////////////////

















