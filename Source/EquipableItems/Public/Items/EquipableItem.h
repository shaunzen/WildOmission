// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/InventoryEquipableActor.h"
#include "EquipableItem.generated.h"

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

	// Primary function of the item. example(Eat, Shoot, Heal) Note: Called On Both Server and Client!
	virtual void OnPrimaryPressed();
	virtual void OnPrimaryHeld();
	virtual void OnPrimaryReleased();
	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance);

	// Secondary function of the item. example(Nothing, Aim, Heal others) Note: Called On Both Server and Client!
	virtual void OnSecondaryPressed();
	virtual void OnSecondaryHeld();
	virtual void OnSecondaryReleased();
	virtual void OnSecondaryAnimationClimax(bool FromFirstPersonInstance);

	// Reload function of the item. example(Reload the magazine) Note: Called On Both Server and Client!
	virtual void OnReloadPressed();
	virtual void OnReloadAnimationClimax(bool FromFirstPersonInstance);

	// For things like bolt action weapons
	virtual void OnChamberedAnimationClimax(bool FromFirstPersonInstance);

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
	class UEquipComponent* GetOwnerEquipComponent() const;
	struct FInventoryItem* FindInInventory() const;

};
