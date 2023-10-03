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

	UPROPERTY()
	FString SenderName = TEXT("NAN");

	UPROPERTY()
	FString Message;

	UPROPERTY()
	double TimeSent = 0.0;

	UPROPERTY()
	bool ConnectionUpdate = false;

};