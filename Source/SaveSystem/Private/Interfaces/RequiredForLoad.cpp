// Copyright Telephone Studios. All Rights Reserved.


#include "RequiredForLoad.h"
#include "Kismet/GameplayStatics.h"

// Add default functionality here for any IRequiredForLoad functions that are not pure virtual.

int32 IRequiredForLoad::GetNumRequiredActorsInWorld(UWorld* WorldContextObject)
{
	TArray<AActor*> RequiredActors;
	UGameplayStatics::GetAllActorsWithInterface(WorldContextObject, URequiredForLoad::StaticClass(), RequiredActors);
	return RequiredActors.Num();
}
