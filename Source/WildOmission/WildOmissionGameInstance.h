// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WildOmissionGameInstance.generated.h"

class UMainMenuWidget;
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

	UFUNCTION()
	void Host(FString ServerName);
	
	UFUNCTION()
	void Join(uint32 Index);

	void StartSession();

	void RefreshServerList();
private:

	TSubclassOf<UMainMenuWidget> MainMenuWidgetBlueprintClass;
	UMainMenuWidget* MainMenuWidget;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	FString DesiredServerName;
	void CreateSession();
};
