// Copyright Telephone Studios. All Rights Reserved.


#include "Animals/Animal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationInvokerComponent.h"
#include "Components/VitalsComponent.h"
#include "Components/DistanceDespawnComponent.h"
#include "AnimalSpawnHandler.h"
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

	IdleSound = nullptr;
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	
	SetIdleSoundTimer();

	if (!HasAuthority())
	{
		return;
	}

	VitalsComponent->OnHealthDepleted.AddDynamic(this, &AAnimal::HandleDeath);
	DespawnComponent->OnDespawnConditionMet.AddDynamic(this, &AAnimal::HandleDespawn);
}

void AAnimal::HandleDespawn()
{
	const int32 AnimalIndex = AAnimalSpawnHandler::GetSpawnedAnimals()->IndexOfByKey(this);
	if (AnimalIndex == INDEX_NONE)
	{
		return;
	}

	AAnimalSpawnHandler::GetSpawnedAnimals()->RemoveAtSwap(AnimalIndex, 1, false);
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

void AAnimal::SetIdleSoundTimer()
{
	FTimerHandle IdleSoundTimerHandle;
	FTimerDelegate IdleSoundTimerDelegate;
	const float IdleSoundDelay = FMath::RandRange(3.0f, 10.0f);

	IdleSoundTimerDelegate.BindUObject(this, &AAnimal::PlayIdleSound);
	GetWorld()->GetTimerManager().SetTimer(IdleSoundTimerHandle, IdleSoundTimerDelegate, IdleSoundDelay, false);
}

void AAnimal::PlayIdleSound()
{
	SetIdleSoundTimer();

	if (GetWorld() == nullptr || IdleSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), IdleSound, GetActorLocation());
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