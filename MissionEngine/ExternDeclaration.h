

//-------------------------TYPES-----------------------------
//DECLARTION  FROM "MainFunction.h"
//DECLARTION  FROM "MainClass.h"
extern	class SARMY_DESCRIPTION;

//extern	class CARMY_GAMEOBJ;
//DECLARTION  FROM "MissionInit.h"
extern	struct SXYF;
extern	class CTYPENUM;
extern	class CSAVE_MIS_STRUCT;

//------------------------FUNCTIONS--------------------------
//DECLARTION  FROM "MainFunction.h"
extern	void	REG_ALL_CLASS();
extern	void	UNREG_ALL_CLASS();
extern	int		MinLV(int a, int b);
extern	int		MaxLV(int a, int b);
extern	void	setResource( byte NI, int wood, int food, int stone, int gold, int iron, int coal );
extern	void	setStartPoint( int x, int y );	
extern	void	CreateHeroArmy( byte NI, ClassArray<CTYPENUM>* pARM, SXYF* pPOS );
extern	void	createDefenderArmy(byte NI,SARMY_DESCRIPTION* pAA,SARMY_DESCRIPTION* pDA,ClassArray<SXYF>* pP,ClassArray<SXYF>* pC,ClassArray<SXYF>* pR,ClassArray<CTYPENUM>* pDAR);
extern	void	createAttackerArmy(byte NI,SARMY_DESCRIPTION* pAA,ClassArray<SXYF>* pP,ClassArray<SXYF>* pC,ClassArray<SXYF>* pR, ClassArray<SXYF>* pArt ,ClassArray<CTYPENUM>* pDAR, int Real_NI );
extern	void	createAttackerArmy(CSAVE_MIS_STRUCT* pIMD);
extern	void	createDefenderArmy(CSAVE_MIS_STRUCT* pIMD);
extern	void	createCamp(CSAVE_MIS_STRUCT* pIMD, _str& dir);
extern	void	setDigit(unsigned long int& X, int r, int n);
extern	int		getDigit(const unsigned long int X, int r);
extern	int		getNRate(const int value, const int rate);
//DECLARTION  FROM "MainClass.h"
//DECLARTION  FROM "MissionInit.h"
extern	void	deleteSXYFarr( ClassArray<SXYF>* pXYF );
extern	void	deleteCTYPENUMarr( ClassArray<CTYPENUM>* pTN );
extern	SXYF	getArmyStatrtPos(int sect_id, _str& dir_id);
extern	void	getDefenderPos( ClassArray<SXYF>* pPrm, ClassArray<SXYF>* pCnt, ClassArray<SXYF>* pRsv, ClassArray<SXYF>* pArt, _str& dir_id );
extern	void	getAttackerPos( ClassArray<SXYF>* pPrm, ClassArray<SXYF>* pCnt, ClassArray<SXYF>* pRsv, ClassArray<SXYF>* pArt, _str& dir_id );
extern	void	prepareAI_ZONE(_str& dir_id,LinearArray<int,_int>* pZID,LinearArray<int,_int>* pZALL);
//DECLARTION  FROM "EndMissionData.h"
extern	class	CRestArmy;
extern	class	CEndMissionData;





