// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveRowWidget.generated.h"

class UTextBlock;
class UButton;
class UMainMenuWidget;

UCLASS()
class WILDOMISSION_API USaveRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DateCreated;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DaysPlayed;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(UMainMenuWidget* InParent, uint32 InIndex);

private:
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;

	UPROPERTY()
	UMainMenuWidget* Parent;

	uint32 Index;

	UFUNCTION()
	void OnClicked();

};
