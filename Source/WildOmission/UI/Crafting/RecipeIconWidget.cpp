// Copyright Telephone Studios. All Rights Reserved.


#include "RecipeIconWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"

void URecipeIconWidget::Setup(UCraftingMenuWidget* InParent, const uint32& InIndex, UMaterialInstance* Icon)
{
	Parent = InParent;
	Index = InIndex;

	// this isnt working???
	RecipeIconBorder->SetBrushFromMaterial(Icon);
	FString MaterialName = Icon->GetName();
	
	UE_LOG(LogTemp, Warning, TEXT("Setup recipe icon, Material name %s"), *MaterialName);

	RecipeButton->OnClicked.AddDynamic(this, &URecipeIconWidget::OnClicked);
}

void URecipeIconWidget::OnClicked()
{
	// TODO
	//Parent->SetSelectedRecipe(Index);
}