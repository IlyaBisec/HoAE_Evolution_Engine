//==================================================================================================================//
#include "stdheader.h"
#include "TestEditor.h"
//==================================================================================================================//
//содержит описание схемы АИ для юнитов

DLLEXPORT void ClearSelection(byte Nat);

class UnitAI:public BaseClass{
public:
	OneObject* OB; //указатель на объект

	//для отладки. сохраняется индекс и серия юнита
	word index;
	word serial;

	int idAction; //тип поведения юнита. 0 - тип не задан. Ничего не делает. Значение по умолчанию.
				  //					 1 - защита конкретной точки.

	class Locate:public BaseFunction{
	public:
		void EvaluateFunction(){
			UnitAI* uai=get_parent<UnitAI>();
			if(uai){				
				ClearSelection(0);
				void AddUnitToSelected(byte NI,OneObject* OB);
				AddUnitToSelected(0,uai->OB);
			}
		}

		SAVE(Locate)
			REG_PARENT(BaseFunction);
		ENDSAVE;
	};
	
	Locate loc;

	//описание FSM поведений юнитов.
	//защите конкретной точки.
	class FSM_point_defence:public BaseClass{
	public:
		int stage;

		SAVE(FSM_point_defence)
			REG_MEMBER(_int, stage);
		ENDSAVE;
	} fsmpdef;

	SAVE(UnitAI)
		REG_AUTO(loc);
		REG_MEMBER(_int,idAction);
		REG_MEMBER(_int,index);	
		REG_MEMBER(_int,serial);
		REG_AUTO(fsmpdef);
	ENDSAVE;
};