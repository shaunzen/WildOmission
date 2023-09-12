// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RecurveBowAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONS_API URecurveBowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	URecurveBowAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool Drawn;

private:
	void HandleDrawn();

};
