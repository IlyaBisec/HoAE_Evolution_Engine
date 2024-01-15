#include "stdheader.h"
#define NTicks 100
#define NOLAG
#define USESET 5
DWORD PF_Average[NTicks];
DWORD PF_Prev[NTicks];
int PF_PrevTime=0;
LARGE_INTEGER PF_Dump[NTicks];
LARGE_INTEGER PF_Start[NTicks];
LARGE_INTEGER PF_Finish[NTicks];
char* PF_Message[NTicks];
char* Messages[NTicks];
int NTimers;
class PFRM{
public:
	PFRM();
//	~PFRM();
};
PFRM CCCL;
PFRM::PFRM(){
	memset(PF_Average,0,sizeof PF_Average);
	memset(PF_Prev,0,sizeof PF_Prev);
	memset(PF_Start,0,sizeof PF_Start);
	memset(PF_Finish,0,sizeof PF_Start);
	memset(PF_Dump,0,sizeof PF_Dump);
	memset(PF_Message,0,sizeof PF_Message);
};
CEXPORT
void RunPF(int i,const char* Desc){
#ifndef NOLAG
	if((i>>8)!=USESET)return;
	i&=255;
	QueryPerformanceCounter(&PF_Start[i]);
	PF_Message[i]=(char*)Desc;
#endif
};
CEXPORT
void StopPF(int i){
#ifndef NOLAG
	if((i>>8)!=USESET)return;
	i&=255;
	LARGE_INTEGER LI;
	QueryPerformanceCounter(&LI);
	PF_Finish[i].QuadPart+=LI.QuadPart-PF_Start[i].QuadPart;
#endif
};
extern int RealLx;
void ShowPF(){
#ifdef NOLAG
	return;
#endif
//	StopPF(13);
	char cc[128];
	int max=0;
	for(int i=0;i<100;i++)if(PF_Start[i].LowPart||PF_Start[i].HighPart)max=i+1;
	LARGE_INTEGER FREQ;
	if(!QueryPerformanceFrequency(&FREQ))return;
	//FREQ.QuadPart=Int64ShraMod32(FREQ.QuadPart,10);
	int DX=100;
	int DY=60;
	GPS.DrawFillRect(RealLx-DX-160,DY-10,90+160,max*15+20+20,0x60000000);
	for(int i=0;i<max;i++){
		LARGE_INTEGER DD;
		if(PF_Finish[i].HighPart||PF_Finish[i].LowPart){
			PF_Dump[i]=PF_Finish[i];
		}
		DD.QuadPart=PF_Dump[i].QuadPart;//-PF_Start[i].QuadPart;
		int dt=MulDiv(DD.LowPart,1000000,FREQ.LowPart);
		dt*=100;
		if(!PF_Average[i])PF_Average[i]=dt/100;
		int TT=PF_Average[i]=(PF_Average[i]*30+dt)/31;
		TT/=10000;		
		sprintf(cc,"%d :  %d.%d",i,TT/10,TT%10);		
		if(TT>500){
			GPS.DrawFillRect(RealLx-DX-160,DY+i*15,90+160,15,0xFFFF0000);
		}else
		if(TT>250){
			GPS.DrawFillRect(RealLx-DX-160,DY+i*15,90+160,15,0xA0FFFF00);
		}else
		if(TT>100){
			GPS.DrawFillRect(RealLx-DX-160,DY+i*15,90+160,15,0x8000FF00);
		}else
		if(TT>50){
			GPS.DrawFillRect(RealLx-DX-160,DY+i*15,90+160,15,0x400000FF);
		};	
					
		if(PF_Prev[i]>=PF_Average[i])ShowString(RealLx-DX,DY+i*15,cc,&SmallWhiteFont);
		else ShowString(RealLx-DX,DY+i*15,cc,&SmallYellowFont);
		if(PF_Message[i])ShowString(RealLx-DX-5-GetRLCStrWidth(PF_Message[i],&SmallYellowFont),DY+i*15,PF_Message[i],&SmallYellowFont);
		PF_Finish[i].HighPart=0;
		PF_Finish[i].LowPart=0;
	};
	if(GetTickCount()-PF_PrevTime>4000){
		for(int i=0;i<max;i++){
			PF_Prev[i]=(PF_Average[i]*130)/100;
		};
		PF_PrevTime=GetTickCount();
	};
	//RunPF(13);
};
int AddTime=0;
int NeedAddTime=0;
int PREVT=0;
int GetTicksEx(){
	int T=GetTickCount();
	//T+=T>>10;
	return T;
};
int GetDADDT(int DT){
	DT>>=7;
	if(DT<50)return 1;
	if(DT<100)return 2;
	if(DT<150)return 3;
	if(DT<220)return 4;
	if(DT<350)return 5;
	if(DT<400)return 6;
	if(DT<600)return 7;
	if(DT<1000)return 10;
	return 20;
};

int GetAbsoluteRealTime(){
	//return GetTickCount();
	/*
	LARGE_INTEGER FREQ;
	if(!QueryPerformanceFrequency(&FREQ))return GetTickCount();
	LARGE_INTEGER DD;
	QueryPerformanceCounter(&DD);
	double Freq=double(FREQ.LowPart)+double(FREQ.HighPart)*double(0x100000000);
	double Time=double(DD.LowPart)+double(DD.HighPart)*double(0x100000000);
	return int((Time*1000)/Freq);
	*/
	return GetTickCount();//GetTicksEx();
};
PerfGraph::PerfGraph(){
	memset(&History,0,sizeof History);
	memset(&HStart,0,sizeof HStart);
}
void PerfGraph::Start(int Kind){
	if(Kind<MaxGraphColumns){
		LARGE_INTEGER S;
		QueryPerformanceCounter(&S);
		HStart[Kind]=S;
	}
}
void PerfGraph::Start(int Kind,int SubBranch){
    if(Kind<MaxGraphColumns && SubBranch<MaxSubBranches-1){
        LARGE_INTEGER S;
        QueryPerformanceCounter(&S);
        HStart[Kind+(SubBranch+1)*MaxGraphColumns]=S;
    }    
}
void PerfGraph::End(int Kind){
	if(Kind<MaxGraphColumns){
		LARGE_INTEGER S;
		QueryPerformanceCounter(&S);
		S.QuadPart-=HStart[Kind].QuadPart;
		History[0][Kind]+=(S.LowPart)>>8;
		//HStart[Kind]=0;
	}	
}
void PerfGraph::End(int Kind,int SubBranch){
    if(Kind<MaxGraphColumns && SubBranch<MaxSubBranches-1){
        LARGE_INTEGER S;
        QueryPerformanceCounter(&S);
        int K=Kind+(SubBranch+1)*MaxGraphColumns;
        S.QuadPart-=HStart[K].QuadPart;
        History[0][K]+=(S.LowPart)>>8;        
    }	
}
void PerfGraph::New(){
	for(int i=GraphStackSize-1;i>0;i--){
		for(int j=0;j<MaxGraphColumns*MaxSubBranches;j++){
			History[i][j]=History[i-1][j];
		}        
	}	
    for(int j=0;j<MaxGraphColumns*MaxSubBranches;j++){
        History[0][j]=0;
    }
}
DWORD GCOLORS[]={
	0xFFFF0000,
	0xFF0000FF,
	0xFF00FF00,
	0xFFFFFF00,
	0xFF00FFFF,
	0xFFFF00FF,
	0xFFFFFFFF,
	0xFF000000
};
void PerfGraph::Draw(int x,int y,int Lx){
	//Lx--;
	for(int i=0;i<GraphStackSize;i++){
		int S=0;
		for(int j=0;j<MaxGraphColumns;j++){
			S+=History[i][j];
		}
		if(S){
			int pp=0;
			int p=0;
			int cs=0;
			for(int j=0;j<MaxGraphColumns;j++)if(History[i][j]){
                int ssub=0;
                for(int q=1;q<MaxSubBranches;q++){
                    ssub+=History[i][j+q*MaxGraphColumns];
                }
                if(ssub){
                    cs+=History[i][j]-ssub;
                    int L=Lx*cs/S;
                    GPS.DrawLine(x+1+pp,y+1+i,x+1+L,y+1+i,GCOLORS[j]);
                    pp=L;
                    for(int q=1;q<MaxSubBranches;q++){
                        cs+=History[i][j+q*MaxGraphColumns];
                        int L=Lx*cs/S;
                        GPS.DrawLine(x+1+pp,y+1+i,x+1+L,y+1+i,MulDWORD(GCOLORS[j],256-(q&1)*128-q*32)|0xFF000000);
                        pp=L;
                    }
                }else{
				    cs+=History[i][j];
				    int L=Lx*cs/S;
				    GPS.DrawLine(x+1+pp,y+1+i,x+1+L,y+1+i,GCOLORS[j]);
				    pp=L;
                }
			}
		}
	}	
	GPS.DrawRect(x,y,Lx+1,GraphStackSize+1,0xFFFFFFFF);
}
PerfGraph PGR1;