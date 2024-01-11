// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/MenuInterface.h"
#include "GameplayMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameplayMenuClosedSignature);

UCLASS()
class MENUSYSTEM_API UGameplayMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameplayMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintCallable)
	void Show();
	
	UFUNCTION(BlueprintCallable)
	void Teardown();

	void SetMenuInterface(IMenuInterface* InMenuInterface);

	UFUNCTION()
	void OpenGameMenu();
	
	FOnGameplayMenuClosedSignature OnClosed;

	UFUNCTION()
	void OpenOptionsMenu();

	UFUNCTION()
	void OpenConnectedPlayersMenu();
	
	UFUNCTION(BlueprintCallable)
	bool IsOpen() const;

	IMenuInterface* GetMenuInterface() const;

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* ResumeButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* OptionsButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* ConnectedPlayersButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* HowToPlayButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* FeedbackButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* QuitButtonText;
	
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* SavingTextBlock;

	UPROPERTY(Meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	UWidget* GameMenu;

	UPROPERTY(Meta = (BindWidget))
	class UOptionsWidget* OptionsMenu;

	UPROPERTY(Meta = (BindWidget))
	class UConnectedPlayersWidget* ConnectedPlayersMenu;

	IMenuInterface* MenuInterface;

	bool bOpen;

	UFUNCTION()
	void OpenHelpGuide();

	UFUNCTION()
	void OpenFeedbackPage();

	UFUNCTION()
	void Save();

	UFUNCTION()
	void QuitToMenu();

	UFUNCTION()
	void SetupSavingText(bool PlayerHasAuthority);

	UFUNCTION()
	void HideSavingText();

	void SetQuitButtonText(bool PlayerHasAuthority);

};
