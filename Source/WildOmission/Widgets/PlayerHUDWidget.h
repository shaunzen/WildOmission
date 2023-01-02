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
class USelectedItemWidget;

UCLASS(Abstract)
class WILDOMISSION_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Setters
	void ToggleInventory();
	void SetInteractionPrompt(FString InString);
	void SetVitals(UVitalsComponent* InVitals);

	// Getters
	UInventoryWidget* GetInventoryWidget();
	bool InventoryOpen();
private:

	bool bInventoryOpen;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionPrompt;
	UPROPERTY(meta = (BindWidget))
	UBorder* BackgroundBorder;

	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* Inventory;
	UPROPERTY(meta = (BindWidget))
	USelectedItemWidget* SelectedItem;
	UPROPERTY(meta = (BindWidget))
	UVitalsWidget* Vitals;
};