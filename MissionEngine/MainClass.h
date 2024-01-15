

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
	int		SQ_NUM;		//Кол-во отрядов атакующего.
	int		SQ_K_NUM;	//Кол-во конных отрядов атакующего.
	int		SQ_P_NUM;	//Кол-во пешых отрядов атакующего.
	int		MOBILITY;	//Мобильность армии.	(0-100)
	int		CANNON_NUM;	//Кол-во пушек.

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

	ClassArray<GAMEOBJ>	Primary;	//Главные армии обороны сектора, или наступающих.
	ClassArray<GAMEOBJ>	Control;	//Отряды контроля основных опасных зон.
	ClassArray<GAMEOBJ>	Reserve;	//Отряды резерва.

	void	Init();		//Уничтожить все элементы во всех массивах.
	
	void	addGR(ClassArray<GAMEOBJ>* pAR, GAMEOBJ* pGR);	//Добавить группу в массив pAR.
	void	addPrimaryGR(GAMEOBJ* pGR);	//Добавить группу в основную армию.
	void	addControlGR(GAMEOBJ* pGR);	//Добавить армию в группу контроля.
	void	addReserveGR(GAMEOBJ* pGR);	//Добавить группу в резерв.

	void	addGR(ClassArray<GAMEOBJ>* pAR, GAMEOBJ* pGR, int n);	//Добавить группу pGR в группу n из массива pAR.
	void	addPrimaryGR(GAMEOBJ* pGR, int n);	//Добавить группу в группу Primary под номером n.
	void	addControlGR(GAMEOBJ* pGR, int n);	//Добавить группу в группу Control под номером n.
	void	addReserveGR(GAMEOBJ* pGR, int n);	//Добавить группу в группу Reserve под номером n.

	void	delGrpFromArr(ClassArray<GAMEOBJ>* pAR, int n);	//Удалть группу под номером n из массива pAR;
	void	delPrimaryGR(int n);	//Удалть группу под номером n из массива Primary;
	void	delControlGR(int n);	//Удалть группу под номером n из массива Control;
	void	delReserveGR(int n);	//Удалть группу под номером n из массива Reserve;
	
	void	clearArr(ClassArray<GAMEOBJ>* pAR);	//Очистить массив pAR;
	void	clearPrimary();	//Очистить массив Primary.
	void	clearControl();	//Очистить массив Control.
	void	clearReserve();	//Очистить массив Reserve.

	int		getARMY_SQ_NUM();	//Кол-во отрядов в армии.
	int		getARMY_UT_NUM();	//Кол-во юнитов в армии.

	int		getSQ_NUM(ClassArray<GAMEOBJ>* pAR);	//Кол-во отрядов в pAR.
	int		getPrimarySQ_NUM();	//Кол-во отрядов в Primary.
	int		getControlSQ_NUM();	//Кол-во отрядов в Control.
	int		getReserveSQ_NUM();	//Кол-во отрядов в Reserve.

	int		getSQ_NUM(ClassArray<GAMEOBJ>* pAR, int n);	//Кол-во отрядов в группе n массива pAR.
	int		getPrimarySQ_NUM(int n);	//Кол-во отрядов в группе n массива Primary.
	int		getControlSQ_NUM(int n);	//Кол-во отрядов в группе n массива Control.
	int		getReserveSQ_NUM(int n);	//Кол-во отрядов в группе n массива Reserve.

	int		getUT_NUM(ClassArray<GAMEOBJ>* pAR);	//Кол-во юнитов в массиве pAR.
	int		getPrimaryUT_NUM();	//Кол-во юнитов в массиве Primary.
	int		getControlUT_NUM();	//Кол-во юнитов в массиве Control.
	int		getReserveUT_NUM();	//Кол-во юнитов в массиве Reserve.

	int		getUT_NUM(ClassArray<GAMEOBJ>* pAR, int n);	//Кол-во юнитов в группе n массива pAR.
	int		getPrimaryUT_NUM(int n);	//Кол-во юнитов в группе n массива Primary.
	int		getControlUT_NUM(int n);	//Кол-во юнитов в группе n массива Control.
	int		getReserveUT_NUM(int n);	//Кол-во юнитов в группе n массива Reserve.

	SXYF	getGR_XYF(ClassArray<GAMEOBJ>* pAR, int n);	//Получить координаты и напрвление группы n из массива pAR.
	SXYF	getPrimarySQ_XYF(int n);	//Получить координаты и напрвление группы n из массива Primary.
	SXYF	getControlSQ_XYF(int n);	//Получить координаты и напрвление группы n из массива Control.
	SXYF	getReserveSQ_XYF(int n);	//Получить координаты и напрвление группы n из массива Reserve.

	SAVE(CARMY_GAMEOBJ);
		REG_CLASS(GAMEOBJ);
		REG_AUTO (Primary);
		REG_AUTO (Control);
		REG_AUTO (Reserve);
	ENDSAVE;
};
*/
//----------------------------------------------------------------------------------














