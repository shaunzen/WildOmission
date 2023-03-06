// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Interfaces/Interactable.h"
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

	void SetName(const FName& InName);
	void SetQuantity(const int32& InQuantity);
	void SetStats(const TArray<struct FItemStat>& InStats);
	void SetMesh(UStaticMesh* InMesh);
	void AddImpulse(FVector Impulse);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated, EditAnywhere)
	FName Name;

	UPROPERTY(Replicated, EditAnywhere)
	int32 Quantity;
	
	UPROPERTY(Replicated, EditAnywhere)
	TArray<struct FItemStat> Stats;

	UPROPERTY(Replicated, VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	USoundBase* PickupSound;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayPickupSound();

};
