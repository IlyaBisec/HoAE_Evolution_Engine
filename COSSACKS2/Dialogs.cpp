#include "stdheader.h"
#include "WeaponSystem.h"
#include "TestDialogs.h"
#include "TextVisualElements.h"
#include "cvi_MainMenu.h"
#include "ClassEditorsRegistration.h"
#include "kContext.h"
#include "GameInterface.h"
#include "Surface\Surface.h"
#include "UnitsInterface.h"
void UnPress();
void ErrMS(char* s);
extern int UNI_LINEDLY1;
extern int UNI_LINEDY1;
extern int UNI_LINEDLY2;
extern int UNI_LINEDY2;
extern int UNI_HINTDY1;
extern int UNI_HINTDLY1;
extern int UNI_HINTDY2;
extern int UNI_HINTDLY2;

extern bool InEdit;

struct DrawElm{
    SimpleDialog*   SD;    
    int             x,y;
    int             Level;
    DWORD           diffuse;
    Matrix4D        TM;    
    bool            HaveTM;
};
DynArray<DrawElm> DrawQueue;
int CurrentDlgLevel=0;
bool DlgBatchEnabled=false;

_inline void InitDlgQueue(){
    DrawQueue.FastClear();
    CurrentDlgLevel=0;
}
int subIdx=0;
int subLevel=0;
int prevLevel=0;
const char* cn=NULL;

_inline void AddDlqToQueue(SimpleDialog* SD){
    if(prevLevel!=CurrentDlgLevel){
        subLevel=0;        
    }
    prevLevel=CurrentDlgLevel;
    if(SD->GetClassName()!=cn){
        subLevel++;
    }
    cn=SD->GetClassName();
    DrawElm de;
    de.SD=SD;
    de.x=SD->x;
    de.y=SD->y;
    de.Level=CurrentDlgLevel+/*subLevel+*/subIdx;//SD->ExtraSubLevel;
    de.diffuse=GPS.GetCurrentDiffuse();
    if(GPS.GetScreenSpaceTM()){
        de.TM=*GPS.GetScreenSpaceTM();
        de.HaveTM=true;
    }else de.HaveTM=false;
    DrawQueue.Add(de);
}
_inline void IncDlgLevel(int Value){
    CurrentDlgLevel+=Value;
}
int compareDB(const void *elm1, const void *elm2){
    DrawElm* e1=(DrawElm*)elm1;
    DrawElm* e2=(DrawElm*)elm2;
    return e1->Level>=e2->Level?1:-1;
}
_inline void DrawDlgBatches(){
    int n=DrawQueue.GetAmount();
    if(n){
        bool HaveTM=GPS.GetScreenSpaceTM()!=NULL;
        Matrix4D M;
        if(HaveTM)M=*GPS.GetScreenSpaceTM();
        qsort(DrawQueue.GetValues(),n,sizeof(DrawElm),compareDB);
        int L0=DrawQueue[0].Level;
        for(int i=0;i<n;i++){            
            DrawElm& de=DrawQueue[i];
            static int LVL=0;
            if( (de.Level>>16)==LVL )continue;
            if((L0>>16)!=(de.Level>>16)){
                ISM->Flush();
                rsFlush();
            }
            if(de.SD->Visible){
                L0=de.Level;            
                int dx=de.SD->x-de.x;
                int dy=de.SD->y-de.y;
                de.SD->x-=dx;
                de.SD->y-=dy;
                de.SD->x1-=dx;
                de.SD->y1-=dy;
                GPS.SetCurrentDiffuse(de.diffuse);
                if(de.HaveTM)GPS.SetScreenSpaceTM(&de.TM);
                else GPS.SetScreenSpaceTM(NULL);
                de.SD->_Draw();
                for(int i=0;i<de.SD->v_Actions.GetAmount();i++)if(de.SD->v_Actions[i])de.SD->v_Actions[i]->OnDraw(de.SD);				
                de.SD->x+=dx;
                de.SD->y+=dy;
                de.SD->x1+=dx;
                de.SD->y1+=dy;
            }
        }
        ISM->Flush();
        rsFlush();
        InitDlgQueue();
        if(HaveTM)GPS.SetScreenSpaceTM(&M);
        else GPS.SetScreenSpaceTM(NULL);
    }
}

extern bool realLpressed;
extern bool KeyPressed;
extern int LastKey;
extern bool MUSTDRAW;
extern int RealLx;
extern int RealLy;
extern char* SoundID[MaxSnd];
extern byte LastAsciiKey;
void CheckVirtTBL(void* ptr);
void ShowCharUNICODE(int x,int y,byte* strptr,lpRLCFont lpr);
void ShowChar(int x,int y,char c,lpRLCFont lpf);
void ClearKeyStack();
int ReadKey();
void AddKey(byte Key,byte Ascii);
extern int CurPalette;
void FLUSH(){
#ifdef _USE3D
	GPS.FlushBatches();
#endif
}
void RestoreTextShader(){
	static int s=IRS->GetShaderID("hud");
	if(s!=GPS.GetCurrentShader()){
		ISM->Flush();
		GPS.SetShader(s);
	}
}
void GPS_ShowGP( int x, int y, int gpID, int sprID, byte Nation ){
	int sh=GPS.GetCurrentShader();
    int sh1=sh;
    bool chtm=false;
	if(ISM->GetPackageMagic(gpID)=='D2PG'){
		static int s=IRS->GetShaderID("sprite_natcolor_interface");
		//static int s1=IRS->GetShaderID("sprite_natcolor_interface_1v1");
		//if(GPS.GetScreenSpaceTM()==NULL)sh1=s1;
		//else sh1=s;
        sh1=s;
        if(!GPS.GetScreenSpaceTM()){//overmegohiperpupertruperperenedohack due to fulish nvidia drivers - it is unable to render DXT3 1:1 in screen space
            chtm=true;
            static Matrix4D m;
            static bool _ini=true;
            if(_ini){
                m.rotation(Vector3D(0,0,1),0.0001);
                _ini=false;
            }
            GPS.SetScreenSpaceTM(&m);
        }
    }else{
        static int s=IRS->GetShaderID("hud");
        sh1=s;
    }
    if(sh1!=sh){
        ISM->Flush();
        GPS.SetShader(sh1);
    }
	GPS.ShowGP(x,y,gpID,sprID,Nation);
    if(chtm){
        GPS.SetScreenSpaceTM(NULL);        
    }
	//if(sh!=GPS.GetCurrentShader()){		
    //    ISM->Flush();
	//	GPS.SetShader(sh);
	//}
}
void ErrD(LPCSTR s)
{
	char pal[128];
	sprintf(pal,"%d\\agew_1.pal",CurPalette);
	LoadPalette(pal);
	MessageBox(hwnd,s,"Loading failed...",MB_ICONWARNING|MB_OK);
	assert(false);
};
void ShowString(int x,int y,LPCSTR lps,lpRLCFont lpf);
int DrawMultilineText(int x,int y,char* s,RLCFont* DefaultFont,int& MaxLx,int MaxLy,byte CenterX,bool Draw);
void ShowStringEx(int x,int y,LPCSTR lps,lpRLCFont lpf){
	int Lx=100000;
	DrawMultilineText(x,y,(char*)lps,lpf,Lx,100000,0,1);
}
extern int curptr;
int GetSound(char* Name){
	if(!NSounds)return -1;
	int ID=SearchStr(SoundID,Name,NSounds);
	if(ID==-1){
		/*char cc[128];
		sprintf(cc,"Unknown sound ID: %s",Name);
		MessageBox(NULL,cc,"ERROR!",0);
		ErrM(cc);*/
		return -1;
	};
	return ID;
};
DialogsSystem::DialogsSystem(int x,int y){
	memset(&PrevHintX,0,DWORD(&DefaultHint)-DWORD(&PrevHintX));
	BaseX=x;
	BaseY=y;
	HintY=RealLy-80;
	OkSound=GetSound("STANDART");
	CancelSound=GetSound("STANDART"); 
	UserClickSound=GetSound("STANDART");
	//IntersectWindows(0,0,RealLx,RealLy);
	CenteredHint=1;
	SetHintStyle(1,1,0,0,3,3,20,20,400,400,&SmallWhiteFont,0xFFFFFFFF,0);
	x1=RealLx-1;
	y1=RealLy-1;
	IsDrawActive=true;// new 25.02
};
void DialogsSystem::Init(){
	memset(&PrevHintX,0,DWORD(&DefaultHint)-DWORD(&PrevHintX));
	BaseX=0;
	BaseY=0;	
	OkSound=GetSound("STANDART");
	CancelSound=GetSound("STANDART");
	UserClickSound=GetSound("STANDART");
	//IntersectWindows(0,0,RealLx,RealLy);
	CenteredHint=1;
	SetHintStyle(1,1,0,0,3,3,20,20,400,400,&SmallWhiteFont,0xFFFFFFFF,0);
	x1=RealLx-1;
	y1=RealLy-1;
	HintX=0;
	HintY=0;
	HintMovesWithMouse=true;
	IsDrawActive=true;// new 25.02
	DisableHotKeyInMapEditor=false;
}
DialogsSystem::DialogsSystem(){
    Init();	
};
void DialogsSystem::SetFonts(RLCFont* fActive,
							 RLCFont* fPassive,
							 RLCFont* fDisabled,
							 RLCFont* fMessage){
	Active=fActive;
	Passive=fPassive;
	Disabled=fDisabled;
	Message=fMessage;
};
//------------BaseMeshDialog------------
bool BaseMesh_OnDraw(SimpleDialog* SD){
	BaseMeshDialog* bm=(BaseMeshDialog*)SD;
	if(bm->BM){
		if(bm->x!=bm->PrevX||bm->y!=bm->PrevY){
			int dx=bm->x-bm->PrevX;
			int dy=bm->y-bm->PrevY;
			bm->PrevX=bm->x;
			bm->PrevY=bm->y;
			int N=bm->BM->getNVert();
			for(int i=0;i<N;i++){
				VertexTnL* V=(VertexTnL*)bm->BM->getVertexData(i);
				V->x+=dx;
				V->y+=dy;
			}
		}
		DrawBM(*bm->BM);
	}
	return true;
}
bool BaseMesh_Destroy(SimpleDialog* SD){
	BaseMeshDialog* bm=(BaseMeshDialog*)SD;
	if(bm->BM)delete(bm->BM);
	return true;
}
bool ReadStdElement(BaseClass* Element){
	if(Element->CheckDirectCasting()){
		char Name[128];
		sprintf(Name,"Dialogs\\StdElements\\%s.%s.xml",Element->GetClassName(),Element->GetClassName());
		xmlQuote xml;
		if(xml.ReadFromFile(Name)){
			Element->reset_class(Element);
			ErrorPager err(0);		
			Element->Load(xml,Element,&err,NULL);		
			return true;
		}
	}
	return false;
}
void ParentFrame::Init(){
	ShiftX=0;
	ShiftY=0;
	x=0;y=0;x1=0;y1=0;
	BaseX=0;BaseY=0;
	//ActiveX=0;ActiveY=0;
	//ActiveX1=0;ActiveY1=0;
	ActiveID=0;
	ParentDS=NULL;
	//ActiveParent=NULL;
	EditMode=0;
	Selected=0;
	DeepColor=0;
	Diffuse=0xFFFFFFFF;
	//
	LeftAlign=0;
	RightAlign=0;
	HorizontalCenterAlign=0;
	TopAlign=0;
	BottomAlign=0;
	VerticalCenterAlign=0;
	//
	LA_param=0;
	RA_param=0;
	HCA_param=0;
	TA_param=0;
	BA_param=0;
	VCA_param=0;
	EnableTransform=0;
	Angle=0;
	ScaleX=1.0f;
	ScaleY=1.0f;
	PivotDx=0.0f;
	PivotDy=0.0f;
	Visible=true;
}
ParentFrame::ParentFrame(){	
	Init();
}
//void ParentFrame::reset_class(void* DataPtr){
//	BaseClass::reset_class(DataPtr);
//	ParentFrame* PF=(ParentFrame*)DataPtr;
//	PF->ScaleX=1.0f;
//	PF->ScaleY=1.0f;
//}
void ParentFrame::GetMatrix(Matrix4D& M){
	M=Matrix4D::identity;
	float sgx=FlipX?-ScaleX:ScaleX;
	float sgy=FlipY?-ScaleY:ScaleY;
	float ang=Angle*3.1415926/180.0f;
	float _cos=cos(ang);
	float _sin=sin(ang);
	float x0=x;
	float y0=y;
	if(PivotPosition==1){
		x0=(x+x1)/2;
		y0=(y+y1)/2;		
	}else
	if(PivotPosition==2){
		x0=x1;
		y0=y1;
	}
	x0+=PivotDx;
	y0+=PivotDy;
	M.e30=ShiftX+x0-sgx*x0*_cos+sgy*y0*_sin;
	M.e31=ShiftY+y0-sgy*y0*_cos-sgx*x0*_sin;
	M.e00=sgx*_cos;
	M.e11=sgy*_cos;
	M.e01=sgx*_sin;
	M.e10=-sgy*_sin;	
};
BaseMeshDialog* ParentFrame::addBaseMeshDialog(int x,int y,int Lx,int Ly){
	BaseMeshDialog* bm=new BaseMeshDialog;
	AddDialog(bm);
	bm->NeedToDraw=true;
	bm->x=BaseX+x;
	bm->y=BaseY+y;
	bm->x1=bm->x+Lx-1;
	bm->y1=bm->y+Ly-1;
	bm->OnDraw=&BaseMesh_OnDraw;
	bm->Destroy=&BaseMesh_Destroy;
	bm->Enabled=true;
	bm->Active=false;
	bm->PrevX=bm->x;
	bm->PrevY=bm->y;
	bm->BM=NULL;
	bm->ParentDS=this;
	return bm;
}
void BaseMeshDialog::CreateSquareTnL2(float uL,float vL,float uR,float vR,float uL2,float vL2,float uR2,float vR2,char* Shader,DWORD Diffuse){
	if(BM)delete(BM);
    BM=new BaseMesh;
	BM->create(4,6,vfVertexTnL2);
	word* idx=BM->getIndices();
	idx[0]=0;
	idx[1]=1;
	idx[2]=2;
	idx[3]=2;
	idx[4]=1;
	idx[5]=3;
	VertexTnL2* V=(VertexTnL2*)BM->getVertexData();

	V[0].x=x;
	V[0].y=y;
	V[0].w=1.0;
	V[0].u=uL;
	V[0].v=vL;
	V[0].u2=uL2;
	V[0].v2=vL2;
	V[0].diffuse=Diffuse;        

	V[1].x=x1;
	V[1].y=y;
	V[1].w=1.0;
	V[1].u=uR;
	V[1].v=vL;
	V[1].u2=uR2;
	V[1].v2=vL2;
	V[1].diffuse=Diffuse;        

	V[2].x=x;
	V[2].y=y1;
	V[2].w=1.0;
	V[2].u=uL;
	V[2].v=vR;
	V[2].u2=uL2;
	V[2].v2=vR2;
	V[2].diffuse=Diffuse;        

	V[3].x=x1;
	V[3].y=y1;
	V[3].w=1.0;
	V[3].u=uR;
	V[3].v=vR;
	V[3].u2=uR2;
	V[3].v2=vR2;
	V[3].diffuse=Diffuse;        

	BM->setNInd(6);
	BM->setNPri(2);
	BM->setNVert(4);
	BM->setShader(IRS->GetShaderID(Shader));
}
void BaseMeshDialog::CreateSquareTnL(float uL,float vL,float uR,float vR,char* Shader,DWORD Diffuse){
	if(BM)delete(BM);
    BM=new BaseMesh;
	BM->create(4,6,vfVertexTnL);
	word* idx=BM->getIndices();
	idx[0]=0;
	idx[1]=1;
	idx[2]=2;
	idx[3]=2;
	idx[4]=1;
	idx[5]=3;
	VertexTnL* V=(VertexTnL*)BM->getVertexData();

	V[0].x=x;
	V[0].y=y;
	V[0].w=1.0;
	V[0].u=uL;
	V[0].v=vL;
	V[0].diffuse=Diffuse;        

	V[1].x=x1;
	V[1].y=y;
	V[1].w=1.0;
	V[1].u=uR;
	V[1].v=vL;
	V[1].diffuse=Diffuse;        

	V[2].x=x;
	V[2].y=y1;
	V[2].w=1.0;
	V[2].u=uL;
	V[2].v=vR;
	V[2].diffuse=Diffuse;        

	V[3].x=x1;
	V[3].y=y1;
	V[3].w=1.0;
	V[3].u=uR;
	V[3].v=vR;
	V[3].diffuse=Diffuse;        

	BM->setNInd(6);
	BM->setNPri(2);
	BM->setNVert(4);
	BM->setShader(IRS->GetShaderID(Shader));
}
//-----------------class Picture
bool Picture_OnMouseOver(SimpleDialog* SD){
	Picture* Pic=(Picture*)SD;
	if(Pic->ActivePicture!=Pic->PassivePicture)
		SD->NeedToDraw=true;
	return false;
};
bool Picture_OnDraw(SimpleDialog* SD){
	if(!SD)return false;
	if(!SD->Visible)return false;
	Picture* pic=(Picture*)SD;
	if(!pic->Enabled&&pic->DisabledPicture){
		pic->DisabledPicture->Draw(pic->x,pic->y);
		return true;
	};
	if(pic->MouseOver&&pic->ActivePicture){
		pic->ActivePicture->Draw(pic->x,pic->y);
		return true;
	};
	if(pic->PassivePicture){
		pic->PassivePicture->Draw(pic->x,pic->y);
		return true;
	};
	return false;
};
Picture* ParentFrame::addPicture(SimpleDialog* Parent,
						int px,int py,
						SQPicture* Active,
						SQPicture* Passive,
						SQPicture* Disabled){
	Picture* pic=new Picture;
	AddDialog(pic);
	if(Active!=Passive)pic->MouseOverActive=true;
	pic->NeedToDraw=true;
	pic->Parent=Parent;
	pic->x=BaseX+px;
	pic->y=BaseY+py;
	pic->x1=pic->x+Passive->GetLx()-1;
	pic->y1=pic->y+Passive->GetLy()-1;
	pic->ActivePicture=Active;
	pic->PassivePicture=Passive;
	pic->DisabledPicture=Disabled;
	pic->OnDraw=&Picture_OnDraw;
	pic->OnMouseOver=&Picture_OnMouseOver;
	pic->Enabled=true;
	pic->Active=false;
	return pic;	
};
//-----------------class Canvas----------//
bool CanvasDestroy(SimpleDialog* SD){
	Canvas* CAN=(Canvas*)SD;
	if(CAN->DrawData){
		free(CAN->DrawData);
	};
	return true;
};
#define CV_INT(x) (*(int*)(CAN->DrawData+pos+x))
#define CV_BYTE(x) CAN->DrawData[pos+x]
CEXPORT void CBar(int x,int y,int Lx,int Ly,byte c);
void xLine(int x,int y,int x1,int y1,byte c);
_inline DWORD CurColor(DWORD C){
	return ModDWORD(C,GPS.GetCurrentDiffuse());
}
bool CanvasDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	TempWindow TW;
	GPS.FlushBatches();
	PushWindow(&TW);
	Canvas* CAN=(Canvas*)SD;
	if(!(CAN->L))return true;
	//IntersectWindows(SD->x,SD->y,SD->x1,SD->y1);
	Rct VP=GPS.GetClipArea();	
	int ymin=VP.y-64;
	int ymax=VP.GetBottom();

	int pos=0;
	int x0=CAN->x;
	int y0=CAN->y;
    int maxy=y0;
	if(CAN->VS){
		if(CAN->BottomY>CAN->y1-CAN->y){
			CAN->VS->Visible=1;
			CAN->VS->Enabled=1;
			CAN->VS->SMaxPos=CAN->BottomY-CAN->y1+CAN->y+1;
			if(CAN->VS->SPos>CAN->VS->SMaxPos)CAN->VS->SPos=CAN->VS->SMaxPos;
			y0-=CAN->VS->SPos;
		}else{
			CAN->VS->Visible=0;
			CAN->VS->Enabled=0;
		};
	};
	byte* data=CAN->DrawData;
	int YY;
	do{
		switch(data[pos]){
		case 0://bar
			YY=y0+CV_INT(5);
			if(YY>maxy)maxy=YY;
			if(YY>ymin)GPS.DrawFillRect(x0+CV_INT(1),YY,CV_INT(9),CV_INT(13),CurColor(CV_INT(17)));
			//if(YY>ymax)goto XXZ;
			pos+=21;
			break;
		case 1://Line
			YY=y0+CV_INT(5);
			if(YY>maxy)maxy=YY;
			if(YY>ymin)GPS.DrawLine(x0+CV_INT(1),YY,x0+CV_INT(9),y0+CV_INT(13),CurColor(CV_INT(17)));
			//if(YY>ymax)goto XXZ;
			pos+=21;
			break;
		case 2://Rect
			YY=y0+CV_INT(5);
			if(YY>maxy)maxy=YY;
			if(YY>ymin)GPS.DrawRect(x0+CV_INT(1),YY,CV_INT(9),CV_INT(13),CurColor(CV_INT(17)));
			//if(YY>ymax)goto XXZ;
			pos+=21;
			break;
		case 3://sprite
			YY=y0+CV_INT(5);
			if(YY>maxy)maxy=YY;
			if(YY>ymin)GPS_ShowGP(x0+CV_INT(1),YY,CV_INT(9),CV_INT(13),0);
			//if(YY>ymax)goto XXZ;
			pos+=17;
			break;
		case 4://text
			YY=y0+CV_INT(5);
			if(YY>maxy)maxy=YY;
			if(YY>ymin)ShowStringEx(x0+CV_INT(1),YY,(char*)(CAN->DrawData+pos+13),*((RLCFont**)(CAN->DrawData+pos+9)));
			//if(YY>ymax)goto XXZ;
			pos+=strlen((char*)(CAN->DrawData+pos+13))+14;
			break;
		case 5://centered text
			{
				YY=y0+CV_INT(5);
				if(YY>maxy)maxy=YY;
				char* str=(char*)(CAN->DrawData+pos+13);
				if(YY>ymin){
					RLCFont* RF=*((RLCFont**)(CAN->DrawData+pos+9));
					int L=GetRLCStrWidth(str,RF)>>1;
					ShowString(x0+CV_INT(1)-L,y0+CV_INT(5),str,RF);
					if(y0+CV_INT(5)>ymax)goto XXZ;
				};
				pos+=strlen(str)+14;
			};
			break;
		case 6:
			YY=y0+CV_INT(5);
			if(YY>maxy)maxy=YY;
			if(YY>ymin)GPS.DrawFillRect(x0+CV_INT(1),YY,CV_INT(9),CV_INT(13),CV_INT(17),CV_INT(21),CV_INT(25),CurColor(CV_INT(29)));
			//if(YY>ymax)goto XXZ;
			pos+=33;
			break;
		case 7:
			{
				int npoints=CV_INT(1);
				int thickness=CV_INT(5);
                Vector3D* V=(Vector3D*)(CAN->DrawData+pos+9);
				DWORD* CLR=(DWORD*)(CAN->DrawData+pos+9+npoints*12);
				for(int i=0;i<npoints;i++){
					V[i].x+=x0;
					V[i].y+=y0;
				}
				
                rsFlushPoly2D();
                IRS->SetShader( IRS->GetShaderID( "hud" ) );
                IRS->SetTexture( IRS->GetTextureID( "fatline.tga" ) );
                DrawCurve(thickness,npoints,V,CLR);
                rsFlushPoly2D( false );

                for(int i=0;i<npoints;i++){
					V[i].x-=x0;
					V[i].y-=y0;
				}
				pos+=1+2*4+npoints*16;
			}
			break;
		case 8://menu text
			{
				YY=y0+CV_INT(5);
				if(YY>maxy)maxy=YY;
				char* str=(char*)(CAN->DrawData+pos+18);
				if(YY>ymin){
					RLCFont* CFN=NULL;
					if(CAN->MouseOver&&CAN->Enabled)CFN=*((RLCFont**)(CAN->DrawData+pos+14));
					else CFN=*((RLCFont**)(CAN->DrawData+pos+10));
					int LX=CAN->x1-x0;
					if(CFN)DrawMultilineText(x0+CV_INT(1),YY,str,CFN,LX,100000,CV_BYTE(9),true);
				};
				pos+=strlen(str)+19;
			};
			break;
		default:
			return false;
		};
	}while(pos<CAN->L);
XXZ:;
	GPS.FlushBatches();
	PopWindow(&TW);
	//SD->y1=maxy;
	return true;
};
void Canvas::Clear(){
	L=0;
};
void Canvas::CheckSize(int sz){
	if(sz>MaxL-100){
		MaxL+=1024;
		DrawData=(byte*)realloc(DrawData,MaxL);
	};
};
#define CN_INT(x) (*(int*)(DrawData+L))=x;L+=4;
#define CN_FLOAT(x) (*(float*)(DrawData+L))=x;L+=4;
#define CN_SHORT(x) (*(int*)(DrawData+L))=x;L+=2;
#define CN_BYTE(x) DrawData[L]=x;L++;
void Canvas::AddBar(int x,int y,int Lx,int Ly,DWORD c){
	CheckSize(L+100);
	CN_BYTE(0);
	CN_INT(x);
	CN_INT(y);
	CN_INT(Lx);
	CN_INT(Ly);
	CN_INT(c);
	///y1=max(y1,Canvas::y+y+Ly);
};
void Canvas::AddLine(int x,int y,int x1,int y1,DWORD c){
	CheckSize(L+100);
	CN_BYTE(1);
	CN_INT(x);
	CN_INT(y);
	CN_INT(x1);
	CN_INT(y1);
	CN_INT(c);
	//y1=max(y1,Canvas::y+y1);
};
void Canvas::AddRect(int x,int y,int Lx,int Ly,DWORD c){
	CheckSize(L+100);
	CN_BYTE(2);
	CN_INT(x);
	CN_INT(y);
	CN_INT(Lx);
	CN_INT(Ly);
	CN_INT(c);
	//y1=max(y1,Canvas::y+y+Ly);
};
void Canvas::AddSprite(int x,int y,int GPIDX,int Sprite){
	CheckSize(L+100);
	CN_BYTE(3);
	CN_INT(x);
	CN_INT(y);
	CN_INT(GPIDX);
	CN_INT(Sprite);
	//y1=max(y1,Canvas::y+y);
};
void Canvas::AddText(int x,int y,char* Text,RLCFont* Font){
	CheckSize(L+100);
	CN_BYTE(4);
	CN_INT(x);
	CN_INT(y);
	int V=int(Font);
	CN_INT(V);
	strcpy((char*)(DrawData+L),Text);
	L+=1+strlen(Text);
	//y1=max(y1,Canvas::y+y);
};
void Canvas::AddCText(int x,int y,char* Text,RLCFont* Font){
	CheckSize(13+1+strlen(Text));
	CN_BYTE(5);
	CN_INT(x);
	CN_INT(y);
	int V=int(Font);
	CN_INT(V);
	strcpy((char*)(DrawData+L),Text);
	L+=1+strlen(Text);
};
void Canvas::AddBar4(int x,int y,int Lx,int Ly,DWORD C1,DWORD C2,DWORD C3,DWORD C4){
	CheckSize(L+100);
	CN_BYTE(6);
	CN_INT(x);
	CN_INT(y);
	CN_INT(Lx);
	CN_INT(Ly);
	CN_INT(C1);
	CN_INT(C2);
	CN_INT(C3);
	CN_INT(C4);
}
void Canvas::AddThickLines(int* xy_array,int npoints,int thickness,DWORD Color){
	int sz=npoints*16+4*2+1;
	CheckSize(sz+L+20);
	CN_BYTE(7);
    CN_INT(npoints);
	CN_INT(thickness);
	for(int i=0;i<npoints;i++){
		Vector3D V(xy_array[i*2],xy_array[i*2+1],0);
		CN_FLOAT(V.x);
		CN_FLOAT(V.y);
		CN_FLOAT(V.z);		
	}
	for(int i=0;i<npoints;i++){
		CN_INT(Color);
	}
}
void Canvas::AddMenuText(int x,int y,char* Text,RLCFont* PFont,RLCFont* AFont,byte align){
	CheckSize(18+1+strlen(Text));
	CN_BYTE(8);
	CN_INT(x);
	CN_INT(y);
	CN_BYTE(align);
	int V=int(PFont);
	CN_INT(V);
	V=int(AFont);
	CN_INT(V);
	strcpy((char*)(DrawData+L),Text);
	L+=1+strlen(Text);
}
Canvas::Canvas(){
	Destroy=&CanvasDestroy;
	OnDraw=&CanvasDraw;
	//int xy[]={100,100, 400,100, 400,400, 100,400, 100,100, 100,300, 300,300, 400, 400};
	//AddThickLines(xy,5,10,0x90FF0000);
};
Canvas* ParentFrame::AddCanvas(int x,int y,int Lx,int Ly){
	Canvas* CAN=new Canvas;
	AddDialog(CAN);
	CAN->x=x;
	CAN->y=y;
	CAN->x1=x+Lx-1;
	CAN->y1=y+Ly-1;
	CAN->Active=0;
	CAN->Parent=NULL;
	CAN->Enabled=1;
	CAN->DrawData=NULL;
	CAN->L=0;
	CAN->MaxL=0;
	CAN->Destroy=&CanvasDestroy;
	CAN->OnDraw=&CanvasDraw;
	CAN->VS=NULL;
	return CAN;
};
//-----------------class GPPicture
bool GPPicture_OnMouseOver(SimpleDialog* SD){
	return false;
};
void DrawRotatedPicture(int x,int y,int File,int Sprite,byte Nat,
						int dx,int dy,float Angle,float sx,float sy);
bool GPPicture_OnDraw(SimpleDialog* SD){
	if(!SD)return false;
	if(!SD->Visible)return false;
	GPPicture* pic=(GPPicture*)SD;	

	int FileID=NationalGP.Get(pic->FileID);

	if(pic->ShapeFileID!=0xFFFF){
		pic->ShapeFileID=FileID;
		pic->ShapeSpriteID=pic->SpriteID&4095;
	}	
	pic->x1=pic->x+GPS.GetGPWidth(FileID,pic->SpriteID&4095)-1;
	pic->y1=pic->y+GPS.GetGPHeight(FileID,pic->SpriteID&4095)-1;
	//if(pic->Rotated){
	//	DrawRotatedPicture(pic->x,pic->y,FileID,pic->SpriteID,pic->Nation,pic->rx,pic->ry,pic->Angle,pic->ScaleX,pic->ScaleY);
	//}else 
	GPS_ShowGP(pic->x,pic->y,FileID,pic->SpriteID,pic->Nation);
	FLUSH();
	return false;
};
GPPicture::GPPicture(){
	OnDraw=&GPPicture_OnDraw;
}
GPPicture* ParentFrame::addGPPicture(SimpleDialog* Parent,
									   int dx,int dy,int FileID,int SpriteID){
	if(FileID==0xFFFF)return NULL;
	GPPicture* pic=new GPPicture;
	AddDialog(pic);
	pic->Parent=Parent;
	pic->x=BaseX+dx;
	pic->y=BaseY+dy;
	pic->x1=pic->x+GPS.GetGPWidth(FileID,SpriteID&4095)-1;
	pic->y1=pic->y+GPS.GetGPHeight(FileID,SpriteID&4095)-1;
	pic->FileID=FileID;
	pic->SpriteID=SpriteID;
	pic->OnDraw=&GPPicture_OnDraw;
	pic->OnMouseOver=NULL;
	pic->Enabled=true;
	pic->Active=false;
	pic->Nation=0;
	pic->Rotated=0;
	return pic;
};
void GPPicture::SetRotScale(int cx,int cy,float a,float sx,float sy){
	Rotated=1;
	rx=cx;
	ry=cy;
	Angle=a;
	ScaleX=sx;
	ScaleY=sy;
}
//-----------------class RLCPicture
bool RLCPicture_OnDraw(SimpleDialog* SD){

	if(!SD)return false;
	if(!SD->Visible)return false;
	RLCPicture* pic=(RLCPicture*)SD;
	if(!pic->Enabled&&pic->DisabledPicture){
		ShowRLCItem(pic->x,pic->y,pic->DisabledPicture,pic->dpic,0);
		FLUSH();
		return true;
	};
	if(pic->MouseOver&&pic->ActivePicture){
		ShowRLCItem(pic->x,pic->y,pic->ActivePicture,pic->apic,0);
		FLUSH();
		return true;
	};
	if(pic->PassivePicture){
		ShowRLCItem(pic->x,pic->y,pic->PassivePicture,pic->dpic,0);
		FLUSH();
		return true;
	};
	return false;
};
RLCPicture* ParentFrame::addRLCPicture(SimpleDialog* Parent,int x,int y,
						RLCTable* Active,byte apic,
						RLCTable* Passive,byte ppic,
						RLCTable* Disabled,byte dpic){
	RLCPicture* pic=new RLCPicture;
	AddDialog(pic);
	if(Active!=Passive)pic->MouseOverActive=true;
	pic->NeedToDraw=true;
	pic->Parent=Parent;
	pic->x=BaseX+x;
	pic->y=BaseY+y;
	pic->x1=pic->x+GetRLCWidth(*Passive,ppic);
	pic->y1=pic->y+GetRLCHeight(*Passive,ppic);
	pic->ActivePicture=Active;
	pic->apic=apic;
	pic->PassivePicture=Passive;
	pic->ppic=ppic;
	pic->DisabledPicture=Disabled;
	pic->dpic=dpic;
	pic->OnDraw=&Picture_OnDraw;
	pic->OnMouseOver=&Picture_OnMouseOver;
	pic->Enabled=true;
	pic->Active=false;
	return pic;
};
//------------end of the class Picture


//------------class TextButton

int GetRLen(char* s,RLCFont* font){
	if(!int(s))return 0;
	int LX=100000;
	DrawMultilineText(0,0,s,font,LX,100000,0,false);
	return LX;
};
int GetRHeight(char* s,RLCFont* font){
	if(!int(s))return 0;
	int LX=100000;	
	return DrawMultilineText(0,0,s,font,LX,100000,0,false);
};
int GetRHeight(char* s,RLCFont* font,int maxW){
	if(!int(s))return 0;
	int LX=maxW;	
	return DrawMultilineText(0,0,s,font,LX,100000,0,false);
};


void DrawString(int x,int y,char* str,RLCFont* fnt,byte Align){
	int xx;
	if(!str)return;
	switch(Align){
	case 0:	ShowStringEx(x,y,str,fnt);
			return;
	case 1: //center align
			xx=x-(GetRLen(str,fnt)>>1);
			ShowStringEx(xx,y,str,fnt);
			return;
	case 2: //right align
			xx=x-GetRLen(str,fnt);
			ShowStringEx(xx,y,str,fnt);
	};
};
bool TextButton_OnDraw_Test(SimpleDialog* SD,bool Draw){
	if(!SD)return false;
	if(!SD->Visible)return false;
	if(SD->y1<-100)return false;
	if(SD->y>RealLy+100)return false;
	TextButton* tb=(TextButton*)SD;	

	DWORD col=GPS.GetCurrentDiffuse();

	RLCFont* CFN=NULL;
	if(!tb->Enabled&&tb->DisabledFont){
		CFN=tb->DisabledFont;
		if(tb->ColorFonts) GPS.SetCurrentDiffuse(ModDWORD(tb->cDisabled,col));
	}else 
	if(tb->MouseOver&&tb->ActiveFont){
		CFN=tb->ActiveFont;
		if(tb->ColorFonts) GPS.SetCurrentDiffuse(ModDWORD(tb->cActive,col));
	}else
	if(tb->PassiveFont){
		CFN=tb->PassiveFont;
		if(tb->ColorFonts) GPS.SetCurrentDiffuse(ModDWORD(tb->cPassive,col));
	}
	if(tb->Vertical){
		void ShowAStringEx(int x,int y,RLCFont* F,byte Align,bool Horizontal,char* s,...);
		ShowAStringEx(tb->x,(tb->y+tb->y1)/2,CFN,1,0,"%s",tb->Message);
	}else{
		if(CFN){
			int xc=(tb->x+tb->x1)/2;
			int yc=tb->y;						
			int tLX=tb->MaxWidth?tb->MaxWidth:100000;
			if(tLX<60)tLX=60;
			int ly=DrawMultilineText(0,0,tb->Message,CFN,tLX,100000,0,false);
			int rlen=tLX;
			int x=tb->x;
			int y=tb->y;
			switch(tb->Align){
			case 0:
				tb->x1=tb->x+rlen;
				break;
			case 1:
				tb->x =xc-rlen/2;
				tb->x1=xc+rlen/2;
				x=xc-(rlen>>1);
                if(tb->MaxWidth&&tb->MaxWidth<2000)x=xc-(tb->MaxWidth>>1);
				break;
			case 2:
				tb->x=tb->x1-rlen;
				x=tb->x1-rlen;
			};
			tb->y1=tb->y+ly;
			///ShowString(x,y,tb->Message,CFN);
			int LX=tb->MaxWidth?tb->MaxWidth:100000;
			if(LX<60)LX=60;
			if(Draw)DrawMultilineText(x,y,tb->Message,CFN,LX,100000,tb->Align==1,true);
			//tb->x1=tb->x+LX;
			if(Draw)GPS.SetCurrentDiffuse(col);
			return true;
		};
	}
	return false;
};
bool TextButton_OnDraw(SimpleDialog* SD){
	return TextButton_OnDraw_Test(SD,true);
}
bool TextButton_OnMouseOver(SimpleDialog* SD){
	TextButton* tb=(TextButton*)SD;
	if(tb->PassiveFont!=tb->ActiveFont)
		SD->NeedToDraw=true;
	return false;
};
bool TextButton_Destroy(SimpleDialog* SD);
TextButton::TextButton(){
	OnDraw=&TextButton_OnDraw;
	OnMouseOver=&TextButton_OnMouseOver;
	Destroy=&TextButton_Destroy;
	MaxWidth=10000;
}
bool TextButton_Destroy(SimpleDialog* SD){
	TextButton* tb=(TextButton*)SD;
	if(tb->Message)free(tb->Message);
	return true;
};

TextButton* ParentFrame::addVTextButton(SimpleDialog* Parent,
									   int px,int py,char* str,
									   RLCFont* Active,
									   RLCFont* Passive,
									   RLCFont* Disabled,
									   byte Align){

	TextButton* tb=new TextButton;
	AddDialog(tb);
	tb->MouseOverActive=true;
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+px;
	tb->y=BaseY+py;
	tb->xc=BaseX+px;
	tb->yc=BaseY+py;
	int rlen=GetRLen(str,Passive);
	switch(Align){
	case 1:tb->y-=rlen>>1;
		break;
	case 2:tb->y-=rlen;
	};
	tb->y1=tb->y+rlen-1;
	tb->x1=tb->x+GetRLCHeight(Passive->RLC,'W');
	tb->ActiveFont=Active;
	tb->PassiveFont=Passive;
	tb->DisabledFont=Disabled;
	tb->Align=Align;
	tb->Message=znew(char,strlen(str)+1);
	strcpy(tb->Message,str);
	tb->OnDraw=&TextButton_OnDraw;
	tb->OnMouseOver=&TextButton_OnMouseOver;
	tb->Destroy=&TextButton_Destroy;
	tb->Enabled=true;
	tb->Active=false;
	tb->Vertical=1;
	return tb;	
}
TextButton* ParentFrame::addTextButton(SimpleDialog* Parent,
						int px,int py,char* str,
						RLCFont* Active,
						RLCFont* Passive,
						RLCFont* Disabled,
						byte Align){
	TextButton* tb=new TextButton;
	AddDialog(tb);
	tb->MouseOverActive=true;
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+px;
	tb->y=BaseY+py;
	tb->xc=BaseX+px;
	tb->yc=BaseY+py;
	int rlen=GetRLen(str,Passive);
	switch(Align){
	case 1:tb->x-=rlen>>1;
		break;
	case 2:tb->x-=rlen;
	};
	//tb->x1=tb->x+rlen-1;
	tb->x1=tb->x+rlen; // new 17.03
	tb->y1=tb->y+GetRLCHeight(Passive->RLC,'W');
	tb->ActiveFont=Active;
	tb->PassiveFont=Passive;
	tb->DisabledFont=Disabled;
	tb->Align=Align;
	tb->Message=znew(char,strlen(str)+1);
	strcpy(tb->Message,str);
	tb->OnDraw=&TextButton_OnDraw;
	tb->OnMouseOver=&TextButton_OnMouseOver;
	tb->Destroy=&TextButton_Destroy;
	tb->Enabled=true;
	tb->Active=false;
	tb->Vertical=0;
	//TextButton_OnDraw_Test(tb,false);
	return tb;
};
//------------end of the class TextButton

int VitButton::GetHeightGP(){
	Lx=GetWidth();
	//if(Lx<24)Lx=24;
	int L=Lx;
	if(DisableCycling){
		x1=x/*-1*/+(SpritePassive[State]<0?Lx:GPS.GetGPWidth(GP_File,SpritePassive[State]));
		return SpritePassive[State]<0?DrawMultilineText(0,0,Message,FontPassive,L,100000,1,false):GPS.GetGPHeight(GP_File,SpritePassive[State]);
	}else{
		int Ly=0;
		if(SpritePassive[0]>=0) Ly=GPS.GetGPHeight(GP_File,SpritePassive[0]);
		if(SpritePassive[1]>=0){
			int ly=GPS.GetGPHeight(GP_File,SpritePassive[1]);
			if(ly>Ly) Ly=ly;
		}
		if(Ly==0){
			Ly=DrawMultilineText(0,0,Message,FontPassive,L,100000,1,false);
		}
		//return SpritePassive[1]<0?DrawMultilineText(0,0,Message,FontPassive,Lx,100000,1,false):GPS.GetGPHeight(GP_File,SpritePassive[1]);
		return Ly;
	}	
}
int VitButton::GetTextWidth(){	
	RLCFont* CFN=NULL;	
	if(FontOver&&(MouseOver||State==1)&&Enabled) CFN=FontOver; 
	else if(FontPassive) CFN=FontPassive;
	if(CFN&&Message&&Message[0]){
		int L=1000/*Lx*/;
		DrawMultilineText(0,0,Message,CFN,L,1000000,Align,false);
		return L;
	}else{
		return 0;
	}
};
void DrawHeaderEx2(int x0,int y0,int Lx,int GP_File,int FrameL,int FrameR,int FrameC1,int FrameC2,int FrameC3, byte Nation);
void VitButton::_Draw(){
	if(!Visible) return;
	//Lx=GetWidth();	
	y1=y+GetHeightGP()-1;
	if(x>=x1)
		return;
	//TempWindow TW; ---
	//PushWindow(&TW); ---

	//drawing button
	int CSpr=MouseOver&&Enabled?SpriteOver[State]:SpritePassive[State];	
	/*
	DWORD c=0xFFFFFFFF;
	if(!Enabled){
		c=GPS.GetCurrentDiffuse();
		DWORD cc=ModDWORD(c,0x80FFFFFF);
		GPS.SetCurrentDiffuse(cc);
	}
	*/
	if(CSpr>=0){		
		//IntersectWindows(x,y,x1,y1); ---
		
		int FileID=NationalGP.Get(GP_File);

		if(DisableCycling){
			ShapeFileID=FileID;
			ShapeSpriteID=CSpr;	
			GPS_ShowGP(x,y,FileID,CSpr,Nation);
		}else{
			ShapeFileID=0xFFFF;
			ShapeSpriteID=0xFFFF;	
			if(OneSprited) DrawHeaderEx2(x,y,Lx,FileID,-1,-1,CSpr,CSpr,CSpr,Nation);
			else DrawHeaderEx2(x,y,Lx,FileID,CSpr,CSpr+1,CSpr+2,CSpr+3,CSpr+4,Nation);
		}
		/*
		if(Enabled&&MouseOver){
		for(int q=0;q<tb->Nx;q++)GPS_ShowGP(tb->x+q*tb->OneLx,tb->y,tb->GP_File,tb->Sprite,0);
		GPS.FlushBatches();
		}else{
		if(tb->Sprite1!=-1)for(int q=0;q<tb->Nx;q++)GPS_ShowGP(tb->x+q*tb->OneLx,tb->y,tb->GP_File,tb->Sprite1,0);
		GPS.FlushBatches();
		};
		*/
		FLUSH();
	}else{
		ShapeFileID=0xFFFF;
		//ShapeSpriteID;	
	}
	//drawing text
	if(Message&&Message[0]){
		RLCFont* CFN=NULL;	
		if(FontOver&&(MouseOver||State==1)&&Enabled) CFN=FontOver; else 
			if(FontPassive) CFN=FontPassive;
		if(CFN){
			ISM->Flush();
			DWORD fc=CFN->GetColor();
			if(!Enabled){
				CFN->SetColor(0x60000000+(0x00FFFFFF&fc));
			}		
			int lx=Lx;
			int dy=(y+y1-DrawMultilineText(0,0,Message,CFN,lx,1000000,Align,false))/2;
			DrawMultilineText(x+FontDx,dy+FontDy,Message,CFN,Lx,1000000,Align,true);
			CFN->SetColor(fc);
		};
	}
	//GPS.SetCurrentDiffuse(c);
	//PopWindow(&TW);	
};
bool GP_TextButton_OnDraw(SimpleDialog* SD){
	if(!SD)return false;
	if(!SD->Visible)return false;
	GP_TextButton* tb=(GP_TextButton*)SD;	

	if(!(tb->ActiveFont&&tb->PassiveFont&&tb->DisabledFont))return false;
	int spr=tb->Sprite>=0?tb->Sprite:tb->Sprite1;
	tb->x1=tb->x+GPS.GetGPWidth(tb->GP_File,spr);
	tb->y1=tb->y+GPS.GetGPHeight(tb->GP_File,spr);
	//if(x1>x+2000)x1=x+2000;
	//if(y1>y+2000)y1=y+2000;
	//ShapeFileID=GP_File;
	tb->ShapeSpriteID=spr;
	//drawing button
	TempWindow TW;
	PushWindow(&TW);
	IntersectWindows(tb->x,tb->y,tb->x1+1,tb->y1+10);
	int xc=tb->x+tb->FontDx;
	int yc=((tb->y+tb->y1)>>1)-(GetRLCHeight(tb->ActiveFont->RLC,'W')>>1)+tb->FontDy;
	int curD=GPS.GetCurrentDiffuse();
	if(!tb->Enabled){
		GPS.SetCurrentDiffuse(MulDWORD(curD,230));
	}	
	if(tb->Enabled&&tb->MouseOver){
		for(int q=0;q<tb->Nx;q++)GPS_ShowGP(tb->x+q*tb->OneLx,tb->y,tb->GP_File,tb->Sprite,0);
		GPS.FlushBatches();
        ISM->Flush();
	}else{
		if(tb->Sprite1!=-1)for(int q=0;q<tb->Nx;q++)GPS_ShowGP(tb->x+q*tb->OneLx,tb->y,tb->GP_File,tb->Sprite1,0);
		GPS.FlushBatches();
        ISM->Flush();
	};
	GPS.SetCurrentDiffuse(curD);
	//drawing text
	RLCFont* CFN=NULL;
	if(!tb->Enabled&&tb->DisabledFont)CFN=tb->DisabledFont;
	else 
	if(tb->MouseOver&&tb->ActiveFont)CFN=tb->ActiveFont;
	else
	if(tb->PassiveFont)CFN=tb->PassiveFont;
	if(CFN){
		int rlen=0;
		if(tb->Center)rlen=GetRLen(tb->Message,CFN)+tb->x-tb->x1;
		//ShowStringEx(xc-(rlen>>1),yc,tb->Message,CFN);
		
		int Lx=tb->GetWidth();
		int dy=(tb->y+tb->y1-DrawMultilineText(0,0,tb->Message,CFN,Lx,1000000,tb->Center,false))/2;
		DrawMultilineText(tb->x+tb->FontDx,dy+tb->FontDy,tb->Message,CFN,Lx,1000000,tb->Center,true);

		PopWindow(&TW);
		FLUSH();
		return true;
	};
	PopWindow(&TW);
	FLUSH();
	return false;
};
bool GP_TextButton_Destroy(SimpleDialog* SD){
	GP_TextButton* tb=(GP_TextButton*)SD;
	if(tb->Message)free(tb->Message);
	return true;
};
GP_TextButton* ParentFrame::addGP_TextButton(SimpleDialog* Parent,int x,int y,char* str,
									   int GP_File,int Sprite,RLCFont* Active,RLCFont* Passive){
	GP_TextButton* tb=new GP_TextButton;
	AddDialog(tb);
	tb->MouseOverActive=true;
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->x1=tb->x+GPS.GetGPWidth(GP_File,Sprite)-1;
	tb->y1=tb->y+GPS.GetGPHeight(GP_File,Sprite)-1;
	tb->GP_File=GP_File;
	tb->Sprite=Sprite;
	tb->Sprite1=-1;
	tb->ActiveFont=Active;
	tb->PassiveFont=Passive;
	tb->DisabledFont=Passive;
	tb->FontDy=-1;
	tb->FontDx=0;
	tb->Message=znew(char,strlen(str)+1);
	strcpy(tb->Message,str);
	tb->OnDraw=&GP_TextButton_OnDraw;
	tb->Destroy=&GP_TextButton_Destroy;
	tb->Enabled=true;
	tb->Active=false;
	tb->Center=1;
	tb->Nx=1;
	tb->OneLx=0;
	return tb;
};
void VitButton::SetMessage(char* ms){		
	if(!ms) return;
	int len=strlen(ms);
	if(!Message||len!=strlen(Message)) Message=(char*)realloc(Message,len+1);
	//
	while(*ms==' '||*ms==10||*ms==13){
		ms++;
		len--;
	}			
	int i=len-1;
	while(i>=0&&(ms[i]==' '||ms[i]==10||ms[i]==13)){
		ms[i]=0;
		i--;				
	}
	len=i+1;
	//
	strcpy(Message,ms);
}
void TextButton::SetMessage(char* ms, int Width){
		if(!ms) return;
		int len=strlen(ms);
		if(!Message||len!=strlen(Message)) Message=(char*)realloc(Message,len+1);		
		if(Align){			
			while(*ms==' '||*ms==10||*ms==13){
				ms++;
				len--;
			}			
			int i=len-1;
			while(i>=0&&(ms[i]==' '||ms[i]==10||ms[i]==13)){
				ms[i]=0;
				i--;				
			}
			len=i+1;
		}
		strcpy(Message,ms);
		if(Width>0) MaxWidth=Width;
		if(MaxWidth<30) MaxWidth=10000;			
		int W=MaxWidth;
		y1=y+DrawMultilineText(0,0,Message,PassiveFont,W,10000,0,false);
		if(!Vertical){
			/*
			int Lx=MaxWidth?MaxWidth:100000;
			if(Lx<60)Lx=60;
			int Ly=DrawMultilineText(0,0,Message,ActiveFont,Lx,100000,0,false);			
			int xc=(x+x1)/2;
			y1=y+Ly-1;
			switch(Align){
			case 0:
				x1=x+Lx-1;				
				break;
			case 1:
				x=xc-Lx/2;
				x1=xc+Lx/2;
				break;
			case 2:
				x=x1-Lx;
				break;
			}
			*/
		}
	}
GP_TextButton::GP_TextButton(){
	OnDraw=&GP_TextButton_OnDraw;
	Destroy=&GP_TextButton_Destroy;
	Nx=1;
	Center=1;
}
VitButton::VitButton(){
	Align=1;
	Message=NULL;
	//OnDraw=&GP_TextButton_OnDraw;
	Lx=100;
};
GP_TextButton* ParentFrame::addGP_TextButtonLimited(SimpleDialog* Parent,int x,int y,char* str,
	  int GP_File,int SpriteActive,int SpritePassive,int Lx,RLCFont* Active,RLCFont* Passive){
	GP_TextButton* tb=new GP_TextButton;
	AddDialog(tb);
	tb->MouseOverActive=true;
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->x1=tb->x+Lx-1;
	tb->y1=tb->y+GPS.GetGPHeight(GP_File,SpriteActive)-1;
	tb->GP_File=GP_File;
	tb->Sprite=SpriteActive;
	tb->Sprite1=SpritePassive;
	tb->ActiveFont=Active;
	tb->PassiveFont=Passive;
	tb->DisabledFont=Passive;
	tb->FontDy=-1;
	tb->FontDx=0;
	tb->Message=znew(char,strlen(str)+1);
	strcpy(tb->Message,str);
	tb->OnDraw=&GP_TextButton_OnDraw;
	tb->Destroy=&GP_TextButton_Destroy;
	tb->Enabled=true;
	tb->Active=false;
	tb->Center=1;
	int LXX=GPS.GetGPWidth(GP_File,SpriteActive);
	if(LXX) tb->Nx=(Lx/LXX)+1;
	tb->OneLx=LXX;
	//tb->FontDx=4;
	return tb;
};
bool GP_StdTextButton_OnDraw(SimpleDialog* SD){
	if(!SD)return false;
	if(!SD->Visible)return false;
	GP_TextButton* tb=(GP_TextButton*)SD;
	//drawing button
	TempWindow TW;
	PushWindow(&TW);
	int spr;
	RLCFont* FNT;
	if(tb->Enabled){
		if(tb->MouseOver){
			spr=tb->Sprite;
			FNT=tb->ActiveFont;
		}else{
			spr=tb->Sprite+3;
			FNT=tb->PassiveFont;
		};
	}else{
		spr=tb->Sprite+6;
		FNT=tb->DisabledFont;
	};
	int LL=GPS.GetGPWidth(tb->GP_File,spr+1);
	int LR=GPS.GetGPWidth(tb->GP_File,spr+2);
	GPS_ShowGP(tb->x,tb->y,tb->GP_File,spr+1,0);
	GPS_ShowGP(tb->x1-LR,tb->y,tb->GP_File,spr+2,0);
	IntersectWindows(tb->x+LL,tb->y,tb->x1-LR,tb->y1+10);
	for(int i=0;i<tb->Nx;i++)GPS_ShowGP(tb->x+LL+i*tb->OneLx,tb->y,tb->GP_File,spr,0);
	PopWindow(&TW);

	int LY=GetRLCHeight(FNT->RLC,'C');
	int LX=GetRLen(tb->Message,FNT);
	ShowStringEx((tb->x+tb->x1-LX)/2,(tb->y+tb->y1-LY)/2-1,tb->Message,FNT);
	FLUSH();
	return false;
};
GP_TextButton* ParentFrame::addStdGP_TextButton(int x,int y,int Lx,char* str,
				int GP_File,int Sprite,RLCFont* Active,RLCFont* Passive){
	GP_TextButton* tb=new GP_TextButton;
	AddDialog(tb);
	tb->MouseOverActive=true;
	tb->NeedToDraw=true;
	tb->Parent=NULL;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->x1=tb->x+Lx-1;
	tb->y1=tb->y+GPS.GetGPHeight(GP_File,Sprite);
	tb->GP_File=GP_File;
	tb->Sprite=Sprite;
	tb->Sprite1=0;
	tb->ActiveFont=Active;
	tb->PassiveFont=Passive;
	tb->DisabledFont=Passive;
	tb->FontDy=0;
	tb->FontDx=0;
	tb->Message=znew(char,strlen(str)+1);
	strcpy(tb->Message,str);
	tb->OnDraw=&GP_StdTextButton_OnDraw;
	tb->Destroy=&GP_TextButton_Destroy;
	tb->Enabled=true;
	tb->Active=false;
	tb->Center=1;
	int LXX=GPS.GetGPWidth(GP_File,Sprite);
	tb->Nx=(Lx/LXX)+1;
	tb->OneLx=LXX;
	//tb->FontDx=4;
	return tb;
};
bool UniversalButton_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	UniversalButton* UB=(UniversalButton*)SD;
	int StartIDX=0;
	RLCFont* FONT;
	if(UB->ActiveFont==UB->PassiveFont){
		if(UB->State)FONT=UB->SelectedFont;
		else FONT=UB->PassiveFont;
	}else{
		if(UB->MouseOver)FONT=UB->ActiveFont;
		else if(UB->State)FONT=UB->SelectedFont;
		else FONT=UB->PassiveFont;
	};
	if(UB->MouseOver){
		StartIDX=3*5;
	};
	StartIDX=StartIDX+UB->State*5;
	TempWindow TW;
	PushWindow(&TW);
	int x0=UB->x;
	int x1=UB->x1;
	if(!UB->Tiling){
		int sp=UB->SpritesSet[StartIDX];
		if(sp!=-1)x0+=GPS.GetGPWidth(UB->GP_File,sp);
		sp=UB->SpritesSet[StartIDX+4];
		if(sp!=-1)x1-=GPS.GetGPWidth(UB->GP_File,sp);
	};
	IntersectWindows(x0,0,x1,RealLy-1);
	int pp0=x0*7+UB->y*17;
	int NN=0;
	while(x0<x1&&NN<1000){
		int sp=UB->SpritesSet[StartIDX+1+randoma[pp0&8191]%3];
		NN++;
		if(sp!=-1){
			GPS_ShowGP(x0,UB->y,UB->GP_File,sp,MyNation);
			x0+=GPS.GetGPWidth(UB->GP_File,sp);
		};
		pp0++;
	};
	PopWindow(&TW);
	IntersectWindows(0,0,(UB->x+UB->x1)/2,RealLy-1);
	int sp=UB->SpritesSet[StartIDX];
	if(sp!=-1)GPS_ShowGP(UB->x,UB->y,UB->GP_File,sp,MyNation);
	PopWindow(&TW);
	IntersectWindows((UB->x+UB->x1)/2+1,0,RealLx-1,RealLy-1);
	sp=UB->SpritesSet[StartIDX+4];
	if(sp!=-1)GPS_ShowGP(UB->x1-GPS.GetGPWidth(UB->GP_File,sp),UB->y,UB->GP_File,sp,MyNation);
	PopWindow(&TW);
	//text
	if(FONT){
		int x;
		int y=(UB->y+UB->y1-GetRLCHeight(FONT->RLC,'W'))/2-1;
		if(UB->Center){
			x=(UB->x+UB->x1-GetRLCStrWidth(UB->Message,FONT))/2;
		}else{
			x=x0;
		};
		x+=UB->FontDx;
		y+=UB->FontDy;
		ShowStringEx(x,y,UB->Message,FONT);
	};
	FLUSH();
	return true;
};
bool UniversalButton_Destroy(SimpleDialog* SD){
	UniversalButton* UB=(UniversalButton*)SD;
	free(UB->Message);
	return true;
};
bool UniversalButton_OnKlick(SimpleDialog* SD){
	UniversalButton* UB=(UniversalButton*)SD;
	ParentFrame* DSS=UB->ParentDS;
	if(!DSS)return false;
	if(UB->Group!=-1){
		if(!UB->State)UB->State=1;
		else if(UB->State==1)UB->State=2;
		else UB->State=1;
		UB->Changed=1;
		int N=DSS->DSS.GetAmount();
		for(int i=0;i<N;i++){
			if(DSS->DSS[i]&&DSS->DSS[i]->OnDraw==&UniversalButton_OnDraw){
				UniversalButton* UB1=(UniversalButton*)DSS->DSS[i];
				if(UB1!=UB&&UB1->Group==UB->Group){
					UB1->State=0;
				};
			};
		};
	}else{
		if(!UB->State)UB->State=1;
		else UB->State=0;
		UB->Changed=1;
	};
	return true;
};
UniversalButton* ParentFrame::addUniversalButton(int x,int y,int Lx,char* str,int GP_File,
						int* SprSet,int Group,int NowSelected,bool tiling,
						RLCFont* Active,RLCFont* Passive,RLCFont* Selected){
	UniversalButton* tb=new UniversalButton;
	AddDialog(tb);
	tb->MouseOverActive=true;
	tb->NeedToDraw=true;
	tb->Parent=NULL;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->x1=tb->x+Lx-1;
	tb->y1=tb->y+GPS.GetGPHeight(GP_File,SprSet[0]);
	tb->GP_File=GP_File;
	memcpy(tb->SpritesSet,SprSet,30*4);
	
	tb->Group=Group;
	tb->State=NowSelected;

	tb->ActiveFont=Active;
	tb->PassiveFont=Passive;
	tb->DisabledFont=Passive;
	tb->SelectedFont=Selected;

	tb->FontDy=0;
	tb->FontDx=0;
	tb->Message=znew(char,strlen(str)+1);
	strcpy(tb->Message,str);
	tb->OnDraw  =&UniversalButton_OnDraw;
	tb->Destroy =&UniversalButton_Destroy;
	tb->OnClick =&UniversalButton_OnKlick;
	tb->Enabled=true;
	tb->Active=false;
	tb->Center=1;
	tb->ParentDS=this;
	//tb->FontDx=4;
	return tb;
};
UniversalButton* ParentFrame::addTextureStrip(int x,int y,int Lx,int GP_File,
												int L,int C1,int C2,int C3,int R,bool Tiling){
	int PATTERN[30]={L,C1,C2,C3,R,L,C1,C2,C3,R,L,C1,C2,C3,R,L,C1,C2,C3,R,L,C1,C2,C3,R,L,C1,C2,C3,R};
	return addUniversalButton(x,y,Lx,"",GP_File,PATTERN,-1,0,Tiling,NULL,NULL,NULL);
};
void LoadScreen(char* f, DialogsSystem* S);
void DialogsSystem::LoadScreenFromFile(char* XMLFile)
{
	LoadScreen(XMLFile, this);
}
//-------class : button with text-----------
bool BpxTextButton_OnDraw(SimpleDialog* SD){
	if(!SD)return false;
	if(!SD->Visible)return false;
	BpxTextButton* tb=(BpxTextButton*)SD;
	//drawing button
	SQPicture* SP;
	tb->xc=(tb->x+tb->x1)>>1;
	tb->yc=(tb->y+tb->y1)>>1;
	if(tb->Enabled){
		if(tb->MouseOver&&realLpressed)SP=tb->ActivePicture;
		else SP=tb->PassivePicture;
	}else SP=tb->DisabledPicture;
	SP->Draw(tb->x,tb->y);
	//drawing text
	RLCFont* CFN=NULL;
	if(!tb->Enabled&&tb->DisabledFont)CFN=tb->DisabledFont;
	else 
	if(tb->MouseOver&&tb->ActiveFont)CFN=tb->ActiveFont;
	else
	if(tb->PassiveFont)CFN=tb->PassiveFont;
	if(CFN){
		int rlen=GetRLen(tb->Message,CFN);
		ShowStringEx(tb->xc-(rlen>>1),tb->yc-(GetRLCHeight(CFN->RLC,'W')>>1),tb->Message,CFN);
		return true;
	};
	return false;
};
bool BpxTextButton_OnMouseOver(SimpleDialog* SD){
	if(SD->MouseOver)SD->NeedToDraw=true;
	return false;
};
BpxTextButton* ParentFrame::addBpxTextButton(SimpleDialog* Parent,
						int px,int py,char* str,
						RLCFont* Active,
						RLCFont* Passive,
						RLCFont* Disabled,
						SQPicture* pActive,
						SQPicture* pPassive,
						SQPicture* pDisabled){
	BpxTextButton* tb=new BpxTextButton;
	AddDialog(tb);
	tb->MouseOverActive=true;
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+px;
	tb->y=BaseY+py;
	tb->x1=tb->x+pPassive->GetLx()-1;
	tb->y1=tb->y+pPassive->GetLy()-1;
	tb->ActivePicture=pActive;
	tb->PassivePicture=pPassive;
	tb->DisabledPicture=pDisabled;
	tb->ActiveFont=Active;
	tb->PassiveFont=Passive;
	tb->DisabledFont=Disabled;
	tb->Message=znew(char,strlen(str)+1);
	strcpy(tb->Message,str);
	tb->OnDraw=&BpxTextButton_OnDraw;
	tb->Destroy=&TextButton_Destroy;
	tb->Enabled=true;
	tb->Active=false;
	tb->OnMouseOver=&BpxTextButton_OnMouseOver;
	return tb;
};
//--------end of button with text----------//
bool Video_OnDraw(SimpleDialog* SD){
	VideoButton* tb=(VideoButton*)SD;
	if(!SD->Visible)return 0;
	int dt=GetTickCount()-tb->LastTime;
	if(tb->Stage==2){
		GPS_ShowGP(tb->x,tb->y,tb->GP_off,tb->CurSprite,0);
		if(tb->CurSprite<tb->N_off-1){
			if(dt>40){
				tb->CurSprite++;
				tb->LastTime=GetTickCount();
			};
		}else{
			tb->Stage=0;
			tb->CurSprite=0;
		};
	};
	if(tb->Stage==1){
		GPS_ShowGP(tb->x,tb->y,tb->GP_on,tb->CurSprite,0);
		if(tb->CurSprite<tb->N_on-1){
			if(dt>40){
				tb->CurSprite++;
				tb->LastTime=GetTickCount();
			};
		}else{
			if(!tb->MouseOver){
				tb->Stage=2;
				tb->CurSprite=0;
			};
		};
	};
	if(tb->Stage==0){
		GPS_ShowGP(tb->x,tb->y,tb->GP_on,0,0);
		if(tb->MouseOver){
			tb->Stage=1;
			tb->CurSprite=0;
		};
	};
	return false;
};
VideoButton* ParentFrame::addVideoButton(SimpleDialog* Parent,int x,int y,int GP1,int GP2){
	VideoButton* tb=new VideoButton;
	AddDialog(tb);
	tb->MouseOverActive=true;
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->x1=tb->x+GPS.GetGPWidth(GP1,0)-1;
	tb->y1=tb->y+GPS.GetGPHeight(GP1,0)-1;
	tb->GP_on=GP1;
	tb->GP_off=GP2;
	tb->N_on=GPS.GPNFrames(GP1);
	tb->N_off=GPS.GPNFrames(GP2);
	tb->Stage=0;
	tb->CurSprite=0;
	tb->OnDraw=&Video_OnDraw;
	tb->Destroy=NULL;
	tb->Enabled=true;
	tb->Active=false;
	tb->LastTime=GetTickCount();
	return tb;
};
//---------------GP_Button-----------------//
bool GP_Button_OnDraw(SimpleDialog* SD){
	GP_Button* tb=(GP_Button*)SD;
	if(!tb->Visible)return false;

	if(tb->MouseOver&&tb->Enabled||tb->Active){
		GPS_ShowGP(tb->x,tb->y,tb->GP_File,tb->ActiveFrame,0);
	}else{
		GPS_ShowGP(tb->x,tb->y,tb->GP_File,tb->PassiveFrame,0);
	};
	FLUSH();
	return false;
};
GP_Button* ParentFrame::addGP_Button(SimpleDialog* Parent,int x,int y,int GP_File,int Active,int Passive){
	GP_Button* tb=new GP_Button;
	AddDialog(tb);
	tb->MouseOverActive=true;
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->x1=tb->x+GPS.GetGPWidth(GP_File,Passive)-1;
	tb->y1=tb->y+GPS.GetGPHeight(GP_File,Passive)-1;
	tb->GP_File=GP_File;
	tb->ActiveFrame=Active;
	tb->PassiveFrame=Passive;
	tb->OnDraw=&GP_Button_OnDraw;
	tb->Destroy=NULL;
	tb->Enabled=true;
	tb->Active=false;
	return tb;
};
GP_Button::GP_Button(){
	GP_File=0;
	ActiveFrame=0;
	PassiveFrame=0;
	x=0;y=0;x1=40;y1=40;
	OnDraw=&GP_Button_OnDraw;
	Enabled=true;
	Active=false;
}
//------class : Vertical scroll bar--------//
bool MouseIns(int x,int y,int x1,int y1){
	if(mouseX>=x&&mouseY>=y&&mouseX<=x1&&mouseY<=y1)return true;
	else return false;
};
bool MouseInsL(int x,int y,int lx,int ly){
	return MouseIns(x,y,x+lx-1,y+ly-1);
};
bool VScrollBar_Draw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	VScrollBar* tb=(VScrollBar*)SD;	

	if(tb->GetMaxPos()>0){
		tb->sbar0->Draw(tb->x,tb->sby+2);
		if(MouseInsL(tb->x,tb->y,tb->sblx,tb->btnly)&&realLpressed)
			tb->btn_up1p->Draw(tb->x,tb->y);
		else tb->btn_up1->Draw(tb->x,tb->y);
		if(MouseInsL(tb->x,tb->sby+tb->sbly,tb->sblx,tb->btnly)&&realLpressed)
			tb->btn_dn1p->Draw(tb->x,tb->sby+tb->sbly);
		else tb->btn_dn1->Draw(tb->x,tb->sby+tb->sbly);
		tb->marker->Draw(tb->x+1,tb->y+tb->btnly+div(int(tb->sbly-tb->mkly+2)*tb->SPos,tb->SMaxPos).quot);
	}else{
		tb->sbar1->Draw(tb->x,tb->sby+2);
		tb->btn_up0->Draw(tb->x,tb->y);
		tb->btn_dn0->Draw(tb->x,tb->sby+tb->sbly);
	};
	return true;
};
bool VScrollBar_MouseOver(SimpleDialog* SD){
	VScrollBar* tb=(VScrollBar*)SD;
	int dt=GetTickCount()-tb->LastTime;
	if(tb->SMaxPos>0){
		if(!realLpressed)tb->Zaxvat=false;
		int my=tb->y+tb->btnly+div(int(tb->sbly-tb->mkly+2)*tb->SPos,tb->SMaxPos).quot;
		if(!tb->Zaxvat){
			if(MouseInsL(tb->x,tb->y,tb->sblx,tb->btnly)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				int txx=tb->SPos;
				if(tb->SPos>0)tb->SPos--;
				if(tb->SPos!=txx)tb->NeedToDraw=true;
			};
			if(MouseInsL(tb->x,tb->sby+tb->sbly,tb->sblx,tb->btnly)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				int txx=tb->SPos;
				if(tb->SPos<tb->SMaxPos)tb->SPos++;
				if(tb->SPos!=txx)tb->NeedToDraw=true;
			};
			if(MouseIns(tb->x,tb->y+tb->btnly,tb->x+tb->sblx-1,my)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				int txx=tb->SPos;
				if(tb->SPos>6)tb->SPos-=6;
				else tb->SPos=0;
				if(tb->SPos!=txx)tb->NeedToDraw=true;
			};
			if(MouseIns(tb->x,my+tb->mkly,tb->x+tb->sblx-1,tb->y+tb->sbly+tb->btnly-1)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				int txx=tb->SPos;
				if(tb->SMaxPos-tb->SPos>6)tb->SPos+=6;
				else tb->SPos=tb->SMaxPos;
				if(tb->SPos!=txx)tb->NeedToDraw=true;
			};
			if(MouseInsL(tb->x,my,tb->sblx,tb->mkly)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				tb->Zaxvat=true;
			};
		}else{
			int txx=tb->SPos;
			tb->SPos=div((mouseY-tb->sby-(tb->mkly>>1))*tb->SMaxPos,tb->sbly-tb->mkly).quot;
			if(tb->SPos<0)tb->SPos=0;
			if(tb->SPos>tb->SMaxPos)tb->SPos=tb->SMaxPos;
			if(tb->SPos!=txx)tb->NeedToDraw=true;
		};
	};
	return true;
};
VScrollBar* ParentFrame::addVScrollBar(SimpleDialog* Parent,
						int x,int y,int MaxPos,int Pos,
						SQPicture* btn_up0,
						SQPicture* btn_up1,
						SQPicture* btn_up1p,
						SQPicture* btn_dn0,
						SQPicture* btn_dn1,
						SQPicture* btn_dn1p,
						SQPicture* sbar0,
						SQPicture* sbar1,
						SQPicture* marker){
				
	VScrollBar* tb=new VScrollBar;
	AddDialog(tb);
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->sblx=sbar0->GetLx();
	tb->sbly=sbar0->GetLy();
	tb->mkly=byte(marker->GetLy());
	tb->btnly=byte(btn_dn1->GetLy());
	tb->y1=tb->y+tb->sbly+(tb->btnly<<1)-1;
	tb->x1=tb->x+tb->sblx-1;
	tb->sby=tb->y+tb->btnly;
	tb->SMaxPos=MaxPos;
	tb->SPos=Pos;
	tb->btn_up0=btn_up0;
	tb->btn_up1=btn_up1;
	tb->btn_up1p=btn_up1p;
	tb->btn_dn0=btn_dn0;
	tb->btn_dn1=btn_dn1;
	tb->btn_dn1p=btn_dn1p;
	tb->sbar0=sbar0;
	tb->sbar1=sbar1;
	tb->marker=marker;
	tb->Zaxvat=false;
	tb->OnDraw=&VScrollBar_Draw;
	tb->OnMouseOver=&VScrollBar_MouseOver;
	tb->Enabled=true;
	tb->Active=false;
	tb->GP_File=-1;
	tb->StartGP_Spr=-1;
	tb->OnesDy=1;
	return tb;
};
//-------end of vertical Scroll bar--------//
//------class : Horizontal scroll bar--------//
bool HScrollBar_Draw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	VScrollBar* tb=(VScrollBar*)SD;
	if(tb->SMaxPos>0){
		tb->sbar0->Draw(tb->sby+2,tb->y);
		//left click
		if(MouseInsL(tb->x,tb->y,tb->btnly,tb->sblx)&&realLpressed)
			tb->btn_up1p->Draw(tb->x,tb->y);
		else tb->btn_up1->Draw(tb->x,tb->y);
		//right click
		if(MouseInsL(tb->sby+tb->sbly,tb->y,tb->btnly,tb->sblx)&&realLpressed)
			tb->btn_dn1p->Draw(tb->sby+tb->sbly,tb->y);
		else tb->btn_dn1->Draw(tb->sby+tb->sbly,tb->y);
		tb->marker->Draw(tb->x+tb->btnly+div(int(tb->sbly-tb->mkly+2)*tb->SPos,tb->SMaxPos).quot,tb->y+1);
	}else{
		tb->sbar1->Draw(tb->sby+2,tb->y);
		tb->btn_up0->Draw(tb->x,tb->y);
		tb->btn_dn0->Draw(tb->sby+tb->sbly,tb->x);
	};
	return true;
};
bool HScrollBar_MouseOver(SimpleDialog* SD){
	VScrollBar* tb=(VScrollBar*)SD;
	int dt=GetTickCount()-tb->LastTime;
	if(tb->SMaxPos>0){
		if(!realLpressed)tb->Zaxvat=false;
		int my=tb->x+tb->btnly+div(int(tb->sbly-tb->mkly+2)*tb->SPos,tb->SMaxPos).quot;
		if(!tb->Zaxvat){
			if(MouseInsL(tb->x,tb->y,tb->btnly,tb->sblx)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				int txx=tb->SPos;
				if(tb->SPos>0)tb->SPos--;
				if(tb->SPos!=txx)tb->NeedToDraw=true;
			};
			if(MouseInsL(tb->sby+tb->sbly,tb->y,tb->btnly,tb->sblx)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				int txx=tb->SPos;
				if(tb->SPos<tb->SMaxPos)tb->SPos++;
				if(tb->SPos!=txx)tb->NeedToDraw=true;
			};
			if(MouseIns(tb->x+tb->btnly,tb->y,my,tb->y+tb->sblx-1)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				int txx=tb->SPos;
				if(tb->SPos>6)tb->SPos-=6;
				else tb->SPos=0;
				if(tb->SPos!=txx)tb->NeedToDraw=true;
			};
			if(MouseIns(my+tb->mkly,tb->y,tb->x+tb->sbly+tb->btnly-1,tb->y+tb->sblx-1)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				int txx=tb->SPos;
				if(tb->SMaxPos-tb->SPos>6)tb->SPos+=6;
				else tb->SPos=tb->SMaxPos;
				if(tb->SPos!=txx)tb->NeedToDraw=true;
			};
			if(MouseInsL(my,tb->y,tb->mkly,tb->sblx)&&Lpressed&&dt>150){
				tb->LastTime=GetTickCount();
				//Lpressed=false;
				tb->Zaxvat=true;
			};
		}else{
			int txx=tb->SPos;
			tb->SPos=div((mouseX-tb->sby-(tb->mkly>>1))*tb->SMaxPos,tb->sbly-tb->mkly).quot;
			if(tb->SPos<0)tb->SPos=0;
			if(tb->SPos>tb->SMaxPos)tb->SPos=tb->SMaxPos;
			if(tb->SPos!=txx)tb->NeedToDraw=true;
		};
	};
	return true;
};
VScrollBar* ParentFrame::addHScrollBar(SimpleDialog* Parent,
						int x,int y,int MaxPos,int Pos,
						SQPicture* btn_up0,
						SQPicture* btn_up1,
						SQPicture* btn_up1p,
						SQPicture* btn_dn0,
						SQPicture* btn_dn1,
						SQPicture* btn_dn1p,
						SQPicture* sbar0,
						SQPicture* sbar1,
						SQPicture* marker){
				
	VScrollBar* tb=new VScrollBar;
	AddDialog(tb);
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->sblx=sbar0->GetLy();
	tb->sbly=sbar0->GetLx();
	tb->mkly=byte(marker->GetLx());
	tb->btnly=btn_dn1->GetLx()-2;
	tb->x1=tb->x+tb->sbly+(tb->btnly<<1)-1;
	tb->y1=tb->y+tb->sblx-1;
	tb->sby=tb->x+tb->btnly;
	tb->SMaxPos=MaxPos;
	tb->SPos=Pos;
	tb->btn_up0=btn_up0;
	tb->btn_up1=btn_up1;
	tb->btn_up1p=btn_up1p;
	tb->btn_dn0=btn_dn0;
	tb->btn_dn1=btn_dn1;
	tb->btn_dn1p=btn_dn1p;
	tb->sbar0=sbar0;
	tb->sbar1=sbar1;
	tb->marker=marker;
	tb->Zaxvat=false;
	tb->OnDraw=&HScrollBar_Draw;
	tb->OnMouseOver=&HScrollBar_MouseOver;
	tb->Enabled=true;
	tb->Active=false;
	tb->GP_File=-1;
	tb->StartGP_Spr=-1;
	tb->OnesDy=1;
	return tb;
};
//-------end of horizontal Scroll bar--------//
//--------------GP_Scrollbar-----------------//
bool GP_ScrollBar_OnDraw(SimpleDialog* SD){
	VScrollBar* SB=(VScrollBar*)SD;
	if(!(SB->Enabled&&SB->Visible&&SB->SMaxPos>0)){
		SB->Zaxvat=false;
		return false;
	};
	//
	SB->x1=SB->x+SB->LineLx;
	SB->y1=SB->y+SB->LineLy;
	//
	if(SB->SPos>SB->SMaxPos)SB->SPos=SB->SMaxPos;
	if(!SB->SMaxPos)return false;
	if(SB->LineIndex>=0)GPS_ShowGP(SB->x,SB->y,SB->GP_File,SB->LineIndex,0);
	//setting new value for scroller
	int scx,scy;
	SB->ScrLx=GPS.GetGPWidth(SB->GP_File,0);
	int LLx=0;//GPS.GetGPWidth(SB->GP_File,1);
	int RLx=LLx;//GPS.GetGPWidth(SB->GP_File,2);
	if(SB->LineLx>SB->LineLy){
		scx=SB->x+LLx+(SB->SPos*(SB->LineLx-SB->ScrLx-LLx-RLx))/SB->SMaxPos+SB->ScrDx;
		scy=SB->y+SB->ScrDy;
	}else{
		scx=SB->x+SB->ScrDx;
		scy=SB->y+SB->ScrDy+(SB->SPos*(SB->LineLy-SB->ScrLy))/SB->SMaxPos;
	};
	if(!realLpressed)SB->Zaxvat=false;
	if(!SB->Zaxvat){
		if(Lpressed&&mouseX>scx&&mouseY>scy&&mouseX<scx+SB->ScrLx-1&&mouseY<scy+SB->ScrLy-1){
			SB->Zaxvat=true;
			SB->sbx=mouseX;
			SB->sby=mouseY;
			SB->sblx=SB->SPos;
			Lpressed=false;
			UnPress();
		};
	};
	if(SB->Zaxvat){
		int dx=mouseX-SB->sbx;
		int dy=mouseY-SB->sby;
		if(SB->LineLx>SB->LineLy){
			SB->SPos=SB->sblx+dx*(SB->SMaxPos+1)/(SB->LineLx-SB->ScrLx-LLx-RLx);
		}else{
			SB->SPos=SB->sblx+dy*(SB->SMaxPos+1)/(SB->LineLy-SB->ScrLy);
		};
		if(SB->SPos<0)SB->SPos=0;
		if(SB->SPos>SB->SMaxPos)SB->SPos=SB->SMaxPos;
		if(SB->LineLx>SB->LineLy){
			scx=SB->x+LLx+(SB->SPos*(SB->LineLx-SB->ScrLx-LLx-RLx))/SB->SMaxPos+SB->ScrDx;
			scy=SB->y+SB->ScrDy;
		}else{
			scx=SB->x+SB->ScrDx;
			scy=SB->y+SB->ScrDy+(SB->SPos*(SB->LineLy-SB->ScrLy))/SB->SMaxPos;
		};
	};
	//-----------------------------
	if(SB->LineLx>SB->LineLy){
		GPS_ShowGP(scx,scy,SB->GP_File,SB->ScrollIndex,0);
	}else{
		GPS_ShowGP(scx,scy,SB->GP_File,SB->ScrollIndex,0);
	};
	return false;
};
VScrollBar* ParentFrame::addGP_ScrollBar(SimpleDialog* Parent,int x,int y,int MaxPos,int Pos,int GP_File,
							int ScrIndex,int LineIndex,int ScrDx,int ScrDy){
	VScrollBar* tb=new VScrollBar;
	AddDialog(tb);
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->x1=tb->x+GPS.GetGPWidth(GP_File,LineIndex)-1;
	tb->y1=tb->y1+GPS.GetGPHeight(GP_File,LineIndex)-1;
	tb->LineLx=GPS.GetGPWidth(GP_File,LineIndex);
	tb->LineLy=GPS.GetGPHeight(GP_File,LineIndex);
	tb->ScrLx=GPS.GetGPWidth(GP_File,ScrIndex);
	tb->ScrLy=GPS.GetGPHeight(GP_File,ScrIndex);
	tb->ScrDx=ScrDx;
	tb->ScrDy=ScrDy;
	if(tb->LineLx>tb->LineLy)tb->ScrDy-=tb->ScrLy>>1;
	else tb->ScrDx-=tb->ScrLx>>1;
	tb->GP_File=GP_File;
	tb->ScrollIndex=ScrIndex;
	tb->LineIndex=LineIndex;
	tb->SPos=Pos;
	tb->SMaxPos=MaxPos;
	tb->Active=false;
	tb->Zaxvat=false;
	tb->Enabled=true;
	tb->OnDraw=&GP_ScrollBar_OnDraw;
	tb->StartGP_Spr=-1;
	tb->OnesDy=1;
	return tb;
};
bool NewGP_VScrollBar_OnDraw(SimpleDialog* SD){
	VScrollBar* SB=(VScrollBar*)SD;
	if(SB->SPos>SB->SMaxPos)SB->SPos=SB->SMaxPos;
	if(!(/*SB->Enabled&&*/SB->Visible)){
		//SB->Zaxvat=false;
		return false;
	};
	RestoreTextShader();
	if(SB->ParentDS){
		DialogsDesk* DD=dynamic_cast<DialogsDesk*>(SB->ParentDS);
		if(DD&&!DD->Visible) return false;
	}
	bool DontShow=0;
	DWORD diff=GPS.GetCurrentDiffuse();
	if(!SB->Enabled){
		DontShow=0;
		GPS.SetCurrentDiffuse(0x80FFFFFF);
	}
	if(!SB->SMaxPos){
		SB->SMaxPos=1;
		DontShow=1;		
		GPS.SetCurrentDiffuse(0x80FFFFFF);
	}
	int upid=0;
	int dnid=2;
	if(mouseX>SB->x&&mouseX<SB->x1&&mouseY>SB->y&&mouseY<SB->y+SB->btnly)upid++;
	if(mouseX>SB->x&&mouseX<SB->x1&&mouseY<SB->y1&&mouseY>SB->y1-SB->btnly)dnid++;	
	DrawVScroller(SB->x,SB->y,SB->y1-SB->y+1,SB->GP_File,SB->StartGP_Spr+upid,SB->StartGP_Spr+dnid,SB->StartGP_Spr+5,3);
	GPS.SetCurrentDiffuse(diff);

	int scx,scy;
	int ULY=GPS.GetGPHeight(SB->GP_File,SB->StartGP_Spr+upid);
	int DLY=GPS.GetGPHeight(SB->GP_File,SB->StartGP_Spr+dnid);
	SB->LineLy=SB->y1-SB->y-ULY-DLY;
	scx=SB->x;
	scy=SB->y+SB->btnly+(SB->SPos*(SB->LineLy-SB->ScrLy))/SB->SMaxPos;
	//new 30.03
	int midid=0;
	if(SB->x<=mouseX&&SB->x1>=mouseX&&SB->y+SB->btnly<=mouseY&&scy>=mouseY)midid=2;
	if(SB->x<=mouseX&&SB->x1>=mouseX&&scy+SB->ScrLy<=mouseY&&SB->y1-SB->btnly>=mouseY)midid=4;
	if(SB->Enabled){
		//setting new value for scroller
		if(!realLpressed){
			SB->Zaxvat=false;
			if(SB->Zaxvat2)SB->LastTime-=600;
			SB->Zaxvat2=0;
		}
		if(!SB->Zaxvat){
			if(Lpressed&&mouseX>scx&&mouseY>scy&&mouseX<scx+SB->ScrLx-1&&mouseY<scy+SB->ScrLy-1){
				SB->Zaxvat=true;
				SB->sbx=mouseX;
				SB->sby=mouseY;
				SB->sblx=SB->SPos;
				Lpressed=false;
				UnPress();
			};
		};
		if(SB->Zaxvat&&SB->LineLy!=SB->ScrLy){
			int dx=mouseX-SB->sbx;
			int dy=mouseY-SB->sby;
			SB->SPos=SB->sblx+dy*(SB->SMaxPos+1)/(SB->LineLy-SB->ScrLy);
			if(SB->SPos<0)SB->SPos=0;
			if(SB->SPos>SB->SMaxPos)SB->SPos=SB->SMaxPos;
			scx=SB->x;
			scy=SB->y+SB->btnly+(SB->sblx*(SB->LineLy-SB->ScrLy))/SB->SMaxPos+dy;
			if(scy<SB->y+SB->btnly)scy=SB->y+SB->btnly;
			if(scy>SB->y+SB->btnly+SB->LineLy-SB->ScrLy)scy=SB->y+SB->btnly+SB->LineLy-SB->ScrLy;
		}else{
			int Time=GetTickCount();
			if(realLpressed && Time-SB->LastTime>100){
				SB->LastTime=Time;
				if(!SB->Zaxvat2 && Lpressed){
 					if(upid==1)SB->Zaxvat2=1;
					else if(dnid==3)SB->Zaxvat2=3;
					else if(midid==2)SB->Zaxvat2=2;
					else if(midid==4)SB->Zaxvat2=4;
					if(SB->Zaxvat2)SB->LastTime+=600;
				}
				if(SB->Zaxvat2){
					if(SB->Zaxvat2==1 && upid==1)
						SB->SPos-=SB->OnesDy;
					else if(SB->Zaxvat2==3 && dnid==3)
						SB->SPos+=SB->OnesDy;
					else if(SB->Zaxvat2==2 && midid==2){
						SB->SPos-=SB->ScrDy;
						int min_pos=SB->SMaxPos-SB->SMaxPos*(SB->y1-mouseY-SB->btnly-SB->ScrLy/2)/(SB->LineLy-SB->ScrLy);
						if(SB->SPos<min_pos)SB->SPos=min_pos;
					}else if(SB->Zaxvat2==4 && midid==4){
						SB->SPos+=SB->ScrDy;
						int max_pos=SB->SMaxPos*(mouseY-SB->y-SB->btnly-SB->ScrLy/2)/(SB->LineLy-SB->ScrLy);
						if(SB->SPos>max_pos)SB->SPos=max_pos;
					}
					if(SB->SPos<0)SB->SPos=0;
					if(SB->SPos>SB->SMaxPos)SB->SPos=SB->SMaxPos;
					Lpressed=false;
					UnPress();
				};
			};
		};
	}
	//end new
	/*if(SB->Enabled){
		//if(SB->LineIndex>=0)GPS_ShowGP(SB->x,SB->y,SB->GP_File,SB->LineIndex,0);
		//setting new value for scroller
 		if(!realLpressed)SB->Zaxvat=false;
		if(!SB->Zaxvat){
			if(Lpressed&&mouseX>scx&&mouseY>scy&&mouseX<scx+SB->ScrLx-1&&mouseY<scy+SB->ScrLy-1){
				SB->Zaxvat=true;
				SB->sbx=mouseX;
				SB->sby=mouseY;
				SB->sblx=SB->SPos;
				Lpressed=false;
				UnPress();
			};
		};
		if(SB->Zaxvat&&SB->LineLy!=SB->ScrLy){
			int dx=mouseX-SB->sbx;
			int dy=mouseY-SB->sby;
			SB->SPos=SB->sblx+dy*(SB->SMaxPos+1)/(SB->LineLy-SB->ScrLy);
			if(SB->SPos<0)SB->SPos=0;
			if(SB->SPos>SB->SMaxPos)SB->SPos=SB->SMaxPos;
			scx=SB->x;
			scy=SB->y+SB->btnly+(SB->SPos*(SB->LineLy-SB->ScrLy))/SB->SMaxPos;
		}else{
			if(GetTickCount()-SB->LastTime>100){
				if(upid==1&&realLpressed){//Lpressed){
					SB->SPos-=SB->OnesDy;
					if(SB->SPos<0)SB->SPos=0;
					SB->LastTime=GetTickCount();
					//new 4.03
					Lpressed=false;
					UnPress();
				};
				if(dnid==3&&realLpressed){//Lpressed){
					SB->SPos+=SB->OnesDy;
					if(SB->SPos>SB->SMaxPos)SB->SPos=SB->SMaxPos;
					SB->LastTime=GetTickCount();
					// new 4.03
					Lpressed=false;
					UnPress();
				};
				if(Lpressed&&SB->MouseOver&&upid==0&&dnid==2){
					if(mouseY<scy){
						SB->SPos-=SB->ScrDy;
						if(SB->SPos<0)SB->SPos=0;
						Lpressed=false;
						UnPress();
					}else{
						SB->SPos+=SB->ScrDy;
						if(SB->SPos>SB->SMaxPos)SB->SPos=SB->SMaxPos;
						Lpressed=false;
						UnPress();
					};
				};
			};
		};
	}*/
	//-----------------------------
	if(!DontShow)GPS_ShowGP(scx,scy,SB->GP_File,SB->StartGP_Spr+4,0);
	return false;
};
int ScrollFileID=-1;
VScrollBar* ParentFrame::addNewGP_VScrollBar(SimpleDialog* Parent,int x,int y,int Ly,
						   int MaxPos,int Pos,int GP_File,int Sprite){
	if(ScrollFileID==-1) ScrollFileID=GPS.PreLoadGPImage("Interf3\\elements\\scroll");
	//GP_File=ScrollFileID;
	VScrollBar* tb=new VScrollBar;
	AddDialog(tb);
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->x1=tb->x+GPS.GetGPWidth(GP_File,Sprite)-1;
	tb->y1=tb->y+Ly-1;
	tb->LineLx=GPS.GetGPWidth(GP_File,Sprite);
	tb->LineLy=Ly-2*GPS.GetGPHeight(GP_File,Sprite);
	tb->ScrLx=GPS.GetGPWidth(GP_File,Sprite);
	tb->ScrLy=GPS.GetGPHeight(GP_File,Sprite+4);
	tb->ScrDx=0;
	tb->ScrDy=0;
	//if(tb->LineLx>tb->LineLy)tb->ScrDy-=tb->ScrLy>>1;
	//else tb->ScrDx-=tb->ScrLx>>1;	
	//tb->GP_File=ScrollFileID;
	//tb->StartGP_Spr=0;
	tb->GP_File=GP_File;
	tb->StartGP_Spr=Sprite;
	tb->LineIndex=-1;
	tb->SPos=Pos;
	tb->SMaxPos=MaxPos;
	tb->Active=false;
	tb->Zaxvat=false;
	tb->Enabled=true;
	tb->btnly=GPS.GetGPHeight(GP_File,Sprite);
	tb->OnDraw=&NewGP_VScrollBar_OnDraw;
	tb->LastTime=GetTickCount();
	tb->OnesDy=1;
	return tb;
};
VScrollBar* ParentFrame::addGP_ScrollBarL(SimpleDialog* Parent,int x,int y,
								int MaxPos,int Pos,int GP_File,
								int ScrIndex,int LineLx,int LineLy,int ScrDx,int ScrDy){
	VScrollBar* tb=new VScrollBar;
	AddDialog(tb);
	tb->NeedToDraw=true;
	tb->Parent=Parent;
	tb->x=BaseX+x;
	tb->y=BaseY+y;
	tb->x1=tb->x+LineLx-1;
	tb->y1=tb->y1+LineLy-1;
	tb->LineLx=LineLx;
	tb->LineLy=LineLy;
	tb->ScrLx=GPS.GetGPWidth(GP_File,ScrIndex);
	tb->ScrLy=GPS.GetGPHeight(GP_File,ScrIndex);
	tb->ScrDx=ScrDx;
	tb->ScrDy=ScrDy;
	if(tb->LineLx>tb->LineLy)tb->ScrDy-=tb->ScrLy>>1;
	else tb->ScrDx-=tb->ScrLx>>1;
	tb->GP_File=GP_File;
	tb->ScrollIndex=ScrIndex;
	tb->LineIndex=-1;
	tb->SPos=Pos;
	tb->SMaxPos=MaxPos;
	tb->Active=false;
	tb->Zaxvat=false;
	tb->Enabled=true;
	tb->OnDraw=&GP_ScrollBar_OnDraw;
	tb->StartGP_Spr=-1;
	tb->OnesDy=1;
	return tb;
};
VScrollBar::VScrollBar(){
	static int Slider=-1;
	if(Slider==-1){
		Slider=GPS.PreLoadGPImage("interf3\\elements\\Scroll");
		if(Slider==-1) Slider=0;
	}
	GP_File=Slider;
	ScrollIndex=0;

	int Ly=100;
	LineLx=GPS.GetGPWidth(GP_File,ScrollIndex);
	LineLy=Ly-2*GPS.GetGPHeight(GP_File,ScrollIndex);	
	ScrLx=GPS.GetGPWidth(GP_File,ScrollIndex);
	ScrLy=GPS.GetGPHeight(GP_File,ScrollIndex+4);
	x=0;
	y=0;
	x1=x+LineLx-1;
	y1=y+Ly-1;
	ScrDx=0;
	ScrDy=0;
	//if(tb->LineLx>tb->LineLy)tb->ScrDy-=tb->ScrLy>>1;
	//else tb->ScrDx-=tb->ScrLx>>1;	
	SPos=0;
	SMaxPos=100;
	Active=false;
	Zaxvat=false;
	Zaxvat2=0;
	Enabled=true;
	LineIndex=-1;
	StartGP_Spr=0;
	OnesDy=1;
	Parent=NULL;
	//OnDraw=&GP_ScrollBar_OnDraw;
	OnDraw=&NewGP_VScrollBar_OnDraw;
	NeedToDraw=true;
	LastTime=GetTickCount();
	btnly=GPS.GetGPHeight(GP_File,ScrollIndex);
}
HorisontalSlider::HorisontalSlider(){
	static int Slider=-1;
	if(Slider==-1){
		Slider=GPS.PreLoadGPImage("interf3\\elements\\Scroll");
		if(Slider==-1) Slider=0;
	}
	GP_File=Slider;
	ScrollIndex=0;

	int Ly=100;
	LineLx=GPS.GetGPWidth(GP_File,ScrollIndex);
	LineLy=Ly-2*GPS.GetGPHeight(GP_File,ScrollIndex);	
	ScrLx=GPS.GetGPWidth(GP_File,ScrollIndex);
	ScrLy=GPS.GetGPHeight(GP_File,ScrollIndex+4);
	x=0;
	y=0;
	x1=x+LineLx-1;
	y1=y+Ly-1;
	ScrDx=0;
	ScrDy=0;
	//if(tb->LineLx>tb->LineLy)tb->ScrDy-=tb->ScrLy>>1;
	//else tb->ScrDx-=tb->ScrLx>>1;	
	SPos=0;
	SMaxPos=100;
	Active=false;
	Zaxvat=false;
	Enabled=true;
	LineIndex=-1;
	StartGP_Spr=0;
	OnesDy=1;
	Parent=NULL;
	//OnDraw=&GP_ScrollBar_OnDraw;
	OnDraw=&GP_ScrollBar_OnDraw;
	NeedToDraw=true;
	LastTime=GetTickCount();
	btnly=GPS.GetGPHeight(GP_File,ScrollIndex);
}
//-----------end of GP_ScrollBar-------------//

//-------------class : ComplexBox------------//
bool ComplexBox_OnMouseOver(SimpleDialog* SD){
	//if(!Lpressed)return false;
	ComplexBox* LB=(ComplexBox*)SD;
	SD->NeedToDraw=true;
	int ni=(mouseY-LB->y)/LB->OneLy;
	int curitem;
	if(ni<LB->NOnScr&&LB->FirstVisItem+ni<LB->N){
		curitem=LB->FirstVisItem+ni;
		if(curitem>=0){
			LB->M_OvrItem=curitem;
			if(Lpressed)LB->CurrentItem=curitem;
		};
	}else LB->M_OvrItem=-1;
	return true;
};
bool ComplexBox_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false; 
	ComplexBox* LB=(ComplexBox*)SD;
	if(!LB->MouseOver)LB->M_OvrItem=-1;
	if(LB->VS){
		int mxp=(LB->N-LB->NOnScr)*10;
		if(mxp<0){
			LB->VS->SMaxPos=0;
			LB->VS->SPos=0;
			LB->VS->Visible=1;
			LB->VS->Enabled=0;
		}else{
			LB->VS->SMaxPos=mxp;
			// new 10.03
			extern short WheelDelta;
			if(LB->Enabled && LB->MouseOver && WheelDelta){
				LB->VS->SPos-=WheelDelta/12;
				WheelDelta=0;
			}
			if(LB->VS->SPos<0)LB->VS->SPos=0;
			// end new
			if(LB->VS->SPos>LB->VS->SMaxPos)LB->VS->SPos=LB->VS->SMaxPos;
			LB->VS->Visible=1;
			LB->VS->Enabled=1;
		};
		LB->FirstVisItem=LB->VS->SPos/10;
	};
	for(int i=0;i<LB->NOnScr;i++){
		int cp=i+LB->FirstVisItem;
		if(cp<LB->N){
			int xx=LB->x;
			int yy=LB->y+LB->OneLy*i;
			byte state=0;
			if(LB->M_OvrItem==cp){
				if(LB->GP_Index!=-1)GPS_ShowGP(xx,yy,LB->GP_Index,LB->StSprite+3+(cp%3),0);
				state=2;
			}else if(LB->CurrentItem==cp){
				if(LB->GP_Index!=-1)GPS_ShowGP(xx,yy,LB->GP_Index,LB->StSprite+(cp%3),0);
				state=1;
			};
			LB->DRAWPROC(xx,yy,LB->x1-LB->x,LB->OneLy,cp,state,LB->param);
		};
	};
	return true;
};
ComplexBox* ParentFrame::addComplexBox(int x,int y,int Ny,int OneLy,
						  procDrawBoxElement* PDRAW,int GP_File,int spr){
	ComplexBox* LB=new ComplexBox;
	AddDialog(LB);
	LB->NeedToDraw=true;
	LB->Parent=NULL;
	LB->x=x+BaseX;
	LB->y=y+BaseY;
	LB->OneLy=OneLy;
	LB->N=0;
	LB->x1=x+GPS.GetGPWidth(GP_File,spr);
	LB->y1=y+Ny*OneLy;
	LB->OneLy=OneLy;
	LB->DRAWPROC=PDRAW;
	LB->N=0;
	LB->GP_Index=GP_File;
	LB->StSprite=spr;
	LB->VS=NULL;
	LB->CurrentItem=0;
	LB->M_OvrItem=-1;
	LB->Enabled=1;
	LB->OnMouseOver=&ComplexBox_OnMouseOver;
	LB->OnDraw=&ComplexBox_OnDraw;
	LB->VS=NULL;
	LB->NOnScr=Ny;
	LB->param=0;
	return LB;
};
//-------------end of ComplexBox-------------//
//-------------class : CustomBox------------//
bool CustomBox_OnDraw(SimpleDialog* SD){
	CustomBox* LB=(CustomBox*)SD;
	if(LB->Visible)
		LB->DRAWPROC(LB->x,LB->y,LB->x1-LB->x+1,LB->y1-LB->y+1,0,LB->MouseOver,LB->param);
	return true;
};
CustomBox* ParentFrame::addCustomBox(int x,int y,int Lx,int Ly,procDrawBoxElement* PDRAW){
	CustomBox* LB=new CustomBox;
	AddDialog(LB);
	LB->NeedToDraw=true;
	LB->Parent=NULL;
	LB->x=x+BaseX;
	LB->y=y+BaseY;
	LB->x1=x+Lx-1;
	LB->y1=y+Ly-1;
	LB->DRAWPROC=PDRAW;
	LB->Enabled=1;
	LB->OnDraw=&CustomBox_OnDraw;
	LB->param=0;
	return LB;
};
//-------------end of ComplexBox-------------//
//-------------class : ListBox---------------//
ListBoxItem* ListBox::GetItem(int i){
	ListBoxItem* LB=FirstItem;
	for(int j=0;j<i;j++){
		if(LB)LB=LB->NextItem;
	};
	return LB;
};
bool LB_SORT_ITEM=0;
void ListBox::AddItem(char* str,int n){
	NeedToDraw=true;
	ListBoxItem* LB=new ListBoxItem;
	if(LastItem){
		if(LB_SORT_ITEM){
			ListBoxItem* LBF=FirstItem;
			ListBoxItem** PPTR=&FirstItem;
			bool NeedIns=1;
			do{
				if(_stricmp(LBF->Message,str)>0){
					*PPTR=LB;
					LB->NextItem=LBF;
					NeedIns=0;
				}else{
					if(!LBF->NextItem){
						LBF->NextItem=LB;
						LB->NextItem=0;
						NeedIns=0;
					}else{
						PPTR=&LBF->NextItem;
						LBF=LBF->NextItem;
					};
				};
			}while(NeedIns);
		}else{
			LastItem->NextItem=LB;
			LB->NextItem=NULL;
		};
	}else{
		FirstItem=LB;
		LastItem=LB;
		LB->NextItem=NULL;
	};
	LastItem=LB;
	NItems++;
	LB->Message=znew(char,strlen(str)+1);
	strcpy(LB->Message,str);
	LB->Param1=n;
	LB->Flags=0;
};
void ListBox::AddStaticItem(char* str,int n){
	NeedToDraw=true;
	ListBoxItem* LB=new ListBoxItem;
	if(LastItem){
		LastItem->NextItem=LB;
		LB->NextItem=NULL;
	}else{
		FirstItem=LB;
		LastItem=LB;
		LB->NextItem=NULL;
	};
	LastItem=LB;
	NItems++;
	LB->Message=znew(char,strlen(str)+1);
	strcpy(LB->Message,str);
	LB->Param1=n;
	LB->Flags=1;
};
void ListBox::ClearItems(){
	NeedToDraw=true;
	ListBoxItem* LB1,*LB=FirstItem;
	while(LB){
		free(LB->Message);
		LB1=LB->NextItem;
		delete(LB);
		LB=LB1;
	};
	NItems=0;
	FirstItem=NULL;
	LastItem=NULL;
	NItems=0;
	CurItem=0;
	FLItem=0;
};
void ListBox::SetFirstPos(int n){
	if(n!=FLItem)NeedToDraw=true;
	FLItem=n;
};
void ListBox::SetCurrentItem(int n){
	if(n!=CurItem)NeedToDraw=true;
	CurItem=n;
	if(CurItem>=NItems)CurItem=NItems-1;
	if(!NItems)CurItem=-1;
	if(CurItem<FLItem)FLItem=CurItem;
};
bool ListBox_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	ListBox* LB=(ListBox*)SD;
	if(LB){
		if(!LB->MouseOver)LB->CurMouseOver=-1;
		if(KeyPressed&&LB->NItems&&LB->Active){
			if(LastKey==40)LB->CurItem++;
			if(LastKey==38)LB->CurItem--;
			if(LastKey==34)LB->CurItem+=3;
			if(LastKey==33)LB->CurItem-=3;
			if(LB->CurItem<0)LB->CurItem=0;
			if(LB->CurItem>LB->NItems-1)LB->CurItem=LB->NItems-1;
			if(LastKey==40||LastKey==38||LastKey==34||LastKey==33){
				if(LB->CurItem<LB->FLItem)LB->FLItem=LB->CurItem;
				if(LB->CurItem-LB->FLItem>=LB->ny)LB->FLItem=LB->CurItem-LB->ny+1;
				if(LB->VS&&LB->VS->Visible)LB->VS->SPos=LB->FLItem*32;
			};
			KeyPressed=false;
		};
		if(LB->GP_File==-1){
			int y=LB->y;
			for(int i=0;i<LB->ny;i++){
				if (LB->ItemPic) LB->ItemPic->Draw(LB->x,y);
				int Iind=LB->FLItem+i;
				ListBoxItem* str=LB->GetItem(Iind);
				if(str){
					if(str->Flags){
						ShowStringEx(LB->x+8+LB->FontDx,y+8+LB->FontDy,str->Message,LB->DFont);
					}else{
						if(Iind==LB->CurItem)
							ShowStringEx(LB->x+8+LB->FontDx,y+8+LB->FontDy,str->Message,LB->AFont);
						else ShowStringEx(LB->x+8+LB->FontDx,y+8+LB->FontDy,str->Message,LB->PFont);
					};
				};
				y+=LB->oneLy-1;
			};
		}else{
			int y=LB->y;
			int fdy=((LB->oneLy-GetRLCHeight(LB->AFont->RLC,'W'))>>1)+LB->FontDy;
			int fdx=LB->FontDx;
			for(int i=0;i<LB->ny;i++){
				// -- Inserted by Silver ---29.10.2002
				// -- reason: LB->ItemPic may be null?..
				if (LB->ItemPic) 
				// -- end of hack -- 29.10.2002
				LB->ItemPic->Draw(LB->x,y);

				int Iind=LB->FLItem+i;
				ListBoxItem* str=LB->GetItem(Iind);
				if(str){
					if(LB->M_Over!=-1){
						if(Iind==LB->CurMouseOver){
							GPS_ShowGP(LB->x,y,LB->GP_File,LB->M_Over+(i%3),0);
							ShowStringEx(LB->x+fdx,y+fdy,str->Message,LB->AFont);
						}else{
							if(Iind==LB->CurItem)GPS_ShowGP(LB->x,y,LB->GP_File,LB->Sprite+(i%3),0);
							ShowStringEx(LB->x+fdx,y+fdy,str->Message,LB->PFont);
						};
					}else{
						if(str->Flags){
							ShowStringEx(LB->x+fdx,y+fdy,str->Message,LB->DFont);
						}else{
							if(Iind==LB->CurItem){
								GPS_ShowGP(LB->x,y,LB->GP_File,LB->Sprite+(i%3),0);
								ShowStringEx(LB->x+fdx,y+fdy,str->Message,LB->AFont);
							}else ShowStringEx(LB->x+fdx,y+fdy,str->Message,LB->PFont);
						};
					};
				};
				y+=LB->oneLy;
			};
		};
	};
	return true;
};
bool ListBox_OnMouseOver(SimpleDialog* SD){
	if(!Lpressed)return false;
	ListBox* LB=(ListBox*)SD;
	SD->NeedToDraw=true;
	int ni=div(mouseY-LB->y-8-LB->FontDy,LB->oneLy-1).quot;
	int curitem;
	if(ni<LB->ny){
		if(LB->FLItem+ni<LB->NItems)
			curitem=LB->FLItem+ni;
		else curitem=LB->NItems-1;
		if(curitem>=0){
			ListBoxItem* LBI=LB->GetItem(curitem);
			if(!LBI->Flags)LB->CurItem=curitem;
		};
	};
	return true;
};
bool GPListBox_OnMouseOver(SimpleDialog* SD){
	//if(!Lpressed)return false;
	ListBox* LB=(ListBox*)SD;
	LB->CurMouseOver=-1;
	SD->NeedToDraw=true;
	int ni=div(mouseY-LB->y,LB->oneLy).quot;
	int curitem;
	if(ni<LB->ny){
		if(LB->FLItem+ni<LB->NItems)
			curitem=LB->FLItem+ni;
		else curitem=-1;
		if(curitem==-1)return 1;
		if(Lpressed){
			if(curitem>=0){
				ListBoxItem* LBI=LB->GetItem(curitem);
				if(!LBI->Flags)LB->CurItem=curitem;
			};
		};
		LB->CurMouseOver=curitem;
	};
	return true;
};
bool ListBox_Refresh(SimpleDialog* SD){
	ListBox* LB=(ListBox*)SD;
	if(LB->VS){
		VScrollBar* VS=LB->VS;
		VS->SetMaxPos((LB->NItems-LB->ny)*32);
		VS->OnesDy=32;
		if(VS->GP_File!=-1){
			VS->ScrDy=(LB->ny-1)*32;
		};
		if(VS->GetMaxPos()>0)LB->FLItem=(VS->GetPos()+16)/32;
	};
	return true;
};
ListBox* ParentFrame::addListBox(SimpleDialog* Parent,
						int x,int y,int Ny,
						SQPicture* ItemPic,
						RLCFont* AFont,
						RLCFont* PFont,
						VScrollBar* VS){
	ListBox* LB=new ListBox;
	AddDialog(LB);
	LB->NeedToDraw=true;
	LB->Parent=Parent;
	LB->AFont=AFont;
	LB->PFont=PFont;
	LB->DFont=AFont;
	LB->ItemPic=ItemPic;
	LB->x=x+BaseX;
	LB->y=y+BaseY;
	LB->oneLy=byte(ItemPic->GetLy());
	LB->oneLx=ItemPic->GetLx();
	LB->NItems=0;
	LB->FirstItem=NULL;
	LB->LastItem=NULL;
	LB->y1=LB->y+Ny*int(LB->oneLy);
	LB->x1=LB->x+LB->oneLx;
	LB->OnDraw=&ListBox_OnDraw;
	LB->OnMouseOver=&ListBox_OnMouseOver;
	LB->Refresh=&ListBox_Refresh;
	LB->FLItem=0;
	LB->CurItem=0;
	LB->ny=Ny;
	LB->VS=VS;
	LB->GP_File=-1;
	LB->FontDy=0;
	LB->FontDx=0;
	LB->M_Over=-1;
	LB->CurMouseOver=-1;
	LB->FontDx=0;
	LB->FontDy=0;
	return LB;
};
ListBox* ParentFrame::addListBox(SimpleDialog* Parent,
						int x,int y,int Ny,int Lx,int Ly,
						RLCFont* AFont,
						RLCFont* PFont,
						VScrollBar* VS){
	ListBox* LB=new ListBox;
	AddDialog(LB);
	LB->NeedToDraw=true;
	LB->Parent=Parent;
	LB->AFont=AFont;
	LB->PFont=PFont;
	LB->DFont=AFont;
	LB->ItemPic=NULL;
	LB->x=x+BaseX;
	LB->y=y+BaseY;
	LB->oneLy=Ly;
	LB->oneLx=Lx;
	LB->NItems=0;
	LB->FirstItem=NULL;
	LB->LastItem=NULL;
	LB->y1=LB->y+Ny*int(LB->oneLy);
	LB->x1=LB->x+LB->oneLx;
	LB->OnDraw=&ListBox_OnDraw;
	LB->OnMouseOver=&ListBox_OnMouseOver;
	LB->Refresh=&ListBox_Refresh;
	LB->FLItem=0;
	LB->CurItem=0;
	LB->ny=Ny;
	LB->VS=VS;
	LB->GP_File=-1;
	LB->FontDy=0;
	LB->FontDx=0;
	LB->M_Over=-1;
	return LB;
};
ListBox* ParentFrame::addGP_ListBox(SimpleDialog* Parent,int x,int y,int Ny,
						int GP_File,int Sprite,int Ly,
						RLCFont* AFont,
						RLCFont* PFont,
						VScrollBar* VS){
	ListBox* LB=new ListBox;
	AddDialog(LB);
	LB->NeedToDraw=true;
	LB->Parent=Parent;
	LB->AFont=AFont;
	LB->PFont=PFont;
	LB->DFont=PFont;
	LB->ItemPic=NULL;
	LB->FontDy=0;
	LB->x=x+BaseX;
	LB->y=y+BaseY;
	LB->oneLy=Ly;
	LB->oneLx=GPS.GetGPWidth(GP_File,Sprite);
	LB->NItems=0;
	LB->FirstItem=NULL;
	LB->LastItem=NULL;
	LB->y1=LB->y+Ny*int(LB->oneLy);
	LB->x1=LB->x+LB->oneLx;
	LB->OnDraw=&ListBox_OnDraw;
	LB->OnMouseOver=&GPListBox_OnMouseOver;
	LB->Refresh=&ListBox_Refresh;
	LB->FLItem=0;
	LB->CurItem=0;
	LB->ny=Ny;
	LB->VS=VS;
	LB->FontDx=24;
	LB->FontDy=0;
	LB->Sprite=Sprite;
	LB->GP_File=GP_File;
	LB->M_Over=-1;
	return LB;
};
//-------------end of List Box---------------//
//-----------class : InputBoxOld----------------//
void CutStringToLength(char* str,RLCFont* Fnt,int Lx){
	int L;
	do{
		L=GetRLCStrWidth(str,Fnt);
		if(L>Lx)str[strlen(str)-1]=0;
	}while(L>Lx);

};
bool InputBoxOld_OnDraw(SimpleDialog* SD){
	InputBoxOld* IB=(InputBoxOld*)SD;
	if(!SD->Visible||SD->EditMode)return false;	
	SimpleClassConnector* SCC=IB->Connect_edit_string_to_fileld.Get();
	if(SCC){
		if(!IB->_Str){
			IB->_Str=&IB->DefaultStr;
		}
		SCC->_exchange(*IB->_Str);
	}
	if(SD&&(IB->Str||IB->_Str)){
		//CutStringToLength(IB->Str,IB->AFont,IB->x1-IB->x);
		int LL=IB->x1-IB->x;
		char base[2048];
		strcpy(base,IB->Str?IB->Str:IB->_Str->pchar());
		if(IB->Anonim){
			int L=strlen(IB->Str?IB->Str:IB->_Str->pchar());
			for(int i=0;i<L;i++)base[i]='*';
		};
		int xx=0;
		if(IB->CursPos>strlen(IB->Str?IB->Str:IB->_Str->pchar()))IB->CursPos=strlen(IB->Str?IB->Str:IB->_Str->pchar());
		for(int j=0;j<IB->CursPos;j++){
			xx+=GetRLCWidth(IB->Font->RLC,base[j]);
		};
		if(xx<IB->totdx){
			IB->totdx-=64;
			if(IB->totdx<0)IB->totdx=0;
		};
		if(xx-IB->totdx>LL-32){
			IB->totdx=xx-LL+32;
		};
		TempWindow TW;
		PushWindow(&TW);
		IntersectWindows(IB->x,IB->y,IB->x1,IB->y1);
		
		//if(IB->Active&&(GetTickCount()/250)&1)strcat(base,"|");
		if(IB->Pict)IB->Pict->Draw(IB->x,IB->y);
		if(IB->Active){
			int dx=0;
			if(IB->Centering)dx=(IB->x1-IB->x-GetRLCStrWidth(base,IB->AFont))>>1;
			//ShowString(IB->x+8+dx-IB->totdx,IB->y+4,base,IB->AFont);
			ShowString(IB->x+dx-IB->totdx,IB->y,base,IB->AFont);
		}else{
			int dx=0;
			if(IB->Centering)dx=(IB->x1-IB->totdx-IB->x-GetRLCStrWidth(base,IB->Font))>>1;
			//ShowString(IB->x+8+dx,IB->y+4,base,IB->Font);
			ShowString(IB->x+dx,IB->y,base,IB->Font);
		};
		base[0]='|';
		base[1]=0;
		if(IB->Active&&(GetTickCount()/250)&1){
			//ShowString(IB->x+8+xx-IB->totdx,IB->y+4,base,IB->Font);
			ShowString(IB->x+xx-IB->totdx,IB->y,base,IB->Font);
		}
		PopWindow(&TW);
	};
	return true;
};
bool InputBoxOld_OnClick(SimpleDialog* SD){
	if(!SD->Active)SD->NeedToDraw=true;
	SD->Active=true;
	InputBoxOld* IB=(InputBoxOld*)SD;
	if(IB->MouseOver&&Lpressed&&(IB->Str||IB->_Str)){
		int xxx=mouseX-IB->LastDrawX+IB->totdx;
		int xx1=0;
		int L=IB->Str?strlen(IB->Str):strlen(IB->_Str->pchar());
		int j;
		for(j=0;j<L;j++){
			if(IB->Anonim)xx1+=GetRLCWidth(IB->Font->RLC,'*');
			else xx1+=GetRLCWidth(IB->Font->RLC,IB->Str?IB->Str[j]:(*IB->_Str)[j]);
			if(xx1>=xxx){
				if(j>=0)IB->CursPos=j;
				if(IB->CursPos<0)IB->CursPos=0;
				j=L+1;
			};
		};
		if(j==L)IB->CursPos=L;
		Lpressed=0;
		UnPress();
	};
	return true;
};
extern int NKeys;
bool InputBoxOld_OnKeyDown(SimpleDialog* SD){
	SD->NeedToDraw=true;
	InputBoxOld* IB=(InputBoxOld*)SD;
	IB->Changed=1;
	if(LastKey==VK_LEFT){
		if(IB->CursPos>0)IB->CursPos--;
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_RIGHT){
		if(IB->CursPos<strlen(IB->Str?IB->Str:IB->_Str->pchar()))IB->CursPos++;
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_END){
		IB->CursPos=strlen(IB->Str?IB->Str:IB->_Str->pchar());
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_HOME){
		IB->CursPos=0;
		KeyPressed=0;
		LastKey=0;
		return true;
	}else
	if(LastKey==VK_BACK){
		if(IB->CursPos>0){
			if(IB->Str)	strcpy(IB->Str+IB->CursPos-1,IB->Str+IB->CursPos);
			else IB->_Str->DeleteChars(IB->CursPos-1,1);
			IB->CursPos--;
		};
		//if(strlen(IB->Str)>0)IB->Str[strlen(IB->Str)-1]=0;
		return true;
	}else
	if(LastKey==46){//VK_DEL
		if(IB->Str){
			if(IB->CursPos<strlen(IB->Str))strcpy(IB->Str+IB->CursPos,IB->Str+IB->CursPos+1);
		}else{
            if(IB->CursPos<strlen(IB->_Str->pchar()))IB->_Str->DeleteChars(IB->CursPos,1);
		}
		//if(strlen(IB->Str)>0)IB->Str[strlen(IB->Str)-1]=0;
		return true;
	}else{
		if(LastAsciiKey&&LastAsciiKey>=32){
			LastKey=LastAsciiKey;
			char xx[2];
			xx[1]=0;
			xx[0]=char(LastKey);
			if(strlen(IB->Str?IB->Str:IB->_Str->pchar())+1<DWORD(IB->StrMaxLen)){
				char ccc[2048];
				strcpy(ccc,IB->Str?IB->Str:IB->_Str->pchar());
				ccc[IB->CursPos]=0;
				strcat(ccc,xx);
				strcat(ccc,(IB->Str?IB->Str:IB->_Str->pchar())+IB->CursPos);
				IB->Str?strcpy(IB->Str,ccc):IB->_Str->Assign(ccc);
				IB->CursPos++;
			};
			return true;
		};
	};
	return false;
};
InputBoxOld::InputBoxOld(){
	NeedToDraw=true;
	Str=NULL;
	_Str=NULL;
	OnDraw=&InputBoxOld_OnDraw;
	OnClick=&InputBoxOld_OnClick;
	OnKeyDown=&InputBoxOld_OnKeyDown;
	Centering=false;
};
InputBoxOld* ParentFrame::addInputBoxOld(SimpleDialog* Parent,
									 int x,int y,char* str,
									 int Len,
									 SQPicture* Panel,
									 RLCFont* RFont,
									 RLCFont* AFont){
	ClearKeyStack();
	InputBoxOld* IB=new InputBoxOld;
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
	IB->OnDraw=&InputBoxOld_OnDraw;
	IB->OnClick=&InputBoxOld_OnClick;
	IB->OnKeyDown=&InputBoxOld_OnKeyDown;
	IB->Centering=false;
	IB->CursPos=strlen(str);
	IB->totdx=0;
	IB->Anonim=0;
	IB->ParentDS=this;
	return IB;
};
InputBoxOld* ParentFrame::addInputBoxOld(SimpleDialog* Parent,
									 int x,int y,char* str,
									 int Len,
									 int Lx,int Ly,
									 RLCFont* RFont,
									 RLCFont* AFont,
									 bool Centering){
	InputBoxOld* IB=new InputBoxOld;
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
	IB->OnDraw=&InputBoxOld_OnDraw;
	IB->OnClick=&InputBoxOld_OnClick;
	IB->OnKeyDown=&InputBoxOld_OnKeyDown;
	IB->Centering=Centering;
	IB->CursPos=strlen(str);
	IB->totdx=0;
	IB->Anonim=0;
	IB->ParentDS=this;
	return IB;
};
InputBoxOld* ParentFrame::addInputBoxOld(SimpleDialog* Parent,
								   int x,int y,_str* str,
								   int Len,
								   int Lx,int Ly,
								   RLCFont* RFont,
								   RLCFont* AFont,
								   bool Centering){
	InputBoxOld* IB=new InputBoxOld;
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
	IB->OnDraw=&InputBoxOld_OnDraw;
	IB->OnClick=&InputBoxOld_OnClick;
	IB->OnKeyDown=&InputBoxOld_OnKeyDown;
	IB->Centering=Centering;
	IB->CursPos=strlen(str->pchar());
	IB->totdx=0;
	IB->Anonim=0;
	IB->ParentDS=this;
	return IB;
};
InputBoxOld* ParentFrame::addInputBoxOld(SimpleDialog* Parent,
									 int x,int y,char* str,
									 int Len,
									 int Lx,int Ly,
									 RLCFont* RFont,
									 RLCFont* AFont){
	InputBoxOld* IB=new InputBoxOld;
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
	IB->OnDraw=&InputBoxOld_OnDraw;
	IB->OnClick=&InputBoxOld_OnClick;
	IB->OnKeyDown=&InputBoxOld_OnKeyDown;
	IB->Centering=false;
	IB->CursPos=strlen(str);
	IB->totdx=0;
	IB->Anonim=0;
	IB->ParentDS=this;
	return IB;
};
//------end of class InputBoxOld----------------//
//----------class : CheckBox-----------------//
bool CheckBox_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	CheckBox* CB=(CheckBox*)SD;
	
	if(CB->GP_File!=-1){
		if(CB->Central){
			int Lx=0;
			int Ly=0;
			int spr;
			if(CB->MouseOver)spr=CB->Sprite0;
			else{
				if(CB->State)spr=CB->Sprite1;
				else spr=CB->Sprite2; 
			};
			GPS_ShowGP(CB->x,CB->y,CB->GP_File,spr+1,0);
			GPS_ShowGP(CB->x1-GPS.GetGPWidth(CB->GP_File,spr+2),CB->y,CB->GP_File,spr+2,0);
			TempWindow TW;
			PushWindow(&TW);
			int x0=CB->x+GPS.GetGPWidth(CB->GP_File,spr+1);
			int x1=CB->x1-GPS.GetGPWidth(CB->GP_File,spr+2);
			IntersectWindows(x0,0,x1,RealLy);
			int LL=GPS.GetGPWidth(CB->GP_File,spr);
			if(!LL)LL=1;
			int N=(x1-x0)/LL+1;
			for(int i=0;i<N;i++){
				GPS_ShowGP(x0+i*LL,CB->y,CB->GP_File,spr,0);
			};
			if(CB->MouseOver){
				int Ty=GetRLCHeight(CB->AFont->RLC,'C');
				int Tx=GetRLen(CB->Message,CB->AFont);
				ShowStringEx((CB->x+CB->x1-Tx)>>1,(CB->y+CB->y1-Ty)>>1,CB->Message,CB->AFont);
			}else{
				int Ty=GetRLCHeight(CB->AFont->RLC,'C');
				int Tx=GetRLen(CB->Message,CB->AFont);
				ShowString((CB->x+CB->x1-Tx)>>1,(CB->y+CB->y1-Ty)>>1,CB->Message,CB->Font);
			};
			PopWindow(&TW);
		}else{
			if(CB->State){
				if(CB->MouseOver&&CB->Sprite3!=-1)GPS_ShowGP(CB->x,CB->y,CB->GP_File,CB->Sprite3,0);
				GPS_ShowGP(CB->x,CB->y,CB->GP_File,CB->Sprite1,0);
			}else{
				if(CB->MouseOver)GPS_ShowGP(CB->x,CB->y,CB->GP_File,CB->Sprite0,0);
				else GPS_ShowGP(CB->x,CB->y,CB->GP_File,CB->Sprite2,0);
			};
			if(CB->Enabled){
				if(CB->MouseOver)ShowStringEx(CB->x+GPS.GetGPWidth(CB->GP_File,CB->Sprite0),CB->y+4,CB->Message,CB->Font);
				else ShowStringEx(CB->x+GPS.GetGPWidth(CB->GP_File,CB->Sprite0),CB->y+4,CB->Message,CB->AFont);
			}
		};
	}else{
		/*if(CB->Transparent){
			if(CB->State)CB->OnPic->Draw(CB->x,CB->y);
			else CB->OffPic->Draw(CB->x,CB->y);
		}else{
			if(CB->State)CB->OnPic->Draw(CB->x,CB->y);
			else CB->OffPic->Draw(CB->x,CB->y);
		//};
		if(CB->MouseOver)ShowString(CB->x+CB->OnPic->GetLx(),CB->y+4,CB->Message,CB->Font);
			else ShowString(CB->x+CB->OnPic->GetLx(),CB->y+4,CB->Message,CB->AFont);
			*/
	};
	return true;
};
bool CheckBox_OnClick(SimpleDialog* SD){
	SD->NeedToDraw=true;
	CheckBox* CB=(CheckBox*)SD;
	if(CB->GroupIndex){
		ParentFrame* DSM=CB->ParentDS;
		int st=CB->State;
		for(int i=0;i<DSM->DSS.GetAmount();i++){
			CheckBox* SDD=(CheckBox*)DSM->DSS[i];
			if(SDD&&SDD->OnClick==&CheckBox_OnClick&&SDD->GroupIndex==CB->GroupIndex){
				SDD->State=false;
				SDD->Changed=1;
				//SDD->NeedToDraw=true;
			};
		};
		//if(st==1)CB->State=2;
		//else CB->State=1;
        CB->State=true;
		Lpressed=0;
		UnPress();
		CB->Changed=1;
	}else{
		CB->State=!CB->State;
		Lpressed=0;
		UnPress();
		CB->Changed=1;
		return true;
	};
	return false;
};
bool CheckBox_Destroy(SimpleDialog* SD){
	CheckBox* CB=(CheckBox*)SD;
	free(CB->Message);
	return true;
};
bool CheckBox_OnMouseOver(SimpleDialog* SD){
	SD->NeedToDraw=true;
	return false;
};
CheckBox* ParentFrame::addCheckBox(SimpleDialog* Parent,
									 int x,int y,char* message,
									 int group,bool State,
									 SQPicture* OnPict,
									 SQPicture* OffPict,
									 RLCFont* Font,
									 RLCFont* AFont){
	CheckBox* CB=new CheckBox;
	AddDialog(CB);
	CB->MouseOverActive=true;
	CB->NeedToDraw=true;
	CB->Parent=Parent;
	CB->x=x+BaseX;
	CB->y=y+BaseY;
	CB->GroupIndex=group;
	CB->State=State;
	CB->x1=CB->x+OnPict->GetLx()+GetRLen(message,Font)-1+2;
	CB->y1=CB->y+OnPict->GetLy()-1+2;
	CB->DS=this;
	CB->OnClick=&CheckBox_OnClick;
	CB->Destroy=&CheckBox_Destroy;
	CB->OnDraw=&CheckBox_OnDraw;
	CB->Font=Font;
	CB->AFont=AFont;
	CB->Message=znew(char,strlen(message)+1);
	//CB->Transparent=true;
	CB->GP_File=-1;
	CB->Central=0;
	CB->Sprite3=-1;
	strcpy(CB->Message,message);
	return CB;
};
int CheckBoxFileID=-1;
CheckBox::CheckBox(){
	MouseOverActive=true;
	NeedToDraw=true;
	Parent=Parent;
	x=0;
	y=0;
	GroupIndex=0;
	State=0;	
	if(CheckBoxFileID==-1) CheckBoxFileID=GPS.PreLoadGPImage("interf3\\elements\\checkbox");
	GP_File=CheckBoxFileID;
	Sprite0=0;
	Sprite1=1;
	Sprite2=0;
	Sprite3=-1;
	x1=x+GPS.GetGPWidth(GP_File,Sprite1);//+GetRLen(message,a_font);
	y1=y+24;//GPS.GetGPHeight(GP_File,Sprite1);
	DS=NULL;
	OnClick=&CheckBox_OnClick;
	Destroy=&CheckBox_Destroy;
	OnDraw=&CheckBox_OnDraw;
	Font=&YellowFont;
	AFont=&WhiteFont;	
	//Transparent=true;
	Central=0;
};
CheckBox* ParentFrame::addGP_CheckBox(SimpleDialog* Parent,
									 int x,int y,char* message,RLCFont* a_font,RLCFont* p_font,
									 int group,bool State,
									 int GP,int active,int passive,int mouseover){
	CheckBox* CB=new CheckBox;
	AddDialog(CB);
	CB->MouseOverActive=true;
	CB->NeedToDraw=true;
	CB->Parent=Parent;
	CB->x=x+BaseX;
	CB->y=y+BaseY;
	CB->GroupIndex=group;
	CB->State=State;	
	if(CheckBoxFileID==-1) CheckBoxFileID=GPS.PreLoadGPImage("interf3\\elements\\checkbox");
	CB->GP_File=CheckBoxFileID;
	CB->Sprite0=0;
	CB->Sprite1=1;
	CB->Sprite2=0;
	CB->Sprite3=-1;
	//CB->GP_File=GP;
	//CB->Sprite0=mouseover;
	//CB->Sprite1=active;
	//CB->Sprite2=passive;
	//CB->Sprite3=-1;
	CB->x1=CB->x+GPS.GetGPWidth(CheckBoxFileID,active)+GetRLen(message,a_font);
	CB->y1=CB->y+24;//GPS.GetGPHeight(CheckBoxFileID,active);
	CB->DS=this;
	CB->OnClick=&CheckBox_OnClick;
	CB->Destroy=&CheckBox_Destroy;
	CB->OnDraw=&CheckBox_OnDraw;
	CB->Font=p_font;
	CB->AFont=a_font;
	CB->Message=znew(char,strlen(message)+1);
	//CB->Transparent=true;
	strcpy(CB->Message,message);
	CB->Central=0;
	CB->Sprite3=-1;
	return CB;
};

CheckBox* ParentFrame::addGP_CheckBox2(SimpleDialog* Parent,
				int x,int y,char* message,RLCFont* a_font,RLCFont* p_font,
				int group,bool State,
				int GP,int active,int passive,int mouseover){
	CheckBox* CB=new CheckBox;
	AddDialog(CB);
	CB->MouseOverActive=true;
	CB->NeedToDraw=true;
	CB->Parent=Parent;
	CB->x=x+BaseX;
	CB->y=y+BaseY;
	CB->GroupIndex=group;
	CB->State=State;	
	//if(CheckBoxFileID==-1) CheckBoxFileID=GPS.PreLoadGPImage("interf3\\elements\\checkbox");
	//CB->GP_File=CheckBoxFileID;
	//CB->Sprite0=0;
	//CB->Sprite1=1;
	//CB->Sprite2=0;
	//CB->Sprite3=-1;
	CB->GP_File=GP;
	CB->Sprite0=mouseover;
	CB->Sprite1=active;
	CB->Sprite2=passive;
	CB->Sprite3=-1;
	CB->x1=CB->x+GPS.GetGPWidth(CheckBoxFileID,active)+GetRLen(message,a_font);
	CB->y1=CB->y+24;//GPS.GetGPHeight(CheckBoxFileID,active);
	CB->DS=this;
	CB->OnClick=&CheckBox_OnClick;
	CB->Destroy=&CheckBox_Destroy;
	CB->OnDraw=&CheckBox_OnDraw;
	CB->Font=p_font;
	CB->AFont=a_font;
	CB->Message=znew(char,strlen(message)+1);
	//CB->Transparent=true;
	strcpy(CB->Message,message);
	CB->Central=0;
	CB->Sprite3=-1;
	return CB;
};
//--------end of CheckBox--------------------//
//-----------colored bar---------------------//
CEXPORT
void CBar(int x0,int y0,int Lx0,int Ly0,byte c){
#ifdef _USE3D
	extern PALETTEENTRY GPal[256];
	Rct rct( x0, y0, Lx0, Ly0 );
	if (!GPS.GetClipArea().Clip( Rct( x0, y0, Lx0, Ly0 ) )) return;
	GPS.DrawFillRect( rct.x, rct.y, rct.w, rct.h, 
			ColorValue::ToARGB( 0xFF, GPal[c].peRed, GPal[c].peGreen, GPal[c].peBlue )  );
#else
	if(Lx0<=0||Ly0<=0||x0<0||y0<0||x0+Lx0>ScrWidth||y0+Ly0>SCRSizeY)return;
	//clipping
	int x=x0;
	int y=y0;
	int Lx=Lx0;
	int Ly=Ly0;
	if(x<WindX){
		Lx-=WindX-x;
		x=WindX;
	};
	if(y<WindY){
		Ly-=WindY-y;
		y=WindY;
	};
	if(Lx<=0||Ly<=0)return;
	if(x+Lx>WindX1){
		Lx=WindX1-x+1;
		if(Lx<=0)return;
	};
	if(y+Ly>WindY1){
		Ly=WindY1-y+1;
		if(Ly<=0)return;
	};
	int ofst=int(ScreenPtr)+x+y*ScrWidth;
	int adds=ScrWidth-Lx;
	int Lx4=Lx>>2;
	int Lx1=Lx&3;
	__asm{
		push	edi
		mov		edi,ofst
		mov		edx,Ly
		cld
		mov		al,c
		mov		ah,al
		shl		eax,16
		mov		al,c
		mov		ah,al
		mov		ebx,Lx1
qwr:	mov		ecx,Lx4
		rep		stosd
		mov		ecx,ebx
		rep		stosb
		add		edi,adds
		dec		edx
		jnz		qwr
		pop		edi
	};
#endif // _USE3D
};
bool ColoredBar_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	ColoredBar* PIC=(ColoredBar*)SD;

	if(PIC->Style){
		if(PIC->color==0xFF)GPS.DrawRect(SD->x,SD->y,SD->x1-SD->x,SD->y1-SD->y,SD->Diffuse);
		else Xbar(SD->x,SD->y,SD->x1-SD->x,SD->y1-SD->y,PIC->color);
	}
	else{
		if(PIC->color==0xFF)GPS.DrawFillRect(SD->x,SD->y,SD->x1-SD->x+1,SD->y1-SD->y+1,SD->Diffuse);
		else CBar(SD->x,SD->y,SD->x1-SD->x,SD->y1-SD->y,PIC->color);
	}
	GPS.FlushBatches();
	return true;
};
ColoredBar* ParentFrame::addColoredBar(int x,int y,int Lx,int Ly,byte c){
	ColoredBar* PIC=new ColoredBar;
	AddDialog(PIC);
	PIC->color=c;
	PIC->x=x+BaseX;
	PIC->y=y+BaseY;
	PIC->x1=PIC->x+Lx;
	PIC->y1=PIC->y+Ly;
	PIC->Parent=NULL;
	PIC->OnDraw=&ColoredBar_OnDraw;
	PIC->Style=0;
	return PIC;
};
bool ColoredBar_OnDraw2(SimpleDialog* SD){
	if(!(SD->Visible&&SD->MouseOver))return false;
	ColoredBar* PIC=(ColoredBar*)SD;
	Xbar(SD->x,SD->y,SD->x1-SD->x+1,SD->y1-SD->y+1,PIC->color);
	return true;
};
ColoredBar* ParentFrame::addViewPort2(int x,int y,int Lx,int Ly,byte c){
	ColoredBar* PIC=new ColoredBar;
	AddDialog(PIC);
	PIC->color=c;
	PIC->x=x+BaseX;
	PIC->y=y+BaseY;
	PIC->x1=PIC->x+Lx;
	PIC->y1=PIC->y+Ly;
	PIC->Parent=NULL;
	PIC->OnDraw=&ColoredBar_OnDraw2;
	PIC->Style=0;
	return PIC;
};
//---------------Text Viewer-----------------//
//---------Service functions---------
void TextViewer::AssignScroll(VScrollBar* SB){
	VS=SB;
	SB->SMaxPos=-1;
	SB->SPos=0;
};
void TextViewer::GetNextLine(LineInfo* inf){
	int offs=inf->Offset;
	int x=0;
	word LS=0;
	word LW=0; 
	word NS=0;
	//char tms[128];
	char c;
	int tmsp=0;
	int ofs;
	int wid;
	int spw=GetRLCWidth(Font->RLC,' ');
	int nlsp=0;//��� ���� ������
	while(offs<TextSize){
		//char c=TextPtr[offs];
		//if(c==' '){
			//reading next word
			tmsp=0;//length of string
			nlsp=0;//amount of spaces
			wid=0;
			while(TextPtr[offs+nlsp]==' '&&offs+nlsp<TextSize){
				nlsp++;
				wid+=spw;
			};
			do{
				ofs=offs+nlsp+tmsp;
				c=0;
				if(ofs<TextSize){
					c=TextPtr[ofs];
					if(c!=' '&&c!=0x0D){
						//tms[tmsp]=c;
						//tmsp++;
						int L=1;
						if(c!='~')wid+=GetRLCWidthUNICODE(Font->RLC,(byte*)(TextPtr+ofs),&L);
						tmsp+=L;
					};
				};
			}while(ofs<TextSize&&c!=' '&&c!=0x0D);
			if(LW+wid<Lx&&c==0x0D){
				inf->NeedFormat=false;
				inf->NextOffset=offs+tmsp+nlsp+2;
				inf->NSpaces=NS;
				inf->LineWidth=LW;
				inf->LineSize=offs-inf->Offset+tmsp+nlsp;
				if(inf->NextOffset>TextSize)inf->NextOffset=TextSize;
				return;
			};
			if(LW+wid<Lx){
				//still current line
				LW+=wid;
				offs+=nlsp+tmsp;
				NS+=nlsp;
			}else{
				if(inf->NextOffset==offs+nlsp){
					//need to break line
					tmsp=0;//length of string
					nlsp=0;//amount of spaces
					wid=0;
					while(TextPtr[offs+nlsp]==' '&&offs+nlsp<TextSize){
						nlsp++;
						wid+=spw;
					};
					int prevpos=0;
					int prevwid=0;
					do{
						prevpos=tmsp;
						prevwid=wid;
						ofs=offs+nlsp+tmsp;
						c=0;
						if(ofs<TextSize){
							c=TextPtr[ofs];
							if(c!=' '&&c!=0x0D){
								int L=1;
								if(c!='~')wid+=GetRLCWidthUNICODE(Font->RLC,(byte*)(TextPtr+ofs),&L);
								tmsp+=L;
							};
						};
					}while(ofs<TextSize&&c!=' '&&c!=0x0D&&LW+wid<Lx);
					if(LW+wid>=Lx){
						wid=prevwid;
						tmsp=prevpos;
					};
					inf->NeedFormat=true;
					inf->NextOffset=offs+nlsp+tmsp;
					inf->NSpaces=NS+nlsp;
					inf->LineWidth=LW+wid;
					inf->LineSize=offs+nlsp+tmsp-inf->Offset;
					if(inf->NextOffset>TextSize)inf->NextOffset=TextSize;
					return;
				}else{
					inf->NeedFormat=true;
					inf->NextOffset=offs+nlsp;
					inf->NSpaces=NS;
					inf->LineWidth=LW;
					inf->LineSize=offs-inf->Offset;
					if(inf->NextOffset>TextSize)inf->NextOffset=TextSize;
					return;
				};
				//};
			};
		//};
	};
	inf->NeedFormat=false;
	inf->NextOffset=TextSize;
	inf->NSpaces=NS;
	inf->LineWidth=LW;
	inf->LineSize=offs-inf->Offset;
};
void TextViewer::CreateLinesList(){
	if(!TextPtr)return;
	NLines=0;
	LineInfo LINF;
	LINF.Offset=0;
	do{
		GetNextLine(&LINF);
		if(LINF.NextOffset!=-1)NLines++;
		LINF.Offset=LINF.NextOffset;
	}while(LINF.NextOffset<TextSize&&LINF.NextOffset);
	LinePtrs=znew(char*,NLines);
	LineSize=znew(word,NLines);
	LineWidth=znew(word,NLines);
	NeedFormat=znew(bool,NLines);
	NSpaces=znew(word,NLines);
	LINF.Offset=0;
	NLines=0;
	do{
		GetNextLine(&LINF);
		if(LINF.LineSize){
			LinePtrs[NLines]=TextPtr+LINF.Offset;
			LineSize[NLines]=LINF.LineSize;
			LineWidth[NLines]=LINF.LineWidth;
			NeedFormat[NLines]=LINF.NeedFormat;
			NSpaces[NLines]=LINF.NSpaces;
			NLines++;
		};
		LINF.Offset=LINF.NextOffset;
	}while(LINF.NextOffset<TextSize&&LINF.NextOffset);
};
bool TextViewer_OnDestroy(SimpleDialog* SD){
	TextViewer* TV=(TextViewer*)SD;
	if(TV->TextPtr){
		free(TV->TextPtr);
		free(TV->LinePtrs);
		free(TV->LineSize);
		free(TV->LineWidth);
		free(TV->NeedFormat);
		free(TV->NSpaces);
		TV->TextPtr=NULL;
	};
	return true;
};
bool TextViewer_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	TextViewer* TV=(TextViewer*)SD;
	char c;
	if(!TV->TextPtr)return false;
	if(TV->Line>TV->NLines)TV->Line=TV->NLines-TV->PageSize+1;
	if(TV->Line<0)TV->Line=0;
	if(TV->VS){
		TV->VS->SMaxPos=(TV->NLines-TV->PageSize)*32;
		TV->VS->ScrDy=(TV->PageSize-1)*32;
		TV->VS->OnesDy=32;
		if(TV->VS->SMaxPos>0){
			if(TV->VS->SPos>TV->VS->SMaxPos)TV->VS->SPos=TV->VS->SMaxPos;
			TV->Line=(TV->VS->SPos+16)/32;
			TV->VS->Visible=1;
		}else{
			TV->VS->Visible=0;
		};
	};
	bool Cross=false;
	for(int i=0;i<TV->PageSize;i++){
		int lin=TV->Line+i;
		int yy=TV->y+TV->SymSize*i;
		if(lin<TV->NLines){
			char* off=TV->LinePtrs[lin];
			int x=0;
			int LS=TV->LineSize[lin];
			int LW=TV->LineWidth[lin];
			int NS=TV->NSpaces[lin];
			int addsp=0;
			if(NS>0)addsp=div(TV->Lx-LW,NS).quot;
			if(addsp>5)addsp=5;
			if(TV->NeedFormat[lin]){
				for(int j=0;j<LS;j++){
					c=off[j];
					if(c!='~'){
						int L=1;
						ShowCharUNICODE(TV->x+x,yy,(byte*)(off+j),TV->Font);
						int Lx=GetRLCWidthUNICODE(TV->Font->RLC,(byte*)(off+j),&L);
						j+=L-1;
						if(Cross){
							Hline(TV->x+x,yy+8,TV->x+x+Lx,0);
							Hline(TV->x+x,yy+9,TV->x+x+Lx,0);
						};
						x+=Lx;
						if(c==' ')x+=addsp;
					}else Cross=!Cross;
				};
			}else{
				for(int j=0;j<LS;j++){
					c=off[j];
					if(c!='~'){
						ShowCharUNICODE(TV->x+x,yy,(byte*)(off+j),TV->Font);
						int L=1;
						int Lx=GetRLCWidthUNICODE(TV->Font->RLC,(byte*)(off+j),&L);
						j+=L-1;
						if(Cross){
							Hline(TV->x+x,yy+8,TV->x+x+Lx,0);
							Hline(TV->x+x,yy+9,TV->x+x+Lx,0);
						};
						x+=Lx;
					}else Cross=!Cross;
				};
			};
		};
	};
	return true;
};
bool TextView_OnKeyDown(SimpleDialog* SD){
	TextViewer* TV=(TextViewer*)SD;
	if(!TV->TextPtr)return false;
	if(KeyPressed){
		switch(LastKey){
		case 38:
			TV->Line--;
			KeyPressed=false;
			break;
		case 40:
			TV->Line++;
			KeyPressed=false;
			break;
		case 33:
			TV->Line-=TV->PageSize-1;
			KeyPressed=false;
			break;
		case 34:
			TV->Line+=TV->PageSize-1;
			KeyPressed=false;
			break;
		};
		if(TV->Line>TV->NLines-TV->PageSize)TV->Line=TV->NLines-TV->PageSize;
		if(TV->Line<0)TV->Line=0;
		if(TV->VS&&TV->NLines>TV->PageSize){
			TV->VS->SPos=TV->Line*32;
		};
	};
	return false;
};
bool TextView_OnClick(SimpleDialog* SD){
	if(!SD->Active)SD->NeedToDraw=true;
	SD->Active=true;
	return true;
};
void TextViewer::LoadFile(char * TextFile){
	if(TextPtr){
		free(TextPtr);
		free(LinePtrs);
		free(LineSize);
		free(LineWidth);
		free(NeedFormat);
		free(NSpaces);
		TextPtr=NULL;
	};
	ResFile f1=RReset(TextFile);
	if(f1==INVALID_HANDLE_VALUE){
		TextPtr=NULL;
		NLines=0;
		return;
	};
	TextSize=RFileSize(f1);
	TextPtr=znew(char,TextSize+32);
	RBlockRead(f1,TextPtr,TextSize);
	RClose(f1);
	CreateLinesList();
	Line=0;
	if(VS)VS->SPos=0;
};
TextViewer* ParentFrame::addTextViewer(SimpleDialog* Parent,int x,int y,int Lx,int Ly,char* TextFile,RLCFont* Font){
	TextViewer* TV=new TextViewer;
	AddDialog(TV);
	TV->NeedToDraw=true;
	TV->Parent=Parent;
	TV->x=x+BaseX;
	TV->y=y+BaseY;
	TV->x1=TV->x+Lx-1;
	TV->y1=TV->y+Ly-1;
	TV->Lx=Lx;
	TV->OnDraw=&TextViewer_OnDraw;
	TV->Destroy=&TextViewer_OnDestroy;
	TV->Font=Font;
	TV->Line=0;
	TV->SymSize=GetRLCHeight(Font->RLC,'W');
	if(TV->SymSize==0)
		TV->SymSize=16;
	TV->SymSize+=UNI_LINEDLY2;
	TV->PageSize=div(Ly,TV->SymSize).quot;
	ResFile f1=RReset(TextFile);
	if(f1==INVALID_HANDLE_VALUE){
		TV->TextPtr=NULL;
		TV->NLines=0;
		return TV;
	};
	TV->TextSize=RFileSize(f1);
	TV->TextPtr=znew(char,TV->TextSize+32);
	TV->OnClick=&TextView_OnClick;
	RBlockRead(f1,TV->TextPtr,TV->TextSize);
	RClose(f1);
	TV->CreateLinesList();
	TV->OnKeyDown=&TextView_OnKeyDown;
	TV->Enabled=true;
	TV->Active=true;
	TV->VS=NULL;
	return TV;
}; 
CEXPORT char LASTCLICKCHATNAME[128]="";
CEXPORT FontParam FParam;
bool ChatViewer_OnDraw(SimpleDialog* SD){	
	if(!SD->Visible)return false;
	ChatViewer* CV=(ChatViewer*)SD;
	int NCHATS=*CV->NChats;
	int DYL=CV->ChatDY;
	int CLY=CV->OneLy;
	int NCL=CV->ScrNy;
	int x0=CV->x;
	int y0=CV->y;
	char* PNAME="";
	int NCLINES=0;
	char** ChatMess=*CV->Mess;
	char** ChatSender=*CV->Names;
	for(int i=DYL;i<DYL+NCL;i++){
		char cc3[256];
		if(i<NCHATS){
			if(ChatSender[i][0]){
				sprintf(cc3,"%s: ",ChatSender[i]);

				int Y0=y0+(i-DYL)*CLY;
				//int x1=x0+GetRLCStrWidth(cc3,&RedFont);
				//int y1=Y0+GetRLCHeight(RedFont.RLC,'y');
				int x1=x0+GetRLCStrWidth(cc3,CV->Font2);
				int y1;
				OneFontParam* FP=FParam.GetByGP(int(CV->Font2->RLC));
				if(FP){
					y1=Y0+FP->Bottom;
					Y0+=FP->Top;
				}else 
					y1=Y0+GetRLCHeight(CV->Font2->RLC,'y');
				if(mouseX>=x0&&mouseX<=x1&&mouseY>=Y0&&mouseY<y1){
					//if((GetTickCount()/500)&1)ShowString(x0,y0+(i-DYL)*CLY,cc3,&RedFont);
					//else ShowString(x0,y0+(i-DYL)*CLY,cc3,&BlackFont);
					if((GetTickCount()/500)&1)ShowString(x0,y0+(i-DYL)*CLY,cc3,CV->Font2);
					else ShowString(x0,y0+(i-DYL)*CLY,cc3,CV->Font1);
					if(Lpressed){
						strcpy(LASTCLICKCHATNAME,ChatSender[i]);
						char* ss=strstr(LASTCLICKCHATNAME,"-->");
						if(ss){
							strcpy(LASTCLICKCHATNAME,ss+3);
						};
						ss=strstr(LASTCLICKCHATNAME,"(private)");//GetTextByID("ONLYOU"));
						if(ss)ss[0]=0;
					};
				}else{
					//ShowString(x0,y0+(i-DYL)*CLY,cc3,&RedFont);
					ShowString(x0,y0+(i-DYL)*CLY,cc3,CV->Font2);
				};
				//ShowString(x1,y0+(i-DYL)*CLY,ChatMess[i],&BlackFont);
				ShowString(x1,y0+(i-DYL)*CLY,ChatMess[i],CV->Font1);
			}else{
				//ShowString(x0+32,y0+(i-DYL)*CLY,ChatMess[i],&BlackFont);
				ShowString(x0+32,y0+(i-DYL)*CLY,ChatMess[i],CV->Font1);
			};
		};
	};
	GPS.FlushBatches();
	return 0;
};
ChatViewer* ParentFrame::addChatViewer(SimpleDialog* Parent,int x,int y,int Ny,int OneLy,int OneLx,
										 char*** Mess,char*** Name,int* Nchats){
	ChatViewer* TV=new ChatViewer;
	AddDialog(TV);
	TV->NeedToDraw=true;
	TV->Parent=Parent;
	TV->x=x+BaseX;
	TV->y=y+BaseY;
	TV->x1=TV->x+OneLx-1;
	TV->y1=TV->y+OneLy*Ny-1;
	TV->MaxLx=OneLx;
	TV->OnDraw=&ChatViewer_OnDraw;
	TV->ChatDY=0;
	TV->ScrNy=Ny;
	TV->OneLy=OneLy;
	TV->Mess=Mess;
	TV->Names=Name;
	TV->NChats=Nchats;
	TV->Visible=1;
	// new 16.02
	TV->Font1=&BlackFont;
	TV->Font2=&RedFont;
	return TV;
};
//-----------------Bit Pictures Viewer-------------

void CopyTo16(int x,int y,byte* Src,int Pitch,int Lx,int Ly);
bool BPXView_OnDraw(SimpleDialog* SD){
	return true;
};
bool BPXView_OnKeyDown(SimpleDialog* SD){
	/*
	BPXView* BV=(BPXView*)SD;
	switch(LastKey){
	case 38:if(BV->PosY>0)BV->PosY--;
		break;
	case 40:BV->PosY++;
		break;
	case 33:
		if(BV->PosY>BV->Ny)BV->PosY-=BV->Ny;
		else BV->PosY=0;
		break;
	case 34:
		BV->PosY+=BV->Ny;
	};
	if(BV->VSC){
		BV->VSC->SPos=BV->PosY;
		BV->VSC->NeedToDraw=true;
	};
	KeyPressed=false;
	*/
	return false;
};
bool BPXView_OnMouseOver(SimpleDialog* SD){
	BPXView* BV=(BPXView*)SD;
	if(!BV->EnableSelection)return false;
	if(Lpressed||Rpressed){
		int bx=div(mouseX-BV->x,BV->OneLx).quot;
		int by=div(mouseY-BV->y,BV->OneLy).quot;
		BV->ChoosedPos=bx+(by+BV->PosY)*BV->Nx;
		if(!(GetKeyState(VK_CONTROL)&0x8000)){
			if(Lpressed){
				memset(BV->Choosed,0,BV->Nx*BV->RealNy);
				BV->Choosed[BV->ChoosedPos]=1;
			};
		}else{
			if(Lpressed){
				BV->Choosed[BV->ChoosedPos]++;
				BV->ChoosedPos=-1;
			}else{
				if(BV->Choosed[BV->ChoosedPos])
					BV->Choosed[BV->ChoosedPos]--;
				BV->ChoosedPos=-1;
			};
		};
		Lpressed=false;
		return true;
	};
	return false;
};
bool BPXView_OnDestroy(SimpleDialog* SD){
	BPXView* BV=(BPXView*)SD;
	free(BV->Choosed);
	return true;
};
BPXView* ParentFrame::addBPXView(SimpleDialog* Parent,int x,int y,int OneLx,int OneLy,int Nx,int Ny,int RealNy,byte* Ptr,VScrollBar* VSC){
	BPXView* BV=new BPXView;
	AddDialog(BV);
	BV->NeedToDraw=true;
	BV->Parent=Parent;
	BV->x=x+BaseX;
	BV->y=y+BaseY;
	BV->x1=BV->x+OneLx*Nx-1;
	BV->y1=BV->y+OneLy*Ny-1;
	BV->OnDraw=&BPXView_OnDraw;
	BV->OnKeyDown=&BPXView_OnKeyDown;
	BV->OnMouseOver=&BPXView_OnMouseOver;
	BV->Destroy=&BPXView_OnDestroy;
	BV->Enabled=true;
	BV->Active=Nx>1;
	BV->OneLx=OneLx;
	BV->OneLy=OneLy;
	BV->Nx=Nx;
	BV->Ny=Ny;
	BV->Choosed=znew(byte,Nx*RealNy);
	memset(BV->Choosed,0,Nx*RealNy);
	BV->Done=false;
	BV->RealNy=RealNy;
	BV->PosY=0;
	BV->ChoosedPos=-1;
	BV->Ptr=Ptr;
	BV->VSC=VSC;
	BV->EnableSelection=Nx>1;
	BV->DECLX=0;
	BV->Mode=0;
	if(VSC){
		VSC->SPos=0;
		VSC->SMaxPos=RealNy-Ny;
	};
	return BV;
};
//---------------end of BPXView--------------//
//-----------------RLCListBox----------------//
bool RLCListBox_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	RLCListBox* RB=(RLCListBox*)SD;

	Rct vp = GPS.GetClipArea();
	int WX	= vp.x;
	int WY	= vp.y;
	int WLX	= vp.w;
	int WLY	= vp.h;

	SetRLCWindow(RB->x,RB->y,RB->x1-RB->x+1,RB->y1-RB->y+1,ScrWidth);
	int curx=RB->x-RB->XPos;
	CBar(RB->x,RB->y,RB->x1-RB->x+1,RB->y1-RB->y+1,RB->BackColor);
	for(int i=0;i<RB->NItems;i++){
		int Lx=GPS.GetGPWidth(RB->GPIndex,i);
		if(curx<=RB->x1&&curx+Lx>=RB->x){
			//ShowRLCItem(curx,RB->y,&RB->Items,i,0);
			GPS_ShowGP(curx,RB->y,RB->GPIndex,i,0);
			if(RB->Choosed[i])
				Xbar(curx,RB->y,Lx-1,RB->y1-RB->y+1,RB->SelColor);
		};
		curx+=Lx;
	};
	//restoring the window
	SetRLCWindow(WX,WY,WLX,WLY,ScrWidth);
	return true;
};
bool RLCListBox_OnKeyDown(SimpleDialog* SD){
	RLCListBox* RB=(RLCListBox*)SD;
	return false;
};
bool RLCListBox_OnMouseOver(SimpleDialog* SD){
	RLCListBox* RB=(RLCListBox*)SD;
	if(!RB->NItems)return false;
	if(Lpressed){
		Lpressed=false;
		//let us determine position
		int pos=0;
		int mox=mouseX-RB->x+RB->XPos;
		int Lx;
		int i=0;
		do{
			Lx=GPS.GetGPWidth(RB->GPIndex,i);//GetRLCWidth(RB->Items,i);
			mox-=Lx;
			i++;
		}while(i<RB->NItems&&mox>0);
		if(mox<0){
			pos=i-1;
			//item is choosed;
			if(GetKeyState(VK_CONTROL)&0x8000){
				RB->Choosed[pos]^=1;			
			}else{
				memset(RB->Choosed,0,RB->NItems);
				RB->Choosed[pos]=1;			
				RB->Done=true;
			};
		};
	};
	return true;
};
bool RLCListBox_OnDestroy(SimpleDialog* SD){
	RLCListBox* RB=(RLCListBox*)SD;
	if(RB->Choosed)free(RB->Choosed);
	return true;
};
RLCListBox* ParentFrame::addRLCListBox(SimpleDialog* Parent,
							 int x,int y,int Lx,int Ly,
							 int GPIndex,byte BGColor,byte SelColor){
	RLCListBox* RB=new RLCListBox;
	AddDialog(RB);
	RB->NeedToDraw=true;
	RB->Parent=Parent;
	RB->x=x+BaseX;
	RB->y=y+BaseY;
	RB->x1=RB->x+Lx;
	RB->y1=RB->y+Ly;
	RB->OnDraw=&RLCListBox_OnDraw;
	RB->OnKeyDown=&RLCListBox_OnKeyDown;
	RB->OnMouseOver=&RLCListBox_OnMouseOver;
	RB->Destroy=&RLCListBox_OnDestroy;
	RB->Enabled=true;
	RB->Active=true;
	//RB->Items=Pictures;
	RB->GPIndex=GPIndex;
	RB->NItems=GPS.GPNFrames(GPIndex);//Pictures->SCount&65535;
	RB->SelColor=SelColor;
	RB->BackColor=BGColor;
	RB->Done=false;
	//MaxPos calculating
	int MAXX=0;
	for(int i=0;i<RB->NItems;i++){
		MAXX+=GPS.GetGPWidth(RB->GPIndex,i);
			//GetRLCWidth(Pictures,i);
	};
	RB->MaxXpos=MAXX-Lx;
	RB->Choosed=znew(byte,RB->NItems);
	memset(RB->Choosed,0,RB->NItems);
	RB->XPos=0;
	return RB; 
};
//--------------end of RLCListBox------------//
//-------------------ComboBox----------------//
void LimitString(char* str,lpRLCFont FONT,int L);
bool ComboBox_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	ComboBox* CB=(ComboBox*)SD;

	IndexedClassConnector* IC=CB->ConnectTo_index.Get();
	if(IC){
		Enumerator* E=IC->_get_enum();
		if(E){
			if(CB->NLines!=E->GetAmount()){
				CB->Clear();
				for(int i=0;i<E->GetAmount();i++){
					CB->AddLine(E->GetStr(i));
				}
			}
			IC->_exchange(CB->CurLine);
		}
	}

	if(!CB->NLines)return false;
	//
	int Ly=GPS.GetGPHeight(CB->GP_File,CB->UpPart)+1;
	CB->y1=CB->y+Ly-1;
	//
	CB->DLX=GPS.GetGPWidth(CB->GP_File,CB->UpPart)-CB->x1+CB->x-1;
	if(CB->CurLine>=CB->NLines)CB->CurLine=0;
	int center=0;
	int XI=(CB->x+CB->x1)>>1;
	if(CB->GP_File!=-1&&CB->BackColor)center=1;
	if(CB->GP_File!=-1){
		DWORD c=0xFFFFFFFF;
		if(!CB->Enabled){
			c=GPS.GetCurrentDiffuse();
			DWORD cc=ModDWORD(c,0x80FFFFFF);
			GPS.SetCurrentDiffuse(cc);
		}
		if(CB->DLX){
			TempWindow TW;
			PushWindow(&TW);
			IntersectWindows(0,0,XI,RealLy);
			GPS_ShowGP(CB->x,CB->y,CB->GP_File,CB->UpPart+1-((CB->MouseOver||CB->IsActive)&&CB->Enabled),0);
			PopWindow(&TW);
			IntersectWindows(XI,0,RealLx,RealLy);
			GPS_ShowGP(CB->x-CB->DLX,CB->y,CB->GP_File,CB->UpPart+1-((CB->MouseOver||CB->IsActive)&&CB->Enabled),0);
			PopWindow(&TW);
		}else GPS_ShowGP(CB->x,CB->y,CB->GP_File,CB->UpPart+1-((CB->MouseOver||CB->IsActive)&&CB->Enabled),0);
		GPS.SetCurrentDiffuse(c);
	};
	if(CB->IsActive&&CB->Enabled){
		//CBar(CB->x,CB->y,CB->x1-CB->x+1,CB->y1-CB->y+1,CB->BackColor);
		if(CB->rulermode){
			char ccc[32];
			sprintf(ccc,"%d->%d",CB->MinDeal+CB->CurLine,CB->MinDeal+CB->LightIndex);
			ShowStringEx(CB->x+CB->OneDx+center*((CB->x1-CB->x-GetRLen(ccc,CB->ActiveFont))>>1),CB->y+CB->OneDy,ccc,CB->ActiveFont);
		}else{
			char cc[512];
			strcpy(cc,CB->GetLineMessage(CB->CurLine));
			LimitString(cc,CB->PassiveFont,CB->x1-CB->x-CB->OneDx);
			ShowStringEx(CB->x+CB->OneDx+center*((CB->x1-CB->x-GetRLen(cc,CB->ActiveFont))>>1),CB->y+CB->OneDy,cc,CB->ActiveFont);
		}
	}else{
		//CBar(CB->x,CB->y,CB->x1-CB->x+1,CB->y1-CB->y+1,CB->BackColor);
		if(CB->rulermode){
			char ccc[32];
			sprintf(ccc,"%d",CB->MinDeal+CB->CurLine);
			ShowStringEx(CB->x+CB->OneDx+center*((CB->x1-CB->x-GetRLen(ccc,CB->PassiveFont))>>1),CB->y+CB->OneDy,ccc,CB->PassiveFont);
		}else{
			char cc[512];
			strcpy(cc,CB->GetLineMessage(CB->CurLine));
			LimitString(cc,CB->PassiveFont,CB->x1-CB->x-CB->OneDx);
			ShowStringEx(CB->x+CB->OneDx+center*((CB->x1-CB->x-GetRLen(cc,CB->PassiveFont))>>1),CB->y+CB->OneDy,cc,CB->PassiveFont);			
			const char* h=CB->GetLineHint(CB->CurLine);
			if(h){
				CB->Hint=(char*)h;
			}
		}
	};
	return true;
};
extern short randoma[8192];
#define PTPERDEAL 2
#define STARTOTS 12
#define RULCOLOR 71
bool ComboBox_OnDrawActiveRuler(SimpleDialog* SD){
	ComboBox* CB=(ComboBox*)SD;
	if(!CB->NLines)return false;
	if(CB->CurLine>=CB->NLines)CB->CurLine=0;
	int center=0;
	if(CB->GP_File!=-1&&CB->BackColor)center=1;
	int NeedPoints=CB->NLines*PTPERDEAL+STARTOTS*2;
	int NeedLines=(NeedPoints/CB->OneLy)+1;
	int realots=STARTOTS;
	int NLINES=NeedLines;
	if(CB->VS&&NLINES>CB->MaxLY)NLINES=CB->MaxLY;
	int DealsOnScreen=(NLINES*CB->OneLy+12+4)/PTPERDEAL;
	int MaxScroll=(CB->NLines-DealsOnScreen)+3*(realots/PTPERDEAL);
	int StartDeal=CB->MinDeal;
	if(CB->VS){
		StartDeal=CB->YPos+CB->MinDeal-20;
		extern short WheelDelta;
		if(WheelDelta){
			CB->VS->SPos+=WheelDelta/120;
			if(CB->VS->SPos<0)CB->VS->SPos=0;
			if(CB->VS->SPos>CB->VS->SMaxPos)CB->VS->SPos=CB->VS->SMaxPos;
			WheelDelta=0;
		}
	};
	int FinDeal=CB->MinDeal+CB->YPos+(NLINES*CB->OneLy)/PTPERDEAL+40;
	int MinDeal=CB->MinDeal;
	int MaxDeal=CB->MinDeal+CB->NLines;
	int XI=(CB->x+CB->x1)>>1;
	if(CB->IsActive){
		if(CB->DLX){
			TempWindow TW;
			PushWindow(&TW);
			IntersectWindows(0,0,XI,RealLy);
			GPS_ShowGP(CB->DropX,CB->DropY,CB->GP_File,CB->UpPart+2,0);
			PopWindow(&TW);
			IntersectWindows(XI,0,RealLx,RealLy);
			GPS_ShowGP(CB->DropX-CB->DLX,CB->DropY,CB->GP_File,CB->UpPart+2,0);
			PopWindow(&TW);
		}else GPS_ShowGP(CB->DropX,CB->DropY,CB->GP_File,CB->UpPart+2,0);
		int y0=CB->DropY+CB->UpLy;
		for(int i=0;i<NLINES;i++){
			if(CB->DLX){
				TempWindow TW;
				PushWindow(&TW);
				IntersectWindows(0,0,XI,RealLy);
				GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+(randoma[i*17]&1)+5,0);
				PopWindow(&TW);
				IntersectWindows(XI,0,RealLx,RealLy);
				GPS_ShowGP(CB->DropX-CB->DLX,y0,CB->GP_File,CB->UpPart+(randoma[i*17]&1)+5,0);
				PopWindow(&TW);
			}else GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+(randoma[i*17]&1)+5,0);
			y0+=CB->OneLy;
		};
		if(CB->DLX){
			TempWindow TW;
			PushWindow(&TW);
			IntersectWindows(0,0,XI,RealLy);
			GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+7,0);
			PopWindow(&TW);
			IntersectWindows(XI,0,RealLx,RealLy);
			GPS_ShowGP(CB->DropX-CB->DLX,y0,CB->GP_File,CB->UpPart+7,0);
			PopWindow(&TW);
		}else GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+7,0);
		TempWindow TW;
		PushWindow(&TW);
		IntersectWindows(CB->DropX,CB->DropY+CB->UpLy-4,CB->DropX+164,CB->DropY+CB->OneLy*CB->MaxLY+12);
		int x0=CB->DropX+16;
		char cc[128];
		int i;
		for(i=StartDeal;i<FinDeal;i++){
			if(i>=MinDeal&&i<=MaxDeal){
				int yy=(i-CB->YPos-CB->MinDeal)*PTPERDEAL+CB->DropY+CB->UpLy+realots;
				if((i%50)==0){
					xLine(x0,yy,x0+32,yy,RULCOLOR);
					sprintf(cc,"%d",i);
					ShowStringEx(x0+40,yy-10,cc,&YellowFont);
				}else
				if((i%10)==0){
					xLine(x0,yy,x0+24,yy,RULCOLOR);
					sprintf(cc,"%d",i);
					ShowStringEx(x0+40,yy-10,cc,&YellowFont);
				}else{
					xLine(x0,yy,x0+20,yy,RULCOLOR);
				};
				if(i==CB->LightIndex+CB->MinDeal){
					xLine(x0,yy,x0+32,yy,0xFF);
				};
			};
		};
		PopWindow(&TW);
		if(CB->VS&&NeedLines>CB->MaxLY){
			CB->VS->SMaxPos=MaxScroll;
			CB->VS->Visible=1;
			CB->VS->Enabled=1;
			CB->VS->OnDraw(CB->VS);
			CB->VS->Visible=0;
			CB->VS->Enabled=0;
			CB->YPos=CB->VS->SPos;
			if(CB->YPos>CB->VS->SMaxPos)CB->YPos=CB->VS->SMaxPos;
		};
	};
	return true;
};
bool ComboBox_MouseOverActiveRuler(SimpleDialog* SD,int x,int y,int ID){
	if(!SD->Enabled)return false;
	ComboBox* CB=(ComboBox*)SD;
	if(CB->CurLine>=CB->NLines)CB->CurLine=0;
	//---------------
	int NeedPoints=CB->NLines*PTPERDEAL+STARTOTS*2;
	int NeedLines=(NeedPoints/CB->OneLy)+1;
	int realots=STARTOTS;
	int NLINES=NeedLines;
	if(CB->VS&&NLINES>CB->MaxLY)NLINES=CB->MaxLY;
	int DealsOnScreen=(NLINES*CB->OneLy+12+4)/PTPERDEAL;
	int MaxScroll=(CB->NLines-DealsOnScreen)+3*(realots/PTPERDEAL);
	int StartDeal=CB->MinDeal;
	if(CB->VS){
		StartDeal=CB->YPos+CB->MinDeal-20;
	};
	int FinDeal=CB->MinDeal+CB->YPos+(NLINES*CB->OneLy)/PTPERDEAL+40;
	int MinDeal=CB->MinDeal;
	int MaxDeal=CB->MinDeal+CB->NLines;
	//---------------
	
	if(!CB->NLines)return false;
	if(y<CB->OneDy)return false;
	bool DOIT=1;
	if(CB->VS&&CB->NLines>CB->MaxLY){
		if(mouseX>=CB->VS->x&&mouseY>=CB->VS->y&&mouseX<CB->VS->x1&&mouseY<=CB->VS->y1){
			//CB->VS->OnMouseOver(CB->VS);
			CB->YPos=CB->VS->SPos;
			if(CB->YPos>CB->VS->SMaxPos)CB->YPos=CB->VS->SMaxPos;
			DOIT=0;
		};
	};
	if(DOIT){
		int y0=CB->UpLy-4;
		int y00=CB->UpLy+realots;

		if(y>=y0)CB->LightIndex=(y-y00)/PTPERDEAL;
		if(CB->VS)CB->LightIndex+=CB->YPos;
		if(CB->LightIndex>CB->NLines)CB->LightIndex=CB->NLines;
		if(CB->LightIndex<0)CB->LightIndex=0;
		if(Lpressed){
			if(DOIT){
				CB->CurLine=CB->LightIndex;
				CB->Changed=1;
				CB->NeedRedrawAll=true;
				CB->IsActive=false;
				ParentFrame* DIS=CB->ParentDS;
				DIS->ActiveParent=NULL;
				Lpressed=false;
				realLpressed=0;
				UnPress();
			};
		};
	};
	return true;
};
bool ComboBox_OnDrawActive(SimpleDialog* SD){
	ComboBox* CB=(ComboBox*)SD;
	if(CB->rulermode)return ComboBox_OnDrawActiveRuler(SD);
	if(!CB->NLines)return false;
	if(CB->CurLine>=CB->NLines)CB->CurLine=0;
	int center=0;
	if(CB->GP_File!=-1&&CB->BackColor)center=1;
	int FL=0;
	int LL=CB->NLines;
	int ADLX=CB->DLX;// new 4.02
	if(CB->VS){
		if(CB->NLines>CB->MaxLY)ADLX+=CB->VS->LineLx;
		FL=CB->YPos;
		LL=FL+CB->MaxLY;
		if(LL>CB->NLines)LL=CB->NLines;
		extern short WheelDelta;
		if(WheelDelta){
			CB->VS->SPos-=(WheelDelta/120)*10;
			if(CB->VS->SPos<0)CB->VS->SPos=0;
			if(CB->VS->SPos>CB->VS->SMaxPos)CB->VS->SPos=CB->VS->SMaxPos;
			WheelDelta=0;
		}
	};
	int XI=(CB->x+CB->x1)>>1;
	if(CB->IsActive){
		if(CB->GP_File==-1){
			CBar(CB->DropX,CB->DropY,CB->OneLx,CB->OneLy*CB->NLines+CB->OneDy*2,CB->BackColor);
			Xbar(CB->DropX,CB->DropY,CB->OneLx-1,CB->OneLy*CB->NLines+CB->OneDy*2-1,CB->BorderColor);
		}else{
			if(CB->DLX){
				TempWindow TW;
				PushWindow(&TW);
				IntersectWindows(0,0,XI,RealLy);
				GPS_ShowGP(CB->DropX,CB->DropY,CB->GP_File,CB->UpPart+2,0);
				PopWindow(&TW);
				IntersectWindows(XI,0,RealLx,RealLy);
				GPS_ShowGP(CB->DropX-CB->DLX,CB->DropY,CB->GP_File,CB->UpPart+2,0);
				PopWindow(&TW);
			}else GPS_ShowGP(CB->DropX,CB->DropY,CB->GP_File,CB->UpPart+2,0);
			int y0=CB->DropY+CB->UpLy;
			for(int i=FL;i<LL;i++){
				if(CB->DLX){
					TempWindow TW;
					PushWindow(&TW);
					IntersectWindows(0,0,XI,RealLy);
					GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+(randoma[i*17]&1)+5,0);
					PopWindow(&TW);
					IntersectWindows(XI,0,RealLx,RealLy);
					GPS_ShowGP(CB->DropX-CB->DLX,y0,CB->GP_File,CB->UpPart+(randoma[i*17]&1)+5,0);
					PopWindow(&TW);
				}else GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+(randoma[i*17]&1)+5,0);
				y0+=CB->OneLy;
			};
			if(CB->DLX){
				TempWindow TW;
				PushWindow(&TW);
				IntersectWindows(0,0,XI,RealLy);
				GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+7,0);
				PopWindow(&TW);
				IntersectWindows(XI,0,RealLx,RealLy);
				GPS_ShowGP(CB->DropX-CB->DLX,y0,CB->GP_File,CB->UpPart+7,0);
				PopWindow(&TW);
			}else GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+7,0);
		};
		int y0=CB->DropY+CB->UpLy;
		for(int i=FL;i<LL;i++){
			char cc[512];
			strcpy(cc,CB->GetLineMessage(i));
			LimitString(cc,CB->PassiveFont,CB->x1-CB->x-CB->FontDx);
			if(i==CB->LightIndex){
				if(CB->GP_File!=-1){
					if(ADLX){//CB->DLX){
						TempWindow TW;
						PushWindow(&TW);
						IntersectWindows(0,0,XI,RealLy);
						GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+(randoma[i*17]&1)+3,0);
						PopWindow(&TW);
						IntersectWindows(XI,0,RealLx,RealLy);
						GPS_ShowGP(CB->DropX-ADLX,y0,CB->GP_File,CB->UpPart+(randoma[i*17]&1)+3,0);
						PopWindow(&TW);
					}else GPS_ShowGP(CB->DropX,y0,CB->GP_File,CB->UpPart+(randoma[i*17]&1)+3,0);
				};
				ShowStringEx(CB->x+CB->FontDx+center*((CB->x1-CB->x-GetRLen(cc,CB->ActiveFont))>>1),CB->DropY+CB->FontDy+(i-FL)*CB->OneLy+CB->UpLy,cc,CB->ActiveFont);
			}else ShowStringEx(CB->x+CB->FontDx+center*((CB->x1-CB->x-GetRLen(cc,CB->PassiveFont))>>1),CB->DropY+CB->FontDy+(i-FL)*CB->OneLy+CB->UpLy,cc,CB->PassiveFont);
			y0+=CB->OneLy;
		};
		if(CB->VS&&CB->NLines>CB->MaxLY){
			CB->VS->Visible=1;
			CB->VS->Enabled=1;
			int Ly=CB->VS->y1-CB->VS->y;
			CB->VS->y=CB->DropY+2;
			CB->VS->y1=CB->VS->y+Ly;
			CB->VS->OnDraw(CB->VS);
			CB->VS->Visible=0;
			CB->VS->Enabled=0;
			CB->YPos=(CB->VS->SPos+5)/10;
			if(CB->YPos+CB->MaxLY>CB->NLines)CB->YPos=CB->NLines-CB->MaxLY;
			if(CB->VS->MouseOver){
				UnPress();
				Lpressed=0;
			}
		};
	};
	return true;
};
void DeactivateDSS(ClassArray<SimpleDialog>* DSS);
bool ComboBox_OnMouseOver(SimpleDialog* SD){
	if(!(SD->Enabled&&SD->Visible))return false;
	ComboBox* CB=(ComboBox*)SD;
	if(CB->CurLine>=CB->NLines)CB->CurLine=0;
	if(Lpressed&&CB->NLines){
		//new 29.03
		if(CB->ParentDS)DeactivateDSS(&SD->ParentDS->DSS);
		CB->Active=true;
		//end new
		if(CB->IsActive){
			CB->IsActive=false;
			CB->NeedRedrawAll=true;
			ParentFrame* DIS=CB->ParentDS;
			DIS->ActiveParent=NULL;
		}else{
			if(CB->NLines){
				CB->IsActive=true;
				CB->DropX=CB->x+CB->Center;
				CB->DropY=CB->y1;
				CB->LightIndex=CB->CurLine;
				//if(CB->DropY+CB->NLines*CB->OneLy+CB->OneDy*2>740)
				//	CB->DropY-=CB->NLines*CB->OneLy+CB->OneDy*2-CB->y+CB->y1+6;
				ParentFrame* DIS=CB->ParentDS;
				DIS->ActiveParent=SD;
				DIS->ActiveX=CB->DropX;
				DIS->ActiveY=CB->DropY;
				DIS->ActiveX1=CB->x1;
				int L=CB->MaxLY?CB->MaxLY:100000;
				DIS->ActiveY1=CB->DropY+CB->OneLy*min(CB->NLines,L)+CB->FontDy+CB->UpLy;
				DIS->ActiveParentX=CB->x;
				DIS->ActiveParentY=CB->y;				
                
				if(DIS->ActiveY1>RealLy){
					int dy=DIS->ActiveY1-DIS->ActiveY+CB->GetHeight()+3;
					DIS->ActiveY-=dy;
					DIS->ActiveY1-=dy;
					CB->DropY-=dy;										
				}				

				if(CB->VS){
					//DIS->ActiveX1=CB->VS->x1-1;
					//DIS->ActiveY1=CB->VS->y1;
				};
			};
		};
		Lpressed=false;
		UnPress();
	};
	return true;
};

bool ComboBox_MouseOverActiveZone(SimpleDialog* SD,int x,int y,int ID){
	if(!SD->Enabled)return false;
	ComboBox* CB=(ComboBox*)SD;
	if(CB->CurLine>=CB->NLines)CB->CurLine=0;
	if(CB->rulermode)return ComboBox_MouseOverActiveRuler(SD,x,y,ID);
	if(CB->VS){
		CB->VS->ShiftDialog(ParentFrame::ActiveParentX-CB->x,ParentFrame::ActiveParentY-CB->y);
		if(CB->NLines>CB->MaxLY){
			CB->VS->SMaxPos=(CB->NLines-CB->MaxLY)*10;
			if(CB->VS->SPos>CB->VS->SMaxPos)CB->VS->SPos=CB->VS->SMaxPos;
			CB->YPos=(CB->VS->SPos+5)/10;
		};
		CB->VS->ShiftDialog(-ParentFrame::ActiveParentX+CB->x,-ParentFrame::ActiveParentY+CB->y);
	};
	
	if(!CB->NLines)return false;
	if(y<CB->OneDy)return false;
	bool DOIT=1;
	if(CB->VS&&CB->NLines>CB->MaxLY){
		CB->VS->ShiftDialog(ParentFrame::ActiveParentX-CB->x,ParentFrame::ActiveParentY-CB->y);
		if(mouseX>=CB->VS->x&&mouseY>=CB->VS->y&&mouseX<CB->VS->x1&&mouseY<=CB->VS->y1){
			//CB->VS->OnMouseOver(CB->VS);
			CB->YPos=(CB->VS->SPos+5)/10;
			if(CB->YPos+CB->MaxLY>CB->NLines)CB->YPos=CB->NLines-CB->MaxLY;
			DOIT=0;
		};
		CB->VS->ShiftDialog(-ParentFrame::ActiveParentX+CB->x,-ParentFrame::ActiveParentY+CB->y);
	};
	if(DOIT){
		int y0=CB->UpLy;//CB->OneDy;//+CB->FontDy+CB->UpLy;
		if(y>=y0)CB->LightIndex=div(y-y0,CB->OneLy).quot;
		if(CB->VS)CB->LightIndex+=CB->YPos;
		if(CB->LightIndex>=CB->NLines)CB->LightIndex=CB->NLines-1;
		if(Lpressed){
			if(DOIT){
				CB->CurLine=CB->LightIndex;
				CB->Changed=1;
				if(CB->CurLine<0)CB->CurLine=0;
				if(CB->CurLine>=CB->NLines&&CB->NLines)CB->CurLine=CB->NLines-1;
				CB->NeedRedrawAll=true;
				CB->IsActive=false;
				ParentFrame* DIS=CB->ParentDS;
				DIS->ActiveParent=NULL;
				Lpressed=false;
				void UnPress();
				UnPress();
				realLpressed=0;
				ClassEditorProcessor::ClearPress();
				if(CB->OnSelect)CB->OnSelect(CB);
			};
		};
	};
	return true;
};
bool ComboBox_OnDestroy(SimpleDialog* SD){
	ComboBox* CB=(ComboBox*)SD;
	CB->Clear();
	return true;
};
extern int ScrollFileID;
void ComboBox::AssignScroll(ParentFrame* DSS,VScrollBar** SCR,int GPFile,int Sprite,int MaxLy){
	if(ScrollFileID==-1) ScrollFileID=GPS.PreLoadGPImage("Interf3\\elements\\scroll");
	GPFile=ScrollFileID;
	Sprite=0;
	int LL=3;
	*SCR=DSS->addNewGP_VScrollBar(NULL,x1-DSS->BaseX-GPS.GetGPWidth(GPFile,Sprite),y1+UpLy-DSS->BaseY-LL,OneLy*MaxLy+LL*2,1,0,GPFile,Sprite);
	VS=*SCR;
	VS->Visible=0;
	VS->Enabled=0;
	VS->SPos=0;
	VS->SMaxPos=1;
	YPos=0;
	MaxLY=MaxLy;
};
void ComboBox::CreateRuler(int Min,int NDeals){
	Clear();
	NLines=NDeals;
	MinDeal=Min;
	rulermode=1;
}; 
ComboBox* ParentFrame::addComboBox(SimpleDialog* Parent,int x,int y,int Lx,int Ly,int LineLy,
											byte BackColor,byte BorderColor,
											RLCFont* ActiveFont,RLCFont* PassiveFont,
											char* Contence){
	ComboBox* CB=new ComboBox;
	AddDialog(CB);
	CB->NeedToDraw=true;
	CB->Parent=Parent;
	CB->x=x+BaseX;
	CB->y=y+BaseY;
	CB->x1=CB->x+Lx-1;
	CB->y1=CB->y+Ly-1;
	CB->ActiveFont=ActiveFont;
	CB->PassiveFont=PassiveFont;
	CB->Enabled=true;
	CB->BackColor=BackColor;
	CB->BorderColor=BorderColor;
	CB->CurLine=0;
	CB->FontDx=2;
	CB->FontDy=0;
	CB->OneDx=1;
	CB->OneDy=1;
	CB->OneLx=Lx;
	CB->OneLy=LineLy;
	CB->NLines=0;
	CB->Lines=NULL;
	CB->UpLy=0;
	CB->LightIndex=-1;
	CB->UpLy=0;
	CB->OnDraw=ComboBox_OnDraw;
	CB->OnDrawActive=ComboBox_OnDrawActive;
	CB->Destroy=ComboBox_OnDestroy;
	CB->OnMouseOver=ComboBox_OnMouseOver;
	CB->MouseOverActiveZone=ComboBox_MouseOverActiveZone;
	CB->GP_File=-1;
	CB->ParentDS=this;
	CB->Enabled=true;
	CB->Center=0;
	if(Contence)CB->AddComplexLine(Contence);
	CB->rulermode=0;
	CB->VS=NULL;
	//assert(_CrtCheckMemory());
	return CB;
};
int ComboFileID=-1;
ComboBox* ParentFrame::addGP_ComboBox(SimpleDialog* Parent,int x,int y,int GP_File,
											int UpPart,int Center,int DownPart,
											RLCFont* ActiveFont,RLCFont* PassiveFont,
											char* Contence){
	ComboBox* CB=new ComboBox;
	AddDialog(CB);
	CB->NeedToDraw=true;
	CB->Parent=Parent;
	CB->x=x+BaseX;
	CB->y=y+BaseY;
	CB->OneLx=GPS.GetGPWidth(GP_File,UpPart);
	CB->OneLy=GPS.GetGPHeight(GP_File,UpPart+5);
	int Lx=CB->OneLx;
	int Ly=GPS.GetGPHeight(GP_File,UpPart)+1;
	CB->x1=CB->x+Lx-1;
	CB->y1=CB->y+Ly-1;
	CB->ActiveFont=ActiveFont;
	CB->PassiveFont=PassiveFont;
	CB->Enabled=true;
	CB->BackColor=0;
	CB->BorderColor=0;
	CB->CurLine=0;
	CB->FontDx=25;
	CB->FontDy=2;
	CB->OneDx=24;//0;
	CB->OneDy=3;//GPS.GetGPHeight(GP_File,UpPart+2);
	if(ComboFileID==-1) ComboFileID=GPS.PreLoadGPImage("Interf3\\elements\\combo");
	CB->UpLy=GPS.GetGPHeight(CB->GP_File,CB->UpPart+2);
	CB->GP_File=ComboFileID;
	CB->UpPart=0;
	CB->Center=9;
	CB->DownPart=0;
	//CB->GP_File=GP_File;
	//CB->UpPart=UpPart;
	//CB->Center=Center;
	//CB->DownPart=DownPart;
	CB->NLines=0;
	CB->Lines=NULL;
	CB->LightIndex=-1;
	CB->OnDraw=ComboBox_OnDraw;
	CB->OnDrawActive=ComboBox_OnDrawActive;
	CB->Destroy=ComboBox_OnDestroy;
	CB->OnMouseOver=ComboBox_OnMouseOver;
	CB->MouseOverActiveZone=ComboBox_MouseOverActiveZone;
	CB->ParentDS=this;
	CB->Enabled=true;
	CB->VS=NULL;
	CB->DLX=0;
	if(Contence)CB->AddComplexLine(Contence);
	CB->rulermode=0;
	//assert(_CrtCheckMemory());
	return CB;
};
ComboBox* ParentFrame::addGP_ComboBoxDLX(SimpleDialog* Parent,int x,int y,int LX,int GP_File,
											int UpPart,int Center,int DownPart,
											RLCFont* ActiveFont,RLCFont* PassiveFont,
											char* Contence){
	ComboBox* CB=new ComboBox;
	if(LX>340)LX=340;
	AddDialog(CB);
	if(ReadStdElement(CB)){
		int LY=CB->y1-CB->y+1;
        CB->x=BaseX+x;
		CB->y=BaseY+y;
		CB->x1=CB->x+LX-1;
		CB->y1=CB->y+LY-1;
	}else{
		CB->NeedToDraw=true;
		CB->Parent=Parent;
		CB->x=x+BaseX;
		CB->y=y+BaseY;
		CB->OneLx=GPS.GetGPWidth(GP_File,UpPart);
		CB->DLX=CB->OneLx-LX;
		CB->OneLx=LX;
		CB->OneLy=GPS.GetGPHeight(GP_File,UpPart+5);
		int Lx=CB->OneLx;
		int Ly=GPS.GetGPHeight(GP_File,UpPart)+1;
		CB->x1=CB->x+Lx-1;
		CB->y1=CB->y+Ly-1;
		CB->ActiveFont=ActiveFont;
		CB->PassiveFont=PassiveFont;
		//CB->ActiveFont=&BlackFont;
		//CB->PassiveFont=&BlackFont;
		CB->Enabled=true;
		CB->BackColor=0;
		CB->BorderColor=0;
		CB->CurLine=0;
		CB->FontDx=31;
		CB->FontDy=3;
		CB->OneDx=31;
		CB->OneDy=6;
		//CB->FontDx=25;
		//CB->FontDy=2;
		//CB->OneDx=24;
		//CB->OneDy=3;
		if(ComboFileID==-1) ComboFileID=GPS.PreLoadGPImage("Interf3\\elements\\combo");
		if(GP_File!=-1)
			CB->GP_File=GP_File;
		else
			CB->GP_File=ComboFileID;
		CB->UpPart=0;
		CB->Center=9;
		CB->DownPart=0;
		CB->UpLy=GPS.GetGPHeight(CB->GP_File,CB->UpPart+2);
		//CB->GP_File=GP_File;
		//CB->UpPart=UpPart;
		//CB->Center=Center;
		//CB->DownPart=DownPart;
		//CB->NLines=0;
		//CB->Lines=NULL;
		CB->LightIndex=-1;
		CB->OnDraw=ComboBox_OnDraw;
		CB->OnDrawActive=ComboBox_OnDrawActive;
		CB->Destroy=ComboBox_OnDestroy;
		CB->OnMouseOver=ComboBox_OnMouseOver;
		CB->MouseOverActiveZone=ComboBox_MouseOverActiveZone;
		CB->ParentDS=this;
		CB->Enabled=true;
		CB->VS=NULL;
		if(Contence)CB->AddComplexLine(Contence);
		CB->rulermode=0;
		//assert(_CrtCheckMemory());
	}
	return CB;
};
ComboBox::~ComboBox(){
    Clear();
}
ComboBox::ComboBox(){
	UpPart=0;
	Center=9;
	DownPart=0;
	GP_File=GPS.PreLoadGPImage("Interf3\\InGame\\_common_droplist");
	x=0;y=0;x1=200;
	OneLx=x1-x+1;
	DLX=GPS.GetGPWidth(GP_File,UpPart)-OneLx;
	OneLy=GPS.GetGPHeight(GP_File,UpPart+5);	
	int Ly=GPS.GetGPHeight(GP_File,UpPart)+1;
	y1=y+Ly-1;
	ActiveFont=&BlackFont;
	PassiveFont=&BlackFont;
	Enabled=true;
	BackColor=0;
	BorderColor=0;
	CurLine=0;
	FontDx=25;
	FontDy=2;
	OneDx=24;
	OneDy=3;
	UpLy=GPS.GetGPHeight(GP_File,UpPart+2);	
	NLines=0;
	Lines=NULL;
	LightIndex=-1;
	OnDraw=&ComboBox_OnDraw;
	OnDrawActive=&ComboBox_OnDrawActive;
	Destroy=&ComboBox_OnDestroy;
	OnMouseOver=&ComboBox_OnMouseOver;
	MouseOverActiveZone=&ComboBox_MouseOverActiveZone;	
	Enabled=true;
	VS=NULL;	
	rulermode=0;	
	AddComplexLine("Line1|Line2|Line3|Line4|Line5|Line6|Line7|Line8|Line9|Line10|Line11");
	OnSelect=NULL;
	TestMode=1;
	AutoID=NULL;
}
const char* ComboBox::GetLineMessage(int idx){
	if(AutoID){
		char temp[1024];
		if(!strcmp(AutoID,"@CGRPID")){
			sprintf(temp,"@CGRPID::%s",Lines[idx]);
		}else
		if(!strcmp(AutoID,"@CLASSID")){
			sprintf(temp,"@CLASSID::%s",Lines[idx]);
		}else{
			sprintf(temp,"@ENUMID::%s::%s",AutoID,Lines[idx]);
		}
		char* s=GetTextByID(temp);
		if(s!=temp)return s;		
	}
	return idx<NLines?Lines[idx]:"";
}
const char* ComboBox::GetLineHint(int idx){
	if(AutoID){
		char temp[1024];
		if(!strcmp(AutoID,"@CGRPID")){
			sprintf(temp,"@CGRPHINT::%s",Lines[idx]);
		}else
		if(!strcmp(AutoID,"@CLASSID")){
			sprintf(temp,"@CLASSHINT::%s",Lines[idx]);
		}else{
			sprintf(temp,"@ENUMHINT::%s::%s",AutoID,Lines[idx]);
		}
		sprintf(temp,"@ENUMHINT::%s::%s",AutoID,Lines[idx]);
		char* s=GetTextByID(temp);
		if(s!=temp)return s;		
		bool GetHintDebugMode();
		if(GetHintDebugMode()){
			if(!strcmp(AutoID,"@CGRPID")){
				sprintf(temp,"@CGRPID::%s\\@CGRPHINT::%s",Lines[idx],Lines[idx]);
			}else
			if(!strcmp(AutoID,"@CLASSID")){
				sprintf(temp,"@CLASSID::%s\\@CLASSHINT::%s",Lines[idx],Lines[idx]);
			}else{
				sprintf(temp,"@ENUMID::%s::%s\\@ENUMHINT::%s::%s",AutoID,Lines[idx],AutoID,Lines[idx]);
			}			
			if(Hint&&AllocHint)free(Hint);
			Hint=znew(char,strlen(temp)+1);
			strcpy(Hint,temp);
			AllocHint=1;
		}
	}
	return NULL;
}
void ComboBox::Clear(){
	if(Lines){
		for(int i=0;i<NLines;i++)free(Lines[i]);
		free(Lines);
		Lines=NULL;
		NLines=0;
		CurLine=0;
	};
	rulermode=0;
};
void ComboBox::AddLine(char* Text){
	if(!Text)Text="";
	if(TestMode){
		TestMode=0;
		Clear();
	}
	if(Lines){
        int NC=4+((NLines)&0xFFFFFFFC);
        int NN=4+((NLines+1)&0xFFFFFFFC);
        if(NC!=NN)Lines=(char**)realloc(Lines,NN<<2);
		Lines[NLines]=znew(char,strlen(Text)+1);
		strcpy(Lines[NLines],Text);
		NLines++;
	}else{
		Lines=(char**)malloc(4*4);
		Lines[0]=znew(char,strlen(Text)+1);
		strcpy(Lines[0],Text);
		NLines++;
	};
};
void ComboBox::AddComplexLine(char* Text){
	if(TestMode){
		TestMode=0;
		Clear();
	}
	char* Tmp=znew(char,strlen(Text)+1);
	strcpy(Tmp,Text);
	int pos=0;
	int Nextpos=0;
	bool All=false;
	do{
		while(Tmp[Nextpos]!='|'&&Tmp[Nextpos]!=0)Nextpos++;
		if(Tmp[Nextpos])Tmp[Nextpos]=0;
		else All=true;
		AddLine(Tmp+pos);
		pos=Nextpos+1;
		Nextpos=pos;
	}while(!All);
	free(Tmp);
};
//---------------end of ComboBox-------------//
//-----------------WinComboBox---------------//
bool WinComboBox_OnDraw(SimpleDialog* SD){
	WinComboBox* CB=(WinComboBox*)SD;
	if(!CB->Visible)return false;
	if(!CB->NLines)return false;
	bool CanChoose=GetTickCount()-CB->LastChooseTime>500;
	if(!CB->CurBox){
		int MouseOvList=mouseX>=CB->ListX0&&mouseX<=CB->ListX1&&mouseY>=CB->ListY0&&mouseY<=CB->ListY1;
		if(!CB->Enabled)MouseOvList=false;
		int Cur=-1;
		int Ly=CB->y1-CB->y;
		int i;
		if(CB->Enabled){
			if(CanChoose||MouseOvList){
				for(i=0;i<CB->NBox;i++){
					if(CB->Group[i]->MouseOver)Cur=i;
				};
			}else{
				for(i=0;i<CB->NBox;i++){
					if(CB->Group[i]->ActiveLine!=-1)
						Cur=i;
				};
			};
		};
		if(Cur==-1){
			if(!MouseOvList){
				if(CanChoose)for(i=0;i<CB->NBox;i++)CB->Group[i]->ActiveLine=-1;
			}else{
				for(i=0;i<CB->NBox;i++)if(CB->Group[i]->ActiveLine!=-1)Cur=i;
				for(i=0;i<CB->NBox;i++)CB->Group[i]->ActiveLine=-1;
				if(Cur!=-1){
					int yy=(mouseY-CB->y)/Ly;
					CB->Group[Cur]->ActiveLine=yy;
					if(Lpressed&&yy<CB->Group[Cur]->NLines){
						CB->Group[Cur]->CurLine=yy;
						CB->Group[Cur]->ActiveLine=-1;
						CB->Changed=1;
					};
				};
			};
		}else{
			if(CanChoose){
				for(int i=0;i<CB->NBox;i++)CB->Group[i]->ActiveLine=-1;
				CB->Group[Cur]->ActiveLine=CB->Group[Cur]->CurLine;
				CB->LastChooseTime=GetTickCount();
			};
		};
		for(i=0;i<CB->NBox;i++){
			WinComboBox* WCB=CB->Group[i];
			if(WCB->MouseOver){
				ShowString(WCB->x,WCB->y,WCB->Message,WCB->ActiveFont);
				if(Cur==-1){
					ShowString(WCB->ListX0,WCB->y,WCB->Lines[WCB->CurLine],WCB->ActiveFont);
				};
			}else{
				if(WCB->ActiveLine!=-1&&MouseOvList)ShowString(WCB->x,WCB->y,WCB->Message,WCB->ActiveFont);
				else ShowString(WCB->x,WCB->y,WCB->Message,WCB->PassiveFont);
				if(Cur==-1){
					ShowString(WCB->ListX0,WCB->y,WCB->Lines[WCB->CurLine],WCB->ActiveFont);
				};
			};
			if(Cur==i){
				for(int j=0;j<WCB->NLines;j++){
					if(j==WCB->ActiveLine){
						ShowString(WCB->ListX0,WCB->ListY0+j*Ly,WCB->Lines[j],WCB->ActiveFont);
					}else{
						ShowString(WCB->ListX0,WCB->ListY0+j*Ly,WCB->Lines[j],WCB->PassiveFont);
					};
				};
			};
		};
	};
	return false;
};
bool WinComboBox_OnDestroy(SimpleDialog* SD){
	WinComboBox* CB=(WinComboBox*)SD;
	CB->Clear();
	return true;
};
WinComboBox* ParentFrame::addWinComboBox(SimpleDialog* Parent,char* Message,int x,int y,int Lx,int Ly,
								int ListX,int ListY,int ListLx,int ListLy,
								RLCFont* ActiveFont,RLCFont* PassiveFont,
								WinComboBox** Group,int NInGroup,int CurBox){
	WinComboBox* CB=new WinComboBox;
	AddDialog(CB);
	CB->NeedToDraw=true;
	CB->Parent=Parent;
	CB->x=x+BaseX;
	CB->y=y+BaseY;
	CB->x1=CB->x+Lx-1;
	CB->y1=CB->y+Ly-1;
	CB->ListX0=ListX;
	CB->ListY0=ListY;
	CB->ListX1=ListX+ListLx-1;
	CB->ListY1=ListY+ListLy-1;
	CB->ActiveFont=ActiveFont;
	CB->PassiveFont=PassiveFont;
	CB->Group=Group;
	CB->NBox=NInGroup;
	CB->CurLine=0;
	CB->ActiveLine=-1;
	CB->Lines=NULL;
	CB->NLines=0;
	CB->Message=Message;
	CB->CurBox=CurBox;
	CB->OnDraw=&WinComboBox_OnDraw;
	CB->Destroy=&WinComboBox_OnDestroy;
	CB->Active=false;
	CB->Enabled=true;
	CB->Visible=true;
	CB->LastChooseTime=0;
	return CB;
};
void WinComboBox::AddLine(char* Text){
	if(Lines){
		Lines=(char**)realloc(Lines,(NLines+1)<<2);
		Lines[NLines]=znew(char,strlen(Text)+1);
		strcpy(Lines[NLines],Text);
		NLines++;
	}else{
		Lines=(char**)malloc(4);
		Lines[0]=znew(char,strlen(Text)+1);
		strcpy(Lines[0],Text);
		NLines++;
	};
};
void WinComboBox::Clear(){
	if(Lines){
		for(int i=0;i<NLines;i++)free(Lines[i]);
		free(Lines);
		Lines=NULL;
		NLines=0;
		CurLine=0;
	};
};
//--------------end of WinComboBox-----------//
//-----------------PageControl---------------//
bool GP_PageControl_OnDraw(SimpleDialog* SD){
	GP_PageControl* PC=(GP_PageControl*)SD;
	if(PC->MouseOver&&Lpressed){
		for(int i=0;i<PC->NPages;i++){
			if(mouseX>=PC->Pages[i].x&&mouseX<=PC->Pages[i].x1&&mouseY>=PC->Pages[i].y&&mouseY<=PC->Pages[i].y1){
				PC->CurPage=i;
			};
		};
	};
	GPS_ShowGP(PC->x,PC->y,PC->GP_File,PC->Pages[PC->CurPage].Index,0);
	return false;
};
GP_PageControl* ParentFrame::addPageControl(SimpleDialog* Parent,int x,int y,int GP_File,int FirstIndex){
	GP_PageControl* PC=new GP_PageControl;
	AddDialog(PC);
	PC->NeedToDraw=true;
	PC->Parent=Parent;
	PC->x=x+BaseX;
	PC->y=y+BaseY;
	PC->x1=PC->x+GPS.GetGPWidth(GP_File,FirstIndex);
	PC->y1=PC->y+GPS.GetGPHeight(GP_File,FirstIndex);
	PC->GP_File=GP_File;
	PC->CurPage=0;
	PC->NPages=0;
	PC->Pages=NULL;
	PC->Active=false;
	PC->OnDraw=&GP_PageControl_OnDraw;
	PC->Enabled=true;
	PC->Visible=true;
	return PC;
};
void GP_PageControl::AddPage(int x0,int y0,int x1,int y1,int Index){
	Pages=(OnePage*)realloc(Pages,(NPages+1)*sizeof OnePage);
	Pages[NPages].Index=Index;
	Pages[NPages].x=x0;
	Pages[NPages].y=y0;
	Pages[NPages].x1=x1;
	Pages[NPages].y1=y1;
	NPages++;
};
//-------------end of PageControl------------//
extern int BordGP2;
void DrawHdrTable(int x0,int y0,int x1,int y1);
void DrawScrollDoubleTableF(int x0,int y0,int x1,int y1,int ym);
void DrawStdBar2(int x0,int y0,int x1,int y1);
bool BorderEx_OnDraw(SimpleDialog* SD){
	BorderEx* BR=(BorderEx*)SD;
	switch(BR->Style){
	case 0://simple
		DrawStdBar(BR->x,BR->y,BR->x1,BR->y1);
		break;
	case 1://simple with header
		DrawHdrTable(BR->x,BR->y,BR->x1,BR->y1);
		break;
	case 2://double with scroller in bottom
		DrawScrollDoubleTable(BR->x,BR->y,BR->x1,BR->y1,BR->ymid);
		break;
	case 3://double with scroller in bottom(filled)
		DrawScrollDoubleTableF(BR->x,BR->y,BR->x1,BR->y1,BR->ymid);
		break;
	case 4://simple with header and scroller
		DrawHdrTable(BR->x,BR->y,BR->x1,BR->y1);
		break;
	case 5:
		DrawStdRect2(BR->x,BR->y,BR->x1,BR->y1,BordGP2);
		break;
	case 6:
		DrawStdBar2(BR->x,BR->y,BR->x1,BR->y1);
	};
	return true;
};
BorderEx* ParentFrame::addBorder(int x,int y,int x1,int y1,int Ymid,byte Style){
	BorderEx* BR=new BorderEx;
	AddDialog(BR);
	BR->x=x+BaseX;
	BR->y=y+BaseY;
	BR->x1=x1+BaseX;
	BR->y1=y1+BaseY;
	BR->ymid=Ymid+BaseY;
	BR->Style=Style;
	BR->Enabled=1;
	BR->Visible=1;
	BR->OnDraw=&BorderEx_OnDraw;
	BR->Active=0;
	return BR;
};
void DrawColoredRect(int x,int y,int Lx,int Ly,DWORD Diffuse,int Fade){
	DWORD A=Diffuse>>24;
	DWORD C=Diffuse&0xFFFFFF;
	for(int p=0;A>8;p++){
		if(Lx<=0||Ly<=0)return;
		Vector3D V1(x,y,0);
		Vector3D V2(x+Lx-1,y,0);
		Vector3D V4(x,y+Ly-1,0);
		Vector3D V3(x+Lx-1,y+Ly-1,0);
		const Matrix4D* M=GPS.GetScreenSpaceTM();
		/*if(M){
			M->transformPt(V1);
			M->transformPt(V2);
			M->transformPt(V3);
			M->transformPt(V4);
		}*/
		GPS.DrawLine(V1.x,V1.y,V2.x,V2.y,C+(A<<24));
		GPS.DrawLine(V2.x,V2.y,V3.x,V3.y,C+(A<<24));
		GPS.DrawLine(V3.x,V3.y,V4.x,V4.y,C+(A<<24));
		GPS.DrawLine(V4.x,V4.y,V1.x,V1.y,C+(A<<24));
		//GPS.DrawRect(x,y,Lx,Ly,C+(A<<24));		
		A=A*Fade/100;
		x++;
		y++;
		Lx-=2;
		Ly-=2;
	}
}
bool CustomBorder_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	CustomBorder* BR=(CustomBorder*)SD;
	Corners CR;
	int gp=BR->GP;
	int FIL=-1;
	if(BR->MouseOver&&BR->Enabled){
		memcpy(&CR,BR->BOR_A,32);
		FIL=BR->FILL_A;
	}else{
		memcpy(&CR,BR->BOR_N,32);
		FIL=BR->FILL_N;
	};
	if(gp!=-1){
		if(FIL==-1){
			DrawRect4(BR->x,BR->y,BR->x1,BR->y1,&CR,gp);
		}else{
			DrawFilledRect3(BR->x,BR->y,BR->x1,BR->y1,&CR,gp,FIL,3);
		};
	};
	return true;
};
CustomBorder* ParentFrame::addCustomBorder(int x,int y,int x1,int y1,
							  int gp,int* bn,int* ba,
							  int fill_n,int fill_a){
	CustomBorder* BR=new CustomBorder;
	AddDialog(BR);
	BR->x=x+BaseX;
	BR->y=y+BaseY;
	BR->x1=x1+BaseX;
	BR->y1=y1+BaseY;
	BR->Enabled=1;
	BR->Visible=1;
	BR->OnDraw=&CustomBorder_OnDraw;
	memcpy(BR->BOR_N,bn,32);
	memcpy(BR->BOR_A,ba,32);
	BR->FILL_N=fill_n;
	BR->FILL_A=fill_a;
	BR->GP=gp;
	BR->Active=0;
	return BR;
};
bool BikVideo_OnDraw(SimpleDialog* SD){
	if(!SD->Visible)return false;
	Video* BR=(Video*)SD;
	BinkPlayer* BPlay=(BinkPlayer*)BR->BPlay;
	VideoLayer* VL=(VideoLayer*)BR->VL;
	if(BPlay->Ready){
		int Lx=BPlay->Lx;
		int Ly=BPlay->Ly;
		if(BPlay->GetCurrentFrame()){
			BR->IsFinished=0;
		}else BR->IsFinished=1;
		if(BPlay->OutBuffer)VL->ShowRGB_Buffer(BPlay->OutBuffer,Lx,Ly,SD->x,SD->y,Lx,SD->Diffuse);
	}
}
bool BikVideo_OnDestroy(SimpleDialog* SD){
	Video* BR=(Video*)SD;
	BinkPlayer* BPlay=(BinkPlayer*)BR->BPlay;
	VideoLayer* VL=(VideoLayer*)BR->VL;
	delete(BPlay);
	delete(VL);
	return true;
}
Video* ParentFrame::addVideo(int x,int y,int Lx,int Ly){
	Video* BR=new Video;
	AddDialog(BR);
	BR->x=x+BaseX;
	BR->y=y+BaseY;
	BR->x1=x+Lx-1+BaseX;
	BR->y1=y+Ly-1+BaseY;
	BR->Enabled=1;
	BR->Visible=1;
	BR->OnDraw=&BikVideo_OnDraw;
	BR->Destroy=&BikVideo_OnDestroy;
	BR->Active=0;
	BR->VL=(void*)new VideoLayer;
	BR->BPlay=(void*)new BinkPlayer;
	return BR;
};
void Video::Play(char* file){
	((BinkPlayer*)BPlay)->Open(file);
};
void Video::Stop(){
	((BinkPlayer*)BPlay)->Close();
}
//-------------------------------------------//
void SimpleDialog::Init(){
	Enabled=false;
	x=0;
	y=0;
	x1=0;
	y1=0;
	UserParam=0;
	OnClick=NULL;
	OnUserClick=NULL;
	OnUserRightClick=NULL;
	OnDraw=NULL;
	OnActivate=NULL;
	OnKeyDown=NULL;
	OnLeave=NULL;
	OnMouseOver=NULL;
	MouseOverActive=NULL;
	Destroy=NULL;
	Refresh=NULL;
	Parent=NULL;
	Child=NULL;
	OnDrawActive=NULL;
	OnNewClick=false;
	MouseOver=false;
	MouseOverActive=false;
	Active=false;
	IsActive=false;
	Visible=true;
	Enabled=true;
	NeedRedrawAll=false;
	Hint=NULL;
	MouseSound=-1;
	ClickSound=-1;
	MouseOver=0;
	ParentSB=NULL;
	AllocPtr=NULL;
	ID=0;
	Diffuse=0xFFFFFFFF;
	ShapeFileID=0xFFFF;
	ShapeSpriteID=0xFFFF;
	Changed=0;
	vm_Init=false;
}

SimpleDialog::SimpleDialog(){
	Init();
};
bool SimpleDialog::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	if(ParentFrame::Load(xml,ClassPtr,Error,Extra)){
		if(MouseSound==-1){
			//MouseSound=GetSound("WEAP_SHTIKNEW");
		}
		return true;
	}
	return false;
};
void SimpleDialog::AssignSound(int ID,int Usage){
	switch(Usage){
	case MOUSE_SOUND:
		MouseSound=ID;
		break;
	case CLICK_SOUND:
		ClickSound=ID;
		break;
	};
};

void SimpleDialog::AssignSound(char* Name,int Usage){
	int ID=SearchStr(SoundID,Name,NSounds);
	if(ID==-1){
		char cc[128];
		sprintf(cc,"Unknown sound ID: %s",Name);
		ErrMS(cc);
		return;
	};
	AssignSound(ID,Usage);
};
//----copy rectangle to screen----//
void CopyToScreen(int zx,int zy,int zLx,int zLy){
#ifdef _USE3D
	return;
#endif //_USE3D
	if(!bActive)return; 
	int x=zx;
	int y=zy;
	int Lx=zLx;
	int Ly=zLy;
	//if(zLx<=0||zLy<=0||zx<0||zy<0||zx+zLx>RealLx||zy+zLy>RealLy)return;
	//if(zLx<=0||zLy<=0||zx<0||zy<0)return;
	if(x<0){
		Lx+=x;
		x=0;
	};
	if(y<0){
		Ly+=y;
		y=0;
	};
	if(x+Lx>RealLx)Lx=RealLx-x;
	if(y+Ly>RealLy)Ly=RealLy-y;
	if(Lx<0||Ly<0)return;
	//return;
	int scof=int(ScreenPtr)+x+y*SCRSizeX;
	int reof=int(RealScreenPtr)+x+y*RSCRSizeX;
	int sadd=SCRSizeX-Lx;
	int radd=RSCRSizeX-Lx;
	int Lx4=Lx>>2;
	int Lx1=Lx&3;
	__asm{
		push	esi
		push	edi
		mov		edx,Ly
		or		edx,edx
		jz		lpp4
		mov		esi,scof
		mov		edi,reof
		cld
lpp1:	mov		ecx,Lx4
		jcxz	lpp2
//		cli
		rep		movsd
lpp2:	mov		ecx,Lx1
		jcxz	lpp3
		rep		movsb
lpp3:	//sti
		add		esi,sadd
		add		edi,radd
		dec		edx
		jnz		lpp1
lpp4:	pop		edi
		pop		esi
	};
};
void CopyToOffScreen(int zx,int zy,
					 int srLx,int srLy,
					 byte* data){
	if(!bActive)return;
	int x=zx;
	int y=zy;
	int Lx=srLx;
	int Ly=srLy;
	//if(zLx<=0||zLy<=0||zx<0||zy<0||zx+zLx>RealLx||zy+zLy>RealLy)return;
	//if(zLx<=0||zLy<=0||zx<0||zy<0)return;
	if(x<0){
		Lx+=x;
		x=0;
	};
	if(y<0){
		Ly+=y;
		y=0;
	};
	if(x+Lx>RealLx)Lx=RealLx-x;
	if(y+Ly>RealLy)Ly=RealLy-y;
	if(Lx<0||Ly<0)
		return;
	int scof=int(data);
	int reof=int(ScreenPtr)+x+y*SCRSizeX;
	int sadd=srLx-Lx;
	int radd=SCRSizeX-Lx;
	int Lx4=Lx>>2;
	int Lx1=Lx&3;
	__asm{
		push	esi
		push	edi
		mov		edx,Ly
		or		edx,edx
		jz		lpp4
		mov		esi,scof
		mov		edi,reof
		cld
lpp1:	mov		ecx,Lx4
		jcxz	lpp2
//		cli
		rep		movsd
lpp2:	mov		ecx,Lx1
		jcxz	lpp3
		rep		movsb
lpp3:	//sti
		add		esi,sadd
		add		edi,radd
		dec		edx
		jnz		lpp1
lpp4:	pop		edi
		pop		esi
	};
};
void CopyToRealScreenMMX(int zx,int zy,
						int srLx,int srLy,
						byte* data);
void CopyToRealScreen(int zx,int zy,
					 int srLx,int srLy,
					 byte* data){
	if(!bActive)return;
	//CopyToRealScreenMMX(zx,zy,srLx,srLy,data);
	//return;
	int x=zx;
	int y=zy;
	int Lx=srLx;
	int Ly=srLy;
	//if(zLx<=0||zLy<=0||zx<0||zy<0||zx+zLx>RealLx||zy+zLy>RealLy)return;
	//if(zLx<=0||zLy<=0||zx<0||zy<0)return;
	if(x<0){
		Lx+=x;
		x=0;
	};
	if(y<0){
		Ly+=y;
		y=0;
	};
	if(x+Lx>RealLx)Lx=RealLx-x;
	if(y+Ly>RealLy)Ly=RealLy-y;
	if(Lx<0||Ly<0)
		return;
	int scof=int(data);
	int reof=int(RealScreenPtr)+x+y*RSCRSizeX;
	int sadd=srLx-Lx;
	int radd=RSCRSizeX-Lx;
	int Lx4=Lx>>2;
	int Lx1=Lx&3;
	__asm{
		push	esi
		push	edi
		mov		edx,Ly
		or		edx,edx
		jz		lpp4
		mov		esi,scof
		mov		edi,reof
		cld
lpp1:	mov		ecx,Lx4
		jcxz	lpp2
//		cli
		rep		movsd
lpp2:	mov		ecx,Lx1
		jcxz	lpp3
		rep		movsb
lpp3:	//sti
		add		esi,sadd
		add		edi,radd
		dec		edx
		jnz		lpp1
lpp4:	pop		edi
		pop		esi
	};
};
void CopyToRealScreenMMX(int zx,int zy,
						int srLx,int srLy,
						byte* data){
	int x=zx;
	int y=zy;
	int Lx=srLx;
	int Ly=srLy;
	//if(zLx<=0||zLy<=0||zx<0||zy<0||zx+zLx>RealLx||zy+zLy>RealLy)return;
	//if(zLx<=0||zLy<=0||zx<0||zy<0)return;
	if(x<0){
		Lx+=x;
		x=0;
	};
	if(y<0){
		Ly+=y;
		y=0;
	};
	if(x+Lx>RealLx)Lx=RealLx-x;
	if(y+Ly>RealLy)Ly=RealLy-y;
	if(Lx<0||Ly<0)
		return;
	int scof=int(data);
	int reof=int(RealScreenPtr)+x+y*RSCRSizeX;
	int sadd=srLx-Lx;
	int radd=RSCRSizeX-Lx;
	int Lx4=Lx>>3;
	int Lx1=Lx&7;
	__asm{
		push	esi
		push	edi
		mov		edx,Ly
		or		edx,edx
		jz		lpp4
		mov		esi,scof
		mov		edi,reof
		cld
lpp1:	mov		ecx,Lx4
		jcxz	lpp2
//		cli
lppm1:	movq	mm0,[esi]
		add		esi,8
		movq	[edi],mm0
		add		edi,8
		dec		ecx
		jnz		lppm1
lpp2:	mov		ecx,Lx1
		jcxz	lpp3
		rep		movsb
lpp3:	//sti
		add		esi,sadd
		add		edi,radd
		dec		edx
		jnz		lpp1
lpp4:	pop		edi
		pop		esi
		emms
	};
};
//--------------Handling dialog system----------------//
void MFix();
void RedrawGameBackground();
void DrawSD(SimpleDialog* SD){
	//if(SD->x>WindX1||SD->y>WindY1||SD->x1<WindX||SD->y1<WindY)return;
	int DY=0;
	///try{
		if(SD->ParentSB&&SD->ParentSB->Visible)DY=SD->ParentSB->SPos;
	//}catch(...){};
	SD->y-=DY;
	SD->y1-=DY;
	GPS.SetCurrentDiffuse(SD->Diffuse);
	SD->OnDraw(SD);
	GPS.SetCurrentDiffuse(0xFFFFFFFF);
	SD->y+=DY;
	SD->y1+=DY;
};
void UnPress();
typedef bool FrameEnumFn(SimpleDialog* SD,void* param);
int EnumerateFrames(ClassArray<SimpleDialog>* DSS,SimpleDialog** SDS,int max,FrameEnumFn* fn,void* param){
	int N=0;
	for(int i=0;i<DSS->GetAmount();i++){
		SimpleDialog* SD=(*DSS)[i];
		if(SD&&SD->Visible){
			if(fn(SD,param)&&max){
				SDS[0]=SD;
				max--;
				SDS++;
				N++;
			}
			int n2=EnumerateFrames(&SD->DSS,SDS,max,fn,param);
			max-=n2;
			SDS+=n2;
			N+=n2;
		}
	}
	return N;
}
int EnumerateFramesBack(ClassArray<SimpleDialog>* DSS,SimpleDialog** SDS,int max,FrameEnumFn* fn,void* param){
	int N=0;
	for(int i=DSS->GetAmount()-1;i>=0;i--){
		SimpleDialog* SD=(*DSS)[i];
		if(SD&&SD->Visible){
			int n2=EnumerateFramesBack(&SD->DSS,SDS,max,fn,param);
			max-=n2;
			SDS+=n2;
			N+=n2;
			if(fn(SD,param)&&max){
				SDS[0]=SD;
				max--;
				SDS++;
				N++;
			}						
		}
	}
	return N;
}
bool CheckSelSD(SimpleDialog* SD,void*){
	return SD->Selected;
}
int EnumerateSelectedFrames(ClassArray<SimpleDialog>* DSS,SimpleDialog** SDS,int max){
	return EnumerateFrames(DSS,SDS,max,&CheckSelSD,NULL);
}
bool ShiftDlg(SimpleDialog* SD,void* param){
	if(SD->x||SD->y){
		int l,r,u,d;
		SD->GetMargin(l,u,r,d);		
		for(int i=0;i<SD->DSS.GetAmount();i++){
			if(SD->DSS[i]){
				if(param){
					SD->DSS[i]->ShiftDialog(SD->x+l,SD->y+u);
				}else{
					SD->DSS[i]->ShiftDialog(-SD->x-l,-SD->y-u);
				}
			}
		}
	}
	return false;
}

struct m_xy
{
	int x;
    int y;
};

bool CheckFrameInside(SimpleDialog* SD,void* param){
	if(!SD->Visible)return false;
	return true;
	m_xy* mxy=(m_xy*)param;
	return mxy->x>SD->x&&mxy->y>SD->y&&mxy->x<SD->x1&&mxy->y<SD->y1;
}
SimpleDialog* SelectNearestFrame(ClassArray<SimpleDialog>* DSS,int x,int y,int& xd,int& yd){
	EnumerateFrames(DSS,NULL,1,ShiftDlg,&x);
	m_xy mxy;
    mxy.x=x;
	mxy.y=y;
	SimpleDialog* SDS[4096];
	int N=EnumerateFrames(DSS,SDS,4096,&CheckFrameInside,&mxy);
	int MinR=10000;
	SimpleDialog* BestSD=NULL;
	for(int i=0;i<N;i++){
		SimpleDialog* SD=SDS[i];
		if(SD&&SD->MouseOver){
			if(GetKeyState(VK_CONTROL)&0x8000){
				//if(SD->MouseOver){
					BestSD=SD;
					xd=SD->x;
					yd=SD->y;
				//}
			}else{
				int R=min(min(x-SD->x,SD->x1-x),min(y-SD->y,SD->y1-y));
				if(R<MinR){
					MinR=R;
					BestSD=SD;
					xd=SD->x;
					yd=SD->y;
				}
			}
		}
	}
	EnumerateFramesBack(DSS,NULL,1,ShiftDlg,NULL);
	return BestSD;
}
void DrawSelectedFrames(ClassArray<SimpleDialog>* DSS,int x0,int y0){	
	for(int i=0;i<(*DSS).GetAmount();i++){
		SimpleDialog* SD=(*DSS)[i];
		if(SD){
			int l,r,u,d;
			SD->GetMargin(l,u,r,d);
			SD->ShiftDialog(x0,y0);

			Matrix4D M0;
			int x00=mouseX;
			int y00=mouseY;
			Matrix4D* M=SD->PushMatrix(M0);		

			if(SD->Selected){
				DrawColoredRect(SD->x+l,SD->y+u,SD->x1-SD->x+1-l-r,SD->y1-SD->y+1-u-d,0xFFFF00FF,70);
			}			
			DrawSelectedFrames(&SD->DSS,SD->x+l,SD->y+u);

			SD->PopMatrix(M);
			mouseX=x00;
			mouseY=y00;

			SD->ShiftDialog(-x0,-y0);
		}
	}
}
void ShiftDSS(int x,int y,ClassArray<SimpleDialog>* DSS){
	for(int i=0;i<DSS->GetAmount();i++){
		if((*DSS)[i])(*DSS)[i]->ShiftDialog(x,y);
	}
}
TextIcons TIcons;
TextIcons TIconsTemp;
void LoadTextIcons(){
    xmlQuote xml;
	TIcons.reset_class(&TIcons);
	TIcons.SafeReadFromFile("Dialogs\\TextIcons.xml");
	WIN32_FIND_DATA FD;
	HANDLE H=FindFirstFile("Icons\\*.IconList.xml",&FD);
	if(H!=INVALID_HANDLE_VALUE){
		bool fexist;
		do{
			char cc[128];
			sprintf(cc,"Icons\\%s",FD.cFileName);
			TIcons.SafeReadFromFile(cc);
            fexist=FindNextFile(H,&FD);
		}while(fexist);
		FindClose(H);
	}
}
bool LockManualDialogsShift=0;
bool FontParamInit=0;
//CEXPORT FontParam FParam;
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
	RLCFont FONT;
	RLCFont* CFont;
	RLCFont* DefaultFont;
	DWORD DefaultColor;
	OneFontParam* CurFParam;
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
                    int y0;
                    TextFrame* TF=NULL;
                    char* s=comstr+7;
                    int FLX=MaxWidth;
                    int FLY=DrawMultilineText(-100000,-100000,s,CFont,FLX,10000,false,false);
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
                            DrawMultilineText(x0,TF->FrameY-CurFParam->Bottom,s,CFont,FLX,10000,false,true);
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
						if(Lpressed)strcpy(GlobalTextCommand,CurTxtCom);
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
						if(comstr[1]=='D'){	// damask - ��
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
						CurFParam=FParam.GetByGP(int(CFont->RLC));
					}else{
						OneFontParam* OFP=FParam.GetByName(comstr+1);
						if(OFP){
							CurFParam=OFP;
							FONT.RLC=(RLCTable)OFP->gpFont;
							FONT.FirstSymbol=0;
							FONT.LastSymbol=0;
							if(OFP->EnableShadow){
								FONT.ShadowGP=OFP->gpShadow;
								FONT.ShadowDx=OFP->ShadowDx;
								FONT.ShadowDy=OFP->ShadowDy;
							}else{
								FONT.ShadowGP=-1;
							}
							FONT.Options=16;
							CFont=&FONT;
							FontColor=OFP->DefColor;
						}
					}
				}else
				if((comstr[0]=='P'||comstr[0]=='G')&&comstr[1]==' '){
					int dx=0;
					int dy=0;
					int Lx=-1;
					int Ly=-1;
					int idx=-1;
					int sidx=0;
					int exdy=0;
					if(comstr[0]=='P'){
						char name[256]="";
						sscanf(comstr+1,"%s%d%d%d%d%d%d",name,&sidx,&dx,&dy,&Lx,&Ly,&exdy);
						if(name[0]){
                            static int btnz=GPS.PreLoadGPImage("Interf3\\btnz");
                            if(!strcmp(name,"Interf3\\btnz"))idx=btnz;
							else idx=GPS.PreLoadGPImage(name);
						}
					}else{
						sscanf(comstr+1,"%d%d%d%d%d%d%d",&idx,&sidx,&dx,&dy,&Lx,&Ly,&exdy);
					}
					if(idx!=-1){
						if(Lx==-1){
							Lx=GPS.GetGPWidth(idx,sidx);
						}
						if(Ly==-1){
							Ly=GPS.GetGPHeight(idx,sidx)-dy;
						}
						int dy2=Ly/2-(CurFParam?CurFParam->Bottom-CurFParam->Top:16)/2;
						Ly-=dy2;
						exdy+=dy2;
						if(Draw==1){
							GPS_ShowGP(xL+CurLx-dx,yL-Ly-dy,idx,sidx,0);
							RestoreTextShader();
						}						
						CurLx+=Lx-dx;
                        int H=Ly-dy;
						if(H>Height){
							int DH=H-Height;
							Height=H;
							TotalHeight+=DH;
						}
						if(TotalHeight-Height<exdy){
							TotalHeight=Height+exdy;
						}
					}
				}else
				if(comstr[0]=='I'){
					OneTextIcon* OTI=TIconsTemp.get(comstr+1);
					if(!OTI)OTI=TIcons.get(comstr+1);
					if(OTI){
						if(OTI->IconType==1){
							if(OTI->TextureID>0){
								float TLX=IRS->GetTextureWidth(OTI->TextureID);
								float TLY=IRS->GetTextureHeight(OTI->TextureID);
								if(TLX&&TLY){
									if(Draw==1){
										static int sh=IRS->GetShaderID("hud");
										void DrawTexturedBar(float xL,float yL,float W,float H,
											float txL,float tyL,float tW,float tH,
											DWORD CLU,DWORD CRU,DWORD CLD,DWORD CRD,
											int TextureID,int ShaderID);
										DrawTexturedBar(xL+CurLx-OTI->LeftMargin,yL-OTI->IconLy+OTI->BottomMargin,OTI->IconLx,OTI->IconLy
											,float(OTI->IconX)/TLX,float(OTI->IconY)/TLY,float(OTI->IconLx)/TLX,float(OTI->IconLy)/TLY
											,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,OTI->TextureID,sh);										
									}                           
									int H=OTI->IconLy-OTI->TopMargin;
									if(H>Height){
										int DH=H-Height;
										Height=H;
										TotalHeight+=DH;
									}
									if(TotalHeight-Height<OTI->BottomMargin){
										TotalHeight=Height+OTI->BottomMargin;
									}
									CurLx+=OTI->IconLx-OTI->LeftMargin-OTI->RightMargin;
								}
							}
						}else
						if(OTI->IconType==2){
							if(OTI->ModelID>0){
								if(Draw==1){
                                    Matrix4D M4;
									static int T0=GetTickCount();
									int DT=float(GetTickCount()-T0)/1000.0f;
									//M4.srt(OTI->Scale,Vector3D::oZ,DT*OTI->RotationSpeed,)
								}                           
								int H=OTI->IconLy-OTI->TopMargin;
								if(H>Height){
									int DH=H-Height;
									Height=H;
									TotalHeight+=DH;
								}
								if(TotalHeight-Height<OTI->BottomMargin){
									TotalHeight=Height+OTI->BottomMargin;
								}
								CurLx+=OTI->IconLx-OTI->LeftMargin-OTI->RightMargin;
							}
						}else{
							if(Draw==1){
								GPS_ShowGP(xL+CurLx-OTI->dx,yL-OTI->Ly,OTI->gpFile,OTI->Sprite,0);
								RestoreTextShader();
							}
							CurLx+=OTI->Lx-OTI->dx;						
							int H=OTI->Ly-OTI->dy;						
							if(H>Height){
								int DH=H-Height;
								Height=H;
								TotalHeight+=DH;
							}
							if(TotalHeight-Height<OTI->ExtraBottomDy){
								TotalHeight=Height+OTI->ExtraBottomDy;
							}
						}
					}
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
				}else{
					//search for unknown command
					OneClassStorage* OCS=CGARB.GetClass("TextVisualElement");
					if(OCS){
						int N=OCS->Children.GetAmount();
						for(int j=0;j<N;j++){
							char* name=OCS->Children[j];
							OneClassStorage* ECS=CGARB.GetClass(name);
							if(ECS){
								TextVisualElement* TVE=(TextVisualElement*)ECS->OneMemb;
								if(TVE&&TVE->Assign(comstr)){
									int eLx=Height==0?16:Height;
									int eLy=eLx;
									int eMp=eLy;
									int fly=CurFParam?CurFParam->Bottom-CurFParam->Top:16;
									TVE->GetBestSize(fly,eLx,eLy,eMp);											
									if(Draw==1){
										//TVE->Draw(xL+CurLx,yL-eLy+eMp-fly,eLx,eLy,0xFFFFFFFF);
										TVE->Draw(xL+CurLx,yL-eMp,eLx,eLy,0xFFFFFFFF);
										RestoreTextShader();
									};
									CurLx+=eLx;
									if(eMp>Height)Height=eMp;
                                    int H1=Height+eLy-eMp;
									if(H1>TotalHeight)TotalHeight=H1;
									/*
									if(eLy>TotalHeight)TotalHeight=eLy;
									int hh=eMp;//eLy-eMp+fly;
                                    if(Height<hh)Height=hh;
									int dh=eLy-eMp;
									if(DnHeight<dh)DnHeight=dh;
									*/
								}
							}
						}
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
					if(Lpressed)strcpy(GlobalTextCommand,GlobalTextMouseOverCommand);//CurTxtCom);
					//strcpy(GlobalTextMouseOverCommand,CurTxtCom);
				}
				if(pos==sposL&&s[pos]==' ')pos++;
				else{
					int uW=0;
					int cLx=GetRLCWidthUNICODE(CFont->RLC,(byte*)(s+pos),&uW);
					if(refopen&&CurFParam&&Draw==1){
						if(mouseX>xL+CurLx+dCurLx&&mouseX<=xL+CurLx+dCurLx+cLx&&
							mouseY<yL&&mouseY>yL-CurFParam->Bottom){
                                ActiveRefIndex=curref;
								strcpy(ActiveRefCommand,CurTxtCom);
								RefActivated=true;
							}
					}
					if(CurLx+cLx>MaxWidth && !strchr(Unbreakables,s[pos])){
						if(LastSpacePos>sposL && s[pos]!='\\' && s[pos]!=10){//&&LastSpaceLx>MaxWidth*3/4){
                            //breaking at space
							sposR=LastSpacePos;
							CurLx=LastSpaceLx;
							NSym=LastSpaceSym;
							//sposR=pos;
						}else{
							//breaking at the current symbol
							sposR=prevpos;//pos-1;
						}
					}
					if(pos>=sposR|| (s[pos]=='\\' && NBracks&1) ||s[pos]==13||s[pos]==10){						
						if(s[pos]=='\\'||s[pos]==10){
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
						/*if( ( cc>='A' && cc<='Z' && cp>='a' && cp<='z')
							||( ( cc>='0' && cc<='9' ) && ! ( cp>='0' && cp<='9' ) )
							|| cp==')' || cp==',' || cp==';'){*/
						//new 14.04
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
					if(CurFParam){
						if(Draw==1){
							DWORD PC=CFont->GetColor();
                            if(ForceShadowDrawing)FontColor=0x90000000;
							if(FontColor)CFont->SetColor(FontColor);
							else CFont->SetColor(PC);//CheckFontColor(CFont);
							ShowCharUNICODE(xL+CurLx+dCurLx,yL-CurFParam->Bottom,(byte*)s+pos,CFont);
							CFont->SetColor(PC);
						}
                        int H=CurFParam->Bottom-CurFParam->Top;//+CurFParam->YShift;
						//int H1=CurFParam->Bottom-CurFParam->Top+CurFParam->YShift;//+CurFParam->YShift*2;
						if(H>Height)Height=H;
						//int H1=Height+CurFParam->YShift;
						if(H>TotalHeight)TotalHeight=H;
						if(CurFParam->YShift>DnHeight)DnHeight=CurFParam->YShift;
					}
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
					//if(res)SpaceAddDist=(MaxWidth-CurLx-SymDx*NSym)/(NSpaces);
					//else SpaceAddDist=(MaxWidth-CurLx-SymDx*NSym)/(NSpaces-1);
					if(SpaceAddDist>EngSettings.MaxSpaceExtraDistance){
						SpaceAddDist=0;
						SymDx=0;
					}
					//if(SpaceAddDist>10)SpaceAddDist=10;
				}			
				dCurLx+=NSym*SymDx+(NSpaces-1)*SpaceAddDist;		
			}
		}        
		return res;
	}
};
int DrawMultilineText(int x,int y,char* s,RLCFont* DefaultFont,int& MaxLx,int MaxLy=100000,byte CenterX=0,bool Draw=true){	
	if(!s||!DefaultFont)return 0;
	if(!FontParamInit){
		FParam.Read("Dialogs\\fonts.xml");
		LoadTextIcons();
		FontParamInit=1;
	}
	if(Draw)RestoreTextShader();
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
	T1.DefaultColor=DefaultFont->GetColor();
	T1.CurFParam=FParam.GetByGP(int(DefaultFont->RLC));
	T1.s=s;
	T1.sposL=0;
	T1.MaxWidth=MaxLx;
	T1.FontColor=0;
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
		///if(NL==0/*&&R*/){
		//	TH+=T1.TotalHeight;
		//	T1.yL+=T1.Height;//T1.TotalHeight;
		//}else{
		//	TH+=T1.Height;
		//	T1.yL+=T1.Height;
		//}		

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
		T1.CurFParam=T2.CurFParam;
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
			T1.CurFParam=T2.CurFParam;
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
		static int T0=0;
		if(!T2.RefActivated){			
			if(T0==0)T0=GetTickCount();
			else{
				if(GetTickCount()-T0>200)ActiveRefCom[0]=0;
			}
		}else T0=0;
	}
	if(Draw){
		if(GlobalTextMouseOverCommand[0]&&!strncmp(GlobalTextMouseOverCommand,"$HINT ",6)){
            static bool isins=false;
			static RLCFont* HintFont=&SmallWhiteFont;
			if(!isins){
				isins=true;
				TempWindow TW;
				PushWindow(&TW);
				GPS.SetClipArea(0,0,RealLx,RealLy);
				char* s=GlobalTextMouseOverCommand+6;
				if(!strchr(s,' '))s=GetTextByID(s);
				GPS.FlushBatches();
                ISM->Flush();
				int LX=320;
				int LY=DrawMultilineText(0,0,s,HintFont,LX,100000,false,false);
				int x=mouseX+30;
				int y=mouseY+30;
				if(x+LX+50>RealLx)x=RealLx-LX-50;
				if(y+LY+30>RealLy)y=mouseY-LY-30;
				DrawHintPattern(x,y,x+LX+20,y+LY+20);
				GPS.FlushBatches();
                ISM->Flush();
				LX=320;
				DrawMultilineText(x+10,y+10,s,HintFont,LX,100000,false,true);
				PopWindow(&TW);
				GlobalTextMouseOverCommand[0]=0;
				isins=false;
			}
		}

	}
	return TH;
}
static DWORD DiffuseModulator=0xFFFFFFFF;
extern bool TotalUnPress;
void ParentFrame::ProcessAligning(){	
	if(LeftAlign || RightAlign || TopAlign || BottomAlign || HorizontalCenterAlign || VerticalCenterAlign){
		int px=0;
		int px1=RealLx-1;
		int py=0;
		int py1=RealLy-1;
		int pw=RealLx;
		int ph=RealLy;	

		if(ParentDS){
			px=ParentDS->x;
			px1=ParentDS->x1;
			py=ParentDS->y;
			py1=ParentDS->y1;
			pw=ParentDS->GetWidth();
			ph=ParentDS->GetHeight();
		}

		ParentFrame* PFL=LeftAlignTo.Get();
		if(LeftAlign && !PFL)PFL=ParentDS;
		ParentFrame* PFR=RightAlignTo.Get();
		if(RightAlign && !PFR)PFR=ParentDS;
		ParentFrame* PFT=TopAlignTo.Get();
		if(TopAlign && !PFT)PFT=ParentDS;
		ParentFrame* PFB=BottomAlignTo.Get();
		if(BottomAlign && !PFB)PFR=ParentDS;
		ParentFrame* PFHC=HorizontalCenterAlignTo.Get();
		if(HorizontalCenterAlign && !PFHC)PFHC=ParentDS;
		ParentFrame* PFVC=VerticalCenterAlignTo.Get();
		if(VerticalCenterAlign && !PFVC)PFVC=ParentDS;

		if(LeftAlign==1){
			if(PFL)x=PFL->globalX()+PFL->x1-PFL->x-int(LA_param);
			else x=px1-int(LA_param);//px+pw-int(LA_param);
		}
		if(LeftAlign==2){
			if(PFL)x=PFL->globalX()+int(float(pw)*LA_param);
			else x=px+int(float(pw)*LA_param);
		}
		if(RightAlign==1){
			if(PFR)x1=PFR->globalX()+PFR->x1-PFR->x-int(RA_param);
			else x1=px1-int(RA_param);//px+pw-int(RA_param)-1;
		}
		if(RightAlign==2){
			if(PFR)x1=PFR->globalX()+int(float(PFR->GetWidth())*RA_param);
			else x1=px+int(float(pw)*RA_param);
		}
		if(HorizontalCenterAlign==1){		
			if(PFHC){
				int x0=(PFHC->globalX()+PFHC->x1-PFHC->x-int(HCA_param))&0xFFFFFFFE;
				int LX=x1-x;
				x=x0-LX/2;
				x1=x0+LX/2;
			}else{
				int x0=(px+pw-int(HCA_param))&0xFFFFFFFE;
				int LX=x1-x;
				x=x0-LX/2;
				x1=x0+LX/2;
			}
		}
		if(HorizontalCenterAlign==2){
			if(PFHC){
				int x0=(PFHC->globalX()+int(float(PFHC->GetWidth())*HCA_param))&0xFFFFFFFE;
				int LX=x1-x;		
				x=x0-LX/2;
				x1=x0+LX/2;
			}else{
				int x0=(px+int(float(pw)*HCA_param))&0xFFFFFFFE;
				int LX=x1-x;		
				x=x0-LX/2;
				x1=x0+LX/2;
			}
		}
		if(TopAlign==1){
			if(PFT)y=PFT->globalY()+PFT->y1-PFT->y-int(TA_param);
			else y=py+ph-int(TA_param);
		}
		if(TopAlign==2){
			if(PFT)y=PFT->globalY()+int(float(PFT->GetHeight())*TA_param);
			else y=py+int(float(ph)*TA_param);
		}
		if(BottomAlign==1){
			if(PFB)y1=PFB->globalY()+PFB->y1-PFB->y-int(BA_param);
			else y1=py1-int(BA_param);//py+ph-int(BA_param)-1;
		}
		if(BottomAlign==2){
			if(PFB)y1=PFB->globalY()+int(float(PFB->GetHeight())*BA_param);
			else y1=py+int(float(ph)*BA_param);
		}
		if(VerticalCenterAlign==1){
			if(PFVC){
				int y0=PFVC->globalY()+PFVC->y1-PFVC->y-int(VCA_param);
				int LY=y1-y;
				y=y0-LY/2;
				y1=y0+LY/2;
			}else{
				int y0=py+ph-int(VCA_param);
				int LY=y1-y;
				y=y0-LY/2;
				y1=y0+LY/2;
			}
		}
		if(VerticalCenterAlign==2){
			if(PFVC){
				int y0=PFVC->globalY()+int(float(PFVC->GetHeight())*VCA_param);
				int LY=y1-y;
				y=y0-LY/2;
				y1=y0+LY/2;
			}else{
				int y0=py+int(float(ph)*VCA_param);
				int LY=y1-y;
				y=y0-LY/2;
				y1=y0+LY/2;
			}
		}	
	}
	//*
	if(Visible){
		for(int i=0;i<DSS.GetAmount();i++){
			ParentFrame* PF=DSS[i];
			if(PF){
				PF->ParentDS=this;
				PF->ProcessAligning();
			}
		}
	}
	//*/
}
void ParentFrame::ProcessLocalAligning(){
	if(LeftAlign || RightAlign || TopAlign || BottomAlign || HorizontalCenterAlign || VerticalCenterAlign){
		int px=0;
		int px1=RealLx-1;
		int py=0;
		int py1=RealLy-1;
		int pw=RealLx;
		int ph=RealLy;
		if(ParentDS){
			px=ParentDS->x;
			px1=ParentDS->x1;
			py=ParentDS->y;
			py1=ParentDS->y1;
			pw=ParentDS->GetWidth();
			ph=ParentDS->GetHeight();
		}

		ParentFrame* PFL=LeftAlignTo.Get();
		if(LeftAlign && !PFL)PFL=ParentDS;
		ParentFrame* PFR=RightAlignTo.Get();
		if(RightAlign && !PFR)PFR=ParentDS;
		ParentFrame* PFT=TopAlignTo.Get();
		if(TopAlign && !PFT)PFT=ParentDS;
		ParentFrame* PFB=BottomAlignTo.Get();
		if(BottomAlign && !PFB)PFR=ParentDS;
		ParentFrame* PFHC=HorizontalCenterAlignTo.Get();
		if(HorizontalCenterAlign && !PFHC)PFHC=ParentDS;
		ParentFrame* PFVC=VerticalCenterAlignTo.Get();
		if(VerticalCenterAlign && !PFVC)PFVC=ParentDS;		
		
		pw=px1-px+1;
		ph=py1-py+1;

		if(ParentDS){
			int l,r,u,d;
			ParentDS->GetMargin(l,u,r,d);
			px+=l;
			py+=u;		
		}	

		if(LeftAlign==1){
			if(PFL)x=PFL->globalX()+PFL->x1-PFL->x-int(LA_param);
			else x=px1-int(LA_param);//px+pw-int(LA_param);
		}
		if(LeftAlign==2){
			if(PFL)x=PFL->globalX()+int(float(PFL->GetWidth())*LA_param);
			else x=px+int(float(pw)*LA_param);
		}
		if(RightAlign==1){
			if(PFR)x1=PFR->globalX()+PFR->x1-PFR->x-int(RA_param);
			else x1=px1-int(RA_param);//px+pw-int(RA_param)-1;
		}
		if(RightAlign==2){
			if(PFR)x1=PFR->globalX()+int(float(PFR->GetWidth())*RA_param);
			else x1=px+int(float(pw)*RA_param);
		}
		if(HorizontalCenterAlign==1){		
			if(PFHC){
				int x0=(PFHC->globalX()+PFHC->x1-PFHC->x-int(HCA_param));//&0xFFFFFFFE;
				int LX=x1-x;
				x=x0-LX/2;
				x1=x0+LX/2;
			}else{
				int x0=(px+pw-int(HCA_param))&0xFFFFFFFE;
				int LX=x1-x;
				x=x0-LX/2;
				x1=x0+LX/2;
			}
		}
		if(HorizontalCenterAlign==2){
			if(PFHC){
				int x0=(PFHC->globalX()+int(float(PFHC->GetWidth())*HCA_param));//&0xFFFFFFFE;
				int LX=x1-x;		
				x=x0-LX/2;
				x1=x0+LX/2;
			}else{
				int x0=(px+int(float(pw)*HCA_param))&0xFFFFFFFE;
				int LX=x1-x;		
				x=x0-LX/2;
				x1=x0+LX/2;
			}
		}
		if(TopAlign==1){
			if(PFT)y=PFT->globalY()+PFT->y1-PFT->y-int(TA_param);
			else y=py+ph-int(TA_param);
		}
		if(TopAlign==2){
			if(PFT)y=PFT->globalY()+int(float(PFT->GetHeight())*TA_param);
			else y=py+int(float(ph)*TA_param);
		}
		if(BottomAlign==1){
			if(PFB)y1=PFB->globalY()+PFB->y1-PFB->y-int(BA_param);
			else y1=py1-int(BA_param);//py+ph-int(BA_param)-1;
		}
		if(BottomAlign==2){
			if(PFB)y1=PFB->globalY()+int(float(PFB->GetHeight())*BA_param);
			else y1=py+int(float(ph)*BA_param);
		}
		if(VerticalCenterAlign==1){
			if(PFVC){
				int y0=PFVC->globalY()+PFVC->y1-PFVC->y-int(VCA_param);
				int LY=y1-y;
				y=y0-LY/2;
				y1=y0+LY/2;
			}else{
				int y0=py+ph-int(VCA_param);
				int LY=y1-y;
				y=y0-LY/2;
				y1=y0+LY/2;
			}
		}
		if(VerticalCenterAlign==2){
			if(PFVC){
				int y0=PFVC->globalY()+int(float(PFVC->GetHeight())*VCA_param);
				int LY=y1-y;
				y=y0-LY/2;
				y1=y0+LY/2;
			}else{
				int y0=py+int(float(ph)*VCA_param);
				int LY=y1-y;
				y=y0-LY/2;
				y1=y0+LY/2;
			}
		}	
	}
}
bool ParentFrame::OnChangeMember(BaseClass* MembClass,void* MembPtr,void* MembExtra,const char* MembName){
	
	if(!strcmp(MembName,"LeftAlignTo")){
        SetLeftAlign(-LeftAlign);
	}
	if(!strcmp(MembName,"RightAlignTo")){
		SetRightAlign(-RightAlign);
	}
	if(!strcmp(MembName,"TopAlignTo")){
		SetTopAlign(-TopAlign);
	}
	if(!strcmp(MembName,"BottomAlignTo")){
		SetBottomAlign(-BottomAlign);
	}
	if(!strcmp(MembName,"HorizontalCenterAlignTo")){
		SetHCenterAlign(-HorizontalCenterAlign);
	}
	if(!strcmp(MembName,"VerticalCenterAlignTo")){
		SetVCenterAlign(-VerticalCenterAlign);
	}
	
	return false;
}
void ParentFrame::SetLeftAlign(int p){
	if(p!=LeftAlign){
		p=abs(p);
		ParentFrame* PF=ParentDS;
		if(LeftAlignTo.Get())PF=LeftAlignTo.Get();
		switch(p){
				case 0://AbsoluteLeftAlign
					break;
				case 1://AbsoluteRightAlign
					if(PF)LA_param=PF->globalX()+PF->x1-PF->x-globalX();//PF->GetWidth()-x;
					break;
				case 2://RelativeLeftAlign
					if(PF && PF->x1!=PF->x && PF==ParentDS)LA_param=float(globalX()-PF->globalX())/float(PF->GetWidth());
					break;				
		}
	}
	LeftAlign=p;
}
void ParentFrame::SetRightAlign(int p){
	if(p!=RightAlign){
		p=abs(p);
		ParentFrame* PF=ParentDS;
		if(RightAlignTo.Get())PF=RightAlignTo.Get();
		switch(p){
				case 0://AbsoluteLeftAlign
					break;
				case 1://AbsoluteRightAlign
					if(PF)RA_param=PF->globalX()+PF->x1-PF->x-(globalX()-x+x1);//PF->GetWidth()-x1;
					break;
				case 2://RelativeLeftAlign
					if(PF&&PF->x1!=PF->x&&PF==ParentDS)RA_param=float(globalX()-PF->globalX()+x1-x)/float(PF->GetWidth());
					break;				
		}
	}
	RightAlign=p;
}
void ParentFrame::SetHCenterAlign(int p){
	if(p!=HorizontalCenterAlign){
		p=abs(p);
		ParentFrame* PF=ParentDS;
		if(HorizontalCenterAlignTo.Get())PF=HorizontalCenterAlignTo.Get();
		switch(p){
				case 0://AbsoluteLeftAlign
					break;
				case 1://AbsoluteRightAlign
					if(PF)HCA_param=PF->globalX()+PF->GetWidth()-globalX()-(x+x1)/2;//PF->GetWidth()-(x+x1)/2;
					break;
				case 2://RelativeLeftAlign
					if(PF&&PF->x1!=PF->x&&PF==ParentDS)HCA_param=(float(globalX()*2+x1-x)/2.0f-PF->globalX())/float(PF->GetWidth());
					break;				
		}
	}
	HorizontalCenterAlign=p;
}
void ParentFrame::SetTopAlign(int p){
	if(p!=TopAlign){
		p=abs(p);
		ParentFrame* PF=ParentDS;
		if(TopAlignTo.Get())PF=TopAlignTo.Get();
		switch(p){
				case 0://AbsoluteTopAlign
					break;
				case 1://AbsoluteBottomAlign
					if(PF)TA_param=PF->globalY()+PF->y1-PF->y-globalY();//PF->GetHeight()-y;
					break;
				case 2://RelativeAlign
					if(PF&&PF->y1!=PF->y&&PF==ParentDS)TA_param=float(globalY()-PF->globalY())/float(PF->GetHeight());
					break;				
		}
	}
	TopAlign=p;
}
void ParentFrame::SetBottomAlign(int p){
	if(p!=BottomAlign){
		p=abs(p);
		ParentFrame* PF=ParentDS;
		if(BottomAlignTo.Get())PF=BottomAlignTo.Get();
		switch(p){
				case 0://AbsoluteTopAlign
					break;
				case 1://AbsoluteBottomAlign
					if(PF)BA_param=PF->globalY()+PF->y1-PF->y-globalY()+y-y1;//PF->GetHeight()-y1;
					break;
				case 2://RelativeAlign
					if(PF&&PF->y1!=PF->y&&PF==ParentDS)BA_param=float(globalY()-PF->globalY()+y1-y)/float(PF->GetHeight());
					break;				
		}
	}
	BottomAlign=p;
}
void ParentFrame::SetVCenterAlign(int p){
	if(p!=VerticalCenterAlign){
		p=abs(p);
		ParentFrame* PF=ParentDS;
		if(VerticalCenterAlignTo.Get())PF=VerticalCenterAlignTo.Get();
		switch(p){
				case 0://AbsoluteLeftAlign
					break;
				case 1://AbsoluteRightAlign
					if(PF)VCA_param=PF->globalY()+PF->y1-PF->y-globalY()-(y+y1)/2;//PF->GetHeight()-(y+y1)/2;
					break;
				case 2://RelativeLeftAlign
					if(PF&&PF->y1!=PF->y&&PF==ParentDS)VCA_param=(float(globalY()*2+y1-y)/2.0f-PF->globalY())/float(PF->GetHeight());
					break;				
		}
	}
	VerticalCenterAlign=p;
}
bool WasClickOnSmt=false;
class GlobalHintInfo{
public:
	GlobalHintInfo(){
		HintMessage="";
		PrevHintTime=0;
	}
	_str HintMessage;
	RLCFont* HintFont;
	byte HintStyle;
	int HintLineDistance;
	int HintLX;
	int HintLY;
	int HintX;
	int HintY;
	bool HintFade:1;
	bool HintMovesWithMouse:1;
	bool HintBorder:1;
	byte HintAligningX;
	byte HintAligningY;
	int HintFadeTime;
	DWORD HintBackColor;
	DWORD HintFontColor;
	tpShowHint* HintCallback;
	tpDrawHintBorder* HintBorderCallback;
	bool CenteredHint;
	byte HintFontAlign;
	bool HintXYisGlobal;
	int PrevHintTime;
	int x,y,x1,y1;
	void Draw();
};
void GlobalHintInfo::Draw(){
	if(!HintMessage.pchar())return;
	char* Hint=HintMessage.pchar();
	if(!(Hint&&Hint[0]))return;
	RestoreTextShader();
	if(HintStyle){
		//auto hint
		int MaxLx=80;
		int Ly=0;
		int Ny=1;
		CheckFontColor(HintFont);
		int Dy=GetRLCHeight(HintFont->RLC,'y');
		Rct F;
		GPS.GetGPBoundFrame(int(HintFont->RLC),'Y',F);
		int rdy=Dy-F.y;
		//rdy=rdy*4/3;
		if(HintLineDistance){
			Dy=rdy=HintLineDistance;
		}
		Dy+=UNI_HINTDLY2;
		byte c=' ';
		int x1=0;
		int pos=0;
		int MLX=HintStyle?HintLX:HintY;
		int HDX=HintStyle?HintX:20;
		if(MLX==0)MLX=400;
		MaxLx=MLX;
		int MLX2=MaxLx;
		Ly=DrawMultilineText(-800,-800,Hint,HintFont,MaxLx,100000,0,0);
		int x0,y0;
		int HintAddLy=10;
		if(!HintStyle){
			x0=mouseX; //-MaxLx+20
			y0=mouseY;
		}else{
			if(HintMovesWithMouse){
				x0=mouseX+HintX;
				y0=mouseY+HintY;
			}else{
				x0=HintX>=0?HintX:RealLx+HintX;
				y0=HintY>=0?HintY:RealLy+HintY;
			}
			byte ALIGNX=HintAligningX;
			byte ALIGNY=HintAligningY;
			if(ALIGNX==3){
				if(x0>RealLx/2)ALIGNX=2;
				else ALIGNX=0;
			}
			if(ALIGNY==3){
				if(y0>RealLy-Ly-60)ALIGNY=2;
				else ALIGNY=0;
			}
			switch(ALIGNX){
							case 2:
								x0-=MaxLx-15;
								if(HintMovesWithMouse){
									x0-=HintX*2;
								}
								break;
							case 1:
								x0-=MaxLx/2;
								break;
							case 0:
								//x0+=32;
								break;
			}
			switch(ALIGNY){
							case 2:
								y0-=Ly+HintAddLy+5;
								if(HintMovesWithMouse){
									y0-=HintY*2;
								}
								break;
							case 1:
								y0-=(Ly+HintAddLy)/2;
								break;
							case 0:
								y0+=32+5;
								break;
			}
		}
		if(x0<0)x0=0;
		DWORD D=HintBackColor;
		DWORD GetDwordModulated(DWORD C,int V);
		int FadeMod=0xFF;					
		if(HintFade){
			int FT=HintFadeTime;
			if(!FT)FT=300;
			FadeMod=(GetTickCount()-PrevHintTime-HintFadeTime)*255/FT;
			if(FadeMod>255)FadeMod=255;
		}
		if(!HintXYisGlobal&&!HintMovesWithMouse){
			x0+=x;
			y0+=y;
		}
		if(HintBorder){
			GPS.SetCurrentDiffuse(GetDwordModulated(HintBackColor,FadeMod));
			if(HintBorderCallback)HintBorderCallback(x0,y0,x0+MaxLx+14,y0+Ly+HintAddLy,GPS.GetCurrentDiffuse());
			else DrawHintPattern(x0,y0,x0+MaxLx+14,y0+Ly+HintAddLy);
			GPS.FlushBatches();
			GPS.SetCurrentDiffuse(0xFFFFFFFF);
		}
		x0+=7;
		y0+=5;
		pos=0;
		x1=x0;
		if(HintFontAlign==1){
			x0-=(MLX2-MaxLx)/2;
		}
		MaxLx=MLX2;					
		int Ly1=DrawMultilineText(x0,y0,Hint,HintFont,MaxLx,1000000,HintFontAlign,1);
		Ly=Ly1;
	}else if(CenteredHint){
		int x0=HintX;
		int y0=HintY+UNI_HINTDY1;
		int pos=0;
		int x1=HintX;
		CheckFontColor(HintFont);
		int L=GetRLen(Hint,HintFont);
		ShowStringEx(x0-(L/2),y0,Hint,HintFont);
	}else{
		int x0=HintX;
		int y0=HintY+UNI_HINTDY1;
		int pos=0;
		int x1=HintX;
		CheckFontColor(HintFont);
		int Dy=GetRLCHeight(HintFont->RLC,'y')+UNI_HINTDLY1;
		byte c=' ';
		do{
			c=Hint[pos];
			if(c!=0&&c!='/'&&c!='\\'){
				ShowCharUNICODE(x1,y0,(byte*)(Hint+pos),HintFont);
				int L=1;
				x1+=GetRLCWidthUNICODE(HintFont->RLC,(byte*)(Hint+pos),&L);
				pos+=L;
			}else{
				if(c=='/'||c=='\\'){
					x1=x0;
					y0+=Dy;
					pos++;
				};
			};
		}while(c);
	}
	HintMessage="";
}
GlobalHintInfo GHINT;
void DrawGlobalHint(){
	GHINT.Draw();
}
//
SimpleDialog* SD_Hinted=NULL;
SimpleDialog* UserClickSD=NULL;
SimpleDialog* UserRightClickSD=NULL;
//
void ClearGlobalHint(){
	GHINT.HintMessage.Clear();
	SD_Hinted=NULL;
}
void CheckSD_delete(SimpleDialog* SD){	
	SimpleDialog* hsd=SD_Hinted;
	while(hsd){
		if(hsd==SD){
			SD_Hinted=NULL;
		}
		hsd=hsd->Child;
	};	
	if(SD == SD_Hinted){
		SD_Hinted=NULL;
	}
	if(SD == UserClickSD){
		UserClickSD=NULL;
	}
	if(SD == UserRightClickSD){
		UserRightClickSD=NULL;
	}
}
//
void PlayEffectDialogs(int n);
//
SimpleDialog* GetTopDropAcceptor(SimpleDialog* SD){
	SimpleDialog* SDC=NULL;	
	if(SD->Visible){	
		if(SD->AllowDropOverThisDialog && SD->MouseOver)SDC=SD;
		for(int i=0;i<SD->DSS.GetAmount();i++){
			SimpleDialog* SD1=GetTopDropAcceptor(SD->DSS[i]);
			if(SD1)SDC=SD1;
		}                
	}
	return SDC;
}
SimpleDialog* GetTopDropAcceptor(ParentFrame* PF){
    SimpleDialog* SD=NULL;
	for(int i=0;i<PF->DSS.GetAmount();i++){
		SimpleDialog* SD1=GetTopDropAcceptor(PF->DSS[i]);
		if(SD1)SD=SD1;
	}
	return SD;
}
void SetAutoWH(SimpleDialog* PF){
	if(PF->Visible){
		for(int i=0;i<PF->DSS.GetAmount();i++){
			SetAutoWH(PF->DSS[i]);
		}
	}
	DialogsDesk* DD=dynamic_cast<DialogsDesk*>(PF);
	if(DD){
		if(!DD->Visible){
			if(DD->AutoSetWidth){
				DD->SetWidth(0);
			}
			if(DD->AutoSetHeight){
				DD->SetHeight(0);
			}
		}else
		if(DD->AutoSetWidth || DD->AutoSetHeight){
			int maxx=0;		
			int maxy=0;
			int minx=0;		
			int miny=0;
			for(int i=0;i<DD->DSS.GetAmount();i++){
				SimpleDialog* PF=DD->DSS[i];
				if( PF && PF->Visible ){
					int x1=PF->globalX()+PF->x1-PF->x-DD->globalX();
					int y1=PF->globalY()+PF->y1-PF->y-DD->globalY();
					int x0=PF->globalX()-DD->globalX();
					int y0=PF->globalY()-DD->globalY();
					if(x1>maxx)maxx=x1;
					if(y1>maxy)maxy=y1;
					if(x0<minx)minx=x0;
					if(y0<miny)miny=y0;
				}
			}
            maxx+=DD->AddWidth;
            maxy+=DD->AddHeight;
			int l,t,r,b;
			DD->GetMargin(l,t,r,b);
			if(DD->AutoSetWidth){
				if(DD->RightAlign)DD->x=DD->x1-maxx-r;//-minx;
				else DD->x1=DD->x+maxx+r;
			}
			if(DD->AutoSetHeight){
				if(DD->BottomAlign)DD->y=DD->y1-maxy-b;//-miny;
				else DD->y1=DD->y+maxy+b;
			}
			if( DD->EditMode && DD->DSS.GetAmount()==0 ){
				ListDesk* LD=dynamic_cast<ListDesk*>(PF);
				if(LD){
					LD->x1=LD->x+LD->GetWidth()-1;
				}
			}
		}		
	}
	//if(PF->Visible){
	//	for(int i=0;i<PF->DSS.GetAmount();i++){
	//		SetAutoWH(PF->DSS[i]);
	//	}
	//}
}
void DrawRecursive(SimpleDialog* SD0){
	Matrix4D M00;
	int x00=mouseX;
	int y00=mouseY;
	Matrix4D* M=SD0->PushMatrix(M00);
	SD0->_Draw();
	int NDLG=SD0->DSS.GetAmount();
	if(NDLG){		
		for(int i=0;i<NDLG;i++){
			SimpleDialog* SD=SD0->DSS[i];
			if(SD){
				SD->ShiftDialog(SD0->x,SD0->y);
				SD->CoorIsGlobal=true;				
				DrawRecursive(SD);
				SD->ShiftDialog(-SD0->x,-SD0->y);
				SD->CoorIsGlobal=false;
			}
		}		
	}
	SD0->PopMatrix(M);
	mouseX=x00;
	mouseY=y00;	
}
DWORD PushColor(SimpleDialog* SD,DWORD Mod){
	DWORD D=SD->Diffuse;
	SD->Diffuse=ModDWORD(D,Mod);
	GPS.SetCurrentDiffuse(SD->Diffuse);
	return D;
}
void DrawRecursiveWithFlush(SimpleDialog* SD0,DWORD Color){
	Matrix4D M00;
	int x00=mouseX;
	int y00=mouseY;
	Matrix4D* M=SD0->PushMatrix(M00);
	DWORD D=PushColor(SD0,Color);
	SD0->_Draw();	
	ISM->Flush();
	SD0->Diffuse=D;
	int NDLG=SD0->DSS.GetAmount();
	if(NDLG){		
		for(int i=0;i<NDLG;i++){
			SimpleDialog* SD=SD0->DSS[i];
			if(SD){
				SD->ShiftDialog(SD0->x,SD0->y);
				SD->CoorIsGlobal=true;				
				DrawRecursiveWithFlush(SD,Color);
				SD->ShiftDialog(-SD0->x,-SD0->y);
				SD->CoorIsGlobal=false;
			}
		}		
	}
	SD0->PopMatrix(M);
	mouseX=x00;
	mouseY=y00;	
	ISM->SetCurrentDiffuse(0xFFFFFFFFFF);
}

extern bool InfoMode;
bool DisableHotKey=false;
extern bool DrawDialogs;

void DialogsSystem::ProcessDialogs(){
	extern bool GecOptimDialogs;
	if(GecOptimDialogs) return;
	if(!bActive){
		IsDrawActive=true;
		return;
	}
	TempWindow TW0;
	PushWindow(&TW0);
	ProcessAligning();	
	static bool FirstLPress=true;
	bool NeedSelect=Lpressed&&FirstLPress;
	if(!Lpressed)FirstLPress=true;
	void SetStateToLastPress();
	SetStateToLastPress();	
	if(MUSTDRAW){
		MUSTDRAW=false;
		RedrawGameBackground();
		MarkToDraw();
	};
	static int hud=IRS->GetShaderID("hud");
	GPS.SetShader(hud);
	bool UseMouse=true;
	int NDLG=DSS.GetAmount();
	if(ActiveParent&&(mouseX<ActiveX-24||mouseX>ActiveX1+24||mouseY<ActiveY-32||mouseY>ActiveY1+32)){
		ActiveParent->IsActive=0;
		ActiveParent=NULL;
	}
	if(ActiveParent&&mouseX>=ActiveX&&mouseX<=ActiveX1&&mouseY>=ActiveY&&mouseY<=ActiveY1){
		if(ActiveParent->MouseOverActiveZone)ActiveParent->MouseOverActiveZone(ActiveParent,mouseX-ActiveX,mouseY-ActiveY,ActiveID);
		UseMouse=false;
	}else{
		if(ActiveParent&&Lpressed&&(ActiveParent->x>mouseX||ActiveParent->x1<mouseX||ActiveParent->y>mouseY||ActiveParent->y1<mouseY)){
			ActiveParent->IsActive=false;			
			for(int i=0;i<NDLG;i++){
				SimpleDialog* SD=DSS[i];
				if(SD)SD->NeedToDraw=true;
				else break;
			};
			ActiveParent=NULL;
			Lpressed=false;
			UnPress();
		};
	};
	if(ActiveParent)ActiveParent->NeedToDraw=true;
	if( ActiveParent && ActiveParent->OnDrawActive && IsDrawActive && DrawDialogs ){
		TempWindow TW;
		PushWindow(&TW);
		GPS.SetClipArea( 0, 0, RealLx - 1, RealLy - 1 );
		int dx=ActiveParentX-ActiveParent->x;
		int dy=ActiveParentY-ActiveParent->y;
		ActiveParent->ShiftDialog(dx,dy);
		ActiveParent->OnDrawActive(ActiveParent);
		ActiveParent->ShiftDialog(-dx,-dy);
		PopWindow(&TW);        
	}
	//
	SD_Hinted=NULL;
	UserClickSD=NULL;
	UserRightClickSD=NULL;
	//
	DWORD D=ModDWORD(DiffuseModulator,Diffuse);	
	int CD=DiffuseModulator;
	if(DeepColor)DiffuseModulator=D;
	TotalUnPress=0;

	Matrix4D M00;
	int x00=mouseX;
	int y00=mouseY;
	Matrix4D* M=PushMatrix(M00);

	//WasClickOnSmt=true;

	int sh=GPS.GetCurrentShader();

	DisableHotKey=InEdit&&DisableHotKeyInMapEditor&&InfoMode;
	for(int i=0;i<NDLG;i++){
		SimpleDialog* SD=DSS[i];
		if(SD){
			SD->ShiftDialog(x,y);
			SD->CoorIsGlobal=true;
			SD->ParentDS=this;			
			SD->Process(&UserClickSD,&UserRightClickSD,&SD_Hinted);					
			SD->ShiftDialog(-x,-y);
			SD->CoorIsGlobal=false;
		}
	}

	if( DrawDialogs ){
		ISM->Flush();
		GPS.SetShader(sh);
	}

	PopMatrix(M);
	mouseX=x00;
	mouseY=y00;

	for(int i=0;i<DSS.GetAmount();i++){
		SetAutoWH(DSS[i]);
	}

	if(TotalUnPress){
		Lpressed=0;
		UnPress();
	}
	DiffuseModulator=CD;
	extern bool LockMouse;
	if(UserClickSD&&EditMode){
		/*
		void DeselectDSS(ClassArray<SimpleDialog>* DSS);
		ParentFrame* PFR=this;
		while(PFR->ParentDS)PFR=PFR->ParentDS;
		DeselectDSS(&PFR->DSS);
		UserClickSD->Selected=1;
		*/
	}
	static int DragX=0;
	static int DragY=0;
	static int DragStartX=0;
	static int DragStartY=0;
	if( !(LockMouse||EditMode||TotalUnPress) && DrawDialogs ){		
		if(SD_Hinted&&SD_Hinted->MouseSound!=-1){
			static SimpleDialog* sdHint=NULL;
			if(sdHint!=SD_Hinted){
				sdHint=SD_Hinted;
				//if(SD_Hinted&&SD_Hinted->MouseSound!=-1){
					PlayEffectDialogs(SD_Hinted->MouseSound);
				//}				
			}
		}		
		if(UserClickSD){
			if(UserClickSD->ClickSound!=-1){
				PlayEffectDialogs(UserClickSD->ClickSound);
			}
		}
		if(UserRightClickSD && UserRightClickSD->Visible){
			bool played=false;
			if( UserRightClickSD->AllowDrag ){
				SimpleDialog::DraggedDialog.Set(UserRightClickSD);
				DragX=mouseX-UserRightClickSD->LastDrawX;
				DragY=mouseY-UserRightClickSD->LastDrawY;
				DragStartX=mouseX;
				DragStartY=mouseY;
				Rpressed=false;
				UnPress();   
				SimpleDialog::CallParams.set(__OnBeginDrag,UserRightClickSD,UserRightClickSD,NULL);
				UserRightClickSD->_OnBeginDrag();
				UserRightClickSD->CallOnAction(__OnBeginDrag);				
			}else{
				SimpleDialog::CallParams.set(__OnRightClick,UserRightClickSD,NULL,NULL);
				if(UserRightClickSD->CallOnAction(__OnRightClick)){				
					Rpressed=false;
					UnPress();
				}

				if(UserRightClickSD->UserParam==mcmOk&&OkSound!=-1){
					PlayEffect(OkSound,0,0);
					played=true;
				}
				if(UserRightClickSD->UserParam==mcmCancel&&CancelSound!=-1){
					PlayEffect(CancelSound,0,0);
					played=true;
				};
				if(UserClickSound!=-1&&!played){
					PlayEffectDialogs(UserClickSound);
				};
				bool click=UserRightClickSD->OnUserRightClick&&UserRightClickSD->OnUserRightClick(UserRightClickSD);			
				if(UserRightClickSD->v_Actions.RightClick(UserRightClickSD)||click){			
					Rpressed=false;
					UnPress();
				}
			}
		}
		if(UserClickSD && UserClickSD->Visible){
			bool played=false;			
			{
__doclick:
				//bool LP=Lpressed;
				//if(UserClickSD->_Click()){
				//	if(ParentDS)DeactivateDSS(&ParentDS->DSS);
				//	TotalUnPress=1;
				//	Active=true;
				//}
				//Lpressed=LP;

				SimpleDialog::CallParams.set(__OnLeftClick,UserClickSD,NULL,NULL);
				if(UserClickSD->CallOnAction(__OnLeftClick)){
					Lpressed=false;
					UnPress();
				}
				if(UserClickSD){
					if(UserClickSD->UserParam==mcmOk&&OkSound!=-1){
						PlayEffect(OkSound,0,0);
						played=true;
					};
					if(UserClickSD->UserParam==mcmCancel&&CancelSound!=-1){
						PlayEffect(CancelSound,0,0);
						played=true;
					};
					if(UserClickSound!=-1&&!played){
						PlayEffectDialogs(UserClickSound);
					};
					bool InLpress=Lpressed;
					bool click=UserClickSD->OnUserClick&&UserClickSD->OnUserClick(UserClickSD);
					if(UserClickSD&&UserClickSD->v_Actions.LeftClick(UserClickSD)||click){
						Lpressed=false;
						UnPress();
					}
					if(InLpress&&Lpressed==false){
						UnPress();
						extern bool IgnoreLUnpress;
						extern bool IgnoreRUnpress;
						IgnoreLUnpress=1;
						IgnoreRUnpress=1;
					}
				}
			}
		}
	}
	static int LastDragTime=0;
	static int DragDistance=0;
	static int DragMouseShift=15;
	if( SimpleDialog::DraggedDialog.Get() && DrawDialogs ){
        SimpleDialog* TopDrop=GetTopDropAcceptor((ParentFrame*)this);
		SimpleDialog* SD=SimpleDialog::DraggedDialog.Get();
		int DX=mouseX+DragMouseShift-SD->x;
		//int DX=mouseX-SD->x-DragX;        
		int DY=mouseY-SD->y-DragY;
		int N=Norma(mouseX-DragStartX,mouseY-DragStartY);
		if(N<=DragDistance){
			DX=DragStartX-SD->x-DragX;        
			DY=DragStartY-SD->y-DragY;
		}
		extern bool realRpressed;
		if(realRpressed){
			LastDragTime=GetTickCount();
			SD->x+=DX;
			SD->y+=DY;
			SD->x1+=DX;
			SD->y1+=DY;
			DrawRecursiveWithFlush(SD,0xFFFFFFFF);

			SD->x-=DX;
			SD->y-=DY;
			SD->x1-=DX;
			SD->y1-=DY;
			if(N>=DragDistance){
				SimpleDialog::CallParams.set(__OnDrag,SD,SD,TopDrop);
				SD->_OnDrag();
				SD->CallOnAction(__OnDrag);							
			}
		}else{
			if(TopDrop){
				if(N>=DragDistance){
					SimpleDialog::DraggedDialog.Set(NULL);
					SimpleDialog::CallParams.set(__OnDropOnThis,NULL,SD,TopDrop);
					if(TopDrop){				
						TopDrop->_OnDropOnThis();
						TopDrop->CallOnAction(__OnDropOnThis);
					}
					SimpleDialog::CallParams.set(__OnDropThis,NULL,SD,TopDrop);
					SD->_OnDropThis();
					SD->CallOnAction(__OnDropThis);
					SimpleDialog::CallParams.Dragged=NULL;
				}else{
					SimpleDialog::DraggedDialog.Set(NULL);
					UserClickSD=SD;
					SimpleDialog::CallParams.Dragged=NULL;
					goto __doclick;
				}
			}else{
				if(GetTickCount()-LastDragTime>500 || N<=DragDistance){
					if(N<DragDistance){
						SimpleDialog::DraggedDialog.Set(NULL);
						UserClickSD=SD;
						SimpleDialog::CallParams.Dragged=NULL;
						goto __doclick;
					}else{
						SimpleDialog::DraggedDialog.Set(NULL);
						SimpleDialog::CallParams.Dragged=NULL;
					}
				}
			}
		}
	}
	Hint=NULL;	
	if(EditMode){
		static int PrevUnpMX=-1;
		static int PrevUnpMY=0;
		static int pmx;
		static int pmy;
		static int PrevTime=GetTickCount();
		static int StayTime=0;
		if(pmx==mouseX&&pmy==mouseY){
			StayTime+=GetTickCount()-PrevTime;
		}else StayTime=0;
		bool ShowRect=!(GetKeyState(VK_MENU)&0x8000);
		PrevTime=GetTickCount();
		extern bool realRpressed;
		int xd,yd;
		if(StayTime>50){
			ShiftDSS(x,y,&DSS);
			SimpleDialog* SD=SelectNearestFrame(&DSS,mouseX,mouseY,xd,yd);						
			ShiftDSS(-x,-y,&DSS);
			if(SD){
				int dx=xd-SD->x;
				int dy=yd-SD->y;
				DWORD A=100;
				if(ShowRect)DrawColoredRect(SD->x+dx,SD->y+dy,SD->x1-SD->x+1,SD->y1-SD->y+1,0x60FF0000,70);
			}			
		}
		pmx=mouseX;
		pmy=mouseY;
		if(ShowRect)DrawSelectedFrames(&DSS,x,y);
		if(NeedSelect){
			void DeselectDSS(ClassArray<SimpleDialog>* DSS);
            if(!(GetKeyState(VK_SHIFT)&0x8000))DeselectDSS(&DSS);
			ShiftDSS(x,y,&DSS);
			SimpleDialog* SD=SelectNearestFrame(&DSS,mouseX,mouseY,xd,yd);
			ShiftDSS(-x,-y,&DSS);
			if(SD)SD->Selected=1-SD->Selected;			
			FirstLPress=false;			
		}
		if(!realRpressed){
			PrevUnpMX=mouseX;
			PrevUnpMY=mouseY;
		}
		static int ptime=GetTickCount();
		int mdx=(((GetKeyState(VK_LEFT)&0x8000)?-1:0)+((GetKeyState(VK_RIGHT)&0x8000)?1:0))*((GetKeyState(VK_MENU)&0x8000)?10:1);
		int mdy=(((GetKeyState(VK_UP)&0x8000)?-1:0)+((GetKeyState(VK_DOWN)&0x8000)?1:0))*((GetKeyState(VK_MENU)&0x8000)?10:1);
		if(realRpressed||((mdx||mdy)&&GetTickCount()-ptime>100&&!LockManualDialogsShift)){
			ptime=GetTickCount();
			int dx=realRpressed?mouseX-PrevUnpMX:mdx;
			int dy=realRpressed?mouseY-PrevUnpMY:mdy;
			SimpleDialog* SDS[128];
			int N=EnumerateSelectedFrames(&DSS,SDS,127);
			for(int i=0;i<N;i++){
				if(GetKeyState(VK_SHIFT)&0x8000){
					SDS[i]->ResizeDialog(dx,dy);
				}else SDS[i]->ShiftDialog(dx,dy);
			}
			PrevUnpMX=mouseX;
			PrevUnpMY=mouseY;
			if(N){
				Lpressed=0;
				UnPress();
			}
		}
	}
	/*
	for(i=0;i<NDLG;i++){
		SimpleDialog* SD=DSS[i];
		if(SD){
			
			if(SD->MouseOver&&SD->Visible&&SD->Hint){
				Hint=SD->Hint;
				SD_Hinted=SD;
			};
			if(SD->MouseOver&&!SD->PrevMouseOver){
				if(SD->MouseSound!=-1)PlayEffect(SD->MouseSound,0,0);
			};
			SD->PrevMouseOver=SD->MouseOver;
		}else break;
	};*/
	/*
	if(SD_Hinted&&SD_Hinted->Child){
		while(SD_Hinted){
			if(SD_Hinted->OnDraw&&SD_Hinted->Visible)DrawSD(SD_Hinted);
			SD_Hinted=SD_Hinted->Child;
		};
	};
	*/
	if(SD_Hinted && SD_Hinted->Visible)Hint=SD_Hinted->Hint;
	TempWindow TW;
	if( ActiveParent && ActiveParent->OnDrawActive && IsDrawActive && DrawDialogs ){
		TempWindow TW;
		PushWindow(&TW);
		GPS.SetClipArea( 0, 0, RealLx - 1, RealLy - 1 );
		int dx=ActiveParentX-ActiveParent->x;
		int dy=ActiveParentY-ActiveParent->y;
		ActiveParent->ShiftDialog(dx,dy);
		ActiveParent->OnDrawActive(ActiveParent);
		ActiveParent->ShiftDialog(-dx,-dy);
		PopWindow(&TW);        
	}
	PushWindow(&TW);
#ifndef _USE3D
	WindX=0;
	WindY=0;
	WindLx=RealLx;
	WindLy=RealLy;
	WindX1=WindLx-1;
	WindY1=WindLy-1;
#else
	//GPS.SetClipArea( 0, 0, RealLx, RealLy );
#endif // !_USE3D
	if(!Hint&&DefaultHint[0]){
		Hint=DefaultHint.pchar();
	};	
	if(!PrevHintTime)PrevHintTime=GetTickCount();
	if(HintX==-1){
		HintX=20;
		HintLX=HintY;
		HintY=20;
		HintLY=1000;
		HintBackColor=0xFFFFFFFF;
		HintFontColor=0;
		HintMovesWithMouse=1;
	}
	if((mouseX==PrevHintX&&mouseY==PrevHintY&&GetTickCount()-PrevHintTime>HintFadeTime)||HintFadeTime==0){
		if(HintCallback){
			HintCallback(this,SD_Hinted,Hint,mouseX,mouseY,GetTickCount()-PrevHintTime-HintFadeTime);
		}else{
			if(Hint&&HintFont&&!GSets.DisableHint&&v_MainMenu.Cross==0){
                GHINT.HintFont=HintFont;
				GHINT.HintMessage=Hint;
				GHINT.HintStyle=HintStyle;
				GHINT.HintLineDistance=HintLineDistance;
				GHINT.HintLX=HintLX;
				GHINT.HintLY=HintLY;
				GHINT.x=x;
				GHINT.y=y;
				GHINT.x1=x1;
				GHINT.y1=y1;
				GHINT.HintX=HintX;
				GHINT.HintY=HintY;
				GHINT.HintFade=HintFade;
				GHINT.HintMovesWithMouse=HintMovesWithMouse;
				GHINT.HintBorder=HintBorder;
				GHINT.HintAligningX=HintAligningX;
				GHINT.HintAligningY=HintAligningY;
				GHINT.HintFadeTime=HintFadeTime;
				GHINT.HintBackColor=HintBackColor;
				GHINT.HintFontColor=HintFontColor;
				GHINT.HintCallback=HintCallback;
				GHINT.HintBorderCallback=HintBorderCallback;
				GHINT.CenteredHint=CenteredHint;
				GHINT.HintFontAlign=HintFontAlign;
				GHINT.HintXYisGlobal=HintXYisGlobal;
			}
		}
	}
	if(PrevHintX!=mouseX||PrevHintY!=mouseY){
		PrevHintX=mouseX;
		PrevHintY=mouseY;
		PrevHintTime=GetTickCount();
	}
	//PopWindow(&TW);		
	PushWindow(&TW0);
	if(WasClickOnSmt)MFix();
	WasClickOnSmt=false;
};
void DeactivateDSS0(ClassArray<SimpleDialog>* DSS){	
	int N=DSS->GetAmount();
	for(int i=0;i<N;i++){
		(*DSS)[i]->Active=0;
		if((*DSS)[i]->DSS.GetAmount()){
			DeactivateDSS0(&((*DSS)[i]->DSS));
		}
	}
}
void DeactivateDSS(ClassArray<SimpleDialog>* DSS){	
	BaseClass* B=DSS;
	while(B){
		B=B->GetParent();
		if(B){
			ClassArray<SimpleDialog>* D=dynamic_cast< ClassArray<SimpleDialog>* >(B);
			if(D){
				DSS=D;
			}
		}
	}    
	int N=DSS->GetAmount();
	for(int i=0;i<N;i++){
		(*DSS)[i]->Active=0;
		if((*DSS)[i]->DSS.GetAmount()){
			DeactivateDSS0(&((*DSS)[i]->DSS));
		}
	}
}
bool CheckActiveDSS(ClassArray<SimpleDialog>* DSS){
	int N=DSS->GetAmount();
	for(int i=0;i<N;i++){
		if((*DSS)[i]->Active)return true;
		if((*DSS)[i]->DSS.GetAmount()){
			if(CheckActiveDSS(&((*DSS)[i]->DSS)))return true;
		}
	}
	return false;
}
void DeselectDSS(ClassArray<SimpleDialog>* DSS){
	int N=DSS->GetAmount();
	for(int i=0;i<N;i++)if((*DSS)[i]){
		(*DSS)[i]->Selected=0;
		if((*DSS)[i]->DSS.GetAmount()){
			DeselectDSS(&((*DSS)[i]->DSS));
		}
	}
}
void SelectDialogByPtr(ClassArray<SimpleDialog>* DSS,void* ptr){
	int N=DSS->GetAmount();
	for(int i=0;i<N;i++){
		if(((void*)((*DSS)[i]))==ptr){
            (*DSS)[i]->Selected=true;
		}		
		if((*DSS)[i]->DSS.GetAmount()){
			SelectDialogByPtr(&((*DSS)[i]->DSS),ptr);
		}
	}    
}
//
char* GetKeyName(word Key);
extern EngineSettings EngSettings;
//
void SimpleDialog::SetHint(_str* s, bool addHotKey){
	HintWithHotKey=false;
	if(Hint&&AllocHint)delete(Hint);
	Hint=NULL;
	if(s->pchar()&&s->pchar()[0]){
		char* text=s->pchar();
		if(addHotKey){
			//char key[2]={HotKey,0};
			if(EngSettings.HintTemplate.HotKey.isClear()){
				(*s)+=" ({CG}";
				if(HotCtrl){
					(*s)+="Ctrl+";
				}
				if(HotShift){
					(*s)+="Shift+";
				}
				if(HotAlt){
					(*s)+="Alt+";
				}
				s->print("%s{C})",GetTextByID(GetKeyName(HotKey)));
			}else{
				s->Add(" ");
				s->print(EngSettings.HintTemplate.HotKey.pchar(),GetTextByID(GetKeyName(HotKey)));
			}				
			HintWithHotKey=true;
			text=s->pchar();
		}else{
			text=GetTextByID(s->pchar());
		}
		if( /*text && */text[0] ){
			//if(text!=s->pchar()){
			//	Hint=text;
			//	AllocHint=0;
			//}else{
			Hint=znew(char,strlen(text)+1);
			strcpy(Hint,text);
			AllocHint=1;
			//}
		}
	}
}
//
extern bool ChangeNation;
extern bool EditMapMode;
bool TotalUnPress=0;
Matrix4D* ParentFrame::PushMatrix(Matrix4D& M0){
	const Matrix4D* M=GPS.GetScreenSpaceTM();
	if(M){
		M0=*M;
		M=&M0;
	}
	if(EnableTransform){
		Matrix4D M1;
		Matrix4D M2;
		Matrix4D* MM;
		if(M){			
			GetMatrix(M2);
			MM=&M2;
			M1=*M;
			M1.mulLeft(M2);
		}else{
			GetMatrix(M1);
			MM=&M1;
		}
		GPS.SetScreenSpaceTM(&M1);
		Matrix4D M3;
		M3.inverse(*MM);
		Vector3D V(mouseX,mouseY,0);
		M3.transformPt(V);
		mouseX=V.x;
		mouseY=V.y;
	}	
	return (Matrix4D*)M;
}
void ParentFrame::PopMatrix(Matrix4D* M){
	GPS.SetScreenSpaceTM(M);
}
extern bool EnterChatMode;
//void SimpleDialog::Process(SimpleDialog** LeftClick,SimpleDialog** RightClick,SimpleDialog** HintDlg){
//	CheckVirtTBL(this);
//	// vui_Action
//	bool Act=false;
//	if((!(EditMode||Visible))||GetKeyState(VK_SCROLL)){
//		v_Actions.SetFrameState(this);
//		Act=true;
//	}	
//	if(!Visible) return;
//	int dd=0;
//	try{
//		if(ParentSB&&ParentSB->Visible)dd=ParentSB->SPos;
//	}catch(...){};
//	ShiftDialog(0,-dd);	
//	//
//	Rct VP=GPS.GetClipArea();
//	bool clip=y1>=VP.y||y<-VP.GetBottom();
//	//	
//	Matrix4D M0;
//	Matrix4D* M=PushMatrix(M0);
//	int x0=mouseX;
//	int y0=mouseY;
//	//
//	if(clip){		
//		if(Active){
//			if(OnKeyDown){
//				if(KeyPressed&&OnKeyDown(this))KeyPressed=false;
//			};
//		};
//		if(GPS.GetClipArea().PtIn( mouseX, mouseY )){
//			CheckMouseOver(mouseX,mouseY);
//			if(MouseOver&&Enabled){
//				if(CheckProcessAbility()){
//					if(OnMouseOver)OnMouseOver(this);
//					extern bool NoPress;
//					if(Lpressed&&!NoPress){
//						if(EditMode){
//							if(GetKeyState(VK_MENU)&0x8000){
//								bool LP=Lpressed;
//								if(_Click()){
//									if(ParentDS)DeactivateDSS(&ParentDS->DSS);							
//									TotalUnPress=1;							
//								}
//								Lpressed=LP;
//							}else{
//								bool MakeClipping(SimpleDialog* SD);
//								if(OnDraw&&OnDraw!=&MakeClipping)*LeftClick=this;
//							}
//						}else{
//							if(OnUserClick||v_Actions.isLeftClick()){
//								*LeftClick=this;
//							}
//							bool LP=Lpressed;
//							if(_Click()){
//								if(ParentDS)DeactivateDSS(&ParentDS->DSS);
//								//Lpressed=false;
//								TotalUnPress=1;
//								Active=true;
//							}
//							Lpressed=LP;
//						}
//					}
//					if(Rpressed&&!EditMode){
//						if(OnUserRightClick||v_Actions.isRightClick()){
//							*RightClick=this;
//						}
//					}
//					if(Hint&&Hint[0])*HintDlg=this;
//				}
//			}
//		}else{
//			MouseOver=false;
//		}
//	}
//	if(!(EditMode||Act)){
//		v_Actions.SetFrameState(this);
//		if(!Visible){
//			ShiftDialog(0,dd);
//			mouseX=x0;
//			mouseY=y0;
//			return;
//		}
//	}
//	if(clip){
//		DWORD D=ModDWORD(DiffuseModulator,Diffuse);
//		GPS.SetCurrentDiffuse(D);
//		_Draw();
//		GPS.SetCurrentDiffuse(0xFFFFFFFF);
//		int CD=DiffuseModulator;
//		if(DeepColor)DiffuseModulator=D;
//		int N=DSS.GetAmount();
//		if(N){
//			int l,r,u,d,sx,sy;
//			GetMargin(l,u,r,d);
//			GetShift(sx,sy);
//			TempWindow TW;
//			PushWindow(&TW);
//			IntersectWindows(x+l,y+u,x1-r,y1-d);
//			int i;
//			for(i=0;i<N;i++)if(DSS[i]){
//				//try{
//				DSS[i]->ShiftDialog(x+l-sx,y+u-sy);				
//				//}catch(...){
//
//				//}
//			}
//			for(i=0;i<N;i++)if(DSS[i]){
//				DSS[i]->MouseOver=0;
//				DSS[i]->ParentDS=this;
//				DSS[i]->Process(LeftClick,RightClick,HintDlg);
//			}
//			for(i=N-1;i>=0;i--)if(DSS[i]){
//				DSS[i]->ShiftDialog(-x-l+sx,-y-u+sy);
//			}
//			PopWindow(&TW);
//		}	
//		DiffuseModulator=CD;
//		PopMatrix(M);		
//	}
//	mouseX=x0;
//	mouseY=y0;
//	LastDrawX=x;
//	LastDrawY=y;	
//	if(HotKey&&Enabled&&LastKey==HotKey&&!(EditMode||EnterChatMode)/*&&(!ChangeNation&&!EditMapMode)*/){
//		*LeftClick=this;
//		LastKey=0;
//		//KeyPressed=0;
//	}
//	ShiftDialog(0,dd);
//}

ClassRef<SimpleDialog> SimpleDialog::DraggedDialog;
ActionCallParams SimpleDialog::CallParams;

bool SimpleDialog::CallOnAction(int ActionID){
	bool done=false;
	for(int i=0;i<ConnectActionToBaseFunction.GetAmount();i++){
		if(ConnectActionToBaseFunction[i]->ReasonToCall==ActionID){
			ConnectActionToBaseFunction[i]->Call();            			
			done=true;
		}
	}
	return done;
}
bool SimpleDialog::HaveAction(int ActionID){
	for(int i=0;i<ConnectActionToBaseFunction.GetAmount();i++){
		if(ConnectActionToBaseFunction[i]->ReasonToCall==ActionID)return true;			
	}
    return false;
}

void SimpleDialog::Process(SimpleDialog** LeftClick,SimpleDialog** RightClick,SimpleDialog** HintDlg){
	if(DraggedDialog.Get()==this){
		if(!EditMode){
			ConnectingDialogToClassMembers._excahnge();
			v_Actions.SetFrameState(this);			
		}
		return;
	}
	CheckVirtTBL(this);
	//connection to variables	
	if(!EditMode)ConnectingDialogToClassMembers._excahnge();
	// vui_Action
	bool Act=false;
	if((!(EditMode||Visible))||GetKeyState(VK_SCROLL)){
		v_Actions.SetFrameState(this);
		Act=true;
	}	
	if(!Visible){
		MouseOver=false;
		ProcessLocalAligning();
		return;
	}
	int dd=0;
	//try{
		if(ParentSB&&ParentSB->Visible)dd=ParentSB->SPos;
	//}catch(...){};
	ShiftDialog(0,-dd);	

	Rct VP=GPS.GetClipArea();
	//bool clip=y1>=VP.y||y<-VP.GetBottom();
	bool clip=y1>=VP.y||y<VP.GetBottom();
	Matrix4D* M;
	int x0,y0,CD;
	if( clip && DrawDialogs ){
		Matrix4D M0;
		x0=mouseX;
		y0=mouseY;
		M=PushMatrix(M0);
		if(Active){
			if(OnKeyDown){
				if(KeyPressed&&OnKeyDown(this))KeyPressed=false;
			};
		};
		if(GPS.GetClipArea().PtIn( mouseX, mouseY )){
			CheckMouseOver(mouseX,mouseY);
			if(MouseOver&&Enabled){
				if(CheckProcessAbility()){
					if(OnMouseOver)OnMouseOver(this);
					extern bool NoPress;
					if(Lpressed&&!NoPress){
						WasClickOnSmt=true;
						if(EditMode){
							if(GetKeyState(VK_MENU)&0x8000){
								bool LP=Lpressed;
								if(_Click()){
									if(ParentDS)DeactivateDSS(&ParentDS->DSS);							
									TotalUnPress=1;							
								}
								Lpressed=LP;
							}else{
								bool MakeClipping(SimpleDialog* SD);
								if(OnDraw&&OnDraw!=&MakeClipping)*LeftClick=this;
							}
						}else if(Visible){
							if((OnUserClick||v_Actions.isLeftClick()||HaveAction(__OnLeftClick)||AllowDrag)){
								*LeftClick=this;
							}
							bool LP=Lpressed;
							if(_Click()){
								if(ParentDS)DeactivateDSS(&ParentDS->DSS);
								TotalUnPress=1;
								Active=true;
							}
							Lpressed=LP;
						}
					}
					if(Rpressed&&!EditMode){
						if(OnUserRightClick||v_Actions.isRightClick()||HaveAction(__OnRightClick)){
							if(Visible)*RightClick=this;
						}
					}
					if(Hint&&Hint[0])*HintDlg=this;
				}
			}
		}else{
			MouseOver=false;
		}		
	}
	if(!(EditMode||Act)){
		v_Actions.SetFrameState(this);		
		if(!Visible){
			LastDrawX=x;
			LastDrawY=y;	
			ShiftDialog(0,dd);
			return;
		}
	}
	ShiftDialog(0,dd);
	ProcessLocalAligning();
	ShiftDialog(0,-dd);

	if( clip ){		
		if(DrawDialogs){
			DWORD D=ModDWORD(DiffuseModulator,Diffuse);
			GPS.SetCurrentDiffuse(D);

			if(ForceFlushSprites){
				ISM->Flush();
			}
			if(DlgBatchEnabled){            
				AddDlqToQueue(this);				
			}else{
				if(EnableLevelSorting){
					DlgBatchEnabled=true;                
				}
				_Draw();

				for(int i=0;i<v_Actions.GetAmount();i++)if(v_Actions[i])v_Actions[i]->OnDraw(this);				
			}
			if(ForceFlushSprites){
				ISM->Flush();
				rsFlush();
			}		

			CD=DiffuseModulator;
			if(DeepColor)DiffuseModulator=D;
			int N=DSS.GetAmount();
			if(N){
				int l,r,u,d,sx,sy;
				GetMargin(l,u,r,d);
				GetShift(sx,sy);
				TempWindow TW;
				if(!(DisableClipping||DlgBatchEnabled)){
					PushWindow(&TW);
					IntersectWindows(x+l,y+u,x1-r,y1-d);
				}
				int i;
				for(i=0;i<N;i++)if(DSS[i]){				
					DSS[i]->ShiftDialog(x+l-sx,y+u-sy);
					DSS[i]->CoorIsGlobal=true;
				}
				if(DlgBatchEnabled)IncDlgLevel(65536);
				for(i=0;i<N;i++)if(DSS[i]){
					DSS[i]->MouseOver=0;
					DSS[i]->ParentDS=this;
					subIdx=i;
					DSS[i]->Process(LeftClick,RightClick,HintDlg);                
				}
				if(DlgBatchEnabled)IncDlgLevel(-65536);
				if(EnableLevelSorting && CurrentDlgLevel==0){
					DrawDlgBatches();
					DlgBatchEnabled=false;                
				}
				for(i=0;i<N;i++)if(DSS[i]){
					DSS[i]->ShiftDialog(-x-l+sx,-y-u+sy);
					DSS[i]->CoorIsGlobal=false;
				}
				if(!(DisableClipping||DlgBatchEnabled))PopWindow(&TW);
			}

			DiffuseModulator=CD;
			PopMatrix(M);
			mouseX=x0;
			mouseY=y0;

		}else{
			int i;
			int N=DSS.GetAmount();
			for(i=0;i<N;i++)if(DSS[i]){
				DSS[i]->MouseOver=0;
				DSS[i]->ParentDS=this;
				subIdx=i;
				DSS[i]->Process(LeftClick,RightClick,HintDlg);
			}
		}
	}	
	GPS.SetCurrentDiffuse(0xFFFFFFFF);
	LastDrawX=x;
	LastDrawY=y;	
	if( HotKey&&Enabled&&LastKey==HotKey&&!(EditMode||EnterChatMode||DisableHotKey)/*&&(!ChangeNation&&!EditMapMode)*/ && DrawDialogs ){
		if(!KeysAreFocusedOnInputBox||LastKey==VK_ESCAPE||LastKey==VK_RETURN){
			
			bool Ctrl=GetKeyState(VK_CONTROL)&0x8000;
			bool Shift=GetKeyState(VK_SHIFT)&0x8000;
			bool Alt=GetKeyState(VK_MENU)&0x8000;

			if( HotCtrl==Ctrl && HotShift==Shift && HotAlt==Alt ){
				*LeftClick=this;
				LastKey=0;

				bool LP=Lpressed;
				if(_Click()){
					if(ParentDS)DeactivateDSS(&ParentDS->DSS);
					TotalUnPress=1;
					Active=true;
				}
				Lpressed=LP;
			}
		}
	}
	ShiftDialog(0,dd);
}
void ParentFrame::vm_ActionsAccept(){	
	if(!EditMode){
		SimpleDialog* sd=dynamic_cast<SimpleDialog*>(this);
		if(sd){
			if(sd->Visible){
				//if(sd->v_ActionID) ((vui_Action*)sd->v_ActionID)->Accept(sd);
				sd->v_Actions.Accept(sd);
			}else{
				return;
			}			
		}		
		int N=DSS.GetAmount();
		for(int i=0;i<N;i++){
			SimpleDialog* SD=DSS[i];
			if(SD) SD->vm_ActionsAccept();		
		}
	}
};
void ParentFrame::vm_ActionsCancel(){
	if(!EditMode){
		SimpleDialog* sd=dynamic_cast<SimpleDialog*>(this);
		if(sd){			
			////// if(sd->v_ActionID) ((vui_Action*)sd->v_ActionID)->init=false;
			sd->vm_Init=false;
		}
		int N=DSS.GetAmount();
		for(int i=0;i<N;i++){
			SimpleDialog* SD=DSS[i];
			if(SD) SD->vm_ActionsCancel();
		}
	}
};

void RedrawOffScreenMouse();
void OnMouseMoveRedraw();
void PostRedrawMouse();
void SERROR();
void SERROR1();
void SERROR2();
extern bool InGame;
extern bool RecordMode;
void ShowTMT();
extern bool InMainMenuLoop;
extern bool RUNMAPEDITOR;
extern bool RUNUSERMISSION;
////extern int ItemChoose;
extern int MouseNext[256];
int PrevCHhange=0;
extern bool test16;
void DialogsSystem::RefreshView(){
	if(!InMainMenuLoop){
		if(RUNUSERMISSION||RUNMAPEDITOR){
			ItemChoose=mcmCancel;
		};
	};
	void InternalProcess();
	InternalProcess();
	/*if(!bActive){
		void InternalProcess();
		InternalProcess();
		Sleep(20);
		return;
	}*/
	int T0=GetTickCount();
	if(!PrevCHhange)PrevCHhange=T0-1000;
	if(T0-PrevCHhange>60){
		//curptr=MouseNext[curptr];
		PrevCHhange=T0;
	};
	if(!DDDebug){
		
		//if(InGame)SERROR1();
		ShowTMT();
		RedrawOffScreenMouse();
		if(test16){
			FlipPages();
		}else{
			CopyToScreen(0,0,RealLx,RSCRSizeY);
		};
		PostRedrawMouse();
		
		//FlipPages();
		//if(InGame)SERROR1();
	}else{
		if(DDDebug)RSCRSizeX=SCRSizeX;
		RedrawOffScreenMouse();
		PostRedrawMouse();
		//LockMouse=false;
		FlipPages();
	};
};
SimpleDialog::~SimpleDialog(){	
	void CheckSD_delete(SimpleDialog* SD);
	CheckSD_delete(this);
	if(AllocHint&&Hint)free(Hint);
	if(AllocPtr)free(AllocPtr);
	AllocPtr=NULL;
	if(Destroy)Destroy(this);
}
void DialogsSystem::CloseDialogs(){
	//ActiveX=0;
	//ActiveY=0;
	//ActiveX1=-1;
	//ActiveY1=-1;
	ParentFrame::CloseDialogs();
    ActiveParent=NULL;
}
void KeyTestMem();
void test_class(BaseClass* BC){
	/*if(BC->CheckDirectCasting()){
		int N=BC->GetAmountOfElements();
		for(int j=0;j<N;j++){
			BaseClass* BE=BC->GetElementClass(j);
			if(BE&&BE->CheckDirectCasting()){
				BaseClass* BE1=(BaseClass*)BC->GetElementPtr(j,BC);
				test_class(BE1);
			}
		}
	}	*/
}
void test_class2(BaseClass* BC){
	if(BC->CheckDirectCasting()){
		int N=BC->GetAmountOfElements();
		for(int j=0;j<N;j++){
			BaseClass* BE=BC->GetElementClass(j);
			if(BE&&BE->CheckDirectCasting()){
				BaseClass* BE1=(BaseClass*)BC->GetElementPtr(j,BC);
				test_class2(BE1);
			}
		}
	}
}
void ParentFrame::CloseDialogs(){
	test_class(&DSS);
	DSS.Clear();

};
int GetSound(char* Name);
bool ParentFrame::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	if(ReferableBaseClassWithSource::Load(xml,ClassPtr,Error,Extra)){
		int n=DSS.GetAmount();
		for(int i=0;i<n;i++){
			DSS[i]->ParentDS=this;
		}		
		return true;
	}
	return false;
};
void DialogsSystem::MarkToDraw(){
};

//----------end of handler of the dialogsystem--------//

//transparency effect

byte* TransPtr=NULL;
int TransLx=0;
int TransLy=0;
void MakeTranspSnapshot(){
	if(RealLx!=TransLx||RealLy!=TransLy){
		TransPtr=(byte*)realloc(TransPtr,RealLx*RealLy);
		TransLx=RealLx;
		TransLy=RealLy;
	};
	for(int i=0;i<RealLy;i++){
		memcpy(TransPtr+i*RealLx,(byte*)ScreenPtr+i*ScrWidth,RealLx);
	};
};
void FreeTransBuffer(){
	if(TransPtr)free(TransPtr);
	TransPtr=NULL;
	TransLx=0;
	TransLy=0;
};
void EncodeLine(byte* src,byte* dst,byte* scr,byte* tbl,byte* oddtbl){
	int N=TransLx>>1;
	__asm{
		push esi
		push edi
		push ebx
		mov  esi,tbl
		mov  edi,oddtbl
		mov  ebx,scr
		mov  edx,src
		mov  ecx,dst
LPP1:
		mov  al,[edx]
		mov  ah,[ecx]
		mov  al,[esi+eax]
		mov  [ebx],al
		inc  edx
		inc  ecx
		inc  ebx
		mov  al,[edx]
		mov  ah,[ecx]
		mov  al,[edi+eax]
		mov  [ebx],al
		inc  edx
		inc  ecx
		inc  ebx
		dec  N
		jnz  LPP1

		pop  ebx
		pop  edi
		pop  esi
	};
};
void EncodeLine1(byte* src,byte* dst,byte* scr,byte* tbl){
	int N=TransLx>>1;
	__asm{
		push esi
		push edi
		push ebx
		mov  esi,tbl
		mov  edi,tbl
		mov  ebx,scr
		mov  edx,src
		mov  ecx,dst
LPP1:
		mov  al,[edx]
		mov  ah,[ecx]
		mov  al,[esi+eax]
		mov  [ebx],al
		inc  edx
		inc  ecx
		inc  ebx
		mov  ah,[ecx]
		mov  [ebx],ah
		inc  edx
		inc  ecx
		inc  ebx
		dec  N
		jnz  LPP1

		pop  ebx
		pop  edi
		pop  esi
	};
};
void EncodeLine2(byte* src,byte* dst,byte* scr,byte* tbl){
	int N=TransLx>>1;
	__asm{
		push esi
		push edi
		push ebx
		mov  esi,tbl
		mov  edi,tbl
		mov  ebx,scr
		mov  edx,src
		mov  ecx,dst
LPP1:
		mov  ah,[ecx]
		mov  [ebx],ah
		inc  edx
		inc  ecx
		inc  ebx
		mov  al,[edx]
		mov  ah,[ecx]
		mov  al,[edi+eax]
		mov  [ebx],al
		inc  edx
		inc  ecx
		inc  ebx
		dec  N
		jnz  LPP1

		pop  ebx
		pop  edi
		pop  esi
	};
};

void PerformTransMix(int degree){
};
int Time0=0;
void StartMixing(){
#ifndef _USE3D
	Time0=GetTickCount();
	MakeTranspSnapshot();
#endif
};
void ProcessMixing(){
#ifndef _USE3D
	int idx=(GetTickCount()-Time0)/50;
	if(idx<=8){
		idx--;
		if(idx<0)idx=0;
		if(idx>6)idx=6;
		if(TransPtr){
			PerformTransMix(6-idx);
		};
	}else{
		FreeTransBuffer();
	};
#endif
};
SimpleDialog* ParentFrame::Find(DWORD ID){
	int N=DSS.GetAmount();
	for(int i=0;i<N;i++)if(DSS[i]){
		if(DSS[i]->ID==ID)return DSS[i];
		else{
			SimpleDialog* SD=DSS[i]->Find(ID);
			if(SD)return SD;
		}
	}
	return NULL;
}
bool ParentFrame::DeleteItem(DWORD ID){
	bool Done=0;
	int N=DSS.GetAmount();
	for(int i=0;i<N;i++)if(DSS[i]&&DSS[i]->ID==ID){
		delete(DSS[i]);
		DSS.Del(i,1);
		i--;
		N--;
	}
	return Done;
}
SimpleDialog* ParentFrame::Find(char* ID){
	if(ID){
		int N=DSS.GetAmount();
		for(int i=0;i<N;i++)if(DSS[i]){
			if(DSS[i]->Name.pchar()&&!strcmp(DSS[i]->Name.pchar(),ID))return DSS[i];
			else{
				SimpleDialog* SD=DSS[i]->Find(ID);
				if(SD)return SD;
			}
		}
	}
	return NULL;
}
bool ParentFrame::DeleteItem(char* ID){
	bool Done=0;
	int N=DSS.GetAmount();
	for(int i=0;i<N;i++)if(DSS[i]&&DSS[i]->Name.pchar()&&!strcmp(DSS[i]->Name.pchar(),ID)){
		SimpleDialog* SD=DSS[i];
		delete(SD);
		DSS.Del(i,1);
		i--;
		N--;		
	}
	return Done;
}
void SimpleDialog::AssignID(DWORD V){ID=V;}
void SimpleDialog::AssignSID(char* SV){
	Name.Assign(SV);
}
void DialogsSystem::SetHintStyle(bool MotionWithMouse,bool Border,bool Fade,
					  int FadeTime,byte AligningX,byte AligningY,
					  int dx,int dy,int MaxLx,int MaxLy,RLCFont* Font,
					  DWORD BackColor,DWORD FontColor){
	HintMovesWithMouse=MotionWithMouse;
	HintBorder=Border;
	HintFade=Fade;
	HintFadeTime=FadeTime;
	HintAligningX=AligningX;
	HintAligningY=AligningY;
	HintX=dx;
	HintY=dy;
	HintLX=MaxLx;
	HintLY=MaxLy;
    HintBackColor=BackColor;
	HintFontColor=FontColor;
	HintFont=Font;
	HintStyle=1;
}
//DialogsDesk
DialogsDesk::~DialogsDesk(){
    if(VScroller)delete(VScroller);
	if(HScroller)delete(HScroller);
	VScroller=NULL;
	HScroller=NULL;
}
ClassEditorConnector::ClassEditorConnector(){
    CE=NULL;
    PrevDataPtr=NULL;
}
ClassEditorConnector::~ClassEditorConnector(){
    if(CE)delete(CE);
    CE=NULL;
}
void DialogsDesk::_Draw(){
	if(!Visible)return;
	StdBorder* SB=BScope.Get(BorderIndex);
	if(SB)SB->Draw(x,y,x1,y1);
	DWORD Diff=GPS.GetCurrentDiffuse();
	if(!DeepColor)GPS.SetCurrentDiffuse(0xFFFFFFFF);
	//else GPS.SetCurrentDiffuse(Diffuse);
	if(VScroller&&EnableVScroll){
		if(VScroller->SPos>VScroller->SMaxPos){
			VScroller->SPos=VScroller->SMaxPos;
		}
		StdBorder* SB=BScope.Get(BorderIndex);
		if(SB->VScroller_GP_File>0&&SB->VScroller_GP_File<0xFFFF){
			int Ly=y1-y+1;
			VScroller->y1=VScroller->y+Ly-SB->VScroller_DY_top-SB->VScrolled_DY_bottom;
		}
		VScroller->_Draw();
		GPS.FlushBatches();
	}	
	if(HScroller&&EnableHScroll){
		HScroller->_Draw();
		GPS.FlushBatches();
	}
	extern short WheelDelta;
	if((!DialogsDesk::ActiveParent)&&MouseOver&&VScroller&&VScroller->Visible&&WheelDelta&&EnableVScroll&&(!EditMode)&&VScroller->SMaxPos>1){
		VScroller->SPos-=WheelDelta/4;
		if(VScroller->SPos>VScroller->SMaxPos)VScroller->SPos=VScroller->SMaxPos;
		if(VScroller->SPos<0)VScroller->SPos=0;
		WheelDelta=0;
	}
	GPS.SetCurrentDiffuse(Diff);

    ClassEditorConnector* CEC=UseClassEditor.Get();
    if(CEC){
        int l,t,r,b;
        GetMargin(l,t,r,b);
        BaseTriplet BT=CEC->GetClass();
		if(BT.Data && BT.IsDirectCasting() && BT.Data!=CEC->PrevDataPtr){
            if(CEC->CE)delete(CEC->CE);
            CEC->CE=new ClassEditor;
			CEC->CE->CreateFromClass(this,-l,-t,x1-x+r,y1-y+b,BT.GetClass(),7,"EmptyBorder");
            CEC->PrevDataPtr=BT.Data;
        }
        if(CEC->CE){
            CEC->CE->Process();        
            if(CEC->CE->_x1!=x1-x+r || CEC->CE->_y1!=y1-y+b){
                CEC->PrevDataPtr=NULL;
            }
        }
    }
	// top, bottom
	/*
	int w=GetWidth();
	SimpleDialog* T=Top.Get();
	if(T){
		T->Setx(x+((w-T->GetWidth())>>1));
		T->Sety(y+TopDy); //+T->GetHeight()
		T->_Draw();
	}	
	SimpleDialog* B=Bottom.Get();
	if(B){
		B->Setx(x+((w-T->GetWidth())>>1));
		B->Sety(y1+BottomDy); //-T->GetHeight()
		B->_Draw();
	}
	int bN=Buttons.GetAmount();
	if(bN){
		int bW=0;
		int bY=y1+ButtonsDy;
		for(int i=0;i<bN;i++){
			SimpleDialog* b=Buttons[i];
			b->Setx(bW);
			b->Sety(bY);
			bW+=b->GetWidth()+ButtonsDx;
		}
		int bX=(w-bW)>>1;
		for(int i=0;i<bN;i++){
			SimpleDialog* b=Buttons[i];
			b->Setx(b->Getx()+bX);
		}		
	}
	*/

}
void DialogsDesk::_MouseOver(){
	if(VScroller)VScroller->_MouseOver();
	if(HScroller)HScroller->_MouseOver();
	SimpleDialog::_MouseOver();
}
bool DialogsDesk::_Click(){
	if(VScroller)VScroller->_Click();
	if(HScroller)HScroller->_Click();
	return SimpleDialog::_Click();
}
void ChatDesk::Process(SimpleDialog** LeftClick,SimpleDialog** RightClick,SimpleDialog** HintDlg){	
	if(VScroller&&VScroller->SMaxPos-VScroller->SPos<2){
		int SMax=VScroller->SMaxPos;
		//if(SMax==0) SMax=1;
		DialogsDesk::Process(LeftClick,RightClick,HintDlg);
		if(VScroller->SMaxPos>SMax){
			VScroller->SPos=VScroller->SMaxPos;
		}
	}else{
		DialogsDesk::Process(LeftClick,RightClick,HintDlg);
	}
}
void DialogsDesk::Process(SimpleDialog** LeftClick,SimpleDialog** RightClick,SimpleDialog** HintDlg){
	int N=DSS.GetAmount();
	int maxy=0;
	for(int i=0;i<N;i++){
		SimpleDialog* SD=DSS[i];
		//try{
			if(SD&&SD->Visible){
		        if(SD->y1>maxy)maxy=SD->y1;
			}
		//}catch(...){};
	}
	if(!VScroller){
		StdBorder* SB=BScope.Get(BorderIndex);
		if(SB&&SB->VScroller_GP_File>0&&SB->VScroller_GP_File<0xFFFF){
			int Ly=y1-y+1;
			VScrollBar* VS=addNewGP_VScrollBar(NULL,x1-SB->VScroller_DX_right,y+SB->VScroller_DY_top,
				Ly-SB->VScroller_DY_top-SB->VScrolled_DY_bottom,100,0,SB->VScroller_GP_File,0);
			VScroller=VS;
			VS->ScrDy=Ly-32;
			VS->OnesDy=32;
			VS->ParentDS=this;
			DSS.NValues--;
		}
	}
	if(VScroller){
		VScroller->ParentDS=this;
		int l,u,r,d;
		GetMargin(l,u,r,d);
		maxy-=y1-y-u-d-12;
		if(maxy>0){
            VScroller->SMaxPos=maxy;
			VScroller->Visible=1;
		}else{
			VScroller->SMaxPos=0;
			VScroller->Visible=!HideVScroller;
		}
		YShift=VScroller->SPos;
	}
	SimpleDialog::Process(LeftClick,RightClick,HintDlg);
}
bool DialogsDesk::ShiftDialog(int dx,int dy){
	SimpleDialog::ShiftDialog(dx,dy);
	if(VScroller)VScroller->ShiftDialog(dx,dy);
	if(HScroller)HScroller->ShiftDialog(dx,dy);
	return true;
}
bool DialogsDesk::ResizeDialog(int dx,int dy){
	SimpleDialog::ResizeDialog(dx,dy);
	if(VScroller){
		VScroller->ShiftDialog(dx,0);
		VScroller->ResizeDialog(0,dy);
	}
	if(HScroller){
		HScroller->ShiftDialog(0,dy);
		HScroller->ResizeDialog(dx,0);
	}
	return true;
}
DialogsDesk* ParentFrame::AddDialogsDesk(int x,int y,int Lx,int Ly,const char* BorderID){
	DialogsDesk* DD=new DialogsDesk;
	AddDialog(DD);
	DD->x=BaseX+x;
	DD->y=BaseY+y;
	DD->x1=DD->x+Lx-1;
	DD->y1=DD->y+Ly-1;
	DD->BorderIndex=0;
	DD->EnableVScroll=1;
	DD->EnableHScroll=1;
	Enumerator* E=ENUM.Get("BORDERS");
	if(E){
		DD->BorderIndex=E->Get((char*)BorderID);
		if(DD->BorderIndex>=0){
			StdBorder* SB=BScope.Get(DD->BorderIndex);
			if(SB->VScroller_GP_File>0&&SB->VScroller_GP_File<0xFFFF){
                VScrollBar* VS=DD->addNewGP_VScrollBar(NULL,DD->x1-SB->VScroller_DX_right,DD->y+SB->VScroller_DY_top,
					Ly-SB->VScroller_DY_top-SB->VScrolled_DY_bottom,100,0,SB->VScroller_GP_File,0);
				DD->VScroller=VS;
				VS->ScrDy=Ly-32;
				VS->OnesDy=32;
				DD->DSS.NValues=0;
			}
		}
	}
	return DD;
}
void ParentFrame::SetEditMode(bool Mode){
	EditMode=Mode;
	for(int i=0;i<DSS.GetAmount();i++){
		if(DSS[i])DSS[i]->SetEditMode(Mode);
	}
}
void ParentFrame::AddDialog(SimpleDialog* SD){
	SD->EditMode=EditMode;
	DSS.Add(SD);
}
bool ParentFrame::InsertDialog(int pos, SimpleDialog* SD){
	SD->EditMode=EditMode;
	return DSS.Insert(pos,SD);
}
void ParentFrame::GetDialogsFrame(int& x0,int& y0,int& x1,int& y1){
	x0=100000;
	x1=-100000;
	y0=100000;
	y1=-100000;
	for(int i=0;i<DSS.GetAmount();i++)if(DSS[i]){
		if(DSS[i]->x<x0 )x0=DSS[i]->x;
		if(DSS[i]->y<y0 )y0=DSS[i]->y;
		if(DSS[i]->x1>x1)x1=DSS[i]->x1;
		if(DSS[i]->y1>y1)y1=DSS[i]->y1;
	}
}
SimpleDialog* ParentFrame::ActiveParent=NULL;
int ParentFrame::ActiveX=0;
int ParentFrame::ActiveY=0;
int ParentFrame::ActiveX1=0;
int ParentFrame::ActiveY1=0;
int ParentFrame::ActiveID=0;
int ParentFrame::ActiveParentX=0;
int ParentFrame::ActiveParentY=0;
ParentFrame* ParentFrame::OnlyActiveParent=NULL;
//
void SetTabGroup(SimpleDialog* SD, _str& Group){
	if(Group.pchar()&&Group.pchar()[0]){	
		int id=-1;
		int n=SD->DSS.GetAmount();
		for(int i=0;i<n;i++){
			SimpleDialog* sd=SD->DSS[i];		
			TabButton* tb=dynamic_cast<TabButton*>(sd);
			if(tb){			
				if(tb->Group==Group){
					tb->State=1;
					id=i;
				}else{
					tb->State=0;
				}
				continue;
			}
			TabDesk* td=dynamic_cast<TabDesk*>(sd);
			if(td){
				td->Visible=td->ParentDialogID==Group;
			}
		}
		/*
		if(id!=-1&&id<n-1){
			SimpleDialog* sd=SD->DSS[id];
			SD->DSS[id]=SD->DSS[n-1];
			SD->DSS[n-1]=sd;
		}
		*/
	}
}
bool TabButton::_Click(){
	SetTabGroup((SimpleDialog*)ParentDS,Group);
	//TabList* TL=dynamiccast<TabList*>(ParentDS);
	//if(TL) TL->State=ID;	
	return false;
};
bool ChatDesk::AddElement(char* message){
	SimpleDialog* SD=Element.Get();
	if(SD){
		// find bottom
		int y=0;		
		for(int i=DSS.GetAmount()-1;i>=0;i--){
			if(DSS[i]&&DSS[i]->y1>y) y=DSS[i]->y1;
		}
		// add
		SimpleDialog* bcNew=(SimpleDialog*)SD->new_element();
		if(bcNew){
			SD->Copy(bcNew);
			//
			BaseClass* bcMsg=NULL;
			void* extr;
			void* mess;
			if(bcNew->GetElementByName("Message",&bcMsg,&mess,&extr,bcNew)){
				//  mess is _pointer_to_the_pointer_, not pointer to string
				char* pMess = *((char**)mess);
				int l=strlen(message)+1;
				pMess=(char*)realloc(pMess,l);
				memcpy(pMess,message,l);
				*((char**)mess) = pMess;
			};
			//			
			bcNew->Sety(y+dY);
			AddDialog(bcNew);
			return true;
		}
	}
	return false;
};

_str ListDesk::NewElement::Message;
_str ClassDesk::NewElement::Message;
_str ClassDesk::NewElement::ClassName;

void ListDesk::NewElement::EvaluateFunction(){
	char* Msg=ListDesk::NewElement::Message.pchar();
	ListDesk* LD=get_parent<ListDesk>();
	if(LD){
		LD->AddElement(Msg);
	}
}
void ClassDesk::NewElement::EvaluateFunction(){
	char* Msg=ClassDesk::NewElement::Message.pchar();
	char* cname=ClassDesk::NewElement::ClassName.pchar();
	ClassDesk* LD=get_parent<ClassDesk>();
	if(LD){
		if(cname[0])LD->AddElement(Msg,0,(const char*)cname);
		else{
			for(int i=0;i<LD->Elements.GetAmount();i++){
				LD->AddElement(Msg,0,LD->Elements[i]->ClassName.pchar());
			}
		}
	}
}
bool ListDesk::AddElement(_str& message){
	return AddElement(message.pchar());
};
bool ClassDesk::AddElement(_str& message,const char* ClassName){
	return AddElement(message.pchar(),0,ClassName);
};
void ListDesk::UpdateFreeID(){
    FreeID=1;
	int n=DSS.GetAmount();
	for(int i;i<n;i++){
		SimpleDialog* SD=DSS[i];
		if(FreeID<SD->ID+1){
			FreeID=SD->ID+1;
		}
	}
};
void ClassDesk::UpdateFreeID(){
	FreeID=1;
	int n=DSS.GetAmount();
	for(int i;i<n;i++){
		SimpleDialog* SD=DSS[i];
		if(FreeID<SD->ID+1){
			FreeID=SD->ID+1;
		}
	}
};
ListDesk::ListDesk() : DialogsDesk() {
	UpdateFreeID();
	AmountInLine=1;
};
ClassDesk::ClassDesk() : DialogsDesk() {
	UpdateFreeID();
	AmountInLine=1;
};
bool ListDesk::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){	
	bool ret=DialogsDesk::Load(xml,ClassPtr,Error,Extra);
	UpdateFreeID();
	return ret;
};
bool ClassDesk::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){	
	bool ret=DialogsDesk::Load(xml,ClassPtr,Error,Extra);
	UpdateFreeID();
	return ret;
};
void ClassDesk::V_scrollToElement(int _id,int type/*=0*/){
	if ( VScroller== NULL ) return;
	if ( _id<0 || _id>=DSS.GetAmount() ) return;
	int l_scrlD = ( DSS[_id]->y - DSS[0]->y );
	if ( type==0 )	l_scrlD -= GetHeight()/2;
	l_scrlD = ( (l_scrlD>=0) ? (l_scrlD) : (0) );
	if ( VScroller->SMaxPos!=1 ){
		VScroller->SPos = ( ( l_scrlD<VScroller->SMaxPos ) ? ( l_scrlD ) : ( VScroller->SMaxPos ) );
	};
};
// type 0-on center, 1-on top, 2-on bottom
int ListDesk::AddElement(int Pos, char* message, int HotKey){
	SimpleDialog* SD=Element.Get();
	if(SD){
		SimpleDialog* bcNew=(SimpleDialog*)SD->new_element();

		SD->Copy(bcNew);

		VitButton* VB=dynamic_cast<VitButton*>(SD);
		if(VB){
			
			VitButton* VBN=(VitButton*)bcNew;
			VBN->SetMessage(message);

		}else{
			
			BaseClass* bcMsg=NULL;
			void* extr;
			void* mess;
			if(bcNew->GetElementByName("Message",&bcMsg,&mess,&extr,bcNew)){
				//  mess is _pointer_to_the_pointer_, not pointer to string
				char* pMess = *((char**)mess);
				pMess=(char*)realloc(pMess,strlen(message)+1);
				memcpy(pMess,message,strlen(message)+1);
				*((char**)mess) = pMess;
			};
		}
		bcNew->ID=FreeID;
		FreeID++;
		//
		bcNew->HotKey=HotKey;
		if(DSS.GetAmount()<Pos){
			AddDialog(bcNew);
		}else{
			if(!InsertDialog(Pos,bcNew)){
				delete(bcNew);
				return -1;
			}
		}		
		return bcNew->ID;
	}
	return -1;
};
int ClassDesk::AddElement(int Pos, char* message, int HotKey,const char* ClassName){
	if(!Elements.GetAmount())return 0;
	int _ID=0;
	SimpleDialog* SD=NULL;
	if(ClassName && ClassName[0]){
		for(int i=0;i<Elements.GetAmount();i++)if(Elements[i]->ClassName.equal(ClassName)){
			SD=Elements[i]->Dialog.Get();
			_ID=i;
			break;
		}
	}else{
		SD=Elements[0]->Dialog.Get();  
		_ID=0;
	}	
	if(SD){
		SimpleDialog* bcNew=(SimpleDialog*)SD->new_element();

		SD->Copy(bcNew);

		VitButton* VB=dynamic_cast<VitButton*>(SD);
		if(VB){

			VitButton* VBN=(VitButton*)bcNew;
			VBN->SetMessage(message);

		}else{

			BaseClass* bcMsg=NULL;
			void* extr;
			void* mess;
			if(bcNew->GetElementByName("Message",&bcMsg,&mess,&extr,bcNew)){
				//  mess is _pointer_to_the_pointer_, not pointer to string
				char* pMess = *((char**)mess);
				pMess=(char*)realloc(pMess,strlen(message)+1);
				memcpy(pMess,message,strlen(message)+1);
				*((char**)mess) = pMess;
			};
		}
		bcNew->ID=FreeID++;
		bcNew->CurClassIndex=_ID;

		//
		if(HotKey)bcNew->HotKey=HotKey;
		if(DSS.GetAmount()<Pos){
			AddDialog(bcNew);
		}else{
			if(!InsertDialog(Pos,bcNew)){
				delete(bcNew);
				return 0;
			}
		}		
		return bcNew->ID;
	}
	return 0;
};
int ListDesk::AddElement(char* message, int HotKey){
	return AddElement(100000,message,HotKey);
}
int ClassDesk::AddElement(char* message, int HotKey,const char* ClassName){
	return AddElement(100000,message,HotKey,ClassName);
}
SimpleDialog* ListDesk::GetElementByUID(int UID){
	int n=DSS.GetAmount();
	for(int i=0;i<n;i++){
		SimpleDialog* SD=DSS[i];
		if(SD&&UID==SD->ID){
			return SD;			
		}
	}
	return NULL;
};
SimpleDialog* ClassDesk::GetElementByUID(int UID){
	int n=DSS.GetAmount();
	for(int i=0;i<n;i++){
		SimpleDialog* SD=DSS[i];
		if(SD&&UID==SD->ID){
			return SD;			
		}
	}
	return NULL;
};
bool ListDesk::DelElementByUID(int UID){
	int n=DSS.GetAmount();
	for(int i=0;i<n;i++){
		SimpleDialog* SD=DSS[i];
		if(SD&&UID==SD->ID){
			return DSS.DelElement(i);
		}
	}
	return false;
};
bool ClassDesk::DelElementByUID(int UID){
	int n=DSS.GetAmount();
	for(int i=0;i<n;i++){
		SimpleDialog* SD=DSS[i];
		if(SD&&UID==SD->ID){
			return DSS.DelElement(i);
		}
	}
	return false;
};
char* ListDesk::GetElement(int ID){
	if(ID>=0&&ID<DSS.GetAmount()){
		BaseClass* bcMsg=NULL;
		void* extr;
		void* mess;
		SimpleDialog* SD=DSS[ID];
		if(SD->GetElementByName("Message",&bcMsg,&mess,&extr,SD)){
			char* pMess = *((char**)mess);
			return pMess;
		};
	}
	return NULL;
};
char* ClassDesk::GetElement(int ID){
	if(ID>=0&&ID<DSS.GetAmount()){
		BaseClass* bcMsg=NULL;
		void* extr;
		void* mess;
		SimpleDialog* SD=DSS[ID];
		if(SD->GetElementByName("Message",&bcMsg,&mess,&extr,SD)){
			char* pMess = *((char**)mess);
			return pMess;
		};
	}
	return NULL;
};
int ListDesk::GetElement(char* m){
	if(m&&m[0]){
		BaseClass* bcMsg=NULL;
		void* extr;
		void* mess;
		for(int i=0;i<DSS.GetAmount();i++){
			SimpleDialog* SD=DSS[i];
			if(SD->GetElementByName("Message",&bcMsg,&mess,&extr,SD)){
				char* pMess = *((char**)mess);
				if(!strcmp(pMess,m)) return i;
			};
		}
	}
	return -1;
};
int ClassDesk::GetElement(char* m){
	if(m&&m[0]){
		BaseClass* bcMsg=NULL;
		void* extr;
		void* mess;
		for(int i=0;i<DSS.GetAmount();i++){
			SimpleDialog* SD=DSS[i];
			if(SD->GetElementByName("Message",&bcMsg,&mess,&extr,SD)){
				char* pMess = *((char**)mess);
				if(!strcmp(pMess,m)) return i;
			};
		}
	}
	return -1;
};
void SetAutoIndex(SimpleDialog* SD,int Index){
	SD->AutoIndexInArray=Index;
	for(int i=0;i<SD->DSS.GetAmount();i++){
		SD->DSS[i]->AutoIndexInArray=Index;
		SetAutoIndex(SD->DSS[i],Index);
	}
}
int ListDesk::GetWidth(){
	SimpleDialog* SD=Element.Get();
	if( EditMode && SD && DSS.GetAmount()==0 ){

		if( Visible && AutoSetWidth ){
			int ButW=SD->GetWidth();
			int ButH=SD->GetHeight();

			int maxx=0;
			int maxy=0;

			int xx=startX+marginX;
			int yy=startY+marginY;

			if(AmountInLine<1)AmountInLine=1;

			for(int i=0;i<PreviewAmount;i++){
				if(AlignType==0&&AmountInLine<2){
					SD->SetWidth(ButW);
					yy=marginY+startY+i*(SD->GetHeight()+marginY);
				}else
				if(AlignType==0){
					// Horizontal
					xx=marginX+startX+(i%AmountInLine)*(marginX+ButW);
					yy=marginY+startY+(i/AmountInLine)*(marginY+ButH);
				}else{
					// Vertical
					xx=marginX+startX+(i/AmountInLine)*(marginX+ButW);
					yy=marginY+startY+(i%AmountInLine)*(marginY+ButH);
				}
				if(maxx<xx){
					maxx=xx;
				}
				if(maxy<yy){
					maxy=yy;
				}
			}
			return maxx+SD->GetWidth();
		}

	}
	return DialogsDesk::GetWidth();
};
void ListDesk::_Draw(){
	
	int l,t,r,b;
	GetMargin(l,t,r,b);

	SimpleDialog* SD=Element.Get();

	ArrayConnector* AC=Connect_to_ClonesArray.Get();
	if(AC && SD){
        BaseTriplet T=AC->GetClass();
		if(T.GetClass()){
            ClassArray<BaseClass>* CA=(ClassArray<BaseClass>*)T.GetClass();
			int d=CA->GetAmount()-DSS.GetAmount();
			if(d){
				for(int i=0;i<d;i++){
					AddElement("");                    
				}
				for(int i=0;i>d;i--){
                    DSS.DelElement(DSS.GetAmount()-1);
				}
			}
		}				
	}
	for(int i=0;i<DSS.GetAmount();i++){			
		SetAutoIndex(DSS[i],i);
	}
	int ButW=GetWidth()-(marginX<<1)-l-r;
	if(AlignType!=0||AmountInLine>1){
		if(SD) ButW=SD->GetWidth();
	}
	
	int ButH=25;
	if(SD) ButH=SD->GetHeight();
	
	int xx=startX+marginX;
	int yy=startY+marginY;

	if(AmountInLine<1)AmountInLine=1;

	int N=DSS.GetAmount();
	for(int i=0;i<N;i++){
		SimpleDialog* SD=DSS[i];
		if( SD && SD->Visible ){

			if( AlignType==0 && AmountInLine<2 ){
				SD->Setx(xx);
				SD->Sety(yy);
				SD->SetWidth(ButW);
				yy+=SD->GetHeight()+marginY;
			}else if(AlignType==0){
				// Horizontal
				xx=marginX+startX+(i%AmountInLine)*(marginX+ButW);
				yy=marginY+startY+(i/AmountInLine)*(marginY+ButH);
				SD->Setx(xx);
				SD->Sety(yy);
			}else{
				// Vertical
				xx=marginX+startX+(i/AmountInLine)*(marginX+ButW);
				yy=marginY+startY+(i%AmountInLine)*(marginY+ButH);
				SD->Setx(xx);
				SD->Sety(yy);
			}

		}
	}

	// dialogs desk process
	DialogsDesk::_Draw();
	if( EditMode && SD && DSS.GetAmount()==0 ){
        TempWindow TW;
		PushWindow(&TW);
		IntersectWindows(x+l,y+t,x1-r,y1-b);

		SD->SetWidth(ButW);

		int xx=SD->x;
		int yy=SD->y;

		int sx,sy;
		GetShift(sx,sy);

		int x0=x+l-sx;
		int y0=y+t-sy;

		int xxx=marginX+startX;
		int yyy=marginY+startY;

		for(int i=0;i<PreviewAmount;i++){

			if(AlignType==0&&AmountInLine<2){
				SD->Setx(x0+xxx);
				SD->Sety(y0+yyy);
				SD->SetWidth(ButW);
				yyy+=SD->GetHeight()+marginY;
			}else
			if(AlignType==0){
				// Horizontal
				xxx=marginX+startX+(i%AmountInLine)*(marginX+ButW);
				yyy=marginY+startY+(i/AmountInLine)*(marginY+ButH);
				SD->Setx(x0+xxx);
				SD->Sety(y0+yyy);
			}else{
				// Vertical
				xxx=marginX+startX+(i/AmountInLine)*(marginX+ButW);
				yyy=marginY+startY+(i%AmountInLine)*(marginY+ButH);
				SD->Setx(x0+xxx);
				SD->Sety(y0+yyy);
			}

			//SD->_Draw();
			DrawRecursive(SD);

		}

		SD->Setx(xx);
		SD->Sety(yy);
		PopWindow(&TW);

		ISM->Flush();
	}
};
void BaseDropFunction::DoDrop(){
	(*SrcArray)[SourceDropIndex]=NULL;//removing
	if(DesiredDstDropIndex==-1)DstArray->Add(Element);//adding
	else DstArray->Insert(DesiredDstDropIndex,Element);//inserting
	for(int i=0;i<SrcArray->GetAmount();i++){
		if((*SrcArray)[i]==NULL){
			SrcArray->DelElement(i--);
		}
	}
}
void ClassDesk::_OnDropOnThis(){
	if(AllowDropOverThisDialog){
		SimpleDialog* Dragged=SimpleDialog::CallParams.Dragged;
		if(Dragged){
			BaseClass* BC=Dragged->GetParent();
			if(BC){
				BC=BC->GetParent();
				ClassDesk* CD=dynamic_cast<ClassDesk*>(BC);
				if(CD){
					if(Dragged->CurClassIndex>=0 && Dragged->CurClassIndex<CD->Elements.GetAmount()){
                        const char* ClassName=CD->Elements[Dragged->CurClassIndex]->ClassName.pchar();
						//check for support of this class
						bool fnd=false;
						for(int i=0;i<Elements.GetAmount();i++)if(Elements[i]->ClassName.equal(ClassName)){
                            fnd=true;
							break;
						}
						if(fnd){
							//it is allowed to drop this class
							ArrayConnector* AC_src=CD->Connect_to_ClassArray.Get();
							ArrayConnector* AC_dst=Connect_to_ClassArray.Get();
							if(AC_src && AC_dst){
								BaseClass* SrcBC=AC_src->GetClass().GetClass();
								BaseClass* DstBC=AC_dst->GetClass().GetClass();
								if(SrcBC && DstBC 
									&& strstr(SrcBC->GetClassName(),"ClassArray.") 
									&& strstr(DstBC->GetClassName(),"ClassArray.")){
                                    ClassArray<BaseClass>* SrcArr=(ClassArray<BaseClass>*)(SrcBC);
									ClassArray<BaseClass>* DstArr=(ClassArray<BaseClass>*)(DstBC);
									if(SrcArr && DstArr){
										int SrcIdx=Dragged->AutoIndexInArray;
										if(SrcIdx>=0 && SrcIdx<SrcArr->GetAmount()){
                                            BaseClass* el=(*SrcArr)[SrcIdx];											
											if(el){												
												//attempt to add element in appropriate place
												int bidx=-1;
												for(int i=0;i<DSS.GetAmount();i++){
													SimpleDialog* SD=DSS[i];
													if(SD && SD->MouseOver){
														bidx=i;
														break;
													}
												}
												CallingClassConnector* CCC=Call_BaseDropFunction_OnDrop.Get();
												BaseDropFunction* BDC=NULL;
												if(CCC){
													BaseClass* BC=CCC->GetClass().GetClass();
													if(BC)BDC=dynamic_cast<BaseDropFunction*>(BC);
												}
												if(BDC){
													BDC->DesiredDstDropIndex=bidx;
													BDC->DstArray = DstArr;
													BDC->SrcArray = SrcArr;
													BDC->Element  = el;
													BDC->SourceDropIndex=SrcIdx;
													CCC->Call();													
												}else{
													(*SrcArr)[SrcIdx]=NULL;//removing
													if(bidx==-1)DstArr->Add(el);//adding
													else DstArr->Insert(bidx,el);//inserting
													for(int i=0;i<SrcArr->GetAmount();i++){
														if((*SrcArr)[i]==NULL){
															SrcArr->DelElement(i--);
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
void ClassDesk::_Draw(){
	
	int l,t,r,b;
	GetMargin(l,t,r,b);

	ArrayConnector* AC=Connect_to_ClassArray.Get();
	if(AC){
		BaseTriplet T=AC->GetClass();
		if(T.GetClass()){
			ClassArray<BaseClass>* CA=(ClassArray<BaseClass>*)T.GetClass();
			for(int i=0;i<CA->GetAmount();i++){
				BaseClass* BC=(*CA)[i];
				if(BC){
                    const char* CN=BC->GetClassName();
					SimpleDialog* SD=NULL;
					if(i<DSS.GetAmount()){
                        SD=DSS[i];
						if(!(SD && SD->CurClassIndex>=0 && SD->CurClassIndex<Elements.GetAmount() && Elements[SD->CurClassIndex]->ClassName.equal(CN))){
                            if(SD)delete(SD);
							SD=NULL;
							DSS[i]=NULL;
						}
						if(!SD)DSS.DelElement(i);							
					}else{
						AddElement(i,"",0,CN);
						//break;
					}
					if(!SD)AddElement(i,"",0,CN);
				}
			}
			if(DSS.GetAmount()>CA->GetAmount()){
				int n0=DSS.GetAmount();
				for(int i=CA->GetAmount();i<n0;i++)DSS.DelElement(DSS.GetAmount()-1);
			}
		}				
	}
	for(int i=0;i<DSS.GetAmount();i++){			
		SetAutoIndex(DSS[i],i);
	}
	int ButW=1;
	if(AmountInLine==1){
		for(int i=0;i<DSS.GetAmount();i++)if(DSS[i]){
			int w=DSS[i]->LastDrawX-LastDrawX+DSS[i]->GetWidth();
			if(w>ButW)ButW=w;
		}
	}
	if(AlignType!=0||AmountInLine>1){
		ButW=1;
		for(int i=0;i<Elements.GetAmount();i++){
			SimpleDialog* SD=Elements[i]->Dialog.Get();
			if(SD){
				int w=SD->GetWidth();
				ButW=max(ButW,w);
			}
		}
	}
	if(ButW==1)ButW=GetWidth()-(marginX<<1)-l-r;
	int ButH=25;
	//if(SD) ButH=SD->GetHeight();

	int xx=marginX;
	int yy=marginY;

	if(AmountInLine<1)AmountInLine=1;

	int N=DSS.GetAmount();
	int y1=marginY;
	int y1p=marginY;
	int x1=marginX;
	int x1p=marginX;
	for(int i=0;i<N;i++){
		SimpleDialog* SD=DSS[i];
		if(SD&&SD->Visible){
			if(AlignType==0&&AmountInLine<2){
				SD->SetWidth(ButW);
				yy=y1;
				y1+=SD->GetHeight()+marginY;
				y1p=y1;
			}else
			if(AlignType==0){
				// Horizontal
				xx=marginX+(i%AmountInLine)*(marginX+ButW);
				yy=y1p;
				int yy1=y1p+SD->GetHeight()+marginY;
				y1=std::max(y1,yy1);
				if(i==N-1 || AmountInLine<2 || i%AmountInLine==AmountInLine-1){
					y1p=y1;
				}
			}else{
				// Vertical
				xx=x1p;
				yy=marginY+(i%AmountInLine)*(marginY+ButH);
				int xx1=x1p+SD->GetWidth()+marginX;
				x1=std::max(x1,xx1);
				if(i==N-1 || AmountInLine<2 || i%AmountInLine==AmountInLine-1){
					x1p=x1;
				}
			}
			SD->Setx(xx);
			SD->Sety(yy);					
		}
	}

	// dialogs desk process
	DialogsDesk::_Draw();	
};
void ChatDesk::_Draw(){
	DialogsDesk::_Draw();
	if(EditMode){
		SimpleDialog* SD=Element.Get();
		if(SD){
			TempWindow TW;
			PushWindow(&TW);
			int l,t,r,b;
			GetMargin(l,t,r,b);
			IntersectWindows(x+l,y+t,x1-r,y1-b);
			//
			int xx=SD->x;
			int yy=SD->y;
			int yyy=0;
			int dy=SD->GetHeight()+dY;
			SD->Setx(x+l+xx);
			//SD->SetWidth(ButW);
			//SD->SetHeight(ButH);
			for(int i=0;i<15;i++){
				SD->Sety(y+t+yy+yyy);
				SD->_Draw();
				yyy+=dy;
			}
			SD->Setx(xx);			
			SD->Sety(yy);
			//
			PopWindow(&TW);
		}
	}
}
ProgressBar::ProgressBar(){
    Message="50%";
	Value=50;
	MaxValue=100;
	Font=&SmallWhiteFont;
	BackGP_File=GPS.PreLoadGPImage("Interf3\\BuildProgress");
	ProgressGP_File=GPS.PreLoadGPImage("Interf3\\BuildProgress");
	BackSprite=0;
	ProgressSprite=1;
	BackColor=0xFFFFFFFF;
	ProgressColor=0xFFFFFFFF;
	Scale=1.0f;
}

void DrawPercentGauge( float percent, const Rct& ext, DWORD clr );

void ProgressBar::_Draw(){
	
	if(MaxValue==0)
		return;

	GPS.SetScale(Scale);    

	if(Type==0){
		
		GPS.SetCurrentDiffuse(ModDWORD(Diffuse,BackColor));

		x1=x+int(float(GPS.GetGPWidth(BackGP_File,BackSprite))*Scale);
		y1=y+int(float(GPS.GetGPHeight(BackGP_File,BackSprite))*Scale);
		GPS_ShowGP(x+BackDx,y+BackDy,BackGP_File,BackSprite,Nation);

		TempWindow TW;
		PushWindow(&TW);
		if(MaxValue){
			switch(ProgressType){
				case 0://A->B
					IntersectWindows(0,0,x+ProgressDx+ProgerssMarginLU+(x1-x-ProgerssMarginLU-ProgerssMarginRD)*Value/MaxValue,RealLy);
					break;
				case 1://Top->Bottom
					IntersectWindows(0,0,RealLx,y+ProgressDy+ProgerssMarginLU+(y1-y-ProgerssMarginLU-ProgerssMarginRD)*Value/MaxValue);
					break;
				case 2://Bottom->Top
					IntersectWindows(0,y+ProgressDy+ProgerssMarginLU+(y1-y-ProgerssMarginLU-ProgerssMarginRD)*(MaxValue-Value)/MaxValue,RealLx,RealLy);
					break;
			}
		}
		
		GPS.SetCurrentDiffuse(ModDWORD(Diffuse,ProgressColor));
		GPS_ShowGP(x+ProgressDx,y+ProgressDy,ProgressGP_File,ProgressSprite,Nation);
		PopWindow(&TW);

	}else{
		
		int V=Value;

		int w=GetWidth();
		int h=GetHeight();

		if(GaugeType==0){
			switch(ProgressType){
			case 0:
				V=MaxValue-Value;
				break;
			}
			if (V) DrawPercentGauge(100 * V / MaxValue, Rct(x, y, w, h), Diffuse);
		}else{
			switch(ProgressType){
				case 0: // Horisontal(LeftRight)
					w=w*V/MaxValue;
					break;
				case 1: // Vertical(TopBottom)
					h=h*V/MaxValue;
					break;
				case 2: // Vertical(BottomTop)
					h=h*V/MaxValue;
					y=y1-h-1;
					break;
			}
			rsRect( Rct( x, y, w, h ), 0.0f, Diffuse );
		}

	}
	
	GPS.SetCurrentDiffuse(0xFFFFFFFF);
	GPS.SetScale(1.0);

	if(Font&&Message.pchar()){
		ShowStringEx((x+x1-GetRLen(Message.pchar(),Font))/2+MessageDx,y+MessageDy,Message.pchar(),Font);
	}
}
const char* ParentFrame::GetThisElementView(const char* LocalName){	
	static char pf_text[1024];
	strcpy(pf_text,LocalName);
	if(Name.pchar()&&Name.pchar()[0]){
		sprintf(pf_text+strlen(pf_text)," [%s]",Name.pchar());
	}		
	return pf_text;
}
const char* SimpleDialog::GetThisElementView(const char* LocalName){
	static char pf_text[1024];
	strcpy(pf_text,LocalName);
	int n=v_Actions.GetAmount();
	if(n>0){
		sprintf(pf_text+strlen(pf_text)," A%d",n);
	}		
	if(Name.pchar()&&Name.pchar()[0]){
		sprintf(pf_text+strlen(pf_text)," [%s]",Name.pchar());
	}		
	return pf_text;
}
const char* TextButton::GetThisElementView(const char* LocalName){	
	char* s=(char*)SimpleDialog::GetThisElementView(LocalName);
    strcat(s," ");
	if(Message)strcat(s,Message);
	return s;
}
const char* GP_TextButton::GetThisElementView(const char* LocalName){	
	char* s=(char*)SimpleDialog::GetThisElementView(LocalName);
	strcat(s," ");
	if(Message)strcat(s,Message);
	return s;
}
const char* VitButton::GetThisElementView(const char* LocalName){	
	char* s=(char*)SimpleDialog::GetThisElementView(LocalName);
	strcat(s," ");
	if(Message)strcat(s,Message);
	return s;
}
void DrawTexturedBar(float xL,float yL,float W,float H,
					 float txL,float tyL,float tW,float tH,
					 DWORD CLU,DWORD CRU,DWORD CLD,DWORD CRD,
					 int TextureID,int ShaderID);
bool OnDrawMinimap(SimpleDialog* SD){
	int GetMinimapTexture();
    int t=GetMinimapTexture();
	static int s=IRS->GetShaderID("hud_smooth");
    DrawTexturedBar(SD->x,SD->y,SD->GetWidth(),SD->GetHeight(),0,0,240.0f/256.0f,240.0f/256.0f,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,t,s);
	return true;
}
//
bool BitPicture::SetFileName(char* File, bool Unload){
	if(File&&TrueName!=File){
		Pic.LoadPicture(File,Unload);
		if(File!=FileName.pchar()){
			FileName.Assign(File);
		}
		TrueName.Assign(File);
		return true;
	}
	return false;
};
void BitPicture::_Draw(){
	SetFileName(FileName.pchar());
	if(ActualSize){		
		x1=x+Pic.GetLx()-1;
		y1=y+Pic.GetLy()-1;
	}
	Pic.Draw(x,y,GetWidth(),GetHeight());
}
bool SimpleDialog::AskParentForUsingExpansionClass(char* MemberName,char* ClassName){
	if(!strcmp(MemberName,"v_Actions")){
		OneClassStorage* OCS=CGARB.GetClass(ClassName);
		if(OCS){
			vui_Action* VA=(vui_Action*)OCS->OneMemb;
			if(VA){
				//return strstr(ClassName,"_SP_");
				return VA->isExpandableFor(this);				
			}
		}
		return false;
	}
	return true;
}
void DrawCenteredText(int x,int y,RLCFont* F,char* s,...)
{
    char cc[512];
	va_list va;
	va_start( va, s );
	vsprintf ( cc, s, va );   
	va_end( va );
	int LX=100000;
	int Ly=DrawMultilineText(0,0,cc,F,LX,100000,1,false);	
    x-=LX/2;
	y-=Ly/2;
	DrawMultilineText(x,y,cc,F,LX,100000,1,true);
	GPS.FlushBatches();
}
CEXPORT
void DrawCentralTextWithBorder(char* s){
	int x=RealLx/2;
	int y=RealLy/2;
	int LX=100000;
	int Ly=DrawMultilineText(0,0,s,&BlackFont,LX,100000,1,false);
	x-=LX/2;
	y-=Ly/2;
	DrawStdBar(x-20,y-10,x+LX+20,y+Ly+10);
	DrawMultilineText(x,y,s,&BlackFont,LX,100000,1,true);
	GPS.FlushBatches();
}
//
void ParentFrame::Setx(int _x){
	int dx=_x-x; 
	ShiftDialog(dx,0);
}
void ParentFrame::Sety(int _y){
	int dy=_y-y; 
	ShiftDialog(0,dy);
}
int ParentFrame::GetWidth(){
	return x1-x;//+1;
}
void ParentFrame::SetWidth(int w){
	int xx1=x+w;//-1; 
	if(xx1<x)xx1=x;
	ResizeDialog(xx1-x1,0);
}
void ParentFrame::SetHeight(int h){
	int yy1=y+h-1;
	if(yy1<y)yy1=y;
	ResizeDialog(0,yy1-y1);
}
void ParentFrame::Setxy(int _x, int _y){
	int dx=_x-x;
	int dy=_y-y;
	ShiftDialog(dx,dy);
}
int  ParentFrame::globalX(){
	if(CoorIsGlobal)return x;
    int xx=x;
	ParentFrame* PF=ParentDS;
	while(PF){
		int l,r,t,b;
		PF->GetMargin(l,t,r,b);
        xx+=PF->x+l;
        if(PF->CoorIsGlobal)return xx;
		PF=PF->ParentDS;
	}
	return xx;
}
int  ParentFrame::globalY(){
	if(CoorIsGlobal)return y;
	int yy=y;
	ParentFrame* PF=ParentDS;
	while(PF){
		int l,r,t,b;
		PF->GetMargin(l,t,r,b);
		yy+=PF->y+t;
		if(PF->CoorIsGlobal)return yy;
		PF=PF->ParentDS;
	}
	return yy;
}
int  ParentFrame::globalX1(){
	return globalX()+x1-x;
}
int  ParentFrame::globalY1(){
	return globalX()+y1-y;
}
_str SimpleDialog::NewAction::ActionName;
_str SimpleDialog::NewAction::ActionGroup;
bool SimpleDialog::NewAction::OnUserLeftClick=false;
bool SimpleDialog::NewAction::OnUserRightClick=false;
bool SimpleDialog::NewAction::OnDraw=false;
bool SimpleDialog::NewAction::SetFrameState=false;
class UniAction:public vui_Action{
public:
	_str TempName;
	virtual const char* GetClassName(){
		return TempName.pchar();
	}
	virtual OneClassStorage* GetClassStorage(){
		static OneClassStorage* OCS=NULL;
		if(!OCS)OCS=CGARB.GetClass(GetClassName());
			return OCS;
	}
	virtual BaseClass* new_element(){
		UniAction* UA=new UniAction;
		UA->TempName=TempName;
		return UA;
	}
};
void SimpleDialog::NewAction::EvaluateFunction(){
	char* Name=SimpleDialog::NewAction::ActionName.pchar();
	if(Name&&Name[0]){		
		_str s0;

		s0=__FILE__;
		s0.Replace0("Dialogs.cpp","AutoActions.h");		
		//checking for existing class
		ResFile FT=RReset(s0.pchar());
		if(FT!=INVALID_HANDLE_VALUE){
			int sz=RFileSize(FT);
			char* temp=new char[sz+1];
			RBlockRead(FT,temp,sz);
			temp[sz]=0;
            
			char tm[64];
			sprintf(tm,"%s:public vui_Action",Name);
			bool exit=strstr(temp,tm);
			delete[](temp);
            RClose(FT);
			if(exit)return;
		}

		FILE* H=fopen(s0.pchar(),"a");

		s0=__FILE__;
		s0.Replace0("Dialogs.cpp","AutoActions.cpp");		
		FILE* CPP=fopen(s0.pchar(),"a");		

		s0=__FILE__;
		s0.Replace0("Dialogs.cpp","AutoActionsRegistration.h");		
		FILE* REG=fopen(s0.pchar(),"a");
		
		if(CPP&&H&&REG){
			//NewActions.h
			fprintf(H,"class %s:public vui_Action{\n",Name);
			fprintf(H,"public:\n");
			fprintf(CPP,"//-------------- %s --------------//\n",Name);            
			if(SimpleDialog::NewAction::OnUserLeftClick){				
				fprintf(H,"    virtual bool isLeftClick(){ return true; };\n",Name);
                fprintf(H,"    virtual bool LeftClick(SimpleDialog* SD);\n",Name);
                //NewActions.cpp				
				fprintf(CPP,"bool %s::LeftClick(SimpleDialog* SD){\n    //insert there left click handler, it should return true if click handled\nreturn false;\n}\n",Name);            
			}
			if(SimpleDialog::NewAction::OnUserRightClick){
				fprintf(H,"    virtual bool isRightClick(){ return true; };\n",Name);
				fprintf(H,"    virtual bool RightClick(SimpleDialog* SD);\n",Name);            
				//NewActions.cpp				
				fprintf(CPP,"bool %s::RightClick(SimpleDialog* SD){\n    //insert there right click handler, it should return true if click handled\nreturn false;\n}\n",Name);            
			}
			if(SimpleDialog::NewAction::OnDraw){
				fprintf(H,"    virtual void OnDraw(SimpleDialog* SD);\n",Name);                
				//NewActions.cpp				
				fprintf(CPP,"void %s::OnDraw(SimpleDialog* SD){\n    //insert there your own drawing function\n}\n",Name);            
			}
			if(SimpleDialog::NewAction::SetFrameState){
				fprintf(H,"    virtual void SetFrameState(SimpleDialog* SD);\n",Name);	
				fprintf(CPP,"void %s::SetFrameState(SimpleDialog* SD){\n    //insert there state-control function\n}\n",Name);            
			}
			fprintf(CPP,"//---------- End of: %s ----------//\n\n\n",Name);            
			fprintf(H,"    SAVE(%s)\n",Name);
			fprintf(H,"    REG_PARENT(vui_Action);\n",Name);
			fprintf(H,"    ENDSAVE\n");
			fprintf(H,"};\n",Name);
			fprintf(REG,"REG_CLASS_EX(%s,\"%s\")\n",Name,SimpleDialog::NewAction::ActionGroup.pchar());
			SimpleDialog* SD=get_parent<SimpleDialog>();
			if(SD){
				UniAction* UA=new UniAction;
				UA->TempName=Name;
				SD->v_Actions.Add(UA);
				OneClassStorage* OCS=CGARB.GetClass("vui_Action");
				if(OCS){
					char* top=SimpleDialog::NewAction::ActionGroup.pchar();
					if(!top)top="";
					char* s=new char[strlen(Name)+1];
					strcpy(s,Name);
					OCS->Children.Add(s);

					s=new char[strlen(top)+1];
					strcpy(s,top);					
					OneClassStorage* OCS2=CGARB.GetClass(Name);
					OCS2->OneMemb=UA->new_element();
					OCS2->ClassTopic=s;
				}
			}
		}
		if(CPP)fclose(CPP);
		if(H)fclose(H);
		if(REG)fclose(REG);
	}
}

extern GameSettings GSets;
extern EngineSettings EngSettings;

StdBorder* BordersScope::Get(int i){

	int NatID=GSets.CGame.InterfaceNatID;
	if( NatID>=0 && NatID<EngSettings.NationalBordersStartID.GetAmount() ){
		int id=EngSettings.NationalBordersStartID[NatID];
		if( id>=0 && i+id<Borders.GetAmount() ){
			i+=id;
		}
	}
	
	if(i>=0&&i<Borders.GetAmount()) return Borders[i];
		else return NULL;

}
///-------[ CONNECTION DIALOG TO CLASS ]-------///
//syntax:
//$rootID.field
//$rootID.field.subfield - any depth allowed
//$rootID.field[123].subfield
//$rootID.field.subfield[123]
//////////////////////////////////////////////////

BaseTriplet ControlToClassConnector::GetClass(){
	BaseTriplet T;
	T.Data=T.Extra=T.ClassPtr=NULL;
	T.OCMS=NULL;
	ReferableBaseClass* B=RootClass.Get();
	if(B){
		const char* rstr=ClassFieldPath.pchar();
		if(rstr[0]=='$'){
			const char* s0=strchr(rstr,'.');
			if(s0){
				s0++;
				T.ClassPtr=B;
				T.Data=B;
				T.Extra=NULL;
				T.OCMS=NULL;
				do{
					if(s0[0]=='['){
						if(s0[1]==']'){
							BaseClass* P=GetParent();
							if(P){
								P=P->GetParent();
								if(P){
									SimpleDialog* SD=dynamic_cast<SimpleDialog*>(P);
									if(SD){
										int idx=SD->AutoIndexInArray;
										if(idx<T.ClassPtr->GetAmountOfElements()){
											BaseTriplet T2=T.ClassPtr->GetElementInfo(idx,T.GetData());
											if(T2.Data){
												T=T2;
											}
                                            /*
											BaseClass* BC=NULL;
											void* Data;
											void* Extra;
											BC=T.BC->GetElementClass(idx);
											if(BC){
												Data=T.BC->GetElementPtr(idx,T.Data);
												Extra=T.BC->GetElementExtraPtr(idx);
												T.BC=BC;
												T.Data=Data;
												T.Extra=Extra;
												T.OCMS=T.BC->GetElementCMS(idx);
											}
											*/
										}
									}
								}
							}
							s0+=2;
							if(s0[0]=='.')s0++;
						}else{
							int idx=atoi(s0+1);
							const char* s1=strchr(s0,']');
							if(s1){
								s0=s1+1;
								if(s0[0]=='.')s0++;
							}else{
								T.Data=NULL;
								break;                            
							}
							BaseTriplet T2=T.ClassPtr->GetElementInfo(idx,T.GetData());
							if(T2.GetData()){
								T=T2;
							}
							/*
							BaseClass* BC=NULL;
							void* Data;
							void* Extra;
							BC=T.BC->GetElementClass(idx);
							if(BC){
								Data=T.BC->GetElementPtr(idx,T.Data);
								Extra=T.BC->GetElementExtraPtr(idx);
							}
							*/
						}
					}else{
						const char* s1=strchr(s0,'[');
						const char* s2=strchr(s0,'.');
						const char* s3=s0+strlen(s0);

                        if(s1 && s2 && s2-s1 < 0)s1=s2;
						if(!s1)s1=s2;
						if(!s1)s1=s3;

						char name[128];
						int L=s1-s0;
						strncpy(name,s0,L);
						s0=s1;
						if(s0[0]=='.')s0++;
						name[L]=0;
						BaseTriplet T2=T.ClassPtr->GetElementInfo(name,T.GetData());
						if(T2.Data){
							T=T2;
						}
						/*
						BaseClass* BC=NULL;
						void* Data;
						void* Extra;
						if(T.BC->GetElementByName(name,&BC,&Data,&Extra,T.Data)){
							T.BC=BC;
							T.Data=Data;
							T.Extra=Extra;
							if(T.BC->CheckDirectCasting())T.BC=(BaseClass*)Data;
						}else{
							T.BC=NULL;
							break;
						}
						*/
					}
				}while(s0[0]);
			}
		}
	}
	return T;
}

void AttachRBC_ToClass(BaseClass* BC,ReferableBaseClass* root,const char* RootName){
	if(BC){
		int N=BC->GetAmountOfElements();
		for(int i=0;i<N;i++){
			BaseClass* B=BC->GetElementClass(i);
			if(B->CheckDirectCasting()){
				void* Data=BC->GetElementPtr(i,BC);
				void* Extra=BC->GetElementExtraPtr(i);
				B=(BaseClass*)Data;
				ControlToClassConnector* conn=dynamic_cast<ControlToClassConnector*>(B);
				VariableExchangeReference* ver=dynamic_cast<VariableExchangeReference*>(B);
				if(ver){
					const char* s=ver->VariableReferenceName.pchar();
					if(s[0]=='$'){
						const char* s1=strchr(s,'.');
						if(!s1)s1=strchr(s,'[');
						if(!s1)s1=s+strlen(s);
						int L=s1-s;
						if(!strncmp(RootName,s,L)){
							ver->RootClass.Set(root);
						}
					}                    
				}else
				if(conn){
                    const char* s=conn->ClassFieldPath.pchar();
					if(s[0]=='$'){
                        const char* s1=strchr(s,'.');
						if(!s1)s1=strchr(s,'[');
						if(!s1)s1=s+strlen(s);
                        int L=s1-s;
						if(!strncmp(RootName,s,L)){
							conn->RootClass.Set(root);
						}
					}
				}else
				{
					if(!strstr(B->GetClassName(),"ClassRef.")){
						AttachRBC_ToClass(B,root,RootName);
					}
				}
			}
		}
	}
}
void ParentFrame::AttachToClass(ReferableBaseClass* root,const char* RootName){
	AttachRBC_ToClass(this,root,RootName);
}
Enumerator* IndexedClassConnector::_get_enum(){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();
		if(cname){
			if(!(strcmp(cname,"_index") && strcmp(cname,"_byte_index"))){
				return (Enumerator*)T.Extra;
			}
		}
	}
	return NULL;
}
bool CallingClassConnector::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="Call BaseFunction: {CY}%$ReasonToCall$%{C} call {CY}\"%$BaseFunction_Reference$%\"{C}";
	return true;
}
void IndexedClassConnector::_exchange(int& value){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();
		if(cname){
			int* vi=NULL;
			byte* vb=NULL;
			if(strstr(cname,"_index")){
                vi=((int*)T.Data);
			}
			if(strstr(cname,"_byte_index")){
				vb=((byte*)T.Data);
			}		
			if(vi){
				if(*vi!=prev_value){
					value=*vi;
				}else{
                    *vi=value;	                
				}
				prev_value=value;
			}
			if(vb){
				if(*vb!=prev_value){
					value=*vb;
				}else{
					*vb=value;	                
				}
				prev_value=value;
			}
		}
	}	
}
void _generalExchange(BaseTriplet& T,_str& prev,_str& val){
	if(T.GetClass()){
		xmlQuote xml;
		T.Save(&xml);
		const char* s=xml.Get_string();
		if(!prev.equal(s)){
			val=s;
            prev=s;
		}else{
			xml.Assign_string(val.pchar());
			ErrorPager ep;
			T.Load(&xml);
		}		
	}
}
void SimpleClassConnector::_exchange(_str&  val){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		_generalExchange(T,prev_str,val);
	}
}
void SimpleClassConnector::_exchange(char** val){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();	
		if(!strcmp(cname,"_str")){
			char* s0=*val?*val:"";
			char* sc=((_str*)T.Data)->pchar();
			char* ps=prev_str.pchar();
			if(strcmp(sc,ps)){
				if(*val)free(*val);
				*val=znew(char,strlen(sc)+1);
				strcpy(*val,sc);
			}else{			
				_str& s=*((_str*)T.Data);
				if(!s.equal(s0))s=s0;
			}
			prev_str=s0;
		}else{
			if(*val){
				_str v=*val;
				_generalExchange(T,prev_str,v);
				if(strcmp(*val,v.pchar())){
					if(*val)free(*val);
					*val=znew(char,strlen(v.pchar())+1);
					strcpy(*val,v.pchar());
				}
			}
		}
	}
}
void SimpleClassConnector::_exchange(int&   val){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();	
		if(!strcmp(cname,"_int")){
			int pv=atoi(prev_str.pchar());			
            int& cvv=*((int*)T.Data);
			if(pv!=cvv){
                val=cvv;                                
			}else{
				cvv=val;
			}
			prev_str=val;
		}
	}
}
void SimpleClassConnector::_exchange(DWORD& val){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();	
		if(!strcmp(cname,"_int")){
			DWORD pv=atoi(prev_str.pchar());			
			DWORD& cvv=*((DWORD*)T.Data);
			if(pv!=cvv){
				val=cvv;                                
			}else{
				cvv=val;
			}
			prev_str=int(val);
		}
	}
}
void SimpleClassConnector::_exchange(float& val){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();	
		if(!strcmp(cname,"_bool")){
			float pv=atof(prev_str.pchar());
			float& cvv=*((float*)T.Data);
			if(pv!=cvv){
				val=cvv;                                
			}else{
				cvv=val;
			}
			prev_str.print("%f",cvv);
		}
	}    
}
void SimpleClassConnector::_exchange(bool&  val){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();	
		if(!strcmp(cname,"_bool")){
			bool pv=atoi(prev_str.pchar())!=0;
			bool& cvv=*((bool*)T.Data);
			if(pv!=cvv){
				val=cvv;                                
			}else{
				cvv=val;
			}
			prev_str=int(val);
		}
	}
}
void IntClassConnector::_exchange(int& val){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();	
		if(!(strcmp(cname,"_int") && strcmp(cname,"_sprite") && strcmp(cname,"_ModelID") && strcmp(cname,"_TextureID") && strcmp(cname,"_index"))){
			int& cvv=*((int*)T.Data);
			if(prev_val!=cvv){
				val=cvv;                                
			}else{
				cvv=val;
			}
			prev_val=cvv;
		}else
		if(!strcmp(cname,"_bool")){			
			bool& cvv=*((bool*)T.Data);
			if(bool(prev_val)!=cvv){
				val=cvv;                                
			}else{
				cvv=val;
			}
			prev_val=cvv;
		}else
		if(!(strcmp(cname,"_gpfile") && strcmp(cname,"_word_index") && strcmp(cname,"_WORD") && strcmp(cname,"_short"))){
			short& cvv=*((short*)T.Data);
			if(prev_val!=cvv){
				val=cvv;                                
			}else{
				cvv=val;
			}
			prev_val=cvv;
		}else
		if(!(strcmp(cname,"_byte") && strcmp(cname,"_byte_index"))){
			byte& cvv=*((byte*)T.Data);
			if(prev_val!=cvv){
				val=cvv;                                
			}else{
				cvv=val;
			}
			prev_val=cvv;
		}
	}
}
void BoolClassConnector::_exchange(bool& val){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();	
		if(!strcmp(cname,"_bool")){			
			bool& cvv=*((bool*)T.Data);
			if(prev_val!=cvv){
				val=cvv;                                
			}else{
				cvv=val;
			}
			prev_val=cvv;
		}
	}
}
void IndexComparisonClassConnector::_exchange(bool& val){
	BaseTriplet T=GetClass();
	if(T.GetClass()){
		const char* cname=T.GetClass()->GetClassName();	
		if(!strcmp(cname,"_int")){			
			int& cvv=*((int*)T.Data);
			bool bcvv=(cvv==Is_true_if_equal_to);
			if(prev_val!=bcvv){
				val=bcvv;                                
			}
			prev_val=bcvv;
		}
	}
}
void CallingClassConnector::Call(){
	BaseTriplet B=GetClass();
	if(B.GetClass()){
		BaseFunction* BF=dynamic_cast<BaseFunction*>(B.GetClass());
		if(BF){
			xmlQuote xml;
			for(int i=0;i<Params.GetAmount();i++){
				xmlQuote* nx=new_xmlQuote(Params[i]->Parameter.pchar());
				nx->Assign_string(Params[i]->Value.pchar());
				xml.AddSubQuote(nx);
			}
			ErrorPager ep;
			if(Params.GetAmount())BF->Load(xml,BF,&ep,NULL);
			BF->EvaluateFunction();
        }else{
            if(B.GetClass()->CheckIfFunction()){
                B.GetClass()->call(B.Data,NULL);
            }
        }
	}
}
//testing
class TestConnClass1:public ReferableBaseClass{
public:
	int idx2;
	int intval;
	bool bval2;
	_str str;
	SAVE(TestConnClass1);
		REG_ENUM(_index,idx2,InfoTxt_ENUM);
		REG_MEMBER(_int,intval);
		REG_MEMBER(_bool,bval2);
		REG_AUTO(str);
	ENDSAVE;
};
BaseTriplet SimpleDialog::GetDialogConnection(){
	BaseTriplet B;
	B.ClassPtr=NULL;
	ControlToClassConnector* CCC=ThisDialogIsConnectedTo.Get();
	if(CCC){
		B=CCC->GetClass();
	}
	return B;
}
class TestCBase:public BaseClass{
public:
	SAVE(TestCBase);
	ENDSAVE;
};
class TestCBase1:public TestCBase{
public:
	_str msg1;
	SAVE(TestCBase1);
		REG_PARENT(TestCBase);
		REG_AUTO(msg1);
	ENDSAVE;
};
class TestCBase2:public TestCBase{
public:
	_str msg2;
	SAVE(TestCBase2);
		REG_PARENT(TestCBase);
		REG_AUTO(msg2);
	ENDSAVE;
};

class TestConnClass:public ReferableBaseClass{
public:
	class TestFn:public BaseFunction{
	public:
		void		EvaluateFunction(){
			MessageBox(hwnd,"its me!","---",0);
		}
		SAVE(TestFn);
			REG_PARENT(BaseFunction);		
		ENDSAVE;
	};
	class OnDropFn:public BaseFunction{
		void EvaluateFunction(){
			if(SimpleDialog::CallParams.Dragged && SimpleDialog::CallParams.DropOver){
				BaseClass* EDrag=SimpleDialog::CallParams.Dragged->GetDialogConnection().GetClass();
				BaseClass* EDrop=SimpleDialog::CallParams.DropOver->GetDialogConnection().GetClass();
				ClonesArray<TestConnClass1>* eDrag=dynamic_cast<ClonesArray<TestConnClass1>*>(EDrag);
				ClonesArray<TestConnClass1>* eDrop=dynamic_cast<ClonesArray<TestConnClass1>*>(EDrop);
				int Idx=SimpleDialog::CallParams.Dragged->AutoIndexInArray;
				if(eDrag && eDrop){
					if(dynamic_cast<ListDesk*>(SimpleDialog::CallParams.DropOver)){
						if(Idx>=0 && Idx<eDrag->GetAmount()){
							eDrop->Add((*eDrag)[Idx]);
							(*eDrag)[Idx]=NULL;
							eDrag->DelElement(Idx);
						}
					}else{
						int dropIdx=SimpleDialog::CallParams.DropOver->AutoIndexInArray;
						if(Idx>=0 && Idx<eDrag->GetAmount()){
							if(dropIdx>=0 && dropIdx<eDrop->GetAmount()){
								TestConnClass1* C=(*eDrag)[Idx];
								(*eDrag)[Idx]=NULL;
								eDrop->Insert(dropIdx,C);
								for(int i=eDrag->GetAmount()-1;i>=0;i--)if(!(*eDrag)[i]){
									eDrag->DelElement(i);
								}
							}
						}
					}
				}
			}
		}
		SAVE(OnDropFn);
			REG_PARENT(BaseFunction);		
		ENDSAVE;
	};
	class DropFn2:public BaseDropFunction{
	public:
		void EvaluateFunction(){
			if(rand()<16000)DoDrop();
		}
		SAVE(DropFn2);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	};
	TestFn Fn;
	DropFn2 Rand;

	OnDropFn OnDrop;
	ClonesArray<TestConnClass1> Arr1;
	ClonesArray<TestConnClass1> Arr2;
	TestConnClass1 cc;
    int  idx;
	int  idx3;
	float fv;
    bool bval;
	_str s;
	ClassArray<TestCBase> ARR;
	ClassArray<TestCBase> ARR2;

    const char* _charFN(){
        return "jhjghj";
    }
    int _intFN(){
        return 123453;
    }
    float _floatFN(){
        return 0.51324;
    }
    bool _boolFN(){
        return true;
    }

	SAVE(TestConnClass);

        REG_FN_CHAR  (&TestConnClass::_charFN  );
        REG_FN_FLOAT (&TestConnClass::_floatFN );
        REG_FN_BOOL  (&TestConnClass::_boolFN  );
        REG_FN_INT   (&TestConnClass::_intFN   );
        
		REG_CLASS(TestConnClass1);
		REG_AUTO(cc);
		REG_AUTO(Fn);
		REG_AUTO(Rand);
		REG_AUTO(OnDrop);
		REG_ENUM(_index,idx,VICTORY_COND);
		REG_MEMBER(_bool,bval);
		REG_MEMBER(_int,idx3);
		REG_MEMBER(_float,fv);
		REG_AUTO(s);
		REG_AUTO(Arr1);
		REG_AUTO(Arr2);
		REG_AUTO(ARR);
		REG_AUTO(ARR2);
		REG_CLASS(TestCBase);
		REG_CLASS(TestCBase1);
		REG_CLASS(TestCBase2);
	ENDSAVE;
};

TestConnClass C;

void TestConn(){
	DialogsSystem DS;
	DS.SafeReadFromFile("TESTDIALOG.DialogsSystem.xml");
	DS.AttachToClass(&C);
	do{
        IRS->ClearDevice();
		ProcessMessages();
		if(GetKeyState(VK_SPACE)&0x8000){
            SimpleEditClass("TestConnClass",&C);
		}
		DS.ProcessDialogs();
		DS.RefreshView();
	}while(!(GetKeyState(VK_ESCAPE)&0x8000));
}
void SETAL(DWORD& C,DWORD A){
    C&=0xFFFFFF;
    C|=A;
}
void DrawWindowsStyleBorder(int x,int y,int x1,int y1,DWORD C){
    DWORD A=C&0xFF000000;

    DWORD C1=MulDWORD(C,128+32);
    DWORD C11=MulDWORD(C,128+32+16);
    DWORD C2=MulDWORD(C,128);
    DWORD C3=MulDWORD(C,128-32);
    DWORD C31=MulDWORD(C,128-32-16);

    SETAL(C1,A);
    SETAL(C11,A);
    SETAL(C2,A);
    SETAL(C3,A);
    SETAL(C31,A);

    int w=x1-x+1;
    int h=y1-y+1;
    GPS.DrawFillRect(x+2,y+2,w-4,h-4,C2);
    GPS.DrawFillRect(x,y,1,h,C11);
    GPS.DrawFillRect(x+1,y+1,1,h-2,C1);
    GPS.DrawFillRect(x+w-1,y,1,h,C31);
    GPS.DrawFillRect(x+w-2,y+1,1,h-2,C3);
    GPS.DrawFillRect(x+1,y,w-2,1,C11);
    GPS.DrawFillRect(x+2,y+1,w-4,1,C1);
    GPS.DrawFillRect(x+2,y+h-2,w-4,1,C3);
    GPS.DrawFillRect(x+1,y+h-1,w-2,1,C31);
    GPS.FlushBatches();    
}
bool DrawTexFrame(int TextureID,int Lx,int Ly,TexFrame& tf,int x,int y,Rct& Window){
    Rct rxy(x+tf.x-tf.xc,y+tf.y-tf.yc,tf.w,tf.h);    
    if(Window.Clip(rxy)){
        Rct ruv=rxy;
        ruv.x-=x-tf.xc-0.5f;
        ruv.y-=y-tf.yc-0.5f;
        ruv.x/=Lx;
        ruv.w/=Lx;
        ruv.y/=Ly;
        ruv.h/=Ly;
        DWORD Color=GPS.GetCurrentDiffuse();
        rsQuad2D(Vector3D(rxy.x,rxy.y,0),Vector3D(rxy.x+rxy.w,rxy.y,0),Vector3D(rxy.x,rxy.y+rxy.h,0),Vector3D(rxy.x+rxy.w,rxy.y+rxy.h,0),Color,Color,Color,Color,ruv);
        return true;
    }
    return false;
}
void StdBorder::Draw(int x,int y,int x1,int y1){
    if(UsesTexture){
        rsSetTexture(Texture);
        static int sh=IRS->GetShaderID("hud");
        rsSetShader(sh);
        int Lx=IRS->GetTextureWidth(Texture);
        int Ly=IRS->GetTextureHeight(Texture);
        //filler
        if(TexFiller.w>0 && TexFiller.h>0){
            Rct Win(x+FillerLeftMargin,y+FillerTopMargin,x1-x-FillerLeftMargin-FillerRightMargin,y1-y-FillerTopMargin-FillerBottomMargin);
            int Nx=1+(Win.w/TexFiller.w);
            int Ny=1+(Win.h/TexFiller.h);
			TexFiller.xc=TexFiller.x;
			TexFiller.yc=TexFiller.y;
            for(int ix=0;ix<Nx;ix++){
                for(int iy=0;iy<Ny;iy++){
                    DrawTexFrame(Texture,Lx,Ly,TexFiller,x+ix*TexFiller.w,y+iy*TexFiller.h,Win);
                }
            }
        }
        //drawing corners
        DrawTexFrame(Texture,Lx,Ly,LT,x,y,Rct(0,0,(x+x1)/2,(y+y1)/2));
        DrawTexFrame(Texture,Lx,Ly,RT,x1,y,Rct((x+x1)/2,0,RealLx-(x+x1)/2,(y+y1)/2));
        DrawTexFrame(Texture,Lx,Ly,LB,x,y1,Rct(0,(y+y1)/2,(x+x1)/2,RealLy-(y+y1)/2));
        DrawTexFrame(Texture,Lx,Ly,RB,x1,y1,Rct((x+x1)/2,(y+y1)/2,RealLx-(x+x1)/2,RealLy-(y+y1)/2));        
        //drawing h/v lines
        if(Top.w>0){
            Top.xc=Top.x;
            float w=x1-x-(LT.x+LT.w-LT.xc)-(RT.xc-RT.x);
            if(w>0){
                //Rct TopWin(x+LT.w+LT.x-LT.xc,0,w,RealLy);
				Rct TopWin(x+LT.w+LT.x-LT.xc,0,w,(y+y1)/2);
                int N=1+(TopWin.w/Top.w);
                for(int i=0;i<N;i++){
                    DrawTexFrame(Texture,Lx,Ly,Top,TopWin.x+i*Top.w,y,TopWin);
                }
            }
        }
        if(Bottom.w>0){
            Bottom.xc=Bottom.x;
            float w=x1-x-(LB.x+LB.w-LB.xc)-(RB.xc-RB.x);
            if(w>0){
                //Rct BtmWin(x+LB.w+LB.x-LB.xc,0,w,RealLy);
				Rct BtmWin(x+LB.w+LB.x-LB.xc,(y+y1)/2,w,RealLy-(y+y1)/2);
                int N=1+(BtmWin.w/Bottom.w);
                for(int i=0;i<N;i++){
                    DrawTexFrame(Texture,Lx,Ly,Bottom,BtmWin.x+i*Bottom.w,y1,BtmWin);
                }
            }
        }
        if(Left.h>0){
            Left.yc=Left.y;
            float h=y1-y-(LT.y+LT.h-LT.yc)-(LB.yc-LB.y);
            if(h>0){
                //Rct LeftWin(0,y+LT.y+LT.h-LT.yc,RealLx,h);
				Rct LeftWin(0,y+LT.y+LT.h-LT.yc,(x+x1)/2,h);
                int N=1+(LeftWin.h/Left.h);
                for(int i=0;i<N;i++){
                    DrawTexFrame(Texture,Lx,Ly,Left,x,LeftWin.y+i*Left.h,LeftWin);
                }
            }
        }
        if(Right.h>0){
            Right.yc=Left.y;
            float h=y1-y-(RT.y+RT.h-RT.yc)-(RB.yc-RB.y);
            if(h>0){
                //Rct RightWin(0,y+RT.y+LT.h-RT.yc,RealLx,h);
				Rct RightWin((x+x1)/2,y+RT.y+LT.h-RT.yc,RealLx-(x+x1)/2,h);
                int N=1+(RightWin.h/Right.h);
                for(int i=0;i<N;i++){
                    DrawTexFrame(Texture,Lx,Ly,Right,x1,RightWin.y+i*Right.h,RightWin);
                }
            }
        }        
        rsFlush();
        rsSetTexture( -1 );
        rsRestoreShader();
    }else
    if(WindowsStyle){
        DWORD C=GPS.GetCurrentDiffuse();
        DrawWindowsStyleBorder(x,y,x1,y1,C);
    }else{
        if (GP_File!=0){
            if(NFillers)DrawFilledRect3(x,y,x1,y1,&Edges,GP_File,StartFiller,NFillers);
            else DrawRect4(x,y,x1,y1,&Edges,GP_File);
        }
    }
}
void StdBorder::TestBorder(){
	do{
		float T=GetTickCount()/1000.0f;
		IRS->ClearDevice(0xFF408040);
        ProcessMessages();
		int x0=RealLx/2-200.0f*(cos(T)+1);
		int y0=RealLy/2-150.0f*(cos(T*1.12)+1);
		int x1=RealLx/2+200.0f*(cos(T*1.15)+1);
		int y1=RealLy/2+150.0f*(cos(T*1.11)+1);
		Draw(x0,y0,x1,y1);
		if(!(GetKeyState(VK_MENU)&0x8000))rsFrame(Rct(x0,y0,x1-x0+1,y1-y0+1),0,0xFFFFFFFF); 
		rsFlush();
        FlipPages();
	}while(!KeyPressed);
	UnPress();
	KeyPressed=false;
}
void StdBorder::AutoCreateXCYC_byOuterMargin(){
	LT.xc=LT.x+LeftMarginParam;
	LT.yc=LT.y+TopMarginParam;
	RT.xc=RT.x+RT.w-RightMarginParam;
	RT.yc=RT.y+TopMarginParam;
	LB.xc=LB.x+LeftMarginParam;
	LB.yc=LB.y+LB.h-BottomMarginParam;
	RB.xc=RB.x+RB.w-RightMarginParam;
	RB.yc=RB.y+RB.h-BottomMarginParam;
	Left.xc=Left.x+LeftMarginParam;
	Right.xc=Right.x+Right.w-RightMarginParam;
	Top.yc=Top.y+TopMarginParam;
	Bottom.yc=Bottom.y+Bottom.h-BottomMarginParam;
}
void StdBorder::AutoCreateXCYC_byInnerMargin(){
	LT.xc=LT.x+LT.w-LeftMarginParam;
	LT.yc=LT.y+LT.h-TopMarginParam;
	RT.xc=RT.x+RightMarginParam;
	RT.yc=RT.y+RT.h-TopMarginParam;
	LB.xc=LB.x+LB.w-LeftMarginParam;
	LB.yc=LB.y+BottomMarginParam;
	RB.xc=RB.x+RightMarginParam;
	RB.yc=RB.y+BottomMarginParam;
	Left.xc=Left.x+Left.w-LeftMarginParam;
	Right.xc=Right.x+RightMarginParam;
	Top.yc=Top.y+Top.h-TopMarginParam;
	Bottom.yc=Bottom.y+BottomMarginParam;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////   Scene3D   ///////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "comms\comms.h"
void SceneCamera::Render(Rct &rc) {
    if(rc.w < 1 || rc.h < 1) return;   

	Matrix4D ViewTM;
	cVec3 At;	
    float toEye = SceneCamera::DistanceToEye;
    float Azim  = SceneCamera::Azimuth;
    float Elev  = SceneCamera::Elevation;
    float Roll  = SceneCamera::Roll;
	At.Copy((float *)&ViewPosition);

	CopyMemory((float *)&ViewTM, cMat4::OfLookAt(At, cMath::Rad(Azim), cMath::Rad(Elev), toEye, cMath::Rad(Roll), cMat4::CS_LEFTHANDED).ToFloatPtr(), sizeof(Matrix4D));

    float aspect = rc.h != 0?rc.w / rc.h : 1;
	float zn = zNear;
	float zf = zFar;
	float Fov = cMath::Rad(FOV);
	
	Matrix4D ProjTM;
	CopyMemory((float *)&ProjTM, cMat4::OfProjection(Fov, aspect, zn, zf).ToFloatPtr(), sizeof(Matrix4D));

    static Matrix4D MS ( 1, 0, 0, 0,
                         0, 0,-1, 0,
                         0, 1, 0, 0,
                         0, 0, 0, 1);
    ViewTM.mulLeft(MS);
	IRS->SetViewPort(rc);
	IRS->SetViewTM(ViewTM);
	IRS->SetProjTM(ProjTM);
	IRS->SetWorldTM(Matrix4D::identity);
    IRS->SetViewPort(rc);
    IRS->ClearDevice(0,false,true);
    /*
    Matrix4D wvpTM( IRS->GetWorldTM() );
    wvpTM *= IRS->GetViewTM();
    wvpTM *= IRS->GetProjTM();
    wvpTM.transpose();
    IRS->SetWorldViewProjTM(wvpTM);
    rsFlush();
    ISM->Flush();
    IRS->Flush();
    int id = IMM->GetModelID("Models\\Items\\IteIChDM.C2M");
    PushEntityContext(id);
    IMM->StartModel(id, Matrix4D::identity);
    IMM->DrawModel();
    PopEntityContext();
    ISM->Flush();
    IRS->Flush();
    */
}


void Scene3D::_Draw(){
    SceneCamera* SC=Camera.Get();
    if( SC && Scene.GetAmount() ){
        IEffMgr->SetCurrentSubset(DWORD(this));        
		int t=IRS->GetTexture(3);
		int tv = IRS->GetTextureOverride( 3 );
        IRS->SetTransparentTexOverride(3);
        bool ps=false;
        if(EditMode && GetKeyState(VK_PAUSE)){
            ps=true;
            IEffMgr->Pause(true);
        }
        Rct oldVP = IRS->GetViewPort();
        Rct vp(x,y,GetWidth(),GetHeight());
        Rct nvp( vp );
        oldVP.Clip( nvp );
        if (nvp.w < c_SmallEpsilon || nvp.h < c_SmallEpsilon){
            IEffMgr->SetCurrentSubset(0);
			IRS->SetTextureOverride( -1,3 );
			IRS->SetTexture( t,3 );
            return;
        }
        ICamera* pOldCam = GetCamera();
        SceneCamera* SC=Camera.Get();
        ISM->Flush();
        rsFlush();
        SC->Render(nvp);        
                
        Matrix4D M=Matrix4D::identity;
        PushEntityContext((DWORD)this);
        if(RenderEffects){
            IEffMgr->PreRender();
            IEffMgr->PostEvaluate();
        }
        for(int i=0;i<Scene.GetAmount();i++){
            Scene[i]->Render(M);
        }
		int Tc=GetTickCount();
		int dt=Tc-PrevRenderTime;
		PrevRenderTime=GetTickCount();
		if(dt>400)dt=400;
        if(RenderEffects){            
            IEffMgr->Evaluate(float(dt)/1000);
            IEffMgr->PostRender();
        }        
        IRS->Flush();
        PopEntityContext();
        IRS->SetViewPort( oldVP );
        if (pOldCam) pOldCam->Render();
        if(ps)IEffMgr->Pause(false);
        //IRS->SetTextureOverride( tv,3 );
		IRS->SetTextureOverride( -1,3 );
		IRS->SetTexture( t,3 );
        IEffMgr->SetCurrentSubset(0);
		static int prev_cleanup_time=Tc;

		if(RenderEffects && Tc-prev_cleanup_time>8000){
			prev_cleanup_time=Tc;
			IEffMgr->Evaluate(0.0001);
		}        
    }
}
void ModelInScene::Render(const Matrix4D& M){
    PushEntityContext(Model+(DWORD)this);
    int N=ChildElements.GetAmount();
    if(Model>0 || N>0){
        ISM->Flush();
        int T=GetTickCount();
        if(PrevTimePosition==0){
            PrevTimePosition=T;
        }
        int DT=T-PrevTimePosition;
        Matrix4D MC;
        if(Rotate){            
            float Angle=DT*RotationSpeed*c_PI/500.0f;
            CurrentAngle+=Angle;
            MC.rotation(Line3D(RotationPosition,RotationAxe),CurrentAngle);
            Matrix4D M1;
            M1.st(Scale,Position);
            MC*=M1;
        }else{
            MC.st(Scale,Position);
        }
        MC*=M;        
        if(Model>0){
            AABoundBox AB=IMM->GetBoundBox(Model);
            IMM->StartModel(Model,MC);
            if(Animation>0){
                CurrentTimePosition+=DT*PeriodScale;
                IMM->AnimateModel(Animation,CurrentTimePosition);
            }
            IMM->DrawModel();
            if(ExtraMesh){
                IRS->SetShader( ExtraMesh->getShader() );
                IRS->SetShaderAutoVars();
                IRS->SetTexture( ExtraMesh->getTexture( 0 ), 0, false );
                IRS->SetTexture( ExtraMesh->getTexture( 1 ), 1, false );
                DrawPrimBM( *ExtraMesh );
            }
            //IRS->Flush();
        }
        PrevTimePosition=T;
        for(int i=0;i<ChildElements.GetAmount();i++){
            ChildElements[i]->Render(MC);
        }
    }
    PopEntityContext();
}
void Load3DMap(char* Map);
void Load3DMapForIntro(char* Map);
bool CheckIfFileExists(char* Name);
char CurrentBackgroundMap[128]="";
extern int FogMode;
void BackgroundMap::_Draw(){
    if(Visible){
		extern bool NOPAUSE;
		extern bool LockPause;
		NOPAUSE = true;
		LockPause = false;

        if(MapName.cpchar()[0]){
            bool doit=false; 
            //if(!MapName.equal(CurrentBackgroundMap)){
			if(!MapName.equal(GSets.CGame.cgi_CurrentMap)){
                if(CheckIfFileExists(MapName.pchar())){
					ClearLoadMark();
					AddLoadMark("g_SurfEditor",5);
					AddLoadMark("LoadMap",70);						
					AddLoadMark("ResetGroundCache",5);
					ShowLoadProgress("g_SurfEditor",1,1);
					g_SurfEditor.Load();
                    extern bool ReducedLoadMode;
                    ReducedLoadMode=true;
                    Load3DMapForIntro(MapName.pchar());
					FogMode=false;
                    DriveMode()->NodeView=0; 
                    DriveMode()->NodeAct=4;
                    DriveMode()->EdgeView=0;
                    DriveMode()->fSHOW_ALL.EvaluateFunction();
                    ReducedLoadMode=false;
                    doit=true;
					// hack begin
					extern long StartAboutTime;
					StartAboutTime=GetTickCount();
					// hack end
                }
            }else doit=true;
            if(doit){
                if(!GameInterface::CheckIfCameraIsMoving()){
                    GameInterface::RunCameraTrack(MainTrackName.pchar());
                }
                byte K=LastKey;
                bool KP=KeyPressed;
                bool gm=v_MainMenu.GameMode;
                v_MainMenu.GameMode=true;
                GSets.CGame.ViewMask=0;
                void PreDrawGameProcess();
                PreDrawGameProcess();	
                void ProcessScreenWithoutMouse();
                ProcessScreenWithoutMouse();
                GPS.FlushBatches();
                void PostDrawGameProcess();
                PostDrawGameProcess();
                GSets.CGame.ViewMask=0xFF;
                v_MainMenu.GameMode=gm;
                LastKey=K;
                KeyPressed=KP;
            }
        }
	}else{
		CurrentBackgroundMap[0]=0;
	}
}
void TextureViewer::_Draw(){
	static int shader=IRS->GetShaderID("hud");
	static int shader_sm=IRS->GetShaderID("hud_smooth");
	float TLX=IRS->GetTextureWidth(TextureID);
	float TLY=IRS->GetTextureHeight(TextureID);
	if(TLX>0 && TLY>0){
		DrawTexturedBar(x,y,GetWidth(),GetHeight(),(U+0.5f)/TLX,(V+0.5f)/TLY,LU/TLX,LV/TLY,ColorLT,ColorRT,ColorLB,ColorRB,TextureID,Smooth?shader_sm:shader);
	}
}