// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
//#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
//#include "WildOmission/Core/Interfaces/RequiredForLoad.h"
#include "Structs/InventoryItem.h"
#include "WorldItem.generated.h"

class UActorDespawnComponent;

UCLASS()
class INVENTORY_API AWorldItem : public AActor, public IInteractable//, public ISavableObjectInterface, public IRequiredForLoad
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated, EditAnywhere, SaveGame)
	FInventoryItem Item;

	UPROPERTY(VisibleAnywhere)
	UActorDespawnComponent* DespawnComponent;

	UPROPERTY(VisibleAnywhere, SaveGame)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	USoundBase* ClumpSound;

	UPROPERTY()
	USoundBase* PickupSound;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayClumpSound();

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayPickupSound();

};
