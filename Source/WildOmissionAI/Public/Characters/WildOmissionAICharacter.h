// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WildOmissionAICharacter.generated.h"

class UNavigationInvokerComponent;
class UVitalsComponent;
class UDistanceDespawnComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAICharacterDespawnSignature, AWildOmissionAICharacter*, DespawningAICharacter);

UCLASS()
class WILDOMISSIONAI_API AWildOmissionAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWildOmissionAICharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FOnAICharacterDespawnSignature OnDespawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	float MinTimeBetweenIdleSoundSeconds;

	UPROPERTY(EditDefaultsOnly)
	float MaxTimeBetweenIdleSoundSeconds;

private:
	UPROPERTY(VisibleAnywhere)
	UNavigationInvokerComponent* NavigationInvoker;
	
	UPROPERTY(VisibleAnywhere)
	UVitalsComponent* VitalsComponent;
	
	UPROPERTY(VisibleAnywhere)
	UDistanceDespawnComponent* DespawnComponent;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* IdleSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> RagdollClass;

	UFUNCTION()
	void HandleDespawn();

	UFUNCTION()
	void HandleDeath();
	
	void SetIdleSoundTimer();

	UFUNCTION()
	void PlayIdleSound();

};
