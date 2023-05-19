// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "ChatMessageBox.generated.h"

class UGameChatWidget;

UCLASS()
class WILDOMISSION_API UChatMessageBox : public UEditableTextBox
{
	GENERATED_BODY()

public:
	void SetOwner(UGameChatWidget* InOwner);
protected:
	virtual void HandleOnTextCommitted(const FText& CommittedText, ETextCommit::Type CommitMethod) override;

private:
	UPROPERTY()
	UGameChatWidget* OwnerChatWidget;
	
};
