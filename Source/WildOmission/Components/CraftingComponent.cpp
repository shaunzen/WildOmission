// Copyright Telephone Studios. All Rights Reserved.


#include "CraftingComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	
	ConstructorHelpers::FObjectFinder<UDataTable> RecipeDataTableBlueprint(TEXT("/Game/WildOmission/Core/DataTables/DT_CraftingRecipes"));
	if (RecipeDataTableBlueprint.Object == nullptr)
	{
		return;
	}

	RecipeDataTable = RecipeDataTableBlueprint.Object;
}


// Called when the game starts
void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

FCraftingRecipe* UCraftingComponent::GetRecipe(const FName& RecipeName)
{
	if (RecipeDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Recipe Context"));
	
	return RecipeDataTable->FindRow<FCraftingRecipe>(RecipeName, ContextString);
}