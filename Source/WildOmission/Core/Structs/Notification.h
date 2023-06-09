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
	float Time = 0.0f;

	UPROPERTY()
	float Duration = 0.0f;

	UPROPERTY()
	FString Message = FString();

	UPROPERTY()
	UMaterialInterface* Icon = nullptr;

};