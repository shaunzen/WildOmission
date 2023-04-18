// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "WaterVolume.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API AWaterVolume : public APhysicsVolume
{
	GENERATED_BODY()
public:
	AWaterVolume();

private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WaterPlane;
};
