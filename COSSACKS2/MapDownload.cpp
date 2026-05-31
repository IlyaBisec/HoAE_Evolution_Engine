#include "stdheader.h"
#include "IResourceManager.h"

DWORD GetMapHash(char* Name){
	static DString lastName;
	static DWORD lastMapHash=0;
	static int LastCheckT=GetTickCount()-10000;
	if(lastName!=Name||lastMapHash==0||GetTickCount()-LastCheckT>4000){
		LastCheckT=GetTickCount();
		lastName=Name;
		lastMapHash=0;
		ResFile F=RReset(Name);
		if(F!=INVALID_HANDLE_VALUE){
			int sz=RFileSize(F);
			int sz1=sz/4+1;
			DWORD * buf=new DWORD[sz1];
			memset(buf,0,sz1*4);
			RBlockRead(F,buf,sz);
			RClose(F);
			for(int i=0;i<sz1;i++)lastMapHash+=buf[i];
			delete[](buf);
			return lastMapHash;
		}
	}
	return lastMapHash;
}
class MapPartRequest:public BaseClass{
public:
	int CurrentPool;
	DWORD MapHash;
	_str MapName;
	int StartOffset;
	int Length;
	int ReqTime;
	DWORD ReqPID;
};
class MapPartResult:public MapPartRequest{
public:
	int TotalSize;
	DynArray<char> ResultData;
	bool FinalBlock;
};
class MapInfo:public BaseClass{
public:
	DWORD Hash;
	int Pool;
	int TotalSize;
};
class MapReqList:public BaseClass{
public:
	ClassArray<MapPartRequest> ReqList;
    ClassArray<MapPartResult> ResList;
	ClassArray<MapInfo> FInfo;
	void SetPool(DWORD Hash,int Pool){
		for(int i=0;i<FInfo.GetAmount();i++)if(FInfo[i]->Hash==Hash){
			FInfo[i]->Pool=Pool;
			return;
		}
		MapInfo* MI=new MapInfo;
		MI->Pool=Pool;
		MI->Hash=Hash;
		MI->TotalSize=10000000;
		FInfo.Add(MI);
	}
	int  GetPool(DWORD Hash){
		for(int i=0;i<FInfo.GetAmount();i++)if(FInfo[i]->Hash==Hash){
			return FInfo[i]->Pool;
		}
		return 2;
	}
	int GetNRequests(DWORD Hash){
		int N=0;
		for(int i=0;i<ReqList.GetAmount();i++)if(ReqList[i]->MapHash==Hash){
			N++;
		}
		return N;        
	}
};
MapReqList MDOWNLOAD;
void SendRequest(char* Name,DWORD Hash,int StartOffset,int Length){
	PlayerInfo* HPI=GSets.CGame.GetHostPlayerInfo();
	if(HPI){
		DWORD buf[512];
		buf[0]='GFIL';
		buf[1]=Hash;
		buf[2]=StartOffset;
		buf[3]=Length;
		strcpy((char*)(buf+4),Name);
		int L=16+strlen(Name)+1;
		bool SendToPlayer(DWORD Size,LPVOID lpData,DWORD DPID);
		if(SendToPlayer(L,buf,HPI->PlayerID)){
			MapPartRequest* MR=new MapPartRequest;
			MR->MapHash=Hash;
			MR->MapName=Name;
			MR->StartOffset=StartOffset;
			MR->Length=Length;
			MR->ReqTime=GetTickCount();
			MDOWNLOAD.ReqList.Add(MR);
		}
    }
}
#define BlockSize 512
void SendReply(char* Name,DWORD Hash,int StartOffset,int Length,DWORD PlayerID){
    ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		int sz=RFileSize(F);
		RSeek(F,StartOffset);
		int L=sz-StartOffset;
		if(L>BlockSize)L=BlockSize;
		if(L<0)L=0;
		bool Final=StartOffset+L>=sz;
		char buf[BlockSize+512];
		DWORD* dbuf=(DWORD*)buf;
		dbuf[0]='RFIL';
		dbuf[1]=Hash;
		dbuf[2]=StartOffset;
		dbuf[3]=L;
		dbuf[4]=sz;
		if(Final)dbuf[3]|=65536;
		RBlockRead(F,dbuf+5,L);
		RClose(F);
		strcpy(buf+20+L,Name);
		int psz=L+20+1;
		bool SendToPlayer(DWORD Size,LPVOID lpData,DWORD DPID);
		SendToPlayer(psz,buf,PlayerID);
	}
}

void ReportOnGetBlock(DWORD Hash,char* MapName,int WholeLength,int StartOffset,int Len,char* Data,bool Final){
	int pl=MDOWNLOAD.GetPool(Hash);
	for(int i=MDOWNLOAD.ReqList.GetAmount()-1;i>=0;i--){
		if(MDOWNLOAD.ReqList[i]->MapHash==Hash&&MDOWNLOAD.ReqList[i]->StartOffset==StartOffset){
            MDOWNLOAD.ReqList.DelElement(i);
		}
	}
	MapPartResult* MR=new MapPartResult;
	MR->MapHash=Hash;
	MR->MapName=MapName;
	MR->StartOffset=StartOffset;
	MR->Length=Len;
	//MR->ResultData.Check(Len);
	MR->ResultData.Add(0,Len);
	MR->FinalBlock=Final;
	MR->TotalSize=WholeLength;
	memcpy(MR->ResultData.GetValues(),Data,Len);
	MDOWNLOAD.ResList.Add(MR);
	int rp=32768*5/(5+pl);
	if(rand()<rp)pl++;
	MDOWNLOAD.SetPool(Hash,pl);	
};
void AppendToFile(char* FileName,int L,void* Data){
	FILE* F=fopen(FileName,"ab");
	if(F){
		fwrite(Data,1,L,F);
		fclose(F);
	}
}
int GetFileProgress(char* FileName,DWORD HashValue){
	if(!strncmp(FileName,"RN0 ",4))return 100;
	DWORD H1=GetMapHash(FileName);
	if(H1==HashValue)return 100;  
	bool CheckNoCopy();
	if(CheckNoCopy())return 0;
	char hname[64];
	sprintf(hname,"Temp\\%X.",HashValue);
	ResFile F=RReset(hname);
	int sz=0;
	if(F!=INVALID_HANDLE_VALUE){
		sz=RFileSize(F);
		RClose(F);
	}
	int nappend=0;
	bool finished=0;
	//search for appending blocks
	int N0=MDOWNLOAD.GetPool(HashValue);
	int N1=MDOWNLOAD.GetNRequests(HashValue);
	bool smt=false;
	int maxsz=1000000;
	do{
		smt=false;		
		for(int i=0;i<MDOWNLOAD.ResList.GetAmount();i++){
			if(MDOWNLOAD.ResList[i]->MapHash==HashValue){
				maxsz=MDOWNLOAD.ResList[i]->TotalSize;
				if(MDOWNLOAD.ResList[i]->StartOffset==sz){
					//appending
					AppendToFile(hname,MDOWNLOAD.ResList[i]->ResultData.GetAmount(),MDOWNLOAD.ResList[i]->ResultData.GetValues());
					smt=true;
					int prog=0;
					if(MDOWNLOAD.ResList[i]->TotalSize){
						prog=MDOWNLOAD.ResList[i]->StartOffset*100/MDOWNLOAD.ResList[i]->TotalSize;					
					}				
					char cc1[32];
					sprintf(cc1,"Temp\\%X.progress",HashValue);
					FILE* F=fopen(cc1,"w");
					if(F){
						fprintf(F,"%d",prog);
						fclose(F);
					}
					sz+=MDOWNLOAD.ResList[i]->ResultData.GetAmount();
					if(MDOWNLOAD.ResList[i]->FinalBlock||sz>=maxsz){
						
						IRM->CheckFileAccess(FileName);
						CopyFile(hname,FileName,false);
						
						finished=1;
					}
					MDOWNLOAD.ResList.DelElement(i);
					i--;
					nappend++;
					break;
				}else
				if(MDOWNLOAD.ResList[i]->StartOffset<sz){
					MDOWNLOAD.ResList.DelElement(i);
					i--;
					nappend++;
				}
			}
		}
	}while(smt);
	if(!finished){
		//checking if there exists at least one request		
		nappend=N0-N1;		
		//sending requests again
#define maxM 16384 
		byte Massiv[maxM];
		memset(Massiv,0,sizeof Massiv);
		for(int j=0;j<MDOWNLOAD.ResList.GetAmount();j++){
			if(MDOWNLOAD.ResList[j]->MapHash==HashValue){
				Massiv[MDOWNLOAD.ResList[j]->StartOffset/BlockSize]=1;
			}
		}
		for(int j=0;j<MDOWNLOAD.ReqList.GetAmount();j++){
			if(MDOWNLOAD.ReqList[j]->MapHash==HashValue){
				Massiv[MDOWNLOAD.ReqList[j]->StartOffset/BlockSize]=2;				
			}			
		}
		maxsz=(maxsz/BlockSize)+1;
		for(int i=0;i<nappend&&i<100;i++){
			for(int j=sz/BlockSize;j<maxM&&j<maxsz;j++)if(!Massiv[j]){
				SendRequest(FileName,HashValue,j*BlockSize,BlockSize);
				ProcessMessages();
				N0=MDOWNLOAD.GetPool(HashValue);
				Massiv[j]=2;
				break;
			}			
		}
		//MDOWNLOAD.SetPool(HashValue,N0);
		//resending lost packets
		int ct=GetTickCount();
		for(int j=0;j<MDOWNLOAD.ReqList.GetAmount();j++){
			if(MDOWNLOAD.ReqList[j]->MapHash==HashValue){
				if(ct-MDOWNLOAD.ReqList[j]->ReqTime>1500){
					bool resend=MDOWNLOAD.ReqList[j]->StartOffset==sz;					
                    int p=MDOWNLOAD.GetPool(HashValue);
					p=p/3;
					if(p<2)p=2;
					MDOWNLOAD.SetPool(HashValue,p);
					MDOWNLOAD.ReqList.DelElement(j);
					if(resend){
						SendRequest(FileName,HashValue,sz,BlockSize);
					}
					break;
				}
			}
		}
		PlayerInfo* PI=GSets.CGame.GetHostPlayerInfo();
		int GetLastAnswerT(DWORD ID);
		if(PI&&GetTickCount()-GetLastAnswerT(PI->PlayerID)>4000){
			MDOWNLOAD.SetPool(HashValue,2);
		}
        int pp=MDOWNLOAD.GetPool(HashValue);
		/*char cc[32];
		IRS->SetViewPort(0,0,1024,768);
		GPS.SetScreenSpaceTM(NULL);
		GPS.SetScale(1.0f);
		GPS.DrawFillRect(0,0,1024,140,0xA0000000);
		GPS.FlushBatches();
		sprintf(cc,"pool=%d",pp);
		ShowString(0,0,cc,&SmallWhiteFont);
		for(int i=0;i<maxM;i++){
			int xp=(i*4)%1024;
			int yp=20+((i*4)/1024)*4;
			if(Massiv[i]==1)  GPS.DrawFillRect(xp,yp,3,3,0xFFFFFF00);
			if(Massiv[i]==2)  GPS.DrawFillRect(xp,yp,3,3,0xFFFF0000);
			if(i*BlockSize<sz)GPS.DrawFillRect(xp,yp,3,3,0xFF00FF00);
		}
		GPS.FlushBatches();*/		
	}
	int prog=0;
	char cc1[32];
	sprintf(cc1,"Temp\\%X.progress",HashValue);
	FILE* FL=fopen(cc1,"r");
	if(FL){
		fscanf(FL,"%d",&prog);
		fclose(FL);
	}
	return prog;
}