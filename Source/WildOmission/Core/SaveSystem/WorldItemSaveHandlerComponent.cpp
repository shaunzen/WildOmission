// Copyright Telephone Studios. All Rights Reserved.


#include "WorldItemSaveHandlerComponent.h"
#include "WildOmissionSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWorldItemSaveHandlerComponent::UWorldItemSaveHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UWorldItemSaveHandlerComponent::Save(TArray<FWorldItemSave>& OutSaves)
{
	// loop through all actors of type AWorldItem and save their data to the list
}

void UWorldItemSaveHandlerComponent::Load(const TArray<FWorldItemSave>& InSaves)
{
	// loop through all saves and spawn a world item with the entries data
}