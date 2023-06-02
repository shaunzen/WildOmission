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
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* RecipeButton;
	
	UPROPERTY(Meta = (BindWidget))
	UBorder* RecipeIconBorder;

	UPROPERTY()
	UCraftingMenuWidget* ParentMenu;

	UPROPERTY()
	FCraftingRecipeEntry ParentEntry;
	
	bool Hovering = false;

	UFUNCTION()
	void OnClicked();
};
