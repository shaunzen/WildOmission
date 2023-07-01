// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

class UInventoryMenuWidget;
class UCraftingMenuWidget;
class USelectedItemWidget;
class UHoveredItemNameTag;
class UGameChatWidget;
class UNotificationPanelWidget;
class AItemContainerBase;

UCLASS(Abstract)
class WILDOMISSIONCORE_API UPlayerHUDWidget : public UUserWidget, public IGameChatParentWidget
{
	GENERATED_BODY()

public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual bool Initialize() override;

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
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionPrompt;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* DurabilityBar;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* MenuBackgroundBorder;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	UInventoryMenuWidget* InventoryMenu;

	UPROPERTY(Meta = (BindWidget))
	UCraftingMenuWidget* CraftingMenu;

	UPROPERTY(Meta = (BindWidget))
	UGameChatWidget* Chat;

	UPROPERTY(Meta = (BindWidget))
	UNotificationPanelWidget* NotificationPanel;
	
	UPROPERTY(meta = (BindWidget))
	UVitalsWidget* Vitals;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* BrandingTextBlock;

	void UpdateBrandingText();

	void OpenMenuPanel(bool ShowBackground = true);
	void SwitchToInventoryMenu();
	void SwitchToCraftingMenu();
	void CloseMenuPanel();

	void UpdateInteractionPrompt();
	void UpdateDurabilityPrompt();
	
	void SetMouseCursorToCenter();

	UFUNCTION()
	void MenuBackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

};