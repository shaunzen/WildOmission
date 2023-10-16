// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Bed.h"
#include "Net/UnrealNetwork.h"

ABed::ABed()
{
	UniqueID = -1;
}

void ABed::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABed, UniqueID);
}

int32 ABed::GetUniqueID() const
{
	return UniqueID;
}
