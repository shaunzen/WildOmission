// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "WildOmission/Deployables/Deployable.h"
#include "DeployableItem.generated.h"

class ADeployablePreview;

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
	float DeployableRange;

	bool LineTraceOnChannel(TEnumAsByte<ECollisionChannel> ChannelToTrace, FHitResult& OutHitResult) const;

	FTransform GetPlacementTransform(bool& OutValidSpawn);
	
private:
	UFUNCTION(Client, Reliable)
	void Client_SpawnPreview();
	
	UFUNCTION(Client, Reliable)
	void Client_DestroyPreview();

	UPROPERTY()
	ADeployablePreview* PreviewActor;

	FTransform GetFreehandPlacementTransform();

};
