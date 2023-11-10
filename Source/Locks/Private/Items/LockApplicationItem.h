// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "LockApplicationItem.generated.h"

/**
 * 
 */
UCLASS()
class ALockApplicationItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	ALockApplicationItem();

	virtual void Tick(float DeltaTime) override;

	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID) override;
	virtual void OnUnequip() override;

	virtual void Destroyed() override;

	virtual void OnPrimaryPressed() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ALock> LockActorClass;

	UPROPERTY(EditDefaultsOnly)
	float PlacementRange;

	bool LineTraceOnChannel(TEnumAsByte<ECollisionChannel> ChannelToTrace, FHitResult& OutHitResult) const;

	bool GetPlacementTransform(FTransform& OutPlacementTransform);
	FTransform GetFreehandPlacementTransform();

	FRotator GetFacePlayerRotation(const FVector& PlacementLocation = FVector::ZeroVector, const FVector& Up = FVector::UpVector) const;


	UFUNCTION(Client, Reliable)
	void Client_SpawnPreview();
	UFUNCTION(Client, Reliable)
	void Client_DestroyPreview();

	UPROPERTY()
	class AStaticMeshActor* PreviewActor;

	UPROPERTY()
	UMaterialInterface* PreviewMaterial;

private:

};
