// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/InventoryEquipableActor.h"
#include "EquipableItem.generated.h"

class UEquipComponent;

UCLASS()
class EQUIPABLEITEMS_API AEquipableItem : public AInventoryEquipableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipableItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the item is equiped into the players hands
	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID);

	// Called before the item is unequiped
	virtual void OnUnequip();

	// Primary function of the item. example(Eat, Shoot, Heal)
	virtual void OnPrimaryPressed();
	virtual void OnPrimaryHeld();
	virtual void OnPrimaryReleased();
	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance);

	// Secondary function of the item. example(Nothing, Aim, Heal others)
	virtual void OnSecondaryPressed();
	virtual void OnSecondaryHeld();
	virtual void OnSecondaryReleased();
	virtual void OnSecondaryAnimationClimax(bool FromFirstPersonInstance);

	// Reload function of the item. example(Reload the magazine)
	virtual void OnReloadPressed();
	virtual void OnReloadAnimationClimax(bool FromFirstPersonInstance);

	USkeletalMeshComponent* GetMeshComponent() const;
	
	void SetLocalVisibility(bool bVisible);

	int8 GetFromSlotIndex() const;

	void SetUniqueItemID(const uint32& InUniqueID);

	uint32 GetUniqueItemID() const;

	UAnimMontage* GetEquipMontage() const;
	UAnimMontage* GetEquipItemMontage() const;

	UAnimSequence* GetEquipPose() const;
	UAnimSequence* GetAimAdditivePose() const;

	FTransform GetSocketOffset();
	bool IsLeftHandMounted() const;

	bool PrimaryEnabled() const;
	bool SecondaryEnabled() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FTransform SocketOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool UseLeftHandMount;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* EquipItemMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* PrimaryMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* PrimaryItemMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* SecondaryMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* SecondaryItemMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* ReloadItemMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimSequence* EquipPose;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimSequence* AimAdditivePose;

	UPROPERTY()
	FName ItemName;

	UPROPERTY()
	int8 FromSlotIndex;

	UPROPERTY()
	uint32 UniqueID;

	bool bPrimaryEnabled;
	bool bSecondaryEnabled;

	APawn* GetOwnerPawn() const;
	UEquipComponent* GetOwnerEquipComponent() const;

};
