// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldCreationWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenWorldMenuRequestedSignature, const FString&, WorldName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWorldCreationOnBackClickedSignature);

UCLASS()
class UWorldCreationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWorldCreationWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	FOnOpenWorldMenuRequestedSignature OnOpenWorldMenuRequested;
	FWorldCreationOnBackClickedSignature OnBackClicked;

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* CreateWorldButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* BackButton;
	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* WorldNameInputBox;

	UPROPERTY(Meta = (BindWidget))
	UWidget* InvalidWarningBorder;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* InvalidWarningTextBlock;

	UFUNCTION()
	void ShowInvalidWarning(const FString& Warning);

	UFUNCTION()
	void HideInvalidWarning();

	bool WorldOfSameNameAlreadyExists(const FString& WorldName);
	bool WorldContainsInvalidCharacter(const FString& WorldName);

	UFUNCTION()
	void CreateWorld();

	UFUNCTION()
	void BroadcastOnBackClicked();

	UFUNCTION()
	void WorldNameOnTextChanged(const FText& Text);

};
