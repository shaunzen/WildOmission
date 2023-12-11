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
	void Load(const TArray<struct FChunkSaveData>& InData, const int32& SaveFileVersion);

private:
	struct FActorComponentSaveData FindComponentDataByName(const TArray<struct FActorComponentSaveData>& ComponentSaveList, const FName& ComponentName, UClass* ComponentClass = nullptr);

	static UClass* FindSavableObjectClassUsingIdentifier(const FName& Identifier);

	void FixSaveCompatibility(AActor* ActorToFix, const int32& OldSaveFileVersion);

	UFUNCTION()
	void BroadcastGenerationCompleted();

};
