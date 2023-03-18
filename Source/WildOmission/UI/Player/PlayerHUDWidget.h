// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UTextBlock;
class UBorder;
class UWidgetSwitcher;

class UVitalsComponent;
class UVitalsWidget;

class UInventoryWidget;
class UPlayerInventoryWidget;
class UCraftingWidget;
class USelectedItemWidget;

UCLASS(Abstract)
class WILDOMISSION_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void RefreshInventoryStates();
	
	// Setters
	void ToggleInventoryMenu();
	
	void ToggleCraftingMenu();

	// Getters
	UPlayerInventoryWidget* GetPlayerInventoryWidget();
	
	bool IsMenuOpen() const;

	bool IsInventoryMenuOpen() const;

	bool IsCraftingMenuOpen() const;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionPrompt;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* MenuBackgroundBorder;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UPlayerInventoryWidget* PlayerInventory;
	
	UPROPERTY(Meta = (BindWidget))
	UCraftingWidget* CraftingMenu;

	UPROPERTY(meta = (BindWidget))
	USelectedItemWidget* SelectedItem;
	
	UPROPERTY(meta = (BindWidget))
	UVitalsWidget* Vitals;

	bool bInventoryMenuOpen;

	bool bCraftingMenuOpen;

	void OpenMenuPanel();
	void SwitchToInventoryMenu();
	void SwitchToCraftingMenu();
	void CloseMenuPanel();

	void UpdateInteractionPrompt();
	void UpdateSelectedItemLocation();

	UFUNCTION()
	void MenuBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

};