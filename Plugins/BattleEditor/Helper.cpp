#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
// USE FRO RETURN REFERENCE FOR TEMPORARY OBJECTS, fro DLL exports ///////
namespace __g_dll{
	_str			g_refSTR;
	pParamList		g_refPARAMS;
	pGroup			g_refGROUP;
	iNode			g_refNODE;
	iRect			g_refRECT;
	RoundZone		g_refRZONE;
	SquareZone		g_refSZONE;
	pARRAY			g_refARRAY;
	pSMARTOBJECT	g_refSMARTOBJECT;
	pABILITY		g_refABILITY;
	pSTR_DIALOG		g_refDIALOG;
};

//////////////////////////////////////////////////////////////////////////
// HELPER_FN
//////////////////////////////////////////////////////////////////////////
int l_SendGroupByWay(pGroup& GR,pARRAY& AR){
	if ( GR.Valid()==false || GR.getUN()==0 ) return 0;
	int		X			= GR.getXc();
	int		Y			= GR.getYc();
	int		_i			= 0;
	int		N			= AR.getSIZE();
	int		_id			= -1;
	int		min_dist	= -1;
	int		dist		= 0;
	float	dot			= 0.f;
	iNode		Node0;
	iNode		Node1;
	iVector2D	dir_v;
	iVector2D	dis_v;
	
	while ( _i < N-2 ){
		Node0 	= AR.getNODE(_i);
		Node1 	= AR.getNODE(_i+1);
		if ( Node0.Valid()==false || Node1.Valid()==false ) return 0;
		
		dir_v = iVector2D( Node1.getX()-Node0.getX(), Node1.getY()-Node0.getY() );
		dis_v = iVector2D( Node0.getX()-X, Node0.getY()-Y );
		dot = GI::DOT(dir_v,dis_v);
		dist = 0;
					
		if ( dot>=0 ) {
			dist = GI::NORMA( Node0.getX(), X, Node0.getY(), Y );
			if ( dist<min_dist || min_dist==-1 ){
				min_dist = dist;
				_id = _i;
			};
		};
		_i++;
	};
	
	int First = 0;
	if ( _id!=-1 ){
		while ( _id < N ){
			Node1 = AR.getNODE(_id);
			if ( Node1.Valid()==false ) return 0;
			_id++;
			GR.Send( Node1, DIR_AUTO, First );
			if (First==0) First=2;
		};
	}else{
		Node1 = AR.getNODE(N-1);
		GR.Send( Node1, DIR_AUTO, First );
	};
	
	return 1;
	
};

//////////////////////////////////////////////////////////////////////////
// HELPER_FSM			
//////////////////////////////////////////////////////////////////////////
bool	SO_GROUP_IS_DEAD( pSMARTOBJECT& C_OBJ ){
	pGroup _group 	= C_OBJ.getOBJ_GROUP();
	if ( _group.Valid()==false )return true;
	//if ( _group.getUN()==0 ){
	//	GI::FreeSO_pVAR(_group);
	//	(C_OBJ.getPARAMS()).CLEAR();
	//	return true;
	//};
	return false;
};

int		getEnemyAmount(int NMask,int x, int y, int r, int addR/*=0*/,collect_option Flags/*=COLLECT_UNITS*/){
	pUnitsList	UL;
	RoundZone _zone(x,y,r); _zone.r += addR;
	_zone.CollectUnits(UL,Flags);
	UL.ExcludeByNMask( NMask );
	UL.RemoveDeadUnits();
	return UL._getamount();
};
int		getEnemyAmount(int NMask,iNode& ND,int addR/*=0*/,collect_option Flags/*=COLLECT_UNITS*/){
	pUnitsList	UL;
	RoundZone _zone(ND); _zone.r += addR;
	_zone.CollectUnits(UL,Flags);
	UL.ExcludeByNMask( NMask );
	UL.RemoveDeadUnits();
	return UL._getamount();
};

int		GroupCheckPosArray( pGroup& _group, pARRAY& _arrPos ){
	if ( _group.Valid() && _arrPos.Valid() ){
		int		arrN = _arrPos.getSIZE();
		iNode	node;
		ForEachUnit	(	_group, 
						bool OK = false;
						for(int i=0;i<arrN&&(OK==false);i++){
							node=_arrPos.getNODE(i);
							OK = node.IsIn(unit);
						}; 
						if (!OK) return false;
					);
		return true;
	};
	return false;
};
bool	ProcMsgFree( pMSG& msg, bool curSTATE ){
	if ( msg.str() == "MSG_IS_FREE" ){
		if ( (msg.getPARAMS()).Valid()==false ) return false;
		pParamList _msg_par = msg.getPARAMS();
		if ( _msg_par.check("bFREE")==false	  ) return false;
		_msg_par.getBOOL("bFREE") = curSTATE;
		return true;
	};
	return false;
};
int		GetNationNMASK( int Nat ){
	return GI::GetNationMask(Nat);
/*
	if ( Nat==0 ) return 1;
	if ( Nat==1 ) return 2;
	if ( Nat==2 ) return 4;
	if ( Nat==3 ) return 8;
	if ( Nat==4 ) return 16;
	if ( Nat==5 ) return 32;
	if ( Nat==6 ) return 64;
	if ( Nat==7 ) return 128;
	return 255;
*/
};
int		Get_arrTN_UnitNumberSum( pARRAY& arrTN ){
	int sum = 0;
	if ( arrTN.Valid() && arrTN.getSIZE()>0 ){
		int SN = arrTN.getSIZE();
		for ( int i=0; i<(SN/2); i++ ) sum += arrTN.getINT(2*i+1);
	};
	return sum;
};
iNode	GetSafeNODE( pARRAY& arrNode, int id ){
	if ( arrNode.Valid() ){
		int NN = arrNode.getSIZE();
		if 		( id< 0 )	__g_dll::g_refNODE = arrNode.getNODE(NN-1);
		else if	( id>=NN )	__g_dll::g_refNODE = arrNode.getNODE(0);
		else				__g_dll::g_refNODE = arrNode.getNODE(id);
	};	
	return __g_dll::g_refNODE;
};


int		GetNearestNodeID( pARRAY& arrNode, int x, int y, bool cirkle/*=false*/ ){ 
	int id = -1;
	if ( arrNode.Valid() ){
		int	NN	 = arrNode.getSIZE();
		int	curD = 0;
		int minD = -1;
		iNode node;
		while ( NN-- ){
			node = arrNode.getNODE(NN);
			curD = (int)sqrt( (float)( (node.getX()-x)*(node.getX()-x) + (node.getY()-y)*(node.getY()-y) ) );
			if ( curD < minD || minD == -1 ){
				minD = curD;
				id = NN;
			};
		};
		// Fine node on position dependence
		node = arrNode.getNODE(id);
		iVector2D v0( x-node.getX(),y-node.getY() );
		int nextID = -1; iVector2D nextV;
		NN = arrNode.getSIZE();
		if ( cirkle )	nextID = ( ((id+1)<NN) ? (id+1) : (0) );
		else			nextID = ( ((id+1)<NN) ? (id+1) : (-1) );
		iNode	nextNODE;
		if (nextID!=-1){ 
			nextNODE = arrNode.getNODE(nextID); 
			nextV.x= nextNODE.getX()-node.getX();
			nextV.y= nextNODE.getY()-node.getY();
			if ( GI::DOT(v0,nextV)>=0.0f ){
				return nextID;
			};
		};
	};
	return id;
};

int		LinkNodeToGroup00( iNode& Node, pGroup& Group, iNode& Node10, iNode& Node00, iNode& Node01 ){
	bool OK = Node.Valid()&&Group.Valid()&&Node10.Valid()&&Node00.Valid()&&Node01.Valid();
	if ( !OK ) return 0;
	float	x10 = Node10.getX()  - Node00.getX();
	float	y10 = Node10.getY()  - Node00.getY();
	float	x01 = Node01.getX()  - Node00.getX();
	float	y01 = Node01.getY()  - Node00.getY();
	float	x2  = Group.getXc()  - Node00.getX();
	float	y2  = Group.getYc()  - Node00.getY();
	float	a10 = ( x10 * x2 + y10 * y2 ) / ( x10 * x10 + y10 * y10 );
	float	a01 = ( x01 * x2 + y01 * y2 ) / ( x01 * x01 + y01 * y01 );
	
	float	x1 = x10;
	float	y1 = y10;
	float	a  = a10;
	if ( a<a01 ){
		a = a01;
		x1 = x01;
		y1 = y01;
	};
	
	if 		( a<0.0f )	a=0.0f;
	else if ( a>1.0f ) a=1.0f;
	
	Node.setX( (int)( x1*a + Node00.getX() ) );
	Node.setY( (int)( y1*a + Node00.getY() ) );

	return 1;
};

/*
Description:
	- Используем различные функции поска для AreaID
*/
bool	FINDE_USE_AST_RATE( pSMARTOBJECT& C_OBJ, char* pszAreaID ){
	pGroup		_group = C_OBJ.getOBJ_GROUP();
	pParamList	_par   = C_OBJ.getPARAMS();
	if ( _par.Valid()==false || _group.Valid()==false ) return false;
	if ( _par.check(pszAreaID) ){
		int& AreaID = _par.getINT(pszAreaID);
		if ( AreaID==-1 ){
			int iNewAreaID = FindeEnemyAreaID( _group, 1 );
			if ( iNewAreaID!=-1 ){
				AreaID = iNewAreaID;
				return true;
			};	
		};
	};
	return false;
};

/*
Description:
	- Выбор цели для следования и уничтожения
*/
int 	FindeEnemyAreaID( pGroup& pGRP, int iPower ){
	int AreaID = -1;
	if ( pGRP.Valid()==false || pGRP.getUN()<=0 ) return AreaID;
	be_CAreaListPowerData GRP_ALPD;
	if ( GRP_ALPD.create( pGRP ) ){
		int _iMyLockType	= pGRP.getLockType();
		be_CTotalALPD ENEMY_T_ALPD;
		int iPrivMinRate = -1;
		for ( int _iEnNat=0; _iEnNat<8; _iEnNat++ ){
			if ( !( pGRP.getNMASK() & GetNationNMASK(_iEnNat) ) ){
				if ( ENEMY_T_ALPD.create(_iMyLockType,_iEnNat,iPower) ){
					AreaID = AL_AST_RATE( ENEMY_T_ALPD, iPrivMinRate );
				};
			};
		};
	};
	return AreaID;
};

/*
	- Алгоритмы выбора подходящей цели
*/
// Поиск слабейшего 
int		AL_AST_RATE( be_CTotalALPD& ENEMY_T_ALPD, int iPrivMinRate ){ 
	int iAreaID = -1;
	be_CAreaListPowerData* _ALPD = NULL;
	int _iMinUDP = iPrivMinRate;
	int _iALPD_ID = -1;
	for ( int _iEnAS=0; _iEnAS<ENEMY_T_ALPD.getN(); _iEnAS++ ){
		_ALPD = ENEMY_T_ALPD.getALPD(_iEnAS);
		if ( _ALPD!=NULL && ( _iMinUDP==-1 || _iMinUDP>_ALPD->getUnitsDefencePOWER() ) ){
			_iMinUDP = _ALPD->getUnitsDefencePOWER();
			_iALPD_ID = _iEnAS;
		};
	};
	if ( _iALPD_ID!=-1 ){
		_ALPD = ENEMY_T_ALPD.getALPD(_iALPD_ID);
		iAreaID = _ALPD->getWeakestAST_AID();
	};
	
	return iAreaID;
};
//////////////////////////////////////////////////////////////////////////
























