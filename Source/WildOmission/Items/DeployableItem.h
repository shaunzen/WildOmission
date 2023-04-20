// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "DeployableItem.generated.h"

class ADeployable;
class AStaticMeshActor;

UCLASS()
class WILDOMISSION_API ADeployableItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	ADeployableItem();
	virtual void Tick(float DeltaTime) override;

	virtual void Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;
	virtual void OnUnequip() override;

	virtual void Primary() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADeployable> DeployableActorClass;

	UPROPERTY(EditDefaultsOnly)
	float DeployableRange = 500.0f;

	bool FindPlacableSurface(FHitResult& OutHitResult) const;

private:
	UFUNCTION(Client, Reliable)
	void Client_SpawnPreview();
	
	UFUNCTION(Client, Reliable)
	void Client_DestroyPreview();

	UPROPERTY()
	AStaticMeshActor* PreviewActor;
	
	UPROPERTY()
	UMaterialInstance* PreviewMaterial;
};
