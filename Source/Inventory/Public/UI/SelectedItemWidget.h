// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectedItemWidget.generated.h"

class UBorder;
class UTextBlock;
class UProgressBar;

UCLASS()
class INVENTORY_API USelectedItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void Show();
	void Hide();

private:
	UPROPERTY(meta = (BindWidget))
	UBorder* IconBorder;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuantityText;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* DurabilityBar;

	UFUNCTION()
	void Refresh(const FInventoryItem& SelectedItem);
};
