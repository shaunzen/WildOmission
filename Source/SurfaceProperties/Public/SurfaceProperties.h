// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SurfaceProperties.generated.h"

USTRUCT(BlueprintType)
struct SURFACEPROPERTIES_API FSurfaceProperties : public FTableRowBase
{
	GENERATED_BODY()
	
	FSurfaceProperties();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* FootstepSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* ImpactSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UNiagaraSystem* ImpactParticles;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface* ImpactDecal;

};
