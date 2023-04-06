// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathMenuWidget.generated.h"

class UButton;

UCLASS()
class WILDOMISSION_API UDeathMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDeathMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

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
