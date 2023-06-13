// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheckOptionBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckStateChangedSignature, bool, Checked);

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class WILDOMISSION_API UCheckOptionBox : public UUserWidget
{
	GENERATED_BODY()
public:
	UCheckOptionBox(const FObjectInitializer& ObjectInitializer);

	void Setup(const FString& OptionName, bool CheckedByDefault = false);

	FOnCheckStateChangedSignature OnCheckedStateChanged;

	UFUNCTION(BlueprintCallable)
	bool IsChecked() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* Button;
	UPROPERTY(Meta = (BindWidget))
	UImage* Image;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextBlock;

	UPROPERTY()
	bool Checked;

	void RefreshState();

	UFUNCTION()
	void OnClicked();

};
