// Copyright Telephone Studios. All Rights Reserved.


#include "SurfaceHelpers.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

static UDataTable* SurfacePropertiesDataTable = nullptr;

USurfaceHelpers::USurfaceHelpers()
{
	ConstructorHelpers::FObjectFinder<UDataTable> SurfacePropertiesDataTableObject(TEXT("/Game/WildOmissionCore/PhysicalMaterials/DT_SurfaceProperties"));
	if (SurfacePropertiesDataTableObject.Succeeded())
	{
		SurfacePropertiesDataTable = SurfacePropertiesDataTableObject.Object;
	}
}

FSurfaceProperties* USurfaceHelpers::GetSurfaceProperties(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	switch (SurfaceType)
	{
	case SurfaceType1:	// GRASS
		return GetSurfaceData(TEXT("Grass"));
		break;
	case SurfaceType2:	// GRAVEL
		return GetSurfaceData(TEXT("Gravel"));
		break;
	case SurfaceType4:	// WOOD
		return GetSurfaceData(TEXT("Wood"));
		break;
	case SurfaceType3:	// STONE
		return GetSurfaceData(TEXT("Stone"));
		break;
	case SurfaceType7:	// METAL
		return GetSurfaceData(TEXT("Metal"));
		break;
	case SurfaceType8:	// FLESH
		return GetSurfaceData(TEXT("Flesh"));
		break;
	}

	return GetSurfaceData(TEXT("Stone"));
}

USoundBase* USurfaceHelpers::GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	return GetSurfaceProperties(SurfaceType)->ImpactSound;
}

UNiagaraSystem* USurfaceHelpers::GetImpactParticles(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	return GetSurfaceProperties(SurfaceType)->ImpactParticles;
}

UMaterialInterface* USurfaceHelpers::GetImpactDecal(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	return GetSurfaceProperties(SurfaceType)->ImpactDecal;
}

FSurfaceProperties* USurfaceHelpers::GetSurfaceData(const FName& Name)
{
	if (SurfacePropertiesDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Surface Properties Context"));

	return SurfacePropertiesDataTable->FindRow<FSurfaceProperties>(Name, ContextString, true);
}
