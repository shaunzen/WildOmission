// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

};
