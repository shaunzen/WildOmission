// Copyright Telephone Studios. All Rights Reserved.


#include "Lightning.h"
#include "Components/DirectionalLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightning::ALightning()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	RootComponent = MeshComponent;
	
	LightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(FName("LightComponent"));
	LightComponent->bAtmosphereSunLight = false;
	LightComponent->ForwardShadingPriority = -1;
	LightComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
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
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ThunderSound, GetActorLocation());
}

// Called every frame
void ALightning::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	KillTimer -= DeltaTime;

	if (KillTimer < KINDA_SMALL_NUMBER)
	{
		Destroy();
	}
}

