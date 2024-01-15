#include "common.h"

int PenInterface::width;
int PenInterface::height;
int PenInterface::texborder;
int PenInterface::texborder2;
int PenInterface::WhiteTexture;
int PenInterface::StdShader;
int PenInterface::BumpShader;
int PenInterface::IconsTexture;
int PenInterface::sliderheight;
BaseWidget* PenInterface::ToolPanel=NULL;
const char* PenInterface::IconsName;
BaseWidget* PenInterface::Root=NULL;
DWORD PenInterface::Color;
DrawTexRect* PenInterface::CloseMtl;

bool PenInterface::SetPenShapeTexture(){
	DrawTexRect* tr=(DrawTexRect*)BaseWidget::CurrentWidget;
	if(GeneralPen::CurrentShapePtr){
		tr->Textures[0]=GeneralPen::CurrentShapePtr->GetPreviewTexture();
		tr->Color=0xFFFFFFFF;
	}
    return false;
}
bool PenInterface::SetPenStripTexture(){
	DrawTexRect* tr=(DrawTexRect*)BaseWidget::CurrentWidget;
	if(GeneralPen::CurrentStripPtr){
		tr->Textures[0]=GeneralPen::CurrentStripPtr->GetPreviewTexture();
		tr->Color=0xFFFFFFFF;
	}
    return false;
}
bool PenInterface::PenShapeTextureMenu(int idx){
	DrawTexRect* tr=(DrawTexRect*)BaseWidget::CurrentWidget;
	BasicPenShape* RP=GeneralPen::RadialShapes[idx];	
	tr->Textures[0]=RP->GetPreviewTexture();	
	return false;
}
bool PenInterface::AddPenShape(){
	OpenFileDialog dlg;	
	dlg.AddFilter( "TGA files" , "*.tga" );
	dlg.AddFilter( "BMP files" , "*.bmp" );
	dlg.AddFilter( "JPG files" , "*.jpg" );
	dlg.SetDefaultExtension( "tga" );
	_chdir(IRM->GetHomeDirectory());	
	if (dlg.Show()){
		_chdir(IRM->GetHomeDirectory());	
		char dir	[_MAX_PATH];
		char drive	[_MAX_PATH];
		char file	[_MAX_PATH];
		char ext	[_MAX_PATH];
		_splitpath( dlg.GetFilePath(), drive, dir, file, ext );
		char s[_MAX_PATH];
		sprintf(s,"textures\\patterns\\%s%s",file,ext);
		CopyFile(dlg.GetFilePath(),s,false);
		PatternPen* pp=new PatternPen;
		if(pp->P.LoadTexture(s)){		
			GeneralPen::RadialShapes.Add(pp);
		}else delete(pp);
	}
	return false;
}
bool PenInterface::DrawPenShapeSubmenu(){
	BaseWidget* s_menu=BaseWidget::CurrentWidget;
	int nsh=GeneralPen::RadialShapes.GetAmount()+1;
	if(s_menu->SubWidgets.GetAmount()!=nsh){
		s_menu->SubWidgets.Clear();
		int nx,ny;
		if(nsh>4){
			ny=sqrt(float(nsh));
			nx=(nsh+ny-1)/ny;
		}
		int tw=nx*(width-texborder2)+texborder2;
		int th=ny*(height-texborder2)+texborder2;
		s_menu->Rect=Rct(-tw,s_menu->Rect.y,tw,th);
		int p=0;
		for(int yy=0;yy<ny;yy++){
			int x0=(width-texborder2)*(nx-1)+texborder;
			int y0=(height-texborder2)*yy+texborder;
			for(int i=0;i<nx;i++){
				if(p==nsh-1){
                    DrawTexRect* T1=new DrawTexRect(IconsName,2,icn_lx,icn_ly);
					T1->Color=0xFFFFFFFF;
					s_menu->SubWidgets.Add(T1);
					T1->Rect=Rct(x0,y0,width-texborder2,height-texborder2);
					T1->SetHint("ADD_NEW_PEN_HINT");
					pi_callback(WM_LBUTTONUP,T1,AddPenShape);
					break;
				}else{
					DrawTexRect* T=new DrawTexRect;
					T->Color=0xFFFFFFFF;
					s_menu->SubWidgets.Add(T);
					T->Rect=Rct(x0,y0,width-texborder2,height-texborder2);
					pi_callback1(WM_PAINT,T,PenShapeTextureMenu,p);
					pi_callback1(WM_LBUTTONUP,T,SelectPenShape,p);
					x0-=width-texborder2;
				}
				p++;
			}		
		}
	}
	return false;
}
bool PenInterface::PenStripTextureMenu(int idx){
	DrawTexRect* tr=(DrawTexRect*)BaseWidget::CurrentWidget;
	BasicPenShape* RP=GeneralPen::Strips[idx];	
	tr->Textures[0]=RP->GetPreviewTexture();	
	return false;
}
bool PenInterface::AddPenStrip(){
	OpenFileDialog dlg;
	dlg.AddFilter( "TGA files" , "*.tga" );
	dlg.AddFilter( "BMP files" , "*.bmp" );
	dlg.AddFilter( "JPG files" , "*.jpg" );
	dlg.SetDefaultExtension( "tga" );	
	_chdir(IRM->GetHomeDirectory());	
	if (dlg.Show()){
		_chdir(IRM->GetHomeDirectory());	
		char dir	[_MAX_PATH];
		char drive	[_MAX_PATH];
		char file	[_MAX_PATH];
		char ext	[_MAX_PATH];
		_splitpath( dlg.GetFilePath(), drive, dir, file, ext );
		char s[_MAX_PATH];
		sprintf(s,"textures\\strips\\%s%s",file,ext);
		CopyFile(dlg.GetFilePath(),s,false);
		StripPen* pp=new StripPen;
		if(pp->P.LoadTexture(s)){		
            GeneralPen::Strips.Add(pp);
		}else delete(pp);
	}
	return false;
}
bool PenInterface::DrawPenStripSubmenu(){
	BaseWidget* s_menu=BaseWidget::CurrentWidget;
	int nsh=GeneralPen::Strips.GetAmount()+1;
	if(s_menu->SubWidgets.GetAmount()!=nsh){
		s_menu->SubWidgets.Clear();
		int nx=nsh;
        int ny=1;
		if(nsh>4){
			ny=sqrt(float(nsh));
			nx=(nsh+ny-1)/ny;
		}
		int tw=nx*(width-texborder2)+texborder2;
		int th=ny*(height-texborder2)+texborder2;
		s_menu->Rect=Rct(width,s_menu->Rect.y,tw,th);
		int p=0;
		for(int yy=0;yy<ny;yy++){
			int x0=texborder;//(width-texborder2)*(nx-1)+texborder;
			int y0=(height-texborder2)*yy+texborder;
			for(int i=0;i<nx;i++){
				if(p==nsh-1){
                    DrawTexRect* T1=new DrawTexRect(IconsName,2,icn_lx,icn_ly);
					T1->Color=0xFFFFFFFF;
					s_menu->SubWidgets.Add(T1);
					T1->Rect=Rct(x0,y0,width-texborder2,height-texborder2);
					T1->SetHint("ADD_NEW_PEN_STRIP_HINT");
					pi_callback(WM_LBUTTONUP,T1,AddPenStrip);
					break;
				}else{
					DrawTexRect* T=new DrawTexRect;
					T->Color=0xFFFFFFFF;
					s_menu->SubWidgets.Add(T);
					T->Rect=Rct(x0,y0,width-texborder2,height-texborder2);
					pi_callback1(WM_PAINT,T,PenStripTextureMenu,p);
					pi_callback1(WM_LBUTTONUP,T,SelectPenStrip,p);
					x0+=width-texborder2;
				}
				p++;
			}		
		}
	}
	return false;
}
bool PenInterface::ShowSubmenu(int param){
    BaseWidget* bw=(BaseWidget*)param;
	bw->Visible=true;
	return false;
}
bool PenInterface::HideSubmenu(int param){
	if(!BaseWidget::CurrentWidget->MouseOver){
		BaseWidget* bw=(BaseWidget*)param;
		if(!bw->MouseOver)
			BaseWidget::CurrentWidget->Visible=false;
	}
	return false;
}
bool PenInterface::CloseOnClick(){
	BaseWidget::CurrentWidget->MouseOver=false;
	BaseWidget::CurrentWidget->Visible=false;
	return true;
}
bool PenInterface::SelectPenShape(int idx){
	GeneralPen::CurrentShape=idx;
	GeneralPen::CurrentShapePtr=GeneralPen::RadialShapes[idx];	
	return false;
}
bool PenInterface::SelectPenStrip(int idx){
	GeneralPen::CurrentStrip=idx;
	GeneralPen::CurrentStripPtr=GeneralPen::Strips[idx];	
	return false;
}
void PenInterface::AddText(BaseWidget* Base,const char* Text,int dy){
    if(dy==0){
        FrameWidget* FW=new FrameWidget;
        FW->Rect=Rct(0,0,Base->Rect.w,12);
        FW->Color=0xFF8080FF;
        Base->SubWidgets.Add(FW);
    }
	TextWidget* TW=new TextWidget;	
	Base->SubWidgets.Add(TW);
    TW->Color=0x80FFFFFF;
	TW->Rect=Rct(0,dy,Base->Rect.w,15);
	TW->Text=TextManager::GetText(Text);
	TW->Align=1;
}
void PenInterface::AddHeader(int& y,const char* Text){    
    FrameWidget* FW=new FrameWidget;
    FW->Rect=Rct(0,y,width,13);
    FW->Color=0xFF0640FF;
    Root->SubWidgets.Add(FW);
    TextWidget* TW=new TextWidget;	
	Root->SubWidgets.Add(TW);
    TW->Color=0x80FFFFFF;
	TW->Rect=Rct(0,y,width,15);
	TW->Text=TextManager::GetText(Text);
	TW->Align=1;
    y+=13;
}
void PenInterface::AddToolHeader(const char* Text){
    FrameWidget* FW=new FrameWidget;
    int y=GetToolHeight();
    FW->Rect=Rct(0,y,width,13);
    FW->Color=0xFF0640FF;
    ToolPanel->SubWidgets.Add(FW);
    TextWidget* TW=new TextWidget;	
    FW->SubWidgets.Add(TW);
    TW->Color=0x80FFFFFF;
	TW->Rect=Rct(0,0,width,15);
	TW->Text=TextManager::GetText(Text);
	TW->Align=1;    
}
SimpleSlider* PenInterface::AddToolSlider(float MinValue,float MaxValue,float* ValuePtr,DWORD Color,const char* Message,const char* Hint){
    int y=GetToolHeight();
    FrameWidget* FW=new FrameWidget;
    FW->Rect=Rct(0,y,width,sliderheight);
    FW->Color=ToolPanel->Color;
    ToolPanel->SubWidgets.Add(FW);
    SimpleSlider* SL=new SimpleSlider(Color,ValuePtr,MinValue,MaxValue);
    SL->SetHint(Hint);
    SL->Rect=Rct(0,0,width,sliderheight);
    FW->SubWidgets.Add(SL);
    if(Message[0]){
        TextWidget* TW=new TextWidget;
        TW->Rect=Rct(0,3,width,sliderheight);
        SL->SubWidgets.Add(TW);
    }
    return SL;
}
BaseWidget* PenInterface::AddToolButton(int IconID,const char* Hint){
    int y=GetToolHeight();
    FrameWidget* FW=new FrameWidget;	    
    FW->SetHint(Hint);
    FW->Color=ToolPanel->Color;
    FW->Rect=Rct(0,y,width,height);
    ToolPanel->SubWidgets.Add(FW);
    DrawTexRect* TR=new DrawTexRect(IconsTexture,IconID,icn_lx,icn_ly);
    TR->Rect=Rct(texborder,texborder,width-texborder2,height-texborder2);
    FW->SubWidgets.Add(TR);
    return FW;
}

int PenInterface::GetToolHeight(){
    int h=0;
    for(int i=0;i<ToolPanel->SubWidgets.GetAmount();i++){
        h+=ToolPanel->SubWidgets[i]->Rect.h;
    }
    return h;
}
void PenInterface::CreatePenInterface(){
	GeneralPen::Init();
	Color=0xFFE6E2E6;
	WhiteTexture=IRS->GetTextureID("white");
	StdShader=IRS->GetShaderID("hud_smooth");
	BumpShader=IRS->GetShaderID("obj_preview_hmap");
	IconsName="icons\\baseicons.dds";
	IconsTexture=IRS->GetTextureID(IconsName);	
    sliderheight=20;
	int n=6;
    int nsl=6;
	int nhdr=6;
	int hdrheight=14;
	ScreenProp p=IRS->GetScreenProp();
	Rct sp=IRS->GetViewPort();
	p.m_Height=sp.h;

    width=64+8;
	
    int y=n*width;	
    p.m_Height-=nsl*sliderheight+nhdr*hdrheight+20;
	if(y>p.m_Height){
        width=p.m_Height/n;
		y=n*width;
	}
    int y0=0;
	int x0=p.m_Width-width;
	height=width;
	texborder=4;
	texborder2=texborder*2;    

    int sl_border=0;
    int sl_border2=sl_border*2;

	Root=new BaseWidget;
	Root->Rect=Rct(x0,0,width,height*n);
	Widgets::AddActiveWidget(Root);
	

    // pen shape
    AddHeader(y0,"PEN_SHAPE");
	FrameWidget* FW1=new FrameWidget;	
	Root->SubWidgets.Add(FW1);
	FW1->Rect=Rct(0,y0,width,height);
    FW1->Color=Color;   
	FW1->SetHint("SELECT_PEN_SHAPE");

	//pen shape submenu

    FrameWidget * s_menu=new FrameWidget;	
	pi_callback1(WM_MOUSEMOVE,FW1,ShowSubmenu,int(s_menu));
	s_menu->Rect=Rct(-20,y0,20,height);
	s_menu->Visible=false;
	s_menu->Color=0xFFE6E2E6;
	pi_callback(WM_PAINT,s_menu,DrawPenShapeSubmenu);
	pi_callback1(WM_PAINT,s_menu,HideSubmenu,int(FW1));
	pi_callback(WM_LBUTTONUP,s_menu,CloseOnClick);
	Root->SubWidgets.Add(s_menu);
    
	DrawTexRect* TR1=new DrawTexRect;
	Root->SubWidgets.Add(TR1);
	TR1->Rect=Rct(texborder,y0+texborder,height-texborder2,height-texborder2);
	TR1->Textures[0]=0;
	pi_callback(WM_PAINT,TR1,SetPenShapeTexture);    
    FW1->SetHint("PEN_SHAPE_HINT");
	
	y0+=height;    

    //pen radius slider
    FrameWidget* FW_RADIUS=new FrameWidget;	
	Root->SubWidgets.Add(FW_RADIUS);
	FW_RADIUS->Rect=Rct(0,y0,width,sliderheight);
    FW_RADIUS->Color=Color;
    SimpleSlider* RSL=new SimpleSlider(0xFF00FFFF,&GeneralPen::PenRadius,0,20);
    RSL->Rect=Rct(sl_border,sl_border,width-sl_border2,sliderheight-sl_border2);
    FW_RADIUS->SubWidgets.Add(RSL);	
	AddText(FW_RADIUS,"PEN_RADIUS",3);
    FW_RADIUS->SetHint("PEN_RADIUS_HINT");
    y0+=sliderheight;

    //pen rotation slider
    FrameWidget* FW_ROT=new FrameWidget;	
	Root->SubWidgets.Add(FW_ROT);
	FW_ROT->Rect=Rct(0,y0,width,sliderheight);
    FW_ROT->Color=Color;
    SimpleSlider* RTSL=new SimpleSlider(0xFFFF00FF,&GeneralPen::PenRotation,0,2*c_PI);
    RTSL->Rect=Rct(sl_border,sl_border,width-sl_border2,sliderheight-sl_border2);
    FW_ROT->SubWidgets.Add(RTSL);	
	AddText(FW_ROT,"PEN_ROTSN",3);
    FW_ROT->SetHint("PEN_ROTSN_HINT");
    y0+=sliderheight;

	//matherial selector
    AddHeader(y0,"SELECT_MTL");
	FrameWidget* FW2=new FrameWidget;
    FW2->Color=Color;
	FW2->Rect=Rct(0,y0,width,height);
	Root->SubWidgets.Add(FW2);
	DrawTexRect* MP=new DrawTexRect;
	MP->Rect=Rct(texborder,texborder,width-texborder2,height-texborder2);
	pi_callback2(WM_PAINT,MP,AssignMtlTexture,-1,int(MP));
	FW2->SubWidgets.Add(MP);	
    FW2->SetHint("SELECT_MTL_HINT");

	//mtl-s list
	FrameWidget* MTLIST=new FrameWidget;
	FW2->SubWidgets.Add(MTLIST);	
	MTLIST->Rect=Rct(-20,y0,20,height);
	MTLIST->Color=Color;
	pi_callback1(WM_MOUSEMOVE,FW2,ShowSubmenu,int(MTLIST));	
	pi_callback(WM_PAINT,MTLIST,CreateMtlList);
	pi_callback1(WM_PAINT,MTLIST,HideSubmenu,int(FW2));
	pi_callback(WM_LBUTTONUP,MTLIST,CloseOnClick);	

	//close mtl icon
	CloseMtl=new DrawTexRect(IconsName,3,icn_lx,icn_ly);
    CloseMtl->Rect=Rct(width-texborder-16,texborder,16,16);
	FW2->SubWidgets.Add(CloseMtl);
	//pi_callback(WM_LBUTTONUP,CloseMtl,CloseCurMtl);

	//matherial selector submenu

	y0+=height;
	//depth texture
    AddHeader(y0,"ID_BUMP");
	FrameWidget* FW3=new FrameWidget;
    FW3->Color=Color;
	Root->SubWidgets.Add(FW3);
	FW3->Rect=Rct(0,y0,width,height);
	pi_callback3(WM_PAINT,FW3,CreatePatternMenu,0,int(FW3),BumpShader);	
	y0+=height;

    //pen depth slider
    FrameWidget* FW_DEP=new FrameWidget;	
	Root->SubWidgets.Add(FW_DEP);
	FW_DEP->Rect=Rct(0,y0,width,sliderheight);
    FW_DEP->Color=Color;
    SimpleSlider* DSL=new SimpleSlider(0xFFFF0000,&GeneralPen::DeepDegree,0,1);
    DSL->Rect=Rct(sl_border,sl_border,width-sl_border2,sliderheight-sl_border2);
    FW_DEP->SubWidgets.Add(DSL);
	AddText(FW_DEP,"PEN_DEPTH",3);
    FW_DEP->SetHint("PEN_DEPTH_HINT");
    y0+=sliderheight;

	//smooth degree
	FrameWidget* FW_SMT=new FrameWidget;	
	Root->SubWidgets.Add(FW_SMT);
	FW_SMT->Rect=Rct(0,y0,width,sliderheight);
	FW_SMT->Color=Color;
	SimpleSlider* SSL=new SimpleSlider(0xFF0000FF,&GeneralPen::SmoothDegree,0,1);
	SSL->Rect=Rct(sl_border,sl_border,width-sl_border2,sliderheight-sl_border2);
	FW_SMT->SubWidgets.Add(SSL);
	AddText(FW_SMT,"SMOOTH_DEG",3);
	FW_SMT->SetHint("SMOOTH_DEG_HINT");
	y0+=sliderheight;

	//color texture
    AddHeader(y0,"ID_COLOR");
	FrameWidget* FW4=new FrameWidget;
    FW4->Color=Color;
	Root->SubWidgets.Add(FW4);
	FW4->Rect=Rct(0,y0,width,height);
	pi_callback3(WM_PAINT,FW4,CreatePatternMenu,1,int(FW4),StdShader);	

	y0+=height;
	//color select
    AddHeader(y0,"SEL_COLOR");
	FrameWidget* FW4C=new FrameWidget;
    FW4C->Color=Color;
	Root->SubWidgets.Add(FW4C);
	FW4C->Rect=Rct(0,y0,width,height);	
	pi_callback(WM_PAINT,FW4C,GetCurrColor);	
	pi_callback(WM_LBUTTONUP,FW4C,SelectColor);
    FW4C->SetHint("SEL_COLOR_HINT");
    y0+=height;

    //color opacity slider    
    FrameWidget* FW_OPC=new FrameWidget;	
    FW_OPC->Color=Color;
	Root->SubWidgets.Add(FW_OPC);
	FW_OPC->Rect=Rct(0,y0,width,sliderheight);
    FW_OPC->Color=Color;
    SimpleSlider* OSL=new SimpleSlider(0xFF00FF00,&GeneralPen::ColorOpacity,0,2);
    OSL->Rect=Rct(sl_border,sl_border,width-sl_border2,sliderheight-sl_border2);
    FW_OPC->SubWidgets.Add(OSL);
    AddText(FW_OPC,"PEN_MASK",3);
    FW_OPC->SetHint("PEN_MASK_HINT");
    y0+=sliderheight;    
	
	//alpha texture
    /*
    AddHeader(y0,"ID_OPACITY");
	FrameWidget* FW5=new FrameWidget;
    FW5->Color=Color;
	Root->SubWidgets.Add(FW5);
	FW5->Rect=Rct(0,y0,width,height);
	pi_callback3(WM_PAINT,FW5,CreatePatternMenu,2,int(FW5),StdShader);	
    y0+=height;
    */
	
	//specular texture
    AddHeader(y0,"ID_SPECULAR");
	FrameWidget* FW6=new FrameWidget;
    FW6->Color=Color;
	Root->SubWidgets.Add(FW6);
	FW6->Rect=Rct(0,y0,width,height); 
	pi_callback3(WM_PAINT,FW6,CreatePatternMenu,3,int(FW6),StdShader);	

	y0+=height;
    /*
	//color select
	FrameWidget* FW6C=new FrameWidget;
	Root->SubWidgets.Add(FW6C);
	FW6C->Rect=Rct(0,y0,width,height);	
	pi_callback(WM_PAINT,FW6C,GetCurrSpecular);	
	pi_callback(WM_LBUTTONUP,FW6C,SelectSpecular);	    
    y0+=height;
    */

    //pen specular opacity slider
    FrameWidget* FW_SP=new FrameWidget;	
	Root->SubWidgets.Add(FW_SP);
	FW_SP->Rect=Rct(0,y0,width,sliderheight);
    FW_SP->Color=Color;
    SimpleSlider* SMSL=new SimpleSlider(0xFFFF00FF,&GeneralPen::SpecularOpacity,0,2);
    SMSL->Rect=Rct(sl_border,sl_border,width-sl_border2,sliderheight-sl_border2);
    FW_SP->SubWidgets.Add(SMSL);
    AddText(FW_SP,"PEN_SPEC_OPC",3);
    FW_SP->SetHint("PEN_SPEC_OPC_HINT");
    y0+=sliderheight;

    //pen specular slider
    FrameWidget* FW_SPL=new FrameWidget;	
	Root->SubWidgets.Add(FW_SPL);
	FW_SPL->Rect=Rct(0,y0,width,sliderheight);
    FW_SPL->Color=Color;
    SimpleSlider* SPSL=new SimpleSlider(0xFFFFFF00,&GeneralPen::SpecularDegree,0,2);
    SPSL->Rect=Rct(sl_border,sl_border,width-sl_border2,sliderheight-sl_border2);
    FW_SPL->SubWidgets.Add(SPSL);
    AddText(FW_SPL,"PEN_SPEC",3);
    FW_SPL->SetHint("PEN_SPEC_HINT");
    y0+=sliderheight;

    int xc=-p.m_Width+width;
	int yc=0;

	//file menu
	FrameWidget* FW7=new FrameWidget;
    FW7->Color=Color;
	Root->SubWidgets.Add(FW7);
	FW7->Rect=Rct(xc,yc,width,height);     
    TextMenu* TM=new TextMenu(1,1);
    FW7->SubWidgets.Add(TM);
    DrawTexRect* TR=new DrawTexRect(IconsName,1,icn_lx,icn_ly);
	TR->Rect=Rct(texborder,texborder,width-texborder2,height-texborder2);
	FW7->SubWidgets.Add(TR);


    TextWidget* OPEN=TM->AddLine("OPEN_FILE","OPEN_FILE_HINT");
    pi_callback(WM_LBUTTONUP,OPEN,OpenMesh);

    TextWidget* SAVE=TM->AddLine("SAVE_FILE","SAVE_FILE_HINT");
    pi_callback(WM_LBUTTONUP,SAVE,SaveMesh);

    TextWidget* OPENOBJ=TM->AddLine("OPENOBJ","OPENOBJ_HINT");
    pi_callback(WM_LBUTTONUP,OPENOBJ,OpenObj);

    TextWidget* SAVETEX=TM->AddLine("SAVETEX","SAVETEX_HINT");
    pi_callback(WM_LBUTTONUP,SAVETEX,SaveTex);    

    TextWidget* SAVENM=TM->AddLine("SAVENM","SAVENM_HINT");
    pi_callback(WM_LBUTTONUP,SAVENM,SaveNmap);

    TM->AddSeparator();    

    TextWidget* SAVELOPOLYOBJ=TM->AddLine("SAVELOPOLYOBJ","SAVELOPOLYOBJ_HINT");
    pi_callback(WM_LBUTTONUP,SAVELOPOLYOBJ,SaveLoPolyObj);

    TextWidget* SAVELOPOLYTANG=TM->AddLine("SAVELOPOLYTANG","SAVELOPOLYTANG_HINT");
    pi_callback(WM_LBUTTONUP,SAVELOPOLYTANG,SaveLoPolyTang);

    TextWidget* SAVELOPOLYDISP=TM->AddLine("SAVELOPOLYDISP","SAVELOPOLYDISP_HINT");
    pi_callback(WM_LBUTTONUP,SAVELOPOLYDISP,SaveLoPolyDisp);

    TM->AddSeparator();    

    TextWidget* SAVEMIDPOLYOBJ=TM->AddLine("SAVEMIDPOLYOBJ","SAVEMIDPOLYOBJ_HINT");
    pi_callback(WM_LBUTTONUP,SAVEMIDPOLYOBJ,SaveMidPolyObj);

    TextWidget* SAVEMIDPOLYTANG=TM->AddLine("SAVEMIDPOLYTANG","SAVEMIDPOLYTANG_HINT");
    pi_callback(WM_LBUTTONUP,SAVEMIDPOLYTANG,SaveMidPolyTang);

    TextWidget* SAVEMIDPOLYDISP=TM->AddLine("SAVEMIDPOLYDISP","SAVEMIDPOLYDISP_HINT");
    pi_callback(WM_LBUTTONUP,SAVEMIDPOLYDISP,SaveMidPolyDisp);

    //TM->AddSeparator();

    //TextWidget* SAVEMIDPOLYOBJ=TM->AddLine("SAVEHIPOLYOBJ","SAVEMIDPOLYOBJ_HINT");
    //pi_callback(WM_LBUTTONUP,SAVETANG,SaveHiPolyObj);

    TM->AddSeparator();

    TextWidget* EXIT=TM->AddLine("EXIT","EXIT_HINT");
    pi_callback(WM_LBUTTONUP,EXIT,ExitProgram);

    yc+=height;

    PictMenu* PenPress=new PictMenu;
	PenPress->Color=Color;
	PenPress->CreateDefault(Rct(xc,yc,width,height),&GeneralPen::PenPressureType,1);
	PenPress->SetHint("PENPRESS_OPERATION");
	PenPress->AddPict(14,"PENPRESS_DEFAULT");
	PenPress->AddPict(15,"PENPRESS_SOFT");
	PenPress->AddPict(16,"PENPRESS_CONSTANT");
	PenPress->AddPict(17,"PENPRESS_POINTS");    
	Root->SubWidgets.Add(PenPress);

	yc+=height;

    PictMenu* PenSymm=new PictMenu;
	PenSymm->Color=Color;
	PenSymm->CreateDefault(Rct(xc,yc,width,height),&GeneralPen::SymmetryType,1);
	PenSymm->SetHint("PEN_SYMMETRY_HINT");
    PenSymm->AddPict(5,"PEN_SYMM_NONE");
	PenSymm->AddPict(7,"PEN_SYMM_X");
	PenSymm->AddPict(6,"PEN_SYMM_Y");
	PenSymm->AddPict(8,"PEN_SYMM_Z");	
	Root->SubWidgets.Add(PenSymm);

	yc+=height;

    //pen operation		
	PictMenu* PenOp=new PictMenu;
	PenOp->SetHint("PEN_OPERATION");
	PenOp->Color=Color;
	PenOp->CreateDefault(Rct(xc,yc,width,height),&GeneralPen::PenOpType,1);
	PenOp->AddPict(10,"PENOP_DRAW");
	PenOp->AddPict(11,"PENOP_ADD");	
    int N=BasicTool::GetToolsAmount(0);
    for(int i=0;i<N;i++){        
        BasicTool* T=BasicTool::GetTool(0,i);
        IconInfo I=T->GetIconInfo();
        PenOp->AddPict(I.IconID,T->GetHint());        
    }
	Root->SubWidgets.Add(PenOp);
	yc+=height;

    ToolPanel=new BaseWidget;
    ToolPanel->Rect=Rct(xc,yc,width,1000);
    ToolPanel->Color=Color;	
    Root->SubWidgets.Add(ToolPanel);
}
void PenInterface::CreateStripMenu(BaseWidget* Root){
    int y0=GetToolHeight();
    // pen strip    
	FrameWidget* FW12=new FrameWidget;	
	Root->SubWidgets.Add(FW12);
	FW12->Rect=Rct(0,y0,width,height);
    FW12->Color=Color;   
	FW12->SetHint("SELECT_PEN_STRIP");

    //pen strip submenu

    FrameWidget * s_menu1=new FrameWidget;	
	pi_callback1(WM_MOUSEMOVE,FW12,ShowSubmenu,int(s_menu1));
	s_menu1->Rect=Rct(width,y0,20,height);
	s_menu1->Visible=false;
	s_menu1->Color=0xFFE6E2E6;
	pi_callback(WM_PAINT,s_menu1,DrawPenStripSubmenu);
	pi_callback1(WM_PAINT,s_menu1,HideSubmenu,int(FW12));
	pi_callback(WM_LBUTTONUP,s_menu1,CloseOnClick);
	Root->SubWidgets.Add(s_menu1);
    
	DrawTexRect* TR12=new DrawTexRect;
	Root->SubWidgets.Add(TR12);
	TR12->Rect=Rct(texborder,y0+texborder,height-texborder2,height-texborder2);
	TR12->Textures[0]=0;
	pi_callback(WM_PAINT,TR12,SetPenStripTexture);    
    FW12->SetHint("PEN_STRIP_HINT");
	
	y0+=height;
}
void OnePenStyle::ToXML(xmlQuote& xml){
	for(int i=0;i<4;i++){
		if(Patterns[i]){
			char c[32];
			sprintf(c,"Pattern%d",i);
			xmlQuote* x=new_xmlQuote(c);
			xml.AddSubQuote(x);
			if(Patterns[i]->TextureID>0){
				xmlQuote* x1=new_xmlQuote("Texture");
				x->AddSubQuote(x1);
				const char* t=IRS->GetTextureName(Patterns[i]->TextureID);
				t=IRM->ConvertPathToRelativeForm(t);
				x1->Assign_string((char*)t);
			}
			const char* en=EnablePattern[i]?"true":"false";
			xmlQuote* x1=new_xmlQuote("Enabled");
			x->AddSubQuote(x1);
			x1->Assign_string((char*)en);
		}
	}	
}
void OnePenStyle::FromXML(xmlQuote& xml){
	for(int i=0;i<4;i++){		
		char c[32];
		sprintf(c,"Pattern%d",i);
		xmlQuote* x=xml.GetSubQuote(c);
		if(x){					
			xmlQuote* x1=x->GetSubQuote("Texture");
			if(x1){
                TexturePattern* T=new TexturePattern;
				T->LoadTexture(x1->Get_string());				
				Patterns[i]=T;
			}
            x1=xml.GetSubQuote("Enabled");
			if(x1){
				EnablePattern[i]=!strcmp(x1->Get_string(),"true");
			}
		}
	}	    
}
void GeneralPen::SavePen(){
	_chdir(IRM->GetHomeDirectory());
	FileList F;
	mkdir("Patterns\\");
	CreateSortedFileList("Patterns\\","*.xml",F,false);
	for(int i=0;i<F.GetAmount();i++){
		DeleteFile(F[i]->pchar());
	}
	for(int i=0;i<Pens.GetAmount();i++){
        char c[32];
		sprintf(c,"Patterns\\%.04d.xml",i);
		xmlQuote xml;
		Pens[i]->ToXML(xml);
		xml.WriteToFile(c);
	}
}
void GeneralPen::LoadPen(){
	_chdir(IRM->GetHomeDirectory());
	FileList F;
	mkdir("Patterns\\");
	CreateSortedFileList("Patterns\\","*.xml",F,false); 
	for(int i=0;i<F.GetAmount();i++){
        xmlQuote xml;
		xml.ReadFromFile(F[i]->pchar());
		OnePenStyle* PS=new OnePenStyle;
        PS->FromXML(xml);
		Pens.Add(PS);
	}
}
PenInterface* PenInterface::instance(){
    static PenInterface* pi=new PenInterface;
	return pi;
}
bool PenInterface::SaveLoPolyObj(){
    SaveFileDialog dlg;
	dlg.AddFilter("obj files",    "*.obj"     );
	dlg.SetDefaultExtension( "obj" );
	_chdir(IRM->GetHomeDirectory());
	if (!dlg.Show()) return false;	
    PMS.SaveSrcObj(dlg.GetFilePath());
	return false;    
}
bool PenInterface::SaveLoPolyTang(){    
    SaveFileDialog dlg;
	dlg.AddFilter("tga files",    "*.tga"     );
    dlg.AddFilter("tga files",    "*.dds"     );
	dlg.SetDefaultExtension( "obj" );
	_chdir(IRM->GetHomeDirectory());
	if (!dlg.Show()) return false;	  
    DWORD* T;
    PMS.CreateTangentSpaceNormalMap(PMS.SrcFaces,&T);
    if(T){
        SaveBufferTo(dlg.GetFilePath(),T,PMS.rtSizeX,PMS.rtSizeY);
        delete[]T;
    }
	return false;    
}
bool PenInterface::SaveLoPolyDisp(){
    SaveFileDialog dlg;
	dlg.AddFilter("tga files",    "*.tga"     );
    dlg.AddFilter("tga files",    "*.dds"     );
	dlg.SetDefaultExtension( "obj" );
	_chdir(IRM->GetHomeDirectory());
	if (!dlg.Show()) return false;	
    float* F;
    PMS.CreateDispMap(PMS.SrcFaces,&F,true);
    SaveGreyFloatsTo(dlg.GetFilePath(),F,PMS.rtSizeX,PMS.rtSizeY);
    delete[]F;
	return false;    
}
bool PenInterface::SaveMidPolyObj(){
    SaveFileDialog dlg;
	dlg.AddFilter("obj files",    "*.obj"     );
	dlg.SetDefaultExtension( "obj" );
	_chdir(IRM->GetHomeDirectory());
	if (!dlg.Show()) return false;
    PMS.SaveBigObjFile(dlg.GetFilePath(),0);
	return false;    
}
bool PenInterface::SaveMidPolyTang(){
    SaveFileDialog dlg;
	dlg.AddFilter("tga files",    "*.tga"     );
    dlg.AddFilter("tga files",    "*.dds"     );
	dlg.SetDefaultExtension( "obj" );
	_chdir(IRM->GetHomeDirectory());
	if (!dlg.Show()) return false;	
    DWORD* T;
    PMS.CreateTangentSpaceNormalMap(PMS.Faces,&T);
    if(T){
        SaveBufferTo(dlg.GetFilePath(),T,PMS.rtSizeX,PMS.rtSizeY);
        delete[]T;
    }
	return false;    
}
bool PenInterface::SaveMidPolyDisp(){
    SaveFileDialog dlg;
	dlg.AddFilter("tga files",    "*.tga"     );
    dlg.AddFilter("tga files",    "*.dds"     );
	dlg.SetDefaultExtension( "obj" );
	_chdir(IRM->GetHomeDirectory());
	if (!dlg.Show()) return false;	
    float* F;
    PMS.CreateDispMap(PMS.Faces,&F,true);
    SaveGreyFloatsTo(dlg.GetFilePath(),F,PMS.rtSizeX,PMS.rtSizeY);
    delete[]F;
	return false;    
}
bool PenInterface::ExitProgram(){
    exit(1);
}