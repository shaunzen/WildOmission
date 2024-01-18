// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheckOptionBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckOptionBoxValueChangedSignature, bool, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckOptionBoxValueChangedNoParamsSignature);

UCLASS()
class CUSTOMUI_API UCheckOptionBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UCheckOptionBox(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	void SetChecked(bool IsChecked);

	FOnCheckOptionBoxValueChangedSignature OnValueChanged;
	FOnCheckOptionBoxValueChangedNoParamsSignature OnValueChangedNoParams;

	UFUNCTION(BlueprintCallable)
	bool IsChecked() const;

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* Button;
	UPROPERTY(Meta = (BindWidget))
	class UImage* Image;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TextBlock;

	UPROPERTY()
	bool Checked;

	void RefreshState();

	UFUNCTION()
	void OnClicked();

};
