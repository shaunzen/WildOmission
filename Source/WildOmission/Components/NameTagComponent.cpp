// Copyright Telephone Studios. All Rights Reserved.


#include "NameTagComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UNameTagComponent::UNameTagComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Placeholder = FString("Jerald :S");
	SetText(FText::FromString(Placeholder));

	// Epic thats a typo lmao
	SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
}

void UNameTagComponent::BeginPlay()
{
	Super::BeginPlay();
	
	FString OwnerPlayerName;
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return;
	}

	APlayerState* OwnerState = OwnerPawn->GetPlayerState();
	if (OwnerState == nullptr)
	{
		return;
	}

	OwnerPlayerName = OwnerState->GetPlayerName();

	SetText(FText::FromString(OwnerPlayerName));

	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		return;
	}

	SetVisibility(false);
}

void UNameTagComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Set rotation to always face local player
	
	APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (LocalPawn == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString("failed to get local pawn"));
		return;
	}

	FRotator TagRotation;
	
	TagRotation = UKismetMathLibrary::FindLookAtRotation(LocalPawn->GetActorLocation(), this->GetComponentLocation());
	TagRotation.Yaw += 180.0f;
	TagRotation.Pitch = 0.0f;

	SetWorldRotation(TagRotation);
}