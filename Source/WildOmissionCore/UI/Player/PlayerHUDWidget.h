// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

UCLASS(Abstract)
class WILDOMISSIONCORE_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ShowBranding(bool Show);
	void ShowCrosshair(bool Show);
	void SetHideChatUnlessOpen(bool HideChatUnlessOpen);

	void SetVitalsHidden(bool Hidden);

	// Setters
	UFUNCTION(BlueprintCallable)
	void ToggleInventoryMenu(bool ForceOpen = false);

	UFUNCTION(BlueprintCallable)
	void ToggleCraftingMenu(bool ForceOpen = false);

	UFUNCTION(BlueprintCallable)
	void ToggleChatMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleCoordinates();

	UFUNCTION(BlueprintCallable)
	bool IsMenuOpen() const;

	UFUNCTION(BlueprintCallable)
	bool IsInventoryMenuOpen() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsCraftingMenuOpen() const;

	UFUNCTION(BlueprintCallable)
	bool IsChatMenuOpen() const;

	class UInventoryMenuWidget* GetInventoryMenu() const;

private:
	UPROPERTY(Meta = (BindWidget))
	class UBorder* MenuBackgroundBorder;

	UPROPERTY(Meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	class UWidget* Vitals;

	UPROPERTY(Meta = (BindWidget))
	class UPanelWidget* InventoryPanel;

	UPROPERTY(Meta = (BindWidget))
	class UButton* OpenCraftingButton;

	UPROPERTY(Meta = (BindWidget))
	class UInventoryMenuWidget* InventoryMenu;

	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* CraftingPanel;

	UPROPERTY(Meta = (BindWidget))
	class UButton* OpenInventoryButton;

	UPROPERTY(Meta = (BindWidget))
	class UCraftingMenuWidget* CraftingMenu;

	UPROPERTY(Meta = (BindWidget))
	class UGameChatWidget* Chat;

	UPROPERTY(Meta = (BindWidget))
	class UNotificationPanelWidget* NotificationPanel;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* BrandingTextBlock;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* CoordinatesTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UWidget* Crosshair;

	void UpdateBrandingText();

	UFUNCTION()
	void OpenMenuPanel(bool ShowBackground = true);
	
	UFUNCTION()
	void SwitchToInventoryMenu();
	
	UFUNCTION()
	void SwitchToCraftingMenu();
	
	UFUNCTION()
	void CloseMenuPanel();

	void SetMouseCursorToCenter();

	UFUNCTION()
	void MenuBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnOpenContainerChanged(UInventoryComponent* OpenContainer);

};