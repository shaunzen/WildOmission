// Copyright Telephone Studios. All Rights Reserved.


#include "WeatherEffectHandlerComponent.h"
#include "WildOmission/Weather/Storm.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

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
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + FVector::UpVector * 100000.0f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel2, Params))
	{
		DisableRainfallEffects();
		return;
	}

	AStorm* HitStorm = Cast<AStorm>(HitResult.GetActor());
	if (HitStorm == nullptr)
	{
		DisableRainfallEffects();
		return;
	}

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
	if (SpawnedRainComponent == nullptr)
	{
		SpawnedRainComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(RainParticleSystem, GetOwner()->GetRootComponent(), FName(), FVector(0.0f, 0.0f, 1000.0f), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	}

	SpawnedRainComponent->SetFloatParameter(FName("RainDensity"), RainDensity);
	Fog->FogHeightFalloff = 0.001f;
}

void UWeatherEffectHandlerComponent::DisableRainfallEffects()
{
	if (SpawnedRainComponent == nullptr)
	{
		return;
	}

	SpawnedRainComponent->DestroyComponent();
	SpawnedRainComponent = nullptr;

	Fog->FogHeightFalloff = 0.2f;
}