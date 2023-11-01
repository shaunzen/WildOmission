// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/MenuInterface.h"
#include "Interfaces/GameSettingsInterface.h"
#include "Interfaces/GameSaveLoadController.h"
#include "WildOmissionGameInstance.generated.h"

class UMainMenuWidget;
class UGameplayMenuWidget;
class ULoadingMenuWidget;
class FOnlineSessionSearch;

USTRUCT()
struct FGameError
{
	GENERATED_BODY()

	bool Acknowlaged;
	FString Title;
	FString Message;

	FGameError();
	void Clear();

};

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
	virtual void Host(const FString& ServerName, const FString& WorldName, bool FriendsOnly = false) override;
	virtual void Join(const uint32& Index) override;
	virtual void RefreshServerList() override;
	virtual void QuitToMenu() override;
	// End IMenuInterface Implementation

	void StartSession();

	// Begin IGameSettingsInterface Implementation
	virtual void ApplyAudioSettings() override;
	// End IGameSettingsInterface Implementation

	IOnlineFriendsPtr GetFriendsInterface() const;

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
	UFUNCTION(Exec)
	void RunAutoConfigQualitySettings(bool Override = false);

	FGameError LastError;

	UPROPERTY()
	TSubclassOf<ULoadingMenuWidget> LoadingMenuWidgetBlueprintClass;
	UPROPERTY()
	ULoadingMenuWidget* LoadingMenuWidget;
	
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	IOnlineFriendsPtr FriendsInterface;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	FString DesiredServerName;
	FString WorldToLoad;
	bool FriendsOnlySession;

	UFUNCTION()
	void CreateSession(FName SessionName = FName(""), bool Success = true);

	UFUNCTION()
	void LoadedNewMap(UWorld* InWorld);

	void EndExistingSession();

	bool OnMainMenu;
	bool Loading;
	FString LoadingTitle;
	FString LoadingSubtitle;

};
