// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WildOmissionSaveGame.generated.h"

USTRUCT()
struct FWildOmissionPlayerSave
{
	GENERATED_BODY()

	int32 ID = 0;
	FVector WorldLocation = FVector::ZeroVector;

};

UCLASS()
class WILDOMISSION_API UWildOmissionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UWildOmissionSaveGame();

	UPROPERTY()
	TArray<FWildOmissionPlayerSave> PlayerSaves;
};
