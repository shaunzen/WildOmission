// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EquipComponent.generated.h"

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

	void EquipItem(const FName& ItemName, const int8& FromSlotIndex, const uint32& UniqueID);

	void Disarm();

	void DestroyEquipedItem();

	void PlayEquipMontage(bool FirstPerson);

	void PlayPrimaryMontage(bool FirstPerson);
	bool PrimaryMontagePlaying() const;

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

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FirstPersonItemMeshComponent;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_EquipedItem)
	AEquipableItem* EquipedItem;

	UPROPERTY()
	AEquipableItem* LocalEquipedItemDefaultClass;

	UPROPERTY()
	APawn* OwnerPawn;

	UFUNCTION()
	void OnRep_EquipedItem();

	void EquipFirstPersonViewModel(TSubclassOf<AEquipableItem> ItemClass, const uint32& UniqueID);

	UFUNCTION()
	void RefreshEquipedSlotUI();

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
};
