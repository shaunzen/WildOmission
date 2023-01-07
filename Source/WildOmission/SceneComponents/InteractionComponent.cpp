// Copyright Telephone Studios. All Rights Reserved.


#include "InteractionComponent.h"
#include "../Characters/PlayerCharacter.h"
#include "../PlayerControllers/PlayerCharacterController.h"
#include "../ActorComponents/InventoryComponent.h"
#include "../Actors/WorldItem.h"
#include "../Widgets/PlayerHUDWidget.h"

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

void UInteractionComponent::UpdateInteractionPrompt()
{
	if (PlayerHUDWidget == nullptr)
	{
		return;
	}
	FHitResult HitResult;
	// If we are looking at an interactable item
	if (InteractableItemInRange(HitResult))
	{
		// If the interactable item we are looking at is a world item
		if (AWorldItem* WorldItem = Cast<AWorldItem>(HitResult.GetActor()))
		{
			// Get the item data for the item we are looking at
			FItem* ItemData = OwnerPlayer->GetInventoryComponent()->GetItemData(WorldItem->GetItemName());
			// Return if its nullptr
			if (ItemData == nullptr) return;
			// Set the interaction prompt
			
			PlayerHUDWidget->SetInteractionPrompt(FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemData->DisplayName.ToString()));
		}
		else
		{
			// Clear the interaction prompt
			PlayerHUDWidget->SetInteractionPrompt(FString(TEXT("")));
		}
	}
	else
	{
		// Clear the interaction prompt
		PlayerHUDWidget->SetInteractionPrompt(FString(TEXT("")));
	}
}

void UInteractionComponent::Interact()
{
	FHitResult HitResult;
	// If the actor we are looking at is an interactable
	if (InteractableItemInRange(HitResult))
	{
		// If the actor is a world item
		if (AWorldItem* WorldItem = Cast<AWorldItem>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Interaction with world item with name of: %s"), *WorldItem->GetItemName().ToString());
			// Add item to inventory
			OwnerPlayer->GetInventoryComponent()->AddItem(WorldItem->GetItemName(), WorldItem->GetItemQuantity());
			// Remove item from world
			OwnerPlayerController->Server_DestroyActor(HitResult.GetActor());
		}
	}
}

bool UInteractionComponent::InteractableItemInRange(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + (GetForwardVector() * InteractionRange);
	return GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		Start, End,
		ECollisionChannel::ECC_GameTraceChannel1
	);
}