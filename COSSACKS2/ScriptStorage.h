#ifndef __SCRIPT_STORAGE__
#define	__SCRIPT_STORAGE__

class be_CScriptStore	: public ReferableBaseClass
{
public:
	be_CScriptStore();
	be_CScriptStore(char* name);
	void	init(char* name=NULL);
	~be_CScriptStore();

	virtual bool	CheckIfObjectIsGlobal() { return true; };
	virtual	DWORD	GetClassMask();

	be_CBaseUnaryStatement		MAIN_SCRIPT;
	void						CALL_ALL_SCREPTS();

	void						CLEAR();

	SAVE(be_CScriptStore);
		REG_BASE(ReferableBaseClass);
		INVISIBLE REG_AUTO(Name);
	SAVE_SECTION(0x00000001);
		REG_LOADSAVE("missions\\","script.xml");
	SAVE_SECTION(0xFFFFFFFF);
		REG_AUTO(MAIN_SCRIPT);
	ENDSAVE;
};

extern	be_CScriptStore	g_beSCRIPT_STORE;

void	be_REG_CLASS_ScriptsStore();

#endif//__SCRIPT_STORAGE__