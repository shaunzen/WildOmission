// Copyright Telephone Studios. All Rights Reserved.


#include "ErrorMessagePrompt.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Log.h"

UErrorMessagePrompt::UErrorMessagePrompt(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	TitleTextBlock = nullptr;
	MessageTextBlock = nullptr;
	CloseButton = nullptr;
}

void UErrorMessagePrompt::SetMessage(const FString& MessageTitle, const FString& Message)
{
	if (TitleTextBlock == nullptr || MessageTextBlock == nullptr)
	{
		UE_LOG(LogMenuSystem, Warning, TEXT("ErrorMessagePrompt, TitleTextBlock or MessageTextBlock nullptr."))
		return;
	}

	TitleTextBlock->SetText(FText::FromString(MessageTitle));
	MessageTextBlock->SetText(FText::FromString(Message));
}

void UErrorMessagePrompt::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (CloseButton == nullptr)
	{
		return;
	}

	CloseButton->OnClicked.AddDynamic(this, &UErrorMessagePrompt::BroadcastCloseButtonClicked);
}

void UErrorMessagePrompt::BroadcastCloseButtonClicked()
{
	if (!OnCloseButtonClicked.IsBound())
	{
		UE_LOG(LogMenuSystem, Warning, TEXT("ErrorMessagePrompt, OnCloseButtonClicked is not bound."));
		return;
	}

	OnCloseButtonClicked.Broadcast();
}
