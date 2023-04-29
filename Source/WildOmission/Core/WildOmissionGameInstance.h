// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WildOmissionGameInstance.generated.h"

class UMainMenuWidget;
class UGameplayMenuWidget;
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

	UFUNCTION()
	void StartSingleplayer(const FString& WorldName);

	UFUNCTION()
	void Host(const FString& ServerName, const FString& WorldName);
	
	UFUNCTION()
	void Join(const uint32& Index);

	void StartSession();

	void QuitToMenu();

	void RefreshServerList();

	static TArray<FString> GetAllWorldNames();
	void CreateWorld(const FString& NewWorldName);

	UFUNCTION(BlueprintCallable)
	FString GetVersion() const;

private:

	TSubclassOf<UMainMenuWidget> MainMenuWidgetBlueprintClass;
	UMainMenuWidget* MainMenuWidget;

	TSubclassOf<UGameplayMenuWidget> GameplayMenuWidgetBlueprintClass;
	UGameplayMenuWidget* GameplayMenuWidget;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	FString Version;

	FString DesiredServerName;
	FString WorldToLoad;
	UFUNCTION()
	void CreateSession(FName SessionName = FName(""), bool Success = true);

	void EndExistingSession();

	bool OnMainMenu;

};
