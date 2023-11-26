// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "DeployableItemBase.generated.h"

class ADeployable;
class ADeployablePreview;

UCLASS()
class DEPLOYABLES_API ADeployableItemBase : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	ADeployableItemBase();

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

	// Handle placement logic here, calls to Super should be placed at the end, 
	// as Super will remove the item from inventory, rendering all further logic invalid.
	virtual void OnPlace();

	bool LineTraceOnChannel(TEnumAsByte<ECollisionChannel> ChannelToTrace, FHitResult& OutHitResult) const;

	bool HasBuildingPrivilege(const FVector& LocationToTest) const;

	// By default OutPlacementTransform will be freehand, returns true if valid placement, false if invalid.
	virtual bool GetPlacementTransform(FTransform& OutPlacementTransform);

	FTransform GetFreehandPlacementTransform();

	FRotator GetFacePlayerRotation(const FVector& PlacementLocation = FVector::ZeroVector, const FVector& Up = FVector::UpVector) const;

	virtual UStaticMesh* GetPreviewMesh();

	UFUNCTION(Client, Reliable)
	void Client_SpawnPreview();
	UFUNCTION(Client, Reliable)
	void Client_DestroyPreview();

	UPROPERTY()
	ADeployablePreview* PreviewActor;

};
