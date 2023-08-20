// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerBase.h"
#include "TemporaryItemContainer.generated.h"

class UTimerDespawnComponent;

UCLASS()
class DEPLOYABLES_API ATemporaryItemContainer : public AItemContainerBase
{
	GENERATED_BODY()
public:
	ATemporaryItemContainer();
	
	// Begin IInteractable Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Implementation

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UTimerDespawnComponent* DespawnComponent;

	UPROPERTY(EditDefaultsOnly)
	bool CanBeInteractedWith;

};
