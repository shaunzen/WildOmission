// Copyright Telephone Studios. All Rights Reserved.


#include "GameplayMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WildOmission/Core/WildOmissionGameInstance.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"

UGameplayMenuWidget::UGameplayMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsFocusable = true;
	bOpen = false;
}

bool UGameplayMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	
	if (!Success || ResumeButton == nullptr || QuitButton == nullptr)
	{
		return false;
	}

	// Bind button delegates
	ResumeButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::Teardown);
	QuitButton->OnClicked.AddDynamic(this, &UGameplayMenuWidget::QuitToMenu);

	return true;
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
	
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

	Save();
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

	GameInstance->ReturnToMainMenu();
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