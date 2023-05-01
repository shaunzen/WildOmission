// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerBase.h"
#include "Furnace.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API AFurnace : public AItemContainerBase
{
	GENERATED_BODY()
public:
	AFurnace();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_ToggleState();

private:
	UPROPERTY(Replicated)
	bool TurnedOn;

};
