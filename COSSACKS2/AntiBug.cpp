#include "stdheader.h"
#include "Extensions.h"
#pragma pack(push)
#pragma pack(8)
#include "FMM\FMManager.h"
#pragma pack(pop)
#define CHECKMODE
struct AskMove{
	word ReqID;//индекс перемещаемого
	word PreID;//индекс жаждущего дорватьс€ в эту клетку
	byte x;
	byte y;
	char dx;
	char dy;
};
extern int NAsk;// оличество запросов
extern AskMove Ask[8192];//ћассив запросов

class CSum{
	int* lpInt;
	int NInt;
	int	Summa;
public:
	void GetSum(void* lp,int n);
	void GetSum();
	bool ChkSum();
};

void CSum::GetSum(void* lp,int n){
	lpInt=(int*)lp;
	NInt=n>>2;
	GetSum();
};
void CSum::GetSum(){
#ifdef CHECKMODE
	int SM=0;
	for(int j=0;j<NInt;j++)SM+=lpInt[j];
	Summa=SM;
#endif;
};
bool CSum::ChkSum(){
	int SM=0;
	for(int j=0;j<NInt;j++)SM+=lpInt[j];
	return Summa==SM;
};
//-------------------USING THIS CLASS----------------//
static int NSum=0;
CSum SMS[64];
BigSum();
void ADDSUMM(void* lp,int n);
void GETALL();
void CHKALL();
void ADDSUMM(void* lp,int n){
	if(NSum>=64){
//		assert(NSum<64);
		return;
	};
	SMS[NSum].GetSum(lp,n);
	NSum++;
};
void GETALL(){
#ifdef CHECKMODE
	for(int i=0;i<NSum;i++)	SMS[i].GetSum();
#endif
};
//Asks array integrity checking
void CheckAsks(){
	for(int i=0;i<NAsk;i++){
		word ID=Ask[i].ReqID;
		OneObject* OB=Group[ID];
		//assert(OB&&OB->x==Ask[i].x&&Ask[i].y==OB->y);
	};
};
static dd=0;
int CheckSum;
void AddCS(void* Src,int sz){
    int cc=int(Src);
    __asm{
        push    esi
        xor     eax,eax
        mov     ecx,sz
        shr     ecx,2
        jcxz    kpp
        mov     esi,cc
kpp0:   add     eax,[esi]
        add     esi,4
        dec     ecx
        jnz     kpp0
kpp:    add     CheckSum,eax
        pop     esi
    };
};
/*
void CreateProtection(){
    CheckSumm=0;
    //Images;
    for(int i=0;i<NRMIma


};
*/
void CHKALL(){
	//for(int i=0;i<NSum;i++)assert(SMS[i].ChkSum());
	//assert(_CrtCheckMemory);

	return;
//#ifdef CHECKMODE
	//for(int i=0;i<MAXOBJECT;i++){
	//	OneObject* OB=Group[i];
	//	assert(!(OB&&OB->LocalOrder&&OB->LocalOrder->OrderType==3&&OB->EnemyID==0xFFFF));
	//};
	//for(i=0;i<MAXOBJECT;i++){
	//	OneObject* OB=Group[i];
	//	if(OB&&OB->NIndex<15){
	//		assert(!OB->capBuilding);
	//	};
	//};
//	for(int i=0;i<NSum;i++) assert(SMS[i].ChkSum());
//	if(!_CrtCheckMemory())
//		assert(0);
//	CheckAsks();
	/*for(int i=1;i<msx;i++)
		for(int j=1;j<msy;j++){
			word MID=Mops[j][i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(!OB->capBuilding)assert(OB->x==i&&OB->y==j);//
			};
		};*/
	/*for(int i=0;i<8192;i++){
		OneObject* OBJ=Group[i];
		if(int(OBJ))
			if(OBJ->Lx!=OBJ->Ly||((!OBJ->Sdoxlo)&&Mops[OBJ->y][OBJ->x]!=OBJ->Index))assert(0);
	};*/
//#endif
};
void SetSumms(){
	NSum=0;
};
int* WayX=NULL;
int* WayY=NULL;
int NWay=0;
bool WayMode=0;
void DrawWay(){
	if(!WayMode)return;
	int dx=mapx<<5;
	int dy=mapy<<4;
	for(int i=1;i<NWay;i++)
		DrawLine(WayX[i-1]-dx,WayY[i-1]-dy,WayX[i]-dx,WayY[i]-dy,255);
};
void AddWPoint(int x,int y){
	if(!WayMode)return;
	WayX=(int*)realloc(WayX,(NWay+1)*4);
	WayY=(int*)realloc(WayY,(NWay+1)*4);
	WayX[NWay]=x;
	WayY[NWay]=y;
	NWay++;
};
bool FEX_GetDebugInfo(unsigned Base, unsigned Address, unsigned* pLineNum, char* ppFunc, char* ppSrc);
int FEX_GetCallStackEBP(unsigned* CallStack, int MaxDepth);
int FEX_GetCallStackEBP_Ex(unsigned,unsigned* CallStack, int MaxDepth);

#define GEC_NO_ASSERT

DIALOGS_API
void SmartAssert(const char* File,int Line,const char* expression){
#ifndef GEC_NO_ASSERT
	static _str s;
	s.Clear();
	s.print("Assertion failure (%s) :\nfile: %s, line %d\n\nCall stack state:\n",expression,File,Line);	
	unsigned buff[64];
	int N=FEX_GetCallStackEBP(buff,64);
	for(int i=0;i<64;i++){
		_str s1;
		unsigned LineNum;
		char Func[1024];
		char FileName[1024];
		if(FEX_GetDebugInfo(0x400000,buff[i],&LineNum,Func,FileName)){
			if(LineNum!=-1){
				char* ss=strstr(FileName,"cossacks ii\\");
				if(ss)strcpy(FileName,ss+strlen("cossacks ii\\"));
			    s1.print("%s %s %d\n",Func,FileName,LineNum);
				s+=s1.pchar();
			}
		}
	}
	if(MessageBox(hwnd,s.pchar(),"Assertion!",MB_ICONWARNING|MB_RETRYCANCEL)==IDCANCEL){
		__debugbreak();
        //exit(0);
	}
#endif //GEC_NO_ASSERT
}
extern char FEX_VersionString[];
void PrintLineInfo(DWORD ptr,char* dest){
	dest[0]=0;
	unsigned LineNum;
	char Func[1024];
	char FileName[1024];
	if(FEX_GetDebugInfo(0x400000,ptr,&LineNum,Func,FileName)){
		if(LineNum!=0xFFFFFFFF){
			char* ss=strstr(FileName,"cossacks ii\\");
			if(ss)strcpy(FileName,ss+strlen("cossacks ii\\"));
			sprintf(dest,"%s %s %d",Func,FileName,LineNum);						
		}
	}
}
void clog(char* s){
	FILE* F=fopen("Crash\\crash_in_crash.log","a");
	if(F){
		fprintf(F,"%s",s);
		fclose(F);
	}
}
bool CrashBox(char* Header,unsigned _ebp,unsigned _eip){
	FILE* F=fopen("Crash\\crash_in_crash.log","w");
	if(F){
		int FMM_GetNHandlers();
		int FMM_GetAllocatedBlocks();
		int FMM_GetFreeMem();
		int FMM_GetBlocks();
		fprintf(F,"handlers: %d\nptrs: %d\nmemused: %d\nmemtotal: %d\n",FMM_GetNHandlers(),FMM_GetAllocatedBlocks(),FMM_GetFreeMem(),FMM_GetBlocks()*4096);
		fclose(F);
	}	
	int NSF,NEF,NEXC;
	int CheckMemoryIntegrityEx(int&,int&,int&);
	int NE=CheckMemoryIntegrityEx(NSF,NEF,NEXC);
	clog("CheckMemoryIntegrityEx: done\n");
	static char s2[8192];
	if(NE==0){
		sprintf(s2,"Version:%s\nMemory status: OK\n\nCall stack state after crash:\n\n",FEX_VersionString);	
	}else{
		sprintf(s2,"Version:%s\nMemory status: integrity failure! (%d errors)\nStart chunk fails: %d\nEnd chunk fails: %d\nExceptions: %d\n\nCall stack state after crash:\n\n",FEX_VersionString,NE,NSF,NEF,NEXC);	
	}
	int FMM_GetNHandlers();
	int FMM_GetAllocatedBlocks();
	int FMM_GetFreeMem();
	int FMM_GetBlocks();
	sprintf(s2+strlen(s2),"Memory info:\nHandlers: %d\nPointers: %d\nUsedMemory: %d\nTotalMemory: %d\n\n",FMM_GetNHandlers(),FMM_GetAllocatedBlocks(),FMM_GetFreeMem(),FMM_GetBlocks()*4096);	
	clog("mem info: done\n");
	static unsigned buff[64];
	buff[0]=_eip;	
	int N=FEX_GetCallStackEBP_Ex(_ebp,buff+1,63);	
	clog("FEX_GetCallStackEBP_Ex: done\n");
	bool present=0;
	for(int i=0;i<24;i++){		
		unsigned LineNum;
		static char Func[1024];
		static char FileName[1024];
		if(FEX_GetDebugInfo(0x400000,buff[i],&LineNum,Func,FileName)){
			if(LineNum!=-1){
				char* ss=strstr(FileName,"cossacks ii\\");
				if(ss)strcpy(FileName,ss+strlen("cossacks ii\\"));
				sprintf(s2+strlen(s2),"%s %s %d\n", Func,FileName,LineNum);				
				present=1;
			}
		}
	}
	clog("print call stack: done\n");
	if(!present){		
		sprintf(s2+strlen(s2),".PDB file missed of stack failed\n");
	}
	FILE* F2;
	F2=fopen("\\\\ANDREW-STRATEG\\Exchange\\bugzdump\\fastcrash.txt","a");	
	if(F2){
		char nbuf[64]="?";
		DWORD L=63;
		GetComputerName(nbuf,&L);
		SYSTEMTIME ST;
		GetLocalTime(&ST);
		fprintf(F2,"%d.%d.%d %02d:%02d  %s:%s\n\n%s\n\n",ST.wDay,ST.wMonth,ST.wYear,ST.wHour,ST.wMinute,nbuf,Header,s2);
		fclose(F2);
	}else clog("unable to open dump file.\n");
	clog("fastcrash dump: done\n");
	MessageBox(hwnd,s2,Header,MB_ICONSTOP|MB_SYSTEMMODAL);	
	return true;
}
bool CrashStackToFile(const char* FileName,unsigned _ebp,unsigned _eip)
{
	unsigned Version = 1;
	const unsigned MaxStack = 64;
	unsigned buff[MaxStack];
	memset(buff,0,sizeof(unsigned)*MaxStack);
	buff[0]=_eip;
	int N=FEX_GetCallStackEBP_Ex(_ebp,buff+1,MaxStack-1);
	FILE* FEX_dumpfile = NULL;
	if((FEX_dumpfile = fopen(FileName, "wt")) != NULL)
	{
		fprintf(FEX_dumpfile, "%d\n", Version);
		for(int j = 0; j < N; j++)
			fprintf(FEX_dumpfile, "%08X\n", buff[j]);
		fclose(FEX_dumpfile);	
		return true;
	}
	return false;
}

#include "stdheader.h"
class OneMemRoot;
class MRootList:public ClonesArray<OneMemRoot>{
public:
	bool ForceSimplification(){return true;}
	virtual int GetExpansionRules(){
		return 0;
	}

};
bool GetFastDebugInfo(unsigned Address, unsigned* pLineNum, char* ppFunc, char* ppSrc){
	SAFENEW;
    Enumerator* dL=ENUM.Get("DEBUG_LINES");
	Enumerator* dF=ENUM.Get("DEBUG_FUNC");
	Enumerator* dS=ENUM.Get("DEBUG_SRC");
	char cc[128];
	sprintf(cc,"%X",Address);
	char* L=(char*)dL->Get(cc);
	if((DWORD)L!=0xFFFFFFFF){
        *pLineNum=atoi(L);
		strcpy(ppFunc,(char*)dF->Get(cc));
		strcpy(ppSrc,(char*)dS->Get(cc));
	}else{
        FEX_GetDebugInfo(0x400000,Address,pLineNum,ppFunc,ppSrc);
		char LN[32];
        sprintf(LN,"%d",*pLineNum);
		char* L=new char[strlen(LN)+1];
		strcpy(L,LN);
		dL->Add(cc,(DWORD)L);
		char* F=new char[strlen(ppFunc)+1];
		strcpy(F,ppFunc);
		dF->Add(cc,(DWORD)F);
		char* S=new char[strlen(ppSrc)+1];
		strcpy(S,ppSrc);
		dS->Add(cc,(DWORD)S);		
	}
	ENDNEW;
	return true;
}
class OneMemRoot:public BaseClass{
public:
	MRootList Root;
	DWORD Address;
	int PrevSize;
	int PrevEntries;
	int  Size;
	int  NEntries;
	SAVE(OneMemRoot);
	REG_AUTO(Root);
	ENDSAVE;
	const char* GetThisElementView(const char*){
		unsigned LineNum=0;
		char Func[1024]="???";
		char FileName[1024]="???";
		char res[1024]="unknown";
		char cmd[512]="";
		if(GetFastDebugInfo(Address,&LineNum,Func,FileName)){
			if(LineNum!=-1){									
				sprintf(cmd,"{R FFFF0000 $%d %s}",LineNum,FileName);
				char* ss=strstr(FileName,"cossacks ii\\");
				if(ss)strcpy(FileName,ss+strlen("cossacks ii\\"));									
				char cc[256];
				sprintf(res,"%s %d [%s]\n",FileName,LineNum,Func);
			}
		}
		char* s=GetGlobalBuffer();		
		char* s1=res;
		int L=strlen(s1)-1;
		while(L>0&&s1[L]!='\\')L--;
		if(s1[L]=='\\')L++;
		s1+=L;
		char sz[64];
		char ent[64];
		if(Size>PrevSize){
			sprintf(sz," %.2f {CR}(+%.2f)",float(Size)/1000000,float(Size-PrevSize)/1000000);
		}
		if(Size<PrevSize){
			sprintf(sz," %.2f {CG}(-%.2f)",float(Size)/1000000,float(-Size+PrevSize)/1000000);
		}
		if(Size==PrevSize){
			sprintf(sz," %.2f ",float(Size)/1000000);
		}
		if(NEntries>PrevEntries){
			sprintf(ent,"{CY}[%d ({CR}+%d{CY})]",NEntries,NEntries-PrevEntries);
		}
		if(NEntries<PrevEntries){
			sprintf(ent,"{CY}[%d ({CG}-%d{CY})]",NEntries,-NEntries+PrevEntries);
		}
		if(NEntries==PrevEntries){
			sprintf(ent,"{CY}[%d]",NEntries,NEntries-PrevEntries);
		}
		sprintf(s,"%s %s{C} %s%s{C}",sz,ent,cmd,s1);		
		return s;
	}
	OneMemRoot* Add(DWORD Address,int Size){
		int N=Root.GetAmount();
		for(int i=0;i<N;i++){
			if(Root[i]->Address==Address){
				Root[i]->Size+=Size;
				Root[i]->NEntries++;
				return Root[i];
			}
		}
		OneMemRoot* mr=new OneMemRoot;
		mr->Address=Address;
		mr->Size=Size;
		mr->NEntries=1;
		Root.Add(mr);
		return mr;
	}
	void Sort(){
        bool change=1;
		do{
			change=0;
			for(int i=1;i<Root.GetAmount();i++){
				if(Root[i-1]->Size-Root[i-1]->PrevSize<Root[i]->Size-Root[i]->PrevSize){
					swap(Root[i],Root[i-1]);
					change=1;
				}
			}
		}while(change);
		for(int i=0;i<Root.GetAmount();i++){
			Root[i]->Sort();
		}
	}
	void PreClear(){
		for(int i=0;i<Root.GetAmount();i++){
			Root[i]->PrevSize=Root[i]->Size;
			Root[i]->PrevEntries=Root[i]->NEntries;
			Root[i]->Size=0;
			Root[i]->NEntries=0;
			Root[i]->PreClear();
		}
	}
};
OneMemRoot AllLeaks;
extern FMManager MManager;
extern int LeakStackSize;
extern bool SmartLeakDoctor;
extern char GlobalTextMouseOverCommand[256];
bool ReadWinString(GFILE* F,char* STR,int Max);
#define NShowLines 24
#define LineDist 16
#define DbgFont WhiteFont
void ShowSeveralStrings(char* File,int Line){
	static char prevFile[256]="";
	static int prevLine=0;
	static char CurInfo[NShowLines][256];
	static int lIdx=0;
	if(strcmp(File,prevFile)||Line!=prevLine){
		prevLine=Line;
        strcpy(prevFile,File);
		memset(&CurInfo,0,sizeof CurInfo);
		GFILE* F=Gopen(File,"r");
		if(F){
			int nskip=0;
			char cc[512];
			int nh=NShowLines/2;
			for(int j=0;j<Line-nh;j++){
				ReadWinString(F,cc,512);
				nskip++;
			}
			for(int j=0;j<NShowLines;j++){
				ReadWinString(F,CurInfo[j],255);
                if(nskip+j+1==Line)lIdx=j;
			}
            Gclose(F);
		}
	}
	for(int i=0;i<NShowLines;i++){
		if(i==lIdx)GPS.DrawFillRect(0,i*LineDist,RealLx,LineDist,0x60FF0000);
		ShowString(0,i*LineDist,CurInfo[i],&DbgFont);
	}
}
bool PrepareToShowLeaks(ClassEditor* CE,BaseClass* BC,int Options){
	if(Options==2){
		if(GlobalTextMouseOverCommand[0]){
			GPS.DrawFillRect(0,0,1024,NShowLines*LineDist,0x90000000);
			//ShowString(0,0,GlobalTextMouseOverCommand,&WhiteFont);
			char* sp=strstr(GlobalTextMouseOverCommand," ");
			if(sp){
                sp++;
				ShowSeveralStrings(sp,atoi(GlobalTextMouseOverCommand+1));
			}
			GlobalTextMouseOverCommand[0]=0;
			
		}
	}
	if(Options==3){
		AllLeaks.PreClear();
	}
	if(Options==1){
		extern bool ForcedSmartLeakDoctor;
		SmartLeakDoctor=0;
		ForcedSmartLeakDoctor=false;
		REG_CLASS(OneMemRoot);
		//AllLeaks.Root.Clear();
		FMManager::FMMHANDLER* HPtr=MManager.Handlers;
		int nt=0;
		for(int i=0;i<FMM_MAXHASHITEMS&&nt<11114500;i++){
			try{
				FMManager::FMMHANDLER* HPtr=MManager.HashTableA[i];
				while(HPtr&&nt<11114500){
					if(!HPtr->isFree){
						try{
							byte* ptr=HPtr->BaseAddress;
							byte* ptr0=ptr;
							ptr+=LeakStackSize*4;
							int size=((DWORD*)ptr)[0];
							if(((DWORD*)ptr)[1]=='DOCT'){													
								nt++;
								unsigned* LEAK=(unsigned*)(ptr0);
								OneMemRoot* MR=&AllLeaks;
								for(int i=0;i<LeakStackSize;i++)if(LEAK[LeakStackSize-1-i]){							
									MR=MR->Add(LEAK[LeakStackSize-1-i],size);
								}
							}
						}catch(...){};
					}
					HPtr=HPtr->SubNextA;
				}
			}catch(...){}
		}
		SmartLeakDoctor=1;
		ForcedSmartLeakDoctor=true;
		AllLeaks.Sort();
	}
	return false;
}
void RegShowLeaks(){
	AddStdEditor("PrintLeaks",&AllLeaks,"",RCE_BOTTOM|RCE_AUTOSAVE|RCE_EXITONESCAPE|RCE_EXITONENTER,&PrepareToShowLeaks);	
}
