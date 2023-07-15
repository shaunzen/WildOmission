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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	float GetStability() const;
	
	void CalculateStability(float OwnerStability);

private:
	UPROPERTY(Replicated)
	float Stability;

	void CalculateTouchingBuildingPartsStability();
};
