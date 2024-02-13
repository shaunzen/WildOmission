// Copyright Telephone Studios. All Rights Reserved.


#include "Items/PookaCannon.h"

void APookaCannon::OnPrimaryHeld()
{
	Super::OnPrimaryHeld();
	
	if (!CanFire)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Pooka cannon fire!"));
	Fire();
}
