#include "../Precompiled/stdafx.h"
#include "../IOCP/IOCPContext.h"
#include "ClientSession.h"

ClientSession::ClientSession()
{
	Initialize();
}

ClientSession::~ClientSession()
{
	if (m_OverlappedEx != nullptr)
		delete m_OverlappedEx;

	if (m_ClientSock)
		closesocket(m_ClientSock);
}

ClientSession::ClientSession(ULONGLONG input_IOCPKey)
{
	Initialize();
	m_IOCPKey = input_IOCPKey;
}

void ClientSession::StartRecv()
{
	unsigned long flag = 0;

	WSARecv(
		GetSocket(),
		&m_OverlappedEx->wsabuf,
		1,
		NULL,
		&flag,
		&m_OverlappedEx->wsaOver,
		NULL
	);
}

bool ClientSession::Initialize()
{
	m_OverlappedEx = new OverlappedEx();
	ZeroMemory(&m_OverlappedEx->wsaOver, sizeof(WSAOVERLAPPED));
	m_IOCPKey = 0;
	m_OverlappedEx->opType = oExType::op_Recv;
	m_OverlappedEx->wsabuf.buf = m_OverlappedEx->io_buf;
	m_OverlappedEx->wsabuf.len = sizeof(m_OverlappedEx->io_buf);
	m_OverlappedEx->Owner = this;

	return true;
}