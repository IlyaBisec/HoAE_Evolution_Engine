#ifndef _ICMP_PINGER_H_INCLUDED_
#define _ICMP_PINGER_H_INCLUDED_

#define _COOL_
/*
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <assert.h>
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATUS_FAILED		0xFFFF
#define DEF_PACKET_SIZE		32
#define MAX_PACKET			1024
#define ICMP_ECHO			8
#define ICMP_ECHOREPLY		0
#define ICMP_MIN			8				

typedef struct _ihdr{
  BYTE i_type;
  BYTE i_code; /* type sub code */
  USHORT i_cksum;
  USHORT i_id;
  USHORT i_seq;
}IcmpHeader,*PIcmpHeader;

class CPinger  
{
public:
//---------------------------------------------------------------------
	CPinger();
	virtual ~CPinger();
//---------------------------------------------------------------------
	BOOL	InitNetwork();		// ����������� �������� � ������
	BOOL	DoneNetwork();		// ���������� �������� � �����
//---------------------------------------------------------------------
	BOOL	SetTargetName(LPCSTR lpcszTargetName);
	// ���������� ��� �������������� �����
	// � ����� "xxx.yyy.zzz.sss" ���
	// � ����� "somehost.somedomain.net"
	// ���������� TRUE, ���� ��� Ok,
	// FALSE, ���� ������������ ���, ���
	// ��� ������ ��������� (resolve)
//---------------------------------------------------------------------
	BOOL	GetStatistic(	BYTE * lpcReplies,	// ���������� �������������
							BYTE * lpcTotal);	// ����� �������
	// �������� �������������� ����������
//---------------------------------------------------------------------
	BOOL	SendEcho();						//
	BOOL	RecvEcho();						//
//---------------------------------------------------------------------
protected:
	SOCKET			m_IcmpRawSocket;		//
	BOOL			m_bNetworkInitialized;	//
	BOOL			m_bTargetSet;			//
	sockaddr_in		m_TargetAddr;			//
	DWORD			m_dwLastSendTime;		//
	IcmpHeader		m_IcmpHeader;			//
	int				m_iSeqCounter;			//
	USHORT			checksum(USHORT *, int);
	BYTE			cReplies;
	BYTE			cTimeOuts;
	BYTE			cTotal;

};

#endif // #ifndef _ICMP_PINGER_H_INCLUDED_
