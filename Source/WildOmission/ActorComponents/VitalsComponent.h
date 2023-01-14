// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VitalsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UVitalsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVitalsComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetMaxHealth();
	float GetMaxThirst();
	float GetMaxHunger();

	float GetHealth();
	float GetThirst();
	float GetHunger();

	void SetHealth(float Value);
	void SetThirst(float Value);
	void SetHunger(float Value);

	void LogVitals();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	
	float MaxHealth;
	float MaxThirst;
	float MaxHunger;
	
	float HealthDepletionRate;
	float ThirstDepletionRate;
	float HungerDepletionRate;

	float ThirstThreshold;
	float HungerThreshold;

	float CurrentHealth;
	float CurrentThirst;
	float CurrentHunger;
	
	// Replicated Values
	UPROPERTY(Replicated)
	float ReplicatedCurrentHealth;
	UPROPERTY(Replicated)
	float ReplicatedCurrentThirst;
	UPROPERTY(Replicated)
	float ReplicatedCurrentHunger;

	void CalculateDepletion();
	// TODO server add hunger/thirst/health

};
