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
	virtual void Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;
	virtual void OnUnequip() override;

	virtual void Primary() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADeployable> DeployableActor;
private:
	UFUNCTION(Client, Reliable)
	void Client_SpawnPreview();
	UFUNCTION(Client, Reliable)
	void Client_DestroyPreview();

	UPROPERTY()
	AStaticMeshActor* PreviewActor;
};
