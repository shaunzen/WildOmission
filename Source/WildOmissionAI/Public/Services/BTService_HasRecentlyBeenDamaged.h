// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_HasRecentlyBeenDamaged.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSIONAI_API UBTService_HasRecentlyBeenDamaged : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_HasRecentlyBeenDamaged();

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector LastRememberedHealthKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void OnTakeAnyDamage(UBehaviorTreeComponent& OwnerComp);

};
