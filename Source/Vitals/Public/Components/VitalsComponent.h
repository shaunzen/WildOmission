// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VitalsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginThirstSignature, const float&, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndThirstSignature, const float&, Time);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginStarvingSignature, const float&, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndStarvingSignature, const float&, Time);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepletedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VITALS_API UVitalsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVitalsComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void SetHealth(float Value);
	
	UFUNCTION(BlueprintCallable)
	void SetThirst(float Value);
	
	UFUNCTION(BlueprintCallable)
	void SetHunger(float Value);

	UFUNCTION(BlueprintCallable)
	void AddHealth(float Value);

	UFUNCTION(BlueprintCallable)
	void AddThirst(float Value);

	UFUNCTION(BlueprintCallable)
	void AddHunger(float Value);
	
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxThirst() const;
	
	UFUNCTION(BlueprintCallable)
	float GetMaxHunger() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetThirst() const;

	UFUNCTION(BlueprintCallable)
	float GetHunger() const;

	UFUNCTION(BlueprintCallable)
	bool IsThirsty() const;

	UFUNCTION(BlueprintCallable)
	bool IsStarving() const;

	FOnBeginThirstSignature OnBeginThirst;
	FOnEndThirstSignature OnEndThirst;

	FOnBeginStarvingSignature OnBeginStarving;
	FOnEndStarvingSignature OnEndStarving;

	FOnHealthDepletedSignature OnHealthDepleted;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly)
	float MaxThirst;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxHunger;
	
	UPROPERTY(EditDefaultsOnly)
	float StartHealth;
	
	UPROPERTY(EditDefaultsOnly)
	float StartThirst;
	
	UPROPERTY(EditDefaultsOnly)
	float StartHunger;
	
	UPROPERTY(EditDefaultsOnly)
	float HealthDepletionRate;

	UPROPERTY(EditDefaultsOnly)
	float ThirstDepletionRate;
	
	UPROPERTY(EditDefaultsOnly)
	float HungerDepletionRate;

	UPROPERTY(EditDefaultsOnly)
	float ThirstThreshold;
	
	UPROPERTY(EditDefaultsOnly)
	float HungerThreshold;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* HurtSound;
	
	UPROPERTY(Replicated)
	float CurrentHealth;
	
	UPROPERTY(Replicated)
	float CurrentThirst;

	UPROPERTY(Replicated)
	float CurrentHunger;

	void CalculateDepletion();
	
	void HandleDelegates();

	bool BeginThirstBroadcasted;
	bool EndThirstBroadcasted;
	bool BeginStarvingBroadcasted;
	bool EndStarvingBroadcasted;

	void BroadcastBeginThirst();
	void BroadcastEndThirst();
	void BroadcastBeginStarving();
	void BroadcastEndStarving();

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayHurtSound();

	UFUNCTION()
	void OnOwnerTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
