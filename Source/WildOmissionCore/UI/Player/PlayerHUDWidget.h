// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/InventoryParentWidget.h"
#include "Interfaces/GameChatParentWidget.h"
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
class UGameChatWidget;
class UNotificationPanelWidget;
class AItemContainerBase;

UCLASS(Abstract)
class WILDOMISSIONCORE_API UPlayerHUDWidget : public UUserWidget, public IInventoryParentWidget, public IGameChatParentWidget
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

	// Begin IGameChatParentWidget Implementation
	UFUNCTION(BlueprintCallable)
	virtual void ToggleChatMenu() override;
	// End IGameChatParentWidget Implementation

	// Getters
	UPlayerInventoryWidget* GetPlayerInventoryWidget();

	// Begin IInventoryParentWidget Implementation
	virtual UHoveredItemNameTag* GetHoveredItemNameTag() const override;
	virtual bool SelectedItemVisible() const override;
	// End IInventoryParentWidget Implementation

	UFUNCTION(BlueprintCallable)
	bool IsMenuOpen() const;

	// Begin IInventoryParentWidget Implementation
	UFUNCTION(BlueprintCallable)
	virtual bool IsInventoryMenuOpen() const override;
	// End IInventoryParentWidget Implementation

	UFUNCTION(BlueprintCallable)
	bool IsCraftingMenuOpen() const;

	UFUNCTION(BlueprintCallable)
	bool IsChatMenuOpen() const;

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

	UPROPERTY(Meta = (BindWidget))
	UGameChatWidget* Chat;

	UPROPERTY(Meta = (BindWidget))
	UNotificationPanelWidget* NotificationPanel;
	
	UPROPERTY(meta = (BindWidget))
	UVitalsWidget* Vitals;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* BrandingTextBlock;

	bool bInventoryMenuOpen;

	bool bCraftingMenuOpen;

	void UpdateBrandingText();

	void OpenMenuPanel(bool ShowBackground = true);
	void SwitchToInventoryMenu();
	void SwitchToCraftingMenu();
	void CloseMenuPanel();

	void UpdateInteractionPrompt();
	void UpdateDurabilityPrompt();
	void UpdateFollowMousePointerWidgets();

	void SetMouseCursorToCenter();

	UFUNCTION()
	void MenuBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

};