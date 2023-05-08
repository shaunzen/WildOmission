// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "WildOmission/Core/Enums/ToolType.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "HarvestableResource.generated.h"

UCLASS()
class WILDOMISSION_API AHarvestableResource : public AActor, public ISavableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestableResource();

	virtual void OnHarvest(AActor* HarvestingActor);
	
	TEnumAsByte<EToolType> GetRequiredToolType() const;
	
	int32 GetTotalYield() const;
	
	void SetTotalYield(const int32& InTotalYield);

private:
	UPROPERTY(EditDefaultsOnly)
	FInventoryItem ItemYield;
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EToolType> RequiredToolType;
	
	UPROPERTY(EditDefaultsOnly, SaveGame)
	int32 TotalYield;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

};