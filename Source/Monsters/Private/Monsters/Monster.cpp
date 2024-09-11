// Copyright Telephone Studios. All Rights Reserved.


#include "Monsters/Monster.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/VitalsComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "NiagaraComponent.h"
#include "TimeOfDayManager.h"
#include "Components/InventoryComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Log.h"

float FireDamageCounter = 0.0f;

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));

	EquipComponent = CreateDefaultSubobject<UEquipComponent>(TEXT("EquipComponent"));
	EquipComponent->SetupAttachment(RootComponent);
	EquipComponent->Setup(nullptr, GetMesh());

	FireEffects = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireEffects"));
	FireEffects->SetupAttachment(RootComponent);
	FireEffects->SetAutoActivate(false);

	DefaultWalkSpeed = 300.0f;
	DefaultRunSpeed = 600.0f;

	MaxAttackRange = 300.0f;

	bCanBurnInLight = true;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority() || ItemToEquip.IsZero())
	{
		return;
	}

	InventoryComponent->AddItem(ItemToEquip);
	InventoryComponent->SetToolbarSelectionIndex(3);
}

void AMonster::SetFire()
{
	if (FireEffects == nullptr || FireEffects->IsActive())
	{
		return;
	}

	FireEffects->Activate();
	bIsBurning = true;
}

void AMonster::PutOutFire()
{
	if (FireEffects == nullptr || !FireEffects->IsActive())
	{
		return;
	}

	FireEffects->Deactivate();
	bIsBurning = false;
}

void AMonster::ApplyBurnDamage()
{
	if (VitalsComponent == nullptr)
	{
		return;
	}

	VitalsComponent->AddHealth(-10.0f);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UWorld* World = GetWorld();
	if (World == nullptr || FireEffects == nullptr)
	{
		return;
	}

	ATimeOfDayManager* TimeOfDayManager = ATimeOfDayManager::GetTimeOfDayManager();
	if (TimeOfDayManager && TimeOfDayManager->IsDay())
	{
		SetFire();
	}

	UMovementComponent* MovementComponent = GetMovementComponent();
	if (MovementComponent == nullptr)
	{
		return;
	}

	APhysicsVolume* CurrentPhysicsVolume = MovementComponent->GetPhysicsVolume();
	if (CurrentPhysicsVolume == nullptr)
	{
		return;
	}

	if (CurrentPhysicsVolume->bWaterVolume == true)
	{
		PutOutFire();
		AddMovementInput(FVector::UpVector);
	}

	FHitResult HitResult;
	FVector Start = this->GetActorLocation();
	FVector End = Start + FVector(0.0f, 0.0f, 50000.0f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
	{
		PutOutFire();
	}

	if (!HasAuthority() || bIsBurning == false)
	{
		return;
	}

	FireDamageCounter += DeltaTime;
	if (FireDamageCounter < 1.0f)
	{
		return;
	}

	ApplyBurnDamage();
	FireDamageCounter = 0.0f;
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::Destroyed()
{
	Super::Destroyed();

	if (EquipComponent && EquipComponent->GetEquipedItem())
	{
		EquipComponent->Disarm();
	}
}

void AMonster::Attack()
{
	if (EquipComponent == nullptr)
	{
		return;
	}

	EquipComponent->PrimaryPressed();
}

void AMonster::StopAttack()
{
	if (EquipComponent == nullptr)
	{
		return;
	}

	EquipComponent->PrimaryReleased();
}

bool AMonster::IsOnFire() const
{
	if (FireEffects == nullptr)
	{
		return false;
	}

	return FireEffects->IsActive();
}
