#include "stdheader.h"
#include "../BE_HEADERS.h"

// FSMstate //////////////////////////////////////////////////////////////
be_FSMstate::be_FSMstate(){
	m_strStateName = "STATE_NULL";
};
be_FSMstate::be_FSMstate( const char* strStateName ){
	if ( strStateName && m_pstrOutputState[0] )	m_strStateName = strStateName;
	else										m_strStateName = "STATE_NULL";
};
be_FSMstate::~be_FSMstate(){
	m_pstrInputs.Clear();
	m_pstrOutputState.Clear();
};
char*	be_FSMstate::GetName(){
	return m_strStateName.pchar();	
};
void	be_FSMstate::AddTransition( const char* strInput, const char* strOutputStateName ){
	int	_id = FindID(m_pstrInputs,strInput); 
	if (_id!=-1&&strcmp(m_pstrOutputState[_id]->pchar(),strOutputStateName)==0) return;
	_str* pstr = new _str();
	(*pstr) = strInput;
	m_pstrInputs.Add(pstr);
	pstr = new _str();
	(*pstr) = strOutputStateName;
	m_pstrOutputState.Add(pstr);
	pstr = NULL;
};
void	be_FSMstate::DeleteTransition( const char* strOutputStateName ){
	int _id = FindID(m_pstrOutputState,strOutputStateName);
	while (_id!=-1) {
		m_pstrInputs.DelElement(_id);
		m_pstrOutputState.DelElement(_id);
		_id = FindID(m_pstrOutputState,strOutputStateName);
	};
};
char*	be_FSMstate::GetOutput( const char* strInput ){
	int _id = FindID(m_pstrOutputState,strInput);
	if (_id!=-1) return	m_pstrOutputState[_id]->pchar();
	return m_strStateName.pchar();
};
int		be_FSMstate::GetAmount(){
	return m_pstrInputs.GetAmount();	
};
int		be_FSMstate::FindID( ClonesArray<_str>& strList, const char* str ){
	int N=strList.GetAmount();
	while (N--) {
		if ( strcmp(strList[N]->pchar(),str)==0 ) return N;
	};
	return -1;
};
//////////////////////////////////////////////////////////////////////////







































