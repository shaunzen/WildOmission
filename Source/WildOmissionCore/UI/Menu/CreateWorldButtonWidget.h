// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateWorldButtonWidget.generated.h"

class UButton;
class UBorder;

UCLASS()
class WILDOMISSIONCORE_API UCreateWorldButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UButton* GetButton() const;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* Button;
	UPROPERTY(Meta = (BindWidget))
	UBorder* RowBorder;

	bool Hovering = false;
};
