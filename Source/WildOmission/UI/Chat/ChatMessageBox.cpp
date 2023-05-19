// Copyright Telephone Studios. All Rights Reserved.


#include "ChatMessageBox.h"
#include "GameChatWidget.h"

void UChatMessageBox::SetOwner(UGameChatWidget* InOwner)
{
	OwnerChatWidget = InOwner;
}

void UChatMessageBox::HandleOnTextCommitted(const FText& CommittedText, ETextCommit::Type CommitMethod)
{
	Super::HandleOnTextCommitted(CommittedText, CommitMethod);
	
	if (OwnerChatWidget == nullptr)
	{
		return;
	}

	OwnerChatWidget->SendMessage();
}