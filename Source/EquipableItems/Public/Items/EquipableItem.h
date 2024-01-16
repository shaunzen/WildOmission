// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/InventoryEquipableActor.h"
#include "EquipableItem.generated.h"

class UEquipComponent;
struct FInventoryItem;

UCLASS()
class EQUIPABLEITEMS_API AEquipableItem : public AInventoryEquipableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipableItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the item is equiped into the players hands. Note: Only Called On Server!
	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID);

	// Called before the item is unequiped. Note: Only Called On Server!
	virtual void OnUnequip();

	// Primary function of the item. example(Eat, Shoot, Heal) Note: Only Called On Server!
	virtual void OnPrimaryPressed();
	virtual void OnPrimaryHeld();
	virtual void OnPrimaryReleased();
	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance);

	// Secondary function of the item. example(Nothing, Aim, Heal others) Note: Only Called On Server!
	virtual void OnSecondaryPressed();
	virtual void OnSecondaryHeld();
	virtual void OnSecondaryReleased();
	virtual void OnSecondaryAnimationClimax(bool FromFirstPersonInstance);

	// Reload function of the item. example(Reload the magazine) Note: Only Called On Server!
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

	FTransform GetSocketOffset() const;
	virtual FRotator GetRightArmVelocityOffset() const;
	virtual FRotator GetLeftArmVelocityOffset() const;
	
	bool IsOwnerOurPlayer() const;

	bool IsLeftHandMounted() const;

	bool PrimaryEnabled() const;
	bool SecondaryEnabled() const;

	static void PlayHitmarkerSound();
	static void PlayHeadshotHitmarkerSound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FTransform SocketOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FRotator RightArmVelocityOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FRotator LeftArmVelocityOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool UseLeftHandMount;

	// We use ActionMontage for the montage that the arms will play
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* EquipMontage;

	// We use ActionItemMontage for the montage that the actual item will play
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimMontage* EquipItemMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UAnimSequence* EquipPose;

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
	FInventoryItem* FindInInventory() const;

};
