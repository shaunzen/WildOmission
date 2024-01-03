// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SurfaceProperties.h"
#include "SurfaceHelpers.generated.h"

/**
 * 
 */
UCLASS()
class SURFACEPROPERTIES_API USurfaceHelpers : public UObject
{
	GENERATED_BODY()

public:
	USurfaceHelpers();
	static FSurfaceProperties* GetSurfaceProperties(const uint8& SurfaceType);
	static FSurfaceProperties* GetSurfaceProperties(TEnumAsByte<EPhysicalSurface> SurfaceType);
	static USoundBase* GetFootstepSound(const uint8& SurfaceType);
	static USoundBase* GetFootstepSound(TEnumAsByte<EPhysicalSurface> SurfaceType);
	static USoundBase* GetImpactSound(const uint8& SurfaceType);
	static USoundBase* GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType);
	static class UNiagaraSystem* GetImpactParticles(const uint8& SurfaceType);
	static class UNiagaraSystem* GetImpactParticles(TEnumAsByte<EPhysicalSurface> SurfaceType);
	static UMaterialInterface* GetImpactDecal(const uint8& SurfaceType);
	static UMaterialInterface* GetImpactDecal(TEnumAsByte<EPhysicalSurface> SurfaceType);

private:
	static FSurfaceProperties* GetSurfaceData(const FName& Name);

};
