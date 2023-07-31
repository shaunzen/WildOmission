// Copyright Telephone Studios. All Rights Reserved.


#include "Animals/Animal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationInvokerComponent.h"
#include "Components/VitalsComponent.h"
#include "Components/DistanceDespawnComponent.h"
#include "Components/AnimalSpawnHandlerComponent.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAnimal::AAnimal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));

	VitalsComponent = CreateDefaultSubobject<UVitalsComponent>(TEXT("VitalsComponent"));
	VitalsComponent->SetThirstCanDeplete(false);
	VitalsComponent->SetHungerCanDeplete(false);

	DespawnComponent = CreateDefaultSubobject<UDistanceDespawnComponent>(TEXT("DespawnComponent"));
	DespawnComponent->SetupAttachment(RootComponent);

	CallSound = nullptr;
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority())
	{
		return;
	}

	VitalsComponent->OnHealthDepleted.AddDynamic(this, &AAnimal::HandleDeath);
	DespawnComponent->OnDespawnConditionMet.AddDynamic(this, &AAnimal::HandleDespawn);
}

void AAnimal::HandleDespawn()
{
	UAnimalSpawnHandlerComponent::GetSpawnedAnimals()->Remove(this);
	Destroy();
}

void AAnimal::HandleDeath()
{
	for (const FInventoryItem& ItemToDrop : Drops)
	{
		UInventoryComponent::SpawnWorldItem(GetWorld(), ItemToDrop, this);
	}

	HandleDespawn();
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetMovementComponent()->GetPhysicsVolume()->bWaterVolume == true)
	{
		AddMovementInput(FVector::UpVector);
	}
}

// Called to bind functionality to input
void AAnimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAnimal::PlayCallSound()
{
	if (!HasAuthority())
	{
		return;
	}

	Multi_PlayCallSound();
}

void AAnimal::Multi_PlayCallSound_Implementation()
{
	if (CallSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CallSound, this->GetActorLocation());
}