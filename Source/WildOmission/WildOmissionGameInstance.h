// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Interfaces/MainMenuInterface.h"
#include "WildOmissionGameInstance.generated.h"

class UUserWidget;
class UMainMenuWidget;

UCLASS()
class WILDOMISSION_API UWildOmissionGameInstance : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()
public:
	UWildOmissionGameInstance(const FObjectInitializer& ObjectIntializer);
	virtual void Init();
	
	UFUNCTION(BlueprintCallable)
	void LoadMainMenuWidget();
	
	virtual void Host(FString ServerName) override;
	virtual void Join(uint32 Index) override;
	
	void StartSession();
	
	virtual void LoadMainMenu() override;

	virtual void RefreshServerList() override;
private:

	TSubclassOf<UUserWidget> MainMenuWidgetClass;
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
