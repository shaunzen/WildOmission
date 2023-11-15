// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeleteWorldWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeleteWorldOnDeleteButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeleteWorldOnCancelButtonClickedSignature);

UCLASS()
class UDeleteWorldWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDeleteWorldWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	FDeleteWorldOnDeleteButtonClickedSignature OnDeleteButtonClicked;
	FDeleteWorldOnCancelButtonClickedSignature OnCancelButtonClicked;

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* DeleteButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* CancelButton;

	UFUNCTION()
	void BroadcastDeleteButtonClicked();

	UFUNCTION()
	void BroadcastCancelButtonClicked();

};
