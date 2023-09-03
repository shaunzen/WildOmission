// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyOptionBox.generated.h"

class UButton;
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
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* Button;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock;

	FKey SelectedKey;
	bool AwaitingInput;

	void RefreshTextBlock();

	UFUNCTION()
	void OnClicked();
};
