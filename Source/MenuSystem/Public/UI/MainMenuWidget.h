// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/MenuInterface.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class UWorldSelectionWidget;
class UWorldMenuWidget;
class UOptionsWidget;

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
	void CreateDemoWorld();
	UFUNCTION()
	void OpenWorldMenu();
	UFUNCTION()
	void OpenWorldMenuForWorld(const FString& WorldName);
	UFUNCTION()
	void OpenOptionsMenu();
	UFUNCTION()
	void ExitGame();

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
	UButton* ExitButton;

	UPROPERTY(Meta = (BindWidget))
	UWidget* MainMenu;
	UPROPERTY(Meta = (BindWidget))
	UWorldSelectionWidget* WorldSelectionMenu;
	UPROPERTY(Meta = (BindWidget))
	UWorldMenuWidget* WorldMenu;
	UPROPERTY(Meta = (BindWidget))
	UOptionsWidget* OptionsMenu;

	IMenuInterface* MenuInterface;

};
