// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EquipComponent.generated.h"

class AEquipableItem;
class AWildOmissionCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UEquipComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipItem(TSubclassOf<AEquipableItem> Item);
	void Disarm();

	void PlaySwingAnimation();

	UFUNCTION(BlueprintCallable)
	AEquipableItem* GetEquipedItem();

	UFUNCTION(BlueprintCallable)
	bool IsItemEquiped() const;

	UFUNCTION(Server, Reliable)
	void Server_Primary();

	UFUNCTION(Server, Reliable)
	void Server_Secondary();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FirstPersonItemMeshComponent;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_EquipedItem)
	AEquipableItem* EquipedItem;

	UPROPERTY()
	AWildOmissionCharacter* OwnerCharacter;

	UFUNCTION()
	void OnRep_EquipedItem();

};
