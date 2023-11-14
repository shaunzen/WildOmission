// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaySelectedWorldButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRenameButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeleteButtonClickedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateNewWorldButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMultiplayerButtonClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelButtonClickedSignature);

UCLASS()
class UWorldSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWorldSelectionWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetWorldList(const TArray<FString>& WorldNames);
	void SetSelectedWorld(const FString& WorldName);

	TOptional<FString> SelectedWorldName;

private:
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* WorldListBox;

	UPROPERTY(Meta = (BindWidget))
	class UButton* PlaySelectedButton;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* RenameWorldButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* DeleteWorldButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* CreateNewWorldButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MultiplayerButton;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* CancelButton;

	TSubclassOf<class UWorldRowWidget> WorldRowWidgetClass;
	
	void UpdateListChildren();

	TArray<class UWildOmissionSaveGame*> GetWorldsSortedByLastPlayed(const TArray<FString>& NameList);
	static bool IsSaveMoreRecentlyPlayed(class UWildOmissionSaveGame* SaveA, class UWildOmissionSaveGame* SaveB);

	UFUNCTION()
	void OnPlaySelectedWorldClicked();

	UFUNCTION()
	void OnRenameWorldClicked();

	UFUNCTION()
	void OnDeleteWorldClicked();

	UFUNCTION()
	void OnCreateNewWorldClicked();

	UFUNCTION()
	void OnMultiplayerButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();

};
