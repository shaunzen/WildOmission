// Copyright Telephone Studios. All Rights Reserved.


#include "UI/PlayerRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Log.h"

UPlayerRowWidget::UPlayerRowWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	NameTextBlock = nullptr;
	ViewProfileButton = nullptr;
	KickButton = nullptr;
	UniqueID = TEXT("");
}

void UPlayerRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ViewProfileButton)
	{
		ViewProfileButton->OnClicked.AddDynamic(this, &UPlayerRowWidget::ViewProfile);
	}

	if (KickButton)
	{
		KickButton->OnClicked.AddDynamic(this, &UPlayerRowWidget::Kick);
	}
}

void UPlayerRowWidget::Setup(const FString& Name, const FString& InUniqueID)
{
	NameTextBlock->SetText(FText::FromString(Name));
	UniqueID = InUniqueID;
}

void UPlayerRowWidget::ViewProfile()
{
	const FString ProfileURL = FString::Printf(TEXT("https://steamcommunity.com/profiles/%s/"), *UniqueID);
	UKismetSystemLibrary::LaunchURL(ProfileURL);
}

void UPlayerRowWidget::Kick()
{
	UE_LOG(LogMenuSystem, Display, TEXT("Kicking player %s."), *UniqueID);

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	AGameModeBase* GameMode = World->GetAuthGameMode();
	if (GameMode == nullptr)
	{
		return;
	}

	AGameSession* GameSession = GameMode->GameSession.Get();
	if (GameSession == nullptr)
	{
		return;
	}
	APlayerController* PlayerControllerToKick = GetPlayerControllerForThis();
	if (PlayerControllerToKick == nullptr)
	{
		UE_LOG(LogMenuSystem, Warning, TEXT("Failed to kick player, couldn't get player controller."));
		return;
	}

	GameSession->KickPlayer(PlayerControllerToKick, FText::FromString(TEXT("Kicked by Host.")));
}

APlayerController* UPlayerRowWidget::GetPlayerControllerForThis() const
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return nullptr;
	}

	AGameStateBase* GameState = World->GetGameState();
	if (GameState == nullptr)
	{
		return nullptr;
	}

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (PlayerState == nullptr || PlayerState->GetUniqueId().ToString() != UniqueID)
		{
			continue;
		}

		return PlayerState->GetPlayerController();
	}

	return nullptr;
}
