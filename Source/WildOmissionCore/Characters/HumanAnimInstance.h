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
	void OnPrimaryAnimationClimax();
	UFUNCTION(BlueprintCallable)
	void OnSecondaryAnimationClimax();
	UFUNCTION(BlueprintCallable)
	void OnReloadAnimationClimax();

	UPROPERTY(BlueprintReadOnly)
	float HeadAngle;

	UPROPERTY(BlueprintReadOnly)
	UAnimSequence* EquipedItemPose;

	UPROPERTY(BlueprintReadOnly)
	FRotator RightArmOffset;

	UPROPERTY(BlueprintReadOnly)
	FRotator LeftArmOffset;

protected:
	virtual void CalculateSpeedAndAngle() override;

private:
	UPROPERTY(EditDefaultsOnly)
	bool FirstPersonInstance;

	UFUNCTION()
	void CalculateHeadAngle();

	UFUNCTION()
	void HandleEquipedItemPose();

	UFUNCTION()
	void HandleArmOffset();

};
