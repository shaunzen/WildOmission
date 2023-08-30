// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WildOmissionAIController.generated.h"

class UBehaviorTree;

UCLASS()
class WILDOMISSIONAI_API AWildOmissionAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;
	
};
