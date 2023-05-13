// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldSelectionWidget.generated.h"

class UButton;
class UWorldRowWidget;
class UWildOmissionSaveGame;

UCLASS()
class WILDOMISSION_API UWorldSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UWorldSelectionWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetWorldList(const TArray<FString>& WorldNames);
	void SetSelectedWorld(const FString& WorldName);

private:
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* WorldListBox;

	UPROPERTY(Meta = (BindWidget))
	UButton* SelectButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* BrowseServersButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;

	TSubclassOf<UWorldRowWidget> WorldRowWidgetClass;
	TSubclassOf<UCreateWorldButtonWidget> CreateNewWorldButtonClass;

	TOptional<FString> SelectedWorldName;

	void UpdateListChildren();

	TArray<UWildOmissionSaveGame*> GetWorldsSortedByLastPlayed(const TArray<FString>& NameList);
	static bool IsSaveMoreRecentlyPlayed(UWildOmissionSaveGame* SaveA, UWildOmissionSaveGame* SaveB);

};
