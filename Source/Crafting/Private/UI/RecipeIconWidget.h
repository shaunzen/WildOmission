// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Structs/CraftingRecipeEntry.h"
#include "RecipeIconWidget.generated.h"

class UButton;
class UBorder;
class UCraftingMenuWidget;

UCLASS()
class URecipeIconWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	void Setup(UCraftingMenuWidget* InParentMenu, const FCraftingRecipeEntry& InParentEntry);
	
	UFUNCTION(BlueprintCallable)
	bool IsSelected() const;

	UFUNCTION(BlueprintCallable)
	bool IsCraftable() const;
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* RecipeButton;
	
	UPROPERTY(Meta = (BindWidget))
	UBorder* RecipeIconBorder;

	UPROPERTY(Meta = (BindWidget))
	UBorder* BackgroundBorder;

	UPROPERTY()
	UCraftingMenuWidget* ParentMenu;

	UPROPERTY()
	FCraftingRecipeEntry ParentEntry;
	
	bool Hovering = false;

	UFUNCTION()
	void OnClicked();
};
