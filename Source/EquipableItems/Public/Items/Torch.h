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

private:
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* ParticleComponent;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere)
	bool IsBurning;

};
