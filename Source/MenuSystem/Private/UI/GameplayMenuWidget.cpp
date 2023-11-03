// Copyright Telephone Studios. All Rights Reserved.


#include "UI/GameplayMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/OptionsWidget.h"
#include "ConnectedPlayersWidget.h"
#include "Interfaces/SavablePlayer.h"
#include "Kismet/KismetSystemLibrary.h"

UGameplayMenuWidget::UGameplayMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);
	ResumeButton = nullptr;
	OptionsButton = nullptr;
	HowToPlayButton = nullptr;
	FeedbackButton = nullptr;
	QuitButton = nullptr;
	QuitButtonText = nullptr;
	MenuSwitcher = nullptr;
	GameMenu = nullptr;
	OptionsMenu = nullptr;
	ConnectedPlayersMenu = nullptr;
	MenuInterface = nullptr;
	bOpen = false;
}

void UGameplayMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button delegates
	ResumeButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::Teardown);
	OptionsButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::OpenOptionsMenu);
	FeedbackButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::OpenFeedbackPage);
	HowToPlayButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::OpenHelpGuide);
	QuitButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::QuitToMenu);

	OptionsMenu->OnBackButtonClicked.AddDynamic(this, &UGameplayMenuWidget::OpenGameMenu);
	ConnectedPlayersMenu->OnBackButtonClicked.AddDynamic(this, &UGameplayMenuWidget::OpenGameMenu);
}

FReply UGameplayMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		this->Teardown();
	}

	return FReply::Handled();
}

void UGameplayMenuWidget::Show()
{
	bOpen = true;
	AddToViewport();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	
	SetQuitButtonText(PlayerController->HasAuthority());
	
	OptionsMenu->Refresh();

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	PlayerController->SetMouseLocation(ViewportSizeX / 2, ViewportSizeY / 2);

	Save();
}

void UGameplayMenuWidget::SetMenuInterface(IMenuInterface* InMenuInterface)
{
	MenuInterface = InMenuInterface;
}

void UGameplayMenuWidget::OpenGameMenu()
{
	if (MenuSwitcher == nullptr || GameMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(GameMenu);
}

void UGameplayMenuWidget::OpenOptionsMenu()
{
	if (MenuSwitcher == nullptr || OptionsMenu == nullptr)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(OptionsMenu);
}

void UGameplayMenuWidget::OpenConnectedPlayersMenu()
{
	// TODO
}

bool UGameplayMenuWidget::IsOpen() const
{
	return bOpen;
}

IMenuInterface* UGameplayMenuWidget::GetMenuInterface() const
{
	return MenuInterface;
}

void UGameplayMenuWidget::Teardown()
{
	bOpen = false;
	
	if (OnClosed.IsBound())
	{
		OnClosed.Broadcast();
	}

	RemoveFromParent();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UGameplayMenuWidget::OpenHelpGuide()
{
	UKismetSystemLibrary::LaunchURL(TEXT("https://steamcommunity.com/sharedfiles/filedetails/?id=3029189957"));
}

void UGameplayMenuWidget::OpenFeedbackPage()
{
	UKismetSystemLibrary::LaunchURL(TEXT("https://forms.gle/2GP8ZSTU5ARa5Pmu8"));
}

void UGameplayMenuWidget::Save()
{
	ISavablePlayer* SavablePlayer = Cast<ISavablePlayer>(GetOwningPlayer());
	if (SavablePlayer == nullptr)
	{
		UE_LOG(LogPlayerController, Warning, TEXT("Couldn't call save, SavablePlayer was nullptr."));
		return;
	}

	SavablePlayer->Save();
}

void UGameplayMenuWidget::QuitToMenu()
{
	if (MenuInterface == nullptr)
	{
		return;
	}

	MenuInterface->QuitToMenu();
}

void UGameplayMenuWidget::SetQuitButtonText(bool PlayerHasAuthority)
{
	FString ButtonText;
	
	if (PlayerHasAuthority)
	{
		ButtonText = FString("Save and quit");
	}
	else
	{
		ButtonText = FString("Leave server");
	}

	QuitButtonText->SetText(FText::FromString(ButtonText));
}