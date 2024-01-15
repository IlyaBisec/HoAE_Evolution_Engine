#include "stdheader.h"
#include "BE_HEADERS.h"
#include "GameInterface.h"

bool	be_gCheckDirections(OneObject* pOB,OneObject* pTR){
	bool OK = false;
	if (pOB&&pTR) {
		OK = ( ( (TCos[pTR->RealDir]*(pTR->RealX-pOB->RealX)/16+TSin[pTR->RealDir]*(pTR->RealY-pOB->RealY)/16)<=0 ) ? (true) : (false) );
	};
	return OK;
};

bool	be_gCheckAbsAttackAbility(OneObject* pOB,OneObject* pTR){
	bool OK = false;
	if (pOB&&pTR) {
		OK = ( (pOB->NMask&pTR->NMask) ? (false) : (true) );
		if (OK) OK = ( (pOB->newMons->KillMask&pTR->MathMask) ? (true) : (false) );
	}
	return OK;
};

bool	be_gCheckImedAttackAbility(OneObject* pOB,OneObject* pTR){
	bool OK = false;	
	if (pOB&&pTR) {
		int cD = be_gGetAbsDistance(pOB,pTR);
		if	( pOB->MoreCharacter->DetRadius2[0]!=0 ) {
			OK = (pOB->MoreCharacter->AttackRadius1[0]<cD && cD<pOB->MoreCharacter->AttackRadius2[0]);
		};
		if ( OK==false && pOB->MoreCharacter->DetRadius2[1]!=0 ) {
			OK = (pOB->MoreCharacter->AttackRadius1[1]<cD && cD<pOB->MoreCharacter->AttackRadius2[1]);
		};
	};
	return OK;
};

bool	be_gCheckCurAttackAbility(OneObject* pOB,OneObject* pTR){
	bool OK = false;
	if (pOB&&pTR) {
		OK = be_gCheckAbsAttackAbility(pOB,pTR);
		if (OK) {
			// Test range unit
			OK = be_gCheckImedAttackAbility(pOB,pTR);
			if (OK==false) {
				OK = be_gCheckReachable(pOB,pTR);
			};
		};
	};
	return OK;
};

bool	be_gCheckReachable(OneObject* pOB,OneObject* pTR){
	bool OK = false;
	if (pOB&&pTR) {
		if (pOB->GroupSpeed!=0){
			OK = ( ( be_gGetTopDistance(pOB,pTR)<0xFFFE ) ? (true) : (false) );
		};
	}
	return OK;
};

int		be_gGetAbsDistance(OneObject* pOB,OneObject* pTR){
	if (pOB&&pTR){
		return Norma( (pOB->RealX-pTR->RealX)/16,(pOB->RealY-pTR->RealY)/16 );
	};
	return 0xFFFE;
};

int		be_gGetTopDistance(OneObject* pOB,OneObject* pTR){
	if (pOB&&pTR){
		int GetTopDistance(int xa,int ya,int xb,int yb,int LT);
		return GetTopDistance(pOB->RealX>>10,pOB->RealY>>10,pTR->RealX>>10,pTR->RealY>>10,pOB->LockType);
	};
	return 0xFFFE;
};

int GetTopDistance(int xa,int ya,int xb,int yb,int LT,byte NI);
bool	be_gControlNodeArray(be_CARRAY* pARR,int LT,int NI,bool circle/*=false*/){
	if ( pARR==NULL || LT<0 || LT>4 || NI<0 || NI>7 ) return false;
	int arrN = pARR->GetAmount();
	if ( arrN==1 ) return true;
	// Finde bad First index, and delete not need additional
	int firstID = -1;
	be_CNODE* pND00=NULL;
	be_CNODE* pND01=NULL;
	int _topD = 0;
	int _topD1= 0;
	int _decD = 0;
	/*
	if ( ...->SegmFR & 0x00000010 ) it meen that is end (not need calculate)
	*/
	for ( int id=0; (firstID==-1)&&((!circle&&id<arrN-1)||(circle&&id<arrN)); id++ ){
		pND00 = (be_CNODE*)( (*pARR)[id] );
		pND01 = (be_CNODE*)( (circle&&id==arrN-1) ? ( (*pARR)[0] ) : ( (*pARR)[id+1] ) );
		if ( !( pND00->SegmFR & 0x00000010 ) ){ // !!!
			_topD = GetTopDistance(pND00->x/64,pND00->y/64,pND01->x/64,pND01->y/64,LT,NI);
			_decD = Norma(pND00->x-pND01->x,pND00->y-pND01->y);
			if (_topD==0xFFFF||_topD*64>_decD*2) firstID=id;
		};
	};
	static bool Stop = false;
	if ( firstID==-1 || Stop ){
		return true;
	};
	// BAD WAY !!!
	pND00 = (be_CNODE*)( (*pARR)[firstID] );
	pND01 = (be_CNODE*)( (circle&&firstID==arrN-1) ? ( (*pARR)[0] ) : ( (*pARR)[firstID+1] ) );
	_decD = Norma(pND00->x-pND01->x,pND00->y-pND01->y);
	RoundZone rz0(pND00->x,pND00->y,_decD);
	RoundZone rz1(pND01->x,pND01->y,_decD);
	pUnitsList bl0; rz0.CollectUnits(bl0,COLLECT_BUILDINGS);
	pUnitsList bl1; rz1.CollectUnits(bl1,COLLECT_BUILDINGS);
	bl0 &= bl1;
	int min_sumD = -1;
	int sumD	 = 0;
	iVector2D	pos;
	pUnit		U;
	rz0.x /= 64; rz0.y /= 64;
	rz1.x /= 64; rz1.y /= 64;
	ForEachUnit	( bl0,	{ 
							pos = unit.GetCoor();
							sumD = GetTopDistance(pos.x/64,pos.y/64,rz0.x,rz0.y,LT,NI)
								   +GetTopDistance(pos.x/64,pos.y/64,rz1.x,rz1.y,LT,NI);
							if ( min_sumD>sumD || min_sumD==-1 ){
								min_sumD = sumD;
								U = unit;
							};
						};
				);
	// finde new top pos
	int sn;
	int topX,topY;
	int minTD0=-1;	int topX0=-1,topY0=-1;
	int minTD1=-1;	int topX1=-1,topY1=-1;
	pos = U.GetCoor();
	pos.x /= 64.0f; pos.y /= 64.0f;
	int areaID = -1;
	for ( int i=1; i<4; i++ ){
		sn = Rarr[i].N;
		while (sn--) {
			topX = pos.x+Rarr[i].xi[sn];
			topY = pos.y+Rarr[i].yi[sn];
			areaID = HashTable[LT].GetTopAreaIndex(topX,topY);
			if ( areaID!=0xFFFF && areaID!=0xFFFE ){
				_topD1= Norma(topX-pos.x,topY-pos.y);
				_topD = GetTopDistance(topX,topY,rz0.x,rz0.y,LT,NI);
				if ( _topD!=0xFFFF && ( _topD+_topD1>minTD0 || minTD0==-1 ) ){
					minTD0 = _topD+_topD1;
					topX0 = topX; topY0 = topY;
				};
				_topD = GetTopDistance(topX,topY,rz1.x,rz1.y,LT,NI);
				if ( _topD!=0xFFFF && ( _topD+_topD1>minTD1 || minTD1==-1 ) ){
					minTD1 = _topD+_topD1;
					topX1 = topX; topY1 = topY;
				};
			};
		};
	};
	// add new pos in array
	/*
	set ...->SegmFR += 0x00000100 if it added in recalvulate WAY
	*/
	if ( topX0!=-1 && topY0!=-1 ){
		pND00 = new be_CNODE( topX0*64+32, topY0*64+32 );
		pND00->SegmFR  = 0x00000010;	// END POSITION !!!
		pND00->SegmFR += 0x00000100;	// NEW POS !!!
		pND00->GetObjectGlobalID();
		g_beVAR_STORE.NODE_LIST.Add(pND00);
		pARR->InsertValue(pND00,firstID+1);
	};
	if ( topX1!=-1 && topY1!=-1 ){
		pND01 = new be_CNODE( topX1*64+32, topY1*64+32 );
		pND01->SegmFR  = 0x00000000;
		pND01->SegmFR += 0x00000100;	// NEW POS !!!
		pND01->GetObjectGlobalID();
		g_beVAR_STORE.NODE_LIST.Add(pND01);
		pARR->InsertValue(pND01,firstID+2);
	};
	Stop = true;
	return true;
};

bool	be_gMarkBadNode(be_CARRAY* pARR,int LT,int NI,bool circle/*=false*/){
	if ( pARR==NULL || LT<0 || LT>4 || NI<0 || NI>7 ) return false;
	int arrN = pARR->GetAmount();
	if ( arrN==1 ) return false;
	// Finde bad First index, and delete not need additional
	int firstID = -1;
	be_CNODE* pND00=NULL;
	be_CNODE* pND01=NULL;
	int _topD = 0;
	int _decD = 0;
	/*
	if ( ...->SegmFR & 0x00000010 ) it meen that is end (not need calculate)
	*/
	int areaID00 = -1;
	int areaID01 = -1;
	bool newBAD = false;
	for ( int id=0; (firstID==-1)&&((!circle&&id<arrN-1)||(circle&&id<arrN)); id++ ){
		pND00 = (be_CNODE*)( (*pARR)[id] );
		pND01 = (be_CNODE*)( (circle&&id==arrN-1) ? ( (*pARR)[0] ) : ( (*pARR)[id+1] ) );
		areaID00 = HashTable[LT].GetTopAreaIndex(pND00->x/64,pND00->y/64);
		areaID01 = HashTable[LT].GetTopAreaIndex(pND01->x/64,pND01->y/64);
		if ( areaID01==0xFFFF || areaID01==0xFFFE ){
			newBAD = newBAD || ( (pND01->SegmFR!=100) ? (true) : (false) );
			pND01->SegmFR = 100;
		};
		if ( areaID00!=0xFFFF&&areaID00!=0xFFFE&&areaID01!=0xFFFF&&areaID01!=0xFFFE ){
			_topD = GetTopDistance(pND00->x/64,pND00->y/64,pND01->x/64,pND01->y/64,LT,NI);
			_decD = Norma(pND00->x-pND01->x,pND00->y-pND01->y);
			if (_topD==0xFFFF||_topD*64>_decD*2){
				newBAD = newBAD || ( (pND01->SegmFR!=100) ? (true) : (false) );
				pND01->SegmFR = 100;
			}else if (!newBAD){
				newBAD = newBAD || ( (pND01->SegmFR!=0) ? (true) : (false) );
				pND01->SegmFR = 0;
			};
		};
				
	};
	return newBAD;
};	

void	be_gGetTextInCH(_str& _dstText,char* _srcText,char _openCH,char _closeCH){
	_dstText = "";
	if ( _srcText==NULL || _srcText[0]==0 ) return;
	char* pOpenBR  = strchr(_srcText,_openCH);
	char* pCloseBR = strchr(_srcText,_closeCH);
	int len = (int)(pCloseBR-pOpenBR);
	char* text = (char*)malloc(len);
	pOpenBR = &(pOpenBR[1]);
	strncpy(text,pOpenBR,len-1);
	text[len-1]=0;
	_dstText = text;
	free(text);
};
void	be_gBrushTetx(_str& _text){
	_str textCopy = _text;
	char* pTetx  = textCopy.pchar();
	int i = 0;
	while ( pTetx[i]==' ' ) {
		pTetx = &(pTetx[1]);
	};
	_text = pTetx;
	i = strlen( _text.pchar() )-1;
	while ( _text.pchar()[i]==' ' || _text.pchar()[i]=='\n' || _text.pchar()[i]=='\r' || _text.pchar()[i]=='\t' ) {
		i--;
	};
	_text.DeleteChars(i+1,strlen(_text.pchar())-i-1);
};

void	be_gConvertText(_str& STRING,_str& _textID,_str& _speaker,_str& _spirit,_str& _ogFile,_str& _text){
	_textID	= "";
	_speaker= "";
	_spirit = "";
	_ogFile = "";
	_text	= "";
	if ( STRING.pchar()==NULL || STRING.pchar()[0]==0 )	return;
	char* pTextID_end = strchr( STRING.pchar(), ' ' );
	if ( pTextID_end ) {
		char __textID[128];
		strncpy(__textID,STRING.pchar(),(int)( pTextID_end - STRING.pchar() ));
		__textID[ (int)( pTextID_end - STRING.pchar() ) ] = 0;
		_textID = __textID;
	}else{
		return;
	}
	_str TEXT = strchr( STRING.pchar(), ' ' );
	if ( TEXT.pchar()==NULL || TEXT.pchar()[0]==0 )		return;
	// FILL _text
	char* pCloseBR = strrchr(TEXT.pchar(),']');
	if ( pCloseBR == NULL ){
		_text = TEXT;
		return;
	};
	_text = &(pCloseBR[1]);
	// FILL _speaker
	be_gGetTextInCH( _speaker, TEXT.pchar(), '[', ']' );
	// FILL _spirit
	pCloseBR = strchr(TEXT.pchar(),']');
	pCloseBR = &(pCloseBR[1]);
	be_gGetTextInCH( _spirit, pCloseBR, '[', ']' );
	// FILL _ogFile
	pCloseBR = strchr(pCloseBR,']');
	pCloseBR = &(pCloseBR[1]);
	be_gGetTextInCH( _ogFile, pCloseBR, '[', ']' );
};

void	be_gConvertTextID(_str& _textID,_str& _speaker,_str& _spirit,_str& _ogFile,_str& _text){
	_speaker= "";
	_spirit = "";
	_ogFile = "";
	_text	= "";
	if ( _textID.pchar()==NULL || _textID.pchar()[0]==0 )	return;
	_str TEXT = GetTextByID( _textID.pchar() );
	if ( TEXT.pchar()==NULL || TEXT.pchar()[0]==0 )		return;
	// FILL _text
	char* pCloseBR = strrchr(TEXT.pchar(),']');
	if ( pCloseBR == NULL ){
		_text = TEXT;
		return;
	};
	_text = &(pCloseBR[1]);
	// FILL _speaker
	be_gGetTextInCH( _speaker, TEXT.pchar(), '[', ']' );
	// FILL _spirit
	pCloseBR = strchr(TEXT.pchar(),']');
	pCloseBR = &(pCloseBR[1]);
	be_gGetTextInCH( _spirit, pCloseBR, '[', ']' );
	// FILL _ogFile
	pCloseBR = strchr(pCloseBR,']');
	pCloseBR = &(pCloseBR[1]);
	be_gGetTextInCH( _ogFile, pCloseBR, '[', ']' );
};
bool	be_gPrepareString(_str& FullString,char* TextID,be_CParamList& paramList){
	if (FullString.pchar()!=NULL) FullString.Clear();
	char	data[4096];
	char*	sPart = NULL;
	char	first[4096];
	char	second[4096];
	sprintf(data,"%s",GetTextByID(TextID));
	int NP = paramList.GetAmount();
	int cP=0;
	if (NP==0){
		FullString = data;
		return false;
	};

	// Have some params for insert
	bool stop=false;
	int iteration=0;
	while (!stop) {
		iteration++;
		sprintf(first,"%s","");
		sprintf(second,"%s","");
		sPart = strstr(data,"%");
		if (sPart!=NULL) {
			int nc = (int)(sPart-data+1);
			strncpy(first,data,nc+1);
			first[nc+1]=0;
			if (data[nc+1]!=0)	sprintf(second,"%s",&(data[nc+1]));
			// set params
			if (cP<NP){
				if		 (strcmp("be_CINT",paramList[cP]->GetParamType())==0){
					be_CVariableBase* CVB=paramList[cP]->GetVariable();
					if(CVB){
						int val = ((be_CINT*)(CVB))->GetValue();
						int len = strlen(first)-1;
						if ( first[len]=='t' ){
							int l_val = val;
							_str	l_time;
							if ( l_val/3600>0 ){ 
								l_time += ( l_val/3600 ); 
								l_time += ":"; 
								l_val = l_val - (l_val/3600)*3600; 
							};
							if ( l_val/60>0 ){ 
								if ( l_val/60<=9 ) l_time += "0";
								l_time += ( l_val/60 ); 
								l_time += ":"; 
								l_val = l_val - (l_val/60)*60; 
							}else{ 
								l_time += "00:"; 
							};
							if ( l_val<=9 ) l_time += "0";
							l_time += l_val;
							first[len] = 's';
							sprintf( data, first, l_time.pchar() );
						}else{
							if ( first[len]=='u' ){
								val = abs(val);
								first[len] = 'd';
							};
							sprintf( data, first, val );
						};
						strcpy(first,data);
					}
				}else if (strcmp("be_CSTR",paramList[cP]->GetParamType())==0){
					sprintf( data, first, GetTextByID(((be_CSTR*)((paramList[cP])->GetVariable()))->GetValue()) );
					strcpy(first,data);
				}/*else if (strcmp("",paramList[cP]->ParamType.pchar())==0) {
					_str picture;
					if ( ((vvPICTURE*)(paramList[cP]))->GetAsStringForMessage(picture) ){
						sprintf(data,first,picture.pchar());
						strcpy(first,data);
					}else{
						sprintf(data,first,"NO PICTURE");
						strcpy(first,data);
					};
				}*/
				cP++;
			};
			// create new data string
			if (strlen(second)!=0)	sprintf(data,"%s%s",first,second);
			else					sprintf(data,"%s",first);
		};
		if (sPart==NULL||cP>=NP||iteration>=100) {
			stop=true;
		};
	};

	FullString = data;
	return true;	
};
bool	be_gPrepareString(be_CSTR* FullString,be_CSTR* TextID,be_CARRAY* paramARR){
	if ( FullString==NULL && TextID==NULL ) return false;
	if ( paramARR==NULL )	FullString->SetValue( GetTextByID( TextID->GetValue() ) );
	else					be_gPrepareString(FullString->Value,TextID->GetValue(),paramARR->ValuesArray);
	return true;	
};
// FSM
be_CFSM*			be_gFindeFSM	( const char* FSM_Name ){
	if ( g_beCheckChar(FSM_Name)==false ) return NULL;
	return (be_CFSM*)(g_beVAR_STORE.GET("be_CFSM",(char*)FSM_Name));
};
bool				be_gLoadFSM		( const char* FSM_Name ){
	_str _fileName = "";
	if ( g_beFSM_LOADER.GetFilePath(_fileName,FSM_Name) ){
		be_CFSM* pFSM = new be_CFSM();
		g_beVAR_STORE.FSM_LIST.Add( pFSM );
		pFSM->ReadFromFile( _fileName.pchar() );
		pFSM->LoadLuaFiles();
		pFSM=NULL;
		return true;
	};
	return false;
};
// SO
bool				be_gCheckObjectFSM	( const char* ObjectName, const char* FSM_Name ){
	bool	AlreadyExist = false;
	if ( g_beCheckChar(ObjectName) && g_beCheckChar(FSM_Name) ){
		be_CSMARTOBJECT*	pSO		= NULL;
		be_CVariableBase*	_pVAR	= NULL;
		int N = g_beVAR_STORE.SMARTOBJ_LIST.GetAmount();
		for ( int i=0; (i<N)&&(AlreadyExist==false); i++ ){
			pSO = g_beVAR_STORE.SMARTOBJ_LIST[i];
			if ( pSO && g_beCheckChar(pSO->state_machine.pchar()) &&  strcmp(pSO->state_machine.pchar(),FSM_Name)==0 ){
				_pVAR = g_beVAR_STORE.SMARTOBJ_LIST[i]->GET_OBJECT();
				if ( _pVAR && g_beCheckChar(_pVAR->Name.pchar()) && strcmp(_pVAR->Name.pchar(),ObjectName)==0 ){
					AlreadyExist = true;
				};
			};
		};
	};
	return AlreadyExist;	
};
bool				be_gFreeSO_pSO		( be_CSMARTOBJECT*	pSO	){
	if (pSO) {
		pSO->ControledObject.SetRef("NULL",0xFFFFFFFF);
	};
	return (pSO!=NULL);
};
bool				be_gFreeSO_SOName	( const char* SO_Name  ){
	be_CSMARTOBJECT* pSO = NULL;
	if ( g_beCheckChar( SO_Name ) ) {
		Enumerator* E=ENUM.Get("BE_SMARTOBJECT_ENUM");
		DWORD gid = E->Get((char*)SO_Name);
		if (gid!=-1&&gid<CPGARB.CPointer[gid>>24].GetAmount()) {
			OneClassPointer* pOCP = CPGARB.CPointer[gid>>24][gid&0xFFFFFF];
			if (pOCP) {
				pSO = (be_CSMARTOBJECT*)(pOCP->ClassPtr);
			};
		};
	}
	return be_gFreeSO_pSO(pSO);
};
bool				be_gFreeSO_pVAR		( be_CVariableBase* pVAR ){
	be_CSMARTOBJECT*	pSO		= NULL;
	if ( pVAR && g_beCheckChar(pVAR->Name.pchar()) ){
		be_CVariableBase*	_pVAR	= NULL;
		int N = g_beVAR_STORE.SMARTOBJ_LIST.GetAmount();
		for ( int i=0; i<N; i++ ){
			_pVAR = g_beVAR_STORE.SMARTOBJ_LIST[i]->GET_OBJECT();
			if ( _pVAR && g_beCheckChar(_pVAR->Name.pchar()) && _pVAR->Name==pVAR->Name ){
				pSO = g_beVAR_STORE.SMARTOBJ_LIST[i];
				be_gFreeSO_pSO(pSO);
			};
		};
	};
	return true;
};
be_CSMARTOBJECT*	be_gFindSO		( const char* FSM_Name ){
	be_CSMARTOBJECT* pretSO = NULL;
	be_CSMARTOBJECT* pSO = NULL;
	if ( g_beCheckChar(FSM_Name) ) {
		ReferableBaseClass* pObject = NULL;
		ReferableBaseClass* pFSM = NULL;
		int id = 0;
		while ( pObject=g_beVAR_STORE.GET("be_CSMARTOBJECT",id) ) {
			id++;
			pSO	= (be_CSMARTOBJECT*)(pObject);
			if ( pSO ) {
				pFSM = pSO->state_machine.Get();
				if ( pFSM && g_beCheckChar(pFSM->Name.pchar()) ){
					if ( strcmp(pFSM->Name.pchar(),FSM_Name)==0 ){
						pretSO = pSO;
					};
				};
			};
		};
	};
	return pretSO;
};
be_CSMARTOBJECT*	be_gFindSObyOBName( const char* OB_Name ){
	be_CSMARTOBJECT* pretSO = NULL;
	be_CSMARTOBJECT* pSO = NULL;
	if ( g_beCheckChar(OB_Name) ) {
		ReferableBaseClass* pObject = NULL;
		int id = 0;
		while ( pObject=g_beVAR_STORE.GET("be_CSMARTOBJECT",id) ) {
			id++;
			pSO	= (be_CSMARTOBJECT*)(pObject);
			if ( pSO && g_beCheckChar(pSO->ControledObject.pchar()) && strcmp(OB_Name,pSO->ControledObject.pchar())==0 ) {
				pretSO = pSO;
			};
		};
	};
	return pretSO;
};
be_CSMARTOBJECT*	be_gFindSObyName( const char* SO_Name ){
	be_CSMARTOBJECT* pretSO = NULL;
	be_CSMARTOBJECT* pSO = NULL;
	if ( g_beCheckChar(SO_Name) ) {
		ReferableBaseClass* pObject = NULL;
		int id = 0;
		while ( pObject=g_beVAR_STORE.GET("be_CSMARTOBJECT",id) ) {
			id++;
			pSO	= (be_CSMARTOBJECT*)(pObject);
			if ( pSO && g_beCheckChar(pSO->Name.pchar()) && strcmp(pSO->Name.pchar(),SO_Name)==0 ) {
				pretSO = pSO;
			};
		};
	};
	return pretSO;
};
be_CSMARTOBJECT*	be_gFindFreeSO	( const char* FSM_Name ){
	be_CSMARTOBJECT* pretSO = NULL;
	be_CSMARTOBJECT* pSO = NULL;
	if ( g_beCheckChar(FSM_Name) ) {
		ReferableBaseClass* pObject = NULL;
		ReferableBaseClass* pFSM = NULL;
		int id = 0;
		while ( pObject=g_beVAR_STORE.GET("be_CSMARTOBJECT",id) ) {
			id++;
			pSO	= (be_CSMARTOBJECT*)(pObject);
			if ( pSO && pSO->ControledObject.Get()==NULL ) {
				pFSM = pSO->state_machine.Get();
				if ( pFSM && g_beCheckChar(pFSM->Name.pchar()) ){
					if ( strcmp(pFSM->Name.pchar(),FSM_Name)==0 ){
						pretSO = pSO;
					};
				};
			};
		};
	};
	return pretSO;
};
be_CSMARTOBJECT*	be_gCreateNewSO	( const char* FSM_Name ){
	be_CSMARTOBJECT* pSO	= NULL;
	if ( g_beCheckChar(FSM_Name) ) {
		be_CFSM* pFSM = be_gFindeFSM(FSM_Name);
		if (pFSM==NULL) {
			if (be_gLoadFSM(FSM_Name)){
				pFSM = be_gFindeFSM(FSM_Name);
			};
		};
        if (pFSM) {
			pSO = new be_CSMARTOBJECT();
			pSO->SetDefName();
			pSO->RegisterMembers();
			g_beVAR_STORE.SMARTOBJ_LIST.Add(pSO);
			pSO->ControledObject.SetRef("NULL",0xFFFFFFFF);
			pSO->state_machine.SetRef(pFSM->Name.pchar(),pFSM->GetObjectGlobalID());
//			pSO->CREATE_PRAM_FROM_FSM();
        };	
	};
	return pSO;
};
be_CSMARTOBJECT*	be_gGET_SO		( const char* FSM_Name ){
	be_CSMARTOBJECT* pSO = be_gFindFreeSO(FSM_Name);
	if ( pSO==NULL ) pSO = be_gCreateNewSO(FSM_Name);
	return pSO;	
};

//////////////////////////////////////////////////////////////////////////
// SYATEM
//////////////////////////////////////////////////////////////////////////
bool	be_gFilePresent(const char* fileName){
	if ( g_beCheckChar(fileName)==false ) return false;
	FILE* file = fopen( fileName, "r" );
	if ( file==NULL ) return false;
	else	fclose(file);
	return true;
};

//////////////////////////////////////////////////////////////////////////
// NATION
//////////////////////////////////////////////////////////////////////////
int		be_gGetNKilled(int _nation,int _unitType){
	if ( _nation<0 || _nation>=8 || _unitType<0 || _unitType>=2048 )	return 0;
	return NATIONS[_nation].NKilled[_unitType];
};

//////////////////////////////////////////////////////////////////////////
// SIGNAL (ALERT)
//////////////////////////////////////////////////////////////////////////
void	be_gAddAlertOnMap(int _x,int _y,int _delay,int _nat){
	extern int LastSignalX;
	extern int LastSignalY;
	extern int SignalDelay;
	extern DWORD SignalColor;
	LastSignalX=_x<<4;
	LastSignalY=_y<<4;
	SignalDelay=_delay;//60*256;
	DWORD GetNatColor( int natIdx );
	SignalColor=GetNatColor(_nat);
};


//////////////////////////////////////////////////////////////////////////
// TOPOGRAFY
//////////////////////////////////////////////////////////////////////////
int		be_gGetAreaX(int AreaID, int layerID){
	if ( AreaID>0 && AreaID<0xFFFF && AreaID!=0xFFFE ){
		Area* pArea = HashTable[layerID].GetTopArea( AreaID );
		if ( pArea!=NULL ) {
			return pArea->x;
		};
	};
	return -1;
};
int		be_gGetAreaY(int AreaID, int layerID){
	if ( AreaID>0 && AreaID<0xFFFF && AreaID!=0xFFFE ){
		Area* pArea = HashTable[layerID].GetTopArea( AreaID );
		if ( pArea!=NULL ) {
			return pArea->y;
		};
	};
	return -1;
};
//////////////////////////////////////////////////////////////////////////













