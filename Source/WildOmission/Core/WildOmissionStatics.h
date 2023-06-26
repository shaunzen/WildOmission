//Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WildOmissionStatics.generated.h"

class UNiagaraSystem;

UCLASS()
class WILDOMISSION_API UWildOmissionStatics : public UObject
{
	GENERATED_BODY()
public:

	UWildOmissionStatics();
	static UNiagaraSystem* GetImpactEffectBySurfaceType(const TEnumAsByte<EPhysicalSurface>& Surface);
	static USoundBase* GetImpactSoundBySurfaceType(const TEnumAsByte<EPhysicalSurface>& Surface);
	static UMaterialInterface* GetImpactDecalBySurfaceType(const TEnumAsByte<EPhysicalSurface>& Surface);
	static FVector GetHostLocationInWorld(UWorld* WorldContextObject);

	static float GetSwoopLerp(const float& A, const float& B, const float& TransitionHardness, const float& Alpha, const float& StartAlpha, const float& EndAlpha);
	static void GetWorldSize(UWorld* WorldContextObject, FVector2D& OutWorldSize);
};
