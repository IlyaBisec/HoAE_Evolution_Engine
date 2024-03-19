#include "stdheader.h"
#include "GameExtension.h"
#include "Save_XML_ToMap.h"
class RootXmlSaver : public MapXmlSaver
{
public:
	virtual bool GetXmlToSaveInMap(xmlQuote& xml){
		ext_OnMapSaving(xml);
		return true;
	}
	virtual void SetXmlFromMap(xmlQuote& xml){
		ext_OnMapLoading(xml);		
	}
	virtual DWORD GetSaverID(){return 'TOOR';};
public:
	SAVE(RootXmlSaver);
	REG_PARENT(MapXmlSaver);
	ENDSAVE;
};
ClassArray<MapXmlSaver>* XmlSaver=NULL;
void RegisterXML_Saver(MapXmlSaver* Saver){
    if(!XmlSaver)XmlSaver=new ClassArray<MapXmlSaver>;
	DWORD ID=Saver->GetSaverID();
	for(int i=0;i<XmlSaver->GetAmount();i++){
		assert((*XmlSaver)[i]->GetSaverID()!=ID);
	}
	XmlSaver->Add(Saver);
};
void SaveAllXMLSToMap(ResFile f1){
	if(XmlSaver){
		for(int j=0;j<XmlSaver->GetAmount();j++){
			xmlQuote xml;
			if((*XmlSaver)[j]->GetXmlToSaveInMap(xml)){
				int i='LMXS';
				RBlockWrite(f1,&i,4);
				i=(*XmlSaver)[j]->GetSaverID();				
				DString D1;
				xml.GetXMLSource(&D1);	
				int sz1=strlen(D1.pchar())+1;	
				int sz=sz1+4+4+4;
				RBlockWrite(f1,&sz,4);
				RBlockWrite(f1,&i,4);
				RBlockWrite(f1,&sz1,4);
				RBlockWrite(f1,D1.pchar(),sz1);
			}
		}
	}
}
void LoadXML_FromMap(ResFile f1){
	if(XmlSaver){
		int i=0;
		RBlockRead(f1,&i,4);
		for(int j=0;j<XmlSaver->GetAmount();j++)if((*XmlSaver)[j]->GetSaverID()==i){
			int sz1;
			RBlockRead(f1,&sz1,4);
			char* s=new char[sz1];
			RBlockRead(f1,s,sz1);	
			xmlQuote xml;
			xml.Load(s);
			delete[](s);
			(*XmlSaver)[j]->SetXmlFromMap(xml);
		}
	}
}
void ResetAllXMLS(){
	if(XmlSaver){
		for(int j=0;j<XmlSaver->GetAmount();j++){
			(*XmlSaver)[j]->ResetMap();
		}
	}
}
void RegisterAllSavers(){
	REG_XML_SAVER(RootXmlSaver);
	void RegisterWallsSaver();
	RegisterWallsSaver();
	void	RegisterBattleEditorSaver();	// Vitya for Battle Editor //  [3/11/2004]
	RegisterBattleEditorSaver();			// Vitya for Battle Editor //  [3/11/2004]
}
