// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "WildOmission/Deployables/Deployable.h"
#include "DeployableItem.generated.h"

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
	float DeployableRange;

	bool LineTraceOnCameraChannel(FHitResult& OutHitResult) const;
	FTransform GetPlacementTransform(UBuildAnchorComponent* AnchorToAttachTo);

	bool OnGround;
	bool OnFloor;
	bool OnWall;
	bool InAnchor;
	bool InvalidOverlap;
	bool WithinRange;

	bool SpawnConditionValid() const;

	bool GroundOnlySpawnConditionValid() const;
	bool FloorOnlySpawnConditionValid() const;
	bool GroundOrFloorSpawnConditionValid() const;
	bool WallOnlySpawnConditionValid() const;
	bool AnchorSpawnConditionValid() const;
	bool AnchorOrGroundSpawnConditionValid() const;
	bool AnySurfaceSpawnConditionValid() const;

private:
	UFUNCTION(Client, Reliable)
	void Client_SpawnPreview();
	
	UFUNCTION(Client, Reliable)
	void Client_DestroyPreview();

	void UpdatePreview();

	UFUNCTION()
	void OnPreviewBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void OnPreviewEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY()
	AStaticMeshActor* PreviewActor;

	FTransform GetNonSnappingPlacementTransform();

};
