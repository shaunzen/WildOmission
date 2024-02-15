// Copyright Telephone Studios. All Rights Reserved.


#include "Characters/WildOmissionAICharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationInvokerComponent.h"
#include "Components/VitalsComponent.h"
#include "Components/DistanceDespawnComponent.h"
#include "Ragdolls/Ragdoll.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"
#include "Log.h"

// Sets default values
AWildOmissionAICharacter::AWildOmissionAICharacter()
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

	DefaultWalkSpeed = 300.0f;
	DefaultRunSpeed = 600.0f;

	IdleSoundsEnabled = true;

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		this->bUseControllerRotationYaw = false;
		CharacterMovementComponent->bOrientRotationToMovement = true;
	}

	MinTimeBetweenIdleSoundSeconds = 1.0f;
	MaxTimeBetweenIdleSoundSeconds = 5.0f;

	IdleSound = nullptr;
	RagdollClass = nullptr;
}

void AWildOmissionAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWildOmissionAICharacter, DesiredMovementSpeed);
}

// Called when the game starts or when spawned
void AWildOmissionAICharacter::BeginPlay()
{
	Super::BeginPlay();

	StopRunning();
	SetIdleSoundTimer();

	if (!HasAuthority())
	{
		return;
	}

	VitalsComponent->OnHealthDepleted.AddDynamic(this, &AWildOmissionAICharacter::HandleDeath);
	DespawnComponent->OnDespawnConditionMet.AddDynamic(this, &AWildOmissionAICharacter::HandleDespawn);
}

void AWildOmissionAICharacter::HandleDespawn()
{
	if (OnDespawn.IsBound())
	{
		OnDespawn.Broadcast(this);
	}

	Destroy();
}

void AWildOmissionAICharacter::HandleDeath()
{
	if (RagdollClass)
	{
		TArray<AActor*> AttachedActors;
		GetAttachedActors(AttachedActors);
		ARagdoll* SpawnedRagdoll = GetWorld()->SpawnActor<ARagdoll>(RagdollClass, GetActorLocation(), GetActorRotation());
		if (SpawnedRagdoll == nullptr)
		{
			return;
		}
		
		for (AActor* AttachedActor : AttachedActors)
		{
			if (AttachedActor == nullptr)
			{
				continue;
			}

			const FName Socket = AttachedActor->GetAttachParentSocketName();
			AttachedActor->AttachToComponent(SpawnedRagdoll->GetMeshComponent(), FAttachmentTransformRules::KeepRelativeTransform, Socket);
		}
	}

	HandleDespawn();
}

void AWildOmissionAICharacter::Landed(const FHitResult& HitResult)
{
	Super::Landed(HitResult);

	if (!HasAuthority())
	{
		return;
	}

	UCharacterMovementComponent* OurMovementComponent = GetCharacterMovement();
	if (OurMovementComponent == nullptr || VitalsComponent == nullptr)
	{
		return;
	}

	const float DamageThreshold = 1000.0f;
	const float FallVelocity = OurMovementComponent->GetLastUpdateVelocity().GetAbs().Z;
	if (FallVelocity <= DamageThreshold)
	{
		return;
	}

	const float DamageToApply = FMath::Lerp(0.0f, VitalsComponent->GetMaxHealth(), (FallVelocity - DamageThreshold) / DamageThreshold);
	FPointDamageEvent FallDamageEvent(DamageToApply, HitResult, -FVector::UpVector, nullptr);
	this->TakeDamage(DamageToApply, FallDamageEvent, GetController(), this);
	// TODO crunch sound
}

void AWildOmissionAICharacter::StartRunning()
{
	DesiredMovementSpeed = DefaultRunSpeed;
	OnRep_DesiredMovementSpeed();
}

void AWildOmissionAICharacter::StopRunning()
{
	DesiredMovementSpeed = DefaultWalkSpeed;
	OnRep_DesiredMovementSpeed();
}

void AWildOmissionAICharacter::SetIdleSoundsEnabled(bool InEnabled)
{
	IdleSoundsEnabled = InEnabled;
}

bool AWildOmissionAICharacter::AreIdleSoundsEnabled() const
{
	return IdleSoundsEnabled;
}

void AWildOmissionAICharacter::SetIdleSoundTimer()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FTimerHandle IdleSoundTimerHandle;
	FTimerDelegate IdleSoundTimerDelegate;
	const float IdleSoundDelay = FMath::RandRange(MinTimeBetweenIdleSoundSeconds, MaxTimeBetweenIdleSoundSeconds);

	IdleSoundTimerDelegate.BindUObject(this, &AWildOmissionAICharacter::PlayIdleSound);
	World->GetTimerManager().SetTimer(IdleSoundTimerHandle, IdleSoundTimerDelegate, IdleSoundDelay, false);
}

void AWildOmissionAICharacter::PlayIdleSound()
{
	SetIdleSoundTimer();

	UWorld* World = GetWorld();
	if (World == nullptr || IdleSound == nullptr || IdleSoundsEnabled == false)
	{
		return;
	}

	UGameplayStatics::SpawnSoundAttached(IdleSound, RootComponent);
}

void AWildOmissionAICharacter::OnRep_DesiredMovementSpeed()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent == nullptr)
	{
		return;
	}

	CharacterMovementComponent->MaxWalkSpeed = DesiredMovementSpeed;
}

// Called every frame
void AWildOmissionAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UMovementComponent* OurMovementComponent = GetMovementComponent();
	if (OurMovementComponent == nullptr)
	{
		return;
	}

	APhysicsVolume* CurrentPhysicsVolume = OurMovementComponent->GetPhysicsVolume();
	if (CurrentPhysicsVolume == nullptr)
	{
		return;
	}

	if (CurrentPhysicsVolume->bWaterVolume == false)
	{
		return;
	}
	
	AddMovementInput(FVector::UpVector);
}

// Called to bind functionality to input
void AWildOmissionAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}