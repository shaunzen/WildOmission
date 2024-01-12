// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployable.h"
#include "Sapling.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ASapling : public ADeployable
{
	GENERATED_BODY()

public:
	ASapling();
	
	virtual void OnSpawn() override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sapling")
	float MinMatureTimeSeconds;

	UPROPERTY(EditDefaultsOnly, Category = "Sapling")
	float MaxMatureTimeSeconds;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float TimeToMatureSeconds;

	void GrowUp();
};
