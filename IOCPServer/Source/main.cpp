#include "Precompiled/stdafx.h"
#include "IOCP/IOCPManager.h"

int main()
{
	auto ServerInstance = IOCPManager::Instance();
	ServerInstance->StartThread();
	ServerInstance->StartListen();

	delete ServerInstance;
}