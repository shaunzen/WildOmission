// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldRowWidget.generated.h"

class UTextBlock;
class UButton;
class UWorldSelectionWidget;

UCLASS()
class WILDOMISSION_API UWorldRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WorldNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DateCreated;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DaysPlayed;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(UWorldSelectionWidget* InParent, const FString& InWorldName);
	
	FString GetWorldName();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;

	UPROPERTY()
	UWorldSelectionWidget* Parent;

	FString WorldName;

	UFUNCTION()
	void OnClicked();

};
