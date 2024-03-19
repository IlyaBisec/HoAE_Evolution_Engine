#include "stdheader.h"
#include "BE_HEADERS.h"

_str	ggClassDescr;
/************************************************************************/
/* Classes for create conditions                                        */
/************************************************************************/
// lvCCondition //////////////////////////////////////////////////////////
lvCCondition::lvCCondition(lvCCondition* cond) {
	if (cond!=NULL){
		InfID=cond->InfID;
		Descr=cond->Descr.pchar();
	};
};
void		lvCCondition::GetCopy(lvCCondition** pCopy){
	*pCopy = new lvCCondition(this);
};

char*		lvCCondition::GetSourceCode(int shift/* = 0*/){
	Descr="Base Condition";
	return Descr.pchar();
};

// lvCAriphmetical ///////////////////////////////////////////////////////
lvCAriphmetical::lvCAriphmetical(lvCAriphmetical* pAriphmetical) : lvCCondition(dynamic_cast<lvCCondition*>(pAriphmetical)) {
	if (pAriphmetical!=NULL) {
		OPER=pAriphmetical->OPER;
		lvCCondition* pCOND1 = NULL;
		if (pAriphmetical->COND1.Get()!=NULL) {
			pAriphmetical->COND1.Get()->GetCopy(&pCOND1);
		};
		COND1.Set(pCOND1);
		lvCCondition* pCOND2 = NULL;
		if (pAriphmetical->COND2.Get()!=NULL) {
			pAriphmetical->COND2.Get()->GetCopy(&pCOND2);
		};
		COND2.Set(pCOND2);
	};
};

void		lvCAriphmetical::GetCopy(lvCCondition** pCopy){
	*pCopy = dynamic_cast<lvCCondition*>(new lvCAriphmetical(this));
};

const char* lvCAriphmetical::GetThisElementView(const char* LocalName){
	Descr = "";

	Descr += "{C 0xFF0000ff}(";
	if (COND1.Get()!=NULL) {
		Descr += (COND1.Get())->GetThisElementView(LocalName);
	}else{
		Descr += "???";
	};

	Descr += "{C 0xFF0000ff}";
	switch(OPER) {
	case 0:
		Descr += " + ";
		break;
	case 1:
		Descr += " - ";
		break;
	case 2:
		Descr += " * ";
		break;
	case 3:
		Descr += " / ";
		break;
	default:
		Descr += " ? ";
		break;
	};
	Descr += "{C 0xFF000000}";

	if (COND2.Get()!=NULL) {
		Descr += (COND2.Get())->GetThisElementView(LocalName);
	}else{
		Descr += "???";
	};
	Descr += " ){C 0xFF000000}";
	
	return	Descr.pchar();
};

int			lvCAriphmetical::GetValue(int time){
	int ret = 0;
	lvCCondition* pCon1 = COND1.Get();
	lvCCondition* pCon2 = COND2.Get();
	if (pCon1!=NULL&&pCon2!=NULL) {
		switch(OPER) {
		case 0:
			ret = pCon1->GetValue(time) + pCon2->GetValue(time);
			break;
		case 1:
			ret = pCon1->GetValue(time) - pCon2->GetValue(time);
			break;
		case 2:
			ret = pCon1->GetValue(time) * pCon2->GetValue(time);
			break;
		case 3:
			if (pCon2->GetValue(time)!=0) {
				ret = pCon1->GetValue(time) / pCon2->GetValue(time);
			}else{
				ret = 0;
			};			
			break;
		default:
			ret  = 0;
			break;
		}
	};
	return ret;
};

void		lvCAriphmetical::SetEdgeXY(int _x0,int _y0,int _x1,int _y1) {
	lvCCondition* pCond = COND1.Get();
	if (pCond!=NULL)	pCond->SetEdgeXY(_x0,_y0,_x1,_y1);
	pCond = COND2.Get();
	if (pCond!=NULL)	pCond->SetEdgeXY(_x0,_y0,_x1,_y1);
};

void		lvCAriphmetical::SetSquardID(DWORD sqID){
	lvCCondition* pCond = COND1.Get();
	if (pCond!=NULL)	pCond->SetSquardID(sqID);
	pCond = COND2.Get();
	if (pCond!=NULL)	pCond->SetSquardID(sqID);
};

int			lvCAriphmetical::Power(){
	lvCCondition* pC1 = COND1.Get();
	lvCCondition* pC2 = COND2.Get();
	if (pC1==NULL||pC2==NULL)	return 0;
	int pover = 0;
	pover += pC1->Power();
	pover += pC2->Power();
	if (pover!=0)	pover=1;
	return pover;
};
char*		lvCAriphmetical::GetSourceCode(int shift/* = 0*/){
	GetThisElementView(NULL);
	return Descr.pchar();
};
// lvCLogical ////////////////////////////////////////////////////////////
lvCLogical::lvCLogical(lvCLogical* pLogical) : lvCCondition(dynamic_cast<lvCCondition*>(pLogical)){
	if (pLogical!=NULL) {
		OPER=pLogical->OPER;
		lvCCondition* pCondI=NULL;
		for (int i=0; i<pLogical->CONDS.GetAmount(); i++){
			if (pLogical->CONDS[i]!=NULL) {
				(pLogical->CONDS[i])->GetCopy(&pCondI);
			};
			CONDS.Add(pCondI);
			pCondI=NULL;
		};
	};
};

void		lvCLogical::GetCopy(lvCCondition** pCopy){
	*pCopy = dynamic_cast<lvCCondition*>(new lvCLogical(this));
};

const char* lvCLogical::GetThisElementView(const char* LocalName){
	Descr = "";	

	switch(OPER) {
	case 0:
		Descr += "{C 0xFF0000ff}And{C 0xFF000000} with ";
		break;
	case 1:
		Descr += "{C 0xFF0000ff}Or{C 0xFF000000} with ";
		break;
	default:
		Descr += "{C 0xFF0000ff}???{C 0xFF000000} with ";
		break;
	};

	int N = CONDS.GetAmount();
	Descr += "{C 0xFF0000ff}";
	Descr += N;
	Descr += "{C 0xFF000000}";
	Descr += " conditions";

	Descr += "{C 0xFF0000ff}(\\{C 0xFF000000}";

	_str strSHIFT;
	strSHIFT="  ";
	for (int i=0; i<SHIFT; i++)	strSHIFT+=" ";
	int CNN=CONDS.GetAmount();
	if (CNN>0) {
		for (int i=0; i<CNN; i++){
			Descr += strSHIFT.pchar();
			if (i!=0)	Descr += " ";
			Descr += CONDS[i]->GetSourceCode(SHIFT+3);
			if (i!=CNN-1) {
                if (OPER==0) Descr += "  {C 0xFF0000ff}&&{C 0xFF000000}\\";
				if (OPER==1) Descr += "  {C 0xFF0000ff}||{C 0xFF000000}\\";
			};
		};
	}else{
		Descr += strSHIFT.pchar();
		Descr += "TRUE";
	};

	Descr += "{C 0xFF0000ff}){C 0xFF000000}";
	
	return	Descr.pchar();
};

int			lvCLogical::GetValue(int time){
	int ret  = 0;
	int N = CONDS.GetAmount();
	if (N>0) {
		ret = CONDS[0]->GetValue(time);
		};
	while (N--) {
		if (OPER==0) {
			ret = ret && CONDS[N]->GetValue(time);
		}else{
			ret = ret || CONDS[N]->GetValue(time);
		};
	};
	return	ret;
};

void		lvCLogical::SetEdgeXY(int _x0,int _y0,int _x1,int _y1){
	int N = CONDS.GetAmount();
	while (N--) {
		if (CONDS[N]!=NULL) {
			CONDS[N]->SetEdgeXY(_x0,_y0,_x1,_y1);
		};
	};
};

void		lvCLogical::SetSquardID(DWORD sqID){
	int N = CONDS.GetAmount();
	while (N--) {
		if (CONDS[N]!=NULL) {
			CONDS[N]->SetSquardID(sqID);
		};
	};
};

int			lvCLogical::Power(){
	int N = CONDS.GetAmount();
	int pover = 0;
	for (int i=0; i<N; i++){
		if (CONDS[i]!=NULL)	pover += CONDS[i]->Power();
		else				{ pover=0; i=N; };
	};
	if (pover!=0)	pover=1;
	return pover;
};
char*		lvCLogical::GetSourceCode(int shift/* = 0*/){
	SHIFT=shift;
	GetThisElementView(NULL);
	return Descr.pchar();
};
// lvCCompare ////////////////////////////////////////////////////////////
lvCCompare::lvCCompare(lvCCompare* pCompare) : lvCCondition(dynamic_cast<lvCCondition*>(pCompare)){
	if (pCompare!=NULL) {
		OPER=pCompare->OPER;
		lvCCondition* pCond1 = NULL;
		if (pCompare->COND1.Get()!=NULL) {
			(pCompare->COND1.Get())->GetCopy(&pCond1);
		};
		COND1.Set(pCond1);
		lvCCondition* pCond2 = NULL;
		if (pCompare->COND2.Get()!=NULL) {
			(pCompare->COND2.Get())->GetCopy(&pCond2);
		};
		COND2.Set(pCond2);
	};
};

void		lvCCompare::GetCopy(lvCCondition** pCopy){
	*pCopy = dynamic_cast<lvCCondition*>(new lvCCompare(this));
};

const char* lvCCompare::GetThisElementView(const char* LocalName){
	Descr = "";
	
	if (COND1.Get()!=NULL) {
		Descr += (COND1.Get())->GetThisElementView(LocalName);
	}else{
		Descr += "???";
	};

	Descr += "{C 0xFF0000ff}";
	switch(OPER) {
	case 4:
		Descr += " == ";
		break;
	case 0:
		Descr += " > ";
		break;
	case 1:
		Descr += " < ";
		break;
	case 2:
		Descr += " >= ";
		break;
	case 3:
		Descr += " <= ";
		break;
	case 5:
		Descr += " != ";
		break;
	default:
		Descr += " ? ";
		break;
	};
	Descr += "{C 0xFF000000}";

	if (COND2.Get()!=NULL) {
		Descr += (COND2.Get())->GetThisElementView(LocalName);
	}else{
		Descr += "???";
	};

	return	Descr.pchar();
};

int			lvCCompare::GetValue(int time){
	bool ret = false;
	lvCCondition* pCon1 = COND1.Get();
	lvCCondition* pCon2 = COND2.Get();
	if (pCon1!=NULL&&pCon2!=NULL) {
		switch(OPER) {
		case 4:
			ret = pCon1->GetValue(time) == pCon2->GetValue(time);
			break;
		case 0:
			ret = pCon1->GetValue(time) >  pCon2->GetValue(time);
			break;
		case 1:
			ret = pCon1->GetValue(time) <  pCon2->GetValue(time);
			break;
		case 2:
			ret = pCon1->GetValue(time) >= pCon2->GetValue(time);
			break;
		case 3:
			ret = pCon1->GetValue(time) <= pCon2->GetValue(time);
			break;
		case 5:
			ret = pCon1->GetValue(time) != pCon2->GetValue(time);
			break;
		default:
			ret  = false;
			break;
		};
	};
	if (ret) {
		return 1;
	}
	return 0;
};

void		lvCCompare::SetEdgeXY(int _x0,int _y0,int _x1,int _y1){
	lvCCondition* pCond = COND1.Get();
	if (pCond!=NULL)	pCond->SetEdgeXY(_x0,_y0,_x1,_y1);
	pCond = COND2.Get();
	if (pCond!=NULL)	pCond->SetEdgeXY(_x0,_y0,_x1,_y1);
};

void		lvCCompare::SetSquardID(DWORD sqID){
	lvCCondition* pCond = COND1.Get();
	if (pCond!=NULL)	pCond->SetSquardID(sqID);
	pCond = COND2.Get();
	if (pCond!=NULL)	pCond->SetSquardID(sqID);
};

int			lvCCompare::Power(){
	lvCCondition* pC1 = COND1.Get();
	lvCCondition* pC2 = COND2.Get();
	if (pC1==NULL||pC2==NULL)	return 0;
	int pover = 0;
	pover += pC1->Power();
	pover += pC2->Power();
	if (pover!=0)	pover=1;
	return pover;
}
char*		lvCCompare::GetSourceCode(int shift/* = 0*/){
	GetThisElementView(NULL);
	return Descr.pchar();
};
// lvCFunctional /////////////////////////////////////////////////////////
lvCFunctional::lvCFunctional(lvCFunctional* pFunctional) : lvCCondition(dynamic_cast<lvCCondition*>(pFunctional)){
	if (pFunctional!=NULL) {
		lvCBaseFunction* pBaseFunction=NULL;
		if (pFunctional->Function.Get()){
			pFunctional->Function.Get()->GetCopy( &pBaseFunction );
		};
		if (pBaseFunction!=NULL) Function.Set(pBaseFunction);
	};
};

void		lvCFunctional::GetCopy(lvCCondition** pCopy){
	*pCopy = dynamic_cast<lvCCondition*>(new lvCFunctional(this));
};

const char* lvCFunctional::GetThisElementView(const char* LocalName){
	Descr = "";
	if (Function.Get()!=NULL) {
		Descr += (Function.Get())->GetThisElementView(LocalName);
	}else{
		Descr += "NO FUNCTION";
	};
	return Descr.pchar();
};
int			lvCFunctional::GetValue(int time){
	lvCBaseFunction* pFun = Function.Get();
	if (pFun) {
		return	pFun->GetValue(time);
	};
	return 0;
};

void		lvCFunctional::SetEdgeXY(int _x0,int _y0,int _x1,int _y1){
	lvCBaseFunction* pBaseFunc = Function.Get();
	if (pBaseFunc!=NULL)	pBaseFunc->SetEdgeXY(_x0,_y0,_x1,_y1);
};

void		lvCFunctional::SetSquardID(DWORD sqID){
	lvCBaseFunction* pBaseFunc = Function.Get();
	if (pBaseFunc!=NULL)	pBaseFunc->SetSquardID(sqID);
};

int			lvCFunctional::Power(){
	if (Function.Get()!=NULL)	return Function.Get()->Power();
	return 0;
};
char*		lvCFunctional::GetSourceCode(int shift/* = 0*/){
	GetThisElementView(NULL);
	return Descr.pchar();
};
// lvCBaseScript /////////////////////////////////////////////////////////
lvCBaseScript::lvCBaseScript(lvCBaseScript* pBaseScript){
	if (pBaseScript!=NULL) {
		InfID=pBaseScript->InfID;
	}else{
		InfID=_lvCBaseScript_;
	};
};

void		lvCBaseScript::GetCopy(lvCBaseScript** pCopy){
	*pCopy = new lvCBaseScript(this);
};

// lvCSubSection /////////////////////////////////////////////////////////
const char* lvCSubSection::GetThisElementView(const char* LocalName){
	return Descr.pchar();
};

// lvCOperation //////////////////////////////////////////////////////////
const char* lvCCondForOper::GetThisElementView(const char* LocalName){
	if (DriveMode()->MainEditType==0) {
		return Descr.pchar();
	}else if (DriveMode()->MainEditType==1) {
		ggClassDescr =		"{R 0xFF0000ff BE_CFO ";
		int adress = (int)(this);
		ggClassDescr += adress;
		ggClassDescr +=	"}";
		ggClassDescr += Descr.pchar();
		return ggClassDescr.pchar();
	};
	return Descr.pchar();
};

int			lvCCondForOper::Process(int time){
	// Check all conditions
	int conN = Condition.GetAmount();
	int	RUN = 1;
	while ( (RUN==1) && conN-- ) {
		if ( Condition[conN]->GetValue(time)==0 ) RUN=0;
		};
	int	complite = RUN;
	if (RUN==1) {
		int oprN = Operation.GetAmount();
		while (oprN--) {
			if (Operation[oprN]->Process(time)==0) complite=0;
		};
		if (g_PMF.USE_CALL_OPER_COND){
			if (myID==-1)	myID=g_PMF.getNEW_SELL<lvCCondForOper>(dynamic_cast<lvCCondForOper*>(this));
			g_PMF.addCALL(myID);
		};
	};
	return complite;
};

lvCCondForOper::lvCCondForOper(lvCCondForOper* pCopy) : lvCBaseScript(dynamic_cast<lvCBaseScript*>(pCopy)){
	if (pCopy!=NULL) {
		Descr = pCopy->Descr.pchar();
		lvCCondition* pCond=NULL;
		for (int i=0; i<pCopy->Condition.GetAmount(); i++){
			pCopy->Condition[i]->GetCopy(&pCond);
			if (pCond!=NULL) Condition.Add(pCond);
			pCond=NULL;
		};
		lvCOperation* pOper=NULL;
		for (int i=0; i<pCopy->Operation.GetAmount(); i++){
			pCopy->Operation[i]->GetCopy(&pOper);
			if (pOper!=NULL) Operation.Add(pOper);
			pOper=NULL;
		};
	};
};

void			lvCCondForOper::GetCopy(lvCBaseScript** pCopy){
	lvCCondForOper* pCFO = new lvCCondForOper(this);
	*pCopy = dynamic_cast<lvCBaseScript*>(pCFO);
};

void			lvCCondForOper::Draw(){
	int N = Condition.GetAmount();
	while (N--) {
		Condition[N]->Draw();
	};
	N = Operation.GetAmount();
	while (N--) {
		Operation[N]->Draw();	
	};
};
void			lvCCondForOper::Restart(){
	int ON = Operation.GetAmount();
	while (ON--) {
		Operation[ON]->first=true;
	};
};
char*			lvCCondForOper::GetSourceCode(int shift/* = 0*/){
	ggClassDescr =	"{C 0xFF0000ff}  << {C 0xFFff0000}";
	ggClassDescr +=	Descr.pchar();
	ggClassDescr += " {C 0xFF0000ff}>>\\";
	ggClassDescr += "{C 0xFF0000ff}if ({C 0xFF000000}\\";
	int CNN=Condition.GetAmount();
	if (CNN>0) {
		for (int i=0; i<CNN; i++){
			ggClassDescr += "      ";
			ggClassDescr += Condition[i]->GetSourceCode(6);
			if (i!=CNN-1) {
				ggClassDescr += "  {C 0xFF0000ff}&&{C 0xFF000000}\\";
			};
		};
	}else{
		ggClassDescr += "      TRUE";
	};
	ggClassDescr += "\\    {C 0xFF0000ff})\\{C 0xFF000000}";
	ggClassDescr += " {C 0xFF0000ff}BEGIN\\{C 0xFF000000}";
	int OPN=Operation.GetAmount();
	if (OPN>0) {
		for (int i=0; i<OPN; i++){
			if (i==0)	ggClassDescr += "   ";
			else		ggClassDescr += "    ";
			ggClassDescr += Operation[i]->GetSourceCode();
			ggClassDescr += "\\";
		};
	}else{
		ggClassDescr += "  {C 0xFFff0000}EMPTY OPERATION LIST{C 0xFF000000}";
	}
	ggClassDescr +=	"\\  {C 0xFF0000ff}END";
	return ggClassDescr.pchar();
};
void			lvCCondForOper::Prepare_lua(){
	int N = Condition.GetAmount();
	while (N--) {
		Condition[N]->Prepare_lua();
	};
};
//////////////////////////////////////////////////////////////////////////
void	REG_BE_CONDITION_class(){
	bool	mastREG = true;
	if (true/*mastREG*/) {
		mastREG=false;
		REG_CLASS(lvCBaseScript);
		REG_CLASS(lvCSubSection);
		REG_CLASS(ClassArray<lvCBaseScript>);
		REG_CLASS(lvCCondition);
		REG_CLASS(lvCAriphmetical);
		REG_CLASS(lvCLogical);
		REG_CLASS(lvCCompare);
		REG_CLASS(lvCFunctional);
		REG_CLASS(lvCCondForOper);
	};
};
//////////////////////////////////////////////////////////////////////////






















