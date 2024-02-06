// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChatMessage.generated.h"

/**
 * 
 */
USTRUCT()
struct GAMECHAT_API FChatMessage
{
	GENERATED_BODY()

	FChatMessage();
	FChatMessage(const FString& InSenderName, const FString& InMessage, const bool& InSenderIsAdministrator, const double& InTimeRecieved, const bool& InConnectionUpdate = false);

	UPROPERTY()
	FString SenderName;

	UPROPERTY()
	FString Message;

	UPROPERTY()
	bool SenderIsAdminisrator;

	UPROPERTY()
	double TimeRecieved;

	UPROPERTY()
	bool ConnectionUpdate;

};