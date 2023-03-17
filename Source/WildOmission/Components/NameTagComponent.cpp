// Copyright Telephone Studios. All Rights Reserved.


#include "NameTagComponent.h"
#include "UObject/ConstructorHelpers.h"

UNameTagComponent::UNameTagComponent()
{
	FString Placeholder = FString("Jerald :S");
	SetText(FText::FromString(Placeholder));

	// Epic thats a typo lmao
	SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	
	/*ConstructorHelpers::FObjectFinder<UFont> GameFont(TEXT("/Game/WildOmission/UI/Fonts/Akshar"));
	if (GameFont.Object == nullptr)
	{
		return;
	}

	SetFont(GameFont.Object);*/
}

void UNameTagComponent::BeginPlay()
{
	Super::BeginPlay();
	// Set text to owner player name
}

void UNameTagComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// set rotation to always face player
}