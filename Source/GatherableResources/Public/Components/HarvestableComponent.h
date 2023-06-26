// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Structs/InventoryItem.h"
#include "Enums/ToolType.h"
#include "Components/ActorComponent.h"
#include "HarvestableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GATHERABLERESOURCES_API UHarvestableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHarvestableComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnHarvest(AActor* HarvestingActor);

	TEnumAsByte<EToolType> GetRequiredToolType() const;
	
	int32 GetDurability() const;

	void SetDurability(const int32& InDurability);
	
	FName GetType() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	FInventoryItem ItemYield;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EToolType> RequiredToolType;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Durability;

	UPROPERTY(EditDefaultsOnly)
	FName Type;

};
