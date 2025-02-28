// Packet receiveing & processing routines
//
#include "CommCore.h"

// ---------------------------------------------------------------------------------------------

u_short CCommCore::ReceiveData(LPBYTE lpbBuffer, LPPEER_ID lpPeerId)
{
	sockaddr_in					SenderAddr;
	LPCC_PK_RAW_FRAME			lpFrame;
	u_short						uPeer;
	int							SenderLength=sizeof(sockaddr_in);
	u_short						PeerNum;
	int							i;

	LPCC_PK_SEND_DATA			lpDataPacket;
	LPCC_PK_FRAME_CONFIRM		lpDataConfirmPacket;
	LPCC_PK_CONNECT_REJECT		lpConnectReject;
	LPCC_PK_CONNECT_OK			lpConnectOk;
	LPCC_PK_TRY_CONNECT			lpTryConnect;
	LPCC_PK_SERVER_LIST			lpServerList;
	LPCC_PK_HOST_EXIT			lpHostExit;
	LPCC_PK_SEND_USER_NAME		lpUserName;
	LPCC_PK_SEND_USER_DATA		lpUserData;
	LPCC_PK_SEND_NEW_NAME		lpSendNewName;
	LPCC_PK_SEND_NEW_DATA		lpSendNewData;

	
	// GW Pings

	if((m_bOverInternet)&&(m_bServer)&&(m_bServerOpen)&&((GetTickCount()-m_dwGwLastTick)>20000)){

		CC_PK_GB_PING_SMALL	AskPack;

		AskPack.m_lProto=PROTO_ID;
		AskPack.m_uType=CC_PT_PING_REQUEST;

		sockaddr_in			sinPeer;
		memset(&sinPeer,0x00,sizeof(sockaddr_in));

		sinPeer.sin_family=AF_INET;
		sinPeer.sin_addr=m_gwSrvAddr;
		sinPeer.sin_port=htons(DATA_PORT);        

		sendto(m_DataSocket,
				(char*)&AskPack,
				sizeof(CC_PK_GB_PING_SMALL),
				0x00,
				(sockaddr*)&sinPeer,
				sizeof(sockaddr_in));

		m_dwGwLastTick=GetTickCount();
	};


	ProcessBroadcasts();

	u_long		lDataSize;

	ioctlsocket(m_DataSocket,FIONREAD,&lDataSize);

	if(!lDataSize)
		return 0x00;

	m_dwRxBytes+=lDataSize;

	if(recvfrom(	m_DataSocket,
					(char *)m_lpbRecvBuffer,
					RECV_BUFFER_LENGTH,
					0x00,
					(sockaddr *)&SenderAddr,
					&SenderLength)==SOCKET_ERROR)
		return 0x00;

	m_dwLastPacketTime=CURRENT_TIME;

	lpFrame=(LPCC_PK_RAW_FRAME)m_lpbRecvBuffer;

// ----------------------------------------------------------------------
// December 25 2001
// Adjusting address/port pairs in internal list
// ----------------------------------------------------------------------
/*	
	if(lpFrame->m_PeerId!=BAD_PEER_ID){
		PEER_ID piTestId=GetPeerById(lpFrame->m_PeerId);
		if(piTestId!=BAD_PEER_ID){
			if(m_PeerList[piTestId].m_ex_Addr.s_addr!=SenderAddr.sin_addr.s_addr||
				m_PeerList[piTestId].m_ex_Port!=SenderAddr.sin_port){
				m_PeerList[piTestId].m_ex_Addr.s_addr=SenderAddr.sin_addr.s_addr;
				m_PeerList[piTestId].m_ex_Port=SenderAddr.sin_port;
			};
		};
	};
*/
// ----------------------------------------------------------------------

#ifdef CC_DEBUG
	CHAR		szMsg[255];
	sprintf(szMsg,"after ReceiveData()");
	DebugMessage(szMsg);

	sprintf(szMsg,"     From:	%s:%u",
		inet_ntoa(SenderAddr.sin_addr),
		ntohs(SenderAddr.sin_port));
	DebugMessage(szMsg);

	sprintf(szMsg,"     Type:	%u",lpFrame->m_uType);
	DebugMessage(szMsg);

	sprintf(szMsg,"     PeerId:	%u",lpFrame->m_PeerId);
	DebugMessage(szMsg);

	sprintf(szMsg,"     Size:	%u",lDataSize);
	DebugMessage(szMsg);
#endif //CC_DEBUG

	if(lpFrame->m_lProto!=PROTO_ID)
		return 0x00;

	if(lpFrame->m_uType==CC_PT_PING_CLIENT){
		_log_message("We get a request from GW to ping client");
#ifdef CC_DEBUG
		CHAR		szMsg[255];

		LPCC_PK_GB_PING_XXL lpReqPcktX=(LPCC_PK_GB_PING_XXL)lpFrame;

		sprintf(szMsg,"     Cli:	%s:%u",
			inet_ntoa(lpReqPcktX->m_iaExAddr),
			ntohs(lpReqPcktX->m_usExPort));
		DebugMessage(szMsg);
#endif //CC_DEBUG

		if(m_bServer&&m_bServerOpen){
			CC_PK_GB_PING_SMALL		RepPkt;
			LPCC_PK_GB_PING_XXL		lpReqPckt=NULL;
			lpReqPckt=(LPCC_PK_GB_PING_XXL)lpFrame;

			sockaddr_in			sinPeer;
			memset(&sinPeer,0x00,sizeof(sockaddr_in));

			// to client, that attempt to join : using client-supplied addresses
			for(int z=0;z<lpReqPckt->m_ucAddrNum;z++){
				sinPeer.sin_family=AF_INET;
				sinPeer.sin_addr.s_addr=lpReqPckt->m_iaMyAddr[z].s_addr;
				sinPeer.sin_port=htons(DATA_PORT);

				RepPkt.m_lProto=PROTO_ID;
				RepPkt.m_uType=CC_PT_PING_EMPTY;

				if(sendto(m_DataSocket,
					(char*)&RepPkt,
					sizeof(CC_PK_GB_PING_SMALL),
					0x00,
					(sockaddr*)&sinPeer,
					sizeof(sockaddr_in))==SOCKET_ERROR)
					return 0x00;

				// wait
				Sleep(50);
			};

			// to client, that attempt to join : using GW supplied addresses
			sinPeer.sin_family=AF_INET;
			sinPeer.sin_addr.s_addr=lpReqPckt->m_iaExAddr.s_addr;
			sinPeer.sin_port=lpReqPckt->m_usExPort;

			RepPkt.m_lProto=PROTO_ID;
			RepPkt.m_uType=CC_PT_PING_EMPTY;

			// sending UDP_HOLE_PINGS empty pings to client
			for(int k=0;k<UDP_HOLE_PINGS;k++)
				if(sendto(m_DataSocket,
					(char*)&RepPkt,
					sizeof(CC_PK_GB_PING_SMALL),
					0x00,
					(sockaddr*)&sinPeer,
					sizeof(sockaddr_in))==SOCKET_ERROR)
					return 0x00;
			/*        
			// back to server, informing the data has been sent
			RepLargePkt.m_lProto=PROTO_ID;
			RepLargePkt.m_uType=CC_PT_PING_SERVER;
			RepLargePkt.m_iaExAddr.s_addr=lpReqPckt->m_iaExAddr.s_addr;
			RepLargePkt.m_usExPort=lpReqPckt->m_usExPort;

			if(sendto (	m_DataSocket,
			(char *)&RepLargePkt,
			sizeof(CC_PK_GB_PING_LARGE),
			0x00,
			(sockaddr *)&SenderAddr,
			sizeof(sockaddr_in) )==SOCKET_ERROR)
			return 0x00;
			*/
			return 0x00;
		}else{
			_log_message("We get a request from GW to ping client, but we are not in server mode");
			return 0x00;
		};
	};

	if(lpFrame->m_uType==CC_PT_PING_EMPTY)
		return 0x00;

	if(	(lpFrame->m_PeerId!=BAD_PEER_ID)&&
		(GetPeerById(lpFrame->m_PeerId)==BAD_PEER_ID)&&
		(m_uPeerCount!=0)
	)	return 0x00;

	if(lpFrame->m_lStamp)
		SendConfirmDataPacket(&SenderAddr,lpFrame->m_lStamp);

	if(lpPeerId)
		*lpPeerId=lpFrame->m_PeerId;

	switch(lpFrame->m_uType){
		case CC_PT_SEND_DATA		:
			lpDataPacket=(LPCC_PK_SEND_DATA)lpFrame->m_bData;
			if(lpbBuffer)
				memcpy(lpbBuffer,lpDataPacket->m_bData,lpDataPacket->m_uSize);
			return lpDataPacket->m_uSize;
			break;
		case CC_PT_FRAME_CONFIRM		:
			lpDataConfirmPacket=(LPCC_PK_FRAME_CONFIRM)lpFrame->m_bData;
			QueueDropConfirmedPacket(lpDataConfirmPacket->m_lConfirmStamp);
			return 0x00;
			break;
		case CC_PT_CONNECT_REJECT	:
			if(m_bServer)
				return 0x00;

			lpConnectReject=(LPCC_PK_CONNECT_REJECT)lpFrame->m_bData;
			m_csState=csRejected;
			m_uRejectReason=lpConnectReject->m_uReason;
			return 0x00;
			break;			
		case CC_PT_CONNECT_OK		:
			if(m_bServer)
				return 0x00;

			lpConnectOk=(LPCC_PK_CONNECT_OK)lpFrame->m_bData;
			m_csState=csConnected;
			m_piNumber=lpConnectOk->m_Id;
			strcpy(m_szSessionName,lpConnectOk->m_szSessionName);
			m_dwOptions=lpConnectOk->m_dwOptions;
			return 0x00;
			break;			
		case CC_PT_TRY_CONNECT		:
			lpTryConnect=(LPCC_PK_TRY_CONNECT)lpFrame->m_bData;
			if(!m_bServer){
				SendConnectReject(&SenderAddr,CE_NOT_SERVER);
				return 0x00;
			};
			if(m_uPeerCount==m_uMaxPeers){
				SendConnectReject(&SenderAddr,CE_NO_ROOM);
				return 0x00;
			};
			if(lpTryConnect->m_cProtoVersion!=CC_PROTO_VERSION){
				SendConnectReject(&SenderAddr,CE_BAD_VERSION);
				return 0x00;
			};
			if(m_ssState!=ssOpen){
				SendConnectReject(&SenderAddr,CE_SESSION_CLOSED);
				return 0x00;
			};
			if((PeerNum=GetPeerByCCUID(lpTryConnect->m_szCCUID))!=BAD_PEER_ID){
				if(!SendConnectOk(&SenderAddr,m_PeerList[PeerNum].m_Id))
					return 0x00;

				SendServerList();

				return 0x00;
				break;
			};

			m_PeerList[m_uPeerCount].m_bAlive=TRUE;
			m_PeerList[m_uPeerCount].m_ex_Addr.s_addr=SenderAddr.sin_addr.s_addr;
			m_PeerList[m_uPeerCount].m_ex_Port=SenderAddr.sin_port;
			m_PeerList[m_uPeerCount].m_Id=++m_piAutoInc;
			m_PeerList[m_uPeerCount].m_uLatency=0;
			m_PeerList[m_uPeerCount].m_lpbUserData=NULL;
			m_PeerList[m_uPeerCount].m_uUserDataSize=0;
			strcpy(m_PeerList[m_uPeerCount].m_szUserName,lpTryConnect->m_szUserName);
			memcpy(m_PeerList[m_uPeerCount].m_szCCUID,lpTryConnect->m_szCCUID,22);

			// Over NAT ?
			m_PeerList[m_uPeerCount].m_bOverNAT=TRUE;

			for(i=0;i<lpTryConnect->m_uAddrCount;i++)
				if( (lpTryConnect->m_dwAddrList[i]==SenderAddr.sin_addr.s_addr) &&
					(SenderAddr.sin_port==htons(DATA_PORT) ) )
						m_PeerList[m_uPeerCount].m_bOverNAT=FALSE;

			if(!SendConnectOk(&SenderAddr,m_piAutoInc))
				return 0x00;
			
			m_uPeerCount++;

			SendServerList();

			return 0x00;
			break;
		case CC_PT_SERVER_LIST		:
			if(m_bServer)
				return 0x00;

			lpServerList=(LPCC_PK_SERVER_LIST)lpFrame->m_bData;
			ProcessServerList(lpServerList);
			return 0x00;
			break;
		case CC_PT_HOST_EXIT		:
			if(!m_bServer)
				return 0x00;
			
			lpHostExit=(LPCC_PK_HOST_EXIT)lpFrame->m_bData;

			uPeer=GetPeerById(lpFrame->m_PeerId);
			
			if(uPeer==BAD_PEER_ID)
				return 0x00;

			DropPeer(uPeer);

//			SendDropOk(&SenderAddr);

			SendServerList();

			return 0x00;
			break;
		case CC_PT_HOST_DROP		:
			if(m_bServer)
				return 0x00;
			DropPeer(0);
			return 0x00;
			break;
		case CC_PT_HOST_DROP_OK		:
			m_csState=csNone;
			m_ssState=ssNone;
			return 0x00;
			break;
		case CC_PT_SEND_USER_NAME	:
			if(!m_bServer)
				return 0x00;

			lpUserName=(LPCC_PK_SEND_USER_NAME)lpFrame->m_bData;
			uPeer=GetPeerById(lpFrame->m_PeerId);
			
			if(uPeer==BAD_PEER_ID)
				return 0x00;

			strcpy(m_PeerList[uPeer].m_szUserName,lpUserName->m_szUserName);
//LOOK->SendNewName()
//			SendServerList();
			SendNewName(lpFrame->m_PeerId);

			return 0x00;
			break;
		case CC_PT_SEND_USER_DATA	:
			if(!m_bServer)
				return 0x00;

			lpUserData=(LPCC_PK_SEND_USER_DATA)lpFrame->m_bData;
			uPeer=GetPeerById(lpFrame->m_PeerId);
			
			if(uPeer==BAD_PEER_ID)
				return 0x00;

			SAFE_FREE(m_PeerList[uPeer].m_lpbUserData);

			m_PeerList[uPeer].m_lpbUserData=(LPBYTE)malloc(lpUserData->m_uUserDataSize);
			assert(m_PeerList[uPeer].m_lpbUserData);

			memcpy(m_PeerList[uPeer].m_lpbUserData,lpUserData->m_UserData,lpUserData->m_uUserDataSize);
			m_PeerList[uPeer].m_uUserDataSize=lpUserData->m_uUserDataSize;
//LOOK->SendNewData()
//			SendServerList();
			SendNewData(lpFrame->m_PeerId);

			return 0x00;
			break;
		case CC_PT_SEND_NEW_NAME	:
			if(m_bServer)
				return 0x00;

			lpSendNewName=(LPCC_PK_SEND_NEW_NAME)lpFrame->m_bData;
			
			uPeer=GetPeerById(lpSendNewName->m_PeerId);

			if(uPeer==BAD_PEER_ID)
				return 0x00;

			strcpy(m_PeerList[uPeer].m_szUserName,lpSendNewName->m_szUserName);

			return 0x00;

			break;
		case CC_PT_SEND_NEW_DATA	:
			if(m_bServer)
				return 0x00;

			lpSendNewData=(LPCC_PK_SEND_NEW_DATA)lpFrame->m_bData;

			uPeer=GetPeerById(lpSendNewData->m_PeerId);

			if(uPeer==BAD_PEER_ID)
				return 0x00;

			SAFE_FREE(m_PeerList[uPeer].m_lpbUserData);

			m_PeerList[uPeer].m_lpbUserData=(LPBYTE)malloc(lpSendNewData->m_uUserDataSize);
			assert(m_PeerList[uPeer].m_lpbUserData);

			m_PeerList[uPeer].m_uUserDataSize=lpSendNewData->m_uUserDataSize;

			memcpy(	m_PeerList[uPeer].m_lpbUserData,
					lpSendNewData->m_UserData,
					lpSendNewData->m_uUserDataSize);

			return 0x00;

			break;
		default						:
			break;
	};
	return 0x00;
}

// ---------------------------------------------------------------------------------------------
