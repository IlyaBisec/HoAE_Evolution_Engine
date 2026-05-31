// LF_Server.cpp : Defines the entry point for the DLL application.
//

#include "..\COSSACKS2\common.h"
#include "..\IntExplorer\ParseRQ.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
};
class OneLF_Request{
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
	OneLF_Request();
	~OneLF_Request();
};
class LF_Requests{
public:
	OneLF_Request** REQ;
	int MaxReq;
	int CurREQ;
	LF_Requests();
	~LF_Requests();
	DWORD LF_SendRequest(char* request,int size);
	void LF_Process();
	DWORD LF_GetRequestResult(DWORD Handle,char** Result,int* Size);
	void LF_CloseRequest(DWORD Handle);
	void LF_CloseAll();
};
LF_Requests::LF_Requests(){
	memset(this,0,sizeof *this);
	CurREQ=(GetTickCount()&32767)+1;
};
LF_Requests::~LF_Requests(){
	for(int i=0;i<MaxReq;i++)if(REQ[i])delete(REQ[i]);
	memset(this,0,sizeof *this);
};
OneLF_Request::OneLF_Request(){
	memset(this,0,sizeof *this);
};
OneLF_Request::~OneLF_Request(){
	if(RESULT)free(RESULT);
	memset(this,0,sizeof *this);
};
void LF_Requests::LF_Process(){
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
						sprintf(src,"Internet\\Res\\%s",params[j+j]);
						sprintf(dst,"Internet\\Cash\\%s",params[j+j+1]);
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
			}else
			if(!strcmp(com,"dnfile")){
				if(npr>1){




					REQ[i]->Result.AddComm("LW_cfile");
					char src[256];
					char dst[256];
					int nn=(npr-2)>>1;
					for(int j=0;j<nn;j++){
						sprintf(src,"Internet\\Res\\%s",params[j+j]);
						sprintf(dst,"Internet\\Cash\\%s",params[j+j+1]);
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
				REQ[i]->curstage++;
				REQ[i]->progress=0;
			};
			if(REQ[i]->curstage>=PR->NComm)REQ[i]->Ready=1;
		};
	};
};
DWORD LF_Requests::LF_SendRequest(char* request,int size){
	OneLF_Request* LF=NULL;
	for(int i=0;i<MaxReq&&!LF;i++){
		if(!REQ[i]){
			LF=new OneLF_Request;
			REQ[i]=LF;
		};
	};
	if(!LF){
		MaxReq++;
		REQ=(OneLF_Request**)realloc(REQ,MaxReq*4);
		LF=new OneLF_Request;
		REQ[MaxReq-1]=LF;
	};
	LF->Handle=CurREQ;
	CurREQ++;
	LF->REQ.Extract(request,size);
	LF->progress=0;
	LF->curstage=0;
	LF_Process();
	return LF->Handle;
};
DWORD LF_Requests::LF_GetRequestResult(DWORD Handle,char** Result,int* Size){
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
void LF_Requests::LF_CloseRequest(DWORD Handle){
	for(int i=0;i<MaxReq;i++)if(REQ[i]&&REQ[i]->Handle==Handle){
		delete(REQ[i]);
		REQ[i]=NULL;
	};
};
void LF_Requests::LF_CloseAll(){
	for(int i=0;i<MaxReq;i++)if(REQ[i])LF_CloseRequest(REQ[i]->Handle);
};
//--------exports--------
LF_Requests LFREQ;
CEXPORT
DWORD LF_SendRequest(char* request,int size){
	return LFREQ.LF_SendRequest(request,size);
};
CEXPORT
void LF_Process(){
	LFREQ.LF_Process();
};
CEXPORT
DWORD LF_GetRequestResult(DWORD Handle,char** Result,int* Size){
	return LFREQ.LF_GetRequestResult(Handle,Result,Size);
};
CEXPORT
void LF_CloseRequest(DWORD Handle){
	LFREQ.LF_CloseRequest(Handle);
};
CEXPORT
void LF_CloseAll(){
	LFREQ.LF_CloseAll();
};