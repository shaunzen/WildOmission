// Copyright Telephone Studios. All Rights Reserved.


#include "Components/InteractionComponent.h"
#include "Interfaces/Interactable.h"
#include "Kismet/KismetMathLibrary.h"

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
	Server_Interact();
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
	const FVector Start = GetComponentLocation();
	const FVector End = Start + (GetOwnerForwardVector() * InteractionRange);
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this->GetOwner());
	return GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, Sphere, Params);
}

FVector UInteractionComponent::GetOwnerForwardVector() const
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner == nullptr)
	{
		return this->GetForwardVector();
	}

	return UKismetMathLibrary::GetForwardVector(PawnOwner->GetControlRotation());
}

void UInteractionComponent::Server_Interact_Implementation()
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

	Interactable->Interact(GetOwner());
}