// Copyright Telephone Studios. All Rights Reserved.


#include "InteractionComponent.h"
#include "WildOmission/Interfaces/Interactable.h"
#include "WildOmission/Widgets/PlayerHUDWidget.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractionRange = 300.0f;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInteractionComponent::SetHUDWidget(UPlayerHUDWidget* WidgetToUse)
{
	PlayerHUDWidget = WidgetToUse;
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateInteractionPrompt();
}

void UInteractionComponent::Interact()
{
	FHitResult HitResult;
	if (LineTraceOnInteractableChannel(HitResult))
	{
		if (IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor()))
		{
			Interactable->Interact(GetOwner());
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