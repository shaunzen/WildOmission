// Copyright Telephone Studios. All Rights Reserved.


#include "ToolItem.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "Net/UnrealNetwork.h"

AToolItem::AToolItem()
{
	ToolType = EToolType::WOOD;

	GatherMultiplyer = 1.0f;
	EffectiveRangeCentimeters = 2000.0f;
	SwingTimeSeconds = 1.0f;
}

void AToolItem::Primary()
{
	Super::Primary();

	Client_PlaySwingAnimation();
}

void AToolItem::Secondary()
{
	Super::Secondary();
}

void AToolItem::Client_PlaySwingAnimation_Implementation()
{
	AWildOmissionCharacter* OwnerCharacter = Cast<AWildOmissionCharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		return;
	}
	OwnerCharacter->PlaySwingAnimation();
}