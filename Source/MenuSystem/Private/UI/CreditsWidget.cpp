// Copyright Telephone Studios. All Rights Reserved.


#include "UI/CreditsWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UCreditsWidget::UCreditsWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	AboutTextBlock = nullptr;
	BackButton = nullptr;
}

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FDateTime ZeNNBirthday(1994, 4, 18);
	const FDateTime TimeNow = FDateTime::Now();
	const FDateTime ZeNNAge(TimeNow.GetTicks() - ZeNNBirthday.GetTicks());

	const FString AboutString = FString::Printf(
		TEXT("Hi. My name is ZeNN. The game you are playing is called wild omission on steam. The game did go open source with the MIT licence so I downloaded the source code and decided to keep the game updated. If you have any questions please reach out to me."),
		LarchAge.GetYear() - 1
	);

	if (AboutTextBlock)
	{
		AboutTextBlock->SetText(FText::FromString(AboutString));
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UCreditsWidget::BroadcastOnBackButtonClicked);
	}
}

void UCreditsWidget::BroadcastOnBackButtonClicked()
{
	if (!OnBackButtonClicked.IsBound())
	{
		return;
	}

	OnBackButtonClicked.Broadcast();
}