#include "stdheader.h"
#define DE_LeftLx 500
extern int RealLx;
extern int RealLy;
int EnumerateSelectedFrames(ClassArray<SimpleDialog>* DSS,SimpleDialog** SDS,int max);
EditDialog Patient;
void ProcessDialogsEditor(){
	DialogsSystem DE;
	DialogsSystem DE2;	
	Patient.Dialog.x=0;//DE_LeftLx;
	Patient.Dialog.EditMode=1;
	ColoredBar* CB1=DE.addColoredBar(0,0,DE_LeftLx,RealLy,0xFF);
	CB1->Diffuse=0xFF606060;
	ComplexClassEditor CCE;
	CCE.Create(0,0,DE_LeftLx-1-10,RealLy,40,&Patient);
	extern int LastKey;
	LastKey=0;
	extern int GameMode;
	GameMode=1;
	void* PrevSelPtr=NULL;
	int FloatStage=DE_LeftLx;
	bool PanelL=1;
	
	extern bool InAllEditors;
	InAllEditors=true;

	do{
		Patient.Dialog.SetEditMode(true);
		if((GetKeyState(VK_NUMLOCK )&0x7FFF)){
			/*
			if(PanelL){
				if(mouseX<=FloatStage){
					if(FloatStage<DE_LeftLx)FloatStage+=20;
					if(FloatStage>DE_LeftLx)FloatStage=DE_LeftLx;
				}else{
					if(FloatStage>0)FloatStage-=20;
					if(FloatStage<0)FloatStage=0;
				}
				if(mouseX>=RealLx-8){
					PanelL=0;
				}
			}else{
				if(mouseX>=RealLx-FloatStage-8){
					if(FloatStage<DE_LeftLx)FloatStage+=20;
					if(FloatStage>DE_LeftLx)FloatStage=DE_LeftLx;
				}else{
					if(FloatStage>0)FloatStage-=20;
					if(FloatStage<0)FloatStage=0;
				}
				if(mouseX<=0){
					PanelL=1;
				}
			}
			*/
			FloatStage=20;
		}else{
			FloatStage=DE_LeftLx;
		}
		if(PanelL){			
			if(mouseX>=RealLx-8){
				PanelL=0;
			}
		}else{
			if(mouseX<=0){
				PanelL=1;
			}
		}
		if(PanelL){
			CCE.TopDS.x=FloatStage-DE_LeftLx;
			CCE.BottomDS.x=FloatStage-DE_LeftLx;
			Patient.Dialog.x=0;//FloatStage;
		}else{
			CCE.TopDS.x=RealLx-FloatStage;
			CCE.BottomDS.x=RealLx-FloatStage;
			Patient.Dialog.x=0;
		}
		
		ProcessMessages();	
		if(bActive){
			GPS.DrawFillRect(0,0,RealLx,RealLy,0xFF404040);			
			bool LP=Lpressed;			
			GPS.FlushBatches();
			extern bool LockManualDialogsShift;
			bool CheckActiveDSS(ClassArray<SimpleDialog>* DSS);
            LockManualDialogsShift=CheckActiveDSS(&CCE.TopDS.DSS)||CheckActiveDSS(&CCE.BottomDS.DSS);
			if((PanelL&&mouseX<FloatStage)||((!PanelL)&&mouseX>RealLx-FloatStage)){
				Lpressed=0;
				void UnPress();
				UnPress();
			}else LockManualDialogsShift=0;
			Patient.Dialog.ProcessDialogs();
			Lpressed=LP;
			CCE.Process();

			//CCE.TopDS.RefreshView();
			if(CCE.Top.CurElement!=-1){
				void* ptr=CCE.Top.ElParams[CCE.Top.CurElement]->DataPtr;
				if(ptr!=PrevSelPtr){
					PrevSelPtr=ptr;
					void DeselectDSS(ClassArray<SimpleDialog>* DSS);
					DeselectDSS(&Patient.Dialog.DSS);
					void SelectDialogByPtr(ClassArray<SimpleDialog>* DSS,void* ptr);
					SelectDialogByPtr(&Patient.Dialog.DSS,ptr);
					void DeactivateDSS(ClassArray<SimpleDialog>* DSS);
					DeactivateDSS(&CCE.TopDS.DSS);					
					DeactivateDSS(&CCE.BottomDS.DSS);					
				}
			}	
			SimpleDialog* SDS[8];
			int N=EnumerateSelectedFrames(&Patient.Dialog.DSS,&SDS[0],8);
			if(N==1){
				CCE.Top.ClearActive();
				CCE.Bottom.ClearActive();
				bool ch=0;
				ParentFrame* SD0=SDS[0];

				while(SD0){
					SD0=SD0->ParentDS;
					if(SD0){
						CCE.Top.ExpInf.Add(SD0);

						if(CCE.Top.ExpInf.find(SD0)==-1){
							CCE.Top.ExpInf.Add(SD0);						
							ch=1;
						}
						if(CCE.Top.ExpInf.find(&SD0->DSS)==-1){
							CCE.Top.ExpInf.Add(&SD0->DSS);
							ch=1;
						}
					}					
				}
				SD0=SDS[0];				
				if(ch){
					CCE.Top.ExpInfChanged=1;
					CCE.Process();
				}				
				for(int i=0;i<CCE.Top.ElParams.GetAmount();i++){
					if(CCE.Top.ElParams[i]->DataPtr==(void*)SDS[0]){
						CCE.Top.ElParams[i]->NowActive=1;
						CCE.Top.CurElement=CCE.Top.ElParams[i]->Index;
						CCE.Top.ElParams[i]->CBar->Diffuse=CED_SelFieldColor;	
					}
				}
			}
			if( LastKey=='T' && (GetKeyState(VK_CONTROL)&0x8000) && (GetKeyState(VK_SHIFT)&0x8000) && (GetKeyState(VK_MENU)&0x8000) ){
				void PrintTex();
				PrintTex();
				LastKey=0;
			}
			if(/*LastKey==13||*/LastKey==27)break;			
		};
        FlipPages();
	}while(true);
	LastKey=0;
	//Patient.Dialog.DSS.Clear();

	//extern bool ResPanInit;
	//ResPanInit=false;

	InAllEditors=false;
}