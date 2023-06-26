// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/InventoryItem.h"
#include "Enums/ToolType.h"
//#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
//#include "WildOmission/Core/Interfaces/RequiredForLoad.h"
#include "HarvestableResource.generated.h"

UCLASS()
class GATHERABLERESOURCES_API AHarvestableResource : public AActor//, public ISavableObjectInterface, public IRequiredForLoad
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestableResource();

	virtual void OnHarvest(AActor* HarvestingActor, float GatherMultiplier);
	
	TEnumAsByte<EToolType> GetRequiredToolType() const;
	
	int32 GetDurability() const;
	
	void SetDurability(const int32& InDurability);

protected:
	UPROPERTY(EditDefaultsOnly)
	FInventoryItem ItemYield;

	UPROPERTY(EditDefaultsOnly)
	FInventoryItem SpecialItemDrop;
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EToolType> RequiredToolType;
	
	UPROPERTY(EditDefaultsOnly, SaveGame)
	int32 Durability;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	FInventoryItem HandleYield(float GatherMultiplier);
	bool ShouldGiveSpecialItemDrop();

};