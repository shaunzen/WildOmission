// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Deployable.generated.h"

UENUM()
enum EDeployableType
{
	GroundOnly			UMETA(DisplayName = "Ground Only"),
	FloorOnly			UMETA(DisplayName = "Floor Only"),
	GroundOrFloor		UMETA(DisplayName = "Ground or Floor"),
	WallOnly			UMETA(DisplayName = "Wall Only"),
	DoorwayOnly			UMETA(DisplayName = "Doorway Only"),
	AnyExceptInvalid	UMETA(DisplayName = "Any Except Invalid"),
	AnySurface			UMETA(DisplayName = "Any Surface")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDeployable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WILDOMISSION_API IDeployable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual UStaticMesh* GetMesh() const = 0;

	TEnumAsByte<EDeployableType> GetPlacementType() const;
	bool SnapsToBuildAnchor() const;
	bool FollowsSurfaceNormal() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	TEnumAsByte<EDeployableType> PlacementType = EDeployableType::GroundOrFloor;

	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool SnapToBuildAnchor = false;

	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool UseSurfaceNormal = true;

};
