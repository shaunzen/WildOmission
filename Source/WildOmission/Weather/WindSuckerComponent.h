// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "WindSuckerComponent.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API UWindSuckerComponent : public URadialForceComponent
{
	GENERATED_BODY()

public:
	UWindSuckerComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool HasLineOfSightToComponent(UPrimitiveComponent* InComponent) const;

};
