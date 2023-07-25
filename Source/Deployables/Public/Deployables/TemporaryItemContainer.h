// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerBase.h"
#include "TemporaryItemContainer.generated.h"

class UTimerDespawnComponent;

UCLASS()
class DEPLOYABLES_API ATemporaryItemContainer : public AItemContainerBase
{
	GENERATED_BODY()
public:
	ATemporaryItemContainer();

private:
	UPROPERTY(VisibleAnywhere)
	UTimerDespawnComponent* DespawnComponent;

};
