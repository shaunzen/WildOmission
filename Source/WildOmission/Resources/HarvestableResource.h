// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Structs/InventoryItem.h"
#include "WildOmission/Core/Enums/ToolType.h"
#include "HarvestableResource.generated.h"

UCLASS()
class WILDOMISSION_API AHarvestableResource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestableResource();

	virtual void OnHarvest(AActor* HarvestingActor);
	
	TEnumAsByte<EToolType> GetRequiredToolType() const;
	
	int32 GetDurability() const;
	
	void SetDurability(const int32& InDurability);

	FName GetIdentifier() const;
	void SetIdentifier(const FName& InIdentifier);

private:
	UPROPERTY(EditDefaultsOnly)
	FInventoryItem ItemYield;
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EToolType> RequiredToolType;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Durability;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	FName Identifier;

};