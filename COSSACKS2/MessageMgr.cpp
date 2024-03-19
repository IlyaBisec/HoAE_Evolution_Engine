#include "stdheader.h"	

#include "ClassDeffIDS.h"
#include "MessageMgr.h"

extern	int LastMx;
extern	int LastMy;
extern  bool Lpressed;
extern	bool Rpressed;
void	UnPress();
DLLEXPORT	void ShowPageParam(char* Name,...);
CIMPORT		void AddPulseBox(int x, int y);
void	PlaySound(char* Name);

void	RegClassBMST(){
	REG_CLASS(BaseMesMgrST);
	REG_CLASS(SimpleMesMgr);
	REG_CLASS(C2CampMesMgr);
};

BaseMesMgrST		gMessagesMap;

CEXPORT void	AddMessageMM(BaseMesMgrST* pMessage,char* ParentName){
	gMessagesMap.AddMessage(pMessage,ParentName);
};

int	 gi_ActiveMessCampCII = -1;
CEXPORT int		GetActiveMessCampCII() { //grey [18.06.2004]
	return  gi_ActiveMessCampCII;
}
CEXPORT int		SetActiveMessCampCII(int value) { //grey [18.06.2004]
	return  gi_ActiveMessCampCII = value;
}
CEXPORT bool	ReLoadMessCampCII() { //grey [18.06.2004]
	bool  ret = false;
	gMessagesMap.DeleteData();
	gMessagesMap.InitData();

	C2CampMesMgr* pCampMess = new C2CampMesMgr;
	pCampMess->reset_class( pCampMess );
	//loading from XML
	xmlQuote xml;
	xml.ReadFromFile( "Dialogs\\messages.MessCampCII.xml" ); 
	ErrorPager Error;
	ret = pCampMess->Load(xml, pCampMess ,&Error);			
	//gMessagesMap.
	gMessagesMap.Object_Name	= "Base";
	gMessagesMap.x				= pCampMess->x ;	//  10;
	gMessagesMap.y				= pCampMess->y ;	// 210;
	gMessagesMap.dX				= pCampMess->dX ;	//  10;
	gMessagesMap.dY				= pCampMess->dY ;	//  10;
	gMessagesMap.visible		= true;
	gMessagesMap.AddMessage( pCampMess );
	gMessagesMap.Object_Name = "ROOT_OBJ";
	gi_ActiveMessCampCII = -1;
	return  ret;
}
CEXPORT bool	AddMessageIntoMessCampCII( char* page , char* txt , int id , int param , int state , int x , int y , 
										   char* ParentName ) { //grey [18.06.2004]
	if(gMessagesMap.GetNMess() > 0) {
		BaseMesMgrST* pBaseMes = gMessagesMap.Messages[ 0 ];
		if( pBaseMes && pBaseMes->GetNMess() > 0 ) 
		{
			int  MN = -1;
			for(int i=0; i<pBaseMes->GetNMess() && MN < 0 && ParentName ;i++) 
				if( !strcmp(pBaseMes->Messages[i]->Object_Name.pchar(),ParentName) )  MN = i;
			if( MN < 0 || MN >= pBaseMes->GetNMess() )  MN = 0;
			C2CampMesMgr* pParentMes = dynamic_cast<C2CampMesMgr*>( pBaseMes->Messages[ MN ] );
			if( pParentMes ) 
			{
				C2CampMesMgr* pMess = new C2CampMesMgr;
				pMess->text.DString::Clear();
				pMess->PageName.DString::Clear();		
				pMess->text				   += txt;
				pMess->PageName			   += page;
				if(id>=0)	 pMess->ID		= id;
				if(param>=0) pMess->param	= param;
				if(state>=0) pMess->state	= state;
				if(x>=0)	 pMess->zoneX	= x;
				if(y>=0)	 pMess->zoneY	= y;
				pMess->NewMess				= true ;
				pMess->Object_Name			= pBaseMes->Object_Name;							//"Base";
				pMess->x					= pParentMes->x ;  //new
				pMess->y					= pParentMes->y ;  //new
				pMess->dX					= pParentMes->dX ; //new
				pMess->dY					= pParentMes->dY ; //new
				pMess->FileID				= pParentMes->FileID ;
				pMess->SpriteID				= pParentMes->SpriteID ;
				pMess->SpriteID1			= pParentMes->SpriteID1 ;
				pMess->SpriteID2			= pParentMes->SpriteID2 ;
				pMess->Colour				= pParentMes->Colour ;
				pMess->Time_Pulse			= pParentMes->Time_Pulse ;
				return  gMessagesMap.AddMessage( pMess , pBaseMes->Object_Name.pchar() /*"Base"*/ ) ; //return  gMessagesMap.AddMessage( pMess , ParentName ) ;
			}
		}
	}
	return  false;
};

//======================================================================//
//							 BaseMesMgr									//
//======================================================================//
BaseMesMgrST::BaseMesMgrST(){
	InitData();
	Object_Name="";
};
BaseMesMgrST::BaseMesMgrST(BaseMesMgrST& MM){
	InitData();
	Deleted=MM.Deleted;
	int N = Messages.GetAmount();
	BaseMesMgrST* pCopyMess=NULL;
	while (N--) {
		pCopyMess = new BaseMesMgrST(*(MM.Messages[N]));
		AddMessage(pCopyMess);
		pCopyMess=NULL;
	};
};
BaseMesMgrST::~BaseMesMgrST(){
	DeleteData();
};
void		BaseMesMgrST::DeleteData(){
	Messages.Clear();
	Deleted=true;
	visible=false;
	active=false;
	Object_Name="Base";
};
void		BaseMesMgrST::InitData(){
	InfID=_BaseMesMgrST_;
	Messages.Clear();
	Deleted=false;
	visible=false;
	active=true;
};
void		BaseMesMgrST::AddMessage(BaseMesMgrST* pMessage){
	if (pMessage!=NULL) {
		if (GetNMess()>0) {
			pMessage->SetVisible(GetChildVisible());
		};
        Messages.Add(pMessage);
		ArrangeIcons();
	};
};
bool		BaseMesMgrST::AddMessage(BaseMesMgrST* pMessage,char* ParenName){
//	BaseMesMgrST* pLocalParent=FindeObject(ParenName);
//	if (pLocalParent==NULL)	pLocalParent=this;
//	pLocalParent->AddMessage(pMessage);
	if (ParenName!=NULL&&strcmp(Object_Name.pchar(),ParenName)==0){
		AddMessage(pMessage);
		return true;
	};
	bool	addComplte = false;
	int N = GetNMess();
	while (!addComplte&&N--) {
		addComplte = Messages[N]->AddMessage(pMessage,ParenName);
	};
	return addComplte;
};
void		BaseMesMgrST::SetDelete(){
	Deleted=true;
	int N = Messages.GetAmount();
	while (N--) {
		Messages[N]->SetDelete();
	};
};
void		BaseMesMgrST::ClearDeleted(){
	int N=Messages.GetAmount();
	while (N--) {
		if (Messages[N]->Deleted==true){
			Messages[N]->ClearDeleted();
			Messages.DelElement(N);
		};
	};
	ArrangeIcons();
};
void		BaseMesMgrST::SetObjectName(char* Name){
	if (Name!=NULL) Object_Name=Name;
};
bool		BaseMesMgrST::HANDLE(int mx,int my,
								 bool& LeftPressed,
								 bool& RightPressed,
								 int MapCoordX,int MapCoordY,
								 bool OverMiniMap){
    ClearDeleted();
	if( !active || !GetChildActive() )	return false;//grey

	int N = Messages.GetAmount();
	bool	release=false;
	bool	MLClikOnChild=false;
	int		CID = -1;
	for (int i=0; (i<N)&&(!MLClikOnChild); i++){
		MLClikOnChild = Messages[i]->Icon_OnMouseLClick(mx,my,LeftPressed);
		if (MLClikOnChild) CID=i;
	};
    if (MLClikOnChild) {
		for (i=0; i<N; i++){
			if (i!=CID) {
				Messages[i]->HideAllMessages();
			}else{
				Messages[i]->HANDLE(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap);
			};
		};
		RightPressed=false;
		return true;
    };
	for (i=0; (i<N)&&(!release); i++){
		release = Messages[i]->HANDLE(mx,my,LeftPressed,RightPressed,MapCoordX,MapCoordY,OverMiniMap);
	};
	if (release){
		LeftPressed=false;
		RightPressed=false;
		return true;
	};
	
	if (Icon_OnMouseRClick(mx,my,RightPressed)) {
		SetDelete();
		RightPressed=false;
//		UnPress();
		return true;
	}else if (Icon_OnMouseLClick(mx,my,LeftPressed)) {
		ShowDialog();
		LeftPressed=false;
//		UnPress();
		return true;
	}else if (Icon_MouseOver(mx,my)) {
		ProcessMessage();
	};
	return false;
};
void		BaseMesMgrST::DRAW(){
	ClearDeleted(); // added by grey [15.07.04]
    Icon_Draw();

	if (visible) {
		int N = Messages.GetAmount();
		while (N--) {
			Messages[N]->DRAW();
		};
	};
};
void		BaseMesMgrST::ShowAllMessages(){
	int N = Messages.GetAmount();
	while (N--) {
		Messages[N]->SetVisible(true);
		Messages[N]->ShowAllMessages();
	};
};
void		BaseMesMgrST::HideAllMessages(){
	int N = Messages.GetAmount();
	while (N--) {
		Messages[N]->SetVisible(false);
		Messages[N]->HideAllMessages();
	};
};
void		BaseMesMgrST::ChangeMessVisibleState(){
	int N = Messages.GetAmount();
	while (N--) {
		Messages[N]->HideAllMessages();
		if (Messages[N]->GetVisible()==true) {
			Messages[N]->SetVisible(false);
		}else{
			Messages[N]->SetVisible(true);
		};
	};
};
bool		BaseMesMgrST::GetActive(){
	return active;
};
void		BaseMesMgrST::SetActive(bool _st){
	active=_st;
};
bool		BaseMesMgrST::GetChildActive(){//grey
	bool	act = active;
	for(int i=0; i < GetNMess() && act ; i++)
		if( !Messages[i]->GetActive() )  act = false;
	return act;
};
bool		BaseMesMgrST::GetVisible(){
	return visible;
};
void		BaseMesMgrST::SetVisible(bool _st){
	visible=_st;
};
bool		BaseMesMgrST::GetChildVisible(){
	bool	sv = false;
	for (int i=0; (!sv)&&(i<GetNMess()); i++){
		if (Messages[i]->GetVisible()==true)	sv=true;
	};
	return sv;
};
int			BaseMesMgrST::GetNMess(){
	return Messages.GetAmount();
};
void		BaseMesMgrST::Icon_Draw(){
};
bool		BaseMesMgrST::Icon_MouseOver(int mx,int my){
	return false;
};
bool		BaseMesMgrST::Icon_OnMouseLClick(int mx,int my,bool& LeftPressed){
	if (Icon_MouseOver(mx,my)&&LeftPressed&& this->GetChildActive() ) {//grey
//		LeftPressed=false;
		return true;
	};
	return	false;
};
bool		BaseMesMgrST::Icon_OnMouseRClick(int mx,int my,bool& RightPressed){
	if (Icon_MouseOver(mx,my)) {
		if (RightPressed) {
//			RightPressed=false;
			return true;
		};
	};
	return	false;
};
void		BaseMesMgrST::ShowDialog(){
	ChangeMessVisibleState();
};
void		BaseMesMgrST::ProcessMessage(){
	// ...
};
void		BaseMesMgrST::ArrangeIcons(){
	Icon_Sort();
	// Set positions for icon, represented messages
	int h,w;
	Icon_GetSize(h,w);
	int startX = x+w+dX;
	int startY = y;
	int NMS = Messages.GetAmount();
	BaseMesMgrST* pCurMess=NULL;
	switch(AlignType) {
	case 0:		// Left
		while (NMS--) {
			pCurMess = Messages[NMS];
			if (pCurMess->GetVisible()){
				pCurMess->Icon_SetPos(startX,startY);
				pCurMess->Icon_GetSize(h,w);
				startY += h+dY;
			};
		};
		break;
	case 1:
		break;
	};
};
void		BaseMesMgrST::Icon_Sort(){
};
void		BaseMesMgrST::Icon_SetPos(int  _x,int  _y){
	x = _x;
	y = _y;
};
void		BaseMesMgrST::Icon_GetPos(int& _x,int& _y){
	_x = x;
	_y = y;
};
void		BaseMesMgrST::Icon_GetSize(int& h,int& w){
	h=0;
	w=0;
};
BaseMesMgrST*	BaseMesMgrST::FindeObject(char* ObjectName){
	BaseMesMgrST* Obj = NULL;
	int N = Messages.GetAmount();
	while (N--&&Obj==NULL) {
		if (Messages[N]->CompareName(ObjectName)==true) {
			Obj = Messages[N];
		}else{
			Obj = Messages[N]->FindeObject(ObjectName);
		};
	};
	return Obj;
};
bool			BaseMesMgrST::CompareName(char* ObjectName){
	return (strcmp(ObjectName,Object_Name.pchar())==0);
};
//////////////////////////////////////////////////////////////////////////

// SimpleMesMgr //////////////////////////////////////////////////////////
SimpleMesMgr::SimpleMesMgr() : BaseMesMgrST() {
	InfID=_SimpleMesMgr_;
	dX = 0; //2; //dX = 10;
	dY = 0; //2; //dY = 10;
	AlignType=0;
	Colour=0xFFffffff;
	FileID=0;
	SpriteID=0;
	x=0;
	y=0;
};
SimpleMesMgr::SimpleMesMgr(SimpleMesMgr& MM) : BaseMesMgrST(*(dynamic_cast<BaseMesMgrST*>(&MM))) {
	dX			= MM.dX;
	dY			= MM.dY;
	AlignType	= MM.AlignType;
	Colour		= MM.Colour;
	FileID		= MM.FileID;
	SpriteID	= MM.SpriteID;
	x			= MM.x;
	y			= MM.y;
};
SimpleMesMgr::~SimpleMesMgr(){

};
void		SimpleMesMgr::Icon_Lines(){
	//int		x0,y0,h0,w0;	// Parent pos and size
	//int		x1,y1,h1,w1;	// Child pos and size
	//int		x2,y2,h2,w2;	// Child pos and size

	//bool	first=true;

	//Icon_GetPos (x0,y0);
	//Icon_GetSize(h0,w0);

	//int N = Messages.GetAmount();// -2;//very new - very BAD!!!
	//while (N--) {
	//	if ( Messages[N]->GetVisible()==true && 
	//		 dynamic_cast<C2CampMesMgr*>(Messages[N])->TypeObj >= 2 ) { //grey //if (Messages[N]->GetVisible()==true) {
	//		( dynamic_cast<SimpleMesMgr*>(Messages[N]) )->Icon_GetPos (x2,y2);
	//		( dynamic_cast<SimpleMesMgr*>(Messages[N]) )->Icon_GetSize(h2,w2);
	//		if (first) {
	//			first=false;
	//			GPS.DrawLine(x0+w0,y0+h2/2,x0+w0+dX,y0+h2/2,Colour);
	//			x0 = x0+w0+dX/2;
	//			y0 = y0+h2/2;
	//		}else{
	//			GPS.DrawLine(x0,y0,x0,y0+h1/2+dY+h2/2,Colour);
	//			GPS.DrawLine(x0,y0+h1/2+dY+h2/2,x0+dX/2,y0+h1/2+dY+h2/2,Colour);
	//			y0=y0+h1/2+dY+h2/2;
	//		};
	//		x1=x2;y1=y2;h1=h2;w1=w2;
	//	};
	//};
};
void		SimpleMesMgr::Icon_Draw(){
	if (visible) {
		DWORD oldColour = GPS.GetCurrentDiffuse();
		GPS.SetCurrentDiffuse(Colour);
		GPS.ShowGP(x,y,FileID,SpriteID,0);
		GPS.SetCurrentDiffuse(oldColour);
		
		Icon_Lines();
	};
};
bool		SimpleMesMgr::Icon_MouseOver(int mx,int my){
	int h,w;
    Icon_GetSize(h,w);
	return ( x<mx && mx<x+w && y<my && my<y+h );
};
void		SimpleMesMgr::Icon_GetSize(int& h,int& w){
	h = GPS.GetGPHeight(FileID,SpriteID);
	w = GPS.GetGPWidth(FileID,SpriteID);
};
//////////////////////////////////////////////////////////////////////////

// C2CampMesMgr //////////////////////////////////////////////////////////
C2CampMesMgr::C2CampMesMgr() : SimpleMesMgr()
{
	Init();
	TypeObj=2;
}

C2CampMesMgr::C2CampMesMgr(C2CampMesMgr& MM) : SimpleMesMgr( *(dynamic_cast<SimpleMesMgr*>(&MM)) )
{
	Init();
	NewMess = MM.NewMess;
	// ...
}
C2CampMesMgr::~C2CampMesMgr()
{

};
void	C2CampMesMgr::Init()
{
	// message params
	ID		= -1 ;
	PageName= "NOname" ;
	text	= "NOtext" ;
	param	= -1 ;
	// additional params
	state	= -1 ;
	zoneX	= -1 ;
	zoneY	= -1 ;
	// blink time 
	Time_LastUpdate = 0;
}
bool	C2CampMesMgr::AddMessage(BaseMesMgrST* pMessage,char* ParenName){

	bool addCompl = SimpleMesMgr::AddMessage(pMessage,ParenName);

	if (addCompl) {
		switch(TypeObj) {
		case 0:
			// Base container, must pulse
			if (GetVisible()==false||GetChildVisible()==false) NewMess=true;
			else {
				// corect
				bool sv=GetChildVisible();
				if (sv){
					for (int i=0; i<GetNMess(); i++){
						if (Messages[i]->GetNMess()>0)	Messages[i]->SetVisible(true);
					};
				};
			};
			SetVisible(true);
			break;
		case 1:
			if (GetVisible()==false||GetChildVisible()==false) NewMess=true;
			break;
		case 2:
			// Message, simply add
			break;
		};
		// kill dupes 
		if(pMessage) {
			C2CampMesMgr* pNewMess = dynamic_cast<C2CampMesMgr*>( pMessage );
			bool find  = false;
			if( pNewMess && pNewMess->param >= 0 && pNewMess->state > 0 )
				for(int i=0; i < GetNMess() ; i++)
					if( Messages[i] ) {
						C2CampMesMgr* pMess = dynamic_cast<C2CampMesMgr*>( Messages[i] );
						if( pMess && pMess->ID == pNewMess->ID && pMess->param == pNewMess->param && 
							pMess->state != pNewMess->state ) { // pMess->state == 0 && 
							find = true;
							pMess->SetDelete();
						}
					}
		}
	}
	return addCompl;	
};
void	C2CampMesMgr::SetDelete(){
	if( TypeObj > 1 ) // cant delete
		Deleted=true;
	int N = Messages.GetAmount();
	while (N--) {
		Messages[N]->SetDelete();
	};
};
void	C2CampMesMgr::ClearDeleted(){
	SimpleMesMgr::ClearDeleted();
	if (TypeObj==0) {
		int N = GetNMess();
		bool empty = true;
		while (empty&&N--) {
			if (Messages[N]->GetNMess()>0)	empty=false;	
			if( !strcmp( Messages[N]->Object_Name.pchar() , this->Object_Name.pchar() ) )	empty=false;//grey
			//if( Messages[N]->TypeObj == 2 )	empty=false;//grey
		};
		if (empty==true)	SetVisible(false);
	}else if (TypeObj==1) {
		if (GetNMess()==0)	SetVisible(false);
	};
};
void	C2CampMesMgr::Icon_Draw()
{
	if( visible ) { //new  //by grey - For Blink [19.07.]
		if( NewMess ) {
			if( GetTickCount()-Time_LastUpdate >= Time_Pulse ) {
				ChangeIcon();
				Time_LastUpdate = GetTickCount();
			};
		} else {
			if(active) CurSprite=SpriteID; //CurSprite=SpriteID; //grey [17.12.2004]
		};

		DWORD oldColour = GPS.GetCurrentDiffuse();
		GPS.SetCurrentDiffuse(Colour);
		GPS.ShowGP(x,y,FileID,CurSprite,0);
		GPS.SetCurrentDiffuse(oldColour);
		
		Icon_Lines();
	};

	if( NewMess && !visible && Time_LastUpdate > 0 )  NewMess = false;//new  //by grey - For Blink [19.07.]

	//if (TypeObj==0||TypeObj==1) {
	//	if (NewMess){
	//		if ( GetTickCount()-Time_LastUpdate>=Time_Pulse ){
	//			ChangeIcon();
	//			Time_LastUpdate = GetTickCount();
	//		};
	//	}else{
	//		CurSprite=SpriteID;
	//	}
	//}else{
	//	CurSprite=SpriteID;
	//};
	//if (visible) {
	//	DWORD oldColour = GPS.GetCurrentDiffuse();
	//	GPS.SetCurrentDiffuse(Colour);
	//	GPS.ShowGP(x,y,FileID,CurSprite,0);
	//	GPS.SetCurrentDiffuse(oldColour);
	//	
	//	Icon_Lines();
	//};
};
bool	C2CampMesMgr::Icon_OnMouseRClick(int mx,int my,bool& RightPressed){
	//if( TypeObj != 0 ) // can delete
		return  BaseMesMgrST::Icon_OnMouseRClick( mx, my, RightPressed );
	//return	false; // NOT delete
};
void	C2CampMesMgr::ShowDialog()
{
	if( !visible )	return;
	SetActive( false );//new
	switch(TypeObj) {
	case 0:
		NewMess=false;
		CurSprite=SpriteID;
		break;
	case 1:
		NewMess=false;
		CurSprite=SpriteID;
		break;
	case 2:
		// ...
		NewMess=false;//new
		//CurSprite = SpriteID+1;//new //grey [17.12.2004]
		CurSprite=SpriteID1;//new //grey [17.12.2004]
		ProcessMessActions();
		break;
	};
	
	SimpleMesMgr::ShowDialog();
	SetActive( true );//new
};
void	C2CampMesMgr::ProcessMessActions()
{
	switch(ID) {
		case 0xADD:		// Ress (add res/info)
			ShowPageParam( PageName.pchar() , text.pchar() );
			break;
		case 0xDEF:		// Quest
			//if(state == 1)  PlaySound( "QUESTOK" );
			if(param >= 0)  gi_ActiveMessCampCII = param ;
			ShowPageParam( PageName.pchar() , text.pchar() );
			break;
		case 0xD79:		// defend zone quards
			ShowPageParam( PageName.pchar() , text.pchar() );
			break;
		case 0xAAA:		// simply ShowPageParam
			ShowPageParam( PageName.pchar() , text.pchar() );
			break;
	}
	if(zoneX >= 0 && zoneY >= 0)  AddPulseBox(zoneX, zoneY);
	SetDelete();
	//Deleted = true;
};
void	C2CampMesMgr::ProcessMessage()
{
};
void	C2CampMesMgr::Icon_Sort()
{
	int Num = Messages.GetAmount();
	for(int n=0; n < Num-1 ; n++) { // sorting
		int min = n;
		for(int k=n+1; k < Num ; k++)   
			if( (dynamic_cast<C2CampMesMgr*>(Messages[min]))->ID > (dynamic_cast<C2CampMesMgr*>(Messages[k]))->ID )
				min = k ;
		if( min != n )   for(int m=min; m > n ; m--) Messages.MoveElement(m, -1); 
	}
}
void	C2CampMesMgr::ChangeIcon()
{
	if (CurSprite==SpriteID){
		CurSprite = SpriteID2;	
	}else{
		if(active) CurSprite = SpriteID; //CurSprite = SpriteID; //grey [17.12.2004]
	};
};
//////////////////////////////////////////////////////////////////////////

