// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EquipComponent.generated.h"

class AEquipableItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UEquipComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EquipItem(const FName& ItemName, TSubclassOf<AEquipableItem> ItemClass, const int8& FromSlotIndex, const uint32& UniqueID);

	void Disarm();

	void DestroyEquipedItem();

	void PlayEquipMontage(bool FirstPersonOnly = false);

	void PlayPrimaryMontage();
	bool PrimaryMontagePlaying() const;

	UFUNCTION(BlueprintCallable)
	AEquipableItem* GetLocallyEquipedItem();

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
	AEquipableItem* LocallyEquipedItem;

	UPROPERTY()
	class AWildOmissionCharacter* OwnerCharacter;

	UFUNCTION()
	void OnRep_EquipedItem();

	void EquipFirstPersonViewModel(TSubclassOf<AEquipableItem> ItemClass);

	UFUNCTION()
	void RefreshEquipedSlotUI();

	bool PrimaryHeld;
	bool SecondaryHeld;

	UFUNCTION(Server, Reliable)
	void Server_PrimaryPressed();
	UFUNCTION(Server, Reliable)
	void Server_PrimaryReleased();

	UFUNCTION(Server, Reliable)
	void Server_SecondaryPressed();
	UFUNCTION(Server, Reliable)
	void Server_SecondaryReleased();
};
