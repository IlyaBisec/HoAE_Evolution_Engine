#ifndef __xml_Quote_h__
#define __xml_Quote_h__
#include "dynarray.h"
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
	const char* GetQuoteName();
private:
	DString QuoteName;
	DynArray<xmlQuote*> SubQuotes;
	DString Body;
	DString XMLSource;
};
#endif __xml_Quote_h__