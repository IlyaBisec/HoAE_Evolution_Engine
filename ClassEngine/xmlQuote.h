//----------------------------------------------------------------------------------------------------------------//
#ifndef __xml_Quote_h__
#define __xml_Quote_h__
//----------------------------------------------------------------------------------------------------------------//
#include "..\ClassEngine\dynarray.h"
//#include <pool.hpp>
//----------------------------------------------------------------------------------------------------------------//
class DIALOGS_API CAttribute
{
public:
	CAttribute(const char* name, const char* body);
	int GetInt();
	const char* GetString();
	const char* GetName();
private:
	DString m_text;
	DString m_name;
};


//----------------------------------------------------------------------------------------------------------------//
class DIALOGS_API xmlQuote
{
public:
	xmlQuote();
	~xmlQuote();
	xmlQuote(char* QuoteName);

	//xmlQuote* get(char* id);
	int Get_int();
	//int Get_float();
	const char* Get_string();
	
	int Load(char* XMLSource);
	int GetXMLSource(DString* Source,int shift=0);
	bool ReadFromFile(char* FilePath);
	void WriteToFile(char* FilePath);
		
	void Assign_int(int v);
	//void Assign_float(float v);
	void Assign_string(char* s);
	void Assign_string(DString& s);

	int AddSubQuote(xmlQuote* SubQ);
	int AddSubQuote(char* quotename, char* body);
	int AddSubQuote(char* quotename, int body);
	xmlQuote* GetSubQuote(int Index);
	xmlQuote* GetSubQuote(char* SubQuoteName);
	int GetNSubQuotes();

	void AddAtribute(const char *attributeName, const char *attributeBody);
	int GetNAttribute();
	int GetAttributeInt(int index);
	int GetAttributeInt(const char *name);
	const char* GetAttributeStr(int index);
	const char* GetAttributeStr(const char *name);

	void DelSubQuote(int idx);
	xmlQuote* RemoveSubQuote(int idx);

	const char* GetQuoteName();
	void SetQuoteName(const char* Name);
	void ClearAll();
private:
	DString QuoteName;
	DynArray<xmlQuote*> SubQuotes;
	DString Body;
	DString XMLSource;
	DynArray<CAttribute*> m_attribute;
};
_inline xmlQuote* new_xmlQuota(){
	xmlQuote* x=(xmlQuote*)POOL.Allocate(sizeof(xmlQuote));	
	memset(x,0,sizeof xmlQuote);
	return x;
}
_inline xmlQuote* new_xmlQuote(char* s){
	xmlQuote* x=new_xmlQuota();
	x->SetQuoteName(s);
	return x;
}
_inline void delete_xmlQuote(xmlQuote* x){
	x->ClearAll();
	POOL.Free((BYTE*)x,sizeof xmlQuote);
}
//----------------------------------------------------------------------------------------------------------------//
#endif __xml_Quote_h__
//----------------------------------------------------------------------------------------------------------------//