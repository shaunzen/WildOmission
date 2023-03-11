// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "Components/ActorComponent.h"
#include "HarvestableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UHarvestableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHarvestableComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnHarvest(AActor* HarvestingActor);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	FInventoryItem ItemYeild;

	UPROPERTY(EditDefaultsOnly)
	int32 Durability;

};
