// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ragdolls/Ragdoll.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/ItemContainer.h"
#include "LootableRagdoll.generated.h"

class UInventoryComponent;

UCLASS()
class RAGDOLL_API ALootableRagdoll : public ARagdoll, public IItemContainer, public IInteractable
{
	GENERATED_BODY()
	
public:
	ALootableRagdoll();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Begin IItemContainer Implementation
	virtual void UnOccupy() override;
	// End IItemContainer Implementation

	// Begin IInteractable Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Implementation
	
	UFUNCTION(BlueprintCallable)
	FString GetContainerName() const;
	
	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventoryComponent() const;

protected:
	UPROPERTY(Replicated)
	bool bOccupied;

	UPROPERTY(EditDefaultsOnly, Category = "Item Container")
	FString ContainerName;

	virtual void OnContainerClosed();

private:
	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* InventoryComponent;

	UFUNCTION(Server, Reliable)
	void Server_UnOccupy();

};
