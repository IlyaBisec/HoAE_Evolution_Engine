
#ifndef __HELPER_VAR__
#define __HELPER_VAR__

//////////////////////////////////////////////////////////////////////////
// CHECK FUNCTIONS LIST
//////////////////////////////////////////////////////////////////////////
/*
Check if object target is run away from object attacer
return true if dot product of vectors pTR->direction and
pTR-pOB is less then 0
*/
bool	be_gCheckDirections(OneObject* pOB,OneObject* pTR);

/*
Return true if pOB can attack pTR (anly attack ability)
*/
bool	be_gCheckAbsAttackAbility(OneObject* pOB,OneObject* pTR);

/*
Return true if can attack imediatly
*/
bool	be_gCheckImedAttackAbility(OneObject* pOB,OneObject* pTR);

/*
Return true if pOB can attack pTR
*/
bool	be_gCheckCurAttackAbility(OneObject* pOB,OneObject* pTR);

/*
Return true if pOB can rich pTR (by way of map)
*/
bool	be_gCheckReachable(OneObject* pOB,OneObject* pTR);

//////////////////////////////////////////////////////////////////////////
// GET FUNCTIONS LIST
//////////////////////////////////////////////////////////////////////////
/*
return distance in pixels beetven two objects
*/
int		be_gGetAbsDistance(OneObject* pOB,OneObject* pTR);

/*
return distance bettwen two object use top map
*/
int		be_gGetTopDistance(OneObject* pOB,OneObject* pTR);

/*
check posibility for move between nodes in array
if some way is locked it add two node in array width SegmFR = 10 (delete it if way is free)
and mark one aditional SegmFR += 100 - it is last destenable node
*/
bool	be_gControlNodeArray(be_CARRAY* pARR,int LT,int NI,bool circle=false);

bool	be_gMarkBadNode(be_CARRAY* pARR,int LT,int NI,bool circle=false);	

//////////////////////////////////////////////////////////////////////////
// FMS and SMART_OBJECT global ACTIONS
//////////////////////////////////////////////////////////////////////////
// FSM
be_CFSM*			be_gFindeFSM	( const char* FSM_Name );
bool				be_gLoadFSM		( const char* FSM_Name );
// SO
bool				be_gCheckObjectFSM	( const char* ObjectName, const char* FSM_Name );
bool				be_gFreeSO_SOName	( const char*		SO_Name );
bool				be_gFreeSO_pSO		( be_CSMARTOBJECT*	pSO		);
bool				be_gFreeSO_pVAR		( be_CVariableBase*	pVAR	);
be_CSMARTOBJECT*	be_gFindFreeSO	( const char* FSM_Name );
be_CSMARTOBJECT*	be_gCreateNewSO	( const char* FSM_Name );

be_CSMARTOBJECT*	be_gFindSO			( const char* FSM_Name );
be_CSMARTOBJECT*	be_gFindSObyOBName	( const char* OB_Name  );
be_CSMARTOBJECT*	be_gFindSObyName	( const char* FSM_Name );
be_CSMARTOBJECT*	be_gGET_SO			( const char* FSM_Name );
//////////////////////////////////////////////////////////////////////////
// TEXT CONVERTOR FUNCTIONS
//////////////////////////////////////////////////////////////////////////
void	be_gGetTextInCH(_str& _dstText,char* _srcText,char _openCH,char _closeCH);
void	be_gBrushTetx(_str& _text);
void	be_gConvertText(_str& STRING,_str& _textID,_str& _speaker,_str& _spirit,_str& _ogFile,_str& _text);
void	be_gConvertTextID(_str& _textID,_str& _speaker,_str& _spirit,_str& _ogFile,_str& _text);
bool	be_gPrepareString(_str& FullString,char* TextID,be_CParamList& paramList);
bool	be_gPrepareString(be_CSTR* FullString,be_CSTR* TextID,be_CARRAY* paramARR);

//////////////////////////////////////////////////////////////////////////
// SYATEM
//////////////////////////////////////////////////////////////////////////
bool	be_gFilePresent(const char* fileName);

//////////////////////////////////////////////////////////////////////////
// NATION
//////////////////////////////////////////////////////////////////////////
int		be_gGetNKilled(int _nation,int _unitType);

//////////////////////////////////////////////////////////////////////////
// SIGNAL (ALERT)
//////////////////////////////////////////////////////////////////////////
void	be_gAddAlertOnMap(int _x,int _y,int _delay,int _nat);

//////////////////////////////////////////////////////////////////////////
// TOPOGRAFY
//////////////////////////////////////////////////////////////////////////
int		be_gGetAreaX(int AreaID, int layerID);
int		be_gGetAreaY(int AreaID, int layerID);

#endif//__HELPER_VAR__















