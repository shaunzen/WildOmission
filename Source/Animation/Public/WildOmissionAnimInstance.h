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
	UFUNCTION(BlueprintCallable)
	void PlayFootstepSound();
	
};
