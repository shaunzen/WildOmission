// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IndexedButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIndexedButtonSelectedSignature, const int32&, SelectedIndex);

UCLASS()
class CUSTOMUI_API UIndexedButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UIndexedButton(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	void Setup(const int32& InIndex, const FString& InLabel);
	FOnIndexedButtonSelectedSignature OnSelected;
	
private:
	UPROPERTY(Meta = (BindWidget))
	UButton* Button;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ButtonText;

	int32 Index;

	UFUNCTION()
	void OnClicked();

};
