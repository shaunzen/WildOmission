// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActorSaveData.generated.h"

USTRUCT()
struct WORLDGENERATION_API FActorComponentSaveData
{
	GENERATED_BODY()

	FActorComponentSaveData();

	UPROPERTY()
	FName Name;

	UPROPERTY()
	UClass* Class;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT()
struct WORLDGENERATION_API FActorSaveData
{
	GENERATED_BODY()

	FActorSaveData();

	UPROPERTY()
	FName Identifier;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;

	UPROPERTY()
	TArray<FActorComponentSaveData> ComponentData;

};