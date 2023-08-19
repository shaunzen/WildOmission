// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Structs/InventoryItem.h"
#include "Monster.generated.h"

class UNavigationInvokerComponent;
class UVitalsComponent;
class UPlayerInventoryComponent;
class UEquipComponent;
class UDistanceDespawnComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonsterDespawnSignature, AMonster*, DespawningMonster);

UCLASS()
class MONSTERS_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FOnMonsterDespawnSignature OnDespawn;

	void PlayIdleSound();

	void Attack(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void OnAttackAnimationClimax();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	UNavigationInvokerComponent* NavigationInvoker;
	
	UPROPERTY(VisibleAnywhere)
	UVitalsComponent* VitalsComponent;
	
	UPROPERTY(VisibleAnywhere)
	UPlayerInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere)
	UEquipComponent* EquipComponent;

	UPROPERTY(VisibleAnywhere)
	UDistanceDespawnComponent* DespawnComponent;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* IdleSound;

	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryItem> Drops;

	UPROPERTY(EditDefaultsOnly)
	float MaxAttackRange;

	UPROPERTY()
	APawn* TargetPawn;

	UFUNCTION()
	void HandleDespawn();

	UFUNCTION()
	void HandleDeath();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayAttackEffects();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayIdleSound();

};
