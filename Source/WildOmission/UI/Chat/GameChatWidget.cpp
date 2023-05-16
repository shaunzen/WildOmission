// Copyright Telephone Studios. All Rights Reserved.


#include "GameChatWidget.h"
#include "ChatMessageWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "WildOmission/Core/WildOmissionGameState.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"

UGameChatWidget::UGameChatWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UChatMessageWidget> ChatMessageBlueprint(TEXT("/Game/WildOmission/UI/Chat/WBP_ChatMessage"));
	if (ChatMessageBlueprint.Succeeded())
	{
		ChatMessageClass = ChatMessageBlueprint.Class;
	}

	Opened = false;
}

void UGameChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Close();
	SendMessageButton->OnClicked.AddDynamic(this, &UGameChatWidget::SendMessage);
	AWildOmissionGameState* GameState = Cast<AWildOmissionGameState>(GetWorld()->GetGameState());
	if (GameState == nullptr)
	{
		return;
	}
	GameState->OnNewMessage.AddDynamic(this, &UGameChatWidget::RefreshMessages);

}

void UGameChatWidget::RefreshMessages()
{
	MessageContainer->ClearChildren();

	AWildOmissionGameState* GameState = Cast<AWildOmissionGameState>(GetWorld()->GetGameState());
	if (GameState == nullptr)
	{
		return;
	}

	TArray<FChatMessage> Messages;
	GameState->GetChatMessages(Messages);

}

void UGameChatWidget::Open()
{
	Opened = true;
	MessagePanel->SetVisibility(ESlateVisibility::Visible);
}

void UGameChatWidget::Close()
{
	Opened = false;
	MessagePanel->SetVisibility(ESlateVisibility::Collapsed);
}

bool UGameChatWidget::IsOpen() const
{
	return Opened;
}

void UGameChatWidget::SendMessage()
{
	// Return if no message was typed
	if (MessageBox->GetText().ToString().Len() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot send nothing in game chat."));
		return;
	}

	AWildOmissionGameState* GameState = Cast<AWildOmissionGameState>(GetWorld()->GetGameState());
	if (GameState == nullptr || GetOwningPlayerState() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to send chat message, couldn't get state."));
		return;
	}

	GameState->SendChatMessage(GetOwningPlayerState(), MessageBox->GetText().ToString());
}