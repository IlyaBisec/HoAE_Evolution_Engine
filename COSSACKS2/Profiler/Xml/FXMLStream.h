#ifndef FXMLStreamH
#define FXMLStreamH

#include "FXMLCommon.h"
//-------------------------------------------------------------------------------------
class f_xmlQuote {
	friend class FXMLEncoder;
	friend class FXMLDecoder;
protected:
	static f_FOutStream TmpOutStream;
	static f_FOutStream TextCollector;
	static f_FInStream InStream;
	static f_FOutStream OutStream;

	static void StoreTextCollector(void *userData);

	static void xml_handlerStartElement(void *userData, const char *name, const char **attr);
	static void xml_handlerEndElement(void *userData, const char *name);
	static void xml_handlerText(void *userData, const char *s, int len);

	e_fxmlId Id;
	union {
		char* Name;
		char* Text;
	};
	f_xmlQuote* Next;

public:
	struct FAttribute {
		char* Name;
		char* Value;
		FAttribute* Next;
		
		FAttribute(const char* name, const char* value)
		{
			assert(name && value);
			Name = new char[strlen(name)+1];
			Value = new char[strlen(value)+1];
			strcpy(Name, name);
			strcpy(Value, value);
			Next = NULL;
		}
		~FAttribute(void)
		{
			delete[] Name;
			delete[] Value;
			if(Next) delete Next;
		}

	} *Attributes;
	unsigned AttributesNumber;
	//----------------------------------------------------------------------------------
	// - f_xmlQuote internal methods
	//----------------------------------------------------------------------------------
protected:
	inline void SetName(const char* _Name)
	{
		size_t _NameLen = strlen((char*)_Name)+1;
		if(Name) delete[] Name;
		Name = new char[_NameLen];
		memcpy(Name, _Name, _NameLen);
	}
	inline void SetName(const char* _Name, unsigned _NameLen)
	{
		if(Name) delete[] Name;
		Name = new char[_NameLen+1];
		memcpy(Name, _Name, _NameLen);
		Name[_NameLen] = 0;
	}

	inline void SetText(const char* _Text)
	{
		size_t _TextLen = strlen((char*)_Text) + 1;
		if(Text) delete[] Text;
		Text = new char[_TextLen];
		memcpy(Text, _Text, _TextLen);
	}

	inline void SetText(const char* _Text, unsigned _TextLen)
	{
		if(Text) delete[] Text;
		Text = new char[_TextLen+1];
		memcpy(Text, _Text, _TextLen);
		Text[_TextLen] = 0;
	}

	f_xmlQuote* GetChild(e_fxmlId id, int num = 0)
	{
		f_xmlQuote* t = this;
		if(t->Id != idTag) return NULL;

		int ChildsNumber = 0;
		int d = 1;
		while(d > 0)
		{
			if((t = t->Next) == NULL) return NULL;
			if(d == 1 && t->Id == id && ChildsNumber++ >= num) 
				return t;

			if(t->Id == idTag) d++;
			else
			if(t->Id == idTerminator) d--;
		}
		return NULL;
	}

	f_xmlQuote* GetChild(char* _Name)
	{
		f_xmlQuote* t = this;
		if(t->Id != idTag) return NULL;

		int d = 1;
		while(d > 0)
		{
			if((t = t->Next) == NULL) return NULL;
			if(t->Id == idTag) 
			{
				if(d == 1 && t->Name && strcmp((char*)t->Name, (char*)_Name) == 0) 
					return t;
				d++;
			}
			else
			if(t->Id == idTerminator) d--;
		}
		return NULL;
	}

	f_xmlQuote* GetLastChildTag(void)
	{
		f_xmlQuote* t = this;
		f_xmlQuote* tLast = this;
		if(t->Id != idTag) return NULL;

		int d = 1;
		while(d > 0)
		{
			if((t = t->Next) == NULL) break;
			if(d == 1 && t->Id == idTag) tLast = t;

			if(t->Id == idTag) d++;
			else
			if(t->Id == idTerminator) d--;
		}
		return tLast;
	}

	f_xmlQuote* GetLastChild(void)
	{
		f_xmlQuote* t = this;
		f_xmlQuote* tLast = this;
		if(t->Id != idTag) return NULL;

		int d = 1;
		while(d > 0)
		{
			tLast = t;
			if((t = t->Next) == NULL) break;

			if(t->Id == idTag) d++;
			else
			if(t->Id == idTerminator) d--;
		}
		return tLast;
	}

	inline f_xmlQuote* AddElement(e_fxmlId Id = idNone)
	{
		f_xmlQuote* t = new f_xmlQuote(Id);
		t->Next = Next;
		Next = t;
		return Next;
	}

	// - delete all children in the pair 
	bool Collapse(void)
	{
		if(Id == idTag)
		{
			f_xmlQuote* t1 = this->Next;
			f_xmlQuote* t2 = GetChild(idTerminator);

			while(t1 && t1 != t2)
			{
				f_xmlQuote* tmp = t1->Next;
				t1->Id = idNone;
				delete t1;
				t1 = tmp;
			} 
			Next = t2;
			return true;
		}
		return false;
	}

	bool LoadFromMemory(char* InData, unsigned InLen);
	bool LoadFromPackedMemory(char* InData, unsigned InLen);
	bool SaveToFile(char* OutFileName);
	bool SaveToStream(void);
	bool SaveToPackedFile(char* OutFileName);

	void ProcessAttributes(void);
	void ProcessSave(f_xmlQuote** p_xmlCurr, int* p_Depth);
public:
	static unsigned RawLen;
	static unsigned PackLen;
	static bool isHex;
	//----------------------------------------------------------------------------------
	// - f_xmlQuote public interface
	//----------------------------------------------------------------------------------
	bool Load(char* XMLSource, int InLen = -1)
	{
		if(*XMLSource == 'p')
		{
			assert(InLen > 0);
			return LoadFromPackedMemory(XMLSource, InLen);
		}
		if(*XMLSource != '<')
		{
			FXML_ERROR("Unknown XML file type! Must be '<'\n");
			return false;
		}
		if(InLen == -1)
			InLen = (int)strlen(XMLSource);
		return LoadFromMemory(XMLSource, InLen);
	}

	bool ReadFromFile(char* InFileName)
	{
		if(!InStream.reload(InFileName))
		{
			FXML_ERROR("No such file: %s\n", InFileName);
			return false;
		}
		return Load((char*)InStream.Data, InStream.Size);
	}

	bool WriteToFile(char* FilePath)
	{
		if(Id != idTag) return false;
		SaveToFile(FilePath);
		return true;	
	}

	bool WriteToPackedFile(char* FilePath)
	{
		if(Id != idTag) return false;
		return SaveToPackedFile(FilePath);	
	}

	const char* Get_string(void)
	{
		if(Id != idTag) return NULL;
		f_xmlQuote* t = GetChild(idText);
		if(t == NULL) return NULL;
		return (char*)t->Text;
	}

	int Get_int()
	{
		if(Id != idTag) return 0;
		const char* str = Get_string();
		if(str == NULL) return 0;
		return atoi(str);
	}

	void Assign_string(char* s)
	{
		if(Id != idTag) return;

		f_xmlQuote* t = GetChild(idText);
		if(t == NULL) t = AddElement(idText);
		t->SetText(s);
	}

	void Assign_int(int v)
	{
		char buf[64];
		if(isHex)
			sprintf(buf, "0x%x", v);
		else
			sprintf(buf, "%u", v);
		Assign_string(buf);
	}

	void AssignName(const char* Name)
	{
		SetName(Name);
	}

	f_xmlQuote* AddSubQuote(f_xmlQuote* t1)
	{
		f_xmlQuote* tLast = GetLastChild();
		f_xmlQuote* t1Last = t1->GetChild(idTerminator);
		t1Last->Next = tLast->Next;
		tLast->Next = t1;
		return t1;
	}

	f_xmlQuote* AddSubQuote(char* quotename, char* body)
	{
		f_xmlQuote* t = new f_xmlQuote;
		t->SetName(quotename);
		t->Assign_string(body);
		return AddSubQuote(t);
	}

	f_xmlQuote* AddSubQuote(char* quotename, int body)
	{
		f_xmlQuote* t = new f_xmlQuote;
		t->SetName(quotename);
		t->Assign_int(body);
		return AddSubQuote(t);
	}

	f_xmlQuote* AddSubQuote(char* quotename)
	{
		f_xmlQuote* t = new f_xmlQuote;
		t->SetName(quotename);
		return AddSubQuote(t);
	}

	f_xmlQuote* AddAttribute(const char* name, const char* value)
	{
		FAttribute** pAttr = &Attributes;
		while(*pAttr)
		{
			if(strcmp((*pAttr)->Name, name) == 0)
			{
				assert((*pAttr)->Value);
				delete[] (*pAttr)->Value;
				(*pAttr)->Value = new char[strlen(value)+1];
				strcpy((*pAttr)->Value, value);
				return this;
			}
			pAttr = &((*pAttr)->Next);
		}

		*pAttr = new FAttribute(name, value);
		AttributesNumber++;
		return this;
	}

	f_xmlQuote* AddAttribute(const char* name, const int value)
	{
		assert(name);
		static char buf[32];
		if(isHex)
			sprintf(buf, "0x%08x", value);
		else
			sprintf(buf, "%u", value);
		return AddAttribute(name, buf);
	}

	char* GetAttributeValue(const char* Name)
	{
		assert(Name);
		FAttribute* Attr = Attributes;
		while(Attr)
		{
			assert(Attr->Name);
			assert(Attr->Value);
			if(strcmp(Attr->Name, Name) == 0)
				return Attr->Value;
			Attr = Attr->Next;
		}
		return NULL;
	}

	f_xmlQuote* GetSubQuote(int Index)
	{
		return GetChild(idTag, Index);
	}

	f_xmlQuote* GetSubQuote(char* SubQuoteName)
	{
		return GetChild(SubQuoteName);
	}

	int GetNSubQuotes()
	{
		f_xmlQuote* t = this;
		if(t->Id != idTag) return NULL;

		int ChildsNumber = 0;
		int d = 1;
		while(d > 0)
		{
			if((t = t->Next) == NULL) break;

			if(t->Id == idTag) 
			{
				if(d == 1) ChildsNumber++;
				d++;
			}
			else
			if(t->Id == idTerminator) d--;
		}
		return ChildsNumber;
	}

	const char* GetQuoteName()
	{
		if(Id != idTag) return NULL;
		return (char*)Name;
	}

	//----------------------------------------------------------------------------------
	// - constructors/destructors
	//----------------------------------------------------------------------------------
protected:

	// - simple construct the object
   f_xmlQuote(e_fxmlId id)
   {
		Id = id;
		Name = NULL;
		Attributes = NULL;
		AttributesNumber = 0;
		Next = NULL;
   }

public:

	// - construct the pair Tag->Terminator
	f_xmlQuote()
   {
		Id = idTag;
		Next = new f_xmlQuote(idTerminator);
		Name = NULL;
		Attributes = NULL;
		AttributesNumber = 0;
   }

   ~f_xmlQuote()
   {
		if(Collapse())
			delete Next;
		if(Name) delete[] Name;
		if(Attributes) delete Attributes;
   }
};

#endif
