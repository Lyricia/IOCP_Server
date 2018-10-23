#pragma once

class ClientSession;

constexpr USHORT SERVERPORT = 9000;

class IOCPManager
{
private:
	WSADATA					m_WSA;
	SOCKET					m_ListenSock;
	SOCKADDR_IN				m_ServerAddr;
	HANDLE					m_hIOCP;

	vector<thread>			m_WorkerThread;
	thread					m_TimerThread;

	unordered_map<UINT, ClientSession*>		m_ClientList;

public:
	static IOCPManager* const Instance();
	
	IOCPManager();
	~IOCPManager();
	
	bool				Initialize();
	bool				Release();
	bool				StartThread();
	bool				StartListen();

	void				WorkerThreadFunc();
	void				TimerThreadFunc();
};