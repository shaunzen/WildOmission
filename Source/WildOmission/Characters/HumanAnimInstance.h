// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HumanAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API UHumanAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlaySwingAnimation();
};
