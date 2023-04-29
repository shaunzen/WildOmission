// Copyright Telephone Studios. All Rights Reserved.


#include "Door.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	bCanSpawnOnGround = false;
	bCanSpawnOnFloor = false;
	bCanSpawnOnWall = false;
	CanSpawnOnAnchor = EBuildAnchorType::DoorAnchor;
	bFollowsSurfaceNormal = false;
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bIsOpen);
}

void ADoor::Interact(AActor* Interactor)
{
	bIsOpen = !bIsOpen;
	
	float DoorAngle = 0;

	if (bIsOpen)
	{
		DoorAngle = FMath::Lerp(0.0f, 90.0f, GetWorld()->GetTimeSeconds());
	}
	else
	{
		DoorAngle = FMath::Lerp(90.0f, 0.0f, GetWorld()->GetTimeSeconds());
	}
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw = DoorAngle;

	SetActorRotation(NewRotation);
}

FString ADoor::PromptText()
{

	return FString("Press 'E' on this door");
}