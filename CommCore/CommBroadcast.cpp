// Broadcast receiveing & processing routines
//
#include "CommCore.h"

BOOL	CCommCore::StartServerPoll(LPCSTR lpcszServer)
{
	sockaddr_in			sinPeer;
	CC_PK_REQ_FRAME		ReqFrame;
	
	memset(&sinPeer,0x00,sizeof(sockaddr_in));

	sinPeer.sin_family=AF_INET;

	if(lpcszServer)
		sinPeer.sin_addr.s_addr=inet_addr(lpcszServer);
	else
		sinPeer.sin_addr.s_addr=htonl(INADDR_BROADCAST);

	sinPeer.sin_port=htons(BCAST_PORT);

	ReqFrame.m_Head.m_lProto=PROTO_BC;
	ReqFrame.m_Head.m_uType=CC_PT_BCAST_REQUEST;

	m_dwPollStart=CURRENT_TIME;
	m_uServersCount=0;
	SAFE_FREE(m_pServersList);

	if(sendto (	m_BCastSocket,
				(LPCSTR)&ReqFrame,
				sizeof(CC_PK_REQ_FRAME),
				0x00,
				(sockaddr *)&sinPeer,
				sizeof(sockaddr_in)	)==SOCKET_ERROR)
		return FALSE;
	else
		return TRUE;    
}

DWORD	CCommCore::EnumServers(LPSERVER_ENTRY &lpServers)
{
	ProcessBroadcasts();

	lpServers=m_pServersList;

	return m_uServersCount;
}

BOOL	CCommCore::ProcessBroadcasts()
{
	sockaddr_in			SenderAddr,sinPeer;
	CC_PK_BROADCAST		RcvFrame;
	CC_PK_REP_FRAME		RepFrame;
	int					SenderLength;
	int					nBytesReceived;

	while(TRUE){

		SenderLength=sizeof(sockaddr_in);

		nBytesReceived=recvfrom(	m_BCastSocket,
									(LPSTR)&RcvFrame,
									sizeof(CC_PK_BROADCAST),
									0x00,
									(sockaddr *)&SenderAddr,
									&SenderLength);

		if(!nBytesReceived||nBytesReceived==SOCKET_ERROR)	
			break;

		if(RcvFrame.m_only_head.m_lProto!=PROTO_BC)
			continue;

		switch(RcvFrame.m_only_head.m_uType){
			case CC_PT_BCAST_REQUEST:	{
				if(!m_bServer)
					continue;
				memset(&RepFrame,0x00,sizeof(CC_PK_REP_FRAME));
			
				RepFrame.m_Head.m_lProto=PROTO_BC;
				RepFrame.m_Head.m_uType=CC_PT_BCAST_REPLY;
				strcpy(RepFrame.m_szSessionName,m_szSessionName);
				strcpy(RepFrame.m_szUserName,m_szUserName);
				RepFrame.m_bAvailable=m_bServerOpen;
				RepFrame.m_uPlayersCount=m_uPeerCount;

				memset(&sinPeer,0x00,sizeof(sockaddr_in));
	
				sinPeer.sin_family=AF_INET;
				sinPeer.sin_addr.s_addr=SenderAddr.sin_addr.s_addr;
				sinPeer.sin_port=htons(BCAST_PORT);
			
				sendto (	m_BCastSocket,
							(LPCSTR)&RepFrame,
							sizeof(CC_PK_REP_FRAME),
							0x00,
							(sockaddr *)&sinPeer,
							sizeof(sockaddr_in));
				break;
			}; // CC_PT_BCAST_REQUEST
			case CC_PT_BCAST_REPLY:		{
				if(!m_dwPollStart)
					continue;
				if((CURRENT_TIME-m_dwPollStart)>REP_WAIT_TIME){
					m_dwPollStart=0;
					continue;
				};
				m_uServersCount++;
				m_pServersList=(LPSERVER_ENTRY)realloc(m_pServersList,sizeof(SERVER_ENTRY)*m_uServersCount);
				m_pServersList[m_uServersCount-1].m_bAvailable=RcvFrame.m_rep_frame.m_bAvailable;
				strcpy(m_pServersList[m_uServersCount-1].m_szServerIP,inet_ntoa(SenderAddr.sin_addr));
				strcpy(m_pServersList[m_uServersCount-1].m_szSessionName,RcvFrame.m_rep_frame.m_szSessionName);
				strcpy(m_pServersList[m_uServersCount-1].m_szUserName,RcvFrame.m_rep_frame.m_szUserName);
				m_pServersList[m_uServersCount-1].m_uPlayersCount=RcvFrame.m_rep_frame.m_uPlayersCount;
				break;
			}; // CC_PT_BCAST_REPLY
		}; // switch
	}; // while

	return TRUE;
}














