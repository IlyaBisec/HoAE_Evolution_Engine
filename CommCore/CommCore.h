#ifndef _COMM_CORE_H_INCLUDED_
#define _COMM_CORE_H_INCLUDED_

#pragma pack( push )
//
//	UDP based Communication Core
//	GSC Game World
//	� 2001 Serguei I. Ivantsov aka ManOwaR Linux Lover
//
//	Jun 22, 2001		V0.1
//	Jul 09, 2001		V0.2
//	Aug	06, 2001		V0.4
//	Dec 02, 2003		V0.5
//	Dec 02, 2003		V0.5
//  Sep 20, 2004		V0.6
//  Dec 02, 2004		V0.7
//
// ��� ���������� ������ �� NAT-��, ��������� ������ ������������� �������������
// ����� Microsoft �� ���������� UDP-������� ����� ����

#define _COOL_
//#define CC_DEBUG

#ifdef CC_DEBUG
#define _log_message(message) DebugMessage(message)
#else
#define _log_message(message) 
#endif //CC_DEBUG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <assert.h>
#ifdef CC_DEBUG
#include <stdio.h>
#endif //CC_DEBUG

#pragma warning (disable : 4200)

#pragma pack(1)

// ---------------------------------------------------------------------------------------------
// ����

typedef in_addr PEER_ADDR;
typedef u_short	PEER_PORT;
typedef u_short	PEER_ID,*LPPEER_ID;

// ---------------------------------------------------------------------------------------------
// �������� �������
#define CURRENT_TIME GetTickCount()				// ������� ����� � �������������
#define SAFE_FREE(P) if(P){free(P); P=NULL;}	// ������� ����������� ���������

	
// ---------------------------------------------------------------------------------------------
// ���������
#define PROTO_ID				0x47534370	// ������������� ��������� ('GSCp')
#define PROTO_BC				0x47534362	// ������������� ��������� ('GSCb')
#define DATA_PORT				34000		// ���� ������ (34000)
#define BCAST_PORT				34002		// ���� ������ ���� (34002)
#define RETRY_COUNT				8			// ������� ��� ����� �������� �������� �����
											// � ������ ���������������
											// ����� ���� ����� ��������� �� �������
#define RETRY_TIME				2800		// �������� ������� �������� ����� � �������������
#define REP_WAIT_TIME			2000		// �����, ��������� �� ���� ���������� � ��������
#define MAX_QUEUE_LEN			4096		// ������������ ����� ������� �������
											// ��������� �������������
#define MAX_PEERS				7			// ������������ ����� ������ 
#define CC_PROTO_VERSION		0x03		// ������ ����������������� ���������
#define MAX_PACKET_STAMP		0xEE6B2800	// ������������ ����� ������ (������������ ��� �������������)
#define RECV_BUFFER_LENGTH		32768		// ������ ��������� ������ (8 ��������)
#define MAX_HOST_NAME			32			// ������������ ����� ����� �����
#define UDP_HOLE_PINGS			5			//
// ---------------------------------------------------------------------------------------------
// �������� �����
typedef struct PEER_ENTRY{						// ����
	PEER_ADDR	m_ex_Addr;						// ������� ����� ����� (��� NAT-�)
	PEER_PORT	m_ex_Port;						// ������� ���� ����� (��� NAT-�)
	PEER_ID		m_Id;							// ������������� ����� � ������ �������
	BOOL		m_bAlive;						// �������� �� ���� �� ������ ������
	BOOL		m_bOverNAT;						// ��������� �� ���� �� NAT-�� �� ��������� � �������
	u_short		m_uLatency;						// ����� ������ �����
	CHAR		m_szUserName[MAX_HOST_NAME];	// �������� ����� / ������������
	u_short		m_uUserDataSize;				// ������ ���������������� ������
	LPBYTE		m_lpbUserData;					// ��������� �� ���������������� ������
	CHAR		m_szCCUID[23];					// ���������� ������������� �����
} *LPPEER_ENTRY;

// ---------------------------------------------------------------------------------------------
// ���� ������� (����������� � ���������)
#define CC_PT_FRAME_CONFIRM		0x01	// ���� ������������ ����� ������ ������
#define CC_PT_HOST_ALIVE		0x02	// ���� ������������, ��� �� �����; ��������� Latency
#define CC_PT_SEND_DATA			0x03	// ����� ���������������� ������� ��� �������������
#define CC_PT_SEND_DATA_SECURE	0x04	// ����� ���������������� ������� � ��������������
#define CC_PT_CHCK_HOST			0x05	// ����� ������� ����������� �����; ��������� Latency
#define CC_PT_TRY_CONNECT		0x06	// ������� ����������� � �������
#define CC_PT_CONNECT_OK		0x07	// ���������� �� ����������� � �������
#define CC_PT_CONNECT_REJECT	0x08	// ����� � ����������� � �������
#define CC_PT_SERVER_LIST		0x09	// �������� ������ ������ (�� �������)
#define CC_PT_HOST_EXIT			0x0A	// ���� ������� �� ����
#define CC_PT_HOST_DROP			0x0B	// ���� �������� �� ���� (�� �������)
#define CC_PT_CONNECT_DIFF		0x0C	// ����� �������� � ����� �� ������� �����, ���� �� ����� ��� ������
#define CC_PT_HOST_DROP_OK		0x0D	// ������ ������������ ���������� �����

#define CC_PT_SEND_USER_NAME	0x0E	// ������������ �������� ���� ���
#define CC_PT_SEND_USER_DATA	0x0F	// ������������ �������� ���� ������

#define CC_PT_SEND_NEW_NAME		0x10	// ������ �������� ������ �� ������������ ����� ������������
#define CC_PT_SEND_NEW_DATA		0x11	// ������ �������� ������ �� ������������ ������ ������������

#define CC_PT_BCAST_REQUEST		0x14	// ������ �������� ������ �� ����� ��������
#define CC_PT_BCAST_REPLY		0x15	// ������ �������� ������ � ����

#define CC_PT_PING_REQUEST		0x16	// 
#define CC_PT_PING_REPLY		0x17	// 
#define CC_PT_PING_CLIENT		0x18	// 
#define CC_PT_PING_SERVER		0x19	// 
#define CC_PT_PING_OK			0x1A	// 
#define CC_PT_PING_EMPTY		0x1B	// 
#define CC_PT_PING_INFO			0x1C	// 

//
// ---------------------------------------------------------------------------------------------
enum ConnectState{
	csNone,csWait,csRejected,csConnected,csTimedOut,csBadProto,csSynch
};

enum SessionState{
	ssNone,ssOpen,ssClosed
};

// ---------------------------------------------------------------------------------------------
// ������
#define	BAD_PEER_ID				255		// �������� ������������� �����
#define CE_NOT_SERVER			0x01	// ������� ����������� �� � �������
#define CE_NO_ROOM				0x02	// ��� �������� ������ �����������
#define CE_NO_ERROR				0x03	// ��� ������
#define CE_BAD_VERSION			0x04	// �������� ������ ���������
#define CE_BAD_MSG_SIZE			0x05	// ������������ ����� ���������
#define CE_SESSION_CLOSED		0x06	// ���������� ���������� ����, �.�. ������ ��� �������

// ---------------------------------------------------------------------------------------------
// ��������� �������

// Global ping request
typedef struct CC_PK_GB_PING_SMALL{
	u_long	m_lProto;				// ��� ��������� = PROTO_ID
	u_short	m_uType;				// ��� ������ = CC_PT_PING_REQUEST or CC_PT_PING_OK
} *LPCC_PK_GB_PING_SMALL;

// Global ping reply
typedef struct CC_PK_GB_PING_LARGE{
	u_long	m_lProto;				// ��� ��������� = PROTO_ID
	u_short	m_uType;				// ��� ������ = CC_PT_PING_REPLY or CC_PT_PING_CLIENT or CC_PT_PING_SERVER
	in_addr m_iaExAddr;				//
	u_short m_usExPort;				//
} *LPCC_PK_GB_PING_LARGE;

// Global ping XXL
typedef struct CC_PK_GB_PING_XXL{
	u_long	m_lProto;				// ��� ��������� = PROTO_ID
	u_short	m_uType;				// ��� ������ = CC_PT_PING_INFO
	in_addr m_iaExAddr;				// ����� ���������/����������
	u_short m_usExPort;				// ���� ���������/����������
	u_char	m_ucAddrNum;			// ����� �������������� ������, �������� � m_iaMyAddr
	in_addr m_iaMyAddr[8];			// :))
} *LPCC_PK_GB_PING_XXL;

// ��������� ������������������ ������
typedef struct CC_PK_BC_HEAD{
	u_long	m_lProto;				// ��� ��������� = PROTO_BC
	u_short	m_uType;				// ��� ������ = CC_PT_BCAST_REQUEST ��� CC_PT_BCAST_REPLY
} *LPCC_PK_BC_HEAD;

// ����� ����
typedef struct CC_PK_REQ_FRAME{
	CC_PK_BC_HEAD	m_Head;							// ���������	
} *LPCC_PK_REQ_FRAME;

// ����� �������
typedef struct CC_PK_REP_FRAME{
	CC_PK_BC_HEAD	m_Head;							// ���������	
	CHAR			m_szSessionName[MAX_HOST_NAME];	// �������� ������/������� �������
	CHAR			m_szUserName[MAX_HOST_NAME];	// ��� ������ ���������� ������/������� �������
	u_short			m_uPlayersCount;				// ����� �������������������� ������� �� �������
	BOOL			m_bAvailable;					// TRUE, ���� � ������� ����� ������������
} *LPCC_PK_REP_FRAME;

// �����������
typedef union CC_PK_BROADCAST{
	CC_PK_BC_HEAD	m_only_head;
	CC_PK_REQ_FRAME	m_req_frame;
	CC_PK_REP_FRAME	m_rep_frame;
} *LPCC_PK_BROADCAST;

// ����� ��� ����� ��� ���������
typedef struct CC_PK_RAW_FRAME{
	u_long	m_lProto;				// ��� ���������
	u_short	m_uType;				// ��� ������
	u_long	m_lStamp;				// ���������� ����� ������; 0, ���� �� ��������� �������������
	PEER_ID	m_PeerId;				// ������������� �����������
// ----------------------------------
	BYTE	m_bData[];				// ������������� ��� ������ ������ ���������� �����
} *LPCC_PK_RAW_FRAME;

// CC_PT_FRAME_CONFIRM
typedef struct CC_PK_FRAME_CONFIRM{
	u_long	m_lConfirmStamp;		// ���������� ����� ������, �������������� �����
} *LPCC_PK_FRAME_CONFIRM;

// CC_PT_SEND_DATA
typedef struct CC_PK_SEND_DATA{
	u_short	m_uSize;				// ������ ���� ������
	BYTE	m_bData[];				// ���� ������ ���������� �����
} *LPCC_PK_SEND_DATA;

// CC_PT_TRY_CONNECT
typedef struct CC_PK_TRY_CONNECT{
	CHAR		m_cProtoVersion;	// ����� ������ ��������� / ����������
	CHAR		m_szUserName[MAX_HOST_NAME];
	CHAR		m_szCCUID[23];
	u_short		m_uAddrCount;
	DWORD		m_dwAddrList[];
} *LPCC_PK_TRY_CONNECT;

// CC_PT_CONNECT_OK
typedef struct CC_PK_CONNECT_OK{
	PEER_ID		m_Id;							// ����� ����� � ������ �������
	CHAR		m_szSessionName[MAX_HOST_NAME];	// �������� ������
	DWORD		m_dwOptions;
} *LPCC_PK_CONNECT_OK;

// CC_PT_CONNECT_REJECT
typedef struct CC_PK_CONNECT_REJECT{
	u_short		m_uReason;			// ������� ������
} *LPCC_PK_CONNECT_REJECT;

// CC_PT_SERVER_LIST
typedef struct CC_PK_SERVER_LIST{
	CHAR		m_szSessionName[MAX_HOST_NAME];	// �������� ������
	u_short		m_uCount;						// ���������� ������
	BYTE		m_PeerList[];					// ������ ������ ���������� + ������������ ������ :)
} *LPCC_PK_SERVER_LIST;

// CC_PT_CHCK_HOST
typedef struct CC_PK_CHCK_HOST{
	DWORD		m_dwTickCount;		// ������� ����� �����
} *LPCC_PK_CHCK_HOST;

// CC_PT_HOST_ALIVE
typedef struct CC_PK_HOST_ALIVE{
	DWORD		m_dwTickCount;		// ����� ����� � ������ �������� ������ (�������������)
} *LPCC_PK_HOST_ALIVE;

// CC_PT_HOST_EXIT
typedef struct CC_PK_HOST_EXIT{
	DWORD		m_dwReserved;		//
} *LPCC_PK_HOST_EXIT;

// CC_PT_HOST_DROP
typedef struct CC_PK_HOST_DROP{
	DWORD		m_dwReserved;		//
} *LPCC_PK_HOST_DROP;

// CC_PT_HOST_DROP_OK
typedef struct CC_PK_HOST_DROP_OK{
	DWORD		m_dwReserved;		//
} *LPCC_PK_HOST_DROP_OK;

// CC_PT_SEND_USER_NAME
typedef struct CC_PK_SEND_USER_NAME{
	CHAR		m_szUserName[MAX_HOST_NAME];
} *LPCC_PK_SEND_USER_NAME;

// CC_PT_SEND_USER_DATA
typedef struct CC_PK_SEND_USER_DATA{
	u_short		m_uUserDataSize;
	BYTE		m_UserData[];
} *LPCC_PK_SEND_USER_DATA;

// CC_PT_SEND_NEW_NAME
typedef struct CC_PK_SEND_NEW_NAME{
	PEER_ID		m_PeerId;
	CHAR		m_szUserName[MAX_HOST_NAME];
} *LPCC_PK_SEND_NEW_NAME;

// CC_PT_SEND_NEW_DATA
typedef struct CC_PK_SEND_NEW_DATA{
	PEER_ID		m_PeerId;
	u_short		m_uUserDataSize;
	BYTE		m_UserData[];
} *LPCC_PK_SEND_NEW_DATA;

// ---------------------------------------------------------------------------------------------
// �������� ������ � ������� ���������, ��������� �������������
typedef struct FRAME_ENTRY{						// �����
	LPCC_PK_RAW_FRAME			m_lpFrame;		// ��������� �� �����
	u_short						m_uSize;		// ������ ������
	PEER_ADDR					m_PeerAddr;		// ����� �����
	PEER_PORT					m_PeerPort;		// ���� �����
	DWORD						m_dwSendTime;	// ����� ��������� �������� ������
												// � �������������
	u_short						m_uRetrCount;	// ���������� ������� �������� ������
} *LPFRAME_ENTRY;

// ---------------------------------------------------------------------------------------------
// ������ ��������
typedef struct SERVER_ENTRY{
	CHAR	m_szServerIP[16];				// IP-����� ������� � �������� �������
	CHAR	m_szSessionName[MAX_HOST_NAME];	// �������� ������/������� �������
	CHAR	m_szUserName[MAX_HOST_NAME];	// ��� ������ ���������� ������/������� �������
	u_short	m_uPlayersCount;				// ����� �������������������� ������� �� �������
	BOOL	m_bAvailable;					// TRUE, ���� � ������� ����� ������������
} *LPSERVER_ENTRY;

// ---------------------------------------------------------------------------------------------
// �������� ������� ��������� ������
typedef BOOL (CALLBACK* LP_CC_IDLE_PROC)();
typedef BOOL (CALLBACK* LP_CC_ENUM_PROC)(const PEER_ID PeerID, LPCSTR lpcszPeerName);

// ---------------------------------------------------------------------------------------------
// ����� ���� �����
class CCommCore  
{
// ---------------------------------------------------------------------------------------------
public:
// ---------------------------------------------------------------------------------------------
	LP_CC_IDLE_PROC	lpIdleProc;				// ������� ��������� ������, ���������� ����� �� �����
											// ������ ����������� �������
	LP_CC_ENUM_PROC	lpEnumProc;				// ������� ��������� ������ ��� ������������ ������
// ---------------------------------------------------------------------------------------------
											// ������� ��������� ���������� �����
	BOOL SendToPeer	(PEER_ID piNumber, LPBYTE lpbBuffer, u_short u_Size, BOOL bSecure=FALSE);
											// ������� ��������� ���� ������
	BOOL SendToAll	(LPBYTE lpbBuffer, u_short u_Size, BOOL bSecure=FALSE);
											
	u_short ReceiveData	(LPBYTE lpbBuffer, LPPEER_ID lpPeerId=NULL);	// ��������� ���� ����� �� �������
											// ������ ����� ����������, �� ��������� ������������ ������
	BOOL SendDropClient(PEER_ID PeerID);	// ������ ������ ������� �����
	BOOL InitClient	(LPCSTR lpcszServerIP, LPCSTR lpcszUserName);	// IP ����� � ���� ������ � ������� aaa.bbb.ccc.ddd
	BOOL DoneClient ();						// ������ �������� ������ �� �������� ������� 
	BOOL DoneServer ();						// ��������� ������ ������� ������ ��� �����
	BOOL DeletePeer (PEER_ID piNumber);		// ������� ���� �� ������ ������
	BOOL InitServer	(LPCSTR lpcszSessionName, LPCSTR lpcszUserName);	// �������������� ������
	BOOL QueueProcess();					// ������������ ������� �������� �������
											// ������ ����� ����������

	BOOL	Send();							// ��������� ������ ������
	BOOL	SendServerList();

	BOOL	IsOverNAT(PEER_ID PeerId);	


	LPCSTR	GetUserName(PEER_ID PeerId);	
	BOOL	SetUserName(LPCSTR lpcszUserName);	
	BOOL	SendUserName();

	BOOL	SetSessionName(LPCSTR lpcszSessionName);

	BOOL	GetUserData(PEER_ID PeerId, LPBYTE lpbUserData, u_short * puUserDataSize);
	BOOL	SetUserData(const LPBYTE lpcbUserData, u_short uUserDataSize);
	BOOL	SendUserData();

	VOID	GetServerAddress(LPSTR lpszServerAddress);

	BOOL	EnumPeers();

	BOOL	InitNetwork();					// ������������� �������� ����������
	BOOL	CloseNetwork();					// �������� �������� ����������

	CCommCore		();
	virtual ~CCommCore();

	PEER_ID	GetPeerID()						{	return m_piNumber;		}

	u_short GetPeersCount()					{	return m_uPeerCount;	}
	LPCSTR	GetSessionName()				{	return m_szSessionName;	}

	u_short	GetMaxPeers()					{	return m_uMaxPeers;		}
	VOID	SetMaxPeers(u_short uMaxPeers)	{	m_uMaxPeers=uMaxPeers;	}

	VOID	CloseSession()					{	m_ssState=ssClosed;		}
	SessionState GetSessionState()			{   return m_ssState;		}
	PEER_ID	GetPeerID( int idx )			{	return m_PeerList[idx].m_Id; }


	BOOL	IsClient()						{	return !m_bServer;		}
	BOOL	IsServer()						{	return m_bServer;		}

	VOID	SetOptions(DWORD dwOptions)		{	m_dwOptions=dwOptions;	}
	DWORD	GetOptions()					{	return m_dwOptions;		}

	// ������� �������� ����
	DWORD	GetRxBytes()					{	return m_dwRxBytes;		}
	// ������� ������� ����
	DWORD	GetTxBytes()					{	return m_dwTxBytes;		}
	// ������� ����������� ����
	DWORD	GetNxBytes()					{	return m_dwNxBytes;		}
	// ����� ����� ����������� ����� ��� ������ ��������� �����
	DWORD	GetRecvTimeOut()				{	return (CURRENT_TIME-m_dwLastPacketTime);	}

	VOID	SetInetMode(BOOL bInetMode)		{	m_bOverInternet=bInetMode;	}
	VOID	GetExtAddr(char* pszExtAddr);

	// ��������� ������� �������� �� ����������������� �������
	// ��������! 
	// ��� ������� ������ ���������� ����� �� ����� ����, ��� �������� ����
	VOID	DisableServer(		)			{	m_bServerOpen=FALSE;}
	// ��������� ������� �������� �� ����������������� �������
	VOID	EnableServer()					{	m_bServerOpen=TRUE;	}
	// ������ ����� ���� �� ������� ������� �������� (�������� ������)
	// ����� ������� � ������� REP_WAIT_TIME
	BOOL	StartServerPoll(LPCSTR lpcszServer=NULL);
	// �������� ��������� ������ ���� �� ������� ������� �������� (�������� ������)
	// ���������� ���������� ��������� ��������
	// ������ �������������
	// ...
	// if(StartServerPoll()){
	//		LPSERVER_ENTRY pServers=NULL;
	//		DWORD dwNS=EnumServers(pServers);
	//		while(dwNS){
	//			printf("%s",pServers[dwNS-1].m_szServerIP);
	//			dwNS--;
	//		}
	// }
	DWORD	EnumServers(LPSERVER_ENTRY &lpServers);

// ---------------------------------------------------------------------------------------------
protected:
// ---------------------------------------------------------------------------------------------
#ifdef CC_DEBUG
	FILE	*	m_DebugStream;	
	VOID		DebugMessage(LPCSTR lpcszMessage);
#endif //CC_DEBUG
	
	in_addr		m_gwSrvAddr;
	u_short		m_gwSrvPort;	
	DWORD		m_dwGwLastTick;

	CHAR		m_szUserName[MAX_HOST_NAME];		//
	PEER_ADDR	m_paServAddr;						//
	u_short		m_usServPort;						//

	PEER_ADDR	m_paHostAddr;						//
	u_short		m_usHostPort;						//

	DWORD		m_dwAddrList[8];					// ������ ������� ��������� ������ � network order �������
	u_short		m_uAddrCount;						// ���������� ������� ��������� ������

	DWORD		m_dwLastPacketTime;

	u_short		m_uMaxPeers;
	BOOL		m_bOverNAT;

	BOOL		m_bOverInternet;

	DWORD		m_dwRxBytes;
	DWORD		m_dwTxBytes;
	DWORD		m_dwNxBytes;

	DWORD		m_dwOptions;

	CHAR		m_szSessionName[MAX_HOST_NAME];		//

	CHAR		m_szCCUID[23];

	LPBYTE		m_lpbRecvBuffer;					//

	SessionState	m_ssState;						//
	ConnectState	m_csState;						//
	BOOL			m_bBlockingCall;				// � ������ ���������� �������������� ������
													// ������-�������������

	u_short		m_uRejectReason;					//

	u_short		m_uMaxMsgSize;						//

	SOCKET		m_DataSocket;						// �����, ������������� ���������������� ���������
	SOCKET		m_BCastSocket;						// �����, ������������� ����� ����

	u_long		m_lStamp;							// ���������������� ������� �������

	BOOL		m_bServer;							// �������� �� ���� ��������
	PEER_ID		m_piNumber;							// ������������� ����� � ������ �������
													// ��� ������� ������ ����� 0x01
	PEER_ID		m_piAutoInc;						// ���������������� ������� ������

	LPBYTE		m_lpbUserData;						// ������������ ������ � ������������
	u_short		m_uUserDataSize;					// ������ ������������ ������ � ������������

	PEER_ENTRY	m_PeerList[MAX_PEERS];				// ������ ��������� ������
	u_short		m_uPeerCount;						// ���������� ��������� ������

	FRAME_ENTRY m_FrameList[MAX_QUEUE_LEN];			// ������� ���������, ��������� �������������
	u_short		m_uFrameCount;						// ���������� ��������� � �������

	// ������ ��������

	BOOL			m_bServerOpen;					// TRUE, ���� ������������ ������ �� ���������

	LPSERVER_ENTRY	m_pServersList;					// ������ ��������
	u_short			m_uServersCount;				// ���������� �������� � ������
	DWORD			m_dwPollStart;					// ����� ������ ������. 0 - ���� ����� �������.

// ---------------------------------------------------------------------------------------------

//	PEER_ID		GetIdBySender();					// ���������� ���������� ����� �����
													// �� ��� ������/�����
	u_short GetPeerById(PEER_ID PeerId);			//
	u_short GetPeerByCCUID(LPCSTR lpcszCCUID);
	BOOL	DropPeer(u_short uPeer);					//

	BOOL InitSocket();
	BOOL CloseSocket();
	BOOL InitHost();

	BOOL QueueAddPacket(	PEER_ADDR			PeerAddr, 
							PEER_PORT			PeerPort, 
							LPCC_PK_RAW_FRAME	lpRawFrame,
							u_short				uSize);

	BOOL SendRawPacket(		PEER_ADDR			PeerAddr,				// ����� �����
							PEER_PORT			PeerPort,				// ���� �����
							u_short				uType,					// ��� ������
							LPBYTE				lpbBuffer,				// ����� � �������
							u_short				uSize,					// ������ ������ � ������
							BOOL				bSecureMessage,			// TRUE, ���� ����� ������-��������������� �������� ������
							BOOL				bWaitForCompletion);	// TRUE, ���� ����������� �����,
																		// �.�. ����� ������������� ��� ��������


	BOOL ProcessServerList(LPCC_PK_SERVER_LIST lpServerList);

	BOOL ProcessBroadcasts();
	
	BOOL ReSendFrame( u_short uFrameNum);
	VOID Cleanup();

	BOOL QueueDropPacket(int iFrameNum);
	BOOL QueueDropConfirmedPacket(u_long lStamp);
	BOOL QueueClearAll();
	BOOL QueuePacketExists(u_long lStamp);

	BOOL SendConfirmDataPacket(sockaddr_in * lpSender, u_long lStamp);
	BOOL SendConnectReject(sockaddr_in *lpSender, u_short uReason);
	BOOL SendConnectOk(sockaddr_in *lpSender, PEER_ID PeerId);
	BOOL SendDropOk(sockaddr_in *lpSender);

	BOOL SendNewName(PEER_ID PeerId);		// �������� ���������� � �����	(������)
	BOOL SendNewData(PEER_ID PeerId);		// �������� ���������� � ����	(������)

	VOID SetCommCoreUID(LPCSTR lpcszCCUID);
	VOID NewCommCoreUID(LPSTR lpszCCUID);
	VOID GetCommCoreUID(LPSTR lpszCCUID);

// ---------------------------------------------------------------------------------------------
};

#pragma warning (default : 4200)

#pragma pack( pop )

#endif // _COMM_CORE_H_INCLUDED_
