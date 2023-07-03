// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldCreationWidget.generated.h"

class UButton;
class UEditableTextBox;
class UMainMenuWidget;

UCLASS()
class WILDOMISSIONCORE_API UWorldCreationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(UMainMenuWidget* InMainMenuParent);

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* CreateWorldButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* WorldNameInputBox;

	UPROPERTY()
	UMainMenuWidget* ParentMenu;

	UFUNCTION()
	void CreateWorld();

	UFUNCTION()
	void WorldNameOnTextChanged(const FText& Text);

};
