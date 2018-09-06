#pragma once
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

constexpr unsigned int MAX_BUFF_SIZE = 256;

enum enumOperation {
	op_Send,
	op_Recv,
	op_UpdateScene,
	op_UpdateSend,
	op_Regen,
	op_TerminateRoom
};

class OverlappedEx {
public:
	WSAOVERLAPPED	wsaOver;
	WSABUF			wsabuf;
	char			io_buf[MAX_BUFF_SIZE];
	enumOperation	op;
};