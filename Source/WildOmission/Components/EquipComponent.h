// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EquipComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UEquipComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EquipItem(const FName& ItemName, TSubclassOf<AEquipableItem> Item, const int8& FromSlotIndex, const uint32& UniqueID);

	void Disarm();

	void DestroyEquipedItem();

	void PlayPrimaryMontage();

	UFUNCTION(BlueprintCallable)
	class AEquipableItem* GetEquipedItem();

	UFUNCTION(BlueprintCallable)
	bool IsItemEquiped() const;

	bool PrimaryEnabled() const;
	bool SecondaryEnabled() const;

	UFUNCTION(Server, Reliable)
	void Server_PrimaryPressed();
	UFUNCTION(Server, Reliable)
	void Server_PrimaryReleased();
	
	UFUNCTION(Server, Reliable)
	void Server_SecondaryPressed();
	UFUNCTION(Server, Reliable)
	void Server_SecondaryReleased();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FirstPersonItemMeshComponent;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_EquipedItem)
	class AEquipableItem* EquipedItem;

	UPROPERTY()
	class AWildOmissionCharacter* OwnerCharacter;

	UFUNCTION()
	void OnRep_EquipedItem();

	UFUNCTION()
	void RefreshEquipedSlot();

	bool PrimaryHeld;
	bool SecondaryHeld;

};
