#pragma once

class OverlappedEx;

class ClientSession
{
private: 
	OverlappedEx*		m_OverlappedEx		= nullptr;
	SOCKET				m_ClientSock		= NULL;
	WCHAR				m_ClientName[10]	= {};
	ULONGLONG			m_IOCPKey			= 0;

	int					m_PacketSize		= 0;

public:
	ClientSession();
	~ClientSession();

	ClientSession(ULONGLONG IOCPKey_);

	auto		GetSocket() { return m_ClientSock; }

	void		SetIOCPKey(ULONGLONG key) { m_IOCPKey = key; }
	auto		GetIOCPKey() { return m_IOCPKey; }

	void		StartRecv();

	bool		Initialize();
};