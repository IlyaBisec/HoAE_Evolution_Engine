#include "stdheader.h"
#include "MainInterface.h"


//////////////////////////////////////////////////////////////////////////
// BEMainInterface 
//////////////////////////////////////////////////////////////////////////

//------------------------------------
BEMainInterface		g_BE_INTERF;
//------------------------------------
BEMainInterface::BEMainInterface(){
	_ERROR_ = true;
	_LOADED_ = false;
};
//------------------------------------
BEMainInterface::~BEMainInterface(){
};
//------------------------------------
void	BEMainInterface::INIT(){
	_ERROR_ = false;
	if ( _LOADED_ )	{	
		DS_MAIN_OBJECT.reset_class(&DS_MAIN_OBJECT); 
		_LOADED_=false; 
	};
	if ( !_LOADED_ ){	
		LoadDialogsSystem("Dialogs\\BE_Elements\\MAIN_BE_DIALOG.DialogsSystem.xml"); 
		_LOADED_=true;
	};

	// Global state
	SetVisible( BE_MI_SHOW_ALL_ELEMENT );

	// Buttons for controling panels
	INIT_BUTTONS();

	// Init panels
	INIT_PANELS();

	// Init Buttons
	INIT_BUTTONS_ONPANEL();
};
//------------------------------------
void	BEMainInterface::INIT_BUTTONS(){
	if ( !_LOADED_ || _ERROR_ )	return;

	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_HIDE,(int)(NULL),BEMI_HideAllPanels)) || _ERROR_;

	// Set parameters an callback functions
	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_B1,(int)(BEMI_L1),BEMI_ChangeActivePanel)) || _ERROR_;
	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_B2,(int)(BEMI_L2),BEMI_ChangeActivePanel)) || _ERROR_;
	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_B3,(int)(BEMI_L3),BEMI_ChangeActivePanel)) || _ERROR_;
	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_B4,(int)(BEMI_L4),BEMI_ChangeActivePanel)) || _ERROR_;
	// Set initial state
	SetActiveButton( BEMI_B1 );
};
//------------------------------------
void	BEMainInterface::INIT_PANELS(){
	if ( !_LOADED_ || _ERROR_ )	return;

	// Set initial state
	HideAllPaneles();
	ActivePANEL = BEMI_NULL;
	SetCurViewPanel( BEMI_L1 );
};
//------------------------------------
void	BEMainInterface::INIT_BUTTONS_ONPANEL(){
	if ( !_LOADED_ || _ERROR_ )	return;

	// Set parameters an callback functions
	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_L1_B1,(int)(NULL),BEMI_ChangeActiveButtonOnPanel)) || _ERROR_;

	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_L2_B1,(int)(NULL),BEMI_ChangeActiveButtonOnPanel)) || _ERROR_;
	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_L2_B2,(int)(NULL),BEMI_ChangeActiveButtonOnPanel)) || _ERROR_;
	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_L2_B3,(int)(NULL),BEMI_ChangeActiveButtonOnPanel)) || _ERROR_;
	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_L2_B4,(int)(NULL),BEMI_ChangeActiveButtonOnPanel)) || _ERROR_;
	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_L2_B5,(int)(NULL),BEMI_ChangeActiveButtonOnPanel)) || _ERROR_;

	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_L3_B1,(int)(NULL),BEMI_ChangeActiveButtonOnPanel)) || _ERROR_;

	_ERROR_ = !(SetParams<SimpleDialog>(BEMI_L4_B1,(int)(NULL),BEMI_ChangeActiveButtonOnPanel)) || _ERROR_;

	// Set initial state
	SetActiveButtonOnPanel( BEMI_L1_B1 );
};
//------------------------------------
void	BEMainInterface::DRAW(){
	if ( !_LOADED_ || _ERROR_ )	return;
	
	// Draw Main Interface
	DS_MAIN_OBJECT.ProcessDialogs();
};
//------------------------------------
bool	BEMainInterface::LoadDialogsSystem( char* FileName ){
	_ERROR_ = !( DS_MAIN_OBJECT.ReadFromFile( FileName ) );
	_LOADED_ = true;
    return ( !_ERROR_ );
};
//------------------------------------
void	BEMainInterface::SetPosition( int _x, int _y ){
	if ( !_LOADED_ || _ERROR_ )	return;
	DS_MAIN_OBJECT.Setxy(_x,_y);
};
//------------------------------------
int		BEMainInterface::SetVisible( BEMI_ViewType type ){
	if ( _ERROR_ )	return (int)(BEMI_ERROR);

	if ( DS_Visibility == type ) return (int)(BEMI_OK);

	switch( type ) {
	case BE_MI_SHOW_ALL_ELEMENT:
		SetVisibility<SimpleDialog>(ActivePANEL,true);
		SetVisibility<SimpleDialog>(BEMI_B1,true);
		SetVisibility<SimpleDialog>(BEMI_B2,true);
		SetVisibility<SimpleDialog>(BEMI_B3,true);
		SetVisibility<SimpleDialog>(BEMI_B4,true);
		break;
	case BE_MI_HIDE_ALL_ELEMENT:
		
		break;
	case BE_MI_HIDE_EXCEPT_CONTROLS:
		SetVisibility<SimpleDialog>(BEMI_B1,false);
		SetVisibility<SimpleDialog>(BEMI_B2,false);
		SetVisibility<SimpleDialog>(BEMI_B3,false);
		SetVisibility<SimpleDialog>(BEMI_B4,false);
		SetVisibility<SimpleDialog>(ActivePANEL,false);
        SetActiveButtonOnPanel( BEMI_NULL );
		break;
	default:
		_ERROR_ = true;
		return (int)(BEMI_ERROR);
	}
	
	DS_Visibility = type;

	return (int)(BEMI_OK);
};

//------------------------------------
DWORD	BEMainInterface::SetActiveButton( DWORD	btnID ){
	if ( _ERROR_ )	return BEMI_ERROR;

	SimpleDialog* pSD = FindeElement<SimpleDialog>( ActiveBUTTON );
	if (pSD) pSD->Active = false;
	ActiveBUTTON = BEMI_NULL;
	
	pSD = FindeElement<SimpleDialog>( btnID );
	if (pSD){
		pSD->Active = true;
		ActiveBUTTON = btnID;
	};
	
	return BEMI_OK;
};
//------------------------------------
void	BEMainInterface::HideAllPaneles(){
	if ( !_LOADED_ || _ERROR_ )	return;
	SetVisibility<SimpleDialog>(BEMI_L1,false);
	SetVisibility<SimpleDialog>(BEMI_L2,false);
	SetVisibility<SimpleDialog>(BEMI_L3,false);
	SetVisibility<SimpleDialog>(BEMI_L4,false);
};
//------------------------------------
DWORD	BEMainInterface::SetCurViewPanel( DWORD	pnlID ){
	if ( _ERROR_ )	return BEMI_ERROR;

	if (DS_Visibility==BE_MI_HIDE_EXCEPT_CONTROLS) SetVisible(BE_MI_SHOW_ALL_ELEMENT);

	DWORD OldPanel = ActivePANEL;

	SetVisibility<SimpleDialog>(ActivePANEL,false);
	SetActiveButtonOnPanel( BEMI_NULL );	// Select "Un Select" for current pane
	ActivePANEL	= BEMI_NULL;
	
	if ( SetVisibility<SimpleDialog>(pnlID,true) ) ActivePANEL = pnlID;
	SetActiveButtonOnPanel( BEMI_NULL );	// Select "Un Select" for current pane
		
	return OldPanel;
};
//------------------------------------
DWORD	BEMainInterface::SetActiveButtonOnPanel( DWORD	btnID ){
	if ( _ERROR_ )	return BEMI_ERROR;
	
	// Un select old
	SimpleDialog* pSD = FindeElement<SimpleDialog>( ActiveBUTTON_ONPANEL );
	if (pSD) pSD->Active = false;
	ActiveBUTTON_ONPANEL = BEMI_NULL;

	// Select new
	if (btnID!=BEMI_NULL){
		pSD = FindeElement<SimpleDialog>( btnID );
		if (pSD){
			pSD->Active = true;
			ActiveBUTTON_ONPANEL = btnID;
		};
	};

	return BEMI_OK;
};
//------------------------------------

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// callback functions
//////////////////////////////////////////////////////////////////////////
bool	BEMI_ChangeActivePanel( SimpleDialog* pSD ){
	g_BE_INTERF.SetActiveButton( pSD->ID );
	g_BE_INTERF.SetCurViewPanel( (DWORD)(pSD->UserParam) );
	return true;
};
bool	BEMI_ChangeActiveButtonOnPanel( SimpleDialog* pSD ){
	g_BE_INTERF.SetActiveButtonOnPanel( pSD->ID );
	return true;
};
bool	BEMI_HideAllPanels( SimpleDialog* pSD ){
	if (g_BE_INTERF.DS_Visibility == BE_MI_HIDE_EXCEPT_CONTROLS)
		g_BE_INTERF.SetVisible(BE_MI_SHOW_ALL_ELEMENT);
	else
		g_BE_INTERF.SetVisible(BE_MI_HIDE_EXCEPT_CONTROLS);
	return true;
};
//////////////////////////////////////////////////////////////////////////






















