// Copyright Telephone Studios. All Rights Reserved.


#include "GameplayMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "WildOmission/UI/Menu/OptionsWidget.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"

UGameplayMenuWidget::UGameplayMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);
	bOpen = false;
}

void UGameplayMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button delegates
	ResumeButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::Teardown);
	OptionsButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::OpenOptionsMenu);
	QuitButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::QuitToMenu);

	OptionsMenu->SetParent(this);
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

bool UGameplayMenuWidget::IsOpen() const
{
	return bOpen;
}

void UGameplayMenuWidget::Teardown()
{
	bOpen = false;
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

void UGameplayMenuWidget::Save()
{
	AWildOmissionPlayerController* PlayerController = Cast<AWildOmissionPlayerController>(GetOwningPlayer());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't call save, PlayerController was nullptr."));
		return;
	}

	PlayerController->Save();
}

void UGameplayMenuWidget::QuitToMenu()
{
	UWildOmissionGameInstance* GameInstance = Cast<UWildOmissionGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		return;
	}

	GameInstance->QuitToMenu();
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