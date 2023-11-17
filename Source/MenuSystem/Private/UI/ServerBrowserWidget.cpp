// Copyright Telephone Studios. All Rights Reserved.


#include "ServerBrowserWidget.h"
#include "ServerRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "UI/MainMenuWidget.h"

UServerBrowserWidget::UServerBrowserWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ServerList = nullptr;
	JoinButton = nullptr;
	BackButton = nullptr;
	RefreshListButton = nullptr;
	RefreshListButtonText = nullptr;
	ParentMenu = nullptr;
	ServerRowWidgetClass = nullptr;

	static ConstructorHelpers::FClassFinder<UServerRowWidget> ServerRowWidgetBPClass(TEXT("/Game/MenuSystem/UI/Server/WBP_ServerRow"));
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
		Row->Setup(i, ServerData);
		Row->OnSelected.AddDynamic(this, &UServerBrowserWidget::SelectServerIndex);
		++i;

		ServerList->AddChild(Row);
	}

	FString RefreshString = TEXT("Refresh");
	RefreshListButtonText->SetText(FText::FromString(RefreshString));
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
		Row->SetSelected(SelectedServerIndex.IsSet() && SelectedServerIndex.GetValue() == i);
	}
}

void UServerBrowserWidget::SelectServerIndex(const uint32& SelectedIndex)
{
	SelectedServerIndex = SelectedIndex;
	UpdateServerListChildren();
}

void UServerBrowserWidget::RefreshList()
{
	IMenuInterface* MenuInterface = ParentMenu->GetMenuInterface();
	if (MenuInterface == nullptr)
	{
		return;
	}

	FString WaitingString = FString("...");
	RefreshListButtonText->SetText(FText::FromString(WaitingString));

	MenuInterface->RefreshServerList();
}

void UServerBrowserWidget::JoinServer()
{
	IMenuInterface* MenuInterface = ParentMenu->GetMenuInterface();
	if (MenuInterface == nullptr || SelectedServerIndex.IsSet() == false)
	{
		return;
	}

	MenuInterface->Join(SelectedServerIndex.GetValue());
}