// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HumanAnimInstance.generated.h"

UCLASS()
class WILDOMISSIONCORE_API UHumanAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UHumanAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayMontage(UAnimMontage* Montage, float InMontagePlayRate = 1.0f);

	UFUNCTION(BlueprintCallable)
	void PlayFootstepSound();

	UFUNCTION(BlueprintCallable)
	void OnPrimaryMontageClimax();

	UPROPERTY(BlueprintReadonly)
	float Speed;

	UPROPERTY(BlueprintReadonly)
	float Angle;

	UPROPERTY(BlueprintReadonly)
	float HeadAngle;

	UPROPERTY(BlueprintReadonly)
	bool Falling;

	UPROPERTY(BlueprintReadonly)
	bool Swimming;

	UPROPERTY(BlueprintReadonly)
	bool HoldingItem;

	UPROPERTY(BlueprintReadonly)
	UAnimSequence* EquipedItemPose;

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
	USoundBase* WaterSplashSound;
	
	UPROPERTY(EditDefaultsOnly)
	bool FirstPersonInstance;

	void CalculateSpeedAndAngle();
	
	void CalculateHeadAngle();

	void HandleFalling();

	void HandleSwimming();

	void HandleEquipPose();
	
};
