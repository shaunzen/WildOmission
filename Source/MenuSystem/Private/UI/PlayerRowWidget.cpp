// Copyright Telephone Studios. All Rights Reserved.


#include "UI/PlayerRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerRowWidget::UPlayerRowWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	NameTextBlock = nullptr;
	ViewProfileButton = nullptr;
	KickButton = nullptr;
	UniqueID = TEXT("");
}

void UPlayerRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ViewProfileButton)
	{
		ViewProfileButton->OnClicked.AddDynamic(this, &UPlayerRowWidget::ViewProfile);
	}

	if (KickButton)
	{
		KickButton->OnClicked.AddDynamic(this, &UPlayerRowWidget::Kick);
	}
}

void UPlayerRowWidget::Setup(const FString& Name, const FString& InUniqueID)
{
	NameTextBlock->SetText(FText::FromString(Name));
	UniqueID = InUniqueID;
}

void UPlayerRowWidget::ViewProfile()
{
	const FString ProfileURL = FString::Printf(TEXT("https://steamcommunity.com/id/%s/"), *UniqueID);
	UKismetSystemLibrary::LaunchURL(ProfileURL);
}

void UPlayerRowWidget::Kick()
{
	UE_LOG(LogTemp, Warning, TEXT("Kicking player...\nKick Functionality isnt implemented yet."));
	// TODO figure this out later
}
