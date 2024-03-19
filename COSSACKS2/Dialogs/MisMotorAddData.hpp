
//*************************************************************************************\\
//			Adaitionl data for mission motor: zoneADD and groupADD.					   \\
//*************************************************************************************\\

class CDEF_DATA : public BaseClass
{
public:
	int		m_CoverDirs;		//������������ �����������.		(E,N,...)
	int		m_CoverType;		//��� ��������� �����������.	(Primary,Control,Reserve,Artillery...)
	int		m_Direction;		//����������� ���������� ������.

	SAVE( CDEF_DATA );
	REG_ENUM	( _index, m_CoverDirs, SDIR_ENUM );
	REG_ENUM	( _index, m_CoverType, SDCT_ENUM );
	REG_MEMBER	( _int,	  m_Direction );
	ENDSAVE;
};
class CDEF_DATA_Array:public ClassArray<CDEF_DATA>{
public:
	virtual int GetExpansionRules(){			
		return 1;						//�������� ������ ��� �����, ��� �����������.
		}
};

//��� �������� ����� ����������.
class CZM_ARS_LV : public ZoneExInfo
{
public:
	CZM_ARS_LV(){
		InfID	= _CZM_ARS_LV;
	}

	CDEF_DATA_Array	m_DIR_TYPE;		//����������� ���������������, � ��� ���������������.

	SAVE( CZM_ARS_LV );
		REG_PARENT	( ZoneExInfo );
		REG_CLASS	( CDEF_DATA );
		REG_AUTO	( m_DIR_TYPE );
	ENDSAVE;
};

//��� �������� ����� ��������������.
class CZM_DRS_LV : public ZoneExInfo
{
public:
	CZM_DRS_LV(){
		InfID	= _CZM_DRS_LV;
	}

	CDEF_DATA_Array	m_DIR_TYPE;		//����������� ���������������, � ��� ���������������.

	SAVE( CZM_DRS_LV );
		REG_PARENT	( ZoneExInfo );
		REG_CLASS	( CDEF_DATA );
		REG_AUTO	( m_DIR_TYPE );
	ENDSAVE;
};

//***************************FOR CAMP**********************************
class CAMP_LV : public ZoneExInfo
{
public:
	CAMP_LV(){
		InfID = _CAMP_LV;
	}
	
	int		Direction;
	int		Status;

	SAVE( CAMP_LV );
		REG_PARENT( ZoneExInfo );
		REG_ENUM( _index, Direction, SDIR_ENUM );
		REG_ENUM( _index, Status, PLAYER_STAT_ENUM );
	ENDSAVE;
};
//*********************************************************************

//grey [03.03.2004]
//*************************** FOR STRATEGIC DEFEND ZONE **********************************
class STRAT_ZONE : public ZoneExInfo
{
public:
	STRAT_ZONE() {
		InfID = _STRAT_ZONE_LV;
	}
	
	int		Param;
	int		State; // �������� ����:  1 - AND (����������� ���������)  , 0 - OR (�� ����������� ��������� - 1 �� 3�)

	SAVE( STRAT_ZONE );
		REG_PARENT( ZoneExInfo );
		REG_MEMBER( _int, Param );
		REG_MEMBER( _int, State );
	ENDSAVE;
};
//*********************************************************************
//grey [03.03.2004]

//grey [04.08.2004]
//*************************** FOR BUILD TOWERS&BLOKHAUSES **********************************
class TOWER_ZONE : public ZoneExInfo
{
public:
	TOWER_ZONE() {
		InfID = _TOWER_ZONE_LV;
	}
	
	int		Weight;  // �������� ����
	int		State;   // �������� ����:  ??? not used jet!

	SAVE( TOWER_ZONE );
		REG_PARENT( ZoneExInfo );
		REG_MEMBER( _int, Weight );
		REG_MEMBER( _int, State );
	ENDSAVE;
};
//*********************************************************************
//grey [04.08.2004]

 //****************************FOR AI***********************************
class CAI_ZONE : public BaseClass
{
public:
	int		Direction;		//����������� �����.
	int		Weight;			//��� ��� ������� �����������.

	SAVE(CAI_ZONE);
		REG_ENUM	( _index, Direction, SDIR_ENUM );
		REG_MEMBER	( _int,	Weight );
	ENDSAVE;
};
class CAI_ZONE_Array : public ClassArray<CAI_ZONE>
{
public:
	virtual int GetExpansionRules(){			
		return 1;						//�������� ������ ��� �����, ��� �����������.
	}
};
class CAI_TOTAL_WAR : public AiImportantPoint
{
public:
	CAI_TOTAL_WAR(){
		InfID = AI_IMPORTANT_POINT;
		Extended = true;
	}
	CAI_ZONE_Array	Dir_Weight;

	SAVE(CAI_TOTAL_WAR);    	
		REG_PARENT	(AiImportantPoint);
		REG_PARENT	(ZoneExInfo);
        REG_CLASS	(CAI_ZONE);
		REG_AUTO	(Dir_Weight);
	ENDSAVE;
};
//*********************************************************************






