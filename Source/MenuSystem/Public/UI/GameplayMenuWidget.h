// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/MenuInterface.h"
#include "GameplayMenuWidget.generated.h"

class UTextBlock;
class UButton;
class UWidgetSwitcher;
class UOptionsWidget;

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
	
	UFUNCTION(BlueprintCallable)
	bool IsOpen() const;

	IMenuInterface* GetMenuInterface() const;

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

	IMenuInterface* MenuInterface;

	bool bOpen;

	UFUNCTION()
	void Save();

	UFUNCTION()
	void QuitToMenu();

	void SetQuitButtonText(bool PlayerHasAuthority);

};
