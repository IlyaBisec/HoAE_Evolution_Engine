/*               Fast raster graphics routine
 *  
 *  This module presents several useful procedures for fast output,
 * using RLC graphics format (see the discription below). Before the
 * wor you must call SetRlcWindow to set properly the screen 
 * capabilities.After this you mast call SetScreenPtr to set the
 * output distanation. Uou can use LoadRLC to load the set of RLC
 * pictures into the RLCTable structure. Then you can draw the 
 * picture on the screen wit the procedure ShowRLCItem 
 */
#include "stdheader.h"

void DrawRotatedPicture(int x,int y,int File,int Sprite,byte Nat,
						int dx,int dy,float Angle,float sx,float sy);

extern int SCRSizeX;
extern int SCRSizeY;
extern int RSCRSizeX;
extern int RSCRSizeY;
extern int COPYSizeX;
extern int LOADED;
CEXPORT
void ShowCharUNICODE(int x,int y,byte* strptr,lpRLCFont lpr);
byte PAL1[256];
byte PAL2[256];
byte PAL3[256];
byte PAL4[256];
byte PAL5[256]; 
byte PAL6[256];
byte PAL7[256];
void ErrM(char* s);
extern void* offScreenPtr;


typedef RLCHeader* lpRLCHeader;

typedef xRLCTable* RLCTable; 
typedef RLCTable* lpRLCTable;
RLCFont::RLCFont(int GP_Index){
#ifdef _USE3D
	SetColor( 0xFFFFFFFF );
#endif // _USE3D
	RLC=(RLCTable)GP_Index;
	FirstSymbol=0;
	LastSymbol=0;
	Options=0;
	ShadowGP=-1;
};
void RLCFont::SetGPIndex(int n){
	RLC=(RLCTable)n;
	FirstSymbol=0;
	LastSymbol=0;
	Options=0;
};
RLCFont::RLCFont(){
#ifdef _USE3D
	SetColor( 0xFFFFFFFF );
#endif // _USE3D
	RLC=NULL;
	FirstSymbol=0;
	LastSymbol=0;
	Options=0;
	ShadowGP=-1;
};
RLCFont::~RLCFont(){
	if(RLC&&int(RLC)>4096)free(RLC);
	RLC=NULL;
};
typedef RLCFont* lpRLCFont;
//Window parameters

#ifndef _USE3D
// -- Inserted by Silver ---06.02.2003
// -- reason:
//		Clip area should be set by GPS.SetClipArea( x, y, w, h ); call
// -- end of change -- 06.02.2003
int WindX	= 0; 
int WindY	= 0;
int WindLx	= 0;
int WindLy	= 0;
int WindX1	= 0;
int WindY1	= 0; 
#endif // !_USE3D


int ScrWidth;
int ScrHeight;
int BytesPerPixel=1;
void* ScreenPtr=NULL;
void* RealScreenPtr=NULL;
extern int RealLx;
extern int RealLy;
void InitRLCWindows(){
	Rct vp = GPS.GetClipArea();
	GPS.SetClipArea( vp.x, vp.y, RealLx, RealLy );
	ScrWidth=SCRSizeX;
	ScrHeight=SCRSizeY;
};
/*  Creating window
 */
static int cntr;
static bool InCycle;

int ST_WX;
int ST_WY;
int ST_WLX;
int ST_WLY;
int ST_SCW;
CEXPORT
void SetRLCWindow(int x,int y,int lx,int ly,int slx);
void StoreWindow(){
	Rct vp = GPS.GetClipArea();
	ST_WX	= vp.x;
	ST_WY	= vp.y;
	ST_WLX	= vp.w;
	ST_WLY	= vp.h;
	ST_SCW=ScrWidth;
};
void RestoreWindow(){
	SetRLCWindow(ST_WX,ST_WY,ST_WLX,ST_WLY,ST_SCW);
};
void SetWind(int x,int y,int Lx,int Ly){
	SetRLCWindow(x,y,Lx,Ly,ScrWidth);
};
CEXPORT
void SetRLCWindow(int x,int y,int lx,int ly,int slx)
{
	if (x < 0) { lx += x; x = 0; }
	if (y < 0) { ly += y; y = 0; }
	if (lx > RealLx) lx = RealLx;
	if (ly > RealLy) ly = RealLy;
	GPS.SetClipArea( x, y, lx, ly );
}
//  Setting proper value of the screen pointer
void SetScreenPtr(void)
{
#ifdef COPYSCR
	ScreenPtr=offScreenPtr;
	RealScreenPtr=ddsd.lpSurface;
#else
	ScreenPtr=ddsd.lpSurface;
#endif
}

void ClearScreen();

//Showing RLC image with clipping
#ifndef _USE3D
void ShowRLC(int x,int y,void* PicPtr)
{
	
}
//End of RLC with clipping
//Showing inverse RLC image with clipping
void ShowRLCi(int x,int y,void* PicPtr)
{
	
}
//End of RLC with clipping & with palette
void ShowRLCpal(int x,int y,void* PicPtr,byte* pal)
{
	
}
//End of RLC with clipping & encoding
//Showing inverse RLC image with clipping & encodint
void ShowRLCipal(int x,int y,void* PicPtr,byte* pal)
{
}
//End of inverted RLC with clipping & encoding

//End of RLC with clipping & with palette->fon
void ShowRLCfonpal(int x,int y,void* PicPtr,byte* pal)
{
}
//End of RLC with clipping & encoding
//Showing inverse RLC image with clipping & encodint
void ShowRLCifonpal(int x,int y,void* PicPtr,byte* pal)
{
}
//End of inverted RLC with clipping & encoding->fon


//End of RLC with clipping & with palette(half-transparent fog)
void ShowRLChtpal(int x,int y,void* PicPtr,byte* pal)
{	
}
//End of RLC with clipping & encoding
//Showing inverse RLC image with clipping & encodint(half-transparent fog)
void ShowRLCihtpal(int x,int y,void* PicPtr,byte* pal)
{
}
//End of inverted RLC with clipping & encoding(half-transparent fog)

void ShowRLCp1(int x,int y,void* PicPtr)
{

}
void ShowRLCp2(int x,int y,void* PicPtr)
{

}
void ShowRLCp3(int x,int y,void* PicPtr)
{

}
void ShowRLCp4(int x,int y,void* PicPtr)
{

}
void ShowRLCp5(int x,int y,void* PicPtr)
{

}
void ShowRLCp6(int x,int y,void* PicPtr)
{

}
void ShowRLCp7(int x,int y,void* PicPtr)
{

}
void ShowRLCip1(int x,int y,void* PicPtr)
{

}
void ShowRLCip2(int x,int y,void* PicPtr)
{

}
void ShowRLCip3(int x,int y,void* PicPtr)
{

}
void ShowRLCip4(int x,int y,void* PicPtr)
{

}
void ShowRLCip5(int x,int y,void* PicPtr)
{

}
void ShowRLCip6(int x,int y,void* PicPtr)
{

}
void ShowRLCip7(int x,int y,void* PicPtr)
{

}
void ShowRLCShadow(int x,int y,void* PicPtr)
{

}
void ShowRLCiShadow(int x,int y,void* PicPtr)
{

}
void ShowRLCWhite(int x,int y,void* PicPtr)
{

}
void ShowRLCDarkN(int x,int y,void* PicPtr,int N)
{

};
void ShowRLCiDarkN(int x,int y,void* PicPtr,int N)
{

};
void ShowRLCRedN(int x,int y,void* PicPtr,int N)
{

};
void ShowRLCiRedN(int x,int y,void* PicPtr,int N)
{

};
void ShowRLCItemDarkN(int x,int y,lpRLCTable lprt,int n,int Ints)
{

};
void ShowRLCItemPal(int x,int y,lpRLCTable lprt,int n,byte* Pal)
{

};
void ShowRLCItemGrad(int x,int y,lpRLCTable lprt,int n,byte* Pal)
{

};
void ShowRLCItemRedN(int x,int y,lpRLCTable lprt,int n,int Ints)
{

};
void ShowRLCWFog(int x,int y,void* PicPtr)
{

}
void ShowRLCiWhite(int x,int y,void* PicPtr)
{

}
void ShowRLCiWFog(int x,int y,void* PicPtr)
{

}
void ShowRLCiTrans8(int x,int y,void* PicPtr)
{
	
}
void ShowRLCTrans8(int x,int y,void* PicPtr)
{

}
void ShowRLCDark(int x,int y,void* PicPtr)
{

}
void ShowRLCiDark(int x,int y,void* PicPtr)
{

}
void ShowRLCBlue(int x,int y,void* PicPtr)
{

}
void ShowRLCiBlue(int x,int y,void* PicPtr)
{

}
void ShowRLCFire(int x,int y,void* PicPtr)
{

}
void ShowRLCiFire(int x,int y,void* PicPtr)
{

}

#endif // !_USEGP

//          Loading RLC file
bool LoadRLC(LPCSTR lpFileName,RLCTable *RLCtbl)
{
	return false;
}

//       Showing RLC item
void ShowRLCItem(int x,int y,lpRLCTable lprt,int n,byte nt)
{
};

#ifndef _USE3D
void ShowRLCItemTrans8(int x,int y,lpRLCTable lprt,int n){
    if(n<4096){
		ShowRLCTrans8(x,y,(void*)((*lprt)->OfsTable[n]));
	}else{
		ShowRLCiTrans8(x,y,(void*)((*lprt)->OfsTable[n-4096]));
	};
};
#endif // _USE3D

void ShowRLCItemShadow(int x,int y,lpRLCTable lprt,int n)
{
#ifndef _USE3D
	if(n<4096){
		ShowRLCShadow(x,y,(void*)((*lprt)->OfsTable[n]));
	}else{
		ShowRLCiShadow(x,y,(void*)((*lprt)->OfsTable[n-4096]));
	};
#endif // _USE3D
};

#ifndef _USE3D
void ShowRLCItemMutno(int x,int y,lpRLCTable lprt,int n)
{
	if(n<4096){
		ShowRLCWFog(x,y,(void*)((*lprt)->OfsTable[n]));
	}else{
		ShowRLCiWFog(x,y,(void*)((*lprt)->OfsTable[n-4096]));
	};
};
void ShowRLCItemDark(int x,int y,lpRLCTable lprt,int n)
{
	if(n<4096){
		ShowRLCDark(x,y,(void*)((*lprt)->OfsTable[n]));
	}else{
		ShowRLCiDark(x,y,(void*)((*lprt)->OfsTable[n-4096]));
	};
};
void ShowRLCItemBlue(int x,int y,lpRLCTable lprt,int n)
{
	if(n<4096){
		ShowRLCBlue(x,y,(void*)((*lprt)->OfsTable[n]));
	}else{
		ShowRLCiBlue(x,y,(void*)((*lprt)->OfsTable[n-4096]));
	};
};
void ShowRLCItemFired(int x,int y,lpRLCTable lprt,int n)
{
	if(n<4096){
		ShowRLCFire(x,y,(void*)((*lprt)->OfsTable[n]));
	}else{
		ShowRLCiFire(x,y,(void*)((*lprt)->OfsTable[n-4096]));
	};
};

#endif //_!USE3D

int GetRLCWidth(RLCTable lpr,byte n)
{
	int GPID=int(lpr);
	if(GPID<4096){
		if(n==32)return GPS.GetGPWidth(GPID,'c');//GEC - pro probel
		if(n==9)return GPS.GetGPWidth(GPID,'c')*4;
		return GPS.GetGPWidth(GPID,n);
	};
	if (n<lpr->SCount) return (*((lpRLCHeader)((void*)(lpr->OfsTable[n])))).SizeX;
	else return 0;
}
int GetCHEX(byte c){
	if(c>='0'&&c<='9')return c-'0';
	if(c>='a'&&c<='z')return c+10-'a';
	if(c>='A'&&c<='Z')return c+10-'A';
	return 0;
};
int GetRLCWidthUNICODE(RLCTable lpr,byte* strptr,int* L){
	if(strptr[0]==31){
		if(L)*L=1;
		return 0;
	}
	if(strptr[0]==SIGNBYTE){
		if(L)*L=5;
		int idx=(GetCHEX(strptr[1])<<12)+
			    (GetCHEX(strptr[2])<<8)+
				(GetCHEX(strptr[3])<<4)+
				GetCHEX(strptr[4]);
		int GPID=int(lpr);
		if(GPID<4096){
			UNICODETABLE* UT=GPS.UNITBL[GPID];
			if(!UT)return 0;
			for(int j=0;j<UT->NTables;j++){
				if(idx>=UT->USET[j].Start){
					int v=idx-UT->USET[j].Start;
					if(v<UT->USET[j].NSymbols){
						v+=UT->USET[j].GP_Start;
						return GPS.GetGPWidth(UT->USET[j].GPID,v);
					};
				};
			};
			return 0;
		}else return 0;
	}else{
		if(L)*L=1;
		return GetRLCWidth(lpr,strptr[0]);
	};

};
CEXPORT
int GetRLCHeight(RLCTable lpr,byte n)
{
	int GPID=int(lpr);
	if(GPID<4096){
		return GPS.GetGPHeight(GPID,n);
	};
	if (n<lpr->SCount) return (*((lpRLCHeader)((void*)(lpr->OfsTable[n])))).SizeY;
	else return 0;
}
void RegisterRLCFont(lpRLCFont lrf,RLCTable lpr,int fir)
{
	lrf->FirstSymbol=fir;
	lrf->LastSymbol=lpr->SCount+fir-1;
	lrf->RLC=lpr;
}
void CheckFontColor(lpRLCFont lpf){
	int GPID=int(lpf->RLC);
};
void ShowCharEx(int x,int y,char c,lpRLCFont lpf, bool Horizontal){

	int GPID=int(lpf->RLC);
	if(GPID<4096)
	{
		DWORD C=GPS.GetCurrentDiffuse();
		DWORD C1=lpf->GetColor();
		if(C!=0xFFFFFFFF){
			C1=ModDWORD(C,C1);
		}
		GPS.SetCurrentDiffuse( C1 );

		if(Horizontal)
		{
			GPS.ShowGP(x,y,GPID,byte(c),0);
		}
		else
		{
			DrawRotatedPicture(x,y,GPID,byte(c),0,0,0,c_PI*3/2,1,1);
		}		
		GPS.SetCurrentDiffuse(C);
		return;
	};
	ShowRLCItem(x,y,&(lpf->RLC),c,0);
};

void ShowChar(int x,int y,char c,lpRLCFont lpf){
	ShowCharEx(x,y,c,lpf,1);
}
CEXPORT
void ShowCharUNICODE_ex(int x,int y,byte* strptr,lpRLCFont lpr,bool Horizontal){
	if(strptr[0]==SIGNBYTE){
		int idx=(GetCHEX(strptr[1])<<12)+
			    (GetCHEX(strptr[2])<<8)+
				(GetCHEX(strptr[3])<<4)+
				GetCHEX(strptr[4]);
		int GPID=int(lpr->RLC);
		if(GPID<4096){
			UNICODETABLE* UT=GPS.UNITBL[GPID]; 
			if(UT){
				for(int j=0;j<UT->NTables;j++){
					if(idx>=UT->USET[j].Start){
						int v=idx-UT->USET[j].Start;
						if(v<UT->USET[j].NSymbols)
						{
							v+=UT->USET[j].GP_Start;
							GPID=UT->USET[j].GPID;
							int xx=x+UT->USET[j].DX;
							int yy=y+UT->USET[j].DY;					

							if(lpr->ShadowGP!=-1)
							{
								DWORD CC=GPS.GetCurrentDiffuse();
								GPS.SetCurrentDiffuse(0xFF462814);
								if(Horizontal)
								{
									GPS.ShowGP(xx+lpr->ShadowDx,yy+lpr->ShadowDy,GPID,v,0);
								}
								else
								{
									DrawRotatedPicture(xx+lpr->ShadowDy,yy+lpr->ShadowDx,GPID,v,0,0,0,c_PI/2,1,1);
								}
								GPS.SetCurrentDiffuse(CC);
							}

							DWORD C=GPS.GetCurrentDiffuse();
							DWORD C1=lpr->GetColor();
							if(C!=0xFFFFFFFF){
								C1=ModDWORD(C,C1);
							}
							GPS.SetCurrentDiffuse( C1 );

							if(Horizontal)
							{
								GPS.ShowGP(xx,yy,GPID,v,0);
							}
							else
							{
								DrawRotatedPicture(xx+lpr->ShadowDy,yy+lpr->ShadowDx,GPID,v,0,0,0,c_PI/2,1,1);
							}

							GPS.SetCurrentDiffuse(C);

							return;
						};
					};
				};
			};
		};
	}else{
		if(lpr->ShadowGP!=-1){
			DWORD CC=GPS.GetCurrentDiffuse();
			GPS.SetCurrentDiffuse(0xFF462814);
			if(Horizontal){
				GPS.ShowGP(x+lpr->ShadowDx,y+lpr->ShadowDy,lpr->ShadowGP,strptr[0],0);
			}else{
				DrawRotatedPicture(x-lpr->ShadowDx,y-lpr->ShadowDy,lpr->ShadowGP,strptr[0],0,0,0,c_PI/2,1,1);
			}			
            GPS.SetCurrentDiffuse(CC);
		};
		ShowCharEx(x,y,strptr[0],lpr,Horizontal);
	};
};
CEXPORT
void ShowCharUNICODE(int x,int y,byte* strptr,lpRLCFont lpr){
	ShowCharUNICODE_ex(x,y,strptr,lpr,true);
}

void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf, bool Horizontal)
{
	if (lps==NULL) return;
	int GPID=int(lpf->RLC);
	if(GPID<4096){
		if (GPS.GetDrawMode() == dmText)
		{
			GPS.SetDrawMode( dmSprites );
		}
		else
		{
			GPS.FlushBatches();			 
		}
		//GPS.SetCurrentDiffuse( lpf->GetColor() );


		byte	ch; 
		int		i=0;
		do 
		{
			ch=lps[i];
			if (ch >= 32||ch==9)
			{
				int L=1;
				if(Horizontal){
					ShowCharUNICODE(x,y,(byte*)(lps+i),lpf);				
					x+=GetRLCWidthUNICODE(lpf->RLC,(byte*)(lps+i),&L);
				}else{
					ShowCharUNICODE_ex(x,y,(byte*)(lps+i),lpf,0);				
					y-=GetRLCWidthUNICODE(lpf->RLC,(byte*)(lps+i),&L);
				}
				i+=L-1;
				//if(ch==32)x+=GPS.GetGPWidth(GPID,'c');
			}
			i++;
		}while(ch);
		//GPS.SetCurrentDiffuse( 0xFFFFFFFF );
		return;
	};
	byte	ch;
	int		i=0;
	do 
	{
		ch=lps[i];
		if(ch!=0) 
		{
			ShowRLCItem(x,y,&(lpf->RLC),lpf->FirstSymbol+ch,0);
			x+=GetRLCWidth(lpf->RLC,lpf->FirstSymbol+ch);
		}
		i++;
	}while(ch);
	GPS.SetCurrentDiffuse( 0xFFFFFFFF );
	GPS.SetDrawMode( dmText );
}

void ShowString(int x, int y, LPCSTR lps, lpRLCFont lpf)
{
	ShowStringEx(x,y,lps,lpf,1);	
}
int GetRLCStrWidth(char* str,lpRLCFont lpf){
	if(!str)return 0;
	int L=0;
	int sl=strlen(str);
	for(int i=0;i<sl;i++){
		int LL=1;
		L+=GetRLCWidthUNICODE(lpf->RLC,(byte*)(str+i),&LL);
		i+=LL-1;
	};
	return L;
};
void ShowShadString(int x,int y,LPCSTR lps,lpRLCFont lpf)
{
	if (lps==NULL) return;
	byte	ch;
	int		i=0;
	do 
	{
		ch=lps[i];
		if(ch!=0) 
		{
			ShowRLCItemShadow(x+2,y+2,&(lpf->RLC),lpf->FirstSymbol+ch);
			ShowRLCItem(x,y,&(lpf->RLC),lpf->FirstSymbol+ch,0);
			int LL=1;
			x+=GetRLCWidthUNICODE(lpf->RLC,(byte*)(lps+i),&LL);
			i+=LL-1;
		}
		i++;
	}
    while(ch-0);
}
void LoadPalettes(){
	ResFile f1=RReset("pal1.dat");
	RBlockRead(f1,PAL1,256);
	RClose(f1);
	f1=RReset("pal2.dat");
	RBlockRead(f1,PAL2,256);
	RClose(f1);
	f1=RReset("pal3.dat");
	RBlockRead(f1,PAL3,256);
	RClose(f1);
	f1=RReset("pal4.dat");
	RBlockRead(f1,PAL4,256);
	RClose(f1);
	f1=RReset("pal5.dat");
	RBlockRead(f1,PAL5,256);
	RClose(f1);
	f1=RReset("pal6.dat");
	RBlockRead(f1,PAL6,256);
	RClose(f1);
	f1=RReset("pal7.dat");
	RBlockRead(f1,PAL7,256);
	RClose(f1);
};
RLCFont::RLCFont(char* Name){
	SetColor( 0xFFFFFFFF );
	LoadRLC(Name,&RLC);
	RegisterRLCFont(this,RLC,0);
	Options=0;
	ShadowGP=-1;
};
void RLCFont::SetRedColor(){
	Options=32;
	//SetStdShadow();
};
void RLCFont::SetWhiteColor(){
	Options=16;
	//SetStdShadow();
};
void RLCFont::SetBlackColor(){
	Options=0;
};
void RLCFont::SetColorTable(int n){
	Options=n<<4;
	//SetStdShadow();
};
/* End of Graphics routine */