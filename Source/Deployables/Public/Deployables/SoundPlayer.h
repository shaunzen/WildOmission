// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "Interfaces/Interactable.h"
#include "SoundPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ASoundPlayer : public ADeployable, public IInteractable
{
	GENERATED_BODY()
	
public:
	ASoundPlayer();

	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;

private:
	UPROPERTY(EditDefaultsOnly)
	FString Prompt;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* Sound;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlaySound();

};
