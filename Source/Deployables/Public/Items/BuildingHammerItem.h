// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ToolItem.h"
#include "BuildingHammerItem.generated.h"

UCLASS()
class DEPLOYABLES_API ABuildingHammerItem : public AToolItem
{
	GENERATED_BODY()

public:
	ABuildingHammerItem();

	virtual void Tick(float DeltaTime) override;

	virtual void OnPrimaryHeld() override;
	virtual void OnSecondaryPressed() override;

	virtual void OnUnequip() override;

	UFUNCTION(Server, Reliable)
	void Server_UpgradeCurrentDeployable();
	UFUNCTION(Server, Reliable)
	void Server_DestroyCurrentDeployable();

	bool GetLookingAtItemDurability(float& OutCurrentDurability, float& OutMaxDurability, FString& OutActorName) const;
	static FName GetResourceIDFromMaterialType(TEnumAsByte<EToolType> MaterialType);
	static FInventoryItem GetUpgradeCostForBuildingBlock(class ABuildingBlock* BuildingBlock);
	static FInventoryItem GetDestructionRefundForDeployable(class ADeployable* Deployable);

protected:
	virtual void OnSwingImpact(const FHitResult& HitResult, const FVector& OwnerCharacterLookVector, bool FromFirstPersonInstance) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxRepairAmount;

	UPROPERTY()
	class UBuildingHammerWidget* Widget;

	UFUNCTION()
	void ClearWidget();

	bool HasAuthorization(const FVector& LocationToTest) const;

	void AttemptDeployableRepair(class ADeployable* DeployableToRepair, const FHitResult& HitResult, const FVector& DirectionVector);
	bool CanRepairDeployable(class ADeployable* DeployableToRepair, FInventoryItem& RepairCost) const;
	bool LineTraceOnVisibility(FHitResult& OutHitResult) const;

	UFUNCTION(Client, Reliable)
	void Client_TeardownWidget();

};
