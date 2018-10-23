#include "../Precompiled/stdafx.h"
#include "../IOCP/IOCPContext.h"
#include "ClientSession.h"

ClientSession::ClientSession()
{
	Initialize();
}

ClientSession::~ClientSession()
{
	if (m_ClientSock)
		closesocket(m_ClientSock);
}

ClientSession::ClientSession(const SOCKET& s, ULONGLONG input_IOCPKey)
{
	Initialize();
	m_ClientSock = s;
	m_IOCPKey = input_IOCPKey;
}

void ClientSession::StartRecv()
{
	unsigned long flag = 0;

	//ZeroMemory(&m_OverlappedEx.wsaOver, sizeof(OVERLAPPED));
	WSARecv(
		m_ClientSock,
		&m_OverlappedEx.wsabuf,
		1,
		NULL,
		&flag,
		&m_OverlappedEx.wsaOver,
		NULL
	);
}

void ClientSession::ProcessRecv(ULONG data_recv)
{
	char* bufptr = m_OverlappedEx.io_buf;

	while (0 < data_recv)
	{
		if (m_PacketSize == 0)
			m_PacketSize = bufptr[0];

		ULONG remainsize = m_PacketSize - m_PrevPacketsize;

		if (remainsize <= data_recv) {
			memcpy(m_PrevPacket + m_PrevPacketsize, bufptr, remainsize);

			char* msg = new char[m_PacketSize];
			memcpy(msg, m_PrevPacket, m_PacketSize);
			cout << reinterpret_cast<PK_Login*>(msg)->str << endl;
			delete msg;

			// or
			cout << reinterpret_cast<PK_Login*>(m_PrevPacket)->str << endl;
			
			data_recv -= remainsize;
			bufptr += remainsize;

			m_PacketSize = m_PrevPacketsize = 0;
		}
		else {
			memcpy(m_PrevPacket + m_PrevPacketsize, bufptr, data_recv);
			m_PrevPacketsize += data_recv;
		}
	}

	StartRecv();
}

bool ClientSession::Initialize()
{
	ZeroMemory(&m_OverlappedEx.wsaOver, sizeof(WSAOVERLAPPED));
	m_IOCPKey = 0;
	m_OverlappedEx.opType = oExType::op_Recv;
	m_OverlappedEx.wsabuf.buf = m_OverlappedEx.io_buf;
	m_OverlappedEx.wsabuf.len = sizeof(m_OverlappedEx.io_buf);
	m_OverlappedEx.Owner = this;

	return true;
}