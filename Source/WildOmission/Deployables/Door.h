// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployable.h"
#include "WildOmission/Core/Interfaces/Interactable.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API ADoor : public ADeployable, public IInteractable
{
	GENERATED_BODY()
	
public:
	ADoor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;

private:
	UPROPERTY(Replicated, SaveGame)
		bool bIsOpen;
};
