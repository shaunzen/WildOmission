// Copyright Telephone Studios. All Rights Reserved.


#include "UI/ToolCupboardWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Deployables/ToolCupboard.h"

UToolCupboardWidget::UToolCupboardWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);

	AuthorizeButton = nullptr;
	AuthorizeTextBlock = nullptr;
	ClearAuthorizedButton = nullptr;
	CloseButton = nullptr;
}

void UToolCupboardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AuthorizeButton->OnClicked.AddDynamic(this, &UToolCupboardWidget::OnAuthorizeButtonClicked);
	ClearAuthorizedButton->OnClicked.AddDynamic(this, &UToolCupboardWidget::OnClearAuthorizedButtonClicked);
	CloseButton->OnClicked.AddDynamic(this, &UToolCupboardWidget::Teardown);
}

void UToolCupboardWidget::Setup(AToolCupboard* InToolCupboard)
{
	ToolCupboard = InToolCupboard;

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	this->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(this->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);
}

void UToolCupboardWidget::Teardown()
{
	ToolCupboard = nullptr;

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeGameOnly InputMode;
	
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(false);

	if (OnTeardown.IsBound())
	{
		OnTeardown.Broadcast();
	}

	this->RemoveFromParent();
}

void UToolCupboardWidget::OnAuthorizeButtonClicked()
{
	this->Teardown();
}

void UToolCupboardWidget::OnClearAuthorizedButtonClicked()
{
	this->Teardown();
}
