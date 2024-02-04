// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "WildOmissionGameSession.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSIONCORE_API AWildOmissionGameSession : public AGameSession
{
	GENERATED_BODY()

	virtual void RegisterServer() override;

};
