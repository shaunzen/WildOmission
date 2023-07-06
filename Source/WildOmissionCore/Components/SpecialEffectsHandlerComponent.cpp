// Copyright Telephone Studios. All Rights Reserved.


#include "SpecialEffectsHandlerComponent.h"
#include "Actors//Storm.h"
#include "Components/VitalsComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Camera/CameraComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "MetasoundSource.h"

// Sets default values for this component's properties
USpecialEffectsHandlerComponent::USpecialEffectsHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> RainSystemBlueprint(TEXT("/Game/Weather/Art/NS_Rain"));
	if (RainSystemBlueprint.Succeeded())
	{
		RainParticleSystem = RainSystemBlueprint.Object;
	}
	ConstructorHelpers::FObjectFinder<UMetaSoundSource> RainSoundBlueprint(TEXT("/Game/Weather/Audio/MS_RainFall"));
	if (RainSoundBlueprint.Succeeded())
	{
		RainSound = RainSoundBlueprint.Object;
	}
	RainSoundCutoff = 20000.0f;

	LowHealthEffectThreshold = 60.0f;
}


// Called when the game starts
void USpecialEffectsHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCamera = GetOwner()->FindComponentByClass<UCameraComponent>();

	AActor* HeightFogActor = UGameplayStatics::GetActorOfClass(GetWorld(), AExponentialHeightFog::StaticClass());
	if (HeightFogActor == nullptr)
	{
		return;
	}

	Fog = HeightFogActor->FindComponentByClass<UExponentialHeightFogComponent>();
}


// Called every frame
void USpecialEffectsHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	HandleLowHealthEffects();
	HandleWeatherEffects();
}

void USpecialEffectsHandlerComponent::HandleLowHealthEffects()
{
	if (GetOwner() == nullptr)
	{
		return;
	}

	UVitalsComponent* OwnerVitalsComponent = GetOwner()->FindComponentByClass<UVitalsComponent>();
	if (OwnerVitalsComponent == nullptr)
	{
		return;
	}

	float ContrastAmount = FMath::Lerp(0.7f, 1.0f, FMath::Clamp(OwnerVitalsComponent->GetHealth() / LowHealthEffectThreshold, 0.0f, 1.0f));
	
	OwnerCamera->PostProcessSettings.SceneFringeIntensity = FMath::Lerp(5.0f, 0.0f, FMath::Clamp(OwnerVitalsComponent->GetHealth() / LowHealthEffectThreshold, 0.0f, 1.0f));
	OwnerCamera->PostProcessSettings.ColorContrast = FVector4(ContrastAmount, ContrastAmount, ContrastAmount, ContrastAmount);
	OwnerCamera->PostProcessSettings.BloomDirtMaskIntensity = FMath::Lerp(200.0f, 0.0f, FMath::Clamp(OwnerVitalsComponent->GetHealth() / LowHealthEffectThreshold, 0.0f, 1.0f));
}

void USpecialEffectsHandlerComponent::HandleWeatherEffects()
{
	FHitResult HitResult;
	if (!LineTraceIntoSkyOnChannel(ECollisionChannel::ECC_GameTraceChannel2, HitResult))
	{
		DisableRainfallEffects();
		return;
	}


	AStorm* HitStorm = CastToStorm(HitResult.GetActor());
	if (HitStorm == nullptr)
	{
		DisableRainfallEffects();
		return;
	}

	PreviouslyHitStorm = HitStorm;

	float RainDensity = 0.0f;
	if (!HitStorm->IsRaining(RainDensity))
	{
		DisableRainfallEffects();
		return;
	}

	EnableRainfallEffects(RainDensity);
}

void USpecialEffectsHandlerComponent::EnableRainfallEffects(float RainDensity)
{
	Fog->SetFogDensity(0.05f);
	Fog->SetFogHeightFalloff(0.001f);

	if (SpawnedRainComponent == nullptr)
	{
		SpawnedRainComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(RainParticleSystem, GetOwner()->GetRootComponent(), FName(), FVector(0.0f, 0.0f, 1000.0f), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	}
	SpawnedRainComponent->SetFloatParameter(FName("RainDensity"), RainDensity);

	if (SpawnedRainAudioComponent == nullptr)
	{
		SpawnedRainAudioComponent = UGameplayStatics::SpawnSoundAttached(RainSound, GetOwner()->GetRootComponent());
	}
	SpawnedRainAudioComponent->SetFloatParameter(FName("RainDensity"), RainDensity);

	FHitResult HitResult;
	if (LineTraceIntoSkyOnChannel(ECollisionChannel::ECC_Visibility, HitResult) && CastToStorm(HitResult.GetActor()) == nullptr)
	{
		RainSoundCutoff = FMath::Clamp(FMath::Lerp(RainSoundCutoff, 500.0f, 5.0f * GetWorld()->GetDeltaSeconds()), 500.0f, 20000.0f);
	}
	else
	{
		RainSoundCutoff = FMath::Clamp(FMath::Lerp(RainSoundCutoff, 20000.0f, 5.0f * GetWorld()->GetDeltaSeconds()), 500.0f, 20000.0f);
	}

	SpawnedRainAudioComponent->SetFloatParameter(FName("Cutoff"), RainSoundCutoff);

	if (PreviouslyHitStorm != nullptr)
	{
		PreviouslyHitStorm->SetLocalPlayerUnderneath(true);
	}

}

void USpecialEffectsHandlerComponent::DisableRainfallEffects()
{
	Fog->SetFogDensity(0.02f);
	Fog->SetFogHeightFalloff(0.2f);

	if (SpawnedRainComponent)
	{
		SpawnedRainComponent->DestroyComponent();
		SpawnedRainComponent = nullptr;
	}
	if (SpawnedRainAudioComponent)
	{
		SpawnedRainAudioComponent->Stop();
		SpawnedRainAudioComponent->DestroyComponent();
		SpawnedRainAudioComponent = nullptr;
	}
	if (PreviouslyHitStorm)
	{
		PreviouslyHitStorm->SetLocalPlayerUnderneath(false);
		PreviouslyHitStorm = nullptr;
	}
}

AStorm* USpecialEffectsHandlerComponent::CastToStorm(AActor* InActor)
{
	return Cast<AStorm>(InActor);
}

bool USpecialEffectsHandlerComponent::LineTraceIntoSkyOnChannel(ECollisionChannel ChannelToTrace, FHitResult& OutHitResult) const
{
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + FVector::UpVector * 100000.0f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ChannelToTrace, Params);
}