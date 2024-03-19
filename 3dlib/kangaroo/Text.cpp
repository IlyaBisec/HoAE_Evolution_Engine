#include "StdAfx.h" #include "common.h" 

char GlobalTextCommand[256]="";
char GlobalTextMouseOverCommand[256]="";
bool ForceShadowDrawing=false;

struct TextFrame{
	int FrameY;
	int FrameY1;
	int FrameLx;
};
const char* Unbreakables=".!,?%:'\")+-=@";
struct MultilineTemp{	
	MultilineTemp(){
		//ActiveRefCommand[0]=0;
		LFrame.FrameLx=0;
		RFrame.FrameLx=0;
	}
	DWORD FontColor;
	DWORD RefColor;
	int FONT;
	int CFont;
	int DefaultFont;
	DWORD DefaultColor;
	//OneFontParam* CurFParam;
	int xL;//input
	int yL;//input
	int MaxWidth;//input
	int sposL;//input
	int Height;//output
	int DnHeight;//output
	int CurLx;//output
	int TotalHeight;
	int RTotalHeight;
	int sposR;//output
	int LastSpacePos;//output
	int LastSpaceLx;//output
	int LastSpaceSym;//output
	int NSpaces;//output
	TextFrame LFrame;
	TextFrame RFrame;
	float SymDx;
	float SymTotDx;
	int NSym;
	char* s;	
	int SpaceAddDist;//output
	int ActiveRefIndex;
	char CurTxtCom[256];
	char ActiveRefCommand[256];
	int curref;
	bool refopen;
	bool RefActivated;
	DWORD ColorStack[64];
	int stPos;
	int CurAlign;
	int DefAlign;

	bool DoOneLine(byte Draw){		
		bool res=false;	
		float cumm=0;
		int pos=sposL;
		int NBracks=1;
		if(!Draw){
			LastSpacePos=-1;
			LastSpaceLx=0;
			LastSpaceSym=0;
			Height=0;
			TotalHeight=0;
			NSpaces=0;
			sposR=1000000;
			NSym=0;
			DnHeight=0;
		}		
		CurLx=0;
		int dCurLx=0;
		NSpaces=0;		
		bool first=true;
		int prevpos=pos;
		SetCurrentFont(CFont);
		while(s[pos]){
			if(s[pos]=='{' && NBracks&1 ){
				char comstr[2048];
				int dp=0;
				pos++;
				int Level=0;
				while(dp<2040&& (s[pos+dp]!='}' || Level!=0) &&s[pos+dp]!=0){
					char sp=s[pos+dp];
					if(sp=='{')Level++;
					if(sp=='}')Level--;
					comstr[dp]=sp;
					comstr[dp+1]=0;
					dp++;
				}
				pos+=dp+1;
				if(!strncmp(comstr,"dy",2)){
					int dy=atoi(comstr+2);
					TotalHeight+=dy;
					sposR=pos;
					goto LABEL;
				}else
				if(comstr[0] && !strncmp(comstr+1,"frame",5)){
					int x0;					
					TextFrame* TF=NULL;
					char* s=comstr+7;
					int FLX=MaxWidth;
					int FLY=DrawMultilineText(-100000,-100000,s,CFont,FontColor,FLX,10000,false,false);
					if(comstr[0]=='l' || comstr[0]=='L'){
						TF=&LFrame;
						x0=xL;                        
					}
					if(comstr[0]=='r' || comstr[0]=='R'){
						TF=&RFrame;
						x0=xL+MaxWidth-FLX;
					}
					if(TF){
						TF->FrameLx=FLX;
						TF->FrameY=yL;
						TF->FrameY1=yL+FLY+8;
						if(Draw==1){
							int FLX=MaxWidth;
							DrawMultilineText(x0,TF->FrameY-GetTextHeight(),s,CFont,FontColor,FLX,10000,false,true);
						}
						if(comstr[0]=='L' || comstr[0]=='R'){
							TF->FrameY1=1000000;
						}
					}

				}else
				if(comstr[0]=='R'&&comstr[1]==' '){//reference
					curref++;
					refopen=1;
					sscanf(comstr+2,"%X",&RefColor);
					char* c2=strstr(comstr+2," ");
					if(c2){
						strcpy(CurTxtCom,c2+1);
					}else CurTxtCom[0]=0;
					if(Draw&&!strcmp(ActiveRefCommand,CurTxtCom)){//curref==ActiveRefIndex){
						sscanf(comstr+2,"%X",&FontColor);
						RefColor=FontColor;						
						if(Widgets::lPressed)strcpy(GlobalTextCommand,CurTxtCom);
						strcpy(GlobalTextMouseOverCommand,CurTxtCom);
					}
				}else
				if(comstr[0]=='C'&&(comstr[1]==0||comstr[1]==' '||(comstr[1]!=0&&comstr[2]==0))){
					if(comstr[1]==0){//{C XXXXXXXX}
						if(stPos>0){
							stPos--;
							FontColor=ColorStack[stPos];
						}else FontColor=DefaultColor;
						refopen=0;
					}else{
						if(stPos<63){
							ColorStack[stPos]=FontColor;
							stPos++;
						}
						if(comstr[1]==' '){//{C XXXXXXXX}						
							sscanf(comstr+2,"%X",&FontColor);
						}else
						if(comstr[1]=='B'){
							FontColor=0xFF000000;
						}else
						if(comstr[1]=='R'){
							FontColor=0xFFB83B3F;
						}else
						if(comstr[1]=='D'){	// damask - ??
							FontColor=0xFFFF0000;
						}else
						if(comstr[1]=='N'){
							FontColor=0xFF502515;
						}else
						if(comstr[1]=='W'){
							FontColor=0xFFFFFFFF;
						}else
						if(comstr[1]=='Y'){
							FontColor=0xFFFFFF00;
						}else
						if(comstr[1]=='G'){
							FontColor=0xFF60A05A;
						}
					}
				}else
				if(comstr[0]=='F'){
					if(comstr[1]==0){
						CFont=DefaultFont;
						FontColor=DefaultColor;						
					}else{
						CFont=atoi(comstr+1);										
					}
					SetCurrentFont(CFont);
				}else
				if(comstr[0]=='A'&&strlen(comstr)<=2){
					if(comstr[1]=='C'){
						CurAlign=1;
					}
					if(comstr[1]=='R'){
						CurAlign=2;
					}
					if(comstr[1]=='L'){
						CurAlign=0;
					}					
					if(comstr[1]==0){
						CurAlign=DefAlign;
					}					
				}
			}else{
				if(first){
					first=false;
					if(LFrame.FrameLx){
						if(yL<LFrame.FrameY1)CurLx+=LFrame.FrameLx;
					}
					if(RFrame.FrameLx){
						if(yL<RFrame.FrameY1)MaxWidth-=RFrame.FrameLx;
					}
				}
				if(s[pos]=='\"')NBracks++;
				if(Draw==1&&refopen&&(curref==ActiveRefIndex||!strcmp(ActiveRefCommand,CurTxtCom))){
					FontColor=RefColor;
					if(Widgets::lPressed)strcpy(GlobalTextCommand,GlobalTextMouseOverCommand);//CurTxtCom);
					//strcpy(GlobalTextMouseOverCommand,CurTxtCom);
				}
				if(pos==sposL&&s[pos]==' ')pos++;
				else{
					int uW=1;
					char sc[2];
					sc[0]=s[pos];
					sc[1]=0;
					SetCurrentFont(CFont);
					int cLx=GetTextWidth(sc,&uW);
					if(refopen&&Draw==1){
						if(Widgets::MouseX>xL+CurLx+dCurLx&&Widgets::MouseY<=xL+CurLx+dCurLx+cLx&&
							Widgets::MouseY<yL&&Widgets::MouseY>yL-GetTextHeight()){
								ActiveRefIndex=curref;
								strcpy(ActiveRefCommand,CurTxtCom);
								RefActivated=true;
							}
					}
					if(CurLx+cLx>MaxWidth && !strchr(Unbreakables,s[pos])){
						if(LastSpacePos>sposL && s[pos]!=10){
							//breaking at space
							sposR=LastSpacePos;
							CurLx=LastSpaceLx;
							NSym=LastSpaceSym;
							//sposR=pos;
						}else{
							//breaking at the current symbol
							sposR=prevpos;
						}
					}
					if(pos>=sposR || s[pos]==13||s[pos]==10){						
						if(s[pos]==10){
							sposR=pos+1;
						}
						if(s[pos]==13){
							sposR=pos+1;
							if(s[pos+1]==10){
								pos++;
								sposR++;
							}
						}						
						goto LABEL;
						return false;
					}
					if(pos>0){
						char cc=s[pos];
						char cp=s[pos-1];						
						if( ( cc>='A' && cc<='Z' && cp>='a' && cp<='z')
							||( ( cc>='0' && cc<='9' ) && ! ( cp>='0' && cp<='9' ) )
							|| cp=='!' || cp=='#' || cp=='%' || cp=='&' || (cp>=41 && cp<=47) || cp==':' //punct
							|| cp==';' || (cp>=61 && cp<=64) || (cp>=92 && cp<=96) || (cp>=124 && cp<=127) //punct 
							|| cc==9 || cc=='$' || cc=='(' || cc=='<' || cc=='[' || cc=='{' ){
								LastSpacePos=pos;
								LastSpaceLx=CurLx;
								LastSpaceSym=NSym;
							}
					}
					if(s[pos]==' '&&pos>sposL){
						LastSpacePos=pos;
						LastSpaceLx=CurLx;
						LastSpaceSym=NSym;
						NSpaces++;
					}					
					
					if(Draw==1){						
						if(ForceShadowDrawing)FontColor=0x90000000;						
						DrawChar(xL+CurLx+dCurLx,yL-GetTextHeight(),s[pos],FontColor);						
					}				
					int H=GetTextHeight();
					int _DnHeight=H/8;
					
					if(H>Height)Height=H;					
					if(H>TotalHeight)TotalHeight=H;
					if(_DnHeight>DnHeight)DnHeight=_DnHeight;					
					if(s[pos]==' '&&Draw){
						if(SpaceAddDist)CurLx+=SpaceAddDist;
					}
					CurLx+=cLx;					
					prevpos=pos;
					pos+=uW;
					if(Draw){
						cumm+=SymDx;
						if(cumm>1){
							cumm-=1.0f;
							CurLx++;
						}						
					}else NSym++;
				}
			}
		}
		sposR=pos;
		res=true;
LABEL:
		if(!Draw){
			bool Form=strstr(s,"{R")==NULL;
			SpaceAddDist=0;
			SymDx=0;
			SymTotDx;
			if(Form){
				if(NSym){
					SymDx=0;//float(MaxWidth-CurLx)/NSym;
					if(SymDx>0.5f)SymDx=0.5f;
				}
				if(NSpaces>1){
					SpaceAddDist=(MaxWidth-CurLx-SymDx*NSym)/(NSpaces);
					if(SpaceAddDist>5){
						SpaceAddDist=0;
						SymDx=0;
					}					
				}			
				dCurLx+=NSym*SymDx+(NSpaces-1)*SpaceAddDist;		
			}
		}        
		return res;
	}
};
int DrawMultilineText(int x,int y,char* s,int DefaultFont,DWORD DefColor,int& MaxLx,int MaxLy,byte CenterX,bool Draw){	
	if(!s||!DefaultFont)return 0;	
	int Maxx0=MaxLx;
	MultilineTemp T1;
	MultilineTemp T2;
	static char ActiveRefCom[256]="";
	if(Draw){
		strcpy(T1.ActiveRefCommand,ActiveRefCom);
		strcpy(T2.ActiveRefCommand,ActiveRefCom);
	}else{
		T1.ActiveRefCommand[0]=0;
		T2.ActiveRefCommand[0]=0;
	}
	T1.RefActivated=0;
	T1.xL=x;
	T1.yL=y;
	T1.xL=x;
	T1.DefaultFont=DefaultFont;
	T1.CFont=DefaultFont;	
	T1.DefaultColor=DefColor;
	//T1.CurFParam=FParam.GetByGP(int(DefaultFont->RLC));
	T1.s=s;
	T1.sposL=0;
	T1.MaxWidth=MaxLx;
	T1.FontColor=DefColor;
	T1.curref=0;
	T1.ActiveRefIndex=-1;
	T1.refopen=0;
	T1.stPos=0;
	T1.CurAlign=CenterX;
	T1.DefAlign=CenterX;
	int TH=0;
	bool R;
	int MaxW=0;
	int NL=0;
	do{
		T2=T1;
		int cr=T1.curref;
		bool ro=T1.refopen;
		T1.Height=0;
		T1.TotalHeight=0;
		R=T1.DoOneLine(false);
		T1.MaxWidth=MaxLx;		

		T1.yL+=T1.Height;
		TH+=T1.TotalHeight+(R==false?T1.DnHeight:0);
		int DH=T1.TotalHeight-T1.Height;
		int DN=T1.DnHeight;		

		if((NL==0&&R)||T1.CurAlign){			
			T1.SymDx=0;
			T1.SpaceAddDist=0;
		}else{
			T1.CurLx+=T1.NSym*T1.SymDx+(T1.NSpaces-1)*T1.SpaceAddDist;
		}
		byte align=T1.CurAlign;
		T1.DefaultFont=T2.DefaultFont;
		T1.CFont=T2.CFont;
		//T1.CurFParam=T2.CurFParam;
		T1.FontColor=T2.FontColor;
		T1.FONT=T2.FONT;
		//T1.ColorStack=T2.ColorStack;

		T1.CurAlign=T2.CurAlign;
		if(T1.CurLx>MaxW)MaxW=T1.CurLx;		
		//if(Draw){
		T2=T1;
		T2.curref=cr;
		T2.refopen=ro;
		if(align&&T2.MaxWidth<2000&&T2.MaxWidth){//&&(T2.sposR<strlen(T2.s)||NL>0)){
			T2.xL+=(T2.MaxWidth-T2.CurLx)/2;
			if(Maxx0&&Maxx0<2000)MaxW=Maxx0;
		}			
		T2.DoOneLine(Draw?1:2);
		T2.MaxWidth=MaxLx;
		if(!R){
			DH=max(DH,DN);
			T2.yL+=DH;
			T1.yL+=DH;
		};
		//if(NL!=0){
		//	TH+=T2.DnHeight;
		//	T1.yL+=T2.DnHeight;
		//}
		T1.DefaultFont=T2.DefaultFont;
		T1.CFont=T2.CFont;		
		T1.FontColor=T2.FontColor;
		T1.CurAlign=T2.CurAlign;
		T1.FONT=T2.FONT;
		strcpy(T1.ActiveRefCommand,T2.ActiveRefCommand);
		T1.RefActivated=T2.RefActivated;
		if(T1.RFrame.FrameLx){
			MaxW=MaxLx;
		}
		//T1.ColorStack=T2.ColorStack;
		//}
		T1.sposL=T1.sposR;		
		NL++;
	}while(NL<2500&&!R);
	if(T1.RFrame.FrameLx){
		if(TH<T1.RFrame.FrameY1-y)TH=T1.RFrame.FrameY1-y;
	}
	if(T1.LFrame.FrameLx){
		if(TH<T1.LFrame.FrameY1-y)TH=T1.LFrame.FrameY1-y;
	}
	MaxLx=MaxW;
	if(Draw){
		strcpy(ActiveRefCom,T2.ActiveRefCommand);
		static T0=0;
		if(!T2.RefActivated){			
			if(T0==0)T0=GetTickCount();
			else{
				if(GetTickCount()-T0>200)ActiveRefCom[0]=0;
			}
		}else T0=0;
	}	
	return TH;
}
int DrawBottomAlignedText(int xb,int yb,char* Text,int MaxLx,int FontSize,DWORD Color,int Align,bool Draw){
	int _MaxLx=MaxLx;
    int h=DrawMultilineText(0,0,Text,FontSize,Color,_MaxLx,10000,Align,false);
	yb-=h;
	if(Draw){
		DrawMultilineText(xb,yb,Text,FontSize,Color,_MaxLx,10000,Align,true);
	}
	return yb;
}