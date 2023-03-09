// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HumanAnimInstance.generated.h"

UCLASS()
class WILDOMISSION_API UHumanAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UHumanAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlaySwingAnimation();

	UFUNCTION(BlueprintCallable)
	void PlayFootstepSound();

	UPROPERTY(BlueprintReadonly)
	float Speed;

	UPROPERTY(BlueprintReadonly)
	float Angle;

	UPROPERTY(BlueprintReadonly)
	float HeadAngle;

	UPROPERTY(BlueprintReadonly)
	bool Falling;

	UPROPERTY(BlueprintReadonly)
	bool HoldingItem;

private:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* GrassFootstepSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* GravelFootstepSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* RockFootstepSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* WoodFootstepSound;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* SwingMontage;

	UPROPERTY(EditDefaultsOnly)
	bool StopWalkAnimationWhenFalling;

	void CalculateSpeedAndAngle();
	
	void HandleFalling();

	void HandleItemHolding();
	
};
