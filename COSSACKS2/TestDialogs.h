#ifndef __TESTEDIALOGS_H__
#define __TESTEDITALOGS_H__
#pragma pack( push )
#pragma pack( 1 )
#include "stdheader.h"
//#include "dialogs.h"
//////////////////////////////////////////////////////////////////////////////////////////
//------class InputBox----------------//
extern bool realLpressed;
BOOL ClipBrdSetText(LPCSTR lpcszText){
	INT iLen = lstrlen(lpcszText)+1;
    HGLOBAL hgData = GlobalAlloc(GHND, iLen);
    LPVOID	lpvData = GlobalLock(hgData);
    lstrcpy((LPTSTR)lpvData,lpcszText);
    GlobalUnlock(hgData);
	OpenClipboard(NULL);
	EmptyClipboard();
    SetClipboardData(CF_TEXT,hgData);
    CloseClipboard();
    GlobalFree(hgData);
	return TRUE;
}

BOOL ClipBrdGetText(LPSTR lpszText,LPDWORD lpdwSize){
	OpenClipboard(NULL);
	HGLOBAL hgData=GetClipboardData(CF_TEXT);
	if(!hgData)
		return FALSE;
	LPVOID lpvData=GlobalLock(hgData);
	if(lstrlen((LPSTR)lpvData)>(INT)(*lpdwSize)){
		(*lpdwSize)=lstrlen((LPSTR)lpvData)+1;
		GlobalUnlock(hgData);
		return FALSE;
	};
	lstrcpy(lpszText,(LPSTR)lpvData);
	GlobalUnlock(hgData);
	CloseClipboard();
	return TRUE;
}
/////////////////////////////////////////////////////////////
////////////////////////// new //////////////////////////////

void InputBox_LMousePressed(SimpleDialog* SD, int x, int y){
	if(!SD->Enabled)return;
	InputBox* IB=(InputBox*)SD;
	x+=IB->totdx;
	int length=strlen(IB->Str?IB->Str:IB->_Str->pchar());
	char base[2048];
	if(IB->Anonim)
		for(int i=0;i<length;i++)base[i]='*';
	else
		strcpy(base,IB->Str?IB->Str:IB->_Str->pchar());
	base[length]=0;

	int LL=IB->x1-IB->x;
	int line=0, ch=0, width=0, dx=0, start_ch=0;

	IB->CursPos=length;

	int line_height=0, by=0;

	extern FontParam FParam;
	OneFontParam* FP=FParam.GetByGP(int(IB->Font->RLC));
	if(FP){
		line_height=FP->Bottom-FP->Top+FP->YShift;
		//by=FP->Bottom-line_height+2;
		//by=FP->Top-FP->YShift/2;
	}else{
		int Lx=IB->Multiline?IB->GetWidth():10000;
		line_height=DrawMultilineText(0,0,"W",IB->Font,Lx,1000000,0,false);
		//by=GetRLCHeight(IB->Font->RLC,'W')-line_height;
		line_height=line_height*3/2;
	}

	if(IB->Centering){
		int strWidth=GetRLCStrWidth(base,IB->Font);
		if(LL>strWidth)
			dx=(LL-strWidth)/2;
		else if(IB->Multiline){
			while(ch<length){
				bool end_line=false;
				int width0=0;
				int ch0=0;
				//while(ch<length && ch0<128 && base[ch]>32){
				while(ch<length && ch0<128 && (base[ch]<0 || base[ch]>32)){
					width0+=GetRLCWidth(IB->Font->RLC,base[ch]);
					ch++;
					ch0++;
				}
				if(ch0){
					if(width+width0<LL){
						width+=width0;
					}else{
						ch-=ch0;
						end_line=true;
					}
				}

				width0=0;
				ch0=0;
				//while(ch<length && ch0<128 && base[ch]<=32 && base[ch]!=13 && base[ch]!=10){
				while(ch<length && ch0<128 && (base[ch]>=0 && base[ch]<=32) && base[ch]!=13 && base[ch]!=10){
					width0+=GetRLCWidth(IB->Font->RLC,base[ch]);
					ch++;
					ch0++;
				}
				if(ch0){
					if(width+width0<LL){
						width+=width0;
					}else{
						ch-=ch0;
						end_line=true;
					}
				}
				if(base[ch]==13 && base[ch+1]==10){
					ch+=2;
					end_line=true;
				}
				if(end_line || ch>=length){
					dx=(LL-width)/2;
					if(y>=line_height*line+by && line_height*(line+1)+by>y)
						break;
					width=0;
					line++;
					start_ch=ch;
				}
			}
		}
	}
	width=dx;
	ch=start_ch;
	if(IB->Multiline){
		while(ch<length){
			bool end_line=false;
			int width0=0;
			int ch0=0;
			//while(ch<length && ch0<128 && base[ch]>32){
			while(ch<length && ch0<128 && (base[ch]<0 || base[ch]>32)){
				width0+=GetRLCWidth(IB->Font->RLC,base[ch]);
				ch++;
				ch0++;
			}
			if(ch0){
				if(width+width0<LL){
					width+=width0;
					if(y>=line_height*line+by && line_height*(line+1)+by>y && width>=x){
						while(width>=x && ch>start_ch){
							ch--;
							width-=GetRLCWidth(IB->Font->RLC,base[ch]);
						}
						IB->CursPos=ch;
						break;
					}
				}else{
					ch-=ch0;
					if(y>=line_height*line+by && line_height*(line+1)+by>y){
						IB->CursPos=ch-1;
						break;
					}
					end_line=true;
				}
			}

			width0=0;
			ch0=0;
			//while(ch<length && ch0<128 && base[ch]<=32 && base[ch]!=13 && base[ch]!=10){
			while(ch<length && ch0<128 && (base[ch]>=0 && base[ch]<=32) && base[ch]!=13 && base[ch]!=10){
				width0+=GetRLCWidth(IB->Font->RLC,base[ch]);
				ch++;
				ch0++;
			}
			if(ch0){
				if(width+width0<LL){
					width+=width0;
					if(y>=line_height*line+by && line_height*(line+1)+by>y && width>=x){
						while(width>=x && ch>start_ch){
							ch--;
							width-=GetRLCWidth(IB->Font->RLC,base[ch]);
						}
						IB->CursPos=ch;
						break;
					}
				}else{
					ch-=ch0;
					if(y>=line_height*line+by && line_height*(line+1)+by>y){
						IB->CursPos=ch-1;
						break;
					}
					end_line=true;
				}
			}
			if(base[ch]==13 && base[ch+1]==10){
				if(y>=line_height*line+by && line_height*(line+1)+by>y){
					IB->CursPos=ch-1;
					break;
				}
				ch+=2;
				end_line=true;
			}
			if(end_line){
				width=0;
				line++;
				start_ch=ch;
			}
		}
	}else{
		while(ch<length){
			if(width>=x){
				IB->CursPos=ch;
				break;
			}
			width+=GetRLCWidth(IB->Font->RLC,base[ch]);
			ch++;
		}
	}
	Lpressed=0;
};

void InputBox_CursUpDown(SimpleDialog* SD, int action){
	if(!SD->Enabled)return;
	InputBox* IB=(InputBox*)SD;
	if(!IB->Multiline)return;
	int length=strlen(IB->Str?IB->Str:IB->_Str->pchar());
	char base[2048];
	if(IB->Anonim)
		for(int i=0;i<length;i++)base[i]='*';
	else
		strcpy(base,IB->Str?IB->Str:IB->_Str->pchar());
	int LL=IB->x1-IB->x;

	int line_height=0, by=0;

	extern FontParam FParam;
	OneFontParam* FP=FParam.GetByGP(int(IB->Font->RLC));
	if(FP){
		line_height=FP->Bottom-FP->Top+FP->YShift;
		//by=FP->Bottom-line_height+2;
		//by=FP->Top-FP->YShift/2;
	}else{
		int Lx=IB->Multiline?IB->GetWidth():10000;
		line_height=DrawMultilineText(0,0,"W",IB->Font,Lx,1000000,0,false);
		//by=GetRLCHeight(IB->Font->RLC,'W')-line_height;
		line_height=line_height*3/2;
	}

	int lines=(IB->y1-IB->y)/line_height;
	int line=0, ch=0, width=0;
	int cursor_x=0,cursor_y=0;
	if(lines<2) return;
	if(IB->CursPos==0){
		if(action>0)
			cursor_y=action*line_height;
		else
			return;
		if(!IB->Centering){
			InputBox_LMousePressed(SD,cursor_x,cursor_y);
			return;
		}
	}
	while(ch<length){
		bool end_line=false;
		int width0=0;
		int ch0=0;
		//while(ch<length && ch0<128 && base[ch]>32){
		while(ch<length && ch0<128 && (base[ch]<0 || base[ch]>32)){
			if(IB->CursPos==ch)
				cursor_x=width+width0;
			width0+=GetRLCWidth(IB->Font->RLC,base[ch]);
			ch++;
			ch0++;
		}
		if(ch0){
			if(width+width0<LL){
				width+=width0;
			}else{
				ch-=ch0;
				end_line=true;
			}
		}

		width0=0;
		ch0=0;
		//while(ch<length && ch0<128 && base[ch]<=32 && base[ch]!=13 && base[ch]!=10){
		while(ch<length && ch0<128 && (base[ch]>=0 && base[ch]<=32) && base[ch]!=13 && base[ch]!=10){
			if(IB->CursPos==ch)
				cursor_x=width+width0;
			width0+=GetRLCWidth(IB->Font->RLC,base[ch]);
			ch++;
			ch0++;
		}
		if(ch0){
			if(width+width0<LL){
				width+=width0;
			}else{
				ch-=ch0;
				end_line=true;
			}
		}
		if(base[ch]==13 && base[ch+1]==10){
			if(IB->CursPos==ch || IB->CursPos==ch+1)
				cursor_x=width;
			ch+=2;
			end_line=true;
		}
		if(IB->CursPos>=ch)cursor_x=-1;
		//if(IB->CursPos==ch && ch==length)cursor_x=width;
		//new7.04
		if(IB->CursPos==ch && ch==length){
			if(base[ch-1]==10 && base[ch-2]==13){
				by=line_height;
				cursor_x=0;
			}else 
				cursor_x=width;
		}
		//end new
		if(cursor_x>=0){
			cursor_y=(line+action)*line_height+by;
			if(cursor_y<0) return;
			if(!IB->Centering){
				InputBox_LMousePressed(SD,cursor_x,cursor_y);
				return;
			}
		}
		if(end_line || ch>=length){
			if(IB->Centering && cursor_x>=0){
				cursor_y=(line+action)*line_height+by;
				if(cursor_y<0) return;
				cursor_x=cursor_x+(LL-width)/2;
				InputBox_LMousePressed(SD,cursor_x,cursor_y);
				return;
			}
			width=0;
			line++;
		}
	}
};

bool KeysAreFocusedOnInputBox=0;
bool WasAtLeastOneTimeInInputBox=0;

bool InputBox_OnDraw(SimpleDialog* SD){
	InputBox* IB=(InputBox*)SD;
	if(!SD || !SD->Visible) return false;

	SimpleClassConnector* SCC=IB->Connect_edit_string_to_fileld.Get();
	if(SCC){
		if(!IB->_Str){
			IB->_Str=&IB->DefaultStr;
		}
		SCC->_exchange(*IB->_Str);
	}

	if(IB->Active)WasAtLeastOneTimeInInputBox=true;
	if(SD->EditMode){
		IB->Active=false;
		IB->Str="Test InputBox";
	}
	if(IB->Str || IB->_Str){
		if(IB->MouseOver && realLpressed){
			InputBox_LMousePressed(SD,mouseX-IB->LastDrawX,mouseY-IB->LastDrawY);
			if(IB->Selected==-1)
				IB->Selected=IB->CursPos;
		}

		int LL=IB->x1-IB->x;

		char base[2048];

		if(IB->_Str&&!IB->_Str->pchar()) *IB->_Str="";
		strcpy(base,IB->Str?IB->Str:IB->_Str->pchar());
		int length=strlen(IB->Str?IB->Str:IB->_Str->pchar());

		if(IB->Anonim)
			for(int i=0;i<length;i++)base[i]='*';

		if(base[IB->CursPos]==13)
			IB->CursPos+=2;
		if(base[IB->CursPos]==10)
			IB->CursPos-=2;
		if(IB->CursPos>length)
			IB->CursPos=length;

		if(IB->Selected>=0){
			if(base[IB->Selected]==13)
				IB->Selected+=2;
			if(base[IB->Selected]==10)
				IB->Selected-=2;
			if(IB->Selected>length)
				IB->Selected=length;
		}

		TempWindow TW;
		PushWindow(&TW);
		IntersectWindows(IB->x-2,IB->y-2,IB->x1+2,IB->y1+2);

		if(IB->Pict)IB->Pict->Draw(IB->x,IB->y);

		RLCFont** pFont=NULL;
		if(IB->Active)
			pFont=&IB->AFont;
		else
			pFont=&IB->Font;
		if(!*pFont){
			PopWindow(&TW);
			return false;
		}

		int dx=0, dy=0, line=0, ch=0, ch1=0, lines=0;
		int line_height=0, by=0;

		/*int Lx=IB->Multiline?IB->GetWidth():10000;
		line_height=DrawMultilineText(0,0,"W",*pFont,Lx,1000000,0,false);
		by=(GetRLCHeight((*pFont)->RLC,'W')-line_height)/1;
		line_height=line_height*3/2;

		extern FontParam FParam;
		OneFontParam* FP=FParam.GetByGP(int((*pFont)->RLC));
		if(FP)by=-(line_height-FP->Bottom-2);*/

		extern FontParam FParam;
		OneFontParam* FP=FParam.GetByGP(int((*pFont)->RLC));
		if(FP){
			line_height=FP->Bottom-FP->Top+FP->YShift;
			//by=FP->Bottom-line_height+2;
			by=FP->Top-FP->YShift/2;
		}else{
			int Lx=IB->Multiline?IB->GetWidth():10000;
			line_height=DrawMultilineText(0,0,"W",*pFont,Lx,1000000,0,false);
			by=GetRLCHeight((*pFont)->RLC,'W')-line_height;
			line_height=line_height*3/2;
		}

		if(IB->Multiline)
			lines=(IB->y1-IB->y)/line_height;

		do{
			char base1[2048];
			int width=0;
			int cursor_x=-1,cursor_x1=-1;
			bool end_line=false;
			if(IB->Multiline){
				IB->totdx=0;
				ch1=0;
				while(ch<length){
					char OneWord[128];
					int width0=0;
					int ch0=0;
					//while(ch<length && ch0<128 && base[ch]>32){
					while(ch<length && ch0<128 && (base[ch]<0 || base[ch]>32)){
						if(IB->Active && IB->CursPos==ch)
							cursor_x=width+width0;
						if(IB->Active && IB->Selected==ch)
							cursor_x1=width+width0;
						OneWord[ch0]=base[ch];
						width0+=GetRLCWidth((*pFont)->RLC,base[ch]);
						ch++;
						ch0++;
					}
					OneWord[ch0]=0;
					if(ch0){
						if(width+width0<LL){
							if(ch1>0)strcat(base1,OneWord);
							else strcpy(base1,OneWord);
							width+=width0;
							ch1+=ch0;
						}else{
							ch-=ch0;
							break;
						}
					}

					char OneSpace[128];
					width0=0;
					ch0=0;
					//while(ch<length && ch0<128 && base[ch]<=32 && base[ch]!=13 && base[ch]!=10){
					while(ch<length && ch0<128 && (base[ch]>=0 && base[ch]<=32) && base[ch]!=13 && base[ch]!=10){
						if(IB->Active && IB->CursPos==ch)
							cursor_x=width+width0;
						if(IB->Active && IB->Selected==ch)
							cursor_x1=width+width0;
						OneSpace[ch0]=base[ch];
						width0+=GetRLCWidth((*pFont)->RLC,base[ch]);
						ch++;
						ch0++;
					}
					OneSpace[ch0]=0;
					if(ch0){
						if(width+width0<LL){
							if(ch1>0)strcat(base1,OneSpace);
							else strcpy(base1,OneSpace);
							width+=width0;
							ch1+=ch0;
						}else{
							ch-=ch0;
							break;
						}
					}
					if(base[ch]==13 && base[ch+1]==10){
						ch+=2;
						break;
					}
				}
				if(IB->CursPos>=ch)cursor_x=-1;
				if(IB->Selected>=ch)cursor_x1=-1;
				base1[ch1]=0;
			}else{
				strcpy(base1,base);
				if(IB->Active){
					cursor_x=0;
					for(int i=0;i<IB->CursPos;i++)
						cursor_x+=GetRLCWidth((*pFont)->RLC,base1[i]);
				}
				if(IB->Active && IB->Selected>=0){
					cursor_x1=0;
					for(int i=0;i<IB->Selected;i++)
						cursor_x1+=GetRLCWidth((*pFont)->RLC,base1[i]);
				}
				width=GetRLCStrWidth(base1,*pFont);
			}
			if(IB->Active && IB->CursPos==length && ch>=length)
				cursor_x=width;
			if(IB->Active && IB->Selected==length && ch>=length)
				cursor_x1=width;
			if(width<LL && IB->Centering){
				dx=(LL-width)>>1;
				IB->totdx=0;
			}
			dy=line*line_height;
			if(cursor_x>=0){
				cursor_x=cursor_x+dx-IB->totdx;
				if(cursor_x<0){
					IB->totdx+=cursor_x;
					cursor_x=0;
				}
				if(cursor_x>LL){
					IB->totdx=IB->totdx+cursor_x-LL;
					cursor_x=LL;
				}
			}
			int ey=0;
			int cursor_xpos=cursor_x;//new 7.04
			if(IB->Active && IB->Multiline && IB->CursPos==length && ch==length 
				&& base[ch-1]==10 && base[ch-2]==13){
					ey=line_height;
					if(IB->Centering)cursor_xpos=LL/2;
					else cursor_xpos=0;
			}
			ShowString(IB->x+dx-IB->totdx,IB->y+dy-by,base1,*pFont);
			if(IB->Enabled && cursor_xpos>=0 && (GetTickCount()/250)&1){
				base[0]='|';
				base[1]=0;
				ShowString(IB->x+cursor_xpos-1,IB->y+dy+ey-by,base,IB->Font);
			}
			if(IB->Selected>=0){
				if(IB->Multiline){
					if(cursor_x>=0 && cursor_x1==-1){
						if(IB->Selected<ch)
							cursor_x1=0;
						if(IB->Selected>=ch)
							cursor_x1=LL-2*dx;
					}
					else if(cursor_x1>=0 && cursor_x==-1){
						if(IB->CursPos<ch)
							cursor_x=dx;
						if(IB->CursPos>=ch)
							cursor_x=LL-dx;
					}
					else if(cursor_x==-1 && cursor_x1==-1 &&
						((IB->CursPos>=ch && IB->Selected<ch) || (IB->CursPos<ch && IB->Selected>=ch))){
							cursor_x=dx;
							cursor_x1=LL-2*dx;
					}
				}
				if(cursor_x>=0 && cursor_x1>=0 && IB->CursPos!=IB->Selected && IB->Active){
					cursor_x1=cursor_x1+dx-IB->totdx;
					int rect_x=IB->x+(cursor_x<cursor_x1?cursor_x:cursor_x1);
					if(rect_x<IB->x)rect_x=IB->x;
					int rect_x1=IB->x+max(cursor_x,cursor_x1);
					if(rect_x1>IB->x1)rect_x1=IB->x1;
					int by=EngSettings.vInterf.selBorder;
					DWORD col=EngSettings.vInterf.selColor;
					GPS.DrawFillRect(rect_x,IB->y+dy+by+1,rect_x1-rect_x,line_height-2*by,col);
					GPS.FlushBatches();
				}
			}
			line++;
		}while(line<lines && ch<length);
		if(IB->Multiline && ch<length){
			base[ch]=0;
			IB->Str?strcpy(IB->Str,base):IB->_Str->Assign(base);
		}
		PopWindow(&TW);
	}
	return true;
};
////////////////////////// end new /////////////////////////////////////////

/*void InputBox_LMousePressed(SimpleDialog* SD, int x, int y){
	if(!SD->Enabled)return;
	InputBox* IB=(InputBox*)SD;
	x+=IB->totdx;
	int length=strlen(IB->Str?IB->Str:IB->_Str->pchar());
	char base[2048];
	strcpy(base,IB->Str?IB->Str:IB->_Str->pchar());
	int width=IB->x1-IB->x;
	int height0=0, line=0, ch=0, LL=0, dx=0, start_ch=0;
	//bool next_line=false;
	IB->CursPos=length;
	height0=3*GetRLCHeight(IB->Font->RLC,'Y')/2;
	if(IB->Centering){
		if(width>GetRLCStrWidth(base,IB->Font))
			dx=(width-GetRLCStrWidth(base,IB->Font))/2;
		else if(IB->Multiline){
			dx=width/2;
			int ch0=0;
			for(int i=0;i<length;i++){
				if(IB->Anonim)
					LL+=GetRLCWidth(IB->Font->RLC,'*');
				else
					LL+=GetRLCWidth(IB->Font->RLC,base[i]);
				if(i>0 && base[i-1]>32 && base[i]<=32)
					ch0=i;
				if(LL>width){
					dx=LL;
					if(IB->Anonim)
						LL=GetRLCWidth(IB->Font->RLC,'*');
					else
						LL=GetRLCWidth(IB->Font->RLC,base[i]);
					if(ch0){
						for(int j=ch0;j<i;j++){
							if(IB->Anonim)
								LL+=GetRLCWidth(IB->Font->RLC,'*');
							else
								LL+=GetRLCWidth(IB->Font->RLC,base[j]);
						}
						i=ch0;
					}
					dx=(width-dx+LL)/2;
					if(height0*line<=y && height0*(line+1)>y)
						break;
					if(ch0){
						while(base[i]<=32)i++;
						if(IB->Anonim)
							LL=GetRLCWidth(IB->Font->RLC,'*');
						else
							LL=GetRLCWidth(IB->Font->RLC,base[i]);
					}
					start_ch=i;
					line++;
				}else if(i==length-1 || (IB->Enter && base[i-1]==13 && base[i]==10)){
					dx=(width-LL)/2;
					if(i==length-1 || (height0*line<=y && height0*(line+1)>y))
						break;
					start_ch=i+1;
					LL=0;
					line++;
				}
			}
			if(height0*(line+1)<y)
				start_ch=length;
		}
	}
	LL=dx;
	ch=start_ch;
	int ch0=0;
	bool end=false;
	while(ch<length){
		if(IB->Anonim)
			LL+=GetRLCWidth(IB->Font->RLC,'*');
		else 
			LL+=GetRLCWidth(IB->Font->RLC,base[ch]);
		if(y>=height0*line && height0*(line+1)>y && LL>=x){
			//if(ch && ch==start_ch) 
				//ch++;
			if(!end)
				IB->CursPos=ch;
			if(IB->Multiline)
				end=true;
			else
				break;
		}
		if(IB->Multiline && base[ch]<=32 && base[ch+1]>32){
			if(end)break;
			ch0=ch;
		}
		if(IB->Multiline && (LL>width || (IB->Enter && base[ch-2]==13 && base[ch-1]==10))){
			if(ch0){
				ch=ch0;
				ch0=0;
			}
			if(y>=height0*line && height0*(line+1)>y){
				IB->CursPos=ch;
				break;
			}
			start_ch=ch+1;
			LL=0;
			line++;
		}
		ch++;
	}
	Lpressed=0;
};
void InputBox_CursUpDown(SimpleDialog* SD, int action){
	InputBox* IB=(InputBox*)SD;
	int length=strlen(IB->Str?IB->Str:IB->_Str->pchar());
	char base[2048];
	strcpy(base,IB->Str?IB->Str:IB->_Str->pchar());
	int width=IB->x1-IB->x;
	int line_height=3*GetRLCHeight(IB->Font->RLC,'Y')/2;
	int lines=(IB->y1-IB->y)/line_height;
	int line=0, ch=0, LL, LL1;
	if(lines<2) return;
	do{
		LL=0;
		int ch0=0, cur_x=0, cur_y=-1;
		if(IB->CursPos==0){
			if(action>0)
				cur_y=action*line_height;
			else
				return;
			if(!IB->Centering){
				InputBox_LMousePressed(SD,cur_x,cur_y);
				return;
			}
		}
		while(ch<length){
			if(IB->Anonim)
				LL1=GetRLCWidth(IB->Font->RLC,'*');
			else
				LL1=GetRLCWidth(IB->Font->RLC,base[ch]);
			if(width>LL+LL1){
				LL+=LL1;
				if(IB->CursPos==ch)
					cur_x=LL;
				ch++;
				if(ch==length && IB->CursPos==ch)
					cur_x=LL;
				if(IB->Enter && base[ch-2]==13 && base[ch-1]==10)
					break;
			}else{
				if(ch0 && cur_y>=0){
					while(ch>ch0 || base[ch-1]<=32){
						if(IB->Anonim)
							LL-=GetRLCWidth(IB->Font->RLC,'*');
						else
							LL-=GetRLCWidth(IB->Font->RLC,base[ch-1]);
						ch--;
					}
				}
				ch=ch0+1;
				break;
			}
			if((base[ch-1]<=32 && base[ch]>32) || ch==length){
				if(cur_x){
					cur_y=(line+action)*line_height;
					if(cur_y<0) return;
					if(!IB->Centering){
						InputBox_LMousePressed(SD,cur_x,cur_y);
						return;
					}
				}					
				ch0=ch-1;
			}
		}
		if(IB->Centering && cur_y>=0){
			cur_x=cur_x+(width-LL)/2;
			InputBox_LMousePressed(SD,cur_x,cur_y);
			return;
		}
		line++;
	}while(line<lines);
};

bool KeysAreFocusedOnInputBox=0;
bool WasAtLeastOneTimeInInputBox=0;

bool InputBox_OnDraw(SimpleDialog* SD){
	InputBox* IB=(InputBox*)SD;
	if(!SD || !SD->Visible) return false;
	if(IB->Active)WasAtLeastOneTimeInInputBox=true;
	if(SD->EditMode){
		IB->Active=false;
		IB->Str="Test InputBox";
	}
	if(IB->Str || IB->_Str){
		if(IB->MouseOver && realLpressed){
			InputBox_LMousePressed(SD,mouseX-IB->LastDrawX,mouseY-IB->LastDrawY);
			if(IB->Selected==-1)
				IB->Selected=IB->CursPos;
		}
		int LL=IB->x1-IB->x;
		char base[2048];
		if(IB->_Str&&!IB->_Str->pchar()) *IB->_Str="";
		strcpy(base,IB->Str?IB->Str:IB->_Str->pchar());
		int length=strlen(IB->Str?IB->Str:IB->_Str->pchar());
		if(IB->Anonim)
			for(int i=0;i<length;i++)base[i]='*';
		if(IB->CursPos>length) 
			IB->CursPos=length;
		TempWindow TW;
		PushWindow(&TW);
		IntersectWindows(IB->x-2,IB->y-2,IB->x1+2,IB->y1+2);
		if(IB->Pict)IB->Pict->Draw(IB->x,IB->y);
		RLCFont** pFont=NULL;
		if(IB->Active)
			pFont=&IB->AFont;
		else
			pFont=&IB->Font;
		if(!*pFont) return false;
		int dx=0, dy=0, line=0, ch=0, ch1=0, lines=0;
		int line_height=3*GetRLCHeight((*pFont)->RLC,'Y')/2;
		
		int Lx=IB->Multiline?IB->GetWidth():10000;
		//int by=(tb->y+tb->y1-DrawMultilineText(0,0,base1,*pFont,Lx,1000000,tb->Center,false))/2;		
		line_height=DrawMultilineText(0,0,"W",*pFont,Lx,1000000,0,false);
		//DrawMultilineText(tb->x,dy+tb->FontDy,tb->Message,CFN,Lx,1000000,tb->Center,true);
		int by=(GetRLCHeight((*pFont)->RLC,'W')-line_height)/1;
		line_height=line_height*3/2;

		extern FontParam FParam;
		OneFontParam* FP=FParam.GetByGP(int((*pFont)->RLC));
		if(FP){
			//by=-(IB->GetHeight()-FP->Bottom-2);
			by=-(line_height-FP->Bottom-2);
		}		
		
		if(IB->Multiline)
			lines=(IB->y1-IB->y)/line_height;
		do{
			char base1[2048];
			int width=0;
			int ch0=0;
			int cursor_x=-1,cursor_x1=-1;
			if(IB->Multiline){
				IB->totdx=0;
				ch1=0;
				while(ch<length){
					if(IB->Active && IB->CursPos==ch)
						cursor_x=width;
					if(IB->Active && IB->Selected==ch)
						cursor_x1=width;
					width+=GetRLCWidth((*pFont)->RLC,base[ch]);
					if(width<LL){
						if(base[ch]<=32 && base[ch+1]>32)
							ch0=ch;
						base1[ch1]=base[ch];
						ch++;
						if(IB->Enter && base[ch-2]==13 && base[ch-1]==10)
							break;
						ch1++;
					}else{
						if(base[ch]<=32)ch0=0;
						if(ch0)ch1=ch1-(ch-ch0);
						if(IB->Centering){
							width-=GetRLCWidth((*pFont)->RLC,base[ch]);
							if(ch0){
								while(ch>ch0+1){
									width-=GetRLCWidth((*pFont)->RLC,base[ch-1]);
									ch--;
								}
								while(base[ch0]<=32){
									width-=GetRLCWidth((*pFont)->RLC,base[ch0]);
									ch0--;
								}
							}
						}else if(ch0){
							ch=ch0+1;
						}
						if(IB->CursPos>=ch)cursor_x=-1;
						if(IB->Selected>=ch)cursor_x1=-1;
						break;
					}
				}
				base1[ch1]=0;
			}else{
				strcpy(base1,base);
				if(IB->Active){
					cursor_x=0;
					for(int i=0;i<IB->CursPos;i++)
						cursor_x+=GetRLCWidth((*pFont)->RLC,base1[i]);
				}
				if(IB->Active && IB->Selected>=0){
					cursor_x1=0;
					for(int i=0;i<IB->Selected;i++)
						cursor_x1+=GetRLCWidth((*pFont)->RLC,base1[i]);
				}
				width=GetRLCStrWidth(base1,*pFont);
			}
			if(IB->Active && IB->CursPos==length && ch>=length-1)
				cursor_x=width;
			if(IB->Active && IB->Selected==length && ch>=length-1)
				cursor_x1=width;
			if(width<LL && IB->Centering){
				dx=(LL-width)>>1;
				IB->totdx=0;
			}
			dy=line*line_height;
			if(cursor_x>=0){
				cursor_x=cursor_x+dx-IB->totdx;
				if(cursor_x<0){
					IB->totdx+=cursor_x;
					cursor_x=0;
				}
				if(cursor_x>LL){
					IB->totdx=IB->totdx+cursor_x-LL;
					cursor_x=LL;
				}
			}			
			ShowString(IB->x+dx-IB->totdx,IB->y+dy-by,base1,*pFont);
			if(cursor_x>=0 && (GetTickCount()/250)&1){
				base[0]='|';
				base[1]=0;
				ShowString(IB->x+cursor_x-1,IB->y+dy-by,base,IB->Font);
			}
			if(IB->Selected>=0){
				if(IB->Multiline){
					if(cursor_x>=0 && cursor_x1==-1){
						if(IB->Selected<ch)
							cursor_x1=0;
						if(IB->Selected>ch)
							cursor_x1=LL-2*dx;
					}
					else if(cursor_x1>=0 && cursor_x==-1){
						if(IB->CursPos<ch)
							cursor_x=dx;
						if(IB->CursPos>ch)
							cursor_x=LL-dx;
					}
					else if(cursor_x==-1 && cursor_x1==-1 &&
						((IB->CursPos>=ch && IB->Selected<ch) || (IB->CursPos<ch && IB->Selected>=ch))){
							cursor_x=dx;
							cursor_x1=LL-2*dx;
					}
				}
				if(cursor_x>=0 && cursor_x1>=0){
					cursor_x1=cursor_x1+dx-IB->totdx;
					int rect_x=IB->x+(cursor_x<cursor_x1?cursor_x:cursor_x1);
					if(rect_x<IB->x)rect_x=IB->x;
					int rect_x1=IB->x+max(cursor_x,cursor_x1);
					if(rect_x1>IB->x1)rect_x1=IB->x1;
					int by=EngSettings.vInterf.selBorder;
					DWORD col=EngSettings.vInterf.selColor;
					GPS.DrawFillRect(rect_x,IB->y+dy+by+2,rect_x1-rect_x,line_height-2*by,col);
				}
			}
			line++;
		}while(line<lines && ch<length);
		if(IB->Multiline && ch<length){
			base[ch]=0;
			IB->Str?strcpy(IB->Str,base):IB->_Str->Assign(base);
		}
		PopWindow(&TW);
	}
	return true;
};*/
void ClearKeyStack();
extern int NKeys;
extern byte LastAsciiKey;
bool InputBox_OnClick(SimpleDialog* SD){
	if(!SD->Enabled)return false;
	if(!SD->Active)SD->NeedToDraw=true;
	SD->Active=true;
	InputBox* IB=(InputBox*)SD;
	if(IB->MouseOver&&Lpressed&&(IB->Str||IB->_Str)){
		InputBox_LMousePressed(SD,mouseX-IB->LastDrawX,mouseY-IB->LastDrawY);
		if(GetKeyState(VK_SHIFT)&0x8000){
			if(IB->Selected==-1)
				IB->Selected=IB->CursPos;
		}else
			IB->Selected=-1;
	}
	return true;
};
bool InputBox_OnKeyDown(SimpleDialog* SD){
	if(!SD->Enabled)return false;
	SD->NeedToDraw=true;
	InputBox* IB=(InputBox*)SD;
	//IB->Changed=1;
	if(LastKey==65 && (GetKeyState(VK_CONTROL)&0x8000)){ // Ctrl A
		IB->Selected=0;
		IB->CursPos=strlen(IB->Str?IB->Str:IB->_Str->pchar());
		KeyPressed=0;
		LastKey=0;
		return true;
	}else if( ((LastKey==45 || LastKey==67 || LastKey==88) && (GetKeyState(VK_CONTROL)&0x8000))
		&& IB->Selected>=0 && IB->Selected!=IB->CursPos ){ // Ctrl C, Ctrl X, Ctrl Insert
		char cc[2048];
		strcpy(cc,(IB->Str?IB->Str:IB->_Str->pchar())+min(IB->Selected,IB->CursPos));
		cc[abs(IB->Selected-IB->CursPos)]=0;
		ClipBrdSetText(cc);
		if(LastKey==88){
			int start_del=min(IB->CursPos,IB->Selected);
			int end_del=max(IB->CursPos,IB->Selected);
			if(IB->Str)	strcpy(IB->Str+start_del,IB->Str+end_del);
			else IB->_Str->DeleteChars(start_del,end_del-start_del);
			IB->CursPos=start_del;
			IB->Selected=-1;
		}
		KeyPressed=0;
		LastKey=0;
		return true;
	}else if( (LastKey==45 && (GetKeyState(VK_SHIFT)&0x8000))
			|| (LastKey==86 && (GetKeyState(VK_CONTROL)&0x8000)) ){ // Shift Insert, Ctrl V
		int start_ins;
		if(IB->Selected>=0 && IB->Selected!=IB->CursPos)
			start_ins=min(IB->CursPos,IB->Selected);
		else
			start_ins=IB->CursPos;
		int end_ins=max(IB->CursPos,IB->Selected);
		int size=IB->StrMaxLen-strlen(IB->Str?IB->Str:IB->_Str->pchar())+end_ins-start_ins;
		char cc[2048];
		cc[0]=0;
		if(!ClipBrdGetText(cc,(LPDWORD)&size))
			return false;
		char ccc[2048];
		strcpy(ccc,IB->Str?IB->Str:IB->_Str->pchar());
		ccc[start_ins]=0;
		strcat(ccc,cc);
		strcat(ccc,(IB->Str?IB->Str:IB->_Str->pchar())+end_ins);
		IB->Str?strcpy(IB->Str,ccc):IB->_Str->Assign(ccc);
		size=0;
		while(cc[size]!=0)
			size++;
		IB->CursPos=start_ins+size;
		IB->Selected=-1;
		KeyPressed=0;
		LastKey=0;
		return true;
	}else if(LastKey==VK_TAB){
		// new 26.11 //
		const char* cc=IB->GetClassName();
		DWORD ID0=IB->GetObjectGlobalID();
		int N=IB->ParentDS->DSS.GetAmount();
		bool fnd=false;
		if(GetKeyState(VK_SHIFT)&0x8000){
			for(int i=N-1;i>=0;i--){
				SimpleDialog* SD=IB->ParentDS->DSS[i];
				if(fnd){
					const char* ss=SD->GetClassName();
					if(!strcmp(cc,ss)){
						IB->Active=false;
						SD->Active=true;
						break;
					}
				}
				DWORD ID=SD->GetObjectGlobalID();
				if(ID0==ID)fnd=true;
			}
		}else{
			for(int i=0;i<N;i++){
				SimpleDialog* SD=IB->ParentDS->DSS[i];
				if(fnd){
					const char* ss=SD->GetClassName();
					if(!strcmp(cc,ss)){
						IB->Active=false;
						SD->Active=true;
						break;
					}
				}
				DWORD ID=SD->GetObjectGlobalID();
				if(ID0==ID)fnd=true;
			}
		}
		// end new //
		/*if(GetKeyState(VK_SHIFT)&0x8000){
			if(IB->Selected==-1) 
				IB->Selected=IB->CursPos;
		}else
			IB->Selected=-1;
		IB->CursPos+=8;
		if(IB->CursPos>strlen(IB->Str?IB->Str:IB->_Str->pchar()))
			IB->CursPos=strlen(IB->Str?IB->Str:IB->_Str->pchar());*/
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_UP){
		if(IB->Multiline){
			if(GetKeyState(VK_SHIFT)&0x8000){
				if(IB->Selected==-1) 
					IB->Selected=IB->CursPos;
			}else
				IB->Selected=-1;
			InputBox_CursUpDown(SD,-1);
		}
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_DOWN){
		if(IB->Multiline){
			if(GetKeyState(VK_SHIFT)&0x8000){
				if(IB->Selected==-1) 
					IB->Selected=IB->CursPos;
			}else
				IB->Selected=-1;
			InputBox_CursUpDown(SD,1);
		}
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_LEFT){
		if(GetKeyState(VK_SHIFT)&0x8000){
			if(IB->Selected==-1) 
				IB->Selected=IB->CursPos;
		}else{
			if(IB->Selected>=0 && IB->Selected!=IB->CursPos)
				IB->CursPos=min(IB->Selected,IB->CursPos)+1;
			IB->Selected=-1;
		}
		if(IB->CursPos>0)IB->CursPos--;
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_RIGHT){
		if(GetKeyState(VK_SHIFT)&0x8000){
			if(IB->Selected==-1) 
				IB->Selected=IB->CursPos;
		}else{ 
			if(IB->Selected>=0 && IB->Selected!=IB->CursPos)
				IB->CursPos=max(IB->Selected,IB->CursPos)-1;
			IB->Selected=-1;
		}
		if(IB->CursPos<strlen(IB->Str?IB->Str:IB->_Str->pchar()))IB->CursPos++;
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_END){
		if(GetKeyState(VK_SHIFT)&0x8000){
			if(IB->Selected==-1) 
				IB->Selected=IB->CursPos;
		}else
			IB->Selected=-1;
		IB->CursPos=strlen(IB->Str?IB->Str:IB->_Str->pchar());
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_HOME){
		if(GetKeyState(VK_SHIFT)&0x8000){
			if(IB->Selected==-1) 
				IB->Selected=IB->CursPos;
		}else
			IB->Selected=-1;
		IB->CursPos=0;
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_BACK){
		////////////// new //////////////
		if(IB->CursPos>0 || IB->Selected>0){
			int start_back;
			if(IB->Selected>=0 && IB->Selected!=IB->CursPos)
				start_back=min(IB->CursPos,IB->Selected);
			else
				start_back=IB->CursPos-1;
			int end_back=max(IB->CursPos,IB->Selected);
			if(IB->Multiline){
				char cc[2048];
				strcpy(cc,IB->Str?IB->Str:IB->_Str->pchar());
				if(cc[start_back]==10){
					if(cc[start_back-2]==32)start_back-=2;
					else start_back--;
				}
			}
			if(IB->Str)	strcpy(IB->Str+start_back,IB->Str+end_back);
			else IB->_Str->DeleteChars(start_back,end_back-start_back);
			IB->CursPos=start_back;
		}
		IB->Selected=-1;
		return true;
		///////////// end new /////////////////////

		/*if(IB->CursPos>0 || IB->Selected>0){
			int start_back;
			if(IB->Selected>=0 && IB->Selected!=IB->CursPos)
				start_back=min(IB->CursPos,IB->Selected);
			else
				start_back=IB->CursPos-1;
			int end_back=max(IB->CursPos,IB->Selected);
			if(IB->Str)	strcpy(IB->Str+start_back,IB->Str+end_back);
			else IB->_Str->DeleteChars(start_back,end_back-start_back);
			IB->CursPos=start_back;
		}
		IB->Selected=-1;
		return true;*/

	}else
	if(LastKey==46){//VK_DEL
		///////// new ////////////////
		int start_del=0, end_del=0;
		if(IB->Selected>=0 && IB->Selected!=IB->CursPos){
			start_del=min(IB->CursPos,IB->Selected);
			end_del=max(IB->CursPos,IB->Selected);
		}else
		if(IB->CursPos<strlen(IB->Str?IB->Str:IB->_Str->pchar())){
			start_del=IB->CursPos;
			end_del=IB->CursPos+1;
		}
		if(IB->Multiline){
			char cc[2048];
			strcpy(cc,IB->Str?IB->Str:IB->_Str->pchar());
			if(cc[end_del]==13)end_del+=2;
		}
		if(start_del!=end_del){
			if(IB->Str)	strcpy(IB->Str+start_del,IB->Str+end_del);
			else IB->_Str->DeleteChars(start_del,end_del-start_del);
			IB->CursPos=start_del;
		}
		IB->Selected=-1;
		return true;
		/////////// end new ///////////

		/*if(IB->Selected>=0 && IB->Selected!=IB->CursPos){
			int start_del, end_del;
			start_del=min(IB->CursPos,IB->Selected);
			end_del=max(IB->CursPos,IB->Selected);
			if(IB->Str)	strcpy(IB->Str+start_del,IB->Str+end_del);
			else IB->_Str->DeleteChars(start_del,end_del-start_del);
			IB->CursPos=start_del;
		}else{
			if(IB->Str){
				if(IB->CursPos<strlen(IB->Str))strcpy(IB->Str+IB->CursPos,IB->Str+IB->CursPos+1);
			}else{
				if(IB->CursPos<strlen(IB->_Str->pchar()))IB->_Str->DeleteChars(IB->CursPos,1);
			}
		}
		IB->Selected=-1;
		return true;*/

	}else  /////////// new //////////////
	if(LastAsciiKey){
		if(LastAsciiKey>=32){
			LastKey=LastAsciiKey;
			char xx[2];
			xx[1]=0;
			xx[0]=char(LastKey);
			int start_ins;
			if(IB->Selected>=0 && IB->Selected!=IB->CursPos)
				start_ins=min(IB->CursPos,IB->Selected);
			else
				start_ins=IB->CursPos;
			int end_ins=max(IB->CursPos,IB->Selected);
			if(strlen(IB->Str?IB->Str:IB->_Str->pchar())+1<DWORD(IB->StrMaxLen+end_ins-start_ins)){
				char ccc[2048];
				strcpy(ccc,IB->Str?IB->Str:IB->_Str->pchar());
				ccc[start_ins]=0;
				strcat(ccc,xx);
				strcat(ccc,(IB->Str?IB->Str:IB->_Str->pchar())+end_ins);
				IB->Str?strcpy(IB->Str,ccc):IB->_Str->Assign(ccc);
				IB->CursPos=start_ins+1;
				// by vital
				KeyPressed=0;
				LastKey=0;
				// by vital
			}
			IB->Selected=-1;
			return true;
		}else  
		if(IB->Multiline && LastAsciiKey==13){
			LastKey=LastAsciiKey;
			char xx[4];
			xx[3]=0;
			xx[0]=char(32);
			xx[1]=char(LastKey);
			xx[2]=char(10);
			int start_ins;
			if(IB->Selected>=0 && IB->Selected!=IB->CursPos)
				start_ins=min(IB->CursPos,IB->Selected);
			else
				start_ins=IB->CursPos;
			int end_ins=max(IB->CursPos,IB->Selected);
			if(strlen(IB->Str?IB->Str:IB->_Str->pchar())+1<DWORD(IB->StrMaxLen+end_ins-start_ins)){
				char ccc[2048];
				strcpy(ccc,IB->Str?IB->Str:IB->_Str->pchar());
				ccc[start_ins]=0;
				strcat(ccc,xx);
				strcat(ccc,(IB->Str?IB->Str:IB->_Str->pchar())+end_ins);
				IB->Str?strcpy(IB->Str,ccc):IB->_Str->Assign(ccc);
				IB->CursPos=start_ins+3;
				// by vital
				KeyPressed=0;
				LastKey=0;
				// by vital
			}
			IB->Selected=-1;
			return true;
		};    
	}; ////////// end new /////////////

	/*else{
		if(LastAsciiKey&&LastAsciiKey>=32){
			LastKey=LastAsciiKey;
			char xx[2];
			xx[1]=0;
			xx[0]=char(LastKey);
			int start_ins;
			if(IB->Selected>=0 && IB->Selected!=IB->CursPos)
				start_ins=min(IB->CursPos,IB->Selected);
			else
				start_ins=IB->CursPos;
			int end_ins=max(IB->CursPos,IB->Selected);
			if(strlen(IB->Str?IB->Str:IB->_Str->pchar())+1<DWORD(IB->StrMaxLen+end_ins-start_ins)){
				char ccc[2048];
				strcpy(ccc,IB->Str?IB->Str:IB->_Str->pchar());
				ccc[start_ins]=0;
				strcat(ccc,xx);
				strcat(ccc,(IB->Str?IB->Str:IB->_Str->pchar())+end_ins);
				IB->Str?strcpy(IB->Str,ccc):IB->_Str->Assign(ccc);
				IB->CursPos=start_ins+1;
			}
			IB->Selected=-1;
			return true;
		};
	};*/

	return false;
};
InputBox::InputBox(){
	NeedToDraw=true;
	Str=NULL;
	_Str=NULL;
	OnDraw=&InputBox_OnDraw;
	OnClick=&InputBox_OnClick;
	OnKeyDown=&InputBox_OnKeyDown;
	Selected=-1;
	Enter=false;
	Multiline=false;
	Centering=false;
};
InputBox* ParentFrame::addInputBox(SimpleDialog* Parent,
									 int x,int y,char* str,
									 int Len,
									 SQPicture* Panel,
									 RLCFont* RFont,
									 RLCFont* AFont){
	ClearKeyStack();
	LastAsciiKey=0;
	InputBox* IB=new InputBox;
	AddDialog(IB);
	IB->NeedToDraw=true;
	IB->Parent=Parent;
	IB->x=x+BaseX;
	IB->y=y+BaseY;
	IB->Pict=Panel;
	IB->Font=RFont;
	IB->AFont=AFont;
	IB->StrMaxLen=Len;
	IB->Str=str;
	IB->_Str=NULL;
	IB->x1=IB->x+Panel->GetLx()-1;
	IB->y1=IB->y+Panel->GetLy()-1;
	IB->OnDraw=&InputBox_OnDraw;
	IB->OnClick=&InputBox_OnClick;
	IB->OnKeyDown=&InputBox_OnKeyDown;
	IB->Centering=false;
	IB->CursPos=strlen(str);
	IB->totdx=0;
	IB->Anonim=0;
	IB->Multiline=0;
	IB->ParentDS=this;
	return IB;
};
InputBox* ParentFrame::addInputBox(SimpleDialog* Parent,
									 int x,int y,char* str,
									 int Len,
									 int Lx,int Ly,
									 RLCFont* RFont,
									 RLCFont* AFont,
									 bool Centering){
	ClearKeyStack();
	LastAsciiKey=0;
	InputBox* IB=new InputBox;
	AddDialog(IB);
	IB->NeedToDraw=true;
	IB->Parent=Parent;
	IB->x=x+BaseX;
	if(Centering)IB->x-=Lx>>1;
	IB->y=y+BaseY;
	IB->Pict=NULL;
	IB->Font=RFont;
	IB->AFont=AFont;
	IB->StrMaxLen=Len;
	IB->Str=str;
	IB->_Str=NULL;
	IB->x1=IB->x+Lx;
	IB->y1=IB->y+Ly;
	IB->OnDraw=&InputBox_OnDraw;
	IB->OnClick=&InputBox_OnClick;
	IB->OnKeyDown=&InputBox_OnKeyDown;
	IB->Centering=Centering;
	IB->CursPos=strlen(str);
	IB->totdx=0;
	IB->Anonim=0;
	IB->Multiline=0;
	IB->ParentDS=this;
	return IB;
};
InputBox* ParentFrame::addInputBox(SimpleDialog* Parent,
								   int x,int y,_str* str,
								   int Len,
								   int Lx,int Ly,
								   RLCFont* RFont,
								   RLCFont* AFont,
								   bool Centering){
	ClearKeyStack();
	LastAsciiKey=0;
	InputBox* IB=new InputBox;
	AddDialog(IB);
	IB->NeedToDraw=true;
	IB->Parent=Parent;
	IB->x=x+BaseX;
	if(Centering)IB->x-=Lx>>1;
	IB->y=y+BaseY;
	IB->Pict=NULL;
	IB->Font=RFont;
	IB->AFont=AFont;
	IB->StrMaxLen=Len;
	IB->_Str=str;
	IB->Str=NULL;
	IB->x1=IB->x+Lx;
	IB->y1=IB->y+Ly;
	IB->OnDraw=&InputBox_OnDraw;
	IB->OnClick=&InputBox_OnClick;
	IB->OnKeyDown=&InputBox_OnKeyDown;
	IB->Centering=Centering;
	IB->CursPos=strlen(str->pchar());
	IB->totdx=0;
	IB->Anonim=0;
	IB->Multiline=0;
	IB->ParentDS=this;
	return IB;
};
InputBox* ParentFrame::addInputBox(SimpleDialog* Parent,
									 int x,int y,char* str,
									 int Len,
									 int Lx,int Ly,
									 RLCFont* RFont,
									 RLCFont* AFont){
	ClearKeyStack();
	LastAsciiKey=0;
	InputBox* IB=new InputBox;
	AddDialog(IB);
	IB->NeedToDraw=true;
	IB->Parent=Parent;
	IB->x=x+BaseX;
	IB->y=y+BaseY;
	IB->Pict=NULL;
	IB->Font=RFont;
	IB->AFont=AFont;
	IB->StrMaxLen=Len;
	IB->Str=str;
	IB->_Str=NULL;
	IB->x1=IB->x+Lx-1;
	IB->y1=IB->y+Ly-1;
	IB->OnDraw=&InputBox_OnDraw;
	IB->OnClick=&InputBox_OnClick;
	IB->OnKeyDown=&InputBox_OnKeyDown;
	IB->Centering=false;
	IB->CursPos=strlen(str);
	IB->totdx=0;
	IB->Anonim=0;
	IB->Multiline=0;
	IB->ParentDS=this;
	return IB;
};
//------end of class InputBox----------------//
////////////////////////////////////////////////////////////////////////////////////////
#pragma pack( pop )
#endif 