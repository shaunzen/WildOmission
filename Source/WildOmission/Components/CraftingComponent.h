// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Structs/CraftingRecipe.h"
#include "CraftingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(Server, Reliable)
	void Server_CraftItem(const FName& ItemToCraft);

	static TArray<FName> GetAllRecipes();

	static FCraftingRecipe* GetRecipe(const FName& RecipeName);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
