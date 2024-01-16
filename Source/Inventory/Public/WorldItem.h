// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/SavableObject.h"
#include "Structs/InventoryItem.h"
#include "WorldItem.generated.h"

class UTimerDespawnComponent;

UCLASS()
class INVENTORY_API AWorldItem : public AActor, public IInteractable, public ISavableObject
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

	// Begin ISavableObject Implementation
	virtual FName GetIdentifier() const override;
	// End ISavableObject Implementation

	void SetItem(const FInventoryItem& InItem);
	void AddImpulse(FVector Impulse);

	void SetLastClump(AWorldItem* InLastClump);
	AWorldItem* GetLastClump() const;

	FInventoryItem GetItem() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Item, EditAnywhere, SaveGame)
	FInventoryItem Item;

	UPROPERTY(VisibleAnywhere)
	UTimerDespawnComponent* DespawnComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ItemRootComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	USoundBase* ClumpSound;

	UPROPERTY()
	USoundBase* PickupSound;

	UPROPERTY();
	AWorldItem* LastClump;

	void UpdateAttachedChunk();

	UFUNCTION()
	void OnRep_Item();

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayClumpSound();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayPickupSound();

	UFUNCTION()
	void OnLoadComplete_Implementation();

};
