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

private:
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EToolType> ToolType;
	
	UPROPERTY(EditDefaultsOnly)
	float GatherMultiplyer;
	
	UPROPERTY(EditDefaultsOnly)
	float EffectiveRangeCentimeters;
	
	UPROPERTY(EditDefaultsOnly)
	float SwingTimeSeconds;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlaySwingAnimation();

};