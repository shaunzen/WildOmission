// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "DeployablePreview.generated.h"

class ADeployable;

UCLASS()
class DEPLOYABLES_API ADeployablePreview : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	ADeployablePreview();
	
	void Setup(ADeployable* DeployableToPreview);
	void Setup(UStaticMesh* PreviewMesh);

	void Update(bool IsSpawnValid);

	bool IsOverlappingInvalidObject() const;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CollisionCheckMeshComponent;

	UPROPERTY()
	ADeployable* PreviewingDeployable;


	bool InvalidOverlap;

	int32 OverlapCount;

	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

};
