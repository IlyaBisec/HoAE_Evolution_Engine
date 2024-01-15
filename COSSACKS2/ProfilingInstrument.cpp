#include "stdheader.h"
#include "Extensions.h"
class TimingRoot;
class TimingElement:public BaseClass{
public:
	char Name[64];
	LARGE_INTEGER  AmountOfTime;
	LARGE_INTEGER Start;
	TimingRoot* Parent;
	TimingElement(){
		strcpy(Name,"root");
	}
	double GetTime(){
		return double(AmountOfTime.LowPart)+double(AmountOfTime.HighPart)*double(0x100000000);
	}
};
class TimingRoot;
class TimArray:public ClassArray<TimingRoot>{
public:
	int GetExpansionRules(){
		return 0;
	}
};
class TimingRoot:public BaseClass{
public:
	int Level;
	TimingRoot* Parent;
	TimingElement CurFunc;
	TimArray ChildFunc;
	SAVE(TimingRoot);
	REG_AUTO(ChildFunc);
	ENDSAVE;
	virtual const char* GetElementView(int idx);
	TimingRoot(){
		Parent=NULL;
	}
	double GetTime(){
		double T=0;
		for(int i=0;i<ChildFunc.GetAmount();i++){
			T+=ChildFunc[i]->CurFunc.GetTime();
		}
		return T;
	}
	void SetLevel(int L){
		Level=L;
		for(int i=0;i<ChildFunc.GetAmount();i++){
			ChildFunc[i]->SetLevel(L+1);
		}
	}
};
TimingRoot TRoot;
TimingRoot* TCurrent=&TRoot;
const char* TimingRoot::GetElementView(int idx){
	if(TRoot.CurFunc.AmountOfTime.LowPart){
        //static char temp[128];
		//double T0=double(CurFunc.AmountOfTime.LowPart)+double(CurFunc.AmountOfTime.HighPart)*double(0x100000000);
		//double T1=double(TRoot.CurFunc.AmountOfTime.LowPart)+double(TRoot.CurFunc.AmountOfTime.HighPart)*double(0x100000000);
		//sprintf(temp,"%s : %d%%",CurFunc.Name.pchar(),int(T0*100.0/T1));
		return CurFunc.Name; 
	}else return CurFunc.Name;
}

void StartTiming(const char* ID,LARGE_INTEGER* Time){	
	for(int i=0;i<TCurrent->ChildFunc.GetAmount();i++)if(!strcmp(TCurrent->ChildFunc[i]->CurFunc.Name,ID)){
		TCurrent=TCurrent->ChildFunc[i];
		TCurrent->CurFunc.Start=*Time;
		return;
	}
	TimingRoot* TR=new TimingRoot;
	strcpy(TR->CurFunc.Name,ID);
	TR->Parent=TCurrent;
	TCurrent->ChildFunc.Add(TR);
	TCurrent=TR;
	TCurrent->CurFunc.Start=*Time;
}

bool ZEROTIME=1;
void EndTiming(LARGE_INTEGER* Time){
	(*Time).QuadPart-=TCurrent->CurFunc.Start.QuadPart;	
	if(!ZEROTIME){		
		TCurrent->CurFunc.AmountOfTime.QuadPart+=(*Time).QuadPart;
	}
	TCurrent=TCurrent->Parent;
}
void StopAllTimers(){
	
}
void RegProfView(){
	AddStdEditor("ViewInstrumentationResults",&TRoot,"",RCE_CENTRAL_POSITION|RCE_EXITONESCAPE|RCE_EXITONENTER);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//	             COLLECTING INFO             //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

DWORD ProfBuf[PBUFSIZE];
int ProfBufPos=0;
LARGE_INTEGER TotTime;
void ProcessTimers(){
	static LARGE_INTEGER T0;
	static LARGE_INTEGER T1;
	__asm{
		push eax
		push edx
		push ecx
		rdtsc
		mov ecx,offset T1
		mov  [ecx],eax
		mov  [ecx+4],edx
		pop ecx
		pop edx
		pop eax
	}
	int p=0;
	ProfBufPos>>=2;
	while(p<ProfBufPos){
		if(ProfBuf[p+1]&0x80000000){
			//end chunk
			ProfBuf[p+1]&=0x7FFFFFFF;
			EndTiming((LARGE_INTEGER*)(ProfBuf+p));
			p+=2;
		}else{
			//start chunk
			int Line=ProfBuf[p+3];
			char* s=(char*)ProfBuf[p+2];
			char cc[64];
			if(Line)sprintf(cc,"%s - L%d",s,Line);
			else strcpy(cc,s);
			StartTiming(cc,(LARGE_INTEGER*)(ProfBuf+p));
			p+=4;
		}
	};
	ProfBufPos=0;	
	if(!ZEROTIME){
		TotTime.QuadPart+=T1.QuadPart-T0.QuadPart;
	}
	TRoot.CurFunc.AmountOfTime=TotTime;
	TRoot.SetLevel(0);
	__asm{
		push eax
		push edx
		push ecx
		rdtsc
		mov ecx,offset T0
		mov  [ecx],eax
		mov  [ecx+4],edx
		pop ecx
		pop edx
		pop eax
	}	
	ZEROTIME=!(GetKeyState(VK_NUMLOCK) & 1);
}
class TimArray_editor:public OneElementEditor{
public:	
	void CreateBar(ParentFrame* Base,int x,int y,int Lx,int Ly,char* Header,float stage,bool HDR,int Level){
		Canvas* CN=Base->AddCanvas(x,y,Lx,Ly);
		if(!HDR){			
			if(Level&1){
				CN->AddBar(1,1,Lx-3,Ly-3,0xFF202020);
				CN->AddRect(0,0,Lx-1,Ly-1,0xFFAFAFAF);
			}else{
				CN->AddBar(1,1,Lx-3,Ly-3,0xFF505050);
				CN->AddRect(0,0,Lx-1,Ly-1,0xFFFFFFF);
			}
			DWORD C=0xFF800000;//MixDWORD(0xFF008000,0xFF800000,int((100-stage)*256/100),int(stage*256/100));
			CN->AddBar(1,1,(Lx-2)*stage/100,Ly-2,C);
			char cc[32];
			sprintf(cc,"%.1f%%",stage);
			CN->AddCText(Lx/2,Ly/2-9,cc,&YellowFont);
		}else CN->AddCText(Lx/2,Ly/2-12,Header,&YellowFont);
	}
	virtual int CreateControl(ParentFrame* Base,int x,int y,int x1,int y1,BaseClass* Class,void* DataPtr,void* ExtraPtr, ControlParams* CParam){
		int Ly=y1-y-2;		
		TimingRoot TR;
		int dof=int(&TR.ChildFunc)-int(&TR);
		TimingRoot* PV=(TimingRoot*)(int(DataPtr)-dof);		

		CParam->RightColumnWidth-=PV->Level*8;
		CParam->LeftColumnWidth+=PV->Level*8;
		int Lx=CParam->RightColumnWidth-4;
		x+=PV->Level*8;

		double T0=PV->CurFunc.GetTime();
		double T1=TRoot.CurFunc.GetTime();
		double PT=PV->Parent?PV->Parent->CurFunc.GetTime():T1;
		double DT=PV->CurFunc.GetTime()-PV->GetTime();
        int d=Lx/3-4;
		x+=2;
		Lx/=3;
		CreateBar(Base,x,y+1,Lx,Ly," ",PT>0.000001?T0*100.0/PT:0,0,PV->Level+1);
		x+=Lx+4;
		CreateBar(Base,x,y+1,Lx,Ly," ",T1>0.000001?T0*100.0/T1:0,0,PV->Level+1);
		x+=Lx+4;
		CreateBar(Base,x,y+1,Lx,Ly," ",T0>0.000001?DT*100.0/T0:0,0,PV->Level+1);
		return y1;
	}
	virtual bool Assign(xmlQuote* xml){
		return true;
	}
	virtual int Get(xmlQuote* xml){				
		return true;
	}
	virtual const char* GetClassID(){return "ClassArray.TimingRoot";}
	virtual OneElementEditor* _new(){
		return (OneElementEditor*)new TimArray_editor;
	}
};
void AddTimArrayEditor(){
	type_list.Add(new TimArray_editor);		
}