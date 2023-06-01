// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingRecipeCategoryButton.generated.h"

class UCraftingMenuWidget;
class UButton;
class UTextBlock;
enum EItemCategory;

UCLASS()
class WILDOMISSION_API UCraftingRecipeCategoryButton : public UUserWidget
{
	GENERATED_BODY()

public:	
	UCraftingRecipeCategoryButton(const FObjectInitializer& ObjectInitializer);

	void Setup(UCraftingMenuWidget* InParent, TEnumAsByte<EItemCategory> InCategory);
private:
	UPROPERTY(Meta = (BindWidget))
	UButton* Button;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock;
	
	UPROPERTY()
	UCraftingMenuWidget* ParentCraftingMenu;
	
	UPROPERTY()
	TEnumAsByte<EItemCategory> Category;

	UFUNCTION()
	void OnClicked();

};
