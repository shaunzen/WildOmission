// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

class UVerticalBox;
class UButton;
class USliderOptionBox;
class UWidgetSwitcher;
class USettingsCategoryWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOptionsOnBackButtonPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFieldOfViewChangedSignature);

UCLASS()
class GAMESETTINGS_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UOptionsWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	void Refresh();

	FOptionsOnBackButtonPressedSignature OnBackButtonPressed;
	FOnFieldOfViewChangedSignature OnFieldOfViewChanged;
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UVerticalBox* CategoryButtonsVerticalBox;
	UPROPERTY(Meta = (BindWidget))
	UButton* GameplaySettingsButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* WindowSettingsButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* GraphicsSettingsButton;

	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* CategorySwitcher;

	UPROPERTY(Meta= (BindWidget))
	USettingsCategoryWidget* GameplaySettings;
	UPROPERTY(Meta = (BindWidget))
	USettingsCategoryWidget* WindowSettings;
	UPROPERTY(Meta = (BindWidget))
	USettingsCategoryWidget* GraphicsSettings;


	UPROPERTY(Meta = (BindWidget))
	UButton* ApplyButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* ResetButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;

	UFUNCTION()
	void OpenGameplaySettings();
	UFUNCTION()
	void OpenWindowSettings();
	UFUNCTION()
	void OpenGraphicsSettings();

	void RefreshAllCategoryButtons();
	void RefreshCategoryButtonColor(UButton* ButtonToRefresh);

	UFUNCTION()
	void Apply();

	UFUNCTION()
	void Reset();

	UFUNCTION()
	void Back();

};
