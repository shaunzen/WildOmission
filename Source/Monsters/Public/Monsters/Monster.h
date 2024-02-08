// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/WildOmissionAICharacter.h"
#include "Structs/InventoryItem.h"
#include "Monster.generated.h"

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

	void Attack();
	void StopAttack();

	bool IsOnFire() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UPlayerInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere)
	class UEquipComponent* EquipComponent;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* FireEffects;

	UPROPERTY(EditDefaultsOnly)
	float MaxAttackRange;

	void SetFire();
	void PutOutFire();

	UFUNCTION()
	void ApplyBurnDamage();

	UPROPERTY(EditDefaultsOnly)
	bool bCanBurnInLight;

};
