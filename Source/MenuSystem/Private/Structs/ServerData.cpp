// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/ServerData.h"

FServerData::FServerData()
{
	Name = TEXT("ServerName");
	IsDedicated = false;
	CurrentPlayers = 0;
	MaxPlayers = 0;
	HostUsername = TEXT("Host");
	PingMS = 0;
}
