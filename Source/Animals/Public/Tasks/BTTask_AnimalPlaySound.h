// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AnimalPlaySound.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALS_API UBTTask_AnimalPlaySound : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AnimalPlaySound();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
