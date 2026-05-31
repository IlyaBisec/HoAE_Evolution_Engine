#include "stdheader.h"
int HintTime=0;
//static int HintTime1;
void ResFon(int x,int y,int Lx,int Ly);
void ScrCopy(int x,int y,int Lx,int Ly);
#define NHINT 4
char HintStr[256];
char HintStrLo[256];
char HintStr1[NHINT][256];
byte HintOpt[NHINT];//(0)-usual,(16+x)-in national bar,32-red 
static float  HintTime1[NHINT];
int HintX;
int HintY;
int HintLx;
int HintLy;
void SetupHint(){
	HintY=smapy+smaply*32-20;
	HintX=smapx;
	HintLx=720;
	HintLy=16;
	HintTime=0;
};
extern byte PlayGameMode;
extern bool HideBorderMode;
CEXPORT DWORD GetNatColor( int natIdx );

void ShowHint(){
	if(PlayGameMode==1||HideBorderMode||GSets.CGame.SilenceMessageEvents)
		return;
	if( (GSets.CGame.ViewMask&4)==0 )
		return;
	//ResFon(HintX,HintY,HintLx,HintLy);
	//
	HintX=EngSettings.vInterf.HintX+4;
	if(HintX<0) HintX+=RealLx;
	HintY=EngSettings.vInterf.HintY;
	if(HintY<0) HintY+=RealLy;
	if(HintY==0)HintY=RealLy-300;
	//
	int ShadowShift=1;
	if(HintTime){
        /*
		ShowString(HintX+ShadowShift,HintY+ShadowShift,HintStr,&BlackFont);
        ISM->Flush();
		ShowString(HintX,HintY,HintStr,&WhiteFont);
        ISM->Flush();
		ShowString(HintX+ShadowShift,HintY+20+ShadowShift,HintStrLo,&BlackFont);
        ISM->Flush();
		ShowString(HintX,HintY+20,HintStrLo,&WhiteFont);
        ISM->Flush();
        */
        int LX=10000;
        DrawMultilineText(HintX,HintY,HintStr,&WhiteFont,LX,1111,0,true);
	};
	int yy=HintY;
	int LY=20;
	for(int i=0;i<NHINT;i++){
		if(HintTime1[i]>0){			
			byte opt=HintOpt[i];
			byte NI=opt&7;
			/*
			if(GSets.CGame.cgi_NatRefTBL[MyNation]!=NI){
				GPS.SetCurrentDiffuse(GetNatColor(NI));
			}else{
				GPS.SetCurrentDiffuse(0xFFFFFFFF);
			}
			*/
			static int cf=GPS.PreLoadGPImage("interf3\\chat_player_color");
			int dx=0;
			RLCFont* F=&WhiteFont;
			RLCFont* FB=&BlackFont;

			int LX=10000;
			//LY=20;
			LY=DrawMultilineText(0,0,HintStr1[i],F,LX,100000,0,false);
			//LY-=30;
			//if(LY<25)
			//	LY=25;
			ISM->Flush();
			yy-=LY;

			int DY=0;
			byte opp=opt>>4;

			if( opp==1/* && GSets.CGame.cgi_NatRefTBL[MyNation]!=NI */){				
				dx=GPS.GetGPWidth(cf,0)+7;
				F=&BigWhiteFont;
				FB=&BigBlackFont;
				LX=10000;
				//LY=25;
				DY=0;
				GPS.ShowGP(HintX,yy+14+DY,cf,0,NI);
			}
			HintX+=dx;  
            extern bool ForceShadowDrawing;
            ForceShadowDrawing=true;
            
            DrawMultilineText(HintX+1,yy+DY+1,HintStr1[i],FB,LX,1111,0,true);
            ForceShadowDrawing=false;
			ISM->Flush();
            
            if(opp==2){
                int LX=10000;
                int tt=(GetTickCount()/300)&1;
                if(tt)DrawMultilineText(HintX,yy+DY,HintStr1[i],F,LX,1111,0,true);
                else DrawMultilineText(HintX,yy+DY,HintStr1[i],&RedFont,LX,1111,0,true);
            }else
            if(opp==1){
                int LX=10000;
                //ShowString(HintX,yy+DY,HintStr1[i],F);
                DrawMultilineText(HintX,yy+DY,HintStr1[i],F,LX,1111,0,true);
            }else{
                int LX=10000;
                //ShowString(HintX,yy+DY,HintStr1[i],F);
                DrawMultilineText(HintX,yy+DY,HintStr1[i],F,LX,1111,0,true);
            };
            /*
			ShowString(HintX+ShadowShift,yy+ShadowShift+DY,HintStr1[i],FB);
			if(opp==2){
				int tt=(GetTickCount()/300)&1;
				if(tt)ShowString(HintX,yy+DY,HintStr1[i],F);
				else ShowString(HintX,yy+DY,HintStr1[i],&RedFont);
			}else
			if(opp==1){
				ShowString(HintX,yy+DY,HintStr1[i],F);
				//Xbar(HintX-2,yy,GetRLCStrWidth(HintStr1[i],&WhiteFont)+5,GetRLCHeight(WhiteFont.RLC,'y')+1,0xD0+((opt&7)<<2));
			}else{
				ShowString(HintX,yy+DY,HintStr1[i],F);
			};
            */
			HintX-=dx;
			GPS.SetCurrentDiffuse(0xFFFFFFFF);

			yy-=4;
		};
		//yy-=LY;
	};
	//ScrCopy(HintX,HintY,HintLx,HintLy);
};

bool WasLoHint=0;
void ClearHints(){
	for(int i=0;i<NHINT;i++)HintStr1[i][0]=0;
	WasLoHint=HintStrLo[0]!=0;
	HintStrLo[0]=0;
};
void HideHint(){
	//ResFon(HintX,HintY,HintLx,HintLy);
	//ScrCopy(HintX,HintY,HintLx,HintLy);
};
CEXPORT void AssignHint(char* s,int time){
	if(s[0]==0) 
		return;
	strcpy(HintStr,s);
	//ShowHint();
	HintTime=time;
	WasLoHint=HintStrLo[0]!=0;
	HintStrLo[0]=0;
};
void AssignHintLo(char* s,int time){
	strcpy(HintStrLo,s);
	//ShowHint();
	HintTime=time;
};
CEXPORT void AssignHint1(char* s,int time){
	if(s[0]==0) 
		return;
	/*
	if(!strcmp(s,HintStr1[0])){
		HintTime1[0]=time;
		HintOpt[0]=0;
		return;
	};
	*/
	for(int i=NHINT-1;i>0;i--){
		strcpy(HintStr1[i],HintStr1[i-1]);
		HintTime1[i]=HintTime1[i-1];
		HintOpt[i]=HintOpt[i-1];

	};
	strcpy(HintStr1[0],s);
	HintTime1[0]=time;
	HintOpt[0]=0;
};
extern bool RecordMode;
extern byte PlayGameMode;
extern char LASTCHATSTR[512];
extern byte CHOPT;
CEXPORT void AssignHint1(char* s,int time,byte opt){
	if(s[0]==0) 
		return;
	if(opt>=16){
		if(RecordMode&&!PlayGameMode){
			strcpy(LASTCHATSTR,s);
			CHOPT=opt;
		};
	};
	if(!strcmp(s,HintStr1[0])){
		HintTime1[0]=time;
		HintOpt[0]=opt;
		return;
	};
	for(int i=NHINT-1;i>0;i--){
		strcpy(HintStr1[i],HintStr1[i-1]);
		HintTime1[i]=HintTime1[i-1];
		HintOpt[i]=HintOpt[i-1];

	};
	strcpy(HintStr1[0],s);
	HintTime1[0]=time;
	HintOpt[0]=opt;
};
void GetChar(GeneralObject* GO,char* s){	
	char* hint=NULL;
	hint=GO->newMons->GetLongMessage();
	if(!hint||hint[0]==0){
		Visuals* VS=(Visuals*)GO;
		hint=VS->Message;
	}
	strcpy(s,hint);
};
void GetMonsterCapsString(char* st);
void ProcessHint(){
	ShowHint();
	if(HintTime){	
		HintTime--;
	};
	for(int i=0;i<NHINT;i++){
		if(HintTime1[i]>0){
			HintTime1[i]-=float(GameSpeed)/512.0f;
		};
		if(HintTime1[i]<0){
			HintTime1[i]=0;
		}
	};
};
