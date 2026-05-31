#include "stdheader.h"
#include "Extensions.h"
class OneBinkText:public BaseClass{
public:
	RLCFont* Font;
	float StartTime;
	float Length;
	_str  TextID;
	SAVE(OneBinkText);
		REG_MEMBER(_font,Font);
		REG_MEMBER(_float,StartTime);
		REG_MEMBER(_float,Length);
		REG_AUTO(TextID);		
	ENDSAVE;
	OneBinkText(){
		Font=&YellowFont;
		StartTime=0;
		Length=10;
		TextID="";
	}
};
ClonesArray<OneBinkText> BinkText;
void TryToLoadSubtitles(char* BinkName){
	BinkText.reset_class(&BinkText);
    char name[256];
	strcpy(name,BinkName);
	strupr(name);
	char* s=strstr(name,".BIK");
	if(s){
		s[0]=0;
		strcat(s,".SUBTITLES.XML");
		BinkText.ReadFromFile(name);
	}
}
void DrawSubtitlesInWindow(int x,int y,int Lx,int Ly,float TimePassed){
	for(int i=0;i<BinkText.GetAmount();i++){
		OneBinkText* OBT=BinkText[i];
		if(TimePassed>OBT->StartTime&&TimePassed<OBT->StartTime+OBT->Length){
            char *s=OBT->TextID.pchar();
			if(s&&OBT->Font){
				s=GetTextByID(s);
				int LX=Lx;
                int L=DrawMultilineText(x,y,s,OBT->Font,LX,10000,1,0);
				int LY=120;
				DrawMultilineText(x,y+Ly-(L+LY)/2,s,OBT->Font,LX,10000,1,true);
			}
		}
	}
}
void RegBinkTextEditor(){
	REG_CLASS(OneBinkText);
	REG_LOADSAVE_FOR_CLASS(ClonesArray<OneBinkText>,"","*.subtitles.xml");
	AddStdEditor("Bink subtitles editor",&BinkText,"",RCE_DEFAULT);
}