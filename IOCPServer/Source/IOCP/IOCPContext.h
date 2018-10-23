#pragma once
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#define oExType OverlappedEx::OperationType

class ClientSession;

class OverlappedEx {
public:
	enum class OperationType {
		op_None,
		op_Send,
		op_Recv,
		op_UpdateScene,
		op_UpdateSend,
		op_Regen,
		op_TerminateRoom
	};

public:
	WSAOVERLAPPED	wsaOver;
	WSABUF			wsabuf;
	char			io_buf[MAX_BUFF_SIZE];
	OperationType	opType = OperationType::op_None;
	ClientSession*	Owner = nullptr;
};