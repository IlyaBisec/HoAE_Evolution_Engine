

//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////    SARMY_DESCRIPTION    /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class SARMY_DESCRIPTION : public BaseClass {
public:
	SARMY_DESCRIPTION();
	SARMY_DESCRIPTION(ClassArray<CTYPENUM>* pA);
	~SARMY_DESCRIPTION() {
		return; };
public:
	int		SQ_NUM;		//���-�� ������� ����������.
	int		SQ_K_NUM;	//���-�� ������ ������� ����������.
	int		SQ_P_NUM;	//���-�� ����� ������� ����������.
	int		MOBILITY;	//����������� �����.	(0-100)
	int		CANNON_NUM;	//���-�� �����.

	void	Init();
	bool	setArmy(ClassArray<CTYPENUM>* pA);
	
	bool	operator==(const SARMY_DESCRIPTION& ARM);
	bool	operator< (const SARMY_DESCRIPTION& ARM);
	bool	operator> (const SARMY_DESCRIPTION& ARM);

	// comment by Grey [18/02/2004]
	//SAVE( SARMY_DESCRIPTION );
	//	REG_MEMBER( _int,	SQ_NUM		);
	//	REG_MEMBER( _int,	SQ_K_NUM	);
	//	REG_MEMBER( _int,	SQ_P_NUM	);
	//	REG_MEMBER( _int,	MOBILITY	);
	//	REG_MEMBER( _int,	CANNON_NUM	);
	//ENDSAVE; 
};
//----------------------------------------------------------------------------------

//new
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////    CMissionLetter    /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class CMissionLetter : public BaseClass { //new - LETTERS feat ShowPageParam
public:
	CMissionLetter()  { Init(); };
	~CMissionLetter() {	return; };
	void Init() {
		return;
	}
public:
	int		type;		//
	int		text;		//
	int		realX;
	int		realY;
};
//----------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////    CARMY_GAMEOBJ    ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
/*
class CARMY_GAMEOBJ : public BaseClass {
public:
	CARMY_GAMEOBJ();
	~CARMY_GAMEOBJ();

	ClassArray<GAMEOBJ>	Primary;	//������� ����� ������� �������, ��� �����������.
	ClassArray<GAMEOBJ>	Control;	//������ �������� �������� ������� ���.
	ClassArray<GAMEOBJ>	Reserve;	//������ �������.

	void	Init();		//���������� ��� �������� �� ���� ��������.
	
	void	addGR(ClassArray<GAMEOBJ>* pAR, GAMEOBJ* pGR);	//�������� ������ � ������ pAR.
	void	addPrimaryGR(GAMEOBJ* pGR);	//�������� ������ � �������� �����.
	void	addControlGR(GAMEOBJ* pGR);	//�������� ����� � ������ ��������.
	void	addReserveGR(GAMEOBJ* pGR);	//�������� ������ � ������.

	void	addGR(ClassArray<GAMEOBJ>* pAR, GAMEOBJ* pGR, int n);	//�������� ������ pGR � ������ n �� ������� pAR.
	void	addPrimaryGR(GAMEOBJ* pGR, int n);	//�������� ������ � ������ Primary ��� ������� n.
	void	addControlGR(GAMEOBJ* pGR, int n);	//�������� ������ � ������ Control ��� ������� n.
	void	addReserveGR(GAMEOBJ* pGR, int n);	//�������� ������ � ������ Reserve ��� ������� n.

	void	delGrpFromArr(ClassArray<GAMEOBJ>* pAR, int n);	//������ ������ ��� ������� n �� ������� pAR;
	void	delPrimaryGR(int n);	//������ ������ ��� ������� n �� ������� Primary;
	void	delControlGR(int n);	//������ ������ ��� ������� n �� ������� Control;
	void	delReserveGR(int n);	//������ ������ ��� ������� n �� ������� Reserve;
	
	void	clearArr(ClassArray<GAMEOBJ>* pAR);	//�������� ������ pAR;
	void	clearPrimary();	//�������� ������ Primary.
	void	clearControl();	//�������� ������ Control.
	void	clearReserve();	//�������� ������ Reserve.

	int		getARMY_SQ_NUM();	//���-�� ������� � �����.
	int		getARMY_UT_NUM();	//���-�� ������ � �����.

	int		getSQ_NUM(ClassArray<GAMEOBJ>* pAR);	//���-�� ������� � pAR.
	int		getPrimarySQ_NUM();	//���-�� ������� � Primary.
	int		getControlSQ_NUM();	//���-�� ������� � Control.
	int		getReserveSQ_NUM();	//���-�� ������� � Reserve.

	int		getSQ_NUM(ClassArray<GAMEOBJ>* pAR, int n);	//���-�� ������� � ������ n ������� pAR.
	int		getPrimarySQ_NUM(int n);	//���-�� ������� � ������ n ������� Primary.
	int		getControlSQ_NUM(int n);	//���-�� ������� � ������ n ������� Control.
	int		getReserveSQ_NUM(int n);	//���-�� ������� � ������ n ������� Reserve.

	int		getUT_NUM(ClassArray<GAMEOBJ>* pAR);	//���-�� ������ � ������� pAR.
	int		getPrimaryUT_NUM();	//���-�� ������ � ������� Primary.
	int		getControlUT_NUM();	//���-�� ������ � ������� Control.
	int		getReserveUT_NUM();	//���-�� ������ � ������� Reserve.

	int		getUT_NUM(ClassArray<GAMEOBJ>* pAR, int n);	//���-�� ������ � ������ n ������� pAR.
	int		getPrimaryUT_NUM(int n);	//���-�� ������ � ������ n ������� Primary.
	int		getControlUT_NUM(int n);	//���-�� ������ � ������ n ������� Control.
	int		getReserveUT_NUM(int n);	//���-�� ������ � ������ n ������� Reserve.

	SXYF	getGR_XYF(ClassArray<GAMEOBJ>* pAR, int n);	//�������� ���������� � ���������� ������ n �� ������� pAR.
	SXYF	getPrimarySQ_XYF(int n);	//�������� ���������� � ���������� ������ n �� ������� Primary.
	SXYF	getControlSQ_XYF(int n);	//�������� ���������� � ���������� ������ n �� ������� Control.
	SXYF	getReserveSQ_XYF(int n);	//�������� ���������� � ���������� ������ n �� ������� Reserve.

	SAVE(CARMY_GAMEOBJ);
		REG_CLASS(GAMEOBJ);
		REG_AUTO (Primary);
		REG_AUTO (Control);
		REG_AUTO (Reserve);
	ENDSAVE;
};
*/
//----------------------------------------------------------------------------------














