// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
//#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
//#include "WildOmission/Core/Interfaces/RequiredForLoad.h"
#include "Structs/InventoryItem.h"
#include "CollectableResource.generated.h"

UCLASS()
class GATHERABLERESOURCES_API ACollectableResource : public AActor, public IInteractable//, public ISavableObjectInterface, public IRequiredForLoad
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
	FInventoryItem Yield;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	USoundBase* CollectSound;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayCollectSound();
};
