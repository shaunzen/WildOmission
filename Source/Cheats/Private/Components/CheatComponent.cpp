// Copyright Telephone Studios. All Rights Reserved.


#include "Components/CheatComponent.h"
#include "UI/CheatMenuWidget.h"

// Sets default values for this component's properties
UCheatComponent::UCheatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CheatMenuWidget = nullptr;
	CheatMenuWidgetClass = nullptr;
}

void UCheatComponent::OpenCheatMenu()
{
	if (CheatMenuWidgetClass == nullptr)
	{
		return;
	}

	
}

void UCheatComponent::CloseCheatMenu()
{
	if (CheatMenuWidget == nullptr)
	{
		return;
	}

	
	CheatMenuWidget = nullptr;
}
