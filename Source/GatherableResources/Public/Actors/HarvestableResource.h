// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/InventoryItem.h"
#include "Enums/ToolType.h"
#include "Interfaces/SavableObject.h"
#include "HarvestableResource.generated.h"

class UNavModifierComponent;

UCLASS()
class GATHERABLERESOURCES_API AHarvestableResource : public AActor, public ISavableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestableResource();

	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const;

	virtual void OnHarvest(AActor* HarvestingActor, float GatherMultiplier, bool IsQualityTool);
	
	TEnumAsByte<EToolType> GetRequiredToolType() const;
	
	int32 GetDurability() const;
	
	void SetDurability(const int32& InDurability);

	// Begin ISavableObject Implementation
	virtual FName GetIdentifier() const override;
	// End ISavableObject Implementation


protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryItem> CommonDrops;
	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryItem> RareDrops;
	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryItem> QualityToolDrops;

	// 1 = 100%, 0.5 = 50%, 0 = 0%
	UPROPERTY(EditDefaultsOnly)
	float NormalizedRareDropChance;

	// 1 = 100%, 0.5 = 50%, 0 = 0%
	UPROPERTY(EditDefaultsOnly)
	float NormalizedQualityToolDropChance;
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EToolType> RequiredToolType;
	
	UPROPERTY(EditDefaultsOnly, SaveGame)
	int32 Durability;

	UPROPERTY(EditDefaultsOnly, Category = "Save System")
	FName Identifier;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	UNavModifierComponent* NavigationModifier;

	FInventoryItem HandleYieldFromList(const TArray<FInventoryItem>& DropList, float GatherMultiplier);

};