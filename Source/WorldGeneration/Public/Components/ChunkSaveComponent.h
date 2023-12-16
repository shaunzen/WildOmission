// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChunkSaveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLDGENERATION_API UChunkSaveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChunkSaveComponent();

	void Save(struct FChunkData& OutChunkData, bool AlsoDestroy = false);
	void Load(const struct FChunkData& InChunkData);

	static UClass* FindSavableObjectClassUsingIdentifier(const FName& Identifier);
	static FActorComponentSaveData FindComponentDataByName(const TArray<FActorComponentSaveData>& ComponentSaveList, const FName& ComponentName, UClass* ComponentClass);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class AChunk* GetOwnerChunk() const;

};
