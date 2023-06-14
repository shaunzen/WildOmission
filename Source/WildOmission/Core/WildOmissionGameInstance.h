// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WildOmissionGameInstance.generated.h"

class UMainMenuWidget;
class UGameplayMenuWidget;
class ULoadingMenuWidget;
class FOnlineSessionSearch;

UCLASS()
class WILDOMISSION_API UWildOmissionGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UWildOmissionGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void ShowMainMenuWidget();

	UFUNCTION(BlueprintCallable, Exec)
	void ShowGameplayMenuWidget();

	UFUNCTION(BlueprintCallable, Exec)
	void StartLoading();

	UFUNCTION(BlueprintCallable)
	void StopLoading();

	UFUNCTION(BlueprintCallable)
	void SetLoadingTitle(const FString& InLoadingTitle);

	UFUNCTION(BlueprintCallable)
	void SetLoadingSubtitle(const FString& InLoadingSubtitle);

	//UFUNCTION(BlueprintCallable)
	//void SetLoadingMenuTitle(const FString& InTitle);
	//
	//UFUNCTION(BlueprintCallable)
	//void SetLoadingStep(const FString& InStepName, const int32& InStepPercent = -1);

	UFUNCTION()
	void StartSingleplayer(const FString& WorldName);

	UFUNCTION()
	void Host(const FString& ServerName, const FString& WorldName, bool FriendsOnly = false);
	
	UFUNCTION()
	void Join(const uint32& Index);

	void StartSession();

	void QuitToMenu();

	void RefreshServerList();

	void RefreshMasterVolume();

	static TArray<FString> GetAllWorldNames();
	void CreateWorld(const FString& NewWorldName);

	UFUNCTION(BlueprintCallable)
	FString GetVersion() const;

private:
	UPROPERTY()
	TSubclassOf<UMainMenuWidget> MainMenuWidgetBlueprintClass;
	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

	UPROPERTY()
	TSubclassOf<UGameplayMenuWidget> GameplayMenuWidgetBlueprintClass;
	UPROPERTY()
	UGameplayMenuWidget* GameplayMenuWidget;
	
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
