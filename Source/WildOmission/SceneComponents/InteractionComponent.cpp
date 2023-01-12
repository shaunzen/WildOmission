// Copyright Telephone Studios. All Rights Reserved.


#include "InteractionComponent.h"
#include "WildOmission/Characters/PlayerCharacter.h"
#include "WildOmission/PlayerControllers/PlayerCharacterController.h"
#include "WildOmission/ActorComponents/InventoryComponent.h"
#include "WildOmission/Actors/WorldItem.h"
#include "WildOmission/Widgets/PlayerHUDWidget.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractionRange = 300.0f;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<APlayerCharacter>(GetOwner());
	if (OwnerPlayer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractionComponent: Invalid Owner Player"));
		return;
	}

	OwnerPlayerController = Cast<APlayerCharacterController>(OwnerPlayer->GetController());
	if (OwnerPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractionComponent: Invalid Owner Controller"));
		return;
	}

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