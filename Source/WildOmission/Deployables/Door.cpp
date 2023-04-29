// Copyright Telephone Studios. All Rights Reserved.


#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	bCanSpawnOnGround = false;
	bCanSpawnOnFloor = false;
	bCanSpawnOnWall = false;
	CanSpawnOnAnchor = EBuildAnchorType::DoorAnchor;
	bFollowsSurfaceNormal = false;

	bIsOpen = false;
	SpawnRotation = FRotator::ZeroRotator;
	RotateValue = 160.0f;
}

void ADoor::OnSpawn()
{
	Super::OnSpawn();

	SpawnRotation = GetActorRotation();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorRotation = GetActorRotation();

	if (bIsOpen)
	{
		SetActorRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(SpawnRotation + FRotator(0.0f, RotateValue, 0.0f)), 0.1f));
	}
	else
	{
		SetActorRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(SpawnRotation), 0.1f));
	}
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bIsOpen);
	DOREPLIFETIME(ADoor, SpawnRotation);
}

void ADoor::Interact(AActor* Interactor)
{
	bIsOpen = !bIsOpen;

	if (bIsOpen)
	{
		Client_PlayOpenSound();
	}
	else
	{
		Client_PlayCloseSound();
	}
}

FString ADoor::PromptText()
{
	if (!bIsOpen)
	{
		return FString("Press 'E' to open door");
	}

	return FString("Press 'E' to close door");
}

void ADoor::Client_PlayOpenSound_Implementation()
{
	if (OpenSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
}

void ADoor::Client_PlayCloseSound_Implementation()
{
	if (CloseSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CloseSound, GetActorLocation());
}