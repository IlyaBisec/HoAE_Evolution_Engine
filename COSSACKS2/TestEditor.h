#ifndef __TESTEDITOR_H__
#define __TESTEDITOR_H__
#pragma pack( push )
#pragma pack( 1 )
#include "MapSprites.h"
#include "uiModelView.h"
#include "ClassEditorsRegistration.h"
int mrand();
class DescribedBaseClass:public BaseClass{
public:
	_str Description;
	virtual const char* GetThisElementView(const char* Local){
		static char cc[128];
		if(Description.pchar())
			sprintf(cc,"%s: %s",Local,Description.pchar()); 
		else
			sprintf(cc,"%s",Local); 
		return cc;
	};
};
template<class X>class Array:public ClonesArray<X>{
public:
	virtual const char* GetThisElementView(const char* Local){
		static char cc[128];
		sprintf(cc,"%s: %d",Local,NValues);
		return cc;
	}
};
class TexFact:public BaseClass{
public:
	int Texture;
	int Facture;
	int TexFactDz;
	TexFact(){
		TexFactDz=0;
	}
	SAVE(TexFact);
	REG_MEMBER(_gtexture,Texture);
	REG_MEMBER(_gfacture,Facture);
	REG_MEMBER(_int,TexFactDz);
	ENDSAVE;
	bool ForceSimplification(){
		return true;
	}
};

class Tree:public BaseClass{
public:
	int Trees;
	float MinDistance;
	float WholeDensity;
	_fractal Density;
	Tree(){
		WholeDensity=1.0f;
		MinDistance=0.3f;
	}
	SAVE(Tree)
	REG_ENUM(_index,Trees,TREESGROUPS);
	REG_MEMBER(_float01,MinDistance);
	REG_MEMBER(_float0_10,WholeDensity);
	REG_AUTO(Density);
	ENDSAVE
};
struct ExTexResult{
	int   tdz1;//texfactdz
	int   tdz2;//texfactdz
	int   t1;//textures
	int   t2;
	int   w1;//weights
	int   w2;
	int   f1;//factures
	int   f2;
	float fWeight1;
	float fWeight2;
    int   TreesGrp;//index of trees group
	float TreesDensity;
	float TreesDist;
	byte  LockMask;
};
class ExWeightMap:public BaseClass{
public:
    bool   Enabled;
	MapRef Map;
	ExWeightMap(){
		Enabled=0;
		Map.TopLayerIndex=3;
	}
	SAVE(ExWeightMap)
	REG_MEMBER(_bool,Enabled);
	REG_AUTO(Map);
	ENDSAVE
};
class ExLockInfo:public BaseClass{
public:
	ExLockInfo(){
		Lock0=0;
		Lock1=0;
		Lock2=0;
		Lock3=0;
	}
	bool Lock0;
	bool Lock1;
	bool Lock2;
	bool Lock3;
	SAVE(ExLockInfo)
	REG_MEMBER_EX(_bool,Lock0,LockLandUnits);
	REG_MEMBER_EX(_bool,Lock2,LockHorces);
	ENDSAVE
};
class Condition:public BaseClass{
public:
	int StartHeight;
	int FinHeight;
	int MaxHeightDiff;
	int StartAngle;
	int FinAngle;
	int MaxAngleDiff;
	int CheckDistance;
	int DistanceCheckType;//0-gradient,1-radius,2-isoline
	bool ExcludeCentralPoint;
	int TextureControl;//0-none,1-this,2-not this
	int FactureControl;//0-none,1-this,2-not this
	TexFact TexFactControl;
	SubSection Advanced;
	Condition(){
		StartHeight=0;
		FinHeight=0;
		MaxHeightDiff=0;
		StartAngle=0;
		FinAngle=0;
		MaxAngleDiff=0;
		CheckDistance=0;
		ExcludeCentralPoint=0;
		DistanceCheckType=0;
		TextureControl=0;
		FactureControl=0;
	}
	int GetCheck(int x,int y,int Height,int Angle,int Tex,int Fact,int FactW);
	int GetResult(int x,int y,int Height,int Angle,int Tex,int Fact,int FactW);
	const char* GetThisElementView(const char* Local){
		static char c[128];
		sprintf(c,"%s : H: %d-%d A: %d-%d ",Local,StartHeight,FinHeight,StartAngle,FinAngle);
		return c;
	}
	SAVE(Condition)
	REG_MEMBER(_int,StartHeight);
	REG_MEMBER(_int,FinHeight);
	REG_MEMBER(_int,MaxHeightDiff);
	REG_MEMBER(_int,StartAngle);
	REG_MEMBER(_int,FinAngle);
	REG_MEMBER(_int,MaxAngleDiff);
	REG_AUTO(Advanced);
	REG_MEMBER(_int,CheckDistance);
	REG_ENUM(_index,DistanceCheckType,DISTCHECKTYPE);
	REG_MEMBER(_bool,ExcludeCentralPoint);
	//REG_ENUM(_index,TextureControl,TextureControl);
	//REG_MEMBER_EX(_gtexture,TexFactControl.Texture,CheckTexture);
	//REG_ENUM(_index,FactureControl,FactureControl);
	//REG_MEMBER_EX(_gfacture,TexFactControl.Facture,CheckFacture);
	ENDSAVE
};
class Action:public BaseClass{
public:
	ExWeightMap Mask;
	bool Smoothing;
	float WholeWeight;
	_fractal WeightVariation;
	_fractal TexturesMixing;
	float FacturesWeight;
	_fractal FactWeightVariaton;
	Array<TexFact> Textures;
	ExLockInfo Locking;
    Array<Tree> Trees;
	SubSection Advanced;
	Action(){
		Smoothing=true;
		WholeWeight=1.0f;
		FacturesWeight=1.0f;
		WeightVariation.Scale=2.0f;
		WeightVariation.phaseX=mrand()%20;
		WeightVariation.phaseY=mrand()%20;

		WeightVariation.Contrast=1.0f;
		TexturesMixing.Scale=3.0f+float(mrand()%10)/10.0f;

		TexturesMixing.phaseX=mrand()%20;
		TexturesMixing.phaseY=mrand()%20;

		TexturesMixing.Contrast=1.0f;

		FactWeightVariaton.Center=0.5f;
		FactWeightVariaton.Contrast=1.0;
	}
	bool GetResult(int x,int y,int W0,ExTexResult& result);
	SAVE(Action)	
	REG_CLASS(TexFact);
	REG_AUTO(Textures);
	REG_AUTO(Locking);
	REG_CLASS(Tree);
	REG_AUTO(Trees);
	REG_AUTO(Advanced);
	REG_MEMBER(_bool,Smoothing);
	REG_AUTO(Mask);
	REG_MEMBER(_float0_10,WholeWeight);
	REG_AUTO(WeightVariation);
	REG_AUTO(TexturesMixing);
	REG_MEMBER(_float01,FacturesWeight);
	REG_AUTO(FactWeightVariaton);
	ENDSAVE
};
class ObjectRules:public DescribedBaseClass{
public:
	bool Enabled;
	Condition Condition;
	Action Action;
	ObjectRules(){
		Enabled=false;
	}
	SAVE(ObjectRules)
	REG_AUTO(Description);
	REG_MEMBER(_bool,Enabled);
	REG_AUTO(Condition);
	REG_AUTO(Action);
	ENDSAVE
};
class _grule_editor:public OneElementEditor{
public:
	BaseMeshDialog* BMD;
	virtual const char* GetClassID(){return "ObjectRules";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		y1=y+50;
		ObjectRules* R=(ObjectRules*)DataPtr;
		int count=R->Action.Textures.GetAmount();
		int Ly=y1-y+1;		
		if(count>0){
			int dx,nn,tex;
			int rw=CParam->RightColumnWidth;
			if(32*(1+count)>rw)
				dx=(rw)/(1+count);
			else
				dx=32;
			for(int i=0;i<2*count;i++){
				nn=i/2;
				tex=R->Action.Textures[nn]->Texture;
				BMD=Base->addBaseMeshDialog(x+(nn+i%2)*(dx-1),y,dx,Ly);
				BMD->CreateSquareTnL(float(tex%8)/8.0+float(i%2)*0.00195*float(dx),float(tex/8)/8.0f,float(tex%8)/8.0+float(1+i%2)*0.00195*float(dx),float(tex/8)/8.0f+0.00195*float(Ly),"hud",0xFFFFFFFF);
				if(i>0 && i<2*count-1){
					VertexTnL* V=(VertexTnL*)BMD->BM->getVertexData();
					if(i%2<1){
						V[0].diffuse=0x00FFFFFF;
						V[2].diffuse=0x00FFFFFF;
					}else{
						V[1].diffuse=0x00FFFFFF;
						V[3].diffuse=0x00FFFFFF;
					}
				}
				BMD->BM->setTexture(IRS->GetTextureID("Textures\\GroundTex.bmp"));
				BMD=Base->addBaseMeshDialog(x+(nn+i%2)*(dx-1),y,dx,Ly);
				BMD->CreateSquareTnL(float(i%2)*float(dx)/float(2*Ly),0,float(1+i%2)*float(dx)/float(2*Ly),0.5,"facture3",0xFFFFFFFF);
				if(i>0 && i<2*count-1){
					VertexTnL* V=(VertexTnL*)BMD->BM->getVertexData();
					if(i%2<1){
						V[0].diffuse=0x00FFFFFF;
						V[2].diffuse=0x00FFFFFF;
					}else{
						V[1].diffuse=0x00FFFFFF;
						V[3].diffuse=0x00FFFFFF;
					}
				}
				char cc[32];
				///sprintf(cc,"Ground\\tex%d.bmp",R->Action.Textures[nn]->Facture+1);
				int GetFacture(int id);
				BMD->BM->setTexture(GetFacture(R->Action.Textures[nn]->Facture));
			}
		}
		x=x1;
		int Lx=x1-x+2+75;
		Ly-=25;
		Canvas* CN=Base->AddCanvas(x,y+24,Lx,Ly);
		//CN->AddBar(0,0,Lx,Ly,0xFF000000);
		char cc[32];
		sprintf(cc,"H: %d - %d",R->Condition.StartHeight,R->Condition.FinHeight);
		CN->AddText(10,0,cc,&SmallBlackFont);
		sprintf(cc,"A: %d - %d",R->Condition.StartAngle,R->Condition.FinAngle);
		CN->AddText(10,14,cc,&SmallBlackFont);
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		return true;
	}
	virtual int Get(xmlQuote* xml){
		return true;
	}
	virtual OneElementEditor* _new(){
		return new _grule_editor;
	}
	virtual bool Process(){
		return true;
	}
};
class _color_editor:public OneElementEditor{
public:
	DWORD* COL;
	BYTE a,r,g,b;
	virtual const char* GetClassID(){return "_color";}
	CheckBox* Def;
	ComboBox* DefList;
	Enumerator* DefEnum;
	DWORD ToDWORD(){
		DWORD val;
		val &= 0x00FFFFFF;
		val |= (a << 24);
		val &= 0xFF00FFFF;
		val |= (r << 16);
		val &= 0xFFFF00FF;
		val |= (g << 8);
		val &= 0xFFFFFF00;
		val |= b;
		*COL=val;
		return val;
	}
	virtual bool Process(){
		return false;
	}
	static bool ColorDraw(SimpleDialog* SD){
		_color_editor* CE=(_color_editor*)SD->UserParam;
		if(CE->Def->State==1){
			DWORD c=CE->DefEnum->GetVal(CE->DefList->CurLine);
			CE->b=c; c>>=8;
			CE->g=c; c>>=8;
			CE->r=c; c>>=8;
			CE->a=c;
		}
		GPS.DrawLine(SD->x,SD->y,SD->x+40,SD->y+40,0xFFB0B0B0);
		GPS.DrawLine(SD->x,SD->y+40,SD->x+40,SD->y,0xFFB0B0B0);
		GPS.DrawFillRect(SD->x+1,SD->y+1,39,39,CE->ToDWORD());
		int x,Lx,Lx0,Lx1,Lx2,Lx3;
		x=SD->x+40;
		Lx=SD->x1-x;
		Lx0=CE->a*(SD->x1-x-30)/255;
		Lx1=CE->r*(SD->x1-x-30)/255;
		Lx2=CE->g*(SD->x1-x-30)/255;
		Lx3=CE->b*(SD->x1-x-30)/255;
		GPS.DrawRect(SD->x,SD->y,SD->x1-SD->x,40,0xFFB0B0B0);
		GPS.DrawLine(x,SD->y,x,SD->y+40,0xFFB0B0B0);
		GPS.DrawLine(SD->x1-30,SD->y,SD->x1-30,SD->y+40,0xFFB0B0B0);
		GPS.DrawLine(x,SD->y+10,x+Lx,SD->y+10,0xFFB0B0B0);
		GPS.DrawLine(x,SD->y+20,x+Lx,SD->y+20,0xFFB0B0B0);
		GPS.DrawLine(x,SD->y+30,x+Lx,SD->y+30,0xFFB0B0B0);
		GPS.DrawLine(x+Lx0+1,SD->y,x+Lx0+1,SD->y+10,0xFFB0B0B0);
		GPS.DrawLine(x+Lx1+1,SD->y+10,x+Lx1+1,SD->y+20,0xFFB0B0B0);
		GPS.DrawLine(x+Lx2+1,SD->y+20,x+Lx2+1,SD->y+30,0xFFB0B0B0);
		GPS.DrawLine(x+Lx3+1,SD->y+30,x+Lx3+1,SD->y+40,0xFFB0B0B0);
		GPS.DrawFillRect(x+1,SD->y+1,Lx0,9,0xFFFFFFFF);
		GPS.DrawFillRect(x+1,SD->y+11,Lx1,9,0xFFFF0000);
		GPS.DrawFillRect(x+1,SD->y+21,Lx2,9,0xFF00FF00);
		GPS.DrawFillRect(x+1,SD->y+31,Lx3,9,0xFF0000FF);
		char cc[32];
		ShowString(SD->x1-26,SD->y-1,"a",&SmallWhiteFont);
		ShowString(SD->x1-26,SD->y+9,"r",&SmallWhiteFont);
		ShowString(SD->x1-26,SD->y+19,"g",&SmallWhiteFont);
		ShowString(SD->x1-26,SD->y+29,"b",&SmallWhiteFont);
		sprintf(cc,"%d",CE->a);
		ShowString(SD->x1-18,SD->y-1,cc,&SmallWhiteFont);
		sprintf(cc,"%d",CE->r);
		ShowString(SD->x1-18,SD->y+9,cc,&SmallWhiteFont);
		sprintf(cc,"%d",CE->g);
		ShowString(SD->x1-18,SD->y+19,cc,&SmallWhiteFont);
		sprintf(cc,"%d",CE->b);
		ShowString(SD->x1-18,SD->y+29,cc,&SmallWhiteFont);
		return true;
	}
	static bool ColorClick(SimpleDialog* SD){
		_color_editor* CE=(_color_editor*)SD->UserParam;
		if(CE->Def->State==1){
			return false;
		}
		int dx=mouseX-SD->LastDrawX;
		int dy=mouseY-SD->LastDrawY;
		if(dx>0 && dx<SD->x1-SD->x){
			dx-=40;
			if(dx<0)dx=0;
			if(dx>SD->x1-SD->x-70)dx=SD->x1-SD->x-70;
			if(dy>0 && dy<10){
				CE->a=255*dx/(SD->x1-SD->x-70);
			}else if(dy>10 && dy<20){
				CE->r=255*dx/(SD->x1-SD->x-70);
			}else if(dy>20 && dy<30){
				CE->g=255*dx/(SD->x1-SD->x-70);
			}else if(dy>30 && dy<40){
				CE->b=255*dx/(SD->x1-SD->x-70);
			}
		}
		return false;
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		y1=y+41;
		SimpleDialog* SD=Base->addViewPort(x,y,x1-x-1,y1-y);
		SD->Enabled=1;
		SD->Visible=1;
		COL=(DWORD*)DataPtr;
		SD->UserParam=int(this);
		SD->OnDraw=&ColorDraw;
		SD->OnUserClick=&ColorClick;
        a=(*COL)>>24;
		r=((*COL)>>16)&255;
		g=((*COL)>>8)&255;
		b=(*COL)&255;
		//
		Def=new CheckBox;
		Def->Setx(x);
		Def->Sety(y1);
		Base->DSS.Add(Def);
		DefList=new ComboBox;
		DefList->Setx(x+40);
		DefList->SetWidth(x1-x-40-1);
		DefList->Sety(y1);
		DefEnum=ENUM.Get("PreDefColor");
		for(int i=0;i<DefEnum->GetAmount();i++){
			DefList->AddLine(DefEnum->GetStr(i));
		}
		Base->DSS.Add(DefList);
		y1+=24;
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		return true;
	}
	virtual int Get(xmlQuote* xml){
		char cc[32];
		sprintf(cc,"%x",*COL);
		xml->Assign_string(cc);
		return true;
	}
	virtual OneElementEditor* _new(){
		return new _color_editor;
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////

class _button:public OneElementEditor{
public:
	//BYTE State;
	ClassEditor* CE;
	BaseClass* Element;
    void*      _ptr;
	static DWORD Color;
	virtual DWORD GetColor(){
		return 0xFFB0B0B0;
	}

	static bool ButtonDraw(SimpleDialog* SD){
		_button* TB=(_button*)SD->UserParam;		
		BaseClass* Elm=dynamic_cast<BaseClass*>(TB->Element);		
		if(Elm){
			if(SD->GetHeight()>8){
				SD->y+=CED_BtTopDy;
				SD->y1+=CED_BtBotDy;
			}
			try{			
				int s=Elm->GetSectionStatus();
				if((s&1)||Elm->CheckIfFunction()){
					DWORD C=TB->GetColor();
					GPS.DrawLine(SD->x,SD->y,SD->x1+1,SD->y,MixDWORD(0xFFFFFFFF,C,100,155));
					GPS.DrawLine(SD->x+1,SD->y+1,SD->x1,SD->y+1,MixDWORD(0xFFFFFFFF,C,155,100));
					GPS.DrawLine(SD->x,SD->y,SD->x,SD->y1,MixDWORD(0xFFFFFFFF,C,100,155));
					GPS.DrawLine(SD->x+1,SD->y+1,SD->x+1,SD->y1-1,MixDWORD(0xFFFFFFFF,C,155,100));
					GPS.DrawLine(SD->x1,SD->y,SD->x1,SD->y1,MixDWORD(0xFF000000,C,155,100));
					GPS.DrawLine(SD->x1-1,SD->y+1,SD->x1-1,SD->y1-1,MixDWORD(0xFF000000,C,100,155));
					GPS.DrawLine(SD->x,SD->y1,SD->x1,SD->y1,MixDWORD(0xFF000000,C,155,100));
					GPS.DrawLine(SD->x+1,SD->y1-1,SD->x1-1,SD->y1-1,MixDWORD(0xFF000000,C,100,155));
					DWORD CU=MixDWORD(0xFFFFFFFF,C,30,255-30);
					DWORD CD=MixDWORD(0xFF000000,C,30,255-30);
					GPS.DrawFillRect(SD->x+2,SD->y+2,SD->x1-SD->x-3,SD->y1-SD->y-3,CU,CU,CD,CD);
					GPS.FlushBatches();
					if(s==1){
						int y=(SD->y1+SD->y)/2;
						GPS.DrawLine(SD->x+4,y,SD->x+16,y,0xFF000000);
						GPS.DrawLine(SD->x+4,y+1,SD->x+16,y+1,0xFF000000);
						GPS.DrawLine(SD->x+9,SD->y+3,SD->x+9,SD->y1-3,0xFF000000);
						GPS.DrawLine(SD->x+10,SD->y+3,SD->x+10,SD->y1-3,0xFF000000);
						//GPS.DrawLine(SD->x+4,SD->y+13,SD->x+14,SD->y+13,0xFF000000);
						//GPS.DrawLine(SD->x+9,SD->y+3,SD->x+4,SD->y+13,0xFF000000);
						//GPS.DrawLine(SD->x+9,SD->y+3,SD->x+14,SD->y+13,0xFF000000);
					}
				}else{
					DWORD C=TB->GetColor();
					GPS.DrawLine(SD->x-1,SD->y-1,SD->x1,SD->y-1,MixDWORD(0xFF000000,C,155,100));
					GPS.DrawLine(SD->x-1,SD->y-1,SD->x-1,SD->y1-1,MixDWORD(0xFF000000,C,155,100));
					GPS.DrawLine(SD->x1-1,SD->y-1,SD->x1-1,SD->y1-1,MixDWORD(0xFF000000,C,200,55));
					GPS.DrawLine(SD->x,SD->y1-1,SD->x1-1,SD->y1-1,MixDWORD(0xFF000000,C,200,55));
					DWORD CU=MixDWORD(0xFFFFFFFF,C,30,255-30);
					DWORD CD=MixDWORD(0xFF000000,C,30,255-30);
					GPS.DrawFillRect(SD->x,SD->y,SD->x1-SD->x-1,SD->y1-SD->y-1,CD,CD,CU,CU);
					GPS.FlushBatches();
					if(s==2){
						int y=(SD->y1+SD->y)/2;
						GPS.DrawLine(SD->x+4,y,SD->x+14,y,0xFF000000);						
						GPS.DrawLine(SD->x+4,y+1,SD->x+14,y+1,0xFF000000);						
						//GPS.DrawLine(SD->x+4,SD->y+3,SD->x+14,SD->y+3,0xFF000000);
						//GPS.DrawLine(SD->x+4,SD->y+3,SD->x+9,SD->y+13,0xFF000000);
						//GPS.DrawLine(SD->x+14,SD->y+3,SD->x+9,SD->y+13,0xFF000000);
					}
				}				
			}catch(...){};
			if(SD->GetHeight()>8){
				SD->y-=CED_BtTopDy;
				SD->y1-=CED_BtBotDy;
			}
		}
		return true;
	}
	static bool ButtonClick(SimpleDialog* SD){
		void UnPress();
		UnPress();
		ClassEditorProcessor::ClearPress();
		_button* TB=(_button*)SD->UserParam;
		int s=TB->Element->GetSectionStatus();
		if(s){
            if(s==1)s=2;
			else s=1;
			TB->Element->SetSectionStatus(s);
			TB->CE->ExpInfChanged=1;			
		}
		if(TB->Element->CheckIfFunction()){
			//TB->Element->EvaluateFunction();
            TB->Element->call(TB->_ptr,NULL);
			TB->CE->ExpInfChanged=1;			
		}		
		return false;
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		SimpleDialog* SD=Base->addViewPort(x-CParam->LeftColumnWidth+CParam->AbsMinLeftWidth-42,y+3,x1-x+CParam->LeftColumnWidth-CParam->AbsMinLeftWidth+40,y1-y-5);
		SD->x1-=4;
		CParam->LeftColumnWidth+=CParam->RightColumnWidth;
		CParam->GP_dx=int((CParam->LeftColumnWidth-CParam->AbsMinLeftWidth-CParam->MinLeftWidth+16)/2);
		CParam->TextFont=&CED_BFont;
		SD->OnDraw=&ButtonDraw;
		//State=(BYTE*)DataPtr;
		CE=CParam->CE;
		SD->UserParam=int(this);
		SD->OnUserClick=&ButtonClick;		
        _ptr=DataPtr;
        if(Class->CheckDirectCasting())Element=(BaseClass*)DataPtr;
        else Element=Class;
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		return true;
	}
	virtual int Get(xmlQuote* xml){
		return true;
	}
};
class _button_editor:public _button{
public:
	virtual DWORD GetColor(){
		return 0xFF70B070;
	}
	_button_editor(){
		//State=1;
	}
	virtual const char* GetClassID(){
		return "SubSection";
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _button_editor;
	}
};
class Delimiter_editor:public _button{
public:
	virtual DWORD GetColor(){
		return 0xFF70A0A0;
	}
	virtual const char* GetClassID(){
		return "Delimiter";
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		SimpleDialog* SD=Base->addViewPort(x-CParam->LeftColumnWidth,y,x1-x+CParam->LeftColumnWidth,5);
		CParam->LeftColumnWidth+=CParam->RightColumnWidth;
		CParam->GP_dx=int((CParam->LeftColumnWidth-CParam->AbsMinLeftWidth-CParam->MinLeftWidth)/2);
		CParam->TextFont=&CED_BFont;
		SD->OnDraw=&ButtonDraw;
		CE=CParam->CE;
		SD->UserParam=int(this);		
		Element=(BaseClass*)DataPtr;
		CParam->HideText=true;
		return y+5;
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new Delimiter_editor;
	}
};
class _function_editor:public _button{
public:
	virtual DWORD GetColor(){
		return 0xFFB07070;
	}
	_function_editor(){
		//State=3;
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		SimpleDialog* SD=Base->addViewPort(x-CParam->LeftColumnWidth+CParam->AbsMinLeftWidth-42,y+3,x1-x+CParam->LeftColumnWidth-CParam->AbsMinLeftWidth+40,y1-y-5);
		SD->x+=20;
		SD->x1-=24;
		CParam->LeftColumnWidth+=CParam->RightColumnWidth;
		CParam->GP_dx=int((CParam->LeftColumnWidth-CParam->AbsMinLeftWidth-CParam->MinLeftWidth+16)/2);
		CParam->TextFont=&CED_BFont;
		SD->OnDraw=&ButtonDraw;
		//State=(BYTE*)DataPtr;
		CE=CParam->CE;
		SD->UserParam=int(this);
		SD->OnUserClick=&ButtonClick;
        _ptr=DataPtr;
        if(Class->CheckDirectCasting())Element=(BaseClass*)DataPtr;
        else Element=Class;
		return y1;
	}
	//virtual const char* GetClassID(){
	//	return "BaseFunction";
	//}
    virtual bool CheckClass(BaseClass* BC){
        return BC->CheckIfFunction();
    }
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _function_editor;
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////

class TestButton: public BaseClass{
public:
	SubSection S1;
	int x1;
	DWORD color;
	int x2;
	SubSection S2;
	int y2;
	int y3;
	SAVE(TestButton);
	REG_AUTO(S1);
	REG_MEMBER(_int,x1);
	REG_MEMBER(_color,color);
	REG_MEMBER(_int,x2);
	REG_AUTO(S2);
	REG_MEMBER(_int,y2);
	REG_MEMBER(_int,y3);
	ENDSAVE;
};

/////////////////////////////////////////////////////////////////////////////////////////
class _sprite:public _int{
public:
	virtual const char* GetClassName(){return "_sprite";};
	AUTONEW(_sprite);
};
class _sprite_editor:public OneElementEditor{
public:
	short* GPF;
	int* spriteID;
	char pstr[32];
	InputBox* IB;
	GP_Button* Plus;
	GP_Button* Minus;	
	virtual const char* GetClassID(){return "_sprite";}
	static bool SpriteDraw(SimpleDialog* SD){
		_sprite_editor* SE=(_sprite_editor*)SD->UserParam;
		if(GetKeyState(VK_F3)&0x8000){
			*SE->GPF++;
			sprintf(SE->pstr,"%d",0);
		}
		if(GetKeyState(VK_F4)&0x8000){
			*SE->GPF--;
			sprintf(SE->pstr,"%d",0);
		}
		if(SE->GPF<0)*SE->GPF=0;
		Rct F;
		GPS.GetGPBoundFrame(*SE->GPF,*SE->spriteID,F);
		float scale=F.h?68.0f/F.h:1.0f;
		if(scale>1.0)scale=1.0;
		if(F.w*scale>SD->x1-SD->x-34) scale=(SD->x1-SD->x-34.0f)/F.w;
		GPS.FlushBatches();
		GPS.SetScale(scale);
		GPS.EnableZBuffer(0);
		GPS.ShowGP((SD->x1+SD->x-F.w*scale)/2.0-F.x*scale+12.0,(SD->y1+SD->y-F.h*scale)/2.0-F.y*scale+1.0,*SE->GPF,*SE->spriteID,0);
		GPS.FlushBatches();
		GPS.SetScale(1.0);
		GPS.DrawLine(SD->x,SD->y1-48,SD->x+25,SD->y1-48,0xFFB0B0B0);
		GPS.DrawLine(SD->x,SD->y1-24,SD->x+25,SD->y1-24,0xFFB0B0B0);
		GPS.DrawLine(SD->x+25,SD->y,SD->x+25,SD->y1,0xFFB0B0B0);
		return true;
	}
	static bool SpriteClick(SimpleDialog* SD){
		_sprite_editor* SE=(_sprite_editor*)SD->UserParam;
		int dx=mouseX-SD->LastDrawX;
		if(dx>0 && dx<24){
			int dy,id,add,max;
			dy=mouseY-SD->LastDrawY-24;
			id=atoi(SE->pstr);
			if(GetKeyState(VK_CONTROL)&0x8000) add=10;
			else add=1;
			if(dy>0 && dy<24) id+=add;
			if(dy>24 && dy<48) id-=add;
			max=GPS.GPNFrames(*SE->GPF);
			if(id<-1)id=max-1;
			if(id>=max)id=-1;
			sprintf(SE->pstr,"%d",id);
		}
		Lpressed=false;
		return false;
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		IB=Base->addInputBox(NULL,x+1,y+2,pstr,4,25,y1-y+1,&CED_Font,&CED_AFont);
		IB->Centering=true;
		static int idx=GPS.PreLoadGPImage("Interf3\\btnz");
		Plus=Base->addGP_Button(NULL,x,y1+1,idx,5,4);
		Minus=Base->addGP_Button(NULL,x,y1+25,idx,7,6);
		y1+=48;
		//if(!*GPF) *GPF=24;
		SimpleDialog* SD=Base->addViewPort(x,y,x1-x-1,y1-y);
		SD->Enabled=true;
		SD->Visible=true;
		SD->UserParam=int(this);
		SD->OnDraw=&SpriteDraw;
		SD->OnUserClick=&SpriteClick;
		spriteID=(int*)DataPtr;
		GPF=(short*)(int(spriteID)+int(ExtraPtr));
		sprintf(pstr,"%d",*spriteID);
		return y1;
	}
	virtual bool Process(){
		int id=atoi(pstr);
		if(IB->Active){
			int max=GPS.GPNFrames(*GPF);
			if(LastKey==VK_UP){
				id++;
				if(id>=max)id=-1;
				LastKey=0;
			}else
			if(LastKey==VK_DOWN){
				id--;
				if(id<-1)id=max-1;
				LastKey=0;
			}
			sprintf(pstr,"%d",id);
		}
		*spriteID=id;
		IB->Str=pstr;
		return true;
	}	
	virtual OneElementEditor* _new(){
		return new _sprite_editor;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////
class _ModelID_editor:public OneElementEditor{
public:
	int id;
	int* model_id;
	int ModelShift;
	char str[256];
	char pstr[256];
	LS_gp_param* p;
	static DynArray<DString*> FileArr;
	static int LastRefreshTime;
	InputBox* IB;
	GP_Button*  Plus;
	GP_Button*  Minus;    
	virtual const char* GetClassID(){return "_ModelID";}
	~_ModelID_editor(){	
	}
	void ReadDir(char* Pass);
	static bool ModelDraw(SimpleDialog* SD);
	static bool MinusClick(SimpleDialog* SD);
	static bool PlusClick(SimpleDialog* SD);
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);		
	virtual bool Process();
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _ModelID_editor;
	}
};
///////////////////////////////////////////////////////////////////////////////////////
class _ObjCharacter_editor: public OneElementEditor{
public:
	bool show;
	short GPF;
	int spriteID;
	int Radius;
	int CenterX;
	int CenterY;
	int ClickX;
	int ClickY;
	char pstr[32];
	InputBox* IB;
	GP_Button* Plus;
	GP_Button* Minus;
	virtual const char* GetClassID(){return "ObjCharacter";}
	static bool ObjCharacterDraw(SimpleDialog* SD){
		int wx, wy, ww, wh;
		g_GetWindParam( wx, wy, ww, wh );
		if(SD->y<wy + wh && SD->y1>wy){
			_ObjCharacter_editor* SE=(_ObjCharacter_editor*)SD->UserParam;
			Rct F;
			GPS.GetGPBoundFrame(SE->GPF,SE->spriteID,F);
			float scale=F.h?90.0f/F.h:1.0f;
			if(scale>1.0)scale=1.0;
			if(F.w*scale>SD->x1-SD->x-34) scale=(SD->x1-SD->x-34.0f)/F.w;
			GPS.FlushBatches();
			GPS.SetScale(scale);
			GPS.EnableZBuffer(0);
			int x0=(SD->x1+SD->x-F.w*scale)/2.0-F.x*scale+12.0;
			int y0=(SD->y1+SD->y-F.h*scale)/2.0-F.y*scale+1.0;
			GPS.ShowGP(x0,y0,SE->GPF,SE->spriteID,0);
			GPS.FlushBatches();
			GPS.SetScale(1.0);
			GPS.DrawLine(SD->x+25,SD->y,SD->x+25,SD->y1,0xFFB0B0B0);
			SE->ClickX+=SD->x;
			SE->ClickY+=SD->y;
			if(SE->ClickX>x0+F.x*scale && SE->ClickX<x0+(F.w+F.x)*scale 
				&& SE->ClickY>y0+F.y*scale && SE->ClickY<y0+(F.h+F.y)*scale){
					SE->CenterX=(SE->ClickX-x0)/scale;
					SE->CenterY=(SE->ClickY-y0)/scale;
			}
			int cx=x0+SE->CenterX*scale;
			int cy=y0+SE->CenterY*scale;
			int radius=SE->Radius*scale;
			GPS.DrawLine(cx-4,cy-4,cx+5,cy+5,0x80EBEBEB);
			GPS.DrawLine(cx-4,cy+4,cx+5,cy-5,0x80EBEBEB);
			GPS.DrawLine(cx-radius,cy-5,cx-radius,cy+4,0x80EBEBEB);
			GPS.DrawLine(cx+radius,cy-5,cx+radius,cy+4,0x80EBEBEB);
			GPS.DrawLine(cx-radius,cy,cx+radius,cy,0x80EBEBEB);
		}
		return true;
	}
	static bool ObjCharacterClick(SimpleDialog* SD){
		_ObjCharacter_editor* SE=(_ObjCharacter_editor*)SD->UserParam;
		int dx=mouseX-SD->LastDrawX;
		int dy=mouseY-SD->LastDrawY-25;
		if(dx>0 && dx<24){
			int id,add,max;
			id=atoi(SE->pstr);
			if(GetKeyState(VK_CONTROL)&0x8000) add=10;
			else add=1;
			if(dy>0 && dy<25) id+=add;
			if(dy>50 && dy<75) id-=add;
			max=GPS.GPNFrames(SE->GPF);
			if(id<0)id=max-1;
			if(id>=max)id=0;
			sprintf(SE->pstr,"%d",id);
		}
		if(dx>25 && dx<SD->x1-SD->x){
			SE->ClickX=dx;
			SE->ClickY=dy+25;
		}
		Lpressed=false;
		return false;
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		show=false;
		ObjCharacter* OC=(ObjCharacter*)DataPtr;
		if(!OC->ViewType){
			show=true;
			IB=Base->addInputBox(NULL,x,y+3,pstr,4,25,y1-y+1,&CED_Font,&CED_AFont);
			IB->Centering=true;
			int idx=GPS.PreLoadGPImage("Interf3\\btnz");
			Plus=Base->addGP_Button(NULL,x,y+25,idx,5,4);
			Minus=Base->addGP_Button(NULL,x,y+75,idx,7,6);
			y1=y+100;
			SimpleDialog* SD=Base->addViewPort(x,y,x1-x-1,y1-y);
			SD->Enabled=1;
			SD->Visible=1;
			SD->UserParam=int(this);
			SD->OnDraw=&ObjCharacterDraw;
			SD->OnUserClick=&ObjCharacterClick;
			spriteID=OC->SpriteID;
			GPF=OC->FileID;
			Radius=OC->Radius;
			CenterX=OC->CenterX;
			CenterY=OC->CenterY;
			sprintf(pstr,"%d",spriteID);
		}
		return y1;
	}
	virtual bool Process(){
		if(show){
			spriteID=atoi(pstr);
			IB->Str=pstr;
		}
		return true;
	}	
	virtual OneElementEditor* _new(){
		return new _ObjCharacter_editor;
	}
};
//////////////////////////////////////////////////////////////////////////////////////////
#pragma pack( pop )
#endif 