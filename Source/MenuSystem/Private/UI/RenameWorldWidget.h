// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RenameWorldWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRenameWorldOnRenameButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRenameWorldOnCancelButtonClickedSignature);

UCLASS()
class URenameWorldWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	URenameWorldWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	FRenameWorldOnRenameButtonClickedSignature OnRenameButtonClicked;
	FRenameWorldOnCancelButtonClickedSignature OnCancelButtonClicked;

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* RenameButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* CancelButton;

	UFUNCTION()
	void BroadcastRenameButtonClicked();

	UFUNCTION()
	void BroadcastCancelButtonClicked();

};
