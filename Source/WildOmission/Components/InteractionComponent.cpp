// Copyright Telephone Studios. All Rights Reserved.


#include "InteractionComponent.h"
#include "WildOmission/Core/Interfaces/Interactable.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractionRange = 300.0f;
	InteractionRadius = 5.0f;
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateInteractionPrompt();
}

void UInteractionComponent::Interact()
{
	FHitResult HitResult;
	if (LineTraceOnVisibility(HitResult))
	{
		if (IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor()))
		{
			Server_Interact(HitResult.GetActor());
		}
	}
}

FString UInteractionComponent::GetInteractionString() const
{
	return InteractionString;
}

void UInteractionComponent::UpdateInteractionPrompt()
{
	FHitResult HitResult;
	if (LineTraceOnVisibility(HitResult))
	{
		if (IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor()))
		{
			InteractionString = Interactable->PromptText();
		}
		else
		{
			InteractionString = FString("");
		}
	}
	else
	{
		InteractionString = FString("");
	}
}

bool UInteractionComponent::LineTraceOnVisibility(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + (GetForwardVector() * InteractionRange);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRadius);

	return GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, Sphere);
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