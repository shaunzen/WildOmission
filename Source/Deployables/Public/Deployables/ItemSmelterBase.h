// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/ItemContainerBase.h"
#include "Structs/InventoryItem.h"
#include "ItemSmelterBase.generated.h"

USTRUCT(BlueprintType)
struct FSmeltResult
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInventoryItem RawItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInventoryItem SmeltedItem;

};

class UPointLightComponent;
class UNiagaraComponent;

UCLASS()
class DEPLOYABLES_API AItemSmelterBase : public AItemContainerBase
{
	GENERATED_BODY()
	
public:
	AItemSmelterBase();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void Server_ToggleState(bool NewState);
	
	bool IsTurnedOn() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void SmeltingTick();

	UFUNCTION()
	virtual void OnSmelt();

	virtual void OnTurnedOn();
	virtual void OnTurnedOff();

	UPROPERTY(EditDefaultsOnly, Category = "Smelting")
	float SmeltTimeSeconds;
	
	UPROPERTY(EditDefaultsOnly, Category = "Smelting")
	FInventoryItem FuelSource;
	
	UPROPERTY(EditDefaultsOnly, Category = "Smelting")
	TArray<FSmeltResult> Smeltables;

private:
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* LightComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* ParticlesComponent;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_TurnedOn", SaveGame)
	bool bTurnedOn;

	FTimerHandle SmeltTimerHandle;

	void FindAndSmeltAllSmeltables();

	UFUNCTION()
	void OnRep_TurnedOn();

	virtual void OnLoadComplete_Implementation() override;

};
