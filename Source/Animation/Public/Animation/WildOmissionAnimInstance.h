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

	UFUNCTION(BlueprintCallable)
	void PlayMontage(UAnimMontage* Montage, float MontagePlayRate);

	UFUNCTION(BlueprintCallable)
	void PlayFootstepSound();
	
	UFUNCTION(BlueprintCallable, BlueprintThreadSafe)
	virtual float GetSpeed() const;

	UFUNCTION(BlueprintCallable, BlueprintThreadSafe)
	virtual float GetAngle() const;
	
	UFUNCTION(BlueprintCallable, BlueprintThreadSafe)
	bool IsFalling() const;

	UFUNCTION(BlueprintCallable, BlueprintThreadSafe)
	bool IsSwimming() const;
	
};
