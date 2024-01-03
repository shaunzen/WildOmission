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

FSurfaceProperties* USurfaceHelpers::GetSurfaceProperties(const uint8& SurfaceType)
{
	switch (SurfaceType)
	{
	case 1:	// STONE
		return GetSurfaceData(TEXT("Stone"));
		break;
	case 2:	// GRASS
		return GetSurfaceData(TEXT("Grass"));
		break;
	case 3:	// DIRT
		return GetSurfaceData(TEXT("Gravel"));
		break;
	case 4:	// SAND
		return GetSurfaceData(TEXT("Gravel"));
		break;
	case 5:	// GRAVEL
		return GetSurfaceData(TEXT("Gravel"));
		break;
	case 6:	// SNOW
		return GetSurfaceData(TEXT("Grass"));
		break;
	case 7:	// RESERVED
		return GetSurfaceData(TEXT("Stone"));
		break;
	}

	return GetSurfaceData(TEXT("Stone"));
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
	case SurfaceType3:	// STONE
		return GetSurfaceData(TEXT("Stone"));
		break;
	case SurfaceType4:	// WOOD
		return GetSurfaceData(TEXT("Wood"));
		break;
	case SurfaceType5:	// LEAVES
		return GetSurfaceData(TEXT("Grass"));
		break;
	case SurfaceType6:	// CLOTH
		return GetSurfaceData(TEXT("Wood"));
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

USoundBase* USurfaceHelpers::GetFootstepSound(const uint8& SurfaceType)
{
	FSurfaceProperties* SurfaceProperties = GetSurfaceProperties(SurfaceType);
	if (SurfaceProperties == nullptr)
	{
		return nullptr;
	}

	return SurfaceProperties->FootstepSound;
}

USoundBase* USurfaceHelpers::GetFootstepSound(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	FSurfaceProperties* SurfaceProperties = GetSurfaceProperties(SurfaceType);
	if (SurfaceProperties == nullptr)
	{
		return nullptr;
	}

	return SurfaceProperties->FootstepSound;
}

USoundBase* USurfaceHelpers::GetImpactSound(const uint8& SurfaceType)
{
	FSurfaceProperties* SurfaceProperties = GetSurfaceProperties(SurfaceType);
	if (SurfaceProperties == nullptr)
	{
		return nullptr;
	}

	return SurfaceProperties->ImpactSound;
}

USoundBase* USurfaceHelpers::GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	FSurfaceProperties* SurfaceProperties = GetSurfaceProperties(SurfaceType);
	if (SurfaceProperties == nullptr)
	{
		return nullptr;
	}

	return SurfaceProperties->ImpactSound;
}

UNiagaraSystem* USurfaceHelpers::GetImpactParticles(const uint8& SurfaceType)
{
	FSurfaceProperties* SurfaceProperties = GetSurfaceProperties(SurfaceType);
	if (SurfaceProperties == nullptr)
	{
		return nullptr;
	}

	return SurfaceProperties->ImpactParticles;
}

UNiagaraSystem* USurfaceHelpers::GetImpactParticles(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	FSurfaceProperties* SurfaceProperties = GetSurfaceProperties(SurfaceType);
	if (SurfaceProperties == nullptr)
	{
		return nullptr;
	}

	return SurfaceProperties->ImpactParticles;
}

UMaterialInterface* USurfaceHelpers::GetImpactDecal(const uint8& SurfaceType)
{
	FSurfaceProperties* SurfaceProperties = GetSurfaceProperties(SurfaceType);
	if (SurfaceProperties == nullptr)
	{
		return nullptr;
	}

	return SurfaceProperties->ImpactDecal;
}

UMaterialInterface* USurfaceHelpers::GetImpactDecal(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	FSurfaceProperties* SurfaceProperties = GetSurfaceProperties(SurfaceType);
	if (SurfaceProperties == nullptr)
	{
		return nullptr;
	}

	return SurfaceProperties->ImpactDecal;
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
