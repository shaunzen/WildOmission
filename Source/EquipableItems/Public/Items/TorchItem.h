// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "TorchItem.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UAudioComponent;
class UPointLightComponent;

UCLASS()
class EQUIPABLEITEMS_API ATorchItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	ATorchItem();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnSecondaryPressed() override;

	virtual void OnUnequip() override;

private:
	UPROPERTY(VisibleInstanceOnly, Replicated, ReplicatedUsing = OnRep_IsBurning)
	bool IsBurning;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* FireParticleSystem;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* BurningSound;

	UPROPERTY(EditDefaultsOnly)
	float LightBrightness;

	UPROPERTY(EditDefaultsOnly)
	float LightRadius;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor LightColor;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* SpawnedFireParticles;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* SpawnedBurningSound;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* SpawnedLightComponent;

	UFUNCTION()
	void OnRep_IsBurning();

	FTimerHandle DecrementDurabilityTimerHandle;
	UFUNCTION()
	void DecrementDurability();

	USkeletalMeshComponent* GetMeshComponentToAttachTo() const;

	void StartFireEffects();
	void StopFireEffects();

};
