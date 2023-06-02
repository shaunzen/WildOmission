// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "WildOmission/Core/Structs/CraftingRecipeEntry.h"
#include "RecipeIconWidget.generated.h"

class UButton;
class UBorder;
class UCraftingMenuWidget;

UCLASS()
class WILDOMISSION_API URecipeIconWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	void Setup(UCraftingMenuWidget* InParentMenu, const FCraftingRecipeEntry& InParentEntry);
	
	UFUNCTION(BlueprintCallable)
	bool IsSelected() const;

	UFUNCTION(BlueprintCallable)
	bool IsCraftable() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* RecipeButton;
	
	UPROPERTY(Meta = (BindWidget))
	UBorder* RecipeIconBorder;

	UPROPERTY()
	UCraftingMenuWidget* ParentMenu;

	UPROPERTY()
	FCraftingRecipeEntry ParentEntry;
	
	UFUNCTION()
	void OnClicked();
};
