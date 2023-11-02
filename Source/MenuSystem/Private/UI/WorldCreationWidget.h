// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldCreationWidget.generated.h"

class UButton;
class UEditableTextBox;
class UTextBlock;
class UWidget;
class UMainMenuWidget;

UCLASS()
class UWorldCreationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWorldCreationWidget(const FObjectInitializer& ObjectInitializer);

	void Setup(UMainMenuWidget* InMainMenuParent);

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* CreateWorldButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* WorldNameInputBox;

	UPROPERTY(Meta = (BindWidget))
	UWidget* InvalidWarningBorder;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* InvalidWarningTextBlock;

	UPROPERTY()
	UMainMenuWidget* ParentMenu;

	UFUNCTION()
	void ShowInvalidWarning(const FString& Warning);

	UFUNCTION()
	void HideInvalidWarning();

	bool WorldOfSameNameAlreadyExists(const FString& WorldName);
	bool WorldContainsInvalidCharacter(const FString& WorldName);

	UFUNCTION()
	void CreateWorld();

	UFUNCTION()
	void WorldNameOnTextChanged(const FText& Text);

};
