// Copyright Telephone Studios. All Rights Reserved.


#include "Buttons/IndexedButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UIndexedButton::UIndexedButton(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Button = nullptr;
	ButtonText = nullptr;
	Index = INDEX_NONE;
}

void UIndexedButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button == nullptr)
	{
		return;
	}

	Button->OnClicked.AddDynamic(this, &UIndexedButton::OnClicked);
}

void UIndexedButton::Setup(const int32& InIndex, const FString& InLabel)
{
	Index = InIndex;
	if (ButtonText)
	{
		ButtonText->SetText(FText::FromString(InLabel));
	}
}

void UIndexedButton::OnClicked()
{
	if (!OnSelected.IsBound())
	{
		return;
	}

	OnSelected.Broadcast(Index);
}
