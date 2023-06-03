// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/ItemData.h"
#include "WildOmission/UI/Custom/UIColor.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WildOmissionStatics.generated.h"

class UNiagaraSystem;

UCLASS()
class WILDOMISSION_API UWildOmissionStatics : public UObject
{
	GENERATED_BODY()
public:

	UWildOmissionStatics();
	static FItemData* GetItemData(const FName& ItemName);
	static FUIColor* GetUIColor(const FName& ColorName);
	static FLinearColor GetHighlightedColor(FUIColor* Color);
	static FLinearColor GetSelectedColor(FUIColor* Color);
	static UNiagaraSystem* GetImpactEffectBySurfaceType(const TEnumAsByte<EPhysicalSurface>& Surface);
	static USoundBase* GetImpactSoundBySurfaceType(const TEnumAsByte<EPhysicalSurface>& Surface);
	static UMaterialInterface* GetImpactDecalBySurfaceType(const TEnumAsByte<EPhysicalSurface>& Surface);
	static FVector GetHostLocationInWorld(UWorld* WorldContextObject);
	static void SpawnWorldItem(UWorld* WorldContextObject, const struct FInventoryItem& ItemToSpawn, AActor* SpawningActor);
	static void GetWorldSize(UWorld* WorldContextObject, FVector2D& OutWorldSize);
};
