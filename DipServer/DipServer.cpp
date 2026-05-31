// DS_Server.cpp : Defines the entry point for the DLL application.
//

#include "..\COSSACKS2\common.h"
#include "Import.h"
#include "..\IntExplorer\ParseRQ.h"
#include "CommonDip.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	return TRUE;
};
CEXPORT
void RegisterDipClasses(){
	REG_CLASS(SetlUpgrade);
	REG_CLASS(SettlementInfo);
}
class OneDS_Request{
public:
	char WIN[16];
	DWORD Handle;
	ParsedRQ REQ;
	int curstage;
	int progress;
	ParsedRQ Result;
	bool Ready;
	char* RESULT;
	int RSIZE;
	OneDS_Request();
	~OneDS_Request();
};
class DS_Requests{
public:
	OneDS_Request** REQ;
	int MaxReq;
	int CurREQ;
	DS_Requests();
	~DS_Requests();
	DWORD DS_SendRequest(char* request,int size);
	void DS_Process();
	DWORD DS_GetRequestResult(DWORD Handle,char** Result,int* Size);
	void DS_CloseRequest(DWORD Handle);
	void DS_CloseAll();
};
DS_Requests::DS_Requests(){
	memset(this,0,sizeof *this);
	CurREQ=(GetTickCount()&32767)+1;
};
DS_Requests::~DS_Requests(){
	for(int i=0;i<MaxReq;i++)if(REQ[i])delete(REQ[i]);
	memset(this,0,sizeof *this);
};
OneDS_Request::OneDS_Request(){
	memset(this,0,sizeof *this);
};
OneDS_Request::~OneDS_Request(){
	if(RESULT)free(RESULT);
    RESULT=NULL;
	memset(this,0,sizeof *this);
};
void HandleDipRequest(char* com,ParsedRQ* Request,ParsedRQ* Result);
void Handle_dipcomm(char* com,char** params,int npr,ParsedRQ* Result);
void DS_Requests::DS_Process(){
	for(int i=0;i<MaxReq;i++)if(REQ[i]){
		ParsedRQ* PR=&REQ[i]->REQ;
		int p=REQ[i]->curstage;
		int s=REQ[i]->progress;
		if(p>=PR->NComm)REQ[i]->Ready=1;
		else{
			char* com=REQ[i]->REQ.Comm[p].ComID;
			char** params=REQ[i]->REQ.Comm[p].Params;
			int npr=REQ[i]->REQ.Comm[p].NParams;
			if(!strcmp(com,"open")){
				if(npr>1){
					ResFile F=RReset(params[0]);
					if(F!=INVALID_HANDLE_VALUE){
						int size=RFileSize(F);
						char* data=(char*)malloc(size+1);
						RBlockRead(F,data,size);
						RClose(F);
						REQ[i]->Result.AddComm("LW_show");
						REQ[i]->Result.AddParam(data,size);
						REQ[i]->Result.AddParam(params[1],strlen(params[1]));
						free(data);
					};
				};
				REQ[i]->curstage++;
				REQ[i]->progress=0;
			}else
			if(!strcmp(com,"get")){
				if(npr>1){
					REQ[i]->Result.AddComm("LW_cfile");
					char src[256];
					char dst[256];
					int nn=(npr-2)>>1;
					for(int j=0;j<nn;j++){
						sprintf(src,"Missions\\Res\\%s",params[j+j]);
						sprintf(dst,"Missions\\Cash\\%s",params[j+j+1]);
						ResFile F=RReset(src);
						if(F!=INVALID_HANDLE_VALUE){
							int sz=RFileSize(F);
							char* data=(char*)malloc(sz+1);
							RBlockRead(F,data,sz);
							RClose(F);
							REQ[i]->Result.AddParam(data,sz);
							REQ[i]->Result.AddParam(params[j+j+1],strlen(params[j+j+1]));
							free(data);
						};
					};
					REQ[i]->Result.AddParam(params[npr-2],strlen(params[npr-2]));
				};
				REQ[i]->curstage++;
				REQ[i]->progress=0;
			}else{
				Handle_dipcomm(com,params,npr,&REQ[i]->Result);
				REQ[i]->Result.AddParam(params[npr-2],strlen(params[npr-2]));
				REQ[i]->curstage++;
			};
			if(REQ[i]->curstage>=PR->NComm)REQ[i]->Ready=1;
		};
	};
};
DWORD DS_Requests::DS_SendRequest(char* request,int size){
	OneDS_Request* LF=NULL;
	for(int i=0;i<MaxReq&&!LF;i++){
		if(!REQ[i]){
			LF=new OneDS_Request;
			REQ[i]=LF;
		};
	};
	if(!LF){
		MaxReq++;
		REQ=(OneDS_Request**)realloc(REQ,MaxReq*4);
		LF=new OneDS_Request;
		REQ[MaxReq-1]=LF;
	};
	LF->Handle=CurREQ;
	CurREQ++;
	LF->REQ.Extract(request,size);
	LF->progress=0;
	LF->curstage=0;
	DS_Process();
	return LF->Handle;
};
DWORD DS_Requests::DS_GetRequestResult(DWORD Handle,char** Result,int* Size){
	for(int i=0;i<MaxReq;i++)if(REQ[i]&&REQ[i]->Ready&&REQ[i]->Handle==Handle){
		if(!REQ[i]->RESULT){
			int sz=REQ[i]->Result.Compact(NULL,0);
			REQ[i]->RESULT=(char*)malloc(sz);
			REQ[i]->RSIZE=sz;
			REQ[i]->Result.Compact(REQ[i]->RESULT,sz);
		};
		*Result=REQ[i]->RESULT;
		*Size=REQ[i]->RSIZE;
		return 128;
	};
	return 0;
};
void DS_Requests::DS_CloseRequest(DWORD Handle){
	for(int i=0;i<MaxReq;i++)if(REQ[i]&&REQ[i]->Handle==Handle){
		delete(REQ[i]);
		REQ[i]=NULL;
	};
};
void DS_Requests::DS_CloseAll(){
	for(int i=0;i<MaxReq;i++)if(REQ[i])DS_CloseRequest(REQ[i]->Handle);
};
//--------exports--------
DS_Requests LFREQ;
CEXPORT
DWORD DS_SendRequest(char* request,int size){
	return LFREQ.DS_SendRequest(request,size);
};
CEXPORT
void DS_Process(){
	LFREQ.DS_Process();
};
CEXPORT
DWORD DS_GetRequestResult(DWORD Handle,char** Result,int* Size){
	return LFREQ.DS_GetRequestResult(Handle,Result,Size);
};
CEXPORT
void DS_CloseRequest(DWORD Handle){
	LFREQ.DS_CloseRequest(Handle);
};
CEXPORT
void DS_CloseAll(){
	LFREQ.DS_CloseAll();
};