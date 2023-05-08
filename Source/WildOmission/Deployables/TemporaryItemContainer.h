// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerBase.h"
#include "TemporaryItemContainer.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API ATemporaryItemContainer : public AItemContainerBase
{
	GENERATED_BODY()

public:
	ATemporaryItemContainer();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay();

private:
	UPROPERTY(EditAnywhere, SaveGame)
	float DespawnTimeSeconds;

};
