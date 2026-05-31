// Client/Server initialization & finalization
//
#include "CommCore.h"

// ---------------------------------------------------------------------------------------------
#ifdef CC_DEBUG
VOID CCommCore::DebugMessage(LPCSTR lpcszMessage)
{
	SYSTEMTIME		SysTime;
	GetLocalTime(&SysTime);

	m_DebugStream=fopen("C:\\CommCore.log","a+");
	
	fprintf(m_DebugStream,"%2.2u:%2.2u:%2.2u.%3.3u		%s\n",
		SysTime.wHour,
		SysTime.wMinute,
		SysTime.wSecond,
		SysTime.wMilliseconds,
		lpcszMessage);

	fclose(m_DebugStream);
}
#endif //CC_DEBUG


CCommCore::CCommCore()
{
	m_uFrameCount=0;
	m_lStamp=0;
	m_uPeerCount=0;
	m_bServer=FALSE;
	m_piNumber=0;
	m_ssState=ssNone;
	m_csState=csNone;
	m_bBlockingCall=FALSE;
	m_piAutoInc=0;
	m_lpbRecvBuffer=(LPBYTE)malloc(RECV_BUFFER_LENGTH);
	assert(m_lpbRecvBuffer);
	m_uMaxMsgSize=0;
	lpIdleProc=NULL;
	lpEnumProc=NULL;
	m_szUserName[0]='\0';
	m_szSessionName[0]='\0';
	m_paServAddr.s_addr=inet_addr("0.0.0.0");	
	m_lpbUserData=NULL;
	m_uUserDataSize=0;
	m_bBlockingCall=FALSE;
	m_uMaxPeers=MAX_PEERS;
	GetCommCoreUID(m_szCCUID);
	m_uAddrCount=0;
	m_dwOptions=0;
	m_dwRxBytes=0;
	m_dwTxBytes=0;
	m_dwNxBytes=0;

	m_bServerOpen=FALSE;

	m_pServersList=NULL;
	m_uServersCount=0;
	m_dwPollStart=0;

	m_dwGwLastTick=0;

	m_bOverInternet=FALSE;

#ifdef CC_DEBUG
	DebugMessage("-- Log Started ---------------------------------");
#endif //CC_DEBUG
}

// ---------------------------------------------------------------------------------------------

CCommCore::~CCommCore()
{
#ifdef CC_DEBUG
	DebugMessage("-- Log Stopped ---------------------------------");
#endif //CC_DEBUG
	SAFE_FREE(m_lpbRecvBuffer);
}

// ---------------------------------------------------------------------------------------------

BOOL CCommCore::InitServer(LPCSTR lpcszSessionName, LPCSTR lpcszUserName)
{
	_log_message("InitServer()");

	m_dwRxBytes=0;
	m_dwTxBytes=0;
	m_dwNxBytes=0;

	strcpy(m_szUserName,lpcszUserName);

	m_bServer=TRUE;
	
	m_piNumber=1;
	m_piAutoInc=1;
	m_uPeerCount=1;

	m_PeerList[0].m_bAlive=TRUE;
	m_PeerList[0].m_Id=1;
	m_PeerList[0].m_uLatency=0;
	m_PeerList[0].m_bOverNAT=FALSE;

	if(m_bOverInternet){
		// Who Am I
		CC_PK_GB_PING_SMALL	AskPack;
		CC_PK_GB_PING_LARGE	RepPack;

		AskPack.m_lProto=PROTO_ID;
		AskPack.m_uType=CC_PT_PING_REQUEST;

		sockaddr_in			sinPeer;
		memset(&sinPeer,0x00,sizeof(sockaddr_in));

		char szDatFile[MAX_PATH];
		GetModuleFileName(NULL,szDatFile,MAX_PATH);
		*(strrchr(szDatFile,'\\')+1)='\0';
		strcat(szDatFile,"Internet\\ggwdc.dat");

		FILE *dat_file=fopen(szDatFile,"r");

		if(!dat_file)
			return FALSE;

		fread(&m_gwSrvAddr,sizeof(in_addr),1,dat_file);
		fread(&m_gwSrvPort,sizeof(u_short),1,dat_file);
		fclose(dat_file);

		sinPeer.sin_family=AF_INET;
		sinPeer.sin_addr=m_gwSrvAddr;
		sinPeer.sin_port=htons(DATA_PORT);        

		if(sendto(m_DataSocket,
				(char *)&AskPack,
				sizeof(CC_PK_GB_PING_SMALL),
				0x00,
				(sockaddr *)&sinPeer,
				sizeof(sockaddr_in) )==SOCKET_ERROR)
		return FALSE;

		m_dwGwLastTick=GetTickCount();

		u_long		lDataSize;

		DWORD		dwTimeOut=GetTickCount();

		for(;;){
			ioctlsocket(m_DataSocket,FIONREAD,&lDataSize);
			if(lDataSize)
				break;
			if((GetTickCount()-dwTimeOut)>8000)
				return FALSE;
			Sleep(100);
		};

		sockaddr_in					SenderAddr;
		int							SenderLength=sizeof(sockaddr_in);
			
		if(recvfrom(m_DataSocket,
					(char *)&RepPack,
					sizeof(CC_PK_GB_PING_LARGE),
					0x00,
					(sockaddr *)&SenderAddr,
					&SenderLength)==SOCKET_ERROR)
		return FALSE;
		
		m_PeerList[0].m_ex_Addr=RepPack.m_iaExAddr;
		m_PeerList[0].m_ex_Port=RepPack.m_usExPort;

	}else{
		m_PeerList[0].m_ex_Addr.s_addr=m_dwAddrList[0];
		m_PeerList[0].m_ex_Port=htons(DATA_PORT);
	};

	m_PeerList[0].m_lpbUserData=m_lpbUserData;
	m_PeerList[0].m_uUserDataSize=m_uUserDataSize;
	strcpy(m_PeerList[0].m_szUserName,m_szUserName);
	strcpy(m_szSessionName,lpcszSessionName);
	memcpy(m_PeerList[0].m_szCCUID,m_szCCUID,22);

	m_ssState=ssOpen;

	m_uMaxPeers=MAX_PEERS;

	m_bServerOpen=TRUE;

	return TRUE;		
}

// ---------------------------------------------------------------------------------------------

BOOL CCommCore::InitClient(LPCSTR lpcszServerIP, LPCSTR lpcszUserName)
{
	_log_message("InitClient()");

	char szServerIP[512];
	
	m_usServPort=htons(DATA_PORT);

	strcpy(szServerIP,lpcszServerIP);

	char* p=strrchr(szServerIP,':');

	if(p){
		_log_message("Using internet version");

		m_usServPort=htons(atoi(p+1));
		*p='\0';

		// ask server via GW to send us pings
		CC_PK_GB_PING_XXL	AskPack;
		CC_PK_GB_PING_SMALL	RepPack;

		ZeroMemory(&AskPack,sizeof(CC_PK_GB_PING_XXL));

		AskPack.m_lProto=PROTO_ID;
		AskPack.m_uType=CC_PT_PING_CLIENT;
		AskPack.m_iaExAddr.s_addr=inet_addr(szServerIP);
		AskPack.m_usExPort=m_usServPort;

		// deterining our interfaces and addresses
		hostent	*pHostName=gethostbyname(NULL);

		u_short usAddrCount=0;

		if(pHostName){
			while(pHostName->h_addr_list[usAddrCount]){
				memcpy(&AskPack.m_iaMyAddr[usAddrCount],pHostName->h_addr_list[usAddrCount],pHostName->h_length);
#ifdef CC_DEBUG
				CHAR		szMsg1[255];
				sprintf(szMsg1,"Address #%u :	[%s]",usAddrCount+1,inet_ntoa(AskPack.m_iaMyAddr[usAddrCount]));	
				_log_message(szMsg1);
#endif //CC_DEBUG
				usAddrCount++;
			};
		}else{
			_log_message("Call to gethostbyname(NULL) returned NULL");
		};

		AskPack.m_ucAddrNum=usAddrCount;

		sockaddr_in			sinPeer;
		memset(&sinPeer,0x00,sizeof(sockaddr_in));

		char szDatFile[MAX_PATH];
		GetModuleFileName(NULL,szDatFile,MAX_PATH);
		*(strrchr(szDatFile,'\\')+1)='\0';
		strcat(szDatFile,"Internet\\ggwdc.dat");

		_log_message("Reading config file");
		FILE *dat_file=fopen(szDatFile,"r");

		if(!dat_file)
			return FALSE;

		in_addr SrvAddr;
		u_short usServPort;

		fread(&SrvAddr,sizeof(in_addr),1,dat_file);
		fread(&usServPort,sizeof(u_short),1,dat_file);
		fclose(dat_file);

		sinPeer.sin_family=AF_INET;
		sinPeer.sin_addr=SrvAddr;
		sinPeer.sin_port=htons(DATA_PORT);        

		_log_message("Sending request to server via GW to ping us");
		// sending request to server via GW server to ping us using supplied addresses
		if(sendto(m_DataSocket,
				(char *)&AskPack,
				sizeof(CC_PK_GB_PING_XXL),
				0x00,
				(sockaddr *)&sinPeer,
				sizeof(sockaddr_in) )==SOCKET_ERROR)
		return FALSE;

		_log_message("Sending direct pings to server");
		// sending direct pings to server
		CC_PK_GB_PING_SMALL		RepPkt;
		RepPkt.m_lProto=PROTO_ID;
		RepPkt.m_uType=CC_PT_PING_EMPTY;

		// to server, that I attempt to join

		memset(&sinPeer,0x00,sizeof(sockaddr_in));

		sinPeer.sin_family=AF_INET;
		sinPeer.sin_addr.s_addr=inet_addr(szServerIP);
		sinPeer.sin_port=m_usServPort;

#ifdef CC_DEBUG
		CHAR		szMsg[255];

		sprintf(szMsg,"     Pinging : %s:%u",
			inet_ntoa(sinPeer.sin_addr),
			ntohs(sinPeer.sin_port));
		DebugMessage(szMsg);
#endif //CC_DEBUG
		// sending UDP_HOLE_PINGS empty pings to client
		for(int k=0;k<UDP_HOLE_PINGS;k++)
		if(sendto(m_DataSocket,
				(char*)&RepPkt,
				sizeof(CC_PK_GB_PING_SMALL),
				0x00,
				(sockaddr*)&sinPeer,
				sizeof(sockaddr_in))==SOCKET_ERROR){
					_log_message("Errors while sending data");
					return 0x00;
				};

		// reply

		u_long		lDataSize;
		DWORD		dwTimeOut=GetTickCount();
		
read_again:

		_log_message("Waiting for reply from server");

		bool bSomeRecv=false;

		// waiting for reply from server
		for(;;){
			ioctlsocket(m_DataSocket,FIONREAD,&lDataSize);
			if(lDataSize){
				_log_message("Something received");
				bSomeRecv=true;
				break;
			};
			if((GetTickCount()-dwTimeOut)>5000){
				_log_message("Nothing received in 5 seconds");
				bSomeRecv=false;
				break;
			};
			Sleep(100);
		};

		if(!bSomeRecv)
			goto ping_back;

		sockaddr_in					SenderAddr;
		int							SenderLength=sizeof(sockaddr_in);
			
		_log_message("Receiving data");
		if(recvfrom(m_DataSocket,
					(char *)&RepPack,
					sizeof(CC_PK_GB_PING_SMALL),
					0x00,
					(sockaddr *)&SenderAddr,
					&SenderLength)==SOCKET_ERROR){
						int wse=WSAGetLastError();
#ifdef CC_DEBUG
						sprintf(szMsg,"recv error = %i",wse);
						_log_message(szMsg);
#endif //CC_DEBUG
						if(wse==WSAECONNRESET){
							_log_message("Remote router reports host unreachable");
							goto read_again;
						}else{
							return FALSE;
						};
					};

#ifdef CC_DEBUG
//		CHAR		szMsg[255];

		sprintf(szMsg,"     Type: %u (from: %s:%u)",
			RepPack.m_uType,
			inet_ntoa(SenderAddr.sin_addr),
			ntohs(SenderAddr.sin_port));
		DebugMessage(szMsg);
#endif //CC_DEBUG

		if(RepPack.m_uType==CC_PT_PING_EMPTY){ //very good, direct or semidirect connection
			_log_message("We get a (semi) direct reply");
			// correcting server ea:ep pair ???
#ifdef CC_DEBUG
			if(m_usServPort!=SenderAddr.sin_port)
				_log_message("correcting server ea:ep pair");
#endif //CC_DEBUG
			m_usServPort=SenderAddr.sin_port;
			strcpy(szServerIP,inet_ntoa(SenderAddr.sin_addr));
		};

ping_back:

//		CC_PK_GB_PING_SMALL		RepPkt;
		RepPkt.m_lProto=PROTO_ID;
		RepPkt.m_uType=CC_PT_PING_EMPTY;

		// to server, that I attempt to join

		memset(&sinPeer,0x00,sizeof(sockaddr_in));

		sinPeer.sin_family=AF_INET;
		sinPeer.sin_addr.s_addr=inet_addr(szServerIP);
		sinPeer.sin_port=m_usServPort;

#ifdef CC_DEBUG
		//CHAR		szMsg[255];

		sprintf(szMsg,"     Pinging : %s:%u",
			inet_ntoa(sinPeer.sin_addr),
			ntohs(sinPeer.sin_port));
		DebugMessage(szMsg);
#endif //CC_DEBUG
		// sending UDP_HOLE_PINGS empty pings to client
		for(int k=0;k<UDP_HOLE_PINGS;k++)
		if(sendto(m_DataSocket,
				(char*)&RepPkt,
				sizeof(CC_PK_GB_PING_SMALL),
				0x00,
				(sockaddr*)&sinPeer,
				sizeof(sockaddr_in))==SOCKET_ERROR){
					_log_message("Errors while sending data");
					return 0x00;
				};
		
		// we can try to connect now
	};

	_log_message("Starting connect transaction");

	m_dwRxBytes=0;
	m_dwTxBytes=0;
	m_dwNxBytes=0;

	m_piAutoInc=1;
	m_uPeerCount=0;

	strcpy(m_szUserName,lpcszUserName);

	LPCC_PK_TRY_CONNECT	lpConnectPacket;

	lpConnectPacket=(LPCC_PK_TRY_CONNECT)malloc(sizeof(CC_PK_TRY_CONNECT)+(m_uAddrCount*sizeof(DWORD)));

	m_bServer=FALSE;

	m_piNumber=BAD_PEER_ID;

	lpConnectPacket->m_cProtoVersion=CC_PROTO_VERSION;
	lpConnectPacket->m_uAddrCount=m_uAddrCount;
	strcpy(lpConnectPacket->m_szUserName,m_szUserName);
	memcpy(lpConnectPacket->m_szCCUID,m_szCCUID,22);
	memcpy(lpConnectPacket->m_dwAddrList,m_dwAddrList,(m_uAddrCount*sizeof(DWORD)));
	m_paServAddr.s_addr=inet_addr(szServerIP);	

	m_csState=csWait;

	if(!SendRawPacket(	m_paServAddr, 
						m_usServPort,
						CC_PT_TRY_CONNECT,
						(LPBYTE)lpConnectPacket,
						sizeof(CC_PK_TRY_CONNECT)+(m_uAddrCount*sizeof(DWORD)),
						TRUE,
						FALSE)){
			free(lpConnectPacket);
			return FALSE;
	};

	free(lpConnectPacket);

	DWORD dwTime=CURRENT_TIME;

	while( ((CURRENT_TIME-dwTime)<(RETRY_TIME*(RETRY_COUNT+3))) && m_csState==csWait){

		ReceiveData(NULL);
		QueueProcess();
		if(lpIdleProc)
			lpIdleProc();
	};

	if(m_csState!=csConnected)
		return FALSE;

	m_ssState=ssOpen;

	m_bServerOpen=FALSE;

	return TRUE;
}

// ---------------------------------------------------------------------------------------------

BOOL CCommCore::DoneClient()
{
	_log_message("DoneClient()");

	CC_PK_HOST_EXIT		HostExitPacket;

	HostExitPacket.m_dwReserved=0xFF;
	if(m_ssState!=ssClosed){
		SendRawPacket(	m_paServAddr,
						m_usServPort,
						CC_PT_HOST_EXIT,
						(LPBYTE)&HostExitPacket,
						sizeof(CC_PK_HOST_EXIT),
						TRUE,
						FALSE);
	}

	QueueClearAll();

	Cleanup();

	return TRUE;
}

// ---------------------------------------------------------------------------------------------

BOOL CCommCore::DoneServer()
{
	_log_message("DoneServer()");

	int i;
	if(m_ssState!=ssClosed){
		for(i=1;i<m_uPeerCount;i++)
			SendDropClient(m_PeerList[i].m_Id);
	}
/*
	while(m_uFrameCount>0){
		ReceiveData(NULL);
		QueueProcess();
		if(lpIdleProc)
			lpIdleProc();
	};
*/

	QueueClearAll();

	Cleanup();

	return TRUE;
}

// ---------------------------------------------------------------------------------------------

VOID CCommCore::Cleanup()
{
	_log_message("Cleanup()");

	for(int i=0;i<m_uPeerCount;i++){
		SAFE_FREE(m_PeerList[i].m_lpbUserData);
		m_PeerList[i].m_uUserDataSize=0;
	}

	m_uPeerCount=0;

	SAFE_FREE(m_lpbUserData);

	SAFE_FREE(m_pServersList);

	m_lpbUserData=NULL;
	m_uUserDataSize=0;

	m_dwTxBytes=0;
	m_dwNxBytes=0;
	m_dwRxBytes=0;

	m_csState=csNone;
	m_ssState=ssNone;

	m_bServer=FALSE;
	m_bServerOpen=FALSE;
};
