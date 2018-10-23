#pragma once

constexpr auto MAX_BUFF_SIZE	= 4096;
constexpr auto MAX_PACKET_SIZE	= 255;

namespace MSGTYPE {
	enum MSGSTATE {
		NONE
		, LOGIN
		, DISCONNECT
	};

}

#pragma pack(push, 1)
struct PacketBase {
	PacketBase() : size(0), type(MSGTYPE::MSGSTATE::NONE) {}
	BYTE		size;
	BYTE		type;
};

struct PK_Login : public PacketBase {
	PK_Login(){
		size = sizeof(PK_Login);
		type = MSGTYPE::MSGSTATE::LOGIN;
		ZeroMemory(str, sizeof(str));
	}

	char str[100];
};
#pragma pack(pop)