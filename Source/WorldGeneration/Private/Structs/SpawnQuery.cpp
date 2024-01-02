// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/SpawnQuery.h"

FSpawnQuery::FSpawnQuery()
{
	SpawnData = FSpawnData();
	TestValue = 0.0f;
}

FSpawnQuery::FSpawnQuery(const FSpawnData& InSpawnData, float InTestValue)
{
	SpawnData = InSpawnData;
	TestValue = InTestValue;
}
