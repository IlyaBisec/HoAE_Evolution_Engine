extern DWORD RedColor;
extern DWORD GreenColor;
extern DWORD BlueColor;
extern DWORD GreyColor;
bool GetCursorPos2(POINT* p);

class PenInterface{
public:
	static int width;
	static int height;
	static int texborder;
	static int texborder2;
	static DWORD Color;
	static void CreatePenInterface();
	static PenInterface* instance();
	static int WhiteTexture;
	static int StdShader;
	static int BumpShader;
	static int IconsTexture;
	static const char* IconsName;
    static BaseWidget* Root;
    static BaseWidget* ToolPanel;
	static BaseWidget* ColorTexturePreview;
	static BaseWidget* SpecularTexturePreview;
	static BaseWidget* NormalMapPreview;
    static BaseWidget* LayersMenu;
    static int sliderheight;
	//pen shape submenu
	static void CreateTexPreview(int TextureID,int ShaderID,BaseWidget** W);
	bool CloseTexPreview();
	bool ViewTextures();
	bool SetPenShapeTexture();
	bool PenShapeTextureMenu(int idx);
	bool DrawPenShapeSubmenu();
	bool ShowSubmenu(int param);
	bool HideSubmenu(int);
	bool CloseOnClick();
	bool SelectPenShape(int idx);
    bool SetPenStripTexture();
    bool SelectPenStrip(int idx);
    bool DrawPenStripSubmenu();
    bool AddPenStrip();
    bool PenStripTextureMenu(int idx);
	static void AddText(BaseWidget* Base,const char* Text,int dy=0);
    static void AddHeader(int& y,const char* Tex);
    static int  GetToolHeight();
    static void AddToolHeader(const char* Tex);
    static void CreateStripMenu(BaseWidget* W);
    static SimpleSlider* AddToolSlider(float MinValue,float MaxValue,float* ValuePtr,DWORD Color,const char* Message,const char* Hint);
    static BaseWidget* AddToolButton(int IconID,const char* Hint);

	//matherial select submenu
	static DrawTexRect* CloseMtl;
	bool DrawMatherialIcon();
	bool DrawMatherialSubmenu();
	bool CloseCurMtl();    	
	
	bool DisableChannel(int PatternIndex){
		GeneralPen::EnableChannel[PatternIndex]=!GeneralPen::EnableChannel[PatternIndex];		
		GeneralPen::SavePen();
		return true;
	}
	bool SelectCurrentTexture(int PatternIndex){
		GeneralPen::Pens[GeneralPen::CurrentPen]->SelectTexture(PatternIndex);
		GeneralPen::EnableChannel[PatternIndex]=true;
        return false;
	}
	bool AssignPatternTexture(int PatternIndex,int W){
		DrawTexRect* TEX=(DrawTexRect*)W;
		if(GeneralPen::CurrentPenPtr->Patterns[PatternIndex]){
			TEX->Textures[0]=GeneralPen::CurrentPenPtr->Patterns[PatternIndex]->TextureID;			
			TEX->Textures[1]=WhiteTexture;
			TEX->Textures[2]=WhiteTexture;
			float du=0.25;
			float T=GetTickCount()/8000.0f;
			float u0=(1+cos(T))*(1-du)/2;
			float v0=(1+cos(T*1.2))*(1-du)/2;
			TEX->uv=Rct(u0,v0,du,du);
			if(PatternIndex==0)TEX->Shader=BumpShader;
			else TEX->Shader=StdShader;
		}else{
			static int T=IRS->GetTextureID("icons\\baseicons.dds");
			TEX->SetIcon(T,4,icn_lx,icn_ly);
			TEX->Shader=StdShader;
		}
		return false;
	}
	bool AssignPatternTexture(int PatternIndex,int PenIndex,int W){
		OnePenStyle* OPS=GeneralPen::CurrentPenPtr;
		GeneralPen::CurrentPenPtr=GeneralPen::Pens[PenIndex];
		AssignPatternTexture(PatternIndex,W);
		GeneralPen::CurrentPenPtr=OPS;
		return false;
	}
	bool AssignMtlTexture(int PenIndex,int W){
		DrawTexRect* TEX=(DrawTexRect*)W;
		int idx=PenIndex==-1?GeneralPen::CurrentPen:PenIndex;
		OnePenStyle* OPS=GeneralPen::Pens[idx];
		if(OPS){
			for(int i=0;i<3;i++){
				TEX->Textures[i]=WhiteTexture;
				if(OPS->Patterns[i] /*&& OPS->EnablePattern[i]*/)TEX->Textures[i]=OPS->Patterns[i]->TextureID;
			}
			float du=0.25;
			float T=GetTickCount()/8000.0f;
			float u0=(1+cos(T))*(1-du)/2;
			float v0=(1+cos(T*1.2))*(1-du)/2;
			TEX->uv=Rct(u0,v0,du,du);			
			TEX->Shader=BumpShader;
		}else{
			static int T=IRS->GetTextureID(IconsName);
			TEX->SetIcon(T,4,icn_lx,icn_ly);		
			TEX->Shader=StdShader;
		}
		return false;
	}
	bool SelectBlank(int PatternIndex,int value){
		GeneralPen::CurrentPenPtr->EnablePattern[PatternIndex]=!!value;		
		GeneralPen::EnableChannel[PatternIndex]=true;
		GeneralPen::SavePen();
		return false;
	}
	bool DisablePattern(int PatternIndex){
		GeneralPen::EnableChannel[PatternIndex]=false;
		GeneralPen::SavePen();
		return false;
	}
	bool ControlDisable(int PatternIndex,int Ptr){
		DrawTexRect* TR=(DrawTexRect*)Ptr;
        TR->Visible=!GeneralPen::EnableChannel[PatternIndex];		
		return false;
	}
	bool ControlTextureVisibility(int PatternIndex,int TexPtr){
        DrawTexRect* TR=(DrawTexRect*)TexPtr;
		TR->Visible=GeneralPen::CurrentPenPtr->EnablePattern[PatternIndex];
		return false;
	}
	bool ControlBlankVisibility(int PatternIndex,int TexPtr){
		DrawTexRect* TR=(DrawTexRect*)TexPtr;
		TR->Visible=!GeneralPen::CurrentPenPtr->EnablePattern[PatternIndex];
		return false;
	}
	bool AssignPopup(int F,int Key,int Shift,int Ctrl,int Alt){
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
	bool AddPenShape();
	//create pattern menu
	bool CreatePatternMenu(int PatternIndex,int FramePtr,int ShaderID){        
		
        static const char* IDS_HINT[]={"ID_BUMP_HINT","ID_COLOR_HINT","ID_OPACITY_HINT","ID_SPECULAR_HINT"};
        static const char* IDS_TEX[]={"ID_BUMP_TEX_HINT","ID_COLOR_TEX_HINT","ID_OPACITY_TEX_HINT","ID_SPECULAR_TEX_HINT"};
        static const char* IDS_OPEN[]={"ID_BUMP_OPEN_HINT","ID_COLOR_OPEN_HINT","ID_OPACITY_OPEN_HINT","ID_SPECULAR_OPEN_HINT"};
        static const char* IDS_BLANK[]={"ID_BUMP_BLANK_HINT","ID_COLOR_BLANK_HINT","ID_OPACITY_BLANK_HINT","ID_SPECULAR_BLANK_HINT"};
        static const char* IDS_DISABLE[]={"ID_BUMP_DISABLE_HINT","ID_COLOR_DISABLE_HINT","ID_OPACITY_DISABLE_HINT","ID_SPECULAR_DISABLE_HINT"};
		static char KeyID[]={'D','C',0,'R'};

		FrameWidget* FW=(FrameWidget*)FramePtr;		
		if(FW->SubWidgets.GetAmount()<=1){			
			FrameWidget* FW0=FW;
            FrameWidget* FWS=new FrameWidget;
			FW->SubWidgets.Add(FWS);
			FWS->Rect=Rct(-20,0,20,height);
			FWS->Color=Color;
			pi_callback5(WM_KEYDOWN,FW,AssignPopup,int(FWS),KeyID[PatternIndex],0,0,0)
            pi_callback1(WM_PAINT,FWS,HideSubmenu,int(FW));
			pi_callback(WM_LBUTTONUP,FWS,CloseOnClick);
			pi_callback1(WM_MOUSEMOVE,FW,ShowSubmenu,int(FWS));
			FW=FWS;
            FW0->SetHint(IDS_HINT[PatternIndex]);

			int n=4;			
			int tw=n*(height-texborder2)+texborder2;
			FW->Rect=Rct(-tw,0,tw,height);
			int posx=texborder;

			DrawTexRect* OPEN=new DrawTexRect(IconsName,1,icn_lx,icn_ly);
			OPEN->Rect=Rct(posx,texborder,height-texborder2,height-texborder2);				
			pi_callback1(WM_LBUTTONUP,OPEN,SelectCurrentTexture,PatternIndex);
			FW->SubWidgets.Add(OPEN);
            posx+=height-texborder2;
            OPEN->SetHint(IDS_OPEN[PatternIndex]);


			DrawTexRect* TEX=new DrawTexRect;
			TEX->Color=0xFFFFFFFF;
			TEX->Rect=Rct(posx,texborder,height-texborder2,height-texborder2);			
			TEX->Shader=ShaderID;
			pi_callback2(WM_PAINT,TEX,AssignPatternTexture,PatternIndex,int(TEX));
			pi_callback2(WM_LBUTTONUP,TEX,SelectBlank,PatternIndex,1);
			FW->SubWidgets.Add(TEX);
			posx+=height-texborder2;
            TEX->SetHint(IDS_TEX[PatternIndex]);

			DrawTexRect* TEX2=new DrawTexRect;
			TEX2->Color=0xFFFFFFFF;
			TEX2->Rect=Rct(texborder,texborder,width-texborder2,height-texborder2);			
			TEX2->Shader=ShaderID;
			pi_callback2(WM_PAINT,FW0,AssignPatternTexture,PatternIndex,int(TEX2));
			pi_callback2(WM_PAINT,FW0,ControlTextureVisibility,PatternIndex,int(TEX2));
			FW0->SubWidgets.Add(TEX2);	            

			DrawTexRect* BLANK=new DrawTexRect(IconsName,4,icn_lx,icn_ly);
			BLANK->Color=0xFFFFFFFF;
			BLANK->Rect=Rct(posx,texborder,height-texborder2,height-texborder2);
			pi_callback2(WM_LBUTTONUP,BLANK,SelectBlank,PatternIndex,0);
			FW->SubWidgets.Add(BLANK);
			posx+=height-texborder2;
            BLANK->SetHint(IDS_BLANK[PatternIndex]);

			DrawTexRect* BLANK2=new DrawTexRect(IconsName,4,icn_lx,icn_ly);
			BLANK2->Color=0xFFFFFFFF;
			BLANK2->Rect=Rct(texborder+(width-height)/2,texborder,height-texborder2,height-texborder2);			
			FW0->SubWidgets.Add(BLANK2);
			pi_callback2(WM_PAINT,FW0,ControlBlankVisibility,PatternIndex,int(BLANK2));
			

			DrawTexRect* DISABLE=new DrawTexRect(IconsName,0,icn_lx,icn_ly);
			DISABLE->Rect=Rct(posx,texborder,height-texborder2,height-texborder2);
			pi_callback1(WM_LBUTTONUP,DISABLE,DisablePattern,PatternIndex);
			FW->SubWidgets.Add(DISABLE);
			posx+=width-texborder2;			
            DISABLE->SetHint(IDS_DISABLE[PatternIndex]);

			DrawTexRect* DIS2=new DrawTexRect(IconsName,0,icn_lx,icn_ly);
			float d=height/6;
			DIS2->Rect=Rct(texborder+12+(width-height)/2,texborder+d,height-texborder2-d*2,height-texborder2-d*2);
			DIS2->Visible=false;
            DIS2->Color=0x80FFFFFF;
			FW0->SubWidgets.Add(DIS2);
			pi_callback1(WM_RBUTTONUP,FW0,DisableChannel,PatternIndex);
			pi_callback2(WM_PAINT,FW0,ControlDisable,PatternIndex,int(DIS2));
		}
		return false;
	}	

	bool AddNewMtl(){
		OnePenStyle* PS=new OnePenStyle;
		GeneralPen::Pens.Add(PS);
		GeneralPen::CurrentPen=GeneralPen::Pens.GetAmount()-1;
		GeneralPen::CurrentPenPtr=PS;
		return false;
	}
	bool SelectCurrentMtl(int Idx){
		GeneralPen::CurrentPen=Idx;
		GeneralPen::CurrentPenPtr=GeneralPen::Pens[Idx];
        return false;
	}
	bool ShowFileDialog(FileDialog& dlg,char* temp){		
		_chdir(temp);
		if (!dlg.Show()){
			_chdir(IRM->GetHomeDirectory());
			return false;
		}
		_getcwd(temp,512);
		_chdir(IRM->GetHomeDirectory());
		return true;
	}	
	//create matherials list menu
	bool CreateMtlList(){
		float width=height;
		BaseWidget* BW=BaseWidget::CurrentWidget;
		if(BW->SubWidgets.GetAmount()!=GeneralPen::Pens.GetAmount()+1){
			BW->SubWidgets.Clear();			
			//int tw=n*(width-texborder2)+texborder2;			
			int posx=texborder;

			int nsh=GeneralPen::Pens.GetAmount()+1;			
			int nx,ny;
			if(nsh>4){
				ny=sqrt(float(nsh));
				nx=(nsh+ny-1)/ny;
			}else{
				nx=nsh;
				ny=1;
			}
			int tw=nx*(width-texborder2)+texborder2;
			int th=ny*(height-texborder2)+texborder2;
			BW->Rect=Rct(-tw,0,tw,th);

			int p=0;
			for(int yy=0;yy<ny;yy++){
				int x0=(width-texborder2)*(nx-1)+texborder;
				int y0=(height-texborder2)*yy+texborder;
				for(int i=0;i<nx;i++){
					if(p==nsh-1){
						DrawTexRect* T1=new DrawTexRect(IconsName,2,icn_lx,icn_ly);
						T1->Color=0xFFFFFFFF;
						BW->SubWidgets.Add(T1);
						T1->Rect=Rct(x0,y0,width-texborder2,height-texborder2);
						T1->SetHint("ADD_NEW_MTL_HINT");
						pi_callback(WM_LBUTTONUP,T1,AddNewMtl);
						break;
					}else{
						DrawTexRect* T=new DrawTexRect;
						T->Color=0xFFFFFFFF;
						BW->SubWidgets.Add(T);
						T->Rect=Rct(x0,y0,width-texborder2,height-texborder2);
						pi_callback2(WM_PAINT,T,AssignMtlTexture,p,int(T));
						pi_callback1(WM_LBUTTONUP,T,SelectCurrentMtl,p);
						x0-=width-texborder2;
					}
					p++;
				}		
			}
			/*            
			DrawTexRect* TR=new DrawTexRect(IconsName,2,icn_lx,icn_ly);
			TR->Rect=Rct(texborder,texborder,width-texborder2,height-texborder2);
			TR->SetHint("ADD_NEW_MTL_HINT");
			pi_callback(WM_LBUTTONUP,TR,AddNewMtl);
			BW->SubWidgets.Add(TR);
			posx+=width-texborder2;
			n--;

			for(int i=0;i<n;i++){
                DrawTexRect* TR=new DrawTexRect(IconsName,4,icn_lx,icn_ly);
				TR->Rect=Rct(posx,texborder,width-texborder2,height-texborder2);
				pi_callback1(WM_LBUTTONUP,TR,SelectCurrentMtl,i);
				pi_callback2(WM_PAINT,TR,AssignMtlTexture,i,int(TR));
				BW->SubWidgets.Add(TR);
				posx+=width-texborder2;
			}
			*/
		}
		return false;
	}
	bool OpenMesh(){
		OpenFileDialog dlg;
		dlg.AddFilter( "3-brush files",    "*.3b"     );	
		dlg.SetDefaultExtension( "3b" );
		_chdir(IRM->GetHomeDirectory());
		static char temp[512]="UserData\\";		
		if (!ShowFileDialog(dlg,temp)) return false;
		PMS.LoadMesh(dlg.GetFilePath());
		return false;
	}
	bool SaveMesh(){
		SaveFileDialog dlg;
		dlg.AddFilter("3-brush files",    "*.3b"     );
		dlg.SetDefaultExtension( "3b" );
		_chdir(IRM->GetHomeDirectory());
		static char temp[512]="UserData\\";		
		if (!ShowFileDialog(dlg,temp)) return false;
		PMS.SaveMesh(dlg.GetFilePath());
		return false;
	}
	bool OpenObj(){
		OpenFileDialog dlg;
		dlg.AddFilter( "Obj files",    "*.obj"     );	
		dlg.SetDefaultExtension( "obj" );
		static char temp[512]="Samples\\";		
		if (!ShowFileDialog(dlg,temp))return false;		
		PMS.ReadObj(dlg.GetFilePath());
		return false;
	}
	bool LoadTexture(){
		OpenFileDialog dlg;
		dlg.AddFilter( "dds (8888) - files","*.dds"     );	
		dlg.AddFilter( "tga - files",    "*.tga"     );	
		dlg.AddFilter( "bmp - files",    "*.bmp"     );	
		dlg.SetDefaultExtension( "dds" );
		static char temp[512]="UserData\\";		
		if (!ShowFileDialog(dlg,temp))return false;		
		int T=IRS->GetTextureID(dlg.GetFilePath());
		if(T!=-1)PMS.ApplyTexture(T);
		return false;
	}
	bool SaveTex(){
		SaveFileDialog dlg;
		dlg.AddFilter( "Textures files", "*.dds" );
		dlg.SetDefaultExtension( "dds" );
		static char temp[512]="UserData\\";		
		if (!ShowFileDialog(dlg,temp)) return false;
		PMS.SaveTexture(dlg.GetFilePath());
		return false;
	}
	bool SaveNmap(){
		SaveFileDialog dlg;
		dlg.AddFilter( "Textures files", "*.dds" );
		dlg.SetDefaultExtension( "dds" );	
		static char temp[512]="UserData\\";		
		if (!ShowFileDialog(dlg,temp)) return false;
		PMS.SaveNormalmap(dlg.GetFilePath());
		return false;
	}	
	bool GetCurrColor(){
		Vector4D vc=GeneralPen::Color;
		vc*=255.0f;
		BaseWidget::CurrentWidget->Color=V4D2DW(vc)|0xFF000000;
		return false;
	}
	bool SelectColor(){
		Vector4D vc=GeneralPen::Color;		
		vc*=255.0f;
		PickColorDialog dlg( V4D2DW(vc) );
		if (dlg.Show())
		{
			DWORD color = dlg.GetColor();
			GeneralPen::Color=DW2V4D(color);			
			GeneralPen::Color/=255.0f;
		}
		return true;
	}	
	bool GetCurrSpecular(){
		float s=GeneralPen::SpecularDegree*255;
		Vector4D vc(s,s,s,255);
		BaseWidget::CurrentWidget->Color=V4D2DW(vc)|0xFF000000;
		return false;
	}
	bool SelectSpecular(){
		float s=GeneralPen::SpecularDegree*255;
		Vector4D vc(s,s,s,255);				
		PickColorDialog dlg( V4D2DW(vc) );
		if (dlg.Show())
		{
			DWORD color = dlg.GetColor();			
			GeneralPen::SpecularDegree=(color&255)/255.0f;
		}
		return true;
	}
    bool SaveLoPolyObj();
    bool SaveLoPolyTang();
    bool SaveLoPolyDisp();
    bool SaveMidPolyObj();
    bool SaveMidPolyTang();
    bool SaveMidPolyDisp();
    bool ExitProgram();
    bool ControlVisibleLayer(int LayerID);
    bool ToggleLayerVisibility(int LayerID);
    bool ControlLayerBackground(int LayerID);
    bool SelectCurrentLayer(int LayerID);
    bool DrawLayersInterface();
    static void CreateLayersInterface();
};
