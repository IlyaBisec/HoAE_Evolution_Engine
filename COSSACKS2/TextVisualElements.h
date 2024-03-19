class TextVisualElement:public BaseClass{
public:
	virtual bool Assign(char* TextCommand){return false;};
	virtual void Draw(int x,int y,int Lx,int Ly,DWORD Color){};
	virtual void GetBestSize(int FontSizeY,int& Lx,int& Ly,int& LowMarkPos){};
	SAVE(TextVisualElement);
	ENDSAVE;
};
//{UNIT UnitID}, example {UNIT FrnFuz(FR)}
class TVE_UnitPreview:public BaseClass{
public:
    GeneralObject* GO;
	virtual bool Assign(char* TextCommand);
	virtual void Draw(int x,int y,int Lx,int Ly,DWORD Color);
	virtual void GetBestSize(int FontSizeY,int& Lx,int& Ly,int& LowMarkPos);
	SAVE(TVE_UnitPreview);
	REG_PARENT(TextVisualElement);
	ENDSAVE;
};
class TVE_FacturePreview:public BaseClass{
public:
	int texid;
	virtual bool Assign(char* TextCommand);
	virtual void Draw(int x,int y,int Lx,int Ly,DWORD Color);
	virtual void GetBestSize(int FontSizeY,int& Lx,int& Ly,int& LowMarkPos);
	SAVE(TVE_FacturePreview);
	REG_PARENT(TextVisualElement);
	ENDSAVE;
};
class TVE_StdTexturePreview:public BaseClass{
public:
	int texid;
	virtual bool Assign(char* TextCommand);
	virtual void Draw(int x,int y,int Lx,int Ly,DWORD Color);
	virtual void GetBestSize(int FontSizeY,int& Lx,int& Ly,int& LowMarkPos);
	SAVE(TVE_StdTexturePreview);
	REG_PARENT(TextVisualElement);
	ENDSAVE;
};
//{#barXXXXXXXX SizeX} or {#barXXXXXXXX}
//example {#barFF00FF00 32} or {#barFF00FF00}
class TVE_ColorRect:public BaseClass{
public:
	DWORD COLOR;
	int SizeX;
	virtual bool Assign(char* TextCommand);
	virtual void Draw(int x,int y,int Lx,int Ly,DWORD Color);
	virtual void GetBestSize(int FontSizeY,int& Lx,int& Ly,int& LowMarkPos);
	SAVE(TVE_ColorRect);
	REG_PARENT(TextVisualElement);
	ENDSAVE;
};
//{tex texturename x y lx ly  u v u1 v1  C1 C2 C3 C4   ActualWidth}
class TVE_TextureRect:public BaseClass{
public:
	DWORD C1,C2,C3,C4;
	float u,v,u1,v1;
	int dx,dy,lx,ly,ActualWidth;	
	int TextureID;
	virtual bool Assign(char* TextCommand);
	virtual void Draw(int x,int y,int Lx,int Ly,DWORD Color);
	virtual void GetBestSize(int FontSizeY,int& Lx,int& Ly,int& LowMarkPos);
	SAVE(TVE_TextureRect);
	REG_PARENT(TextVisualElement);
	ENDSAVE;
};
//{plot Lx Ly commands}
//commands:
//C XXXXXXXX   - set current color
//SX scale     - set current x-scale
//SY scale     - set current x-scale
//M x y        - set current cursor pos to (xy)
//L x y        - draw line to (xy)
//B x y Lx Ly  - draw filled rect
//D x y Wx Hy XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX - draw filled rect 4 colors! (clt,crt,clb,crb)
//R x y XXXXXXXX - draw filled circle

class TVE_Plot:public TextVisualElement{
public:
	DWORD COLOR,LTCOLOR,LBCOLOR,RTCOLOR,RBCOLOR,FCOLOR;
	float scaleX,scaleY;
	int px,py,dlx,dly,drx,dry,lrx,lry,lx,ly,R;
	char* param;
	virtual bool Assign(char* TextCommand);
	virtual void Draw(int x,int y,int Lx,int Ly,DWORD Color);
	virtual void GetBestSize(int FontSizeY,int& Lx,int& Ly,int& LowMarkPos);
	SAVE(TVE_Plot);
	REG_PARENT(TextVisualElement);
	ENDSAVE;
};
