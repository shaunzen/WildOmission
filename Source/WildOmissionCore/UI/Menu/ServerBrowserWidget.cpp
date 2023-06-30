// Copyright Telephone Studios. All Rights Reserved.


#include "ServerBrowserWidget.h"
#include "ServerRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "MainMenuWidget.h"
#include "WildOmissionCore/WildOmissionGameInstance.h"

UServerBrowserWidget::UServerBrowserWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UServerRowWidget> ServerRowWidgetBPClass(TEXT("/Game/WildOmission/UI/Menu/WBP_ServerRow"));
	if (ServerRowWidgetBPClass.Succeeded())
	{
		ServerRowWidgetClass = ServerRowWidgetBPClass.Class;
	}
}

void UServerBrowserWidget::Setup(UMainMenuWidget* InMainMenuParent)
{
	ParentMenu = InMainMenuParent;

	JoinButton->OnClicked.AddDynamic(this, &UServerBrowserWidget::JoinServer);
	RefreshListButton->OnClicked.AddDynamic(this, &UServerBrowserWidget::RefreshList);
	BackButton->OnClicked.AddDynamic(ParentMenu, &UMainMenuWidget::OpenWorldSelectionMenu);
}

void UServerBrowserWidget::Open()
{
	RefreshList();
}

void UServerBrowserWidget::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRowWidget* Row = CreateWidget<UServerRowWidget>(World, ServerRowWidgetClass);
		if (Row == nullptr)
		{
			return;
		}

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		FString HostString = FString::Printf(TEXT("Host: %s"), *ServerData.HostUsername);
		Row->HostUser->SetText(FText::FromString(HostString));
		FString FractionString = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionString));
		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}

	FString RefreshString = FString("Refresh");
	RefreshListButtonText->SetText(FText::FromString(RefreshString));
}

void UServerBrowserWidget::SelectServerIndex(uint32 Index)
{
	SelectedServerIndex = Index;
	UpdateServerListChildren();
}

void UServerBrowserWidget::UpdateServerListChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRowWidget* Row = Cast<UServerRowWidget>(ServerList->GetChildAt(i));
		if (Row == nullptr)
		{
			return;
		}
		Row->Selected = (SelectedServerIndex.IsSet() && SelectedServerIndex.GetValue() == i);
	}
}

void UServerBrowserWidget::RefreshList()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());

	if (GameInstance == nullptr)
	{
		return;
	}

	FString WaitingString = FString("...");
	RefreshListButtonText->SetText(FText::FromString(WaitingString));

	GameInstance->RefreshServerList();
}

void UServerBrowserWidget::JoinServer()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());

	if (GameInstance == nullptr || SelectedServerIndex.IsSet() == false)
	{
		return;
	}

	GameInstance->Join(SelectedServerIndex.GetValue());
}