#pragma once

class OverlappedEx;

class ClientSession
{
private: 
	OverlappedEx		m_OverlappedEx;
	SOCKET				m_ClientSock		= NULL;
	WCHAR				m_ClientName[10]	= {};
	ULONGLONG			m_IOCPKey			= 0;

	int					m_PacketSize		= 0;
	int					m_PrevPacketsize;
	unsigned char		m_PrevPacket[MAX_PACKET_SIZE];
	
public:
	ClientSession();
	~ClientSession();

	ClientSession(const SOCKET& s, ULONGLONG IOCPKey_);

	void		SetSocket(const SOCKET& s) { m_ClientSock = s; }
	auto		GetSocket() { return m_ClientSock; }

	void		SetIOCPKey(ULONGLONG key) { m_IOCPKey = key; }
	auto		GetIOCPKey() { return m_IOCPKey; }

	void		StartRecv();
	void		ProcessRecv(ULONG data_recv);

	bool		Initialize();
};