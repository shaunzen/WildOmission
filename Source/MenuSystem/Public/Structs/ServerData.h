// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ServerData.generated.h"

USTRUCT()
struct MENUSYSTEM_API FServerData
{
	GENERATED_BODY()

	FServerData();

	UPROPERTY()
	FString Name;
	
	UPROPERTY()
	bool IsDedicated;

	UPROPERTY()
	uint16 CurrentPlayers;

	UPROPERTY()
	uint16 MaxPlayers;

	UPROPERTY()
	FString HostUsername;

	UPROPERTY()
	int32 PingMS;

};