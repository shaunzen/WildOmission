// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "BuildAnchorComponent.h"
#include "Deployable.generated.h"

UENUM()
enum EDeployableType
{
	GroundOnly		UMETA(DisplayName = "Ground Only"),
	AnchorOnly		UMETA(DisplayName = "Anchor Only"),
	GroundOrAnchor	UMETA(DisplayName = "Ground Or Anchor")
};

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

	TEnumAsByte<EDeployableType> GetPlacementType() const;
	TEnumAsByte<EBuildAnchorType> SnapsToBuildAnchor() const;
	bool FollowsSurfaceNormal() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	TEnumAsByte<EDeployableType> PlacementType;

	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	TEnumAsByte<EBuildAnchorType> BuildAnchorToSnapTo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool UseSurfaceNormal;

};
