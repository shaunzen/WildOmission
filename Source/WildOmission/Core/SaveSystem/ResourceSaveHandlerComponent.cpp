// Copyright Telephone Studios. All Rights Reserved.


#include "ResourceSaveHandlerComponent.h"

// Sets default values for this component's properties
UResourceSaveHandlerComponent::UResourceSaveHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UResourceSaveHandlerComponent::Generate()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Generating New Resources."));
}

void UResourceSaveHandlerComponent::Save()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Saving Resources."));
}

void UResourceSaveHandlerComponent::Load()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString("Loading Resources from save."));
}