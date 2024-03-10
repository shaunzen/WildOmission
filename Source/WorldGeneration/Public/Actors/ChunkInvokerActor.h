// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkInvokerActor.generated.h"

UCLASS()
class WORLDGENERATION_API AChunkInvokerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkInvokerActor();

	void SetRenderDistance(const uint8& InRenderDistance);
	uint8 GetRenderDistance() const;

private:
	UPROPERTY(VisibleAnywhere)
	class UChunkInvokerComponent* ChunkInvokerComponent;

};
