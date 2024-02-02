// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreditsWidgetOnBackButtonClickedSignature);

UCLASS()
class UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCreditsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	FCreditsWidgetOnBackButtonClickedSignature OnBackButtonClicked;

private:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* AboutTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* BackButton;

	UFUNCTION()
	void BroadcastOnBackButtonClicked();

};
