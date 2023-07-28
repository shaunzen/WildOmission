// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlaySoundReplicated.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALS_API UBTTask_PlaySoundReplicated : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PlaySoundReplicated();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	UPROPERTY(EditAnywhere)
	USoundBase* Sound;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlaySound(const FVector& SoundLocation);

};
