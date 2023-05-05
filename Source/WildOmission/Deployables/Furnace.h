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
	
	virtual void BeginPlay() override;

	virtual void OnTurnedOn();
	virtual void OnTurnedOff();

private:
	UPROPERTY(EditDefaultsOnly)
	float SmeltTimeInSeconds;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_TurnedOn", SaveGame)
	bool bTurnedOn;

	FTimerHandle SmeltTimerHandle;

	void SmeltingTick();

	UFUNCTION()
	void SmeltItems();

	UFUNCTION()
	void OnRep_TurnedOn();

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

};
