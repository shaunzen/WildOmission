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

	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;

	virtual void OnSecondaryPressed() override;
	virtual void OnSecondaryAnimationClimax(bool FromFirstPersonInstance) override;
	virtual void OnSecondaryReleased() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimSequence* DefaultEquipPose;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimSequence* DrawnEquipPose;

	UPROPERTY()
	bool AtFullDraw;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayThirdPersonSecondaryMontage();

};
