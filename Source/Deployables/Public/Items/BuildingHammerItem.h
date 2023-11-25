// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ToolItem.h"
#include "BuildingHammerItem.generated.h"

class ADeployable;
class ABuildingBlock;
class UBuildingHammerWidget;

UCLASS()
class DEPLOYABLES_API ABuildingHammerItem : public AToolItem
{
	GENERATED_BODY()

public:
	ABuildingHammerItem();

	virtual void OnPrimaryHeld() override;
	virtual void OnSecondaryPressed() override;

	virtual void OnUnequip() override;

	UFUNCTION(Server, Reliable)
	void Server_UpgradeCurrentDeployable();
	UFUNCTION(Server, Reliable)
	void Server_DestroyCurrentDeployable();

	bool GetLookingAtItemDurability(float& OutCurrentDurability, float& OutMaxDurability, FString& OutActorName) const;
	static FName GetResourceIDFromMaterialType(TEnumAsByte<EToolType> MaterialType);
	static FInventoryItem GetUpgradeCostForBuildingBlock(ABuildingBlock* BuildingBlock);
	static FInventoryItem GetDestructionRefundForDeployable(ADeployable* Deployable);

protected:
	virtual void OnSwingImpact(const FHitResult& HitResult, const FVector& OwnerCharacterLookVector, bool FromFirstPersonInstance) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxRepairAmount;

	UPROPERTY()
	UBuildingHammerWidget* Widget;

	UFUNCTION()
	void ClearWidget();

	bool HasAuthorization(const FVector& LocationToTest) const;

	void AttemptDeployableRepair(ADeployable* DeployableToRepair, const FHitResult& HitResult, const FVector& DirectionVector);
	bool CanRepairDeployable(ADeployable* DeployableToRepair, FInventoryItem& RepairCost) const;
	bool LineTraceOnVisibility(FHitResult& OutHitResult) const;

};
