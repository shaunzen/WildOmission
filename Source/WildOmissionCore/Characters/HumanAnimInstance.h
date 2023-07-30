// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/WildOmissionAnimInstance.h"
#include "HumanAnimInstance.generated.h"

UCLASS()
class WILDOMISSIONCORE_API UHumanAnimInstance : public UWildOmissionAnimInstance
{
	GENERATED_BODY()
	
public:
	UHumanAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void OnPrimaryMontageClimax();

	UPROPERTY(BlueprintReadOnly)
	float HeadAngle;

	UPROPERTY(BlueprintReadOnly)
	UAnimSequence* EquipedItemPose;

private:
	UPROPERTY(EditDefaultsOnly)
	bool FirstPersonInstance;

	UFUNCTION()
	void CalculateHeadAngle();

	UFUNCTION()
	void HandleEquipedItemPose();

};
