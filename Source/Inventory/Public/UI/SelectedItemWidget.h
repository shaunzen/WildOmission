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

	void SetItem(UMaterialInstance* ItemIcon, const int32& Quantity);
	
	void Show();
	
	void Hide();

private:
	UPROPERTY(meta = (BindWidget))
	UBorder* IconBorder;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuantityText;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* DurabilityBar;

	FString GetQuantityString(const int32& Quantity);
	
	UFUNCTION()
	void Refresh(const FInventoryItem& SelectedItem);
};
