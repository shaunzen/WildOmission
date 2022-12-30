// (c) 2023 Telephone Studios, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UWrapBox;
class UVitalsComponent;
class UInventoryComponent;
class UInventorySlotWidget;

UCLASS(Abstract)
class WILDOMISSION_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	virtual void NativeConstruct() override;

	void SetInteractionPrompt(FString InString);
	void SetVitals(UVitalsComponent* InVitals);
private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ThirstBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HungerBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionPrompt;

	// Inventory
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotClass;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InventoryName;
	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryContentsWrapBox;
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
};