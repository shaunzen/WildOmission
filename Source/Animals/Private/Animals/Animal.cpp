// Copyright Telephone Studios. All Rights Reserved.


#include "Animals/Animal.h"
#include "NavigationInvokerComponent.h"
#include "Components/VitalsComponent.h"
#include "Components/DistanceDespawnComponent.h"
#include "Components/AnimalSpawnHandlerComponent.h"

// Sets default values
AAnimal::AAnimal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
	VitalsComponent = CreateDefaultSubobject<UVitalsComponent>(TEXT("VitalsComponent"));

	DespawnComponent = CreateDefaultSubobject<UDistanceDespawnComponent>(TEXT("DespawnComponent"));
	DespawnComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority())
	{
		return;
	}

	DespawnComponent->OnDespawnConditionMet.AddDynamic(this, &AAnimal::HandleDespawn);
}

void AAnimal::HandleDespawn()
{
	UAnimalSpawnHandlerComponent::GetSpawnedAnimals()->Remove(this);
	Destroy();
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAnimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

