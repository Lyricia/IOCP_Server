#include "stdafx.h"
#include "IOCPManager.h"

int main()
{
	auto ServerInstance = IOCPManager::Instance();
	ServerInstance->StartThread();
	ServerInstance->StartListen();

	delete ServerInstance;
}