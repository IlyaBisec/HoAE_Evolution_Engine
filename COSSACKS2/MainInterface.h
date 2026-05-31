
//////////////////////////////////////////////////////////////////////////
// 
// Main interface for control BE objects, action etc.
//
//////////////////////////////////////////////////////////////////////////

#include "InterfaceBase.h"

#ifndef __MAIN_BE_INTERFACE__
#define	__MAIN_BE_INTERFACE__

//////////////////////////////////////////////////////////////////////////
// ID for button controls BEMI(Battle editor main interface) L(List ID) B(Button ID)
//////////////////////////////////////////////////////////////////////////
#define BEMI_NULL	0xFFFFFFF0
#define BEMI_OK		0xFFFFFFF1
#define BEMI_ERROR	0xFFFFFFF2
// Controls
#define	BEMI_HIDE	0x00000001
#define	BEMI_MOVE	0x00000002
// Buttons for select panel list
#define BEMI_B1		10
#define BEMI_B2		20
#define BEMI_B3		30
#define BEMI_B4		40
// Panels
#define BEMI_L1		1010
#define BEMI_L2		1020
#define BEMI_L3		1030
#define BEMI_L4		1040
// Buttons on panls
// Panel 1
#define BEMI_L1_B1	101010	// Un Select
#define BEMI_L1_B2	101020
#define BEMI_L1_B3	101030
#define BEMI_L1_B4	101040
#define BEMI_L1_B5	101050
#define BEMI_L1_B6	101060
// Panel 2
#define BEMI_L2_B1	102010	// Un Select
#define BEMI_L2_B2	102020	// Add
#define BEMI_L2_B3	102030	// Del
#define BEMI_L2_B4	102040	// Move
#define BEMI_L2_B5	102050	// Edit
// Panel 3
#define BEMI_L3_B1	103010	// Un Select	
#define BEMI_L3_B2	103020
#define BEMI_L3_B3	103030
#define BEMI_L3_B4	103040
#define BEMI_L3_B5	103050
#define BEMI_L3_B6	103060
// Panel 4
#define BEMI_L4_B1	104010	// Un Select
#define BEMI_L4_B2	104020
#define BEMI_L4_B3	104030
#define BEMI_L4_B4	104040
#define BEMI_L4_B5	104050
#define BEMI_L4_B6	104060

// Addition data structures
enum	BEMI_ViewType
{
	BE_MI_SHOW_ALL_ELEMENT		= 0,	// Show use satates
	BE_MI_HIDE_ALL_ELEMENT		= 1,	// Hide all ( donot use states )
	BE_MI_HIDE_EXCEPT_CONTROLS	= 2		// Hide all except controls
};

class BEMainInterface : public BEInterface_base
{
public:
	BEMainInterface();
	~BEMainInterface();

	// DS GLOBAL FOR CLASS
	bool			_ERROR_;
	bool			_LOADED_;
	DialogsSystem	DS_MAIN_OBJECT;		// DS object - main menu it self

	// States
	BEMI_ViewType	DS_Visibility;			// Visibility state
	DWORD			ActiveBUTTON;			// Current button according to panel
	DWORD			ActivePANEL;			// Current active panel
	DWORD			ActiveBUTTON_ONPANEL;	// Current active button (BEMI_NULL - no active)

	void			INIT();
	void			DRAW();

	void			INIT_BUTTONS();
	void			INIT_PANELS();
	void			INIT_BUTTONS_ONPANEL();

	// Actions
	friend	bool	BEMI_ChangeActivePanel( SimpleDialog* pSD );
	friend	bool	BEMI_ChangeActiveButtonOnPanel( SimpleDialog* pSD );
	friend	bool	BEMI_HideAllPanels( SimpleDialog* pSD );
	
protected:
	// DS actions
	bool	LoadDialogsSystem	( char* FileName );			// Load from file
	void	SetPosition			( int _x, int _y );			// Set DS position
	int		SetVisible			( BEMI_ViewType	type );		// Set visibility state
	template<class T> 
		T*		FindeElement( char* _name )	
		{ 
			if (_ERROR_||_name==NULL||_name[0]==0) return NULL; 
			return dynamic_cast<T*>( DS_MAIN_OBJECT.Find( _name ) );
		};
	template<class T> 
		T*		FindeElement( DWORD _id )
		{ 
			if (_ERROR_) return NULL; 
			return dynamic_cast<T*>( DS_MAIN_OBJECT.Find( _id ) );
		};
	template<class T>
		bool	SetVisibility( char* _name, bool NewState )
		{
			T* pELEM = FindeElement<T>( _name );
			if ( pELEM ) pELEM->Visible = NewState;
			return (pELEM!=NULL);
		}
	template<class T>
		bool	SetVisibility( DWORD _id, bool NewState )
		{
			T* pELEM = FindeElement<T>( _id );
			if ( pELEM ) pELEM->Visible = NewState;
			return (pELEM!=NULL);
		}

	template<class T>
		bool	SetParams( DWORD _elID, int UserParam, VCall* OnUserClick ){
			T* pCURR = FindeElement<T>( _elID );
			if (pCURR!=NULL){
				pCURR->UserParam	= UserParam;
				pCURR->OnUserClick	= OnUserClick;
			};
			return (pCURR!=NULL);
		};
	template<class T>
		bool	SetParams( char* _name, int UserParam, VCall* OnUserClick ){
			T* pCURR = FindeElement<T>( _name );
			if (pCURR!=NULL){
				pCURR->UserParam	= UserParam;
				pCURR->OnUserClick	= OnUserClick;
			};
			return (pCURR!=NULL);
		};
	template<class T>
		bool	SetScreenPosition( DWORD _id, int _x, int _y ){
			T* pCURR = FindeElement<T>( _id );
			if (pCURR!=NULL) {
				pCURR->Setxy(_x,_y);
			};
			return (pCURR!=NULL);
		};
	template<class T>
		bool	SetScreenPosition( char* _name, int _x, int _y ){
			T* pCURR = FindeElement<T>( _name );
			if (pCURR!=NULL) {
				pCURR->Setxy(_x,_y);
			};
			return (pCURR!=NULL);
		};
	// BUTTONS for activate panels
	DWORD	SetActiveButton			( DWORD	btnID );	// Set active button for panel (only button)

	// PANEL Actions
	void	HideAllPaneles			();					// Hide all panels ( for internal use )
	DWORD	SetCurViewPanel			( DWORD	pnlID );	// Set active panel	(only panel, another hide)
	DWORD	SetActiveButtonOnPanel	( DWORD	btnID );	// Set active button on panel (only button)
	// BUTTON Actions
};

extern	BEMainInterface		g_BE_INTERF;

bool	BEMI_ChangeActivePanel( SimpleDialog* pSD );	// Chenge current panel
bool	BEMI_ChangeActiveButtonOnPanel( SimpleDialog* pSD );
bool	BEMI_HideAllPanels( SimpleDialog* pSD );

#endif//__MAIN_BE_INTERFACE__



























