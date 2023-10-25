// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ProjectileWeaponItem.h"
#include "BowItem.generated.h"


UCLASS()
class WEAPONS_API ABowItem : public AProjectileWeaponItem
{
	GENERATED_BODY()

public:
	ABowItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;

	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance) override;

	virtual void OnSecondaryPressed() override;
	virtual void OnSecondaryAnimationClimax(bool FromFirstPersonInstance) override;
	virtual void OnSecondaryReleased() override;

	UFUNCTION(BlueprintCallable)
	bool IsDrawn() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimSequence* DefaultEquipPose;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimSequence* DrawnEquipPose;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* DrawMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* DrawItemMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* ReleaseMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* ReleaseItemMontage;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Drawn)
	bool Drawn;

	UFUNCTION()
	void OnRep_Drawn();

};
