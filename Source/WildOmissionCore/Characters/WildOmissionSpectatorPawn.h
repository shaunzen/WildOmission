// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "WildOmissionSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSIONCORE_API AWildOmissionSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	AWildOmissionSpectatorPawn();

private:
	UPROPERTY()
	class UChunkInvokerComponent* ChunkInvokerComponent;

};
