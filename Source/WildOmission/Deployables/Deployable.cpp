// Copyright Telephone Studios. All Rights Reserved.


#include "Deployable.h"

// Add default functionality here for any IDeployable functions that are not pure virtual.
TEnumAsByte<EDeployableType> IDeployable::GetPlacementType() const
{
	return PlacementType;
}

bool IDeployable::SnapsToBuildAnchor() const
{
	return SnapToBuildAnchor;
}

bool IDeployable::FollowsSurfaceNormal() const
{
	return UseSurfaceNormal;
}