// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathMenuWidget.generated.h"

class UButton;

UCLASS()
class WILDOMISSIONCORE_API UDeathMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDeathMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Show();

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* RespawnButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* QuitButton;

	UFUNCTION()
	void RespawnButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

	void Teardown();

};
