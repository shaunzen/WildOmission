// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "AirRaidSiren.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API AAirRaidSiren : public ADeployable
{
	GENERATED_BODY()

public:
	AAirRaidSiren();

	void SoundAlarm();
	void StopAlarm();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	bool CurrentlyActive;

	UFUNCTION()
	void CheckForTornado();

};
