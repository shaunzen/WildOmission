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
	
	UPROPERTY(BlueprintReadonly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	float Angle;
	
	UPROPERTY(BlueprintReadOnly)
	bool Falling;
	
	UPROPERTY(BlueprintReadOnly)
	bool Swimming;

protected:
	UFUNCTION()
	virtual void CalculateSpeedAndAngle();

private:
	UFUNCTION()
	void HandleFalling();
	
	UFUNCTION()
	void HandleSwimming();

};
