// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerBase.h"
#include "TemporaryItemContainer.generated.h"

class UActorDespawnComponent;

UCLASS()
class DEPLOYABLES_API ATemporaryItemContainer : public AItemContainerBase
{
	GENERATED_BODY()
public:
	ATemporaryItemContainer();

private:
	UPROPERTY(VisibleAnywhere)
	UActorDespawnComponent* DespawnComponent;

};
