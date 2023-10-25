// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SwingableItem.h"
#include "Enums/ToolType.h"
#include "ToolItem.generated.h"

UCLASS()
class EQUIPABLEITEMS_API AToolItem : public ASwingableItem
{
	GENERATED_BODY()

public:
	AToolItem();

	virtual void OnPrimaryHeld() override;

	
	float GetGatherMultiplier() const;
	
	bool IsQualityTool() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TEnumAsByte<EToolType> ToolType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float GatherMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool QualityTool;

	virtual void OnSwingImpact(const FHitResult& HitResult, const FVector& OwnerCharacterLookVector, bool FromFirstPersonInstance) override;

};