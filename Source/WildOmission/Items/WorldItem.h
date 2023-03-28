// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Interfaces/Interactable.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "WorldItem.generated.h"

UCLASS()
class WILDOMISSION_API AWorldItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//* Begin Interactable Interface implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	//* End Interactable Interface implementation

	void SetItem(const FInventoryItem& InItem);
	void AddImpulse(FVector Impulse);

	FInventoryItem GetItem() const;
	bool IgnoredInSave() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated, EditAnywhere)
	FInventoryItem Item;

	UPROPERTY(EditAnywhere)
	bool IgnoreInSave;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	USoundBase* PickupSound;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayPickupSound();

};
