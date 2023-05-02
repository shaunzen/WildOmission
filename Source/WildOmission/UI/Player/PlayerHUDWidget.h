// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UTextBlock;
class UBorder;
class UCanvasPanel;
class UWidgetSwitcher;
class UHorizontalBox;
class UProgressBar;

class UVitalsComponent;
class UVitalsWidget;

class UInventoryWidget;
class UPlayerInventoryWidget;
class UCraftingMenuWidget;
class USelectedItemWidget;
class UHoveredItemNameTag;

class AItemContainerBase;

UCLASS(Abstract)
class WILDOMISSION_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual bool Initialize() override;

	void OpenContainer(AItemContainerBase* Container);

	// Setters
	UFUNCTION(BlueprintCallable)
	void ToggleInventoryMenu();
	
	UFUNCTION(BlueprintCallable)
	void ToggleCraftingMenu();

	// Getters
	UPlayerInventoryWidget* GetPlayerInventoryWidget();
	UHoveredItemNameTag* GetHoveredItemNameTag() const;
	bool SelectedItemVisible() const;

	UFUNCTION(BlueprintCallable)
	bool IsMenuOpen() const;

	UFUNCTION(BlueprintCallable)
	bool IsInventoryMenuOpen() const;

	UFUNCTION(BlueprintCallable)
	bool IsCraftingMenuOpen() const;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionPrompt;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* DurabilityBar;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* MenuBackgroundBorder;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	UHorizontalBox* InventoryHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	UPlayerInventoryWidget* PlayerInventory;
	
	UPROPERTY()
	UInventoryWidget* OpenContainerWidget;

	UPROPERTY(Meta = (BindWidget))
	UCraftingMenuWidget* CraftingMenu;

	UPROPERTY(meta = (BindWidget))
	USelectedItemWidget* SelectedItem;
	
	UPROPERTY(Meta = (BindWidget))
	UHoveredItemNameTag* HoveredItemNameTag;

	UPROPERTY(meta = (BindWidget))
	UVitalsWidget* Vitals;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* BrandingTextBlock;

	bool bInventoryMenuOpen;

	bool bCraftingMenuOpen;

	void UpdateBrandingText();

	void OpenMenuPanel();
	void SwitchToInventoryMenu();
	void SwitchToCraftingMenu();
	void CloseMenuPanel();

	void UpdateInteractionPrompt();
	void UpdateDurabilityPrompt();
	void UpdateFollowMousePointerWidgets();

	UFUNCTION()
	void MenuBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

};