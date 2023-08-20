// Copyright Telephone Studios. All Rights Reserved.


#include "Monsters/Monster.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationInvokerComponent.h"
#include "Components/VitalsComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "Components/DistanceDespawnComponent.h"
#include "Components/InventoryComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));

	VitalsComponent = CreateDefaultSubobject<UVitalsComponent>(TEXT("VitalsComponent"));
	VitalsComponent->SetThirstCanDeplete(false);
	VitalsComponent->SetHungerCanDeplete(false);

	InventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));

	EquipComponent = CreateDefaultSubobject<UEquipComponent>(TEXT("EquipComponent"));
	EquipComponent->SetupAttachment(RootComponent);
	EquipComponent->Setup(nullptr, GetMesh());


	DespawnComponent = CreateDefaultSubobject<UDistanceDespawnComponent>(TEXT("DespawnComponent"));
	DespawnComponent->SetupAttachment(RootComponent);

	IdleSound = nullptr;
	TargetPawn = nullptr;
	RagdollClass = nullptr;

	MaxAttackRange = 300.0f;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	SetIdleSoundTimer();

	if (!HasAuthority())
	{
		return;
	}

	FInventoryItem ZombieArmsItem;
	ZombieArmsItem.Name = TEXT("zombie.arms");
	ZombieArmsItem.Quantity = 1;
	InventoryComponent->AddItem(ZombieArmsItem);
	InventoryComponent->SetToolbarSelectionIndex(1);
	VitalsComponent->OnHealthDepleted.AddDynamic(this, &AMonster::HandleDeath);
	DespawnComponent->OnDespawnConditionMet.AddDynamic(this, &AMonster::HandleDespawn);
}

void AMonster::HandleDespawn()
{
	if (OnDespawn.IsBound())
	{
		OnDespawn.Broadcast(this);
	}

	Destroy();
}

void AMonster::HandleDeath()
{
	if (RagdollClass)
	{
		GetWorld()->SpawnActor<AActor>(RagdollClass, GetActorLocation(), GetActorRotation());
	}
	HandleDespawn();
}

void AMonster::SetIdleSoundTimer()
{
	FTimerHandle IdleSoundTimerHandle;
	FTimerDelegate IdleSoundTimerDelegate;
	const float IdleSoundDelay = FMath::RandRange(1.0f, 5.0f);

	IdleSoundTimerDelegate.BindUObject(this, &AMonster::PlayIdleSound);
	GetWorld()->GetTimerManager().SetTimer(IdleSoundTimerHandle, IdleSoundTimerDelegate, IdleSoundDelay, false);
}

void AMonster::PlayIdleSound()
{
	SetIdleSoundTimer();

	if (GetWorld() == nullptr || IdleSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), IdleSound, GetActorLocation());
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetMovementComponent()->GetPhysicsVolume()->bWaterVolume == true)
	{
		AddMovementInput(FVector::UpVector);
	}
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APawn* AMonster::GetTargetPawn() const
{
	return TargetPawn;
}

void AMonster::Attack(AActor* Target)
{
	TargetPawn = Cast<APawn>(Target);
	EquipComponent->PrimaryPressed();
}

void AMonster::StopAttack()
{
	EquipComponent->PrimaryReleased();
	TargetPawn = nullptr;
}