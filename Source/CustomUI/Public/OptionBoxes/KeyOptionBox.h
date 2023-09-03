// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyOptionBox.generated.h"

class UBorder;
class UTextBlock;

UCLASS()
class CUSTOMUI_API UKeyOptionBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetSelectedKey(const FKey& NewSelectedKey);
	FKey GetSelectedKey() const;
protected:
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UBorder* Border;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock;

	FKey SelectedKey;
	bool AwaitingInput;

	void RefreshTextBlock();

};
