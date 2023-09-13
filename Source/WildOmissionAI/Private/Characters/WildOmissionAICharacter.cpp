// Copyright Telephone Studios. All Rights Reserved.


#include "Characters/WildOmissionAICharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavigationInvokerComponent.h"
#include "Components/VitalsComponent.h"
#include "Components/DistanceDespawnComponent.h"
#include "Characters/WildOmissionAIRagdoll.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"
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

	MinTimeBetweenIdleSoundSeconds = 1.0f;
	MaxTimeBetweenIdleSoundSeconds = 5.0f;

	IdleSound = nullptr;
	RagdollClass = nullptr;
}

// Called when the game starts or when spawned
void AWildOmissionAICharacter::BeginPlay()
{
	Super::BeginPlay();

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
		AWildOmissionAIRagdoll* SpawnedRagdoll = GetWorld()->SpawnActor<AWildOmissionAIRagdoll>(RagdollClass, GetActorLocation(), GetActorRotation());
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
			AttachedActor->AttachToComponent(SpawnedRagdoll->GetMeshComponent(), FAttachmentTransformRules::KeepWorldTransform, Socket);
		}
	}
	HandleDespawn();
}

void AWildOmissionAICharacter::SetIdleSoundTimer()
{
	FTimerHandle IdleSoundTimerHandle;
	FTimerDelegate IdleSoundTimerDelegate;
	const float IdleSoundDelay = FMath::RandRange(MinTimeBetweenIdleSoundSeconds, MaxTimeBetweenIdleSoundSeconds);

	IdleSoundTimerDelegate.BindUObject(this, &AWildOmissionAICharacter::PlayIdleSound);
	GetWorld()->GetTimerManager().SetTimer(IdleSoundTimerHandle, IdleSoundTimerDelegate, IdleSoundDelay, false);
}

void AWildOmissionAICharacter::PlayIdleSound()
{
	SetIdleSoundTimer();

	if (GetWorld() == nullptr || IdleSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), IdleSound, GetActorLocation());
}

// Called every frame
void AWildOmissionAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetMovementComponent()->GetPhysicsVolume()->bWaterVolume == true)
	{
		AddMovementInput(FVector::UpVector);
	}
}

// Called to bind functionality to input
void AWildOmissionAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}