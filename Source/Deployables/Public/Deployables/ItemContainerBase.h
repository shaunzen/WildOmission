// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployable.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/ItemContainer.h"
#include "ItemContainerBase.generated.h"

class UInventoryComponent;

UCLASS()
class DEPLOYABLES_API AItemContainerBase : public ADeployable, public IItemContainer, public IInteractable
{
	GENERATED_BODY()

public:
	AItemContainerBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Begin IItemContainer Implementation
	virtual void UnOccupy() override;
	// End IItemContainer Implementation

	// Begin IInteractable Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Implementation

	virtual void OnDeployableDestroyed() override;

	UFUNCTION(BlueprintCallable)
	FString GetContainerName() const;
	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventoryComponent() const;

protected:
	virtual bool CanOpen(AActor* Interactor) const;
	
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
