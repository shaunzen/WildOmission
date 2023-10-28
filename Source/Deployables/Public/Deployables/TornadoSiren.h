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

	void SoundAlarm();
	void StopAlarm();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UFUNCTION()
	void CheckForTornado();

};
