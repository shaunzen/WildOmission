// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ChunkInvokerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORLDGENERATION_API UChunkInvokerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChunkInvokerComponent();

	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	
	void SetRenderDistance(const uint8& InRenderDistance);
	uint8 GetRenderDistance() const;
	float GetRenderDistanceCentimeters() const;

	static TArray<UChunkInvokerComponent*> GetAllInvokers();
	static uint8 GetDefaultRenderDistance();
	static float GetDefaultRenderDistanceCentimeters();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	uint8 RenderDistance;
		
};
