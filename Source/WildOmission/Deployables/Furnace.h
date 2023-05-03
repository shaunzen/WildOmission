// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainerBase.h"
#include "Furnace.generated.h"

class UPointLightComponent;
class UAudioComponent;

UCLASS()
class WILDOMISSION_API AFurnace : public AItemContainerBase
{
	GENERATED_BODY()
public:
	AFurnace();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void Server_ToggleState(bool bState);

	bool IsTurnedOn() const;

protected:
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void Client_OnTurnedOn();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Client_OnTurnedOff();

private:
	UPROPERTY(EditDefaultsOnly)
	float SmeltTimeInSeconds;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(Replicated)
	bool bTurnedOn;

	FTimerHandle SmeltTimerHandle;

	void SmeltingTick();

	UFUNCTION()
	void SmeltItems();

};
