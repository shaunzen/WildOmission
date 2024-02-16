// Copyright Telephone Studios. All Rights Reserved.


#include "Items/PookaCannon.h"

void APookaCannon::OnPrimaryHeld()
{
	Super::OnPrimaryHeld();
	
	if (!CanFire)
	{
		return;
	}

	Multi_Fire();
}

void APookaCannon::Multi_Fire_Implementation()
{
	Fire();
}
