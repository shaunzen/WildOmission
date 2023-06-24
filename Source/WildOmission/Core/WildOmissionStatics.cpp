// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionStatics.h"
#include "WildOmission/Core/GameModes/WildOmissionGameMode.h"
#include "WildOmission/Core/SaveSystem/SaveHandler.h"
#include "WildOmission/Core/SaveSystem/WorldGenerationHandlerComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NiagaraSystem.h"
#include "Engine/DataTable.h"

static UDataTable* UIColorsTable = nullptr;
static USoundBase* WoodImpactSound = nullptr;
static USoundBase* RockImpactSound = nullptr;
static UNiagaraSystem* WoodImpactEffect = nullptr;
static UNiagaraSystem* RockImpactEffect = nullptr;
static UNiagaraSystem* MetalImpactEffect = nullptr;
static UMaterialInterface* WoodImpactDecalMaterial = nullptr;
static UMaterialInterface* RockImpactDecalMaterial = nullptr;
static UMaterialInterface* MetalImpactDecalMaterial = nullptr;

UWildOmissionStatics::UWildOmissionStatics()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> UIColorsTableBlueprint(TEXT("/Game/WildOmission/UI/Custom/DT_UIColors"));
	if (UIColorsTableBlueprint.Succeeded())
	{
		UIColorsTable = UIColorsTableBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> WoodImpactSoundObject(TEXT("/Game/WildOmission/Items/EquipableItems/Audio/Tools/WoodImpact_Cue"));
	if (WoodImpactSoundObject.Succeeded())
	{
		WoodImpactSound = WoodImpactSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> RockImpactSoundObject(TEXT("/Game/WildOmission/Items/EquipableItems/Audio/Tools/RockImpact_Cue"));
	if (RockImpactSoundObject.Succeeded())
	{
		RockImpactSound = RockImpactSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> WoodImpactEffectBlueprint(TEXT("/Game/WildOmission/Art/Effects/NS_Impact_Wood"));
	if (WoodImpactEffectBlueprint.Succeeded())
	{
		WoodImpactEffect = WoodImpactEffectBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> RockImpactEffectBlueprint(TEXT("/Game/WildOmission/Art/Effects/NS_Impact_Rock"));
	if (RockImpactEffectBlueprint.Succeeded())
	{
		RockImpactEffect = RockImpactEffectBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> MetalImpactEffectBlueprint(TEXT("/Game/WildOmission/Art/Effects/NS_Impact_Metal"));
	if (MetalImpactEffectBlueprint.Succeeded())
	{
		MetalImpactEffect = MetalImpactEffectBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> WoodImpactDecalMaterialObject(TEXT("/Game/WildOmission/Art/Effects/M_Impact_Wood_Inst"));
	if (WoodImpactDecalMaterialObject.Succeeded())
	{
		WoodImpactDecalMaterial = WoodImpactDecalMaterialObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> RockImpactDecalMaterialObject(TEXT("/Game/WildOmission/Art/Effects/M_Impact_Rock_Inst"));
	if (RockImpactDecalMaterialObject.Succeeded())
	{
		RockImpactDecalMaterial = RockImpactDecalMaterialObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MetalImpactDecalMaterialObject(TEXT("/Game/WildOmission/Art/Effects/M_Impact_Metal_Inst"));
	if (MetalImpactDecalMaterialObject.Succeeded())
	{
		MetalImpactDecalMaterial = MetalImpactDecalMaterialObject.Object;
	}
}

FUIColor* UWildOmissionStatics::GetUIColor(const FName& ColorName)
{
	if (UIColorsTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("UI Color Context"));

	return UIColorsTable->FindRow<FUIColor>(ColorName, ContextString, true);
}

FLinearColor UWildOmissionStatics::GetHighlightedColor(FUIColor* Color)
{
	return FLinearColor(Color->Default.R + Color->HighlightedOffset, Color->Default.G + Color->HighlightedOffset, Color->Default.B + Color->HighlightedOffset, 1.0f);
}

FLinearColor UWildOmissionStatics::GetSelectedColor(FUIColor* Color)
{
	return FLinearColor(Color->Default.R + Color->SelectedOffset, Color->Default.G + Color->SelectedOffset, Color->Default.B + Color->SelectedOffset, 1.0f);
}

UNiagaraSystem* UWildOmissionStatics::GetImpactEffectBySurfaceType(const TEnumAsByte<EPhysicalSurface>& Surface)
{
	switch (Surface)
	{
	case SurfaceType4:	// WOOD
		return WoodImpactEffect;
		break;
	case SurfaceType3:	// ROCK
		return RockImpactEffect;
		break;
	case SurfaceType7:	// METAL
		return MetalImpactEffect;
		break;
	}

	return RockImpactEffect;
}

USoundBase* UWildOmissionStatics::GetImpactSoundBySurfaceType(const TEnumAsByte<EPhysicalSurface>& Surface)
{
	switch (Surface)
	{
	case SurfaceType4:	// WOOD
		return WoodImpactSound;
		break;
	case SurfaceType3:	// ROCK
		return RockImpactSound;
		break;
	case SurfaceType7:	// METAL
		//return MetalImpactSound;
		break;
	}

	return WoodImpactSound;
}

UMaterialInterface* UWildOmissionStatics::GetImpactDecalBySurfaceType(const TEnumAsByte<EPhysicalSurface>& Surface)
{
	switch (Surface)
	{
	case SurfaceType4:	// WOOD
		return WoodImpactDecalMaterial;
		break;
	case SurfaceType3:	// ROCK
		return RockImpactDecalMaterial;
		break;
	case SurfaceType7:	// METAL
		return MetalImpactDecalMaterial;
		break;
	}

	return RockImpactDecalMaterial;
}

FVector UWildOmissionStatics::GetHostLocationInWorld(UWorld* WorldContextObject)
{
	APlayerController* HostPlayerController = WorldContextObject->GetFirstPlayerController();
	if (HostPlayerController == nullptr)
	{
		return FVector::ZeroVector;
	}

	APawn* HostPawn = HostPlayerController->GetPawn();
	if (HostPawn == nullptr)
	{
		return FVector::ZeroVector;
	}

	return HostPawn->GetActorLocation();
}

float UWildOmissionStatics::GetSwoopLerp(const float& A, const float& B, const float& TransitionHardness, const float& Alpha, const float& StartAlpha, const float& EndAlpha)
{
	float ZeroAlpha = Alpha - StartAlpha;
	float Duration = EndAlpha - StartAlpha;

	return FMath::Lerp(A, B, FMath::Clamp(FMath::Sin((ZeroAlpha / Duration) * 6.0f) * TransitionHardness, 0.0f, 1.0f));
}

void UWildOmissionStatics::GetWorldSize(UWorld* WorldContextObject, FVector2D& OutWorldSize)
{
	if (WorldContextObject == nullptr)
	{
		return;
	}
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(WorldContextObject->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}
	ASaveHandler* SaveHandler = GameMode->GetSaveHandler();
	if (SaveHandler == nullptr)
	{
		return;
	}

	OutWorldSize = SaveHandler->GetWorldGenerationHandler()->GetWorldSizeMeters();
}