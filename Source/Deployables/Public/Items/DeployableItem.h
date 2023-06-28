// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "Deployables/Deployable.h"
#include "DeployableItem.generated.h"

class ADeployablePreview;

UCLASS()
class DEPLOYABLES_API ADeployableItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	ADeployableItem();
	virtual void Tick(float DeltaTime) override;

	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;
	virtual void OnUnequip() override;

	virtual void Destroyed() override;

	virtual void OnPrimaryPressed() override;

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

	FRotator GetFacePlayerRotation(const FVector& Up = FVector::UpVector) const;
	
	float GetOffsetFromNearestSnapDegree(const float& InAxis) const;

};
