#pragma once

#ifndef __be_CVariableBase__
#define __be_CVariableBase__

class DIALOGS_API be_CVariableBase : public ReferableBaseClass
{
public:
	be_CVariableBase();
	virtual	~be_CVariableBase();
public:
	virtual	be_CVariableBase*	GetAS_BASE()			{ return (be_CVariableBase*)this; };

	virtual			bool	CheckIfObjectIsGlobal();
	virtual	const	char*	GetThisElementView(const char* LoaclName);
	virtual			DWORD	GetClassMask();

	virtual			void	SetDefName();
	virtual			bool	SetName(const char* _Name);

	virtual			char*	GetName() const;
	virtual			void	SetName_LUA(const char* _Name);

	virtual			bool	GetValueAsSTR(_str& ValueSTR);

	virtual			void	vDraw(){};

	// temporary applying params
	int							RandomCoof;
	virtual			void		SetRandomApplyPower(const int power);
	virtual			int			GetRandomApplyPower();
	virtual			bool		TryRandom();
	
public:
	SAVE(be_CVariableBase);
		SAVE_SECTION(0x00000001);
		REG_AUTO(Name);
		SAVE_SECTION(0x00000002);
		INVISIBLE REG_AUTO(Name);
	ENDSAVE;
};

#endif // __be_CVariableBase__