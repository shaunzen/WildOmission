// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AnimalAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class NPC_API AAnimalAIController : public AAIController
{
	GENERATED_BODY()
public:
	AAnimalAIController();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UBehaviorTreeComponent* BehaviorTreeComponent;
	
	UPROPERTY(VisibleAnywhere)
	UBlackboardComponent* BlackboardComponent;

};
