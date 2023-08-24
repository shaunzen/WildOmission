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
	
	UFUNCTION(BlueprintCallable)
	void OpenMainMenu();
	UFUNCTION(BlueprintCallable)
	void OpenWorldSelectionMenu();
	UFUNCTION(BlueprintCallable)
	void CreateDemoWorld();
	UFUNCTION(BlueprintCallable)
	void OpenWorldMenu();
	UFUNCTION(BlueprintCallable)
	void OpenWorldMenuForWorld(const FString& WorldName);
	UFUNCTION(BlueprintCallable)
	void OpenOptionsMenu();
	UFUNCTION(BlueprintCallable)
	void OpenCreditsMenu();
	UFUNCTION(BlueprintCallable)
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
	UButton* CreditsButton;
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
	UPROPERTY(Meta = (BindWidget))
	UWidget* CreditsMenu;

	IMenuInterface* MenuInterface;

};
