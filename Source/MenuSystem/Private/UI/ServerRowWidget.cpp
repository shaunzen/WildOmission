// Copyright Telephone Studios. All Rights Reserved.


#include "ServerRowWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Structs/ServerData.h"
#include "Color/UIColors.h"

UServerRowWidget::UServerRowWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	RowButton = nullptr;
	ServerNameTextBlock = nullptr;
	HostNameTextBlock = nullptr;
	ConnectionFractionTextBlock = nullptr;
	
	Index = INDEX_NONE;

	Selected = false;
	Hovering = false;
}

void UServerRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RowButton->OnClicked.AddDynamic(this, &UServerRowWidget::OnClicked);
}

void UServerRowWidget::Setup(const uint32& InIndex, const FServerData& ServerData)
{
	Index = InIndex;

	const FString HostString = ServerData.IsDedicated ? FString::Printf(TEXT("Ping: %i"), ServerData.PingMS) : FString::Printf(TEXT("Host: %s"), *ServerData.HostUsername);
	const FString FractionString = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
	
	ServerNameTextBlock->SetText(FText::FromString(ServerData.Name));
	HostNameTextBlock->SetText(FText::FromString(HostString));
	ConnectionFractionTextBlock->SetText(FText::FromString(FractionString));
}

void UServerRowWidget::SetSelected(bool NewSelected)
{
	Selected = NewSelected;
}

void UServerRowWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FUIColor* Blue = UUIColors::GetBaseColor(TEXT("Blue"));
	FUIColor* LightGray= UUIColors::GetBaseColor(TEXT("LightGray"));
	if (Blue == nullptr || LightGray == nullptr)
	{
		return;
	}

	FLinearColor ButtonColor = Selected ? Blue->Default : LightGray->Default;
	RowButton->SetBackgroundColor(ButtonColor * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
}

void UServerRowWidget::OnClicked()
{
	if (!OnSelected.IsBound())
	{
		return;
	}

	OnSelected.Broadcast(Index);
}