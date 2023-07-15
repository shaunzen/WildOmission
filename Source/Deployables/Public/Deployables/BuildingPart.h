// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployable.h"
#include "BuildingPart.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ABuildingPart : public ADeployable
{
	GENERATED_BODY()

public:
	ABuildingPart();

	float GetStability() const;
private:
	UPROPERTY()
	float Stability;

};
