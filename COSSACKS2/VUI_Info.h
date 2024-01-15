#pragma once

class VUI_InfoPanel{
public:
	VUI_InfoPanel(void);
	~VUI_InfoPanel(void);

	int bfile,bsprite;
	//int bx,by;
	int pfile,psprite;
	int px,py;

	//GP_Button* Border;
	//GPPicture* Portret;

	//virtual bool Init();
	//virtual void Draw(int x, int y);
};


class VUI_IP_Kri: public VUI_InfoPanel
{
public:
	//VUI_IP_Kri(void);
	//~VUI_IP_Kri(void);
};

class VUI_Command: public BaseClass
{
public:
protected:
private:
};

int vgf_UI_Process();
void vgf_UI_Clear();