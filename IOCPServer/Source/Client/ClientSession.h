#pragma once

class OverlappedEx;

class ClientSession
{
private: 
	OverlappedEx		stOverlappedEx;
	SOCKET				Client_Sock;
	WCHAR				UserName[10] = {};
	unsigned long long	ID;

public:
	ClientSession();
	~ClientSession();

	bool		Initialize();
};

