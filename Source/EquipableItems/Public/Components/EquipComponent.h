// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EquipComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRefreshEquipedSlotUISignature, const uint8&, SlotIndex);

class AEquipableItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EQUIPABLEITEMS_API UEquipComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Setup(USkeletalMeshComponent* FirstPersonMeshComponent, USkeletalMeshComponent* ThirdPersonMeshComponent);

	void EquipItem(const FName& ItemName, TSubclassOf<AEquipableItem> ItemClass, const int8& FromSlotIndex, const uint32& UniqueID);

	void Disarm();

	void DestroyEquipedItem();

	void UpdateControlRotation(const FRotator& InNewControlRotation);
	FRotator GetOwnerControlRotation() const;

	void PlayMontage(UAnimMontage* Montage, bool FirstPerson);
	void PlayItemMontage(UAnimMontage* Montage, bool FirstPerson);

	bool IsMontagePlaying(UAnimMontage* Montage) const;
	bool IsItemMontagePlaying(UAnimMontage* Montage) const;
	
	FRefreshEquipedSlotUISignature RefreshEquipedSlotUI;

	UFUNCTION(BlueprintCallable)
	AEquipableItem* GetEquipedItem();

	UFUNCTION(BlueprintCallable)
	AEquipableItem* GetLocalEquipedItemDefaultClass();

	UFUNCTION(BlueprintCallable)
	bool IsItemEquiped() const;

	bool PrimaryEnabled() const;
	bool SecondaryEnabled() const;

	void PrimaryPressed();
	void PrimaryReleased();

	void SecondaryPressed();
	void SecondaryReleased();

	void ReloadPressed();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonItemComponent;
	
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_EquipedItem)
	AEquipableItem* EquipedItem;

	UPROPERTY()
	AEquipableItem* LocalEquipedItemDefaultClass;

	UPROPERTY()
	APawn* OwnerPawn;
	
	UPROPERTY()
	USkeletalMeshComponent* OwnerFirstPersonMesh;
	
	UPROPERTY()
	USkeletalMeshComponent* OwnerThirdPersonMesh;

	UPROPERTY()
	FRotator OwnerReplicatedControlRotation;

	UFUNCTION()
	void OnRep_EquipedItem();

	void EquipFirstPersonViewModel(TSubclassOf<AEquipableItem> ItemClass, const uint32& UniqueID);

	UFUNCTION()
	void RefreshEquip(const int8& NewSlotIndex, const FInventorySlot& NewSlot);

	bool PrimaryHeld;
	bool SecondaryHeld;

	bool IsEquipedItemValid() const;

	UFUNCTION(Server, Reliable)
	void Server_PrimaryPressed();
	UFUNCTION(Server, Reliable)
	void Server_PrimaryReleased();

	UFUNCTION(Server, Reliable)
	void Server_SecondaryPressed();
	UFUNCTION(Server, Reliable)
	void Server_SecondaryReleased();

	UFUNCTION(Server, Reliable)
	void Server_ReloadPressed();

};
