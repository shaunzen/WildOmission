// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/Lightning.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightning::ALightning()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
	LightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
	LightComponent->SetUseTemperature(true);
	LightComponent->SetTemperature(30000.0f);
	LightComponent->AttenuationRadius = 100000.0f;
	LightComponent->Intensity = 999999999.0f;
	LightComponent->SetupAttachment(MeshComponent);

	KillTimer = 0.2f;
}

// Called when the game starts or when spawned
void ALightning::BeginPlay()
{
	Super::BeginPlay();
	
	if (ThunderSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ThunderSound, this->GetActorLocation());
}

// Called every frame
void ALightning::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	KillTimer -= DeltaTime;

	if (KillTimer < KINDA_SMALL_NUMBER)
	{
		this->Destroy();
	}
}

