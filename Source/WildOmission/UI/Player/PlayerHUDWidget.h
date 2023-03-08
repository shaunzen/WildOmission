// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UTextBlock;
class UBorder;

class UVitalsComponent;
class UVitalsWidget;

class UInventoryWidget;
class UPlayerInventoryWidget;
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
	void ToggleInventory();
	
	// Getters
	UPlayerInventoryWidget* GetPlayerInventoryWidget();
	
	bool IsInventoryMenuOpen();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionPrompt;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* BackgroundBorder;

	UPROPERTY(meta = (BindWidget))
	UPlayerInventoryWidget* PlayerInventory;
	
	UPROPERTY(meta = (BindWidget))
	USelectedItemWidget* SelectedItem;
	
	UPROPERTY(meta = (BindWidget))
	UVitalsWidget* Vitals;

	bool bInventoryMenuOpen;

	void UpdateInteractionPrompt();
	void UpdateSelectedItemLocation();

	UFUNCTION()
	void BackgroundMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

};