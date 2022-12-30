// (c) 2023 Telephone Studios, all rights reserved.


#include "InteractionComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractionRange = 300.0f;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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