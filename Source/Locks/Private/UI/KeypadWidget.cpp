// Copyright Telephone Studios. All Rights Reserved.


#include "UI/KeypadWidget.h"
#include "Locks/Lock.h"
#include "Components/LockModifierComponent.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Log.h"

UKeypadWidget::UKeypadWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);

	CodeTextBlock = nullptr;
	OneKeyButton = nullptr;
	TwoKeyButton = nullptr;
	ThreeKeyButton = nullptr;
	FourKeyButton = nullptr;
	FiveKeyButton = nullptr;
	SixKeyButton = nullptr;
	SevenKeyButton = nullptr;
	EightKeyButton = nullptr;
	NineKeyButton = nullptr;
	ZeroKeyButton = nullptr;
	BackspaceButton = nullptr;
	LockActionButton = nullptr;
	LockActionTextBlock = nullptr;
	UnlockSizeBox = nullptr;
	UnlockButton = nullptr;
	RemoveLockSizeBox = nullptr;
	RemoveLockButton = nullptr;
	CloseButton = nullptr;

	PendingCode = TEXT("");

	LockToModify = nullptr;
	ModifyingComponent = nullptr;
}

void UKeypadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OneKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnOneClicked);
	TwoKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnTwoClicked);
	ThreeKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnThreeClicked);
	FourKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnFourClicked);
	FiveKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnFiveClicked);
	SixKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnSixClicked);
	SevenKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnSevenClicked);
	EightKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnEightClicked);
	NineKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnNineClicked);
	ZeroKeyButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnZeroClicked);
	BackspaceButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnBackspaceClicked);
	LockActionButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnLockActionClicked);
	UnlockButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnUnlockClicked);
	RemoveLockButton->OnClicked.AddDynamic(this, &UKeypadWidget::OnRemoveLockClicked);
	CloseButton->OnClicked.AddDynamic(this, &UKeypadWidget::Teardown);

	RefreshCodeTextBlock();
}

void UKeypadWidget::Setup(ALock* InLock, ULockModifierComponent* InModifyingComponent, TEnumAsByte<ELockOperation> Operation)
{
	LockToModify = InLock;
	ModifyingComponent = InModifyingComponent;
	LockOperation = Operation;

	if (LockOperation == ELockOperation::ELO_SetCode || LockOperation == ELockOperation::ELO_ModifyCode)
	{
		SetupAuthorized();
	}
	else
	{
		SetupStranger();
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(this->TakeWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);

	this->AddToViewport();
}

void UKeypadWidget::Teardown()
{
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

void UKeypadWidget::OnOneClicked()
{
	AddCharacterToCode(TEXT("1"));
}

void UKeypadWidget::OnTwoClicked()
{
	AddCharacterToCode(TEXT("2"));
}

void UKeypadWidget::OnThreeClicked()
{
	AddCharacterToCode(TEXT("3"));
}

void UKeypadWidget::OnFourClicked()
{
	AddCharacterToCode(TEXT("4"));
}

void UKeypadWidget::OnFiveClicked()
{
	AddCharacterToCode(TEXT("5"));
}

void UKeypadWidget::OnSixClicked()
{
	AddCharacterToCode(TEXT("6"));
}

void UKeypadWidget::OnSevenClicked()
{
	AddCharacterToCode(TEXT("7"));
}

void UKeypadWidget::OnEightClicked()
{
	AddCharacterToCode(TEXT("8"));
}

void UKeypadWidget::OnNineClicked()
{
	AddCharacterToCode(TEXT("9"));
}

void UKeypadWidget::OnZeroClicked()
{
	AddCharacterToCode(TEXT("0"));
}

void UKeypadWidget::OnBackspaceClicked()
{
	if (PendingCode.Len() <= 0)
	{
		return;
	}

	PendingCode = PendingCode.LeftChop(1);

	RefreshCodeTextBlock();
}

void UKeypadWidget::OnUnlockClicked()
{
	if (ModifyingComponent == nullptr)
	{
		return;
	}

	ModifyingComponent->Server_ClearLockCode(LockToModify);
	this->Teardown();
}

void UKeypadWidget::OnRemoveLockClicked()
{
	if (ModifyingComponent == nullptr)
	{
		return;
	}

	ModifyingComponent->Server_RemoveLock(LockToModify);
	this->Teardown();
}

void UKeypadWidget::SetupStranger()
{
	UnlockSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	UnlockButton->SetIsEnabled(false);

	RemoveLockSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	RemoveLockButton->SetIsEnabled(false);

	LockActionTextBlock->SetText(FText::FromString(TEXT("Authorize")));
}

void UKeypadWidget::SetupAuthorized()
{
	RemoveLockSizeBox->SetVisibility(ESlateVisibility::Visible);
	RemoveLockButton->SetIsEnabled(true);

	FString ActionString(TEXT(""));
	if (LockOperation == ELockOperation::ELO_SetCode)
	{
		UnlockSizeBox->SetVisibility(ESlateVisibility::Collapsed);
		UnlockButton->SetIsEnabled(false);
		ActionString = TEXT("Set Code");
	}
	else if (LockOperation == ELockOperation::ELO_ModifyCode)
	{
		UnlockSizeBox->SetVisibility(ESlateVisibility::Visible);
		UnlockButton->SetIsEnabled(true);
		ActionString = TEXT("Change Code");
	}

	LockActionTextBlock->SetText(FText::FromString(ActionString));
}

void UKeypadWidget::OnLockActionClicked()
{
	if (PendingCode.Len() < 4)
	{
		UE_LOG(LogLocks, Display, TEXT("Code was %i characters, thats too short."), PendingCode.Len());
		return;
	}

	switch (LockOperation)
	{
	case ELockOperation::ELO_Authorize:
		ModifyingComponent->Server_AuthorizeLock(LockToModify, PendingCode);
		break;
	case ELockOperation::ELO_ModifyCode:
		ModifyingComponent->Server_SetLockCode(LockToModify, PendingCode);
		break;
	case ELockOperation::ELO_SetCode:
		ModifyingComponent->Server_SetLockCode(LockToModify, PendingCode);
		break;
	}

	this->Teardown();
}

void UKeypadWidget::AddCharacterToCode(const FString& CharacterToAdd)
{
	if (PendingCode.Len() >= 4)
	{
		return;
	}

	PendingCode.Append(CharacterToAdd);

	RefreshCodeTextBlock();
}

void UKeypadWidget::RefreshCodeTextBlock()
{
	FString DisplayCode = TEXT("");
	for (int32 i = 0; i < PendingCode.Len(); i++)
	{
		DisplayCode.Append(TEXT("*"));
	}

	CodeTextBlock->SetText(FText::FromString(DisplayCode));
}
