// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/WildOmissionAICharacter.h"
#include "Structs/InventoryItem.h"
#include "Monster.generated.h"

class UPlayerInventoryComponent;
class UEquipComponent;
class UNiagaraComponent;
class ATimeOfDayHandler;

UCLASS()
class MONSTERS_API AMonster : public AWildOmissionAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Destroyed() override;

	APawn* GetTargetPawn() const;

	void Attack(AActor* Target);
	void StopAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
private:
	UPROPERTY(VisibleAnywhere)
	UPlayerInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere)
	UEquipComponent* EquipComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* FireEffects;

	UPROPERTY(EditDefaultsOnly)
	float MaxAttackRange;

	UPROPERTY()
	APawn* TargetPawn;

	FTimerHandle BurnDamageTimerHandle;

	void SetFire();
	void PutOutFire();

	UFUNCTION()
	void SetBurnDamageTimer();
	
	UFUNCTION()
	void ApplyBurnDamage();

};
