#ifndef __XMLQUOTE_HPP__
#define __XMLQUOTE_HPP__
//================================================================================================================//
//----------------------------------------------------------------------------------------------------------------//
CAttribute::CAttribute(const char* name, const char* body)
{
	m_text=body;
	m_name=name;
}
int CAttribute::GetInt()
{
	int rez=0;
	if(m_text.L)
	{
		rez=atoi(m_text.pchar());
	}
	return rez;
}
const char* CAttribute::GetString()
{
	return m_text.pchar();
}
const char* CAttribute::GetName()
{
	return m_name.pchar();
}
//----------------------------------------------------------------------------------------------------------------//
xmlQuote::xmlQuote()
{}
xmlQuote::xmlQuote(char* QN)
{
	QuoteName.Clear();
	QuoteName.Add(QN);
}
xmlQuote::~xmlQuote()
{
	ClearAll();
}
int xmlQuote::Get_int()
{
	int rez=0;
	if(Body.L)
	{
		rez=atoi(Body.pchar());
	}
	return rez;
}
void xmlQuote::DelSubQuote(int idx){
	delete_xmlQuote(SubQuotes[idx]);
	SubQuotes.Del(idx,1);
}
xmlQuote* xmlQuote::RemoveSubQuote(int idx){
	xmlQuote* x=SubQuotes[idx];
	SubQuotes[idx]=NULL;
	SubQuotes.Del(idx,1);
	return x;
}
const char* xmlQuote::Get_string()
{
	return Body.pchar();
}
int xmlQuote::Load(char* XML)
{
	if(XML&&XML[0])
	{
		int pos=0;
		QuoteName.Clear();
		char* fr=strstr(XML,"<");//Текст после <
		char* en=NULL;
		if(fr&&*(fr+1)!='/')
		{
			en=strstr(fr,">");//Текст после >
			if(en)
			{
				int sz=en-fr;//Текст в <>
				QuoteName.Allocate(sz);
				memmove(QuoteName.pchar(),fr+1,sz-1);
				QuoteName.pchar()[sz-1]='\0';
				QuoteName.L=sz-1;
				while(true)
				{
					//en=strstr(fr+pos,">");
					char* nm = strstr(en+1+pos,"<");//Следующее имя? Подзаголовок то есть
					if(nm)//Если такой подзаголовок есть, продолжаем
					{
						int ds=nm-en-pos+1;
						bool havedata=false;
						char* ffr=NULL;
						char* ffl=NULL;
						for(int i=1;i<(ds-1);i++)
						{
							if(en[i]!=10&&en[i]!=13&&en[i]!=32&&en[i]!=9)
							{
								havedata=true;
								if(!ffr)
									ffr=&en[i];
								ffl=&en[i];
							}
						}
						if(havedata)
						{
							int r=ffl-ffr+1;
							Body.Clear();
							Body.Allocate(r+1);
							memmove(Body.pchar(),ffr,r);
							Body.pchar()[r]='\0';
							Body.L=r;
						}
						if(*(nm+1)=='/')
						{
							if(strstr(nm+2,QuoteName.pchar())==(nm+2))
							{
								//OK
								char* lp=strstr(nm+2,">");
								if(lp)
								{
									if(Body.pchar()[0])
									{
										Body.Replace0("&lt","<");
										Body.Replace0("&gt",">");
									}
									return lp-XML+1;
								}
								else
									return -1;
							}
							else
								return -1;
						}
						else
						{   
							xmlQuote* tmp=new_xmlQuota();
							SubQuotes.Add(tmp);
							int p=tmp->Load(en+1+pos);
							if(p==-1){
								return -1;
							}else{
								pos+=p;
							}							
						}
					}else 
						return -1;
				}
			}else 
				return -1;
		}else 
			return -1;
	}
	if(Body.pchar()[0])
	{
		Body.Replace0("&lt","<");
		Body.Replace0("&gt",">");
	}
	return 0;
}
/*
&lt 
&gt 
 */
int xmlQuote::GetXMLSource(DString* Source,int shift)
{
	int j;
	DString tempBody;
	for(j=0;j<shift;j++,Source->Add(" "));
	Source->Add("<");Source->Add(QuoteName);Source->Add(">");
	int n=SubQuotes.GetAmount();
	if(n)Source->Add("\n");
	if(Body.L)
	{
		if(n)for(j=0;j<(shift+1);j++,Source->Add(" "));
		tempBody=Body;
		tempBody.Replace0("<","&lt");
		tempBody.Replace0(">","&gt");
		Source->Add(tempBody);
		if(n)Source->Add("\n");
	}
	for(int i=0;i<n;i++)
		if(SubQuotes[i])
			SubQuotes[i]->GetXMLSource(Source,shift+1);
	if(n){for(j=0;j<shift;j++,Source->Add(" "));}
	Source->Add("</");Source->Add(QuoteName);Source->Add(">\n");
	return 0;
}
bool xmlQuote::ReadFromFile(char* FilePath)
{
	XMLSource.ReadFromFile(FilePath);
	//Log.Warning("File path: %s", FilePath);
	if(XMLSource.pchar()[0]==0)
		return false;
	int l=Load(XMLSource.pchar());
	if(l==-1){
		Log.Error("XML Load Error! XML: %s", FilePath);
		return false;
	}
	return true;
}
void xmlQuote::WriteToFile(char* FilePath)
{
	XMLSource.Clear();
	GetXMLSource(&XMLSource);
	XMLSource.WriteToFile(FilePath);
	XMLSource.Clear();
}
void xmlQuote::Assign_int(int v)
{
	Body.Clear();
	Body.Add(v);
}
void xmlQuote::Assign_string(char* s)
{
	Body.Clear();
	Body.Add(s);
}
void xmlQuote::Assign_string(DString& s)
{
	Body.Clear();
	Body.Add(s);
}
int xmlQuote::AddSubQuote(xmlQuote* SubQ)
{
	SubQuotes.Add(SubQ);
	return SubQuotes.GetAmount();
}
int xmlQuote::AddSubQuote(char* quotename, char* body)
{
	SubQuotes.Add(new_xmlQuote(quotename));
	SubQuotes[SubQuotes.GetAmount()-1]->Assign_string(body);
	return SubQuotes.GetAmount();
}
int xmlQuote::AddSubQuote(char* quotename, int body)
{
	SubQuotes.Add(new_xmlQuote(quotename));
	SubQuotes[SubQuotes.GetAmount()-1]->Assign_int(body);
	return SubQuotes.GetAmount();
}
xmlQuote* xmlQuote::GetSubQuote(int Index)
{
	xmlQuote* rez=NULL;
	if(Index<SubQuotes.GetAmount())
		return (SubQuotes[Index]);
	return rez;
}
xmlQuote* xmlQuote::GetSubQuote(char* SubQuoteName)
{
	int n=SubQuotes.GetAmount();
	for(int i=0;i<n;i++)
		if(SubQuotes[i])
			if(!strcmp(SubQuoteName,SubQuotes[i]->GetQuoteName()))
				return (SubQuotes[i]);
	return NULL;
}
int xmlQuote::GetNSubQuotes()
{
	return SubQuotes.GetAmount();
}
const char* xmlQuote::GetQuoteName()
{
	return QuoteName.cpchar();
}
void xmlQuote::AddAtribute(const char *attributeName, const char *attributeBody)
{
	m_attribute.Add(new CAttribute(attributeName, attributeBody));
}
int xmlQuote::GetNAttribute()
{
	return m_attribute.GetAmount();
}
int xmlQuote::GetAttributeInt(int index)
{
	int rez=0;
	if(GetNAttribute()>index)
	{
		rez=m_attribute[index]->GetInt();
	}
	return rez;
}
int xmlQuote::GetAttributeInt(const char *name)
{
	int rez=0;
	int n=GetNAttribute();
	for(int i=0;i<n;i++)
	{
		if(!strcmp(name,m_attribute[i]->GetName()))
		{
			rez=m_attribute[i]->GetInt();
			break;
		}
	}
	return rez;
}
const char* xmlQuote::GetAttributeStr(int index)
{
	const char* rez=NULL;
	if(GetNAttribute()>index)
	{
		rez=m_attribute[index]->GetString();
	}
	return rez;
}
const char* xmlQuote::GetAttributeStr(const char *name)
{
	const char*  rez=NULL;
	int n=GetNAttribute();
	for(int i=0;i<n;i++)
	{
		if(!strcmp(name,m_attribute[i]->GetName()))
		{
			rez=m_attribute[i]->GetString();
			break;
		}
	}
	return rez;
}
void xmlQuote::SetQuoteName(const char* Name){
	QuoteName=Name;
}
void xmlQuote::ClearAll()
{
	XMLSource.Clear();
	QuoteName.Clear();
	Body.Clear();
	int n=SubQuotes.GetAmount();
	for(int i=0;i<n;i++){		
		if(SubQuotes[i]){			
			delete_xmlQuote(SubQuotes[i]);
			SubQuotes[i]=NULL;
		}
	}
	SubQuotes.Clear();
	int m=m_attribute.GetAmount();
	for(int i=0;i<m;i++)
	{
		if(m_attribute[i])
		{
			delete (m_attribute[i]);
			m_attribute[i]=NULL;
		}
	}
	m_attribute.Clear();
}
//================================================================================================================//
#endif __XMLQUOTE_HPP__