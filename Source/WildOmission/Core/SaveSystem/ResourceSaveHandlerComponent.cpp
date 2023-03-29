// Copyright Telephone Studios. All Rights Reserved.


#include "ResourceSaveHandlerComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UResourceSaveHandlerComponent::UResourceSaveHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FClassFinder<AActor> Tree01Blueprint(TEXT("/Game/WildOmission/Environment/Trees/BP_Tree_01"));
	ConstructorHelpers::FClassFinder<AActor> Tree02Blueprint(TEXT("/Game/WildOmission/Environment/Trees/BP_Tree_02"));
	ConstructorHelpers::FClassFinder<AActor> Tree03Blueprint(TEXT("/Game/WildOmission/Environment/Trees/BP_Tree_03"));
	ConstructorHelpers::FClassFinder<AActor> StoneNodeBlueprint(TEXT("/Game/WildOmission/Environment/Nodes/BP_StoneNode"));

	if (!Tree01Blueprint.Succeeded() || !Tree02Blueprint.Succeeded() || !Tree03Blueprint.Succeeded() || !StoneNodeBlueprint.Succeeded())
	{
		return;
	}

	Tree01 = Tree01Blueprint.Class;
	Tree02 = Tree02Blueprint.Class;
	Tree03 = Tree03Blueprint.Class;
	StoneNode = StoneNodeBlueprint.Class;
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