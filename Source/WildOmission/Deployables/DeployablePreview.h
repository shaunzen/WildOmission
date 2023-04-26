// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "DeployablePreview.generated.h"

class ADeployable;

UCLASS()
class WILDOMISSION_API ADeployablePreview : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	ADeployablePreview();
	
	void Setup(ADeployable* DeployableToPreview);
	void Update(bool IsSpawnValid);

	bool IsGrounded() const;
	bool IsOverlappingInvalidObject() const;

private:
	UPROPERTY()
	ADeployable* PreviewingDeployable;

	bool OnGround;
	bool InvalidOverlap;

	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

};
