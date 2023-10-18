// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Torch.generated.h"

class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class EQUIPABLEITEMS_API ATorch : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	ATorch();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnSecondaryPressed() override;

private:
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* ParticleComponent;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = OnRep_IsBurning)
	bool IsBurning;

	UFUNCTION()
	void OnRep_IsBurning();

	FTimerHandle DecrementDurabilityTimerHandle;
	UFUNCTION()
	void DecrementDurability();

	void StartFireEffects();
	void StopFireEffects();

};
