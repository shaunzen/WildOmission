// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WildOmissionAICharacter.generated.h"

class UNavigationInvokerComponent;
class UVitalsComponent;
class UDistanceDespawnComponent;
class AWildOmissionAIRagdoll;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAICharacterDespawnSignature, AWildOmissionAICharacter*, DespawningAICharacter);

UCLASS()
class WILDOMISSIONAI_API AWildOmissionAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWildOmissionAICharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FOnAICharacterDespawnSignature OnDespawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	UNavigationInvokerComponent* NavigationInvoker;

	UPROPERTY(VisibleAnywhere)
	UVitalsComponent* VitalsComponent;

	UPROPERTY(VisibleAnywhere)
	UDistanceDespawnComponent* DespawnComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI Character")
	USoundBase* IdleSound;

	UPROPERTY(EditDefaultsOnly, Category = "AI Character")
	TSubclassOf<AWildOmissionAIRagdoll> RagdollClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI Character")
	float DefaultWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "AI Character")
	float DefaultRunSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "AI Character")
	float MinTimeBetweenIdleSoundSeconds;

	UPROPERTY(EditDefaultsOnly, Category = "AI Character")
	float MaxTimeBetweenIdleSoundSeconds;

	UFUNCTION()
	virtual void HandleDespawn();

	UFUNCTION()
	virtual void HandleDeath();

	UFUNCTION()
	virtual void StartRunning();

	UFUNCTION()
	virtual void StopRunning();

private:
	void SetIdleSoundTimer();

	UFUNCTION()
	void PlayIdleSound();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_DesiredMovementSpeed)
	float DesiredMovementSpeed;

	UFUNCTION()
	void OnRep_DesiredMovementSpeed();

};
