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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// Gets the local current health
	float GetHealth();
	// Gets the local current thirst
	float GetThirst();
	//Gets the local current hunger
	float GetHunger();

	// Sets the health to a new value, MUST be called from the server or else will do nothing
	void SetHealth(float Value);
	// Sets the thirst to a new value, MUST be called from the server or else will do nothing
	void SetThirst(float Value);
	// Sets the hunger to a new value, MUST be called from the server or else will do nothing
	void SetHunger(float Value);

	// Adds the specified value to health, MUST be called from the server or else will do nothing
	void AddHealth(float Value);
	// Adds the specified value to thirst, MUST be called from the server or else will do nothing
	void AddThirst(float Value);
	// Adds the specified value to hunger, MUST be called from the server or else will do nothing
	void AddHunger(float Value);

	float GetMaxHealth();
	float GetMaxThirst();
	float GetMaxHunger();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//*
	// Max Values
	//*
	float MaxHealth;
	float MaxThirst;
	float MaxHunger;
	
	//*
	// Start Values
	//*
	float StartHealth;
	float StartThirst;
	float StartHunger;

	//*
	// Depletion Rates
	//*
	float HealthDepletionRate;
	float ThirstDepletionRate;
	float HungerDepletionRate;

	//*
	// Threshold for health damage
	//*
	float ThirstThreshold;
	float HungerThreshold;

	//*
	// Current local values
	//*
	UPROPERTY(Replicated)
	float CurrentHealth;
	UPROPERTY(Replicated)
	float CurrentThirst;
	UPROPERTY(Replicated)
	float CurrentHunger;

	void CalculateDepletion();
	
};
