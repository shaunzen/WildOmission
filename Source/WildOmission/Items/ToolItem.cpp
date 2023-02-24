// Copyright Telephone Studios. All Rights Reserved.


#include "ToolItem.h"
#include "Net/UnrealNetwork.h"

AToolItem::AToolItem()
{
	ToolType = EToolType::WOOD;

	GatherMultiplyer = 1.0f;
	EffectiveRangeCentimeters = 2000.0f;
	SwingTimeSeconds = 1.0f;
	Swinging = false;
}

void AToolItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AToolItem, Swinging);
}

void AToolItem::Primary()
{
	Super::Primary();
	Swinging = true;

}

void AToolItem::Secondary()
{
	Super::Secondary();
	Swinging = false;
}

bool AToolItem::IsSwinging() const
{
	return Swinging;
}

void AToolItem::OnRep_Swinging()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Orange, FString::Printf(TEXT("swinging %i"), Swinging));
}