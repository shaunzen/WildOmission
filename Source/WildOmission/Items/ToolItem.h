// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "WildOmission/Core/Enums/ToolType.h"
#include "ToolItem.generated.h"

UCLASS()
class WILDOMISSION_API AToolItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	AToolItem();

	virtual void Primary() override;

	virtual void Secondary() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsSwinging() const;

private:
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EToolType> ToolType;
	
	UPROPERTY(EditDefaultsOnly)
	float GatherMultiplyer;
	
	UPROPERTY(EditDefaultsOnly)
	float EffectiveRangeCentimeters;
	
	UPROPERTY(EditDefaultsOnly)
	float SwingTimeSeconds;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Swinging)
	bool Swinging;

	UFUNCTION()
	void OnRep_Swinging();

};