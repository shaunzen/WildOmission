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

	//* Begin Interactable Interface implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	//* End Interactable Interface implementation

	// Sets the item id name for this world item
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Client_SetItemName(FName InName);

	// Sets the item quantity for this world item
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Client_SetItemQuantity(int32 InQuantity);
	
	// Sets the item mesh for this world item
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Client_SetItemMesh(UStaticMesh* InMesh);

	// Sets all properties for this world item in one go
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Client_SetItemProperties(FName InName, int32 InQuanitty = 1, UStaticMesh* InMesh = nullptr, FVector InLocation = FVector::ZeroVector);

	// Gets the item name
	UFUNCTION(BlueprintCallable)
	FName GetItemName();
	
	// Gets the item quantity
	UFUNCTION(BlueprintCallable)
	int32 GetItemQuantity();
	
	// Gets the items static mesh component
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetItemMesh();

	void AddImpulse(FVector Impulse);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	FName ItemName;

	UPROPERTY(EditAnywhere)
	int32 ItemQuantity;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY()
	USoundBase* PickupSound;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayPickupSound();

};
