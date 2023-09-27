// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasRecentlyDamaged.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSIONAI_API UBTDecorator_HasRecentlyDamaged : public UBTDecorator
{
	GENERATED_BODY()
public:

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	virtual void OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) override;
};
