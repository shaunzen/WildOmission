// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Interfaces/Interactable.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "CollectableResource.generated.h"

UCLASS()
class WILDOMISSION_API ACollectableResource : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableResource();

	//* Begin Interactable Interface Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	//* End Interactable Interface Implementation

private:
	UPROPERTY(EditDefaultsOnly)
	FInventoryItem Yeild;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	USoundBase* CollectSound;

	UFUNCTION(NetMulticast, Unreliable)
	void Client_PlayCollectSound();
};
