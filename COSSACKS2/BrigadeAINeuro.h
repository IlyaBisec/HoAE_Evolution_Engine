#pragma once
//----------------------------------------------------------------------------------------------------------------//
class AutoForm;
//----------------------------------------------------------------------------------------------------------------//
class History
{
public:
	History(float* Inn, int NIn, float* Outt, int NOut);
	~History();

	float* vIn;
	//int vNIn;
	float* vOut;
	//int vNOut;
	History*  Next;
};
//----------------------------------------------------------------------------------------------------------------//
class NeuroInit : public BaseFunction
{
public:
	void EvaluateFunction();
	SAVE(NeuroInit)
		REG_PARENT(BaseFunction);;
	ENDSAVE;
};
//----------------------------------------------------------------------------------------------------------------//
class SaveHistory : public BaseFunction
{
public:
	void EvaluateFunction();
	SAVE(SaveHistory)
		REG_PARENT(BaseFunction);
	ENDSAVE;
};
//----------------------------------------------------------------------------------------------------------------//
class LoadHistory : public BaseFunction
{
public:
	void EvaluateFunction();
	SAVE(LoadHistory)
		REG_PARENT(BaseFunction);
	ENDSAVE;
};
//----------------------------------------------------------------------------------------------------------------//
class CMakeHistory : public BaseFunction
{
public:
	void EvaluateFunction();
	SAVE(CMakeHistory)
		REG_PARENT(BaseFunction);
	ENDSAVE;
};
//----------------------------------------------------------------------------------------------------------------//
class CLearnNeuro : public BaseFunction
{
public:
	void EvaluateFunction();
	SAVE(CLearnNeuro)
		REG_PARENT(BaseFunction);
	ENDSAVE;
};
//----------------------------------------------------------------------------------------------------------------//
class Neuro2AutoForm;
class Neuro : public BaseClass
{
public:
	Neuro();
	~Neuro();
	_str Name;
	void Init(int NIn, int NOut);
	bool Get(float* Inn, float* Outt);

	float Learn(float MaxErr, int MaxIter, float V);
	float Learn(float* Inn, float* Outt, float V);
	
	void Clear();

	void YouAreGood(float LearnV);
	void YouAreBad(float LearnV);

	int vNIn;
	int vNOut;
	ClonesArray< LinearArray<float, _float> > Matr;
	History* RezHistory;

	int NLessons;
	int NGet;
	int NGood;
	int NBad;

	NeuroInit Initial;
	SaveHistory SaveH;
	LoadHistory LoadH;
	float MaxError;
	int NIter;
	float Step;
	CLearnNeuro LearnNeuro;

	SAVE(Neuro)
		REG_AUTO(Name);
		REG_AUTO(Initial);
		REG_AUTO(Matr);
		REG_MEMBER(_int,NLessons);
		REG_MEMBER(_int,NGet);
		REG_MEMBER(_int,NGood);
		REG_MEMBER(_int,NBad);
		REG_MEMBER(_int,vNIn);
		REG_MEMBER(_int,vNOut);
		REG_AUTO(SaveH);
		REG_AUTO(LoadH);
		REG_MEMBER(_float,MaxError);
		REG_MEMBER(_int,NIter);
		REG_MEMBER(_float,Step);
		REG_AUTO(LearnNeuro);
	ENDSAVE;
};
//----------------------------------------------------------------------------------------------------------------//
class Neuro2AutoForm : public BaseClass
{
public:
	Neuro2AutoForm();
	UnitTypeList UnitsType;
	LinearArray<int, _int> InputParamIndex;
	LinearArray<int, _int> OutputCommandIndex;
	ClonesArray<Neuro> NeuroList;

	SAVE(Neuro2AutoForm)
		REG_AUTO(UnitsType);
		REG_AUTO(InputParamIndex);
		REG_AUTO(OutputCommandIndex);
		REG_AUTO(NeuroList);
	ENDSAVE;

	bool AutoFormProcess(AutoForm* A, float V);
};
//----------------------------------------------------------------------------------------------------------------//
class CNeuroStorage : public BaseClass
{
public:
	CNeuroStorage();
	ClonesArray<Neuro2AutoForm> GenoFond;


	float V;
	SAVE(CNeuroStorage)
		REG_AUTO(GenoFond);
		REG_MEMBER(_float01,V);
	ENDSAVE;

	int AutoFormProcess(AutoForm* A);
	bool SetNeroMatrix(AutoForm* A, const char* Name);

	bool IsLoad;
	void LoadFromFile();
};
//----------------------------------------------------------------------------------------------------------------//
/*
class CTempAutoForm : public AutoForm
{
public:
	virtual int GetSituationByIndex(int I);
	int GetMinValueSituationByIndex(int I);
	bool IsDiscreteSituationByIndex(int I);
	Neuro2AutoForm *m_pN2A;
	int m_MatrIndex;
	History *m_pInput;
	History* GetStudies(History *input);
};
*/
//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//
