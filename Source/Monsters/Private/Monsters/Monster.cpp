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

	MaxAttackRange = 300.0f;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority())
	{
		return;
	}
	FInventoryItem ZombieArmsItem;
	ZombieArmsItem.Name = TEXT("zombie.arms");
	ZombieArmsItem.Quantity = 1;
	InventoryComponent->AddItem(ZombieArmsItem);
	InventoryComponent->SetToolbarSelectionIndex(0);
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
	for (const FInventoryItem& ItemToDrop : Drops)
	{
		UInventoryComponent::SpawnWorldItem(GetWorld(), ItemToDrop, this);
	}

	HandleDespawn();
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

void AMonster::PlayIdleSound()
{
	if (!HasAuthority())
	{
		return;
	}

	Multi_PlayIdleSound();
}

void AMonster::Attack(AActor* Target)
{
	Multi_PlayAttackEffects();

	TargetPawn = Cast<APawn>(Target);
	
}

void AMonster::OnAttackAnimationClimax()
{
	UE_LOG(LogMonsters, Warning, TEXT("Attack Climax."));

	if (TargetPawn == nullptr || FVector::Distance(this->GetActorLocation(), TargetPawn->GetActorLocation()) > MaxAttackRange)
	{
		return;
	}

	FPointDamageEvent HitByMonsterEvent(10.0f, FHitResult(), GetActorForwardVector(), nullptr);
	TargetPawn->TakeDamage(10.0f, HitByMonsterEvent, this->GetController(), this);
}

void AMonster::Multi_PlayAttackEffects_Implementation()
{
	// TODO Attack Effects
	UE_LOG(LogMonsters, Warning, TEXT("Playing Attack Effects..."));
}

void AMonster::Multi_PlayIdleSound_Implementation()
{
	if (IdleSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), IdleSound, this->GetActorLocation());
}