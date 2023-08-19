// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_LastKnownLocation.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API UBTService_LastKnownLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_LastKnownLocation();

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector PlayerActorBlackboardKey;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
