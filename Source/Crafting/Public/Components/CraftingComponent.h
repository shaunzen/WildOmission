// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/CraftingRecipe.h"
#include "CraftingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemCraftedSignature, const FName&, ItemID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAFTING_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();

	UFUNCTION(Server, Reliable)
	void Server_CraftItem(const FName& ItemToCraft);

	FOnItemCraftedSignature OnItemCrafted;

	static TArray<FName> GetAllRecipes();

	static FCraftingRecipe* GetRecipe(const FName& RecipeName);

};
