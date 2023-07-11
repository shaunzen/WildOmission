// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Door.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractionMesh"));
	InteractionMesh->SetupAttachment(DeployableRootComponent);
	InteractionMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractionMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	InteractionMesh->SetVisibility(false);

	OpenSound = nullptr;
	CloseSound = nullptr;

	bCanSpawnOnGround = false;
	bCanSpawnOnFloor = false;
	bCanSpawnOnWall = false;
	CanSpawnOnAnchor = EBuildAnchorType::DoorAnchor;
	bFollowsSurfaceNormal = false;

	bIsOpen = false;
	RotateValue = 120.0f;
}

void ADoor::OnSpawn()
{
	Super::OnSpawn();

}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorRotation = MeshComponent->GetRelativeRotation();
	
	if (bIsOpen)
	{
		MeshComponent->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, RotateValue, 0.0f)), 0.1f));
	}
	else
	{
		
		MeshComponent->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat::Identity, 0.1f));
	}
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bIsOpen);
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

	FlushNetDormancy();
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