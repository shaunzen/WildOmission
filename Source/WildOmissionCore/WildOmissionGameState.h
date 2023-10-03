// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WildOmissionGameState.generated.h"

UCLASS()
class WILDOMISSIONCORE_API AWildOmissionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	bool IsCheatingEnabled() const;

};
