#pragma once
#ifndef __CLASSEDITOR_H__
#define __CLASSEDITOR_H__

void addmr1(int v,char* s,int L);
#define addmr(v) addmr1(v,__FILE__,__LINE__)

#pragma pack( push )
#pragma pack( 1 )
#define CED_BFont SmallBlackFont
#define CED_Font SmallYellowFont
#define CED_AFont SmallWhiteFont
#define CED_BtTopDy -1
#define CED_BtBotDy 2
#define CED_MEMBERS 1
#define CED_TREE 2
#define CED_EMPTY 4
#define CED_Border "StandartBorder"
#define CED_BackColor 0xFF404040
#define CED_FieldColor 0xAF606060
#define CED_SelFieldColor 0xAF60A060
#define CED_LineDist 18
#define bt_yscale (CED_LineDist/24.0f)
#define CED_SqSize 4
#define CED_LevelWidth 12
#define CED_TextDy 2

//#define CED_LineDist 20

extern IRenderSystem*	IRS;
#define TEXFORMAT cfARGB4444
#define TEXPIXSIZE 2
#define REG_AUTO_EX2(x,Name) reg_auto(this,x,Name);reg_class(&x)
float sq_norma(float x,float y);
class ClassEditor;
struct ElementParam;
struct ControlParams
{
	int  LeftColumnWidth;
	int  RightColumnWidth;
	int  BaseClassType;
	int  MinLeftWidth;
	int  AbsMinLeftWidth;
	bool VerticalText;
	bool HideText;
	int  IconFile;
	int  IconSprite;
	int  GP_dx;
	int  GP_dy;
	int  GP_Lx;
	ElementParam* ELP;
	RLCFont* TextFont;
	ClassEditor* CE;
};

class DynTexManager{
public:
	static int NTX;
	static DynArray<int> TexList;
	static int GetTexture(){
		if(TexList.GetAmount()){
			int v=TexList[0];
			TexList.Del(0,1);
			return v;
		}else{
			char cc[32];
			sprintf(cc,"temptex%d",NTX++);
			int TextureID=IRS->CreateTexture(cc,256,256,TEXFORMAT,1,tmpManaged);
			return TextureID;
		}
	}
	static void FreeTexture(int idx){
		TexList.Add(idx);
	};
};
class ClassEditor;
class OneElementEditor{
public:
	virtual ~OneElementEditor(){
	}
	ClassEditor* CE;
	virtual const char* GetClassID(){return NULL;}
	virtual bool CheckClass(BaseClass* BC){return false;}
	virtual int CreateControl	(ParentFrame* Base,int x,int y,int x1,int y1,
								 BaseClass* BC,void* Ptr,void* ExtraPtr,
								 ControlParams* CParam){return y1;}
	//you should use this function to create editing controls, (x,y,x1,y1) - desirable
    //coordinates of control, Base - container for controls, CParam - extra parameters, see ControlParams

	virtual void  Assign		(BaseClass* BC,void* Ptr,void* ExtraPtr);
	//Class Editor calls Assign to assign value to the control. It occurs on start initialisation
	//and also when element chenges outside the editor
	
	virtual int   Get			(BaseClass* BC,void* Ptr,void* ExtraPtr);
	//Class Editor calls Get to obtain data from the control. It occurs every timer tick, so
	//very often. Get should return 2 if recreating of editor required.

	virtual DWORD GetHashValue	(BaseClass* BC,void* Ptr,void* ExtraPtr);
	//Class Editor uses this function to determine if value of edited element has changed outside
	//of its editor. If value changes, CE calls Assign

	//old style editor functions, they saved for compartability, 
	//don't use them for new editors

	virtual bool Assign(xmlQuote* xml){return true;}
	virtual int Get(xmlQuote* xml){return true;}
	virtual bool Process(){
		return false;
	}

	//new style of Assign/Get
	/*
	virtual bool AssignIfChanged(BaseClass* BC,void* DataPtr,void* ExtraPtr,int& HashValue){
		xmlQuote xml;
		BC->Save(xml,DataPtr,ExtraPtr);
		DWORD GetXML_Hash(xmlQuote& xml);
		int V=GetXML_Hash(xml);
		if(V!=HashValue){
			HashValue=V;
			return Assign(&xml);
		}else return false;
	}
	virtual int Get(BaseClass* BC,void* DataPtr,void* ExtraPtr,int& HashValue){
		xmlQuote xml;
		int r=Get(&xml);
		if(r){
			ErrorPager EP(1);
			BC->Load(xml,DataPtr,&EP,ExtraPtr);
			DWORD GetXML_Hash(xmlQuote& xml);
			HashValue=GetXML_Hash(xml);
		}
		return r;
	}
	*/

	virtual OneElementEditor* _new(){
		return new OneElementEditor;
	}
	virtual bool GetVersion(){
		return 0;
	}
};
//---------------------------------------------------------------------------------------------------------------//
class _test_editor:public OneElementEditor
{
public:
	char str[32];
	virtual const char* GetClassID(){return "_test";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam)
		{
			//Base->addInputBox(NULL,x,y,str,16,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
			CParam->LeftColumnWidth+=CParam->RightColumnWidth;
			CParam->RightColumnWidth=0;
			CParam->BaseClassType=100;
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){
			strcpy(str,xml->Get_string());
			return true;
		}
		virtual int Get(xmlQuote* xml){
			//int i=atoi(str);
			//xml->Assign_int(i);
			return true;
		}
		virtual OneElementEditor* _new(){
			return (OneElementEditor*)new _test_editor;
		}
		virtual bool Process(){
			return true;
		}

};
//---------------------------------------------------------------------------------------------------------------//
class _int_editor:public OneElementEditor{
public:
	char str[32];
	InputBox* IB;
	virtual const char* GetClassID(){return "_int";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		IB=Base->addInputBox(NULL,x,y,str,16,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		if(!IB->Active)strcpy(str,xml->Get_string());
		return true;
	}
	virtual int Get(xmlQuote* xml){
        int i=atoi(str);
		xml->Assign_int(i);
		return true;
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _int_editor;
	}
};
class _short_editor: public OneElementEditor{
public:
	char str[32];
	InputBox* IB;
	virtual const char* GetClassID(){return "_short";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
			IB=Base->addInputBox(NULL,x,y,str,16,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){
			if(!IB->Active)strcpy(str,xml->Get_string());
			return true;
		}
		virtual int Get(xmlQuote* xml){
			int i=atoi(str);
			xml->Assign_int(i);
			return true;
		}
		virtual OneElementEditor* _new(){
			return (OneElementEditor*)new _short_editor;
		}
};
class _BYTE_editor:public OneElementEditor{
public:
	char str[32];
	InputBox* IB;
	virtual const char* GetClassID(){return "_BYTE";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
			IB=Base->addInputBox(NULL,x,y,str,16,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){
			if(!IB->Active)strcpy(str,xml->Get_string());
			return true;
		}
		virtual int Get(xmlQuote* xml){
			int i=atoi(str);
			xml->Assign_int(i);
			return true;
		}
		virtual OneElementEditor* _new(){
			return (OneElementEditor*)new _BYTE_editor;
		}
};
class _float_editor:public OneElementEditor{
public:
	char str[32];
	InputBox* IB;
	virtual const char* GetClassID(){return "_float";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
			IB=Base->addInputBox(NULL,x,y,str,16,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){
			if(!IB->Active)strcpy(str,xml->Get_string());
			return true;
		}
		virtual int Get(xmlQuote* xml){
			float V=0;
			int z=sscanf(str,"%f",&V);
			char cc[32];
			sprintf(cc,"%f",V);
			xml->Assign_string(cc);
			return true;
		}
		virtual OneElementEditor* _new(){
			return (OneElementEditor*)new _float_editor;
		}
};
class _DWORD_editor:public OneElementEditor{
public:
	char str[32];
	InputBox* IB;
	virtual const char* GetClassID(){return "_DWORD";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
			IB=Base->addInputBox(NULL,x,y,str,16,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){
			if(!IB->Active)strcpy(str,xml->Get_string());
			return true;
		}
		virtual int Get(xmlQuote* xml){
			DWORD V=0;
			int z=sscanf(str,"%X",&V);
			char cc[32];
			sprintf(cc,"%X",V);
			xml->Assign_string(cc);
			return true;
		}
		virtual OneElementEditor* _new(){
			return (OneElementEditor*)new _DWORD_editor;
		}
};
class ClassRef_editor:public OneElementEditor{
public:
	ComboBox* List;
	DynArray<DWORD> PIDS;
	void* cptr;
    
	virtual bool CheckClass(BaseClass* BC){
		const char* s=BC->GetClassName();
		const char* s1=strstr(s,"ClassRef.");
		return s1==s;	
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
	virtual bool Assign(xmlQuote* xml){		
		return true;
	}
	virtual int Get(xmlQuote* xml){		
		return true;
	}
	virtual OneElementEditor* _new(){
		return new ClassRef_editor;
	}
};
class _index_editor:public OneElementEditor{
public:
	ComboBox* Topics;
	ComboBox* Box;
	Enumerator* E;
	_str SearchMask;
	virtual const char* GetClassID(){return "_index";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);		
	virtual bool Assign(xmlQuote* xml);
	virtual int Get(xmlQuote* xml);
	virtual OneElementEditor* _new(){
		return new _index_editor;
	}
};
class _variant_editor:public _index_editor{
public:
	virtual const char* GetClassID(){return "_variant";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);		
	virtual OneElementEditor* _new(){
		return new _variant_editor;
	}
};
class _ClassIndex_editor:public OneElementEditor{
public:
	ComboBox* Box;
	virtual const char* GetClassID(){return "_ClassIndex";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);		
	virtual OneElementEditor* _new(){
		return new _ClassIndex_editor;
	}
	virtual void  Assign		(BaseClass* BC,void* Ptr,void* ExtraPtr);
	virtual int   Get			(BaseClass* BC,void* Ptr,void* ExtraPtr);
	virtual DWORD GetHashValue	(BaseClass* BC,void* Ptr,void* ExtraPtr);
};
class _strindex_editor:public _index_editor{
public:
	virtual const char* GetClassID(){return "_strindex";}		
	virtual OneElementEditor* _new(){
		return new _strindex_editor;
	}
};
class _byte_index_editor:public OneElementEditor{
public:
	ComboBox* Box;
	virtual const char* GetClassID(){return "_byte_index";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
		virtual bool Assign(xmlQuote* xml){
		for(int i=0;i<Box->NLines;i++)if(!strcmp(Box->Lines[i],xml->Get_string())){
			Box->CurLine=i;
		}
		return true;
		}
		virtual int Get(xmlQuote* xml){
			///if(Box->CurLine==0)xml->Assign_string("");
			if(Box->NLines)xml->Assign_string(Box->Lines[Box->CurLine]);
			return true;
		}
		virtual OneElementEditor* _new(){
			return new _byte_index_editor;
		}
};
class _bool_editor:public OneElementEditor{
public:
	CheckBox* Box;
	virtual const char* GetClassID(){return "_bool";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
			static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo");
			int grp=GetTickCount();
			Box=Base->addGP_CheckBox(NULL,x,y,"",&CED_AFont,&CED_AFont,0,0,GPS.PreLoadGPImage("Interf3\\elements\\CheckBox"),1,0,0);
			Box->y1=y1;
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){
			Box->State=int(!strcmp(xml->Get_string(),"true"));
			return true;
		}
		virtual int Get(xmlQuote* xml){
			if(Box->State)xml->Assign_string("true");
			else xml->Assign_string("false");
			return true;
		}
		virtual OneElementEditor* _new(){
			return new _bool_editor;
		}
};
class _textid_editor:public OneElementEditor{
	InputBox* IB;
	//char* Data;
public:
	char str[2048];	
	virtual const char* GetClassID(){ 
		return "_textid"; 
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
							BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
	virtual bool Assign(xmlQuote* xml){
		if(!IB->Active){
			if(xml->Get_string())strcpy(str,xml->Get_string());
			else str[0]=0;
		}
		return true;
	}
	virtual int Get(xmlQuote* xml){
		xml->Assign_string(str);
		return true;
	}
	/*
	virtual bool Process(){
		strcpy(Data,str);
		return true;
	}
	*/
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _textid_editor;
	}
};
class _str_editor:public _textid_editor{
public:
	virtual const char* GetClassID(){
		return "_str";
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _str_editor;
	}
};
class _lpchar_editor:public _textid_editor{
public:
	virtual const char* GetClassID(){
		return "_lpchar";
	}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _lpchar_editor;
	}
};
class _UneUS_editor:public OneElementEditor{
public:
	char str[256];
	virtual const char* GetClassID(){return "_OneUS";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
			x-=CParam->LeftColumnWidth;
			x+=50;
			InputBox* IB=Base->addInputBox(NULL,x,y,str,120,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
			IB->Enabled=0;
			CParam->RightColumnWidth+=CParam->LeftColumnWidth-50;
			CParam->LeftColumnWidth=50;
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){			
			int x,y,i;
			int z=sscanf(xml->Get_string(),"%d%d%d",&x,&y,&i);
			if(z==3){
				OneObject* OB=Group[i];
				if(OB){
					sprintf(str,"[%d]%s",OB->Index,OB->Ref.General->Message);
				}
			}
			return true;
		}
		virtual int Get(xmlQuote* xml){			
			return true;
		}
		virtual OneElementEditor* _new(){
			return (OneElementEditor*)new _UneUS_editor;
		}
};
struct LS_gp_param{
	ClassEditor* CE;
	const char* ext;
	const char* startdir;
	char* destname;
};
bool LS_gp_UserClick(SimpleDialog* SD);
class _gpfile_editor:public OneElementEditor{
public:
	char str[256];
	virtual const char* GetClassID(){return "_gpfile";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
			InputBox* IB=Base->addInputBox(NULL,x,y,str,120,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
			IB->Enabled=0;
			SimpleDialog* SD=Base->addColoredBar(x,y,x1-x+1,y1-y+1,0xFF);
			SD->Diffuse=0x60FF0000;
			SD->OnUserClick=&LS_gp_UserClick;
			LS_gp_param* p=new LS_gp_param;
			p->destname=str;
			p->ext="*.G16,*.G17,*.G2D,*.GP2";
			p->startdir="Interf3\\";
			p->CE=CParam->CE;
			SD->AllocPtr=(char*)p;
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){
			if(xml->Get_string())strcpy(str,xml->Get_string());
			else str[0]=0;
			return true;
		}
		virtual int Get(xmlQuote* xml){
			xml->Assign_string(str);
			return true;
		}
		virtual OneElementEditor* _new(){
			return (OneElementEditor*)new _gpfile_editor;
		}
};
class _picfile_editor:public OneElementEditor{
	_picfile* Data;
	BitPicture* BP;
	BitPicture* Preview;
public:
	char str[256];
	virtual const char* GetClassID(){ return "_picfile"; }
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
						BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
	virtual bool Process();
	/*
	virtual bool Assign(xmlQuote* xml){
		if(xml->Get_string())strcpy(str,xml->Get_string());
		else str[0]=0;
		return true;
	}
	virtual int Get(xmlQuote* xml){
		xml->Assign_string(str);
		return true;
	}
	*/
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _picfile_editor;
	}
};

class _TextureID_editor:public OneElementEditor{
public:
	char str[256];
	virtual const char* GetClassID(){return "_TextureID";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
			InputBox* IB=Base->addInputBox(NULL,x,y,str,120,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
			IB->Enabled=0;
			SimpleDialog* SD=Base->addViewPort(x,y,x1-x+1,y1-y+1);
			SD->OnUserClick=&LS_gp_UserClick;
			LS_gp_param* p=new LS_gp_param;
			p->destname=str;
			p->ext="*.DDS,.TGA,.BMP,.JPG";
			p->startdir="Textures\\";
			p->CE=CParam->CE;
			SD->AllocPtr=(char*)p;
			return y1;
		}
		virtual bool Assign(xmlQuote* xml){
			if(xml->Get_string())strcpy(str,xml->Get_string());
			else str[0]=0;
			return true;
		}
		virtual int Get(xmlQuote* xml){
			xml->Assign_string(str);
			return true;
		}
		virtual OneElementEditor* _new(){
			return (OneElementEditor*)new _TextureID_editor;
		}
};
class _font_editor:public OneElementEditor{
public:
	ComboBox* Box;
	virtual const char* GetClassID(){return "_font";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
	virtual bool Assign(xmlQuote* xml){
		for(int i=0;i<Box->NLines;i++)if(!strcmp(Box->Lines[i],xml->Get_string())){
			Box->CurLine=i;
		}
		return true;
	}
	virtual int Get(xmlQuote* xml){
		xml->Assign_string(Box->Lines[Box->CurLine]);
		return true;
	}
	virtual OneElementEditor* _new(){
		return new _font_editor;
	}

};
class _gtexture:public _int{
public:
	virtual const char* GetClassName(){return "_gtexture";};
	AUTONEW(_gtexture);
};
class _gfacture:public _int{
public:
	virtual const char* GetClassName(){return "_gfacture";};
	AUTONEW(_gfacture);
};
class _gtexture_editor;
struct _gt_Param{
	DialogsDesk* DD;
	_gtexture_editor* _gt;
	int tindex;
};
bool VisControl(SimpleDialog* SD);
bool te_TextureClick(SimpleDialog* SD);
class _gtexture_editor:public OneElementEditor{
public:
	BaseMeshDialog* BMD;
	DialogsDesk* DD;
	int texidx;
	int prevTexidx;
	int* txptr;
	virtual const char* GetClassID(){return "_gtexture";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		txptr=(int*)DataPtr;
		y1=y+32;
		BMD=Base->addBaseMeshDialog(x,y,64,y1-y+1);
		BMD->CreateSquareTnL(0,0,0.0,0.0,"hud",0xFFFFFFFF);
		BMD->BM->setTexture(IRS->GetTextureID("Textures\\GroundTex.bmp"));
		BMD->OnUserClick=&VisControl;
		int cx=(x+x1)/2;
		int cy=(y+y1)/2;
		int LX=150;
		int LY=130;
		DD=Base->AddDialogsDesk(x,y1-LY,LX,LY,"EmptyBorder");
		DD->Visible=0;
		BMD->UserParam=int(DD);
		for(int i=0;i<64;i++){
			BaseMeshDialog* BSM=DD->addBaseMeshDialog((i%4)*31,(i/4)*31,32,32);
			BSM->CreateSquareTnL(float(i%8)/8.0,float(i/8)/8.0f,float(i%8)/8.0+0.1249,float(i/8)/8.0f+0.1249,"hud",0xFFFFFFFF);
			BSM->BM->setTexture(IRS->GetTextureID("Textures\\GroundTex.bmp"));
			BSM->OnUserClick=&te_TextureClick;
			_gt_Param* _gtp=znew(_gt_Param,1);
            _gtp->_gt=this;
			_gtp->tindex=i;
			_gtp->DD=DD;
			BSM->AllocPtr=(char*)_gtp;
		}
		texidx=0;
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		texidx=0;
		const char* s=xml->Get_string();
		if(s)texidx=atoi(s);
		*txptr=texidx;
		VertexTnL* V=(VertexTnL*)BMD->BM->getVertexData();
		float u0=float(texidx%8)/8.0f;
		float v0=float(texidx/8)/8.0f;
		V[0].u=u0;
		V[0].v=v0;
		V[1].u=u0+0.125f;
		V[1].v=v0;
		V[2].u=u0;
		V[2].v=v0+0.125;
		V[3].u=u0+0.125;
		V[3].v=v0+0.125;
		for(int i=0;i<4;i++)V[i].diffuse=texidx==-1?0x20FFFFFF:0xFFFFFFFF;
		return true;
	}
	virtual int Get(xmlQuote* xml){
		xml->Assign_int(texidx);
		if(!DD->Visible){
			DD->VScroller->SPos=(texidx/4)*31;
			if(DD->VScroller->SPos>DD->VScroller->SMaxPos)DD->VScroller->SPos=DD->VScroller->SMaxPos;
		}
		if(DD->Visible&&!DD->MouseOver){			
			DD->Visible=0;
			ParentFrame::OnlyActiveParent=NULL;
		}
		VertexTnL* V=(VertexTnL*)BMD->BM->getVertexData();
		for(int i=0;i<4;i++)
			V[i].diffuse=texidx==-1?
			((DWORD)(30.0f*(1.0f+cosf(float(GetTickCount()%1000000)/200.0f)))<<24)+0xFF0000:0xFFFFFFFF;
		if(prevTexidx==-1&&texidx!=-1){
			prevTexidx=texidx;
			return 2;
		}
		prevTexidx=texidx;
		return true;
	}
	virtual OneElementEditor* _new(){
		return new _gtexture_editor;
	}
};
class _gfacture_editor:public _gtexture_editor{
public:
	virtual const char* GetClassID(){return "_gfacture";}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		txptr=(int*)DataPtr;
		y1=y+48;
		int LX=150;
		int LY=130;
		//x1=x+LX;
		BMD=Base->addBaseMeshDialog(x,y,x1-x+1,y1-y+1);
		BMD->CreateSquareTnL(0,0,1.0,1.0f*float(y1-y)/float(x1-x),"hud",0xFFFFFFFF);
		BMD->BM->setTexture(IRS->GetTextureID("Textures\\GroundTex.bmp"));
		BMD->OnUserClick=&VisControl;
		int cx=(x+x1)/2;
		int cy=(y+y1)/2;		
		DD=Base->AddDialogsDesk(x,y1-LY,LX,LY,"EmptyBorder");
		DD->Visible=0;
		BMD->UserParam=int(DD);
		int GetFacture(int id);
		int GetNFactures();

		for(int i=0;i<GetNFactures();i++){
			BaseMeshDialog* BSM=DD->addBaseMeshDialog(0,i*LY,LX,LY);
			BSM->CreateSquareTnL(0,0,1,1,"hud",0xFFFFFFFF);
			//char cc[32];
			//sprintf(cc,"Ground\\tex%d.bmp",i+1);
			BSM->BM->setTexture(GetFacture(i));
			BSM->OnUserClick=&te_TextureClick;
			_gt_Param* _gtp=znew(_gt_Param,1);
			_gtp->_gt=(_gtexture_editor*)this;
			_gtp->tindex=i;
			_gtp->DD=DD;
			BSM->AllocPtr=(char*)_gtp;
		}
		texidx=0;
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		texidx=0;
		const char* s=xml->Get_string();
		if(s)texidx=atoi(s);
		*txptr=texidx;
		char cc[32];
		int GetNFactures();
		int GetFacture(int id);
		//sprintf(cc,"Ground\\tex%d.bmp",texidx+1);
		BMD->BM->setTexture(GetFacture(texidx));		
		DD->VScroller->SPos=texidx==-1?0:texidx*130;
		return true;
	}
	virtual int Get(xmlQuote* xml){
		xml->Assign_int(texidx);
		if(DD->Visible&&!DD->MouseOver){			
			DD->Visible=0;
			ParentFrame::OnlyActiveParent=NULL;
		}
		VertexTnL* V=(VertexTnL*)BMD->BM->getVertexData();
		for(int i=0;i<4;i++)V[i].diffuse=texidx==-1?0x20FFFFFF:0xFFFFFFFF;
		return true;
	}
	virtual OneElementEditor* _new(){
		return new _gfacture_editor;
	}
};
class _axe1d:public OneElementEditor{
public:
	float minV;
	float maxV;
	float* V;
	float Ticks[16];
	bool Visible[16];
	bool Long[16];

	int NTicks;
	virtual float GetFunc(float x){
		return x;
	}
	virtual float GetInvFunc(float x){
		return x;
	}
	static bool DrawScale(SimpleDialog* SD){
		_axe1d* axe=(_axe1d*)SD->UserParam;
        float f=*axe->V;		
		int LX=SD->x1-SD->x+1;
		GPS.DrawRect(SD->x+1,SD->y+1,LX,2,0xFF404040);
		for(int i=0;i<axe->NTicks;i++){
            int xx=SD->x+int(LX*axe->GetFunc(axe->Ticks[i]));
			GPS.DrawLine(xx+1,SD->y+1,xx+1,SD->y+6,0xFF404040);
		}
		GPS.DrawRect(SD->x,SD->y,LX,2,0xFFA0A0A0);
		GPS.DrawRect(SD->x+1,SD->y+1,LX-2,1,0xFFB0B0B0);
		for(int i=0;i<axe->NTicks;i++){
			int xx=SD->x+int(LX*axe->GetFunc(axe->Ticks[i]));
			GPS.DrawLine(xx,SD->y,xx,SD->y+6+int(axe->Long[i])*2,0xFFA0A0A0);
		}
		for(int i=0;i<axe->NTicks;i++)if(axe->Visible[i]){
			int xx=SD->x+int(LX*axe->GetFunc(axe->Ticks[i]));
			char txt[32];
			sprintf(txt,"%.1f",axe->Ticks[i]);
			char* ss=strstr(txt,".0");
			if(ss)ss[0]=0;
			if(i==axe->NTicks-1){
				xx-=GetRLCStrWidth(txt,&SmallYellowFont);
			}else if(i>0)xx-=GetRLCStrWidth(txt,&SmallYellowFont)/2;
			ShowString(xx,SD->y+6,txt,&SmallYellowFont);
		}
		int xx=SD->x+int(axe->GetFunc(f)*LX);
		GPS.DrawLine(xx,SD->y,xx,SD->y1,0x6FFFFFFF);
		for(int p=0;p<6;p++){
			DWORD C=0xFF-p*40;
			GPS.DrawLine(xx,SD->y,xx+p,SD->y1,0x6F000000+(C<<16)+(C<<8)+C);
			GPS.DrawLine(xx,SD->y,xx-p,SD->y1,0x6F000000+(C<<16)+(C<<8)+C);
		}
		return true;
	}
	static bool ClickScale(SimpleDialog* SD){
		_axe1d* axe=(_axe1d*)SD->UserParam;
        int dx=mouseX-SD->x;
		if(dx>=0&&dx<SD->x1){
			float iv=float(dx)/float(SD->x1-SD->x+1);
			*axe->V=axe->GetInvFunc(iv);
		}
		return false;
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		SimpleDialog* SD=Base->addViewPort(x,y,x1-x-1,y1-y);
		SD->Enabled=1;
		SD->Visible=1;
		SD->OnDraw=&DrawScale;
		SD->UserParam=int(this);
		SD->OnClick=&ClickScale;
		V=(float*)DataPtr;
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		return true;
	}
	virtual int Get(xmlQuote* xml){		
		char cc[32];
		sprintf(cc,"%f",*V);
		xml->Assign_string(cc);
		return true;
	}
};

//-----------------------------------------------------------------------------
// CtrlISliderEditor
//-----------------------------------------------------------------------------
class CtrlISliderEditor : public OneElementEditor {
public:
	CtrlISliderArgs *m_pArgs;
	float m_CurPos;
	
	static bool OnDraw(SimpleDialog *);
	static bool OnMouseOver(SimpleDialog *);
	virtual int CreateControl(ParentFrame *, int, int, int, int, BaseClass *, void *, void *, ControlParams *);

	virtual const char * GetClassID() { return "CtrlISlider"; }
	virtual OneElementEditor * _new() { return (OneElementEditor *) new CtrlISliderEditor; }

	virtual bool Assign(xmlQuote *);
	virtual int Get(xmlQuote *);

	int GetValue() const;
	void SetValue(int);
};

//-----------------------------------------------------------------------------
// CtrlFSliderEditor
//-----------------------------------------------------------------------------
class CtrlFSliderEditor : public OneElementEditor {
public:
	CtrlFSliderArgs *m_pArgs;
	float m_CurPos;
	
	static bool OnDraw(SimpleDialog *);
	static bool OnMouseOver(SimpleDialog *);
	virtual int CreateControl(ParentFrame *, int, int, int, int, BaseClass *, void *, void *, ControlParams *);

	virtual const char * GetClassID() { return "CtrlFSlider"; }
	virtual OneElementEditor * _new() { return (OneElementEditor *) new CtrlFSliderEditor; }

	virtual bool Assign(xmlQuote *);
	virtual int Get(xmlQuote *);

	float GetValue() const;
	void SetValue(float);
};

class _float01_editor:public _axe1d{
public:
	_float01_editor(){
		NTicks=11;
		for(int i=0;i<11;i++){
			Ticks[i]=float(i)*0.1f;
			Visible[i]=0;
			Long[i]=0;
		}
		Visible[5]=1;
		Long[5]=1;
        minV=0.0f;
		maxV=1.0f;
	}
	virtual const char* GetClassID(){return "_float01";}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _float01_editor;
	}
};
class _float0_10_editor:public _axe1d{
public:
	_float0_10_editor(){
		NTicks=8;
		Ticks[0]=0.0f;
		Ticks[1]=0.5f;
		Ticks[2]=1.0f;
		Ticks[3]=2.0f;
		Ticks[4]=4.0f;
		Ticks[5]=6.0f;
		Ticks[6]=8.0f;
		Ticks[7]=10.0f;
		for(int i=0;i<NTicks;i++){
			Visible[i]=1;
			Long[i]=0;
		}
		minV=0.0f;
		maxV=10.0f;
	}
	float GetInvFunc(float x){
        return x*x*10.0f;
	}
	float GetFunc(float x){
		return sqrt(x/10.0f);
	}
	virtual const char* GetClassID(){return "_float0_10";}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new _float0_10_editor;
	}
};
class _graph2d:public OneElementEditor{
public:
	_graph2d(){
		TextureID=-1;
	}
	virtual void InitBeforeGetValues(){};
	virtual ~_graph2d(){
		if(TextureID!=-1){
			DynTexManager::FreeTexture(TextureID);
		}
	}
	int TextureID;
	BaseMeshDialog* BM;
	int Lx,Ly; 
	int CurHash;
	virtual int GetHash(){return 0;};
	virtual float GetValue(int x,int y){return 0.0f;}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
	virtual int Get(xmlQuote* xml);
};
class _fractal:public BaseClass{
public:
	int   phaseX;
	int   phaseY;
	float Scale;
	float Center;
	float Contrast;
	int   Mapping;
	int   Type;
    ClonesArray<_fractal> SummOfFractals;
	_fractal(){
		Scale    = 1.0f;
		Center   = 0.5f;
		Contrast = 1.0f;
		int mrand();
		phaseX   = mrand();
		phaseY   = mrand();
		addmr(phaseX);
		addmr(phaseY);
		Mapping  = 0;
		Type=0;
	}
	void ShiftXY(int& x,int& y);
	virtual float GetFValue(int x,int y);
	SAVE(_fractal)
	    REG_MEMBER(_float0_10,Scale);
	    REG_MEMBER(_float01,Center);
	    REG_MEMBER(_float0_10,Contrast);
	    //REG_ENUM(_index,Mapping,FMAPPING);
	    REG_ENUM(_index,Type,FRACTYPE);

        SAVE_SECTION(2);

        REG_AUTO(SummOfFractals);
	ENDSAVE
    virtual DWORD GetClassMask(){
        return Type==9?2:1;
    }
};
class _fractalLine:public _fractal{
public:	
	SAVE(_fractalLine)
		REG_PARENT(_fractal);
	ENDSAVE
};
class _fractalLayer:public _fractal{
public:	
	float Constant;
	float CenterX;
	float CenterY;
	float LinearX;
	float LinearY;
	float SquareX;
	float SquareY;	
	_fractalLayer(){
        Contrast=0.5;
		Constant=0;
		LinearX=0;
		LinearY=0;
		SquareX=0;
		SquareY=0;
		CenterX=0;
		CenterY=0;
	}
	SAVE(_fractalLayer)
		REG_BASE(_fractal);
		REG_MEMBER(_float0_10,Scale);
		REG_MEMBER_EX(_float01,Center,Filling);
		REG_MEMBER_EX(_float01,Contrast,EdgesWidth);
		REG_ENUM(_index,Mapping,FMAPPING);
		REG_ENUM(_index,Type,FRACTYPE);
	ENDSAVE
	float GetFValue(int x,int y);
};
class _fractal_editor:public _graph2d{
public:
	_fractal* F;	
	virtual int GetHash(){
        return int(F->Center*1737+F->Contrast*19971+F->Scale*453109+F->Mapping*999+F->Type*199932);
	}
	virtual float GetValue(int x,int y){
		return F->GetFValue(x<<6,y<<6);
	}
	virtual int Get(xmlQuote* xml){
		//ErrorPager EP;
		//F->Save(*xml,F);
		_graph2d::Get(xml);
		return 0;//true;
	}
	virtual bool Assign(xmlQuote* xml){
		return true;
	}
	virtual OneElementEditor* _new(){
		return new _fractal_editor;
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		y1=y+64;
		F=(_fractal*)DataPtr;
		return _graph2d::CreateControl(Base,x,y,x1,y1,Class,DataPtr,ExtraPtr, CParam);
	}
	virtual const char* GetClassID(){return "_fractal";}
    
};
class _fractalLayer_editor:public _fractal_editor{
public:	
	virtual void InitBeforeGetValues();
	virtual float GetValue(int x,int y);	
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
	virtual const char* GetClassID(){return "_fractalLayer";}
	virtual OneElementEditor* _new(){return new _fractalLayer_editor;}
	virtual int GetHash();
};
class _fractalLine_editor:public _fractal_editor{
public:	
	virtual int GetHash();
	virtual float GetValue(int x,int y);
	virtual const char* GetClassID(){return "_fractalLine";}
	virtual OneElementEditor* _new(){return new _fractalLine_editor;}
};
typedef int modFunc(int x0,int y0,int dx,int dy);
_inline DWORD MixDWORD(DWORD C1,DWORD C2,int W1,int W2){
	DWORD R=0;
	if(W1<0)W1=0;
	if(W2<0)W2=0;
	if(W1>255)W1=255;
	if(W2>255)W2=255;
	int DW=(W1+W2)/2;
	for(int i=0;i<4;i++){
		//R<<=8;
		int CH1=C1&255;
		int CH2=C2&255;
		C1>>=8;
		C2>>=8;
		int CC=(CH1*W1+CH2*W2+DW)>>8;
		if(CC>255)CC=255;
        R|=(CC<<(i<<3));
	}
    return R;
}
_inline DWORD ModDWORD(DWORD C1,DWORD C2){
	DWORD R=0;
	for(int i=0;i<4;i++){
		int CH1=C1&255;
		int CH2=C2&255;
		C1>>=8;
		C2>>=8;
		int CC=(CH1*CH2)>>8;
		if(CC>255)CC=255;
		R|=(CC<<(i<<3));
	}
	return R;
}
_inline DWORD MulDWORD(DWORD C1,int Mul){
	DWORD R=0;
	for(int i=0;i<4;i++){
		int CH1=C1&255;
		C1>>=8;
		int CC=(CH1*Mul)>>8;
		if(CC>255)CC=255;
		R|=(CC<<(i<<3));
	}
	return R;
}
class BasePaintMap:public BaseClass{
public:
	int TextureIndex;
	_str Desc;
	BasePaintMap(){
		TextureIndex=-1;
	}
	virtual void  Clear(){
		int tlx,tly;
		GetSize(tlx,tly);
		CreateTexturePart(0,0,tlx,tly);
	}
	virtual void  AddPalColor(DWORD C){};
	virtual void  ClearPal(){};
	virtual void  SetSize(int Lx,int Ly){}
	virtual void  GetSize(int& Lx,int& Ly){}
	virtual void  Free(){}
	virtual DWORD GetPixelColor(int x,int y){return 0;}
	virtual void  SetPixelColor(int x,int y,DWORD Color){}//possibly not supported
	virtual void  SetValue(int x,int y,int Index,int Weight){}
	virtual void  GetValue(int x,int y,int& Index,int& Weight,int& NextIndex){}
	virtual int   GetPaletteSize(){return 0;}
	virtual DWORD GetPaletteColor(int Index){return NULL;}
	virtual BYTE GetGreyScale(int x,int y){
		return 0;
	}
	virtual void SetGreyScale(int x,int y,byte V){};
	virtual float GetGreyPrecise(float x,float y);
	virtual ~BasePaintMap(){
		Free();
		if(TextureIndex!=-1)DynTexManager::FreeTexture(TextureIndex);
		TextureIndex=-1;
	}
	void SetValueInRound(int x,int y,int r,int dr,int Index,modFunc* fn=NULL);
	void PreciseSetValueInRound(float x,float y,float r,float dr);
	void PreciseClearValueInRound(float x,float y,float r,float dr);
	void SetSmoothInRadius(int x,int y,int r,int dr);
	void CreateTexturePart(int x,int y,int Lx,int Ly);
	static char* printbyte(BYTE V){
		static char cc[3]="00";
        char M16[]="0123456789ABCDEF";
		cc[1]=M16[V&15];
		cc[0]=M16[V>>4];
		return cc;
	}
	static void outbyte(BYTE V,char* out){
		char M16[]="0123456789ABCDEF";
		out[1]=M16[V&15];
		out[0]=M16[V>>4];
	}
	static BYTE get16(char v){
		switch(v){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return v-'0';
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				return v-'A'+10;
		}
		return 0;
	}
	static BYTE getbyte(char* s){
        return get16(s[1])+(get16(s[0])<<4);
	}
	virtual void Save(xmlQuote& xml,void* ClassPtr,void* Extra){
        BasePaintMap* BPM=(BasePaintMap*)ClassPtr;
		int lx,ly;//,pp;
		GetSize(lx,ly);
		char* s=new char[lx*ly*2+256];
        outbyte(lx&255,s);
		outbyte((lx>>8)&255,s+2);
		outbyte(ly&255,s+4);
		outbyte((ly>>8)&255,s+6);
		int pos=8;
		int NC=GetPaletteSize();
		outbyte(NC&255,s+pos);
		outbyte((NC>>8)&255,s+pos+2);		
		pos+=4;
		for(int i=0;i<NC;i++){
			DWORD C=GetPaletteColor(i);
			outbyte(C&255,s+pos  );
			outbyte((C>>8)&255,s+pos+2);
			outbyte((C>>16)&255,s+pos+4);
			outbyte((C>>24)&255,s+pos+6);
			pos+=8;
		}

		int sz=lx*ly;
		for(int i=0;i<sz;i++){
			BYTE V=GetGreyScale(i%lx,i/lx);
			outbyte(V,s+(i*2)+pos);
		}
		s[(sz*2)+pos]=0;
		int L=strlen(s);
		xml.Assign_string(s);
	}
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
		int lx=0;
		int ly=0;		
        char* S=(char*)xml.Get_string();
        lx=int(getbyte(S))+int(getbyte(S+2))*256;
		ly=int(getbyte(S+4))+int(getbyte(S+6))*256;
        SetSize(lx,ly);
		int NC=int(getbyte(S+8))+int(getbyte(S+10))*256;
		int pos=12;

        ClearPal();
		for(int i=0;i<NC;i++){
			DWORD D=DWORD(getbyte(S+pos))+(DWORD(getbyte(S+pos+2))<<8)+(DWORD(getbyte(S+pos+4))<<16)+(DWORD(getbyte(S+pos+6))<<24);
			AddPalColor(D);
            pos+=8;
		}
		int L=strlen(S);
		if(L-pos!=lx*ly*2){
            Error->xml_print(xml,"Unable to load BasePaintMap");
			return false;
		}else{
            int sz=lx*ly;
			for(int i=0;i<sz;i++){
				SetGreyScale(i%lx,i/lx,getbyte(S+i*2+pos));
			}
		}
		CreateTexturePart(0,0,lx,ly);
		return true;
	}
	SAVE(BasePaintMap)
	ENDSAVE
};
struct LayerParam{
	DWORD Color;
	bool Visible;
	_str Name;
};

//class ClimatMap:public BasePaintMap{
//public:
//	LinearArray<BYTE ,_BYTE> R;
//	LinearArray<BYTE ,_BYTE> G;
//	LinearArray<BYTE ,_BYTE> B;
//	LinearArray<DWORD,_DWORD> Palette;
//	int Lx,Ly;
//	ClimatMap(){
//		Palette.Add(0xFFFFE020);//desert
//		Palette.Add(0xFF90C020);//savanna
//		Palette.Add(0xFF80FF00);//tropics
//		Palette.Add(0xFF00FF00);//green grass
//		Palette.Add(0xFF40A040);//northen grass
//		Palette.Add(0xFFD0E0E0);//northen pole
//		SetSize(256,256);
//	}
//	virtual void  Clear(){
//		if(Lx&&Ly){
//			memset(&R[0],Palette[0],R.GetAmount());
//			memset(&G[0],Palette[0],G.GetAmount());
//			memset(&B[0],Palette[0],B.GetAmount());
//		}
//		BasePaintMap::Clear();
//	}
//	virtual void  SetSize(int _Lx,int _Ly){
//		Lx=_Lx;
//		Ly=_Ly;
//		R.Add(0,Lx*Ly);
//		G.Add(0,Lx*Ly);
//		B.Add(0,Lx*Ly);
//		Clear();
//	}
//	virtual void  GetSize(int& _Lx,int& _Ly){
//        _Lx=Lx;
//		_Ly=Ly;
//	}
//	virtual void  Free(){
//		R.Clear();
//		G.Clear();
//		B.Clear();
//	}
//	virtual DWORD GetPixelColor(int x,int y){
//		if(x<0)x=0;
//		if(x>=Lx)x=Lx-1;
//		if(y<0)y=0;
//		if(y>=Ly)y=Ly-1;
//		int ofs=x+y*Lx;
//		return 0xFF000000+(R[ofs]<<16)+(G[ofs]<<8)+B[ofs];
//	}
//	virtual void  SetValue(int x,int y,int Index,int Weight){
//		if(x>=0&&x<Lx&&y>=0&&y<Ly){
//			if(Weight<0)Weight=0;
//			if(Weight>255)Weight=255;
//			int ofs=x+y*Lx;
//			DWORD C0=Palette[Index];
//			int r0=(C0>>16)&0xFF;
//			int g0=(C0>>8)&0xFF;
//			int b0=C0&0xFF;
//			int r1=R[ofs];
//			int g1=G[ofs];
//			int b1=B[ofs];
//			R[ofs]=(r0*Weight+r1*(255-Weight))/255;
//			G[ofs]=(g0*Weight+g1*(255-Weight))/255;
//			B[ofs]=(b0*Weight+b1*(255-Weight))/255;
//		}
//	}
//	virtual void  GetValue(int x,int y,int& Index,int& Weight,int& NextIndex){
//		if(x<0)x=0;
//		if(y<0)y=0;
//		if(x>=Lx)x=Lx-1;
//		if(y>=Ly)y=Ly-1;
//		int ofs=x+y*Lx;
//		int r=R[ofs];
//		int g=G[ofs];
//		int b=B[ofs];
//		int N=Palette.GetAmount();
//		int best=0;
//		int next=0;
//		int rb=10000;
//		int rn=10000;
//		for(int i=0;i<N;i++){
//            int r=abs(r-((Palette[i]>>16)&255))+abs(g-((Palette[i]>>8)&255))+abs(b-(Palette[i]&255));
//			if(r<rb){
//				rn=rb;
//				next=best;
//                best=i;
//				rb=r;
//			}else{
//				if(r<rn){
//					next=i;
//					rn=r;
//				}
//			}
//		}
//		Index=best;
//		NextIndex=next;
//		if(rn){
//            Weight=rn*255/(rn+rb);
//		}else{
//            Weight=255;
//		}
//	}
//	virtual int   GetPaletteSize(){
//		return Palette.GetAmount();
//	}
//	virtual DWORD GetPaletteColor(int Index){
//		return Palette[Index];
//	}
//};
class GreyMap:public BasePaintMap{
public:
	LinearArray<BYTE ,_BYTE> V;
	DWORD Color;
	int Lx,Ly;
	virtual void  AddPalColor(DWORD C){
		if(C)Color=C;
	};
	virtual void  ClearPal(){
	};
	GreyMap(){
		Color=0xFFFFFFFF;
		SetSize(512,512);
	}
	GreyMap(DWORD C){
		Color=C;
		SetSize(512,512);
	}
	virtual void  Clear(){
		if(Lx&&Ly){
			memset(&V[0],0,V.GetAmount());
		}
		BasePaintMap::Clear();
	}
	virtual void  SetSize(int _Lx,int _Ly){
		Lx=_Lx;
		Ly=_Ly;
		V.Add(0,Lx*Ly/8);
		Clear();
	}
	virtual void  GetSize(int& _Lx,int& _Ly){
		_Lx=Lx;
		_Ly=Ly;
	}
	virtual void  Free(){
		V.Clear();
	}
	virtual DWORD GetPixelColor(int x,int y){
		return GetGreyScale(x,y)?Color:0;
	}
	virtual void  SetValue(int x,int y,int Index,int Weight);
	virtual void  GetValue(int x,int y,int& Index,int& Weight,int& NextIndex);
	virtual BYTE GetGreyScale(int x,int y);
	virtual void SetGreyScale(int x,int y,byte v);
	virtual int   GetPaletteSize(){
		return 2;
	}
	virtual DWORD GetPaletteColor(int Index){
		return Index?Color:0;
	}
	SAVE(GreyMap);
	ENDSAVE;
};

class GreyMap8bit:public BasePaintMap{
public:
	LinearArray<BYTE ,_BYTE> V;
	DWORD Color;
	int Lx,Ly;
	virtual void  AddPalColor(DWORD C){
		if(C)Color=C;
	};
	virtual void  ClearPal(){
	};
	int GreyMap(){
		Color=0xFFFFFFFF;
		SetSize(512,512);
	}
	int GreyMap(DWORD C){
		Color=C;
		SetSize(512,512);
	}
	virtual void  Clear(){
		if(Lx&&Ly){
			memset(&V[0],0,V.GetAmount());
		}
		BasePaintMap::Clear();
	}
	virtual void  SetSize(int _Lx,int _Ly){
		Lx=_Lx;
		Ly=_Ly;
		V.Add(0,Lx*Ly);
		Clear();
	}
	virtual void  GetSize(int& _Lx,int& _Ly){
		_Lx=Lx;
		_Ly=Ly;
	}
	virtual void  Free(){
		V.Clear();
	}
	virtual DWORD GetPixelColor(int x,int y){
		if(x<0)x=0;
		if(x>=Lx)x=Lx-1;
		if(y<0)y=0;
		if(y>=Ly)y=Ly-1;
		int ofs=x+y*Lx;
		int c=V[ofs];
		return MixDWORD(Color,0,c,0);
	}
	virtual void  SetValue(int x,int y,int Index,int Weight){
		if(x>=0&&x<Lx&&y>=0&&y<Ly){
			if(Weight<0)Weight=0;
			if(Weight>255)Weight=255;
			int ofs=x+y*Lx;
			if(Index)Index=255;
			int V0=Index;
			int V1=V[ofs];			
			V[ofs]=(V0*Weight+V1*(255-Weight))/255;
		}
	}
	virtual void  GetValue(int x,int y,int& Index,int& Weight,int& NextIndex){
		if(x<0)x=0;
		if(y<0)y=0;
		if(x>=Lx)x=Lx-1;
		if(y>=Ly)y=Ly-1;
		int ofs=x+y*Lx;
		Index=1;
		NextIndex=0;
		Weight=V[ofs];		
	}
	virtual BYTE GetGreyScale(int x,int y){
		if(x<0)x=0;
		if(y<0)y=0;
		if(x>=Lx)x=Lx-1;
		if(y>=Ly)y=Ly-1;
		int ofs=x+y*Lx;
		return V[ofs];		
	}
	virtual void SetGreyScale(int x,int y,byte v){
		if(x>=0&&x<Lx&&y>=0&&y<Ly){
			V[x+y*Lx]=v;
		}
	}
	virtual int   GetPaletteSize(){
		return 2;
	}
	virtual DWORD GetPaletteColor(int Index){
		return Index?Color:0;
	}
	SAVE(GreyMap8bit);
	ENDSAVE;
};
class MapsScope:public BaseClass{
public:
	MapsScope(){
		
	}
	~MapsScope(){
		xmlQuote xml;
		Save(xml,this);
		//xml.WriteToFile("MapLayers.xml");
	}
	void Clear(){
		//for(int i=0;i<Maps.GetAmount();i++)delete(Maps[i]);
		//for(int i=0;i<MapsNames.GetAmount();i++)delete(MapsNames[i]);
		Maps.Clear();
		MapsNames.Clear();
	}
	void SetDefaults(){	
		Enumerator* E1=ENUM.Get("LAYERSLIST");
		E1->Clear();

		Maps.Add(new GreyMap(0xFF909020));
		_str* str=new _str;
		str->Assign("Climat1");
		MapsNames.Add(str);
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFF509020));
		str=new _str;
		str->Assign("Climat2");
		MapsNames.Add(str);
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFF209020));
		str=new _str;
		str->Assign("Climat3");
		MapsNames.Add(str);
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFF909090));
		str=new _str;
		str->Assign("Climat4");
		MapsNames.Add(str);
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFF0000FF));
		str=new _str;
		str->Assign("Water");
		MapsNames.Add(str);
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFF50FF40));
		str=new _str;
		str->Assign("Trees");
		MapsNames.Add(str);
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFFFF0000));
		str=new _str;
		str->Assign("Smooth");
		MapsNames.Add(str);
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFFFFFF00));
		str=new _str;
		str->Assign("Zone1");
		MapsNames.Add(str);
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFF00FFFF));
		str=new _str;
		str->Assign("Zone2");
		MapsNames.Add(str);
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFFFF00FF));
		str=new _str;
		str->Assign("Zone3");
		MapsNames.Add(str);			
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFFFFFFFF));
		str=new _str;
		str->Assign("Zone4");
		MapsNames.Add(str);			
		E1->Add(str->pchar());

		Maps.Add(new GreyMap(0xFFFF8000));
		str=new _str;
		str->Assign("Zone5");
		MapsNames.Add(str);					
		E1->Add(str->pchar());
		void TestLayersColors();
		TestLayersColors();
	}
	ClassArray<BasePaintMap> Maps;
	//DynArray<int> TexIndices;
	ClassArray<_str> MapsNames;
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		BaseClass::Load(xml,ClassPtr,Error,Extra);
		Enumerator* E=ENUM.Get("LAYERSLIST");
		E->Clear();
		for(int i=0;i<MapsNames.GetAmount();i++){
			E->Add(MapsNames[i]->pchar());
			//int ti=DynTexManager::GetTexture();
			//TexIndices.Add(ti);
		}
		return true;
	}
	SAVE(MapsScope)
	REG_AUTO(Maps);
	REG_AUTO(MapsNames);
	ENDSAVE
};
extern MapsScope* MLayers;
class MapRef:public BaseClass{
public:
	int   TopLayerIndex;
	float PenRadius;
	float PenWidth;
	int   PenColor;
	int   PenRColor;
	int   ActionType;
	MapRef(){
		PenColor=1;
		PenRColor=0;
		PenWidth=0.5f;
		PenRadius=0.5f;
		ActionType=0;
		TopLayerIndex=0;
	}
	SAVE(MapRef)
	REG_MEMBER(_float01,PenRadius);
	REG_MEMBER(_float01,PenWidth);
	REG_ENUM(_index,ActionType,DRAWACTION);
	REG_ENUM_EX(_index,TopLayerIndex,LAYERSLIST,CurrentLayer);
	ENDSAVE
};
#define pYsize 20
class MapRef_editor:public OneElementEditor{
public:
	static float GlobalShiftX;
	static float GlobalShiftY;
	static float GlobalScale;
	DynArray<BaseMeshDialog*> Meshes;
	DynArray<int> MIndex;	
	int cTopIndex;
	MapRef* MRef;
	SimpleDialog* Palet;
	int CurLColor;
	int CurRColor;
	bool noedit;
	static bool _DrawPalette(SimpleDialog* SD);
	virtual const char* GetClassID(){return "MapRef";}
	void CheckTopLayer();
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
		virtual bool Assign(xmlQuote* xml){return true;}
		virtual int Get(xmlQuote* xml);
		virtual OneElementEditor* _new(){
			return new MapRef_editor;
		}
};
//------------------! ostorogno gluki !----------------------------//
class FileElement:public _str 
{
public:
	FileElement(){;}
	DString* OnClickSet;
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);
	FileElement(char* filename, char* way, DString* DS);
	DString Way;
	word Type;
	virtual const char* GetClassName();
};
class DirectoryElement:public FileElement
{
public:
	DynArray<FileElement*> FileArr;
	bool Loaded;
	DString Way;
	DString Mask;

	DString Root;

	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);
	int GetAmountOfElements();
	const char* GetElementID(int Index);
	BaseClass* GetElementClass(int Index);
	void* GetElementPtr(int Index,void* base);
	virtual bool CheckDirectCasting();
	AUTONEW(DirectoryElement);
	DirectoryElement();
	DirectoryElement( char* DirName, char* Mas , char* way, DString* DS);
	virtual ~DirectoryElement();
	void ReadDir();
	static int compare(const void* p1, const void* p2);	
	void ReadFile(WIN32_FIND_DATA* FD);
	void ReadDir(WIN32_FIND_DATA* FD);
};

class FilePathEditor:public DirectoryElement
{
public:
	FilePathEditor();
	FilePathEditor(char* RootDir, char* Maska);
	virtual const char* GetClassName();
};
class FilePathEditor_editor:public OneElementEditor{
public:
	//char str[256];
	_str* str;
	InputBox* IB;
	virtual const char* GetClassID();
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam);
	virtual bool Assign(xmlQuote* xml);
	virtual int Get(xmlQuote* xml);
	virtual OneElementEditor* _new();
	//virtual DWORD GetHashValue(BaseClass*,void*,void*){return 0;};
};
class testFilePathEditor:public BaseClass
{
public:
	testFilePathEditor();
	testFilePathEditor(const char* path,const char* mask);
	int eee;
	FilePathEditor FILEPATH;
	void CheckName();
	SAVE(testFilePathEditor)
	REG_AUTO_EX2(FILEPATH,">");
	ENDSAVE
};
class SimpleDialog_editor:public OneElementEditor{
public:
	SimpleDialog* SD;
	CheckBox* CB;
	virtual bool CheckClass(BaseClass* BC);
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,
						void* DataPtr,void* ExtraPtr, ControlParams* CParam);
	virtual bool Process();
	DWORD GetHashValue(BaseClass* BC,void* Ptr,void* ExtraPtr){
		return 0;
	}
	void Assign(BaseClass* BC,void* Ptr,void* ExtraPtr){};
	virtual OneElementEditor* _new();
};
//----------------------------------------------------------------//
extern DynArray<OneElementEditor*> type_list;
class ClassEditor;
struct ElementParam{
    int YPos;
	OneElementEditor* OEE;
	ColoredBar* CBar;
	BaseClass* BC;
	void* DataPtr;
	void* ExtraPtr;
	int Index;
	bool NowActive;
	ClassEditor* Owner;
	_str FieldName;
	BaseClass* BaseParent;
	int BaseParentIndex;	
	DWORD BackColor;
	byte BaseClassType;
	DWORD XMLHash;
	DWORD ParentClassMask;
};

typedef DynArray<void*> ExpandInfo;

class ClassEditor{
public:
	VScrollBar* MainScroller;
	Canvas* MainCanvas;
	//source params
	BaseClass* _EdClass;
	int _x,_y,_x1,_y1;
	DWORD _Opt;
	static ExpandInfo* CurExpInf;
	static bool CheckIfOpen(BaseClass* BC);
	//-------------
	xmlQuote CurClip;
	bool CheckClipStatus(int ElIndex);
	ExpandInfo ExpInf;
	void AddExpInf(void* ptr){
		if(ExpInf.find(ptr)==-1){
			ExpInf.Add(ptr);
		}
	}
	void AddExpInf(BaseClass* BC){
		while(BC){
			AddExpInf((void*)BC);
			BC=BC->GetParent();
		}
	}
	void DelExpInf(void* ptr){
		int p=ExpInf.find(ptr);
		if(p!=-1){
			ExpInf.Del(p,1);
			}
	}
	bool HaveExpInf(void* ptr){
		return ExpInf.find(ptr)!=-1;
	}
	bool ExpInfChanged;
	int CurElement;
	//DialogsSystem Editor;
	ParentFrame* Frame;
	const char* Border;
	DialogsDesk* Base;
	DynArray<OneElementEditor*> ElementEditors;
	DynArray<ElementParam*> ElParams;
	DWORD Opt;
	void ClearAll();
	void CreateFromClass(int x,int y,int x1,int y1,BaseClass* BC,DWORD Opt,const char* Border=CED_Border);
	void CreateFromClass(ParentFrame* Dest,int x,int y,int x1,int y1,BaseClass* BC,DWORD Opt,const char* Border=CED_Border);
	int CreateElement(ParentFrame* Base,int Level,DWORD Mask,int x,int y0,int l_lx,int r_lx,
		BaseClass* BC,int Index,BaseClass* ParentBC,const char* ElName);
	void* GetCurrentSelectedPtr();
	BaseClass* GetCurrentClass();
	const char* GetCurrentFieldName();
	void Process();
	void ClearActive();
	void setScrollerOnSelectedElement();
	ClassEditor();
	~ClassEditor();
};
class ComplexClassEditor{
public:
	ClassEditor   Top;
	ClassEditor   Bottom;
	DialogsSystem TopDS;
	DialogsSystem BottomDS;
	~ComplexClassEditor(){
        Top.Base=NULL;
		Bottom.Base=NULL;
	}
	_str border;
	void Create(int x,int y,int x1,int y1,int DivFacor,BaseClass* BC,const char* Border="StandartBorder"){
        border=Border;
		Top.CreateFromClass(&TopDS,x,y,x1,y+(y1-y)*DivFacor/100-2,BC,2,Border);
		Bottom.CreateFromClass(&BottomDS,x,y+(y1-y)*DivFacor/100+2,x1,y1,BC,1,Border);
    }
	void Process();
};
#pragma pack( pop )
#endif //__CLASSEDITOR_H__