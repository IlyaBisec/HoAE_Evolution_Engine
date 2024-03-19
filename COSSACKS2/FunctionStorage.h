#ifndef	__FUNCTION__STORAGE__ 
#define	__FUNCTION__STORAGE__

template<class T>
class	be_CFN_ARRAY		: public ClassArray<be_CBaseParamFunction>
{
public:
	virtual bool			ForceSimplification() { return true; };

	virtual	T*		GET(const char* fn_name) const;

	virtual	void	ADD_FN( const char* fn_name );
	virtual	void	ADD_FN_PARAM( const char* fn_name, const char* par_type, const char* par_name, const bool local );
};

// LUA Function Storage
class	be_CLUA_FN_STORE	: public be_CFN_ARRAY<be_CBaseLUAFunction>
{
public:
	virtual	void	ADD_FN( const char* fn_name );
	virtual	void	ADD_FN_PARAM( const char* fn_name, const char* par_type, const char* par_name, const bool local );	

	SAVE(be_CLUA_FN_STORE);
		REG_LOADSAVE("Mission\\","LUA_FN.xml");
	ENDSAVE;
};
void	g_be_LUA_ADD_FN( const char* fn_name );
void	g_be_LUA_ADD_FN_PARAM( const char* fn_name, const char* par_type, const char* par_name );

// Module Function Storage
class	be_CMODULE_FN_STORE	: public be_CFN_ARRAY<be_CParamFunction>
{
public:
	
	SAVE(be_CMODULE_FN_STORE);
		REG_LOADSAVE("Mission\\","MODULE_FN.xml");
	ENDSAVE;
};

// TOTAL Function Storage
class	be_CTOTAL_FN_STORE	: public BaseClass
{
public:
	virtual	bool			AskParentForUsingExpansionClass(char* MemberName,char* ClassName);
			void			CLEAR();
public:
	// LUA
	be_CLUA_FN_STORE		LUA_FN_STORE;
	void					LUA_FN_CLEAR();
	int						GetFileList(ClassArray<_str>& FilesList);
	// MODULE
	be_CMODULE_FN_STORE		MODULE_FN_STORE;
	void					MODUL_FN_CLEAR();
public:
	SAVE(be_CTOTAL_FN_STORE);
		REG_LOADSAVE("Mission\\","TOTAL_FN.xml");
		NOSAVE REG_AUTO(LUA_FN_STORE);
		REG_AUTO(MODULE_FN_STORE);
	ENDSAVE;
};

extern	be_CTOTAL_FN_STORE	g_BE_FN_STORE;
//////////////////////////////////////////////////////////////////////////
void be_REG_CLASS_FnStorage();

#endif//__FUNCTION__STORAGE__