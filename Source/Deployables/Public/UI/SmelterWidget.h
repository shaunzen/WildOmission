// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/InventoryWidget.h"
#include "SmelterWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class DEPLOYABLES_API USmelterWidget : public UInventoryWidget
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
