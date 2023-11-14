// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/ServerData.h"
#include "Interfaces/MenuInterface.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class UWorldSelectionWidget;
class UWorldCreationWidget;
class UWorldMenuWidget;
class UServerBrowserWidget;
class UOptionsWidget;
class UErrorMessagePrompt;

UCLASS()
class MENUSYSTEM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OpenMainMenu();
	UFUNCTION()
	void OpenWorldSelectionMenu();
	UFUNCTION()
	void OpenWorldCreationMenu();
	UFUNCTION()
	void OpenWorldMenu();
	UFUNCTION()
	void OpenWorldMenuForWorld(const FString& WorldName);
	UFUNCTION()
	void OpenRenameWorldMenu();
	UFUNCTION()
	void OpenDeleteWorldMenu();
	UFUNCTION()
	void OpenServerBrowserMenu();
	UFUNCTION()
	void OpenOptionsMenu();
	UFUNCTION()
	void OpenFeedbackPage();
	UFUNCTION()
	void ExitGame();
	UFUNCTION()
	void OpenErrorPrompt(const FString& Title, const FString& Error);

	void SetServerList(TArray<FServerData> InServerData);

	void Setup(IMenuInterface* InMenuInterface);
	void Teardown();

	IMenuInterface* GetMenuInterface() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	UButton* PlayButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* OptionsButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* FeedbackButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(Meta = (BindWidget))
	UWidget* MainMenu;
	UPROPERTY(Meta = (BindWidget))
	UWorldSelectionWidget* WorldSelectionMenu;
	UPROPERTY(Meta = (BindWidget))
	UWorldCreationWidget* WorldCreationMenu;
	UPROPERTY(Meta = (BindWidget))
	UWorldMenuWidget* WorldMenu;
	UPROPERTY(Meta = (BindWidget))
	class URenameWorldWidget* RenameWorldMenu;
	UPROPERTY(Meta = (BindWidget))
	class UDeleteWorldWidget* DeleteWorldMenu;
	UPROPERTY(Meta = (BindWidget))
	UServerBrowserWidget* ServerBrowserMenu;
	UPROPERTY(Meta = (BindWidget))
	UOptionsWidget* OptionsMenu;
	UPROPERTY(Meta = (BindWidget))
	UErrorMessagePrompt* ErrorMessagePrompt;

	IMenuInterface* MenuInterface;

};
