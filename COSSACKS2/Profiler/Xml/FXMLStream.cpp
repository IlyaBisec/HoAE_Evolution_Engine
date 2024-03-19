//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "FXMLStream.h"
#include "FXMLCommon.h"
//---------------------------------------------------------------------------
f_FOutStream f_xmlQuote::TmpOutStream;
f_FOutStream f_xmlQuote::TextCollector;
f_FInStream  f_xmlQuote::InStream;
f_FOutStream f_xmlQuote::OutStream;
unsigned   f_xmlQuote::RawLen = 0;
unsigned   f_xmlQuote::PackLen = 0;
bool		  f_xmlQuote::isHex = false;
//---------------------------------------------------------------------------
void f_xmlQuote::StoreTextCollector(void *userData)
{
	if(TextCollector.Size)
	{
		unsigned wpos = 0, Size = TextCollector.Size;
		TmpOutStream.reload(Size);
		while(1)
		{
			unsigned wlen = FXMLGetWord((char*)TextCollector.Data, wpos, Size);
			if(wlen == 0) break;
			if(isspace((TextCollector.Data + wpos)[wlen-1]))
				(TextCollector.Data + wpos)[wlen-1] = ' ';
			TmpOutStream.putblock(TextCollector.Data + wpos, wlen);
			wpos += wlen;
		}
		if(TmpOutStream.Size)
		{
			while(isspace(TmpOutStream.Data[TmpOutStream.Size-1])) 
			{
				TmpOutStream.Pos--;
				TmpOutStream.Size--;
			}
			f_xmlQuote* Curr = *(f_xmlQuote**)userData;
			Curr->Id = idText;
			Curr->SetText((char*)TmpOutStream.Data, TmpOutStream.Size);
			Curr->AddElement();
			*(f_xmlQuote**)userData = Curr->Next;
		}
		TextCollector.reload(0U);
	}
}
//---------------------------------------------------------------------------
void f_xmlQuote::xml_handlerStartElement(void *userData, const char *Name, const char **Attr)
{
	// - store and drop the collected text
	StoreTextCollector(userData);
	f_xmlQuote* Curr = *(f_xmlQuote**)userData;

	// - store the tag
	{
		unsigned AttributesNumber = 0;
		for(int i = 0; Attr[i*2]; i++) 
			AttributesNumber++;

		unsigned NameLen = (unsigned)strlen((char*)Name) + 1;

		Curr->Id = idTag;
		Curr->SetName((char*)Name);
		if(AttributesNumber)
		{
			for(i = 0; Attr[i*2]; i++) 
				Curr->AddAttribute(Attr[i*2], Attr[i*2+1]);
		}
		#ifdef FXML_DEBUG
			printf("%s", Name);
			for(i = 0; Attr[i]; i += 2) 
				printf(" %s=\"%s\"", Attr[i], Attr[i + 1]);
		#endif
	}
	Curr->AddElement();
	*(f_xmlQuote**)userData = Curr->Next;
}
//---------------------------------------------------------------------------
void f_xmlQuote::xml_handlerEndElement(void *userData, const char *Name)
{
	// - store and drop the collected text
	StoreTextCollector(userData);
	f_xmlQuote* Curr = *(f_xmlQuote**)userData;

	// - store the terminator
	{
		Curr->Id = idTerminator;

		#ifdef FXML_DEBUG
			printf("/%s", Name);
		#endif
	}
	Curr->AddElement();
	*(f_xmlQuote**)userData = Curr->Next;
}
//---------------------------------------------------------------------------
void f_xmlQuote::xml_handlerText(void *userData, const char *Text, int TextLen)
{
	TextCollector.putblock((unsigned char*)Text, TextLen);
}
//---------------------------------------------------------------------------
// - loading and saving routines
//---------------------------------------------------------------------------
void f_xmlQuote::ProcessAttributes(void)
{
	FAttribute* Attr = Attributes;
	while(Attr)
	{
		FXMLPutc(' ');
		FXMLPrint(Attr->Name);
		FXMLPutc('=');
		FXMLPutc('\"');
		FXMLPrintEx(Attr->Value);
		FXMLPutc('\"');
		Attr = Attr->Next;
	}
}
//---------------------------------------------------------------------------
void f_xmlQuote::ProcessSave(f_xmlQuote** p_xmlCurr, int* p_Depth)
{
	f_xmlQuote* El1 = (*p_xmlCurr);
	(*p_xmlCurr) = (*p_xmlCurr)->Next;
	f_xmlQuote* El2 = (*p_xmlCurr);

   const char* Name = El1->Name;
   unsigned NameLen = (unsigned)strlen((char*)Name);

	if(El2->Id == idTerminator)
	{
   	// 1) An element without children is printed as a <foo /> node
		for(int i = 0; i < (*p_Depth); i++) FXMLPutc(' ');
		FXMLPutc('<');
		FXMLPrint(Name);
		El1->ProcessAttributes();
		FXMLPutc('>');
		FXMLPrint("</");
		FXMLPrint(Name);
		El1->ProcessAttributes();
		FXMLPrint(">\n");
		(*p_xmlCurr) = (*p_xmlCurr)->Next;
	}
	else
   if(El2->Id == idText && El2->Next->Id == idTerminator)
	{
   	// 2) An element with only a text child is printed as <foo> text </foo>
		for(int i = 0; i < (*p_Depth); i++) FXMLPutc(' ');
		FXMLPutc('<');
		FXMLPrint(Name);
		El1->ProcessAttributes();
		FXMLPutc('>');
		FXMLPrintEx(El2->Text);
		FXMLPrint("</");
		FXMLPrint(Name);
		El1->ProcessAttributes();
		FXMLPrint(">\n");

		(*p_xmlCurr) = (*p_xmlCurr)->Next;
		(*p_xmlCurr) = (*p_xmlCurr)->Next;
	}
	else
	{
   	// 3) An element with children is printed on multiple lines.
		for(int i = 0; i < (*p_Depth); i++) FXMLPrint(" ");
		FXMLPutc('<');
		FXMLPrint(Name);
		El1->ProcessAttributes();
		FXMLPrint(">\n");

		while(1)
      {
			if(El2->Id == idTag)
			{
				(*p_Depth)++;
            ProcessSave(p_xmlCurr, p_Depth);
				El2 = (*p_xmlCurr);
			}
         else
			if(El2->Id == idText)
         {
				(*p_xmlCurr) = (*p_xmlCurr)->Next;
				char* Text = El2->Text;
				for(int i = 0; i <= (*p_Depth); i++) FXMLPutc(' ');
				FXMLPrintEx(Text);
				FXMLPutc('\n');
				El2 = (*p_xmlCurr);
         }
			else
			if(El2->Id == idTerminator)
			{
				(*p_xmlCurr) = (*p_xmlCurr)->Next;
				for(int i = 0; i < (*p_Depth); i++) FXMLPutc(' ');
				FXMLPrint("</");
				FXMLPrint(Name);
				El1->ProcessAttributes();
				FXMLPrint(">\n");
			   break;
			}
			else
			{
				FXML_ERROR("Unknown tag!\n");
				return;
			}
      }
	}
	(*p_Depth)--;
}
//---------------------------------------------------------------------------
bool f_xmlQuote::SaveToStream(void)
{
	if(Id != idTag) return NULL;
	OutStream.reload(RawLen);

	f_xmlQuote* t = this;
	int Depth = 0;

	ProcessSave(&t, &Depth);
	return true;
}
//---------------------------------------------------------------------------
bool f_xmlQuote::SaveToFile(char* OutFileName)
{
	if(!SaveToStream()) return false;
	OutStream.save(OutFileName);
	RawLen = OutStream.Size;
	return true;
}
//---------------------------------------------------------------------------
