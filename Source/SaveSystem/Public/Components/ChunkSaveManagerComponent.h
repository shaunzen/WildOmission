// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChunkSaveManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWorldGenerationCompletedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAVESYSTEM_API UChunkSaveManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChunkSaveManagerComponent();

	void Generate();
	FOnWorldGenerationCompletedSignature OnWorldGenerationCompleted;

	void Save(TArray<struct FChunkSaveData>& OutData);
	void Load(const TArray<struct FChunkSaveData>& InData);

private:

	UFUNCTION()
	void BroadcastGenerationCompleted();

};
