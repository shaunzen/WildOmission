// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Notification.generated.h"

USTRUCT()
struct FNotification
{
	GENERATED_BODY()

	UPROPERTY()
	FName Identifier;

	UPROPERTY()
	bool Negative;

	UPROPERTY()
	bool Positive;

	UPROPERTY()
	float Time;

	UPROPERTY()
	float Duration;

	UPROPERTY()
	FString Message;

	UPROPERTY()
	UMaterialInterface* Icon;

	FNotification();

};