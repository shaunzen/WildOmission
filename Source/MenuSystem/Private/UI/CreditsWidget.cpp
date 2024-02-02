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

	const FDateTime LarchBirthday(2006, 1, 13);
	const FDateTime TimeNow = FDateTime::Now();
	const FDateTime LarchAge(TimeNow.GetTicks() - LarchBirthday.GetTicks());

	const FString AboutString = FString::Printf(
		TEXT("Wild Omission is an open-world multiplayer sandbox survival game with severe weather. Developed by %i year old indie game developer Larch. I (Larch) started working on Wild Omission in December of 2022 (when I was 16), It has been an incredible journey. I never could have imagined where it would take me, but I'm glad I have stuck to it and continued working on it. I am eager to hear your thoughts about the game! If you have any questions or comments I can be reached at telephonestudioscontact@gmail.com."),
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