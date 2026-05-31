#include "StdAfx.h" #include "common.h" 

ClonesArray<TextItem> TextManager::Text;

void TextManager::Init(){	
	if(Text.GetAmount()==0){        
		REG_CLASS(TextItem);
		Text.SafeReadFromFile("text.xml");
	}
}
const char* TextManager::GetText(const char* ID){
    Init();
	for(int i=0;i<Text.GetAmount();i++){
		if(Text[i]->ID.equal(ID))return Text[i]->Text.cpchar();
	}
    TextItem* Item=new TextItem;
    Item->ID=ID;
    Item->Text=ID;
    Text.Add(Item);
    Text.WriteToFile("text.xml");
	return Item->Text.cpchar();
}
const char* TextManager::GetID(const char* ID){
    Init();
	for(int i=0;i<Text.GetAmount();i++){
		if(Text[i]->Text.equal(ID))return Text[i]->ID.cpchar();
	}    
	return ID;
}

void BaseWidget::Init(){
	EnablePopup=false;
    Enabled=true;
    Visible=true;
    ClipSubWidgets=false;
    MouseOver=false;
	HaveFocus=false;
	Color=0xFF808080;
	Rect=Rct(0,0,0,0);
	AutoSetWidth=false;
	AutoSetHeight=false;

	lMargin=0;
	rMargin=0;
	tMargin=0;
	bMargin=0;

	lAttachMargin=0;
	rAttachMargin=0;
	tAttachMargin=0;
	bAttachMargin=0;

	NeibAttachType=wa_None;
	WhatAttachToParent=wa_None;
	WhereAttachToParent=wa_None;

    static int CID=0;
    if(CID==0){
        CID=-1;
        FILE* F=fopen("serial.txt","r");
        if(F){
            fscanf(F,"%X",&CID);
            fclose(F);
        }
    }
    Code=CID;
}
BaseWidget::~BaseWidget(){
}
void BaseWidget::SetHint(const char* s){
	TextManager::Init();
    if(s && s[0])Hint=TextManager::GetText(s);
    else Hint=NULL;
}
void BaseWidget::_draw(){
	if(!MouseOver)EnablePopup=false;
	if(Visible){
        SubWidgets.SetParent(this);
		CallCallbacks(WM_PAINT);
		for(int i=0;i<SubWidgets.GetAmount();i++){
			BaseWidget* bw=SubWidgets[i];			
			bw->_draw();
		}
	}
}
void BaseWidget::_setup_positions(){
	if(Visible){
		bool req=false;
		float minX=CurrentPos.x;
		float maxX=-FLT_MAX;
		float minY=CurrentPos.y;
		float maxY=-FLT_MAX;
		for(int i=0;i<SubWidgets.GetAmount();i++){
			BaseWidget* bw=SubWidgets[i];
			if(bw->EnablePopup){
				bw->CurrentPos=bw->PopupPos;				
			}else{
				bw->CurrentPos.x=CurrentPos.x+bw->Rect.x+lMargin;
				bw->CurrentPos.y=CurrentPos.y+bw->Rect.y+tMargin;			
			}
			bw->_setup_positions();			
			minX=min(minX,bw->CurrentPos.x);
			minY=min(minY,bw->CurrentPos.y);
			maxX=max(maxX,bw->CurrentPos.x);
			maxY=max(maxY,bw->CurrentPos.y);
		}
		if(AutoSetWidth){
			Rect.w=maxX-CurrentPos.x+rMargin;
		}
		if(AutoSetHeight){
			Rect.h=maxY-CurrentPos.y+bMargin;
		}
	}
}
void BaseWidget::AddCallback(DWORD Message,WBaseCallback* cb){
    cb->Message=Message;
    Callbacks.Add(cb);
}
void BaseWidget::AddFirstCallback(DWORD Message,WBaseCallback* cb){
    cb->Message=Message;
    Callbacks.Insert(0,cb);
}
BaseWidget* BaseWidget::CurrentWidget=NULL;
bool BaseWidget::CallCallbacks(DWORD Message){
    bool res=false;
    CurrentWidget=this;
    for(int i=0;i<Callbacks.GetAmount();i++){
        if(Callbacks[i]->Message==Message){
            res |= Callbacks[i]->call();
        }
    }
    return res;
}
bool BaseWidget::_tmp_AssignPopup(int F,int Key,int Shift,int Ctrl,int Alt){
	if(Widgets::lastKey==Key){
		int _shift=(GetKeyState(VK_SHIFT)&0x8000)?1:0;
		int _ctrl=(GetKeyState(VK_CONTROL)&0x8000)?1:0;
		int _alt=(GetKeyState(VK_MENU)&0x8000)?1:0;
		if(Shift==_shift && Ctrl==_ctrl && Alt==_alt){
			POINT p;
			if(GetCursorPos2(&p)){
				BaseWidget* W=(BaseWidget*)F;
				W->PopupPos.x=p.x-W->Rect.w/2;
				W->PopupPos.y=p.y-W->Rect.h/2;
				W->Visible=true;
				W->MouseOver=true;
				W->EnablePopup=true;
				return true;
			}
		}
	}
	return false;
}
void BaseWidget::AssignPopup(BaseWidget* RootWidget,int Key,bool Shift,bool Ctrl,bool Alt){
    RootWidget->AddCallback(WM_KEYDOWN,new wpcallback<BaseWidget>(this,_tmp_AssignPopup,int(this),Key,Shift,Ctrl,Alt));
}
ClassArray<BaseWidget> Widgets::ActiveWidgets;
_str Widgets::CurrentHint;

int  Widgets::MouseX;
int  Widgets::MouseY;
bool Widgets::lPressed;
bool Widgets::rPressed;
bool Widgets::mPressed;
int  Widgets::lastKey;
int  Widgets::lastWheel;

void Widgets::AddActiveWidget(BaseWidget* W){
    ActiveWidgets.Add(W);
}
void Widgets::RemoveActiveWidget(BaseWidget* W){
    int pos=ActiveWidgets.find(W);
    if(pos!=-1){
        ActiveWidgets.DelElement(pos);
    }
}
void Widgets::RemoveAll(){
    ActiveWidgets.Clear();
}

bool Widgets::ProcessWindowsMessages(MSG& msg){
    static DynArray<BaseWidget*> List;
    List.FastClear();
    if( msg.message == WM_LBUTTONDOWN ||
        msg.message == WM_LBUTTONUP   ||
        msg.message == WM_RBUTTONDOWN ||
        msg.message == WM_RBUTTONUP   ||
        msg.message == WM_MBUTTONDOWN ||
        msg.message == WM_MBUTTONUP   ||

        msg.message == WM_LBUTTONDBLCLK ||
        msg.message == WM_MBUTTONDBLCLK ||
        msg.message == WM_RBUTTONDBLCLK ||
        msg.message == WM_MOUSEMOVE       ){

            MouseX = LOWORD( msg.lParam );
            MouseY = HIWORD( msg.lParam );

            CreateWidgetsListUnderPoint(NULL,MouseX, MouseY,List);
            if(msg.message == WM_LBUTTONDOWN)lPressed=true;
            if(msg.message == WM_LBUTTONUP)lPressed=false;
            if(msg.message == WM_RBUTTONDOWN)rPressed=true;
            if(msg.message == WM_RBUTTONUP)rPressed=false;
            if(msg.message == WM_MBUTTONDOWN)mPressed=true;
            if(msg.message == WM_MBUTTONUP)mPressed=false;
            for(int i=List.GetAmount()-1;i>=0;i--){
                BaseWidget* bw=List[i];
                if(bw->CallCallbacks( msg.message ))return true;
            }
    }
    if(msg.message == WM_MOUSEWHEEL){
        lastWheel = (short) HIWORD( msg.wParam );
        CreateWidgetsListUnderPoint(NULL,MouseX, HIWORD( msg.lParam ),List);
        for(int i=List.GetAmount()-1;i>=0;i--){
            BaseWidget* bw=List[i];
            if(bw->CallCallbacks( msg.message ))return true;
        }
    }
    if(msg.message == WM_KEYDOWN || msg.message == WM_KEYUP){
        lastKey = msg.wParam;
        CreateWidgetsList(NULL,List);
        for(int i=List.GetAmount()-1;i>=0;i--){
            BaseWidget* bw=List[i];
            if(bw->HaveFocus && bw->CallCallbacks( msg.message ))return true;
        }
        for(int i=List.GetAmount()-1;i>=0;i--){
            BaseWidget* bw=List[i];
            if(bw->CallCallbacks( msg.message ))return true;
        }
    }
	return false;
}
void Widgets::DrawWidgets(){	
	for(int i=0;i<ActiveWidgets.GetAmount();i++){
		BaseWidget* bw=ActiveWidgets[i];		
		bw->CurrentPos.x=bw->Rect.x+bw->lMargin;
		bw->CurrentPos.y=bw->Rect.y+bw->tMargin;
		bw->_setup_positions();
	}
    for(int i=0;i<ActiveWidgets.GetAmount();i++){
        BaseWidget* bw=ActiveWidgets[i];		
		bw->_draw();
    }
	if(CurrentHint.pchar()[0]){		
		int L=GetTextWidth("%s",CurrentHint.pchar());
		Rct rct=IRS->GetViewPort();		
		static int tex=IRS->GetTextureID("stdbutton.dds");
		int h=-DrawBottomAlignedText(0,0,CurrentHint.pchar(),rct.w-20,12,0xFFFFFFFF,0,false);
		DrawFrame(Rct(0,rct.h-h,rct.w,h),tex,Rct(0,0,1,1),0x80000090);
		DrawBottomAlignedText(10,rct.h,CurrentHint.pchar(),rct.w-20,12,0xFFFFFFFF,0,true);
	}
	FlushText();
}
void Widgets::CreateWidgetsListUnderPoint(BaseWidget* W,int x,int y,DynArray<BaseWidget*>& List){
    if(W){
        if(W->Visible){
            if(x>=W->CurrentPos.x && y>=W->CurrentPos.y && x<W->CurrentPos.x+W->Rect.w && y<W->CurrentPos.y+W->Rect.h){
                List.Add(W);
                W->MouseOver=true;
                for(int i=0;i<W->SubWidgets.GetAmount();i++){                
                    CreateWidgetsListUnderPoint(W->SubWidgets[i],x,y,List);
                }
            }else{
                if(!W->ClipSubWidgets){
                    for(int i=0;i<W->SubWidgets.GetAmount();i++){                
                        CreateWidgetsListUnderPoint(W->SubWidgets[i],x,y,List);
                    }
                }
            }
        }
    }else{
        ClearMouseOver(NULL);
        for(int i=0;i<ActiveWidgets.GetAmount();i++){
            BaseWidget* bw=ActiveWidgets[i];
            CreateWidgetsListUnderPoint(bw,x,y,List);
        }
		CurrentHint="";
		int N=List.GetAmount();
		if(N){
			for(int i=N-1;i>=0;i--){
				char* s=List[i]->Hint.pchar();
				if(s[0]){
					CurrentHint=s;
                    break;
				}
			}
		}else CurrentHint="";
    }
}
void Widgets::CreateWidgetsList(BaseWidget* W,DynArray<BaseWidget*>& List){
    if(W){
        if(W->Visible){        
            List.Add(W);            
            for(int i=0;i<W->SubWidgets.GetAmount();i++){                
                CreateWidgetsList(W->SubWidgets[i],List);
            }
        }        
    }else{        
        for(int i=0;i<ActiveWidgets.GetAmount();i++){
            BaseWidget* bw=ActiveWidgets[i];
            CreateWidgetsList(bw,List);
        }
    }
}
void Widgets::ClearMouseOver(BaseWidget* W){
    if(W){
        W->MouseOver=false;
        for(int i=0;i<W->SubWidgets.GetAmount();i++){                
           ClearMouseOver(W->SubWidgets[i]);
        }        
    }else{
        for(int i=0;i<ActiveWidgets.GetAmount();i++){
            BaseWidget* bw=ActiveWidgets[i];
            ClearMouseOver(bw);
        }
    }
}
void Widgets::KillFocus(BaseWidget* W){
    if(W){
        W->HaveFocus=false;
        for(int i=0;i<W->SubWidgets.GetAmount();i++){                
           KillFocus(W->SubWidgets[i]);
        }        
    }else{
        for(int i=0;i<ActiveWidgets.GetAmount();i++){
            BaseWidget* bw=ActiveWidgets[i];
            KillFocus(bw);
        }
    }
}
bool BaseWidget::skip_lclick(){
    Widgets::lPressed=false;
    return false;
}
bool BaseWidget::skip_rclick(){
    Widgets::rPressed=false;
    return false;
}
//widgets implementations
void PanelWidget::Init(){	
	AddCallback(WM_PAINT,new wpcallback<PanelWidget>(this,&PanelWidget::draw));    
}
bool PanelWidget::draw(){
	Rct R(CurrentPos.x,CurrentPos.y,Rect.w,Rect.h);
	rsRect(R,0,Color,Color,Color,Color);
	rsFlush();
	return false;
}
void DrawTexRect::Init(){
	static int sh=IRS->GetShaderID("hud_smooth");
	static int tex=IRS->GetTextureID("default.dds");
	Shader=sh;	
	uv=Rct(0,0,1,1);
	AddCallback(WM_PAINT,new wpcallback<DrawTexRect>(this,&DrawTexRect::draw));
}
DrawTexRect::DrawTexRect(const char* name,int idx,int nx,int ny){
    Init();
	Color=0xFFFFFFFF;
	SetIcon(IRS->GetTextureID(name),idx,nx,ny);    
}
DrawTexRect::DrawTexRect(int tex,int idx,int nx,int ny){
	Init();
	Color=0xFFFFFFFF;
	SetIcon(tex,idx,nx,ny);
}
void DrawTexRect::SetIcon(int tex,int idx,int nx,int ny){	
	Textures[0]=tex;
	float w=1.0f/nx;
	float h=1.0f/ny;
	uv=Rct(w*(idx%nx),h*(idx/nx),w,h);
}
bool DrawTexRect::draw(){
	Rct R(CurrentPos.x,CurrentPos.y,Rect.w,Rect.h);
	rsSetShader(Shader);
	rsSetTexture(Textures[0]);
	rsSetTexture(Textures[1],1);
	IRS->SetTexture(Textures[2],2);
	IRS->SetTexture(Textures[3],3);
	rsRect(R,uv,0,Color,Color,Color,Color);
	rsFlush();
	rsRestoreShader();
	return false;
}
void TextWidget::Init(){
	Align=1;
	Size=8;
	AddCallback(WM_PAINT,new wpcallback<TextWidget>(this,&TextWidget::draw));
	Color=0xFFFFFFFF;
}
bool TextWidget::draw(){
	SetCurrentFont(Size);
	Rct R(CurrentPos.x,CurrentPos.y,Rect.w,Rect.h);		
	if(Align==0)DrawText(R.x,R.y,Color,"%s",Text.pchar());
	else{
		float L=GetTextWidth("%s",Text.pchar());
		if(Align==1){
			DrawText(R.x+(R.w-L)/2-1,R.y+1,0x80000000,"%s",Text.pchar());
            DrawText(R.x+(R.w-L)/2,R.y,Color,"%s",Text.pchar());
		}else{
			DrawText(R.x+(R.w-L)/2-1,R.y,0x80000000,"%s",Text.pchar());
			DrawText(R.x+(R.w-L)/2,R.y,Color,"%s",Text.pchar());
		}
	}
	SetCurrentFont(8);
	return false;
}
void FrameWidget::Init(){	
	static int tex=IRS->GetTextureID("stdbutton.dds");
	uv=Rct(0,0,1,1);
	FrameTexture=tex;
	AddCallback(WM_PAINT,new wpcallback<FrameWidget>(this,&FrameWidget::draw));
    AddCallback(WM_LBUTTONDOWN,new wpcallback<BaseWidget>(this,&BaseWidget::skip_lclick));
    AddCallback(WM_LBUTTONDOWN,new wpcallback<BaseWidget>(this,&BaseWidget::skip_rclick));	
}
bool FrameWidget::draw(){
	Rct R(CurrentPos.x,CurrentPos.y,Rect.w,Rect.h);		
	DrawFrame(R,FrameTexture,uv,Color);
	return false;
}


SimpleSlider::SimpleSlider(DWORD _Color,float* ptr,float minv,float maxv){
    Init();
    VariablePtr=ptr;
    MinValue=minv;
    MaxValue=maxv;
    Color=_Color;
}
bool SimpleSlider::draw(){
    DWORD C1=_ModDWORDRGB(Color,0xFF909090);
	C1&=0xFFFFFF;
	C1|=0x80000000;
    float v=CurrPos;
    if(VariablePtr)v=*VariablePtr;
    float p=floorf((v-MinValue)*Rect.w/(MaxValue-MinValue));
    if(p<0)p=0;
    if(p>Rect.w)p=Rect.w;
	float h1=(Rect.h-1)*0.3;
	float h2=(Rect.h-1)*0.7;
	rsRestoreShader();
    rsRect(Rct(CurrentPos.x,CurrentPos.y,p,h1),0,C1,C1,Color,Color);
	rsRect(Rct(CurrentPos.x,CurrentPos.y+h1,p,h2-h1+1),0,Color,Color,Color,Color);	
    rsRect(Rct(CurrentPos.x,CurrentPos.y+h2,p,Rect.h-h2-1),0,Color,Color,C1,C1);
	rsFlush();
	static int tex=IRS->GetTextureID("stdbutton.dds");
	DrawFrame(Rct(CurrentPos.x-4+p,CurrentPos.y,9,Rect.h),tex,Rct(0,0,1,1),Color);
    return false;
}
bool SimpleSlider::on_mmove(){
    if(GetKeyState(VK_LBUTTON)&0x8000){
        float dx=Widgets::MouseX-CurrentPos.x;
        float v=MinValue+dx*(MaxValue-MinValue)/Rect.w;
        CurrPos=v;
        if(VariablePtr)*VariablePtr=v;
		Widgets::lPressed=false;
        return true;
    }
    return false;
}
void SimpleSlider::Init(){
    VariablePtr=NULL;
    CurrPos=0;
    MinValue=0;
    MaxValue=1;
    AddCallback(WM_PAINT,new wpcallback<SimpleSlider>(this,&SimpleSlider::draw));
    AddCallback(WM_LBUTTONDOWN,new wpcallback<SimpleSlider>(this,&SimpleSlider::on_mmove));
    AddCallback(WM_MOUSEMOVE,new wpcallback<SimpleSlider>(this,&SimpleSlider::on_mmove));
}
bool GlobalProcessWidgets(MSG& msg){	
	try{
		return Widgets::ProcessWindowsMessages(msg);
	}catch(...){}
	return true;
}
void PictMenu::Create(Rct R,int Icon,int Nx,int Ny,int* _VariablePtr,int _Align){
	Rect=R;
	IconsTexture=Icon;
	IcNx=Nx;
	IcNy=Ny;
	VariablePtr=_VariablePtr;
	CurrPos=0;
	Margin=2;
	Align=_Align;
	FW=new FrameWidget;
	FW->Color=Color;
	FW->Rect=Rct(0,0,R.w,R.h);
	SubWidgets.Add(FW);
	FW=new FrameWidget;
	FW->Color=Color;
	FW->Rect=Rct(0,0,R.w,R.h);	
	BG=new DrawTexRect(Icon,0,Nx,Ny);
	BG->Rect=Rct(Margin,Margin,R.w-Margin*2,R.h-Margin*2);		
	FW->Color=Color;
	SubWidgets.Add(FW);
	SubWidgets.Add(BG);	
	CommandStyle=false;
	HeaderPicture=-1;

	FW->AddCallback(WM_PAINT,new wpcallback<PictMenu>(this,HideSubmenu,int(this)));
	AddCallback(WM_PAINT,new wpcallback<PictMenu>(this,OnDraw));
	FW->AddCallback(WM_LBUTTONUP,new wpcallback<PictMenu>(this,CloseOnClick));
	AddCallback(WM_MOUSEMOVE,new wpcallback<PictMenu>(this,ShowSubmenu,int(FW)));

}
bool PictMenu::ShowSubmenu(int param){
	BaseWidget* bw=(BaseWidget*)param;
	bw->Visible=true;
	return false;
}
bool PictMenu::HideSubmenu(int param){
	if(!BaseWidget::CurrentWidget->MouseOver){
		BaseWidget* bw=(BaseWidget*)param;
		if(!bw->MouseOver)
			BaseWidget::CurrentWidget->Visible=false;
	}
	return false;
}
bool PictMenu::CloseOnClick(){
	BaseWidget::CurrentWidget->MouseOver=false;
	BaseWidget::CurrentWidget->Visible=false;
	return true;
}
bool PictMenu::OnClick(int idx){
    CurrPos=idx;
	if(VariablePtr)*VariablePtr=idx;
	return false;
}
void PictMenu::Init(){    
}
void PictMenu::AssignSubPopup(int Key,bool Shift,bool Ctrl,bool Alt){
	FW->AssignPopup(this,Key,Shift,Ctrl,Alt);	
}
bool PictMenu::OnDraw(){
	if(BG && Icons.GetAmount()){
		BG->SetIcon(IconsTexture,HeaderPicture==-1?Icons[CurrPos]:HeaderPicture,IcNx,IcNy);
		BG->Rect=Rct((Rect.w-Rect.h)/2+Margin,Margin,Rect.h-Margin*2,Rect.h-Margin*2);
	}
	if(FW){
		int n=Icons.GetAmount();
		if(FW->SubWidgets.GetAmount()!=n){
			FW->SubWidgets.Clear();
			for(int i=0;i<n;i++){
				DrawTexRect* TR=new DrawTexRect;
				//TR->Color=Color;
				TR->Color=0xFFFFFFFF;
				TR->AddCallback(WM_LBUTTONUP,new wpcallback<PictMenu>(this,OnClick,i));				
				TR->SetIcon(IconsTexture,Icons[i],IcNx,IcNy);
				TR->SetHint(Hints[i]);
				FW->SubWidgets.Add(TR);
			}
		}
		if(Align==0){//left
			float w=Rect.w;
			float h=Rect.h;
			float ww=Margin*2+n*(h-Margin*2);
            FW->Rect=Rct(-ww,0,ww,h);			
			for(int i=0;i<n;i++){
				DrawTexRect* TR=(DrawTexRect*)FW->SubWidgets[i];
				TR->Rect=Rct(ww-Margin-(h-Margin*2)*(i+1),Margin,h-Margin*2,h-Margin*2);
			}
		}
		if(Align==1){//right
			float w=Rect.w;
			float h=Rect.h;
			float ww=Margin*2+n*(h-Margin*2);
			FW->Rect=Rct(w,0,ww,h);			
			for(int i=0;i<n;i++){
				DrawTexRect* TR=(DrawTexRect*)FW->SubWidgets[i];
				TR->Rect=Rct(Margin+(h-Margin*2)*i,Margin,h-Margin*2,h-Margin*2);
			}
		}
		if(Align==2){//top
			float w=Rect.w;
			float h=Rect.h;
			float ww=Margin*2+n*(h-Margin*2);
			FW->Rect=Rct(0,-ww,w,ww);			
			for(int i=0;i<n;i++){
				DrawTexRect* TR=(DrawTexRect*)FW->SubWidgets[i];
				TR->Rect=Rct(Margin,-Margin-(w-Margin*2)*(i+1),w-Margin*2,h-Margin*2);
			}
		}
		if(Align==3){//bottom
			float w=Rect.w;
			float h=Rect.h;
			float ww=Margin*2+n*(h-Margin*2);
			FW->Rect=Rct(0,h,w,ww);
			for(int i=0;i<n;i++){
				DrawTexRect* TR=(DrawTexRect*)FW->SubWidgets[i];
				TR->Rect=Rct(Margin,Margin+(w-Margin*2)*i,w-Margin*2,h-Margin*2);
			}
		}
	}
    return false;
}
void PictMenu::CreateDefault(Rct R,int* _VariablePtr,int Align){
    int Tex=IRS->GetTextureID("icons\\baseicons.dds");
	if(Tex>=0){
		int nx=IRS->GetTextureWidth(Tex)/64;
		int ny=IRS->GetTextureHeight(Tex)/64;
        Create(R,Tex,nx,ny,_VariablePtr,Align);
	}
}
void PictMenu::AddPict(int Icon,const char* Hint){
    Icons.Add(Icon);
	Hints.Add(Hint);
}
//text menu
bool TextMenu::ShowSubmenu(){
    Rct vp=IRS->GetViewPort();
    if(Align==0 || Align==1){//left,right
        if(CurrentPos.x<vp.w/2){
            Align=1;
            FW->Rect.x=Rect.w;
        }else{
            Align=0;
            FW->Rect.x=-FW->Rect.w;
        }
        if(CurrentPos.y<vp.h/2){
            FW->Rect.y=0;
        }else{
            FW->Rect.y=Rect.h-FW->Rect.h;
        }        
    }
    if(Align==2 || Align==3){//top,bottom
        if(CurrentPos.y<vp.h/2){
            Align=3;
            FW->Rect.y=Rect.h;
        }else{
            Align=2;
            FW->Rect.y=-FW->Rect.h;
        }
        if(CurrentPos.x<vp.w/2){
            FW->Rect.x=0;
        }else{
            FW->Rect.x=Rect.w-FW->Rect.w;
        }        
    }
    FW->Visible=true;
    return false;
}
bool TextMenu::HideSubmenu(int param){
	if(!BaseWidget::CurrentWidget->MouseOver){
		BaseWidget* bw=(BaseWidget*)param;
		if(!bw->MouseOver)
			BaseWidget::CurrentWidget->Visible=false;
	}
	return false;
}
bool TextMenu::CloseOnClick(){
	BaseWidget::CurrentWidget->MouseOver=false;
	BaseWidget::CurrentWidget->Visible=false;
	return true;
}

bool TextMenu::OnDraw(){ 
    if(ParentBC && ParentBC->GetParent()){
        BaseWidget* W=(BaseWidget*)ParentBC->GetParent();
        Rect=Rct(0,0,W->Rect.w,W->Rect.h);
    }
    return false;
}
bool TextMenu::DrawSel(int Ptr){
    BaseWidget* BW=(BaseWidget*)Ptr;    
    if(BW->MouseOver){        
        //DrawFrame(Rct(BW->CurrentPos.x,BW->CurrentPos.y-Margin,BW->Rect.w,BW->Rect.h+Margin),-1,Rct(0,0,1,1),0x60FFFFFF);
        DrawFrame(Rct(FW->CurrentPos.x,BW->CurrentPos.y-2,FW->Rect.w,BW->Rect.h+2),-1,Rct(0,0,1,1),0x80FFFFFF);
    }
    return false;
}
void TextMenu::Init(){
    Align=3;    
    Margin=4;
    FW=new FrameWidget;
    FW->AddCallback(WM_PAINT,new wpcallback<TextMenu>(this,HideSubmenu,int(this)));
	AddCallback(WM_PAINT,new wpcallback<TextMenu>(this,OnDraw));
	FW->AddCallback(WM_LBUTTONUP,new wpcallback<TextMenu>(this,CloseOnClick));
	AddCallback(WM_MOUSEMOVE,new wpcallback<TextMenu>(this,ShowSubmenu));    
    SubWidgets.Add(FW);    
}
TextMenu::TextMenu(int a,int e){
    Init();
    Align=a;    
    ElmAlign=e;
}
void TextMenu::SetupPositions(){
    float y0=Margin;
    float x0=Margin+8;
    float maxx=0;
    float maxy=0;    
    bool hAlign=false;
    for(int i=0;i<FW->SubWidgets.GetAmount();i++){
        BaseWidget* BW=FW->SubWidgets[i];
        BW->Rect.x=x0;
        BW->Rect.y=y0;
        float x1=x0+BW->Rect.w;
        float y1=y0+BW->Rect.h;
        if(hAlign)x0+=BW->Rect.w;
        else y0+=BW->Rect.h;        
        maxx=max(maxx,x1);
        maxy=max(maxy,y1);        
    }
    FW->Rect.w=maxx+Margin+8;
    FW->Rect.h=maxy+Margin;
}
TextWidget* TextMenu::AddLine(const char* Text,const char* Hint){
    TextWidget* TW=new TextWidget;
    Text=TextManager::GetText(Text);
	TW->Size=10;
	SetCurrentFont(TW->Size);
    TW->SetHint(Hint);
    TW->Text=Text;
    TW->Align=ElmAlign;
    TW->Rect.w=GetTextWidth("%s",Text);	
    TW->Rect.h=GetTextHeight()+5;
	SetCurrentFont(8);
    TW->AddFirstCallback(WM_PAINT,new wpcallback<TextMenu>(this,DrawSel,int(TW)));
    FW->SubWidgets.Add(TW);
    SetupPositions();
    return TW;
}
bool TextMenu::DrawSep(int Ptr){
    BaseWidget* BW=(BaseWidget*)Ptr;
    BaseWidget::CurrentWidget->Rect.w=BW->Rect.w-22;
    return false;
}
void TextMenu::AddSeparator(){
    FrameWidget* SEP=new FrameWidget;
    SEP->Rect.w=0;
    SEP->Rect.h=4;
    SEP->AddFirstCallback(WM_PAINT,new wpcallback<TextMenu>(this,DrawSep,int(FW)));    
    FW->SubWidgets.Add(SEP);
    BaseWidget* BW=new BaseWidget;
    BW->Rect=Rct(0,0,6,6);
    FW->SubWidgets.Add(BW);
    SetupPositions();    
}
void TextMenu::DelLine(int idx){
    FW->SubWidgets.DelElement(idx);
    SetupPositions();
}