// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployable.h"
#include "Interfaces/Interactable.h"
#include "Door.generated.h"

class ULockComponent;

UCLASS()
class DEPLOYABLES_API ADoor : public ADeployable, public IInteractable
{
	GENERATED_BODY()
	
public:
	ADoor();
	
	virtual void OnSpawn() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Begin IInteractable Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Implementation

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* InteractionMesh;

	UPROPERTY(VisibleAnywhere)
	ULockComponent* LockComponent;

	UPROPERTY(Replicated, SaveGame)
	bool bIsOpen;
	
	UPROPERTY(EditDefaultsOnly, Category = "Deployable Sound")
	USoundBase* OpenSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Deployable Sound")
	USoundBase* CloseSound;

	float OpenAngle;
	
	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayOpenSound();
	
	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayCloseSound();
};
