// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployable.h"
#include "DeployableThatSpins.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API ADeployableThatSpins : public ADeployable
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	
};
