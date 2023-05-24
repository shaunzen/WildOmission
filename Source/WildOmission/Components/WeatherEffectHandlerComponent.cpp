// Copyright Telephone Studios. All Rights Reserved.


#include "WeatherEffectHandlerComponent.h"
#include "WildOmission/Weather/Storm.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "MetasoundSource.h"

// Sets default values for this component's properties
UWeatherEffectHandlerComponent::UWeatherEffectHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> RainSystemBlueprint(TEXT("/Game/WildOmission/Art/Weather/NS_Rain"));
	if (RainSystemBlueprint.Succeeded())
	{
		RainParticleSystem = RainSystemBlueprint.Object;
	}
	ConstructorHelpers::FObjectFinder<UMetaSoundSource> RainSoundBlueprint(TEXT("/Game/WildOmission/Weather/Sound/MS_RainFall"));
	if (RainSoundBlueprint.Succeeded())
	{
		RainSound = RainSoundBlueprint.Object;
	}
}


// Called when the game starts
void UWeatherEffectHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* HeightFogActor = UGameplayStatics::GetActorOfClass(GetWorld(), AExponentialHeightFog::StaticClass());
	if (HeightFogActor == nullptr)
	{
		return;
	}

	Fog = HeightFogActor->FindComponentByClass<UExponentialHeightFogComponent>();
	if (Fog)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found fog!"));
	}
}


// Called every frame
void UWeatherEffectHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FHitResult HitResult;
	if (LineTraceIntoSkyOnChannel(ECollisionChannel::ECC_GameTraceChannel2, HitResult))
	{
		DisableRainfallEffects();
		return;
	}


	AStorm* HitStorm = nullptr;
	if (!ActorIsStorm(HitResult.GetActor(), HitStorm))
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

void UWeatherEffectHandlerComponent::EnableRainfallEffects(float RainDensity)
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
	bool InCover = LineTraceIntoSkyOnChannel(ECollisionChannel::ECC_Visibility, HitResult) && !ActorIsStorm(HitResult.GetActor(), nullptr);
	if (InCover)
	{
		SpawnedRainAudioComponent->SetFloatParameter(FName("Cutoff"), 200.0f);
	}
	else
	{
		SpawnedRainAudioComponent->SetFloatParameter(FName("Cutoff"), 1000.0f);
	}

	if (PreviouslyHitStorm != nullptr)
	{
		PreviouslyHitStorm->SetLocalPlayerUnderneath(true);
	}

}

void UWeatherEffectHandlerComponent::DisableRainfallEffects()
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
		SpawnedRainAudioComponent->DestroyComponent();
		SpawnedRainAudioComponent = nullptr;
	}
	if (PreviouslyHitStorm)
	{
		PreviouslyHitStorm->SetLocalPlayerUnderneath(false);
		PreviouslyHitStorm = nullptr;
	}
}

bool UWeatherEffectHandlerComponent::ActorIsStorm(AActor* InActor, AStorm* OutStorm)
{
	OutStorm = Cast<AStorm>(InActor);
	if (OutStorm == nullptr)
	{
		return false;
	}

	return true;
}

bool UWeatherEffectHandlerComponent::LineTraceIntoSkyOnChannel(const ECollisionChannel& ChannelToTrace, FHitResult& OutHitResult) const
{
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + FVector::UpVector * 100000.0f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ChannelToTrace, Params);
}