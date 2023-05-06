// Copyright Telephone Studios. All Rights Reserved.


#include "InteractionComponent.h"
#include "WildOmission/Core/Interfaces/Interactable.h"
#include "WildOmission/Core/Interfaces/DurabilityInterface.h"

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
	if (!LineTraceOnVisibility(HitResult))
	{
		return;
	}

	IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor());
	if (Interactable == nullptr)
	{
		return;
	}

	Server_Interact(HitResult.GetActor());
}

FString UInteractionComponent::GetInteractionString() const
{
	return InteractionString;
}

void UInteractionComponent::UpdateInteractionPrompt()
{
	FHitResult HitResult;
	if (!LineTraceOnVisibility(HitResult))
	{
		InteractionString = FString("");
		return;
	}

	IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor());
	if (Interactable == nullptr)
	{
		InteractionString = FString("");
		return;
	}
	
	InteractionString = Interactable->PromptText();
}

bool UInteractionComponent::LineTraceOnVisibility(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + (GetForwardVector() * InteractionRange);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this->GetOwner());
	return GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, Sphere, Params);
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