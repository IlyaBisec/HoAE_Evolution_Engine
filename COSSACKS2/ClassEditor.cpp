#include "stdheader.h"
#include "ClassEditor.h"
#include "FilePreview.h"
#include "AI_Scripts.h"
#include "SuperEditor.h"
#include "WeaponSystem.h"
#include "VisualSoundSystem.h"
#include "UF_NumericalReturner.h"
#include "TestEditor.h"
#include "FileDialog.h"
#include "ClassPresentation.h"
#include "Surface\SurfaceGenerator.h"

void KeyTestMem();
void TestDynMem();
void RegizterActiveZoneEditors();
void RegizterVictoryConditionEditors();
//
struct LS_Param{
	BaseClass* BC;
	void* DataPtr;
	void* ExtraPtr;
	ClassEditor* CE;
	const char* StartDir;
	const char* Mask;
};
DynArray<OneElementEditor*> type_list;
bool DrawNewButton(SimpleDialog*  SD){
	DWORD CU=MixDWORD (SD->Diffuse,0xFFD0D0D0,150,55);
	DWORD CUR=MixDWORD(SD->Diffuse,0xFFD0F0D0,150,65);
	DWORD CD=MixDWORD (SD->Diffuse,0xFF101010,150,55);
	DWORD CDR=MixDWORD(SD->Diffuse,0xFF105010,150,65);
	int Lx=SD->x1-SD->x+1;
	int Ly=SD->y1-SD->y+1;
	GPS.DrawFillRect(SD->x,SD->y,2,Ly,CU,CU,CD,CD);
	GPS.DrawFillRect(SD->x+Lx-3,SD->y,2,Ly,CUR,CUR,CDR,CDR);
	GPS.DrawFillRect(SD->x+2,SD->y,Lx-4,2,CU,CUR,CU,CUR);
	GPS.DrawFillRect(SD->x+2,SD->y+Ly-2,Lx-4,2,CD,CDR,CD,CDR);
	GPS.DrawFillRect(SD->x+2,SD->y+2,Lx-4,Ly-4,CD,CDR,CU,CUR);
	GPS.FlushBatches();
	return true;
}
#define setstyle(x) x->OnDraw=&DrawNewButton;
void InitTypeList(){
	REG_CLASS(CtrlISlider);
	REG_CLASS(CtrlFSlider);
	REG_CLASS(_sprite);
	REG_CLASS(AI_Trigger);
	REG_CLASS(_UnitType);
	//conditions
	REG_CLASS(AI_Condition);
	
	REG_CLASS_EX(Difficulty,"Difficulty");

	REG_CLASS_EX(AmountOfReadyUnits,"Units");
	REG_CLASS_EX(UnitsProduced,"Units");
	REG_CLASS_EX(UnitsLost,"Units");

	REG_CLASS_EX(UpgradeIsRun,"Upgrades");	
	
	REG_CLASS_EX(CurResources,"Resources");
	REG_CLASS_EX(TotalResource,"Resources");
	REG_CLASS_EX(ai_cResourcePeasants,"Resources");

	REG_CLASS_EX(AmountOfSettlements,"Settlements");
	REG_CLASS_EX(AmountOfEnemySettlements,"Settlements");	

	REG_CLASS_EX(FreeLivePlaces,"LivePlaces");
	REG_CLASS_EX(TotalLivePlaces,"LivePlaces");
	REG_CLASS_EX(LivePlacesUsed,"LivePlaces");

	REG_CLASS_EX(GamingTime,"Timing");
	REG_CLASS_EX(OnceInPeriod,"Timing");	

	REG_CLASS_EX(EnemyNearBase,"EnemySpying");

	//actions
	REG_CLASS(AI_Action);
	
	REG_CLASS_EX(PerformUpgrade,"Upgrades");
	REG_CLASS_EX(ai_aEnableIndividualUpgrade,"Upgrades");
	REG_CLASS_EX(ai_aDisableIndividualUpgrade,"Upgrades");

	REG_CLASS(aia_UnitType);
	REG_CLASS(aia_SplashBonus);
	REG_CLASS(aia_UnitTypeLinks);
	REG_CLASS(Build);	
	REG_CLASS(Produce);
	REG_CLASS(AddResources);
	REG_CLASS(GoDiversion);	
	//<test by Flame Master>
	//REG_CLASS(EnterBuilding);	
	//</test>
	REG_CLASS(PeonsDistribution);
	REG_CLASS(SettlementsPriory);
	REG_CLASS(UpgradeSettlements);
	
	REG_CLASS(BridgeConstruction);
	REG_CLASS(AI_Act_DeleteNotBusy);
	REG_CLASS(AutoUpRomeHero);
	REG_CLASS(BuyResource);	

	//other
	REG_CLASS(AI_Folder);
	REG_CLASS(SprGroup);
	REG_CLASS(ObjCharacter);
	REG_CLASS(OneSprSection);
	REG_CLASS(LockInfoPoint);
	REG_CLASS(BornRef);
	REG_CLASS(ObjCharRef);
	REG_CLASS(OneConnector);
	extern void RegisterSuperEditorClasses();
	RegisterSuperEditorClasses();
	void RegisterTextVisualElements();
	RegisterTextVisualElements();
	void RegGeneratorClass();
	RegGeneratorClass();
    type_list.Add(new _int_editor);
	type_list.Add(new _short_editor);
	type_list.Add(new _BYTE_editor);
	type_list.Add(new _float_editor);
	type_list.Add(new _float01_editor);
	type_list.Add(new _float0_10_editor);
	type_list.Add(new _index_editor);
	type_list.Add(new _byte_index_editor);
	type_list.Add(new _bool_editor);
	type_list.Add(new _textid_editor);
	type_list.Add(new _str_editor);
	type_list.Add(new _lpchar_editor);
	type_list.Add(new _font_editor);
	type_list.Add(new _DWORD_editor);
	type_list.Add(new _gtexture_editor);
	type_list.Add(new _gfacture_editor);
	type_list.Add(new _fractal_editor);
	type_list.Add(new _fractalLine_editor);
	type_list.Add(new _fractalLayer_editor);
	type_list.Add(new MapRef_editor);
	type_list.Add(new _test_editor);
	type_list.Add(new _UneUS_editor);
	//
	type_list.Add(new _gpfile_editor);	
	type_list.Add(new _picfile_editor);
	//
	type_list.Add(new FilePathEditor_editor);
	type_list.Add(new _UnitType_editor);
	type_list.Add(new AI_Condition_editor);
	type_list.Add(new AI_ConditionsArray_editor);
	type_list.Add(new AI_Action_editor);
	type_list.Add(new _UnitTypeShortcut_editor);	
	type_list.Add(new UnitDefinition_editor);
	type_list.Add(new AI_Trigger_editor);
	type_list.Add(new _WaveFileName_editor);
	type_list.Add(new _ModelID_editor);
	type_list.Add(new _TextureID_editor);
	type_list.Add(new ClassRef_editor);
	type_list.Add(new UserFriendlyNumericalReturner_editor);
	type_list.Add(new _strindex_editor);
	type_list.Add(new _grule_editor); //new
	type_list.Add(new _color_editor);//new
	type_list.Add(new _button_editor);//new
	type_list.Add(new _function_editor);
	type_list.Add(new Delimiter_editor);
	type_list.Add(new _sprite_editor);
	type_list.Add(new _ObjCharacter_editor);
	type_list.Add(new SimpleDialog_editor);
	type_list.Add(new _variant_editor);
	type_list.Add(new _ClassIndex_editor);
	type_list.Add(new CtrlISliderEditor);
	type_list.Add(new CtrlFSliderEditor);
	void AddTimArrayEditor();
	AddTimArrayEditor();
	// vital
	RegizterActiveZoneEditors();
	RegizterVictoryConditionEditors();

	REG_CLASS(NatColorSettings);

}
bool CBR_OnUserClick(SimpleDialog* SD){	
	ElementParam* EP=(ElementParam*)SD->UserParam;
	if(!EP->BC)return false;

	int ce=EP->Owner->CurElement;	
	EP->Owner->ClearActive();
	EP->NowActive=1;
	EP->Owner->CurElement=EP->Index;
	if(EP->BC->CheckDirectCasting()){
		bool ei=EP->Owner->HaveExpInf(EP->DataPtr);
		if(!(GetKeyState(VK_CONTROL)&0x8000)){
			BaseClass* B=(BaseClass*)EP->DataPtr;
			while(B){
				if(B==EP->Owner->_EdClass){
					EP->Owner->ExpInf.Clear();
					break;
				}
				B=B->GetParent();
			}			
		}
		EP->Owner->AddExpInf((BaseClass*)EP->DataPtr);
		EP->Owner->ExpInfChanged=1;
		if(ce==EP->Index){
			if(ei)EP->Owner->DelExpInf(EP->DataPtr);
		}
	}
	EP->CBar->Diffuse=CED_SelFieldColor;	
	if(EP->BaseClassType==100)
	{
		BaseClass* FF=(BaseClass*)EP->BC;
		if(FF)
		{
			xmlQuote Q;
			FF->Load(Q,NULL,NULL);
			if(Q.Get_string())EP->OEE->Assign(&Q);
			EP->XMLHash=EP->OEE->GetHashValue(EP->BC,EP->DataPtr,EP->ExtraPtr);
		}
	}
	return true;
}
void ScaleControl(SimpleDialog* SD){
	SD->EnableTransform=true;
	SD->ScaleX=1.0f;
	SD->ScaleY=bt_yscale;
}
void ClassEditor::CreateFromClass(int x,int y,int x1,int y1,BaseClass* BC,DWORD Opt,const char* Border){
	static DialogsSystem DSS;
	DSS.CloseDialogs();
	CreateFromClass(&DSS,x,y,x1,y1,BC,Opt,Border);
}
void ClassEditor::CreateFromClass(ParentFrame* Dest,int x,int y,int x1,int y1,BaseClass* BC,DWORD Opt1,const char* Border){
	Opt=Opt1;
	if(Opt&2)Opt|=4;
	ClassEditor::Border=Border;
	Frame=Dest;
	_x=x;
	_y=y;
	_x1=x1;
	_y1=y1;
	_EdClass=BC;
	_Opt=Opt;	
	DialogsDesk* DD;	
	if(Base){
		DD=Base;
		DD->CloseDialogs();
		Base=NULL;
	}else{
		ColoredBar* CB=Dest->addColoredBar(x,y,x1-x+1,y1-y+1,0xFF);
		CB->Diffuse=CED_BackColor;
		Dest->DSS.Clear();
		DD=Dest->AddDialogsDesk(x,y,x1-x+1,y1-y+1,Border);
		Base=DD;
		MainScroller=DD->VScroller;		
	}	
	MainCanvas=DD->AddCanvas(0,0,x1-x+1,16);
	int l,r,u,d;
	DD->GetMargin(l,u,r,d);
	x+=l;y+=u;x1-=r;y1-=d;
	int N=BC->GetAmountOfEditableElements(BC,Opt);
	int tlx=x1-x+1;
	int l_lx=tlx/2;
	if(tlx<400)l_lx=4*tlx/10;
	if(l_lx>300)l_lx=300;
	int r_lx=tlx-l_lx-2;
	CurElement=-1;
	int y0=0;

	char cc[64];
	sprintf(cc,"LSENUM_%s",BC->GetClassName());
	Enumerator* E=ENUM.Get(cc);
	static int bidx=GPS.PreLoadGPImage("Interf3\\btnz2");
	if(E&&E->GetAmount()>=2){//load,save		
		LS_Param* LS=new LS_Param;
		LS_Param* LL=new LS_Param;
		LS->BC=BC;
		LS->DataPtr=BC;
		LS->ExtraPtr=NULL;
		LS->CE=this;
		LS->Mask=E->GetStr(1);
		LS->StartDir=E->GetStr(0);
		*LL=*LS;
		int LX=16;
		GP_Button* BS=Base->addGP_Button(NULL,x,0,bidx,3,2);
		//ScaleControl(BS);
		BS->ShapeFileID=bidx;
		BS->ShapeSpriteID=3;
		bool CE_SaveClick(SimpleDialog* SD);
		BS->OnUserClick=&CE_SaveClick;
		BS->AllocPtr=(char*)LS;
		GP_Button* BL=Base->addGP_Button(NULL,x+16,0,bidx,1,0);
		//ScaleControl(BL);
		BL->ShapeFileID=bidx;
		BL->ShapeSpriteID=1;
		ColoredBar* CB=Base->addColoredBar(x+16*2,0,x1-x-16*2,16,0xFF);
		Base->addTextButton(NULL,x+26*2+6,CED_TextDy,(char*)BC->GetClassName(),&CED_AFont,&CED_AFont,&CED_AFont,0);
		CB->Diffuse=0xFF1020F0;
		setstyle(CB);
		bool CE_LoadClick(SimpleDialog* SD);
		BL->OnUserClick=&CE_LoadClick;
		BL->AllocPtr=(char*)LL;
		y0+=22;
	}
	
	int SectState=0;
	for(int i=0;i<N;i++){				
		BaseClass* EBC  =BC->GetEditableElementClass(i,BC,Opt);
		void* EPT       =BC->GetEditableElementData (i,BC,Opt);
		int s=0;
		if(EBC&&EBC->CheckDirectCasting()){
			EBC=(BaseClass*)EPT;
			s=EBC->GetSectionStatus();
		}						
		if(SectState!=1||s!=0)y0=CreateElement(DD,0,0,x,y0,l_lx,r_lx,BC,i,NULL,NULL);
		if(s)SectState=s;
	}        
	int idx=DD->DSS.find(MainCanvas);
	if(idx!=-1){
		DD->DSS.Del(idx,1);
		DD->DSS.Add(MainCanvas);
	}
	if(ExpInf.find(BC)==-1){
		ExpInf.Add(BC);
		ExpInfChanged=1;
	}
}
struct exp_info{
	void* elem;
    ClassEditor* CE;
};

bool EXP_OnUserClick(SimpleDialog* SD){
	exp_info* inf=(exp_info*)SD->AllocPtr;
	if(inf){
		int pos=inf->CE->ExpInf.find(inf->elem);
		if(pos==-1){
			inf->CE->ExpInf.Add(inf->elem);
		}else inf->CE->ExpInf.Del(pos,1);
		inf->CE->ExpInfChanged=1;
		Lpressed=false;
	}
	Lpressed=false;
	return false;
}
bool ExpandLinearArray(SimpleDialog* SD){
	ElementParam* ELP=(ElementParam*)SD->UserParam;
	if(ELP){
		((BaseClass*)ELP->DataPtr)->ExpandWith("",ELP->DataPtr);
		ELP->Owner->ExpInfChanged=1;
	}
	Lpressed=false;
    return true;
}

class ClassArray_AddElmParam{
public:
	ClassArray_AddElmParam(){
		ClassToExpand=NULL;
	}
	ClassExpParams EXP;
	BaseClass* ClassToExpand;
	DynArray<const char*> Classes;
	DynArray<const char*> Topics;
	ComboBox* TopicsCB;
	ComboBox* Type;
	BaseClass* Elem;
	ClassEditor* CE;	
};
bool ReadStdElement(BaseClass* Element);
bool ClassArray_AddElement(SimpleDialog* SD){
    ClassArray_AddElmParam* CAAP=(ClassArray_AddElmParam*)SD->AllocPtr;	
	if(CAAP->Type->NLines){
		int idx=CAAP->Elem->ExpandWith(CAAP->Classes[CAAP->Type->CurLine],CAAP->Elem);
		if(idx!=-1){
            BaseClass* BC=CAAP->Elem->GetElementClass(idx);
			void*     DAT=CAAP->Elem->GetElementPtr(idx,CAAP->Elem);
			if(BC->CheckDirectCasting()){
				ReadStdElement((BaseClass*)DAT);
			}
		}
		CAAP->CE->ExpInfChanged=1;
	}
	return true;
}
void SetGlobalComboState(const char* ClassName,int Topic,int ClassIdx){
	Enumerator* E=ENUM.Get("COMBOPOSITIONS");
	E->Add((char*)ClassName,ClassIdx+(Topic<<16));
}
void GetGlobalComboState(const char* ClassName,int& Topic,int& ClassIdx){
	Topic=0;
	ClassIdx=0;
	Enumerator* E=ENUM.Get("COMBOPOSITIONS");
	DWORD V=E->Get((char*)ClassName);
	if(V!=0xFFFFFFFF){
        ClassIdx=V&0xFFFF;
		Topic=V>>16;
	}
}
bool ClassArray_OnComboTopicSelect(SimpleDialog* SD){
	ClassArray_AddElmParam* CAAP=(ClassArray_AddElmParam*)SD->UserParam;	
	if(CAAP->TopicsCB){
		CAAP->Type->Clear();
		CAAP->Classes.Clear();
        const char* Topic=CAAP->Topics[CAAP->TopicsCB->CurLine];
		for(int i=0;i<CAAP->EXP.ExpList.GetAmount();i++){
			if(!strcmp(CAAP->EXP.TopicsList[CAAP->EXP.TopicsIdxs[i]],Topic)){
				CAAP->Type->AddLine(CAAP->EXP.ExpList[i]);
				CAAP->Classes.Add(CAAP->EXP.ExpList[i]);
			}
		}
		SetGlobalComboState(CAAP->Elem->GetClassName(),CAAP->TopicsCB->CurLine,0);
		/*
		const char* XBC=CAAP->Elem->GetExpansionBaseClass();
		if(XBC){
			OneClassStorage* OCS=CGARB.GetClass(XBC);			
			CAAP->Type->Clear();
			CAAP->Classes.Clear();
			if(OCS){
				for(int i=0;i<OCS->Children.GetAmount();i++){
					OneClassStorage* CCS=CGARB.GetClass(OCS->Children[i]);
					if(CCS&&CCS->ClassTopic&&!strcmp(CCS->ClassTopic,Topic)){
						CAAP->Type->AddLine(OCS->Children[i]);
						CAAP->Classes.Add(OCS->Children[i]);
					}
				}
				SetGlobalComboState(CAAP->Elem->GetClassName(),CAAP->TopicsCB->CurLine,0);
			}
		}
		*/
	}	
	return true;
}
bool ClassArray_OnComboClassSelect(SimpleDialog* SD){
	if(!SD->UserParam)return true;
	ClassArray_AddElmParam* CAAP=(ClassArray_AddElmParam*)SD->UserParam;		
	int topic=0;
	int classidx=0;
	GetGlobalComboState(CAAP->Elem->GetClassName(),topic,classidx);
	classidx=CAAP->Type->CurLine;
    SetGlobalComboState(CAAP->Elem->GetClassName(),topic,classidx);
	/*if(CAAP->ClassToExpand){
        CAAP->ClassToExpand->ExpandWith(CAAP->Classes[classidx],CAAP->ClassToExpand);
		CAAP->CE->ExpInfChanged=1;
	}*/
	return true;
}

bool ProcessLoadSave(int x,int y,int Lx,int Ly,_str& str,const char* mask,const char* StartDir,bool Load,ParentFrame* Back=NULL);
extern EditDialog Patient;
extern bool v_DialogsMissed;
bool CE_LoadClick(SimpleDialog* SD){
	LS_Param* LS=(LS_Param*)SD->AllocPtr;
	_str str;
	str="";
	int x0=0;
	int y0=0;
	ParentFrame* P0=LS->CE->Frame;
	ParentFrame* P00=P0;
	while(P0){
		x0+=P0->x;
		y0+=P0->y;
		P00=P0;
		P0=P0->ParentDS;
	}
	const char* src=LS->StartDir;
	if(LS->BC->CheckDirectCasting()){
		BaseClass* BC=(BaseClass*)LS->DataPtr;
		if(BC->GetSource())src=BC->GetSource();
	}
	bool CTRL=GetKeyState(VK_CONTROL)&0x8000;
	if(ProcessLoadSave(x0,y0,LS->CE->_x1-LS->CE->_x+1,LS->CE->_y1-LS->CE->_y+1,str,LS->Mask,src,1,P00)){
		ParentFrame* PF=dynamic_cast<ParentFrame*>(LS->BC);
		if(!CTRL)LS->BC->reset_class(LS->DataPtr);
		ErrorPager Err;
		if(LS->BC->CheckDirectCasting()&&((BaseClass*)LS->DataPtr)->SafeReadFromFile(str.pchar())){
			LS->CE->ExpInfChanged=1;
			BaseClass* SF_BaseClass;
			void* SF_DataPtr;
			void* SF_ExtraPtr;
			if(LS->BC->GetElementByName("SourceFile",&SF_BaseClass,&SF_DataPtr,&SF_ExtraPtr,LS->DataPtr)){
				if(!strcmp(SF_BaseClass->GetClassName(),"_str")){
					((_str*)SF_DataPtr)->Assign(str.pchar());
				}
			}			
		}
		v_DialogsMissed=false;		
	}
	Patient.Dialog.SetEditMode(1);
	return true;
}
bool CE_SaveClick(SimpleDialog* SD){
	testFilePathEditor TPZ;
	LS_Param* LS=(LS_Param*)SD->AllocPtr;
	_str str;
	str="";
	int x0=0;
	int y0=0;
	ParentFrame* P0=LS->CE->Frame;
	ParentFrame* P00=P0;
	while(P0){
		x0+=P0->x;
		y0+=P0->y;
		P00=P0;
		P0=P0->ParentDS;
	}
	const char* src=LS->StartDir;
	if(LS->BC->CheckDirectCasting()){
		BaseClass* BC=(BaseClass*)LS->DataPtr;
		if(BC->GetSource())src=BC->GetSource();
	}
	if(LS->BC->CheckDirectCasting()&&ProcessLoadSave(x0,y0,LS->CE->_x1-LS->CE->_x+1,LS->CE->_y1-LS->CE->_y+1,str,LS->Mask,src,0,P00)){
		((BaseClass*)LS->DataPtr)->WriteToFile(str.pchar());
	}
	return true;
}
struct LS_MoveParam{
	ClassEditor* Owner;
	BaseClass* BC;
	int Index;
	char op;
};
bool CE_MoveLine(SimpleDialog* SD){	
	LS_MoveParam* LS=(LS_MoveParam*)SD->AllocPtr;
	if(LS->op==2){
        LS->BC->DelElement(LS->Index);
	}else{
		if(GetKeyState(VK_CONTROL)&0x8000){
			LS->BC->MoveElement(LS->Index,LS->op<<6);
		}else{
			LS->BC->MoveElement(LS->Index,LS->op);
		}		
	}
	LS->Owner->ExpInfChanged=1;
	return true;
}

void CheckDSS(ParentFrame* P){
	for(int i=0;i<P->DSS.GetAmount();i++)if(P->DSS[i]){
		try{
			P->DSS[i]->ShiftDialog(1,0);
			P->DSS[i]->ShiftDialog(-1,0);
		}catch(...){
			int xxx=0;
		}
	}
}
char t_decoder[256]="X";
char d_tblR[]="‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ˜¯˘¸˚˙˝˛ˇ¿¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ‹€⁄›ﬁﬂ";
char d_tblT[]="abvgdegzijklmnoprstufxc4ss'i'euaABVGDEGZIJKLMNOPRSTUFXC4SS'I'EUA";
void Translit(char* s){
	if(!s)return;
	if(t_decoder[0]=='X'){
		for(int i=0;i<256;i++)t_decoder[i]=i;
		for(int i=0;d_tblR[i];i++){
			t_decoder[d_tblR[i]]=d_tblT[i];
		}
	}
	for(int i=0;s[i];i++){
		s[i]=t_decoder[s[i]];
	}
}
//  |
//  |
//  |
DWORD ce_CC=0xF0B0B0B0;
DWORD ce_CL=0xF0F0E0D0;
DWORD ce_CD=0xF0606060;

void DrawVElement(Canvas* CN,int x,int y,int x1,int y1){
	int LX=x1-x+1;
	int L2=LX/2;
    CN->AddBar4(x+L2-1,y,3,y1-y+1,ce_CL,ce_CD,ce_CL,ce_CD);
}
//  |
//  |--
//  |
void DrawTElement(Canvas* CN,int x,int y,int x1,int y1){
	int LX=x1-x+1;
	int L2=LX/2;
	CN->AddBar4(x+L2-1,y,3,y1-y+1,ce_CL,ce_CD,ce_CL,ce_CD);    
	//CN->AddBar4(x+L2+1,(y+y1)/2-1,LX-L2-2,3,ce_CL,ce_CL,ce_CD,ce_CD);
}
//  |
//  |--
//  
void DrawCElement(Canvas* CN,int x,int y,int x1,int y1){
	int LX=x1-x+1;
	int L2=LX/2;
	CN->AddBar4(x+L2-1,y,3,(y1-y+1)/2,ce_CL,ce_CD,ce_CL,ce_CD);    
	CN->AddBar4(x+L2,(y+y1)/2-1,LX-L2-2+1,3,ce_CL,ce_CL,ce_CD,ce_CD);
}
//  
//  [+]
//  
int RLX=12;
int DX=7;
int DX1=4;
void DrawPElement(Canvas* CN,int x,int y,int x1,int y1){
	int LX=x1-x+1;
	int L2=LX/2;
	int yc=(y+y1)/2;
	CN->AddBar4(x-DX,yc-RLX/2,2,RLX,ce_CL,ce_CL,ce_CD,ce_CD);
	CN->AddBar4(x+RLX-2-DX1,yc-RLX/2,2,RLX,ce_CL,ce_CL,ce_CD,ce_CD);
	CN->AddBar4(x-DX+2,yc-RLX/2,RLX-4+DX-DX1,2,ce_CL,ce_CL,ce_CL,ce_CL);
	CN->AddBar4(x-DX+2,yc+RLX/2-2,RLX-4+DX-DX1,2,ce_CD,ce_CD,ce_CD,ce_CD);
	CN->AddBar4(x-DX+2,yc-RLX/2+2,RLX-4+DX-DX1,RLX-4,ce_CD,ce_CD,ce_CL,ce_CL);
	int xc=x+(RLX-DX1-DX)/2+1;
	CN->AddBar4(xc-3,yc-1,6,2,0xFF808080,0xFF808080,0xFF404040,0xFF404040);
	CN->AddBar4(xc-1,yc-3,2,6,0xFF606060,0xFF606060,0xFF606060,0xFF606060);
}
void GetPElementSize(int x,int y,int x1,int y1,Rct& size){
	int LX=x1-x+1;
	int L2=LX/2;
	int yc=(y+y1)/2;
	size.x=x-DX;
	size.y=yc-RLX/2;
	size.w=RLX+DX-DX1;
	size.h=RLX;
}
//
//  [-]
//  
void DrawMElement(Canvas* CN,int x,int y,int x1,int y1){
	int LX=x1-x+1;
	int L2=LX/2;
	int yc=(y+y1)/2;
	CN->AddBar4(x-DX,yc-RLX/2,2,RLX,ce_CL,ce_CL,ce_CD,ce_CD);
	CN->AddBar4(x+RLX-2-DX1,yc-RLX/2,2,RLX,ce_CL,ce_CL,ce_CD,ce_CD);
	CN->AddBar4(x-DX+2,yc-RLX/2,RLX-4+DX-DX1,2,ce_CL,ce_CL,ce_CL,ce_CL);
	CN->AddBar4(x-DX+2,yc+RLX/2-2,RLX-4+DX-DX1,2,ce_CD,ce_CD,ce_CD,ce_CD);
	CN->AddBar4(x-DX+2,yc-RLX/2+2,RLX-4+DX-DX1,RLX-4,ce_CD,ce_CD,ce_CL,ce_CL);
	int xc=x+(RLX-DX1-DX)/2+1;	
	CN->AddBar4(xc-3,yc-1,6,2,0xFF606060,0xFF606060,0xFF606060,0xFF606060);
	//CN->AddBar4(xc-1,yc-3,2,7,0xFF000000,0xFF000000,0xFF000000,0xFF000000);
}
extern WholeClassPresentation CE_WCP;
ExpandInfo* ClassEditor::CurExpInf=NULL;
bool ClassEditor::CheckIfOpen(BaseClass* BC){
	if(CurExpInf){
		if(CurExpInf->find(BC)!=-1){
			if(BC->GetParent()){
				if(strstr(BC->GetParent()->GetClassName(),"ClassRef."))return true;
                return CheckIfOpen(BC->GetParent());
			}else return true;
		}else return false;
	}else return false;
}
int ClassEditor::CreateElement(ParentFrame* Base,int Level,DWORD Mask,int x,int y0,int l_lx,int r_lx,
							BaseClass* BC,int i,BaseClass* ParentBC,const char* ElName){
	static BaseClass* ELMLIST[256];
	static int NELM=0;
	CurExpInf=&ExpInf;

	xmlQuote xml;
	int NElm		 =BC->GetAmountOfEditableElements(BC,Opt);
	BaseClass* ElmBC =BC->GetEditableElementClass(i,BC,Opt);
	void* ElmPtr	 =BC->GetEditableElementData (i,BC,Opt);
	void* ElmExtra	 =BC->GetEditableElementExtra(i,BC,Opt);
	int IDX			 =BC->GetRealMaskedPosition(BC->GetIndexOfEditableElement(i,BC,Opt));
	const char* ename=BC->GetEditableElementName(i,BC,Opt);
	bool ReadOnly    =BC->CheckIfElementReadOnly(i,BC,Opt);
	bool Invisible   =BC->CheckIfElementInvisible(i,BC,Opt);
	if(Invisible) return y0;	

    ClassExpParams ExpList;
	BC->GetEditableElementExpansionList(ExpList,i,BC,Opt,ParentBC,ElName);
	bool IsNode=BC->CheckIfEditableElementIsNode(i,BC,Opt);	

	_str eview;
	_str ehint;
	eview="";
	ehint="";
	if(ElmBC)ElmBC->GetElementMessageAndHintInEditor(eview,ehint,BC,(char*)ename);
	
	if(eview.pchar()[0]==0){
        eview=ename;        
	}

	char* ElmView=(char*)BC->GetEditableElementView(i,BC,eview.pchar(),Opt);

	///if(strcmp(ElmView,ename)){		
		eview=ElmView;
	//}else{
		if( ElmBC && ElmBC->CheckDirectCasting() && ((BaseClass*)ElmPtr)->GetClassPresentationMask(eview,ElmBC,NULL,"root",0)){
			CE_WCP.GetPresentation(eview,(BaseClass*)ElmPtr);
		//	CE_WCP.GetPresentation(eview,(BaseClass*)ElmPtr,ElmPtr,NULL,NULL);
		}
	//}
	//eview.Limit(1024);
	Translit(eview.pchar());
	char cc[2048];
	if(!strcmp(ElmView,"add...")){
		sprintf(cc,"@ADDID::%s",BC->GetClassName());
		char* s=GetTextByID(cc);
		if(s[0]!='@')eview=s;
		else eview=ElmView;
		bool GetHintDebugMode();
		if(GetHintDebugMode()){
			sprintf(cc,"@ADDHINT::%s",BC->GetClassName());
			ehint=GetTextByID(cc);
		}
	}
	//searching for corresponding editor of element
	OneElementEditor* OED=NULL;
	char* CName;
	if(ElmBC&&ElmPtr){			
		CName=(char*)ElmBC->GetClassName();
		int pass=0;
		do{
			for(int j=0;j<type_list.GetAmount();j++){
				const char* cn=type_list[j]->GetClassID();
				if(cn){
					if(!strcmp(cn,CName)){
						OED=type_list[j]->_new();
						break;
					}
				}else{
					if(type_list[j]->CheckClass(ElmBC)){
						OED=type_list[j]->_new();
						break;
					}
				}
			}
			if(!OED){
				pass++;
				OneClassStorage* OCS=ElmBC->GetClassStorage();
				int np=OCS->Parents.GetAmount();				
				if(pass<=np){
					CName=OCS->Parents[np-pass];
				}else pass=100;
			}			
		}while(pass<100&&!OED);
	}
	// simplification of tree
	if(!(GetKeyState(VK_MENU)&0x8000)){		
		/*
		if(ElmBC&&ElmBC->CheckDirectCasting()){
			int NE=ElmBC->GetAmountOfEditableElements(ElmPtr,7);
			if(Level>0&&ExpList.ExpList.GetAmount()==0&&OED==NULL
				&&NE==1&&NE==ElmBC->GetAmountOfEditableElements(ElmPtr,3)){
					y0=CreateElement(Base,Level,Mask,x,y0,l_lx,r_lx,(BaseClass*)ElmPtr,0,BC,ename);				
					return y0;
				}
		}
		*/
		if(ElmBC&&ElmBC->CheckDirectCasting()){
			ElmBC=(BaseClass*)ElmPtr;
			int NE=BC->GetAmountOfEditableElements(BC,Opt);
			sprintf(cc,"LSENUM_%s",ElmBC->GetClassName());
			Enumerator* E=ENUM.Get(cc);
			if(Level>0&&((NE==1&&ExpList.ExpList.GetAmount()==0&&OED==NULL&&E->GetAmount()<2&&!(strchr(eview.pchar(),'{')||BC->GetExpansionRules()&3))||ElmBC->ForceSimplification())){
				BaseClass* EP=(BaseClass*)ElmPtr;
				int nElm = EP->GetAmountOfEditableElements(EP,Opt);				
				int SectState=0;
				int NS=0;
				for(int j=0;j<nElm;j++){					
					BaseClass* EBC  =EP->GetEditableElementClass(j,EP,Opt);
					void* EPT       =EP->GetEditableElementData (j,EP,Opt);
					int s=0;
					if(EBC&&EBC->CheckDirectCasting()){
						EBC=(BaseClass*)EPT;
						s=EBC->GetSectionStatus();
					}						
					if(SectState!=1||s!=0)NS++;
					if(s!=0)SectState=s;
				}
				int NS0=0;
				SectState=0;
				for(int j=0;j<nElm;j++){
					int rdx=0;
					if(j==0&&BC->GetExpansionRules()&3){
						int LX=16;
                        rdx=LX;
						LS_MoveParam* LS=new LS_MoveParam;			
						LS->BC=BC;
						LS->Index=IDX;
						LS->op=2;
						LS->Owner=this;						
						static int bidx=GPS.PreLoadGPImage("Interf3\\btnz2");
						GP_Button* BD=Base->addGP_Button(NULL,l_lx+r_lx-LX+2,y0,bidx,9,8);
						BD->ShapeFileID=bidx;
						BD->ShapeSpriteID=9;
						BD->OnUserClick=&CE_MoveLine;
						BD->AllocPtr=(char*)LS;						
						//ScaleControl(BD);
					}
					BaseClass* EBC  =EP->GetEditableElementClass(j,EP,Opt);
					void* EPT       =EP->GetEditableElementData (j,EP,Opt);					
					int s=0;
					if(EBC&&EBC->CheckDirectCasting()){
						EBC=(BaseClass*)EPT;
						s=EBC->GetSectionStatus();
					}					
					if(SectState!=1||s!=0){
						DWORD m=Mask;
						if(j<nElm-1) m|=1<<(Level-1);
						y0=CreateElement(Base,Level,m,x,y0,l_lx,r_lx-rdx,EP,j,BC,ename);
						NS0++;
					}
					if(s!=0)SectState=s;
				}				
				return y0;
			}
		}
	}
	//basic graphical controls, now - simply colored bars
	ColoredBar* CB0=Base->addColoredBar(0,y0,l_lx,CED_LineDist-2,0xFF);
	setstyle(CB0);
	DWORD DIFF[]={0xAF000000,0xAF000050,0xAF003050,0xAF500000,0xAF500050};
	CB0->Diffuse=DIFF[Level%5];//0xAF000000+0x00000050*(Level&1);//CED_FieldColor-0x101010*(Level<6?Level:6);	
	ColoredBar* CB=Base->addColoredBar(l_lx+2,y0,r_lx,CED_LineDist-2,0xFF);
	setstyle(CB);
	CB->Diffuse=CED_FieldColor;
	if(ehint.pchar()&&ehint.pchar()[0]){
		CB0->Hint=znew(char,strlen(ehint.pchar())+1);
		strcpy(CB0->Hint,ehint.pchar());
		CB0->AllocHint=1;
		CB->Hint=znew(char,strlen(ehint.pchar())+1);
		strcpy(CB->Hint,ehint.pchar());		
		CB->AllocHint=1;
	}
	ControlParams CP;
	CP.LeftColumnWidth=l_lx;
	CP.RightColumnWidth=r_lx;
	CP.BaseClassType=-1;		
	CP.VerticalText=0;
	CP.HideText=0;
	CP.IconFile=-1;
	CP.GP_dx=0;
	CP.GP_dy=0;
	CP.CE=this;
	CP.TextFont=&CED_Font;
	int y00=y0;	
	int adds=CED_LevelWidth-CED_SqSize+2;
	int x0=8+Level*CED_LevelWidth;	
	CP.MinLeftWidth=GetRLCStrWidth(eview.pchar(),&CED_Font)+8;
	CP.AbsMinLeftWidth=x0+adds+8+CED_LineDist;	
	if(OED)OED->CE=this;
	ElementParam* EP=new ElementParam;
	int idx=ElementEditors.Add(OED);
	EP->YPos=y0;
	EP->BC=ElmBC;
	EP->DataPtr=ElmPtr;
	EP->ExtraPtr=ElmExtra;
	EP->Index=ElParams.GetAmount();
	EP->CBar=CB0;
	EP->OEE=OED;
	EP->NowActive=0;
	EP->OEE=OED;
	EP->Owner=this;
	EP->FieldName=eview;
	EP->BaseParent=BC;
	EP->BaseParentIndex=IDX;
	EP->BackColor=CB0->Diffuse;
	EP->ParentClassMask=BC->GetClassMask();
	CP.ELP=EP;
	int eidx=ElParams.Add(EP);
	CB0->UserParam=int(EP);
	ElementParam* EPR=ElParams[eidx];
	CB0->OnUserClick=&CBR_OnUserClick;	
	int yy=y0+CED_LineDist;
	//load,save,move,delete:
	int BTLX=0;
	if(ElmBC){		
		sprintf(cc,"LSENUM_%s",ElmBC->GetClassName());
		Enumerator* E=ENUM.Get(cc);		
		if(BC->GetExpansionRules()&3){
			BTLX+=16;
		}
		if(E&&E->GetAmount()>=2){//load,save		
			BTLX+=16*2;
		}
		//if(BC->MoveElement(IDX,0)){//move
		//	BTLX+=26*2;
		//}		
	}
	CP.RightColumnWidth-=BTLX;
	int maxwid=100000;
	if(OED){
		int nb=Base->DSS.GetAmount();
		yy=OED->CreateControl(Base,l_lx+2,y00,l_lx+r_lx-BTLX,y0+CED_LineDist-2,ElmBC,ElmPtr,ElmExtra,&CP);	
		if(ReadOnly){
			for(int q=nb;q<Base->DSS.GetAmount();q++){
				if(Base->DSS[q])Base->DSS[q]->Enabled=false;
			}
		}
		OED->Assign(ElmBC,ElmPtr,ElmExtra);
		if(CP.BaseClassType!=-1)
			EP->BaseClassType=CP.BaseClassType;
	}else{
		if(ElmPtr&&!ExpList.ExpList.GetAmount()){
			CP.LeftColumnWidth+=CP.RightColumnWidth;
			CP.RightColumnWidth=0;
		}
		/*if(eview.pchar()){
			int GetRHeight(char* s,RLCFont* font,int maxW);
			maxwid=x0+adds+CP.GP_dx
			int yy2=y0+GetRHeight(eview.pchar(),CP.TextFont,)+4;
			yy=max(yy2,yy);
		}*/
	}
	if(CP.RightColumnWidth)CP.RightColumnWidth+=BTLX;
	else CP.LeftColumnWidth+=BTLX;
	y0=yy;	
	CB0->x1=CP.LeftColumnWidth;
	CB->x=CB0->x1+2;
	CB->x1=CB0->x1+2+CP.RightColumnWidth;
	if(eview.pchar() && !CP.HideText){
		int GetRHeight(char* s,RLCFont* font,int maxW);
		maxwid=CB0->x1-x0-adds-CP.GP_dx-4;
		int yy2=y00+GetRHeight(eview.pchar(),CP.TextFont,maxwid)+CED_TextDy+4;
		y0=max(yy2,y0);
		yy=y0;
	}
	static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo2");
	if(ExpList.ExpList.GetAmount()){//if there is something to expand
		int topidx=0;
		int classidx=0;
		GetGlobalComboState(BC->GetClassName(),topidx,classidx);
		ClassArray_AddElmParam* CAAP=znew(ClassArray_AddElmParam,1);
		ComboBox* CBB=NULL;
		ComboBox* Tops=NULL;
		char tid[512];

		for(int i=0;i<ExpList.ExpList.GetAmount();i++)CAAP->EXP.ExpList.Add(ExpList.ExpList[i]);
		for(int i=0;i<ExpList.TopicsIdxs.GetAmount();i++)CAAP->EXP.TopicsIdxs.Add(ExpList.TopicsIdxs[i]);
		for(int i=0;i<ExpList.TopicsList.GetAmount();i++)CAAP->EXP.TopicsList.Add(ExpList.TopicsList[i]);

		if(ExpList.TopicsList.GetAmount()){			
			Tops = Base->addGP_ComboBoxDLX(NULL,l_lx+2,y00,r_lx-BTLX,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
			VScrollBar* VS1;
			Tops->AssignScroll((DialogsSystem*)Base,&VS1,3,0,8);

			CBB  = Base->addGP_ComboBoxDLX(NULL,l_lx+2,y00+Tops->GetHeight(),r_lx-BTLX,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
			VScrollBar* VS2;
			CBB->AssignScroll((DialogsSystem*)Base,&VS2,3,0,8);

			y0=max(y00+2*Tops->GetHeight(),y0);
			for(int j=0;j<ExpList.TopicsList.GetAmount();j++){
				Tops->AddLine(ExpList.TopicsList[j]);
				CAAP->Topics.Add(ExpList.TopicsList[j]);
			}			
			Tops->CurLine=topidx;			
			if(Tops->CurLine>Tops->NLines){
				Tops->CurLine=0;
				topidx=0;
				
			}            
			Tops->AutoID="@CGRPID";
			for(int j=0;j<ExpList.ExpList.GetAmount();j++){
				if(ExpList.TopicsIdxs[j]!=-1&&topidx==ExpList.TopicsIdxs[j]){
					CBB->AddLine(ExpList.ExpList[j]);
					CAAP->Classes.Add(ExpList.ExpList[j]);
				}
			}
			CBB->AutoID="@CLASSID";
		}else{
			CBB  = Base->addGP_ComboBoxDLX(NULL,l_lx+2,y00,r_lx-BTLX,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
			VScrollBar* VS;
			CBB->AssignScroll((DialogsSystem*)Base,&VS,3,0,8);

			y0=max(y00+CBB->GetHeight(),y0);
			for(int j=0;j<ExpList.ExpList.GetAmount();j++){				
				CBB->AddLine(ExpList.ExpList[j]);
				CAAP->Classes.Add(ExpList.ExpList[j]);
			}
			CBB->AutoID="@CLASSID";
		}
		if(CBB){
			CBB->CurLine=classidx;
			if(CBB->CurLine>CBB->NLines)CBB->CurLine=0;
			CAAP->ClassToExpand=BC;//ElmBC&&ElmBC->CheckDirectCasting()?(BaseClass*)ElmPtr:NULL;
			CBB->CurLine=classidx;			
			CAAP->Type=CBB;
			CAAP->Elem=BC;
			CAAP->CE=this;	
			CAAP->TopicsCB=Tops;
			if(!ElmPtr){
				CB0->AllocPtr=(char*)CAAP;
				CB0->OnUserClick=&ClassArray_AddElement;
				CBB->OnSelect=&ClassArray_OnComboClassSelect;
				CBB->UserParam=(int)CAAP;
			}
		}
		if(Tops){
			Tops->OnSelect=&ClassArray_OnComboTopicSelect;
			Tops->UserParam=(int)CAAP;
		}
	}
	CB0->y1=y0;
	CB->y1=y0;
	int clDist=y0-y00;
	//adding tree lines	
	for(int j=0;j<Level;j++){		
		if(Mask&(1<<j)){
			//MainCanvas->AddLine(8+j*CED_LevelWidth+CED_LevelWidth/2,y00,8+j*CED_LevelWidth+CED_LevelWidth/2,y00+clDist,0xFFFFFFFF);		
			if(j==Level-1)DrawTElement(MainCanvas,8+j*CED_LevelWidth,y00,8+j*CED_LevelWidth+CED_LevelWidth,y00+clDist);
			else DrawVElement(MainCanvas,8+j*CED_LevelWidth,y00,8+j*CED_LevelWidth+CED_LevelWidth,y00+clDist);
		}else
		if(j==Level-1){
			//MainCanvas->AddLine(8+j*CED_LevelWidth+CED_LevelWidth/2,y00,8+j*CED_LevelWidth+CED_LevelWidth/2,y00+clDist/2,0xFFFFFFFF);
			//MainCanvas->AddLine(8+j*CED_LevelWidth+CED_LevelWidth/2,y00+clDist/2,8+(j+1)*CED_LevelWidth+CED_LevelWidth/2-CED_SqSize,y00+clDist/2,0xFFFFFFFF);
			DrawCElement(MainCanvas,8+j*CED_LevelWidth,y00,8+j*CED_LevelWidth+CED_LevelWidth,y00+clDist);
		}
		MainCanvas->y1=y00;
	}
	if(ElmBC&&IsNode&&ElmBC->CheckDirectCasting()){
		//adding +,- in square to expand/close
		adds=CED_LevelWidth+10;
		BaseClass* B2=(BaseClass*)ElmPtr;
		int N1=B2->GetAmountOfEditableElements(B2,Opt);
		int s=ElmBC->GetSectionStatus();
		if(s!=1&&s!=2){
			Rct R;
			GetPElementSize(x0,y00,x0+CED_LevelWidth,y00+clDist,R);
			SimpleDialog* VP=Base->addViewPort(R.x,R.y,R.w,R.h);
			exp_info* einf=(exp_info*)malloc(sizeof exp_info);
			einf->CE=this;
			einf->elem=ElmPtr;
			VP->AllocPtr=(char*)einf;
			VP->OnUserClick=&EXP_OnUserClick;
			//MainCanvas->AddRect(x0-CED_SqSize+CED_LevelWidth/2,y00-CED_SqSize+clDist/2,CED_SqSize*2,CED_SqSize*2,0xFFFFFFFF);
			//MainCanvas->AddLine(x0-CED_SqSize+CED_LevelWidth/2+2,y00+clDist/2,x0+CED_SqSize+CED_LevelWidth/2-1,y00+clDist/2,0xFFFFFFFF);			
			MainCanvas->y1=y00;
		}
		if(NELM<255){
			bool NOEXP=false;
			for(int i=0;i<NELM;i++)if(ELMLIST[i]==BC)NOEXP=true;//unlimited recursion checking			
			if(!NOEXP){
				ELMLIST[NELM]=BC;
				NELM++;
				if(ExpInf.find(ElmPtr)!=-1){
					//expanding element
					//MainCanvas->AddLine(x0+CED_LevelWidth/2,y00+clDist/2+CED_SqSize,x0+CED_LevelWidth/2,y00+clDist,0xFFFFFFFF);
					DrawVElement(MainCanvas,x0,y00+clDist/2+CED_SqSize,x0+CED_LevelWidth,y00+clDist);
					MainCanvas->y1=y00;
					int SectState=0;
					int NS=0;
					for(int j=0;j<N1;j++){
						BaseClass* EBC  =B2->GetEditableElementClass(j,B2,Opt);
						void* EPT       =B2->GetEditableElementData (j,B2,Opt);
						int s=0;
						if(EBC&&EBC->CheckDirectCasting()){
							EBC=(BaseClass*)EPT;
							s=EBC->GetSectionStatus();
						}						
						if(SectState!=1||s!=0)NS++;
						if(s!=0)SectState=s;
					}
					int NS0=0;
					SectState=0;
					for(int j=0;j<N1;j++){
						BaseClass* EBC  =B2->GetEditableElementClass(j,B2,Opt);
						void* EPT       =B2->GetEditableElementData (j,B2,Opt);
						int s=0;
						if(EBC&&EBC->CheckDirectCasting()){
							EBC=(BaseClass*)EPT;
							s=EBC->GetSectionStatus();
						}						
						if(SectState!=1||s!=0){
							y0=CreateElement(Base,Level+1,NS0==NS-1?Mask:Mask|(1<<Level),x,y0,l_lx,r_lx,B2,j,BC,ename);
							NS0++;
						}
						if(s!=0)SectState=s;
						
					}
					DrawMElement(MainCanvas,x0,y00,x0+CED_LevelWidth,y00+clDist);
				}else{
					//element is not expanded
					if(s!=1&&s!=2){
						//MainCanvas->AddLine(x0+CED_LevelWidth/2,y00+clDist/2-CED_SqSize+2,x0+CED_LevelWidth/2,y00+clDist/2+CED_SqSize-1,0xFFFFFFFF);
						DrawPElement(MainCanvas,x0,y00,x0+CED_LevelWidth,y00+clDist);
						MainCanvas->y1=y00;
					}
				}
				NELM--;
			}
		}
	}
	//adding text field
	if(!CP.HideText){
		char* el=EPR?EPR->FieldName.pchar():eview.pchar();
		//char cc[2048];
		char* e1=el;
		if(strlen(el)<128){
			sprintf(cc,"$cid_%s",el);
			e1=GetTextByID(cc);
			if(e1[0]=='$')e1=el;
		}
		if(CP.VerticalText){
			Base->addVTextButton(NULL,x0+adds+CP.GP_dx+CED_LineDist/2-8,y00+CP.GP_dy+CED_LineDist/2+(clDist-CED_LineDist)/2,e1,CP.TextFont,CP.TextFont,CP.TextFont,1);
		}else{			
			TextButton* TB=Base->addTextButton(NULL,x0+adds+CP.GP_dx,y00+CP.GP_dy+CED_TextDy/*+(clDist-CED_LineDist)/2*/,e1,CP.TextFont,CP.TextFont,CP.TextFont,0);						
			TB->MaxWidth=maxwid;
			//if(CP.MinLeftWidth>CB0->x1-TB->x-2){
            //    float scx=float(CB0->x1-TB->x-2)/float(CP.MinLeftWidth);
			//	TB->EnableTransform=true;
			//	TB->ScaleX=scx;
			//}

			//int Y1=TB->y1+CED_TextDy;
			//if(Y1>CB->y1)CB->y1=Y1;
			//if(Y1>CB0->y1)CB0->y1=Y1;
			//if(y0<Y1)y0=Y1;
		};
	}	
	//load,save,move,delete:
	if(ElmBC){		
		sprintf(cc,"LSENUM_%s",ElmBC->GetClassName());
		Enumerator* E=ENUM.Get(cc);
		static int bidx=GPS.PreLoadGPImage("Interf3\\btnz2");
		if(BC->GetExpansionRules()&3){//delete
			LS_MoveParam* LS=new LS_MoveParam;			
			LS->BC=BC;
			LS->Index=IDX;
			LS->op=2;
			LS->Owner=this;
			int LX=16;
			GP_Button* BD=Base->addGP_Button(NULL,CB->x1-LX,CB->y,bidx,9,8);
			BD->ShapeFileID=bidx;
			BD->ShapeSpriteID=9;
			BD->OnUserClick=&CE_MoveLine;
			BD->AllocPtr=(char*)LS;
			CB->x1-=LX;
			//ScaleControl(BD);
		}
		if(E&&E->GetAmount()>=2){//load,save		
			LS_Param* LS=new LS_Param;
			LS_Param* LL=new LS_Param;
			LS->BC=ElmBC;
			LS->DataPtr=ElmPtr;
			LS->ExtraPtr=ElmExtra;
			LS->CE=this;
			LS->Mask=E->GetStr(1);
			LS->StartDir=E->GetStr(0);
			*LL=*LS;
			int LX=16;
			GP_Button* BS=Base->addGP_Button(NULL,CB->x1-LX,CB->y,bidx,3,2);
			BS->ShapeFileID=bidx;
			BS->ShapeSpriteID=3;
			BS->OnUserClick=&CE_SaveClick;
			BS->AllocPtr=(char*)LS;
			GP_Button* BL=Base->addGP_Button(NULL,CB->x1-LX-LX,CB->y,bidx,1,0);
			BL->ShapeFileID=bidx;
			BL->ShapeSpriteID=1;
			BL->OnUserClick=&CE_LoadClick;
			BL->AllocPtr=(char*)LL;
			CB->x1-=LX*2;
			//ScaleControl(BS);
			//ScaleControl(BL);
		}
		/*
		if(BC->MoveElement(IDX,0)){//move		
			LS_MoveParam* LS=new LS_MoveParam;
			LS_MoveParam* LL=new LS_MoveParam;
			LS->BC=BC;
			LS->Index=IDX;
			LS->op=1;
			LS->Owner=this;
			LL->BC=BC;
			LL->Index=IDX;
			LL->op=-1;
			LL->Owner=this;
			int LX=26;
			GP_Button* BD=Base->addGP_Button(NULL,CB->x1-LX,CB->y,bidx,5,4);
			BD->ShapeFileID=bidx;
			BD->ShapeSpriteID=5;
			BD->OnUserClick=&CE_MoveLine;
			BD->AllocPtr=(char*)LL;
			if(i==0)BD->Diffuse=0x90FFFFFF;
			GP_Button* BL=Base->addGP_Button(NULL,CB->x1-LX-LX,CB->y,bidx,7,6);
			BL->ShapeFileID=bidx;
			BL->ShapeSpriteID=7;
			BL->OnUserClick=&CE_MoveLine;
			BL->AllocPtr=(char*)LS;
			if(IDX==BC->GetAmountOfElements()-1)BL->Diffuse=0x90FFFFFF;
			CB->x1-=LX*2;	
			ScaleControl(BD);
			ScaleControl(BL);
		}	
		*/
	}	
	if(CP.IconFile!=-1){
		Base->addGP_Button(NULL,x0+adds+CP.GP_dx,y00+CED_TextDy+(clDist-CED_LineDist)/2+CP.GP_dy,CP.IconFile,CP.IconSprite,CP.IconSprite);
		adds+=CP.GP_Lx;
	}
	if(CheckClipStatus(eidx)){
		int x0=CB0->x+1;
		int y0=(CB0->y+CB0->y1)/2-1;
		ColoredBar* CB2=Base->addColoredBar(x0,y0,2,3,255);
		CB2->Diffuse=0x4000FF00;
		CB2=Base->addColoredBar(x0+2,y0-2,1,7,255);
		CB2->Diffuse=0x2000FF00;
		CB2=Base->addColoredBar(x0+3,y0-1,1,5,255);
		CB2->Diffuse=0x2000FF00;
		CB2=Base->addColoredBar(x0+4,y0,1,3,255);
		CB2->Diffuse=0x23000FF00;
		CB2=Base->addColoredBar(x0+5,y0+1,1,1,255);
		CB2->Diffuse=0x3000FF00;
	}	
	return y0;
}
void ClassEditor::ClearActive(){
	for(int i=0;i<ElParams.GetAmount();i++){
		ElementParam* EP=ElParams[i];
		EP->CBar->Diffuse=EP->BackColor;//CED_FieldColor;
		EP->NowActive=0;
	}
}
void ClassEditor::setScrollerOnSelectedElement(){
	if ( MainScroller== NULL ) return;
	if ( CurElement!=-1 && ElParams[CurElement]->CBar!=NULL ){
		int l_scrlD = ElParams[CurElement]->CBar->y - Frame->GetHeight()/2;
		l_scrlD = ( (l_scrlD>=0) ? (l_scrlD) : (0) );
		MainScroller->SPos = l_scrlD;
	};
};
DWORD GetSHash(const char* s){
	DWORD H=0;
	if(s){
		int L=strlen(s);
		for(int i=0;i<L;i++)H+=s[i]*(i+1);
	}
	return H;
}
DWORD GetXML_HashNR(xmlQuote& xml){
	DWORD H=0;	
	H+=GetSHash(xml.Get_string());
	H+=GetSHash(xml.GetQuoteName());
	return H;
}
DWORD GetXML_Hash(xmlQuote& xml){
	DWORD H=0;
    H+=GetSHash(xml.Get_string());
	H+=GetSHash(xml.GetQuoteName());
	if(!xml.Get_string()[0]){
		for(int i=0;i<xml.GetNSubQuotes();i++){
			H+=GetXML_HashNR(*(xml.GetSubQuote(i)));
		}
	}
	return H;
}
bool ClassEditor::CheckClipStatus(int CurElement){
	if(CurElement!=-1&&CurClip.GetQuoteName()){		
		ElementParam* ELP=ElParams[CurElement];				
		if(ELP->BC){			
			if(!strcmp(CurClip.GetQuoteName(),ELP->BaseParent->GetClassName())){
				if(CurClip.GetNSubQuotes()==1){
					return true;				
				}
			}else{
				if(!strcmp(CurClip.GetQuoteName(),ELP->BC->GetClassName())){
					if(CurClip.GetNSubQuotes()==1&&ELP->BC->CheckDirectCasting()){
						xmlQuote* x2=CurClip.GetSubQuote(0);
						BaseClass* BCL=(BaseClass*)ELP->DataPtr;
						return (BCL->GetExpansionRules()&3)!=0;					
					}
				}
			}
		}
	}
	return false;
}
void ClassEditor::Process(){
	extern bool IgnoreProcessMessages;
	IgnoreProcessMessages=0;
	if(!MainScroller)return;
	extern bool KeyPressed;
	extern int LastKey;
	/*if(KeyPressed&&LastKey==VK_DELETE){
		for(int i=0;i<ElementEditors.GetAmount();i++)if(ElParams[i]->NowActive){
			OneElementEditor* OEE=ElementEditors[i];
			ElementParam* ELP=ElParams[i];
			if(ELP->BaseParent->CheckDirectCasting()){
				if(ELP->BaseParent->DelElement(ELP->BaseParentIndex))ExpInfChanged=1;
			}
		}
	}*/
	if((GetKeyState(VK_CONTROL)&0x8000) && (GetKeyState(VK_SHIFT)&0x8000) && LastKey=='D'){//copy test
		if(CurElement!=-1){
			LastKey=0;		
			ElementParam* ELP=ElParams[CurElement];
			if(ELP->BaseParent){
				int p=ELP->BaseParent->ExpandWith(ELP->BC->GetClassName(),ELP->BaseParent);
				if(p!=-1){
					BaseClass* B=(BaseClass*)ELP->BaseParent->GetElementPtr(p,ELP->BaseParent);
					((BaseClass*)ELP->DataPtr)->Copy(B,false);
				}
				ExpInfChanged=1;
			}
		}
	}
	if((GetKeyState(VK_CONTROL)&0x8000) && LastKey==VK_UP){
		if(CurElement!=-1){
			LastKey=0;		
			ElementParam* ELP=ElParams[CurElement];
			if(ELP->BaseParent){
				ELP->BaseParent->MoveElement(ELP->BaseParentIndex,-1);
				ExpInfChanged=1;
			}
		}
	}
	if((GetKeyState(VK_CONTROL)&0x8000) && LastKey==VK_DOWN){
		if(CurElement!=-1){
			LastKey=0;		
			ElementParam* ELP=ElParams[CurElement];
			if(ELP->BaseParent){
				ELP->BaseParent->MoveElement(ELP->BaseParentIndex,1);
				ExpInfChanged=1;
			}
		}
	}
	if(LastKey=='C'&&(GetKeyState(VK_CONTROL)&0x8000)){//copy
		if(CurElement!=-1){
			LastKey=0;		
			ElementParam* ELP=ElParams[CurElement];
			if(ELP->BaseParent){
				xmlQuote xml((char*)ELP->BaseParent->GetClassName());
				xmlQuote* xml2=new_xmlQuote((char*)(ELP->BC)->GetClassName());
				(ELP->BC)->Save(*xml2,ELP->DataPtr,ELP->ExtraPtr);
				xml.AddSubQuote(xml2);
				xml.WriteToFile("Dialogs\\clip.xml");	
				CurClip.ClearAll();
				CurClip.ReadFromFile("Dialogs\\clip.xml");
				ExpInfChanged=1;
			}
		}
	}
	if(LastKey=='V'&&(GetKeyState(VK_CONTROL)&0x8000)){//paste			
		if(CurElement!=-1){
			LastKey=0;	
			ElementParam* ELP=ElParams[CurElement];
            xmlQuote xml;
			if(ELP->BaseParent&&xml.ReadFromFile("Dialogs\\CLIP.xml")){				
				if(!strcmp(xml.GetQuoteName(),ELP->BaseParent->GetClassName())){
					if(xml.GetNSubQuotes()==1){
						xmlQuote* x2=xml.GetSubQuote(0);
						int idx=ELP->BaseParent->ExpandWith(x2->GetQuoteName(),ELP->BaseParent);
						if(idx!=-1){
                            BaseClass* elm=ELP->BaseParent->GetElementClass(idx);
							void* eData=ELP->BaseParent->GetElementPtr(idx,ELP->BaseParent);
							void* eExtra=ELP->BaseParent->GetElementExtraPtr(idx);
							ErrorPager ep;
							ELP->BC->Load(*x2,eData,&ep,eExtra);
							ExpInfChanged=1;
						}else{
							ErrorPager ep(1);
							ELP->BC->Load(*x2,ELP->DataPtr,&ep,ELP->ExtraPtr);
							ExpInfChanged=1;
						}
					}
				}else
				if(!strcmp(xml.GetQuoteName(),ELP->BC->GetClassName())){
					if(xml.GetNSubQuotes()==1&&ELP->BC->CheckDirectCasting()){
						xmlQuote* x2=xml.GetSubQuote(0);
						BaseClass* BCL=(BaseClass*)ELP->DataPtr;
						int idx=BCL->ExpandWith(x2->GetQuoteName(),BCL);
						if(idx!=-1){
							BaseClass* elm=BCL->GetElementClass(idx);
							void* eData=BCL->GetElementPtr(idx,BCL);
							void* eExtra=BCL->GetElementExtraPtr(idx);
							ErrorPager ep;
							((BaseClass*)eData)->Load(*x2,eData,&ep,eExtra);
							ExpInfChanged=1;
						}
					}
				}
			}
		}
	}
	if(ExpInfChanged){
		void* celm=NULL;
		if(CurElement!=-1){
			celm=ElParams[CurElement]->DataPtr;
		}
		int vs=0;
		int vsm=0;
		try{
			if(MainScroller){
				vs=MainScroller->SPos;
				vsm=MainScroller->SMaxPos;
			}
		}catch(...){
			MainScroller=NULL;
		}
		ClearAll();
        CreateFromClass(Frame,_x,_y,_x1,_y1,_EdClass,_Opt,Border);
		ExpInfChanged=0;
		if(MainScroller){
			MainScroller->SPos=vs; //*MainScroller->SMaxPos/vsm
			//MainScroller->SMaxPos=vsm;
		}
		if(celm){
			for(int i=0;i<ElParams.GetAmount();i++){
				if(ElParams[i]->DataPtr==celm){
					ElParams[i]->NowActive=1;
					CurElement=i;
					ElParams[i]->CBar->Diffuse=CED_SelFieldColor;	                    
				}
			}
		}
	}
	if(MainScroller){
        try{
    		if(!MainScroller->Visible||MainScroller->SMaxPos==1)MainScroller->SPos=0;
        }catch(...){
            MainScroller=NULL;            
        }
    }
	xmlQuote xml;
	ErrorPager ep;
	for(int i=0;i<ElementEditors.GetAmount();i++){
        OneElementEditor* OEE=ElementEditors[i];
		ElementParam* ELP=ElParams[i];
		if(OEE){
			BOOL Changed=0;                
			OneClassStorage* OCS=ELP->BaseParent->GetClassStorage();
			if(ELP->BC)ELP->BC->ProcessInEditor(ELP->BaseParent);
			if(!OEE->Process()){
				if(OCS->Members.GetAmount()){
					OneClassMemberStorage* OCMS=OCS->Members[ELP->BaseParentIndex];
					BaseClass* BASE=OCMS->Member;
					if(OCMS->IntPropertyR){						
						int x=OCMS->IntPropertyR((BaseClass*)ELP->BaseParent);
						DWORD HV=OEE->GetHashValue(ELP->BC,&x,ELP->ExtraPtr);
						if(HV!=ELP->XMLHash){
							ELP->OEE->Assign(ELP->BC,&x,ELP->ExtraPtr);
							ELP->XMLHash=HV;
							Changed=1;
						}else{
							int r=OEE->Get(ELP->BC,&x,ELP->ExtraPtr);
							if(r==2){
								ExpInfChanged=1;
							}
							if(OCMS->IntPropertyW){								
								OCMS->IntPropertyW((BaseClass*)ELP->BaseParent,x);
								ELP->XMLHash=OEE->GetHashValue(ELP->BC,&x,ELP->ExtraPtr);
							}
						}
					}else
					if(OCMS->GeneralPropertyR){
						void* prop=OCMS->GeneralPropertyR(ELP->BaseParent);
						DWORD HV=OEE->GetHashValue(ELP->BC,prop,ELP->ExtraPtr);
						if(HV!=ELP->XMLHash){
							ELP->OEE->Assign(ELP->BC,prop,ELP->ExtraPtr);
							ELP->XMLHash=HV;
							Changed=1;
						}else{
							int r=OEE->Get(ELP->BC,prop,ELP->ExtraPtr);
							if(r==2){
								ExpInfChanged=1;
							}
							if(OCMS->GeneralPropertyW){									
								OCMS->GeneralPropertyW(ELP->BaseParent,prop);
								ELP->XMLHash=OEE->GetHashValue(ELP->BC,prop,ELP->ExtraPtr);
							}
						}
					}else{
						DWORD HV=OEE->GetHashValue(ELP->BC,ELP->DataPtr,ELP->ExtraPtr);
						if(HV!=ELP->XMLHash){
							ELP->OEE->Assign(ELP->BC,ELP->DataPtr,ELP->ExtraPtr);
							ELP->XMLHash=HV;
							Changed=1;
						}else{
							int r=OEE->Get(ELP->BC,ELP->DataPtr,ELP->ExtraPtr);
							DWORD HV=OEE->GetHashValue(ELP->BC,ELP->DataPtr,ELP->ExtraPtr);
							if(HV!=ELP->XMLHash)Changed=true;
							ELP->XMLHash=HV;
							if(r==2){
								ExpInfChanged=1;
							}
						}						
					}
				}else{			
					DWORD HV=OEE->GetHashValue(ELP->BC,ELP->DataPtr,ELP->ExtraPtr);
					if(HV!=ELP->XMLHash){
						ELP->OEE->Assign(ELP->BC,ELP->DataPtr,ELP->ExtraPtr);
						ELP->XMLHash=HV;
						Changed=1;
					}else{	
						int r=OEE->Get(ELP->BC,ELP->DataPtr,ELP->ExtraPtr);
						DWORD HV=OEE->GetHashValue(ELP->BC,ELP->DataPtr,ELP->ExtraPtr);
						if(HV!=ELP->XMLHash)Changed=true;
						ELP->XMLHash=HV;
						if(r==2){
							ExpInfChanged=1;
						}
					}
				}				
			}else{
				DWORD HV=OEE->GetHashValue(ELP->BC,ELP->DataPtr,ELP->ExtraPtr);
				if(HV!=ELP->XMLHash){
					Changed=ELP->XMLHash!=0;
					ELP->XMLHash=HV;
				}
			}
			if(Changed){
				if(ELP->BaseParent){
					if(ELP->BaseParent->OnChangeMember(ELP->BC,ELP->DataPtr,ELP->ExtraPtr,ELP->FieldName.pchar()))ExpInfChanged=true;
					if(ELP->BC->CheckDirectCasting()){
						BaseClass* B1=(BaseClass*)ELP->DataPtr;
						if(B1->GetAmountOfElements()==0){
                            if(B1->OnChangeMember(B1,ELP->DataPtr,ELP->ExtraPtr,ELP->FieldName.pchar()))ExpInfChanged=true;
						}
					}
				}
			}
			if(ELP->BaseParent){
				if(ELP->BaseParent->GetClassMask()!=ELP->ParentClassMask){
					ExpInfChanged=true;
				}
			}
		}
	}
	/*
	ElementParam* SEP=NULL;
	if(CurElement!=-1){
		SEP=ElParams[CurElement];
		if(SEP->BC&&SEP->BC->CheckDirectCasting()){
			for(int i=ExpInf.GetAmount()-1;i>=0;i--){
				BaseClass* BC=(BaseClass*)ExpInf[i];
				bool have=0;
				if(BC){
					try{
						while(BC&&!have){
							if(abs(int(BC)-int(SEP->DataPtr))<16)have=1;
							BC=BC->GetParent();
						}
					}catch(...){
						have=1;
					};
					BC=(BaseClass*)ExpInf[i];
					BaseClass* BC2=(BaseClass*)SEP->DataPtr;
					try{
						while(BC2&&!have){
							if(abs(int(BC)-int(BC2))<16)have=1;
							BC2=BC2->GetParent();
						}
					}catch(...){
						have=1;
					};
					if(!have){
						ExpInf.Del(i,1);
						ExpInfChanged=true;
					}
				}
			}
		}
	}
	*/
	if(ExpInfChanged){
		Process();
		ExpInfChanged=0;
	}
}
ClassEditor::ClassEditor(){
    CurElement=-1;
	Frame=NULL;
	Base=NULL;
}
ClassEditor::~ClassEditor(){
	ClearAll();
	CurExpInf=NULL;
}
void* ClassEditor::GetCurrentSelectedPtr(){
	if(CurElement!=-1){
		return ElParams[CurElement]->DataPtr;
	}else return NULL;
}
BaseClass* ClassEditor::GetCurrentClass(){
	if(CurElement!=-1){
		return ElParams[CurElement]->BC;
	}else return NULL;
}
const char* ClassEditor::GetCurrentFieldName(){
	if(CurElement!=-1){
		return ElParams[CurElement]->FieldName.pchar();
	}else return NULL;
}
void ClassEditor::ClearAll(){
	if(!Frame)return;
	CurElement=-1;	
	for(int i=0;i<ElParams.GetAmount();i++)if(ElParams[i]){
		delete(ElParams[i]);
		ElParams[i]=NULL;
	}
	ElParams.Clear();
	for(i=0;i<ElementEditors.GetAmount();i++)if(ElementEditors[i]){
		delete ElementEditors[i];
		ElementEditors[i]=NULL;
	}
	ElementEditors.Clear();
	if(Base)Base->CloseDialogs();
	//Frame->CloseDialogs();
	//Frame=NULL;
	Base=NULL;
}
bool te_TextureClick(SimpleDialog* SD){
	_gt_Param* _gtp=(_gt_Param*)SD->AllocPtr;
	if(_gtp){
		_gtp->_gt->texidx=_gtp->tindex;
		xmlQuote xml;
        int r=_gtp->_gt->Get(&xml);
		if(r==2){
			_gtp->_gt->CE->ExpInfChanged=1;
		}
		_gtp->_gt->Assign(&xml);
		_gtp->DD->Visible=0;
		ParentFrame::OnlyActiveParent=NULL;
	}
	return true;
}
extern int RealLx;
extern int RealLy;
bool VisControl(SimpleDialog* SD){
	DialogsDesk* SD1=(DialogsDesk*)SD->UserParam;
	SD1->Visible=1;
	SD1->MouseOver=1;
	ParentFrame::OnlyActiveParent=SD1;
	return true;
}
//int _graph2d::TextureID=-1;
DynArray<int> DynTexManager::TexList;
int DynTexManager::NTX=0;
DynArray<BasePaintMap*> Maps;
MapsScope* MLayers=NULL;
void SaveMLayersToFileInCompactForm(char* filename){
	if(!MLayers)return;
	ResFile F=RRewrite(filename);
	if(F!=INVALID_HANDLE_VALUE){
		int v='1RLM';
		RBlockWrite(F,&v,4);
		v=MLayers->Maps.GetAmount();
		RBlockWrite(F,&v,4);
		for(int i=0;i<MLayers->Maps.GetAmount();i++){
			GreyMap* BPM=(GreyMap*)MLayers->Maps[i];
			char* name=MLayers->MapsNames[i]->pchar();			
			v=strlen(name);
			RBlockWrite(F,&v,4);
			RBlockWrite(F,name,v+1);
			name=MLayers->Maps[i]->Desc.pchar();
			v=strlen(name);
			RBlockWrite(F,&v,4);
			RBlockWrite(F,name,v+1);
			int lx,ly;
			BPM->GetSize(lx,ly);
			bool something=false;
			int sz=BPM->V.GetAmount();
			for(int i=0;i<sz;i++)if(BPM->V[i])something=true;
			if(something){
				RBlockWrite(F,&lx,4);
				RBlockWrite(F,&ly,4);
				int NC=BPM->GetPaletteSize();
				RBlockWrite(F,&NC,4);
				for(int i=0;i<NC;i++){
					DWORD C=BPM->GetPaletteColor(i);
					RBlockWrite(F,&C,4);
				}
				int sz=lx*ly/8;			
				RBlockWrite(F,BPM->V.GetValues(),sz);
			}else{
				lx=0;
				RBlockWrite(F,&lx,4);
				RBlockWrite(F,&lx,4);
				int NC=BPM->GetPaletteSize();
				RBlockWrite(F,&NC,4);
				for(int i=0;i<NC;i++){
					DWORD C=BPM->GetPaletteColor(i);
					RBlockWrite(F,&C,4);
				}
			}
		}
        RClose(F);                
	}
}
bool LoadMLayersFromFileInCompactForm(char* filename){
	ResFile F=RReset(filename);	
	if(F!=INVALID_HANDLE_VALUE){
		MLayers->Clear();
		int magix;
		int v;
		int NL;
		RBlockRead(F,&magix,4);		
		RBlockRead(F,&NL,4);
		for(int i=0;i<NL;i++){
			GreyMap* BPM=new GreyMap;
			MLayers->Maps.Add(BPM);	
			char name[512];			
			RBlockRead(F,&v,4);
			RBlockRead(F,name,v+1);
			_str* s=new _str;
			*s=name;
			MLayers->MapsNames.Add(s);
			RBlockRead(F,&v,4);
			RBlockRead(F,name,v+1);
			BPM->Desc=name;
			int lx,ly;			
			RBlockRead(F,&lx,4);
			RBlockRead(F,&ly,4);
			bool empty=lx==0;
			if(lx==0){
				lx=512;
				ly=512;
			}
			if(lx!=BPM->Lx || ly!=BPM->Ly){
				BPM->Free();
				BPM->SetSize(lx,ly);
			}
			int NC;
			RBlockRead(F,&NC,4);
			for(int i=0;i<NC;i++){				
				DWORD C;
				RBlockRead(F,&C,4);
                BPM->AddPalColor(C);
			}
			int sz=lx*ly;
			if(magix=='YALM'){
				byte* p=new byte[sz];
                RBlockRead(F,p,sz);
				for(int i=0;i<lx;i++){
					for(int j=0;j<ly;j++){
                        BPM->SetGreyScale(i,j,p[i+j*lx]);
					}
				}
				delete[]p;
			}else{
				if(!empty)RBlockRead(F,BPM->V.GetValues(),sz/8);
			}
			BPM->CreateTexturePart(0,0,lx,ly);
		}
		RClose(F); 
		void TestLayersColors();
		TestLayersColors();	
		return true;
	}
	return false;
}
DIALOGS_API void DrawCircle(float x,float y,float r,DWORD Color){
	int Np=r*20;
	float xp,yp;
	for(int i=0;i<=Np;i++){
		float a=3.1415f*2.0f*i/Np;
		float xx=x+(r*cos(a));
		float yy=y+(r*sin(a));
		if(i){
			GPS.DrawLine(xx,yy,xp,yp,Color);
		}
		xp=xx;
		yp=yy;
	}
}
void DrawCircle2(float x,float y,float r,float r2,DWORD Color){
	int Np=r*20;
	float xp,yp;
	for(int i=0;i<=Np;i++){
		float a=3.1415f*2.0f*i/Np;
		float xx=x+(r*cos(a));
		float yy=y+(r2*sin(a));
		if(i){
			GPS.DrawLine(xx,yy,xp,yp,Color);
		}
		xp=xx;
		yp=yy;
	}
}
bool MMItemChoose(SimpleDialog* SD);
//extern int ItemChoose;
extern int GameMode;
bool ProcessLoadSaveOld(int x,int y,int Lx,int Ly,_str& str,const char* mask,const char* StartDir,bool Load,ParentFrame* Back){	
	Lpressed=0;
	//UnPress();
	//y=0;
	if(y==0)Ly=RealLy-y;

	FilePreview* FPV=GetPreviewer(mask+1);
	int PR_Ly=0;
	if(FPV)PR_Ly=FPV->EstimateSize(Ly);

	testFilePathEditor TPZ(StartDir,mask);
	ClassEditor CE;
	DialogsSystem GDS;
	GDS.CloseDialogs();
	DialogsDesk* DD=GDS.AddDialogsDesk(x,y,Lx,Ly,"StandartBorderNotScrollable");
	//ColoredBar* CB3=DD->addColoredBar(0,Ly,Lx,50,0xFF);
	//CB3->Diffuse=CED_BackColor;
	ColoredBar* CB4=DD->addColoredBar(0,Ly-PR_Ly,Lx,PR_Ly,0xFF);
	CB4->Diffuse=CED_BackColor-0x00300030;
	CE.CreateFromClass(DD,0,0,Lx,Ly-PR_Ly,&TPZ,3,"StandartBorder");
	if(CE.ExpInf.find(&TPZ.FILEPATH)==-1){
		CE.ExpInf.Add(&TPZ.FILEPATH);
	}
	void ProcessScreen();
	extern int LastKey;
	LastKey=0;
	extern byte MI_Mode;	
	int NBTN=3;
	int BTDY=30;
	RLCFont* AF=&WhiteFont;
	RLCFont* PF=&YellowFont;
	//const char* msgs1[]={"Load","Cancel",""};
	//const char* msgs2[]={"Save","Cancel",""};
	//const char** msgs=Load?msgs1:msgs2;
	//TextButton* TBS[6];
	//int Tl=0;
	//for(int i=1;i<NBTN;i++){
	//	Tl+=GetRLCStrWidth((char*)msgs[i-1],AF);
	//}
	//Tl=Lx-Tl;
	//Tl/=NBTN;
	//int xx=Tl;
	//for(int i=1;i<NBTN;i++){
	//	TextButton* TB=DD->addTextButton(NULL,xx,Ly-BTDY,(char*)msgs[i-1],AF,PF,PF,0);
	//	xx+=Tl+GetRLCStrWidth((char*)msgs[i-1],AF);
	//	TB->OnUserClick=&MMItemChoose;
	//	TB->UserParam=i;
	//}
	int idx=GPS.PreLoadGPImage("Interf3\\btnz");
	int pidx=Load?0:2;
	GP_Button* OK=GDS.addGP_Button(NULL,x+Lx-54-26,y+3,idx,pidx+1,pidx);
	OK->UserParam=1;
	OK->OnUserClick=&MMItemChoose;
	GP_Button* CANCEL=GDS.addGP_Button(NULL,x+Lx-54,y+3,idx,9,8);
	CANCEL->UserParam=2;
	CANCEL->OnUserClick=&MMItemChoose;

	ItemChoose=-1;
	do{
		if(bActive){
			if(GameMode!=1)ProcessScreen();
			else IRS->ClearDevice();
		}
		ProcessMessages();
		if(bActive){
			bool LPR=Lpressed;
			Lpressed=0;
			if(Back){
				for(int i=0;i<Back->DSS.GetAmount();i++){
					if(Back->DSS[i]){
						Back->DSS[i]->ShiftDialog(Back->x,Back->y);
						SimpleDialog* S1;
						SimpleDialog* S2;
						SimpleDialog* S3;
						Back->DSS[i]->Process(&S1,&S2,&S3);
						Back->DSS[i]->ShiftDialog(-Back->x,-Back->y);
					}

				}				
			}
			Lpressed=LPR;			
			CE.Process();
			GDS.ProcessDialogs();						
			if(FPV){
				TPZ.CheckName();
				FPV->DrawPreview(x+1,y+Ly-PR_Ly+10,x+Lx-1,y+Ly-1,TPZ.FILEPATH.pchar());
			}
			GDS.RefreshView();		
			if(ItemChoose==1||LastKey==13){//load/save
				TPZ.CheckName();
				str=TPZ.FILEPATH.pchar();
				if(FPV)delete(FPV);
				LastKey=0;
				ItemChoose=-1;
				LastKey=0;
				return true;
			}else if(ItemChoose==2||LastKey==27){
				TPZ.CheckName();
				str=TPZ.FILEPATH.pchar();
				if(FPV)delete(FPV);
				LastKey=0;
				ItemChoose=-1;
				LastKey=0;
				return false;
			}		
			ItemChoose=-1;
		}
	}while(true);	
	if(FPV)delete(FPV);
	return false;
}
bool ProcessLoadSave(int x,int y,int Lx,int Ly,_str& str,const char* mask,const char* StartDir,bool Load,ParentFrame* Back){
	if(y==0)Ly=RealLy-y;
	ceFileDialog FD;
	char* s=NULL;
	char s2[512];
	strcpy(s2,mask);
	char* fs=s2;
	do{
		s=strtok(fs,",");
		if(s){
			fs=NULL;
			_str* str1=new _str;
			if(s[0]=='*')*str1=s+1;
			else *str1=s;			
			FD.FileType.Add(str1);
		}
	}while(s);
	s=NULL;
	strcpy(s2,StartDir);
	fs=s2;
	int L=strlen(fs);
	if(!(fs[L-1]=='\\'||fs[L-1]=='/')){
        L--;		
		while(L>=0&&fs[L]!='\\'&&fs[L]!='/')L--;
		if(L!=0){
            FD.FileName=fs+L+1;
			fs[L+1]=0;
		}
	}
	do{
		s=strtok(fs,"\\/");
		if(s&&s[0]){
			_str* str1=new _str;
			*str1=s;
			FD.Pass.Add(str1);
			fs=NULL;
		}
	}while(s);
	FD.LoadDialogStyle=Load;
	bool r=FD.CreateDialogEditor(x,y,x+Lx-1,y+Ly-1);
	str=FD.FullFileName;
	return r;
}
bool LS_gp_UserClick(SimpleDialog* SD){
	LS_gp_param* LS=(LS_gp_param*)SD->AllocPtr;		
	_str str;
	str="";
	int x0=0;
	int y0=0;
	ParentFrame* P0=LS->CE->Frame;
	ParentFrame* P00=P0;
	while(P0){
		x0+=P0->x;
		y0+=P0->y;
		P00=P0;
		P0=P0->ParentDS;
	}
	if(ProcessLoadSave(x0,y0,LS->CE->_x1-LS->CE->_x+1,LS->CE->_y1-LS->CE->_y+1,str,LS->ext,LS->startdir,1)){
		strcpy(LS->destname,str.pchar());
		char* s=strstr(LS->destname,".G17");
		if(s)s[0]=0;
		s=strstr(LS->destname,".G2D");
		if(s)s[0]=0;	
		s=strstr(LS->destname,".GP2");
		if(s)s[0]=0;	
		s=strstr(LS->destname,".G16");
		if(s)s[0]=0;	
		s=strstr(LS->destname,".g17");
		if(s)s[0]=0;
		s=strstr(LS->destname,".g2d");
		if(s)s[0]=0;	
		s=strstr(LS->destname,".gp2");
		if(s)s[0]=0;	
		s=strstr(LS->destname,".g16");
		if(s)s[0]=0;	
	}
	return true;
}
int _byte_index_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
	BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo2");
	Box=Base->addGP_ComboBoxDLX(NULL,x,y,x1-x+1,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
	VScrollBar* VS;
	Box->AssignScroll((DialogsSystem*)Base,&VS,3,0,8);
	if(ExtraPtr){
		Enumerator* E=(Enumerator*)ExtraPtr;
		//Box->AddLine("--empty--");
		Box->AutoID=E->EnumeratorID;
		for(int i=0;i<E->GetAmount();i++){
			Box->AddLine(E->GetStr(i));
			//int SZ=sizeof(*E->Strings.SHash.pool);
		}
	}
	return Box->y1;
}
int _font_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
	BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo2");
	Box=Base->addGP_ComboBoxDLX(NULL,x,y,x1-x+1,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
	VScrollBar* VS;
	Box->AssignScroll((DialogsSystem*)Base,&VS,3,0,8);
	Enumerator* E=ENUM.Get("FONTS");
	if(E){
		for(int i=0;i<E->GetAmount();i++){
			Box->AddLine(E->GetStr(i));
		}
	}
	return Box->y1;
}
int _index_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
	BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){		

	if(ExtraPtr){
		static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo2");
		E=(Enumerator*)ExtraPtr;
		int N=E->GetAmount();
		bool ctg=false;
		for(int i=0;i<N;i++)if(E->GetCategory(i))ctg=true;
		if(ctg){
			Topics=Base->addGP_ComboBoxDLX(NULL,x,y,x1-x+1,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
			VScrollBar* VSC;
			Topics->AssignScroll((DialogsSystem*)Base,&VSC,3,0,8);							

			Box=Base->addGP_ComboBoxDLX(NULL,x,Topics->y1,x1-x+1,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
			VScrollBar* VS;
			Box->AssignScroll((DialogsSystem*)Base,&VS,3,0,8);				
			Box->AutoID=E->EnumeratorID;

			return Box->y1;
		}else{
			Box=Base->addGP_ComboBoxDLX(NULL,x,y,x1-x+1,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
			VScrollBar* VS;
			Box->AssignScroll((DialogsSystem*)Base,&VS,3,0,8);				
			Box->AutoID=E->EnumeratorID;
			for(int i=0;i<E->GetAmount();i++){
				Box->AddLine(E->GetStr(i));
			}
			y1=Box->y1;
		}
	}
	return y1;
}
bool _index_editor::Assign(xmlQuote* xml){
	const char* s=xml->Get_string();
	if(!s)s="";
	//if(s){
		if(Topics){
			if(E->GetAmount()){
				int idx=E->FindStr((char*)s);
				if(idx==-1)idx=0;
				char top[256]="";
				char* t=E->GetCategory(idx);
				if(t)strcpy(top,t);
				if(Topics->NLines && !Topics->TestMode)strcpy(top,Topics->Lines[Topics->CurLine]);
				Topics->Clear();
				Box->Clear();
				int n=E->GetAmount();
				for(int j=0;j<n;j++){
                    char* top1=E->GetCategory(j);
					if(!top1)top1="";
					if(!strcmp(top,top1)){
                        Box->AddLine(E->GetStr(j));
						if(!strcmp(E->GetStr(j),s))Box->CurLine=Box->NLines-1;
					}
					bool fnd=false;
					for(int j=0;j<Topics->NLines;j++)if(!strcmp(Topics->Lines[j],top1)){
						fnd=true;
						break;
					}
					if(!fnd){
                        Topics->AddLine(top1);
						if(!strcmp(top1,top))Topics->CurLine=Topics->NLines-1;
					}
				}
			}
		}else{
			for(int i=0;i<Box->NLines;i++)if(!strcmp(Box->Lines[i],xml->Get_string())){
				Box->CurLine=i;
			}
		}
	//}
	return true;
}
int _index_editor::Get(xmlQuote* xml){	
	if(Topics&&Topics->Changed){
		char s[512];
		strcpy(s,Topics->Lines[Topics->CurLine]);
		for(int j=0;j<E->GetAmount();j++){
            char* t1=E->GetCategory(j);
			if(!t1)t1="";
			if(!strcmp(t1,s)){
				xml->Assign_string(E->GetStr(j));
				Assign(xml);
			}
		}
		Topics->Changed=false;
	}else{
		if(Box){
			if(!Box->Changed)return false;
			Box->Changed=0;
			if(Box->Lines){
				xml->Assign_string(Box->Lines[Box->CurLine]);
			}else xml->Assign_string("");
		}
	}
	return true;
}
void  _ClassIndex_editor::Assign		(BaseClass* BC,void* Ptr,void* ExtraPtr){
	_ClassIndex* CI=(_ClassIndex*)Ptr;
	char ElemName[128];
	char ArrayName[128];
	sscanf((char*)ExtraPtr,"%s%s",ArrayName,ElemName);
	int ElemID=-1;
	if(CI&&CI->pchar()){
		BaseClass* ElemArray = CI->getARRAY((char*)ExtraPtr);
		if (ElemArray!=NULL){
			int N=ElemArray->GetAmountOfElements();
							
			for(int i=0;(i<N)&&(ElemID==-1);i++){
				BaseClass* ClassType=ElemArray->GetElementClass(i);
				void* ElmData=ElemArray->GetElementPtr(i,ElemArray);
				void* Extra=ElemArray->GetElementExtraPtr(i);
				
				BaseClass* E_Class=NULL;
				void* E_Data;
				void* E_Extra;
				if(ClassType->GetElementByName(ElemName,&E_Class,&E_Data,&E_Extra,ClassType)){
					xmlQuote xml;
					E_Class->Save(xml,E_Data,E_Extra);
					const char* s=xml.Get_string();
					
					if (s!=NULL && !strcmp( s, CI->pchar() )) ElemID=i;
				}

			};

		};
	};
	if (ElemID!=-1) Box->CurLine=ElemID;
	else			Box->CurLine=0;
}
int   _ClassIndex_editor::Get			(BaseClass* BC,void* Ptr,void* ExtraPtr){
	_ClassIndex* CI=(_ClassIndex*)Ptr;
	if(Box->NLines){
		_str nameID;
		nameID = Box->Lines[Box->CurLine];
		( *( (_str*)CI ) ) = nameID.pchar();
//		CI->INDEX=-1;
//		CI->NUMBER=-1;
	}
	return 0;
}
DWORD _ClassIndex_editor::GetHashValue	(BaseClass* BC,void* Ptr,void* ExtraPtr){
	_ClassIndex* CI=(_ClassIndex*)Ptr;
	return CI->pchar()?0:GetSHash(CI->pchar());
}
int _variant_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
								   BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo2");
	Box=Base->addGP_ComboBoxDLX(NULL,x,y,x1-x+1,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
	VScrollBar* VS;
	Box->AssignScroll((DialogsSystem*)Base,&VS,3,0,8);
	_variant* V=dynamic_cast<_variant*>(Class);
	if(V){
        ClonesArray<_str> List;
		V->GetVariants(List);
		Box->AddLine("--empty--");		
		for(int i=0;i<List.GetAmount();i++){
			Box->AddLine(List[i]->pchar());
		}
	}
	return Box->y1;
};
int _ClassIndex_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo2");
	Box=Base->addGP_ComboBoxDLX(NULL,x,y,x1-x+1,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
	VScrollBar* VS;
	Box->AssignScroll((DialogsSystem*)Base,&VS,3,0,8);

	BaseClass* Owner=NULL;
	//Owner is a pointer to the class that contains _ClassIndex
    char* opt=(char*)ExtraPtr;
	//temp
	//Box->AddLine(opt);
	Box->Clear();
	//----vitya should obtain it----//
	BaseClass* ClassArrayPtr=NULL;
	char ElemName[128];
	char ArrayName[128];
	sscanf(opt,"%s%s",ArrayName,ElemName);
		
	if (strcmp(Class->GetClassName(),"_ClassIndex")==0){
//		_ClassIndex* CI=(_ClassIndex*)(CParam->ELP->BaseParent);
		_ClassIndex* CI=(_ClassIndex*)(DataPtr);
		if (CI!=NULL) ClassArrayPtr=CI->getARRAY(ArrayName);
	};

	//find class array
	if(ClassArrayPtr&&ElemName){
		int N=ClassArrayPtr->GetAmountOfElements();
		for(int i=0;i<N;i++){
			BaseClass* ClassType=ClassArrayPtr->GetElementClass(i);
			void* ElmData=ClassArrayPtr->GetElementPtr(i,ClassArrayPtr);
			void* Extra=ClassArrayPtr->GetElementExtraPtr(i);

			BaseClass* E_Class=NULL;
			void* E_Data;
			void* E_Extra;
			if(ClassType->GetElementByName(ElemName,&E_Class,&E_Data,&E_Extra,ClassType)){
				xmlQuote xml;
				E_Class->Save(xml,E_Data,E_Extra);
				const char* s=xml.Get_string();
				if(s){
					Box->AddLine((char*)s);
				}
			}
		}
	}

	return Box->y1;
};
bool ClassRef_OnComboSelect(SimpleDialog* SD){
	ClassRef_editor* CRE=(ClassRef_editor*)SD->UserParam;
    ComboBox* CB=(ComboBox*)SD;
	ClassRef<BaseClass>* CR=(ClassRef<BaseClass>*)CRE->cptr;
	CPGARB.DelRef(CR->CPG_Index);
	CR->CPG_Index=CB->CurLine>0?CRE->PIDS[CB->CurLine-1]:0xFFFFFFFF;
	CPGARB.AddRef(CR->CPG_Index);
	return true;
}
struct ref_sort{
	int pid;
	char* name;
	bool cur;
};
int ref_sort_compare( const void *arg1, const void *arg2 ){
	ref_sort* v1=(ref_sort*)arg1;
	ref_sort* v2=(ref_sort*)arg2;
	return stricmp(v1->name,v2->name);
}

int ClassRef_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
	BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){

	static int CBB_GPFILE=GPS.PreLoadGPImage("Interf3\\elements\\Combo2");
	List=Base->addGP_ComboBoxDLX(NULL,x,y,x1-x+1,CBB_GPFILE,0,9,0,&CED_BFont,&CED_BFont,NULL);
	VScrollBar* VS;
	List->AssignScroll((DialogsSystem*)Base,&VS,3,0,8);
	char* s1=((char*)Class->GetClassName())+9;
	List->UserParam=(int)this;
	List->OnSelect=&ClassRef_OnComboSelect;
	List->AddLine("--empty--");
	PIDS.Clear();
	cptr=DataPtr;
	DynArray<ref_sort> Elms;
	ClassRef<BaseClass>* P=(ClassRef<BaseClass>*)DataPtr;
	BaseClass* ROOT=Class->CheckDirectCasting()?((BaseClass*)DataPtr)->GetRoot():NULL;
	if(ROOT){
		for(int j=0;j<nCP_Branches;j++){
			int N=CPGARB.CPointer[j].GetAmount();
			for(int i=0;i<N;i++){
				OneClassPointer* CP=CPGARB.CPointer[j][i];
				try{
					if(CP->ClassPtr&&CP->NamePtr&&CP->NamePtr->pchar()){
						OneClassStorage* OCS=CP->ClassPtr->GetClassStorage();
						if(OCS){
							bool unf=strcmp(CP->ClassPtr->GetClassName(),s1);
							if(unf){
								for(int j=0;j<OCS->Parents.GetAmount()&&unf;j++){
									unf=strcmp(OCS->Parents[j],s1);
								}
							}
							if(!unf){
								BaseClass* R1=CP->ClassPtr->GetRoot();
								if(CP->ClassPtr->CheckIfObjectIsGlobal()||R1==ROOT){
									if(CP->NamePtr->pchar()&&CP->NamePtr->pchar()[0]){
										ref_sort rs;
										rs.pid=CP->Index;
										rs.name=CP->NamePtr->pchar();
										if(P->Get())rs.cur=!strcmp(rs.name,P->GetObjectName());//CP->ClassPtr==P->Get();
										else rs.cur=false;
										Elms.Add(rs);
									}
								}
							}
						}
					}
				}catch(...){};						
			}
		}
		qsort(Elms.Values,Elms.GetAmount(),sizeof(ref_sort),ref_sort_compare);
		for(int i=0;i<Elms.GetAmount();i++){
			List->AddLine(Elms[i].name);
			PIDS.Add(Elms[i].pid);
			if(Elms[i].cur)List->CurLine=List->NLines-1;
		}			
	}
	return List->y1;
}
void ComplexClassEditor::Process(){
	//checking if the current element exists
	bool found=0;
	for(int i=0;i<Top.ElParams.GetAmount();i++){
		if(Top.ElParams[i]->DataPtr==(void*)Bottom._EdClass){
			found=true;
			break;
		}
	}
	if(BottomDS.DSS.GetAmount()&&!found){
		BottomDS.CloseDialogs();
		Bottom.Base=NULL;
		Bottom.ClearAll();
	}
	//try{
	Bottom.Process();
	//}catch(...){
	//	BottomDS.CloseDialogs();
	//	Bottom.ClearAll();
	//	Bottom.Base=NULL;
	//}
	Top.Process();			
	TopDS.ProcessDialogs();	
	//Top.Process();
	static void* PrevSelPtr=NULL;
	if(Top.CurElement!=-1){
		void* ptr=Top.ElParams[Top.CurElement]->DataPtr;
		if(ptr!=PrevSelPtr){
			PrevSelPtr=ptr;
			if(Top.ElParams[Top.CurElement]&&Top.ElParams[Top.CurElement]->BC&&Top.ElParams[Top.CurElement]->BC->CheckDirectCasting()){
				BottomDS.CloseDialogs();
				Bottom.Base=NULL;
				Bottom.ClearAll();				
				Bottom.CreateFromClass(&BottomDS,Bottom._x,Bottom._y,Bottom._x1,Bottom._y1,(BaseClass*)ptr,1,border.pchar());
			}
		}
	}
	if(!Top.ExpInfChanged){
		BottomDS.ProcessDialogs();
		//BottomDS.RefreshView();		
		Bottom.Process();
	}	
	if(Top.ExpInfChanged){
		Top.Process();
	}
}
//////////////////////////////////////////////////////////////////////////
bool FileElement::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra)
{
	if(OnClickSet)
	{
		OnClickSet->Clear();
		OnClickSet->Add(Way);
		OnClickSet->Add(this->pchar());
		xml.Assign_string(OnClickSet->pchar());
	}
	return true;
}
FileElement::FileElement(char* filename, char* way, DString* DS)
{
	Type=0;
	this->Add(filename);
	Way.Add(way);
	OnClickSet=DS;
}
const char* FileElement::GetClassName()
{
	return "_test";
}
bool DirectoryElement::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra)
{
	return true;
}

int DirectoryElement::GetAmountOfElements(){
	return FileArr.GetAmount();
}
const char* DirectoryElement::GetElementID(int Index){
	return FileArr[Index]->pchar();
}
BaseClass* DirectoryElement::GetElementClass(int Index){
	return FileArr[Index];
}
void* DirectoryElement::GetElementPtr(int Index,void* base)
{
	return FileArr[Index];
}

bool DirectoryElement::CheckDirectCasting(){
	return true;
}
DirectoryElement::DirectoryElement()
{
	Loaded=false;
}
DirectoryElement::DirectoryElement( char* DirName, char* Mas , char* way, DString* DS)
{
	Type=1;
	this->Add(DirName);
	Way.Add(way);
	Mask.Add(Mas);
	OnClickSet=DS;

	ReadDir();
	Loaded=true;
}

DirectoryElement::~DirectoryElement()
{
	int am=FileArr.GetAmount();
	for(int i=0;i<am;i++)
	{
		if(FileArr[i])
			delete (FileArr[i]);
	}
}
void DirectoryElement::ReadDir()
{
	Loaded=true;

	LPGSCFindInfo lpFindInfo=NULL;
	WIN32_FIND_DATA	FindData;
	HANDLE	hFindFile;
	LPGSCFileList	lpFileList=NULL;

	DString TM;
	TM.Add(Way);
	
	//read Dir
	DString TMD;
	TMD.Add(TM);
	TMD.Add("*");
	hFindFile=FindFirstFile(TMD.pchar(),&FindData);
	if(hFindFile!=INVALID_HANDLE_VALUE)
	{
		ReadDir(&FindData);
		while(FindNextFile(hFindFile,&FindData))
		{
			ReadDir(&FindData);	
		};
		FindClose(hFindFile);
	}
	//
	//read files
	TM.Add(Mask);
	hFindFile=FindFirstFile(TM.pchar(),&FindData);
	if(hFindFile!=INVALID_HANDLE_VALUE)
	{
		ReadFile(&FindData);
		while(FindNextFile(hFindFile,&FindData))
		{
			ReadFile(&FindData);	
		};
		FindClose(hFindFile);
	}
	//
	if(FileArr.GetAmount())
	{
		qsort(FileArr.GetValues(),FileArr.GetAmount(),sizeof(FileElement*),DirectoryElement::compare);
	}
}
int DirectoryElement::compare(const void* p1, const void* p2)
{
	int rez=0;
	FileElement** fe1 = (FileElement**)p1;
	FileElement** fe2 = (FileElement**)p2;
	if((*fe1)&&(*fe2))
	{
		if((*fe1)->Type==(*fe2)->Type)
		{
			const char* n1=(*fe1)->pchar();
			const char* n2=(*fe2)->pchar();
			return stricmp(n1,n2);
		}
		else
			if((*fe1)->Type>(*fe2)->Type)
				return -1;
			else
				return 1;
	}
	if((*fe1))
		rez++;
	if((*fe2))
		rez--;

	return rez;
}	
void DirectoryElement::ReadFile(WIN32_FIND_DATA* FD)
{
	if(strcmp(FD->cFileName,".")&&strcmp(FD->cFileName,".."))
	{
		if(!(FD->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
		{
			FileArr.Add(new FileElement(FD->cFileName, Way.pchar(),OnClickSet));
		}
	}
}
void DirectoryElement::ReadDir(WIN32_FIND_DATA* FD)
{
	if(strcmp(FD->cFileName,".")&&strcmp(FD->cFileName,".."))
	{
		if(FD->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			DString NP;
			NP.Add(Way);
			NP.Add(FD->cFileName);
			NP.Add("\\");
			FileArr.Add(new DirectoryElement(FD->cFileName, Mask.pchar(), NP.pchar(),OnClickSet));
		}
	}
}

FilePathEditor::FilePathEditor()
{
	OnClickSet=(DString*)this;
}
FilePathEditor::FilePathEditor(char* RootDir, char* Maska)
{
	Type=2;

	this->Add("xml source");
	Way.Add(RootDir);
	Mask.Add(Maska);
	OnClickSet=(DString*)this;

	ReadDir();

	Loaded=true;
}
const char* FilePathEditor::GetClassName()
{
	return "FilePathEditor";
}

const char* FilePathEditor_editor::GetClassID(){return "FilePathEditor";}
int FilePathEditor_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
		BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	str=(_str*)DataPtr;
	CParam->MinLeftWidth=64;
	CParam->RightColumnWidth+=CParam->LeftColumnWidth-CParam->MinLeftWidth;
	x-=CParam->LeftColumnWidth-CParam->MinLeftWidth;
	CParam->LeftColumnWidth=CParam->MinLeftWidth;			
	IB=Base->addInputBox(NULL,x,y,str,120,x1-x+1,y1-y+1,&CED_Font,&CED_AFont,0);
	return y1;
}
bool FilePathEditor_editor::Assign(xmlQuote* xml){
	if(xml->Get_string())*str=xml->Get_string();
	else str->Clear();
	return true;
}
int FilePathEditor_editor::Get(xmlQuote* xml){
	xml->Assign_string(str->pchar());
	return true;
}
OneElementEditor* FilePathEditor_editor::_new(){
	return (OneElementEditor*)new FilePathEditor_editor;
}
testFilePathEditor::testFilePathEditor()
{
	FILEPATH.Add("");
	FILEPATH.Way.Add("Missions\\");
	FILEPATH.Mask.Add("*.*");
	FILEPATH.ReadDir();
}
testFilePathEditor::testFilePathEditor(const char* path,const char* mask)
{
	FILEPATH.Add("");
	FILEPATH.Way.Add((char*)path);
	FILEPATH.Mask.Add((char*)mask);
	FILEPATH.ReadDir();
}
void testFilePathEditor::CheckName(){
	strupr(FILEPATH.Way.pchar());
	strupr(FILEPATH.pchar());
	strupr(FILEPATH.Mask.pchar());
	if(!strstr(FILEPATH.pchar(),FILEPATH.Way.pchar())){
		_str s1;
		s1=FILEPATH.Way.pchar();
		s1+=FILEPATH.pchar();
		FILEPATH.Assign(s1.pchar());			
	}		
	if(!strstr(FILEPATH.pchar(),FILEPATH.Mask.pchar()+1)){
		FILEPATH+=FILEPATH.Mask.pchar()+1;
	}
}
bool SimpleDialog_editor::CheckClass(BaseClass* BC){
	SimpleDialog* S=dynamic_cast<SimpleDialog*>(BC);
	return S!=NULL;
}
bool SimpleDialog_editor::Process(){
	SD->Visible=CB->State;
	CB->x1=CB->x+18;
	CB->y1=CB->y+18;
	return true;
}
OneElementEditor* SimpleDialog_editor::_new(){
	return (OneElementEditor*)new SimpleDialog_editor;
}
int SimpleDialog_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,	BaseClass* Class,
									void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	SD=(SimpleDialog*)DataPtr;
	//CB=Base->addGP_CheckBox(NULL,x-CParam->LeftColumnWidth,y,"",&YellowFont,&YellowFont,0,SD->Visible,0,1,0,0);
	int cx=x-CParam->LeftColumnWidth+CParam->AbsMinLeftWidth-26;
	CB=Base->addGP_CheckBox(NULL,cx,y,"",&YellowFont,&YellowFont,0,SD->Visible,0,1,0,0);
	CParam->GP_dx+=14;
	CParam->LeftColumnWidth+=CParam->RightColumnWidth;
	CParam->RightColumnWidth=0;
	return y1;
}		
//
bool ucl_PicFileDelete(SimpleDialog* SD){
	_picfile_editor* E=(_picfile_editor*)SD->UserParam;
	E->str[0]=0;	
	return true;
};
int _picfile_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
							BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	// init
	Data=(_picfile*)DataPtr;
	if(Data->pchar()) strcpy(str,Data->pchar()); else str[0]=0;
	//
	InputBox* IB=Base->addInputBox(NULL,x,y,str,120,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
	IB->Enabled=0;
	SimpleDialog* SD=Base->addViewPort(x,y,x1-x+1,y1-y+1);
	SD->OnUserClick=&LS_gp_UserClick;
	LS_gp_param* p=new LS_gp_param;
	p->destname=str;
	//_str* Ext=(_str*)((int)this+(int)ExtraPtr);
	//p->ext=(const char*)Ext->pchar();
	p->ext="*.JPG,*.TGA,*.BMP";
	p->startdir="interf3\\";
	p->CE=CParam->CE;
	SD->AllocPtr=(char*)p;
	// delete
	int LX=25;
	static int bidx=GPS.PreLoadGPImage("Interf3\\btnz");
	GP_Button* BD=Base->addGP_Button(NULL,x1-LX,y,bidx,9,8);	
	BD->ShapeFileID=bidx;
	BD->ShapeSpriteID=9;
	BD->OnUserClick=ucl_PicFileDelete;
	BD->UserParam=(int)this;
	// preview
	Canvas* C=new Canvas;
	C->Setx(x);
	C->Sety(y1);
	C->SetWidth(x1-x+1);
	C->SetHeight(240);
	Base->DSS.Add(C);
	Preview=new BitPicture;
	Preview->SetWidth(x1-x+1);
	Preview->SetHeight(240);
	C->DSS.Add(Preview);
	return y1+240;
}
bool _picfile_editor::Process(){
	Data->Assign(str);
	//Preview->FileName.Assign(str);
	Preview->SetFileName(str);
	Preview->Visible=str[0]!=0;
	return true;
}
//new-style editor function defaults
void OneElementEditor::Assign(BaseClass* BC,void* Ptr,void* ExtraPtr){
	xmlQuote xml;
	BC->Save(xml,Ptr,ExtraPtr);
    Assign(&xml);
}
int OneElementEditor::Get(BaseClass* BC,void* Ptr,void* ExtraPtr){
	xmlQuote xml;	
	int v=Get(&xml);
	if(v){
		ErrorPager ep(0);
        //BC->reset_class(Ptr);
		BC->Load(xml,Ptr,&ep,ExtraPtr);
	}
	return v;
}
DWORD OneElementEditor::GetHashValue(BaseClass* BC,void* Ptr,void* ExtraPtr){
	xmlQuote xml;
	BC->Save(xml,Ptr,ExtraPtr);
	return GetXML_Hash(xml);
}

//void DrawThumbnail( DWORD mdlID, const Rct& vp, DWORD animID, float animTime, float rotAngle, float scale, float shift, DWORD bgColor );
bool _ModelID_editor::ModelDraw(SimpleDialog* SD){
	_ModelID_editor* ME=(_ModelID_editor*)SD->UserParam;
	if(*ME->model_id>0){
        try{
		    TempWindow TW;
		    PushWindow(&TW);
		    IntersectWindows(SD->x,SD->y+25,SD->x1,SD->y1);   
			int ModelID=*ME->model_id;
			int AnimID=-1;
			if(ME->ModelShift){
				AnimID=ModelID;
				ModelID=*((int*)(((BYTE*)ME->model_id)-ME->ModelShift));
			}
			int T=IMM->GetAnimTime(AnimID);
			if(T>0)T=GetTickCount()%T;
		    DrawThumbnail( ModelID, Rct(SD->x,SD->y+25,SD->x1-SD->x,SD->y1-SD->y-25),AnimID,T );
		    IntersectWindows(0,0,10,10);
		    PopWindow(&TW);
        }catch(...){
        }
	}
	GPS.DrawRect(SD->x,SD->y+1,SD->x1-SD->x-1,SD->y1-SD->y-2,0xFFB0B0B0);
	GPS.DrawLine(SD->x,SD->y+25,SD->x1,SD->y+25,0xFFB0B0B0);
	return true;
}
struct _strLParam{
	int x,y,Lx,Ly;
	_str mask;
    char* dest;    
};
bool _strUserClick(SimpleDialog* SD){
	_strLParam* slp=(_strLParam*)SD->AllocPtr;
	_str str;
	str=slp->dest;
	if(ProcessLoadSave(slp->x,slp->y,slp->Lx,slp->Ly,str,slp->mask.pchar(),slp->dest,true)){
		strcpy(slp->dest,str.pchar());
	};
	return true;
}
int _textid_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
				BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	//Data=(char*)DataPtr;
	if(ExtraPtr){
		IB=Base->addInputBox(NULL,x,y,str,2000,x1-x+1-26,y1-y+1,&CED_Font,&CED_AFont);
		static int bidx=GPS.PreLoadGPImage("Interf3\\btnz");
		GP_Button* GPB=Base->addGP_Button(NULL,x1-24,y,bidx,1,0);
		GPB->OnUserClick=&_strUserClick;
		_strLParam* lp=znew(_strLParam,1);
		GPB->AllocPtr=(char*)lp;
		lp->x=CParam->CE->Frame->x;
		lp->y=CParam->CE->Frame->y;
		lp->Lx=CParam->CE->Frame->x1-lp->x;
		lp->Ly=CParam->CE->Frame->y1-lp->y;
		char* ms=(char*)ExtraPtr;

		if(!strcmp(ms,"auto")){			
			char cc[256];
			sprintf(cc,".%s.xml",CParam->ELP->BaseParent->GetClassName());
			lp->mask=cc;
		}else lp->mask=(char*)ExtraPtr;
		lp->dest=str;
	}else{
		IB=Base->addInputBox(NULL,x,y,str,2000,x1-x+1,y1-y+1,&CED_Font,&CED_AFont);
	}	
	return y1;
}
void _fractal::ShiftXY(int& x,int& y){
    float v1=GetFValue((x+y)/2,(x-y)/2);
	float v2=GetFValue(x+4398,y+7995);
	x+=(v1-0.5f)*300;
	y+=(v2-0.5f)*300;
};
int _fractalLine_editor::GetHash(){
	_fractalLayer* FL=(_fractalLayer*)F;
	return int(FL->Center*1737+FL->Contrast*19971+FL->Scale*453109+FL->Mapping*999+FL->Type*199932
		+FL->Constant*799+FL->LinearX*999+FL->LinearY*12987+FL->SquareX*34512+FL->SquareY*234183);
}
int _fractalLayer_editor::GetHash(){
	_fractalLayer* FL=(_fractalLayer*)F;
	float fp=0.5f;
	if(FL->GetParent()){
		fp=FL->GetParent()->GetFloatField("FillingPercent");
	}
	return int(FL->Center*1737+FL->Contrast*19971+FL->Scale*453109+FL->Mapping*999+FL->Type*199932
		+FL->Constant*799+FL->LinearX*999+FL->LinearY*12987+FL->SquareX*34512+FL->SquareY*234183+fp*134563);
}
float _fractalLine_editor::GetValue(int x,int y){
	int xx=x<<5;
	int yy=y<<5;
	F->ShiftXY(xx,yy);
	float v=0.5f+(xx-3840)/600.0f+(yy-640)/200.0f;
	if(v<0)v=0;
	if(v>1)v=1;
	return v;
}
void ShowStringEx(int x,int y,LPCSTR lps,lpRLCFont lpf);
DWORD MR_Disable[128];
bool MR_Init=false;

float MapRef_editor::GlobalShiftX=0;
float MapRef_editor::GlobalShiftY=0;
float MapRef_editor::GlobalScale=1;
extern int SurfaceGenMinimap;
bool MapRef_editor::_DrawPalette(SimpleDialog* SD){
	if(!MR_Init){
		MR_Init=true;
		memset(MR_Disable,0,sizeof MR_Disable);
	}
	MapRef_editor* MRE=(MapRef_editor*)SD->UserParam;
	BasePaintMap* Map=MLayers->Maps[MRE->MRef->TopLayerIndex];		
	if(MRE->Meshes.GetAmount()&&SD->Enabled){
		void DrawCircle(int x,int y,int r,DWORD Color);
		BaseMeshDialog* BM=MRE->Meshes[MRE->Meshes.GetAmount()-2];
		ShowStringEx(SD->x,SD->y-20/*+BM->y-BM->y1*/,Map->Desc.pchar(),&SmallWhiteFont);
		if(BM->MouseOver){
			int lx,ly;
			Map->GetSize(lx,ly);
			int x=mouseX;
			int y=mouseY;
			int r=MRE->MRef->PenRadius*64*(BM->x1-BM->x)/lx;
			int dr=MRE->MRef->PenWidth*32*(BM->x1-BM->x)/lx;
			TempWindow TW;
			PushWindow(&TW);
			IntersectWindows(SD->x,SD->y+BM->y-BM->y1,SD->x1,SD->y-1);
			void DrawCircle2(float x,float y,float r,float r2,DWORD Color);
			int r2=r*(BM->y1-BM->y+1)/(BM->x1-BM->x+1);
			int r22=(r+dr)*(BM->y1-BM->y+1)/(BM->x1-BM->x+1);
			DrawCircle2(x,y,r*GlobalScale,r2*GlobalScale,0x80FFFFFF);
			DrawCircle2(x,y,(r+dr)*GlobalScale,r22*GlobalScale,0x80FFFFFF);
			PopWindow(&TW);
		}
		int N=MRE->Meshes.GetAmount();
		for(int i=0;i<N;i++){
			BaseMeshDialog* BM=MRE->Meshes[i];
			DWORD C=i==MRE->MRef->TopLayerIndex?0xB0FFFFFF:0x40FFFFFF;
			if(i==N-1){
				BM->CreateSquareTnL(GlobalShiftX,GlobalShiftY,(GlobalShiftX+1/GlobalScale)*240.0/256.0,(GlobalShiftY+1/GlobalScale)*240.0/256.0,"LayersEditor",0x80FFFFFF);				
				BM->BM->setTexture(SurfaceGenMinimap);
			}else{
				BM->CreateSquareTnL(GlobalShiftX,GlobalShiftY,GlobalShiftX+1/GlobalScale,GlobalShiftY+1/GlobalScale,"LayersEditor",C);
				BM->BM->setTexture(MLayers->Maps[i]->TextureIndex);
			}
		}
	}
	int NP=MRE->Meshes.GetAmount()-1;	
	if(NP){
		MRE->Meshes[NP]->Visible=GetKeyState(VK_MENU) & 0x8000;
		if(NP>256)NP=256;
		int xi[257];
		for(int i=0;i<=NP;i++){
			xi[i]=(SD->x1-SD->x)*i/NP;
		}
		for(int i=0;i<NP;i++){
			BasePaintMap* Map1=MLayers->Maps[i];
			GPS.DrawFillRect(SD->x+xi[i],SD->y,xi[i+1]-xi[i],SD->y1-SD->y,Map1->GetPaletteColor(1));
			if(i==MRE->MRef->TopLayerIndex){
				GPS.DrawFillRect(SD->x+(xi[i]+xi[i+1])/2-2,(SD->y+SD->y1)/2-2,4,4,0xFFFFFFFF);
			}							
		}
		GPS.DrawRect(SD->x,SD->y,SD->x1-SD->x,SD->y1-SD->y,0x90000000);
		GPS.DrawRect(SD->x+1,SD->y+1,SD->x1-SD->x-2,SD->y1-SD->y-2,0x90000000);
		GPS.FlushBatches();
		for(int i=0;i<NP;i++){
			if(MR_Disable[i>>5]&(1<<(i&31))){
				int x0=SD->x+(xi[i]+xi[i+1])/2;
				int y0=(SD->y+SD->y1)/2;
				GPS.DrawLine(x0-8-1,y0-8,x0+8-1,y0+8,0x60000000);
				GPS.DrawLine(x0-8+2,y0-8,x0+8+2,y0+8,0x60000000);
				GPS.DrawLine(x0-8-1,y0+8,x0+8-1,y0-8,0x60000000);
				GPS.DrawLine(x0-8+2,y0+8,x0+8+2,y0-8,0x60000000);				

				GPS.DrawLine(x0-8,y0-8,x0+8,y0+8,0xFFFF0000);
				GPS.DrawLine(x0-8+1,y0-8,x0+8+1,y0+8,0xFFFF0000);
				GPS.DrawLine(x0-8,y0+8,x0+8,y0-8,0xFFFF0000);
				GPS.DrawLine(x0-8+1,y0+8,x0+8+1,y0-8,0xFFFF0000);				

			}
		}
		if(SD->MouseOver){
			if(Lpressed){
				int L=MRE->MRef->TopLayerIndex=SD->MouseRelX*NP/(SD->x1-SD->x);
				//MR_Disable[MRE->MRef->TopLayerIndex>>5]&=~(1<<(MRE->MRef->TopLayerIndex&31));								
				memset(MR_Disable,0,sizeof MR_Disable);
				if(LAYERSLIST.Layers[L]->ThisLayerDeterminesRace){					
					for(int i=0;i<LAYERSLIST.Layers.GetAmount();i++)if(i!=L && LAYERSLIST.Layers[i]->ThisLayerDeterminesRace){
						MR_Disable[MRE->MRef->TopLayerIndex>>5]|=(1<<(i&31));
					}
				}
			}
			if(Rpressed){
				int i=SD->MouseRelX*NP/(SD->x1-SD->x);
				MR_Disable[i>>5]^=1<<(i&31);
			}
			Lpressed=0;	
			Rpressed=0;
		}			
	}
	GPS.FlushBatches();
	return true;
}
void MapRef_editor::CheckTopLayer(){
	int NM=Meshes.GetAmount()-1;
	if(cTopIndex!=MRef->TopLayerIndex){
		int ctidx=MIndex[NM-1];
		if(MLayers->Maps[ctidx]->GetPaletteColor(0)!=0){
			MIndex.Del(NM-1,1);
			MIndex.Insert(0,ctidx);
		}
		int pp=MIndex.find(MRef->TopLayerIndex);
		if(pp!=-1){
			MIndex.Del(pp,1);
			MIndex.Add(MRef->TopLayerIndex);
		}
		int N=MLayers->Maps.GetAmount();
		for(int i=0;i<N;i++){
			Meshes[i]->BM->setTexture(MLayers->Maps[MIndex[i]]->TextureIndex);
		}
		cTopIndex=MRef->TopLayerIndex;
	}
}
//MapRef_editor options (in ExtraPtr)
//-maxLx VV
//-maxLy VV
//-noedit
//-grid COST
//-nopalette
//-defpalet VV
//-nosquare
int GetOption(void* ExPtr,char* opname){
	if(ExPtr){
        char* s=strstr((char*)ExPtr,opname);
		if(s){
			s+=strlen(opname);
			if(s[-1]==' '){
                return atoi(s);
			}else{
				return 1;
			}
		}
	}
	return 0;
}
int MapRef_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,
						  BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
    
	int DW=CParam->LeftColumnWidth-CParam->AbsMinLeftWidth-4;
	x-=DW;
	CParam->RightColumnWidth+=DW;
	CParam->LeftColumnWidth-=DW;            
	CParam->VerticalText=1;
	if(!MLayers)MLayers=new MapsScope;
	MRef=(MapRef*)DataPtr;

	int maxLx=GetOption(ExtraPtr,"-maxLx ");
	if(!maxLx)maxLx=10000;
	int maxLy=GetOption(ExtraPtr,"-maxLy ");
	if(!maxLy)maxLy=10000;
    bool nosquare=GetOption(ExtraPtr,"-nosquare");
	bool nopal=GetOption(ExtraPtr,"-nopalette");
	noedit=GetOption(ExtraPtr,"-noedit");


	if(x1-x>maxLx)x1=x+maxLx;
	if(!nosquare){
		//if(y1-y<64)
		y1=y+x1-x;
	}
	int N=MLayers->Maps.GetAmount();	

	for(int i=0;i<N;i++){
		BaseMeshDialog* BM=Base->addBaseMeshDialog(x,y,x1-x+1,y1-y+1);
		Meshes.Add(BM);
		if(i==N-1)BM->CreateSquareTnL(0,0,1,1,"LayersEditor",0xB0FFFFFF);
		else BM->CreateSquareTnL(0,0,1,1,"LayersEditor",0x40FFFFFF);
		BM->BM->setTexture(MLayers->Maps[i]->TextureIndex);
		MIndex.Add(i);
	}

	BaseMeshDialog* BM=Base->addBaseMeshDialog(x,y,x1-x+1,y1-y+1);
	BM->Enabled=1;
	BM->Visible=1;
	Meshes.Add(BM);
	BM->CreateSquareTnL(0,0,240.0/256.0,240.0/256.0,"LayersEditor",0x80FFFFFF);
	BM->BM->setTexture(SurfaceGenMinimap);

	int gcost=GetOption(ExtraPtr,"-grid ");
	if(gcost){
		for(int i=0;i<15360;i+=gcost){
			for(int j=0;j<15360;j+=gcost){
				int xx=x+(i*(x1-x)/15360);
				int yy=y+(j*(y1-x)/15360);
				ColoredBar* C=Base->addColoredBar(xx,yy,2,2,0xFF);
				C->Diffuse=0x60FF0000;
			}
		}
	}
	cTopIndex=GetOption(ExtraPtr,"-defpalet ");
	CheckTopLayer();
	if(!nopal){
		Palet=Base->addViewPort(x,y1,x1-x+1,pYsize);
		Palet->OnDraw=&_DrawPalette;
		Palet->UserParam=int(this);
		y1+=pYsize;
	}
	CurRColor=MRef->PenRColor;
	CurLColor=MRef->PenColor;	
	return y1;
}
int Layers_modFunc(int x0,int y0,int dx,int dy){
	int maxv=0;
	for(int i=0;i<MLayers->Maps.GetAmount();i++)if(MR_Disable[i>>5]&(1<<(i&31))){
		int id,w,n;
        MLayers->Maps[i]->GetValue(x0+dx,y0+dy,id,w,n);
		if(w>maxv)maxv=w;
	}
	return 255-maxv;
}
int MapRef_editor::Get(xmlQuote* xml){	
	CheckTopLayer();
	if(noedit)return false;
	int NM=Meshes.GetAmount()-1;
	BasePaintMap* Map=MLayers->Maps[MRef->TopLayerIndex];
	if(NM){		
		BaseMeshDialog* BM=Meshes[NM-1];
		if(BM->MouseOver){

			static int prevActTime=GetTickCount();
			if(GetTickCount()-prevActTime>50){
				if(GetKeyState(VK_ADD)&0x8000){
					GlobalScale/=0.9;
				}
				if(GetKeyState(VK_SUBTRACT)&0x8000){
					GlobalScale*=0.9;
				}
				if(GetKeyState(VK_LEFT)&0x8000){
					GlobalShiftX-=0.05/GlobalScale;
				}
				if(GetKeyState(VK_RIGHT)&0x8000){
					GlobalShiftX+=0.05/GlobalScale;
				}
				if(GetKeyState(VK_UP)&0x8000){
					GlobalShiftY-=0.05/GlobalScale;
				}
				if(GetKeyState(VK_DOWN)&0x8000){
					GlobalShiftY+=0.05/GlobalScale;
				}
				prevActTime=GetTickCount();
				if(GlobalScale<1)GlobalScale=1;
                if(GlobalShiftX<0)GlobalShiftX=0;
				if(GlobalShiftY<0)GlobalShiftY=0;
				if(GlobalShiftX>1-1/GlobalScale)GlobalShiftX=1-1/GlobalScale;
				if(GlobalShiftY>1-1/GlobalScale)GlobalShiftY=1-1/GlobalScale;
			}		

			static int prevX=0;
			static int prevY=0;
			static bool prevpress=0;
			if(BM->MouseRelX!=prevX||BM->MouseRelY!=prevY){
				extern bool realLpressed;
				extern bool realRpressed;			
				if(realLpressed&&!ParentFrame::ActiveParent){							
					int lx,ly;
					Map->GetSize(lx,ly);
					int x=BM->MouseRelX*lx/(BM->x1-BM->x)/GlobalScale+lx*GlobalShiftX;
					int y=BM->MouseRelY*ly/(BM->y1-BM->y)/GlobalScale+ly*GlobalShiftY;
					int dx=0;
					int dy=0;
					if(prevpress){
						dx=(BM->MouseRelX-prevX)*lx/(BM->x1-BM->x)/GlobalScale;
						dy=(BM->MouseRelY-prevY)*ly/(BM->y1-BM->y)/GlobalScale;
					}
					int R=int(sqrt(float(dx*dx+dy*dy)));
					int N=R/4;							
					for(int i=0;i<=N;i++)Map->SetValueInRound(x-(dx*i/(N+1)),y-(dy*i/(N+1)),MRef->PenRadius*64,MRef->PenWidth*32,MRef->PenColor,Layers_modFunc);
					prevX=BM->MouseRelX;
					prevY=BM->MouseRelY;
					prevpress=1;
				}else prevpress=0;
				if(realRpressed&&!ParentFrame::ActiveParent){
					int lx,ly;
					Map->GetSize(lx,ly);
					int x=BM->MouseRelX*lx/(BM->x1-BM->x)/GlobalScale+lx*GlobalShiftX;
					int y=BM->MouseRelY*ly/(BM->y1-BM->y)/GlobalScale+ly*GlobalShiftY;
					Map->SetValueInRound(x,y,MRef->PenRadius*64,MRef->PenWidth*32,MRef->PenRColor);
					prevX=BM->MouseRelX;
					prevY=BM->MouseRelY;
				}
				if(!ParentFrame::ActiveParent){
					Lpressed=0;
					Rpressed=0;
				}
			}else prevpress=0;					
		}
	}
	return false;
}
extern int LastMx;
extern int LastMy;
void GlobalHandleMouse();
void WriteMeshToObj_VertexN(BaseMesh& bm,const char* fname){
    FILE* F=fopen(fname,"w");
    if(F){
        VertexN* vn=(VertexN*)bm.getVertexData();
        for(int i=0;i<bm.getNVert();i++){
            fprintf(F,"v %f %f %f\n",vn->x,vn->z,vn->y);
            vn++;
        }
        word* idxs=bm.getIndices();
        for(int i=0;i<bm.getNInd();i+=3){
            fprintf(F,"f %d %d %d\n",idxs[0]+1,idxs[1]+1,idxs[2]+1);
            idxs+=3;
        }
    }
    fclose(F);
}
void DrawFractalPreview3D(_graph2d* f){
	_fractal_editor* fr=(_fractal_editor*)(f);	
	BaseMesh BMS[25];
	int NM=0;
	Vector3D v0=ICam->GetLookAt();
	int L=1024;
	for(int ix=-1;ix<=1;ix++){
		for(int iy=-1;iy<=1;iy++){
			int p=ix+1+(iy+1)*3;
			BaseMesh& BM=BMS[p];
			BM.createPatch(180,180,Vector3D(v0.x-L+L*2*ix,v0.y-L+L*2*iy,0),Vector3D(v0.x+L+L*2*ix,v0.y-L+L*2*iy,0),Vector3D(v0.x-L+L*2*ix,v0.y+L+L*2*iy,0));
			BM.convertVF(vfVertexN);
			int nv=BM.getNVert();
			VertexN* vn=(VertexN*)BM.getVertexData();
			for(int i=0;i<nv;i++){
				vn->z=fr->F->GetFValue(vn->x,vn->y)*256.0f;
				vn->u=0;
				vn->v=1-vn->z/256.0f;
                Vector3D N(0,0,16);
				N.x=(fr->F->GetFValue(vn->x-8,vn->y)-fr->F->GetFValue(vn->x+8,vn->y))*256.0f;
				N.y=(fr->F->GetFValue(vn->x,vn->y-8)-fr->F->GetFValue(vn->x,vn->y+8))*256.0f;
				N.normalize();
				vn->nx=N.x;
				vn->ny=N.y;
				vn->nz=N.z;
				vn++;
			}
			vn=(VertexN*)BM.getVertexData();
			word* idxs=BM.getIndices();
			int ni=BM.getNInd();
			for(int i=0;i<ni;i+=6){
                int v1=idxs[0];
				int v2=idxs[1];
				int v3=idxs[2];
				int v4=idxs[5];
				float h1=vn[v1].z;
				float h2=vn[v2].z;
				float h3=vn[v3].z;
				float h4=vn[v4].z;	
                float h0=fr->F->GetFValue((vn[v1].x+vn[v3].x)/2,(vn[v1].y+vn[v3].y)/2)*256.0f;
                h0*=2;
                if(fabs(h1+h4-h0)>fabs(h2+h3-h0)){
					idxs[2]=v4;
					idxs[3]=v1;
					idxs[4]=v4;
					idxs[5]=v3;
				}
				idxs+=6;
            }
			BM.setShader(IRS->GetShaderID("fractal_show"));
			BM.setTexture(IRS->GetTextureID("mount_layers.dds"),0);	
			BM.calcNormals();
			NM++;
		}
	}
	g_SurfEditor.HideUI=true;
    int id=GetTickCount()&65535;
	do{
		IRS->ClearDevice(0xFF004000);
		IRS->SetShader(IRS->GetShaderID("fractal_show"));		
		IRS->SetTexture(IRS->GetTextureID("mount_layers.dds"));
		IRS->SetShaderAutoVars();
		for(int p=0;p<NM;p++){
			DrawPrimBM(BMS[p]);
		}
		bool add=GetKeyState(VK_ADD) & 0x8000;
		bool sub=GetKeyState(VK_SUBTRACT) & 0x8000;
		if(add || sub){
			float mod=add?1.05:0.95;
			for(int p=0;p<NM;p++){
				BaseMesh& BM=BMS[p];
				int nv=BM.getNVert();
				VertexN* vn=(VertexN*)BM.getVertexData();
				for(int i=0;i<nv;i++){
					vn[i].z*=mod;
				}
			}
		}        
		ProcessMessages();
        if(GetKeyState('W') & 0x8000){
            for(int p=0;p<NM;p++){
                char s[128];
                sprintf(s,"Meshes\\fractals\\f_%X_%d.obj",id,p);
                WriteMeshToObj_VertexN(BMS[p],s);
            }            
        }
		SetupCamera();		
		GlobalHandleMouse();
		FlipPages();
	}while(LastKey!=27);
	g_SurfEditor.HideUI=false;
	KeyPressed=false;
	LastKey=0;
}
bool DrawF_preview(SimpleDialog* SD){
	DrawFractalPreview3D((_graph2d*)SD->UserParam);
	return true;
}
int _graph2d::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	if(TextureID==-1){
		TextureID=DynTexManager::GetTexture();
	}
	if(TextureID>0){
		BM=Base->addBaseMeshDialog(x,y,x1-x+1,y1-y+1);
		BM->CreateSquareTnL(0,0,float(x1-x)/256.0f,float(y1-y)/256.0f,"hud",0xFFFFFFFF);
		BM->BM->setTexture(TextureID);
		CurHash=0;
		Lx=x1-x+1;
		Ly=y1-y+1;
		SimpleDialog* SD=Base->AddCanvas(x,y,x1-x+1,y1-y+1);
		SD->UserParam=(int)this;
		SD->OnUserClick=&DrawF_preview;
	}
	return y1;
}
int _graph2d::Get(xmlQuote* xml){
	if(TextureID>0&&CurHash!=GetHash()){
		int Pitch;
		BYTE* B=IRS->LockTexBits(TextureID,Pitch);
		if(B){
            InitBeforeGetValues();
			for(int iy=0;iy<Ly;iy++){
				int ofs=iy*Pitch;
				for(int ix=0;ix<Lx;ix++){
					int V=int(GetValue(ix,iy)*255.0f);
					if(V>255)V=255;
					if(V<0)V=0;
					//*((WORD*)(B+ofs))=(V>>3)+((V>>2)<<5)+((V>>3)<<11);
					DWORD C=MixDWORD(0xFFFFFFFF,0xFF400000,V,255-V);
					WORD DWORD2WORD(DWORD C);
					*((WORD*)(B+ofs))=DWORD2WORD(C);//0xFF000000+(V<<16)+(V<<8)+V;
					ofs+=TEXPIXSIZE;
				}
			}
			IRS->UnlockTexBits(TextureID);
		}
		CurHash=GetHash();
	}
	return true;
}
float _fractalLayer::GetFValue(int x,int y){
    int R=Contrast*3+1;
	int NP=0;
	int NF=0;
	for(int i=0;i<R;i++){
		int n=Rarr[i].N;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		for(int j=0;j<n;j++){
			float V=_fractal::GetFValue(x+xi[j]*32,y+yi[j]*32)-0.5f;
			float dx=x-CenterX;
			float dy=y-CenterY;
			addmr(V*1000);
			addmr(Constant*1000);
			addmr(LinearX*10000);
			addmr(LinearY*10000);
			addmr(SquareX*100000);
			addmr(SquareY*100000);
			V+=Constant+dx*LinearX+dy*LinearY+dx*SquareX*dx+dy*SquareY*dy;
			addmr(V*1000);
			if(V>0)NF++;
			NP++;
		}
	}
	addmr(NF*1000);
	addmr(NP);
	return NP?float(NF)/NP:0;
}
float rand2f(float x1,float x2){
	return x1+((x2-x1)*rand()/32768.0f);
}
void _fractalLayer_editor::InitBeforeGetValues(){
	_fractalLayer* FL=(_fractalLayer*)F;
	//FL->Center=0;
	FL->LinearX=0;
	FL->LinearY=0;
	FL->SquareX=0;
	FL->SquareY=0;
	float coef=0.0004;
	float coef2=coef*coef;
	if(F&&F->GetParent()){
		BaseClass* BC=F->GetParent();
        FL->CenterX=240*16;
		FL->CenterY=240*16;
		FL->Constant=(2.0f*rand2f(BC->GetFloatField("Constant_start"),BC->GetFloatField("Constant_final"))-1.0f)/2.0f;
		FL->LinearX= (2.0f*rand2f(BC->GetFloatField("LinearX_start") ,BC->GetFloatField("LinearX_final")) -1.0f)*coef/2.0f;
		FL->LinearY= (2.0f*rand2f(BC->GetFloatField("LinearY_start") ,BC->GetFloatField("LinearY_final")) -1.0f)*coef/2.0f;
		FL->SquareX= (2.0f*rand2f(BC->GetFloatField("SquareX_start") ,BC->GetFloatField("SquareX_final")) -1.0f)*coef2/2.0f;
		FL->SquareY= (2.0f*rand2f(BC->GetFloatField("SquareY_start") ,BC->GetFloatField("SquareY_final")) -1.0f)*coef2/2.0f;
		float fp=BC->GetFloatField("FillingPercent");
		//tuning filling percent
		short ptx[512];
		short pty[512];
		int npt=0;
		for(int q=0;q<2048&&npt<256;q++){
			int x=mrand()&511;
			int y=mrand()&511;
			ptx[npt]=x;
			pty[npt]=y;
			npt++;
		}
		if(npt){
			int dir=0;
			int nitr=0;
			do{
				int ns=0;
				for(int q=0;q<npt;q++){
					if(FL->GetFValue(ptx[q]*16,pty[q]*16)>0.5f)ns++;
				}
				float pp=float(ns)/npt;
				if(pp>fp){
					if(dir==0)dir=1;
					if(dir==-1)break;
					FL->Constant-=0.01;
				}else
					if(pp<fp){
						if(dir==0)dir=-1;
						if(dir==1)break;
						FL->Constant+=0.01;
					}
					nitr++;
			}while(nitr<300);
		}
	}
	
};
float _fractalLayer_editor::GetValue(int x,int y){	
	return F->GetFValue(x<<6,y<<6);
}
int _fractalLayer_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	y1=y+120;
	x1=x+120;
	F=(_fractal*)DataPtr;
	return _graph2d::CreateControl(Base,x,y,x1,y1,Class,DataPtr,ExtraPtr, CParam);
}

bool ReadWinString(GFILE* F,char* STR,int Max);
int CalculateCheckSum(char* FN){
	GFILE* F=Gopen(FN,"r");
	int Sum=0;
	if(F){
		char ccc[513];
		int i=0;
		while( bool Done=ReadWinString(F,ccc,512) ){
			for(int j=0;j<513;j++){
				if(ccc[j]){
					Sum+=ccc[j]*(j+1)*(++i);
				}else{
					break;
				}
			}
		}
	}
	Gclose(F);
	return Sum;
}
void _util_SaveClassLikeFolder(BaseClass* BC,const char* xmlName,const char* FieldName){
	xmlQuote xml;
	BaseClass::SaveInShortForm=BC->ShouldSaveInShortForm();
	BC->Save(xml,BC,NULL);
	BaseClass::SaveInShortForm=false;
	char name[256];
	strcpy(name,xmlName);
	strupr(name);
	char* s=strstr(name,".XML");
	if(s)s[0]=0;
	strcat(name,"\\");
	CreateDirectory(name,0);

	//StrHash DelList;
	FileList DelList;
	CreateSortedFileList(name,"*.*",DelList,false);
	//for(int i=0;i<FL.GetAmount();i++){
	//	char x[512];
	//	sprintf(x,"%s%s",name,FL[i]->pchar());
	//	DeleteFile(x);
	//}

	// calculate checksum
	DynArray<int> DelChkSum;
	for(int i=0;i<DelList.GetAmount();i++){
		char fn[256];
		sprintf(fn,"%s%s",name,DelList[i]->pchar());
		DelChkSum.Add(CalculateCheckSum(fn));
	}

	for(int i=0;i<xml.GetNSubQuotes();i++){
		xmlQuote* x=xml.GetSubQuote(i);
		xmlQuote* n=x->GetSubQuote((char*)FieldName);
		if(n){
			char* tmpf="Temp\\$$$temp$$$.xml";
			x->WriteToFile(tmpf);
			int Sum=CalculateCheckSum(tmpf);

			bool write=true;
			bool exist=false;

			for(int i=0;i<DelList.GetAmount();i++){
				char* df=DelList[i]->pchar();
				if(!stricmp(n->Get_string(),df)){
					exist=true;
					if(Sum==DelChkSum[i]){
						write=false;
					}
					DelList.DelElement(i);
					DelChkSum.Del(i,1);
					break;
				}
			}

			if(write){
				char fn[256];
				sprintf(fn,"%s%s",name,n->Get_string());
				if(!exist) DeleteFile(fn);
				x->WriteToFile(fn);
			}
		}
	}

	for(int i=0;i<DelList.GetAmount();i++){
		char x[512];
		sprintf(x,"%s%s",name,DelList[i]->pchar());
		DeleteFile(x);
	}
}
//returns false if 
bool _util_LoadClassLikeFolder(BaseClass* BC,const char* xmlName,const char* FieldName){
	char name[256];
	BC->reset_class(BC);
	strcpy(name,xmlName);
	strupr(name);
	char* s=strstr(name,".XML");
	if(s)s[0]=0;
	strcat(name,"\\");	
	FileList FL;
	CreateSortedFileList(name,"*.*",FL,false);
	xmlQuote xml;
	for(int i=0;i<FL.GetAmount();i++){
		char fn[256];
		char* field=FL[i]->pchar();
		sprintf(fn,"%s%s",name,field);
		xmlQuote* x=new_xmlQuote();
		if(x->ReadFromFile(fn)){
			//xmlQuote* n=x->GetSubQuote((char*)FieldName);
			//if (n) n->Assign_string(field);
            //if (n) x->SetQuoteName(n->Get_string());
			xml.AddSubQuote(x);
		}
		else delete(x);
	}
	if(!FL.GetAmount())xml.ReadFromFile((char*)xmlName);
	ErrorPager ep(0);
	BC->Load(xml,BC,&ep,NULL);
	return FL.GetAmount()!=0;
}
DWORD _button::Color=0xFFB0B0B0;	
DynArray<DString*> _ModelID_editor::FileArr;
int _ModelID_editor::LastRefreshTime=0;
float BasePaintMap::GetGreyPrecise(float x,float y){
	int lx,ly;
	GetSize(lx,ly);
	x*=lx;
	y*=ly;
	int xx=int(x);
	int yy=int(y);
	float f1=float(GetGreyScale(xx,yy));
	float f2=float(GetGreyScale(xx+1,yy));
	float f3=float(GetGreyScale(xx,yy+1));
	float f4=float(GetGreyScale(xx+1,yy+1));
	float dx=x-xx;
	float dy=y-yy;
	return f1+dx*(f2-f1)+dy*(f3-f1)+dx*dy*(f4+f1-f3-f2);
}
void BasePaintMap::SetValueInRound(int x,int y,int r,int dr,int Index,modFunc* fn){
	if(dr<1)dr=1;
	int r1=r+dr+4;
	for(int dx=-r1;dx<r1;dx++){
		for(int dy=-r1;dy<r1;dy++){
			int R=sqrt(float(dx*dx+dy*dy));
			int W=0;
			if(R<r)W=255;
			else if(R<r+dr)W=255*(r+dr-R)/dr;
			if(fn)W=W*fn(x,y,dx,dy)/255;
			SetValue(x+dx,y+dy,Index,W);
		}
	}		
	int ctime=GetTickCount();
	static int ptime=ptime;
	//if(ctime-ptime>3000){
	//	int lx,ly;
	//	GetSize(lx,ly);
	//	CreateTexturePart(0,0,lx,ly);
	//}else 
	CreateTexturePart(x-r-dr,y-r-dr,(r+dr+2)*2,(r+dr+2)*2);
}
void BasePaintMap::PreciseSetValueInRound(float x,float y,float r,float dr){
	if(dr<1)dr=1;
	int r1=int(r+dr+4);
	int x0=x-r1;
	int x1=x+r1;
	int y0=y-r1;
	int y1=y+r1;
	for(int dx=x0;dx<=x1;dx++){
		for(int dy=y0;dy<y1;dy++){
			int R=sq_norma(x-dx,y-dy);
			int W=0;
			if(R<r)W=255;
			else if(R<r+dr)W=255*(r+dr-R)/dr;
			SetValue(dx,dy,255,W);
		}
	}				
}
void BasePaintMap::PreciseClearValueInRound(float x,float y,float r,float dr){
	if(dr<1)dr=1;
	int r1=int(r+dr+4);
	int x0=x-r1;
	int x1=x+r1;
	int y0=y-r1;
	int y1=y+r1;
	for(int dx=x0;dx<=x1;dx++){
		for(int dy=y0;dy<y1;dy++){
			int R=sq_norma(x-dx,y-dy);
			int W=0;
			if(R<r)W=255;
			else if(R<r+dr)W=255*(r+dr-R)/dr;
			SetValue(dx,dy,0,W);
		}
	}				
}
void BasePaintMap::SetSmoothInRadius(int x,int y,int r,int dr){
	if(dr<1)dr=1;
	int r1=r+dr+4;
	for(int dx=-r1;dx<r1;dx++){
		for(int dy=-r1;dy<r1;dy++){
			int R=sqrt(float(dx*dx+dy*dy));
			int W=0;
			if(R<r)W=256;
			else if(R<r+dr)W=256*(r+dr-R)/dr;
			int xc=x+dx;
			int yc=y+dy;
			if(W>0){
				DWORD CL=GetPixelColor(xc-1,yc);
				DWORD CR=GetPixelColor(xc+1,yc);
				DWORD CU=GetPixelColor(xc,yc-1);
				DWORD CD=GetPixelColor(xc,yc+1);
				DWORD C1=MixDWORD(CL,CR,128,128);
				DWORD C2=MixDWORD(CU,CD,128,128);
				C1=MixDWORD(C1,C2,128,128);
				if(W<255){
					DWORD C0=GetPixelColor(xc,yc);
					C1=MixDWORD(C1,C0,W,256-W);
				}
				SetPixelColor(xc,yc,C1);
			}
		}
	}
	CreateTexturePart(x-r-dr,y-r-dr,(r+dr+2)*2,(r+dr+2)*2);
}
extern bool ReducedLoadMode;
void BasePaintMap::CreateTexturePart(int x,int y,int Lx,int Ly){
    if(ReducedLoadMode)return;
	if(TextureIndex==-1){
		TextureIndex=DynTexManager::GetTexture();
	}
	if(TextureIndex!=-1){
		int tlx,tly;
		GetSize(tlx,tly);
		if(tlx&&tly){
			int pitch;
			BYTE* data=IRS->LockTexBits(TextureIndex,pitch);
			// If TestCooperativeLevel FAILED!
			if(!data) {
				return;
			}

			for(int iy=0;iy<Ly;iy++){
				for(int ix=0;ix<Lx;ix++){
					int xx=x+ix;  	             
					int yy=y+iy;
					if(xx>=0&&yy>=0&&xx<tlx&&yy<tly){
						DWORD C=GetPixelColor(xx,yy);
						//WORD CW=((C&255)>>3)+((((C>>8)&255)>>2)<<5)+((((C>>16)&255)>>3)<<11);
						xx=xx*256/tlx;
						yy=yy*256/tlx;
						//*((WORD*)(data+xx+xx+yy*pitch))=CW;
						WORD DWORD2WORD(DWORD C);
						*((WORD*)(data+xx*TEXPIXSIZE+yy*pitch))=DWORD2WORD(C);
					}
				}
			}
		}
		IRS->UnlockTexBits(TextureIndex);
		IRS->SaveTexture(TextureIndex,"test.dds");
	}
}
//////////////////////////////////////////////////////////////////////////
void  GreyMap::SetValue(int x,int y,int Index,int Weight){
	if(Index){
		if(Weight>128)SetGreyScale(x,y,255);
	}else{
		if(Weight>128)SetGreyScale(x,y,0);
	}
}
void  GreyMap::GetValue(int x,int y,int& Index,int& Weight,int& NextIndex){
	Index=1;
	NextIndex=0;
	Weight=GetGreyScale(x,y);
}
BYTE GreyMap::GetGreyScale(int x,int y){
	if(x<0)x=0;
	if(y<0)y=0;
	if(x>=Lx)x=Lx-1;
	if(y>=Ly)y=Ly-1;
	int ofs=x+y*Lx;
	return V[ofs>>3] & (1<<(ofs&7)) ? 255 : 0;		
}
void GreyMap::SetGreyScale(int x,int y,byte v){
	if(x>=0&&x<Lx&&y>=0&&y<Ly){
		int ofs=x+y*Lx;
		if(v<128){
			V[ofs>>3]&=255-(1<<(ofs&7));
		}else{
			V[ofs>>3]|=1<<(ofs&7);
		}
	}
}

//-----------------------------------------------------------------------------
// CtrlISliderEditor
//-----------------------------------------------------------------------------
bool CtrlISliderEditor::OnDraw(SimpleDialog *pSD) {
	CtrlISliderEditor *pSlider = (CtrlISliderEditor *)pSD->UserParam;

	int Lx = pSD->x1 - pSD->x + 1, Ly = pSD->y1 - pSD->y + 1;

	int p = pSD->x + int(pSlider->m_CurPos * Lx);
	int c = Ly / 2, R = Ly / 2;
	cColor CurClr;
	CurClr = cColor::Lerp(cColor::Blue / 3, cColor::Blue, pSlider->m_CurPos);
	for(int h = 0; h < Ly; h++) {
		float k = 1.0f - cMath::Square(float(h - c)) / float(R * R);
		cColor Clr = CurClr * k;
		Clr.a = 1.0f;
		GPS.DrawLine(pSD->x, pSD->y + h, p, pSD->y + h, Clr.ToBgra());
	}

	static RLCFont *pFont = &SmallWhiteFont;
	char Buffer[32];
	sprintf(Buffer, "%d", pSlider->m_pArgs->MinValue);
	ShowString(pSD->x + 5, pSD->y, Buffer, pFont);

	char Str[32];
	sprintf(Str, "%d", pSlider->GetValue());
	int x0 = (pSD->x + pSD->x1) / 2;
	int y0 = (pSD->y + pSD->y1) / 2;
	if(strlen(Str)) {
		int w = GetRLCStrWidth(Str, pFont);
		int h = GetRLCHeight(pFont->RLC, 'W');
		ShowString(x0 - w / 2, y0 - h / 2, Str, pFont);
	}

	sprintf(Str, "%d", pSlider->m_pArgs->MaxValue);
	if(strlen(Str)) {
		int w = GetRLCStrWidth(Str, pFont);
		int h = GetRLCHeight(pFont->RLC, 'W');
		ShowString(pSD->x1 - w - 5, y0 - h / 2, Str, pFont);
	}

	return true;
}

bool CtrlISliderEditor::OnMouseOver(SimpleDialog *pSD) {
	extern bool realLpressed;
	if(realLpressed) {
		Lpressed = false;
		CtrlISliderEditor *pSlider = (CtrlISliderEditor *)pSD->UserParam;
		if(mouseY >= pSD->y && mouseY < pSD->y1) {
			int dx = mouseX - pSD->x;
			if(dx >= 0 && dx < pSD->x1) {
				pSlider->m_CurPos = float(dx) / float(pSD->x1 - pSD->x + 1);
			}
		}
	}
	return false;
}

int CtrlISliderEditor::CreateControl(ParentFrame *pParent, int x0, int y0, int x1, int y1, BaseClass *, void *, void *pArgs, ControlParams *) {
	SimpleDialog *pSD = pParent->addViewPort(x0, y0, x1 - x0 - 1, y1 - y0);
	pSD->Enabled = pSD->Visible = 1;
	pSD->OnDraw = &OnDraw;
	pSD->OnMouseOver = &OnMouseOver;
	// Args:
	assert(pArgs);
	m_pArgs = (CtrlISliderArgs *)pArgs;
	pSD->UserParam = int(this);
	assert(m_pArgs->DefValue >= m_pArgs->MinValue && m_pArgs->DefValue <= m_pArgs->MaxValue);
	SetValue(m_pArgs->DefValue);
	
	return y1;
}

bool CtrlISliderEditor::Assign(xmlQuote *xml) {
	int n = xml->Get_int();
	SetValue(n);
	return true;
}

int CtrlISliderEditor::GetValue() const {
	float fValue = float(m_pArgs->MinValue) + m_CurPos * float(m_pArgs->MaxValue - m_pArgs->MinValue);
	float Sign = fValue > 0.0f ? 1.0f : (fValue < 0.0f ? -1.0f : 0.0f);
	return int(fValue + Sign * 0.5f);
}

void CtrlISliderEditor::SetValue(int n) {
	if(n == cMath::IntMaxValue || n == cMath::IntMinValue) {
		n = m_pArgs->DefValue;
	}
	if(n >= m_pArgs->MinValue && n <= m_pArgs->MaxValue) {
		m_CurPos = float(n - m_pArgs->MinValue) / float(m_pArgs->MaxValue - m_pArgs->MinValue);
	}
}

int CtrlISliderEditor::Get(xmlQuote *xml) {
	xml->Assign_int(GetValue());
	return true;
}

//-----------------------------------------------------------------------------
// CtrlFSliderEditor
//-----------------------------------------------------------------------------
bool CtrlFSliderEditor::OnDraw(SimpleDialog *pSD) {
	CtrlFSliderEditor *pSlider = (CtrlFSliderEditor *)pSD->UserParam;

	int Lx = pSD->x1 - pSD->x + 1, Ly = pSD->y1 - pSD->y + 1;

	int p = pSD->x + int(pSlider->m_CurPos * Lx);
	int c = Ly / 2, R = Ly / 2;
	cColor CurClr = cColor::Lerp(cColor::Red / 3, cColor::Red, pSlider->m_CurPos);
	for(int h = 0; h < Ly; h++) {
		float k = 1.0f - cMath::Square(float(h - c)) / float(R * R);
		cColor Clr = CurClr * k;
		Clr.a = 1.0f;
		GPS.DrawLine(pSD->x, pSD->y + h, p, pSD->y + h, Clr.ToBgra());
	}

	static RLCFont *pFont = &SmallWhiteFont;
	char Buffer[64];
	sprintf(Buffer, "%.3f", pSlider->m_pArgs->MinValue);
	ShowString(pSD->x + 5, pSD->y, Buffer, pFont);

	char Str[64];
	sprintf(Str, "%.3f", pSlider->GetValue());
	int x0 = (pSD->x + pSD->x1) / 2;
	int y0 = (pSD->y + pSD->y1) / 2;
	if(strlen(Str)) {
		int w = GetRLCStrWidth(Str, pFont);
		int h = GetRLCHeight(pFont->RLC, 'W');
		ShowString(x0 - w / 2, y0 - h / 2, Str, pFont);
	}

	sprintf(Str, "%.3f", pSlider->m_pArgs->MaxValue);
	if(strlen(Str)) {
		int w = GetRLCStrWidth(Str, pFont);
		int h = GetRLCHeight(pFont->RLC, 'W');
		ShowString(pSD->x1 - w - 5, y0 - h / 2, Str, pFont);
	}

	return true;
}

//-----------------------------------------------------------------------------
// CtrlFSliderEditor::OnMouseOver
//-----------------------------------------------------------------------------
bool CtrlFSliderEditor::OnMouseOver(SimpleDialog *pSD) {
	CtrlFSliderEditor *pSlider = (CtrlFSliderEditor *)pSD->UserParam;
	if(mouseY < pSD->y || mouseY >= pSD->y1) {
		return false;
	}
	const int dx = mouseX - pSD->x;
	if(dx < 0 || dx >= pSD->x1) {
		return false;
	}
	
	extern bool realLpressed;
	extern bool realRpressed;

	float Scale;
	if(realLpressed || realRpressed) {
		Scale = float(dx) / float(pSD->x1 - pSD->x + 1);
	}

	if(realRpressed) {
		Rpressed = false;
		if(Scale < 0.5f) {
			pSlider->m_CurPos = 0.0f;
		} else {
			pSlider->m_CurPos = 1.0f;
		}
	}
	if(realLpressed) {
		Lpressed = false;
		pSlider->m_CurPos = Scale;
	}
	return false;
} // CtrlFSliderEditor::OnMouseOver

int CtrlFSliderEditor::CreateControl(ParentFrame *pParent, int x0, int y0, int x1, int y1, BaseClass *, void *, void *pArgs, ControlParams *) {
	SimpleDialog *pSD = pParent->addViewPort(x0, y0, x1 - x0 - 1, y1 - y0);
	pSD->Enabled = pSD->Visible = 1;
	pSD->OnDraw = &OnDraw;
	pSD->OnMouseOver = &OnMouseOver;
	// Args:
	assert(pArgs);
	m_pArgs = (CtrlFSliderArgs *)pArgs;
	pSD->UserParam = int(this);
	assert(m_pArgs->DefValue >= m_pArgs->MinValue && m_pArgs->DefValue <= m_pArgs->MaxValue);
	SetValue(m_pArgs->DefValue);
	
	return y1;
}

bool CtrlFSliderEditor::Assign(xmlQuote *xml) {
	const char *pStr = xml->Get_string();
	float f = atof(pStr);
	SetValue(f);
	return true;
}

float CtrlFSliderEditor::GetValue() const {
	return m_pArgs->MinValue + m_CurPos * (m_pArgs->MaxValue - m_pArgs->MinValue);
}

void CtrlFSliderEditor::SetValue(float f) {
	if(fabs(f) == cMath::FloatMaxValue || fabs(f) == cMath::FloatMinValue) {
		f = m_pArgs->DefValue;
	}
	if(f >= m_pArgs->MinValue && f <= m_pArgs->MaxValue) {
		m_CurPos = (f - m_pArgs->MinValue) / (m_pArgs->MaxValue - m_pArgs->MinValue);
	}
}

int CtrlFSliderEditor::Get(xmlQuote *xml) {
	char Str[128];
	sprintf(Str, "%f", GetValue());
	xml->Assign_string(Str);
	return true;
}
float _fractal::GetFValue(int x,int y){		
    int GetFractalValEx(int x,int y,int Type);		
    if(Type==9){
        float s0=0;
        int n=SummOfFractals.GetAmount();
        x*=Scale;
        y*=Scale;
        for(int i=0;i<n;i++){
            s0+=SummOfFractals[i]->GetFValue(x,y)-0.5;
        }
        s0=s0*Contrast+Center;
        return s0;
    }else{
        x+=phaseX;
        y+=phaseY;
        int fv=GetFractalValEx(int(x*Scale),int(y*Scale),Type);		
        float V=float(fv)/512.0f;
        V=0.5f+(V-1.0f+Center)*Contrast;
        if(V<0.0f)V=0.0f;
        if(V>1.0f)V=1.0f;		
        return V;
    }
}
void _ModelID_editor::ReadDir(char* Pass){						
	WIN32_FIND_DATA	FD;
	HANDLE	hFindDir;
	DString root;
	root.Add(Pass);
	root.Add("*");
	hFindDir=FindFirstFile(root.pchar(),&FD);
	if(hFindDir!=INVALID_HANDLE_VALUE)
	{
		do{
			if(strcmp(FD.cFileName,".") && strcmp(FD.cFileName,"..") && !strstr(FD.cFileName,".svn")){
				if(FD.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
					DString new_root;
					new_root.Add(Pass);
					new_root.Add(FD.cFileName);
					new_root.Add("\\");
					ReadDir(new_root.pchar());
				}else{
					if(strstr(FD.cFileName,".c2m") || strstr(FD.cFileName,".C2M")){
						DString file;
						file.Add(Pass);
						file.Add(FD.cFileName);
						FileArr.Add(new DString(file.pchar()));
					}
				}
			}
		}while(FindNextFile(hFindDir,&FD));
		FindClose(hFindDir);
	}
}
bool _ModelID_editor::MinusClick(SimpleDialog* SD){
	return false;
}
bool _ModelID_editor::PlusClick(SimpleDialog* SD){
	return false;
}
int _ModelID_editor::CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
	model_id=(int*)DataPtr;
	ModelShift=int(ExtraPtr);
	strcpy(str,IMM->GetModelFileName(*model_id));
	strcpy(pstr,str);
	id=-1;
	IB=Base->addInputBox(NULL,x+3,y+4,str,120,x1-x-5,y1-y+1,&CED_Font,&CED_AFont);
	IB->Centering=true;
	int idx=GPS.PreLoadGPImage("Interf3\\btnz");
	y1+=100;
	IB->Enabled=0;
	SimpleDialog* SD=Base->addViewPort(x,y,x1-x+1,y1-y+1);
	SD->UserParam=int(this);
	p=new LS_gp_param;
	p->destname=str;
	p->ext="*.C2M";
	p->startdir="Models\\";
	p->CE=CParam->CE;
	SD->OnDraw=&ModelDraw;
	SD->OnUserClick=&LS_gp_UserClick;
	SD->AllocPtr=(char*)p;
	return y1;
}
bool _ModelID_editor::Process(){	
	*model_id=IMM->GetModelID(str);	
	return true;
}	