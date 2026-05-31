#include "stdheader.h"
//#define RECDEBUG
//#include "UnSyncro.h"
#ifndef CHECK_SYNC_FAILURE
#define NOWORK
#endif
int prevrtime=0;
FILE* FX=NULL;
extern bool RecordMode;
extern byte PlayGameMode;
char CURLOG[32];
void CleanRec(){
#ifdef RECDEBUG
	if(FX){
		fclose(FX);
		if(PlayGameMode){
			FX=fopen("lox1.log","w");
			strcpy(CURLOG,"lox1.log");
		}else{ 
			FX=fopen("lox0.log","w");
			strcpy(CURLOG,"lox0.log");
		};
	};
	FX=NULL;
#endif
};
void WriteRec(char* s){
	if(!(PlayGameMode||RecordMode))return;
	if(GetTickCount()-prevrtime>10000){
		if(FX){
			fclose(FX);
			if(PlayGameMode){
				FX=fopen("lox1.log","a");
				strcpy(CURLOG,"lox1.log");
			}else{ 
				FX=fopen("lox0.log","a");
				strcpy(CURLOG,"lox0.log");
			};
		};
		prevrtime=GetTickCount();
	};
	if(!FX){
		if(PlayGameMode){
			FX=fopen("lox1.log","a");
			strcpy(CURLOG,"lox1.log");
		}else{ 
			FX=fopen("lox0.log","a");
			strcpy(CURLOG,"lox0.log");
		};
	};
	fprintf(FX,s);
};
Syncro::Syncro(){
#ifdef NOWORK
	return;
#endif
	NSlots=0;
	DeleteFile("trace.log");

	//Outf=fopen("trace.log","w");
};
Syncro::~Syncro(){
#ifdef NOWORK
	return;
#endif
	if(Outf){
		fclose(Outf);
	};
};
extern int tmtmt;
extern DWORD RealTime;
int SyncroDone=0;
void Syncro::Save(){
#ifdef NOWORK
	return;
#endif
	//if(SyncroDone>5)return;
	char cname[128];
	DWORD L=128;
	GetComputerName(cname,&L);
	char s1[256];
	SYSTEMTIME ST;
	GetLocalTime(&ST);	
	//sprintf(s1,"\\\\-GEC-\\Exchange\\trace.log\\trace_%d_%d_%d_%d_%s.log",ST.wDay,ST.wMonth,ST.wHour,ST.wMinute,cname);
	Outf=fopen("trace.log","a");
	//Outf=fopen(s1,"a");
	if(Outf){
		fprintf(Outf,"********[%d]*******\n",RealTime);
		for(int i=0;i<NSlots;i++){
			if(RSL[i].Type==0 && RSL[i].Param!=0xFFFF){
				OneObject* OB=Group[RSL[i].Param];
				char cc1[128];
				if(OB){
					sprintf(cc1,"%s(%d)",OB->Ref.General->MonsterID,OB->Index);
					if(OB)fprintf(Outf,"%s %d %d %s\n",RSL[i].FileName,RSL[i].Line,RSL[i].rpos,cc1);
					else fprintf(Outf,"%s %d %d ???\n",RSL[i].FileName,RSL[i].Line,RSL[i].rpos);
				}else fprintf(Outf,"%s %d %d %d\n",RSL[i].FileName,RSL[i].Line,RSL[i].rpos,RSL[i].Param);
			}else fprintf(Outf,"%s %d %d %d\n",RSL[i].FileName,RSL[i].Line,RSL[i].rpos,RSL[i].Param);
		};
		fclose(Outf);
		SyncroDone++;
		//Outf=fopen("trace.log","a");
	};
	
	Clear();
};
void Syncro::Clear(){
#ifdef NOWORK
	return;
#endif
	NSlots=0;
};

char* its(int i){
#ifdef NOWORK
	return NULL;
#endif
	char xxx[16];
	sprintf(xxx,"@%d",i);
	char* z=znew(char,strlen(xxx)+1);
	strcpy(z,xxx);
	return z;
};
void Syncro::Copy(Syncro* Syn){
	memcpy(Syn->RSL,RSL,NSlots*sizeof RandSlot);
	Syn->NSlots=NSlots;
	NSlots=0;
};
Syncro SYN;
Syncro SYN1;
extern short randoma[8192];
extern char LASTFILE[128];
extern int LastLine;
char* LastFile;
DLLEXPORT
int RandNew(char* File,int Line){
	LastFile=File;
	LastLine=Line;
	int r=randoma[rpos];
	rpos=(rpos+1)&8191;
	if(SYN.NSlots<maxUline){
		RandSlot* RS=SYN.RSL+SYN.NSlots;
		RS->FileName=File;
		RS->Line=Line;
		RS->Param=0;
		RS->rpos=rpos;
		RS->Type=-1;
		SYN.NSlots++;
	};
#ifdef RECDEBUG
	char cc[128];
	sprintf(cc,"%s %d %d 1\n",File,Line,rpos);
	WriteRec(cc);
#endif
	return r;
};
DLLEXPORT
void AddRandNew(char* File,int Line,int Param){
	LastFile=File;
	LastLine=Line;
	rpos=(rpos+Param)&8191;
	if(SYN.NSlots<maxUline){
		RandSlot* RS=SYN.RSL+SYN.NSlots;
		RS->FileName=File;
		RS->Line=Line;
		RS->Param=Param;
		RS->rpos=rpos;
		RS->Type=-1;
		SYN.NSlots++;
	};
#ifdef RECDEBUG
	char cc[128];
	sprintf(cc,"%s %d %d %d\n",File,Line,rpos,Param);
	WriteRec(cc);
#endif
};
DLLEXPORT
void AddUN(char* File,int Line,int Param,int Type){
	LastFile=File;
	LastLine=Line;
	rpos=(rpos+Param)&8191;
	if(SYN.NSlots<maxUline){
		RandSlot* RS=SYN.RSL+SYN.NSlots;
		RS->FileName=File;
		RS->Line=Line;
		RS->Param=Param;
		RS->rpos=rpos;
		RS->Type=Type;
		SYN.NSlots++;
	};
#ifdef RECDEBUG
	char cc[128];
	sprintf(cc,"%s %d %d %d\n",File,Line,rpos,Param);
	WriteRec(cc);
#endif
};