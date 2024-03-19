#ifndef FXMLCommonH
#define FXMLCommonH

#include <windows.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "FXMLMessages.h"
#include "..\\FStreams\\FBaseStream.h"
//---------------------------------------------------------------------------
//#define FXML_DEBUG
//---------------------------------------------------------------------------
enum e_fxmlId {
   idTerminator = 0,
   idText,
   idTextTag,
   idTextAttribute,
   idTag = 16,
   idNone = 0xFFFFFFFF
};
//---------------------------------------------------------------------------
enum e_fxmlFlags {
   flEmpty = 0,
   flInPool,
   flInMemory
};
//---------------------------------------------------------------------------
enum e_fxmlKind {
   FXML_KIND_NONE,
   FXML_KIND_COMPLEX,
   FXML_KIND_SIMPLY,
   FXML_KIND_ALONE,
   FXML_KIND_ATTRIBUTE
};
//---------------------------------------------------------------------------
enum e_fxmlType {
   FXML_TYPE_NONE,
   FXML_TYPE_BOOL,
   FXML_TYPE_INTEGER,
   FXML_TYPE_FLOAT
};
//---------------------------------------------------------------------------
static unsigned FXMLGetWord(const char* str, unsigned& w1, unsigned w2)
{
	while(w1 < w2 && isspace(str[w1])) w1++;
	if(w1 >= w2) return 0;
	unsigned len = 0;
	while(w1 + len < w2 && !isspace(str[w1 + len])) len++;
	if(len && w1 + len < w2 && isspace(str[w1 + len])) 
		len++;
	return len;
}
//---------------------------------------------------------------------------
#define FXMLPutc(ch) {\
	OutStream.putc(ch);\
}
#define FXMLPutcEx(ch) {\
	if(ch == '&')\
		OutStream.putblock((unsigned char*)"&amp;", 5);\
	else\
	if(ch == '<')\
		OutStream.putblock((unsigned char*)"&lt;", 4);\
	else\
	if(ch == '>')\
		OutStream.putblock((unsigned char*)"&gt;", 4);\
	else\
	if(ch == '"')\
		OutStream.putblock((unsigned char*)"&quot;", 6);\
	else\
	if(ch == 9)\
		OutStream.putblock((unsigned char*)"&#9;", 4);\
	else\
	if(ch == 10)\
		OutStream.putblock((unsigned char*)"&#10;", 5);\
	else\
	if(ch == 13)\
		OutStream.putblock((unsigned char*)"&#13;", 5);\
	else\
		OutStream.putc(ch);\
}
#define FXMLPrint(str) {\
	char* pos = (char*)(str);\
	while(*pos)\
	{\
		FXMLPutc(*pos);\
		pos++;\
	}\
}
#define FXMLPrintEx(str) {\
	char* pos = (char*)(str);\
	while(*pos)\
	{\
		FXMLPutcEx(*pos);\
		pos++;\
	}\
}
#define FXMLPrintN(Depth) {\
   if(Depth >= 0)\
   {\
      FXMLPrint("\r\n");\
      for(int i = 0; i < Depth; i++) FXMLPutc(' ');\
   }\
}
//---------------------------------------------------------------------------
#endif
