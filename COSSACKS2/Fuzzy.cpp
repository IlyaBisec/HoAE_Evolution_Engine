
#include "stdheader.h"
#include "fuzzy.h"

// CFuzzyRule ////////////////////////////////////////////////////////////
CFuzzyRule::CFuzzyRule(){
	Init();
};
CFuzzyRule::CFuzzyRule(char *name, float min0, float min100, float max100, float max0){
	Init();
	Setup(name, min0, min100, max100, max0);
};
CFuzzyRule::CFuzzyRule(_str& name, float min0, float min100, float max100, float max0){
	Init();
	Setup(name, min0, min100, max100, max0);
};
void	CFuzzyRule::Setup(char *name, float min0, float min100, float max100, float max0) {
	Init();
	m_min0percent   = min0;
	m_min100percent = min100;
	m_max0percent   = max0;
	m_max100percent = max100;
	if (name!=NULL) m_name = name;
};
void	CFuzzyRule::Setup(_str& name, float min0, float min100, float max100, float max0){
	Init();
	m_min0percent   = min0;
	m_min100percent = min100;
	m_max0percent   = max0;
	m_max100percent = max100;
	if (name.str!=NULL) m_name = name.str;
};
CFuzzyRule::~CFuzzyRule(){
};
float	CFuzzyRule::IsTrueToWhatDegree(float datapoint)
{
	if (datapoint < m_min0percent || datapoint > m_max0percent) {
		return(0.0f);
	};

	if (datapoint >= m_min100percent && datapoint <= m_max100percent) {
		return(1.0f);
	};

	if (datapoint >= m_min0percent && datapoint <= m_min100percent) {
		float widthofgreyarea = (float)fabs(m_min0percent - m_min100percent);

		float relativedatapoint = datapoint-m_min0percent;

		return((float)fabs(relativedatapoint / widthofgreyarea));
	};

	if (datapoint >= m_max100percent && datapoint <= m_max0percent) {
		float widthofgreyarea = (float)fabs(m_max0percent - m_max100percent);

		float relativedatapoint = datapoint-m_max0percent;

		return((float)fabs(relativedatapoint / widthofgreyarea));
	};

	assert(0);
	return(0);
};
char*	CFuzzyRule::GetName(void){ 
	return(m_name.str); 
};
void	CFuzzyRule::Init(void){ 
	m_min100percent = m_max100percent = m_min0percent = m_max0percent = 0.0f; 
	m_name = "NoName";
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
float FuzzyAND(CFuzzyRule &rule1, float data1, CFuzzyRule &rule2, float data2)
{
	// a fuzzy and operation is only as true as the minimum trueness of both pairs.
	// in other words, a fuzzy logic operation is only as true as its weakest trueness.
	float true1 = rule1.IsTrueToWhatDegree(data1);
	float true2 = rule2.IsTrueToWhatDegree(data2);

	return(min(true1, true2));
};
float FuzzyOR(CFuzzyRule &rule1, float data1, CFuzzyRule &rule2, float data2)
{
	float true1 = rule1.IsTrueToWhatDegree(data1);
	float true2 = rule2.IsTrueToWhatDegree(data2);

	return(max(true1, true2));
};
//////////////////////////////////////////////////////////////////////////
















































