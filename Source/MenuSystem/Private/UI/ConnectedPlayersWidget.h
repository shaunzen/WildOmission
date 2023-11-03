// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConnectedPlayersWidget.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConnectedPlayersOnBackButtonClickedSignature);

UCLASS()
class UConnectedPlayersWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UConnectedPlayersWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	void Refresh();

	FConnectedPlayersOnBackButtonClickedSignature OnBackButtonClicked;

private:
	UPROPERTY(Meta = (BindWidget))
	UWidget* ConnectedPlayersPanel;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* CloseButton;

	UFUNCTION()
	void Back();

};
