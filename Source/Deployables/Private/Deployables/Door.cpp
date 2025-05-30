// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Door.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LockComponent.h"
#include "Locks/Lock.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent->SetMobility(EComponentMobility::Type::Movable);

	InteractionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractionMesh"));
	InteractionMesh->SetupAttachment(DeployableRootComponent);
	InteractionMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractionMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	InteractionMesh->SetVisibility(false);

	LockComponent = CreateDefaultSubobject<ULockComponent>(TEXT("LockComponent"));
	LockComponent->SetupAttachment(MeshComponent);

	OpenSound = nullptr;
	CloseSound = nullptr;

	bCanSpawnOnGround = false;
	bCanSpawnOnFloor = false;
	bCanSpawnOnWall = false;
	CanSpawnOnAnchor = EBuildAnchorType::DoorAnchor;
	bFollowsSurfaceNormal = false;

	bIsOpen = false;
	OpenAngle = 120.0f;
}

void ADoor::OnSpawn()
{
	Super::OnSpawn();

}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOpen)
	{
		MeshComponent->SetRelativeRotation(FMath::Lerp(MeshComponent->GetRelativeRotation(), FRotator(0.0f, OpenAngle, 0.0f), 10.0f * DeltaTime));
	}
	else
	{
		
		MeshComponent->SetRelativeRotation(FMath::Lerp(MeshComponent->GetRelativeRotation(), FRotator::ZeroRotator, 10.0f * DeltaTime));
	}
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bIsOpen);
}

void ADoor::Interact(AActor* Interactor)
{
	if (IsValid(LockComponent->GetLock()) && !LockComponent->GetLock()->IsAuthorized(Cast<APawn>(Interactor)) && LockComponent->GetLock()->IsLocked())
	{
		return;
	}

	bIsOpen = !bIsOpen;

	if (bIsOpen)
	{
		Client_PlayOpenSound();
	}
	else
	{
		Client_PlayCloseSound();
	}

	FlushNetDormancy();
}

FString ADoor::PromptText()
{
	if (!bIsOpen)
	{
		return TEXT("open door");
	}

	return TEXT("close door");
}

void ADoor::Client_PlayOpenSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
}

void ADoor::Client_PlayCloseSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CloseSound, GetActorLocation());
}