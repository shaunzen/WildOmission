// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WildOmissionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATION_API UWildOmissionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UWildOmissionAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void PlayFootstepSound();
	
	UPROPERTY(BlueprintCallable)
	float GetSpeed() const;

	UPROPERTY(BlueprintCallable)
	float GetAngle() const;
	
	UPROPERTY(BlueprintCallable)
	bool IsFalling() const;

	UPROPERTY(BlueprintCallable)
	bool IsSwimming() const;
	
private:
	float Speed;

	float Angle;

	bool Falling;

	bool Swimming;

	UPROPERTY()
	USoundBase* GrassFootstepSound;
	
	UPROPERTY()
	USoundBase* GravelFootstepSound;
	
	UPROPERTY()
	USoundBase* RockFootstepSound;
	
	UPROPERTY()
	USoundBase* WoodFootstepSound;
	
	UPROPERTY()
	USoundBase* WaterSplashSound;

	void CalculateSpeedAndAngle();
	void HandleFalling();
	void HandleSwimming();

};
