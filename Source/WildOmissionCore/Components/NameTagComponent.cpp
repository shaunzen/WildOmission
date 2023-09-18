// Copyright Telephone Studios. All Rights Reserved.


#include "NameTagComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UNameTagComponent::UNameTagComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Placeholder = FString("Jerald :s");
	SetText(FText::FromString(Placeholder));

	// Epic thats a typo lmao
	SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
}

void UNameTagComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNameTagComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateRotation();
	UpdateName();
	UpdateVisibility();
}

void UNameTagComponent::UpdateRotation()
{
	APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (LocalPawn == nullptr)
	{
		return;
	}

	FRotator TagRotation;

	TagRotation = UKismetMathLibrary::FindLookAtRotation(LocalPawn->GetActorLocation(), this->GetComponentLocation());
	TagRotation.Yaw += 180.0f;
	TagRotation.Pitch = 0.0f;

	SetWorldRotation(TagRotation);
}

void UNameTagComponent::UpdateName()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return;
	}

	APlayerState* OwnerPlayerState = OwnerPawn->GetPlayerState();
	if (OwnerPlayerState == nullptr)
	{
		return;
	}

	SetText(FText::FromString(OwnerPlayerState->GetPlayerName()));
}

void UNameTagComponent::UpdateVisibility()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner == nullptr || !PawnOwner->IsLocallyControlled())
	{
		return;
	}

	SetVisibility(false);
}
