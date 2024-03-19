/*****************************************************************************/
/*	File:	rsDSSParser.h
/*	Desc:	Parser of the DSS files
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.04.2003
/*****************************************************************************/
#ifndef __RSDSSPARSER_H__
#define __RSDSSPARSER_H__

namespace dss{
/*****************************************************************************/
/*	Class:	DSDefinition
/*****************************************************************************/
class DSDefinition 
{	
public:
	virtual const char*			GetStateName()	const			{ return "Unknown"; }
	virtual const char*			GetValType()	const			{ return "Unknown"; }

	virtual int					GetStateDevID() const			{ return 0; }
	virtual void				Apply( const DeviceState& ds )	{}

	virtual bool				ParseValue( const char* strVal, DeviceState& ds ) { return false; }
	virtual bool				IsRS() const { return false; }
	virtual const				char* ValToStr( DWORD val ) const { return "Unknown"; }

}; // class DSDefinition

/*****************************************************************************/
/*	Class:	RSDefinition
/*****************************************************************************/
class RSDefinition : public DSDefinition
{
public:
	virtual void				Apply( const DeviceState& ds );
	virtual bool				IsRS() const { return true; }

}; // class RSDefinition

/*****************************************************************************/
/*	Class:	TSSDefinition
/*****************************************************************************/
class TSSDefinition : public DSDefinition
{
public:
	virtual void				Apply( const DeviceState& ds );
	virtual bool				IsRS() const { return false; }

}; // class TSSDefinition

/*****************************************************************************/
/*	Class:	BoolValue
/*	Desc:	Boolean device state value
/*****************************************************************************/
class BoolValue
{
public:
	static bool ParseValue( const char* strVal, DeviceState& ds ) 
	{ 
		if (!strnicmp( strVal, "true", 4 ) || strVal[0] == '1')
		{
			ds.value = 1;
		}
		else if (!strnicmp( strVal, "false", 5 ) || strVal[0] == '0')
		{
			ds.value = 0;
		}
		else
		{
			return false;
		}
		return true; 
	}

	static const char* ValToStr( DWORD val )
	{
		if (val == 0) return "False";
		return "True";
	}
}; // class BoolValue

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
inline float DW2F( DWORD f ) { return *((float*)&f); }
/*****************************************************************************/
/*	Class:	FloatValue
/*	Desc:	Float value 
/*****************************************************************************/
class FloatValue 
{
public:
	static bool ParseValue( const char* strVal, DeviceState& ds ) 
	{ 
		float val;
		int nV = sscanf( strVal, "%f", &val );
		if (nV == 0) return false;
		ds.value = F2DW( val );
		return true;
	}

	static const char* ValToStr( DWORD val )
	{
		static char buf[256];
		float fval = DW2F( val );
		sprintf( buf, "%f", fval );
		return buf;
	}
}; // class FloatValue

/*****************************************************************************/
/*	Class:	HexValue
/*	Desc:	Hex devstate value 
/*****************************************************************************/
class HexValue 
{
public:
	static bool ParseValue( const char* strVal, DeviceState& ds ) 
	{ 
		DWORD val;
		int nV = sscanf( strVal, "%x", &val );
		if (nV == 0) return false;
		ds.value = val;
		return true;
	}

	static const char* ValToStr( DWORD val )
	{
		static char buf[256];
		sprintf( buf, "0x%X", val );
		return buf;
	}
}; // class HexValue

/*****************************************************************************/
/*	Class:	IntValue
/*	Desc:	Integer device state value
/*****************************************************************************/
class IntValue
{
public:
	static bool ParseValue( const char* strVal, DeviceState& ds ) 
	{ 
		int val;
		int nV = sscanf( strVal, "%d", &val );
		if (nV == 0) return false;
		ds.value = val;
		return true; 
	}

	static const char* ValToStr( DWORD val )
	{
		static char buf[256];
		sprintf( buf, "0x%d", val );
		return buf;
	}
}; // class IntValue

const int c_DSSHashSize		= 17389;
const int c_MaxDSSNum		= 256;
const int c_MaxEnumValNum	= 1024;

/*****************************************************************************/
/*	Class:	DSSParser
/*	Desc:	Reads DeviceStateSet script in the XML format
/*****************************************************************************/
class DSSParser : public Singleton<DSSParser>, public XMLParser
{
	DSBlock*				m_pDSS;
	PointerHash<DSDefinition, 
				c2::string,
				c_DSSHashSize, 
				c_MaxDSSNum>		m_StateDesc;

	enum ParsingState
	{
		psUnknown		= 0,
		psIdle			= 1,
		psInsideDSS		= 2,
		psParsingRS		= 3,
		psParsingTSS	= 4
	};

	ParsingState				m_State;
	int							m_CurTexStage;
	c2::string					m_CurTag;
	c2::string					m_CurValue;

public:
				DSSParser		();
	virtual		~DSSParser		();
	
	bool		ParseFile		( const char* fname,	DSBlock& dss );
	bool		ParseBuffer		( char* buffer,			DSBlock& dss );

	const char*	GetStateName	( const DeviceState& ds, bool isRS );
	const char* GetStateValue	( const DeviceState& ds, bool isRS );

	void		OnOpen			( const char* tag );
	void		OnClose			( const char* tag );
	void		OnValue			( const char* value );
	void		OnAttribute		( const char* name, const char* value );
	int			StateFromDevID	( int devID );
	bool		GetStateDevID	( int defIdx, int& devID, bool& isRS );
	void		Init			();

protected:
	void			AddStateDef	( DSDefinition* pDS );
	DSDefinition*	FindStateDefinition( const char* stateName );

private:
	void		Reset();
	void		SpewError		( const char* errMsg );
	void		AddDeviceState	();

}; // class DSSParser 

struct EnumVal
{
	const char*		name;
	DWORD			val;
};

#define REG_DS(CName)	AddStateDef(new CName);

#define REGENUM(EName) const EnumVal __##EName##Enum[] = {
								
#define ENVAL(VName,Val) {#VName, Val},		
#define ENDENUM(EName)	{NULL,0} }; class EName##Value { public:\
							static bool ParseValue( const char* strVal, DeviceState& ds )	\
								{ int val;  int cEVal = 0;									\
									const char* pS = __##EName##Enum[0].name;				\
									while (pS){ if (!strnicmp( pS, strVal, strlen( pS ) ))	\
									{ ds.value = __##EName##Enum[cEVal].val; return true; }	\
										cEVal++; pS = __##EName##Enum[cEVal].name; }		\
									if (sscanf( strVal, "%d", &val) == 1)					\
									{ ds.value = val; return true; }						\
										return false; }										\
							static const char* ValToStr( DWORD val ){						\
								int cEVal = 0; const char* pS = __##EName##Enum[0].name;	\
								while (pS){ if (val == __##EName##Enum[cEVal].val)			\
												{ return __##EName##Enum[cEVal].name; }		\
											cEVal++; pS = __##EName##Enum[cEVal].name; }	\
										return "Unknown";}											\
						};


#define DEVSTATE(SName,DevID,VClass)	virtual const char*	GetStateName() const \
										{ return #SName; }						 \
										virtual const char*	GetValType() const	 \
										{ return #VClass; }						 \
										virtual int	GetStateDevID() const		 \
										{ return DevID; }						 \
										virtual bool ParseValue( const char* strVal, DeviceState& ds )	 \
										{ return VClass::ParseValue( strVal, ds ); }\
										virtual const char* ValToStr( DWORD val ) const { return VClass::ValToStr( val ); }

}; // namespace dss

#include "rsDeviceStatesDX8.h"

#endif // __RSDSSPARSER_H__