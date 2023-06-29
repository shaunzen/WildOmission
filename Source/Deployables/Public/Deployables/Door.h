// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployable.h"
#include "Interfaces/Interactable.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ADoor : public ADeployable, public IInteractable
{
	GENERATED_BODY()
	
public:
	ADoor();
	
	virtual void OnSpawn() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;

private:
	UPROPERTY(Replicated, SaveGame)
	bool bIsOpen;
	UPROPERTY(Replicated, SaveGame)
	FRotator SpawnRotation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Deployable Sound")
	USoundBase* OpenSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Deployable Sound")
	USoundBase* CloseSound;

	float RotateValue;
	FRotator DoorRotation;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayOpenSound();
	
	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayCloseSound();
};
