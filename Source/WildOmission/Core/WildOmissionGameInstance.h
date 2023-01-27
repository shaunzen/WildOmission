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
	void StartSingleplayer(FString SaveName);

	UFUNCTION()
	void Host(FString ServerName, FString SaveName);
	
	UFUNCTION()
	void Join(uint32 Index);

	void StartSession();

	void RefreshServerList();

	static TArray<FString> GetAllSaveGameSlotNames();
	void CreateSave(const FString& NewSaveName);
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

	FString DesiredServerName;
	FString SaveToLoad;
	void CreateSession();

	bool OnMainMenu;

};
