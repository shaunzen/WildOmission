// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "FurnaceWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class WILDOMISSION_API UFurnaceWidget : public UInventoryWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* ToggleButton;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ToggleText;

	UFUNCTION()
	void ToggleButtonPressed();

};
