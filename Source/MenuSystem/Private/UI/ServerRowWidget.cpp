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
	RowBorder = nullptr;
	ServerNameTextBlock = nullptr;
	HostNameTextBlock = nullptr;
	ConnectionFractionTextBlock = nullptr;
	PingTextBlock = nullptr;

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

	const FString HostString = FString::Printf(TEXT("Host: %s"), *ServerData.HostUsername);
	const FString FractionString = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
	const FString PingString = FString::Printf(TEXT("Ping: %ims"), ServerData.PingMS);

	ServerNameTextBlock->SetText(FText::FromString(ServerData.Name));
	HostNameTextBlock->SetText(FText::FromString(HostString));
	ConnectionFractionTextBlock->SetText(FText::FromString(FractionString));
	PingTextBlock->SetText(FText::FromString(PingString));
}

void UServerRowWidget::SetSelected(bool NewSelected)
{
	Selected = NewSelected;
}

void UServerRowWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Selected)
	{
		FUIColor* Blue = UUIColors::GetBaseColor(TEXT("Blue"));

		if (Hovering)
		{
			RowBorder->SetBrushColor(UUIColors::GetHighlightedColor(Blue) * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			RowBorder->SetBrushColor(Blue->Default * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}
	else
	{
		FUIColor* DarkGray = UUIColors::GetBaseColor(TEXT("DarkGray"));

		if (Hovering)
		{
			RowBorder->SetBrushColor(UUIColors::GetHighlightedColor(DarkGray) * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
		}
		else
		{
			RowBorder->SetBrushColor(DarkGray->Default * FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}
}

void UServerRowWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	Hovering = true;
}

void UServerRowWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	Hovering = false;
}

void UServerRowWidget::OnClicked()
{
	
}