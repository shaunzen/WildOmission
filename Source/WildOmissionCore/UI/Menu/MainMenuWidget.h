// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WildOmissionCore/Structs/ServerData.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class UWorldSelectionWidget;
class UWorldCreationWidget;
class UWorldMenuWidget;
class UServerBrowserWidget;
class UOptionsWidget;

UCLASS()
class WILDOMISSIONCORE_API UMainMenuWidget : public UUserWidget
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
	void OpenServerBrowserMenu();
	UFUNCTION()
	void OpenOptionsMenu();
	UFUNCTION()
	void ExitGame();

	void SetServerList(TArray<FServerData> InServerData);

	void Setup();
	void Teardown();

private:
	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	UButton* PlayButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* OptionsButton;
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
	UServerBrowserWidget* ServerBrowserMenu;
	UPROPERTY(Meta = (BindWidget))
	UOptionsWidget* OptionsMenu;

};
