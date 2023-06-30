// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayMenuWidget.generated.h"

class UTextBlock;
class UButton;
class UWidgetSwitcher;
class UOptionsWidget;

UCLASS()
class WILDOMISSIONCORE_API UGameplayMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameplayMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void Show();
	
	UFUNCTION()
	void OpenGameMenu();
	
	UFUNCTION()
	void OpenOptionsMenu();
	
	UFUNCTION(BlueprintCallable)
	bool IsOpen() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* QuitButtonText;
	
	UPROPERTY(Meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	UWidget* GameMenu;

	UPROPERTY(Meta = (BindWidget))
	UOptionsWidget* OptionsMenu;

	bool bOpen;

	UFUNCTION()
	void Teardown();

	UFUNCTION()
	void Save();

	UFUNCTION()
	void QuitToMenu();

	void SetQuitButtonText(bool PlayerHasAuthority);

};
