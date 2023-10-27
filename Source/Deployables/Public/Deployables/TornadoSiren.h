// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "TornadoSiren.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ATornadoSiren : public ADeployable
{
	GENERATED_BODY()

public:
	ATornadoSiren();

private:
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

};
