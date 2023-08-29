// Copyright Telephone Studios. All Rights Reserved.


#include "Monsters/Monster.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/VitalsComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/EquipComponent.h"
#include "NiagaraComponent.h"
#include "TimeOfDayHandler.h"
#include "Components/InventoryComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Log.h"

static ATimeOfDayHandler* TIME_OF_DAY_HANDLER = nullptr;

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

	TargetPawn = nullptr;
	
	MaxAttackRange = 300.0f;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if (TIME_OF_DAY_HANDLER == nullptr)
	{
		TIME_OF_DAY_HANDLER = Cast<ATimeOfDayHandler>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimeOfDayHandler::StaticClass()));
	}

	if (!HasAuthority())
	{
		return;
	}

	FInventoryItem ZombieArmsItem;
	ZombieArmsItem.Name = TEXT("zombie.arms");
	ZombieArmsItem.Quantity = 1;
	InventoryComponent->AddItem(ZombieArmsItem);
	InventoryComponent->SetToolbarSelectionIndex(1);
}

void AMonster::SetBurnDamageTimer()
{
	FTimerDelegate BurnDamageTimerDelegate;
	BurnDamageTimerDelegate.BindUObject(this, &AMonster::ApplyBurnDamage);
	GetWorld()->GetTimerManager().SetTimer(BurnDamageTimerHandle, BurnDamageTimerDelegate, 1.0f, false);
}

void AMonster::ApplyBurnDamage()
{
	VitalsComponent->AddHealth(-10.0f);
	SetBurnDamageTimer();
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TIME_OF_DAY_HANDLER && TIME_OF_DAY_HANDLER->IsDay() && FireEffects->IsActive() == false)
	{
		FireEffects->Activate();

		if (HasAuthority())
		{
			SetBurnDamageTimer();
		}
	}

	if (GetMovementComponent()->GetPhysicsVolume()->bWaterVolume == true)
	{
		if (FireEffects->IsActive())
		{
			FireEffects->Deactivate();
			if (HasAuthority())
			{
				GetWorld()->GetTimerManager().ClearTimer(BurnDamageTimerHandle);
			}
		}
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