#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__
#include <ClassEngine.h>
#include <more_types.h>
#pragma pack( push )
#pragma pack( 1 )
#include "EngineSettings.h"
class LangClass:public BaseClass{
public:
	void SaveToFile(char* fName);
	bool LoadFromFile(char* fname);
	SAVE(LangClass)		
	ENDSAVE
};
class Language:public LangClass{
public:
	virtual void reset_class(void* DataPtr);
	int LangIndex;

	//-----------registration section--------------
	SAVE(Language){
		REG_MEMBER(_int,LangIndex);
	}ENDSAVE;
	void Save(){
        SaveToFile("Text\\LanguageSettings.xml");
	}
	void Update() {
	}
	void Load(){
		LoadFromFile("Text\\LanguageSettings.xml");
	}
	void Init();
	Language(){		
		Init();
	}
};
//
#ifdef IMPLEMENT_CLASS_FACTORY
Language LangSettings;
//
void LangClass::SaveToFile(char* fName){
	xmlQuote xml("Language");
	BaseClass::Save(xml,this);
	xml.WriteToFile(fName);
}
bool LangClass::LoadFromFile(char* fname){
	xmlQuote xml;
	if(xml.ReadFromFile(fname)){
		ErrorPager EP;
		BaseClass::Load(xml,this,&EP);
		return true;
	}else return false;
}
#else //IMPLEMENT_CLASS_FACTORY
extern Language LangSettings;
#endif //IMPLEMENT_CLASS_FACTORY
#pragma pack( pop )
#endif //__GAME_SETTINGS_H__