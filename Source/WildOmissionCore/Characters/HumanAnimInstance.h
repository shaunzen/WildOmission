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
	
	UFUNCTION(BlueprintCallable)
	void OnPrimaryMontageClimax();

	UFUNCTION(BlueprintCallable, BlueprintThreadSafe)
	float GetHeadAngle() const;

	UFUNCTION(BlueprintCallable, BlueprintThreadSafe)
	UAnimSequence* GetEquipedItemPose() const;

private:
	UPROPERTY(EditDefaultsOnly)
	bool FirstPersonInstance;

};
