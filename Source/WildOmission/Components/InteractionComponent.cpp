// Copyright Telephone Studios. All Rights Reserved.


#include "InteractionComponent.h"
#include "WildOmission/Core/Interfaces/Interactable.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractionRange = 300.0f;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInteractionComponent::Setup(APawn* InOwnerPawn, UPlayerHUDWidget* WidgetToUse)
{
	OwnerPawn = InOwnerPawn;
	PlayerHUDWidget = WidgetToUse;
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (OwnerPawn == nullptr || OwnerPawn->IsLocallyControlled() == false)
	{
		return;
	}
	UpdateInteractionPrompt();
}

void UInteractionComponent::Interact()
{
	FHitResult HitResult;
	if (LineTraceOnInteractableChannel(HitResult))
	{
		if (IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor()))
		{
			Server_Interact(HitResult.GetActor());
		}
	}
}

void UInteractionComponent::UpdateInteractionPrompt()
{
	FHitResult HitResult;
	if (LineTraceOnInteractableChannel(HitResult))
	{
		if (IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor()))
		{
			PlayerHUDWidget->SetInteractionPrompt(Interactable->PromptText());
		}
	}
	else
	{
		PlayerHUDWidget->SetInteractionPrompt(FString(""));
	}
}

bool UInteractionComponent::LineTraceOnInteractableChannel(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + (GetForwardVector() * InteractionRange);
	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1);
}

void UInteractionComponent::Server_Interact_Implementation(AActor* ActorToInteract)
{
	IInteractable* InteractableActor = Cast<IInteractable>(ActorToInteract);
	if (InteractableActor == nullptr)
	{
		return;
	}

	InteractableActor->Interact(GetOwner());
}