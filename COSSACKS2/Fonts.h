//fonts
#ifndef FONTS_USER
#define FONTS_API __declspec(dllexport)
#else
#define FONTS_API __declspec(dllimport)
#endif
extern FONTS_API RLCTable t16x16r;
extern FONTS_API RLCFont  fn10;
extern FONTS_API RLCFont  fn8;
extern FONTS_API RLCFont  WhiteFont;
extern FONTS_API RLCFont  YellowFont;
extern FONTS_API RLCFont  RedFont;
extern FONTS_API RLCFont  BlackFont;
extern FONTS_API RLCFont  OrangeFont;
extern FONTS_API RLCFont  GrayFont;
extern FONTS_API RLCFont  SmallWhiteFont;
extern FONTS_API RLCFont  SmallYellowFont;
extern FONTS_API RLCFont  SmallRedFont;
extern FONTS_API RLCFont  SmallBlackFont;
extern FONTS_API RLCFont  SmallOrangeFont;
extern FONTS_API RLCFont  SmallGrayFont;
extern FONTS_API RLCFont  SmallWhiteFont1;
extern FONTS_API RLCFont  SmallYellowFont1;
extern FONTS_API RLCFont  SmallRedFont1;
extern FONTS_API RLCFont  SmallGrayFont1;
extern FONTS_API RLCFont  SmallBlackFont1;
extern FONTS_API RLCFont  BigWhiteFont;
extern FONTS_API RLCFont  BigYellowFont;
extern FONTS_API RLCFont  BigRedFont;
extern FONTS_API RLCFont  BigBlackFont;
extern FONTS_API RLCFont  SpecialWhiteFont;
extern FONTS_API RLCFont  SpecialYellowFont;
extern FONTS_API RLCFont  SpecialRedFont;
extern FONTS_API RLCFont  SpecialBlackFont;
extern FONTS_API RLCFont  SpecialGrayFont;

extern FONTS_API RLCFont  fon40y1;
extern FONTS_API RLCFont  fon40y5;
extern FONTS_API RLCFont  fon30y1;
extern FONTS_API RLCFont  fon30y3;
extern FONTS_API RLCFont  fon30y5;
extern FONTS_API RLCFont  fon18w;
extern FONTS_API RLCFont  fon18y3;
extern FONTS_API RLCFont  fon18y5;
extern FONTS_API RLCFont  fon16y1;
extern FONTS_API RLCFont  fon16y3;
extern FONTS_API RLCFont  fon16y5;
extern FONTS_API RLCFont  fon13y2;
extern FONTS_API RLCFont  fonG60y5;
extern FONTS_API RLCFont  fonG60w;

extern FONTS_API RLCFont fonMenuText[7];

#ifndef FONTS_USER
void ShowProgressBar(char* Message,int N,int NMax);
void ShowTable(char* Message,char* Hint);
#endif