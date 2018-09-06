#include "stdafx.h"
#include "IOCPContext.h"
#include "ClientSession.h"
#include "IOCPManager.h"

IOCPManager * const IOCPManager::Instance()
{
	static IOCPManager* manager{ nullptr };
	if (manager == nullptr) {
		manager = new IOCPManager();
	}
	return manager;
}

IOCPManager::IOCPManager()
{
	if (!Initialize()) {
		cout << "Initialize Failed" << endl;
		exit(0);
	}
}

IOCPManager::~IOCPManager()
{
	Release();
	cout << "IOCP Manager Release Complete" << endl;
}

bool IOCPManager::Initialize()
{
	int retval;

	if (WSAStartup(MAKEWORD(2, 2), &m_WSA) != 0)
	{
		cout << "WinSock Error" << endl;
		return false;
	}
	cout << "WSAStartUP" << endl;

	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	m_ListenSock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_ListenSock == INVALID_SOCKET)
	{
		cout << "Listen Socket Error" << endl;
		return false;
	}
	cout << "Listen Socket Initialized" << endl;

	ZeroMemory(&m_ServerAddr, sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServerAddr.sin_port = htons(m_ServerPort);
	retval = ::bind(m_ListenSock, (SOCKADDR *)&m_ServerAddr, sizeof(m_ServerAddr));
	if (retval == SOCKET_ERROR)
	{
		cout << "Bind Error" << endl;
		return false;
	}
	cout << "Local Address Bind Initialized" << endl;

	return true;
}

bool IOCPManager::Release()
{
	for (auto& t : m_WorkerThread) { t.join(); }
	m_TimerThread.join();

	closesocket(m_ListenSock);
	cout << "close ListenSock" << endl;
	WSACleanup();
	cout << "WSACleanup" << endl;
	return true;
}

bool IOCPManager::StartThread()
{
	for (int i = 0; i < thread::hardware_concurrency(); ++i)
	{
		m_WorkerThread.push_back(thread{ [this]() { WorkerThreadFunc(); } });
	}

	m_TimerThread = thread{ [this]() { TimerThreadFunc(); } };

	cout << "Thread Initialized" << endl;

	return true;
}

bool IOCPManager::StartListen()
{
	int retval{ 0 };

	retval = listen(m_ListenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "Listening Failed" << endl;
		return false;
	}
	cout << "Start Server Listening" << endl;

	while (true)
	{
		SOCKET client_sock;
		SOCKADDR_IN clientaddr;
		int addrlen;
		UINT clientCounter = 0;

		ZeroMemory(&clientaddr, sizeof(SOCKADDR_IN));
		clientaddr.sin_family = AF_INET;
		clientaddr.sin_port = htons(m_ServerPort);
		clientaddr.sin_addr.s_addr = INADDR_ANY;
		addrlen = sizeof(clientaddr);

		client_sock = WSAAccept(m_ListenSock, (SOCKADDR*)&clientaddr, &addrlen, NULL, NULL);
		if (client_sock == INVALID_SOCKET)
		{
			cout << "client socket error" << endl;
			return false;
		}

//		CreateIoCompletionPort(
//			reinterpret_cast<HANDLE*>(newclient->sock),
//			m_hIOCP,
//			newclient->IOCPKey,
//			0
//		);

		unsigned long flag = 0;

//		WSARecv(
//			newclient->sock,
//			&newclient->OverlappedEx.wsabuf,
//			1,
//			NULL,
//			&flag,
//			&newclient->OverlappedEx.wsaOver,
//			NULL
//		);

	}
	
	return true;
}

void IOCPManager::WorkerThreadFunc()
{
	while (true)
	{
		unsigned long datasize;
		unsigned long long key;
		WSAOVERLAPPED* pOverlapped;

		BOOL isSuccess = GetQueuedCompletionStatus(m_hIOCP, &datasize, &key, &pOverlapped, INFINITE);
		OverlappedEx* o = reinterpret_cast<OverlappedEx*>(pOverlapped);
		
		if (0 == isSuccess)
		{
			// disconnect
			continue;
		}
		else if (0 == datasize && (o->op == op_Send || o->op == op_Recv))
		{
			// Disconnect
			continue;
		}

		// Send, Recv
		if (o->op == op_Recv)
		{
			// Need Buffer manager

			//ClientInfo* client = server->m_ClientFinder[key];
			//int Received_size = datasize;
			//char* ptr = o->io_buf;
			//
			//while (0 < Received_size)
			//{
			//	if (client->packetsize == 0)
			//		client->packetsize = ptr[0];
			//
			//	int remainsize = client->packetsize - client->prev_packetsize;
			//
			//	if (remainsize <= Received_size) {
			//		memcpy(client->prev_packet + client->prev_packetsize, ptr, remainsize);
			//
			//		MSGWRAPPER msg = new char[client->packetsize];
			//		memcpy(msg, client->prev_packet, client->packetsize);
			//		server->ProcessMSG(key, msg);
			//		delete msg;
			//
			//		Received_size -= remainsize;
			//		ptr += remainsize;
			//		client->packetsize = 0;
			//		client->prev_packetsize = 0;
			//
			//		//if (remainsize == 0) break;
			//	}
			//	else {
			//		memcpy(client->prev_packet + client->prev_packetsize, ptr, Received_size);
			//		client->prev_packetsize += Received_size;
			//	}
			//
			//}
			//unsigned long rFlag = 0;
			//ZeroMemory(&o->wsaOver, sizeof(OVERLAPPED));
			//WSARecv(client->sock, &o->wsabuf, 1, NULL, &rFlag, &o->wsaOver, NULL);
		}
	}
}

void IOCPManager::TimerThreadFunc()
{
	while (true) {

	}
}
