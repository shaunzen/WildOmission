// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsOnBackButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFieldOfViewChangedSignature);

UCLASS()
class GAMESETTINGS_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOptionsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	void Refresh();

	FOptionsOnBackButtonClickedSignature OnBackButtonClicked;
	FOnFieldOfViewChangedSignature OnFieldOfViewChanged;

private:
	UPROPERTY(Meta = (BindWidget))
	class UVerticalBox* CategoryButtonsVerticalBox;
	UPROPERTY(Meta = (BindWidget))
	class UButton* AudioSettingsButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* GameplaySettingsButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* ControlsSettingsButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* WindowSettingsButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* PostProcessingSettingsButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* GraphicsSettingsButton;

	UPROPERTY(Meta = (BindWidget))
	class UWidgetSwitcher* CategorySwitcher;

	UPROPERTY(Meta = (BindWidget))
	class USettingsCategoryWidget* AudioSettings;
	UPROPERTY(Meta = (BindWidget))
	class USettingsCategoryWidget* GameplaySettings;
	UPROPERTY(Meta = (BindWidget))
	class USettingsCategoryWidget* ControlsSettings;
	UPROPERTY(Meta = (BindWidget))
	class USettingsCategoryWidget* WindowSettings;
	UPROPERTY(Meta = (BindWidget))
	class USettingsCategoryWidget* PostProcessingSettings;
	UPROPERTY(Meta = (BindWidget))
	class USettingsCategoryWidget* GraphicsSettings;

	UPROPERTY(Meta = (BindWidget))
	class UButton* ResetButton;
	UPROPERTY(Meta = (BindWidget))
	class UButton* BackButton;

	UFUNCTION()
	void OpenAudioSettings();
	UFUNCTION()
	void OpenGameplaySettings();
	UFUNCTION()
	void OpenControlsSettings();
	UFUNCTION()
	void OpenWindowSettings();
	UFUNCTION()
	void OpenPostProcessingSettings();
	UFUNCTION()
	void OpenGraphicsSettings();

	void RefreshCategoryButtonColor(class UButton* ButtonToRefresh);

	UFUNCTION()
	void Apply();

	UFUNCTION()
	void Reset();

	UFUNCTION()
	void Back();

};
