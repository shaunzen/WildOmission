// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployable.h"
#include "Seedling.generated.h"

UCLASS()
class DEPLOYABLES_API ASeedling : public ADeployable
{
	GENERATED_BODY()

public:
	ASeedling();
	
	virtual void OnSpawn() override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Seedling")
	TArray<TSubclassOf<AActor>> MatureStates;

	UPROPERTY(EditDefaultsOnly, Category = "Seedling")
	float MinMatureTimeSeconds;
	
	UPROPERTY(EditDefaultsOnly, Category = "Seedling")
	float MaxMatureTimeSeconds;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float TimeToMatureSeconds;

	void GrowUp();

};
