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

	// Called when the item is equiped into the players hands
	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID);

	// Called before the item is unequiped
	virtual void OnUnequip();

	// Primary function of the item. example(Eat, Shoot, Heal)
	virtual void OnPrimaryPressed();
	virtual void OnPrimaryHeld();
	virtual void OnPrimaryReleased();

	// Secondary function of the item. example(Nothing, Aim, Heal others)
	virtual void OnSecondaryPressed();
	virtual void OnSecondaryHeld();
	virtual void OnSecondaryReleased();

	UStaticMesh* GetMesh();
	
	void SetLocalVisibility(bool bVisible);

	int8 GetFromSlotIndex() const;

	void SetUniqueItemID(const uint32& InUniqueID);

	uint32 GetUniqueItemID() const;

	UAnimMontage* GetEquipMontage() const;

	UAnimSequence* GetEquipPose() const;

	FTransform GetSocketOffset();

	bool PrimaryEnabled() const;
	bool SecondaryEnabled() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	FTransform SocketOffset;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimSequence* EquipPose;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* EquipSound;

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

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayThirdPersonEquipMontage();

};
