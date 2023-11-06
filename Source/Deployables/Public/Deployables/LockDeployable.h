// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "Interfaces/Interactable.h"
#include "LockDeployable.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ALockDeployable : public ADeployable, public IInteractable
{
	GENERATED_BODY()
	
public:
	// Begin IInteractable Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Implementation

};
