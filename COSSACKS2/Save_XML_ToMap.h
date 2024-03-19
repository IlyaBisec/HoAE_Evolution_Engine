#pragma once
class MapXmlSaver:public BaseClass{
public:
	virtual bool GetXmlToSaveInMap(xmlQuote& xml){return false;};
	virtual bool GetXmlToSaveInSaveFile(xmlQuote& xml){return false;};

	virtual void SetXmlFromMap(xmlQuote& xml){};
	virtual void SetXmlFromSaveFile(xmlQuote& xml){};

	virtual void ResetMap(){};

	virtual DWORD GetSaverID(){return 0;};

	SAVE(MapXmlSaver);
	ENDSAVE;
};
DIALOGS_API void RegisterXML_Saver(MapXmlSaver* Saver);
#define REG_XML_SAVER(x) RegisterXML_Saver(new x);