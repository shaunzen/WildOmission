// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "BuildAnchorComponent.h"
#include "Deployable.generated.h"

UCLASS()
class WILDOMISSION_API ADeployable : public AActor, public ISavableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeployable();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMesh* GetMesh() const;

	bool CanSpawnOnGround() const;
	bool CanSpawnOnFloor() const;
	bool CanSpawnOnWall() const;
	TEnumAsByte<EBuildAnchorType> CanSpawnOnBuildAnchor() const;
	bool FollowsSurfaceNormal() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool bCanSpawnOnGround;
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool bCanSpawnOnFloor;
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool bCanSpawnOnWall;
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	TEnumAsByte<EBuildAnchorType> CanSpawnOnAnchor;
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool bFollowsSurfaceNormal;

};
