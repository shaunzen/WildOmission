// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ClosestVisiblePlayer.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSIONAI_API UBTService_ClosestVisiblePlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_ClosestVisiblePlayer();

protected:
	UPROPERTY(EditAnywhere)
	float MaxChaseDistance;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnValueChanged(bool ValueSet, UBehaviorTreeComponent& OwnerComp);

};
