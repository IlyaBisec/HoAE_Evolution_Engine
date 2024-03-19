
#ifndef __END_MISSION_DATA__
#define __END_MISSION_DATA__

//-------------------------------------

class	CResourceBM : public BaseClass
{
public:
	CResourceBM()  { Init(); return; };
	~CResourceBM() { Init(); return; };

public:
	int		wood;		
	int		food;
	int		stone;
	int		gold;
	int		iron;
	int		coal;
	int		recruit;		//Рекруты для призыва в армию.

	SAVE( CResourceBM ); 
		REG_MEMBER( _int, wood );
		REG_MEMBER( _int, food );
		REG_MEMBER( _int, stone );
		REG_MEMBER( _int, gold );
		REG_MEMBER( _int, iron );
		REG_MEMBER( _int, coal );
		REG_MEMBER( _int, recruit );
	ENDSAVE;

	void	operator=(CResourceBM& RES) {
		wood	= RES.wood;
		food	= RES.food;
		stone	= RES.stone;
		gold	= RES.gold;
		iron	= RES.iron;
		coal	= RES.coal;
		recruit = RES.recruit;
	}

	void	Init() { wood=food=stone=gold=iron=coal=recruit=0; }

	int		GetResource(int res) {
		if(res== 0 )	return	wood;
		if(res== 3 )	return	food;
		if(res== 2 )	return	stone;
		if(res== 1 )	return	gold;
		if(res== 4 )	return	iron;
		if(res== 5 )	return	coal;
		return	recruit;
	}
};
//-------------------------------------

class CRestArmy_LV : public BaseClass
{
public:
	CRestArmy_LV()		{ Init(); };
	~CRestArmy_LV()		{ Init(); };

	int		inTypeID;
	int		inAmount;
	_str	TypeName_sID;		//Строковой идентификатор типа отряда.//grey

	LinearArray<int,_int>	Kills;	//grey 

public:
	SAVE( CRestArmy_LV );
		REG_AUTO  (	      TypeName_sID );
		REG_MEMBER( _int, inTypeID );
		REG_MEMBER( _int, inAmount );
		REG_AUTO  (       Kills );				
	ENDSAVE;

	void	setTypeName( const char* TypeName ) {
		if ( TypeName!=NULL ) {
			TypeName_sID = (char*)TypeName;
		}
	}
	void	Init() { //grey
		Kills.Clear(); 
		TypeName_sID.DString::Clear(); 
		inTypeID=inAmount=0; 
	}
};

//-------------------------------------

class CEndMissionData : public BaseClass
{
public:
	CEndMissionData() { 
		Init(); 
	}
	//CEndMissionData(){
	//	AttackerArmy.Clear();
	//	DefenderArmy.Clear();
	//	AttackerNI = -1;
	//	DefenderNI = -1;
	//	WinnerNI   = -1;
	//	AttackerExperience = 0;
	//	DefenderExperience = 0;
	//	strcpy(m_chFilePath, "Missions\\MissionEngin\\EndMissionData.xml" );
	//};
	~CEndMissionData() { 
		Init();
		/*AttackerArmy.Clear();
		DefenderArmy.Clear();*/
	}
public:
	int							WinnerNI;				//Победившая нация.
	ClassArray<CRestArmy_LV>	AttackerArmy;			//Остатки армии нападающего.
	ClassArray<CRestArmy_LV>	DefenderArmy;			//Остатки армии защитника.
	int							AttackerNI;				//Нация атакующего.
	int							DefenderNI;				//Нация защитника.
	int							AttackerExperience;		//Опыт полученный в бою атакующим.
	int							DefenderExperience;		//Опыт полученный в бою защитником.
	CResourceBM					AttackerRes;			// Resource	
	CResourceBM					DefenderRes;			// Resource	

	char						m_chFilePath[256];

	SAVE( CEndMissionData );
		REG_MEMBER( _int, WinnerNI   );

		REG_CLASS ( CRestArmy_LV );
		REG_CLASS ( CResourceBM  );

		REG_MEMBER( _int, AttackerNI );
		REG_MEMBER( _int, AttackerExperience );
		REG_AUTO  ( AttackerArmy	 );
		REG_AUTO  ( AttackerRes		 ); 

		REG_MEMBER( _int, DefenderNI );
		REG_MEMBER( _int, DefenderExperience );
		REG_AUTO  ( DefenderArmy	 );
		REG_AUTO  ( DefenderRes		 ); 
	ENDSAVE;

public:
	void	operator=(CEndMissionData& EMD) {
		WinnerNI			= EMD.WinnerNI;
		AttackerNI			= EMD.AttackerNI;
		AttackerExperience	= EMD.AttackerExperience;
		AttackerRes			= EMD.AttackerRes; 
		DefenderNI			= EMD.DefenderNI;
		DefenderExperience	= EMD.DefenderExperience;
		DefenderRes			= EMD.DefenderRes; 
	}

	void	Init()
	{
		WinnerNI   = -1;
		AttackerNI = -1;			DefenderNI = -1;
		AttackerExperience = 0;		DefenderExperience = 0;
		AttackerArmy.Clear();		DefenderArmy.Clear();
		AttackerRes.Init();			DefenderRes.Init();

		strcpy(m_chFilePath, "Missions\\MissionEngin\\EndMissionData.xml" );
	}

	bool	LoadXML()			
	{
		// loading from XML
		xmlQuote xml("CEndMissionData");
		xml.ReadFromFile( m_chFilePath );	
		ErrorPager Error;
		return this->Load(xml,this,&Error);
	}

	bool SaveXML()		
	{	
		// saving to XML
		xmlQuote xml("CEndMissionData");
		this->Save(xml,this);
		xml.WriteToFile( m_chFilePath );	
		return true;
	}

};
//-------------------------------------

#endif//__END_MISSION_DATA__