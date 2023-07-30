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
	void PlayMontage(UAnimMontage* Montage, float MontagePlayRate);

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
	
protected:
	virtual void CalculateSpeedAndAngle();

private:
	float Speed;
	float Angle;
	bool Falling;
	bool Swimming;

	void HandleFalling();
	void HandleSwimming();

};
