// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/MenuInterface.h"
#include "Interfaces/GameSettingsInterface.h"
#include "Interfaces/GameSaveLoadController.h"
#include "WildOmissionGameInstance.generated.h"

class UMainMenuWidget;
class UGameplayMenuWidget;
class ULoadingMenuWidget;

UCLASS()
class WILDOMISSIONCORE_API UWildOmissionGameInstance : public UGameInstance, public IMenuInterface, public IGameSettingsInterface, public IGameSaveLoadController
{
	GENERATED_BODY()
public:
	UWildOmissionGameInstance(const FObjectInitializer& ObjectInitializer);
	static UWildOmissionGameInstance* GetWildOmissionGameInstance(UWorld* WorldContextObject);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void ShowMainMenuWidget();

	UFUNCTION(BlueprintCallable, Exec)
	void ShowGameplayMenuWidget();

	// Begin IGameSaveLoadController Implementation
	virtual void StartLoading() override;
	virtual void StopLoading() override;
	virtual void SetLoadingTitle(const FString& InLoadingTitle) override;
	virtual void SetLoadingSubtitle(const FString& InLoadingSubtitle) override;
	virtual void CreateWorld(const FString& NewWorldName) override;
	// End IGameSaveLoadController Implementation

	// Begin IMenuInterface Implementation
	virtual void StartSingleplayer(const FString& WorldName) override;
	virtual void QuitToMenu() override;
	// End IMenuInterface Implementation

	// Begin IGameSettingsInterface Implementation
	virtual void ApplyMasterVolume() override;
	// End IGameSettingsInterface Implementation

	UFUNCTION(BlueprintCallable)
	FString GetVersion() const;

	UMainMenuWidget* GetMainMenuWidget() const;
	UGameplayMenuWidget* GetGameplayMenuWidget() const;

private:
	UPROPERTY()
	TSubclassOf<UMainMenuWidget> MainMenuWidgetBlueprintClass;
	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

	UPROPERTY()
	TSubclassOf<UGameplayMenuWidget> GameplayMenuWidgetBlueprintClass;

	UPROPERTY()
	UGameplayMenuWidget* GameplayMenuWidget;
	UFUNCTION()
	void ClearGameplayMenuWidget();

	// Will try to auto-configure graphics settings if first launch of game
	UFUNCTION()
	void RunAutoConfigQualitySettings();

	UPROPERTY()
	TSubclassOf<ULoadingMenuWidget> LoadingMenuWidgetBlueprintClass;
	UPROPERTY()
	ULoadingMenuWidget* LoadingMenuWidget;

	FString WorldToLoad;

	UFUNCTION()
	void LoadedNewMap(UWorld* InWorld);

	bool OnMainMenu;
	bool Loading;
	FString LoadingTitle;
	FString LoadingSubtitle;

};
