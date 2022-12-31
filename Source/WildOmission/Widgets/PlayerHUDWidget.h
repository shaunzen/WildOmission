// (c) 2023 Telephone Studios, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UBorder;

class UVitalsComponent;
class UInventoryComponent;

class UVitalsWidget;
class UInventoryWidget;


UCLASS(Abstract)
class WILDOMISSION_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	void Setup(UInventoryComponent* InInventoryComponent);

	void ToggleInventory();
	
	void SetInteractionPrompt(FString InString);
	void SetVitals(UVitalsComponent* InVitals);
private:

	bool bInventoryOpen;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionPrompt;
	UPROPERTY(meta = (BindWidget))
	UBorder* BackgroundBorder;
	
	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* Inventory;
	UPROPERTY(meta = (BindWidget))
	UVitalsWidget* Vitals;
};