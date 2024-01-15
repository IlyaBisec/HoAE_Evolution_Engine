
//////////////////////////////////////////////////////////////////////////////
class CFuzzyRule : public BaseClass
{
public:
	CFuzzyRule();
	CFuzzyRule(char* name, float min0, float min100, float max100, float max0);
	CFuzzyRule(_str& name, float min0, float min100, float max100, float max0);

	void	Setup(char *name, float min0, float min100, float max100, float max0);
	void	Setup(_str& name, float min0, float min100, float max100, float max0);

	~CFuzzyRule();

	float	IsTrueToWhatDegree(float datapoint);
	char*	GetName(void);

private:
	void Init(void);

	_str  m_name;		   // name of this fuzzy rule
	float m_min100percent; // smallest number considered 100% true
	float m_max100percent; // biggest number considered 100% true                                         
	float m_min0percent;   // smallest number still considered within fuzzy rule
	float m_max0percent;   // biggest number still considered within fuzzy rule

	SAVE(CFuzzyRule);
		REG_AUTO(m_name);
		REG_MEMBER(_float,m_min100percent);
		REG_MEMBER(_float,m_max100percent);
		REG_MEMBER(_float,m_min0percent);
		REG_MEMBER(_float,m_max0percent);
	ENDSAVE;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Fuzzy Operations
//////////////////////////////////////////////////////////////////////////
float FuzzyAND(CFuzzyRule &rule1, float data1, CFuzzyRule &rule2, float data2);
float FuzzyOR(CFuzzyRule &rule1, float data1, CFuzzyRule &rule2, float data2);
//////////////////////////////////////////////////////////////////////////













































