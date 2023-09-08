// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/GameChatParentWidget.h"
#include "PlayerHUDWidget.generated.h"

class UTextBlock;
class UBorder;
class UWidgetSwitcher;

class UInventoryMenuWidget;
class UCraftingMenuWidget;
class UGameChatWidget;
class UNotificationPanelWidget;

UCLASS(Abstract)
class WILDOMISSIONCORE_API UPlayerHUDWidget : public UUserWidget, public IGameChatParentWidget
{
	GENERATED_BODY()

public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;	
	
	void ShowBranding(bool Show);
	void ShowCrosshair(bool Show);

	// Setters
	UFUNCTION(BlueprintCallable)
	void ToggleInventoryMenu(bool ForceOpen = false);

	UFUNCTION(BlueprintCallable)
	void ToggleCraftingMenu(bool ForceOpen = false);

	// Begin IGameChatParentWidget Implementation
	UFUNCTION(BlueprintCallable)
	virtual void ToggleChatMenu() override;
	// End IGameChatParentWidget Implementation

	UFUNCTION(BlueprintCallable)
	bool IsMenuOpen() const;

	UFUNCTION(BlueprintCallable)
	bool IsInventoryMenuOpen() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsCraftingMenuOpen() const;

	UFUNCTION(BlueprintCallable)
	bool IsChatMenuOpen() const;

	UInventoryMenuWidget* GetInventoryMenu() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UBorder* MenuBackgroundBorder;

	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	UInventoryMenuWidget* InventoryMenu;

	UPROPERTY(Meta = (BindWidget))
	UCraftingMenuWidget* CraftingMenu;

	UPROPERTY(Meta = (BindWidget))
	UGameChatWidget* Chat;

	UPROPERTY(Meta = (BindWidget))
	UNotificationPanelWidget* NotificationPanel;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* BrandingTextBlock;

	void UpdateBrandingText();

	void OpenMenuPanel(bool ShowBackground = true);
	void SwitchToInventoryMenu();
	void SwitchToCraftingMenu();
	void CloseMenuPanel();

	void SetMouseCursorToCenter();

	UFUNCTION()
	void MenuBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnOpenContainerChanged(UInventoryComponent* OpenContainer);

};