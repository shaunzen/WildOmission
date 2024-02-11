// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "Interfaces/Interactable.h"
#include "Sign.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ASign : public ADeployable, public IInteractable
{
	GENERATED_BODY()
	
public:
	ASign();

	// Begin IInteractable Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Implementation

private:
	UPROPERTY(VisibleAnywhere)
	class UTextRenderComponent* TextRenderComponent;

};
