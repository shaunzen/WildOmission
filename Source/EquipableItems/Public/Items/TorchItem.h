// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "TorchItem.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class EQUIPABLEITEMS_API ATorchItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	ATorchItem();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnSecondaryPressed() override;

private:
	UPROPERTY(VisibleInstanceOnly, Replicated, ReplicatedUsing = OnRep_IsBurning)
	bool IsBurning;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* FireParticleSystem;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* BurningSound;

	UPROPERTY()
	UNiagaraComponent* SpawnedFireParticles;

	UPROPERTY()
	UAudioComponent* SpawnedBurningSound;

	UFUNCTION()
	void OnRep_IsBurning();

	FTimerHandle DecrementDurabilityTimerHandle;
	UFUNCTION()
	void DecrementDurability();

	USkeletalMeshComponent* GetMeshComponentToAttachTo() const;

	void StartFireEffects();
	void StopFireEffects();

};
